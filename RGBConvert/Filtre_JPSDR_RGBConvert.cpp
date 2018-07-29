#define NOMINMAX
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "RGBConvert_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Pascal.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

#if _MSC_VER >= 1900
#define AVX2_BUILD_POSSIBLE
#endif


extern "C" void JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2(const void *scr,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX(const void *scr,void *dst,int32_t w);

extern "C" void JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar444_16_SSE2(const void *scr,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar444_16_AVX(const void *scr,void *dst,int32_t w);

extern "C" void JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8(const void *src,void *dst,int32_t w, int32_t h,ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2(const void *src,void *dst,int32_t w16, int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX(const void *src,void *dst,int32_t w16, int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2(const void *src1,const void *src2,void *dst,int32_t w16, int32_t h,
	ptrdiff_t src_pitch2,ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX(const void *src1,const void *src2,void *dst,int32_t w16, int32_t h,
	ptrdiff_t src_pitch2,ptrdiff_t dst_pitch);

extern "C" void JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16(const void *src,void *dst,int32_t w, int32_t h,ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16_SSE41(const void *src,void *dst,int32_t w16, int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16_AVX(const void *src,void *dst,int32_t w16, int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_SSE2(const void *src1,const void *src2,void *dst,int32_t w16, int32_t h,
	ptrdiff_t src_pitch2,ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX(const void *src1,const void *src2,void *dst,int32_t w16, int32_t h,
	ptrdiff_t src_pitch2,ptrdiff_t dst_pitch);


#ifdef AVX2_BUILD_POSSIBLE
extern "C" void JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(const void *scr_1,const void *src_2,void *dst,int32_t w);

extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2(const void *src1,const void *src2,void *dst,int32_t w32, int32_t h,
	ptrdiff_t src_pitch2,ptrdiff_t dst_pitch);
extern "C" void JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2(const void *src1,const void *src2,void *dst,int32_t w16, int32_t h,
	ptrdiff_t src_pitch2,ptrdiff_t dst_pitch);
#endif

extern "C" void JPSDR_RGBConvert_RGB32toYUYV_SSE2(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,const int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,
	int16_t Max_V);
extern "C" void JPSDR_RGBConvert_RGB32toYUYV_AVX(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,const int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,
	int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB64toYU64_SSE41(const void *src,void *dst,int32_t w,int32_t h,
	int32_t offset_Y,int32_t offset_U,int32_t offset_V,const int32_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,uint16_t Min_Y,uint16_t Max_Y,uint16_t Min_U,uint16_t Max_U,uint16_t Min_V,
	uint16_t Max_V);
extern "C" void JPSDR_RGBConvert_RGB64toYU64_AVX(const void *src,void *dst,int32_t w,int32_t h,
	int32_t offset_Y,int32_t offset_U,int32_t offset_V,const int32_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,uint16_t Min_Y,uint16_t Max_Y,uint16_t Min_U,uint16_t Max_U,uint16_t Min_V,
	uint16_t Max_V);

extern "C" void JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int16_t offset_R,int16_t offset_G,int16_t offset_B,const int16_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_YV24toRGB32_AVX(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int16_t offset_R,int16_t offset_G,int16_t offset_B,const int16_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);

extern "C" void JPSDR_RGBConvert_Convert_10_YV24toRGB64_SSE41(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_12_YV24toRGB64_SSE41(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_14_YV24toRGB64_SSE41(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_16_YV24toRGB64_SSE41(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);

extern "C" void JPSDR_RGBConvert_Convert_10_YV24toRGB64_AVX(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_12_YV24toRGB64_AVX(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_14_YV24toRGB64_AVX(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);
extern "C" void JPSDR_RGBConvert_Convert_16_YV24toRGB64_AVX(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int32_t offset_R,int32_t offset_G,int32_t offset_B,const int32_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);

extern "C" void JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,const int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);
extern "C" void JPSDR_RGBConvert_Convert_RGB32toYV24_AVX(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,const int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_Convert_RGB64toYV24_SSE41(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int32_t offset_Y,int32_t offset_U,int32_t offset_V,const int32_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,uint16_t Min_Y,uint16_t Max_Y,
	uint16_t Min_U,uint16_t Max_U,uint16_t Min_V,uint16_t Max_V);
extern "C" void JPSDR_RGBConvert_Convert_RGB64toYV24_AVX(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int32_t offset_Y,int32_t offset_U,int32_t offset_V,const int32_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,uint16_t Min_Y,uint16_t Max_Y,
	uint16_t Min_U,uint16_t Max_U,uint16_t Min_V,uint16_t Max_V);


typedef struct _dataLookUp
{
	uint16_t Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V;
	int32_t Offset_Y,Offset_U,Offset_V,Offset_R,Offset_G,Offset_B;
	double Coeff_Y,Coeff_U,Coeff_V;
} dataLookUp;

typedef struct _MT_Data_Info
{
	void *src1,*src2,*src3;
	void *dst1,*dst2,*dst3;
	ptrdiff_t src_pitch1,src_pitch2,src_pitch3;
	ptrdiff_t dst_pitch1,dst_pitch2,dst_pitch3;
	ptrdiff_t src_modulo1,src_modulo2,src_modulo3;
	ptrdiff_t dst_modulo1,dst_modulo2,dst_modulo3;
	int32_t src_Y_h_min,src_Y_h_max,src_Y_w;
	int32_t src_UV_h_min,src_UV_h_max,src_UV_w;
	int32_t dst_Y_h_min,dst_Y_h_max,dst_Y_w;
	int32_t dst_UV_h_min,dst_UV_h_max,dst_UV_w;
	bool top,bottom;
	bool src_w,src_h;
} MT_Data_Info;


class JPSDR_RGBConvertData
{
public:
	uint8_t color_matrix,output_mode;
	bool full_range,auto_detect;
	bool mt_mode;

	JPSDR_RGBConvertData(void);
};


JPSDR_RGBConvertData::JPSDR_RGBConvertData(void)
{
	color_matrix=1;
	output_mode=0;
	full_range=false;
	auto_detect=false;
	mt_mode=true;
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


class JPSDR_RGBConvert : public JPSDRVDXVideoFilter
{
public:
	JPSDR_RGBConvert(){}
	JPSDR_RGBConvert(const JPSDR_RGBConvert& a)
	{
		AVX2_Enable=a.AVX2_Enable;
		AVX_Enable=a.AVX_Enable;
		SSE41_Enable=a.SSE41_Enable;
		SSE2_Enable=a.SSE2_Enable;
		mData=a.mData;
		InternalInit();
	}
	virtual ~JPSDR_RGBConvert();

	virtual bool Init();
	virtual uint32 GetParams();
	virtual void Start();
	virtual void End();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	void Compute_Lookup8(void);
	void Compute_Lookup16(void);
	
	VDXVF_DECLARE_SCRIPT_METHODS();

	uint8_t bits_per_pixel;

protected:
	Image_Data image_data;
	int16_t Tlookup[9*256];
	uint16_t lookup_Upscale8[256];
	int32_t Tlookup16[9*65536];
	uint32_t lookup_Upscale16[65536];
	uint8_t convertion_mode;
	uint16_t Min_U,Max_U,Min_Y,Max_Y,Min_V,Max_V;
	int32_t Offset_Y,Offset_U,Offset_V,Offset_R,Offset_G,Offset_B;
	double Coeff_Y,Coeff_U,Coeff_V;
	uint8_t *BufferU_444,*BufferV_444,*BufferU_422,*BufferV_422;
	ptrdiff_t pitch444,pitch422,modulo444,modulo422;
	bool SSE2_Enable,SSE41_Enable,AVX_Enable,AVX2_Enable,VDub2_Enable;

	dataLookUp dl;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[3][MAX_MT_THREADS];
	uint8_t total_cpu,threads_number[3],max_threads;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_RGBConvertData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_RGBConvert)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_RGBConvert,ScriptConfig,"iiiii")
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
		case 4 : CheckDlgButton(mhdlg,IDC_BT2020,1); break;
	}

	switch (mData.output_mode)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_OUTPUT_YV24,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_OUTPUT_YV16,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_OUTPUT_YUY2,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_OUTPUT_YV12,1); break;
	}

	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
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
	if (!!IsDlgButtonChecked(mhdlg,IDC_BT2020)) mData.color_matrix=4;

	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YV24)) mData.output_mode=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YV16)) mData.output_mode=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YUY2)) mData.output_mode=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_YV12)) mData.output_mode=3;

	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);
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
		case IDC_BT2020 :
		case IDC_FULL_RANGE :
		case IDC_AUTODETECT :
			if (mifp && SaveToData())
			{
				if (filter->bits_per_pixel==8) filter->Compute_Lookup8();
				else filter->Compute_Lookup16();
				mifp->RedoFrame();
			}
			return true;
		case IDC_OUTPUT_YV24 :
		case IDC_OUTPUT_YV16 :
		case IDC_OUTPUT_YUY2 :
		case IDC_OUTPUT_YV12 :
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}


bool JPSDR_RGBConvert::Init()
{
	AVX2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX2)!=0);
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
	SSE41_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE41)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	InternalInit();

	return(true);
}


void JPSDR_RGBConvert::InternalInit(void)
{
	StaticThreadpoolF=StaticThreadpool;

	for (uint16_t i=0; i<MAX_MT_THREADS; i++)
	{
		MT_Thread[i].pClass=this;
		MT_Thread[i].f_process=0;
		MT_Thread[i].thread_Id=(uint8_t)i;
		MT_Thread[i].pFunc=StaticThreadpoolF;
	}
	BufferU_444=NULL;
	BufferV_444=NULL;
	BufferU_422=NULL;
	BufferV_422=NULL;

	UserId=0;
	if (poolInterface->GetThreadPoolInterfaceStatus())
	{
		total_cpu=poolInterface->GetThreadNumber(0,true);

		if (total_cpu>1)
			threadpoolAllocated=poolInterface->AllocateThreads(total_cpu,0,0,true,false,true,-1);
		else threadpoolAllocated=false;
		if (threadpoolAllocated) poolInterface->GetUserId(UserId);
	}
	else
	{
		total_cpu=0;
		threadpoolAllocated=false;
	}
}


JPSDR_RGBConvert::~JPSDR_RGBConvert()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
}


uint8_t CreateMTData(MT_Data_Info MT_Data[],uint8_t max_threads,uint8_t threads_number,int32_t size_x,int32_t size_y,
	bool div_src_w_UV,bool div_src_h_UV,bool div_dst_w_UV,bool div_dst_h_UV)
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
		MT_Data[0].src_UV_h_max=div_src_h_UV ? size_y >> 1:size_y;
		MT_Data[0].dst_UV_h_max=div_dst_h_UV ? size_y >> 1:size_y;
		MT_Data[0].src_Y_w=size_x;
		MT_Data[0].dst_Y_w=size_x;
		MT_Data[0].src_UV_w=div_src_w_UV ? size_x >> 1:size_x;
		MT_Data[0].dst_UV_w=div_dst_w_UV ? size_x >> 1:size_x;
		return(1);
	}

	int32_t dh_Y,src_dh_UV,dst_dh_UV,h_y;
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
		MT_Data[0].src_UV_h_max=div_src_h_UV ? size_y >> 1:size_y;
		MT_Data[0].dst_UV_h_max=div_dst_h_UV ? size_y >> 1:size_y;
		MT_Data[0].src_Y_w=size_x;
		MT_Data[0].dst_Y_w=size_x;
		MT_Data[0].src_UV_w=div_src_w_UV ? size_x >> 1:size_x;
		MT_Data[0].dst_UV_w=div_dst_w_UV ? size_x >> 1:size_x;
		return(1);
	}

	src_dh_UV=div_src_h_UV ? dh_Y>>1:dh_Y;
	dst_dh_UV=div_dst_h_UV ? dh_Y>>1:dh_Y;

	MT_Data[0].top=true;
	MT_Data[0].bottom=false;
	MT_Data[0].src_Y_h_min=0;
	MT_Data[0].src_Y_h_max=dh_Y;
	MT_Data[0].dst_Y_h_min=0;
	MT_Data[0].dst_Y_h_max=dh_Y;
	MT_Data[0].src_UV_h_min=0;
	MT_Data[0].src_UV_h_max=src_dh_UV;
	MT_Data[0].dst_UV_h_min=0;
	MT_Data[0].dst_UV_h_max=dst_dh_UV;

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
		MT_Data[i].src_UV_h_max=MT_Data[i].src_UV_h_min+src_dh_UV;
		MT_Data[i].dst_UV_h_min=MT_Data[i-1].dst_UV_h_max;
		MT_Data[i].dst_UV_h_max=MT_Data[i].dst_UV_h_min+dst_dh_UV;
		i++;
	}
	MT_Data[max-1].bottom=true;
	MT_Data[max-1].src_Y_h_max=size_y;
	MT_Data[max-1].dst_Y_h_max=size_y;
	MT_Data[max-1].src_UV_h_max=div_src_h_UV ? size_y >> 1:size_y;
	MT_Data[max-1].dst_UV_h_max=div_dst_h_UV ? size_y >> 1:size_y;
	for (i=0; i<max; i++)
	{
		MT_Data[i].src_Y_w=size_x;
		MT_Data[i].dst_Y_w=size_x;
		MT_Data[i].src_UV_w=div_src_w_UV ? size_x >> 1:size_x;
		MT_Data[i].dst_UV_w=div_dst_w_UV ? size_x >> 1:size_x;
	}
	return(max);
}


static void Convert_Progressive_8_YV12toYV16_SSE2(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U16=(w_U+15)>>4,w_V16=(w_V+15)>>4;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	//Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_U,src_Un,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_U,src_Up,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_U,src_Un,dstUp,w_U16);
		dstUp+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_U,src_Up,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}


// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_V,src_Vn,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_V,src_Vp,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_V,src_Vn,dstVp,w_V16);
		dstVp+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2(src_V,src_Vp,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_8_YV12toYV16_AVX(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U16=(w_U+15)>>4,w_V16=(w_V+15)>>4;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	//Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_U,src_Un,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_U,src_Up,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_U,src_Un,dstUp,w_U16);
		dstUp+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_U,src_Up,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_V,src_Vn,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_V,src_Vp,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_V,src_Vn,dstVp,w_V16);
		dstVp+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX(src_V,src_Vp,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_16_YV12toYV16_SSE2(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U << 1,w_V2=w_V << 1;
	const int32_t w_U8=(w_U+7)>>3,w_V8=(w_V+7)>>3;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;


	//Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_U,src_Un,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_U,src_Up,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_U,src_Un,dstUp,w_U8);
		dstUp+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_U,src_Up,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_V,src_Vn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_V,src_Vp,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_V,src_Vn,dstVp,w_V8);
		dstVp+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_SSE2(src_V,src_Vp,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_16_YV12toYV16_AVX(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U << 1,w_V2=w_V << 1;
	const int32_t w_U8=(w_U+7)>>3,w_V8=(w_V+7)>>3;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	//Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_U,src_Un,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_U,src_Up,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_U,src_Un,dstUp,w_U8);
		dstUp+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_U,src_Up,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_V,src_Vn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_V,src_Vp,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_V,src_Vn,dstVp,w_V8);
		dstVp+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX(src_V,src_Vp,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


#ifdef AVX2_BUILD_POSSIBLE
static void Convert_Progressive_8_YV12toYV16_AVX2(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U32=(w_U+31)>>5,w_V32=(w_V+31)>>5;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	//Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_U,src_Un,dstUp,w_U32);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_U,src_Up,dstUp,w_U32);
		dstUp+=dst_pitch_U;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_U,src_Un,dstUp,w_U32);
		dstUp+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_U,src_Up,dstUp,w_U32);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_V,src_Vn,dstVp,w_V32);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_V,src_Vp,dstVp,w_V32);
		dstVp+=dst_pitch_V;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_V,src_Vn,dstVp,w_V32);
		dstVp+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2(src_V,src_Vp,dstVp,w_V32);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_16_YV12toYV16_AVX2(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U << 1,w_V2=w_V << 1;
	const int32_t w_U16=(w_U+15)>>4,w_V16=(w_V+15)>>4;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	//Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_U,src_Un,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_U,src_Up,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_U,src_Un,dstUp,w_U16);
		dstUp+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_U,src_Up,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_V,src_Vn,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_V,src_Vp,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_V,src_Vn,dstVp,w_V16);
		dstVp+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2(src_V,src_Vp,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}
#endif


static void Convert_Progressive_8_YV12toYV16(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU_=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV_=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	uint8_t *dst_U,*dst_V;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	dst_U=dstU_;
	dst_V=dstV_;

	//Move_Full(srcY,dstY,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j])>>2);
			dst_U+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j])>>2);
		dst_U+=dst_pitch_U;

		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j])>>2);
		dst_U+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j])>>2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_U);
			dst_U+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_V,src_V,w_V);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_V; j++)
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j])>>2);
			dst_V+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j])>>2);
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j])>>2);
		dst_V+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			for(int32_t j=0; j<w_V; j++)
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j])>>2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_V);
			dst_V+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_16_YV12toYV16(const MT_Data_Info &mt_data_inf,const uint32_t *lookup)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU_=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV_=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU_=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV_=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	uint8_t *dst_U,*dst_V;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U << 1,w_V2=w_V << 1;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcU_;
	src_V=srcV_;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	dst_U=dstU_;
	dst_V=dstV_;

	//Move_Full(srcY,dstY,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_U,src_U,w_U2);
			dst_U+=dst_pitch_U;

			const uint16_t *srcU=(const uint16_t *)src_U,*srcUn=(const uint16_t *)src_Un;
			uint16_t *dstU=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dstU[j]=(uint16_t)((lookup[srcU[j]]+(uint32_t)srcUn[j])>>2);
			dst_U+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		const uint16_t *srcU=(const uint16_t *)src_U,*srcUp=(const uint16_t *)src_Up;
		uint16_t *dstU=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dstU[j]=(uint16_t)((lookup[srcU[j]]+(uint32_t)srcUp[j])>>2);
		dst_U+=dst_pitch_U;

		const uint16_t *srcUn=(const uint16_t *)src_Un;
		dstU=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dstU[j]=(uint16_t)((lookup[srcU[j]]+(uint32_t)srcUn[j])>>2);
		dst_U+=dst_pitch_U;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			const uint16_t *srcU=(const uint16_t *)src_U,*srcUp=(const uint16_t *)src_Up;
			uint16_t *dstU=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dstU[j]=(uint16_t)((lookup[srcU[j]]+(uint32_t)srcUp[j])>>2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_U2);
			dst_U+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_V,src_V,w_V2);
			dst_V+=dst_pitch_V;

			const uint16_t *srcV=(const uint16_t *)src_V,*srcVn=(const uint16_t *)src_Vn;
			uint16_t *dstV=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dstV[j]=(uint16_t)((lookup[srcV[j]]+(uint32_t)srcVn[j])>>2);
			dst_V+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		const uint16_t *srcV=(const uint16_t *)src_V,*srcVp=(const uint16_t *)src_Vp;
		uint16_t *dstV=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dstV[j]=(uint16_t)((lookup[srcV[j]]+(uint32_t)srcVp[j])>>2);
		dst_V+=dst_pitch_V;

		const uint16_t *srcVn=(const uint16_t *)src_Vn;
		dstV=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dstV[j]=(uint16_t)((lookup[srcV[j]]+(uint32_t)srcVn[j])>>2);
		dst_V+=dst_pitch_V;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			const uint16_t *srcV=(const uint16_t *)src_V,*srcVp=(const uint16_t *)src_Vp;
			uint16_t *dstV=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dstV[j]=(uint16_t)((lookup[srcV[j]]+(uint32_t)srcVp[j])>>2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_V2);
			dst_V+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_8_YV16toYV24(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const int32_t w_U=(dst_w>>1)-1,w_V=(dst_w>>1)-1;

	//Move_Full(srcY,dstY,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		int32_t j2=0;

		for(int32_t j=0; j<w_U; j++)
		{
			dstU[j2++]=srcU[j];
			dstU[j2++]=(uint8_t)(((uint16_t)srcU[j]+(uint16_t)srcU[j+1])>>1);
		}
		dstU[j2++]=srcU[w_U];
		dstU[j2]=srcU[w_U];

		srcU+=src_pitch_U;
		dstU+=dst_pitch_U;
	}

// Planar V
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		int32_t j2=0;

		for(int32_t j=0; j<w_V; j++)
		{
			dstV[j2++]=srcV[j];
			dstV[j2++]=(uint8_t)(((uint16_t)srcV[j]+(uint16_t)srcV[j+1])>>1);
		}
		dstV[j2++]=srcV[w_V];
		dstV[j2]=srcV[w_V];

		srcV+=src_pitch_V;
		dstV+=dst_pitch_V;
	}
}


static void Convert_8_YV16toYV24_SSE2(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U16=((w_U+15) >> 4)-1,w_V16=((w_V+15) >> 4)-1;
	const int32_t w_U0=(w_U-(w_U16 << 4))-1,w_V0=(w_V-(w_V16 << 4))-1;
	const uint32_t offsetU8=w_U16 << 4,offsetU16=offsetU8 << 1;
	const uint32_t offsetV8=w_V16 << 4,offsetV16=offsetV8 << 1;

	//Move_Full(srcY,dstY,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2(srcU,dstU,w_U16);

		const uint8_t *src=srcU+offsetU8;
		uint8_t *dst=dstU+offsetU16;
		int32_t j2=0;

		for(int32_t j=0; j<w_U0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint8_t)(((uint16_t)src[j]+(uint16_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_U0];
		dst[j2]=src[w_U0];

		srcU+=src_pitch_U;
		dstU+=dst_pitch_U;
	}

// Planar V
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2(srcV,dstV,w_V16);

		const uint8_t *src=srcV+offsetV8;
		uint8_t *dst=dstV+offsetV16;
		int32_t j2=0;

		for(int32_t j=0; j<w_V0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint8_t)(((uint16_t)src[j]+(uint16_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_V0];
		dst[j2]=src[w_V0];

		srcV+=src_pitch_V;
		dstV+=dst_pitch_V;
	}
}


static void Convert_8_YV16toYV24_AVX(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U16=((w_U+15) >> 4)-1,w_V16=((w_V+15) >> 4)-1;
	const int32_t w_U0=(w_U-(w_U16 << 4))-1,w_V0=(w_V-(w_V16 << 4))-1;
	const uint32_t offsetU8=w_U16 << 4,offsetU16=offsetU8 << 1;
	const uint32_t offsetV8=w_V16 << 4,offsetV16=offsetV8 << 1;

	//Move_Full(srcY,dstY,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX(srcU,dstU,w_U16);

		const uint8_t *src=srcU+offsetU8;
		uint8_t *dst=dstU+offsetU16;
		int32_t j2=0;

		for(int32_t j=0; j<w_U0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint8_t)(((uint16_t)src[j]+(uint16_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_U0];
		dst[j2]=src[w_U0];

		srcU+=src_pitch_U;
		dstU+=dst_pitch_U;
	}

// Planar V
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX(srcV,dstV,w_V16);

		const uint8_t *src=srcV+offsetV8;
		uint8_t *dst=dstV+offsetV16;
		int32_t j2=0;

		for(int32_t j=0; j<w_V0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint8_t)(((uint16_t)src[j]+(uint16_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_V0];
		dst[j2]=src[w_V0];

		srcV+=src_pitch_V;
		dstV+=dst_pitch_V;
	}
}


static void Convert_16_YV16toYV24(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const int32_t w_U=(dst_w>>1)-1,w_V=(dst_w>>1)-1;

	//Move_Full(srcY,dstY,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		const uint16_t *src=(uint16_t *)srcU;
		uint16_t *dst=(uint16_t *)dstU;
		int32_t j2=0;

		for(int32_t j=0; j<w_U; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint16_t)(((uint32_t)src[j]+(uint32_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_U];
		dst[j2]=src[w_U];

		srcU+=src_pitch_U;
		dstU+=dst_pitch_U;
	}

// Planar V
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		const uint16_t *src=(uint16_t *)srcV;
		uint16_t *dst=(uint16_t *)dstV;
		int32_t j2=0;

		for(int32_t j=0; j<w_V; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint16_t)(((uint32_t)src[j]+(uint32_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_V];
		dst[j2]=src[w_V];

		srcV+=src_pitch_V;
		dstV+=dst_pitch_V;
	}
}


static void Convert_16_YV16toYV24_SSE2(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U8=((w_U+7) >> 3)-1,w_V8=((w_V+7) >> 3)-1;
	const int32_t w_U0=(w_U-(w_U8 << 3))-1,w_V0=(w_V-(w_V8 << 3))-1;
	const uint32_t offsetU8=w_U8 << 4,offsetU16=offsetU8 << 1;
	const uint32_t offsetV8=w_V8 << 4,offsetV16=offsetV8 << 1;

	//Move_Full(srcY,dstY,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_16_SSE2(srcU,dstU,w_U8);

		const uint16_t *src=(uint16_t *)(srcU+offsetU8);
		uint16_t *dst=(uint16_t *)(dstU+offsetU16);
		int32_t j2=0;

		for(int32_t j=0; j<w_U0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint16_t)(((uint32_t)src[j]+(uint32_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_U0];
		dst[j2]=src[w_U0];

		srcU+=src_pitch_U;
		dstU+=dst_pitch_U;
	}

// Planar V
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_16_SSE2(srcV,dstV,w_V8);

		const uint16_t *src=(uint16_t *)(srcV+offsetV8);
		uint16_t *dst=(uint16_t *)(dstV+offsetV16);
		int32_t j2=0;

		for(int32_t j=0; j<w_V0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint16_t)(((uint32_t)src[j]+(uint32_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_V0];
		dst[j2]=src[w_V0];

		srcV+=src_pitch_V;
		dstV+=dst_pitch_V;
	}
}


static void Convert_16_YV16toYV24_AVX(const MT_Data_Info &mt_data_inf)
{
	//const uint8_t *srcY=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcU=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcV=(const uint8_t *)mt_data_inf.src3;
	//uint8_t *dstY=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	//const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	//const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U8=((w_U+7) >> 3)-1,w_V8=((w_V+7) >> 3)-1;
	const int32_t w_U0=(w_U-(w_U8 << 3))-1,w_V0=(w_V-(w_V8 << 3))-1;
	const uint32_t offsetU8=w_U8 << 4,offsetU16=offsetU8 << 1;
	const uint32_t offsetV8=w_V8 << 4,offsetV16=offsetV8 << 1;

	//Move_Full(srcY,dstY,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_16_AVX(srcU,dstU,w_U8);

		const uint16_t *src=(uint16_t *)(srcU+offsetU8);
		uint16_t *dst=(uint16_t *)(dstU+offsetU16);
		int32_t j2=0;

		for(int32_t j=0; j<w_U0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint16_t)(((uint32_t)src[j]+(uint32_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_U0];
		dst[j2]=src[w_U0];

		srcU+=src_pitch_U;
		dstU+=dst_pitch_U;
	}

// Planar V
	for(int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		JPSDR_RGBConvert_Convert_Planar422_to_Planar444_16_AVX(srcV,dstV,w_V8);

		const uint16_t *src=(uint16_t *)(srcV+offsetV8);
		uint16_t *dst=(uint16_t *)(dstV+offsetV16);
		int32_t j2=0;

		for(int32_t j=0; j<w_V0; j++)
		{
			dst[j2++]=src[j];
			dst[j2++]=(uint16_t)(((uint32_t)src[j]+(uint32_t)src[j+1])>>1);
		}
		dst[j2++]=src[w_V0];
		dst[j2]=src[w_V0];

		srcV+=src_pitch_V;
		dstV+=dst_pitch_V;
	}
}


static void Convert_YV24toRGB32(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const uint8_t *src_y=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src_u=(uint8_t *)mt_data_inf.src2;
	const uint8_t *src_v=(uint8_t *)mt_data_inf.src3;
	RGB32BMP *dst=(RGB32BMP *)mt_data_inf.dst1;
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	for (int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,g,b;
			const uint16_t y=src_y[j],u=src_u[j],v=src_v[j];

			r=(lookup[y]+lookup[v+256]+(int16_t)dl.Offset_R) >> 5;
			g=(lookup[y]+lookup[u+512]+lookup[v+768]+(int16_t)dl.Offset_G) >> 5;
			b=(lookup[y]+lookup[u+1024]+(int16_t)dl.Offset_B) >> 5;

			dst[j].b=(uint8_t)std::min((int16_t)255,std::max((int16_t)0,b));
			dst[j].g=(uint8_t)std::min((int16_t)255,std::max((int16_t)0,g));
			dst[j].r=(uint8_t)std::min((int16_t)255,std::max((int16_t)0,r));
			dst[j].alpha=0;
		}
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
		src_y+=src_pitch_y;
		src_u+=src_pitch_u;
		src_v+=src_pitch_v;
	}
}


static void Convert_YV24toRGB32_SSE2(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;

	JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
		w,h_Y_max-h_Y_min,(int16_t)dl.Offset_R,(int16_t)dl.Offset_G,(int16_t)dl.Offset_B,lookup,mt_data_inf.src_modulo1,
		mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
}


static void Convert_YV24toRGB32_AVX(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;

	JPSDR_RGBConvert_Convert_YV24toRGB32_AVX(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
		w,h_Y_max-h_Y_min,(int16_t)dl.Offset_R,(int16_t)dl.Offset_G,(int16_t)dl.Offset_B,lookup,mt_data_inf.src_modulo1,
		mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
}


static void Convert_YV24toRGB64_SSE41(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup,
	const uint8_t bits_per_pixel)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;

	switch(bits_per_pixel)
	{
		case 10 :
			JPSDR_RGBConvert_Convert_10_YV24toRGB64_SSE41(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		case 12 :
			JPSDR_RGBConvert_Convert_12_YV24toRGB64_SSE41(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		case 14 :
			JPSDR_RGBConvert_Convert_14_YV24toRGB64_SSE41(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		case 16 :
			JPSDR_RGBConvert_Convert_16_YV24toRGB64_SSE41(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		default : break;
	}
}


static void Convert_YV24toRGB64_AVX(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup,
	const uint8_t bits_per_pixel)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;

	switch(bits_per_pixel)
	{
		case 10 :
			JPSDR_RGBConvert_Convert_10_YV24toRGB64_AVX(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		case 12 :
			JPSDR_RGBConvert_Convert_12_YV24toRGB64_AVX(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		case 14 :
			JPSDR_RGBConvert_Convert_14_YV24toRGB64_AVX(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		case 16 :
			JPSDR_RGBConvert_Convert_16_YV24toRGB64_AVX(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h_Y_max-h_Y_min,dl.Offset_R,dl.Offset_G,dl.Offset_B,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.src_modulo2,mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
			break;
		default : break;
	}
}


static void Convert_YV24toRGB64(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup,
	const uint8_t bits_per_pixel)
{
	const uint16_t *src_y=(uint16_t *)mt_data_inf.src1;
	const uint16_t *src_u=(uint16_t *)mt_data_inf.src2;
	const uint16_t *src_v=(uint16_t *)mt_data_inf.src3;
	RGB64BMP *dst=(RGB64BMP *)mt_data_inf.dst1;
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const uint32_t vmax=(uint32_t)1 << bits_per_pixel;
	const uint32_t vmax2=vmax*2,vmax3=vmax*3,vmax4=vmax*4;

	for (int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int32_t r,g,b;
			const uint32_t y=src_y[j],u=src_u[j],v=src_v[j];

			r=(lookup[y]+lookup[v+vmax]+dl.Offset_R) >> 8;
			g=(lookup[y]+lookup[u+vmax2]+lookup[v+vmax3]+dl.Offset_G) >> 8;
			b=(lookup[y]+lookup[u+vmax4]+dl.Offset_B) >> 8;

			dst[j].b=(uint16_t)std::min(65535,std::max(0,b));
			dst[j].g=(uint16_t)std::min(65535,std::max(0,g));
			dst[j].r=(uint16_t)std::min(65535,std::max(0,r));
			dst[j].alpha=0;
		}
		dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);
		src_y=(uint16_t *)((uint8_t *)src_y+src_pitch_y);
		src_u=(uint16_t *)((uint8_t *)src_u+src_pitch_u);
		src_v=(uint16_t *)((uint8_t *)src_v+src_pitch_v);
	}
}


static void Convert_RGB32toYV24(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const RGB32BMP *src=(RGB32BMP *)mt_data_inf.src1;
	uint8_t *dst_y=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dst_u=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dst_v=(uint8_t *)mt_data_inf.dst3;
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.dst_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.dst_Y_h_max;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch_y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_u=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_v=mt_data_inf.dst_pitch3;

	for (int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,u,v;
			const uint16_t b=src[j].b,g=src[j].g,r=src[j].r;

			y=((int16_t)dl.Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			u=((int16_t)dl.Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=((int16_t)dl.Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;

			dst_y[j]=(uint8_t)std::min((int16_t)dl.Max_Y,std::max((int16_t)dl.Min_Y,y));
			dst_u[j]=(uint8_t)std::min((int16_t)dl.Max_U,std::max((int16_t)dl.Min_U,u));
			dst_v[j]=(uint8_t)std::min((int16_t)dl.Max_V,std::max((int16_t)dl.Min_V,v));
		}
		src=(RGB32BMP *)((uint8_t *)src+src_pitch);
		dst_y+=dst_pitch_y;
		dst_u+=dst_pitch_u;
		dst_v+=dst_pitch_v;
	}
}


static void Convert_RGB32toYV24_SSE2(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h=mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;

	JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
		w,h,(int16_t)dl.Offset_Y,(int16_t)dl.Offset_U,(int16_t)dl.Offset_V,lookup,mt_data_inf.src_modulo1,
		mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,
		(int16_t)dl.Min_Y,(int16_t)dl.Max_Y,(int16_t)dl.Min_U,(int16_t)dl.Max_U,(int16_t)dl.Min_V,(int16_t)dl.Max_V);
}


static void Convert_RGB32toYV24_AVX(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h=mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;

	JPSDR_RGBConvert_Convert_RGB32toYV24_AVX(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
		w,h,(int16_t)dl.Offset_Y,(int16_t)dl.Offset_U,(int16_t)dl.Offset_V,lookup,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,
		(int16_t)dl.Min_Y,(int16_t)dl.Max_Y,(int16_t)dl.Min_U,(int16_t)dl.Max_U,(int16_t)dl.Min_V,(int16_t)dl.Max_V);
}


static void Convert_RGB64toYV24(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup)
{
	const RGB64BMP *src=(RGB64BMP *)mt_data_inf.src1;
	uint8_t *dst_y_=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dst_u_=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dst_v_=(uint8_t *)mt_data_inf.dst3;
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.dst_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.dst_Y_h_max;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch_y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_u=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_v=mt_data_inf.dst_pitch3;

	for (int32_t i=h_Y_min; i<h_Y_max; i++)
	{
		uint16_t *dst_y=(uint16_t *)dst_y_;
		uint16_t *dst_u=(uint16_t *)dst_u_;
		uint16_t *dst_v=(uint16_t *)dst_v_;

		for (int32_t j=0; j<w; j++)
		{
			int32_t y,u,v;
			const uint32_t b=src[j].b,g=src[j].g,r=src[j].r;

			y=(dl.Offset_Y+lookup[r]+lookup[g+65536]+lookup[b+131072]) >> 8;
			u=(dl.Offset_U+lookup[r+196608]+lookup[g+262144]+lookup[b+327680]) >> 8;
			v=(dl.Offset_V+lookup[r+393216]+lookup[g+458752]+lookup[b+524288]) >> 8;

			dst_y[j]=(uint16_t)std::min((int32_t)dl.Max_Y,std::max((int32_t)dl.Min_Y,y));
			dst_u[j]=(uint16_t)std::min((int32_t)dl.Max_U,std::max((int32_t)dl.Min_U,u));
			dst_v[j]=(uint16_t)std::min((int32_t)dl.Max_V,std::max((int32_t)dl.Min_V,v));
		}
		src=(RGB64BMP *)((uint8_t *)src+src_pitch);
		dst_y_+=dst_pitch_y;
		dst_u_+=dst_pitch_u;
		dst_v_+=dst_pitch_v;
	}
}


static void Convert_RGB64toYV24_SSE41(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h=mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;

	JPSDR_RGBConvert_Convert_RGB64toYV24_SSE41(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
		w,h,dl.Offset_Y,dl.Offset_U,dl.Offset_V,lookup,mt_data_inf.src_modulo1,
		mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,
		(uint16_t)dl.Min_Y,(uint16_t)dl.Max_Y,(uint16_t)dl.Min_U,(uint16_t)dl.Max_U,(uint16_t)dl.Min_V,(uint16_t)dl.Max_V);
}


static void Convert_RGB64toYV24_AVX(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup)
{
	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h=mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;

	JPSDR_RGBConvert_Convert_RGB64toYV24_AVX(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
		w,h,dl.Offset_Y,dl.Offset_U,dl.Offset_V,lookup,mt_data_inf.src_modulo1,
		mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,
		(uint16_t)dl.Min_Y,(uint16_t)dl.Max_Y,(uint16_t)dl.Min_U,(uint16_t)dl.Max_U,(uint16_t)dl.Min_V,(uint16_t)dl.Max_V);
}


static void Convert_RGB32toYUY2(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const RGB32BMP *src;
	uint32_t *dst;

	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h=mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;

	const int32_t w0=((w+1) >> 1)-1;

	src=(RGB32BMP *)mt_data_inf.src1;
	dst=(uint32_t *)mt_data_inf.dst1;

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
			y=((int16_t)dl.Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			u=((int16_t)dl.Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
			v=((int16_t)dl.Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
			d.yuyv.y1=(uint8_t)std::min((int16_t)dl.Max_Y,std::max((int16_t)dl.Min_Y,y));
			d.yuyv.u=(uint8_t)std::min((int16_t)dl.Max_U,std::max((int16_t)dl.Min_U,u));
			d.yuyv.v=(uint8_t)std::min((int16_t)dl.Max_V,std::max((int16_t)dl.Min_V,v));
			k++;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=((int16_t)dl.Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			d.yuyv.y2=(uint8_t)std::min((int16_t)dl.Max_Y,std::max((int16_t)dl.Min_Y,y));
			k++;
			dst[j]=d.data32;
		}

		int16_t y,u,v;
		uint16_t b,g,r;
		UYUYV d;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		y=((int16_t)dl.Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
		u=((int16_t)dl.Offset_U+lookup[r+768]+lookup[g+1024]+lookup[b+1280]) >> 6;
		v=((int16_t)dl.Offset_V+lookup[r+1536]+lookup[g+1792]+lookup[b+2048]) >> 6;
		d.yuyv.y1=(uint8_t)std::min((int16_t)dl.Max_Y,std::max((int16_t)dl.Min_Y,y));
		d.yuyv.u=(uint8_t)std::min((int16_t)dl.Max_U,std::max((int16_t)dl.Min_U,u));
		d.yuyv.v=(uint8_t)std::min((int16_t)dl.Max_V,std::max((int16_t)dl.Min_V,v));
		k++;

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=((int16_t)dl.Offset_Y+lookup[r]+lookup[g+256]+lookup[b+512]) >> 6;
			d.yuyv.y2=(uint8_t)std::min((int16_t)dl.Max_Y,std::max((int16_t)dl.Min_Y,y));
		}
		else d.yuyv.y2=(uint8_t)dl.Min_Y;

		dst[w0]=d.data32;

		src=(RGB32BMP *)((uint8_t *)src+mt_data_inf.src_pitch1);
		dst=(uint32_t *)((uint8_t *)dst+mt_data_inf.dst_pitch1);
	}
}


static void Convert_RGB32toYUY2_SSE2(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const int32_t h = mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w = mt_data_inf.src_Y_w;

	JPSDR_RGBConvert_RGB32toYUYV_SSE2(mt_data_inf.src1,mt_data_inf.dst1,w,h,(int16_t)dl.Offset_Y,(int16_t)dl.Offset_U,
		(int16_t)dl.Offset_V,lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,(int16_t)dl.Min_Y,(int16_t)dl.Max_Y,
		(int16_t)dl.Min_U,(int16_t)dl.Max_U,(int16_t)dl.Min_V,(int16_t)dl.Max_V);
}


static void Convert_RGB32toYUY2_AVX(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int16_t *lookup)
{
	const int32_t h = mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w = mt_data_inf.src_Y_w;

	JPSDR_RGBConvert_RGB32toYUYV_AVX(mt_data_inf.src1,mt_data_inf.dst1,w,h,(int16_t)dl.Offset_Y,(int16_t)dl.Offset_U,
		(int16_t)dl.Offset_V,lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,(int16_t)dl.Min_Y,(int16_t)dl.Max_Y,
		(int16_t)dl.Min_U,(int16_t)dl.Max_U,(int16_t)dl.Min_V,(int16_t)dl.Max_V);
}


static void Convert_RGB64toYU64(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup)
{
	const RGB64BMP *src;
	uint64_t *dst;

	const int32_t w=mt_data_inf.dst_Y_w;
	const int32_t h=mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;

	const int32_t w0=((w+1) >> 1)-1;

	src=(RGB64BMP *)mt_data_inf.src1;
	dst=(uint64_t *)mt_data_inf.dst1;

	for (int32_t i=0; i<h; i++)
	{
		int32_t k=0;

		for (int32_t j=0; j<w0; j++)
		{
			int32_t y,u,v;
			uint32_t b,g,r;
			UYUYV16 d;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(dl.Offset_Y+lookup[r]+lookup[g+65536]+lookup[b+131072]) >> 8;
			u=(dl.Offset_U+lookup[r+196608]+lookup[g+262144]+lookup[b+327680]) >> 8;
			v=(dl.Offset_V+lookup[r+393216]+lookup[g+458752]+lookup[b+524288]) >> 8;

			d.yuyv.y1=(uint16_t)std::min((int32_t)dl.Max_Y,std::max((int32_t)dl.Min_Y,y));
			d.yuyv.u=(uint16_t)std::min((int32_t)dl.Max_U,std::max((int32_t)dl.Min_U,u));
			d.yuyv.v=(uint16_t)std::min((int32_t)dl.Max_V,std::max((int32_t)dl.Min_V,v));
			k++;

			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(dl.Offset_Y+lookup[r]+lookup[g+65536]+lookup[b+131072]) >> 8;
			d.yuyv.y2=(uint16_t)std::min((int32_t)dl.Max_Y,std::max((int32_t)dl.Min_Y,y));
			k++;
			dst[j]=d.data64;
		}

		int32_t y,u,v;
		uint32_t b,g,r;
		UYUYV16 d;

		b=src[k].b;
		g=src[k].g;
		r=src[k].r;
		y=(dl.Offset_Y+lookup[r]+lookup[g+65536]+lookup[b+131072]) >> 8;
		u=(dl.Offset_U+lookup[r+196608]+lookup[g+262144]+lookup[b+327680]) >> 8;
		v=(dl.Offset_V+lookup[r+393216]+lookup[g+458752]+lookup[b+524288]) >> 8;

		d.yuyv.y1=(uint16_t)std::min((int32_t)dl.Max_Y,std::max((int32_t)dl.Min_Y,y));
		d.yuyv.u=(uint16_t)std::min((int32_t)dl.Max_U,std::max((int32_t)dl.Min_U,u));
		d.yuyv.v=(uint16_t)std::min((int32_t)dl.Max_V,std::max((int32_t)dl.Min_V,v));
		k++;

		if (k<w)
		{
			b=src[k].b;
			g=src[k].g;
			r=src[k].r;
			y=(dl.Offset_Y+lookup[r]+lookup[g+65536]+lookup[b+131072]) >> 8;
			d.yuyv.y2=(uint16_t)std::min((int32_t)dl.Max_Y,std::max((int32_t)dl.Min_Y,y));
		}
		else d.yuyv.y2=dl.Min_Y;

		dst[w0]=d.data64;

		src=(RGB64BMP *)((uint8_t *)src+mt_data_inf.src_pitch1);
		dst=(uint64_t *)((uint8_t *)dst+mt_data_inf.dst_pitch1);
	}
}


static void Convert_RGB64toYU64_SSE41(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup)
{
	const int32_t h = mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w = mt_data_inf.src_Y_w;

	JPSDR_RGBConvert_RGB64toYU64_SSE41(mt_data_inf.src1,mt_data_inf.dst1,w,h,dl.Offset_Y,dl.Offset_U,dl.Offset_V,
		lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,dl.Min_Y,dl.Max_Y,dl.Min_U,dl.Max_U,dl.Min_V,dl.Max_V);
}


static void Convert_RGB64toYU64_AVX(const MT_Data_Info &mt_data_inf,const dataLookUp &dl,const int32_t *lookup)
{
	const int32_t h = mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w = mt_data_inf.src_Y_w;

	JPSDR_RGBConvert_RGB64toYU64_AVX(mt_data_inf.src1,mt_data_inf.dst1,w,h,dl.Offset_Y,dl.Offset_U,dl.Offset_V,
		lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,dl.Min_Y,dl.Max_Y,dl.Min_U,dl.Max_U,dl.Min_V,dl.Max_V);
}


static void Convert_Planar444toPlanar422_8(const MT_Data_Info &mt_data_inf)
{
	const int32_t w=mt_data_inf.dst_UV_w;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8(mt_data_inf.src1,mt_data_inf.dst1,w,h,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8(mt_data_inf.src2,mt_data_inf.dst2,w,h,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}


static void Convert_Planar444toPlanar422_16(const MT_Data_Info &mt_data_inf)
{
	const int32_t w=mt_data_inf.dst_UV_w;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16(mt_data_inf.src1,mt_data_inf.dst1,w,h,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16(mt_data_inf.src2,mt_data_inf.dst2,w,h,
		mt_data_inf.src_modulo2,mt_data_inf.dst_modulo2);
}


static void Convert_Planar444toPlanar422_8_SSE2(const MT_Data_Info &mt_data_inf)
{
	const int32_t w16=(mt_data_inf.src_UV_w+15)>>4;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2(mt_data_inf.src1,mt_data_inf.dst1,w16,h,
		mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2(mt_data_inf.src2,mt_data_inf.dst2,w16,h,
		mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
}


static void Convert_Planar444toPlanar422_16_SSE41(const MT_Data_Info &mt_data_inf)
{
	const int32_t w8=(mt_data_inf.src_UV_w+7)>>3;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16_SSE41(mt_data_inf.src1,mt_data_inf.dst1,w8,h,
		mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16_SSE41(mt_data_inf.src2,mt_data_inf.dst2,w8,h,
		mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
}


static void Convert_Planar444toPlanar422_8_AVX(const MT_Data_Info &mt_data_inf)
{
	const int32_t w16=(mt_data_inf.src_UV_w+15)>>4;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX(mt_data_inf.src1,mt_data_inf.dst1,w16,h,
		mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX(mt_data_inf.src2,mt_data_inf.dst2,w16,h,
		mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
}


static void Convert_Planar444toPlanar422_16_AVX(const MT_Data_Info &mt_data_inf)
{
	const int32_t w8=(mt_data_inf.src_UV_w+7)>>3;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16_AVX(mt_data_inf.src1,mt_data_inf.dst1,w8,h,
		mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar444_to_Planar422_16_AVX(mt_data_inf.src2,mt_data_inf.dst2,w8,h,
		mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
}


static void Convert_Planar422toPlanar420_8(const MT_Data_Info &mt_data_inf)
{
	const uint8_t *srcU=(const uint8_t *)((uint8_t *)mt_data_inf.src1);
	const uint8_t *srcUn=srcU+mt_data_inf.src_pitch1;
	uint8_t *dstU=(uint8_t *)mt_data_inf.dst1;
	const uint8_t *srcV=(const uint8_t *)((uint8_t *)mt_data_inf.src2);
	const uint8_t *srcVn=srcV+mt_data_inf.src_pitch2;
	uint8_t *dstV=(uint8_t *)mt_data_inf.dst2;
	const ptrdiff_t src_pitch_U_2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t src_pitch_V_2=mt_data_inf.src_pitch2 << 1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch2;
	const int32_t w=mt_data_inf.dst_UV_w;
	const int32_t h_min=mt_data_inf.dst_UV_h_min;
	const int32_t h_max=mt_data_inf.dst_UV_h_max;

	for(int32_t i=h_min; i<h_max; i++)
	{
		for(int32_t j=0; j<w; j++)
			dstU[j]=(uint8_t)(((uint16_t)srcU[j]+(uint16_t)srcUn[j]) >> 1);

		srcU+=src_pitch_U_2;
		srcUn+=src_pitch_U_2;
		dstU+=dst_pitch_U;
	}

	for(int32_t i=h_min; i<h_max; i++)
	{
		for(int32_t j=0; j<w; j++)
			dstV[j]=(uint8_t)(((uint16_t)srcV[j]+(uint16_t)srcVn[j]) >> 1);

		srcV+=src_pitch_V_2;
		srcVn+=src_pitch_V_2;
		dstV+=dst_pitch_V;
	}
}


static void Convert_Planar422toPlanar420_16(const MT_Data_Info &mt_data_inf)
{
	const uint8_t *srcU_=(const uint8_t *)((uint8_t *)mt_data_inf.src1);
	const uint8_t *srcUn_=srcU_+mt_data_inf.src_pitch1;
	uint8_t *dstU_=(uint8_t *)mt_data_inf.dst1;
	const uint8_t *srcV_=(const uint8_t *)((uint8_t *)mt_data_inf.src2);
	const uint8_t *srcVn_=srcV_+mt_data_inf.src_pitch2;
	uint8_t *dstV_=(uint8_t *)mt_data_inf.dst2;
	const ptrdiff_t src_pitch_U_2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t src_pitch_V_2=mt_data_inf.src_pitch2 << 1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch2;
	const int32_t w=mt_data_inf.dst_UV_w;
	const int32_t h_min=mt_data_inf.dst_UV_h_min;
	const int32_t h_max=mt_data_inf.dst_UV_h_max;

	for(int32_t i=h_min; i<h_max; i++)
	{
		const uint16_t *srcU=(const uint16_t *)srcU_,*srcUn=(const uint16_t *)srcUn_;
		uint16_t *dstU=(uint16_t *)dstU_;

		for(int32_t j=0; j<w; j++)
			dstU[j]=(uint16_t)(((uint32_t)srcU[j]+(uint32_t)srcUn[j]) >> 1);

		srcU_+=src_pitch_U_2;
		srcUn_+=src_pitch_U_2;
		dstU_+=dst_pitch_U;
	}

	for(int32_t i=h_min; i<h_max; i++)
	{
		const uint16_t *srcV=(const uint16_t *)srcV_,*srcVn=(const uint16_t *)srcVn_;
		uint16_t *dstV=(uint16_t *)dstV_;

		for(int32_t j=0; j<w; j++)
			dstV[j]=(uint16_t)(((uint32_t)srcV[j]+(uint32_t)srcVn[j]) >> 1);

		srcV_+=src_pitch_V_2;
		srcVn_+=src_pitch_V_2;
		dstV_+=dst_pitch_V;
	}
}


static void Convert_Planar422toPlanar420_8_SSE2(const MT_Data_Info &mt_data_inf)
{
	const int32_t w16=(mt_data_inf.dst_UV_w+15)>>4;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2(mt_data_inf.src1,(void *)(((uint8_t *)mt_data_inf.src1)+mt_data_inf.src_pitch1),
		mt_data_inf.dst1,w16,h,mt_data_inf.src_pitch1 << 1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2(mt_data_inf.src2,(void *)(((uint8_t *)mt_data_inf.src2)+mt_data_inf.src_pitch2),
		mt_data_inf.dst2,w16,h,mt_data_inf.src_pitch2 << 1,mt_data_inf.dst_pitch2);
}


static void Convert_Planar422toPlanar420_16_SSE2(const MT_Data_Info &mt_data_inf)
{
	const int32_t w8=(mt_data_inf.dst_UV_w+7)>>3;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_SSE2(mt_data_inf.src1,(void *)(((uint8_t *)mt_data_inf.src1)+mt_data_inf.src_pitch1),
		mt_data_inf.dst1,w8,h,mt_data_inf.src_pitch1 << 1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_SSE2(mt_data_inf.src2,(void *)(((uint8_t *)mt_data_inf.src2)+mt_data_inf.src_pitch2),
		mt_data_inf.dst2,w8,h,mt_data_inf.src_pitch2 << 1,mt_data_inf.dst_pitch2);
}


static void Convert_Planar422toPlanar420_8_AVX(const MT_Data_Info &mt_data_inf)
{
	const int32_t w16=(mt_data_inf.dst_UV_w+15)>>4;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX(mt_data_inf.src1,(void *)(((uint8_t *)mt_data_inf.src1)+mt_data_inf.src_pitch1),
		mt_data_inf.dst1,w16,h,mt_data_inf.src_pitch1 << 1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX(mt_data_inf.src2,(void *)(((uint8_t *)mt_data_inf.src2)+mt_data_inf.src_pitch2),
		mt_data_inf.dst2,w16,h,mt_data_inf.src_pitch2 << 1,mt_data_inf.dst_pitch2);
}


static void Convert_Planar422toPlanar420_16_AVX(const MT_Data_Info &mt_data_inf)
{
	const int32_t w8=(mt_data_inf.dst_UV_w+7)>>3;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX(mt_data_inf.src1,(void *)(((uint8_t *)mt_data_inf.src1)+mt_data_inf.src_pitch1),
		mt_data_inf.dst1,w8,h,mt_data_inf.src_pitch1 << 1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX(mt_data_inf.src2,(void *)(((uint8_t *)mt_data_inf.src2)+mt_data_inf.src_pitch2),
		mt_data_inf.dst2,w8,h,mt_data_inf.src_pitch2 << 1,mt_data_inf.dst_pitch2);
}


#ifdef AVX2_BUILD_POSSIBLE
static void Convert_Planar422toPlanar420_8_AVX2(const MT_Data_Info &mt_data_inf)
{
	const int32_t w32=(mt_data_inf.dst_UV_w+31)>>5;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2(mt_data_inf.src1,(void *)(((uint8_t *)mt_data_inf.src1)+mt_data_inf.src_pitch1),
		mt_data_inf.dst1,w32,h,mt_data_inf.src_pitch1 << 1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2(mt_data_inf.src2,(void *)(((uint8_t *)mt_data_inf.src2)+mt_data_inf.src_pitch2),
		mt_data_inf.dst2,w32,h,mt_data_inf.src_pitch2 << 1,mt_data_inf.dst_pitch2);
}

static void Convert_Planar422toPlanar420_16_AVX2(const MT_Data_Info &mt_data_inf)
{
	const int32_t w16=(mt_data_inf.dst_UV_w+15)>>4;
	const int32_t h=mt_data_inf.dst_UV_h_max-mt_data_inf.dst_UV_h_min;

	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2(mt_data_inf.src1,(void *)(((uint8_t *)mt_data_inf.src1)+mt_data_inf.src_pitch1),
		mt_data_inf.dst1,w16,h,mt_data_inf.src_pitch1 << 1,mt_data_inf.dst_pitch1);
	JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2(mt_data_inf.src2,(void *)(((uint8_t *)mt_data_inf.src2)+mt_data_inf.src_pitch2),
		mt_data_inf.dst2,w16,h,mt_data_inf.src_pitch2 << 1,mt_data_inf.dst_pitch2);
}
#endif


void JPSDR_RGBConvert::Compute_Lookup16(void)
{
	double kr,kg,kb;
	double Rv,Gu,Gv,Bu;
	double Ur,Ug,Ub,Vr,Vg,Vb;
	int32_t Off_Y,Off_U,Off_V;
	const uint8_t pshift1=bits_per_pixel-8,pshift2=24-bits_per_pixel;
	const int32_t vmax=1 << bits_per_pixel;
	const double coeff_div=((double)vmax)/65536.0,coeff_mul=(double)((int32_t)1 << pshift2);

	if (mData.auto_detect && (convertion_mode!=0))
	{
		Min_Y=image_data.src_Y_Min;
		Max_Y=image_data.src_Y_Max;
		Min_U=image_data.src_U_Min;
		Max_U=image_data.src_U_Max;
		Min_V=image_data.src_V_Min;
		Max_V=image_data.src_V_Max;
		Coeff_Y=image_data.src_Coeff_Y;
		Coeff_U=image_data.src_Coeff_U;
		Coeff_V=image_data.src_Coeff_V;
		kr=image_data.src_Kr;
		kg=image_data.src_Kg;
		kb=image_data.src_Kb;
	}
	else
	{
		if (mData.full_range)
		{
			Min_Y=0;
			Max_Y=255 << pshift1;
			Min_U=0;
			Max_U=255 << pshift1;
			Min_V=0;
			Max_V=255 << pshift1;
			Coeff_Y=1.0;
			Coeff_U=1.0;
			Coeff_V=1.0;
		}
		else
		{
			Min_Y=16 << pshift1;
			Max_Y=235 << pshift1;
			Min_U=16 << pshift1;
			Max_U=240 << pshift1;
			Min_V=16 << pshift1;
			Max_V=240 << pshift1;
			Coeff_Y=219.0/255.0;
			Coeff_U=224.0/255.0;
			Coeff_V=224.0/225.0;
		}	
		switch (mData.color_matrix)
		{
			case 0 : kr=0.2126; kb=0.0722; break;
			case 1 : kr=0.299; kb=0.114; break;
			case 2 : kr=0.212; kb=0.087; break;
			case 3 : kr=0.3; kb=0.11; break;
			case 4 : kr=0.2627; kb=0.0593; break;
		}
		kg=1.0-kr-kb;
	}
	dl.Min_Y=Min_Y;
	dl.Max_Y=Max_Y;
	dl.Min_U=Min_U;
	dl.Max_U=Max_U;
	dl.Min_V=Min_V;
	dl.Max_V=Max_V;
	dl.Coeff_Y=Coeff_Y;
	dl.Coeff_U=Coeff_U;
	dl.Coeff_V=Coeff_V;

	Rv=2.0*(1.0-kr);
	Gu=-2.0*kb*(1.0-kb)/kg;
	Gv=-2.0*kr*(1.0-kr)/kg;
	Bu=2.0*(1.0-kb);

	Ur=-0.5*kr/(1.0-kb);
	Ug=-0.5*kg/(1.0-kb);
	Ub=0.5;
	Vr=0.5;
	Vg=-0.5*kg/(1.0-kr);
	Vb=-0.5*kb/(1.0-kr);

	/*
	R',G',B' : RGB value normalised between (0.0,0.0,0.0)[Black] to (1.0,1.0,1.0)[White]
	Y' between (0.0 / +1.0) and Pb/Pr between (-0.5 / +0.5)

	Y'=Kr*R'+Kg*G'+Kb*B'
	Pb=0.5*(B'-Y')/(1-Kb)=-0.5*Kr/(1-Kb))*R'-0.5*Kg/(1-Kb)*G'+0.5*B'
	Pr=0.5*(R'-Y')/(1-Kr)=0.5*R'-0.5*Kg/(1-Kr)*G'-0.5*Kb/(1-Kr)*B'

	R'=Y'+2*(1-Kr)*Pr
	G'=Y'-(2*Kb*(1-Kb)/Kg)*Pb-(2*Kr*(1-Kr)/Kg)*Pr
	B'=Y'+2*(1-Kb)*Pb

	Status with actual values here.
	(Y) Y'=Kr*R'+Kg*G'+Kb*B'
	(U) Pb=(u1*R'+u2*G'+B')*0.5
	(V) Pr=(R'+v1*G'+v2*B')*0.5

	(R) R'=Y'+r1*Pb
	(G) G'=Y'+g1*Pb+g2*Pr
	(B) B'=Y'+b1*Pb

	For 8 bits data limited range :
	(Y',Cb,Cr) = (16,128,128) + (219*Y',224*Pb,224*Pr)

	Y = Off_Y + (Kr*R+Kg*G+Kb*B)*Coeff_Y
	(Cb) U = Off_U + (Ur*R+Ug*G+Ub*B)*Coeff_U
	(Cr) V = Off_V + (Vr*R+Vg*G+Vb*B)*Coeff_V

	R = Y/Coeff_Y + Rv*V/Coeff_V + Off_R [ Off_R = -(Off_Y/Coeff_Y+Rv*Off_V/Coeff_V) ]
	G = Y/Coeff_Y + Gu*U/Coeff_U + Gv*V/Coeff_V + Off_G [ Off_G = -(Off_Y/Coeff_Y+Gu*Off_U/Coeff_U+Gv*Off_V/Coeff_V) ]
	B = Y/Coeff_Y + Bu*U/Coeff_U + Off_B [ Off_B = -(Off_Y/Coeff_Y+Bu*Off_U/Coeff_U) ]
	*/

	Off_Y=Min_Y;
	Off_U=128 << pshift1;
	Off_V=128 << pshift1;

	Offset_Y=(Off_Y << 8)+128;
	Offset_U=(Off_U << 8)+128;
	Offset_V=(Off_V << 8)+128;

	Offset_R=(int32_t)-round(coeff_mul*(0.5+Off_Y/Coeff_Y+Rv*Off_V/Coeff_V));
	Offset_G=(int32_t)-round(coeff_mul*(0.5+Off_Y/Coeff_Y+Gu*Off_U/Coeff_U+Gv*Off_V/Coeff_V));
	Offset_B=(int32_t)-round(coeff_mul*(0.5+Off_Y/Coeff_Y+Bu*Off_U/Coeff_U));

	dl.Offset_Y=Offset_Y;
	dl.Offset_U=Offset_U;
	dl.Offset_V=Offset_V;
	dl.Offset_R=Offset_R;
	dl.Offset_G=Offset_G;
	dl.Offset_B=Offset_B;

	switch (convertion_mode)
	{
		case 0 :
			for (int32_t i=0; i<65536; i++)
			{
				Tlookup16[i]=(int32_t)round(i*coeff_div*kr*Coeff_Y*256.0);
				Tlookup16[i+65536]=(int32_t)round(i*coeff_div*kg*Coeff_Y*256.0);
				Tlookup16[i+2*65536]=(int32_t)round(i*coeff_div*kb*Coeff_Y*256.0);
				Tlookup16[i+3*65536]=(int32_t)round(i*coeff_div*Ur*Coeff_U*256.0);
				Tlookup16[i+4*65536]=(int32_t)round(i*coeff_div*Ug*Coeff_U*256.0);
				Tlookup16[i+5*65536]=(int32_t)round(i*coeff_div*Ub*Coeff_U*256.0);
				Tlookup16[i+6*65536]=(int32_t)round(i*coeff_div*Vr*Coeff_V*256.0);
				Tlookup16[i+7*65536]=(int32_t)round(i*coeff_div*Vg*Coeff_V*256.0);
				Tlookup16[i+8*65536]=(int32_t)round(i*coeff_div*Vb*Coeff_V*256.0);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
			for (int32_t i=0; i<vmax; i++)
			{
				Tlookup16[i]=(int32_t)round(i*coeff_mul/Coeff_Y);
				Tlookup16[i+vmax]=(int32_t)round(i*coeff_mul*Rv/Coeff_V);
				Tlookup16[i+2*vmax]=(int32_t)round(i*coeff_mul*Gu/Coeff_U);
				Tlookup16[i+3*vmax]=(int32_t)round(i*coeff_mul*Gv/Coeff_V);
				Tlookup16[i+4*vmax]=(int32_t)round(i*coeff_mul*Bu/Coeff_U);
			}
			break;
	}
}


void JPSDR_RGBConvert::Compute_Lookup8(void)
{
	double kr,kg,kb;
	double Rv,Gu,Gv,Bu;
	double Ur,Ug,Ub,Vr,Vg,Vb;
	int32_t Off_Y,Off_U,Off_V;

	if (mData.auto_detect && (convertion_mode!=0))
	{
		Min_Y=image_data.src_Y_Min;
		Max_Y=image_data.src_Y_Max;
		Min_U=image_data.src_U_Min;
		Max_U=image_data.src_U_Max;
		Min_V=image_data.src_V_Min;
		Max_V=image_data.src_V_Max;
		Coeff_Y=image_data.src_Coeff_Y;
		Coeff_U=image_data.src_Coeff_U;
		Coeff_V=image_data.src_Coeff_V;
		kr=image_data.src_Kr;
		kg=image_data.src_Kg;
		kb=image_data.src_Kb;
	}
	else
	{
		if (mData.full_range)
		{
			Min_Y=0;
			Max_Y=255;
			Min_U=0;
			Max_U=255;
			Min_V=0;
			Max_V=255;
			Coeff_Y=1.0;
			Coeff_U=1.0;
			Coeff_V=1.0;
		}
		else
		{
			Min_Y=16;
			Max_Y=235;
			Min_U=16;
			Max_U=240;
			Min_V=16;
			Max_V=240;
			Coeff_Y=219.0/255.0;
			Coeff_U=224.0/255.0;
			Coeff_V=224.0/225.0;
		}	
		switch (mData.color_matrix)
		{
			case 0 : kr=0.2126; kb=0.0722; break;
			case 1 : kr=0.299; kb=0.114; break;
			case 2 : kr=0.212; kb=0.087; break;
			case 3 : kr=0.3; kb=0.11; break;
			case 4 : kr=0.2627; kb=0.0593; break;
		}
		kg=1.0-kr-kb;
	}
	dl.Min_Y=Min_Y;
	dl.Max_Y=Max_Y;
	dl.Min_U=Min_U;
	dl.Max_U=Max_U;
	dl.Min_V=Min_V;
	dl.Max_V=Max_V;
	dl.Coeff_Y=Coeff_Y;
	dl.Coeff_U=Coeff_U;
	dl.Coeff_V=Coeff_V;

	Rv=2.0*(1.0-kr);
	Gu=-2.0*kb*(1.0-kb)/kg;
	Gv=-2.0*kr*(1.0-kr)/kg;
	Bu=2.0*(1.0-kb);

	Ur=-0.5*kr/(1.0-kb);
	Ug=-0.5*kg/(1.0-kb);
	Ub=0.5;
	Vr=0.5;
	Vg=-0.5*kg/(1.0-kr);
	Vb=-0.5*kb/(1.0-kr);

	/*
	R',G',B' : RGB value normalised between (0.0,0.0,0.0)[Black] to (1.0,1.0,1.0)[White]
	Y' between (0.0 / +1.0) and Pb/Pr between (-0.5 / +0.5)

	Y'=Kr*R'+Kg*G'+Kb*B'
	Pb=0.5*(B'-Y')/(1-Kb)=-0.5*Kr/(1-Kb))*R'-0.5*Kg/(1-Kb)*G'+0.5*B'
	Pr=0.5*(R'-Y')/(1-Kr)=0.5*R'-0.5*Kg/(1-Kr)*G'-0.5*Kb/(1-Kr)*B'

	R'=Y'+2*(1-Kr)*Pr
	G'=Y'-(2*Kb*(1-Kb)/Kg)*Pb-(2*Kr*(1-Kr)/Kg)*Pr
	B'=Y'+2*(1-Kb)*Pb

	Status with actual values here.
	(Y) Y'=Kr*R'+Kg*G'+Kb*B'
	(U) Pb=(u1*R'+u2*G'+B')*0.5
	(V) Pr=(R'+v1*G'+v2*B')*0.5

	(R) R'=Y'+r1*Pb
	(G) G'=Y'+g1*Pb+g2*Pr
	(B) B'=Y'+b1*Pb

	For 8 bits data limited range :
	(Y',Cb,Cr) = (16,128,128) + (219*Y',224*Pb,224*Pr)

	Y = Off_Y + (Kr*R+Kg*G+Kb*B)*Coeff_Y
	(Cb) U = Off_U + (Ur*R+Ug*G+Ub*B)*Coeff_U
	(Cr) V = Off_V + (Vr*R+Vg*G+Vb*B)*Coeff_V

	R = Y/Coeff_Y + Rv*V/Coeff_V + Off_R [ Off_R = -(Off_Y/Coeff_Y+Rv*Off_V/Coeff_V) ]
	G = Y/Coeff_Y + Gu*U/Coeff_U + Gv*V/Coeff_V + Off_G [ Off_G = -(Off_Y/Coeff_Y+Gu*Off_U/Coeff_U+Gv*Off_V/Coeff_V) ]
	B = Y/Coeff_Y + Bu*U/Coeff_U + Off_B [ Off_B = -(Off_Y/Coeff_Y+Bu*Off_U/Coeff_U) ]
	*/

	Off_Y=Min_Y;
	Off_U=128;
	Off_V=128;

	Offset_Y=(Off_Y << 6)+32;
	Offset_U=(Off_U << 6)+32;
	Offset_V=(Off_V << 6)+32;

	Offset_R=(int32_t)-round(16.0+32.0*(Off_Y/Coeff_Y+Rv*Off_V/Coeff_V));
	Offset_G=(int32_t)-round(16.0+32.0*(Off_Y/Coeff_Y+Gu*Off_U/Coeff_U+Gv*Off_V/Coeff_V));
	Offset_B=(int32_t)-round(16.0+32.0*(Off_Y/Coeff_Y+Bu*Off_U/Coeff_U));

	dl.Offset_Y=Offset_Y;
	dl.Offset_U=Offset_U;
	dl.Offset_V=Offset_V;
	dl.Offset_R=Offset_R;
	dl.Offset_G=Offset_G;
	dl.Offset_B=Offset_B;

	switch (convertion_mode)
	{
		case 0 :
			for (int16_t i=0; i<=255; i++)
			{
				Tlookup[i]=(int16_t)round(i*kr*Coeff_Y*64.0);
				Tlookup[i+256]=(int16_t)round(i*kg*Coeff_Y*64.0);
				Tlookup[i+512]=(int16_t)round(i*kb*Coeff_Y*64.0);
				Tlookup[i+768]=(int16_t)round(i*Ur*Coeff_U*64.0);
				Tlookup[i+1024]=(int16_t)round(i*Ug*Coeff_U*64.0);
				Tlookup[i+1280]=(int16_t)round(i*Ub*Coeff_U*64.0);
				Tlookup[i+1536]=(int16_t)round(i*Vr*Coeff_V*64.0);
				Tlookup[i+1792]=(int16_t)round(i*Vg*Coeff_V*64.0);
				Tlookup[i+2048]=(int16_t)round(i*Vb*Coeff_V*64.0);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
			for (int16_t i=0; i<=255; i++)
			{
				Tlookup[i]=(int16_t)round(i*32.0/Coeff_Y);
				Tlookup[i+256]=(int16_t)round(i*Rv*32.0/Coeff_V);
				Tlookup[i+512]=(int16_t)round(i*Gu*32.0/Coeff_U);
				Tlookup[i+768]=(int16_t)round(i*Gv*32.0/Coeff_V);
				Tlookup[i+1024]=(int16_t)round(i*Bu*32.0/Coeff_U);
			}
			break;
	}
}


uint32 JPSDR_RGBConvert::GetParams()
{
	if (g_VFVAPIVersion<12) return(FILTERPARAM_NOT_SUPPORTED);

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
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :*/
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :*/
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar16 :
/*		case nsVDXPixmap::kPixFormat_Y16 :
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

	const uint8_t bit_pixel=GetBitDepth(&fa->src);

	image_data.src_full_mode=CheckFullRangeMode(&fa->src);
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);
	
	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_XRGB8888 :
			convertion_mode=0;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YU64 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
			convertion_mode=1;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
			convertion_mode=2;
			break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
			convertion_mode=3;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
			convertion_mode=4;
			break;
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
			convertion_mode=5;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
			convertion_mode=6;
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
					if (bit_pixel>8) pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar16;
					else
					{
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
					}
					break;			
				case 1 :
					if (bit_pixel>8) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar16;
					else
					{
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
					}
					break;			
				case 2 :
					if (bit_pixel>8) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YU64;
					else
					{
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
					}
					break;			
				case 3 :
					if (bit_pixel>8) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar16;
					else
					{
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
					}
					break;			
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
			if (bit_pixel>8) pxdst.format=nsVDXPixmap::kPixFormat_XRGB64;
			else pxdst.format=nsVDXPixmap::kPixFormat_XRGB8888;
			break;
		default : break;
	}

	pxdst.pitch=0;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	if (VDub2_Enable)
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo((fa->dst).mpPixmap);

		switch(convertion_mode)
		{
			case 0 :
				if ((mData.color_matrix==0) || (mData.color_matrix==2))
					info->colorSpaceMode=nsVDXPixmap::kColorSpaceMode_709;
				else info->colorSpaceMode=nsVDXPixmap::kColorSpaceMode_601;
				if (mData.full_range) info->colorRangeMode=nsVDXPixmap::kColorRangeMode_Full;
				else info->colorRangeMode=nsVDXPixmap::kColorRangeMode_Limited;
				break;
			case 1 :
			case 2 :
			case 3 :
			case 4 :
			case 5 :
			case 6 :
				info->colorSpaceMode=nsVDXPixmap::kColorSpaceMode_None;
				info->colorRangeMode=nsVDXPixmap::kColorRangeMode_None;
				break;
			default : break;
		}
	}

	if (VDub2_Enable)
		return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES_32);
	else
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
	}
}


void JPSDR_RGBConvert::Start()
{
	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}

	if (!poolInterface->GetThreadPoolInterfaceStatus())
	{
		ff->Except("Error with the TheadPool status!");
		return;
	}

	if ((total_cpu>1) && (UserId==0))
	{
		ff->Except("Error with the TheadPool getting UserId!");
		return;
	}

	SetImageData(image_data);

	bits_per_pixel=image_data.src_bits_pixel;
	const uint32_t vmax=1 << bits_per_pixel;

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	for(uint16_t i=0; i<256; i++)
		lookup_Upscale8[i]=3*i+2;

	for (uint32_t i=0; i<vmax; i++)
		lookup_Upscale16[i]=3*i+2;

	if  (mData.mt_mode && (image_data.src_h0>=32) && (image_data.dst_h0>=32))
	{
		for(uint8_t i=0; i<3; i++)
			threads_number[i]=total_cpu;
	}
	else
	{
		for(uint8_t i=0; i<3; i++)
			threads_number[i]=1;
	}

	const ptrdiff_t line0=(bits_per_pixel==8)?image_data.dst_w0:2*image_data.dst_w0;

	switch(convertion_mode)
	{
		case 0 :
			switch(mData.output_mode)
			{
				case 1 :
					pitch444=((line0+31)>>5)<<5;
					modulo444=pitch444-line0;
					BufferU_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
					BufferV_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
					if ((BufferU_444==NULL) || (BufferV_444==NULL))
					{
						ff->ExceptOutOfMemory();
						return;
					}
					break;
				case 3 :
					pitch444=((line0+31)>>5)<<5;
					pitch422=(((line0>>1)+31)>>5)<<5;
					modulo444=pitch444-line0;
					modulo422=pitch422-(line0>>1);
					BufferU_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
					BufferV_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
					BufferU_422=(uint8_t *)_aligned_malloc(pitch422*image_data.dst_h0,64);
					BufferV_422=(uint8_t *)_aligned_malloc(pitch422*image_data.dst_h0,64);
					if ((BufferU_444==NULL) || (BufferV_444==NULL) || (BufferV_422==NULL) || (BufferV_422==NULL))
					{
						ff->ExceptOutOfMemory();
						return;
					}
					break;
				default : break;
			}
			break;
		case 4 :
			pitch444=((line0+31)>>5)<<5;
			modulo444=pitch444-line0;
			BufferU_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
			BufferV_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
			if ((BufferU_444==NULL) || (BufferV_444==NULL))
			{
				ff->ExceptOutOfMemory();
				return;
			}
			break;
		case 6 :
			pitch444=((line0+31)>>5)<<5;
			pitch422=(((line0>>1)+31)>>5)<<5;
			modulo444=pitch444-line0;
			modulo422=pitch422-(line0>>1);
			BufferU_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
			BufferV_444=(uint8_t *)_aligned_malloc(pitch444*image_data.dst_h0,64);
			BufferU_422=(uint8_t *)_aligned_malloc(pitch422*image_data.dst_h0,64);
			BufferV_422=(uint8_t *)_aligned_malloc(pitch422*image_data.dst_h0,64);
			if ((BufferU_444==NULL) || (BufferV_444==NULL) || (BufferV_422==NULL) || (BufferV_422==NULL))
			{
				ff->ExceptOutOfMemory();
				return;
			}
			break;
		default : break;
	}

	if (convertion_mode==0)
	{
		threads_number[0]=CreateMTData(MT_Data[0],threads_number[0],threads_number[0],
			image_data.src_w0,image_data.src_h0,false,false,false,false);
		threads_number[1]=CreateMTData(MT_Data[1],threads_number[1],threads_number[1],
			image_data.src_w0,image_data.src_h0,false,false,true,false);
		threads_number[2]=CreateMTData(MT_Data[2],threads_number[2],threads_number[2],
			image_data.src_w0,image_data.src_h0,true,false,true,true);
	}
	else
	{
		threads_number[0]=CreateMTData(MT_Data[0],threads_number[0],threads_number[0],
			image_data.src_w0,image_data.src_h0,true,true,true,false);
		threads_number[1]=CreateMTData(MT_Data[1],threads_number[1],threads_number[1],
			image_data.src_w0,image_data.src_h0,true,false,false,false);
		threads_number[2]=CreateMTData(MT_Data[2],threads_number[2],threads_number[2],
			image_data.src_w0,image_data.src_h0,false,false,false,false);
	}

	max_threads=threads_number[0];
	for(uint8_t i=1; i<3; i++)
		if (max_threads<threads_number[i]) max_threads=threads_number[i];

	if (max_threads>1)
	{
		if (!threadpoolAllocated)
		{
			ff->Except("Error with the TheadPool while allocating threadpool!");
			return;
		}
	}

	if (bits_per_pixel==8) Compute_Lookup8();
	else Compute_Lookup16();
}


void JPSDR_RGBConvert::End()
{
	my_aligned_free(BufferV_422);
	my_aligned_free(BufferU_422);
	my_aligned_free(BufferV_444);
	my_aligned_free(BufferU_444);
}


void JPSDR_RGBConvert::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];
	int8_t color_matrix;
	bool full_range;

	if (mData.auto_detect && (convertion_mode!=0))
	{
		full_range=image_data.src_full_mode;
		switch(image_data.src_color_matrix)
		{
			case MATRIX_BT601 : color_matrix=1; break;
			case MATRIX_BT709 : color_matrix=0; break;
			case MATRIX_BT2020 : color_matrix=4; break;
			default : color_matrix=1; break;
		}
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
		case 4 : strcpy_s(output_string,255," Color matrix : BT.2020"); break;
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


void JPSDR_RGBConvert::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_RGBConvert *ptrClass=(JPSDR_RGBConvert *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : Convert_RGB32toYV24_AVX(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 2 : Convert_RGB32toYV24_SSE2(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 3 : Convert_RGB32toYV24(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 4 : Convert_RGB32toYUY2_AVX(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 5 : Convert_RGB32toYUY2_SSE2(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 6 : Convert_RGB32toYUY2(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 7 : Convert_YV24toRGB32_AVX(ptrClass->MT_Data[2][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 8 : Convert_YV24toRGB32_SSE2(ptrClass->MT_Data[2][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 9 : Convert_YV24toRGB32(ptrClass->MT_Data[2][data->thread_Id],ptrClass->dl,ptrClass->Tlookup); break;
		case 10 : Convert_8_YV16toYV24_AVX(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 11 : Convert_8_YV16toYV24_SSE2(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 12 : Convert_8_YV16toYV24(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 13 : Convert_Progressive_8_YV12toYV16_AVX(ptrClass->MT_Data[0][data->thread_Id]); break;
		case 14 : Convert_Progressive_8_YV12toYV16_SSE2(ptrClass->MT_Data[0][data->thread_Id]); break;
		case 15 : Convert_Progressive_8_YV12toYV16(ptrClass->MT_Data[0][data->thread_Id],ptrClass->lookup_Upscale8); break;
		case 16 : Convert_Planar444toPlanar422_8_AVX(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 17 : Convert_Planar444toPlanar422_8_SSE2(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 18 : Convert_Planar444toPlanar422_8(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 20 : Convert_Planar422toPlanar420_8_AVX(ptrClass->MT_Data[2][data->thread_Id]); break;
		case 21 : Convert_Planar422toPlanar420_8_SSE2(ptrClass->MT_Data[2][data->thread_Id]); break;
		case 22 : Convert_Planar422toPlanar420_8(ptrClass->MT_Data[2][data->thread_Id]); break;

		case 24 : Convert_RGB64toYV24_AVX(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16); break;
		case 25 : Convert_RGB64toYV24_SSE41(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16); break;
		case 26 : Convert_RGB64toYV24(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16); break;
		case 27 : Convert_RGB64toYU64_AVX(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16); break;
		case 28 : Convert_RGB64toYU64_SSE41(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16); break;
		case 29 : Convert_RGB64toYU64(ptrClass->MT_Data[0][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16); break;
		case 30 : Convert_YV24toRGB64_AVX(ptrClass->MT_Data[2][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16,
					  ptrClass->bits_per_pixel); break;
		case 31 : Convert_YV24toRGB64_SSE41(ptrClass->MT_Data[2][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16,
					  ptrClass->bits_per_pixel); break;
		case 32 : Convert_YV24toRGB64(ptrClass->MT_Data[2][data->thread_Id],ptrClass->dl,ptrClass->Tlookup16,
					   ptrClass->bits_per_pixel); break;
		case 33 : Convert_16_YV16toYV24_AVX(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 34 : Convert_16_YV16toYV24_SSE2(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 35 : Convert_16_YV16toYV24(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 36 : Convert_Progressive_16_YV12toYV16_AVX(ptrClass->MT_Data[0][data->thread_Id]); break;
		case 37 : Convert_Progressive_16_YV12toYV16_SSE2(ptrClass->MT_Data[0][data->thread_Id]); break;
		case 38 : Convert_Progressive_16_YV12toYV16(ptrClass->MT_Data[0][data->thread_Id],ptrClass->lookup_Upscale16); break;
		case 39 : Convert_Planar444toPlanar422_16_AVX(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 40 : Convert_Planar444toPlanar422_16_SSE41(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 41 : Convert_Planar444toPlanar422_16(ptrClass->MT_Data[1][data->thread_Id]); break;
		case 43 : Convert_Planar422toPlanar420_16_AVX(ptrClass->MT_Data[2][data->thread_Id]); break;
		case 44 : Convert_Planar422toPlanar420_16_SSE2(ptrClass->MT_Data[2][data->thread_Id]); break;
		case 45 : Convert_Planar422toPlanar420_16(ptrClass->MT_Data[2][data->thread_Id]); break;
#ifdef AVX2_BUILD_POSSIBLE
		case 19 : Convert_Planar422toPlanar420_8_AVX2(ptrClass->MT_Data[2][data->thread_Id]); break;
		case 23 :  Convert_Progressive_8_YV12toYV16_AVX2(ptrClass->MT_Data[0][data->thread_Id]); break;
		case 42 : Convert_Planar422toPlanar420_16_AVX2(ptrClass->MT_Data[2][data->thread_Id]); break;
		case 46 :  Convert_Progressive_16_YV12toYV16_AVX2(ptrClass->MT_Data[0][data->thread_Id]); break;
#endif
		default : ;
	}
}


void JPSDR_RGBConvert::Run()
{
	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	if (max_threads>1)
	{
		if (!poolInterface->RequestThreadPool(UserId,max_threads,MT_Thread,-1,false)) return;
	}

	if (VDub2_Enable)
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo((fa->dst).mpPixmap);

		switch(bits_per_pixel)
		{
			case 8 :
				info->ref_r=0xFF;
				info->ref_g=0xFF;
				info->ref_b=0xFF;
				info->ref_a=0xFF;
				break;
			default :
				info->ref_r=0xFFFF;
				info->ref_g=0xFFFF;
				info->ref_b=0xFFFF;
				info->ref_a=0xFFFF;
				break;
		}
	}

	uint8_t f_proc=0,nb_thread;
	const uint8_t offset_proc=(bits_per_pixel>8)?23:0;

	if (convertion_mode==0)
	{
		switch(mData.output_mode)
		{
			case 0 :
				for (uint8_t i=0; i<threads_number[0]; i++)
				{
					MT_Data[0][i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[0][i].src_Y_h_min));
					MT_Data[0][i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[0][i].dst_Y_h_min));
					MT_Data[0][i].dst2=(void *)((uint8_t *)image_data.dst_plane1+(image_data.dst_pitch1*MT_Data[0][i].dst_UV_h_min));
					MT_Data[0][i].dst3=(void *)((uint8_t *)image_data.dst_plane2+(image_data.dst_pitch2*MT_Data[0][i].dst_UV_h_min));
					MT_Data[0][i].src_pitch1=image_data.src_pitch0;
					MT_Data[0][i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[0][i].dst_pitch2=image_data.dst_pitch1;
					MT_Data[0][i].dst_pitch3=image_data.dst_pitch2;
					MT_Data[0][i].src_modulo1=image_data.src_modulo0;
					MT_Data[0][i].dst_modulo1=image_data.dst_modulo0;
					MT_Data[0][i].dst_modulo2=image_data.dst_modulo1;
					MT_Data[0][i].dst_modulo3=image_data.dst_modulo2;
				}
				if (AVX_Enable) f_proc=1+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=2+offset_proc;
					else f_proc=3+offset_proc;
				}
				nb_thread=threads_number[0];
				break;
			case 2 :
				for (uint8_t i=0; i<threads_number[0]; i++)
				{
					MT_Data[0][i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[0][i].src_Y_h_min));
					MT_Data[0][i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[0][i].dst_Y_h_min));
					MT_Data[0][i].src_pitch1=image_data.src_pitch0;
					MT_Data[0][i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[0][i].src_modulo1=image_data.src_modulo0;
					MT_Data[0][i].dst_modulo1=image_data.dst_modulo0;
				}
				if (AVX_Enable) f_proc=4+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=5+offset_proc;
					else f_proc=6+offset_proc;
				}
				nb_thread=threads_number[0];
				break;
			case 1 :
			case 3 :
				for (uint8_t i=0; i<threads_number[0]; i++)
				{
					MT_Data[0][i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[0][i].src_Y_h_min));
					MT_Data[0][i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[0][i].dst_Y_h_min));
					MT_Data[0][i].dst2=(void *)(BufferU_444+(pitch444*MT_Data[0][i].dst_UV_h_min));
					MT_Data[0][i].dst3=(void *)(BufferV_444+(pitch444*MT_Data[0][i].dst_UV_h_min));
					MT_Data[0][i].src_pitch1=image_data.src_pitch0;
					MT_Data[0][i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[0][i].dst_pitch2=pitch444;
					MT_Data[0][i].dst_pitch3=pitch444;
					MT_Data[0][i].src_modulo1=image_data.src_modulo0;
					MT_Data[0][i].dst_modulo1=image_data.dst_modulo0;
					MT_Data[0][i].dst_modulo2=modulo444;
					MT_Data[0][i].dst_modulo3=modulo444;
				}
				if (AVX_Enable) f_proc=1+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=2+offset_proc;
					else f_proc=3+offset_proc;
				}
				nb_thread=threads_number[0];
				break;
			default : f_proc=0; break;
		}
	}
	else
	{
		switch(convertion_mode)
		{
			case 3 :
				for (uint8_t i=0; i<threads_number[2]; i++)
				{
					MT_Data[2][i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[2][i].src_Y_h_min));
					MT_Data[2][i].src2=(void *)((uint8_t *)image_data.src_plane1+(image_data.src_pitch1*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].src3=(void *)((uint8_t *)image_data.src_plane2+(image_data.src_pitch2*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[2][i].dst_Y_h_min));
					MT_Data[2][i].src_pitch1=image_data.src_pitch0;
					MT_Data[2][i].src_pitch2=image_data.src_pitch1;
					MT_Data[2][i].src_pitch3=image_data.src_pitch2;
					MT_Data[2][i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[2][i].src_modulo1=image_data.src_modulo0;
					MT_Data[2][i].src_modulo2=image_data.src_modulo1;
					MT_Data[2][i].src_modulo3=image_data.src_modulo2;
					MT_Data[2][i].dst_modulo1=image_data.dst_modulo0;
				}
				if (AVX_Enable) f_proc=7+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=8+offset_proc;
					else f_proc=9+offset_proc;
				}
				nb_thread=threads_number[2];
				break;
			case 4 :
				for (uint8_t i=0; i<threads_number[1]; i++)
				{
					MT_Data[1][i].src2=(void *)((uint8_t *)image_data.src_plane1+(image_data.src_pitch1*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].src3=(void *)((uint8_t *)image_data.src_plane2+(image_data.src_pitch2*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].dst2=(void *)(BufferU_444+(pitch444*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].dst3=(void *)(BufferV_444+(pitch444*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].src_pitch2=image_data.src_pitch1;
					MT_Data[1][i].src_pitch3=image_data.src_pitch2;
					MT_Data[1][i].dst_pitch2=pitch444;
					MT_Data[1][i].dst_pitch3=pitch444;
					MT_Data[1][i].src_modulo2=image_data.src_modulo1;
					MT_Data[1][i].src_modulo3=image_data.src_modulo2;
					MT_Data[1][i].dst_modulo2=modulo444;
					MT_Data[1][i].dst_modulo3=modulo444;
				}
				if (AVX_Enable) f_proc=10+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=11+offset_proc;
					else f_proc=12+offset_proc;
				}
				nb_thread=threads_number[1];
				break;
			case 6 :
				for (uint8_t i=0; i<threads_number[0]; i++)
				{
					MT_Data[0][i].src2=(void *)((uint8_t *)image_data.src_plane1+(image_data.src_pitch1*MT_Data[0][i].src_UV_h_min));
					MT_Data[0][i].src3=(void *)((uint8_t *)image_data.src_plane2+(image_data.src_pitch2*MT_Data[0][i].src_UV_h_min));
					MT_Data[0][i].dst2=(void *)(BufferU_422+(pitch422*MT_Data[0][i].dst_UV_h_min));
					MT_Data[0][i].dst3=(void *)(BufferV_422+(pitch422*MT_Data[0][i].dst_UV_h_min));
					MT_Data[0][i].src_pitch2=image_data.src_pitch1;
					MT_Data[0][i].src_pitch3=image_data.src_pitch2;
					MT_Data[0][i].dst_pitch2=pitch422;
					MT_Data[0][i].dst_pitch3=pitch422;
					MT_Data[0][i].src_modulo2=image_data.src_modulo1;
					MT_Data[0][i].src_modulo3=image_data.src_modulo2;
					MT_Data[0][i].dst_modulo2=modulo422;
					MT_Data[0][i].dst_modulo3=modulo422;
				}
#ifdef AVX2_BUILD_POSSIBLE
				if (AVX2_Enable) f_proc=23+offset_proc;
				else
#endif
				{
					if (AVX_Enable) f_proc=13+offset_proc;
					else
					{
						if (SSE2_Enable || SSE41_Enable) f_proc=14+offset_proc;
						else f_proc=15+offset_proc;
					}
				}
				nb_thread=threads_number[0];
				break;
			default : f_proc=0; break;
		}
	}

	if (f_proc!=0)
	{
		if (nb_thread>1)
		{
			for(uint8_t i=0; i<nb_thread; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<nb_thread; i++)
				MT_Thread[i].f_process=0;
		}
		else
		{
			switch(f_proc)
			{
				case 1 : Convert_RGB32toYV24_AVX(MT_Data[0][0],dl,Tlookup); break;
				case 2 : Convert_RGB32toYV24_SSE2(MT_Data[0][0],dl,Tlookup); break;
				case 3 : Convert_RGB32toYV24(MT_Data[0][0],dl,Tlookup); break;
				case 4 : Convert_RGB32toYUY2_AVX(MT_Data[0][0],dl,Tlookup); break;
				case 5 : Convert_RGB32toYUY2_SSE2(MT_Data[0][0],dl,Tlookup); break;
				case 6 : Convert_RGB32toYUY2(MT_Data[0][0],dl,Tlookup); break;
				case 7 : Convert_YV24toRGB32_AVX(MT_Data[2][0],dl,Tlookup); break;
				case 8 : Convert_YV24toRGB32_SSE2(MT_Data[2][0],dl,Tlookup); break;
				case 9 : Convert_YV24toRGB32(MT_Data[2][0],dl,Tlookup); break;
				case 10 : Convert_8_YV16toYV24_AVX(MT_Data[1][0]); break;
				case 11 : Convert_8_YV16toYV24_SSE2(MT_Data[1][0]); break;
				case 12 : Convert_8_YV16toYV24(MT_Data[1][0]); break;
				case 13 : Convert_Progressive_8_YV12toYV16_AVX(MT_Data[0][0]); break;
				case 14 : Convert_Progressive_8_YV12toYV16_SSE2(MT_Data[0][0]); break;
				case 15 : Convert_Progressive_8_YV12toYV16(MT_Data[0][0],lookup_Upscale8); break;

				case 24 : Convert_RGB64toYV24_AVX(MT_Data[0][0],dl,Tlookup16); break;
				case 25 : Convert_RGB64toYV24_SSE41(MT_Data[0][0],dl,Tlookup16); break;
				case 26 : Convert_RGB64toYV24(MT_Data[0][0],dl,Tlookup16); break;
				case 27 : Convert_RGB64toYU64_AVX(MT_Data[0][0],dl,Tlookup16); break;
				case 28 : Convert_RGB64toYU64_SSE41(MT_Data[0][0],dl,Tlookup16); break;
				case 29 : Convert_RGB64toYU64(MT_Data[0][0],dl,Tlookup16); break;
				case 30 : Convert_YV24toRGB64_AVX(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 31 : Convert_YV24toRGB64_SSE41(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 32 : Convert_YV24toRGB64(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 33 : Convert_16_YV16toYV24_AVX(MT_Data[1][0]); break;
				case 34 : Convert_16_YV16toYV24_SSE2(MT_Data[1][0]); break;
				case 35 : Convert_16_YV16toYV24(MT_Data[1][0]); break;
				case 36 : Convert_Progressive_16_YV12toYV16_AVX(MT_Data[0][0]); break;
				case 37 : Convert_Progressive_16_YV12toYV16_SSE2(MT_Data[0][0]); break;
				case 38 : Convert_Progressive_16_YV12toYV16(MT_Data[0][0],lookup_Upscale16); break;
#ifdef AVX2_BUILD_POSSIBLE
				case 23 : Convert_Progressive_8_YV12toYV16_AVX2(MT_Data[0][0]); break;
				case 46 : Convert_Progressive_16_YV12toYV16_AVX2(MT_Data[0][0]); break;
#endif
				default : break;
			}
		}
	}

	if (convertion_mode==0)
	{
		switch(mData.output_mode)
		{
			case 1 :
				for (uint8_t i=0; i<threads_number[1]; i++)
				{
					MT_Data[1][i].src1=(void *)(BufferU_444+(pitch444*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].src2=(void *)(BufferV_444+(pitch444*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].dst1=(void *)((uint8_t *)image_data.dst_plane1+(image_data.dst_pitch1*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].dst2=(void *)((uint8_t *)image_data.dst_plane2+(image_data.dst_pitch2*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].src_pitch1=pitch444;
					MT_Data[1][i].src_pitch2=pitch444;
					MT_Data[1][i].dst_pitch1=image_data.dst_pitch1;
					MT_Data[1][i].dst_pitch2=image_data.dst_pitch2;
					MT_Data[1][i].src_modulo1=modulo444;
					MT_Data[1][i].src_modulo2=modulo444;
					MT_Data[1][i].dst_modulo1=image_data.dst_modulo1;
					MT_Data[1][i].dst_modulo2=image_data.dst_modulo2;
				}
				if (AVX_Enable) f_proc=16+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=17+offset_proc;
					else f_proc=18+offset_proc;
				}
				nb_thread=threads_number[1];
				break;
			case 3 :
				for (uint8_t i=0; i<threads_number[1]; i++)
				{
					MT_Data[1][i].src1=(void *)(BufferU_444+(pitch444*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].src2=(void *)(BufferV_444+(pitch444*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].dst1=(void *)(BufferU_422+(pitch422*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].dst2=(void *)(BufferV_422+(pitch422*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].src_pitch1=pitch444;
					MT_Data[1][i].src_pitch2=pitch444;
					MT_Data[1][i].dst_pitch1=pitch422;
					MT_Data[1][i].dst_pitch2=pitch422;
					MT_Data[1][i].src_modulo1=modulo444;
					MT_Data[1][i].src_modulo2=modulo444;
					MT_Data[1][i].dst_modulo1=modulo422;
					MT_Data[1][i].dst_modulo2=modulo422;
				}
				if (AVX_Enable) f_proc=16+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=17+offset_proc;
					else f_proc=18+offset_proc;
				}
				nb_thread=threads_number[1];
				break;
			default : f_proc=0; break;
		}
	}
	else
	{
		switch(convertion_mode)
		{
			case 4 :
				for (uint8_t i=0; i<threads_number[2]; i++)
				{
					MT_Data[2][i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[2][i].src_Y_h_min));
					MT_Data[2][i].src2=(void *)(BufferU_444+(pitch444*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].src3=(void *)(BufferV_444+(pitch444*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[2][i].dst_Y_h_min));
					MT_Data[2][i].src_pitch1=image_data.src_pitch0;
					MT_Data[2][i].src_pitch2=pitch444;
					MT_Data[2][i].src_pitch3=pitch444;
					MT_Data[2][i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[2][i].src_modulo1=image_data.src_modulo0;
					MT_Data[2][i].src_modulo2=modulo444;
					MT_Data[2][i].src_modulo3=modulo444;
					MT_Data[2][i].dst_modulo1=image_data.dst_modulo0;
				}
				if (AVX_Enable) f_proc=7+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=8+offset_proc;
					else f_proc=9+offset_proc;
				}
				nb_thread=threads_number[2];
				break;
			case 6 :
				for (uint8_t i=0; i<threads_number[1]; i++)
				{
					MT_Data[1][i].src2=(void *)(BufferU_422+(pitch422*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].src3=(void *)(BufferV_422+(pitch422*MT_Data[1][i].src_UV_h_min));
					MT_Data[1][i].dst2=(void *)(BufferU_444+(pitch444*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].dst3=(void *)(BufferV_444+(pitch444*MT_Data[1][i].dst_UV_h_min));
					MT_Data[1][i].src_pitch2=pitch422;
					MT_Data[1][i].src_pitch3=pitch422;
					MT_Data[1][i].dst_pitch2=pitch444;
					MT_Data[1][i].dst_pitch3=pitch444;
					MT_Data[1][i].src_modulo2=modulo422;
					MT_Data[1][i].src_modulo3=modulo422;
					MT_Data[1][i].dst_modulo2=modulo444;
					MT_Data[1][i].dst_modulo3=modulo444;
				}
				if (AVX_Enable) f_proc=10+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=11+offset_proc;
					else f_proc=12+offset_proc;
				}
				nb_thread=threads_number[1];
				break;
			default : f_proc=0; break;
		}
	}

	if (f_proc!=0)
	{
		if (nb_thread>1)
		{
			for(uint8_t i=0; i<nb_thread; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<nb_thread; i++)
				MT_Thread[i].f_process=0;
		}
		else
		{
			switch(f_proc)
			{
				case 7 : Convert_YV24toRGB32_AVX(MT_Data[2][0],dl,Tlookup); break;
				case 8 : Convert_YV24toRGB32_SSE2(MT_Data[2][0],dl,Tlookup); break;
				case 9 : Convert_YV24toRGB32(MT_Data[2][0],dl,Tlookup); break;
				case 10 : Convert_8_YV16toYV24_AVX(MT_Data[1][0]); break;
				case 11 : Convert_8_YV16toYV24_SSE2(MT_Data[1][0]); break;
				case 12 : Convert_8_YV16toYV24(MT_Data[1][0]); break;
				case 16 : Convert_Planar444toPlanar422_8_AVX(MT_Data[1][0]); break;
				case 17 : Convert_Planar444toPlanar422_8_SSE2(MT_Data[1][0]); break;
				case 18 : Convert_Planar444toPlanar422_8(MT_Data[1][0]); break;

				case 30 : Convert_YV24toRGB64_AVX(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 31 : Convert_YV24toRGB64_SSE41(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 32 : Convert_YV24toRGB64(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 33 : Convert_16_YV16toYV24_AVX(MT_Data[1][0]); break;
				case 34 : Convert_16_YV16toYV24_SSE2(MT_Data[1][0]); break;
				case 35 : Convert_16_YV16toYV24(MT_Data[1][0]); break;
				case 39 : Convert_Planar444toPlanar422_16_AVX(MT_Data[1][0]); break;
				case 40 : Convert_Planar444toPlanar422_16_SSE41(MT_Data[1][0]); break;
				case 41 : Convert_Planar444toPlanar422_16(MT_Data[1][0]); break;
				default : break;
			}
		}
	}

	if (convertion_mode==0)
	{
		switch(mData.output_mode)
		{
			case 3 :
				for (uint8_t i=0; i<threads_number[2]; i++)
				{
					MT_Data[2][i].src1=(void *)(BufferU_422+(pitch422*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].src2=(void *)(BufferV_422+(pitch422*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].dst1=(void *)((uint8_t *)image_data.dst_plane1+(image_data.dst_pitch1*MT_Data[2][i].dst_UV_h_min));
					MT_Data[2][i].dst2=(void *)((uint8_t *)image_data.dst_plane2+(image_data.dst_pitch2*MT_Data[2][i].dst_UV_h_min));
					MT_Data[2][i].src_pitch1=pitch422;
					MT_Data[2][i].src_pitch2=pitch422;
					MT_Data[2][i].dst_pitch1=image_data.dst_pitch1;
					MT_Data[2][i].dst_pitch2=image_data.dst_pitch2;
					MT_Data[2][i].src_modulo1=modulo422;
					MT_Data[2][i].src_modulo2=modulo422;
					MT_Data[2][i].dst_modulo1=image_data.dst_modulo1;
					MT_Data[2][i].dst_modulo2=image_data.dst_modulo2;
				}
#ifdef AVX2_BUILD_POSSIBLE
				if (AVX2_Enable) f_proc=19+offset_proc;
				else
#endif
				{
					if (AVX_Enable) f_proc=20+offset_proc;
					else
					{
						if (SSE2_Enable || SSE41_Enable) f_proc=21+offset_proc;
						else f_proc=22+offset_proc;
					}
				}
				nb_thread=threads_number[2];
				break;
			default : f_proc=0; break;
		}
	}
	else
	{
		switch(convertion_mode)
		{
			case 6 :
				for (uint8_t i=0; i<threads_number[2]; i++)
				{
					MT_Data[2][i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[2][i].src_Y_h_min));
					MT_Data[2][i].src2=(void *)(BufferU_444+(pitch444*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].src3=(void *)(BufferV_444+(pitch444*MT_Data[2][i].src_UV_h_min));
					MT_Data[2][i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[2][i].dst_Y_h_min));
					MT_Data[2][i].src_pitch1=image_data.src_pitch0;
					MT_Data[2][i].src_pitch2=pitch444;
					MT_Data[2][i].src_pitch3=pitch444;
					MT_Data[2][i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[2][i].src_modulo1=image_data.src_modulo0;
					MT_Data[2][i].src_modulo2=modulo444;
					MT_Data[2][i].src_modulo3=modulo444;
					MT_Data[2][i].dst_modulo1=image_data.dst_modulo0;
				}
				if (AVX_Enable) f_proc=7+offset_proc;
				else
				{
					if (SSE2_Enable || SSE41_Enable) f_proc=8+offset_proc;
					else f_proc=9+offset_proc;
				}
				nb_thread=threads_number[2];
				break;
			default : f_proc=0; break;
		}
	}

	if (f_proc!=0)
	{
		if (nb_thread>1)
		{
			for(uint8_t i=0; i<nb_thread; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<nb_thread; i++)
				MT_Thread[i].f_process=0;
		}
		else
		{
			switch(f_proc)
			{
				case 7 : Convert_YV24toRGB32_AVX(MT_Data[2][0],dl,Tlookup); break;
				case 8 : Convert_YV24toRGB32_SSE2(MT_Data[2][0],dl,Tlookup); break;
				case 9 : Convert_YV24toRGB32(MT_Data[2][0],dl,Tlookup); break;
				case 20 : Convert_Planar422toPlanar420_8_AVX(MT_Data[2][0]); break;
				case 21 : Convert_Planar422toPlanar420_8_SSE2(MT_Data[2][0]); break;
				case 22 : Convert_Planar422toPlanar420_8(MT_Data[2][0]); break;

				case 30 : Convert_YV24toRGB64_AVX(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 31 : Convert_YV24toRGB64_SSE41(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 32 : Convert_YV24toRGB64(MT_Data[2][0],dl,Tlookup16,bits_per_pixel); break;
				case 43 : Convert_Planar422toPlanar420_16_AVX(MT_Data[2][0]); break;
				case 44 : Convert_Planar422toPlanar420_16_SSE2(MT_Data[2][0]); break;
				case 45 : Convert_Planar422toPlanar420_16(MT_Data[2][0]); break;
#ifdef AVX2_BUILD_POSSIBLE
				case 19 : Convert_Planar422toPlanar420_8_AVX2(MT_Data[2][0]); break;
				case 42 : Convert_Planar422toPlanar420_16_AVX2(MT_Data[2][0]); break;
#endif
				default : break;
			}
		}
	}

	if (max_threads>1) poolInterface->ReleaseThreadPool(UserId,false);
}


void JPSDR_RGBConvert::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.color_matrix=argv[0].asInt();
	mData.full_range=!!argv[1].asInt();
	mData.output_mode=argv[2].asInt();
	mData.auto_detect=!!argv[3].asInt();
	mData.mt_mode=!!argv[4].asInt();
}


bool JPSDR_RGBConvert::Configure(VDXHWND hwnd)
{
	JPSDR_RGBConvertDialog dlg(mData, fa->ifp2, this);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_RGBConvert::GetScriptString(char *buf, int maxlen)
{ 
 	SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d, %d)",mData.color_matrix,mData.full_range,
		mData.output_mode,mData.auto_detect,mData.mt_mode);
}


extern VDXFilterDefinition2 filterDef_JPSDR_RGBConvert=
VDXVideoFilterDefinition<JPSDR_RGBConvert>("JPSDR","RGBConvert v3.1.2","RGB <-> YCbCr convertion with color matrix option.\n[SSE2][AVX][AVX2] Optimised.");
