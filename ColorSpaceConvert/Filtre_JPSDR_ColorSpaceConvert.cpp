#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

#include "..\asmlib\asmlib.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

extern "C" int IInstrSet;

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
	bool mt_mode;

	JPSDR_ColorSpaceConvertData(void);
};



typedef struct _MT_Data_Info
{
	void *src1,*src2,*src3;
	void *dst1,*dst2,*dst3;
	ptrdiff_t src_pitch1,src_pitch2,src_pitch3;
	ptrdiff_t dst_pitch1,dst_pitch2,dst_pitch3;
	ptrdiff_t src_modulo1,src_modulo2,src_modulo3;
	ptrdiff_t dst_modulo1,dst_modulo2,dst_modulo3;
	int32_t src_Y_h_min,src_Y_h_max,src_Y_w,src_Y_w_32;
	int32_t src_UV_h_min,src_UV_h_max,src_UV_w,src_UV_w_32;
	int32_t dst_Y_h_min,dst_Y_h_max,dst_Y_w,dst_Y_w_32;
	int32_t dst_UV_h_min,dst_UV_h_max,dst_UV_w,dst_UV_w_32;
	bool top,bottom;
} MT_Data_Info;



JPSDR_ColorSpaceConvertData::JPSDR_ColorSpaceConvertData(void)
{
	color_matrix_in=0;
	color_matrix_out=1;
	full_range_in=false;
	full_range_out=false;
	mt_mode=true;
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
	virtual void End();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	void Compute_Lookup(void);
	
	VDXVF_DECLARE_SCRIPT_METHODS();

protected:
	Image_Data image_data;
	int16_t lookup_Cvt[2304],lookup_Upsc[256];
	int16_t Min_U_In,Max_U_In,Min_Y_In,Max_Y_In,Min_V_In,Max_V_In;
	int16_t Min_U_Out,Max_U_Out,Min_Y_Out,Max_Y_Out,Min_V_Out,Max_V_Out;
	int16_t Offset_Y,Offset_U,Offset_V;
	double Coeff_Y_In,Coeff_U_In,Coeff_V_In;
	double Coeff_Y_Out,Coeff_U_Out,Coeff_V_Out;
	uint8_t convertion_mode;
	bool SSE2_Enable;
	size_t CPU_Cache_Size,Cache_Setting;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits);

	inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);

	void ConvertYV24(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,const int32_t w,
		const int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void ConvertYV24_MT(uint8_t thread_num);

	void ConvertYV16(const void *src_y,const void *src_u,const void *src_v,void *dst_y,void *dst_u,void *dst_v,const int32_t w,
		const int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void ConvertYV16_MT(uint8_t thread_num);

	void ConvertYV12_Progressif_MT_1(uint8_t thread_num);
	void ConvertYV12_Progressif_MT_2(uint8_t thread_num);

	void ConvertYV12_Progressif_SSE2_MT_1(uint8_t thread_num);
	void ConvertYV12_Progressif_SSE2_MT_2(uint8_t thread_num);

	void ConvertYUY2(const void *src,void *dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void ConvertYUY2_MT(uint8_t thread_num);

	void ConvertUYVY(const void *src,void *dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void ConvertUYVY_MT(uint8_t thread_num);
		
	JPSDR_ColorSpaceConvertData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_ColorSpaceConvert)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_ColorSpaceConvert,ScriptConfig,"iiiii")
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

	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
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

	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);
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
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
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
		lookup_Upsc[i]=(uint16_t)(3*i);

	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	if (IInstrSet<0) InstructionSet();
	CPU_Cache_Size=DataCacheSize(0)>>2;

	for (i=0; i<MAX_MT_THREADS; i++)
	{
		MT_Thread[i].pClass=NULL;
		MT_Thread[i].f_process=0;
		MT_Thread[i].thread_Id=(uint8_t)i;
		MT_Thread[i].pFunc=NULL;
	}

	threads_number=1;
	threadpoolAllocated=false;
	UserId=0;

	return(true);
}


uint8_t JPSDR_ColorSpaceConvert::CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits)
{
	if ((max_threads<=1) || (max_threads>threads_number))
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].src_Y_h_min=0;
		MT_Data[0].dst_Y_h_min=0;
		MT_Data[0].src_Y_h_max=size_y;
		MT_Data[0].dst_Y_h_max=size_y;
		MT_Data[0].src_UV_h_min=0;
		MT_Data[0].dst_UV_h_min=0;
		switch (div_y)
		{
			case 0 : 
				MT_Data[0].src_UV_h_max=size_y;
				MT_Data[0].dst_UV_h_max=size_y;
				break;
			case 1 : 
				MT_Data[0].src_UV_h_max=size_y >> 1;
				MT_Data[0].dst_UV_h_max=size_y >> 1;
				break;
			case 2 : 
				MT_Data[0].src_UV_h_max=size_y >> 2;
				MT_Data[0].dst_UV_h_max=size_y >> 2;
				break;
			default :
				MT_Data[0].src_UV_h_max=size_y;
				MT_Data[0].dst_UV_h_max=size_y;
				break;
		}
		MT_Data[0].src_Y_w=size_x;
		MT_Data[0].dst_Y_w=size_x;
		switch (div_x)
		{
			case 0 :
				MT_Data[0].src_UV_w=size_x;
				MT_Data[0].dst_UV_w=size_x;
				break;
			case 1 :
				MT_Data[0].src_UV_w=size_x >> 1;
				MT_Data[0].dst_UV_w=size_x >> 1;
				break;
			case 2 :
				MT_Data[0].src_UV_w=size_x >> 2;
				MT_Data[0].dst_UV_w=size_x >> 2;
				break;
			default :
				MT_Data[0].src_UV_w=size_x;
				MT_Data[0].dst_UV_w=size_x;
				break;
		}
		switch (_32bits)
		{
			case 0 :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w;
				break;
			case 1 :
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w+1)>>1;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w+1)>>1;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w+1)>>1;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w+1)>>1;
				break;
			case 2 :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w>>2;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w>>2;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w>>2;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w>>2;
				break;
			default :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w;
				break;
		}
		return(1);
	}

	int32_t dh_Y,dh_UV,h_y;
	uint8_t i,max=0;

	dh_Y=(size_y+(int32_t)max_threads-1)/(int32_t)max_threads;
	if (dh_Y<16) dh_Y=16;
	if ((dh_Y & 3)!=0) dh_Y=((dh_Y+3) >> 2) << 2;

	h_y=0;
	while (h_y<(size_y-16))
	{
		max++;
		h_y+=dh_Y;
	}

	if (max==1)
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].src_Y_h_min=0;
		MT_Data[0].dst_Y_h_min=0;
		MT_Data[0].src_Y_h_max=size_y;
		MT_Data[0].dst_Y_h_max=size_y;
		MT_Data[0].src_UV_h_min=0;
		MT_Data[0].dst_UV_h_min=0;
		switch (div_y)
		{
			case 0 : 
				MT_Data[0].src_UV_h_max=size_y;
				MT_Data[0].dst_UV_h_max=size_y;
				break;
			case 1 : 
				MT_Data[0].src_UV_h_max=size_y >> 1;
				MT_Data[0].dst_UV_h_max=size_y >> 1;
				break;
			case 2 : 
				MT_Data[0].src_UV_h_max=size_y >> 2;
				MT_Data[0].dst_UV_h_max=size_y >> 2;
				break;
			default :
				MT_Data[0].src_UV_h_max=size_y;
				MT_Data[0].dst_UV_h_max=size_y;
				break;
		}
		MT_Data[0].src_Y_w=size_x;
		MT_Data[0].dst_Y_w=size_x;
		switch (div_x)
		{
			case 0 :
				MT_Data[0].src_UV_w=size_x;
				MT_Data[0].dst_UV_w=size_x;
				break;
			case 1 :
				MT_Data[0].src_UV_w=size_x >> 1;
				MT_Data[0].dst_UV_w=size_x >> 1;
				break;
			case 2 :
				MT_Data[0].src_UV_w=size_x >> 2;
				MT_Data[0].dst_UV_w=size_x >> 2;
				break;
			default :
				MT_Data[0].src_UV_w=size_x;
				MT_Data[0].dst_UV_w=size_x;
				break;
		}
		switch (_32bits)
		{
			case 0 :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w;
				break;
			case 1 :
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w+1)>>1;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w+1)>>1;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w+1)>>1;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w+1)>>1;
				break;
			case 2 :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w>>2;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w>>2;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w>>2;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w>>2;
				break;
			default :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w;
				break;
		}
		return(1);
	}

	switch (div_y)
	{
		case 0 : dh_UV=dh_Y; break;
		case 1 : dh_UV=dh_Y >> 1; break;
		case 2 : dh_UV=dh_Y >> 2; break;
		default : dh_UV=dh_Y;
	}

	MT_Data[0].top=true;
	MT_Data[0].bottom=false;
	MT_Data[0].src_Y_h_min=0;
	MT_Data[0].src_Y_h_max=dh_Y;
	MT_Data[0].dst_Y_h_min=0;
	MT_Data[0].dst_Y_h_max=dh_Y;
	MT_Data[0].src_UV_h_min=0;
	MT_Data[0].src_UV_h_max=dh_UV;
	MT_Data[0].dst_UV_h_min=0;
	MT_Data[0].dst_UV_h_max=dh_UV;

	i=1;
	while (i<max)
	{
		MT_Data[i].top=false;
		MT_Data[i].bottom=false;
		MT_Data[i].src_Y_h_min=MT_Data[i-1].src_Y_h_max;
		MT_Data[i].src_Y_h_max=MT_Data[i].src_Y_h_min+dh_Y;
		MT_Data[i].dst_Y_h_min=MT_Data[i-1].dst_Y_h_max;
		MT_Data[i].dst_Y_h_max=MT_Data[i].dst_Y_h_min+dh_Y;
		MT_Data[i].src_UV_h_min=MT_Data[i-1].src_UV_h_max;
		MT_Data[i].src_UV_h_max=MT_Data[i].src_UV_h_min+dh_UV;
		MT_Data[i].dst_UV_h_min=MT_Data[i-1].dst_UV_h_max;
		MT_Data[i].dst_UV_h_max=MT_Data[i].dst_UV_h_min+dh_UV;
		i++;
	}
	MT_Data[max-1].bottom=true;
	MT_Data[max-1].src_Y_h_max=size_y;
	MT_Data[max-1].dst_Y_h_max=size_y;
	switch (div_y)
	{
		case 0 :
			MT_Data[max-1].src_UV_h_max=size_y;
			MT_Data[max-1].dst_UV_h_max=size_y;
			break;
		case 1 :
			MT_Data[max-1].src_UV_h_max=size_y >> 1;
			MT_Data[max-1].dst_UV_h_max=size_y >> 1;
			break;
		case 2 :
			MT_Data[max-1].src_UV_h_max=size_y >> 2;
			MT_Data[max-1].dst_UV_h_max=size_y >> 2;
			break;
		default :
			MT_Data[max-1].src_UV_h_max=size_y;
			MT_Data[max-1].dst_UV_h_max=size_y;
			break;
	}
	for (i=0; i<max; i++)
	{
		MT_Data[i].src_Y_w=size_x;
		MT_Data[i].dst_Y_w=size_x;
		switch (div_x)
		{
			case 0 :
				MT_Data[i].src_UV_w=size_x;
				MT_Data[i].dst_UV_w=size_x;
				break;
			case 1 :
				MT_Data[i].src_UV_w=size_x >> 1;
				MT_Data[i].dst_UV_w=size_x >> 1;
				break;
			case 2 :
				MT_Data[i].src_UV_w=size_x >> 2;
				MT_Data[i].dst_UV_w=size_x >> 2;
				break;
			default :
				MT_Data[i].src_UV_w=size_x;
				MT_Data[i].dst_UV_w=size_x;
				break;
		}
		switch (_32bits)
		{
			case 0 :
				MT_Data[i].src_Y_w_32=MT_Data[i].src_Y_w;
				MT_Data[i].dst_Y_w_32=MT_Data[i].dst_Y_w;
				MT_Data[i].src_UV_w_32=MT_Data[i].src_UV_w;
				MT_Data[i].dst_UV_w_32=MT_Data[i].dst_UV_w;
				break;
			case 1 :
				MT_Data[i].src_Y_w_32=(MT_Data[i].src_Y_w+1)>>1;
				MT_Data[i].dst_Y_w_32=(MT_Data[i].dst_Y_w+1)>>1;
				MT_Data[i].src_UV_w_32=(MT_Data[i].src_UV_w+1)>>1;
				MT_Data[i].dst_UV_w_32=(MT_Data[i].dst_UV_w+1)>>1;
				break;
			case 2 :
				MT_Data[i].src_Y_w_32=MT_Data[i].src_Y_w>>2;
				MT_Data[i].dst_Y_w_32=MT_Data[i].dst_Y_w>>2;
				MT_Data[i].src_UV_w_32=MT_Data[i].src_UV_w>>2;
				MT_Data[i].dst_UV_w_32=MT_Data[i].dst_UV_w>>2;
				break;
			default :
				MT_Data[i].src_Y_w_32=MT_Data[i].src_Y_w;
				MT_Data[i].dst_Y_w_32=MT_Data[i].dst_Y_w;
				MT_Data[i].src_UV_w_32=MT_Data[i].src_UV_w;
				MT_Data[i].dst_UV_w_32=MT_Data[i].dst_UV_w;
				break;
		}
	}
	return(max);
}


inline void JPSDR_ColorSpaceConvert::Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src=(uint8_t *)src_;
	uint8_t *dst=(uint8_t *)dst_;

	if ((h==1) || ((src_pitch==dst_pitch) && (abs(src_pitch)==w)))
	{
		if (src_pitch<0)
		{
			src+=(h-1)*src_pitch;
			dst+=(h-1)*dst_pitch;
		}
		A_memcpy(dst,src,(size_t)h*(size_t)w);
	}
	else
	{
		for(int32_t i=0; i<h; i++)
		{
			A_memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
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
		lookup_Cvt[i]=(int16_t)round((i*x1*Coeff_Y_Out*64.0)/Coeff_Y_In);
		lookup_Cvt[i+256]=(int16_t)round((i*x2*Coeff_Y_Out*64.0)/Coeff_U_In);
		lookup_Cvt[i+512]=(int16_t)round((i*x3*Coeff_Y_Out*64.0)/Coeff_V_In);
		lookup_Cvt[i+768]=(int16_t)round((i*x4*Coeff_U_Out*64.0)/Coeff_Y_In);
		lookup_Cvt[i+1024]=(int16_t)round((i*x5*Coeff_U_Out*64.0)/Coeff_U_In);
		lookup_Cvt[i+1280]=(int16_t)round((i*x6*Coeff_U_Out*64.0)/Coeff_V_In);
		lookup_Cvt[i+1536]=(int16_t)round((i*x7*Coeff_V_Out*64.0)/Coeff_Y_In);
		lookup_Cvt[i+1792]=(int16_t)round((i*x8*Coeff_V_Out*64.0)/Coeff_U_In);
		lookup_Cvt[i+2048]=(int16_t)round((i*x9*Coeff_V_Out*64.0)/Coeff_V_In);
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

	if (!poolInterface->GetThreadPoolInterfaceStatus())
	{
		ff->Except("Error with the TheadPool status !");
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
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
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0;
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
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
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
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0;
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
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0;
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

	const size_t img_size=idata.dst_size0+idata.dst_size1+idata.dst_size2;

	if (img_size<=MAX_CACHE_SIZE)
	{
		if (CPU_Cache_Size>=img_size) Cache_Setting=img_size;
		else Cache_Setting=16;
	}
	else Cache_Setting=16;

	Compute_Lookup();

	if  ( mData.mt_mode && (idata.src_h0>=32) && (idata.dst_h0>=32) &&
		((mData.color_matrix_in!=mData.color_matrix_out) ||
		(mData.full_range_in!=mData.full_range_out)) )
	{
		threads_number=poolInterface->GetThreadNumber(0,true);
		if (threads_number==0)
		{
			ff->Except("Error with the TheadPool while getting CPU info !");
			return;
		}
	}
	else threads_number=1;

	switch (idata.src_video_mode)
	{
		case 0 :
		case 1 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,0,0,0);
			break;
		case 4 :
		case 9 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,0,0,2);
			break;
		case 2 :
		case 3 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,1,0,1);
			break;
		case 5 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,1,0,2);
			break;
		case 6 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,1,1,2);
			break;
		case 7 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,2,0,2);
			break;
		case 8 :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,2,2,2);
			break;
		default :
			threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,0,0,0);
			break;
	}

	if (threads_number>1)
	{
		StaticThreadpoolF=StaticThreadpool;

		for (uint8_t i=0; i<threads_number; i++)
		{
			MT_Thread[i].pClass=this;
			MT_Thread[i].f_process=0;
			MT_Thread[i].thread_Id=(uint8_t)i;
			MT_Thread[i].pFunc=StaticThreadpoolF;
		}
		if (!threadpoolAllocated)
			threadpoolAllocated=poolInterface->AllocateThreads(UserId,threads_number,0,0,true,false,0);
		if (!threadpoolAllocated)
		{			
			ff->Except("Error with the TheadPool while allocating threadpool !");
			return;
		}
	}

}


void JPSDR_ColorSpaceConvert::End()
{
	if (threadpoolAllocated)
	{
		poolInterface->DeAllocateThreads(UserId);
		UserId=0;
		threadpoolAllocated=false;
	}
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



void JPSDR_ColorSpaceConvert::ConvertYV24(const void *src_y_,const void *src_u_,const void *src_v_,void *dst_y_,void *dst_u_,void *dst_v_,const int32_t w,
		const int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const uint8_t *src_y,*src_u,*src_v;
	uint8_t *dst_y,*dst_u,*dst_v;

	const int16_t *lookup=lookup_Cvt;

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



void JPSDR_ColorSpaceConvert::ConvertYV24_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (SSE2_Enable)
		JPSDR_ColorSpaceConvert_YV24_SSE(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,
			mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			Offset_Y,Offset_U,Offset_V,lookup_Cvt,mt_data_inf.src_modulo1,mt_data_inf.src_modulo2,
			mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,
			Min_Y_Out,Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);				
	else
		ConvertYV24(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,
			mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1,mt_data_inf.src_pitch2,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch1,
			mt_data_inf.dst_pitch2,mt_data_inf.dst_pitch3);				
}



void JPSDR_ColorSpaceConvert::ConvertYV16(const void *src_y_,const void *src_u_,const void *src_v_,void *dst_y_,void *dst_u_,void *dst_v_,const int32_t w,
		const int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const uint8_t *src_y,*src_u,*src_v;
	uint8_t *dst_y,*dst_u,*dst_v;
	const int32_t w0=w-1;

	const int16_t *lookup=lookup_Cvt;

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


void JPSDR_ColorSpaceConvert::ConvertYV16_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (SSE2_Enable)
		JPSDR_ColorSpaceConvert_YV16_SSE(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,
			mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			Offset_Y,Offset_U,Offset_V,lookup_Cvt,mt_data_inf.src_modulo1,mt_data_inf.src_modulo2,
			mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,
			Min_Y_Out,Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);				
	else
		ConvertYV16(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,
			mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1,mt_data_inf.src_pitch2,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch1,
			mt_data_inf.dst_pitch2,mt_data_inf.dst_pitch3);				
}



void JPSDR_ColorSpaceConvert::ConvertYV12_Progressif_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_y_=mt_data_inf.src1;
	const void *src_u_=mt_data_inf.src2;
	const void *src_v_=mt_data_inf.src3;
	void *dst_y_=mt_data_inf.dst1;
	const int32_t w=mt_data_inf.src_UV_w;
	const int32_t h_UV_min=mt_data_inf.src_UV_h_min;
	const int32_t h_UV_max=mt_data_inf.src_UV_h_max;
	const ptrdiff_t src_pitch_y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_y=mt_data_inf.dst_pitch1;

	const int32_t h_1 = mt_data_inf.bottom ? h_UV_max-1:h_UV_max;
	const int32_t h_0 = mt_data_inf.top ? 1:h_UV_min;

	const uint8_t *src_Y,*src_U,*src_V,*src_Up,*src_Vp,*src_Un,*src_Vn;
	uint8_t *dst_Y;
	const int32_t w0=w-1;

	const int16_t *lookup=lookup_Cvt;
	const int16_t *lookup2=lookup_Upsc;

	dst_Y=(uint8_t *)dst_y_;

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;
	src_Up=src_U-src_pitch_u;
	src_Un=src_U+src_pitch_u;
	src_Vp=src_V-src_pitch_v;
	src_Vn=src_V+src_pitch_v;

	if (mt_data_inf.top)
	{
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
	}

	for(int32_t i=h_0; i<h_1; i++)
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

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_1; i<h_UV_max; i++)
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
		src_U=(uint8_t *)src_u_;
		src_V=(uint8_t *)src_v_;
	}

}


void JPSDR_ColorSpaceConvert::ConvertYV12_Progressif_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_y_=mt_data_inf.src1;
	const void *src_u_=mt_data_inf.src2;
	const void *src_v_=mt_data_inf.src3;
	void *dst_u_=mt_data_inf.dst2;
	void *dst_v_=mt_data_inf.dst3;
	const int32_t w=mt_data_inf.src_UV_w;
	const int32_t h_UV_min=mt_data_inf.src_UV_h_min;
	const int32_t h_UV_max=mt_data_inf.src_UV_h_max;
	const ptrdiff_t src_pitch_y2=mt_data_inf.src_pitch1 << 2;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_u=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_v=mt_data_inf.dst_pitch3;

	const uint8_t *src_Y,*src_Yn,*src_U,*src_V;
	uint8_t *dst_U,*dst_V;

	const int16_t *lookup=lookup_Cvt;

	dst_U=(uint8_t *)dst_u_;
	dst_V=(uint8_t *)dst_v_;

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;
	src_Yn=src_Y+mt_data_inf.src_pitch1;

	for(int32_t i=h_UV_min; i<h_UV_max; i++)
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

		src_Y+=src_pitch_y2;
		src_Yn+=src_pitch_y2;
		src_U+=src_pitch_u;
		src_V+=src_pitch_v;
		dst_U+=dst_pitch_u;
		dst_V+=dst_pitch_v;
	}

}


void JPSDR_ColorSpaceConvert::ConvertYV12_Progressif_SSE2_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_y_=mt_data_inf.src1;
	const void *src_u_=mt_data_inf.src2;
	const void *src_v_=mt_data_inf.src3;
	void *dst_y_=mt_data_inf.dst1;
	const int32_t w=mt_data_inf.src_UV_w;
	const int32_t h_UV_min=mt_data_inf.src_UV_h_min;
	const int32_t h_UV_max=mt_data_inf.src_UV_h_max;
	const ptrdiff_t src_pitch_y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_y=mt_data_inf.dst_pitch1;

	const int32_t h_1 = mt_data_inf.bottom ? h_UV_max-1:h_UV_max;
	const int32_t h_0 = mt_data_inf.top ? 1:h_UV_min;

	const uint8_t *src_Y,*src_U,*src_V,*src_Up,*src_Vp,*src_Un,*src_Vn;
	uint8_t *dst_Y;

	dst_Y=(uint8_t *)dst_y_;

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;
	src_Up=src_U-src_pitch_u;
	src_Un=src_U+src_pitch_u;
	src_Vp=src_V-src_pitch_v;
	src_Vn=src_V+src_pitch_v;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<1; i++)
		{
			JPSDR_ColorSpaceConvert_YV12_SSE2_1(src_Y,src_U,src_V,dst_Y,lookup_Cvt,w,Offset_Y,Min_Y_Out,Max_Y_Out);

			dst_Y+=dst_pitch_y;
			src_Y+=src_pitch_y;

			JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Un,src_V,src_Vn,dst_Y,lookup_Cvt,w,Offset_Y,Min_Y_Out,Max_Y_Out);

			dst_Y+=dst_pitch_y;
			src_Y+=src_pitch_y;

			src_U+=src_pitch_u;
			src_Up+=src_pitch_u;
			src_Un+=src_pitch_u;

			src_V+=src_pitch_v;
			src_Vp+=src_pitch_v;
			src_Vn+=src_pitch_v;
		}
	}


	for(int32_t i=h_0; i<h_1; i++)
	{

		JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Up,src_V,src_Vp,dst_Y,lookup_Cvt,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Un,src_V,src_Vn,dst_Y,lookup_Cvt,w,Offset_Y,Min_Y_Out,Max_Y_Out);

		dst_Y+=dst_pitch_y;
		src_Y+=src_pitch_y;

		src_U+=src_pitch_u;
		src_Up+=src_pitch_u;
		src_Un+=src_pitch_u;

		src_V+=src_pitch_v;
		src_Vp+=src_pitch_v;
		src_Vn+=src_pitch_v;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_1; i<h_UV_max; i++)
		{

			JPSDR_ColorSpaceConvert_YV12_SSE2_2(src_Y,src_U,src_Up,src_V,src_Vp,dst_Y,lookup_Cvt,w,Offset_Y,Min_Y_Out,Max_Y_Out);

			dst_Y+=dst_pitch_y;
			src_Y+=src_pitch_y;

			JPSDR_ColorSpaceConvert_YV12_SSE2_1(src_Y,src_U,src_V,dst_Y,lookup_Cvt,w,Offset_Y,Min_Y_Out,Max_Y_Out);

			dst_Y+=dst_pitch_y;
			src_Y+=src_pitch_y;

			src_U+=src_pitch_u;
			src_Up+=src_pitch_u;
			src_Un+=src_pitch_u;

			src_V+=src_pitch_v;
			src_Vp+=src_pitch_v;
			src_Vn+=src_pitch_v;
		}
	}
}



void JPSDR_ColorSpaceConvert::ConvertYV12_Progressif_SSE2_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_y_=mt_data_inf.src1;
	const void *src_u_=mt_data_inf.src2;
	const void *src_v_=mt_data_inf.src3;
	void *dst_u_=mt_data_inf.dst2;
	void *dst_v_=mt_data_inf.dst3;
	const int32_t w=mt_data_inf.src_UV_w;
	const int32_t h_UV_min=mt_data_inf.src_UV_h_min;
	const int32_t h_UV_max=mt_data_inf.src_UV_h_max;
	const ptrdiff_t src_pitch_y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_u=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_v=mt_data_inf.dst_pitch3;
	const ptrdiff_t src_modulo_y=mt_data_inf.src_modulo1;
	const ptrdiff_t src_modulo_u=mt_data_inf.src_modulo2;
	const ptrdiff_t src_modulo_v=mt_data_inf.src_modulo3;
	const ptrdiff_t dst_modulo_y=mt_data_inf.dst_modulo1;
	const ptrdiff_t dst_modulo_u=mt_data_inf.dst_modulo2;
	const ptrdiff_t dst_modulo_v=mt_data_inf.dst_modulo3;

	const uint8_t *src_Y,*src_U,*src_V;
	uint8_t *dst_U,*dst_V;

	dst_U=(uint8_t *)dst_u_;
	dst_V=(uint8_t *)dst_v_;

	src_Y=(uint8_t *)src_y_;
	src_U=(uint8_t *)src_u_;
	src_V=(uint8_t *)src_v_;

	JPSDR_ColorSpaceConvert_YV12_SSE2_3(src_Y,src_U,src_V,dst_U,dst_V,lookup_Cvt,w,h_UV_max-h_UV_min,
		src_pitch_y,src_modulo_y,src_modulo_u,src_modulo_v,dst_modulo_u,dst_modulo_v,Offset_U,Min_U_Out,
		Max_U_Out,Offset_V,Min_V_Out,Max_V_Out);
}


void JPSDR_ColorSpaceConvert::ConvertYUY2(const void *src_,void *dst_,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src;
	YUYV *dst;
	const int32_t w0=w-1;

	const int16_t *lookup=lookup_Cvt;

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


void JPSDR_ColorSpaceConvert::ConvertYUY2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (SSE2_Enable)
		JPSDR_ColorSpaceConvert_YUY2_SSE(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,Offset_Y,Offset_U,Offset_V,lookup_Cvt,
			mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,Min_Y_Out,Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);
	else
		ConvertYUY2(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
}


void JPSDR_ColorSpaceConvert::ConvertUYVY(const void *src_,void *dst_,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src;
	UYVY *dst;
	const int32_t w0=w-1;

	const int16_t *lookup=lookup_Cvt;

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


void JPSDR_ColorSpaceConvert::ConvertUYVY_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (SSE2_Enable)
		JPSDR_ColorSpaceConvert_UYVY_SSE(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,Offset_Y,Offset_U,Offset_V,lookup_Cvt,
			mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,Min_Y_Out,Max_Y_Out,Min_U_Out,Max_U_Out,Min_V_Out,Max_V_Out);
	else
		ConvertUYVY(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
}


void JPSDR_ColorSpaceConvert::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_ColorSpaceConvert *ptrClass=(JPSDR_ColorSpaceConvert *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->ConvertYUY2_MT(data->thread_Id); break;
		case 2 : ptrClass->ConvertUYVY_MT(data->thread_Id); break;
		case 3 : ptrClass->ConvertYV24_MT(data->thread_Id); break;
		case 4 : ptrClass->ConvertYV16_MT(data->thread_Id); break;
		case 5 : ptrClass->ConvertYV12_Progressif_MT_1(data->thread_Id); break;
		case 6 : ptrClass->ConvertYV12_Progressif_MT_2(data->thread_Id); break;
		case 7 : ptrClass->ConvertYV12_Progressif_SSE2_MT_1(data->thread_Id); break;
		case 8 : ptrClass->ConvertYV12_Progressif_SSE2_MT_2(data->thread_Id); break;
		default : ;
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

	SetMemcpyCacheLimit(Cache_Setting);
	SetMemsetCacheLimit(Cache_Setting);

	if ((mData.color_matrix_in==mData.color_matrix_out) && (mData.full_range_in==mData.full_range_out))
	{
		if (convertion_mode==4)
		{
			Move_Full(idata.src_plane0,idata.dst_plane0,idata.src_w0,idata.src_h0,
				idata.src_pitch0,idata.dst_pitch0);
			Move_Full(idata.src_plane1,idata.dst_plane1,idata.src_w1,idata.src_h1,
				idata.src_pitch1,idata.dst_pitch1);
			Move_Full(idata.src_plane2,idata.dst_plane2,idata.src_w2,idata.src_h2,
				idata.src_pitch2,idata.dst_pitch2);
		}

		return;
	}

	for (uint8_t i=0; i<threads_number; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)idata.src_plane0+(idata.src_pitch0*MT_Data[i].src_Y_h_min));
		MT_Data[i].src2=(void *)((uint8_t *)idata.src_plane1+(idata.src_pitch1*MT_Data[i].src_UV_h_min));
		MT_Data[i].src3=(void *)((uint8_t *)idata.src_plane2+(idata.src_pitch2*MT_Data[i].src_UV_h_min));
		MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0+(idata.dst_pitch0*MT_Data[i].dst_Y_h_min));
		MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1+(idata.dst_pitch1*MT_Data[i].dst_UV_h_min));
		MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2+(idata.dst_pitch2*MT_Data[i].dst_UV_h_min));
		MT_Data[i].src_pitch1=idata.src_pitch0;
		MT_Data[i].src_pitch2=idata.src_pitch1;
		MT_Data[i].src_pitch3=idata.src_pitch2;
		MT_Data[i].dst_pitch1=idata.dst_pitch0;
		MT_Data[i].dst_pitch2=idata.dst_pitch1;
		MT_Data[i].dst_pitch3=idata.dst_pitch2;
		MT_Data[i].src_modulo1=idata.src_modulo0;
		MT_Data[i].src_modulo2=idata.src_modulo1;
		MT_Data[i].src_modulo3=idata.src_modulo2;
		MT_Data[i].dst_modulo1=idata.dst_modulo0;
		MT_Data[i].dst_modulo2=idata.dst_modulo1;
		MT_Data[i].dst_modulo3=idata.dst_modulo2;
	}

	if (threads_number>1)
	{
		if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,0,false))
		{
			uint8_t f_proc=0,f_proc2=0;

			switch (convertion_mode)
			{
				case 0 : f_proc=1; break;
				case 1 : f_proc=2; break;
				case 2 : f_proc=3; break;
				case 3 : f_proc=4; break;
				case 4 :
					if (SSE2_Enable)
					{
						f_proc=7;
						f_proc2=8;
					}
					else
					{
						f_proc=5;
						f_proc2=6;
					}
					break;
				default : ;
			}

			if (f_proc<5)
			{
				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=f_proc;

				if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=0;
			}
			else
			{
				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=f_proc;

				if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=f_proc2;

				if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=0;
			}
			poolInterface->ReleaseThreadPool(UserId);
		}
	}
	else
	{
		switch (convertion_mode)
		{
			case 0 : ConvertYUY2_MT(0); break;
			case 1 : ConvertUYVY_MT(0); break;
			case 2 : ConvertYV24_MT(0); break;
			case 3 : ConvertYV16_MT(0); break;
			case 4 : 
				if (SSE2_Enable)
				{
					ConvertYV12_Progressif_SSE2_MT_1(0);
					ConvertYV12_Progressif_SSE2_MT_2(0);
				}
				else
				{
					ConvertYV12_Progressif_MT_1(0);
					ConvertYV12_Progressif_MT_2(0);
				}
				break;
			default : ;
		}
	}

}



void JPSDR_ColorSpaceConvert::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.color_matrix_in=argv[0].asInt();
	mData.color_matrix_out=argv[1].asInt();
	mData.full_range_in=!!argv[2].asInt();
	mData.full_range_out=!!argv[3].asInt();
	mData.mt_mode=!!argv[4].asInt();
}

bool JPSDR_ColorSpaceConvert::Configure(VDXHWND hwnd)
{
	JPSDR_ColorSpaceConvertDialog dlg(mData, fa->ifp2, this);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_ColorSpaceConvert::GetScriptString(char *buf, int maxlen)
{
 
  
    SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d, %d)",mData.color_matrix_in,mData.color_matrix_out,mData.full_range_in,
		mData.full_range_out,mData.mt_mode);
}


extern VDXFilterDefinition filterDef_JPSDR_ColorSpaceConvert=
VDXVideoFilterDefinition<JPSDR_ColorSpaceConvert>("JPSDR","ColorSpaceConvert v2.1.0","YCbCr color space convertion.\n[ASM][SSE2] Optimised.");
