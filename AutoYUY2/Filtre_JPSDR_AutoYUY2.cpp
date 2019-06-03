/*
 *  AutoYUY2()
 *
 *  Adaptive YV12 upsampling. Progressive picture areas are upsampled
 *  progressively and interlaced areas are upsampled interlaced.
 *  Copyright (C) 2005 Donald A. Graft
 *  Modified by JPSDR
 *	
 *  AutoYUY2 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  AutoYUY2 is distributed in the hope that it will be useful,
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

#include "AUTOYUY2_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

#if _MSC_VER >= 1900
#define AVX2_BUILD_POSSIBLE
#endif


extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_1(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);

extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);

extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_1(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);

extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);


extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(const void *scr_1,const void *src_2,void *dst,int w16);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(const void *scr_1,const void *src_2,void *dst,int w16);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(const void *scr_1,const void *src_2,void *dst,int w8);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(const void *scr_1,const void *src_2,void *dst,int w8);

extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(const void *scr_1,const void *src_2,void *dst,int w16);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(const void *scr_1,const void *src_2,void *dst,int w16);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(const void *scr_1,const void *src_2,void *dst,int w8);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(const void *scr_1,const void *src_2,void *dst,int w8);

extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(const void *scr_1,const void *src_2,void *dst,int w16);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(const void *scr_1,const void *src_2,void *dst,int w16);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(const void *scr_1,const void *src_2,void *dst,int w8);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(const void *scr_1,const void *src_2,void *dst,int w8);

#ifdef AVX2_BUILD_POSSIBLE
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(const void *scr_1,const void *src_2,void *dst,int w32);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(const void *scr_1,const void *src_2,void *dst,int w16);

extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(const void *scr_1,const void *src_2,void *dst,int w32);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(const void *scr_1,const void *src_2,void *dst,int w16);

extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(const void *scr_1,const void *src_2,void *dst,int w32);
extern "C" void JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(const void *scr_1,const void *src_2,void *dst,int w16);
#endif


#define Interlaced_Tab_Size 3


class JPSDR_AutoYUY2Data
{
public:
	JPSDR_AutoYUY2Data();

	uint8_t output_format;
	uint8_t convert_mode;
	uint8_t threshold;
	bool mt_mode;
};


JPSDR_AutoYUY2Data::JPSDR_AutoYUY2Data()
{
	output_format=2;
	convert_mode=0;
	threshold=4;
	mt_mode=true;
}


typedef struct _MT_Data_Info
{
	void *src1,*src2,*src3;
	void *dst1,*dst2,*dst3;
	ptrdiff_t src_pitch1,src_pitch2,src_pitch3;
	ptrdiff_t dst_pitch1,dst_pitch2,dst_pitch3;
	int32_t src_Y_h_min,src_Y_h_max,src_Y_w;
	int32_t src_UV_h_min,src_UV_h_max,src_UV_w;
	int32_t dst_Y_h_min,dst_Y_h_max,dst_Y_w;
	int32_t dst_UV_h_min,dst_UV_h_max,dst_UV_w;
	bool top,bottom;
} MT_Data_Info;


class JPSDR_AutoYUY2Dialog : public VDXVideoFilterDialog
{
public:
	JPSDR_AutoYUY2Dialog(JPSDR_AutoYUY2Data& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_AUTO_YUY2),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_AutoYUY2Data& mData;
	JPSDR_AutoYUY2Data moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_AutoYUY2Dialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_AutoYUY2Dialog::OnInit()
{
	bool enable_dlg;

	moldData=mData;

	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
	switch (mData.output_format)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_YUY2,1); break;
		case 1 : CheckDlgButton(mhdlg,IDC_UYVY,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_PLANAR_422,1); break;
	}
	switch (mData.convert_mode)
	{
		case 0 : CheckDlgButton(mhdlg,IDC_PROGRESSIVE,1); enable_dlg=false; break;
		case 1 : CheckDlgButton(mhdlg,IDC_INTERLACED,1); enable_dlg=false; break;
		case 2 : CheckDlgButton(mhdlg,IDC_AUTOMATIC,1); enable_dlg=true; break;
		case 3 : CheckDlgButton(mhdlg,IDC_TEST,1); enable_dlg=true; break;
	}

	SetDlgItemInt(mhdlg,IDC_THRESHOLD,mData.threshold,FALSE);

	EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD),enable_dlg);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD_TEXT),enable_dlg);

	HWND hwndPreview = GetDlgItem(mhdlg,IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}


void JPSDR_AutoYUY2Dialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_AutoYUY2Dialog::SaveToData()
{
	uint8_t thrs;
	BOOL success;

	if (!!IsDlgButtonChecked(mhdlg,IDC_YUY2)) mData.output_format=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_UYVY)) mData.output_format=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_PLANAR_422)) mData.output_format=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_PROGRESSIVE)) mData.convert_mode=0;
	if (!!IsDlgButtonChecked(mhdlg,IDC_INTERLACED)) mData.convert_mode=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_AUTOMATIC)) mData.convert_mode=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_TEST)) mData.convert_mode=3;

	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);

	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD,&success,FALSE);
	if (success) mData.threshold=thrs;

	return true;
}


bool JPSDR_AutoYUY2Dialog::OnCommand(int cmd)
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
		case IDC_PROGRESSIVE :
		case IDC_INTERLACED :
			EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD),false);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD_TEXT),false);
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_AUTOMATIC :
		case IDC_TEST :
			EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD),true);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD_TEXT),true);
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_THRESHOLD :
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_YUY2 :
		case IDC_UYVY :
		case IDC_PLANAR_422 :
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}


class JPSDR_AutoYUY2 : public JPSDRVDXVideoFilter
{
public:
	virtual ~JPSDR_AutoYUY2();
	JPSDR_AutoYUY2(){}
	JPSDR_AutoYUY2(const JPSDR_AutoYUY2& a)
	{
		AVX2_Enable=a.AVX2_Enable;
		AVX_Enable=a.AVX_Enable;
		SSE2_Enable = a.SSE2_Enable;
		mData=a.mData;
		InternalInit();
	}

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
	bool *interlaced_tab_U[MAX_MT_THREADS][Interlaced_Tab_Size],*interlaced_tab_V[MAX_MT_THREADS][Interlaced_Tab_Size];
	Image_Data image_data;
	uint16_t *lookup_Upscale8;
	uint32_t *lookup_Upscale16;
	bool SSE2_Enable,AVX_Enable,AVX2_Enable,VDub2_Enable;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y);

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_AutoYUY2Data mData;
};


VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_AutoYUY2)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_AutoYUY2,ScriptConfig,"iiii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_AutoYUY2::Init()
{
	AVX2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX2)!=0);
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	InternalInit();

	return(true);
}


void JPSDR_AutoYUY2::InternalInit()
{
	int16_t i,j;

	for (j=0; j<MAX_MT_THREADS; j++)
	{
		for (i=0; i<Interlaced_Tab_Size; i++)
		{
			interlaced_tab_U[j][i]=NULL;
			interlaced_tab_V[j][i]=NULL;
		}
	}

	lookup_Upscale16=NULL;
	lookup_Upscale8=NULL;

	StaticThreadpoolF=StaticThreadpool;

	for (i=0; i<MAX_MT_THREADS; i++)
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


JPSDR_AutoYUY2::~JPSDR_AutoYUY2()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
}


uint32 JPSDR_AutoYUY2::GetParams()
{
	if (g_VFVAPIVersion<12) return(FILTERPARAM_NOT_SUPPORTED);

	bool full_mode,bt709;
	uint8_t bit_pixel;

	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));

	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;

	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :*/
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
/*		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :*/				
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
/*		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
/*		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :*/
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

	full_mode=CheckFullRangeMode(&fa->src);
	bit_pixel=GetBitDepth(&fa->src);
	if (GetColorMode(&fa->src)==MATRIX_BT709) bt709=true;
	else bt709=false;

	image_data.src_full_mode=full_mode;
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset=fa->src.offset;

/*	switch(mData.output_format)
	{
		case 0 : pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV; break;
		case 1 : pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY; break;
		case 2 : pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar; break;
	}*/
	if (bit_pixel==8)
	{
		switch(mData.output_format)
		{
			case 0 : 	if (full_mode)
						{
							if (bt709) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR;
							else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_FR;
						}
						else
						{
							if (bt709) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV_709;
							else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_YUYV;
						}
						break;
			case 1 : 	if (full_mode)
						{
							if (bt709) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR;
							else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_FR;
						}
						else
						{
							if (bt709) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY_709;
							else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_UYVY;
						}						
						break;
			case 2 : 	if (full_mode)
						{
							if (bt709) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR;
							else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_FR;
						}
						else
						{
							if (bt709) pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar_709;
							else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar;
						}						
						break;
		}

	}
	else pxdst.format=nsVDXPixmap::kPixFormat_YUV422_Planar16;

	if (VDub2_Enable)
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo((fa->dst).mpPixmap);

		if (full_mode) info->colorRangeMode=nsVDXPixmap::kColorRangeMode_Full;
		else info->colorRangeMode=nsVDXPixmap::kColorRangeMode_Limited;

		if (bt709) info->colorSpaceMode=nsVDXPixmap::kColorSpaceMode_709;
		else info->colorSpaceMode=nsVDXPixmap::kColorSpaceMode_601;
	}

	pxdst.pitch=0;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	if (VDub2_Enable)
		return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES_32);
	else
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
	}
}


uint8_t JPSDR_AutoYUY2::CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y)
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
		MT_Data[0].src_UV_h_max=size_y >> 1;
		MT_Data[0].dst_UV_h_max=size_y;
		MT_Data[0].src_Y_w=size_x;
		MT_Data[0].dst_Y_w=size_x;
		MT_Data[0].src_UV_w=size_x >> 1;
		MT_Data[0].dst_UV_w=size_x >> 1;
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
		MT_Data[0].src_UV_h_max=size_y >> 1;
		MT_Data[0].dst_UV_h_max=size_y;
		MT_Data[0].src_Y_w=size_x;
		MT_Data[0].dst_Y_w=size_x;
		MT_Data[0].src_UV_w=size_x >> 1;
		MT_Data[0].dst_UV_w=size_x >> 1;
		return(1);
	}

	dh_UV=dh_Y>>1; 

	MT_Data[0].top=true;
	MT_Data[0].bottom=false;
	MT_Data[0].src_Y_h_min=0;
	MT_Data[0].src_Y_h_max=dh_Y;
	MT_Data[0].dst_Y_h_min=0;
	MT_Data[0].dst_Y_h_max=dh_Y;
	MT_Data[0].src_UV_h_min=0;
	MT_Data[0].src_UV_h_max=dh_UV;
	MT_Data[0].dst_UV_h_min=0;
	MT_Data[0].dst_UV_h_max=dh_Y;

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
		MT_Data[i].dst_UV_h_max=MT_Data[i].dst_UV_h_min+dh_Y;
		i++;
	}
	MT_Data[max-1].bottom=true;
	MT_Data[max-1].src_Y_h_max=size_y;
	MT_Data[max-1].dst_Y_h_max=size_y;
	MT_Data[max-1].src_UV_h_max=size_y >> 1;
	MT_Data[max-1].dst_UV_h_max=size_y;
	for (i=0; i<max; i++)
	{
		MT_Data[i].src_Y_w=size_x;
		MT_Data[i].dst_Y_w=size_x;
		MT_Data[i].src_UV_w=size_x >> 1;
		MT_Data[i].dst_UV_w=size_x >> 1;
	}
	return(max);
}


void JPSDR_AutoYUY2::Start()
{
	int16_t i,j;
	bool ok;

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

	if ((image_data.src_h0 & 3)!=0)
	{
		ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
		return;
	}

	if ((image_data.src_h0<8) || (image_data.src_w0<4))
	{
		ff->Except("Size must be at least 4x8!");
		return;
	}

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if (mData.mt_mode && ((image_data.src_h0>=32) && (image_data.dst_h0>=32))) threads_number=total_cpu;
	else threads_number=1;

	threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0);

	const uint32_t vmax=1 << image_data.src_bits_pixel;

	lookup_Upscale8=(uint16_t *)malloc(3*256*sizeof(uint16_t));
	lookup_Upscale16=(uint32_t *)malloc(3*vmax*sizeof(uint32_t));

	if ((lookup_Upscale8==NULL) || (lookup_Upscale16==NULL))
	{
		ff->ExceptOutOfMemory();
		return;
	}

	if (mData.convert_mode>1)
	{
		for (j=0; j<threads_number; j++)
		{
			for (i=0; i<Interlaced_Tab_Size; i++)
			{
				interlaced_tab_U[j][i]=(bool*)malloc(image_data.src_w1*sizeof(bool));
				interlaced_tab_V[j][i]=(bool*)malloc(image_data.src_w2*sizeof(bool));
			}
		}

		ok=true;
		for (j=0; j<threads_number; j++)
		{
			for (i=0; i<Interlaced_Tab_Size; i++)
			{
				ok=ok && (interlaced_tab_U[j][i]!=NULL);
				ok=ok && (interlaced_tab_V[j][i]!=NULL);
			}
		}

		if (!ok)
		{
			ff->ExceptOutOfMemory();
			return;
		}
	}

	for (uint16_t k=0; k<256; k++)
	{
		lookup_Upscale8[k]=3*k;
		lookup_Upscale8[k+256]=5*k;
		lookup_Upscale8[k+512]=7*k;
	}
	for (uint32_t k=0; k<vmax; k++)
	{
		lookup_Upscale16[k]=3*k;
		lookup_Upscale16[k+vmax]=5*k;
		lookup_Upscale16[k+2*vmax]=7*k;
	}

	if (threads_number>1)
	{
		if (!threadpoolAllocated)
		{
			ff->Except("Error with the TheadPool while allocating threadpool!");
			return;
		}
	}
}


void JPSDR_AutoYUY2::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];

	switch(mData.output_format)
	{
		case 0 : strcpy_s(output_string,255," Output : YUY2"); break;
		case 1 : strcpy_s(output_string,255," Output : UYVY"); break;
		case 2 : strcpy_s(output_string,255," Output : Planar 4:2:2"); break;
	}
	switch(mData.convert_mode)
	{
		case 0 : strcat_s(output_string,255," Mode : Progressive"); break;
		case 1 : strcat_s(output_string,255," Mode : Interlaced"); break;
		case 2 : strcat_s(output_string,255," Mode : Automatic Threshold : %d"); break;
		case 3 : strcat_s(output_string,255," Mode : Test Threshold : %d"); break;
	}

	switch(mData.convert_mode)
	{
		case 0 :
		case 1 :
			SafePrintf(buf,maxlen,output_string);
			break;
		case 2 :
		case 3 :
			SafePrintf(buf,maxlen,output_string,mData.threshold);
			break;
	}

}


static inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src=(uint8_t *)src_;
	uint8_t *dst=(uint8_t *)dst_;

	if ((src_pitch==dst_pitch) && (abs(src_pitch)==w))
	{
		if (src_pitch<0)
		{
			src+=(h-1)*src_pitch;
			dst+=(h-1)*dst_pitch;
		}
		memcpy(dst,src,(size_t)h*(size_t)w);
	}
	else
	{
		for(int32_t i=0; i<h; i++)
		{
			memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
}


static void Convert_Interlaced_8_YV16_SSE2(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *dst_U=mt_data_inf.dst2;
	void *dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_U=mt_data_inf.src_UV_w;
	const int32_t w_V=mt_data_inf.src_UV_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t w_U4=(w_U+15)>>4,w_V4=(w_V+15)>>4;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			memcpy(dst_U,src_Un,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Unn,src_U,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_Un,src_Unnn,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_U,src_Upp,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Up,src_Un,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Unn,src_U,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_Un,src_Unnn,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_U,src_Upp,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Up,src_Un,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			memcpy(dst_U,src_Un,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);
			
			memcpy(dst_V,src_Vn,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Vnn,src_V,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_Vn,src_Vnnn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_V,src_Vpp,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Vp,src_Vn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Vnn,src_V,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_Vn,src_Vnnn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2(src_V,src_Vpp,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2(src_Vp,src_Vn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			memcpy(dst_V,src_Vn,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Interlaced_8_YV16_AVX(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *dst_U=mt_data_inf.dst2;
	void *dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_U=mt_data_inf.src_UV_w;
	const int32_t w_V=mt_data_inf.src_UV_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t w_U4=(w_U+15)>>4,w_V4=(w_V+15)>>4;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			memcpy(dst_U,src_Un,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Unn,src_U,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_Un,src_Unnn,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_U,src_Upp,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Up,src_Un,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Unn,src_U,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_Un,src_Unnn,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_U,src_Upp,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Up,src_Un,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			memcpy(dst_U,src_Un,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);
			
			memcpy(dst_V,src_Vn,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Vnn,src_V,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_Vn,src_Vnnn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_V,src_Vpp,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Vp,src_Vn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Vnn,src_V,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_Vn,src_Vnnn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX(src_V,src_Vpp,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX(src_Vp,src_Vn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			memcpy(dst_V,src_Vn,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Interlaced_8_YV16(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *_dst_U=mt_data_inf.dst2;
	void *_dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_U=mt_data_inf.src_UV_w;
	const int32_t w_V=mt_data_inf.src_UV_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_V,*dst_U;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	dst_U=(uint8_t *)_dst_U;
	dst_V=(uint8_t *)_dst_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_U);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
		dst_U+=dst_pitch_U;

		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
		dst_U+=dst_pitch_U;

		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
		dst_U+=dst_pitch_U;

		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
		dst_U+=dst_pitch_U;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_U);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_U);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_V);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_V);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_V; j++)
				dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_V; j++)
				dst_V[j]=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
		dst_V+=dst_pitch_V;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			for(int32_t j=0; j<w_V; j++)
				dst_V[j]=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_V; j++)
				dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_V);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_V);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Progressive_8_YV16_SSE2(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *dst_U=mt_data_inf.dst2;
	void *dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_U=mt_data_inf.src_UV_w;
	const int32_t w_V=mt_data_inf.src_UV_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U4=(w_U+15)>>4,w_V4=(w_V+15)>>4;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_U,src_Un,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_U,src_Up,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_U,src_Un,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_U,src_Up,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);


			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

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
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_V,src_Vn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_V,src_Vp,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_V,src_Vn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2(src_V,src_Vp,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);


			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_8_YV16_AVX(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *dst_U=mt_data_inf.dst2;
	void *dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_U=mt_data_inf.src_UV_w;
	const int32_t w_V=mt_data_inf.src_UV_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U4=(w_U+15)>>4,w_V4=(w_V+15)>>4;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_U,src_Un,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_U,src_Up,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_U,src_Un,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_U,src_Up,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);


			memcpy(dst_U,src_U,w_U);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

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
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_V,src_Vn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_V,src_Vp,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_V,src_Vn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX(src_V,src_Vp,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);


			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_8_YV16(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *_dst_U=mt_data_inf.dst2;
	void *_dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_U=mt_data_inf.src_UV_w;
	const int32_t w_V=mt_data_inf.src_UV_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	uint8_t *dst_U,*dst_V;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	dst_U=(uint8_t *)_dst_U;
	dst_V=(uint8_t *)_dst_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_U,src_U,w_U);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_U; j++)
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
			dst_U+=dst_pitch_U;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
		dst_U+=dst_pitch_U;

		for(int32_t j=0; j<w_U; j++)
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
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
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
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
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
			dst_V+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_V; j++)
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
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
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_V);
			dst_V+=dst_pitch_V;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


void Convert_Interlaced_16_YV16_SSE2(const MT_Data_Info &mt_data_inf)
{

	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t w_U8=(w_U+7)>>3,w_V8=(w_V+7)>>3;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// U Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Unn,src_U,dstUp,w_U8);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_Un,src_Unnn,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_U,src_Upp,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Up,src_Un,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Unn,src_U,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_Un,src_Unnn,dstUp,w_U8);
		dstUp+=dst_pitch_U;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_U,src_Upp,dstUp,w_U8);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Up,src_Un,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// V Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Vnn,src_V,dstVp,w_V8);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_Vn,src_Vnnn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_V,src_Vpp,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Vp,src_Vn,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Vnn,src_V,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_Vn,src_Vnnn,dstVp,w_V8);
		dstVp+=dst_pitch_V;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2(src_V,src_Vpp,dstVp,w_V8);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2(src_Vp,src_Vn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


void Convert_Interlaced_16_YV16_AVX(const MT_Data_Info &mt_data_inf)
{

	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t w_U8=(w_U+7)>>3,w_V8=(w_V+7)>>3;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// U Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Unn,src_U,dstUp,w_U8);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_Un,src_Unnn,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_U,src_Upp,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Up,src_Un,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Unn,src_U,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_Un,src_Unnn,dstUp,w_U8);
		dstUp+=dst_pitch_U;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_U,src_Upp,dstUp,w_U8);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Up,src_Un,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// V Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Vnn,src_V,dstVp,w_V8);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_Vn,src_Vnnn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_V,src_Vpp,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Vp,src_Vn,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Vnn,src_V,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_Vn,src_Vnnn,dstVp,w_V8);
		dstVp+=dst_pitch_V;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX(src_V,src_Vpp,dstVp,w_V8);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX(src_Vp,src_Vn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Progressive_16_YV16_SSE2(const MT_Data_Info &mt_data_inf)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t w_U8=(w_U+7)>>3,w_V8=(w_V+7)>>3;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vn=src_V+src_pitchV;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_U,src_Un,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_U,src_Up,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_U,src_Un,dstUp,w_U8);
		dstUp+=dst_pitch_U;

		src_U+=src_pitchU;
		src_Up+=src_pitchU;
		src_Un+=src_pitchU;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_U,src_Up,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_V,src_Vn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_V,src_Vp,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_V,src_Vn,dstVp,w_V8);
		dstVp+=dst_pitch_V;

		src_V+=src_pitchV;
		src_Vp+=src_pitchV;
		src_Vn+=src_pitchV;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2(src_V,src_Vp,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}
}


static void Convert_Progressive_16_YV16_AVX(const MT_Data_Info &mt_data_inf)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t w_U8=(w_U+7)>>3,w_V8=(w_V+7)>>3;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vn=src_V+src_pitchV;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_U,src_Un,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_U,src_Up,dstUp,w_U8);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_U,src_Un,dstUp,w_U8);
		dstUp+=dst_pitch_U;

		src_U+=src_pitchU;
		src_Up+=src_pitchU;
		src_Un+=src_pitchU;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_U,src_Up,dstUp,w_U8);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_V,src_Vn,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_V,src_Vp,dstVp,w_V8);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_V,src_Vn,dstVp,w_V8);
		dstVp+=dst_pitch_V;

		src_V+=src_pitchV;
		src_Vp+=src_pitchV;
		src_Vn+=src_pitchV;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX(src_V,src_Vp,dstVp,w_V8);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}
}


static void Convert_Interlaced_16_YV16(const MT_Data_Info &mt_data_inf,const uint32_t *lookup,const uint8_t bits_per_pixel)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_V,*dst_U;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const uint32_t vmax=(uint32_t)1 << bits_per_pixel;
	const uint32_t vmax2= vmax << 1;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;
	dst_U=dstUp;
	dst_V=dstVp;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_U2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_U2);
			dst_U+=dst_pitch_U;

			const uint16_t *src1=(uint16_t *)src_Unn;
			const uint16_t *src2=(uint16_t *)src_U;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_U+=dst_pitch_U;

			src1=(uint16_t *)src_Un;
			src2=(uint16_t *)src_Unnn;
			dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		const uint16_t *src1=(uint16_t *)src_U;
		const uint16_t *src2=(uint16_t *)src_Upp;
		uint16_t *dst=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
		dst_U+=dst_pitch_U;

		src1=(uint16_t *)src_Up;
		src2=(uint16_t *)src_Un;
		dst=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
		dst_U+=dst_pitch_U;

		src1=(uint16_t *)src_Unn;
		src2=(uint16_t *)src_U;
		dst=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
		dst_U+=dst_pitch_U;

		src1=(uint16_t *)src_Un;
		src2=(uint16_t *)src_Unnn;
		dst=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
		dst_U+=dst_pitch_U;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Upp;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_U+=dst_pitch_U;

			src1=(uint16_t *)src_Up;
			src2=(uint16_t *)src_Un;
			dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_U2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_U2);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_V2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_V2);
			dst_V+=dst_pitch_V;

			const uint16_t *src1=(uint16_t *)src_Vnn;
			const uint16_t *src2=(uint16_t *)src_V;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_V+=dst_pitch_V;

			src1=(uint16_t *)src_Vn;
			src2=(uint16_t *)src_Vnnn;
			dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		const uint16_t *src1=(uint16_t *)src_V;
		const uint16_t *src2=(uint16_t *)src_Vpp;
		uint16_t *dst=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
		dst_V+=dst_pitch_V;

		src1=(uint16_t *)src_Vp;
		src2=(uint16_t *)src_Vn;
		dst=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
		dst_V+=dst_pitch_V;

		src1=(uint16_t *)src_Vnn;
		src2=(uint16_t *)src_V;
		dst=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
		dst_V+=dst_pitch_V;

		src1=(uint16_t *)src_Vn;
		src2=(uint16_t *)src_Vnnn;
		dst=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
		dst_V+=dst_pitch_V;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vpp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_V+=dst_pitch_V;

			src1=(uint16_t *)src_Vp;
			src2=(uint16_t *)src_Vn;
			dst=(uint16_t *)dst_V;
		
			for(int32_t j=0; j<w_V; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_V2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_V2);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Progressive_16_YV16(const MT_Data_Info &mt_data_inf,const uint32_t *lookup,const uint8_t bits_per_pixel)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	uint8_t *dst_U,*dst_V;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vn=src_V+src_pitchV;
	dst_U=dstUp;
	dst_V=dstVp;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_U,src_U,w_U2);
			dst_U+=dst_pitch_U;

			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Un;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
			dst_U+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		const uint16_t *src1=(uint16_t *)src_U;
		const uint16_t *src2=(uint16_t *)src_Up;
		uint16_t *dst=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
		dst_U+=dst_pitch_U;

		src2=(uint16_t *)src_Un;
		dst=(uint16_t *)dst_U;

		for(int32_t j=0; j<w_U; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
		dst_U+=dst_pitch_U;

		src_U+=src_pitchU;
		src_Up+=src_pitchU;
		src_Un+=src_pitchU;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Up;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_U; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_U2);
			dst_U+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dst_V,src_V,w_V2);
			dst_V+=dst_pitch_V;

			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vn;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
			dst_V+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		const uint16_t *src1=(uint16_t *)src_V;
		const uint16_t *src2=(uint16_t *)src_Vp;
		uint16_t *dst=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
		dst_V+=dst_pitch_V;

		src2=(uint16_t *)src_Vn;
		dst=(uint16_t *)dst_V;

		for(int32_t j=0; j<w_V; j++)
			dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
		dst_V+=dst_pitch_V;

		src_V+=src_pitchV;
		src_Vp+=src_pitchV;
		src_Vn+=src_pitchV;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_V; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_V2);
			dst_V+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}

}


#ifdef AVX2_BUILD_POSSIBLE
void Convert_Interlaced_8_YV16_AVX2(const MT_Data_Info &mt_data_inf)
{

	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U32=(w_U+31)>>5,w_V32=(w_V+31)>>5;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;

	Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// U Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Unn,src_U,dstUp,w_U32);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_Un,src_Unnn,dstUp,w_U32);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_U,src_Upp,dstUp,w_U32);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Up,src_Un,dstUp,w_U32);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Unn,src_U,dstUp,w_U32);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_Un,src_Unnn,dstUp,w_U32);
		dstUp+=dst_pitch_U;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_U,src_Upp,dstUp,w_U32);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Up,src_Un,dstUp,w_U32);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// V Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Vnn,src_V,dstVp,w_V32);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_Vn,src_Vnnn,dstVp,w_V32);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_V,src_Vpp,dstVp,w_V32);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Vp,src_Vn,dstVp,w_V32);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Vnn,src_V,dstVp,w_V32);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_Vn,src_Vnnn,dstVp,w_V32);
		dstVp+=dst_pitch_V;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX2(src_V,src_Vpp,dstVp,w_V32);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX2(src_Vp,src_Vn,dstVp,w_V32);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


void Convert_Interlaced_16_YV16_AVX2(const MT_Data_Info &mt_data_inf)
{

	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t w_U16=(w_U+15)>>4,w_V16=(w_V+15)>>4;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// U Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Unn,src_U,dstUp,w_U16);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_Un,src_Unnn,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_U,src_Upp,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Up,src_Un,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Unn,src_U,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_Un,src_Unnn,dstUp,w_U16);
		dstUp+=dst_pitch_U;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_U,src_Upp,dstUp,w_U16);
			dstUp+=dst_pitch_U;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Up,src_Un,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_Un,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// V Planar
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Vnn,src_V,dstVp,w_V16);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_Vn,src_Vnnn,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_V,src_Vpp,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Vp,src_Vn,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Vnn,src_V,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_Vn,src_Vnnn,dstVp,w_V16);
		dstVp+=dst_pitch_V;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX2(src_V,src_Vpp,dstVp,w_V16);
			dstVp+=dst_pitch_V;
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX2(src_Vp,src_Vn,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_Vn,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Progressive_8_YV16_AVX2(const MT_Data_Info &mt_data_inf)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U32=(w_U+31)>>5,w_V32=(w_V+31)>>5;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vn=src_V+src_pitchV;

	Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_U,src_Un,dstUp,w_U32);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_U,src_Up,dstUp,w_U32);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_U,src_Un,dstUp,w_U32);
		dstUp+=dst_pitch_U;

		src_U+=src_pitchU;
		src_Up+=src_pitchU;
		src_Un+=src_pitchU;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_U,src_Up,dstUp,w_U32);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_V,src_Vn,dstVp,w_V32);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_V,src_Vp,dstVp,w_V32);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_V,src_Vn,dstVp,w_V32);
		dstVp+=dst_pitch_V;

		src_V+=src_pitchV;
		src_Vp+=src_pitchV;
		src_Vn+=src_pitchV;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX2(src_V,src_Vp,dstVp,w_V32);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}
}


static void Convert_Progressive_16_YV16_AVX2(const MT_Data_Info &mt_data_inf)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_U=dst_w>>1,w_V=dst_w>>1;
	const int32_t w_U2=w_U<<1,w_V2=w_V<<1;
	const int32_t w_U16=(w_U+15)>>4,w_V16=(w_V+15)>>4;
	const int32_t h_2 = mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 2:h_Y_min;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vn=src_V+src_pitchV;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_U,src_Un,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_U,src_Up,dstUp,w_U16);
		dstUp+=dst_pitch_U;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_U,src_Un,dstUp,w_U16);
		dstUp+=dst_pitch_U;

		src_U+=src_pitchU;
		src_Up+=src_pitchU;
		src_Un+=src_pitchU;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_U,src_Up,dstUp,w_U16);
			dstUp+=dst_pitch_U;

			memcpy(dstUp,src_U,w_U2);
			dstUp+=dst_pitch_U;

			src_U+=src_pitchU;
			src_Up+=src_pitchU;
			src_Un+=src_pitchU;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_V,src_Vn,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_V,src_Vp,dstVp,w_V16);
		dstVp+=dst_pitch_V;
		JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_V,src_Vn,dstVp,w_V16);
		dstVp+=dst_pitch_V;

		src_V+=src_pitchV;
		src_Vp+=src_pitchV;
		src_Vn+=src_pitchV;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX2(src_V,src_Vp,dstVp,w_V16);
			dstVp+=dst_pitch_V;

			memcpy(dstVp,src_V,w_V2);
			dstVp+=dst_pitch_V;

			src_V+=src_pitchV;
			src_Vp+=src_pitchV;
			src_Vn+=src_pitchV;
		}
	}
}
#endif


static void Convert_Progressive_to_YUY2_SSE2(const MT_Data_Info &mt_data_inf)

{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_2=mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
			
			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_to_YUY2_AVX(const MT_Data_Info &mt_data_inf)

{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_2=mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
			
			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_to_YUY2(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_Y;
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	YUYV *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_2=mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	src_Y=(uint8_t *)_src_Y;
	dst=(YUYV *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<2; y+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t y=h_0; y<h_2; y+=2)
	{
		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
				dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_2; y<h_Y_max; y+=2)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Interlaced_to_YUY2_SSE2(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_4=mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Interlaced_to_YUY2_AVX(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_4=mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Interlaced_to_YUY2(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	YUYV *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4=mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	src_Y=(uint8_t *)_src_Y;
	dst=(YUYV *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;
				
				for (int32_t j=0; j<w_Y; j+=2)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t y=h_0; y<h_4; y+=4)
	{
		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
				dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
				dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
				dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_4; y<h_Y_max; y+=4)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Progressive_to_UYVY_SSE2(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_2=mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((char *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_to_UYVY_AVX(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_2=mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<2; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((char *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Progressive_to_UYVY(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_Y;
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	UYVY *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_2=mt_data_inf.bottom ? h_Y_max-2:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 2:h_Y_min;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	src_Y=(uint8_t *)_src_Y;
	dst=(UYVY *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<2; y+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t y=h_0; y<h_2; y+=2)
	{
		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
				dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_2; y<h_Y_max; y+=2)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}


static void Convert_Interlaced_to_UYVY_SSE2(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_4=mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Interlaced_to_UYVY_AVX(const MT_Data_Info &mt_data_inf)
{
	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t w_Y4=(w_Y+7)>>3;
	const int32_t h_4=mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;

	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(src_Y,src_U,src_V,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
			dst=(void *)((uint8_t *)dst+dst_pitch);
			src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Interlaced_to_UYVY(const MT_Data_Info &mt_data_inf,const uint16_t *lookup)
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	UYVY *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4=mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0=mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	src_Y=(uint8_t *)_src_Y;
	dst=(UYVY *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}

	for(int32_t y=h_0; y<h_4; y+=4)
	{
		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
				dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
				dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
				dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;
				dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_4; y<h_Y_max; y+=4)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Automatic_to_YUY2(const MT_Data_Info &mt_data_inf,const uint16_t *lookup,const int16_t threshold,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[])
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	YUYV *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	src_Y=(uint8_t *)_src_Y;
	dst=(YUYV *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;
			
			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;

					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
						(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
						((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
						(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
						((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;
					
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
					
					if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
						(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
						((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
						((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
				
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

		for (int32_t j=0; j<w_Y2; j++)
		{
			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold) &&
				(((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j])) ||
				((src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold) &&
				(((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j])) ||
				((src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;
			
			if (((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold) &&
				(((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j])) ||
				((src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold) &&
				(((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j])) ||
				((src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;
		}
	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t y=h_0; y<h_4; y+=4)
	{
		{
			int32_t i=0;
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4) >> 3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				}

				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4) >> 3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabv1=interlaced_tab_V[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
					(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
					((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;	

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				}

				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
					(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
					((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				}

				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			bool *itabu0=interlaced_tab_U[index_tab_0],*itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
					(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
					((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				}

				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
					((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_4; y<h_Y_max; y+=4)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Test_to_YUY2(const MT_Data_Info &mt_data_inf,const uint16_t *lookup,const int16_t threshold,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[])
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	YUYV *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	src_Y=(uint8_t *)_src_Y;
	dst=(YUYV *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;
			
			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;

					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
						(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
						((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
						(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
						((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;
					
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
					
					if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
						(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
						((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
						((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
				
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

		for (int32_t j=0; j<w_Y2; j++)
		{
			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold) &&
				(((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j])) ||
				((src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold) &&
				(((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j])) ||
				((src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;
			
			if (((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold) &&
				(((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j])) ||
				((src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold) &&
				(((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j])) ||
				((src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;
		}
	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t y=h_0; y<h_4; y+=4)
	{
		{
			int32_t i=0;
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				}

				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabv1=interlaced_tab_V[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
					(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
					((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;	

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				}

				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
					(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
					((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				}

				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			bool *itabu0=interlaced_tab_U[index_tab_0],*itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
					(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
					((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				}

				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
					((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
				}
			}
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_4; y<h_Y_max; y+=4)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
			}
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Automatic_to_UYVY(const MT_Data_Info &mt_data_inf,const uint16_t *lookup,const int16_t threshold,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[])
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	UYVY *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	src_Y=(uint8_t *)_src_Y;
	dst=(UYVY *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;

					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
						(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
						((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
						(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
						((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;

					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);

					if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
						(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
						((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
						((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
				
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

		for (int32_t j=0; j<w_Y2; j++)
		{

			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold) &&
				(((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j])) ||
				((src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold) &&
				(((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j])) ||
				((src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;

			if (((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold) &&
				(((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j])) ||
				((src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold) &&
				(((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j])) ||
				((src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;			
		}
	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t y=h_0; y<h_4; y+=4)
	{
		{
			int32_t i=0;
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4) >> 3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				}

				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4) >> 3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabv1=interlaced_tab_V[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
					(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
					((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;	

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				}

				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
					(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
					((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				}

				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			bool *itabu0=interlaced_tab_U[index_tab_0],*itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
					(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
					((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				}

				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
					((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_4; y<h_Y_max; y+=4)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Test_to_UYVY(const MT_Data_Info &mt_data_inf,const uint16_t *lookup,const int16_t threshold,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[])
{
	const void *_src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *_dst=mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	UYVY *dst;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Upp=src_U-2*src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Unn=src_U+2*src_pitch_U;
	src_Unnn=src_U+3*src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vpp=src_V-2*src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	src_Vnn=src_V+2*src_pitch_V;
	src_Vnnn=src_V+3*src_pitch_V;
	src_Y=(uint8_t *)_src_Y;
	dst=(UYVY *)_dst;

	if (mt_data_inf.top)
	{
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;

					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
						(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
						((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
						(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
						((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;

					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);

					if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
						(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
						((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
						((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
				
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

		for (int32_t j=0; j<w_Y2; j++)
		{

			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold) &&
				(((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j])) ||
				((src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold) &&
				(((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j])) ||
				((src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;

			if (((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold) &&
				(((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j])) ||
				((src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold) &&
				(((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j])) ||
				((src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;			
		}
	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t y=h_0; y<h_4; y+=4)
	{
		{
			int32_t i=0;
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				}

				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabv1=interlaced_tab_V[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
					(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
					((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;	

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				}

				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
					(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
					((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				}

				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		{
			int32_t i=0;
			bool *itabu0=interlaced_tab_U[index_tab_0],*itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2],*itabv2=interlaced_tab_V[index_tab_2];

			for (int32_t j=0; j<w_Y2; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
					(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
					((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst[j].u=239;
				}
				else
				{
					dst[j].u=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				}

				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
					((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst[j].v=239;
				}
				else
				{
					dst[j].v=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
				}
			}
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t y=h_4; y<h_Y_max; y+=4)
		{
			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
					dst[j].v=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_Y2; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;
					dst[j].u=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
					dst[j].v=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
			}
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(UYVY *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Automatic_8_YV16(const MT_Data_Info &mt_data_inf,const uint16_t *lookup,const int16_t threshold_,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[])
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_U,*dst_V;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int32_t w_UV=dst_w>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;
	dst_U=dstUp;
	dst_V=dstVp;

	Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U	
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_UV);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			{
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				
				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold_) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold_) &&
						(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
						((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold_) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold_) &&
						(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
						((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;
					
					dst_U[j]=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				}
			}
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}
	else
	{
		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				
		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold_) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold_) &&
				(((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j])) ||
				((src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold_) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold_) &&
				(((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j])) ||
				((src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;
		}

	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];

			// Upsample as needed.
			for(int32_t j=0; j<w_UV; j++)
			{
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst_U[j]=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold_) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold_) &&
					(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
					((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;			

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			bool *itabu0=interlaced_tab_U[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold_) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold_) &&
					(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
					((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst_U[j]=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_UV);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_UV);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_UV; j++)
				dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
			dst_V+=dst_pitch_V;

			{
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold_) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold_) &&
						(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
						((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
						(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
						((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
					
					dst_V[j]=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold_) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold_) &&
				(((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j])) ||
				((src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold_) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold_) &&
				(((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j])) ||
				((src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;
		}
	}
	
	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst_V[j]=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1];
			bool *itabv2=interlaced_tab_V[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold_) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold_) &&
					(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
					((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			bool *itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabv2=interlaced_tab_V[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
					(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
					((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst_V[j]=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			for(int32_t j=0; j<w_UV; j++)
				dst_V[j]=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_UV; j++)
				dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_UV);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Test_8_YV16(const MT_Data_Info &mt_data_inf,const uint16_t *lookup,const int16_t threshold_,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[])
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_U,*dst_V;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int32_t w_UV=dst_w>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;
	dst_U=dstUp;
	dst_V=dstVp;

	Move_Full(srcYp,dstYp,dst_w,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U	
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_UV);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			{
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				
				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold_) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold_) &&
						(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
						((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold_) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold_) &&
						(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
						((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;
					
					dst_U[j]=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				}
			}
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}
	else
	{
		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				
		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold_) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold_) &&
				(((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j])) ||
				((src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold_) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold_) &&
				(((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j])) ||
				((src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;
		}

	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];

			// Upsample as needed.
			for(int32_t j=0; j<w_UV; j++)
			{
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst_U[j]=239;
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold_) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold_) &&
					(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
					((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;			

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst_U[j]=239;
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst_U[j]=239;
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			bool *itabu0=interlaced_tab_U[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold_) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold_) &&
					(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
					((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst_U[j]=239;
				}
				else
				{
					dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[(uint16_t)src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[(uint16_t)src_Un[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_UV);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_UV);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_UV; j++)
				dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[(uint16_t)src_V[j]+256]+4)>>3);
			dst_V+=dst_pitch_V;

			{
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold_) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold_) &&
						(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
						((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
						(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
						((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
					
					dst_V[j]=(uint8_t)((lookup[(uint16_t)src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
				}
			}
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold_) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold_) &&
				(((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j])) ||
				((src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold_) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold_) &&
				(((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j])) ||
				((src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;
		}
	}
	
	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst_V[j]=239;
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1];
			bool *itabv2=interlaced_tab_V[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold_) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold_) &&
					(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
					((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst_V[j]=239;
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst_V[j]=239;
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			bool *itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabv2=interlaced_tab_V[index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold_) &&
					(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
					((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst_V[j]=239;
				}
				else
				{
					dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			for(int32_t j=0; j<w_UV; j++)
				dst_V[j]=(uint8_t)((lookup[(uint16_t)src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
			dst_V+=dst_pitch_V;

			for(int32_t j=0; j<w_UV; j++)
				dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[(uint16_t)src_Vn[j]+256]+4)>>3);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_UV);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Automatic_16_YV16(const MT_Data_Info &mt_data_inf,const uint32_t *lookup,const int16_t threshold,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[],uint8_t bits_per_pixel)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_U,*dst_V;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int32_t w_UV=dst_w >> 1;
	const int32_t w_UV2=w_UV << 1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;
	const int32_t threshold_=(int32_t)threshold << (bits_per_pixel-8);
	const uint32_t vmax=(uint32_t)1 << bits_per_pixel;
	const uint32_t vmax2=vmax << 1;

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;
	dst_U=dstUp;
	dst_V=dstVp;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U	
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_UV2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV2);
			dst_U+=dst_pitch_U;

			const uint16_t *src1=(uint16_t *)src_Unn;
			const uint16_t *src2=(uint16_t *)src_U;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_U+=dst_pitch_U;

			{
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				const uint16_t *src3=(uint16_t *)src_Un;
				const uint16_t *src4=(uint16_t *)src_Unnn;
				dst=(uint16_t *)dst_U;

				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
						(abs((int32_t)src_Unn[j]-(int32_t)src3[j])>=threshold_) &&
						(((src2[j]>src3[j]) && (src1[j]>src3[j])) ||
						((src2[j]<src3[j]) && (src1[j]<src3[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int32_t)src3[j]-(int32_t)src1[j])>=threshold_) &&
						(abs((int32_t)src_Unnn[j]-(int32_t)src1[j])>=threshold_) &&
						(((src3[j]>src1[j]) && (src4[j]>src1[j])) ||
						((src3[j]<src1[j]) && (src4[j]<src1[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;
					
					dst[j]=(uint16_t)((lookup[(uint32_t)src3[j]+vmax2]+(uint32_t)src4[j]+4)>>3);
				}
			}
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}
	else
	{
		const uint16_t *src1=(uint16_t *)src_Upp;
		const uint16_t *src2=(uint16_t *)src_Up;
		const uint16_t *src3=(uint16_t *)src_U;
		const uint16_t *src4=(uint16_t *)src_Un;

		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				
		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
				(abs((int32_t)src_U[j]-(int32_t)src2[j])>=threshold_) &&
				(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
				((src1[j]<src2[j]) && (src3[j]<src2[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
				(abs((int32_t)src4[j]-(int32_t)src3[j])>=threshold_) &&
				(((src2[j]>src3[j]) && (src4[j]>src3[j])) ||
				((src2[j]<src3[j]) && (src4[j]<src3[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;
		}

	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Upp;
			const uint16_t *src3=(uint16_t *)src_Up;
			uint16_t *dst=(uint16_t *)dst_U;

			// Upsample as needed.
			for(int32_t j=0; j<w_UV; j++)
			{
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src3[j]+2) >> 2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Un;
			const uint16_t *src3=(uint16_t *)src_Unn;
			const uint16_t *src4=(uint16_t *)src_Up;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;			

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst[j]=(uint16_t)((lookup[src4[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Unn;
			const uint16_t *src2=(uint16_t *)src_U;
			const uint16_t *src3=(uint16_t *)src_Un;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src3[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			bool *itabu0=interlaced_tab_U[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Un;
			const uint16_t *src2=(uint16_t *)src_Unn;
			const uint16_t *src3=(uint16_t *)src_Unnn;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src3[j]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Upp;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_U+=dst_pitch_U;

			src1=(uint16_t *)src_Up;
			src2=(uint16_t *)src_Un;
			dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_UV2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV2);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_UV2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV2);
			dst_V+=dst_pitch_V;

			const uint16_t *src1=(uint16_t *)src_Vnn;
			const uint16_t *src2=(uint16_t *)src_V;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_V+=dst_pitch_V;

			{
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];
				const uint16_t *src3=(uint16_t *)src_Vn;
				const uint16_t *src4=(uint16_t *)src_Vnnn;
				dst=(uint16_t *)dst_V;

				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
						(abs((int32_t)src1[j]-(int32_t)src3[j])>=threshold_) &&
						(((src2[j]>src3[j]) && (src1[j]>src3[j])) ||
						((src2[j]<src3[j]) && (src1[j]<src3[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int32_t)src3[j]-(int32_t)src1[j])>=threshold_) &&
						(abs((int32_t)src4[j]-(int32_t)src1[j])>=threshold_) &&
						(((src3[j]>src1[j]) && (src4[j]>src1[j])) ||
						((src3[j]<src1[j]) && (src4[j]<src1[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
					
					dst[j]=(uint16_t)((lookup[(uint32_t)src3[j]+vmax2]+(uint32_t)src4[j]+4)>>3);
				}
			}
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];
		const uint16_t *src1=(uint16_t *)src_Vpp;
		const uint16_t *src2=(uint16_t *)src_Vp;
		const uint16_t *src3=(uint16_t *)src_V;
		const uint16_t *src4=(uint16_t *)src_Vn;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
				(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
				(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
				((src1[j]<src2[j]) && (src3[j]<src2[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
				(abs((int32_t)src4[j]-(int32_t)src3[j])>=threshold_) &&
				(((src2[j]>src3[j]) && (src4[j]>src3[j])) ||
				((src2[j]<src3[j]) && (src4[j]<src3[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;
		}
	}
	
	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vpp;
			const uint16_t *src3=(uint16_t *)src_Vp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src3[j]+2) >> 2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1];
			bool *itabv2=interlaced_tab_V[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vn;
			const uint16_t *src3=(uint16_t *)src_Vnn;
			const uint16_t *src4=(uint16_t *)src_Vp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst[j]=(uint16_t)((lookup[src4[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Vnn;
			const uint16_t *src2=(uint16_t *)src_V;
			const uint16_t *src3=(uint16_t *)src_Vn;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src3[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			bool *itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabv2=interlaced_tab_V[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Vn;
			const uint16_t *src2=(uint16_t *)src_Vnn;
			const uint16_t *src3=(uint16_t *)src_Vnnn;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src3[j]+4)>>3);
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vpp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_V+=dst_pitch_V;

			src1=(uint16_t *)src_Vp;
			src2=(uint16_t *)src_Vn;
			dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_UV2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV2);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


static void Convert_Test_16_YV16(const MT_Data_Info &mt_data_inf,const uint32_t *lookup,const int16_t threshold,
	bool *interlaced_tab_U[],bool *interlaced_tab_V[],uint8_t bits_per_pixel)
{
	const uint8_t *srcYp=(const uint8_t *)mt_data_inf.src1;
	const uint8_t *srcUp=(const uint8_t *)mt_data_inf.src2;
	const uint8_t *srcVp=(const uint8_t *)mt_data_inf.src3;
	uint8_t *dstYp=(uint8_t *)mt_data_inf.dst1;
	uint8_t *dstUp=(uint8_t *)mt_data_inf.dst2;
	uint8_t *dstVp=(uint8_t *)mt_data_inf.dst3;
	const int32_t dst_w=mt_data_inf.dst_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitchU=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitchV=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch_Y=mt_data_inf.dst_pitch1;
	const ptrdiff_t dst_pitch_U=mt_data_inf.dst_pitch2;
	const ptrdiff_t dst_pitch_V=mt_data_inf.dst_pitch3;

	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_U,*dst_V;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int32_t w_UV=dst_w >> 1;
	const int32_t w_UV2=w_UV << 1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

	const ptrdiff_t pitch_U_2=2*src_pitchU;
	const ptrdiff_t pitch_V_2=2*src_pitchV;
	const int32_t threshold_=(int32_t)threshold << (bits_per_pixel-8);
	const uint32_t vmax=(uint32_t)1 << bits_per_pixel;
	const uint32_t vmax2=vmax << 1;
	const uint16_t valUV=239 << (bits_per_pixel-8);

	src_U=srcUp;
	src_V=srcVp;
	src_Up=src_U-src_pitchU;
	src_Upp=src_U-2*src_pitchU;
	src_Un=src_U+src_pitchU;
	src_Unn=src_U+2*src_pitchU;
	src_Unnn=src_U+3*src_pitchU;
	src_Vp=src_V-src_pitchV;
	src_Vpp=src_V-2*src_pitchV;
	src_Vn=src_V+src_pitchV;
	src_Vnn=src_V+2*src_pitchV;
	src_Vnnn=src_V+3*src_pitchV;
	dst_U=dstUp;
	dst_V=dstVp;

	Move_Full(srcYp,dstYp,dst_w << 1,h_Y_max-h_Y_min,src_pitch_Y,dst_pitch_Y);

// Planar U	
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_U,src_U,w_UV2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV2);
			dst_U+=dst_pitch_U;

			const uint16_t *src1=(uint16_t *)src_Unn;
			const uint16_t *src2=(uint16_t *)src_U;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_U+=dst_pitch_U;

			{
				bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				const uint16_t *src3=(uint16_t *)src_Un;
				const uint16_t *src4=(uint16_t *)src_Unnn;
				dst=(uint16_t *)dst_U;

				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
						(abs((int32_t)src_Unn[j]-(int32_t)src3[j])>=threshold_) &&
						(((src2[j]>src3[j]) && (src1[j]>src3[j])) ||
						((src2[j]<src3[j]) && (src1[j]<src3[j])))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int32_t)src3[j]-(int32_t)src1[j])>=threshold_) &&
						(abs((int32_t)src_Unnn[j]-(int32_t)src1[j])>=threshold_) &&
						(((src3[j]>src1[j]) && (src4[j]>src1[j])) ||
						((src3[j]<src1[j]) && (src4[j]<src1[j])))))
						itabu1[j]=true;
					else itabu1[j]=false;
					
					dst[j]=(uint16_t)((lookup[(uint32_t)src3[j]+vmax2]+(uint32_t)src4[j]+4)>>3);
				}
			}
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}
	else
	{
		const uint16_t *src1=(uint16_t *)src_Upp;
		const uint16_t *src2=(uint16_t *)src_Up;
		const uint16_t *src3=(uint16_t *)src_U;
		const uint16_t *src4=(uint16_t *)src_Un;

		bool *itabu0=interlaced_tab_U[0],*itabu1=interlaced_tab_U[1];
				
		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
				(abs((int32_t)src_U[j]-(int32_t)src2[j])>=threshold_) &&
				(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
				((src1[j]<src2[j]) && (src3[j]<src2[j])))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
				(abs((int32_t)src4[j]-(int32_t)src3[j])>=threshold_) &&
				(((src2[j]>src3[j]) && (src4[j]>src3[j])) ||
				((src2[j]<src3[j]) && (src4[j]<src3[j])))))
				itabu1[j]=true;
			else itabu1[j]=false;
		}

	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabu0=interlaced_tab_U[index_tab_0],*itabu1=interlaced_tab_U[index_tab_1];
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Upp;
			const uint16_t *src3=(uint16_t *)src_Up;
			uint16_t *dst=(uint16_t *)dst_U;

			// Upsample as needed.
			for(int32_t j=0; j<w_UV; j++)
			{
				if ((itabu0[j]) || (itabu1[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src3[j]+2) >> 2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1];
			bool *itabu2=interlaced_tab_U[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Un;
			const uint16_t *src3=(uint16_t *)src_Unn;
			const uint16_t *src4=(uint16_t *)src_Up;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabu2[j]=true;
				else itabu2[j]=false;			

				// Upsample as needed.
				if ((itabu2[j]) || (itabu1[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			const bool *itabu1=interlaced_tab_U[index_tab_1],*itabu2=interlaced_tab_U[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Unn;
			const uint16_t *src2=(uint16_t *)src_U;
			const uint16_t *src3=(uint16_t *)src_Un;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabu1[j]) || (itabu2[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src3[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		{
			bool *itabu0=interlaced_tab_U[index_tab_0];
			const bool *itabu2=interlaced_tab_U[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Un;
			const uint16_t *src2=(uint16_t *)src_Unn;
			const uint16_t *src3=(uint16_t *)src_Unnn;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabu0[j]=true;
				else itabu0[j]=false;

				// Upsample as needed.
				if ((itabu0[j]) || (itabu2[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_U+=dst_pitch_U;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_U+=pitch_U_2;
		src_Up+=pitch_U_2;
		src_Upp+=pitch_U_2;
		src_Un+=pitch_U_2;
		src_Unn+=pitch_U_2;
		src_Unnn+=pitch_U_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			const uint16_t *src1=(uint16_t *)src_U;
			const uint16_t *src2=(uint16_t *)src_Upp;
			uint16_t *dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_U+=dst_pitch_U;

			src1=(uint16_t *)src_Up;
			src2=(uint16_t *)src_Un;
			dst=(uint16_t *)dst_U;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_U,w_UV2);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV2);
			dst_U+=dst_pitch_U;

			src_U+=pitch_U_2;
			src_Up+=pitch_U_2;
			src_Upp+=pitch_U_2;
			src_Un+=pitch_U_2;
			src_Unn+=pitch_U_2;
			src_Unnn+=pitch_U_2;
		}
	}

// Planar V
	if (mt_data_inf.top)
	{
		for(int32_t i=0; i<4; i+=4)
		{
			memcpy(dst_V,src_V,w_UV2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV2);
			dst_V+=dst_pitch_V;

			const uint16_t *src1=(uint16_t *)src_Vnn;
			const uint16_t *src2=(uint16_t *)src_V;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_V+=dst_pitch_V;

			{
				bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];
				const uint16_t *src3=(uint16_t *)src_Vn;
				const uint16_t *src4=(uint16_t *)src_Vnnn;
				dst=(uint16_t *)dst_V;

				for(int32_t j=0; j<w_UV; j++)
				{
					if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
						(abs((int32_t)src1[j]-(int32_t)src3[j])>=threshold_) &&
						(((src2[j]>src3[j]) && (src1[j]>src3[j])) ||
						((src2[j]<src3[j]) && (src1[j]<src3[j])))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if (((abs((int32_t)src3[j]-(int32_t)src1[j])>=threshold_) &&
						(abs((int32_t)src4[j]-(int32_t)src1[j])>=threshold_) &&
						(((src3[j]>src1[j]) && (src4[j]>src1[j])) ||
						((src3[j]<src1[j]) && (src4[j]<src1[j])))))
						itabv1[j]=true;
					else itabv1[j]=false;
					
					dst[j]=(uint16_t)((lookup[(uint32_t)src3[j]+vmax2]+(uint32_t)src4[j]+4)>>3);
				}
			}
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
	else
	{
		bool *itabv0=interlaced_tab_V[0],*itabv1=interlaced_tab_V[1];
		const uint16_t *src1=(uint16_t *)src_Vpp;
		const uint16_t *src2=(uint16_t *)src_Vp;
		const uint16_t *src3=(uint16_t *)src_V;
		const uint16_t *src4=(uint16_t *)src_Vn;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
				(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
				(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
				((src1[j]<src2[j]) && (src3[j]<src2[j])))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if (((abs((int32_t)src2[j]-(int32_t)src3[j])>=threshold_) &&
				(abs((int32_t)src4[j]-(int32_t)src3[j])>=threshold_) &&
				(((src2[j]>src3[j]) && (src4[j]>src3[j])) ||
				((src2[j]<src3[j]) && (src4[j]<src3[j])))))
				itabv1[j]=true;
			else itabv1[j]=false;
		}
	}
	
	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabv0=interlaced_tab_V[index_tab_0],*itabv1=interlaced_tab_V[index_tab_1];
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vpp;
			const uint16_t *src3=(uint16_t *)src_Vp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv0[j]) || (itabv1[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src3[j]+2) >> 2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1];
			bool *itabv2=interlaced_tab_V[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vn;
			const uint16_t *src3=(uint16_t *)src_Vnn;
			const uint16_t *src4=(uint16_t *)src_Vp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabv2[j]=true;
				else itabv2[j]=false;			

				// Upsample as needed.
				if ((itabv2[j]) || (itabv1[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			const bool *itabv1=interlaced_tab_V[index_tab_1],*itabv2=interlaced_tab_V[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Vnn;
			const uint16_t *src2=(uint16_t *)src_V;
			const uint16_t *src3=(uint16_t *)src_Vn;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
				if ((itabv1[j]) || (itabv2[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src3[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		{
			bool *itabv0=interlaced_tab_V[index_tab_0];
			const bool *itabv2=interlaced_tab_V[index_tab_2];
			const uint16_t *src1=(uint16_t *)src_Vn;
			const uint16_t *src2=(uint16_t *)src_Vnn;
			const uint16_t *src3=(uint16_t *)src_Vnnn;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int32_t)src1[j]-(int32_t)src2[j])>=threshold_) &&
					(abs((int32_t)src3[j]-(int32_t)src2[j])>=threshold_) &&
					(((src1[j]>src2[j]) && (src3[j]>src2[j])) ||
					((src1[j]<src2[j]) && (src3[j]<src2[j])))))
					itabv0[j]=true;
				else itabv0[j]=false;

				// Upsample as needed.
				if ((itabv0[j]) || (itabv2[j]))
				{
					dst[j]=valUV;
				}
				else
				{
					dst[j]=(uint16_t)((lookup[src1[j]]+(uint32_t)src2[j]+2)>>2);
				}
			}
		}
		dst_V+=dst_pitch_V;

		index_tab_0=(index_tab_0+2)%Interlaced_Tab_Size;
		index_tab_1=(index_tab_1+2)%Interlaced_Tab_Size;
		index_tab_2=(index_tab_2+2)%Interlaced_Tab_Size;

		src_V+=pitch_V_2;
		src_Vp+=pitch_V_2;
		src_Vpp+=pitch_V_2;
		src_Vn+=pitch_V_2;
		src_Vnn+=pitch_V_2;
		src_Vnnn+=pitch_V_2;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_4; i<h_Y_max; i+=4)
		{
			const uint16_t *src1=(uint16_t *)src_V;
			const uint16_t *src2=(uint16_t *)src_Vpp;
			uint16_t *dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[(uint32_t)src1[j]+vmax2]+(uint32_t)src2[j]+4)>>3);
			dst_V+=dst_pitch_V;

			src1=(uint16_t *)src_Vp;
			src2=(uint16_t *)src_Vn;
			dst=(uint16_t *)dst_V;

			for(int32_t j=0; j<w_UV; j++)
				dst[j]=(uint16_t)((lookup[src1[j]]+lookup[(uint32_t)src2[j]+vmax]+4)>>3);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_V,w_UV2);
			dst_V+=dst_pitch_V;

			memcpy(dst_V,src_Vn,w_UV2);
			dst_V+=dst_pitch_V;

			src_V+=pitch_V_2;
			src_Vp+=pitch_V_2;
			src_Vpp+=pitch_V_2;
			src_Vn+=pitch_V_2;
			src_Vnn+=pitch_V_2;
			src_Vnnn+=pitch_V_2;
		}
	}
}


void JPSDR_AutoYUY2::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_AutoYUY2 *ptrClass=(JPSDR_AutoYUY2 *)data->pClass;
	
	switch(data->f_process)
	{
			case 1 : Convert_Automatic_to_YUY2(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8,ptrClass->mData.threshold,
						 ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id]);
				break;
			case 2 : Convert_Test_to_YUY2(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8,ptrClass->mData.threshold,
						 ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id]);
				break;
			case 3 : Convert_Automatic_to_UYVY(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8,ptrClass->mData.threshold,
						 ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id]);
				break;
			case 4 : Convert_Test_to_UYVY(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8,ptrClass->mData.threshold,
						 ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id]);
				break;
			case 5 : Convert_Automatic_8_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8,ptrClass->mData.threshold,
						 ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id]);
				break;
			case 6 : Convert_Test_8_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8,ptrClass->mData.threshold,
						 ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id]);
				break;
			case 8 : Convert_Progressive_8_YV16_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 9 : Convert_Progressive_8_YV16_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 10 : Convert_Progressive_8_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8); break;
			case 12 : Convert_Interlaced_8_YV16_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 13 : Convert_Interlaced_8_YV16_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 14 : Convert_Interlaced_8_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8); break;
			case 15 : Convert_Progressive_to_YUY2_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 16 : Convert_Progressive_to_YUY2_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 17 : Convert_Progressive_to_YUY2(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8); break;
			case 18 : Convert_Interlaced_to_YUY2_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 19 : Convert_Interlaced_to_YUY2_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 20 : Convert_Interlaced_to_YUY2(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8); break;
			case 21 : Convert_Progressive_to_UYVY_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 22 : Convert_Progressive_to_UYVY_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 23 : Convert_Progressive_to_UYVY(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8); break;
			case 24 : Convert_Interlaced_to_UYVY_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 25 : Convert_Interlaced_to_UYVY_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 26 : Convert_Interlaced_to_UYVY(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale8); break;
			case 27 : Convert_Automatic_16_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale16,ptrClass->mData.threshold,
						  ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id],
						  ptrClass->image_data.src_bits_pixel);
				break;
			case 28 : Convert_Test_16_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale16,ptrClass->mData.threshold,
						  ptrClass->interlaced_tab_U[data->thread_Id],ptrClass->interlaced_tab_V[data->thread_Id],
						  ptrClass->image_data.src_bits_pixel);
				break;
			case 30 : Convert_Progressive_16_YV16_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 31 : Convert_Progressive_16_YV16_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 32 : Convert_Progressive_16_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale16,
						  ptrClass->image_data.src_bits_pixel);
				break;
			case 34 : Convert_Interlaced_16_YV16_AVX(ptrClass->MT_Data[data->thread_Id]); break;
			case 35 : Convert_Interlaced_16_YV16_SSE2(ptrClass->MT_Data[data->thread_Id]); break;
			case 36 : Convert_Interlaced_16_YV16(ptrClass->MT_Data[data->thread_Id],ptrClass->lookup_Upscale16,
						  ptrClass->image_data.src_bits_pixel);
				break;
#ifdef AVX2_BUILD_POSSIBLE
			case 7 : Convert_Progressive_8_YV16_AVX2(ptrClass->MT_Data[data->thread_Id]); break;
			case 11 : Convert_Interlaced_8_YV16_AVX2(ptrClass->MT_Data[data->thread_Id]); break;
			case 29 : Convert_Progressive_16_YV16_AVX2(ptrClass->MT_Data[data->thread_Id]); break;
			case 33 : Convert_Interlaced_16_YV16_AVX2(ptrClass->MT_Data[data->thread_Id]); break;
#endif
		default : break;
	}
}


void JPSDR_AutoYUY2::Run()
{
	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;

	if (VDub2_Enable)
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo((fa->dst).mpPixmap);

		switch(image_data.src_bits_pixel)
		{
			case 8 :
				info->ref_r=0xFF;
				info->ref_g=0xFF;
				info->ref_b=0xFF;
				info->ref_a=0xFF;
				break;
			case 10 :
				info->ref_r=0x3FF;
				info->ref_g=0x3FF;
				info->ref_b=0x3FF;
				info->ref_a=0x3FF;
				break;
			case 12 :
				info->ref_r=0xFFF;
				info->ref_g=0xFFF;
				info->ref_b=0xFFF;
				info->ref_a=0xFFF;
				break;
			case 14 :
				info->ref_r=0x3FFF;
				info->ref_g=0x3FFF;
				info->ref_b=0x3FFF;
				info->ref_a=0x3FFF;
				break;
			default :
				info->ref_r=0xFFFF;
				info->ref_g=0xFFFF;
				info->ref_b=0xFFFF;
				info->ref_a=0xFFFF;
				break;
		}
	}

	uint8_t f_proc=0;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	for(uint8_t i=0; i<threads_number; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)image_data.src_plane0+(MT_Data[i].src_Y_h_min*image_data.src_pitch0));
		MT_Data[i].src2=(void *)((uint8_t *)image_data.src_plane1+(MT_Data[i].src_UV_h_min*image_data.src_pitch1));
		MT_Data[i].src3=(void *)((uint8_t *)image_data.src_plane2+(MT_Data[i].src_UV_h_min*image_data.src_pitch2));
		MT_Data[i].src_pitch1=image_data.src_pitch0;
		MT_Data[i].src_pitch2=image_data.src_pitch1;
		MT_Data[i].src_pitch3=image_data.src_pitch2;
		MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0+(MT_Data[i].dst_Y_h_min*image_data.dst_pitch0));
		MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1+(MT_Data[i].dst_UV_h_min*image_data.dst_pitch1));
		MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2+(MT_Data[i].dst_UV_h_min*image_data.dst_pitch2));
		MT_Data[i].dst_pitch1=image_data.dst_pitch0;
		MT_Data[i].dst_pitch2=image_data.dst_pitch1;
		MT_Data[i].dst_pitch3=image_data.dst_pitch2;
	}

	switch(image_data.dst_video_mode)
	{
		case VMODE_YUYV :
			switch(mData.convert_mode)
			{
				case 0 :
					if (AVX_Enable) f_proc=15;
					else
					{
						if (SSE2_Enable) f_proc=16;
						else f_proc=17;
					}
					break;
				case 1 :
					if (AVX_Enable) f_proc=18;
					else
					{
						if (SSE2_Enable) f_proc=19;
						else f_proc=20;
					}
					break;
				case 2 : f_proc=1; break;
				case 3 : f_proc=2; break;
				default : break;
			}
			break;
		case VMODE_UYVY :
			switch(mData.convert_mode)
			{
				case 0 :
					if (AVX_Enable) f_proc=21;
					else
					{
						if (SSE2_Enable) f_proc=22;
						else f_proc=23;
					}
					break;
				case 1 :
					if (AVX_Enable) f_proc=24;
					else
					{
						if (SSE2_Enable) f_proc=25;
						else f_proc=26;
					}
					break;
				case 2 : f_proc=3; break;
				case 3 : f_proc=4; break;
				default : break ;
			}
			break;
		case VMODE_PLANAR_YUV422 :
			if (image_data.src_bits_pixel==8)
			{
				switch(mData.convert_mode)
				{
					case 0 :
#ifdef AVX2_BUILD_POSSIBLE
						if (AVX2_Enable) f_proc=7;
						else
#endif
						{
							if (AVX_Enable) f_proc=8;
							else
							{
								if (SSE2_Enable) f_proc=9;
								else f_proc=10;
							}
						}
						break;
					case 1 :
#ifdef AVX2_BUILD_POSSIBLE
						if (AVX2_Enable) f_proc=11;
						else
#endif
						{
							if (AVX_Enable) f_proc=12;
							else
							{
								if (SSE2_Enable) f_proc=13;
								else f_proc=14;
							}
						}
						break;
					case 2 : f_proc=5; break;
					case 3 : f_proc=6; break;
					default : break;
				}
			}
			else
			{
				switch(mData.convert_mode)
				{
					case 0 :
#ifdef AVX2_BUILD_POSSIBLE
						if (AVX2_Enable) f_proc=29;
						else
#endif
						{
							if (AVX_Enable) f_proc=30;
							else
							{
								if (SSE2_Enable) f_proc=31;
								else f_proc=32;
							}
						}
						break;
					case 1 :
#ifdef AVX2_BUILD_POSSIBLE
						if (AVX2_Enable) f_proc=33;
						else
#endif
						{
							if (AVX_Enable) f_proc=34;
							else
							{
								if (SSE2_Enable) f_proc=35;
								else f_proc=36;
							}
						}
						break;
					case 2 : f_proc=27; break;
					case 3 : f_proc=28; break;
					default : break;
				}
			}
			break;
		default : break;
	}

	if (threads_number>1)
	{
		if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
		{
			for(uint8_t i=0; i<threads_number; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<threads_number; i++)
				MT_Thread[i].f_process=0;

			poolInterface->ReleaseThreadPool(UserId,false);
		}
	}
	else
	{
		switch(f_proc)
		{
			case 1 : Convert_Automatic_to_YUY2(MT_Data[0],lookup_Upscale8,mData.threshold,
						 interlaced_tab_U[0],interlaced_tab_V[0]);
				break;
			case 2 : Convert_Test_to_YUY2(MT_Data[0],lookup_Upscale8,mData.threshold,
						 interlaced_tab_U[0],interlaced_tab_V[0]);
				break;
			case 3 : Convert_Automatic_to_UYVY(MT_Data[0],lookup_Upscale8,mData.threshold,
						 interlaced_tab_U[0],interlaced_tab_V[0]);
				break;
			case 4 : Convert_Test_to_UYVY(MT_Data[0],lookup_Upscale8,mData.threshold,
						 interlaced_tab_U[0],interlaced_tab_V[0]);
				break;
			case 5 : Convert_Automatic_8_YV16(MT_Data[0],lookup_Upscale8,mData.threshold,
						 interlaced_tab_U[0],interlaced_tab_V[0]);
				break;
			case 6 : Convert_Test_8_YV16(MT_Data[0],lookup_Upscale8,mData.threshold,
						 interlaced_tab_U[0],interlaced_tab_V[0]);
				break;
			case 8 : Convert_Progressive_8_YV16_AVX(MT_Data[0]); break;
			case 9 : Convert_Progressive_8_YV16_SSE2(MT_Data[0]); break;
			case 10 : Convert_Progressive_8_YV16(MT_Data[0],lookup_Upscale8); break;
			case 12 : Convert_Interlaced_8_YV16_AVX(MT_Data[0]); break;
			case 13 : Convert_Interlaced_8_YV16_SSE2(MT_Data[0]); break;
			case 14 : Convert_Interlaced_8_YV16(MT_Data[0],lookup_Upscale8); break;
			case 15 : Convert_Progressive_to_YUY2_AVX(MT_Data[0]); break;
			case 16 : Convert_Progressive_to_YUY2_SSE2(MT_Data[0]); break;
			case 17 : Convert_Progressive_to_YUY2(MT_Data[0],lookup_Upscale8); break;
			case 18 : Convert_Interlaced_to_YUY2_AVX(MT_Data[0]); break;
			case 19 : Convert_Interlaced_to_YUY2_SSE2(MT_Data[0]); break;
			case 20 : Convert_Interlaced_to_YUY2(MT_Data[0],lookup_Upscale8); break;
			case 21 : Convert_Progressive_to_UYVY_AVX(MT_Data[0]); break;
			case 22 : Convert_Progressive_to_UYVY_SSE2(MT_Data[0]); break;
			case 23 : Convert_Progressive_to_UYVY(MT_Data[0],lookup_Upscale8); break;
			case 24 : Convert_Interlaced_to_UYVY_AVX(MT_Data[0]); break;
			case 25 : Convert_Interlaced_to_UYVY_SSE2(MT_Data[0]); break;
			case 26 : Convert_Interlaced_to_UYVY(MT_Data[0],lookup_Upscale8); break;
			case 27 : Convert_Automatic_16_YV16(MT_Data[0],lookup_Upscale16,mData.threshold,
						  interlaced_tab_U[0],interlaced_tab_V[0],image_data.src_bits_pixel);
				break;
			case 28 : Convert_Test_16_YV16(MT_Data[0],lookup_Upscale16,mData.threshold,
						  interlaced_tab_U[0],interlaced_tab_V[0],image_data.src_bits_pixel);
				break;
			case 30 : Convert_Progressive_16_YV16_AVX(MT_Data[0]); break;
			case 31 : Convert_Progressive_16_YV16_SSE2(MT_Data[0]); break;
			case 32 : Convert_Progressive_16_YV16(MT_Data[0],lookup_Upscale16,image_data.src_bits_pixel); break;
			case 34 : Convert_Interlaced_16_YV16_AVX(MT_Data[0]); break;
			case 35 : Convert_Interlaced_16_YV16_SSE2(MT_Data[0]); break;
			case 36 : Convert_Interlaced_16_YV16(MT_Data[0],lookup_Upscale16,image_data.src_bits_pixel); break;
#ifdef AVX2_BUILD_POSSIBLE
			case 7 : Convert_Progressive_8_YV16_AVX2(MT_Data[0]); break;
			case 11 : Convert_Interlaced_8_YV16_AVX2(MT_Data[0]); break;
			case 29 : Convert_Progressive_16_YV16_AVX2(MT_Data[0]); break;
			case 33 : Convert_Interlaced_16_YV16_AVX2(MT_Data[0]); break;
#endif
			default : break;
		}
	}
}




void JPSDR_AutoYUY2::End()
{
	int16_t i,j;

	for (j=threads_number-1; j>=0; j--)
	{
		for (i=Interlaced_Tab_Size-1; i>=0; i--)
		{
			myfree(interlaced_tab_V[j][i]);
			myfree(interlaced_tab_U[j][i]);
		}
	}
	myfree(lookup_Upscale16);
	myfree(lookup_Upscale8);
}



void JPSDR_AutoYUY2::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.output_format=argv[0].asInt();
	mData.convert_mode=argv[1].asInt();
	mData.threshold=argv[2].asInt();
	mData.mt_mode=!!argv[3].asInt();
}

bool JPSDR_AutoYUY2::Configure(VDXHWND hwnd)
{
	JPSDR_AutoYUY2Dialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_AutoYUY2::GetScriptString(char *buf, int maxlen)
{
 
  
    SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d)",
		mData.output_format,mData.convert_mode,mData.threshold,mData.mt_mode);
}



extern VDXFilterDefinition2 filterDef_JPSDR_AutoYUY2=
VDXVideoFilterDefinition<JPSDR_AutoYUY2>("JPSDR","AutoYUY2 v4.1.2","Convert Planar4:2:0 to 4:2:2 modes. [SSE2][AVX][AVX2] Optimised.");

