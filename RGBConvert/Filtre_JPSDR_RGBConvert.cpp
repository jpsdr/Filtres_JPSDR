#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "RGBConvert_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

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
extern "C" void JPSDR_RGBConvert_RGB32toYV12_SSE_1(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_pitch,ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch_Y,ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,
	int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);
extern "C" void JPSDR_RGBConvert_RGB32toYV12_SSE_2(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_pitch,ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch_Y,ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,
	int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_YV24toRGB32_SSE(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int16_t offset_R,int16_t offset_G,int16_t offset_B,int16_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);

extern "C" void JPSDR_RGBConvert_RGB32toYV24_AVX(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB32toYV16_AVX(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,int16_t Min_Y,int16_t Max_Y,
	int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB32toYUYV_AVX(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,
	int16_t Max_V);

extern "C" void JPSDR_RGBConvert_RGB32toYV12_AVX(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_pitch,ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch_Y,ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,
	int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);
extern "C" void JPSDR_RGBConvert_RGB32toYV12_AVX_1(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_pitch,ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch_Y,ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,
	int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);
extern "C" void JPSDR_RGBConvert_RGB32toYV12_AVX_2(const void *src,void *dst_y,void *dst_u,void *dst_v,int32_t w,int32_t h,
	int16_t offset_Y,int16_t offset_U,int16_t offset_V,int16_t *lookup, ptrdiff_t src_pitch,ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch_Y,ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,
	int16_t Min_Y,int16_t Max_Y,int16_t Min_U,int16_t Max_U,int16_t Min_V,int16_t Max_V);

extern "C" void JPSDR_RGBConvert_YV24toRGB32_AVX(const void *src_y,const void *src_u,const void *src_v,void *dst,int32_t w,
	int32_t h,int16_t offset_R,int16_t offset_G,int16_t offset_B,int16_t *lookup,
	ptrdiff_t src_modulo_Y,ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_modulo);


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


class JPSDR_RGBConvert : public VDXVideoFilter
{
public:
	JPSDR_RGBConvert(){}
	JPSDR_RGBConvert(const JPSDR_RGBConvert& a)
	{
		AVX_Enable=a.AVX_Enable;
		SSE2_Enable = a.SSE2_Enable;
		mData=a.mData;
		InternalInit();
	}
	virtual ~JPSDR_RGBConvert();

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
	int16_t Tlookup[2304];
	uint8_t convertion_mode;
	int16_t Min_U,Max_U,Min_Y,Max_Y,Min_V,Max_V;
	int16_t Offset_Y,Offset_U,Offset_V,Offset_R,Offset_G,Offset_B;
	double Coeff_Y,Coeff_U,Coeff_V;
	bool SSE2_Enable,AVX_Enable;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	uint8_t CreateMTData(uint8_t max_threads,int32_t src_size_x,int32_t src_size_y,int32_t dst_size_x,int32_t dst_size_y,bool src_UV_w,bool src_UV_h,bool dst_UV_w,bool dst_UV_h);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);

	void RGB32toYV12_SSE_MT(uint8_t thread_num);
	void RGB32toYV12_AVX_MT(uint8_t thread_num);

	void RGB32toYV24_MT(uint8_t thread_num);
	void RGB32toYV16_MT(uint8_t thread_num);
	void RGB32toYUY2_MT(uint8_t thread_num);
	void RGB32toYV12_MT(uint8_t thread_num);
	void YV24toRGB32_MT(uint8_t thread_num);

	void RGB32toYV24(const void *src,void *dst_y,void *dst_u,void *dst_v,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void RGB32toYV16(const void *src,void *dst_y,void *dst_u,void *dst_v,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v);
	void RGB32toYUY2(const void *src,void *dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void YV24toRGB32(const void *src_y,const void *src_u,const void *src_v, void *dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,
		ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch);
		
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
				filter->Compute_Lookup();
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
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
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


uint8_t JPSDR_RGBConvert::CreateMTData(uint8_t max_threads,int32_t src_size_x,int32_t src_size_y,int32_t dst_size_x,int32_t dst_size_y,bool src_UV_w,bool src_UV_h,bool dst_UV_w,bool dst_UV_h)
{
	if ((max_threads<=1) || (max_threads>threads_number))
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].src_Y_h_min=0;
		MT_Data[0].dst_Y_h_min=0;
		MT_Data[0].src_Y_h_max=src_size_y;
		MT_Data[0].dst_Y_h_max=dst_size_y;
		MT_Data[0].src_UV_h_min=0;
		MT_Data[0].dst_UV_h_min=0;
		if (src_UV_h) MT_Data[0].src_UV_h_max=src_size_y >> 1;
		else MT_Data[0].src_UV_h_max=src_size_y;
		if (dst_UV_h) MT_Data[0].dst_UV_h_max=dst_size_y >> 1;
		else MT_Data[0].dst_UV_h_max=dst_size_y;
		MT_Data[0].src_Y_w=src_size_x;
		MT_Data[0].dst_Y_w=dst_size_x;
		if (src_UV_w) MT_Data[0].src_UV_w=src_size_x >> 1;
		else MT_Data[0].src_UV_w=src_size_x;
		if (dst_UV_w) MT_Data[0].dst_UV_w=dst_size_x >> 1;
		else MT_Data[0].dst_UV_w=dst_size_x;
		return(1);
	}

	int32_t _y_min,_dh;
	int32_t src_dh_Y,src_dh_UV,dst_dh_Y,dst_dh_UV;
	int32_t h_y;
	uint8_t i,max_src=0,max_dst=0,max;

	dst_dh_Y=(dst_size_y+(uint32_t)max_threads-1)/(uint32_t)max_threads;
	if (dst_dh_Y<16) dst_dh_Y=16;
	if ((dst_dh_Y & 3)!=0) dst_dh_Y=((dst_dh_Y+3) >> 2) << 2;

	if (src_size_y==dst_size_y) src_dh_Y=dst_dh_Y;
	else
	{
		src_dh_Y=trunc((((float)src_size_y)/((float)dst_size_y))*(float)dst_dh_Y);
		if (src_dh_Y<16) src_dh_Y=16;
		if ((src_dh_Y & 3)!=0) src_dh_Y=((src_dh_Y+3) >> 2) << 2;
	}

	_y_min=src_size_y;
	_dh=src_dh_Y;
	h_y=0;
	while (h_y<(_y_min-16))
	{
		max_src++;
		h_y+=_dh;
	}

	_y_min=dst_size_y;
	_dh=dst_dh_Y;
	h_y=0;
	while (h_y<(_y_min-16))
	{
		max_dst++;
		h_y+=_dh;
	}

	max=(max_src<max_dst) ? max_src:max_dst;

	if (max==1)
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].src_Y_h_min=0;
		MT_Data[0].dst_Y_h_min=0;
		MT_Data[0].src_Y_h_max=src_size_y;
		MT_Data[0].dst_Y_h_max=dst_size_y;
		MT_Data[0].src_UV_h_min=0;
		MT_Data[0].dst_UV_h_min=0;
		if (src_UV_h) MT_Data[0].src_UV_h_max=src_size_y >> 1;
		else MT_Data[0].src_UV_h_max=src_size_y;
		if (dst_UV_h) MT_Data[0].dst_UV_h_max=dst_size_y >> 1;
		else MT_Data[0].dst_UV_h_max=dst_size_y;
		MT_Data[0].src_Y_w=src_size_x;
		MT_Data[0].dst_Y_w=dst_size_x;
		if (src_UV_w) MT_Data[0].src_UV_w=src_size_x >> 1;
		else MT_Data[0].src_UV_w=src_size_x;
		if (dst_UV_w) MT_Data[0].dst_UV_w=dst_size_x >> 1;
		else MT_Data[0].dst_UV_w=dst_size_x;
		return(1);
	}

	src_dh_UV= src_UV_h ? src_dh_Y>>1 : src_dh_Y;
	dst_dh_UV= dst_UV_h ? dst_dh_Y>>1 : dst_dh_Y;

	MT_Data[0].top=true;
	MT_Data[0].bottom=false;
	MT_Data[0].src_Y_h_min=0;
	MT_Data[0].src_Y_h_max=src_dh_Y;
	MT_Data[0].dst_Y_h_min=0;
	MT_Data[0].dst_Y_h_max=dst_dh_Y;
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
		MT_Data[i].src_Y_h_max=MT_Data[i].src_Y_h_min+src_dh_Y;
		MT_Data[i].dst_Y_h_min=MT_Data[i-1].dst_Y_h_max;
		MT_Data[i].dst_Y_h_max=MT_Data[i].dst_Y_h_min+dst_dh_Y;
		MT_Data[i].src_UV_h_min=MT_Data[i-1].src_UV_h_max;
		MT_Data[i].src_UV_h_max=MT_Data[i].src_UV_h_min+src_dh_UV;
		MT_Data[i].dst_UV_h_min=MT_Data[i-1].dst_UV_h_max;
		MT_Data[i].dst_UV_h_max=MT_Data[i].dst_UV_h_min+dst_dh_UV;
		i++;
	}

	MT_Data[max-1].bottom=true;
	MT_Data[max-1].src_Y_h_max=src_size_y;
	MT_Data[max-1].dst_Y_h_max=dst_size_y;
	if (src_UV_h) MT_Data[max-1].src_UV_h_max=src_size_y >> 1;
	else MT_Data[max-1].src_UV_h_max=src_size_y;
	if (dst_UV_h) MT_Data[max-1].dst_UV_h_max=dst_size_y >> 1;
	else MT_Data[max-1].dst_UV_h_max=dst_size_y;

	for (i=0; i<max; i++)
	{
		MT_Data[i].src_Y_w=src_size_x;
		MT_Data[i].dst_Y_w=dst_size_x;
		if (src_UV_w) MT_Data[i].src_UV_w=src_size_x >> 1;
		else MT_Data[i].src_UV_w=src_size_x;
		if (dst_UV_w) MT_Data[i].dst_UV_w=dst_size_x >> 1;
		else MT_Data[i].dst_UV_w=dst_size_x;
	}

	return(max);
}



void JPSDR_RGBConvert::Compute_Lookup(void)
{
	double kr,kg,kb;
	double Rv,Gu,Gv,Bu;
	double Ur,Ug,Ub,Vr,Vg,Vb;
	int16_t Off_Y,Off_U,Off_V;

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

	Offset_R=(int16_t)-round(16.0+32.0*(Off_Y/Coeff_Y+Rv*Off_V/Coeff_V));
	Offset_G=(int16_t)-round(16.0+32.0*(Off_Y/Coeff_Y+Gu*Off_U/Coeff_U+Gv*Off_V/Coeff_V));
	Offset_B=(int16_t)-round(16.0+32.0*(Off_Y/Coeff_Y+Bu*Off_U/Coeff_U));

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
			for (int16_t i=0; i<=255; i++)
			{
				Tlookup[i]=(int16_t)round(i*32.0/Coeff_Y);
				Tlookup[i+256]=(int16_t)round(i*Rv*32.0/Coeff_V);
				Tlookup[i+512]=(int16_t)round(i*Gu*32.0/Coeff_U);
				Tlookup[i+768]=(int16_t)round(i*Gv*32.0/Coeff_V);
				Tlookup[i+1024]=(int16_t)round(i*Bu*32.0/Coeff_U);
				Tlookup[i+1280]=0;
				Tlookup[i+1536]=0;
				Tlookup[i+1792]=0;
				Tlookup[i+2048]=0;
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

	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
			convertion_mode=0;
			break;
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
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
			convertion_mode=3;
			break;
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

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
}


void JPSDR_RGBConvert::Start()
{
	Image_Data idata;

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

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if  (mData.mt_mode && (idata.src_h0>=32) && (idata.dst_h0>=32)) threads_number=total_cpu;
	else threads_number=1;

	bool div_src_w,div_dst_w,div_src_h,div_dst_h;

	switch (idata.src_video_mode)
	{
		case 0 :
		case 1 :
		case 4 :
			div_src_w=false;
			div_src_h=false;
		case 2 :
		case 3 :
		case 5 :
			div_src_w=true;
			div_src_h=false;
			break;
		case 6 :
			div_src_w=true;
			div_src_h=true;
			break;
		default :
			div_src_w=false;
			div_src_h=false;
			break;
	}

	switch (idata.dst_video_mode)
	{
		case 0 :
		case 1 :
		case 4 :
			div_dst_w=false;
			div_dst_h=false;
		case 2 :
		case 3 :
		case 5 :
			div_dst_w=true;
			div_dst_h=false;
			break;
		case 6 :
			div_dst_w=true;
			div_dst_h=true;
			break;
		default :
			div_dst_w=false;
			div_dst_h=false;
			break;
	}

	threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0,idata.dst_w0,idata.dst_h0,
		div_src_w,div_src_h,div_dst_w,div_dst_h);

	if (threads_number>1)
	{
		if (!threadpoolAllocated)
		{
			ff->Except("Error with the TheadPool while allocating threadpool!");
			return;
		}
	}

	Compute_Lookup();
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




void JPSDR_RGBConvert::RGB32toYV24(const void *src_,void *dst_y_,void *dst_u_,void *dst_v_,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,
		ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const RGB32BMP *src;
	uint8_t *dst_y,*dst_u,*dst_v;

	const int16 *lookup=Tlookup;

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


void JPSDR_RGBConvert::RGB32toYV16(const void *src_,void *dst_y_,void *dst_u_,void *dst_v_,const int32_t w,
	const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_y,ptrdiff_t dst_pitch_u,ptrdiff_t dst_pitch_v)
{
	const RGB32BMP *src;
	uint8_t *dst_y,*dst_u,*dst_v;

	const int16 *lookup=Tlookup;
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



void JPSDR_RGBConvert::RGB32toYUY2(const void *src_,void *dst_,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src;
	uint32_t *dst;

	const int16 *lookup=Tlookup;
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



void JPSDR_RGBConvert::RGB32toYV12_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_=mt_data_inf.src1;
	const void *dst_y_=mt_data_inf.dst1;
	const void *dst_u_=mt_data_inf.dst2;
	const void *dst_v_=mt_data_inf.dst3;
	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;
	ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	ptrdiff_t dst_pitch_y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_u=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_v=mt_data_inf.dst_pitch3;

	const RGB32BMP *src,*src2;
	uint8_t *dst_y,*dst_y2,*dst_u,*dst_v;

	const int16 *lookup=Tlookup;
	const int32_t w0=((w+1) >> 1)-1;
	const int32_t h0=mt_data_inf.bottom ? ((h+1) >> 1)-1:((h+1) >> 1);

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

	if (mt_data_inf.bottom)
	{
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
}


void JPSDR_RGBConvert::RGB32toYV12_SSE_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;

	if (mt_data_inf.bottom)
		JPSDR_RGBConvert_RGB32toYV12_SSE_2(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
			w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,mt_data_inf.dst_pitch1,
			mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,
			Min_V,Max_V);
	else
		JPSDR_RGBConvert_RGB32toYV12_SSE_1(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
			w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,mt_data_inf.dst_pitch1,
			mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,
			Min_V,Max_V);
}


void JPSDR_RGBConvert::RGB32toYV12_AVX_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;

	if (mt_data_inf.bottom)
		JPSDR_RGBConvert_RGB32toYV12_AVX_2(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
			w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,mt_data_inf.dst_pitch1,
			mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,
			Min_V,Max_V);
	else
		JPSDR_RGBConvert_RGB32toYV12_AVX_1(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
			w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,mt_data_inf.dst_pitch1,
			mt_data_inf.dst_modulo1,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,
			Min_V,Max_V);
}


void JPSDR_RGBConvert::YV24toRGB32(const void *src_y_,const void *src_u_,const void *src_v_, void *dst_,
	const int32_t w,const int32_t h,ptrdiff_t src_pitch_y,ptrdiff_t src_pitch_u,ptrdiff_t src_pitch_v,ptrdiff_t dst_pitch)
{
	RGB32BMP *dst;
	const uint8_t *src_y,*src_u,*src_v;

	const int16 *lookup=Tlookup;

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


void JPSDR_RGBConvert::RGB32toYV24_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;

	if (AVX_Enable)
		JPSDR_RGBConvert_RGB32toYV24_AVX(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
			w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,
			mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
	else
	{
		if (SSE2_Enable)
			JPSDR_RGBConvert_RGB32toYV24_SSE(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
				w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,
				mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
		else
			RGB32toYV24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,w,h,mt_data_inf.src_pitch1,
				mt_data_inf.dst_pitch1,mt_data_inf.dst_pitch2,mt_data_inf.dst_pitch3);
	}
}


void JPSDR_RGBConvert::RGB32toYV16_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;

	if (AVX_Enable)
		JPSDR_RGBConvert_RGB32toYV16_AVX(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
			w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,
			mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
	else
	{
		if (SSE2_Enable)
			JPSDR_RGBConvert_RGB32toYV16_SSE(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,
				w,h,Offset_Y,Offset_U,Offset_V,Tlookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,
				mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
		else
			RGB32toYV16(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.dst2,mt_data_inf.dst3,w,h,mt_data_inf.src_pitch1,
				mt_data_inf.dst_pitch1,mt_data_inf.dst_pitch2,mt_data_inf.dst_pitch3);
	}
}


void JPSDR_RGBConvert::RGB32toYUY2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;

	if (AVX_Enable)
		JPSDR_RGBConvert_RGB32toYUYV_AVX(mt_data_inf.src1,mt_data_inf.dst1,w,h,Offset_Y,Offset_U,Offset_V,
			Tlookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
	else
	{
		if (SSE2_Enable)
			JPSDR_RGBConvert_RGB32toYUYV_SSE(mt_data_inf.src1,mt_data_inf.dst1,w,h,Offset_Y,Offset_U,Offset_V,
				Tlookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
		else
			RGB32toYUY2(mt_data_inf.src1,mt_data_inf.dst1,w,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	}
}


void JPSDR_RGBConvert::YV24toRGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t h = mt_data_inf.src_h ? mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min : mt_data_inf.dst_Y_h_max-mt_data_inf.dst_Y_h_min;
	const int32_t w = mt_data_inf.src_w ? mt_data_inf.src_Y_w : mt_data_inf.dst_Y_w;

	if (AVX_Enable)
		JPSDR_RGBConvert_YV24toRGB32_AVX(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
			w,h,Offset_R,Offset_G,Offset_B,Tlookup,mt_data_inf.src_modulo1,mt_data_inf.src_modulo2,
			mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
	else
	{
		if (SSE2_Enable)
			JPSDR_RGBConvert_YV24toRGB32_SSE(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,
				w,h,Offset_R,Offset_G,Offset_B,Tlookup,mt_data_inf.src_modulo1,mt_data_inf.src_modulo2,
				mt_data_inf.src_modulo3,mt_data_inf.dst_modulo1);
		else
			YV24toRGB32(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst1,w,h,mt_data_inf.src_pitch1,
				mt_data_inf.src_pitch2,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch1);
	}
}



void JPSDR_RGBConvert::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_RGBConvert *ptrClass=(JPSDR_RGBConvert *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->RGB32toYV24_MT(data->thread_Id); break;
		case 2 : ptrClass->RGB32toYV16_MT(data->thread_Id); break;
		case 3 : ptrClass->RGB32toYUY2_MT(data->thread_Id); break;
		case 4 : ptrClass->YV24toRGB32_MT(data->thread_Id); break;
		case 5 : ptrClass->RGB32toYV12_MT(data->thread_Id); break;
		case 6 : ptrClass->RGB32toYV12_SSE_MT(data->thread_Id); break;
		case 7 : ptrClass->RGB32toYV12_AVX_MT(data->thread_Id); break;
		default : ;
	}
}


void JPSDR_RGBConvert::Run()
{
	Image_Data idata;
	int32_t w,h;
	ptrdiff_t d_dst0,d_dst1,d_dst2;
	ptrdiff_t d_src0,d_src1,d_src2;
	bool src_w,src_h;

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
					src_w=true;
					src_h=true;
					d_src0=0; d_src1=0; d_src2=0; 
					d_dst0=0; d_dst1=0; d_dst2=0;
					break;
				case 1 :
					h=idata.src_h0;
					src_h=true;
					d_dst1=0; d_dst2=0;
					d_src1=0; d_src2=0;
					if (idata.src_w0<idata.dst_w0)
					{
						w=idata.src_w0;
						src_w=true;
						d_src0=0;
						d_dst0=idata.dst_w0-idata.src_w0;
					}
					else
					{
						if (idata.src_w0>idata.dst_w0)
						{
							w=idata.dst_w0;
							src_w=false;
							d_dst0=0;
							d_src0=(idata.src_w0-idata.dst_w0) << 2;
						}
						else
						{
							w=idata.src_w0;
							src_w=true;
							d_dst0=0; d_src0=0;
						}
					}
					break;
				case 2 :
					h=idata.src_h0;
					src_h=true;
					d_dst1=0; d_dst2=0;
					d_src1=0; d_src2=0;
					if (idata.src_w0<idata.dst_w0)
					{
						w=idata.src_w0;
						src_w=true;
						d_src0=0;
						d_dst0=0;
					}
					else
					{
						if (idata.src_w0>idata.dst_w0)
						{
							w=idata.dst_w0;
							src_w=false;
							d_dst0=0;
							d_src0=(idata.src_w0-idata.dst_w0) << 2;
						}
						else
						{
							w=idata.src_w0;
							src_w=true;
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
						src_w=true;
						d_src0=0;
						d_dst0=idata.dst_w0-idata.src_w0;
					}
					else
					{
						if (idata.src_w0>idata.dst_w0)
						{
							w=idata.dst_w0;
							src_w=false;
							d_dst0=0;
							d_src0=(idata.src_w0-idata.dst_w0) << 2;
						}
						else
						{
							w=idata.src_w0;
							src_w=true;
							d_dst0=0; d_src0=0;
						}
					}
					if (idata.src_h0<idata.dst_h0)
					{
						h=idata.src_h0;
						src_h=true;
					}
					else
					{
						h=idata.dst_h0;
						src_h=false;
					}
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
			src_w=true;
			src_h=true;
			d_src0=0; d_src1=0; d_src2=0; 
			d_dst0=0; d_dst1=0; d_dst2=0;
			break;
		default :
			w=idata.src_w0;
			h=idata.src_h0;
			src_w=true;
			src_h=true;
			d_src0=0; d_src1=0; d_src2=0; 
			d_dst0=0; d_dst1=0; d_dst2=0;
			break;
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
		MT_Data[i].src_modulo1=idata.src_modulo0+d_src0;
		MT_Data[i].src_modulo2=idata.src_modulo1+d_src1;
		MT_Data[i].src_modulo3=idata.src_modulo2+d_src2;
		MT_Data[i].dst_modulo1=idata.dst_modulo0+d_dst0;
		MT_Data[i].dst_modulo2=idata.dst_modulo1+d_dst1;
		MT_Data[i].dst_modulo3=idata.dst_modulo2+d_dst2;
		MT_Data[i].src_w=src_w;
		MT_Data[i].src_h=src_h;
	}

	if (threads_number>1)
	{
		if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
		{
			uint8_t f_proc=0;

			switch(convertion_mode)
			{
				case 0 :
					switch(mData.output_mode)
					{
						case 0 : f_proc=1; break;
						case 1 : f_proc=2; break;
						case 2 : f_proc=3; break;
						case 3 : 
							if (AVX_Enable) f_proc=7;
							else
							{
								if (SSE2_Enable) f_proc=6;
								else f_proc=5;
							}
							break;
					}
					break;
				case 3 : f_proc=4; break;
			}

			for(uint8_t i=0; i<threads_number; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<threads_number; i++)
				MT_Thread[i].f_process=0;

			poolInterface->ReleaseThreadPool(UserId,true);
		}
	}
	else
	{
		switch(convertion_mode)
		{
			case 0 :
				switch(mData.output_mode)
				{
					case 0 : RGB32toYV24_MT(0); break;
					case 1 : RGB32toYV16_MT(0); break;
					case 2 : RGB32toYUY2_MT(0); break;
					case 3 :
						if (AVX_Enable)
							JPSDR_RGBConvert_RGB32toYV12_AVX(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,
									Offset_Y,Offset_U,Offset_V,Tlookup,idata.src_pitch0,idata.src_modulo0+d_src0,idata.dst_pitch0,idata.dst_modulo0+d_dst0,
									idata.dst_modulo1+d_dst1,idata.dst_modulo2+d_dst2,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
						else
						{
							if (SSE2_Enable)
								JPSDR_RGBConvert_RGB32toYV12_SSE(idata.src_plane0,idata.dst_plane0,idata.dst_plane1,idata.dst_plane2,w,h,
										Offset_Y,Offset_U,Offset_V,Tlookup,idata.src_pitch0,idata.src_modulo0+d_src0,idata.dst_pitch0,idata.dst_modulo0+d_dst0,
										idata.dst_modulo1+d_dst1,idata.dst_modulo2+d_dst2,Min_Y,Max_Y,Min_U,Max_U,Min_V,Max_V);
							else RGB32toYV12_MT(0);
						}
						break;
				}
				break;
			case 3 : YV24toRGB32_MT(0); break;
		}
	}
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
VDXVideoFilterDefinition<JPSDR_RGBConvert>("JPSDR","RGBConvert v2.3.4","RGB <-> YCbCr convertion with color matrix option.\n[ASM][SSE2] Optimised.");
