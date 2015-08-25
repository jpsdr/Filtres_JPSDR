#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

extern int g_VFVAPIVersion;


extern "C" void JPSDR_RGBConvert_RGB32toYV24_SSE(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB32toYV16_SSE(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB32toYUYV_SSE(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,
	int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB32toYV12_SSE(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_pitch,ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch_Y,ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,
	int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_YV24toRGB32_SSE(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int16_t offset_R,int16_t offset_G,int16_t offset_B,int16_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);



class JPSDR_RGBConvertData
{
public:
	uint8_t color_matrix,output_mode;
	bool full_range,auto_detect;

	JPSDR_RGBConvertData(void);
};


JPSDR_RGBConvertData::JPSDR_RGBConvertData(void)
{
	color_matrix=1;
	output_mode=0;
	full_range=false;
	auto_detect=false;
}

class JPSDR_RGBConvert;


class JPSDR_RGBConvertDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_RGBConvertDialog(JPSDR_RGBConvertData& Data, IVDXFilterPreview2 *ifp, JPSDR_RGBConvert *filt) : mData(Data), mifp(ifp), filter(filt)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_RGBCONVERT),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_RGBConvertData& mData;
	JPSDR_RGBConvertData moldData;
	IVDXFilterPreview2 *const mifp;
	JPSDR_RGBConvert *const filter;
};


class JPSDR_RGBConvert : public VDXVideoFilter
{
public:
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	void Compute_Lookup(void);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	int16_t lookup[2304];
	uint8_t convertion_mode,autodetect_color;
	int16_t Min_U,Max_U,Min_Y,Max_Y,Min_V,Max_V;
	int16_t Offset_Y,Offset_U,Offset_V,Offset_R,Offset_G,Offset_B;
	double Coeff_Y,Coeff_U,Coeff_V;
	bool autodetect_range;
	bool SSE2_Enable;

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
	void RGB32toYV24(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void RGB32toYV16(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void RGB32toYUY2(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void RGB32toYV12(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void YV24toRGB32(const void *src_y,const void *src_u,const void *src_v, void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,
		ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch);
		
	JPSDR_RGBConvertData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_RGBConvert)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_RGBConvert,ScriptConfig,"iiii")
VDXVF_END_SCRIPT_METHODS()


INT_PTR JPSDR_RGBConvertDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_RGBConvertDialog::OnInit()
{
	moldData=mData;

	switch (mData.color_matrix)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_BT709,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_BT601,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_SMPTE_240M,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_FCC,1); break;
	}

	switch (mData.output_mode)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_OUTPUT_YV24,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_OUTPUT_YV16,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_OUTPUT_YUY2,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_OUTPUT_YV12,1); break;
	}

	CheckDlgButton(mhdlg,IDC_FULL_RANGE,mData.full_range?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_AUTODETECT,mData.auto_detect?BST_CHECKED:BST_UNCHECKED);

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}


void JPSDR_RGBConvertDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}

bool JPSDR_RGBConvertDialog::SaveToData()
{

	if (!!IsDlgButtonChecked(mhdlg,IDC_BT709)) mData.color_matrix=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_BT601)) mData.color_matrix=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_SMPTE_240M)) mData.color_matrix=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_FCC)) mData.color_matrix=3;

	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YV24)) mData.output_mode=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YV16)) mData.output_mode=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YUY2)) mData.output_mode=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YV12)) mData.output_mode=3;

	mData.full_range=!!IsDlgButtonChecked(mhdlg,IDC_FULL_RANGE);
	mData.auto_detect=!!IsDlgButtonChecked(mhdlg,IDC_AUTODETECT);

	return true;
}



bool JPSDR_RGBConvertDialog::OnCommand(int cmd)
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
		case IDC_BT709 :
		case IDC_BT601 :
		case IDC_SMPTE_240M :
		case IDC_FCC :
		case IDC_FULL_RANGE :
		case IDC_AUTODETECT :
			if (mifp && SaveToData())
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			return true;
		case IDC_OUTPUT_YV24 :
		case IDC_OUTPUT_YV16 :
		case IDC_OUTPUT_YUY2 :
		case IDC_OUTPUT_YV12 :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



void JPSDR_RGBConvert::Compute_Lookup(void)
{
	double kr,kg,kb;
	double u1,u2,v1,v2,r1,g1,g2,b1;
	int16_t i;
	int8_t color_matrix;
	bool full_range;

	if (mData.auto_detect)
	{
		full_range=autodetect_range;
		color_matrix=autodetect_color;
	}
	else
	{
		full_range=mData.full_range;
		color_matrix=mData.color_matrix;
	}

	if (full_range)
	{
		Min_Y=0;
		Max_Y=255;
		Min_U=0;
		Max_U=255;
		Min_V=0;
		Max_V=255;
		Coeff_Y=255.0;
		Coeff_U=255.0;
		Coeff_V=255.0;
	}
	else
	{
		Min_Y=16;
		Max_Y=235;
		Min_U=16;
		Max_U=240;
		Min_V=16;
		Max_V=240;
		Coeff_Y=219.0;
		Coeff_U=224.0;
		Coeff_V=224.0;
	}	
	
	switch (color_matrix)
	{
		case 0 : kr=0.2126; kb=0.0722; break;
		case 1 : kr=0.299; kb=0.114; break;
		case 2 : kr=0.212; kb=0.087; break;
		case 3 : kr=0.3; kb=0.11; break;
	}
	kg=1.0-kr-kb;

	u1=-kr/(1.0-kb); u2=-kg/(1.0-kb);
	v1=-kg/(1.0-kr); v2=-kb/(1.0-kr);

	r1=2.0*(1.0-kr);
	g1=-(2.0*(1.0-kb)*kb)/kg; g2=-(2.0*(1.0-kr)*kr)/kg;
	b1=2.0*(1.0-kb);

	Offset_Y=(Min_Y << 6)+32;
	Offset_U=(128 << 6)+32;
	Offset_V=(128 << 6)+32;

	Offset_R=(int16_t)-round(16.0+((32.0*255.0*Min_Y)/Coeff_Y)+((128.0*r1*255.0*32.0)/Coeff_V));
	Offset_G=(int16_t)-round(16.0+((32.0*255.0*Min_Y)/Coeff_Y)+((128.0*g1*255.0*32.0)/Coeff_U)+((128.0*g2*255.0*32.0)/Coeff_V));
	Offset_B=(int16_t)-round(16.0+((32.0*255.0*Min_Y)/Coeff_Y)+((128.0*b1*255.0*32.0)/Coeff_U));

	switch (convertion_mode)
	{
		case 0 :
			for (i=0; i<=255; i++)
			{
				lookup[i]=(int16_t)round((i*kr*Coeff_Y*64.0)/255.0);
				lookup[i+256]=(int16_t)round((i*kg*Coeff_Y*64.0)/255.0);
				lookup[i+512]=(int16_t)round((i*kb*Coeff_Y*64.0)/255.0);
				lookup[i+768]=(int16_t)round((i*u1*Coeff_U*0.5*64.0)/255.0);
				lookup[i+1024]=(int16_t)round((i*u2*Coeff_U*0.5*64.0)/255.0);
				lookup[i+1280]=(int16_t)round((i*Coeff_U*0.5*64.0)/255.0);
				lookup[i+1536]=(int16_t)round((i*Coeff_V*0.5*64.0)/255.0);
				lookup[i+1792]=(int16_t)round((i*v1*Coeff_V*0.5*64.0)/255.0);
				lookup[i+2048]=(int16_t)round((i*v2*Coeff_V*0.5*64.0)/255.0);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
			for (i=0; i<=255; i++)
			{
				lookup[i]=(int16_t)round((i*255.0*32.0)/Coeff_Y);
				lookup[i+256]=(int16_t)round((i*r1*255.0*32.0)/Coeff_V);
				lookup[i+512]=(int16_t)round((i*g1*255.0*32.0)/Coeff_U);
				lookup[i+768]=(int16_t)round((i*g2*255.0*32.0)/Coeff_V);
				lookup[i+1024]=(int16_t)round((i*b1*255.0*32.0)/Coeff_U);
				lookup[i+1280]=0;
				lookup[i+1536]=0;
				lookup[i+1792]=0;
				lookup[i+2048]=0;
			}
			break;
	}

}



uint32 JPSDR_RGBConvert::GetParams()
{
	if (g_VFVAPIVersion<12) return(FILTERPARAM_NOT_SUPPORTED);

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :*/				
		case nsVDXPixmap::kPixFormat_XRGB8888 :
/*		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
/*		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
/*		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
/*		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
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
			convertion_mode=0;
			autodetect_color=mData.color_matrix;
			autodetect_range=mData.full_range;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
			convertion_mode=1;
			autodetect_color=0;
			autodetect_range=false;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
			convertion_mode=1;
			autodetect_color=1;
			autodetect_range=true;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
			convertion_mode=1;
			autodetect_color=0;
			autodetect_range=true;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
			convertion_mode=1;
			autodetect_color=1;
			autodetect_range=false;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
			convertion_mode=2;
			autodetect_color=0;
			autodetect_range=false;
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
			convertion_mode=2;
			autodetect_color=1;
			autodetect_range=true;
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
			convertion_mode=2;
			autodetect_color=0;
			autodetect_range=true;
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
			convertion_mode=2;
			autodetect_color=1;
			autodetect_range=false;
			break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
			autodetect_color=0;
			autodetect_range=false;
			convertion_mode=3;
			break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
			autodetect_color=1;
			autodetect_range=true;
			convertion_mode=3;
			break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
			autodetect_color=0;
			autodetect_range=true;
			convertion_mode=3;
			break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
			autodetect_color=1;
			autodetect_range=false;
			convertion_mode=3;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
			autodetect_color=0;
			autodetect_range=false;
			convertion_mode=4;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
			autodetect_color=1;
			autodetect_range=true;
			convertion_mode=4;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
			autodetect_color=0;
			autodetect_range=true;
			convertion_mode=4;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
			autodetect_color=1;
			autodetect_range=false;
			convertion_mode=4;
			break;
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
			autodetect_color=0;
			autodetect_range=false;
			convertion_mode=5;
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
			autodetect_color=1;
			autodetect_range=true;
			convertion_mode=5;
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
			autodetect_color=0;
			autodetect_range=true;
			convertion_mode=5;
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
			autodetect_color=1;
			autodetect_range=false;
			convertion_mode=5;
			break;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset=fa->src.offset;

/*	switch(convertion_mode)
	{
		case 0 : pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar; break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 : pxdst.format=nsVDXPixmap::kPixFormat_XRGB8888; break;
	}*/
	switch(convertion_mode)
	{
		case 0 :
			switch(mData.output_mode)
			{
				case 0 :
					if ((mData.color_matrix==0) || (mData.color_matrix==2))
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_709;
					}
					else
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar;
					}
					break;			
				case 1 :
					if ((mData.color_matrix==0) || (mData.color_matrix==2))
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709;
					}
					else
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar;
					}
					break;			
				case 2 :
					if ((mData.color_matrix==0) || (mData.color_matrix==2))
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709;
					}
					else
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV;
					}
					break;			
				case 3 :
					if ((mData.color_matrix==0) || (mData.color_matrix==2))
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709;
					}
					else
					{
						if (mData.full_range) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_FR;
						else pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar;
					}
					break;			
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 : pxdst.format=nsVDXPixmap::kPixFormat_XRGB8888; break;
	}
	pxdst.pitch=0;

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM);
}


void JPSDR_RGBConvert::Start()
{
	Image_Data idata;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter !");
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
				idata.src_modulo0=idata.src_pitch0-4*idata.src_w0;
				idata.src_size0=idata.src_h0*4*idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=idata.src_h0*4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=idata.src_w0*idata.src_h0;
				idata.src_size1=idata.src_w1*idata.src_h1;
				idata.src_size2=idata.src_w2*idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=idata.src_h0*idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :			
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=idata.src_h0*idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;																
				break;
		}
		switch(pxdst.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.dst_video_mode=1;
				idata.dst_modulo0=idata.dst_pitch0-4*idata.dst_w0;
				idata.dst_size0=idata.dst_h0*4*idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :			
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :			
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=idata.dst_h0*4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :			
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :			
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :			
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :			
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :			
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=idata.dst_w0*idata.dst_h0;
				idata.dst_size1=idata.dst_w1*idata.dst_h1;
				idata.dst_size2=idata.dst_w2*idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=idata.dst_h0*idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :						
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=idata.dst_h0*idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
		}

	image_data=idata;

	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	Compute_Lookup();
}





void JPSDR_RGBConvert::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];
	int8_t color_matrix;
	bool full_range;

	if (mData.auto_detect)
	{
		full_range=autodetect_range;
		color_matrix=autodetect_color;
	}
	else
	{
		full_range=mData.full_range;
		color_matrix=mData.color_matrix;
	}

	switch(color_matrix)
	{
		case 0 : strcpy_s(output_string,255," Color matrix : BT.709"); break;
		case 1 : strcpy_s(output_string,255," Color matrix : BT.601"); break;
		case 2 : strcpy_s(output_string,255," Color matrix : SMPTE 240M"); break;
		case 3 : strcpy_s(output_string,255," Color matrix : FCC"); break;
	}
	if (mData.auto_detect) strcat_s(output_string,255," Autodetec");
	switch(mData.output_mode)
	{
		case 0 : strcat_s(output_string,255," Output YV24"); break;
		case 1 : strcat_s(output_string,255," Output YV16"); break;
		case 2 : strcat_s(output_string,255," Output YUY2"); break;
		case 3 : strcat_s(output_string,255," Output YV12"); break;
	}
	if (full_range) strcat_s(output_string,255," Full Range YCbCr");
	SafePrintf(buf,maxlen,output_string);

}




void JPSDR_RGBConvert::RGB32toYV24(const void *src_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const RGB32BMP *src;
	uint8_t *dst_y,*dst_u,*dst_v;

	src=(RGB32BMP *)src_;
	dst_y=(uint8_t *)dst_y_;
	dst_u=(uint8_t *)dst_u_;
	dst_v=(uint8_t *)dst_v_;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,u,v;
			const uint16_t b=src[j].b,g=src[j].g,r=src[j].r;

			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;

			if (y<Min_Y) dst_y[j]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[j]=(uint8_t)Max_Y;
				else dst_y[j]=(uint8_t)y;
			}
			if (u<Min_U) dst_u[j]=(uint8_t)Min_U;
			else
			{
				if (u>Max_U) dst_u[j]=(uint8_t)Max_U;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V) dst_v[j]=(uint8_t)Min_V;
			else
			{
				if (v>Max_V) dst_v[j]=(uint8_t)Max_V;
				else dst_v[j]=(uint8_t)v;
			}
		}
		src=(RGB32BMP *)((uint8_t *)src+src_pitch);
		dst_y+=dst_pitch_y;
		dst_u+=dst_pitch_u;
		dst_v+=dst_pitch_v;
	}
}


void JPSDR_RGBConvert::RGB32toYV16(const void *src_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const RGB32BMP *src;
	uint8_t *dst_y,*dst_u,*dst_v;
	const int32_t w0=((w+1) >> 1)-1;

	src=(RGB32BMP *)src_;
	dst_y=(uint8_t *)dst_y_;
	dst_u=(uint8_t *)dst_u_;
	dst_v=(uint8_t *)dst_v_;

	for (int32_t i=0; i<h; i++)
	{
		int32_t k=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y,u,v;
			uint16_t b,g,r;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			if (u<Min_U) dst_u[j]=(uint8_t)Min_U;
			else
			{
				if (u>Max_U) dst_u[j]=(uint8_t)Max_U;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V) dst_v[j]=(uint8_t)Min_V;
			else
			{
				if (v>Max_V) dst_v[j]=(uint8_t)Max_V;
				else dst_v[j]=(uint8_t)v;
			}
			k++;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			k++;
		}

		int16_t y,u,v;
		uint16_t b,g,r;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
		u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
		v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
		if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
			else dst_y[k]=(uint8_t)y;
		}
		if (u<Min_U) dst_u[w0]=(uint8_t)Min_U;
		else
		{
			if (u>Max_U) dst_u[w0]=(uint8_t)Max_U;
			else dst_u[w0]=(uint8_t)u;
		}
		if (v<Min_V) dst_v[w0]=(uint8_t)Min_V;
		else
		{
			if (v>Max_V) dst_v[w0]=(uint8_t)Max_V;
			else dst_v[w0]=(uint8_t)v;
		}
		k++;

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
		}

		src=(RGB32BMP *)((uint8_t *)src+src_pitch);
		dst_y+=dst_pitch_y;
		dst_u+=dst_pitch_u;
		dst_v+=dst_pitch_v;
	}
}



void JPSDR_RGBConvert::RGB32toYUY2(const void *src_,void *dst_,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src;
	uint32_t *dst;
	const int32_t w0=((w+1) >> 1)-1;

	src=(RGB32BMP *)src_;
	dst=(uint32_t *)dst_;

	for (int32_t i=0; i<h; i++)
	{
		int32_t k=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y,u,v;
			uint16_t b,g,r;
			UYUYV d;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
			if (y<Min_Y) d.yuyv.y1=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) d.yuyv.y1=(uint8_t)Max_Y;
				else d.yuyv.y1=(uint8_t)y;
			}
			if (u<Min_U) d.yuyv.u=(uint8_t)Min_U;
			else
			{
				if (u>Max_U) d.yuyv.u=(uint8_t)Max_U;
				else d.yuyv.u=(uint8_t)u;
			}
			if (v<Min_V) d.yuyv.v=(uint8_t)Min_V;
			else
			{
				if (v>Max_V) d.yuyv.v=(uint8_t)Max_V;
				else d.yuyv.v=(uint8_t)v;
			}
			k++;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) d.yuyv.y2=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) d.yuyv.y2=(uint8_t)Max_Y;
				else d.yuyv.y2=(uint8_t)y;
			}
			k++;
			dst[j]=d.data32;
		}

		int16_t y,u,v;
		uint16_t b,g,r;
		UYUYV d;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
		u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
		v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
		if (y<Min_Y) d.yuyv.y1=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) d.yuyv.y1=(uint8_t)Max_Y;
			else d.yuyv.y1=(uint8_t)y;
		}
		if (u<Min_U) d.yuyv.u=(uint8_t)Min_U;
		else
		{
			if (u>Max_U) d.yuyv.u=(uint8_t)Max_U;
			else d.yuyv.u=(uint8_t)u;
		}
		if (v<Min_V) d.yuyv.v=(uint8_t)Min_V;
		else
		{
			if (v>Max_V) d.yuyv.v=(uint8_t)Max_V;
			else d.yuyv.v=(uint8_t)v;
		}
		k++;

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) d.yuyv.y2=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) d.yuyv.y2=(uint8_t)Max_Y;
				else d.yuyv.y2=(uint8_t)y;
			}
		}
		else d.yuyv.y2=(uint8_t)Min_Y;

		dst[w0]=d.data32;

		src=(RGB32BMP *)((uint8_t *)src+src_pitch);
		dst=(uint32_t *)((uint8_t *)dst+dst_pitch);
	}
}


void JPSDR_RGBConvert::RGB32toYV12(const void *src_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const RGB32BMP *src,*src2;
	uint8_t *dst_y,*dst_y2,*dst_u,*dst_v;
	const int32_t w0=((w+1) >> 1)-1;
	const int32_t h0=((h+1) >> 1)-1;

	src=(RGB32BMP *)src_;
	src2=(RGB32BMP *)((uint8_t *)src_+src_pitch);
	dst_y=(uint8_t *)dst_y_;
	dst_y2=dst_y+dst_pitch_y;
	dst_u=(uint8_t *)dst_u_;
	dst_v=(uint8_t *)dst_v_;

	src_pitch<<=1;
	dst_pitch_y<<=1;

	for (int32_t i=0; i<h0; i++)
	{
		int32_t k;

		k=0;
		for (int32_t j=0; j<w0; j++)
		{
			int16_t y,u,v;
			uint16_t b,g,r,b2,r2,g2;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			b2=src2[k].b;
			g2=src2[k].g;
			r2=src2[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
			if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
				else dst_y2[k]=(uint8_t)y;
			}
			k++;

			r+=r2; r>>=1;
			g+=g2; g>>=1;
			b+=b2; b>>=1;
			u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
			if (u<Min_U) dst_u[j]=(uint8_t)Min_U;
			else
			{
				if (u>Max_U) dst_u[j]=(uint8_t)Max_U;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V) dst_v[j]=(uint8_t)Min_V;
			else
			{
				if (v>Max_V) dst_v[j]=(uint8_t)Max_V;
				else dst_v[j]=(uint8_t)v;
			}

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			b2=src2[k].b;
			g2=src2[k].g;
			r2=src2[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
			if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
				else dst_y2[k]=(uint8_t)y;
			}
			k++;
		}

		int16_t y,u,v;
		uint16_t b,g,r,b2,r2,g2;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		b2=src2[k].b;
		g2=src2[k].g;
		r2=src2[k].r;
		y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
		if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
			else dst_y[k]=(uint8_t)y;
		}
		y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
		if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
			else dst_y2[k]=(uint8_t)y;
		}
		k++;

		r+=r2; r>>=1;
		g+=g2; g>>=1;
		b+=b2; b>>=1;
		u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
		v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
		if (u<Min_U) dst_u[w0]=(uint8_t)Min_U;
		else
		{
			if (u>Max_U) dst_u[w0]=(uint8_t)Max_U;
			else dst_u[w0]=(uint8_t)u;
		}
		if (v<Min_V) dst_v[w0]=(uint8_t)Min_V;
		else
		{
			if (v>Max_V) dst_v[w0]=(uint8_t)Max_V;
			else dst_v[w0]=(uint8_t)v;
		}

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			b2=src2[k].b;
			g2=src2[k].g;
			r2=src2[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
			if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
				else dst_y2[k]=(uint8_t)y;
			}
		}

		src=(RGB32BMP *)((uint8_t *)src+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		dst_y+=dst_pitch_y;
		dst_y2+=dst_pitch_y;
		dst_u+=dst_pitch_u;
		dst_v+=dst_pitch_v;
	}

	if ((h&1)==0)
	{
		int32_t k;

		k=0;
		for (int32_t j=0; j<w0; j++)
		{
			int16_t y,u,v;
			uint16_t b,g,r,b2,r2,g2;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			b2=src2[k].b;
			g2=src2[k].g;
			r2=src2[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
			if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
				else dst_y2[k]=(uint8_t)y;
			}
			k++;

			r+=r2; r>>=1;
			g+=g2; g>>=1;
			b+=b2; b>>=1;
			u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
			if (u<Min_U) dst_u[j]=(uint8_t)Min_U;
			else
			{
				if (u>Max_U) dst_u[j]=(uint8_t)Max_U;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V) dst_v[j]=(uint8_t)Min_V;
			else
			{
				if (v>Max_V) dst_v[j]=(uint8_t)Max_V;
				else dst_v[j]=(uint8_t)v;
			}

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			b2=src2[k].b;
			g2=src2[k].g;
			r2=src2[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
			if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
				else dst_y2[k]=(uint8_t)y;
			}
			k++;
		}

		int16_t y,u,v;
		uint16_t b,g,r,b2,r2,g2;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		b2=src2[k].b;
		g2=src2[k].g;
		r2=src2[k].r;
		y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
		if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
			else dst_y[k]=(uint8_t)y;
		}
		y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
		if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
			else dst_y2[k]=(uint8_t)y;
		}
		k++;

		r+=r2; r>>=1;
		g+=g2; g>>=1;
		b+=b2; b>>=1;
		u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
		v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
		if (u<Min_U) dst_u[w0]=(uint8_t)Min_U;
		else
		{
			if (u>Max_U) dst_u[w0]=(uint8_t)Max_U;
			else dst_u[w0]=(uint8_t)u;
		}
		if (v<Min_V) dst_v[w0]=(uint8_t)Min_V;
		else
		{
			if (v>Max_V) dst_v[w0]=(uint8_t)Max_V;
			else dst_v[w0]=(uint8_t)v;
		}

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			b2=src2[k].b;
			g2=src2[k].g;
			r2=src2[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			y=(Offset_Y+lookup[r2]+lookup[g2+256]+lookup[b2+512]) >> 6;
			if (y<Min_Y) dst_y2[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y2[k]=(uint8_t)Max_Y;
				else dst_y2[k]=(uint8_t)y;
			}
		}
	}
	else
	{
		int32_t k;

		k=0;
		for (int32_t j=0; j<w0; j++)
		{
			int16_t y,u,v;
			uint16_t b,g,r;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			if (u<Min_U) dst_u[j]=(uint8_t)Min_U;
			else
			{
				if (u>Max_U) dst_u[j]=(uint8_t)Max_U;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V) dst_v[j]=(uint8_t)Min_V;
			else
			{
				if (v>Max_V) dst_v[j]=(uint8_t)Max_V;
				else dst_v[j]=(uint8_t)v;
			}
			k++;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
			k++;
		}

		int16_t y,u,v;
		uint16_t b,g,r;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
		u=(Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
		v=(Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
		if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
		else
		{
			if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
			else dst_y[k]=(uint8_t)y;
		}
		if (u<Min_U) dst_u[w0]=(uint8_t)Min_U;
		else
		{
			if (u>Max_U) dst_u[w0]=(uint8_t)Max_U;
			else dst_u[w0]=(uint8_t)u;
		}
		if (v<Min_V) dst_v[w0]=(uint8_t)Min_V;
		else
		{
			if (v>Max_V) dst_v[w0]=(uint8_t)Max_V;
			else dst_v[w0]=(uint8_t)v;
		}
		k++;

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			if (y<Min_Y) dst_y[k]=(uint8_t)Min_Y;
			else
			{
				if (y>Max_Y) dst_y[k]=(uint8_t)Max_Y;
				else dst_y[k]=(uint8_t)y;
			}
		}
	}
}


void JPSDR_RGBConvert::YV24toRGB32(const void *src_y_,const void *src_u_,const void *src_v_, void *dst_,int32_t w,int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,
		ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch)
{
	RGB32BMP *dst;
	const uint8_t *src_y,*src_u,*src_v;

	dst=(RGB32BMP *)dst_;
	src_y=(uint8_t *)src_y_;
	src_u=(uint8_t *)src_u_;
	src_v=(uint8_t *)src_v_;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,g,b;
			const uint16_t y=src_y[j],u=src_u[j],v=src_v[j];

			r=(lookup[y]+lookup[v+256]+Offset_R) >> 5;
			g=(lookup[y]+lookup[u+512]+lookup[v+768]+Offset_G) >> 5;
			b=(lookup[y]+lookup[u+1024]+Offset_B) >> 5;

			if (b<0) dst[j].b=0;
			else
			{
				if (b>255) dst[j].b=255;
				else dst[j].b=(uint8_t)b;
			}
			if (g<0) dst[j].g=0;
			else
			{
				if (g>255) dst[j].g=255;
				else dst[j].g=(uint8_t)g;
			}
			if (r<0) dst[j].r=0;
			else
			{
				if (r>255) dst[j].r=255;
				else dst[j].r=(uint8_t)r;
			}
			dst[j].alpha=255;
		}
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
		src_y+=src_pitch_y;
		src_u+=src_pitch_u;
		src_v+=src_pitch_v;
	}

}



void JPSDR_RGBConvert::Run()
{
	Image_Data idata;
	int32_t w,h;
	ptrdiff_t d_dst0,d_dst1,d_dst2;
	ptrdiff_t d_src0,d_src1,d_src2;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;

	const VDXPixmap& pxdst=*fa->dst.mpPixmap;
	const VDXPixmap& pxsrc=*fa->src.mpPixmap;

	idata.src_plane0=pxsrc.data;
	idata.src_plane1=pxsrc.data2;
	idata.src_plane2=pxsrc.data3;
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	switch(convertion_mode)
	{
		case 0 :
			switch(mData.output_mode)
			{
				case 0 :
					w=idata.src_w0;
					h=idata.src_h0;
					d_src0=0; d_src1=0; d_src2=0; 
					d_dst0=0; d_dst1=0; d_dst2=0;
					break;
				case 1 :
					h=idata.src_h0;
					d_dst1=0; d_dst2=0;
					d_src1=0; d_src2=0;
					if (idata.src_w0<idata.dst_w0)
					{
						w=idata.src_w0;
						d_src0=0;
						d_dst0=idata.dst_w0-idata.src_w0;
					}
					else
					{
						if (idata.src_w0>idata.dst_w0)
						{
							w=idata.dst_w0;
							d_dst0=0;
							d_src0=(idata.src_w0-idata.dst_w0) << 2;
						}
						else
						{
							w=idata.src_w0;
							d_dst0=0; d_src0=0;
						}
					}
					break;
				case 2 :
					h=idata.src_h0;
					d_dst1=0; d_dst2=0;
					d_src1=0; d_src2=0;
					if (idata.src_w0<idata.dst_w0)
					{
						w=idata.src_w0;
						d_src0=0;
						d_dst0=0;
					}
					else
					{
						if (idata.src_w0>idata.dst_w0)
						{
							w=idata.dst_w0;
							d_dst0=0;
							d_src0=(idata.src_w0-idata.dst_w0) << 2;
						}
						else
						{
							w=idata.src_w0;
							d_dst0=0; d_src0=0;
						}
					}
					break;
				case 3 :
					d_dst1=0; d_dst2=0;
					d_src1=0; d_src2=0;
					if (idata.src_w0<idata.dst_w0)
					{
						w=idata.src_w0;
						d_src0=0;
						d_dst0=idata.dst_w0-idata.src_w0;
					}
					else
					{
						if (idata.src_w0>idata.dst_w0)
						{
							w=idata.dst_w0;
							d_dst0=0;
							d_src0=(idata.src_w0-idata.dst_w0) << 2;
						}
						else
						{
							w=idata.src_w0;
							d_dst0=0; d_src0=0;
						}
					}
					if (idata.src_h0<idata.dst_h0) h=idata.src_h0;
					else h=idata.dst_h0;
					break;
				default :
					w=idata.src_w0;
					h=idata.src_h0;
					d_src0=0; d_src1=0; d_src2=0; 
					d_dst0=0; d_dst1=0; d_dst2=0;
					break;
			}
		case 3 :
			w=idata.src_w0;
			h=idata.src_h0;
			d_src0=0; d_src1=0; d_src2=0; 
			d_dst0=0; d_dst1=0; d_dst2=0;
			break;
		default :
			w=idata.src_w0;
			h=idata.src_h0;
			d_src0=0; d_src1=0; d_src2=0; 
			d_dst0=0; d_dst1=0; d_dst2=0;
			break;
	}

	if (SSE2_Enable)
	{
		switch(convertion_mode)
		{
			case 0 :
				switch(mData.output_mode)
				{
					case 0 :
						JPSDR_RGBConvert_RGB32toYV24_SSE(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,
							Offset_Y,Offset_U,Offset_V,lookup,idata.src_modulo0+d_src0,idata.dst_modulo0+d_dst0,idata.dst_modulo1+d_dst1,
							idata.dst_modulo2+d_dst2,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
						break;
					case 1 :
						JPSDR_RGBConvert_RGB32toYV16_SSE(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,
							Offset_Y,Offset_U,Offset_V,lookup,idata.src_modulo0+d_src0,idata.dst_modulo0+d_dst0,idata.dst_modulo1+d_dst1,
							idata.dst_modulo2+d_dst2,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
						break;
					case 2 :
						JPSDR_RGBConvert_RGB32toYUYV_SSE(idata.src_plane0,idata.dst_plane0,w,h,
							Offset_Y,Offset_U,Offset_V,lookup,idata.src_modulo0+d_src0,idata.dst_modulo0+d_dst0,
							Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
						break;
					case 3 :
						JPSDR_RGBConvert_RGB32toYV12_SSE(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,
								Offset_Y,Offset_U,Offset_V,lookup,idata.src_pitch0,idata.src_modulo0+d_src0,idata.dst_pitch0,idata.dst_modulo0+d_dst0,
								idata.dst_modulo1+d_dst1,idata.dst_modulo2+d_dst2,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
						break;
				}
				break;
			case 3 :
					JPSDR_RGBConvert_YV24toRGB32_SSE(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,w,h,
						Offset_R,Offset_G,Offset_B,lookup,idata.src_modulo0+d_src0,idata.src_modulo1+d_src1,idata.src_modulo2+d_src2,
						idata.dst_modulo0+d_dst0);
					break;
		}
	}
	else
	{
		switch(convertion_mode)
		{
			case 0 : 
				switch(mData.output_mode)
				{
					case 0 :
						RGB32toYV24(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,idata.src_pitch0,
							idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
						break;
					case 1 :
						RGB32toYV16(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,idata.src_pitch0,
							idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
						break;
					case 2 :
						RGB32toYUY2(idata.src_plane0,idata.dst_plane0,w,h,idata.src_pitch0,idata.dst_pitch0);
						break;
					case 3 :
						RGB32toYV12(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,idata.src_pitch0,
							idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
						break;
				}
				break;
			case 3 : YV24toRGB32(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,w,h,idata.src_pitch0,
						 idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0);
					break;
		}
	}
}



void JPSDR_RGBConvert::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.color_matrix=argv[0].asInt();
	mData.full_range=!!argv[1].asInt();
	mData.output_mode=argv[2].asInt();
	mData.auto_detect=!!argv[3].asInt();
}

bool JPSDR_RGBConvert::Configure(VDXHWND hwnd)
{
	JPSDR_RGBConvertDialog dlg(mData, fa->ifp2, this);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_RGBConvert::GetScriptString(char *buf, int maxlen)
{
 
  
	SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d)",mData.color_matrix,mData.full_range,mData.output_mode,mData.auto_detect);
}


extern VDXFilterDefinition filterDef_JPSDR_RGBConvert=
VDXVideoFilterDefinition<JPSDR_RGBConvert>("JPSDR","RGB Convert v1.4.0","RGB <-> YCbCr convertion with color matrix option.\n[ASM][SSE2] Optimised.");
