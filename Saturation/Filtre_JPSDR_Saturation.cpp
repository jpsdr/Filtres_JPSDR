/*
 *  Saturation
 *
 *  Allow to change saturation, brightness and contrast.
 *  Copyright (C) 2005 JPSDR
 *	
 *  Saturation is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  Saturation is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 */
 
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "./Saturation_resource.h"

#include "../Filtres_JPSDR/JPSDRVideoFilter.h"
#include "../Filtres_JPSDR/VideoFilterDialog.h"

#include "../Filtres_JPSDR/Pascal.h"

#include "../Filtres_JPSDR/ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

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

extern "C" void JPSDR_Saturation_AVX_RGB24(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_R,int16_t offset_G,int16_t offset_B,int16_t *lookup,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
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

extern "C" void JPSDR_Saturation_Y_AVX_RGB24(const void *src,void *dst,int32_t w,int32_t h,
	int16_t offset_Y,int16_t *lookup,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
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
	bool BW_mode,full_range;
	uint8_t full_mode,color_space;
	bool mt_mode;
	bool automatic;

	JPSDR_SaturationData(void);
};

JPSDR_SaturationData::JPSDR_SaturationData(void)
{
	saturation=0;
	brightness=0;
	contrast=0;
	hue=0;
	BW_mode=false;
	full_range=false;
	full_mode=1;
	color_space=1;
	mt_mode=true;
	automatic=true;
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
	void RefreshDisplay(void);
	void SetDisplay(void);
	
	JPSDR_SaturationData& mData;
	JPSDR_SaturationData moldData;
	IVDXFilterPreview2 *const mifp;
	JPSDR_Saturation *const filter;
};


class JPSDR_Saturation : public JPSDRVDXVideoFilter
{
public:
	JPSDR_Saturation(){}
	JPSDR_Saturation(const JPSDR_Saturation& a)
	{
		AVX_Enable = a.AVX_Enable;
		SSE2_Enable = a.SSE2_Enable;
		mData=a.mData;
		InternalInit();
	}
	virtual ~JPSDR_Saturation();

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
	int16_t lookup[2304],offset_R,offset_G,offset_B,offset_Y,offset_U,offset_V;
	bool SSE2_Enable,AVX_Enable,VDub2_Enable;
	uint16_t lookup_hue_CbCr[65536];

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

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
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Saturation,ScriptConfig,"iiiiiiiiii")
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


void JPSDR_SaturationDialog::SetDisplay(void)
{
	switch (mData.color_space)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_BT709,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_BT601,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_SMPTE_240M,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_FCC,1); break;
		case 4 : CheckDlgButton(mhdlg,IDC_BT2020,1); break;
	}

	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_BW,mData.BW_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_FULL_RANGE,mData.full_range?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_AUTO,mData.automatic?BST_CHECKED:BST_UNCHECKED);
	SetDlgItemInt(mhdlg,IDC_SATURATION,mData.saturation,TRUE);
	SetDlgItemInt(mhdlg,IDC_BRIGHTNESS,mData.brightness,TRUE);
	SetDlgItemInt(mhdlg,IDC_CONTRAST,mData.contrast,TRUE);
	SetDlgItemInt(mhdlg,IDC_HUE,mData.hue,TRUE);

	switch (mData.full_mode)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_FULL_IN,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_FULL_OUT,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_FULL_IN_OUT,1); break;
	}
}


void JPSDR_SaturationDialog::RefreshDisplay(void)
{
	bool ena_full;

	EnableWindow(GetDlgItem(mhdlg,IDC_JPSDR_SAT_EXT),!mData.BW_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_SATURATION),!mData.BW_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_JPSDR_HUE_EXT),!mData.BW_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_HUE),!mData.BW_mode);
	ena_full=mData.full_range && !mData.automatic;

	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_MODE),ena_full);
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_IN),ena_full);
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_OUT),ena_full);
	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_IN_OUT),ena_full);

	EnableWindow(GetDlgItem(mhdlg,IDC_FULL_RANGE),!mData.automatic);
}

bool JPSDR_SaturationDialog::OnInit()
{
	moldData=mData;

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	SetDisplay();
	RefreshDisplay();

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
	mData.full_range=!!IsDlgButtonChecked(mhdlg,IDC_FULL_RANGE);
	mData.automatic=!!IsDlgButtonChecked(mhdlg,IDC_AUTO);
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
	switch(cmd)
	{
		case IDC_BW :
			mData.BW_mode=!!IsDlgButtonChecked(mhdlg,IDC_BW);
			if (mifp)
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			RefreshDisplay();
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
		case IDC_BT2020 :
		case IDC_FCC :
		case IDC_SMPTE_240M :
			if (mifp && SaveToData())
			{
				filter->Compute_Lookup();
				mifp->RedoFrame();
			}
			return true;
		case IDC_FULL_RANGE :
		case IDC_AUTO :
			SaveToData();
			RefreshDisplay();
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
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	InternalInit();

	return(true);
}


void JPSDR_Saturation::InternalInit(void)
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
			threadpoolAllocated=poolInterface->AllocateThreads(total_cpu,0,0,true,false,true,HighestThreadLevel,-1);
		else threadpoolAllocated=false;
		if (threadpoolAllocated) poolInterface->GetUserId(UserId);
	}
	else
	{
		total_cpu=0;
		threadpoolAllocated=false;
	}
}


JPSDR_Saturation::~JPSDR_Saturation()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
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
	uint8_t i,max=1;

	dh_Y=(size_y+(int32_t)max_threads-1)/(int32_t)max_threads;
	if (dh_Y<16) dh_Y=16;
	if ((dh_Y & 3)!=0) dh_Y=((dh_Y+3) >> 2) << 2;

	h_y=dh_Y;
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

		memset(dst,offset,(size_t)h*(size_t)w);
	}
	else
	{
		for(int32_t i=0; i<h; i++)
		{
			memset(dst,offset,w);
			dst+=dst_pitch;
		}
	}
}


uint32 JPSDR_Saturation::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;
	
	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;

	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));

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
/*
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
/*
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
	}

	image_data.src_full_mode=CheckFullRangeMode(&fa->src);
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	if ((!mData.automatic) && mData.full_range && (mData.full_mode!=0))
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

	if ((!mData.automatic) && mData.full_range && (mData.full_mode==0))
	{
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV411_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV410_Planar;
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_Y8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420ib_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420it_Planar;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV444_Planar_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV411_Planar_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :				
				pxdst.format=nsVDXPixmap::kPixFormat_YUV410_Planar_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420ib_Planar_709;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
				pxdst.format=nsVDXPixmap::kPixFormat_YUV420it_Planar_709;
				break;
		}		
	}

	fa->dst.offset = fa->src.offset;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
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

	if (AVX_Enable)
		JPSDR_Saturation_AVX_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_R,offset_G,offset_B,lookup,
			mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
	else
	{
		if (SSE2_Enable)
			JPSDR_Saturation_SSE2_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_R,offset_G,offset_B,lookup,
				mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
		else
			JPSDR_Saturation_Non_SSE_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_R,offset_G,offset_B,lookup,
				mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
	}
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

	if (AVX_Enable)
		JPSDR_Saturation_Y_AVX_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_Y,lookup,mt_data_inf.src_modulo1,
			mt_data_inf.dst_modulo1);
	else
	{
		if (SSE2_Enable)
			JPSDR_Saturation_Y_SSE2_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_Y,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.dst_modulo1);
		else
			JPSDR_Saturation_Y_Non_SSE_RGB24(mt_data_inf.src1,mt_data_inf.dst1,mt_data_inf.src_Y_w,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,offset_Y,lookup,mt_data_inf.src_modulo1,
				mt_data_inf.dst_modulo1);
	}
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
	bool chg_scale;

	if ((image_data.src_w0==0) || (image_data.src_h0==0)) return;
	if (mData.full_range && (mData.full_mode!=2)) chg_scale=true;
	else chg_scale=false;
	if ((mData.brightness==0) && (mData.contrast==0) && (mData.hue==0) && (mData.saturation==0) && (!mData.BW_mode) 
		&& (!chg_scale)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;


	for (uint8_t i=0; i<threads_number; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)image_data.src_plane0+(image_data.src_pitch0*MT_Data[i].src_Y_h_min));
		MT_Data[i].src2=(void *)((uint8_t *)image_data.src_plane1+(image_data.src_pitch1*MT_Data[i].src_UV_h_min));
		MT_Data[i].src3=(void *)((uint8_t *)image_data.src_plane2+(image_data.src_pitch2*MT_Data[i].src_UV_h_min));
		MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(image_data.dst_pitch0*MT_Data[i].dst_Y_h_min));
		MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1+(image_data.dst_pitch1*MT_Data[i].dst_UV_h_min));
		MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2+(image_data.dst_pitch2*MT_Data[i].dst_UV_h_min));
		MT_Data[i].src_pitch1=image_data.src_pitch0;
		MT_Data[i].src_pitch2=image_data.src_pitch1;
		MT_Data[i].src_pitch3=image_data.src_pitch2;
		MT_Data[i].dst_pitch1=image_data.dst_pitch0;
		MT_Data[i].dst_pitch2=image_data.dst_pitch1;
		MT_Data[i].dst_pitch3=image_data.dst_pitch2;
		MT_Data[i].src_modulo1=image_data.src_modulo0;
		MT_Data[i].src_modulo2=image_data.src_modulo1;
		MT_Data[i].src_modulo3=image_data.src_modulo2;
		MT_Data[i].dst_modulo1=image_data.dst_modulo0;
		MT_Data[i].dst_modulo2=image_data.dst_modulo1;
		MT_Data[i].dst_modulo3=image_data.dst_modulo2;
	}

	if (threads_number>1)
	{
		uint8_t f_proc=0,f_proc2=0;

		if (mData.BW_mode)
		{
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA : f_proc=6; break;
				case VMODE_YUYV : f_proc=4; break;
				case VMODE_UYVY : f_proc=5; break;
				case VMODE_PLANAR_YUV444 :
				case VMODE_PLANAR_YUV422 :
				case VMODE_PLANAR_YUV420 :
				case VMODE_PLANAR_YUV411 :
				case VMODE_PLANAR_YUV410 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) f_proc=7;
					Planar_Fill(image_data.dst_plane1,image_data.src_w1,image_data.src_h1,(uint8_t)(offset_U>>4),
						image_data.dst_pitch1);
					Planar_Fill(image_data.dst_plane2,image_data.src_w2,image_data.src_h2,(uint8_t)(offset_V>>4),
						image_data.dst_pitch2);
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) f_proc=7;
					break;
			}
		}
		else
		{
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA : f_proc=3; break;
				case VMODE_YUYV : f_proc=1; break;
				case VMODE_UYVY : f_proc=2; break;
				case VMODE_PLANAR_YUV444 :
				case VMODE_PLANAR_YUV422 :
				case VMODE_PLANAR_YUV420 :
				case VMODE_PLANAR_YUV411 :
				case VMODE_PLANAR_YUV410 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) f_proc=7;
					if (mData.hue==0)
					{
						if ((mData.saturation!=0) || chg_scale) f_proc2=8;
					}
					else f_proc2=10;
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) f_proc=7;
					break;
			}
		}

		if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
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

			poolInterface->ReleaseThreadPool(UserId,false);
		}
	}
	else
	{
		if (mData.BW_mode)
		{
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA : Saturation_Y_RGB24(0); break;
				case VMODE_YUYV : Saturation_Y_YUYV(0); break;
				case VMODE_UYVY : Saturation_Y_UYVY(0); break;
				case VMODE_PLANAR_YUV444 :
				case VMODE_PLANAR_YUV422 :
				case VMODE_PLANAR_YUV420 :
				case VMODE_PLANAR_YUV411 :
				case VMODE_PLANAR_YUV410 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) Saturation_Planar_Y(0);
					Planar_Fill(image_data.dst_plane1,image_data.src_w1,image_data.src_h1,(uint8_t)(offset_U>>4),
						image_data.dst_pitch1);
					Planar_Fill(image_data.dst_plane2,image_data.src_w2,image_data.src_h2,(uint8_t)(offset_V>>4),
						image_data.dst_pitch2);
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) Saturation_Planar_Y(0);
					break;
			}
		}
		else
		{
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA : Saturation_RGB24(0); break;
				case VMODE_YUYV : Saturation_YUYV(0); break;
				case VMODE_UYVY : Saturation_UYVY(0); break;
				case VMODE_PLANAR_YUV444 :
				case VMODE_PLANAR_YUV422 :
				case VMODE_PLANAR_YUV420 :
				case VMODE_PLANAR_YUV411 :
				case VMODE_PLANAR_YUV410 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) Saturation_Planar_Y(0);
					if (mData.hue==0)
					{
						if ((mData.saturation!=0) || chg_scale)
						{
							Saturation_Planar_U(0);
							Saturation_Planar_V(0);
						}
					}
					else Saturation_Hue_Planar_YUV(0);
					break;
				case 9 :
					if ((mData.brightness!=0) || (mData.contrast!=0) || chg_scale) Saturation_Planar_Y(0);
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
	bool full_range;
	uint8_t full_mode,color_space;

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

	if (mData.automatic)
	{
		full_range=idata.src_full_mode;
		full_mode=full_range?2:0;
		switch(idata.src_color_matrix)
		{
			case MATRIX_BT601 : color_space=1; break;
			case MATRIX_BT709 : color_space=0; break;
			case MATRIX_BT2020 : color_space=4; break;
			default : color_space=1; break;
		}
	}
	else
	{
		full_range=mData.full_range;
		full_mode=mData.full_mode;
		color_space=mData.color_space;
	}

	if (full_range)
	{
		switch(full_mode)
		{
			case 0 :
				offs_Y_In=0; offs_Y_Out=Min_Y; miny=Min_Y; maxy=Max_Y; scale_Y=(Max_Y-Min_Y)/255.0;
				coeff_U_In=255.0; coeff_U_Out=Max_U-Min_U; minu=Min_U; maxu=Max_U; scale_U=(Max_U-Min_U)/255.0;
				coeff_V_In=255.0; coeff_V_Out=Max_V-Min_V; minv=Min_V; maxv=Max_V; scale_V=(Max_V-Min_V)/255.0;
				break;
			case 1 :
				offs_Y_In=Min_Y; offs_Y_Out=0; miny=0; maxy=255; scale_Y=255.0/(Max_Y-Min_Y);
				coeff_U_In=Max_U-Min_U; coeff_U_Out=255.0; minu=0; maxu=255; scale_U=255.0/(Max_U-Min_U);
				coeff_V_In=Max_V-Min_V; coeff_V_Out=255.0; minv=0; maxv=255; scale_V=255.0/(Max_V-Min_V);
				break;
			case 2 :
				offs_Y_In=0; offs_Y_Out=0; miny=0; maxy=255; scale_Y=1.0;
				coeff_U_In=Max_U-Min_U; coeff_U_Out=255.0; minu=0; maxu=255; scale_U=255.0/(Max_U-Min_U);
				coeff_V_In=255.0; coeff_V_Out=255.0; minv=0; maxv=255; scale_V=1.0;
				break;
		}
	}
	else
	{
		miny=Min_Y;
		maxy=Max_Y;
		offs_Y_In=Min_Y;
		offs_Y_Out=Min_Y;
		scale_Y=1.0;
		minu=Min_U;
		maxu=Max_U;
		coeff_U_In=Max_U-Min_U;
		coeff_U_Out=Max_U-Min_U;
		scale_U=1.0;
		minv=Min_V;
		maxv=Max_V;
		coeff_V_In=Max_V-Min_V;
		coeff_V_Out=Max_V-Min_V;
		scale_V=1.0;
	}

	switch(color_space)
	{
		case 0 : kr=0.2126; kb=0.0722; break;
		case 1 : kr=0.299; kb=0.114; break;
		case 2 : kr=0.212; kb=0.087; break;
		case 3 : kr=0.11; kb=0.3; break;
		case 4 : kr=0.2627; kb=0.0593; break;
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

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if  (mData.mt_mode && (image_data.src_h0>=32) && (image_data.dst_h0>=32)) threads_number=total_cpu;
	else threads_number=1;

	switch (image_data.src_video_mode)
	{
		case VMODE_BMP_RGBA :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,0,0,0);
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_Y :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,0,0,2);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,1,0,1);
			break;
		case VMODE_PLANAR_YUV422 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,1,0,2);
			break;
		case VMODE_PLANAR_YUV420 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,1,1,2);
			break;
		case VMODE_PLANAR_YUV411 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,2,0,2);
			break;
		case VMODE_PLANAR_YUV410 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,2,2,2);
			break;
		default :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,0,0,0);
			break;
	}

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


void JPSDR_Saturation::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf,maxlen, "Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",mData.saturation,mData.hue,mData.brightness,
		mData.contrast,mData.BW_mode,mData.full_range,mData.full_mode,mData.color_space,mData.mt_mode,mData.automatic);
}



bool JPSDR_Saturation::Configure(VDXHWND hwnd)
{
	JPSDR_SaturationDialog dlg(mData, fa->ifp2, this);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_Saturation::GetSettingString(char *buf, int maxlen)
{

	bool full_range;
	uint8_t full_mode,color_space;
	char output_string[255];

	if (mData.automatic)
	{
		full_range=image_data.src_full_mode;
		full_mode=full_range?2:0;
		switch(image_data.src_color_matrix)
		{
			case MATRIX_BT601 : color_space=1; break;
			case MATRIX_BT709 : color_space=0; break;
			case MATRIX_BT2020 : color_space=4; break;
			default : color_space=1; break;
		}
	}
	else
	{
		full_range=mData.full_range;
		full_mode=mData.full_mode;
		color_space=mData.color_space;
	}

	if (full_range) strcpy_s(output_string,255," Full range");
	else strcpy_s(output_string,255,"");
	if (full_range)
	{
		switch(full_mode)
		{
			case 0 : strcat_s(output_string,255," Input"); break;
			case 1 : strcat_s(output_string,255," Output"); break;
			case 2 : strcat_s(output_string,255," Input & Output"); break;
		}
	}
	switch(color_space)
	{
		case 0 : strcat_s(output_string,255," BT.709"); break;
		case 1 : strcat_s(output_string,255," BT.601"); break;
		case 2 : strcat_s(output_string,255," SMPTE 240M"); break;
		case 3 : strcat_s(output_string,255," FCC"); break;
		case 4 : strcat_s(output_string,255," BT.2020"); break;
	}
	if (!mData.BW_mode)
	{
		if (mData.saturation<0) strcat_s(output_string,255," Sat : %d%%");
		else strcat_s(output_string,255," Sat : +%d%%");
		if (mData.hue<0) strcat_s(output_string,255," Hue : %d�");
		else strcat_s(output_string,255," Hue : +%d�");
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
	mData.full_range=!!argv[5].asInt();
	mData.full_mode=argv[6].asInt();
	mData.color_space=argv[7].asInt();
	mData.mt_mode=!!argv[8].asInt();
	mData.automatic=!!argv[9].asInt();
}


		
extern VDXFilterDefinition2 filterDef_JPSDR_Saturation=
VDXVideoFilterDefinition<JPSDR_Saturation>("JPSDR","Sat/Hue/Bright/Contr v4.5.3","[SSE2][AVX] Optimised.");