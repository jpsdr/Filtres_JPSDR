#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

extern int g_VFVAPIVersion;

extern "C" void JPSDR_ColorSpaceConvert_Move8_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_ColorSpaceConvert_Move32_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);

extern "C" void JPSDR_ColorSpaceConvert_YV24_SSE(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,
	int32_t w,int32_t h, int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo_y,
	ptrdiff_t src_modulo_u,ptrdiff_t src_modulo_v,ptrdiff_t dst_modulo_y,ptrdiff_t dst_modulo_u,ptrdiff_t dst_modulo_v,
	int16_t Min_Y,int16_t Max_Y, int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_ColorSpaceConvert_YV16_SSE(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,
	int32_t w,int32_t h, int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo_y,
	ptrdiff_t src_modulo_u,ptrdiff_t src_modulo_v,ptrdiff_t dst_modulo_y,ptrdiff_t dst_modulo_u,ptrdiff_t dst_modulo_v,
	int16_t Min_Y,int16_t Max_Y, int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_ColorSpaceConvert_YUY2_SSE(const void *src,void *dst,int32_t w,int32_t h, int16_t offset_Y,int16_t offset_U,
	int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,ptrdiff_t dst_modulo, int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_ColorSpaceConvert_UYVY_SSE(const void *src,void *dst,int32_t w,int32_t h, int16_t offset_Y,int16_t offset_U,
	int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,ptrdiff_t dst_modulo, int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_ColorSpaceConvert_YV12_SSE2_1(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *lookup,
	int32_t w0,int16_t Offset_Y,int16_t Min_Y,int16_t Max_Y);
extern "C" void JPSDR_ColorSpaceConvert_YV12_SSE2_2(const void *src_Y,const void *src_U1,const void *src_U2,const void *src_V1,const void *src_V2,
	void *dst_Y, void *lookup, int32_t w0,int16_t Offset_Y,int16_t Min_Y,int16_t Max_Y);
extern "C" void JPSDR_ColorSpaceConvert_YV12_SSE2_3(const void *src_y,const void *src_u,const void *src_v,void *dst_u,void *dst_v,
	int16_t *lookup,int32_t w,int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_modulo_y,ptrdiff_t src_modulo_u,ptrdiff_t src_modulo_v,
	ptrdiff_t dst_modulo_u,ptrdiff_t dst_modulo_v,int16_t offset_U,int16_t Min_U,int16_t Max_U,int16_t offset_V,int16_t Min_V,int16_t Max_V);



class JPSDR_ColorSpaceConvertData
{
public:
	uint8_t color_matrix_in,color_matrix_out;
	bool full_range_in,full_range_out;

	JPSDR_ColorSpaceConvertData(void);
};


JPSDR_ColorSpaceConvertData::JPSDR_ColorSpaceConvertData(void)
{
	color_matrix_in=0;
	color_matrix_out=1;
	full_range_in=false;
	full_range_out=false;
}

class JPSDR_ColorSpaceConvert;


class JPSDR_ColorSpaceConvertDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_ColorSpaceConvertDialog(JPSDR_ColorSpaceConvertData& Data, IVDXFilterPreview2 *ifp, JPSDR_ColorSpaceConvert *filt) : mData(Data), mifp(ifp), filter(filt)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_COLORSPACECONVERT),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_ColorSpaceConvertData& mData;
	JPSDR_ColorSpaceConvertData moldData;
	IVDXFilterPreview2 *const mifp;
	JPSDR_ColorSpaceConvert *const filter;
};


class JPSDR_ColorSpaceConvert : public VDXVideoFilter
{
public:
	virtual bool Init();
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
	int16_t lookup[2304],lookup2[256];
	int16_t Min_U_In,Max_U_In,Min_Y_In,Max_Y_In,Min_V_In,Max_V_In;
	int16_t Min_U_Out,Max_U_Out,Min_Y_Out,Max_Y_Out,Min_V_Out,Max_V_Out;
	int16_t Offset_Y,Offset_U,Offset_V;
	double Coeff_Y_In,Coeff_U_In,Coeff_V_In;
	double Coeff_Y_Out,Coeff_U_Out,Coeff_V_Out;
	uint8_t convertion_mode;
	bool SSE2_Enable;

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
	void ConvertYV24(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void ConvertYV16(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void ConvertYV12_Progressif(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void ConvertYV12_Progressif_SSE2(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_modulo_y,ptrdiff_t src_pitch_u,ptrdiff_t src_modulo_u,ptrdiff_t src_pitch_v,
		ptrdiff_t src_modulo_v,ptrdiff_t dst_pitch_y,ptrdiff_t dst_pitch_u,ptrdiff_t dst_modulo_u,ptrdiff_t dst_pitch_v,ptrdiff_t dst_modulo_v);
	void ConvertYUY2(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void ConvertUYVY(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
		
	JPSDR_ColorSpaceConvertData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_ColorSpaceConvert)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_ColorSpaceConvert,ScriptConfig,"iiii")
VDXVF_END_SCRIPT_METHODS()


INT_PTR JPSDR_ColorSpaceConvertDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_ColorSpaceConvertDialog::OnInit()
{
	moldData=mData;

	switch (mData.color_matrix_in)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_BT709_IN,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_BT601_IN,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_SMPTE_240M_IN,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_FCC_IN,1); break;
	}
	switch (mData.color_matrix_out)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_BT709_OUT,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_BT601_OUT,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_SMPTE_240M_OUT,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_FCC_OUT,1); break;
	}

	CheckDlgButton(mhdlg,IDC_FULL_RANGE_IN,mData.full_range_in?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_FULL_RANGE_OUT,mData.full_range_out?BST_CHECKED:BST_UNCHECKED);

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}


void JPSDR_ColorSpaceConvertDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}

bool JPSDR_ColorSpaceConvertDialog::SaveToData()
{

	if (!!IsDlgButtonChecked(mhdlg,IDC_BT709_IN)) mData.color_matrix_in=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_BT601_IN)) mData.color_matrix_in=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_SMPTE_240M_IN)) mData.color_matrix_in=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_FCC_IN)) mData.color_matrix_in=3;
	if (!!IsDlgButtonChecked(mhdlg,IDC_BT709_OUT)) mData.color_matrix_out=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_BT601_OUT)) mData.color_matrix_out=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_SMPTE_240M_OUT)) mData.color_matrix_out=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_FCC_OUT)) mData.color_matrix_out=3;

	mData.full_range_in=!!IsDlgButtonChecked(mhdlg,IDC_FULL_RANGE_IN);
	mData.full_range_out=!!IsDlgButtonChecked(mhdlg,IDC_FULL_RANGE_OUT);

	return true;
}



bool JPSDR_ColorSpaceConvertDialog::OnCommand(int cmd)
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
		case IDC_BT709_IN :
		case IDC_BT601_IN :
		case IDC_SMPTE_240M_IN :
		case IDC_FCC_IN :
		case IDC_BT709_OUT :
		case IDC_BT601_OUT :
		case IDC_SMPTE_240M_OUT :
		case IDC_FCC_OUT :
		case IDC_FULL_RANGE_IN :
		case IDC_FULL_RANGE_OUT :
			if (mifp && SaveToData())
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}


bool JPSDR_ColorSpaceConvert::Init()
{
	uint16_t i;

	for (i=0; i<256; i++)
		lookup2[i]=(uint16_t)(3*i);

	return(true);
}


void JPSDR_ColorSpaceConvert::Compute_Lookup(void)
{
	double kr_in,kg_in,kb_in;
	double kr_out,kg_out,kb_out;
	double u1,u2,v1,v2,r1,g1,g2,b1;
	double x1,x2,x3,x4,x5,x6,x7,x8,x9;
	int16_t i;

	if (mData.full_range_in)
	{
		Min_Y_In=0;
		Max_Y_In=255;
		Min_U_In=0;
		Max_U_In=255;
		Min_V_In=0;
		Max_V_In=255;
		Coeff_Y_In=255.0;
		Coeff_U_In=255.0;
		Coeff_V_In=255.0;
	}
	else
	{
		Min_Y_In=16;
		Max_Y_In=235;
		Min_U_In=16;
		Max_U_In=240;
		Min_V_In=16;
		Max_V_In=240;
		Coeff_Y_In=219.0;
		Coeff_U_In=224.0;
		Coeff_V_In=224.0;
	}
	if (mData.full_range_out)
	{
		Min_Y_Out=0;
		Max_Y_Out=255;
		Min_U_Out=0;
		Max_U_Out=255;
		Min_V_Out=0;
		Max_V_Out=255;
		Coeff_Y_Out=255.0;
		Coeff_U_Out=128.0;
		Coeff_V_Out=128.0;
	}
	else
	{
		Min_Y_Out=16;
		Max_Y_Out=235;
		Min_U_Out=16;
		Max_U_Out=240;
		Min_V_Out=16;
		Max_V_Out=240;
		Coeff_Y_Out=219.0;
		Coeff_U_Out=112.0;
		Coeff_V_Out=112.0;
	}
	
	switch (mData.color_matrix_in)
	{
		case 0 : kr_in=0.2126; kb_in=0.0722; break;
		case 1 : kr_in=0.299; kb_in=0.114; break;
		case 2 : kr_in=0.212; kb_in=0.087; break;
		case 3 : kr_in=0.3; kb_in=0.11; break;
	}
	kg_in=1.0-kr_in-kb_in;
	switch (mData.color_matrix_out)
	{
		case 0 : kr_out=0.2126; kb_out=0.0722; break;
		case 1 : kr_out=0.299; kb_out=0.114; break;
		case 2 : kr_out=0.212; kb_out=0.087; break;
		case 3 : kr_out=0.3; kb_out=0.11; break;
	}
	kg_out=1.0-kr_out-kb_out;

	u1=-kr_out/(1.0-kb_out); u2=-kg_out/(1.0-kb_out);
	v1=-kg_out/(1.0-kr_out); v2=-kb_out/(1.0-kr_out);

	r1=2.0*(1.0-kr_in);
	g1=-(2.0*(1.0-kb_in)*kb_in)/kg_in; g2=-(2.0*(1.0-kr_in)*kr_in)/kg_in;
	b1=2.0*(1.0-kb_in);
	x1=1.0; x2=(g1*kg_out+kb_out*b1); x3=(kr_out*r1+kg_out*g2);
	x4=(u1+u2+1.0); x5=(u2*g1+b1); x6=(u1*r1+u2*g2);
	x7=(v1+v2+1.0); x8=(v1*g1+v2*b1); x9=(r1+v1*g2);


	Offset_Y=(int16_t)round(32.0+(Min_Y_Out*64.0)-Coeff_Y_Out*((x1*Min_Y_In*64.0/Coeff_Y_In)+(x2*128.0*64.0/Coeff_U_In)
		+(x3*128.0*64.0/Coeff_V_In)));
	Offset_U=(int16_t)round(32.0+(128.0*64.0)-Coeff_U_Out*((x4*Min_Y_In*64.0/Coeff_Y_In)+(x5*128.0*64.0/Coeff_U_In)
		+(x6*128.0*64.0/Coeff_V_In)));
	Offset_V=(int16_t)round(32.0+(128.0*64.0)-Coeff_V_Out*((x7*Min_Y_In*64.0/Coeff_Y_In)+(x8*128.0*64.0/Coeff_U_In)
		+(x9*128.0*64.0/Coeff_V_In)));

	for (i=0; i<=255; i++)
	{
		lookup[i]=(int16_t)round((i*x1*Coeff_Y_Out*64.0)/Coeff_Y_In);
		lookup[i+256]=(int16_t)round((i*x2*Coeff_Y_Out*64.0)/Coeff_U_In);
		lookup[i+512]=(int16_t)round((i*x3*Coeff_Y_Out*64.0)/Coeff_V_In);
		lookup[i+768]=(int16_t)round((i*x4*Coeff_U_Out*64.0)/Coeff_Y_In);
		lookup[i+1024]=(int16_t)round((i*x5*Coeff_U_Out*64.0)/Coeff_U_In);
		lookup[i+1280]=(int16_t)round((i*x6*Coeff_U_Out*64.0)/Coeff_V_In);
		lookup[i+1536]=(int16_t)round((i*x7*Coeff_V_Out*64.0)/Coeff_Y_In);
		lookup[i+1792]=(int16_t)round((i*x8*Coeff_V_Out*64.0)/Coeff_U_In);
		lookup[i+2048]=(int16_t)round((i*x9*Coeff_V_Out*64.0)/Coeff_V_In);
	}

}



uint32 JPSDR_ColorSpaceConvert::GetParams()
{
	if (g_VFVAPIVersion<12) return(FILTERPARAM_NOT_SUPPORTED);

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :
		case nsVDXPixmap::kPixFormat_XRGB8888 :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
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
		case nsVDXPixmap::kPixFormat_YUV422_YUYV : 
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR : 
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 : 
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR : 
			convertion_mode=0; break;
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
			convertion_mode=1; break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
			convertion_mode=2; break;
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
			convertion_mode=3; break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
			convertion_mode=4; break;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;
	
	switch(convertion_mode)
	{
		case 0 :
			if ((mData.color_matrix_out==0) || (mData.color_matrix_out==2))
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709;
			}
			else
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV;
			}
			break;
		case 1 :
			if ((mData.color_matrix_out==0) || (mData.color_matrix_out==2))
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_709;
			}
			else
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY;
			}
			break;
		case 2 :
			if ((mData.color_matrix_out==0) || (mData.color_matrix_out==2))
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_709;
			}
			else
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar;
			}
			break;
		case 3 :
			if ((mData.color_matrix_out==0) || (mData.color_matrix_out==2))
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709;
			}
			else
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar;
			}
			break;
		case 4 :
			if ((mData.color_matrix_out==0) || (mData.color_matrix_out==2))
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709;
			}
			else
			{
				if (mData.full_range_out) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_FR;
				else pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar;
			}
			break;
	}

	fa->dst.offset=fa->src.offset;

	if (convertion_mode==4)
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_PURE_TRANSFORM | FILTERPARAM_SWAP_BUFFERS);
	}
	else
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_PURE_TRANSFORM);
	}

}


void JPSDR_ColorSpaceConvert::Start()
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





void JPSDR_ColorSpaceConvert::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];

	switch(mData.color_matrix_in)
	{
		case 0 : strcpy_s(output_string,255," Input : BT.709"); break;
		case 1 : strcpy_s(output_string,255," Input : BT.601"); break;
		case 2 : strcpy_s(output_string,255," Input : SMPTE 240M"); break;
		case 3 : strcpy_s(output_string,255," Input : FCC"); break;
	}
	if (mData.full_range_in) strcat_s(output_string,255," Full Range YCbCr");
	switch(mData.color_matrix_out)
	{
		case 0 : strcat_s(output_string,255," Output : BT.709"); break;
		case 1 : strcat_s(output_string,255," Output : BT.601"); break;
		case 2 : strcat_s(output_string,255," Output : SMPTE 240M"); break;
		case 3 : strcat_s(output_string,255," Output : FCC"); break;
	}
	if (mData.full_range_out) strcat_s(output_string,255," Full Range YCbCr");
	SafePrintf(buf,maxlen,output_string);

}



void JPSDR_ColorSpaceConvert::ConvertYV24(const void *src_y_,const void *src_u_,const void *src_v_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const uint8_t *src_y,*src_u,*src_v;
	uint8_t *dst_y,*dst_u,*dst_v;

	src_y=(uint8_t *)src_y_;
	src_u=(uint8_t *)src_u_;
	src_v=(uint8_t *)src_v_;
	dst_y=(uint8_t *)dst_y_;
	dst_u=(uint8_t *)dst_u_;
	dst_v=(uint8_t *)dst_v_;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,u,v;
			const uint16_t y0=src_y[j],u0=src_u[j],v0=src_v[j];

			y=(Offset_Y+lookup[y0]+lookup[u0+256]+lookup[v0+512]) >> 6;
			u=(Offset_U+lookup[y0+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
			v=(Offset_V+lookup[y0+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
			if (y<Min_Y_Out) dst_y[j]=(uint8_t)Min_Y_Out;
			else
			{
				if (y>Max_Y_Out) dst_y[j]=(uint8_t)Max_Y_Out;
				else dst_y[j]=(uint8_t)y;
			}
			if (u<Min_U_Out) dst_u[j]=(uint8_t)Min_U_Out;
			else
			{
				if (u>Max_U_Out) dst_u[j]=(uint8_t)Max_U_Out;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V_Out) dst_v[j]=(uint8_t)Min_V_Out;
			else
			{
				if (v>Max_V_Out) dst_v[j]=(uint8_t)Max_V_Out;
				else dst_v[j]=(uint8_t)v;
			}
		}
		src_y+=src_pitch_y;
		src_u+=src_pitch_u;
		src_v+=src_pitch_v;
		dst_y+=dst_pitch_y;
		dst_u+=dst_pitch_u;
		dst_v+=dst_pitch_v;
	}
}




void JPSDR_ColorSpaceConvert::ConvertYV16(const void *src_y_,const void *src_u_,const void *src_v_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const uint8_t *src_y,*src_u,*src_v;
	uint8_t *dst_y,*dst_u,*dst_v;
	const int32_t w0=w-1;

	src_y=(uint8_t *)src_y_;
	src_u=(uint8_t *)src_u_;
	src_v=(uint8_t *)src_v_;
	dst_y=(uint8_t *)dst_y_;
	dst_u=(uint8_t *)dst_u_;
	dst_v=(uint8_t *)dst_v_;

	for (int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		uint16_t u0=*src_u,v0=*src_v;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2,u,v;
			const uint16_t y0_1=src_y[j2],y0_2=src_y[j2+1];
			const uint16_t u1=src_u[j+1],v1=src_v[j+1];

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			u=(Offset_U+lookup[y0_1+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
			v=(Offset_V+lookup[y0_1+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
			if (y1<Min_Y_Out) dst_y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_y[j2++]=(uint8_t)Max_Y_Out;
				else dst_y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_y[j2++]=(uint8_t)Max_Y_Out;
				else dst_y[j2++]=(uint8_t)y2;
			}
			if (u<Min_U_Out) dst_u[j]=(uint8_t)Min_U_Out;
			else
			{
				if (u>Max_U_Out) dst_u[j]=(uint8_t)Max_U_Out;
				else dst_u[j]=(uint8_t)u;
			}
			if (v<Min_V_Out) dst_v[j]=(uint8_t)Min_V_Out;
			else
			{
				if (v>Max_V_Out) dst_v[j]=(uint8_t)Max_V_Out;
				else dst_v[j]=(uint8_t)v;
			}
			u0=u1; v0=v1;
		}

		int16_t y1,y2,u,v;
		const uint16_t y0_1=src_y[j2],y0_2=src_y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		u=(Offset_U+lookup[y0_1+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
		v=(Offset_V+lookup[y0_1+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
		if (y1<Min_Y_Out) dst_y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_y[j2++]=(uint8_t)Max_Y_Out;
			else dst_y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_y[j2++]=(uint8_t)Max_Y_Out;
			else dst_y[j2++]=(uint8_t)y2;
		}
		if (u<Min_U_Out) dst_u[w0]=(uint8_t)Min_U_Out;
		else
		{
			if (u>Max_U_Out) dst_u[w0]=(uint8_t)Max_U_Out;
			else dst_u[w0]=(uint8_t)u;
		}
		if (v<Min_V_Out) dst_v[w0]=(uint8_t)Min_V_Out;
		else
		{
			if (v>Max_V_Out) dst_v[w0]=(uint8_t)Max_V_Out;
			else dst_v[w0]=(uint8_t)v;
		}

		src_y+=src_pitch_y;
		src_u+=src_pitch_u;
		src_v+=src_pitch_v;
		dst_y+=dst_pitch_y;
		dst_u+=dst_pitch_u;
		dst_v+=dst_pitch_v;
	}
}


void JPSDR_ColorSpaceConvert::ConvertYV12_Progressif(const void *src_y_,const void *src_u_,const void *src_v_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const uint8_t *src_Y,*src_Yn,*src_U,*src_V,*src_Up,*src_Vp,*src_Un,*src_Vn;
	uint8_t *dst_Y,*dst_U,*dst_V;
	const int32_t w0=w-1,h_1=h-1;

	dst_Y=(uint8_t *)dst_y_;
	dst_U=(uint8_t *)dst_u_;
	dst_V=(uint8_t *)dst_v_;

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;
	src_Up=src_U-src_pitch_u;
	src_Un=src_U+src_pitch_u;
	src_Vp=src_V-src_pitch_v;
	src_Vn=src_V+src_pitch_v;


	for(int32_t i=0; i<1; i++)
	{
		int32_t j2;
		int16_t y1,y2;
		uint16_t y0_1,y0_2,u0,v0;

		u0=*src_U;
		v0=*src_V;
		j2=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2;
			const uint16_t y0_1=src_Y[j2],y0_2=src_Y[j2+1];
			const uint16_t u1=src_U[j+1],v1=src_U[j+1];

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		y0_1=src_Y[j2];
		y0_2=src_Y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y2;
		}

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		u0=(lookup2[*src_U]+(uint16_t)src_Un[1]+2)>>2;
		v0=(lookup2[*src_V]+(uint16_t)src_Vn[1]+2)>>2;
		j2=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2;
			const uint16_t y0_1=src_Y[j2],y0_2=src_Y[j2+1];
			const uint16_t u1=(lookup2[src_U[j+1]]+(uint16_t)src_Un[j+1]+2)>>2;
			const uint16_t v1=(lookup2[src_V[j+1]]+(uint16_t)src_Vn[j+1]+2)>>2;

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		y0_1=src_Y[j2];
		y0_2=src_Y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y2;
		}

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}


	for(int32_t i=1; i<h_1; i++)
	{
		int32_t j2;
		int16_t y1,y2;
		uint16_t y0_1,y0_2,u0,v0;

		u0=(lookup2[*src_U]+(uint16_t)src_Up[1]+2)>>2;
		v0=(lookup2[*src_V]+(uint16_t)src_Vp[1]+2)>>2;
		j2=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2;
			const uint16_t y0_1=src_Y[j2],y0_2=src_Y[j2+1];
			const uint16_t u1=(lookup2[src_U[j+1]]+(uint16_t)src_Up[j+1]+2)>>2;
			const uint16_t v1=(lookup2[src_V[j+1]]+(uint16_t)src_Vp[j+1]+2)>>2;

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		y0_1=src_Y[j2];
		y0_2=src_Y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y2;
		}

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		u0=(lookup2[*src_U]+(uint16_t)src_Un[1]+2)>>2;
		v0=(lookup2[*src_V]+(uint16_t)src_Vn[1]+2)>>2;
		j2=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2;
			const uint16_t y0_1=src_Y[j2],y0_2=src_Y[j2+1];
			const uint16_t u1=(lookup2[src_U[j+1]]+(uint16_t)src_Un[j+1]+2)>>2;
			const uint16_t v1=(lookup2[src_V[j+1]]+(uint16_t)src_Vn[j+1]+2)>>2;

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		y0_1=src_Y[j2];
		y0_2=src_Y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y2;
		}

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}


	for(int32_t i=h_1; i<h; i++)
	{
		int32_t j2;
		int16_t y1,y2;
		uint16_t y0_1,y0_2,u0,v0;

		u0=(lookup2[*src_U]+(uint16_t)src_Up[1]+2)>>2;
		v0=(lookup2[*src_V]+(uint16_t)src_Vp[1]+2)>>2;
		j2=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2;
			const uint16_t y0_1=src_Y[j2],y0_2=src_Y[j2+1];
			const uint16_t u1=(lookup2[src_U[j+1]]+(uint16_t)src_Up[j+1]+2)>>2;
			const uint16_t v1=(lookup2[src_V[j+1]]+(uint16_t)src_Vp[j+1]+2)>>2;

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		y0_1=src_Y[j2];
		y0_2=src_Y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y2;
		}

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		u0=*src_U;
		v0=*src_V;
		j2=0;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2;
			const uint16_t y0_1=src_Y[j2],y0_2=src_Y[j2+1];
			const uint16_t u1=src_U[j+1],v1=src_V[j+1];

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y1;
			}
			if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
				else dst_Y[j2++]=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		y0_1=src_Y[j2];
		y0_2=src_Y[j2+1];

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		if (y1<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y1;
		}
		if (y2<Min_Y_Out) dst_Y[j2++]=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst_Y[j2++]=(uint8_t)Max_Y_Out;
			else dst_Y[j2++]=(uint8_t)y2;
		}

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}

	src_Y=(uint8_t *)src_y_;
	src_Yn=src_Y+src_pitch_y;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;

	for(int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w; j++)
		{
			int16_t u,v;
			const uint16_t y0=(src_Y[j2]+src_Yn[j2])>>1;
			const uint16 u0=src_U[j],v0=src_V[j];

			u=(Offset_U+lookup[y0+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
			v=(Offset_V+lookup[y0+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
			if (u<Min_U_Out) dst_U[j]=(uint8_t)Min_U_Out;
			else
			{
				if (u>Max_U_Out) dst_U[j]=(uint8_t)Max_U_Out;
				else dst_U[j]=(uint8_t)u;
			}
			if (v<Min_V_Out) dst_V[j]=(uint8_t)Min_V_Out;
			else
			{
				if (v>Max_V_Out) dst_V[j]=(uint8_t)Max_V_Out;
				else dst_V[j]=(uint8_t)v;
			}
			j2+=2;
		}

		src_Y+=src_pitch_y << 1;
		src_Yn+=src_pitch_y << 1;
		src_U+=src_pitch_u;
		src_V+=src_pitch_v;
		dst_U+=dst_pitch_u;
		dst_V+=dst_pitch_v;
	}

}


void JPSDR_ColorSpaceConvert::ConvertYV12_Progressif_SSE2(const void *src_y_,const void *src_u_,const void *src_v_,void *dst_y_,void *dst_u_,void *dst_v_,int32_t w,
		int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_modulo_y,ptrdiff_t src_pitch_u,ptrdiff_t src_modulo_u,ptrdiff_t src_pitch_v,
		ptrdiff_t src_modulo_v,ptrdiff_t dst_pitch_y,ptrdiff_t dst_pitch_u,ptrdiff_t dst_modulo_u,ptrdiff_t dst_pitch_v,ptrdiff_t dst_modulo_v)
{
	const uint8_t *src_Y,*src_U,*src_V,*src_Up,*src_Vp,*src_Un,*src_Vn;
	uint8_t *dst_Y,*dst_U,*dst_V;
	const int32_t h_1=h-1;

	dst_Y=(uint8_t *)dst_y_;
	dst_U=(uint8_t *)dst_u_;
	dst_V=(uint8_t *)dst_v_;

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;
	src_Up=src_U-src_pitch_u;
	src_Un=src_U+src_pitch_u;
	src_Vp=src_V-src_pitch_v;
	src_Vn=src_V+src_pitch_v;


	for(int32_t i=0; i<1; i++)
	{
		JPSDR_ColorSpaceConvert_YV12_SSE2_1(src_Y,src_U,src_V,dst_Y,lookup,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Un,src_V,src_Vn,dst_Y,lookup,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}


	for(int32_t i=1; i<h_1; i++)
	{

		JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Up,src_V,src_Vp,dst_Y,lookup,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Un,src_V,src_Vn,dst_Y,lookup,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}


	for(int32_t i=h_1; i<h; i++)
	{

		JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Up,src_V,src_Vp,dst_Y,lookup,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		JPSDR_ColorSpaceConvert_YV12_SSE2_1(src_Y,src_U,src_V,dst_Y,lookup,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;

	JPSDR_ColorSpaceConvert_YV12_SSE2_3(src_Y,src_U,src_V,dst_U,dst_V,lookup,w,h,src_pitch_y,src_modulo_y,
		src_modulo_u,src_modulo_v,dst_modulo_u,dst_modulo_v,Offset_U,Min_U_Out,Max_U_Out,Offset_V,Min_V_Out,Max_V_Out);
}


void JPSDR_ColorSpaceConvert::ConvertYUY2(const void *src_,void *dst_,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src;
	YUYV *dst;
	const int32_t w0=w-1;

	src=(YUYV *)src_;
	dst=(YUYV *)dst_;

	for (int32_t i=0; i<h; i++)
	{
		uint16_t u0=src->u,v0=src->v;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2,u,v;
			const uint16_t y0_1=src[j].y1,y0_2=src[j].y2;
			const uint16_t u1=src[j+1].u,v1=src[j+1].v;

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			u=(Offset_U+lookup[y0_1+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
			v=(Offset_V+lookup[y0_1+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
			if (y1<Min_Y_Out) dst[j].y1=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst[j].y1=(uint8_t)Max_Y_Out;
				else dst[j].y1=(uint8_t)y1;
			}
			if (u<Min_U_Out) dst[j].u=(uint8_t)Min_U_Out;
			else
			{
				if (u>Max_U_Out) dst[j].u=(uint8_t)Max_U_Out;
				else dst[j].u=(uint8_t)u;
			}
			if (y2<Min_Y_Out) dst[j].y2=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst[j].y2=(uint8_t)Max_Y_Out;
				else dst[j].y2=(uint8_t)y2;
			}
			if (v<Min_V_Out) dst[j].v=(uint8_t)Min_V_Out;
			else
			{
				if (v>Max_V_Out) dst[j].v=(uint8_t)Max_V_Out;
				else dst[j].v=(uint8_t)v;
			}
			u0=u1; v0=v1;
		}

		int16_t y1,y2,u,v;
		const uint16_t y0_1=src[w0].y1,y0_2=src[w0].y2;

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		u=(Offset_U+lookup[y0_1+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
		v=(Offset_V+lookup[y0_1+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
		if (y1<Min_Y_Out) dst[w0].y1=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst[w0].y1=(uint8_t)Max_Y_Out;
			else dst[w0].y1=(uint8_t)y1;
		}
		if (u<Min_U_Out) dst[w0].u=(uint8_t)Min_U_Out;
		else
		{
			if (u>Max_U_Out) dst[w0].u=(uint8_t)Max_U_Out;
			else dst[w0].u=(uint8_t)u;
		}
		if (y2<Min_Y_Out) dst[w0].y2=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst[w0].y2=(uint8_t)Max_Y_Out;
			else dst[w0].y2=(uint8_t)y2;
		}
		if (v<Min_V_Out) dst[w0].v=(uint8_t)Min_V_Out;
		else
		{
			if (v>Max_V_Out) dst[w0].v=(uint8_t)Max_V_Out;
			else dst[w0].v=(uint8_t)v;
		}

		src=(YUYV *)((uint8_t *)src + src_pitch);
		dst=(YUYV *)((uint8_t *)dst + dst_pitch);
	}
}


void JPSDR_ColorSpaceConvert::ConvertUYVY(const void *src_,void *dst_,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src;
	UYVY *dst;
	const int32_t w0=w-1;

	src=(UYVY *)src_;
	dst=(UYVY *)dst_;

	for (int32_t i=0; i<h; i++)
	{
		uint16_t u0=src->u,v0=src->v;

		for (int32_t j=0; j<w0; j++)
		{
			int16_t y1,y2,u,v;
			const uint16_t y0_1=src[j].y1,y0_2=src[j].y2;
			const uint16_t u1=src[j+1].u,v1=src[j+1].v;

			y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
			y2=(Offset_Y+lookup[y0_2]+lookup[((u0+u1)>>1)+256]+lookup[((v0+v1)>>1)+512]) >> 6;
			u=(Offset_U+lookup[y0_1+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
			v=(Offset_V+lookup[y0_1+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
			if (u<Min_U_Out) dst[j].u=(uint8_t)Min_U_Out;
			else
			{
				if (u>Max_U_Out) dst[j].u=(uint8_t)Max_U_Out;
				else dst[j].u=(uint8_t)u;
			}
			if (y1<Min_Y_Out) dst[j].y1=(uint8_t)Min_Y_Out;
			else
			{
				if (y1>Max_Y_Out) dst[j].y1=(uint8_t)Max_Y_Out;
				else dst[j].y1=(uint8_t)y1;
			}
			if (v<Min_V_Out) dst[j].v=(uint8_t)Min_V_Out;
			else
			{
				if (v>Max_V_Out) dst[j].v=(uint8_t)Max_V_Out;
				else dst[j].v=(uint8_t)v;
			}
			if (y2<Min_Y_Out) dst[j].y2=(uint8_t)Min_Y_Out;
			else
			{
				if (y2>Max_Y_Out) dst[j].y2=(uint8_t)Max_Y_Out;
				else dst[j].y2=(uint8_t)y2;
			}
			u0=u1; v0=v1;
		}

		int16_t y1,y2,u,v;
		const uint16_t y0_1=src[w0].y1,y0_2=src[w0].y2;

		y1=(Offset_Y+lookup[y0_1]+lookup[u0+256]+lookup[v0+512]) >> 6;
		y2=(Offset_Y+lookup[y0_2]+lookup[u0+256]+lookup[v0+512]) >> 6;
		u=(Offset_U+lookup[y0_1+768]+lookup[u0+1024]+lookup[v0+1280]) >> 6;
		v=(Offset_V+lookup[y0_1+1536]+lookup[u0+1792]+lookup[v0+2048]) >> 6;
		if (u<Min_U_Out) dst[w0].u=(uint8_t)Min_U_Out;
		else
		{
			if (u>Max_U_Out) dst[w0].u=(uint8_t)Max_U_Out;
			else dst[w0].u=(uint8_t)u;
		}
		if (y1<Min_Y_Out) dst[w0].y1=(uint8_t)Min_Y_Out;
		else
		{
			if (y1>Max_Y_Out) dst[w0].y1=(uint8_t)Max_Y_Out;
			else dst[w0].y1=(uint8_t)y1;
		}
		if (v<Min_V_Out) dst[w0].v=(uint8_t)Min_V_Out;
		else
		{
			if (v>Max_V_Out) dst[w0].v=(uint8_t)Max_V_Out;
			else dst[w0].v=(uint8_t)v;
		}
		if (y2<Min_Y_Out) dst[w0].y2=(uint8_t)Min_Y_Out;
		else
		{
			if (y2>Max_Y_Out) dst[w0].y2=(uint8_t)Max_Y_Out;
			else dst[w0].y2=(uint8_t)y2;
		}

		src=(UYVY *)((uint8_t *)src + src_pitch);
		dst=(UYVY *)((uint8_t *)dst + dst_pitch);
	}
}


void JPSDR_ColorSpaceConvert::Run()
{
	Image_Data idata;

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

	if ((mData.color_matrix_in==mData.color_matrix_out) && (mData.full_range_in==mData.full_range_out))
	{
		if (convertion_mode==4)
		{
			if ((idata.src_w0&0x03)==0)
				JPSDR_ColorSpaceConvert_Move32_Full(idata.src_plane0,idata.dst_plane0,idata.src_w0>>2,idata.src_h0,
					idata.src_modulo0,idata.dst_modulo0);
			else
				JPSDR_ColorSpaceConvert_Move8_Full(idata.src_plane0,idata.dst_plane0,idata.src_w0,idata.src_h0,
					idata.src_modulo0,idata.dst_modulo0);
			if ((idata.src_w1&0x03)==0)
				JPSDR_ColorSpaceConvert_Move32_Full(idata.src_plane1,idata.dst_plane1,idata.src_w1>>2,idata.src_h1,
					idata.src_modulo1,idata.dst_modulo1);
			else
				JPSDR_ColorSpaceConvert_Move8_Full(idata.src_plane1,idata.dst_plane1,idata.src_w1,idata.src_h1,
					idata.src_modulo1,idata.dst_modulo1);
			if ((idata.src_w2&0x03)==0)
				JPSDR_ColorSpaceConvert_Move32_Full(idata.src_plane2,idata.dst_plane2,idata.src_w2>>2,idata.src_h2,
					idata.src_modulo2,idata.dst_modulo2);
			else
				JPSDR_ColorSpaceConvert_Move8_Full(idata.src_plane2,idata.dst_plane2,idata.src_w2,idata.src_h2,
					idata.src_modulo2,idata.dst_modulo2);
		}
		else return;
	}

	if (SSE2_Enable)
	{
		switch (convertion_mode)
		{
			case 0 :
					JPSDR_ColorSpaceConvert_YUY2_SSE(idata.src_plane0,idata.dst_plane0,(idata.src_w0+1)>>1,idata.src_h0,Offset_Y,Offset_U,Offset_V,
						lookup,idata.src_modulo0,idata.dst_modulo0,Min_Y_Out,Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);
					break;
			case 1 : 
					JPSDR_ColorSpaceConvert_UYVY_SSE(idata.src_plane0,idata.dst_plane0,(idata.src_w0+1)>>1,idata.src_h0,Offset_Y,Offset_U,Offset_V,
						lookup,idata.src_modulo0,idata.dst_modulo0,Min_Y_Out,Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);
					break;
			case 2 :
					JPSDR_ColorSpaceConvert_YV24_SSE(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,idata.src_w0,idata.src_h0,Offset_Y,
						Offset_U,Offset_V,lookup,idata.src_modulo0,idata.src_modulo1,idata.src_modulo2,idata.dst_modulo0,idata.dst_modulo1,idata.dst_modulo2,Min_Y_Out,
						Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);				
					break;
			case 3 :
					JPSDR_ColorSpaceConvert_YV16_SSE(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,(idata.src_w0+1)>>1,idata.src_h0,Offset_Y,
						Offset_U,Offset_V,lookup,idata.src_modulo0,idata.src_modulo1,idata.src_modulo2,idata.dst_modulo0,idata.dst_modulo1,idata.dst_modulo2,Min_Y_Out,
						Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);				
					break;
			case 4 : ConvertYV12_Progressif_SSE2(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,(idata.src_w0+1)>>1,(idata.src_h0+1)>>1,
						idata.src_pitch0,idata.src_modulo0,idata.src_pitch1,idata.src_modulo1,idata.src_pitch2,idata.src_modulo2,idata.dst_pitch0,idata.dst_pitch1,idata.dst_modulo1,
						idata.dst_pitch2,idata.dst_modulo2);
					break;
		}
	}
	else
	{
		switch (convertion_mode)
		{
			case 0 : ConvertYUY2(idata.src_plane0,idata.dst_plane0,(idata.src_w0+1)>>1,idata.src_h0,idata.src_pitch0,idata.dst_pitch0);
					break;
			case 1 : ConvertUYVY(idata.src_plane0,idata.dst_plane0,(idata.src_w0+1)>>1,idata.src_h0,idata.src_pitch0,idata.dst_pitch0);
					break;
			case 2 : ConvertYV24(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,idata.src_w0,idata.src_h0,idata.src_pitch0,
						idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
					break;
			case 3 : ConvertYV16(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,(idata.src_w0+1)>>1,idata.src_h0,idata.src_pitch0,
						idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
					break;
			case 4 : ConvertYV12_Progressif(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,(idata.src_w0+1)>>1,(idata.src_h0+1)>>1,
						idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
					break;
		}
	}

}



void JPSDR_ColorSpaceConvert::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.color_matrix_in=argv[0].asInt();
	mData.color_matrix_out=argv[1].asInt();
	mData.full_range_in=!!argv[2].asInt();
	mData.full_range_out=!!argv[3].asInt();
}

bool JPSDR_ColorSpaceConvert::Configure(VDXHWND hwnd)
{
	JPSDR_ColorSpaceConvertDialog dlg(mData, fa->ifp2, this);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_ColorSpaceConvert::GetScriptString(char *buf, int maxlen)
{
 
  
    SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d)",mData.color_matrix_in,mData.color_matrix_out,mData.full_range_in,
		mData.full_range_out);
}


extern VDXFilterDefinition filterDef_JPSDR_ColorSpaceConvert=
VDXVideoFilterDefinition<JPSDR_ColorSpaceConvert>("JPSDR","Color Space Convert v1.4.0","YCbCr color space convertion.\n[ASM][SSE2] Optimised.");
