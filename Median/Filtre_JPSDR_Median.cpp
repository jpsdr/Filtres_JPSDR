#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "Median_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

#define Max_Median_Size 50

extern ThreadPoolInterface *poolInterface;

extern int g_VFVAPIVersion;

extern "C" void JPSDR_Median_RGB32_Move_src(const void *src, void *planar_R, void *planar_G,void *planar_B,
	int32_t w,int32_t h,ptrdiff_t src_offset,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_RGB32_Move_src_SSE_2(const void *src, void *planar_R, void *planar_G,void *planar_B,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Median_RGB32_Move_src_AVX_2(const void *src, void *planar_R, void *planar_G,void *planar_B,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Median_YUYV_Move_src(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_offset,ptrdiff_t dst_offset_Y,ptrdiff_t dst_offset_UV);
extern "C" void JPSDR_Median_YUYV_Move_src_SSE_2(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_UV);
extern "C" void JPSDR_Median_YUYV_Move_src_AVX_2(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_UV);

extern "C" void JPSDR_Median_UYVY_Move_src(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_offset,ptrdiff_t dst_offset_Y,ptrdiff_t dst_offset_UV);
extern "C" void JPSDR_Median_UYVY_Move_src_SSE_2(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_UV);
extern "C" void JPSDR_Median_UYVY_Move_src_AVX_2(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_UV);

extern "C" void JPSDR_Median_RGB32_Move_dst(const void *planar_R,const void *planar_G,const void *planar_B,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_offset,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_RGB32_Move_dst_SSE_2(const void *planar_R,const void *planar_G,const void *planar_B,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Median_RGB32_Move_dst_AVX_2(const void *planar_R,const void *planar_G,const void *planar_B,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Median_YUYV_Move_dst(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_offset_Y,ptrdiff_t src_offset_UV,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_YUYV_Move_dst_SSE_2(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_UV,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Median_YUYV_Move_dst_AVX_2(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_UV,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Median_UYVY_Move_dst(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_offset_Y,ptrdiff_t src_offset_UV,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_UYVY_Move_dst_SSE_2(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_UV,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Median_UYVY_Move_dst_AVX_2(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_UV,ptrdiff_t dst_pitch);


typedef struct _MT_Data_Info
{
	void *src;
	void *dst;
	ptrdiff_t src_pitch,src_modulo,dst_pitch,dst_modulo;
	int32_t src_h_min,src_h_max,src_w;
	int32_t src_Y_h_min,src_Y_h_max,src_Y_w,src_Y_w_32;
	int32_t src_UV_h_min,src_UV_h_max,src_UV_w,src_UV_w_32;
	int32_t dst_Y_h_min,dst_Y_h_max,dst_Y_w,dst_Y_w_32;
	int32_t dst_UV_h_min,dst_UV_h_max,dst_UV_w,dst_UV_w_32;
	int16_t threshold;
	uint8_t *data;
	int32_t m_size;
	uint32_t size_data;
	bool top,bottom;
} MT_Data_Info;



class JPSDR_MedianData
{
public :
	uint8_t threshold[3];
	int32_t median_size[3];
	bool setting_mode,interlace_mode;
	bool filter_disable[3];
	uint8_t filter_mode;
	bool mt_mode;

	JPSDR_MedianData(void);
};

JPSDR_MedianData::JPSDR_MedianData(void)
{
	uint8_t i;

	for (i=0; i<3; i++)
	{
		median_size[i]=1;
		filter_disable[i]=false;
	}
	threshold[0]=17;
	threshold[1]=11;
	threshold[2]=8;
	filter_mode=1;
	interlace_mode=false;
	setting_mode=false;
	mt_mode=true;
}


class JPSDR_MedianDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_MedianDialog(JPSDR_MedianData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_MEDIAN),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_MedianData& mData;
	JPSDR_MedianData moldData;
	IVDXFilterPreview2 *const mifp;
};



INT_PTR JPSDR_MedianDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_MedianDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_DISABLE_0,mData.filter_disable[0]?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_DISABLE_1,mData.filter_disable[1]?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_DISABLE_2,mData.filter_disable[2]?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_MODE,mData.setting_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_INTERLACE,mData.interlace_mode?BST_CHECKED:BST_UNCHECKED);
	switch (mData.filter_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_SQUARE,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_HORIZONTAL,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_VERTICAL,1); break;
	}
	SetDlgItemInt(mhdlg,IDC_0_MEDIAN_SIZE,mData.median_size[0],FALSE);
	SetDlgItemInt(mhdlg,IDC_0_THRESHOLD,mData.threshold[0],FALSE);
	SetDlgItemInt(mhdlg,IDC_1_MEDIAN_SIZE,mData.median_size[1],FALSE);
	SetDlgItemInt(mhdlg,IDC_1_THRESHOLD,mData.threshold[1],FALSE);
	SetDlgItemInt(mhdlg,IDC_2_MEDIAN_SIZE,mData.median_size[2],FALSE);
	SetDlgItemInt(mhdlg,IDC_2_THRESHOLD,mData.threshold[2],FALSE);

	EnableWindow(GetDlgItem(mhdlg,IDC_0_MEDIAN_SIZE),!mData.filter_disable[0]);
	EnableWindow(GetDlgItem(mhdlg,IDC_0_THRESHOLD),!mData.filter_disable[0]);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_0),!mData.filter_disable[0]);
	EnableWindow(GetDlgItem(mhdlg,IDC_1_MEDIAN_SIZE),!mData.filter_disable[1]);
	EnableWindow(GetDlgItem(mhdlg,IDC_1_THRESHOLD),!mData.filter_disable[1]);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_1),!mData.filter_disable[1]);
	EnableWindow(GetDlgItem(mhdlg,IDC_2_MEDIAN_SIZE),!mData.filter_disable[2]);
	EnableWindow(GetDlgItem(mhdlg,IDC_2_THRESHOLD),!mData.filter_disable[2]);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_2),!mData.filter_disable[2]);

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}



void JPSDR_MedianDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}



bool JPSDR_MedianDialog::SaveToData()
{
	int32_t data;
	BOOL success;
	
	mData.filter_disable[0]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_0);
	mData.filter_disable[1]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_1);
	mData.filter_disable[2]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_2);
	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);

	mData.setting_mode=!!IsDlgButtonChecked(mhdlg,IDC_MODE);
	mData.interlace_mode=!!IsDlgButtonChecked(mhdlg,IDC_INTERLACE);

	if (!!IsDlgButtonChecked(mhdlg,IDC_SQUARE)) mData.filter_mode=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_HORIZONTAL)) mData.filter_mode=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_VERTICAL)) mData.filter_mode=3;

	data=GetDlgItemInt(mhdlg,IDC_0_MEDIAN_SIZE,&success,FALSE);
	if (success)
	{
		if (data<=0) data=1;
		if (data>Max_Median_Size) data=Max_Median_Size;
		mData.median_size[0]=data;
	}
	data=GetDlgItemInt(mhdlg,IDC_1_MEDIAN_SIZE,&success,FALSE);
	if (success)
	{
		if (data<=0) data=1;
		if (data>Max_Median_Size) data=Max_Median_Size;
		mData.median_size[1]=data;
	}
	data=GetDlgItemInt(mhdlg,IDC_2_MEDIAN_SIZE,&success,FALSE);
	if (success)
	{
		if (data<=0) data=1;
		if (data>Max_Median_Size) data=Max_Median_Size;
		mData.median_size[2]=data;
	}
	data=GetDlgItemInt(mhdlg,IDC_0_THRESHOLD,&success,FALSE);
	if (success)
	{
		if (data>255) data=255;
		mData.threshold[0]=(uint8_t)data;
	}
	data=GetDlgItemInt(mhdlg,IDC_1_THRESHOLD,&success,FALSE);
	if (success)
	{
		if (data>255) data=255;
		mData.threshold[1]=(uint8_t)data;
	}
	data=GetDlgItemInt(mhdlg,IDC_2_THRESHOLD,&success,FALSE);
	if (success)
	{
		if (data>255) data=255;
		mData.threshold[2]=(uint8_t)data;
	}
	
	return true;
}


bool JPSDR_MedianDialog::OnCommand(int cmd)
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
		case IDC_DISABLE_0 :
			mData.filter_disable[0]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_0);
			if (mifp) mifp->RedoFrame();
			EnableWindow(GetDlgItem(mhdlg,IDC_0_MEDIAN_SIZE),!mData.filter_disable[0]);
			EnableWindow(GetDlgItem(mhdlg,IDC_0_THRESHOLD),!mData.filter_disable[0]);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_0),!mData.filter_disable[0]);
			return true;
		case IDC_DISABLE_1 :
			mData.filter_disable[1]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_1);
			if (mifp) mifp->RedoFrame();
			EnableWindow(GetDlgItem(mhdlg,IDC_1_MEDIAN_SIZE),!mData.filter_disable[1]);
			EnableWindow(GetDlgItem(mhdlg,IDC_1_THRESHOLD),!mData.filter_disable[1]);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_1),!mData.filter_disable[1]);
			return true;
		case IDC_DISABLE_2 :
			mData.filter_disable[2]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_2);
			if (mifp) mifp->RedoFrame();
			EnableWindow(GetDlgItem(mhdlg,IDC_2_MEDIAN_SIZE),!mData.filter_disable[2]);
			EnableWindow(GetDlgItem(mhdlg,IDC_2_THRESHOLD),!mData.filter_disable[2]);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_2),!mData.filter_disable[2]);
			return true;
		case IDC_0_THRESHOLD :
		case IDC_1_THRESHOLD :
		case IDC_2_THRESHOLD :
		case IDC_MODE :
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_SQUARE :
		case IDC_HORIZONTAL :
		case IDC_VERTICAL :
		case IDC_0_MEDIAN_SIZE :
		case IDC_1_MEDIAN_SIZE :
		case IDC_2_MEDIAN_SIZE :
		case IDC_INTERLACE :
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



class JPSDR_Median : public VDXVideoFilter
{
public:
	JPSDR_Median(){}
	JPSDR_Median(const JPSDR_Median& a)
	{
		AVX_Enable=a.AVX_Enable;
		SSE2_Enable = a.SSE2_Enable;
		mData=a.mData;
		InternalInit();
	}
	virtual ~JPSDR_Median();

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
	uint8_t *buffer_in[3],*buffer_out[3];
	ptrdiff_t buffer_pitch[3],buffer_modulo[3];
	uint32_t buffer_size[3];
	uint8_t *Tdata[MAX_MT_THREADS];
	bool SSE2_Enable,AVX_Enable;
	int32_t max_median_size;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits);

	void square_median_filter(uint8_t thread_num);
	void square_median_filter_2(uint8_t thread_num);

	void horizontal_median_filter(uint8_t thread_num);
	void horizontal_median_filter_2(uint8_t thread_num);

	void vertical_median_filter(uint8_t thread_num);
	void vertical_median_filter_2(uint8_t thread_num);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_MedianData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Median)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Median,ScriptConfig,"iiiiiiiiiiiii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_Median::Init()
{
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	InternalInit();

	return(true);
}


void JPSDR_Median::InternalInit(void)
{
	uint16_t i;

	for (i=0; i<3; i++)
	{
		buffer_in[i]=NULL;
		buffer_out[i]=NULL;
	}

	StaticThreadpoolF=StaticThreadpool;

	for (i=0; i<MAX_MT_THREADS; i++)
	{
		MT_Thread[i].pClass=this;
		MT_Thread[i].f_process=0;
		MT_Thread[i].thread_Id=(uint8_t)i;
		MT_Thread[i].pFunc=StaticThreadpoolF;
		Tdata[i]=NULL;
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


JPSDR_Median::~JPSDR_Median()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
}


uint8_t JPSDR_Median::CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits)
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
	if (dh_Y<max_median_size) dh_Y=max_median_size;
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


uint32 JPSDR_Median::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

	bool swap_buffer;
	
	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :*/						
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
			swap_buffer=false;
			break;
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :			*/	
			swap_buffer=true;
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
	
	if (swap_buffer)
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
	}
	else
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
	}
}



void JPSDR_Median::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];
	signed long size[3];
	unsigned char i;

	for (i=0; i<3; i++)
	{
		if (mData.filter_disable[i]) size[i]=0;
		else size[i]=mData.median_size[i];
	}
	strcpy_s(output_string,255," Size:(%d,%d,%d) Thrs:(%d,%d,%d) Mode:%d");
	if (mData.setting_mode) strcat_s(output_string,255," Test");
	if (mData.interlace_mode) strcat_s(output_string,255," Intrl");
	SafePrintf(buf,maxlen,output_string,size[0],size[1],size[2],mData.threshold[0],mData.threshold[1],
		mData.threshold[2],mData.filter_mode);
}




void JPSDR_Median::square_median_filter(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t m_size=mt_data_inf.m_size;
	const int16_t threshold=mt_data_inf.threshold;
	uint8_t *data=mt_data_inf.data;
	const uint32_t size_data=mt_data_inf.size_data;

	const int32_t w=mt_data_inf.src_w;
	const int32_t h_max=mt_data_inf.src_h_max;
	const int32_t h_min=mt_data_inf.src_h_min;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch;

	const uint8_t *src;
	uint8_t *dst;
	
	src=(uint8_t *)mt_data_inf.src;
	dst=(uint8_t *)mt_data_inf.dst;

	const ptrdiff_t _size=m_size*src_pitch;	
	const uint32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=(m_size << 1)+1;
	const int32_t w_1=w-m_size;
	const int32_t h_1 = mt_data_inf.bottom ? h_max-m_size:h_max;
	const int32_t h_0 = mt_data_inf.top ? m_size:h_min;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}

	for (int32_t i=h_0; i<h_1; i++)
	{
		memcpy(dst,src,m_size);

		ptrdiff_t d_size=-_size;

		for (int32_t j=m_size; j<w_1; j++)
		{
			uint8_t *p=data;
			const uint8_t *src_2=src+(d_size++);

			for (int32_t k=0; k<m_4; k++)
			{
				memcpy(p,src_2,m_4);
				p+=m_4;
				src_2+=src_pitch;
			}

			int16_t max;

			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
				{
					if (max<(int16_t)data[l])
					{
						max=data[l];
						indice=l;
					}
				}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)src[j])<=threshold) dst[j]=src[j];
			else dst[j]=(uint8_t)max;
		}

		memcpy(dst+w_1,src+w_1,m_size);

		dst+=dst_pitch;
		src+=src_pitch; 
	}

	if (mt_data_inf.bottom) 
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
}



void JPSDR_Median::square_median_filter_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t m_size=mt_data_inf.m_size;
	const int16_t threshold=mt_data_inf.threshold;
	uint8_t *data=mt_data_inf.data;
	const uint32_t size_data=mt_data_inf.size_data;

	const int32_t w=mt_data_inf.src_w;
	const int32_t h_max=mt_data_inf.src_h_max;
	const int32_t h_min=mt_data_inf.src_h_min;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch;

	const uint8_t *src;
	uint8_t *dst;
	
	src=(uint8_t *)mt_data_inf.src;
	dst=(uint8_t *)mt_data_inf.dst;

	const ptrdiff_t _size=m_size*src_pitch;	
	const uint32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=(m_size << 1)+1;
	const int32_t w_1=w-m_size;
	const int32_t h_1 = mt_data_inf.bottom ? h_max-m_size:h_max;
	const int32_t h_0 = mt_data_inf.top ? m_size:h_min;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}

	for (int32_t i=h_0; i<h_1; i++)
	{

		memcpy(dst,src,m_size);

		ptrdiff_t d_size=-_size;

		for (int32_t j=m_size; j<w_1; j++)
		{
			uint8_t *p=data;
			const uint8_t *src_2=src+(d_size++);

			for (int32_t k=0; k<m_4; k++)
			{
				memcpy(p,src_2,m_4);
				p+=m_4;
				src_2+=src_pitch;
			}

			int16_t max;

			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
				{
					if (max<(int16_t)data[l])
					{
						max=data[l];
						indice=l;
					}
				}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)src[j])<=threshold) dst[j]=src[j];
			else dst[j]=255;
		}

		memcpy(dst+w_1,src+w_1,m_size);

		dst+=dst_pitch;
		src+=src_pitch; 
	}

	if (mt_data_inf.bottom) 
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
}


	

void JPSDR_Median::horizontal_median_filter(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t m_size=mt_data_inf.m_size;
	const int16_t threshold=mt_data_inf.threshold;
	uint8_t *data=mt_data_inf.data;
	const uint32_t size_data=mt_data_inf.size_data;

	const int32_t w=mt_data_inf.src_w;
	const int32_t h_max=mt_data_inf.src_h_max;
	const int32_t h_min=mt_data_inf.src_h_min;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch;

	const uint8_t *src;
	uint8_t *dst;
	
	src=(uint8_t *)mt_data_inf.src;
	dst=(uint8_t *)mt_data_inf.dst;

	const uint32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=(m_size << 1)+1;
	const int32_t w_1=w-m_size;

	for (int32_t i=h_min; i<h_max; i++)
	{
		memcpy(dst,src,m_size);

		ptrdiff_t d_size=0;

		for (int32_t j=m_size; j<w_1; j++)
		{
			memcpy(data,src+(d_size++),m_4);

			int16_t max;

			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
				{
					if (max<(int16_t)data[l])
					{
						max=data[l];
						indice=l;
					}
				}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)src[j])<=threshold) dst[j]=src[j];
			else dst[j]=(uint8_t)max;
		}

		memcpy(dst+w_1,src+w_1,m_size);

		dst+=dst_pitch;
		src+=src_pitch;		
	}
}


void JPSDR_Median::horizontal_median_filter_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t m_size=mt_data_inf.m_size;
	const int16_t threshold=mt_data_inf.threshold;
	uint8_t *data=mt_data_inf.data;
	const uint32_t size_data=mt_data_inf.size_data;

	const int32_t w=mt_data_inf.src_w;
	const int32_t h_max=mt_data_inf.src_h_max;
	const int32_t h_min=mt_data_inf.src_h_min;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch;

	const uint8_t *src;
	uint8_t *dst;
	
	src=(uint8_t *)mt_data_inf.src;
	dst=(uint8_t *)mt_data_inf.dst;

	const uint32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=(m_size << 1)+1;
	const int32_t w_1=w-m_size;

	for (int32_t i=h_min; i<h_max; i++)
	{
		memcpy(dst,src,m_size);

		ptrdiff_t d_size=0;

		for (int32_t j=m_size; j<w_1; j++)
		{
			memcpy(data,src+(d_size++),m_4);

			int16_t max;

			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
				{
					if (max<(int16_t)data[l])
					{
						max=data[l];
						indice=l;
					}
				}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)src[j])<=threshold) dst[j]=src[j];
			else dst[j]=255;
		}

		memcpy(dst+w_1,src+w_1,m_size);

		dst+=dst_pitch;
		src+=src_pitch;		
	}
}



void JPSDR_Median::vertical_median_filter(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t m_size=mt_data_inf.m_size;
	const int16_t threshold=mt_data_inf.threshold;
	uint8_t *data=mt_data_inf.data;
	const uint32_t size_data=mt_data_inf.size_data;

	const int32_t w=mt_data_inf.src_w;
	const int32_t h_max=mt_data_inf.src_h_max;
	const int32_t h_min=mt_data_inf.src_h_min;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch;

	const uint8_t *src;
	uint8_t *dst;
	
	src=(uint8_t *)mt_data_inf.src;
	dst=(uint8_t *)mt_data_inf.dst;

	const ptrdiff_t _size=m_size*src_pitch;	
	const uint32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=(m_size << 1)+1;
	const int32_t h_1 = mt_data_inf.bottom ? h_max-m_size:h_max;
	const int32_t h_0 = mt_data_inf.top ? m_size:h_min;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
	
	for (int32_t i=h_0; i<h_1; i++)
	{
		ptrdiff_t d_size=-_size;

		for (int32_t j=0; j<w; j++)
		{
			const uint8_t *src_2=src+(d_size++);

			for (int32_t k=0; k<m_4; k++)
			{
				data[k]=*src_2;
				src_2+=src_pitch;
			}

			int16_t max;

			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
				{
					if (max<(int16_t)data[l])
					{
						max=data[l];
						indice=l;
					}
				}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)src[j])<=threshold) dst[j]=src[j];
			else dst[j]=(uint8_t)max;
		}

		dst+=dst_pitch;
		src+=src_pitch;		
	}

	if (mt_data_inf.bottom) 
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}

}



void JPSDR_Median::vertical_median_filter_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const int32_t m_size=mt_data_inf.m_size;
	const int16_t threshold=mt_data_inf.threshold;
	uint8_t *data=mt_data_inf.data;
	const uint32_t size_data=mt_data_inf.size_data;

	const int32_t w=mt_data_inf.src_w;
	const int32_t h_max=mt_data_inf.src_h_max;
	const int32_t h_min=mt_data_inf.src_h_min;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch;

	const uint8_t *src;
	uint8_t *dst;
	
	src=(uint8_t *)mt_data_inf.src;
	dst=(uint8_t *)mt_data_inf.dst;

	const ptrdiff_t _size=m_size*src_pitch;	
	const uint32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=(m_size << 1)+1;
	const int32_t h_1 = mt_data_inf.bottom ? h_max-m_size:h_max;
	const int32_t h_0 = mt_data_inf.top ? m_size:h_min;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
	
	for (int32_t i=h_0; i<h_1; i++)
	{
		ptrdiff_t d_size=-_size;

		for (int32_t j=0; j<w; j++)
		{
			const uint8_t *src_2=src+(d_size++);

			for (int32_t k=0; k<m_4; k++)
			{
				data[k]=*src_2;
				src_2+=src_pitch;
			}

			int16_t max;

			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
				{
					if (max<(int16_t)data[l])
					{
						max=data[l];
						indice=l;
					}
				}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)src[j])<=threshold) dst[j]=src[j];
			else dst[j]=255;
		}

		dst+=dst_pitch;
		src+=src_pitch;		
	}

	if (mt_data_inf.bottom) 
	{
		for(int32_t i=0; i<m_size; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}

}




void JPSDR_Median::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_Median *ptrClass=(JPSDR_Median *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->square_median_filter(data->thread_Id); break;
		case 2 : ptrClass->horizontal_median_filter(data->thread_Id); break;
		case 3 : ptrClass->vertical_median_filter(data->thread_Id); break;
		case 4 : ptrClass->square_median_filter_2(data->thread_Id); break;
		case 5 : ptrClass->horizontal_median_filter_2(data->thread_Id); break;
		case 6 : ptrClass->vertical_median_filter_2(data->thread_Id); break;
		default : ;
	}
}



void JPSDR_Median::Run()
{
	Image_Data idata;
	int16_t i,plan_max;
	uint32_t size_data[3];
	uint8_t *ptrSrc[3],*ptrDst[3];
	ptrdiff_t srcPitch[3],dstPitch[3];
	bool use_buffer;
	int32_t h_current,w_current;

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

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			if (AVX_Enable)
			{
				JPSDR_Median_RGB32_Move_src_AVX_2(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+3)>>2,idata.src_h0,idata.src_pitch0,buffer_pitch[0]);
			}
			else
			{
				if (SSE2_Enable)
				{
					JPSDR_Median_RGB32_Move_src_SSE_2(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
						(idata.src_w0+3)>>2,idata.src_h0,idata.src_pitch0,buffer_pitch[0]);
				}
				else
					JPSDR_Median_RGB32_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
						idata.src_w0,idata.src_h0,idata.src_modulo0,buffer_modulo[0]);
			}
			ptrSrc[0]=buffer_in[0];
			ptrSrc[1]=buffer_in[1];
			ptrSrc[2]=buffer_in[2];
			ptrDst[0]=buffer_out[0];
			ptrDst[1]=buffer_out[1];
			ptrDst[2]=buffer_out[2];
			srcPitch[0]=buffer_pitch[0];
			srcPitch[1]=buffer_pitch[1];
			srcPitch[2]=buffer_pitch[2];
			dstPitch[0]=buffer_pitch[0];
			dstPitch[1]=buffer_pitch[1];
			dstPitch[2]=buffer_pitch[2];
			use_buffer=true;
			break;
		case 2 :
			if (AVX_Enable)
			{
				JPSDR_Median_YUYV_Move_src_AVX_2(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+7)>>3,idata.src_h0,idata.src_pitch0,buffer_pitch[0],buffer_pitch[1]);
			}
			else
			{
				if (SSE2_Enable)
				{
					JPSDR_Median_YUYV_Move_src_SSE_2(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
						(idata.src_w0+7)>>3,idata.src_h0,idata.src_pitch0,buffer_pitch[0],buffer_pitch[1]);
				}
				else
					JPSDR_Median_YUYV_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
						(idata.src_w0+1)>>1,idata.src_h0,idata.src_modulo0,buffer_modulo[0],buffer_modulo[1]);
			}
			ptrSrc[0]=buffer_in[0];
			ptrSrc[1]=buffer_in[1];
			ptrSrc[2]=buffer_in[2];
			ptrDst[0]=buffer_out[0];
			ptrDst[1]=buffer_out[1];
			ptrDst[2]=buffer_out[2];
			srcPitch[0]=buffer_pitch[0];
			srcPitch[1]=buffer_pitch[1];
			srcPitch[2]=buffer_pitch[2];
			dstPitch[0]=buffer_pitch[0];
			dstPitch[1]=buffer_pitch[1];
			dstPitch[2]=buffer_pitch[2];
			use_buffer=true;
			break;
		case 3 :
			if (AVX_Enable)
			{
				JPSDR_Median_UYVY_Move_src_AVX_2(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+7)>>3,idata.src_h0,idata.src_pitch0,buffer_pitch[0],buffer_pitch[1]);
			}
			else
			{
				if (SSE2_Enable)
				{
					JPSDR_Median_UYVY_Move_src_SSE_2(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
						(idata.src_w0+7)>>3,idata.src_h0,idata.src_pitch0,buffer_pitch[0],buffer_pitch[1]);
				}
				else
					JPSDR_Median_UYVY_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
						(idata.src_w0+1)>>1,idata.src_h0,idata.src_modulo0,buffer_modulo[0],buffer_modulo[1]);
			}
			ptrSrc[0]=buffer_in[0];
			ptrSrc[1]=buffer_in[1];
			ptrSrc[2]=buffer_in[2];
			ptrDst[0]=buffer_out[0];
			ptrDst[1]=buffer_out[1];
			ptrDst[2]=buffer_out[2];
			srcPitch[0]=buffer_pitch[0];
			srcPitch[1]=buffer_pitch[1];
			srcPitch[2]=buffer_pitch[2];
			dstPitch[0]=buffer_pitch[0];
			dstPitch[1]=buffer_pitch[1];
			dstPitch[2]=buffer_pitch[2];
			use_buffer=true;
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
		case 9 :
			ptrSrc[0]=(uint8_t *)idata.src_plane0;
			ptrSrc[1]=(uint8_t *)idata.src_plane1;
			ptrSrc[2]=(uint8_t *)idata.src_plane2;
			ptrDst[0]=(uint8_t *)idata.dst_plane0;
			ptrDst[1]=(uint8_t *)idata.dst_plane1;
			ptrDst[2]=(uint8_t *)idata.dst_plane2;
			srcPitch[0]=idata.src_pitch0;
			srcPitch[1]=idata.src_pitch1;
			srcPitch[2]=idata.src_pitch2;
			dstPitch[0]=idata.dst_pitch0;
			dstPitch[1]=idata.dst_pitch1;
			dstPitch[2]=idata.dst_pitch2;
			use_buffer=false;
			break;
	}

	if (idata.video_mode==9) plan_max=1;
	else plan_max=3;

	switch(mData.filter_mode)
	{
	case 1 :	for (i=0; i<plan_max; i++)
				{
					size_data[i]=2*mData.median_size[i]+1;
					size_data[i]*=size_data[i];
				}
				break;
	case 2 :
	case 3 :	for (i=0; i<plan_max; i++)
					size_data[i]=2*mData.median_size[i]+1;
				break;
	}

	if (mData.interlace_mode)
	{
		for (i=0; i<plan_max; i++)
		{
			if (!mData.filter_disable[i])
			{
				for (uint8_t j=0; j<threads_number; j++)
				{
					MT_Data[j].size_data=size_data[i];
					MT_Data[j].threshold=mData.threshold[i];
					MT_Data[j].data=Tdata[j];
					MT_Data[j].m_size=mData.median_size[i];
					MT_Data[j].src_pitch=srcPitch[i] << 1;
					MT_Data[j].dst_pitch=dstPitch[i] << 1;
					switch(i)
					{
						case 0 :
							MT_Data[j].src_w=MT_Data[j].src_Y_w;
							MT_Data[j].src_h_min=MT_Data[j].src_Y_h_min;
							MT_Data[j].src_h_max=MT_Data[j].src_Y_h_max;
							break;
						case 1 :
						case 2 :
							MT_Data[j].src_w=MT_Data[j].src_UV_w;
							MT_Data[j].src_h_min=MT_Data[j].src_UV_h_min;
							MT_Data[j].src_h_max=MT_Data[j].src_UV_h_max;
							break;
					}
					MT_Data[j].src=(void *)(ptrSrc[i]+(MT_Data[j].src_pitch*MT_Data[j].src_h_min));
					MT_Data[j].dst=(void *)(ptrDst[i]+(MT_Data[j].dst_pitch*MT_Data[j].src_h_min));
				}

				if (threads_number>1)
				{
					if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
					{
						uint8_t f_proc=0;

						if (mData.setting_mode)
						{
							switch(mData.filter_mode)
							{
								case 1 : f_proc=4; break;
								case 2 : f_proc=5; break;
								case 3 : f_proc=6; break;
							}
						}
						else
						{
							switch(mData.filter_mode)
							{
								case 1 : f_proc=1; break;
								case 2 : f_proc=2; break;
								case 3 : f_proc=3; break;
							}
						}

						for(uint8_t j=0; j<threads_number; j++)
							MT_Thread[j].f_process=f_proc;

						if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

						for (uint8_t j=0; j<threads_number; j++)
						{
							MT_Data[j].src=(void *)(ptrSrc[i]+(MT_Data[j].src_pitch*MT_Data[j].src_h_min+(MT_Data[j].src_pitch >> 1)));
							MT_Data[j].dst=(void *)(ptrDst[i]+(MT_Data[j].dst_pitch*MT_Data[j].src_h_min+(MT_Data[j].dst_pitch >> 1)));
						}

						if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

						for(uint8_t j=0; j<threads_number; j++)
							MT_Thread[j].f_process=0;

						poolInterface->ReleaseThreadPool(UserId,true);
					}
				}
				else
				{
					if (mData.setting_mode)
					{
						switch(mData.filter_mode)
						{
							case 1 :
								square_median_filter_2(0);
								MT_Data[0].src=(void *)(ptrSrc[i]+(MT_Data[0].src_pitch*MT_Data[0].src_h_min+(MT_Data[0].src_pitch >> 1)));
								MT_Data[0].dst=(void *)(ptrDst[i]+(MT_Data[0].dst_pitch*MT_Data[0].src_h_min+(MT_Data[0].dst_pitch >> 1)));
								square_median_filter_2(0);
								break;
							case 2 :
								horizontal_median_filter_2(0);
								MT_Data[0].src=(void *)(ptrSrc[i]+(MT_Data[0].src_pitch*MT_Data[0].src_h_min+(MT_Data[0].src_pitch >> 1)));
								MT_Data[0].dst=(void *)(ptrDst[i]+(MT_Data[0].dst_pitch*MT_Data[0].src_h_min+(MT_Data[0].dst_pitch >> 1)));
								horizontal_median_filter_2(0);
								break;
							case 3 :
								vertical_median_filter_2(0);
								MT_Data[0].src=(void *)(ptrSrc[i]+(MT_Data[0].src_pitch*MT_Data[0].src_h_min+(MT_Data[0].src_pitch >> 1)));
								MT_Data[0].dst=(void *)(ptrDst[i]+(MT_Data[0].dst_pitch*MT_Data[0].src_h_min+(MT_Data[0].dst_pitch >> 1)));
								vertical_median_filter_2(0);
								break;
						}
					}
					else
					{
						switch(mData.filter_mode)
						{
							case 1 :
								square_median_filter(0);
								MT_Data[0].src=(void *)(ptrSrc[i]+(MT_Data[0].src_pitch*MT_Data[0].src_h_min+(MT_Data[0].src_pitch >> 1)));
								MT_Data[0].dst=(void *)(ptrDst[i]+(MT_Data[0].dst_pitch*MT_Data[0].src_h_min+(MT_Data[0].dst_pitch >> 1)));
								square_median_filter(0);
								break;
							case 2 :
								horizontal_median_filter(0);
								MT_Data[0].src=(void *)(ptrSrc[i]+(MT_Data[0].src_pitch*MT_Data[0].src_h_min+(MT_Data[0].src_pitch >> 1)));
								MT_Data[0].dst=(void *)(ptrDst[i]+(MT_Data[0].dst_pitch*MT_Data[0].src_h_min+(MT_Data[0].dst_pitch >> 1)));
								horizontal_median_filter(0);
								break;
							case 3 :
								vertical_median_filter(0);
								MT_Data[0].src=(void *)(ptrSrc[i]+(MT_Data[0].src_pitch*MT_Data[0].src_h_min+(MT_Data[0].src_pitch >> 1)));
								MT_Data[0].dst=(void *)(ptrDst[i]+(MT_Data[0].dst_pitch*MT_Data[0].src_h_min+(MT_Data[0].dst_pitch >> 1)));
								vertical_median_filter(0);
								break;
						}
					}
				}
			}
			else
			{
				switch(i)
				{
					case 0 :
						h_current=idata.src_h0;
						w_current=idata.src_w0;
						break;
					case 1 :
						h_current=idata.src_h1;
						w_current=idata.src_w1;
						break;
					case 2 :
						h_current=idata.src_h2;
						w_current=idata.src_w2;
						break;
				}
				
				if (use_buffer) memcpy(buffer_out[i],buffer_in[i],buffer_size[i]);
				else
				{
					for(int32_t j=0; j<h_current; j++)
					{
						memcpy(ptrDst[i],ptrSrc[i],w_current);
						ptrSrc[i]+=srcPitch[i];
						ptrDst[i]+=dstPitch[i];
					}
				}
			}
		}
	}
	else		// End of Interlace part
	{
		for (i=0; i<plan_max; i++)
		{
			if (!mData.filter_disable[i])
			{
				for (uint8_t j=0; j<threads_number; j++)
				{
					MT_Data[j].size_data=size_data[i];
					MT_Data[j].threshold=mData.threshold[i];
					MT_Data[j].data=Tdata[j];
					MT_Data[j].m_size=mData.median_size[i];
					MT_Data[j].src_pitch=srcPitch[i];
					MT_Data[j].dst_pitch=dstPitch[i];
					switch(i)
					{
						case 0 :
							MT_Data[j].src_w=MT_Data[j].src_Y_w;
							MT_Data[j].src_h_min=MT_Data[j].src_Y_h_min;
							MT_Data[j].src_h_max=MT_Data[j].src_Y_h_max;
							break;
						case 1 :
						case 2 :
							MT_Data[j].src_w=MT_Data[j].src_UV_w;
							MT_Data[j].src_h_min=MT_Data[j].src_UV_h_min;
							MT_Data[j].src_h_max=MT_Data[j].src_UV_h_max;
							break;
					}
					MT_Data[j].src=(void *)(ptrSrc[i]+(MT_Data[j].src_pitch*MT_Data[j].src_h_min));
					MT_Data[j].dst=(void *)(ptrDst[i]+(MT_Data[j].dst_pitch*MT_Data[j].src_h_min));
				}

				if (threads_number>1)
				{
					if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
					{
						uint8_t f_proc=0;

						if (mData.setting_mode)
						{
							switch(mData.filter_mode)
							{
								case 1 : f_proc=4; break;
								case 2 : f_proc=5; break;
								case 3 : f_proc=6; break;
							}
						}
						else
						{
							switch(mData.filter_mode)
							{
								case 1 : f_proc=1; break;
								case 2 : f_proc=2; break;
								case 3 : f_proc=3; break;
							}
						}

						for(uint8_t j=0; j<threads_number; j++)
							MT_Thread[j].f_process=f_proc;

						if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

						for(uint8_t j=0; j<threads_number; j++)
							MT_Thread[j].f_process=0;

						poolInterface->ReleaseThreadPool(UserId,true);
					}
				}
				else
				{
					if (mData.setting_mode)
					{
						switch(mData.filter_mode)
						{
							case 1 : square_median_filter_2(0); break;
							case 2 : horizontal_median_filter_2(0); break;
							case 3 : vertical_median_filter_2(0); break;
						}
					}
					else
					{
						switch(mData.filter_mode)
						{
							case 1 : square_median_filter(0); break;
							case 2 : horizontal_median_filter(0); break;
							case 3 : vertical_median_filter(0); break;
						}
					}
				}
			}
			else
			{
				switch(i)
				{
					case 0 :
						h_current=idata.src_h0;
						w_current=idata.src_w0;
						break;
					case 1 :
						h_current=idata.src_h1;
						w_current=idata.src_w1;
						break;
					case 2 :
						h_current=idata.src_h2;
						w_current=idata.src_w2;
						break;
				}

				if (use_buffer) memcpy(buffer_out[i],buffer_in[i],buffer_size[i]);
				else
				{
					for(int32_t j=0; j<h_current; j++)
					{
						memcpy(ptrDst[i],ptrSrc[i],w_current);
						ptrSrc[i]+=srcPitch[i];
						ptrDst[i]+=dstPitch[i];
					}
				}
			}
		}
	}

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			if (AVX_Enable)
			{
				JPSDR_Median_RGB32_Move_dst_AVX_2(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+3)>>2,idata.src_h0,buffer_pitch[0],idata.dst_pitch0);
			}
			else
			{
				if (SSE2_Enable)
				{
					JPSDR_Median_RGB32_Move_dst_SSE_2(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
						(idata.src_w0+3)>>2,idata.src_h0,buffer_pitch[0],idata.dst_pitch0);
				}
				else
					JPSDR_Median_RGB32_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
						idata.src_w0,idata.src_h0,buffer_modulo[0],idata.dst_modulo0);
			}
			break;
		case 2 :
			if (AVX_Enable)
			{
				JPSDR_Median_YUYV_Move_dst_AVX_2(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+7)>>3,idata.src_h0,buffer_pitch[0],buffer_pitch[1],idata.dst_pitch0);
			}
			else
			{
				if (SSE2_Enable)
				{
					JPSDR_Median_YUYV_Move_dst_SSE_2(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
						(idata.src_w0+7)>>3,idata.src_h0,buffer_pitch[0],buffer_pitch[1],idata.dst_pitch0);
				}
				else
					JPSDR_Median_YUYV_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
						(idata.src_w0+1)>>1,idata.src_h0,buffer_modulo[0],buffer_modulo[1],idata.dst_modulo0);
			}
			break;
		case 3 :
			if (AVX_Enable)
			{
				JPSDR_Median_UYVY_Move_dst_AVX_2(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+7)>>3,idata.src_h0,buffer_pitch[0],buffer_pitch[1],idata.dst_pitch0);
			}
			else
			{
				if (SSE2_Enable)
				{
					JPSDR_Median_UYVY_Move_dst_SSE_2(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
						(idata.src_w0+7)>>3,idata.src_h0,buffer_pitch[0],buffer_pitch[1],idata.dst_pitch0);
				}
				else
					JPSDR_Median_UYVY_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
						(idata.src_w0+1)>>1,idata.src_h0,buffer_modulo[0],buffer_modulo[1],idata.dst_modulo0);
			}
			break;
	}

}





void JPSDR_Median::Start()
{
	Image_Data idata;
	bool ok;
	int32_t i;
	uint32_t size_data_max;
	int32_t h,w;

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

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			buffer_pitch[0]=((idata.src_w0+ALIGN_SIZE-1) >> ALIGN_SHIFT) << ALIGN_SHIFT;
			buffer_pitch[1]=buffer_pitch[0];
			buffer_pitch[2]=buffer_pitch[0];
			buffer_modulo[0]=buffer_pitch[0]-idata.src_w0;
			buffer_modulo[1]=buffer_modulo[0];
			buffer_modulo[2]=buffer_modulo[0];
			buffer_size[0]=idata.src_h0*(uint32_t)buffer_pitch[0];
			buffer_size[1]=buffer_size[0];
			buffer_size[2]=buffer_size[0];
			break;
		case 2 :
		case 3 :
			buffer_pitch[0]=((idata.src_w0+ALIGN_SIZE-1) >> ALIGN_SHIFT) << ALIGN_SHIFT;
			buffer_pitch[1]=((((idata.src_w0+1)>>1)+ALIGN_SIZE-1) >> ALIGN_SHIFT) << ALIGN_SHIFT;
			buffer_pitch[2]=buffer_pitch[1];
			buffer_modulo[0]=buffer_pitch[0]-idata.src_w0;
			buffer_modulo[1]=buffer_pitch[1]-((idata.src_w0+1)>>1);
			buffer_modulo[2]=buffer_modulo[1];
			buffer_size[0]=idata.src_h0*(uint32_t)buffer_pitch[0];
			buffer_size[1]=idata.src_h0*(uint32_t)buffer_pitch[1];
			buffer_size[2]=buffer_size[1];
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
		case 9 :
			buffer_pitch[0]=0;
			buffer_pitch[1]=0;
			buffer_pitch[2]=0;
			buffer_modulo[0]=0;
			buffer_modulo[1]=0;
			buffer_modulo[2]=0;
			buffer_size[0]=0;
			buffer_size[1]=0;
			buffer_size[2]=0;
			break;
	}

	max_median_size=0;

	for (i=0; i<3; i++)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				w=idata.src_w0;
				h=idata.src_h0;
				break;
			case 2 :
			case 3 :
				switch(i)
				{
					case 0 :
						w=idata.src_w0;
						h=idata.src_h0;
						break;
					case 1 :
					case 2 :
						w=(idata.src_w0+1)>>1;
						h=idata.src_h0;
						break;
				}
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				switch(i)
				{
					case 0 :
						w=idata.src_w0;
						h=idata.src_h0;
						break;
					case 1 :
						w=idata.src_w1;
						h=idata.src_h1;
						break;
					case 2 :
						w=idata.src_w2;
						h=idata.src_h2;
						break;
				}
				break;
			case 9 :
				switch(i)
				{
					case 0 :
						w=idata.src_w0;
						h=idata.src_h0;
						break;
					case 1 :
					case 2 :
						continue;
						break;
				}
		}
		if (mData.median_size[i]>=(h >> 1)) mData.median_size[i]=(h >> 1)-1;
		if (mData.median_size[i]>=(w >> 1)) mData.median_size[i]=(w >> 1)-1;
		if (max_median_size<mData.median_size[i]) max_median_size=mData.median_size[i];
	}

	size_data_max=((uint32_t)max_median_size << 1)+1;
	size_data_max*=size_data_max;
	if ((size_data_max & (ALIGN_SIZE-1))!=0)
		size_data_max=((size_data_max + ALIGN_SIZE-1) >> ALIGN_SHIFT) << ALIGN_SHIFT;

	for (i=0; i<3; i++)
	{
		if (buffer_size[i]>0)
			buffer_in[i]=(uint8_t *)_aligned_malloc(buffer_size[i]*sizeof(uint8_t),ALIGN_SIZE);
		else buffer_in[i]=NULL;
	}
	for (i=0; i<3; i++)
	{
		if (buffer_size[i]>0)
			buffer_out[i]=(uint8_t *)_aligned_malloc(buffer_size[i]*sizeof(uint8_t),ALIGN_SIZE);
		else buffer_out[i]=NULL;
	}

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if  (mData.mt_mode && (idata.src_h0>=32) && (idata.dst_h0>=32) ) threads_number=total_cpu;
	else threads_number=1;

	if (mData.interlace_mode)
	{
		switch (idata.src_video_mode)
		{
			case 0 :
			case 1 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,0,0,0);
				break;
			case 4 :
			case 9 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,0,0,2);
				break;
			case 2 :
			case 3 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,1,0,1);
				break;
			case 5 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,1,0,2);
				break;
			case 6 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,1,1,2);
				break;
			case 7 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,2,0,2);
				break;
			case 8 :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,2,2,2);
				break;
			default :
				threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0>>1,0,0,0);
				break;
		}
	}
	else
	{
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
	}

	for (i=0; i<threads_number; i++)
		Tdata[i]=(uint8_t *)_aligned_malloc(size_data_max*sizeof(uint8_t),ALIGN_SIZE);

	ok=true;
	for (i=0; i<3; i++)
		if (buffer_size[i]>0) ok=ok && (buffer_in[i]!=NULL);
	for (i=0; i<3; i++)
		if (buffer_size[i]>0) ok=ok && (buffer_out[i]!=NULL);
	for (i=0; i<threads_number; i++)
		ok=ok && (Tdata[i]!=NULL);

	if (!ok)
	{
		ff->ExceptOutOfMemory();
		return;
	}

	for (i=0; i<threads_number; i++)
		memset(Tdata[i],0,size_data_max);

	if (threads_number>1)
	{
		if (!threadpoolAllocated)
		{
			ff->Except("Error with the TheadPool while allocating threadpool!");
			return;
		}
	}
}



void JPSDR_Median::End()
{
	int16_t i;

	for (i=threads_number-1; i>=0; i--)
		my_aligned_free(Tdata[i]);
	for (i=2; i>=0; i--)
		my_aligned_free(buffer_out[i]);
	for (i=2; i>=0; i--)
		my_aligned_free(buffer_in[i]);
}



bool JPSDR_Median::Configure(VDXHWND hwnd)
{
	JPSDR_MedianDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}




void JPSDR_Median::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
    mData.median_size[0]=argv[0].asInt();
	mData.median_size[1]=argv[1].asInt();
	mData.median_size[2]=argv[2].asInt();
    mData.threshold[0]=argv[3].asInt();
	mData.threshold[1]=argv[4].asInt();
	mData.threshold[2]=argv[5].asInt();
	mData.setting_mode=!!argv[6].asInt();
	mData.filter_mode=argv[7].asInt();
	mData.interlace_mode=!!argv[8].asInt();
	mData.filter_disable[0]=!!argv[9].asInt();
	mData.filter_disable[1]=!!argv[10].asInt();
	mData.filter_disable[2]=!!argv[11].asInt();
	mData.mt_mode=!!argv[12].asInt();
}



void JPSDR_Median::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf, maxlen,"Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
        mData.median_size[0],mData.median_size[1],mData.median_size[2],
        mData.threshold[0],mData.threshold[1],mData.threshold[2],mData.setting_mode,mData.filter_mode,
		mData.interlace_mode,mData.filter_disable[0],mData.filter_disable[1],mData.filter_disable[2],
		mData.mt_mode);
}

extern VDXFilterDefinition filterDef_JPSDR_Median=
VDXVideoFilterDefinition<JPSDR_Median>("JPSDR","Median v3.3.2","Median filter with threshold.");
