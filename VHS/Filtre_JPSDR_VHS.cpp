#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "VHS_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Pascal.h"

extern int g_VFVAPIVersion;

extern "C" void JPSDR_VHS_Move32_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);


class JPSDR_VHSData
{
public:
	uint16_t threshold1,threshold2;
	uint8_t filter_mode;

	JPSDR_VHSData(void);
};

JPSDR_VHSData::JPSDR_VHSData(void)
{
	filter_mode=1;
	threshold1=12;
	threshold2=16;
}



class JPSDR_VHSDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_VHSDialog(JPSDR_VHSData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_VHS),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_VHSData& mData;
	JPSDR_VHSData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_VHSDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG : return !OnInit();
		case WM_DESTROY :
			OnDestroy();
			break;
		case WM_COMMAND :
			if (OnCommand(LOWORD(wParam))) return TRUE;
			break;
	}
	return FALSE;
}


bool JPSDR_VHSDialog::OnInit()
{
	moldData=mData;

	switch (mData.filter_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_MODE_1,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_MODE_2,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_MODE_3,1); break;
	}
	SetDlgItemInt(mhdlg,IDC_THRESHOLD_1,mData.threshold1,FALSE);
	SetDlgItemInt(mhdlg,IDC_THRESHOLD_2,mData.threshold2,FALSE);

	return false;
}


void JPSDR_VHSDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}



bool JPSDR_VHSDialog::SaveToData()
{
	uint16_t thrs;
	BOOL success;
	
	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD_1,&success,FALSE);
	if (success) mData.threshold1=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD_2,&success,FALSE);
	if (success) mData.threshold2=thrs;
	if (!!IsDlgButtonChecked(mhdlg, IDC_MODE_1)) mData.filter_mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_MODE_2)) mData.filter_mode=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_MODE_3)) mData.filter_mode=3;

	return true;
}



bool JPSDR_VHSDialog::OnCommand(int cmd)
{
	switch(cmd)
	{
		case IDOK :
			SaveToData();
			EndDialog(mhdlg,true);
			return true;
		case IDCANCEL :
			mData=moldData;
			EndDialog(mhdlg,false);
			return true;
	}
		
	return false;
}



class JPSDR_VHS : public JPSDRVDXVideoFilter
{
public:
	virtual bool Init();
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual void End();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	uint8_t *_x[3][3];
	uint8_t *_y[3];
	uint8_t *line_out[3];
	uint8_t lookup[2806];
	uint8_t indice_0,indice_1,indice_2;
	bool VDub2_Enable;

	void InternalInit(void);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_VHSData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_VHS)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_VHS,ScriptConfig,"iii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_VHS::Init()
{
	for (int16_t i=0; i<3; i++)
	{
		for (int16_t j=0; j<3; j++)
			_x[i][j]=NULL;
	}
	for (int16_t i=0; i<3; i++)
		_y[i]=NULL;
	for (int16_t i=0; i<3; i++)
		line_out[i]=NULL;

	return(true);
}


uint32 JPSDR_VHS::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;
	
	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;
	
	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));

	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :*/
		case nsVDXPixmap::kPixFormat_XRGB8888 :
/*		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar16 :
		case nsVDXPixmap::kPixFormat_Y16 :
		case nsVDXPixmap::kPixFormat_YUV444_Y416 :
		case nsVDXPixmap::kPixFormat_YUV444_V410 :
		case nsVDXPixmap::kPixFormat_YUV444_Y410 :
		case nsVDXPixmap::kPixFormat_R210 :
		case nsVDXPixmap::kPixFormat_R10K :
		case nsVDXPixmap::kPixFormat_YUV422_P210 :
		case nsVDXPixmap::kPixFormat_YUV420_P010 :
		case nsVDXPixmap::kPixFormat_YUV422_P216 :
		case nsVDXPixmap::kPixFormat_YUV420_P016 :
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_YU64 :*/			
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	image_data.src_full_mode=CheckFullRangeMode(&fa->src);
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	fa->dst.offset = fa->src.offset;
	return (FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
}



bool JPSDR_VHS::Configure(VDXHWND hwnd)
{
	JPSDR_VHSDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_VHS::GetSettingString(char *buf, int maxlen)
{
	SafePrintf(buf,maxlen," Thresholds : (%d,%d) Mode : %d",mData.threshold1,mData.threshold2,
		mData.filter_mode);
}



void JPSDR_VHS::Run()
{
	int32_t w,h;
	const RGB32BMP *src;
	RGB32BMP *dst;
	uint8_t *xr[3],*xg[3],*xb[3],*yr,*yg,*yb,*p0,*p1,*p2;
	uint8_t *r,*g,*b;
	uint16_t threshold1,threshold2;
	uint16_t xr0,xr1,xr2,y;
	uint16_t xg0,xg1,xg2;
	uint16_t xb0,xb1,xb2;
	int16_t dr1,dr2;
	int16_t dg1,dg2;
	int16_t db1,db2;
	uint32_t indice;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;
	const VDXFBitmap& pxsrc2 = fa->src;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	r=line_out[0];
	g=line_out[1];
	b=line_out[2];
	h=image_data.src_h0;
	w=image_data.src_w0;
	src=(RGB32BMP *)image_data.src_plane0;
	dst=(RGB32BMP *)image_data.dst_plane0;
	for (int32_t i=0; i<3; i++)
	{
		xr[i]=_x[0][i];
		xg[i]=_x[1][i];
		xb[i]=_x[2][i];
	}
	yr=_y[0];
	yg=_y[1];
	yb=_y[2];

	if (pxsrc2.mFrameNumber==0)
	{
		JPSDR_VHS_Move32_Full(src,dst,w,h,image_data.src_modulo0,image_data.dst_modulo0);
		indice=0;
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xb[0][indice]=src->b;
				xg[0][indice]=src->g;
				xr[0][indice]=src->r;
				src++;
				indice++;
			}
			src=(RGB32BMP *)((uint8_t *)src+image_data.src_modulo0);
		}
		return;
	}

	if (pxsrc2.mFrameNumber==1)
	{
		JPSDR_VHS_Move32_Full(src,dst,w,h,image_data.src_modulo0,image_data.dst_modulo0);
		indice=0;
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xb[1][indice]=src->b;
				xg[1][indice]=src->g;
				xr[1][indice]=src->r;
				src++;
				yr[indice]=(uint8_t)((((uint16_t)xr[1][indice])+((uint16_t)xr[0][indice])) >> 1);
				yg[indice]=(uint8_t)((((uint16_t)xg[1][indice])+((uint16_t)xg[0][indice])) >> 1);
				yb[indice]=(uint8_t)((((uint16_t)xb[1][indice])+((uint16_t)xb[0][indice])) >> 1);
				indice++;
			}
			src=(RGB32BMP *)((uint8_t *)src+image_data.src_modulo0);
		}
		return;
	}

	threshold1=mData.threshold1;
	threshold2=mData.threshold2;
	indice=0;
	p0=xr[indice_0];
	p1=xg[indice_0];
	p2=xb[indice_0];
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			p2[indice]=src->b;
			p1[indice]=src->g;
			p0[indice]=src->r;
			src++;
			indice++;
		}
		src=(RGB32BMP *)((uint8_t *)src+image_data.src_modulo0);
	}

	indice=0;
	switch (mData.filter_mode)
	{
	case 1 :
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xr0=xr[indice_0][indice];
				xr1=xr[indice_1][indice];
				xr2=xr[indice_2][indice];
				xg0=xg[indice_0][indice];
				xg1=xg[indice_1][indice];
				xg2=xg[indice_2][indice];
				xb0=xb[indice_0][indice];
				xb1=xb[indice_1][indice];
				xb2=xb[indice_2][indice];
				dr1=xr0-xr1;
				dr2=xr0-xr2;
				dg1=xg0-xg1;
				dg2=xg0-xg2;
				db1=xb0-xb1;
				db2=xb0-xb2;
				if ( (abs(dr1)<=threshold1) && (abs(dr2)<=threshold1) && (abs(dg1)<=threshold1)
					&& (abs(dg2)<=threshold1) && (abs(db1)<=threshold1) && (abs(db2)<=threshold1)
					&& (abs(dr1+dr2)<=threshold2) && (abs(dg1+dg2)<=threshold2)
					&& (abs(db1+db2)<=threshold2) )
				{
					y=yr[indice];
					y=lookup[xr0+xr1+xr2+y];
					r[j]=(uint8_t)y;
					yr[indice]=(uint8_t)y;
					y=yg[indice];
					y=lookup[xg0+xg1+xg2+y];
					g[j]=(uint8_t)y;
					yg[indice]=(uint8_t)y;
					y=yb[indice];
					y=lookup[xb0+xb1+xb2+y];
					b[j]=(uint8_t)y;
					yb[indice]=(uint8_t)y;
				}
				else
				{
					r[j]=(uint8_t)xr0;
					yr[indice]=(uint8_t)((xr0+xr1) >> 1);
					g[j]=(uint8_t)xg0;
					yg[indice]=(uint8_t)((xg0+xg1) >> 1);
					b[j]=(uint8_t)xb0;
					yb[indice]=(uint8_t)((xb0+xb1) >> 1);
				}
				indice++;
			}
			for (int32_t j=0; j<w; j++)
			{
				dst->b=b[j];
				dst->g=g[j];
				dst->r=r[j];
				dst++;
			}
			dst=(RGB32BMP *)((uint8_t *)dst+image_data.dst_modulo0);
		}
		break;
	case 2 :
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xr0=xr[indice_0][indice];
				xr1=xr[indice_1][indice];
				xr2=xr[indice_2][indice];
				xg0=xg[indice_0][indice];
				xg1=xg[indice_1][indice];
				xg2=xg[indice_2][indice];
				xb0=xb[indice_0][indice];
				xb1=xb[indice_1][indice];
				xb2=xb[indice_2][indice];
				dr1=xr0-xr1;
				dr2=xr0-xr2;
				dg1=xg0-xg1;
				dg2=xg0-xg2;
				db1=xb0-xb1;
				db2=xb0-xb2;
				if ( (abs(dr1)<=threshold1) && (abs(dr2)<=threshold1) && (abs(dg1)<=threshold1)
					&& (abs(dg2)<=threshold1) && (abs(db1)<=threshold1) && (abs(db2)<=threshold1)
					&& (abs(dr1+dr2)<=threshold2) && (abs(dg1+dg2)<=threshold2)
					&& (abs(db1+db2)<=threshold2) )
				{
					y=yr[indice];
					y=lookup[xr0+xr1+xr2+(y << 2)];
					r[j]=(uint8_t)y;
					yr[indice]=(uint8_t)y;
					y=yg[indice];
					y=lookup[xg0+xg1+xg2+(y << 2)];
					g[j]=(uint8_t)y;
					yg[indice]=(uint8_t)y;
					y=yb[indice];
					y=lookup[xb0+xb1+xb2+(y << 2)];
					b[j]=(uint8_t)y;
					yb[indice]=(uint8_t)y;
				}
				else
				{
					r[j]=(uint8_t)xr0;
					yr[indice]=(uint8_t)((xr0+xr1) >> 1);
					g[j]=(uint8_t)xg0;
					yg[indice]=(uint8_t)((xg0+xg1) >> 1);
					b[j]=(uint8_t)xb0;
					yb[indice]=(uint8_t)((xb0+xb1) >> 1);
				}
				indice++;
			}
			for (int32_t j=0; j<w; j++)
			{
				dst->b=b[j];
				dst->g=g[j];
				dst->r=r[j];
				dst++;
			}
			dst=(RGB32BMP *)((uint8_t *)dst+image_data.dst_modulo0);
		}
		break;
	case 3 :
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xr0=xr[indice_0][indice];
				xr1=xr[indice_1][indice];
				xr2=xr[indice_2][indice];
				xg0=xg[indice_0][indice];
				xg1=xg[indice_1][indice];
				xg2=xg[indice_2][indice];
				xb0=xb[indice_0][indice];
				xb1=xb[indice_1][indice];
				xb2=xb[indice_2][indice];
				dr1=xr0-xr1;
				dr2=xr0-xr2;
				dg1=xg0-xg1;
				dg2=xg0-xg2;
				db1=xb0-xb1;
				db2=xb0-xb2;
				if ( (abs(dr1)<=threshold1) && (abs(dr2)<=threshold1) && (abs(dg1)<=threshold1)
					&& (abs(dg2)<=threshold1) && (abs(db1)<=threshold1) && (abs(db2)<=threshold1)
					&& (abs(dr1+dr2)<=threshold2) && (abs(dg1+dg2)<=threshold2)
					&& (abs(db1+db2)<=threshold2) )
				{
					y=yr[indice];
					y=lookup[xr0+xr1+xr2+(y << 3)];
					r[j]=(uint8_t)y;
					yr[indice]=(uint8_t)y;
					y=yg[indice];
					y=lookup[xg0+xg1+xg2+(y << 3)];
					g[j]=(uint8_t)y;
					yg[indice]=(uint8_t)y;
					y=yb[indice];
					y=lookup[xb0+xb1+xb2+(y << 3)];
					b[j]=(uint8_t)y;
					yb[indice]=(uint8_t)y;
				}
				else
				{
					r[j]=(uint8_t)xr0;
					yr[indice]=(uint8_t)((xr0+xr1) >> 1);
					g[j]=(uint8_t)xg0;
					yg[indice]=(uint8_t)((xg0+xg1) >> 1);
					b[j]=(uint8_t)xb0;
					yb[indice]=(uint8_t)((xb0+xb1) >> 1);
				}
				indice++;
			}
			for (int32_t j=0; j<w; j++)
			{
				dst->b=b[j];
				dst->g=g[j];
				dst->r=r[j];
				dst++;
			}
			dst=(RGB32BMP *)((uint8_t *)dst+image_data.dst_modulo0);
		}
		break;
	}

	indice_0=(indice_0+1)%3;
	indice_1=(indice_1+1)%3;
	indice_2=(indice_2+1)%3;
}




void JPSDR_VHS::Start()
{
	bool ok;
	int16_t i,j;
	uint32_t size;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}
	
	SetImageData(image_data);

	size=(image_data.src_h0*image_data.src_w0)*sizeof(uint8_t);
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			_x[i][j]=(uint8_t *)malloc(size);
	for (i=0; i<3; i++)
		_y[i]=(uint8_t *)malloc(size);
	for (i=0; i<3; i++)
		line_out[i]=(uint8_t *)malloc(image_data.src_w0*sizeof(uint8_t));
	ok=true;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			ok=ok && (_x[i][j]!=NULL);
	for (i=0; i<3; i++)
		ok=ok && (_y[i]!=NULL);
	for (i=0; i<3; i++)
		ok=ok && (line_out[i]!=NULL);
	if (!ok)
	{
		for (i=2; i>=0; i--)
			myfree(line_out[i]);
		for (i=2; i>=0; i--)
			myfree(_y[i]);
		for (i=2; i>=0; i--)
		{
			for (j=2; j>=0; j--)
				myfree(_x[i][j]);
		}
		ff->ExceptOutOfMemory();
		return;
	}
	switch (mData.filter_mode)
	{
		case 1 : j=1020; break;
		case 2 : j=1785; break;
		case 3 : j=2805; break;
	}
	for (i=0; i<=j; i++)
		switch (mData.filter_mode)
		{
			case 1 : lookup[i]=(uint8_t)round((float)i/4.0); break;
			case 2 : lookup[i]=(uint8_t)round((float)i/7.0); break;
			case 3 : lookup[i]=(uint8_t)round((float)i/11.0); break;
		}
	for (i=j+1; i<=2805; i++)
		lookup[i]=255;
	indice_0=2;
	indice_1=1;
	indice_2=0;
}




void JPSDR_VHS::End()
{
	int16_t i,j;

	for (i=2; i>=0; i--)
		myfree(line_out[i]);
	for (i=2; i>=0; i--)
		myfree(_y[i]);
	for (i=2; i>=0; i--)
	{
		for (j=2; j>=0; j--)
			myfree(_x[i][j]);
	}
}



void JPSDR_VHS::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
    mData.threshold1=argv[0].asInt();
    mData.threshold2=argv[1].asInt();
	mData.filter_mode=argv[2].asInt();
}


void JPSDR_VHS::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf, maxlen,"Config(%d, %d, %d)",mData.threshold1,mData.threshold2,mData.filter_mode);
}


extern VDXFilterDefinition2 filterDef_JPSDR_VHS=
VDXVideoFilterDefinition<JPSDR_VHS>("JPSDR","VHS I v2.4.1","Filter to remove VHS noise.");

