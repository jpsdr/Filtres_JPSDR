#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

#include "..\asmlib\asmlib.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

extern "C" int IInstrSet;

extern "C" void JPSDR_Saturation_YUYV(const void *src,void *dst,int32_t w,int32_t h,
	int16_t *lookup,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_UYVY(const void *src,void *dst,int32_t w,int32_t h,
	int16_t *lookup,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_Planar_YUV(const void *src,void *dst,int32_t w,int32_t h,
	int16_t *lookup,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_Hue_Planar_YUV(const void *src_U,const void *src_V,
	void *dst_U,void *dst_V,int32_t w,int32_t h,uint16_t *lookup_cbcr,
	ptrdiff_t src_U_modulo,ptrdiff_t src_V_modulo,ptrdiff_t dst_U_modulo,
	ptrdiff_t dst_V_modulo);

extern "C" void JPSDR_Saturation_SSE2_RGB24(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_R,int16_t offset_G,int16_t offset_B,int16_t *lookup,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_Non_SSE_RGB24(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_R,int16_t offset_G,int16_t offset_B,int16_t *lookup,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo);

extern "C" void JPSDR_Saturation_Y_YUYV(const void *src,void *dst,int32_t w,int32_t h,
	uint8_t offset_U,uint8_t offset_V,int16_t *lookup,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_Y_UYVY(const void *src,void *dst,int32_t w,int32_t h,
	uint8_t offset_U,uint8_t offset_V,int16_t *lookup,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_Y_SSE2_RGB24(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t *lookup,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Saturation_Y_Non_SSE_RGB24(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t *lookup,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);


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


#define Min_Y 16
#define Max_Y 235
#define Min_U 16
#define Max_U 240
#define Min_V 16
#define Max_V 240


class JPSDR_SaturationData
{
public:
	int16_t saturation,brightness,contrast,hue;
	bool BW_mode,full_YCbCr_Y,full_YCbCr_Cb,full_YCbCr_Cr;
	uint8_t full_mode,color_space;
	bool mt_mode;

	JPSDR_SaturationData(void);
};

JPSDR_SaturationData::JPSDR_SaturationData(void)
{
	saturation=0;
	brightness=0;
	contrast=0;
	hue=0;
	BW_mode=false;
	full_YCbCr_Y=false;
	full_YCbCr_Cb=false;
	full_YCbCr_Cr=false;
	full_mode=1;
	color_space=1;
	mt_mode=true;
}

class JPSDR_Saturation;


class JPSDR_SaturationDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_SaturationDialog(JPSDR_SaturationData& Data, IVDXFilterPreview2 *ifp, JPSDR_Saturation *filt) : mData(Data),mifp(ifp),filter(filt)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_SATURATION),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_SaturationData& mData;
	JPSDR_SaturationData moldData;
	IVDXFilterPreview2 *const mifp;
	JPSDR_Saturation *const filter;
};


class JPSDR_Saturation : public VDXVideoFilter
{
public:
	virtual bool Init();
	virtual uint32 GetParams();
	virtual void Start();
	virtual void End();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	void Compute_Lookup(void);

	VDXVF_DECLARE_SCRIPT_METHODS();

protected:
	Image_Data image_data;
	int16_t lookup[2304],offset_R,offset_G,offset_B,offset_Y,offset_U,offset_V;
	bool SSE_Integer_Enable,SSE2_Enable;
	uint16_t lookup_hue_CbCr[65536];
	size_t CPU_Cache_Size,Cache_Setting;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits);

	void Saturation_YUYV(uint8_t thread_num);
	void Saturation_UYVY(uint8_t thread_num);
	void Saturation_Planar_Y(uint8_t thread_num);
	void Saturation_Planar_U(uint8_t thread_num);
	void Saturation_Planar_V(uint8_t thread_num);
	void Saturation_Hue_Planar_YUV(uint8_t thread_num);
	void Saturation_RGB24(uint8_t thread_num);

	void Saturation_Y_YUYV(uint8_t thread_num);
	void Saturation_Y_UYVY(uint8_t thread_num);
	void Saturation_Y_RGB24(uint8_t thread_num);

	inline void Planar_Fill(void *dst_, const int32_t w,const int32_t h,const uint8_t offset,
		ptrdiff_t dst_pitch);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_SaturationData mData;
};


VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Saturation)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Saturation,ScriptConfig,"iiiiiiiiiii")
VDXVF_END_SCRIPT_METHODS()


INT_PTR JPSDR_SaturationDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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

bool JPSDR_SaturationDialog::OnInit()
{
	bool ena_full;

	moldData=mData;

	switch (mData.color_space)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_BT709,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_BT601,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_SMPTE_240M,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_FCC,1); break;
	}

	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_BW,mData.BW_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_FULL_YCbCr_Y,mData.full_YCbCr_Y?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_FULL_YCbCr_Cb,mData.full_YCbCr_Cb?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_FULL_YCbCr_Cr,mData.full_YCbCr_Cr?BST_CHECKED:BST_UNCHECKED);
	SetDlgItemInt(mhdlg,IDC_SATURATION,mData.saturation,TRUE);
	SetDlgItemInt(mhdlg,IDC_BRIGHTNESS,mData.brightness,TRUE);
	SetDlgItemInt(mhdlg,IDC_CONTRAST,mData.contrast,TRUE);
	SetDlgItemInt(mhdlg,IDC_HUE,mData.hue,TRUE);
	EnableWindow(GetDlgItem(mhdlg,IDC_JPSDR_SAT_EXT),!mData.BW_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_SATURATION),!mData.BW_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_JPSDR_HUE_EXT),!mData.BW_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_HUE),!mData.BW_mode);
	ena_full=mData.full_YCbCr_Cb || mData.full_YCbCr_Cr || mData.full_YCbCr_Y;
	switch (mData.full_mode)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_FULL_IN,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_FULL_OUT,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_FULL_IN_OUT,1); break;
	}
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_MODE),ena_full);
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_IN),ena_full);
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_OUT),ena_full);
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_IN_OUT),ena_full);

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}


void JPSDR_SaturationDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_SaturationDialog::SaveToData()
{
	BOOL success;
	sint16 val;

	if (!!IsDlgButtonChecked(mhdlg,IDC_BT709)) mData.color_space=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_BT601)) mData.color_space=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_SMPTE_240M)) mData.color_space=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_FCC)) mData.color_space=3;

	if (!!IsDlgButtonChecked(mhdlg,IDC_FULL_IN)) mData.full_mode=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_FULL_OUT)) mData.full_mode=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_FULL_IN_OUT)) mData.full_mode=2;

	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);
	mData.BW_mode=!!IsDlgButtonChecked(mhdlg,IDC_BW);
	mData.full_YCbCr_Y=!!IsDlgButtonChecked(mhdlg,IDC_FULL_YCbCr_Y);
	mData.full_YCbCr_Cb=!!IsDlgButtonChecked(mhdlg,IDC_FULL_YCbCr_Cb);
	mData.full_YCbCr_Cr=!!IsDlgButtonChecked(mhdlg,IDC_FULL_YCbCr_Cr);
	val=GetDlgItemInt(mhdlg,IDC_SATURATION,&success,TRUE);
	if (success)
	{
		if (val<-100) val=-100;
		mData.saturation=val;
	}
	val=GetDlgItemInt(mhdlg,IDC_HUE,&success,TRUE);
	if (success)
	{
		if (val<-360) val=-360;
		if (val>360) val=360;
		mData.hue=val;
	}
	val=GetDlgItemInt(mhdlg,IDC_BRIGHTNESS,&success,TRUE);
	if (success)
	{
		if (val<-255) val=-255;
		if (val>255) val=255;
		mData.brightness=val;
	}
	val=GetDlgItemInt(mhdlg,IDC_CONTRAST,&success,TRUE);
	if (success)
	{
		if (val<-100) val=-100;
		mData.contrast=val;
	}

	return true;
}


bool JPSDR_SaturationDialog::OnCommand(int cmd)
{
	bool ena_full;

	switch(cmd)
	{
		case IDC_BW :
			mData.BW_mode=!!IsDlgButtonChecked(mhdlg,IDC_BW);
			if (mifp)
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			EnableWindow(GetDlgItem(mhdlg,IDC_JPSDR_SAT_EXT),!mData.BW_mode);
			EnableWindow(GetDlgItem(mhdlg,IDC_SATURATION),!mData.BW_mode);
			EnableWindow(GetDlgItem(mhdlg,IDC_JPSDR_HUE_EXT),!mData.BW_mode);
			EnableWindow(GetDlgItem(mhdlg,IDC_HUE),!mData.BW_mode);
			return true;
		case IDC_SATURATION :
		case IDC_HUE :
		case IDC_BRIGHTNESS :
		case IDC_CONTRAST :
		case IDC_FULL_IN :
		case IDC_FULL_OUT :
		case IDC_FULL_IN_OUT :
		case IDC_BT709 :
		case IDC_BT601 :
		case IDC_FCC :
		case IDC_SMPTE_240M :
			if (mifp && SaveToData())
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			return true;
		case IDC_FULL_YCbCr_Y :
		case IDC_FULL_YCbCr_Cb :
		case IDC_FULL_YCbCr_Cr :
			SaveToData();
			ena_full=mData.full_YCbCr_Cb || mData.full_YCbCr_Cr || mData.full_YCbCr_Y;
			EnableWindow(GetDlgItem(mhdlg,IDC_FULL_MODE),ena_full);
			EnableWindow(GetDlgItem(mhdlg,IDC_FULL_IN),ena_full);
			EnableWindow(GetDlgItem(mhdlg,IDC_FULL_OUT),ena_full);
			EnableWindow(GetDlgItem(mhdlg,IDC_FULL_IN_OUT),ena_full);
			if (mifp)
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			return true;
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDOK :
			SaveToData();
			EndDialog(mhdlg,true);
			return true;
		case IDCANCEL :
			mData=moldData;
			EndDialog(mhdlg,false);
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}


bool JPSDR_Saturation::Init()
{
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	if (IInstrSet<0) InstructionSet();
	CPU_Cache_Size=DataCacheSize(0)>>2;

	for (uint16_t i=0; i<MAX_MT_THREADS; i++)
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


uint8_t JPSDR_Saturation::CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits)
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


inline void JPSDR_Saturation::Planar_Fill(void *dst_, const int32_t w,const int32_t h,const uint8_t offset,
		ptrdiff_t dst_pitch)
{
	uint8_t *dst=(uint8_t *)dst_;

	if ((h==1) || (abs(dst_pitch)==w))
	{
		if (dst_pitch<0) dst+=(h-1)*dst_pitch;

		A_memset(dst,offset,(size_t)h*(size_t)w);
	}
	else
	{
		for(int32_t i=0; i<h; i++)
		{
			A_memset(dst,offset,w);
			dst+=dst_pitch;
		}
	}
}


uint32 JPSDR_Saturation::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;
	
	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

	if (mData.hue==0)
	{
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
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
				break;
			default : return FILTERPARAM_NOT_SUPPORTED;
		}
	}
	else
	{
		switch(pxsrc.format)
		{
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
				break;
			default : return FILTERPARAM_NOT_SUPPORTED;
		}
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	if ((mData.full_YCbCr_Y || mData.full_YCbCr_Cb || mData.full_YCbCr_Cr) && (mData.full_mode!=0))
	{
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV411_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV410_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				pxdst.format=nsVDXPixmap::kPixFormat_Y8_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420it_Planar_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
				pxdst.format=nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR;
				break;
		}		
	}
	fa->dst.offset = fa->src.offset;

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM);
}



void JPSDR_Saturation::Saturation_YUYV(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_YUYV(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,
		mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}


void JPSDR_Saturation::Saturation_UYVY(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_UYVY(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,
		mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}



void JPSDR_Saturation::Saturation_Planar_Y(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_Planar_YUV(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
		mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}



void JPSDR_Saturation::Saturation_Planar_U(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_Planar_YUV(mt_data_inf.src2,mt_data_inf.dst2,mt_data_inf.src_UV_w,
		mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min,lookup+256,mt_data_inf.src_modulo2,
		mt_data_inf.dst_modulo2);
}


void JPSDR_Saturation::Saturation_Planar_V(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_Planar_YUV(mt_data_inf.src3,mt_data_inf.dst3,mt_data_inf.src_UV_w,
		mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min,lookup+512,mt_data_inf.src_modulo3,
		mt_data_inf.dst_modulo3);
}


void JPSDR_Saturation::Saturation_Hue_Planar_YUV(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_Hue_Planar_YUV(mt_data_inf.src2,mt_data_inf.src3,mt_data_inf.dst2,mt_data_inf.dst3,
		mt_data_inf.src_UV_w,mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min,lookup_hue_CbCr,mt_data_inf.src_modulo2,
		mt_data_inf.src_modulo3,mt_data_inf.dst_modulo2,mt_data_inf.dst_modulo3);
}


void JPSDR_Saturation::Saturation_RGB24(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (SSE2_Enable)
		JPSDR_Saturation_SSE2_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_R,offset_G,offset_B,lookup,
			mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
	else
		JPSDR_Saturation_Non_SSE_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_R,offset_G,offset_B,lookup,
			mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}


void JPSDR_Saturation::Saturation_Y_YUYV(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_Y_YUYV(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,
		mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,(uint8_t)(offset_U>>4),(uint8_t)(offset_V>>4),
		lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);

}


void JPSDR_Saturation::Saturation_Y_UYVY(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_Saturation_Y_UYVY(mt_data_inf.src1,mt_data_inf.dst1,(mt_data_inf.src_Y_w+1)>>1,
		mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,(uint8_t)(offset_U>>4),(uint8_t)(offset_V>>4),
		lookup,mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);

}


void JPSDR_Saturation::Saturation_Y_RGB24(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (SSE2_Enable)
		JPSDR_Saturation_Y_SSE2_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_Y,lookup,mt_data_inf.src_modulo1,
			mt_data_inf.dst_modulo1);
	else
		JPSDR_Saturation_Y_Non_SSE_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_Y,lookup,mt_data_inf.src_modulo1,
			mt_data_inf.dst_modulo1);
}



void JPSDR_Saturation::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_Saturation *ptrClass=(JPSDR_Saturation *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->Saturation_YUYV(data->thread_Id); break;
		case 2 : ptrClass->Saturation_UYVY(data->thread_Id); break;
		case 3 : ptrClass->Saturation_RGB24(data->thread_Id); break;
		case 4 : ptrClass->Saturation_Y_YUYV(data->thread_Id); break;
		case 5 : ptrClass->Saturation_Y_UYVY(data->thread_Id); break;
		case 6 : ptrClass->Saturation_Y_RGB24(data->thread_Id); break;
		case 7 : ptrClass->Saturation_Planar_Y(data->thread_Id); break;
		case 8 : ptrClass->Saturation_Planar_U(data->thread_Id); break;
		case 9 : ptrClass->Saturation_Planar_V(data->thread_Id); break;
		case 10 : ptrClass->Saturation_Hue_Planar_YUV(data->thread_Id); break;
		default : ;
	}
}


void JPSDR_Saturation::Run()
{
	Image_Data idata;
	bool chg_scale_Y,chg_scale_UV;

	idata=image_data;

	if ((idata.src_w0==0) || (idata.src_h0==0)) return;
	if (mData.full_YCbCr_Y && (mData.full_mode!=2)) chg_scale_Y=true;
	else chg_scale_Y=false;
	if ((mData.full_YCbCr_Cb || mData.full_YCbCr_Cr) && (mData.full_mode!=2)) chg_scale_UV=true;
	else chg_scale_UV=false;
	if ((mData.brightness==0) && (mData.contrast==0) && (mData.hue==0) && (mData.saturation==0) && (!mData.BW_mode) 
		&& (!chg_scale_Y) && (!chg_scale_UV)) return;

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
		uint8_t f_proc=0,f_proc2=0;

		if (mData.BW_mode)
		{
			switch (idata.video_mode)
			{
				case 0 :
				case 1 : f_proc=6; break;
				case 2 : f_proc=4; break;
				case 3 : f_proc=5; break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) f_proc=7;
					Planar_Fill(idata.dst_plane1,idata.src_w1,idata.src_h1,(uint8_t)(offset_U>>4),
						idata.dst_pitch1);
					Planar_Fill(idata.dst_plane2,idata.src_w2,idata.src_h2,(uint8_t)(offset_V>>4),
						idata.dst_pitch2);
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) f_proc=7;
					break;
			}
		}
		else
		{
			switch (idata.video_mode)
			{
				case 0 :
				case 1 : f_proc=3; break;
				case 2 : f_proc=1; break;
				case 3 : f_proc=2; break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) f_proc=7;
					if (mData.hue==0)
					{
						if ((mData.saturation!=0) || chg_scale_UV) f_proc2=8;
					}
					else f_proc2=10;
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) f_proc=7;
					break;
			}
		}

		if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,0,false))
		{
			for(uint8_t i=0; i<threads_number; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<threads_number; i++)
				MT_Thread[i].f_process=0;

			if (f_proc2!=0)
			{
				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=f_proc2;

				if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

				for(uint8_t i=0; i<threads_number; i++)
					MT_Thread[i].f_process=0;

				if (f_proc2==8)
				{
					for(uint8_t i=0; i<threads_number; i++)
						MT_Thread[i].f_process=9;

					if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

					for(uint8_t i=0; i<threads_number; i++)
						MT_Thread[i].f_process=0;
				}
			}

			poolInterface->ReleaseThreadPool(UserId);
		}
	}
	else
	{
		if (mData.BW_mode)
		{
			switch (idata.video_mode)
			{
				case 0 :
				case 1 : Saturation_Y_RGB24(0); break;
				case 2 : Saturation_Y_YUYV(0); break;
				case 3 : Saturation_Y_UYVY(0); break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) Saturation_Planar_Y(0);
					Planar_Fill(idata.dst_plane1,idata.src_w1,idata.src_h1,(uint8_t)(offset_U>>4),
						idata.dst_pitch1);
					Planar_Fill(idata.dst_plane2,idata.src_w2,idata.src_h2,(uint8_t)(offset_V>>4),
						idata.dst_pitch2);
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) Saturation_Planar_Y(0);
					break;
			}
		}
		else
		{
			switch (idata.video_mode)
			{
				case 0 :
				case 1 : Saturation_RGB24(0); break;
				case 2 : Saturation_YUYV(0); break;
				case 3 : Saturation_UYVY(0); break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) Saturation_Planar_Y(0);
					if (mData.hue==0)
					{
						if ((mData.saturation!=0) || chg_scale_UV)
						{
							Saturation_Planar_U(0);
							Saturation_Planar_V(0);
						}
					}
					else Saturation_Hue_Planar_YUV(0);
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale_Y) Saturation_Planar_Y(0);
					break;
			}
		}
	}

}


void JPSDR_Saturation::Compute_Lookup(void)
{
	int32_t i,j;
	double alpha,beta,r_r,r_g,r_b,g_r,g_g,g_b,b_r,b_g,b_b;
	double y_r,y_g,y_b;
	double kr,kg,kb;
	double u,v,angle,l;
	int16_t miny,maxy,minu,maxu,minv,maxv;
	int32_t offs_Y_In,offs_Y_Out;
	uint16_t cb,cr;
	double coeff_U_In,coeff_V_In,coeff_U_Out,coeff_V_Out;
	double scale_Y,scale_U,scale_V;
	Image_Data idata;

	idata=image_data;

	/*
	Y=kr*R+kg*G+kb*B
	U=(B-Y)x... -> Cb
	V=(R-Y)x... -> Cr
	Modifs -> Cr'=alpha*Cr , Cb'=alpha*Cb , Y'=beta*(Y+brightnes)
	R',G',B' = linear comb de Y',Cr',Cb' => linear comb R,G,B

	U=0,436x(B-Y)/(1-kb)
	V=0,615x(R-Y)/(1-kr)
	Pb=(Ux0,5)/0,436  Pr=(Vx0,5)/0,615
	Cb=128+224xPb     Cr=128+224xPr
	*/

	if (mData.full_YCbCr_Y)
	{
		switch(mData.full_mode)
		{
			case 0 : offs_Y_In=0; offs_Y_Out=Min_Y; miny=Min_Y; maxy=Max_Y; scale_Y=(Max_Y-Min_Y)/255.0; break;
			case 1 : offs_Y_In=Min_Y; offs_Y_Out=0; miny=0; maxy=255; scale_Y=255.0/(Max_Y-Min_Y); break;
			case 2 : offs_Y_In=0; offs_Y_Out=0; miny=0; maxy=255; scale_Y=1.0; break;
		}
	}
	else
	{
		miny=Min_Y;
		maxy=Max_Y;
		offs_Y_In=Min_Y;
		offs_Y_Out=Min_Y;
		scale_Y=1.0;
	}

	if (mData.full_YCbCr_Cb)
	{
		switch(mData.full_mode)
		{
			case 0 : coeff_U_In=255.0; coeff_U_Out=Max_U-Min_U; minu=Min_U; maxu=Max_U; scale_U=(Max_U-Min_U)/255.0; break;
			case 1 : coeff_U_In=Max_U-Min_U; coeff_U_Out=255.0; minu=0; maxu=255; scale_U=255.0/(Max_U-Min_U); break;
			case 2 : coeff_U_In=255.0; coeff_U_Out=255.0; minu=0; maxu=255; scale_U=1.0; break;
		}
	}
	else
	{
		minu=Min_U;
		maxu=Max_U;
		coeff_U_In=Max_U-Min_U;
		coeff_U_Out=Max_U-Min_U;
		scale_U=1.0;
	}

	if (mData.full_YCbCr_Cr)
	{
		switch(mData.full_mode)
		{
			case 0 : coeff_V_In=255.0; coeff_V_Out=Max_V-Min_V; minv=Min_V; maxv=Max_V; scale_V=(Max_V-Min_V)/255.0; break;
			case 1 : coeff_V_In=Max_V-Min_V; coeff_V_Out=255.0; minv=0; maxv=255; scale_V=255.0/(Max_V-Min_V); break;
			case 2 : coeff_V_In=255.0; coeff_V_Out=255.0; minv=0; maxv=255; scale_V=1.0; break;
		}
	}
	else
	{
		minv=Min_V;
		maxv=Max_V;
		coeff_V_In=Max_V-Min_V;
		coeff_V_Out=Max_V-Min_V;
		scale_V=1.0;
	}

	switch(mData.color_space)
	{
		case 0 : kr=0.2126; kb=0.0722; break;
		case 1 : kr=0.299; kb=0.114; break;
		case 2 : kr=0.212; kb=0.087; break;
		case 3 : kr=0.11; kb=0.3; break;
	}
	kg=1.0-kr-kb;
	alpha=1.0+(mData.saturation/100.0);
	beta=1.0+(mData.contrast/100.0);
	r_r=alpha*(1.0-kr)+beta*kr;
	r_g=kg*(beta-alpha);
	r_b=kb*(beta-alpha);
	b_r=kr*(beta-alpha);
	b_g=kg*(beta-alpha);
	b_b=alpha*(1.0-kb)+beta*kb;
	g_r=kr*(beta-alpha);
	g_g=beta-(beta-alpha)*(kr+kb);
	g_b=kb*(beta-alpha);
	y_r=beta*kr;
	y_g=beta*kg;
	y_b=beta*kb;
	offset_R=(int16_t)round(beta*mData.brightness*16.0+8.0);
	offset_G=(int16_t)round(beta*mData.brightness*16.0+8.0);
	offset_B=(int16_t)round(beta*mData.brightness*16.0+8.0);
	offset_Y=(int16_t)round((beta*(mData.brightness-offs_Y_In*scale_Y)+offs_Y_Out)*16.0+8.0);
	if (mData.BW_mode)
	{
		offset_U=(int16_t)round(128.0*16.0+8.0);
		offset_V=(int16_t)round(128.0*16.0+8.0);
	}
	else
	{
		offset_U=(int16_t)round(128.0*(1-alpha*scale_U)*16.0+8.0);
		offset_V=(int16_t)round(128.0*(1-alpha*scale_V)*16.0+8.0);
	}

	switch (idata.video_mode)
	{
		case 0 :
		case 1 :
			if (mData.BW_mode)
			{
					for (i=0; i<256; i++)
					{
						lookup[i]=(int16_t)round(i*y_r*16.0);
						lookup[i+256]=(int16_t)round(i*y_g*16.0);
						lookup[i+512]=(int16_t)round(i*y_b*16.0);
					}
			}
			else
			{
					for (i=0; i<256; i++)
					{
						lookup[i]=(int16_t)round(i*r_r*16.0);
						lookup[i+256]=(int16_t)round(i*r_g*16.0);
						lookup[i+512]=(int16_t)round(i*r_b*16.0);
						lookup[i+768]=(int16_t)round(i*g_r*16.0);
						lookup[i+1024]=(int16_t)round(i*g_g*16.0);
						lookup[i+1280]=(int16_t)round(i*g_b*16.0);
						lookup[i+1536]=(int16_t)round(i*b_r*16.0);
						lookup[i+1792]=(int16_t)round(i*b_g*16.0);
						lookup[i+2048]=(int16_t)round(i*b_b*16.0);
					}
			}
			break;
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
		case 9 :
			for (i=0; i<256; i++)
			{
				lookup[i]=(int16_t)(((int16_t)round(beta*i*16.0*scale_Y)+offset_Y)>>4);		// Lookup Y
				if (lookup[i]<miny)
				{
					lookup[i]=miny;
				}
				if (lookup[i]>maxy)
				{
					lookup[i]=maxy;
				}
			}
			if (!mData.BW_mode)
			{
					for (i=0; i<256; i++)
					{
						lookup[i+256]=(int16_t)(((int16_t)round(alpha*i*16.0*scale_U)+offset_U)>>4);		// Lookup U
						lookup[i+512]=(int16_t)(((int16_t)round(alpha*i*16.0*scale_V)+offset_V)>>4);		// Lookup V
						if (lookup[i+256]<minu)
						{
							lookup[i+256]=minu;
						}
						if (lookup[i+256]>maxu)
						{
							lookup[i+256]=maxu;
						}
						if (lookup[i+512]<minv)
						{
							lookup[i+512]=minv;
						}
						if (lookup[i+512]>maxv)
						{
							lookup[i+512]=maxv;
						}
					}
			}
			break;
	}

	// i=Cb j=Cr
	for (i=0; i<256; i++)
	{
		for (j=0; j<256; j++)
		{
			u=((i-128.0)/coeff_U_In)*0.436/0.5;
			v=((j-128.0)/coeff_V_In)*0.615/0.5;
			angle=atan2(v,u);
			angle=180.0*angle/M_PI;
			l=sqrt(u*u+v*v)*alpha;
			angle=angle+mData.hue;
			angle=(angle/180.0)*M_PI;
			u=l*cos(angle);
			v=l*sin(angle);
			u=128.0+coeff_U_Out*(u*0.5/0.436);
			v=128.0+coeff_V_Out*(v*0.5/0.615);
			if (u<minu) u=minu;
			if (u>maxu) u=maxu;
			if (v<minv) v=minv;
			if (v>maxv) v=maxv;
			cb=(uint16_t)round(u);
			cr=(uint16_t)round(v);
			lookup_hue_CbCr[i*256+j]=(cb << 8)+cr;
		}
	}
}


void JPSDR_Saturation::Start()
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

	if  (mData.mt_mode && (idata.src_h0>=32) && (idata.dst_h0>=32) )
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

	const size_t img_size=idata.dst_size0+idata.dst_size1+idata.dst_size2;

	if (img_size<=MAX_CACHE_SIZE)
	{
		if (CPU_Cache_Size>=img_size) Cache_Setting=img_size;
		else Cache_Setting=16;
	}
	else Cache_Setting=16;

	Compute_Lookup();
}


void JPSDR_Saturation::End()
{
	if (threadpoolAllocated)
	{
		poolInterface->DeAllocateThreads(UserId);
		UserId=0;
		threadpoolAllocated=false;
	}
}


void JPSDR_Saturation::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf,maxlen, "Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",mData.saturation,mData.hue,mData.brightness,
		mData.contrast,mData.BW_mode,mData.full_YCbCr_Y,mData.full_YCbCr_Cb,mData.full_YCbCr_Cr,mData.full_mode,
		mData.color_space,mData.mt_mode);
}



bool JPSDR_Saturation::Configure(VDXHWND hwnd)
{
	JPSDR_SaturationDialog dlg(mData, fa->ifp2, this);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_Saturation::GetSettingString(char *buf, int maxlen)
{

	char output_string[255];

	if (mData.full_YCbCr_Y) strcpy_s(output_string,255," Full Y");
	else strcpy_s(output_string,255,"");
	if (mData.full_YCbCr_Cb) strcat_s(output_string,255," Full Cb");
	if (mData.full_YCbCr_Cr) strcat_s(output_string,255," Full Cr");
	if (mData.full_YCbCr_Y || mData.full_YCbCr_Cb || mData.full_YCbCr_Cr)
	{
		switch(mData.full_mode)
		{
			case 0 : strcat_s(output_string,255," Input"); break;
			case 1 : strcat_s(output_string,255," Output"); break;
			case 2 : strcat_s(output_string,255," Input & Output"); break;
		}
	}
	switch(mData.color_space)
	{
		case 0 : strcat_s(output_string,255," BT.709"); break;
		case 1 : strcat_s(output_string,255," BT.601"); break;
		case 2 : strcat_s(output_string,255," SMPTE 240M"); break;
		case 3 : strcat_s(output_string,255," FCC"); break;
	}
	if (!mData.BW_mode)
	{
		if (mData.saturation<0) strcat_s(output_string,255," Sat : %d%%");
		else strcat_s(output_string,255," Sat : +%d%%");
		if (mData.hue<0) strcat_s(output_string,255," Hue : %d°");
		else strcat_s(output_string,255," Hue : +%d°");
	}
	else strcat_s(output_string,255," B&W Mode");
	if (mData.brightness<0) strcat_s(output_string,255," Bright : %d");
	else strcat_s(output_string,255," Bright : +%d");
	if (mData.contrast<0) strcat_s(output_string,255," Contr : %d%%");
	else strcat_s(output_string,255," Contr : +%d%%");
	if (!mData.BW_mode)
		SafePrintf(buf,maxlen,output_string,mData.saturation,mData.hue,mData.brightness,mData.contrast);
	else
		SafePrintf(buf,maxlen,output_string,mData.brightness,mData.contrast);
}


void JPSDR_Saturation::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
    mData.saturation=argv[0].asInt();
	mData.hue=argv[1].asInt();
	mData.brightness=argv[2].asInt();
	mData.contrast=argv[3].asInt();
	mData.BW_mode=!!argv[4].asInt();
	mData.full_YCbCr_Y=!!argv[5].asInt();
	mData.full_YCbCr_Cb=!!argv[6].asInt();
	mData.full_YCbCr_Cr=!!argv[7].asInt();
	mData.full_mode=argv[8].asInt();
	mData.color_space=argv[9].asInt();
	mData.mt_mode=!!argv[10].asInt();
}


		
extern VDXFilterDefinition filterDef_JPSDR_Saturation=
VDXVideoFilterDefinition<JPSDR_Saturation>("JPSDR","Sat/Hue/Bright/Contr v4.1.0","[ASM][SSE2] Optimised.");