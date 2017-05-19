#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "VHS_II_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

extern int g_VFVAPIVersion;


class JPSDR_VHS_IIData
{
public:
	uint16_t threshold1,threshold2;
	uint8_t filter_mode;

	JPSDR_VHS_IIData(void);
};


JPSDR_VHS_IIData::JPSDR_VHS_IIData(void)
{
	filter_mode=1;
	threshold1=12;
	threshold2=16;
}



class JPSDR_VHS_IIDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_VHS_IIDialog(JPSDR_VHS_IIData& Data, IVDXFilterPreview *ifp) : mData(Data), mifp(ifp)
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
	
	JPSDR_VHS_IIData& mData;
	JPSDR_VHS_IIData moldData;
	IVDXFilterPreview *const mifp;
};


INT_PTR JPSDR_VHS_IIDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_VHS_IIDialog::OnInit()
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


void JPSDR_VHS_IIDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}



bool JPSDR_VHS_IIDialog::SaveToData()
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



bool JPSDR_VHS_IIDialog::OnCommand(int cmd)
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



class JPSDR_VHS_II : public VDXVideoFilter
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
	uint8_t *_x[3][5];
	uint8_t *_y[3];
	uint8_t *line_out[3];
	uint8_t lookup[2806];
	uint32_t size;
	uint8_t indice_4,indice_3,indice_2,indice_1,indice_0;

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_VHS_IIData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_VHS_II)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_VHS_II,ScriptConfig,"iii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_VHS_II::Init()
{
	int16_t i,j;

	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
			_x[i][j]=NULL;
	}
	for (i=0; i<3; i++)
		_y[i]=NULL;
	for (i=0; i<3; i++)
		line_out[i]=NULL;

	return(true);
}

uint32 JPSDR_VHS_II::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;
	
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
		case nsVDXPixmap::kPixFormat_VDXA_YUV :			*/				
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_NONE;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_Y8 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_NONE;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :				
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;

	switch(pxdst.format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_NONE;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_Y8 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_NONE;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :				
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
	}

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_HAS_LAG(2)|FILTERPARAM_ALIGN_SCANLINES);
}



bool JPSDR_VHS_II::Configure(VDXHWND hwnd)
{
	JPSDR_VHS_IIDialog dlg(mData, fa->ifp);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_VHS_II::GetSettingString(char *buf, int maxlen)
{
	SafePrintf(buf,maxlen," Thresholds : (%d,%d) Mode : %d",mData.threshold1,mData.threshold2,
		mData.filter_mode);
}





void JPSDR_VHS_II::Run()
{
	Image_Data idata;
	int32_t w,h;
	const RGB32BMP *src;
	RGB32BMP *dst;
	uint8_t *xr[5],*xg[5],*xb[5],*yr,*yg,*yb,*p0,*p1,*p2;
	uint8_t *r,*g,*b;
	uint16_t threshold1,threshold2;
	uint16_t xr0,xr1,xr2,xr3,xr4,y;
	uint16_t xg0,xg1,xg2,xg3,xg4;
	uint16_t xb0,xb1,xb2,xb3,xb4;
	int16_t dr1,dr2,dr3,dr4;
	int16_t dg1,dg2,dg3,dg4;
	int16_t db1,db2,db3,db4;
	uint32_t indice;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;

	const VDXPixmap& pxdst=*fa->dst.mpPixmap;
	const VDXPixmap& pxsrc=*fa->src.mpPixmap;
	const VDXFBitmap& pxsrc2 = fa->src;

	idata.src_plane0=pxsrc.data;
	idata.src_plane1=pxsrc.data2;
	idata.src_plane2=pxsrc.data3;
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	r=line_out[0];
	g=line_out[1];
	b=line_out[2];
	h=idata.src_h0;
	w=idata.src_w0;
	src=(RGB32BMP *)idata.src_plane0;
	dst=(RGB32BMP *)idata.dst_plane0;
	threshold1=mData.threshold1;
	threshold2=mData.threshold2;
	for (int32_t i=0; i<5; i++)
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
			src=(RGB32BMP *)((uint8_t *)src+idata.src_modulo0);
		}
		return;
	}

	if (pxsrc2.mFrameNumber==1)
	{
		indice=0;
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xb[1][indice]=src->b;
				xg[1][indice]=src->g;
				xr[1][indice]=src->r;
				src++;
				indice++;
			}
			src=(RGB32BMP *)((uint8_t *)src+idata.src_modulo0);
		}
		return;
	}

	if (pxsrc2.mFrameNumber==2)
	{
		indice=0;
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xb[2][indice]=src->b;
				xg[2][indice]=src->g;
				xr[2][indice]=src->r;
				src++;
				yr[indice]=(uint8_t)((((uint16_t)xr[1][indice])+((uint16_t)xr[0][indice])) >> 1);
				yg[indice]=(uint8_t)((((uint16_t)xg[1][indice])+((uint16_t)xg[0][indice])) >> 1);
				yb[indice]=(uint8_t)((((uint16_t)xb[1][indice])+((uint16_t)xb[0][indice])) >> 1);
				indice++;
			}
			src=(RGB32BMP *)((uint8_t *)src+idata.src_modulo0);
		}
		switch (mData.filter_mode)
		{
		case 1 :
			indice=0;
			for (int32_t i=0; i<h; i++)
			{
				for (int32_t j=0; j<w; j++)
				{
					xr0=xr[2][indice];
					xr1=xr[1][indice];
					xr2=xr[0][indice];
					xg0=xg[2][indice];
					xg1=xg[1][indice];
					xg2=xg[0][indice];
					xb0=xb[2][indice];
					xb1=xb[1][indice];
					xb2=xb[0][indice];
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
						yr[indice]=(uint8_t)(((uint16_t)xr[2][indice]+(uint16_t)xr[1][indice]) >> 1);
						g[j]=(uint8_t)xg0;
						yg[indice]=(uint8_t)(((uint16_t)xg[2][indice]+(uint16_t)xg[1][indice]) >> 1);
						b[j]=(uint8_t)xb0;
						yb[indice]=(uint8_t)(((uint16_t)xb[2][indice]+(uint16_t)xb[1][indice]) >> 1);
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
				dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
			}
			break;
		case 2 :
			indice=0;
			for (int32_t i=0; i<h; i++)
			{
				for (int32_t j=0; j<w; j++)
				{
					xr0=xr[2][indice];
					xr1=xr[1][indice];
					xr2=xr[0][indice];
					xg0=xg[2][indice];
					xg1=xg[1][indice];
					xg2=xg[0][indice];
					xb0=xb[2][indice];
					xb1=xb[1][indice];
					xb2=xb[0][indice];
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
						yr[indice]=(uint8_t)(((uint16_t)xr[2][indice]+(uint16_t)xr[1][indice]) >> 1);
						g[j]=(uint8_t)xg0;
						yg[indice]=(uint8_t)(((uint16_t)xg[2][indice]+(uint16_t)xg[1][indice]) >> 1);
						b[j]=(uint8_t)xb0;
						yb[indice]=(uint8_t)(((uint16_t)xb[2][indice]+(uint16_t)xb[1][indice]) >> 1);
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
				dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
			}
			break;
		case 3 :
			indice=0;
			for (int32_t i=0; i<h; i++)
			{
				for (int32_t j=0; j<w; j++)
				{
					xr0=xr[2][indice];
					xr1=xr[1][indice];
					xr2=xr[0][indice];
					xg0=xg[2][indice];
					xg1=xg[1][indice];
					xg2=xg[0][indice];
					xb0=xb[2][indice];
					xb1=xb[1][indice];
					xb2=xb[0][indice];
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
						yr[indice]=(uint8_t)(((uint16_t)xr[2][indice]+(uint16_t)xr[1][indice]) >> 1);
						g[j]=(uint8_t)xg0;
						yg[indice]=(uint8_t)(((uint16_t)xg[2][indice]+(uint16_t)xg[1][indice]) >> 1);
						b[j]=(uint8_t)xb0;
						yb[indice]=(uint8_t)(((uint16_t)xb[2][indice]+(uint16_t)xb[1][indice]) >> 1);
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
				dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
			}
			break;
		}

		return;
	}

	if (pxsrc2.mFrameNumber==3)
	{
		indice=0;
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xb[3][indice]=src->b;
				xg[3][indice]=src->g;
				xr[3][indice]=src->r;
				src++;
				indice++;
			}
			src=(RGB32BMP *)((uint8_t *)src+idata.src_modulo0);
		}
		switch (mData.filter_mode)
		{
		case 1 :
			indice=0;
			for (int32_t i=0; i<h; i++)
			{
				for (int32_t j=0; j<w; j++)
				{
					xr0=xr[3][indice];
					xr1=xr[2][indice];
					xr2=xr[1][indice];
					xg0=xg[3][indice];
					xg1=xg[2][indice];
					xg2=xg[1][indice];
					xb0=xb[3][indice];
					xb1=xb[2][indice];
					xb2=xb[1][indice];
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
						yr[indice]=(uint8_t)(((uint16_t)xr[3][indice]+(uint16_t)xr[2][indice]) >> 1);
						g[j]=(uint8_t)xg0;
						yg[indice]=(uint8_t)(((uint16_t)xg[3][indice]+(uint16_t)xg[2][indice]) >> 1);
						b[j]=(uint8_t)xb0;
						yb[indice]=(uint8_t)(((uint16_t)xb[3][indice]+(uint16_t)xb[2][indice]) >> 1);
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
				dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
			}
			break;
		case 2 :
			indice=0;
			for (int32_t i=0; i<h; i++)
			{
				for (int32_t j=0; j<w; j++)
				{
					xr0=xr[3][indice];
					xr1=xr[2][indice];
					xr2=xr[1][indice];
					xg0=xg[3][indice];
					xg1=xg[2][indice];
					xg2=xg[1][indice];
					xb0=xb[3][indice];
					xb1=xb[2][indice];
					xb2=xb[1][indice];
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
						yr[indice]=(uint8_t)(((uint16_t)xr[3][indice]+(uint16_t)xr[2][indice]) >> 1);
						g[j]=(uint8_t)xg0;
						yg[indice]=(uint8_t)(((uint16_t)xg[3][indice]+(uint16_t)xg[2][indice]) >> 1);
						b[j]=(uint8_t)xb0;
						yb[indice]=(uint8_t)(((uint16_t)xb[3][indice]+(uint16_t)xb[2][indice]) >> 1);
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
				dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
			}
			break;
		case 3 :
			indice=0;
			for (int32_t i=0; i<h; i++)
			{
				for (int32_t j=0; j<w; j++)
				{
					xr0=xr[3][indice];
					xr1=xr[2][indice];
					xr2=xr[1][indice];
					xg0=xg[3][indice];
					xg1=xg[2][indice];
					xg2=xg[1][indice];
					xb0=xb[3][indice];
					xb1=xb[2][indice];
					xb2=xb[1][indice];
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
						yr[indice]=(uint8_t)(((uint16_t)xr[3][indice]+(uint16_t)xr[2][indice]) >> 1);						
						g[j]=(uint8_t)xg0;
						yg[indice]=(uint8_t)(((uint16_t)xg[3][indice]+(uint16_t)xg[2][indice]) >> 1);
						b[j]=(uint8_t)xb0;
						yb[indice]=(uint8_t)(((uint16_t)xb[3][indice]+(uint16_t)xb[2][indice]) >> 1);
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
				dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
			}
			break;
		}

		return;
	}


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
		src=(RGB32BMP *)((uint8_t *)src+idata.src_modulo0);
	}
	indice=0;
	switch (mData.filter_mode)
	{
	case 1 :
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xr0=xr[indice_2][indice];
				xr1=xr[indice_3][indice];
				xr2=xr[indice_4][indice];
				xg0=xg[indice_2][indice];
				xg1=xg[indice_3][indice];
				xg2=xg[indice_4][indice];
				xb0=xb[indice_2][indice];
				xb1=xb[indice_3][indice];
				xb2=xb[indice_4][indice];
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
					xr3=xr[indice_1][indice];
					xr4=xr[indice_0][indice];
					xg3=xg[indice_1][indice];
					xg4=xg[indice_0][indice];
					xb3=xb[indice_1][indice];
					xb4=xb[indice_0][indice];
					dr3=xr0-xr3;
					dr4=xr0-xr4;
					dg3=xg0-xg3;
					dg4=xg0-xg4;
					db3=xb0-xb3;
					db4=xb0-xb4;
					if ( (abs(dr3)<=threshold1) && (abs(dr4)<=threshold1)
						&& (abs(dg3)<=threshold1) && (abs(dg4)<=threshold1)
						&& (abs(db3)<=threshold1) && (abs(db4)<=threshold1)
						&& (abs(dr3+dr4)<=threshold2) && (abs(dg3+dg4)<=threshold2)
						&& (abs(db3+db4)<=threshold2) )
					{
						y=(xr3+xr4) >> 1;
						y=lookup[xr0+xr3+xr4+y];
						r[j]=(uint8_t)y;
						yr[indice]=(uint8_t)y;
						y=(xg3+xg4) >> 1;
						y=lookup[xg0+xg3+xg4+y];
						g[j]=(uint8_t)y;
						yg[indice]=(uint8_t)y;
						y=(xb3+xb4) >> 1;
						y=lookup[xb0+xb3+xb4+y];
						b[j]=(uint8_t)y;
						yb[indice]=(uint8_t)y;
					}
					else
					{
						if ( (abs(dr3)<=threshold1) && (abs(dr1)<=threshold1)
							&& (abs(dg3)<=threshold1) && (abs(dg1)<=threshold1)
							&& (abs(db3)<=threshold1) && (abs(db1)<=threshold1)
							&& (abs(dr3+dr1)<=threshold2) && (abs(dg3+dg1)<=threshold2)
							&& (abs(db3+db1)<=threshold2) )
						{
							y=(xr3+xr1) >> 1;
							y=lookup[xr0+xr3+xr1+y];
							r[j]=(uint8_t)y;
							yr[indice]=(uint8_t)y;
							y=(xg3+xg1) >> 1;
							y=lookup[xg0+xg3+xg1+y];
							g[j]=(uint8_t)y;
							yg[indice]=(uint8_t)y;
							y=(xb3+xb1) >> 1;
							y=lookup[xb0+xb3+xb1+y];
							b[j]=(uint8_t)y;
							yb[indice]=(uint8_t)y;
						}
						else
						{
							r[j]=(uint8_t)xr0;
							yr[indice]=(uint8_t)xr0;
							g[j]=(uint8_t)xg0;
							yg[indice]=(uint8_t)xg0;
							b[j]=(uint8_t)xb0;
							yb[indice]=(uint8_t)xb0;
						}
					}
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
			dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
		}
		break;
	case 2 :
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xr0=xr[indice_2][indice];
				xr1=xr[indice_3][indice];
				xr2=xr[indice_4][indice];
				xg0=xg[indice_2][indice];
				xg1=xg[indice_3][indice];
				xg2=xg[indice_4][indice];
				xb0=xb[indice_2][indice];
				xb1=xb[indice_3][indice];
				xb2=xb[indice_4][indice];
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
					xr3=xr[indice_1][indice];
					xr4=xr[indice_0][indice];
					xg3=xg[indice_1][indice];
					xg4=xg[indice_0][indice];
					xb3=xb[indice_1][indice];
					xb4=xb[indice_0][indice];
					dr3=xr0-xr3;
					dr4=xr0-xr4;
					dg3=xg0-xg3;
					dg4=xg0-xg4;
					db3=xb0-xb3;
					db4=xb0-xb4;
					if ( (abs(dr3)<=threshold1) && (abs(dr4)<=threshold1)
						&& (abs(dg3)<=threshold1) && (abs(dg4)<=threshold1)
						&& (abs(db3)<=threshold1) && (abs(db4)<=threshold1)
						&& (abs(dr3+dr4)<=threshold2) && (abs(dg3+dg4)<=threshold2)
						&& (abs(db3+db4)<=threshold2) )
					{
						y=(xr3+xr4) >> 1;
						y=lookup[xr0+xr3+xr4+(y << 2)];
						r[j]=(uint8_t)y;
						yr[indice]=(uint8_t)y;
						y=(xg3+xg4) >> 1;
						y=lookup[xg0+xg3+xg4+(y << 2)];
						g[j]=(uint8_t)y;
						yg[indice]=(uint8_t)y;
						y=(xb3+xb4) >> 1;
						y=lookup[xb0+xb3+xb4+(y << 2)];
						b[j]=(uint8_t)y;
						yb[indice]=(uint8_t)y;
					}
					else
					{
						if ( (abs(dr3)<=threshold1) && (abs(dr1)<=threshold1)
							&& (abs(dg3)<=threshold1) && (abs(dg1)<=threshold1)
							&& (abs(db3)<=threshold1) && (abs(db1)<=threshold1)
							&& (abs(dr3+dr1)<=threshold2) && (abs(dg3+dg1)<=threshold2)
							&& (abs(db3+db1)<=threshold2) )
						{
							y=(xr3+xr1) >> 1;
							y=lookup[xr0+xr3+xr1+(y << 2)];
							r[j]=(uint8_t)y;
							yr[indice]=(uint8_t)y;
							y=(xg3+xg1) >> 1;
							y=lookup[xg0+xg3+xg1+(y << 2)];
							g[j]=(uint8_t)y;
							yg[indice]=(uint8_t)y;
							y=(xb3+xb1) >> 1;
							y=lookup[xb0+xb3+xb1+(y << 2)];
							b[j]=(uint8_t)y;
							yb[indice]=(uint8_t)y;
						}
						else
						{
							r[j]=(uint8_t)xr0;
							yr[indice]=(uint8_t)xr0;
							g[j]=(uint8_t)xg0;
							yg[indice]=(uint8_t)xg0;
							b[j]=(uint8_t)xb0;
							yb[indice]=(uint8_t)xb0;
						}
					}
					
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
			dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
		}
		break;
	case 3 :
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				xr0=xr[indice_2][indice];
				xr1=xr[indice_3][indice];
				xr2=xr[indice_4][indice];
				xg0=xg[indice_2][indice];
				xg1=xg[indice_3][indice];
				xg2=xg[indice_4][indice];
				xb0=xb[indice_2][indice];
				xb1=xb[indice_3][indice];
				xb2=xb[indice_4][indice];
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
					xr3=xr[indice_1][indice];
					xr4=xr[indice_0][indice];
					xg3=xg[indice_1][indice];
					xg4=xg[indice_0][indice];
					xb3=xb[indice_1][indice];
					xb4=xb[indice_0][indice];
					dr3=xr0-xr3;
					dr4=xr0-xr4;
					dg3=xg0-xg3;
					dg4=xg0-xg4;
					db3=xb0-xb3;
					db4=xb0-xb4;
					if ( (abs(dr3)<=threshold1) && (abs(dr4)<=threshold1)
						&& (abs(dg3)<=threshold1) && (abs(dg4)<=threshold1)
						&& (abs(db3)<=threshold1) && (abs(db4)<=threshold1)
						&& (abs(dr3+dr4)<=threshold2) && (abs(dg3+dg4)<=threshold2)
						&& (abs(db3+db4)<=threshold2) )
					{
						y=(xr3+xr4) >> 1;
						y=lookup[xr0+xr3+xr4+(y << 3)];
						r[j]=(uint8_t)y;
						yr[indice]=(uint8_t)y;
						y=(xg3+xg4) >> 1;
						y=lookup[xg0+xg3+xg4+(y << 3)];
						g[j]=(uint8_t)y;
						yg[indice]=(uint8_t)y;
						y=(xb3+xb4) >> 1;
						y=lookup[xb0+xb3+xb4+(y << 3)];
						b[j]=(uint8_t)y;
						yb[indice]=(uint8_t)y;
					}
					else
					{
						if ( (abs(dr3)<=threshold1) && (abs(dr1)<=threshold1)
							&& (abs(dg3)<=threshold1) && (abs(dg1)<=threshold1)
							&& (abs(db3)<=threshold1) && (abs(db1)<=threshold1)
							&& (abs(dr3+dr1)<=threshold2) && (abs(dg3+dg1)<=threshold2)
							&& (abs(db3+db1)<=threshold2) )
						{
							y=(xr3+xr1) >> 1;
							y=lookup[xr0+xr3+xr1+(y << 3)];
							r[j]=(uint8_t)y;
							yr[indice]=(uint8_t)y;
							y=(xg3+xg1) >> 1;
							y=lookup[xg0+xg3+xg1+(y << 3)];
							g[j]=(uint8_t)y;
							yg[indice]=(uint8_t)y;
							y=(xb3+xb1) >> 1;
							y=lookup[xb0+xb3+xb1+(y << 3)];
							b[j]=(uint8_t)y;
							yb[indice]=(uint8_t)y;
						}
						else
						{
							r[j]=(uint8_t)xr0;
							yr[indice]=(uint8_t)xr0;
							g[j]=(uint8_t)xg0;
							yg[indice]=(uint8_t)xg0;
							b[j]=(uint8_t)xb0;
							yb[indice]=(uint8_t)xb0;
						}
					}
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
			dst=(RGB32BMP *)((uint8_t *)dst+idata.dst_modulo0);
		}
		break;
	}

	indice_0=(indice_0+1)%5;
	indice_1=(indice_1+1)%5;
	indice_2=(indice_2+1)%5;
	indice_3=(indice_3+1)%5;
	indice_4=(indice_4+1)%5;
}






void JPSDR_VHS_II::Start()
{
	Image_Data idata;
	bool ok;
	int16_t i,j;
	uint32_t size;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}
	
	const VDXPixmapLayout& pxdst=*fa->dst.mpPixmapLayout;
	const VDXPixmapLayout& pxsrc=*fa->src.mpPixmapLayout;

		idata.src_h0=pxsrc.h;
		idata.src_w0=pxsrc.w;
		idata.src_pitch0=pxsrc.pitch;
		idata.src_pitch1=pxsrc.pitch2;
		idata.src_pitch2=pxsrc.pitch3;

		idata.dst_h0=pxdst.h;
		idata.dst_w0=pxdst.w;
		idata.dst_pitch0=pxdst.pitch;
		idata.dst_pitch1=pxdst.pitch2;
		idata.dst_pitch2=pxdst.pitch3;
		
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.src_video_mode=1;
				idata.video_mode=1;
				idata.src_line0=4*idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_line0=idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :			
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_line0=idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
		}
		idata.src_modulo0=idata.src_pitch0-idata.src_line0;
		idata.src_pitch0_al=(((ptrdiff_t)idata.src_line0+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		idata.src_modulo0_al=idata.src_pitch0_al-idata.src_line0;
		idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_line0;
		idata.src_size0_al=(size_t)idata.src_h0*(size_t)idata.src_pitch0_al;
		idata.src_modulo1=idata.src_pitch1-idata.src_line1;
		idata.src_pitch1_al=(((ptrdiff_t)idata.src_line1+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		idata.src_modulo1_al=idata.src_pitch1_al-idata.src_line1;
		idata.src_size1=(size_t)idata.src_h1*(size_t)idata.src_line1;
		idata.src_size1_al=(size_t)idata.src_h1*(size_t)idata.src_pitch1_al;
		idata.src_modulo2=idata.src_pitch2-idata.src_line2;
		idata.src_pitch2_al=(((ptrdiff_t)idata.src_line2+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		idata.src_modulo2_al=idata.src_pitch2_al-idata.src_line2;
		idata.src_size2=(size_t)idata.src_h2*(size_t)idata.src_line2;
		idata.src_size2_al=(size_t)idata.src_h2*(size_t)idata.src_pitch2_al;

		switch(pxdst.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.dst_video_mode=1;
				idata.dst_line0=4*idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :			
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :			
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :			
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :			
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :			
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :			
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :			
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.dst_video_mode=9;
				idata.dst_line0=idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :						
				idata.dst_video_mode=9;
				idata.dst_line0=idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
		}
	idata.dst_modulo0=idata.dst_pitch0-idata.dst_line0;
	idata.dst_pitch0_al=(((ptrdiff_t)idata.dst_line0+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	idata.dst_modulo0_al=idata.dst_pitch0_al-idata.dst_line0;
	idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_line0;
	idata.src_size0_al=(size_t)idata.dst_h0*(size_t)idata.dst_pitch0_al;
	idata.dst_modulo1=idata.dst_pitch1-idata.dst_line1;
	idata.dst_pitch1_al=(((ptrdiff_t)idata.dst_line1+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	idata.dst_modulo1_al=idata.dst_pitch1_al-idata.dst_line1;
	idata.dst_size1=(size_t)idata.dst_h1*(size_t)idata.dst_line1;
	idata.src_size1_al=(size_t)idata.dst_h1*(size_t)idata.dst_pitch1_al;
	idata.dst_modulo2=idata.dst_pitch2-idata.dst_line2;
	idata.dst_pitch2_al=(((ptrdiff_t)idata.dst_line2+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	idata.dst_modulo2_al=idata.dst_pitch2_al-idata.dst_line2;
	idata.dst_size2=(size_t)idata.dst_h2*(size_t)idata.dst_line2;
	idata.src_size2_al=(size_t)idata.dst_h2*(size_t)idata.dst_pitch2_al;

	if (idata.src_bits_pixel<=16)
	{
		if (idata.src_full_mode)
		{
			idata.src_Y_Min=0;
			idata.src_U_Min=0;
			idata.src_V_Min=0;

			idata.src_Y_Max=(uint16_t)(((int)1 << idata.src_bits_pixel)-1);
			idata.src_U_Max=(uint16_t)(((int)1 << idata.src_bits_pixel)-1);
			idata.src_V_Max=(uint16_t)(((int)1 << idata.src_bits_pixel)-1);
		}
		else
		{
			idata.src_Y_Min=(uint16_t)((int)16 << (idata.src_bits_pixel-8));
			idata.src_U_Min=(uint16_t)((int)16 << (idata.src_bits_pixel-8));
			idata.src_V_Min=(uint16_t)((int)16 << (idata.src_bits_pixel-8));
			idata.src_Y_Max=(uint16_t)((int)235 << (idata.src_bits_pixel-8));
			idata.src_U_Max=(uint16_t)((int)240 << (idata.src_bits_pixel-8));
			idata.src_V_Max=(uint16_t)((int)240 << (idata.src_bits_pixel-8));
		}
	}
	if (idata.dst_bits_pixel<=16)
	{
		if (idata.dst_full_mode)
		{
			idata.dst_Y_Min=0;
			idata.dst_U_Min=0;
			idata.dst_V_Min=0;

			idata.dst_Y_Max=(uint16_t)(((int)1 << idata.dst_bits_pixel)-1);
			idata.dst_U_Max=(uint16_t)(((int)1 << idata.dst_bits_pixel)-1);
			idata.dst_V_Max=(uint16_t)(((int)1 << idata.dst_bits_pixel)-1);
		}
		else
		{
			idata.dst_Y_Min=(uint16_t)((int)16 << (idata.dst_bits_pixel-8));
			idata.dst_U_Min=(uint16_t)((int)16 << (idata.dst_bits_pixel-8));
			idata.dst_V_Min=(uint16_t)((int)16 << (idata.dst_bits_pixel-8));
			idata.dst_Y_Max=(uint16_t)((int)235 << (idata.dst_bits_pixel-8));
			idata.dst_U_Max=(uint16_t)((int)240 << (idata.dst_bits_pixel-8));
			idata.dst_V_Max=(uint16_t)((int)240 << (idata.dst_bits_pixel-8));
		}
	}

	switch(idata.src_color_matrix)
	{
		case MATRIX_BT601 :
			idata.src_Kr=0.299;
			idata.src_Kb=0.114;
			break;
		case MATRIX_BT709 :
			idata.src_Kr=0.2126;
			idata.src_Kb=0.0722;
			break;
		case MATRIX_BT2020 :
			idata.src_Kr=0.2627;
			idata.src_Kb=0.0593;
			break;
		default :
			idata.src_Kr=0.299;
			idata.src_Kb=0.114;
			break;
	}
	idata.src_Kg=1.0-idata.src_Kr-idata.src_Kb;
	if ((idata.src_full_mode) || (idata.src_bits_pixel>16))
	{
		idata.src_Coeff_Y=1.0;
		idata.src_Coeff_U=1.0;
		idata.src_Coeff_V=1.0;
	}
	else
	{
		idata.src_Coeff_Y=219.0/255.0;
		idata.src_Coeff_U=224.0/255.0;
		idata.src_Coeff_V=224.0/255.0;
	}
	idata.src_Ur=-0.5*idata.src_Kr/(1.0-idata.src_Kb);
	idata.src_Ug=-0.5*idata.src_Kg/(1.0-idata.src_Kb);
	idata.src_Ub=0.5;
	idata.src_Vr=0.5;
	idata.src_Vg=-0.5*idata.src_Kg/(1.0-idata.src_Kr);
	idata.src_Vb=-0.5*idata.src_Kb/(1.0-idata.src_Kr);
	idata.src_Rv=2.0*(1.0-idata.src_Kr);
	idata.src_Gu=-2.0*idata.src_Kb*(1.0-idata.src_Kb)/idata.src_Kg;
	idata.src_Gv=-2.0*idata.src_Kr*(1.0-idata.src_Kr)/idata.src_Kg;
	idata.src_Bu=2.0*(1.0-idata.src_Kb);
	if (idata.src_bits_pixel<=16)
	{
		idata.src_Off_Y=idata.src_Y_Min;
		idata.src_Off_U=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.src_Off_V=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.src_Off_R=-((double)idata.src_Off_Y/idata.src_Coeff_Y+idata.src_Rv*(double)idata.src_Off_V/idata.src_Coeff_V);
		idata.src_Off_G=-((double)idata.src_Off_Y/idata.src_Coeff_Y+idata.src_Gu*(double)idata.src_Off_U/idata.src_Coeff_U
			+idata.src_Gv*(double)idata.src_Off_V/idata.src_Coeff_V);
		idata.src_Off_B=-((double)idata.src_Off_Y/idata.src_Coeff_Y+idata.src_Bu*(double)idata.src_Off_U/idata.src_Coeff_U);
	}
	else
	{
		idata.src_Off_Y=0;
		idata.src_Off_U=0;
		idata.src_Off_V=0;
		idata.src_Coeff_Y=1.0;
		idata.src_Coeff_U=1.0;
		idata.src_Coeff_V=1.0;
		idata.src_Off_R=0.0;
		idata.src_Off_G=0.0;
		idata.src_Off_B=0.0;
	}

	switch(idata.dst_color_matrix)
	{
		case MATRIX_BT601 :
			idata.dst_Kr=0.299;
			idata.dst_Kb=0.114;
			break;
		case MATRIX_BT709 :
			idata.dst_Kr=0.2126;
			idata.dst_Kb=0.0722;
			break;
		case MATRIX_BT2020 :
			idata.dst_Kr=0.2627;
			idata.dst_Kb=0.0593;
			break;
		default :
			idata.dst_Kr=0.299;
			idata.dst_Kb=0.114;
			break;
	}
	idata.dst_Kg=1.0-idata.dst_Kr-idata.dst_Kb;
	if ((idata.dst_full_mode) || (idata.dst_bits_pixel>16))
	{
		idata.dst_Coeff_Y=1.0;
		idata.dst_Coeff_U=1.0;
		idata.dst_Coeff_V=1.0;
	}
	else
	{
		idata.dst_Coeff_Y=219.0/255.0;
		idata.dst_Coeff_U=224.0/255.0;
		idata.dst_Coeff_V=224.0/255.0;
	}
	idata.dst_Ur=-0.5*idata.dst_Kr/(1.0-idata.dst_Kb);
	idata.dst_Ug=-0.5*idata.dst_Kg/(1.0-idata.dst_Kb);
	idata.dst_Ub=0.5;
	idata.dst_Vr=0.5;
	idata.dst_Vg=-0.5*idata.dst_Kg/(1.0-idata.dst_Kr);
	idata.dst_Vb=-0.5*idata.dst_Kb/(1.0-idata.dst_Kr);
	idata.dst_Rv=2.0*(1.0-idata.dst_Kr);
	idata.dst_Gu=-2.0*idata.dst_Kb*(1.0-idata.dst_Kb)/idata.dst_Kg;
	idata.dst_Gv=-2.0*idata.dst_Kr*(1.0-idata.dst_Kr)/idata.dst_Kg;
	idata.dst_Bu=2.0*(1.0-idata.dst_Kb);
	if (idata.dst_bits_pixel<=16)
	{
		idata.dst_Off_Y=idata.dst_Y_Min;
		idata.dst_Off_U=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.dst_Off_V=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.dst_Off_R=-((double)idata.dst_Off_Y/idata.dst_Coeff_Y+idata.dst_Rv*(double)idata.dst_Off_V/idata.dst_Coeff_V);
		idata.dst_Off_G=-((double)idata.dst_Off_Y/idata.dst_Coeff_Y+idata.dst_Gu*(double)idata.dst_Off_U/idata.dst_Coeff_U
			+idata.dst_Gv*(double)idata.dst_Off_V/idata.dst_Coeff_V);
		idata.dst_Off_B=-((double)idata.dst_Off_Y/idata.dst_Coeff_Y+idata.dst_Bu*(double)idata.dst_Off_U/idata.dst_Coeff_U);
	}
	else
	{
		idata.dst_Off_Y=0;
		idata.dst_Off_U=0;
		idata.dst_Off_V=0;
		idata.dst_Coeff_Y=1.0;
		idata.dst_Coeff_U=1.0;
		idata.dst_Coeff_V=1.0;
		idata.dst_Off_R=0.0;
		idata.dst_Off_G=0.0;
		idata.dst_Off_B=0.0;
	}
	/*
	R',G',B' : RGB value normalised between (0.0,0.0,0.0)[Black] to (1.0,1.0,1.0)[White]
	Y' between (0.0 / +1.0) and Pb/Pr between (-0.5 / +0.5)

	Y'=Kr*R'+Kg*G'+Kb*B'
	Pb=0.5*(B'-Y')/(1-Kb)=-0.5*Kr/(1-Kb))*R'-0.5*Kg/(1-Kb)*G'+0.5*B'
	Pr=0.5*(R'-Y')/(1-Kr)=0.5*R'-0.5*Kg/(1-Kr)*G'-0.5*Kb/(1-Kr)*B'

	R'=Y'+2*(1-Kr)*Pr
	G'=Y'-(2*Kb*(1-Kb)/Kg)*Pb-(2*Kr*(1-Kr)/Kg)*Pr
	B'=Y'+2*(1-Kb)*Pb

	(Y) Y'=Kr*R'+Kg*G'+Kb*B'
	(U) Pb=Ur*R'+Ug*G'+Ub*B'
	(V) Pr=Vr*R'+Vg*G'+Vb*B'

	(R) R'=Y'+Rv*Pr
	(G) G'=Y'+Gu*Pb+Gv*Pr
	(B) B'=Y'+Bu*Pb

	For 8 bits data limited range :
	(Y',Cb,Cr) = (16,128,128) + (219*Y',224*Pb,224*Pr)

	Y = Off_Y + (Kr*R+Kg*G+Kb*B)*Coeff_Y
	(Cb) U = Off_U + (Ur*R+Ug*G+Ub*B)*Coeff_U
	(Cr) V = Off_V + (Vr*R+Vg*G+Vb*B)*Coeff_V

	R = Y/Coeff_Y + Rv*V/Coeff_V + Off_R [ Off_R = -(Off_Y/Coeff_Y+Rv*Off_V/Coeff_V) ]
	G = Y/Coeff_Y + Gu*U/Coeff_U + Gv*V/Coeff_V + Off_G [ Off_G = -(Off_Y/Coeff_Y+Gu*Off_U/Coeff_U+Gv*Off_V/Coeff_V) ]
	B = Y/Coeff_Y + Bu*U/Coeff_U + Off_B [ Off_B = -(Off_Y/Coeff_Y+Bu*Off_U/Coeff_U) ]
	*/
	
	image_data=idata;

	size=(idata.src_h0*idata.src_w0)*sizeof(uint8_t);
	for (i=0; i<3; i++)
		for (j=0; j<5; j++)
			_x[i][j]=(uint8_t *)malloc(size);
	for (i=0; i<3; i++)
		_y[i]=(uint8_t *)malloc(size);
	for (i=0; i<3; i++)
		line_out[i]=(uint8_t *)malloc(idata.src_w0*sizeof(uint8_t));
	
	ok=true;
	for (i=0; i<3; i++)
		for (j=0; j<5; j++)
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
			for (j=4; j>=0; j--)
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
	indice_0=4;
	indice_1=3;
	indice_2=2;
	indice_3=1;
	indice_4=0;
}


void JPSDR_VHS_II::End()
{
	int16_t i,j;

	for (i=2; i>=0; i--)
		myfree(line_out[i]);
	for (i=2; i>=0; i--)
		myfree(_y[i]);
	for (i=2; i>=0; i--)
	{
		for (j=4; j>=0; j--)
			myfree(_x[i][j]);
	}
}


void JPSDR_VHS_II::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
    mData.threshold1=argv[0].asInt();
    mData.threshold2=argv[1].asInt();
	mData.filter_mode=argv[2].asInt();
}


void JPSDR_VHS_II::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf, maxlen,"Config(%d, %d, %d)",mData.threshold1,mData.threshold2,mData.filter_mode);
}


extern VDXFilterDefinition filterDef_JPSDR_VHS_II=
VDXVideoFilterDefinition<JPSDR_VHS_II>("JPSDR","VHS II v2.3.4","Filter to remove VHS noise [lag 2].");

