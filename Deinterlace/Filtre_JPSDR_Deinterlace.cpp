/*
 *  Deinterlace
 *
 *  Deinterlace video.
 *  Copyright (C) 2000 JPSDR
 *	
 *  Deinterlace is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  Deinterlace is distributed in the hope that it will be useful,
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

#include "Deinterlace_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

#define Number_Max_Lines 2048

extern ThreadPoolInterface *poolInterface;

extern int g_VFVAPIVersion;

#if _MSC_VER >= 1900
#define AVX2_BUILD_POSSIBLE
#endif


extern "C" void JPSDR_Deinterlace_Blend_8(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_24(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_32(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);	
extern "C" void JPSDR_Deinterlace_Blend_16(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);	
extern "C" void JPSDR_Deinterlace_Blend_48(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_64(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);	


extern "C" void JPSDR_Deinterlace_Blend_8_SSE2(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		
extern "C" void JPSDR_Deinterlace_Blend_8_AVX(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		

extern "C" void JPSDR_Deinterlace_Blend_8_Tri_SSE2(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_SSE2_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_SSE2_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_SSE2_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);


extern "C" void JPSDR_Deinterlace_Blend_16_SSE2(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		
extern "C" void JPSDR_Deinterlace_Blend_16_AVX(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		

extern "C" void JPSDR_Deinterlace_Blend_16_Tri_SSE2(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_SSE2_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_SSE2_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_SSE2_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

#ifdef AVX2_BUILD_POSSIBLE
extern "C" void JPSDR_Deinterlace_Blend_8_AVX2(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_AVX2(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX2(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX2(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX2_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX2_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_8_Tri_AVX2_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX2_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX2_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_16_Tri_AVX2_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
#endif


class JPSDR_DeinterlaceData
{
public:
	uint8_t mode;
	//bool keepTop,TFF;
	bool keepTop;
	bool file_mode;
	char filename[1024];
	bool mt_mode;

	JPSDR_DeinterlaceData(void);
};



JPSDR_DeinterlaceData::JPSDR_DeinterlaceData(void)
{
	mode=1;
	keepTop=true;
//	TFF=true;
	strcpy_s(filename,1024,"");
	file_mode=false;
	mt_mode=true;
}


typedef struct _MT_Data_Info
{
	void *src1_0,*src1_1,*src2_0,*src2_1,*src3_0,*src3_1;
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
	bool div_h,dec_h;
} MT_Data_Info;


class JPSDR_DeinterlaceDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_DeinterlaceDialog(JPSDR_DeinterlaceData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_DEINTERLACE),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	void RefreshDisplay(void);
	void SetDisplay(void);
	
	JPSDR_DeinterlaceData& mData;
	JPSDR_DeinterlaceData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_DeinterlaceDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


void JPSDR_DeinterlaceDialog::SetDisplay(void)
{
	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);
	switch (mData.mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_BLEND,BST_CHECKED); break;
		case 2 : CheckDlgButton(mhdlg,IDC_BLEND_TRI,BST_CHECKED); break;
		case 3 :
		case 4 :
			CheckDlgButton(mhdlg,IDC_INTERPOLATE,BST_CHECKED);
			break;
/*		case 5 :
		case 6 :
			CheckDlgButton(mhdlg,IDC_YADIF,BST_CHECKED);
			break;*/
	}
/*	if (mData.TFF) CheckDlgButton(mhdlg,IDC_TFF,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_BFF,BST_CHECKED);*/
	if (mData.keepTop) CheckDlgButton(mhdlg,IDC_BOTTOM,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_TOP,BST_CHECKED);
}


void JPSDR_DeinterlaceDialog::RefreshDisplay(void)
{

	if (mData.file_mode)
	{
		EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_FILENAME),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_BLEND),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BLEND_TRI),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_INTERPOLATE),false);
		//EnableWindow(GetDlgItem(mhdlg,IDC_YADIF),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_STATIC_MODE),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),false);
//		EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_TOP),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),false);
/*		EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);*/
		EnableWindow(GetDlgItem(mhdlg,IDC_ENABLE_MT),false);
//		EnableWindow(GetDlgItem(mhdlg,IDC_PREVIEW),false);
	}
	else
	{
		EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_FILENAME),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BLEND),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_BLEND_TRI),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_INTERPOLATE),true);
		//EnableWindow(GetDlgItem(mhdlg,IDC_YADIF),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_STATIC_MODE),true);
//		EnableWindow(GetDlgItem(mhdlg,IDC_PREVIEW),true);
		switch(mData.mode)
		{
			case 3 :
			case 4 :
				EnableWindow(GetDlgItem(mhdlg,IDC_ENABLE_MT),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),true);
/*				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);*/
				break;
/*			case 5 :
			case 6 :
				EnableWindow(GetDlgItem(mhdlg,IDC_ENABLE_MT),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),true);
				break;*/
			default :
				EnableWindow(GetDlgItem(mhdlg,IDC_ENABLE_MT),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),false);
/*				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);*/
				break;
		}
	}
}



bool JPSDR_DeinterlaceDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_FILE_MODE,mData.file_mode?BST_CHECKED:BST_UNCHECKED);
	if (mData.file_mode)
	{
		SetWindowText(GetDlgItem(mhdlg, IDC_FILENAME),mData.filename);
	}

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}
	
	SetDisplay();
	RefreshDisplay();

	return false;
}


void JPSDR_DeinterlaceDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_DeinterlaceDialog::SaveToData()
{
	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);
	mData.keepTop=!!IsDlgButtonChecked(mhdlg,IDC_BOTTOM);
//	mData.TFF=!!IsDlgButtonChecked(mhdlg,IDC_TFF);
	if ((mData.mode==3) && (mData.keepTop)) mData.mode=4;
	if ((mData.mode==4) && (!mData.keepTop)) mData.mode=3;
/*	if ((mData.mode==5) && (!mData.keepTop)) mData.mode=6;
	if ((mData.mode==6) && (mData.keepTop)) mData.mode=5;*/
	mData.file_mode=!!IsDlgButtonChecked(mhdlg,IDC_FILE_MODE);
	if (mData.file_mode) GetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename, 1024);
	else strcpy_s(mData.filename,1024,"");
	
	return true;
}



bool JPSDR_DeinterlaceDialog::OnCommand(int cmd)
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
		case IDC_BLEND :
			mData.mode=1;
			if (mifp && SaveToData()) mifp->RedoSystem();
			RefreshDisplay();
			return true;
		case IDC_BLEND_TRI :
			mData.mode=2;
			if (mifp && SaveToData()) mifp->RedoSystem();
			RefreshDisplay();
			return true;
		case IDC_INTERPOLATE :
			mData.mode=3;
			if (mifp && SaveToData()) mifp->RedoSystem();
			RefreshDisplay();
			return true;
/*		case IDC_YADIF :
			mData.mode=5;
			if (mifp && SaveToData()) mifp->RedoSystem();
			RefreshDisplay();
			return true;*/
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_BOTTOM :
		case IDC_TOP :
/*		case IDC_TFF :
		case IDC_BFF :*/
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_FILE_MODE :
			mData.file_mode=!!IsDlgButtonChecked(mhdlg, IDC_FILE_MODE);
/*			if (mData.file_mode)
			{
				if (mifp->IsPreviewDisplayed()) mifp->Toggle((VDXHWND)mhdlg);
			}*/
			RefreshDisplay();
			if (mData.file_mode) GetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename, 1024);
			return true;
		case IDC_BROWSE:
			OPENFILENAME ofn;
			mData.filename[0] = '\0';
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = mhdlg;
			ofn.hInstance = NULL;
			ofn.lpTemplateName = NULL;
			ofn.lpstrFilter = "Text Files\0*.txt\0\0";
			ofn.lpstrCustomFilter = NULL;
			ofn.nMaxCustFilter = 0;
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = mData.filename;
			ofn.nMaxFile = 1024;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = "Select File";
			ofn.Flags = OFN_CREATEPROMPT;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;
			ofn.lpstrDefExt = NULL;
			ofn.lCustData = 0;
			ofn.lpfnHook = NULL;
			GetOpenFileName(&ofn);
			if (mData.filename[0] != '\0')
			{
				SetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename);
			}
			return true;
		case IDC_PREVIEW:
			if (mifp)
				mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}


class JPSDR_Deinterlace : public JPSDRVDXVideoFilter
{
public:
	virtual ~JPSDR_Deinterlace();
	JPSDR_Deinterlace(){}
	JPSDR_Deinterlace(const JPSDR_Deinterlace& a)
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
	Image_Data image_data;
	void *buffer0,*buffer1,*buffer2;
	ptrdiff_t buffer0_pitch,buffer0_modulo;
	ptrdiff_t buffer1_pitch,buffer1_modulo;
	ptrdiff_t buffer2_pitch,buffer2_modulo;
	void *buffer[3][2];
	uint32_t debut[Number_Max_Lines],fin[Number_Max_Lines];
	uint8_t tab_mode[Number_Max_Lines];
	uint16_t nbre_file_mode;
	bool SSE2_Enable,AVX_Enable,AVX2_Enable,VDub2_Enable;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,
		uint8_t _32bits,uint8_t nBits_data);

	void Deinterlace_Blend_8_MT_1(uint8_t thread_num);
	void Deinterlace_Blend_8_MT_2(uint8_t thread_num);
	void Deinterlace_Blend_8_MT_3(uint8_t thread_num);
	void Deinterlace_Blend_8_MT_4(uint8_t thread_num);
	void Deinterlace_Blend_8_MT_5(uint8_t thread_num);

	void Deinterlace_Blend_16_MT_1(uint8_t thread_num);
	void Deinterlace_Blend_16_MT_2(uint8_t thread_num);
	void Deinterlace_Blend_16_MT_3(uint8_t thread_num);
	void Deinterlace_Blend_16_MT_4(uint8_t thread_num);
	void Deinterlace_Blend_16_MT_5(uint8_t thread_num);

	void Deinterlace_8_Tri_MT_1(uint8_t thread_num);
	void Deinterlace_8_Tri_MT_2(uint8_t thread_num);
	void Deinterlace_8_Tri_MT_3(uint8_t thread_num);
	void Deinterlace_8_Tri_MT_4(uint8_t thread_num);
	void Deinterlace_8_Tri_MT_5(uint8_t thread_num);

	void Deinterlace_16_Tri_MT_1(uint8_t thread_num);
	void Deinterlace_16_Tri_MT_2(uint8_t thread_num);
	void Deinterlace_16_Tri_MT_3(uint8_t thread_num);
	void Deinterlace_16_Tri_MT_4(uint8_t thread_num);
	void Deinterlace_16_Tri_MT_5(uint8_t thread_num);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_DeinterlaceData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Deinterlace)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Deinterlace,ScriptConfig,"iiis")
VDXVF_END_SCRIPT_METHODS()



bool JPSDR_Deinterlace::Init()
{
	AVX2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX2)!=0);
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	InternalInit();

	return(true);
}


void JPSDR_Deinterlace::InternalInit(void)
{
	int16_t i,j;	

	buffer0=NULL;
	buffer1=NULL;
	buffer2=NULL;
	buffer0_pitch=0;
	buffer0_modulo=0;
	buffer1_pitch=0;
	buffer1_modulo=0;
	buffer2_pitch=0;
	buffer2_modulo=0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<2; j++)
		{
			buffer[i][j]=NULL;
		}
	}

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


JPSDR_Deinterlace::~JPSDR_Deinterlace()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
}


uint8_t JPSDR_Deinterlace::CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,
	uint8_t _32bits,uint8_t nBits_data)
{
	int32_t Coeff_size;

	if (nBits_data==8) Coeff_size=1;
	else
	{
		if (nBits_data<=16) Coeff_size=2;
		else Coeff_size=4;
	}

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
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w*Coeff_size;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w*Coeff_size;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w*Coeff_size;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w*Coeff_size;
				break;
			case 1 :
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w*Coeff_size+1)>>1;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w*Coeff_size+1)>>1;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w*Coeff_size+1)>>1;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w*Coeff_size+1)>>1;
				break;
			case 2 :
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w*Coeff_size+3)>>2;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w*Coeff_size+3)>>2;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w*Coeff_size+3)>>2;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w*Coeff_size+3)>>2;
				break;
			default :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w*Coeff_size;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w*Coeff_size;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w*Coeff_size;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w*Coeff_size;
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
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w*Coeff_size;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w*Coeff_size;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w*Coeff_size;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w*Coeff_size;
				break;
			case 1 :
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w*Coeff_size+1)>>1;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w*Coeff_size+1)>>1;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w*Coeff_size+1)>>1;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w*Coeff_size+1)>>1;
				break;
			case 2 :
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w*Coeff_size+3)>>2;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w*Coeff_size+3)>>2;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w*Coeff_size+3)>>2;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w*Coeff_size+3)>>2;
				break;
			default :
				MT_Data[0].src_Y_w_32=MT_Data[0].src_Y_w*Coeff_size;
				MT_Data[0].dst_Y_w_32=MT_Data[0].dst_Y_w*Coeff_size;
				MT_Data[0].src_UV_w_32=MT_Data[0].src_UV_w*Coeff_size;
				MT_Data[0].dst_UV_w_32=MT_Data[0].dst_UV_w*Coeff_size;
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
				MT_Data[i].src_Y_w_32=MT_Data[i].src_Y_w*Coeff_size;
				MT_Data[i].dst_Y_w_32=MT_Data[i].dst_Y_w*Coeff_size;
				MT_Data[i].src_UV_w_32=MT_Data[i].src_UV_w*Coeff_size;
				MT_Data[i].dst_UV_w_32=MT_Data[i].dst_UV_w*Coeff_size;
				break;
			case 1 :
				MT_Data[i].src_Y_w_32=(MT_Data[i].src_Y_w*Coeff_size+1)>>1;
				MT_Data[i].dst_Y_w_32=(MT_Data[i].dst_Y_w*Coeff_size+1)>>1;
				MT_Data[i].src_UV_w_32=(MT_Data[i].src_UV_w*Coeff_size+1)>>1;
				MT_Data[i].dst_UV_w_32=(MT_Data[i].dst_UV_w*Coeff_size+1)>>1;
				break;
			case 2 :
				MT_Data[i].src_Y_w_32=(MT_Data[i].src_Y_w*Coeff_size+3)>>2;
				MT_Data[i].dst_Y_w_32=(MT_Data[i].dst_Y_w*Coeff_size+3)>>2;
				MT_Data[i].src_UV_w_32=(MT_Data[i].src_UV_w*Coeff_size+3)>>2;
				MT_Data[i].dst_UV_w_32=(MT_Data[i].dst_UV_w*Coeff_size+3)>>2;
				break;
			default :
				MT_Data[i].src_Y_w_32=MT_Data[i].src_Y_w*Coeff_size;
				MT_Data[i].dst_Y_w_32=MT_Data[i].dst_Y_w*Coeff_size;
				MT_Data[i].src_UV_w_32=MT_Data[i].src_UV_w*Coeff_size;
				MT_Data[i].dst_UV_w_32=MT_Data[i].dst_UV_w*Coeff_size;
				break;
		}
	}
	return(max);
}


static inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch)
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


static inline void Move_Half(const void *src_, void *dst_, const int32_t w,const int32_t h,
		const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	const uint8_t *src=(uint8_t *)src_;
	uint8_t *dst=(uint8_t *)dst_;
	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}
}


uint32 JPSDR_Deinterlace::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;

	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));

	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :*/
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
/*		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :*/
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar16 :
		case nsVDXPixmap::kPixFormat_Y16 :
/*		case nsVDXPixmap::kPixFormat_YUV444_Y416 :
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
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar16 :*/
		case nsVDXPixmap::kPixFormat_YUV422_YU64 :
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	image_data.src_full_mode=CheckFullRangeMode(&fa->src);
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;

	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_FR;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR;
			break;
	}
	
	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	if (mData.mt_mode)
	{
		if (!mData.file_mode)
		{
			switch(mData.mode)
			{
				case 1 :
				case 2 :
					if (VDub2_Enable)
						return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES_32);
					else
					{
						if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
						else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					}
					break;
				default :
					if (VDub2_Enable)
						return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES_32);
					else
					{
						if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
						else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					}
					break;
			}
		}
		else
		{
			if (VDub2_Enable)
				return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES_32);
			else
				return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
		}
	}
	else
	{
		if (!mData.file_mode)
		{
			switch(mData.mode)
			{
				case 2 :
					if (VDub2_Enable)
						return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES_32);
					else
					{
						if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
						else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					}
					break;
				default :
					if (VDub2_Enable)
						return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES_32);
					else
					{
						if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
						else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					}
					break;
			}
		}
		else
		{
			if (VDub2_Enable)
				return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES_32);
			else
				return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
		}
	}
}


void JPSDR_Deinterlace::Start()
{
	uint32_t u,v;
	uint16_t w,line_file;
	int16_t i,j;
	int32_t offset;
	FILE *file_in;
	char buffer_in[1024];
	bool buff_ok;

	if ((g_VFVAPIVersion<12) || ((g_VFVAPIVersion<14) && ((mData.mode==5) || (mData.mode==6))))
	{
		ff->Except("This virtualdub version doesn't support this configuration filter!");
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

	if ((image_data.src_h0<8) || (image_data.src_w0<8))
	{
		ff->Except("Size must be at least 8x8!");
		return;
	}

	switch (image_data.video_mode)
	{
		case VMODE_PLANAR_YUV420 :
			if ((image_data.src_h0%4)!=0)
			{
				ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
				return;
			}
			break;
		case VMODE_PLANAR_YUV410 :
			if ((image_data.src_h0%8)!=0)
			{
				ff->Except("Vertical size must be multiple of 8 for 4:1:0!");
				return;
			}
			break;
		default :
			if ((image_data.src_h0%2)!=0)
			{
				ff->Except("Vertical size must be multiple of 2!");
				return;
			}
			break;
	}

	if (mData.file_mode)
	{
		if (strlen(mData.filename)==0)
		{
			ff->Except("No file selected!");
			return;
		}

		nbre_file_mode=0;
		offset=0;
		line_file=0;

		if (fopen_s(&file_in,mData.filename,"rt")!=0)
		{
			strcpy_s(buffer_in,1024,"Unable to open file : ");
			strcat_s(buffer_in,1024,mData.filename);
			ff->Except(buffer_in);
			return;
		}

		while (fgets(buffer_in,1024,file_in))
		{
			line_file++;
			if ((buffer_in[0]=='+') || (buffer_in[0]=='-'))
			{
				if (sscanf_s(buffer_in,"%ld",&offset)==EOF)
				{
					fclose(file_in);
					sprintf_s(buffer_in,1024,"Error in file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
			}
			else
			{
				if (sscanf_s(buffer_in,"%lu %lu %hu",&u,&v,&w)==EOF)
				{
					fclose(file_in);
					sprintf_s(buffer_in,1024,"Error in file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
				else
				{
					if (w>4)
					{
						fclose(file_in);
						sprintf_s(buffer_in,1024,"Incorrect mode number on line %ld.",line_file);
						ff->Except(buffer_in);
						return;
					}
					if ((u+offset)>(v+offset))
					{
						fclose(file_in);
						sprintf_s(buffer_in,1024,"Error on frame position on line %ld.",line_file);
						ff->Except(buffer_in);
						return;
					}
					debut[nbre_file_mode]=u+offset;
					fin[nbre_file_mode]=v+offset;
					tab_mode[nbre_file_mode]=(uint8_t)w;
					nbre_file_mode++;
				}
			}
		}
		fclose(file_in);

		if (nbre_file_mode==0)
		{
			strcpy_s(buffer_in,1024,"No line in file : ");
			strcat_s(buffer_in,1024,mData.filename);
			ff->Except(buffer_in);
			return;
		}

		for (i=0; i<(nbre_file_mode-1); i++)
		{
			for (j=i+1; j<nbre_file_mode; j++)
			{
				if (((debut[j]>=debut[i]) && (debut[j]<=fin[i])) ||
					((fin[j]>=debut[i]) && (fin[j]<=fin[i])))
				{
					sprintf_s(buffer_in,1024,"Overlap around lines %ld and %ld.",i+1,j+1);
					ff->Except(buffer_in);
					return;
				}
			}
		}
	}

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if ((!mData.file_mode) && mData.mt_mode &&
		((image_data.src_h0>=32) && (image_data.dst_h0>=32))) threads_number=total_cpu;
	else threads_number=1;

	switch (image_data.src_video_mode)
	{
		case VMODE_BMP_RGBA :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,0,0,0,image_data.src_bits_pixel);
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_Y :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,0,0,2,image_data.src_bits_pixel);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,1,0,1,image_data.src_bits_pixel);
			break;
		case VMODE_PLANAR_YUV422 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,1,0,2,image_data.src_bits_pixel);
			break;
		case VMODE_PLANAR_YUV420 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,1,1,2,image_data.src_bits_pixel);
			break;
		case VMODE_PLANAR_YUV411 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,2,0,2,image_data.src_bits_pixel);
			break;
		case VMODE_PLANAR_YUV410 :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,2,2,2,image_data.src_bits_pixel);
			break;
		default :
			threads_number=CreateMTData(threads_number,image_data.src_w0,image_data.src_h0,0,0,0,image_data.src_bits_pixel);
			break;
	}
	
	buff_ok=true;
	for (i=0; i<2; i++)
	{
		switch (image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				buffer[0][i]=(void *)_aligned_malloc(image_data.src_pitch0_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				buffer[0][i]=(void *)_aligned_malloc(image_data.src_pitch0_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				buffer[1][i]=(void *)_aligned_malloc(image_data.src_pitch1_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[1][i]!=NULL);
				buffer[2][i]=(void *)_aligned_malloc(image_data.src_pitch2_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[2][i]!=NULL);
				break;
		}
	}
	if (!buff_ok)
	{
		ff->ExceptOutOfMemory();
		return;
	}

	if (mData.file_mode)
	{
		buffer0=(void *)_aligned_malloc(image_data.src_size0_al,ALIGN_SIZE);
		buffer0_pitch=image_data.src_pitch0_al;
		buffer0_modulo=image_data.src_modulo0_al;
		if (image_data.src_size1!=0)
		{
			buffer1=(void *)_aligned_malloc(image_data.src_size1_al,ALIGN_SIZE);
			buffer1_pitch=image_data.src_pitch1_al;
			buffer1_modulo=image_data.src_modulo1_al;
		}
		else buffer1=NULL;
		if (image_data.src_size2!=0)
		{
			buffer2=(void *)_aligned_malloc(image_data.src_size2_al,ALIGN_SIZE);
			buffer2_pitch=image_data.src_pitch2_al;
			buffer2_modulo=image_data.src_modulo2_al;
		}
		else buffer2=NULL;
		buff_ok=(buffer0!=NULL);
		if (image_data.src_size1!=0) buff_ok=buff_ok && (buffer1!=NULL);
		if (image_data.src_size2!=0) buff_ok=buff_ok && (buffer2!=NULL);
		if (!buff_ok)
		{
			ff->ExceptOutOfMemory();
			return;
		}
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
	

void JPSDR_Deinterlace::End()
{
	int16_t i,j;

	my_aligned_free(buffer2);
	my_aligned_free(buffer1);
	my_aligned_free(buffer0);
	for (i=2; i>=0; i--)
	{
		for (j=1; j>=0; j--)
		{
			my_aligned_free(buffer[i][j]);
		}
	}
}


static void Blend_Tri_RGB32(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint8_t)((((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint8_t)((((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint8_t)((((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst[j].alpha=src2[j].alpha;
	}
}


static void Blend_Tri_RGB64(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB64BMP *src1,*src2,*src3;
	RGB64BMP *dst;

	src1=(RGB64BMP *)src;
	src2=(RGB64BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB64BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB64BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint16_t)(((uint32_t)src1[j].r+(uint32_t)src2[j].r+1)>>1);
		dst[j].g=(uint16_t)(((uint32_t)src1[j].g+(uint32_t)src2[j].g+1)>>1);
		dst[j].b=(uint16_t)(((uint32_t)src1[j].b+(uint32_t)src2[j].b+1)>>1);
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint16_t)((((uint32_t)src1[j].r+(uint32_t)src3[j].r+(((uint32_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint16_t)((((uint32_t)src1[j].g+(uint32_t)src3[j].g+(((uint32_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint16_t)((((uint32_t)src1[j].b+(uint32_t)src3[j].b+(((uint32_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB64BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB64BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB64BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint16_t)(((uint32_t)src1[j].r+(uint32_t)src2[j].r+1)>>1);
		dst[j].g=(uint16_t)(((uint32_t)src1[j].g+(uint32_t)src2[j].g+1)>>1);
		dst[j].b=(uint16_t)(((uint32_t)src1[j].b+(uint32_t)src2[j].b+1)>>1);
		dst[j].alpha=src2[j].alpha;
	}
}


static void Blend_Tri_RGB32a(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint8_t)((((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint8_t)((((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint8_t)((((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_RGB64a(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB64BMP *src1,*src2,*src3;
	RGB64BMP *dst;

	src1=(RGB64BMP *)src;
	src2=(RGB64BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB64BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB64BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint16_t)(((uint32_t)src1[j].r+(uint32_t)src2[j].r+1)>>1);
		dst[j].g=(uint16_t)(((uint32_t)src1[j].g+(uint32_t)src2[j].g+1)>>1);
		dst[j].b=(uint16_t)(((uint32_t)src1[j].b+(uint32_t)src2[j].b+1)>>1);
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint16_t)((((uint32_t)src1[j].r+(uint32_t)src3[j].r+(((uint32_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint16_t)((((uint32_t)src1[j].g+(uint32_t)src3[j].g+(((uint32_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint16_t)((((uint32_t)src1[j].b+(uint32_t)src3[j].b+(((uint32_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB64BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB64BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB64BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_RGB32b(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint8_t)((((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint8_t)((((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint8_t)((((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_RGB64b(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB64BMP *src1,*src2,*src3;
	RGB64BMP *dst;

	src1=(RGB64BMP *)src;
	src2=(RGB64BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB64BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB64BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint16_t)((((uint32_t)src1[j].r+(uint32_t)src3[j].r+(((uint32_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint16_t)((((uint32_t)src1[j].g+(uint32_t)src3[j].g+(((uint32_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint16_t)((((uint32_t)src1[j].b+(uint32_t)src3[j].b+(((uint32_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB64BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB64BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB64BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_RGB32c(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint8_t)((((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint8_t)((((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint8_t)((((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst[j].alpha=src2[j].alpha;
	}
}


static void Blend_Tri_RGB64c(const void *src,void *_dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB64BMP *src1,*src2,*src3;
	RGB64BMP *dst;

	src1=(RGB64BMP *)src;
	src2=(RGB64BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB64BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB64BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint16_t)((((uint32_t)src1[j].r+(uint32_t)src3[j].r+(((uint32_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint16_t)((((uint32_t)src1[j].g+(uint32_t)src3[j].g+(((uint32_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint16_t)((((uint32_t)src1[j].b+(uint32_t)src3[j].b+(((uint32_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB64BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB64BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB64BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint16_t)(((uint32_t)src1[j].r+(uint32_t)src2[j].r+1)>>1);
		dst[j].g=(uint16_t)(((uint32_t)src1[j].g+(uint32_t)src2[j].g+1)>>1);
		dst[j].b=(uint16_t)(((uint32_t)src1[j].b+(uint32_t)src2[j].b+1)>>1);
		dst[j].alpha=src2[j].alpha;
	}
}


static void Blend_Tri_RGB32(const void *src,void *_dst,void *buffer[],const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;
	uint8_t indice_buffer;

	src2=(RGB32BMP *)((uint8_t *)src+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	memcpy(buffer[0],src,w << 2);
	indice_buffer=1;

	src1=(RGB32BMP *)buffer[0];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 2);
		indice_buffer=(indice_buffer+1)%2;
		src1=(RGB32BMP *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint8_t)((((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint8_t)((((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint8_t)((((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(RGB32BMP *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst[j].alpha=src2[j].alpha;
	}
}


static void Blend_Tri_RGB64(const void *src,void *_dst,void *buffer[],const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB64BMP *src1,*src2,*src3;
	RGB64BMP *dst;
	uint8_t indice_buffer;

	src2=(RGB64BMP *)((uint8_t *)src+src_pitch);
	src3=(RGB64BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB64BMP *)_dst;

	memcpy(buffer[0],src,w << 3);
	indice_buffer=1;

	src1=(RGB64BMP *)buffer[0];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint16_t)(((uint32_t)src1[j].r+(uint32_t)src2[j].r+1)>>1);
		dst[j].g=(uint16_t)(((uint32_t)src1[j].g+(uint32_t)src2[j].g+1)>>1);
		dst[j].b=(uint16_t)(((uint32_t)src1[j].b+(uint32_t)src2[j].b+1)>>1);
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 3);
		indice_buffer=(indice_buffer+1)%2;
		src1=(RGB64BMP *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(uint16_t)((((uint32_t)src1[j].r+(uint32_t)src3[j].r+(((uint32_t)src2[j].r) << 1))+2) >> 2);
			dst[j].g=(uint16_t)((((uint32_t)src1[j].g+(uint32_t)src3[j].g+(((uint32_t)src2[j].g) << 1))+2) >> 2);
			dst[j].b=(uint16_t)((((uint32_t)src1[j].b+(uint32_t)src3[j].b+(((uint32_t)src2[j].b) << 1))+2) >> 2);
			dst[j].alpha=src2[j].alpha;
		}
		src2=(RGB64BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB64BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB64BMP *)((uint8_t *)dst+dst_pitch);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(RGB64BMP *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=(uint16_t)(((uint32_t)src1[j].r+(uint32_t)src2[j].r+1)>>1);
		dst[j].g=(uint16_t)(((uint32_t)src1[j].g+(uint32_t)src2[j].g+1)>>1);
		dst[j].b=(uint16_t)(((uint32_t)src1[j].b+(uint32_t)src2[j].b+1)>>1);
		dst[j].alpha=src2[j].alpha;
	}
}


static void Blend_Tri_YUV32(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}
	dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}
}


static void Blend_Tri_YUV64(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV16 *src1,*src2,*src3;
	YUYV16 *dst;

	src1=(YUYV16 *)src;
	src2=(YUYV16 *)((uint8_t *)src1+src_pitch);
	src3=(YUYV16 *)((uint8_t *)src2+src_pitch);
	dst=(YUYV16 *)(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint16_t)(((uint32_t)src1[j].y1+(uint32_t)src2[j].y1+1)>>1);
		dst[j].u=(uint16_t)(((uint32_t)src1[j].u+(uint32_t)src2[j].u+1)>>1);
		dst[j].y2=(uint16_t)(((uint32_t)src1[j].y2+(uint32_t)src2[j].y2+1)>>1);
		dst[j].v=(uint16_t)(((uint32_t)src1[j].v+(uint32_t)src2[j].v+1)>>1);
	}
	dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint16_t)((((uint32_t)src1[j].y1+(uint32_t)src3[j].y1+(((uint32_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint16_t)((((uint32_t)src1[j].u+(uint32_t)src3[j].u+(((uint32_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint16_t)((((uint32_t)src1[j].y2+(uint32_t)src3[j].y2+(((uint32_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint16_t)((((uint32_t)src1[j].v+(uint32_t)src3[j].v+(((uint32_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV16 *)((uint8_t *)src1+src_pitch);
		src2=(YUYV16 *)((uint8_t *)src2+src_pitch);
		src3=(YUYV16 *)((uint8_t *)src3+src_pitch);
		dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint16_t)(((uint32_t)src1[j].y1+(uint32_t)src2[j].y1+1)>>1);
		dst[j].u=(uint16_t)(((uint32_t)src1[j].u+(uint32_t)src2[j].u+1)>>1);
		dst[j].y2=(uint16_t)(((uint32_t)src1[j].y2+(uint32_t)src2[j].y2+1)>>1);
		dst[j].v=(uint16_t)(((uint32_t)src1[j].v+(uint32_t)src2[j].v+1)>>1);
	}
}


static void Blend_Tri_YUV32a(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}
	dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_YUV64a(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV16 *src1,*src2,*src3;
	YUYV16 *dst;

	src1=(YUYV16 *)src;
	src2=(YUYV16 *)((uint8_t *)src1+src_pitch);
	src3=(YUYV16 *)((uint8_t *)src2+src_pitch);
	dst=(YUYV16 *)(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint16_t)(((uint32_t)src1[j].y1+(uint32_t)src2[j].y1+1)>>1);
		dst[j].u=(uint16_t)(((uint32_t)src1[j].u+(uint32_t)src2[j].u+1)>>1);
		dst[j].y2=(uint16_t)(((uint32_t)src1[j].y2+(uint32_t)src2[j].y2+1)>>1);
		dst[j].v=(uint16_t)(((uint32_t)src1[j].v+(uint32_t)src2[j].v+1)>>1);
	}
	dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint16_t)((((uint32_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint32_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint16_t)((((uint32_t)src1[j].u+(uint16_t)src3[j].u+(((uint32_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint16_t)((((uint32_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint32_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint16_t)((((uint32_t)src1[j].v+(uint16_t)src3[j].v+(((uint32_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV16 *)((uint8_t *)src1+src_pitch);
		src2=(YUYV16 *)((uint8_t *)src2+src_pitch);
		src3=(YUYV16 *)((uint8_t *)src3+src_pitch);
		dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_YUV32b(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_YUV64b(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV16 *src1,*src2,*src3;
	YUYV16 *dst;

	src1=(YUYV16 *)src;
	src2=(YUYV16 *)((uint8_t *)src1+src_pitch);
	src3=(YUYV16 *)((uint8_t *)src2+src_pitch);
	dst=(YUYV16 *)(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint16_t)((((uint32_t)src1[j].y1+(uint32_t)src3[j].y1+(((uint32_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint16_t)((((uint32_t)src1[j].u+(uint32_t)src3[j].u+(((uint32_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint16_t)((((uint32_t)src1[j].y2+(uint32_t)src3[j].y2+(((uint32_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint16_t)((((uint32_t)src1[j].v+(uint32_t)src3[j].v+(((uint32_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV16 *)((uint8_t *)src1+src_pitch);
		src2=(YUYV16 *)((uint8_t *)src2+src_pitch);
		src3=(YUYV16 *)((uint8_t *)src3+src_pitch);
		dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	}
}


static void Blend_Tri_YUV32c(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}
}


static void Blend_Tri_YUV64c(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV16 *src1,*src2,*src3;
	YUYV16 *dst;

	src1=(YUYV16 *)src;
	src2=(YUYV16 *)((uint8_t *)src1+src_pitch);
	src3=(YUYV16 *)((uint8_t *)src2+src_pitch);
	dst=(YUYV16 *)(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint16_t)((((uint32_t)src1[j].y1+(uint32_t)src3[j].y1+(((uint32_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint16_t)((((uint32_t)src1[j].u+(uint32_t)src3[j].u+(((uint32_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint16_t)((((uint32_t)src1[j].y2+(uint32_t)src3[j].y2+(((uint32_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint16_t)((((uint32_t)src1[j].v+(uint32_t)src3[j].v+(((uint32_t)src2[j].v) << 1))+2) >> 2);
		}
		src1=(YUYV16 *)((uint8_t *)src1+src_pitch);
		src2=(YUYV16 *)((uint8_t *)src2+src_pitch);
		src3=(YUYV16 *)((uint8_t *)src3+src_pitch);
		dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint16_t)(((uint32_t)src1[j].y1+(uint32_t)src2[j].y1+1)>>1);
		dst[j].u=(uint16_t)(((uint32_t)src1[j].u+(uint32_t)src2[j].u+1)>>1);
		dst[j].y2=(uint16_t)(((uint32_t)src1[j].y2+(uint32_t)src2[j].y2+1)>>1);
		dst[j].v=(uint16_t)(((uint32_t)src1[j].v+(uint32_t)src2[j].v+1)>>1);
	}
}


static void Blend_Tri_YUV32(const void *src,void *_dst,void *buffer[],const int32_t w,
	const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;
	uint8_t indice_buffer;

	src2=(YUYV *)((uint8_t *)src+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)_dst;
	memcpy(buffer[0],src,w << 2);
	indice_buffer=1;

	src1=(YUYV *)buffer[0];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}
	dst=(YUYV *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 2);
		indice_buffer=(indice_buffer+1)%2;
		src1=(YUYV *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2);
		}
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(YUYV *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}
}


static void Blend_Tri_YUV64(const void *src,void *_dst,void *buffer[],const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV16 *src1,*src2,*src3;
	YUYV16 *dst;
	uint8_t indice_buffer;

	src2=(YUYV16 *)((uint8_t *)src+src_pitch);
	src3=(YUYV16 *)((uint8_t *)src2+src_pitch);
	dst=(YUYV16 *)_dst;
	memcpy(buffer[0],src,w << 3);
	indice_buffer=1;

	src1=(YUYV16 *)buffer[0];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint16_t)(((uint32_t)src1[j].y1+(uint32_t)src2[j].y1+1)>>1);
		dst[j].u=(uint16_t)(((uint32_t)src1[j].u+(uint32_t)src2[j].u+1)>>1);
		dst[j].y2=(uint16_t)(((uint32_t)src1[j].y2+(uint32_t)src2[j].y2+1)>>1);
		dst[j].v=(uint16_t)(((uint32_t)src1[j].v+(uint32_t)src2[j].v+1)>>1);
	}
	dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 3);
		indice_buffer=(indice_buffer+1)%2;
		src1=(YUYV16 *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(uint16_t)((((uint32_t)src1[j].y1+(uint32_t)src3[j].y1+(((uint32_t)src2[j].y1) << 1))+2) >> 2);
			dst[j].u=(uint16_t)((((uint32_t)src1[j].u+(uint32_t)src3[j].u+(((uint32_t)src2[j].u) << 1))+2) >> 2);
			dst[j].y2=(uint16_t)((((uint32_t)src1[j].y2+(uint32_t)src3[j].y2+(((uint32_t)src2[j].y2) << 1))+2) >> 2);
			dst[j].v=(uint16_t)((((uint32_t)src1[j].v+(uint32_t)src3[j].v+(((uint32_t)src2[j].v) << 1))+2) >> 2);
		}
		src2=(YUYV16 *)((uint8_t *)src2+src_pitch);
		src3=(YUYV16 *)((uint8_t *)src3+src_pitch);
		dst=(YUYV16 *)((uint8_t *)dst+dst_pitch);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(YUYV16 *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=(uint16_t)(((uint32_t)src1[j].y1+(uint32_t)src2[j].y1+1)>>1);
		dst[j].u=(uint16_t)(((uint32_t)src1[j].u+(uint32_t)src2[j].u+1)>>1);
		dst[j].y2=(uint16_t)(((uint32_t)src1[j].y2+(uint32_t)src2[j].y2+1)>>1);
		dst[j].v=(uint16_t)(((uint32_t)src1[j].v+(uint32_t)src2[j].v+1)>>1);
	}
}


static void Blend_Tri_Planar8(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
	dst+=dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
			dst[j]=(uint8_t)((((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2);

		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
}


static void Blend_Tri_Planar16(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1_,*src2_,*src3_;
	uint8_t *dst_;

	src1_=(uint8_t *)src;
	src2_=src1_+src_pitch;
	src3_=src2_+src_pitch;
	dst_=(uint8_t *)_dst;

	const uint16_t *src1=(const uint16_t *)src1_,*src2=(const uint16_t *)src2_;
	uint16_t *dst=(uint16_t *)dst_;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint16_t)(((uint32_t)src1[j]+(uint32_t)src2[j]+1)>>1);
	dst_+=dst_pitch;

	const uint16_t *src3;

	for (int32_t i=0; i<h; i++)
	{
		src1=(const uint16_t *)src1_;
		src2=(const uint16_t *)src2_;
		src3=(const uint16_t *)src3_;
		dst=(uint16_t *)dst_;

		for (int32_t j=0; j<w; j++)
			dst[j]=(uint16_t)((((uint32_t)src1[j]+(uint32_t)src3[j]+(((uint32_t)src2[j]) << 1))+2) >> 2);

		src1_+=src_pitch;
		src2_+=src_pitch;
		src3_+=src_pitch;
		dst_+=dst_pitch;
	}

	src1=(const uint16_t *)src1_;
	src2=(const uint16_t *)src2_;
	dst=(uint16_t *)dst_;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint16_t)(((uint32_t)src1[j]+(uint32_t)src2[j]+1)>>1);
}


static void Blend_Tri_Planar8a(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
	dst+=dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
			dst[j]=(uint8_t)((((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2);

		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}
}


static void Blend_Tri_Planar16a(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1_,*src2_,*src3_;
	uint8_t *dst_;

	src1_=(uint8_t *)src;
	src2_=src1_+src_pitch;
	src3_=src2_+src_pitch;
	dst_=(uint8_t *)_dst;

	const uint16_t *src1=(const uint16_t *)src1_,*src2=(const uint16_t *)src2_;
	uint16_t *dst=(uint16_t *)dst_;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint16_t)(((uint32_t)src1[j]+(uint32_t)src2[j]+1)>>1);
	dst_+=dst_pitch;

	const uint16_t *src3;

	for (int32_t i=0; i<h; i++)
	{
		src1=(const uint16_t *)src1_;
		src2=(const uint16_t *)src2_;
		src3=(const uint16_t *)src3_;
		dst=(uint16_t *)dst_;

		for (int32_t j=0; j<w; j++)
			dst[j]=(uint16_t)((((uint32_t)src1[j]+(uint32_t)src3[j]+(((uint32_t)src2[j]) << 1))+2) >> 2);

		src1_+=src_pitch;
		src2_+=src_pitch;
		src3_+=src_pitch;
		dst_+=dst_pitch;
	}
}


static void Blend_Tri_Planar8b(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
			dst[j]=(uint8_t)((((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2);

		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

}


static void Blend_Tri_Planar16b(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1_,*src2_,*src3_;
	uint8_t *dst_;

	src1_=(uint8_t *)src;
	src2_=src1_+src_pitch;
	src3_=src2_+src_pitch;
	dst_=(uint8_t *)_dst;

	const uint16_t *src1,*src2,*src3;
	uint16_t *dst;

	for (int32_t i=0; i<h; i++)
	{
		src1=(const uint16_t *)src1_;
		src2=(const uint16_t *)src2_;
		src3=(const uint16_t *)src3_;
		dst=(uint16_t *)dst_;

		for (int32_t j=0; j<w; j++)
			dst[j]=(uint16_t)((((uint32_t)src1[j]+(uint32_t)src3[j]+(((uint32_t)src2[j]) << 1))+2) >> 2);

		src1_+=src_pitch;
		src2_+=src_pitch;
		src3_+=src_pitch;
		dst_+=dst_pitch;
	}
}


static void Blend_Tri_Planar8c(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
			dst[j]=(uint8_t)((((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2);

		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
}


static void Blend_Tri_Planar16c(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1_,*src2_,*src3_;
	uint8_t *dst_;

	src1_=(uint8_t *)src;
	src2_=src1_+src_pitch;
	src3_=src2_+src_pitch;
	dst_=(uint8_t *)_dst;

	const uint16_t *src1,*src2,*src3;
	uint16_t *dst;

	for (int32_t i=0; i<h; i++)
	{
		src1=(const uint16_t *)src1_;
		src2=(const uint16_t *)src2_;
		src3=(const uint16_t *)src3_;
		dst=(uint16_t *)dst_;

		for (int32_t j=0; j<w; j++)
			dst[j]=(uint16_t)((((uint32_t)src1[j]+(uint32_t)src3[j]+(((uint32_t)src2[j]) << 1))+2) >> 2);

		src1_+=src_pitch;
		src2_+=src_pitch;
		src3_+=src_pitch;
		dst_+=dst_pitch;
	}

	src1=(const uint16_t *)src1_;
	src2=(const uint16_t *)src2_;
	dst=(uint16_t *)dst_;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint16_t)(((uint32_t)src1[j]+(uint32_t)src2[j]+1)>>1);
}


static void Blend_Tri_Planar8(const void *src,void *_dst,void *buffer[],const int32_t w,
	const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	uint8_t *src1,*src2,*src3,*dst;
	uint8_t indice_buffer;

	src2=((uint8_t *)src)+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;
	memcpy(buffer[0],src,w);
	indice_buffer=1;

	src1=(uint8_t *)buffer[0];

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);

	dst+=dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w);
		indice_buffer=(indice_buffer+1)%2;
		src1=(uint8_t *)buffer[indice_buffer];

		for (int32_t j=0; j<w; j++)
			dst[j]=(uint8_t)((((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2);

		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(uint8_t *)buffer[indice_buffer];

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
}


static void Blend_Tri_Planar16(const void *src,void *_dst,void *buffer[],const int32_t w,
	const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	uint8_t *src1_,*src2_,*src3_,*dst_;
	uint8_t indice_buffer;

	src2_=((uint8_t *)src)+src_pitch;
	src3_=src2_+src_pitch;
	dst_=(uint8_t *)_dst;
	memcpy(buffer[0],src,w);
	indice_buffer=1;

	src1_=(uint8_t *)buffer[0];

	const uint16_t *src1=(const uint16_t *)src1_,*src2=(const uint16_t *)src2_;
	uint16_t *dst=(uint16_t *)dst_;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint16_t)(((uint32_t)src1[j]+(uint32_t)src2[j]+1)>>1);

	dst_+=dst_pitch;

	const uint16_t *src3;

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2_,w);
		indice_buffer=(indice_buffer+1)%2;
		src1_=(uint8_t *)buffer[indice_buffer];

		src1=(const uint16_t *)src1_;
		src2=(const uint16_t *)src2_;
		src3=(const uint16_t *)src3_;
		dst=(uint16_t *)dst_;

		for (int32_t j=0; j<w; j++)
			dst[j]=(uint16_t)((((uint32_t)src1[j]+(uint32_t)src3[j]+(((uint32_t)src2[j]) << 1))+2) >> 2);

		src2_+=src_pitch;
		src3_+=src_pitch;
		dst_+=dst_pitch;
	}

	indice_buffer=(indice_buffer+1)%2;
	src1_=(uint8_t *)buffer[indice_buffer];
	src1=(const uint16_t *)src1_;
	src2=(const uint16_t *)src2_;
	dst=(uint16_t *)dst_;

	for (int32_t j=0; j<w; j++)
		dst[j]=(uint16_t)(((uint32_t)src1[j]+(uint32_t)src2[j]+1)>>1);
}


void JPSDR_Deinterlace::Deinterlace_Blend_8_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_8_AVX2(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_8_AVX(mt_data_inf.src1_0,mt_data_inf.src1_1,
				mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_8_SSE2(mt_data_inf.src1_0,mt_data_inf.src1_1,
					mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else JPSDR_Deinterlace_Blend_24(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
				mt_data_inf.src_Y_w_32,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
					(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
			else
				memcpy(mt_data_inf.dst1,(uint8_t *)mt_data_inf.dst1+mt_data_inf.dst_pitch1,
					mt_data_inf.src_Y_w_32 << 2);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
				(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_16_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_16_AVX2(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_16_AVX(mt_data_inf.src1_0,mt_data_inf.src1_1,
				mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_16_SSE2(mt_data_inf.src1_0,mt_data_inf.src1_1,
					mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else JPSDR_Deinterlace_Blend_48(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
				mt_data_inf.src_Y_w_32>>1,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
					(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
			else
				memcpy(mt_data_inf.dst1,(uint8_t *)mt_data_inf.dst1+mt_data_inf.dst_pitch1,
					mt_data_inf.src_Y_w_32 << 2);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
				(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_8_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_8_AVX2(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_8_AVX(mt_data_inf.src1_0,mt_data_inf.src1_1,
				mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_8_SSE2(mt_data_inf.src1_0,mt_data_inf.src1_1,
					mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else JPSDR_Deinterlace_Blend_32(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
				mt_data_inf.src_Y_w_32,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
					(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
			else
				memcpy(mt_data_inf.dst1,(uint8_t *)mt_data_inf.dst1+mt_data_inf.dst_pitch1,
					mt_data_inf.src_Y_w_32 << 2);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
				(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_16_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_16_AVX2(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_16_AVX(mt_data_inf.src1_0,mt_data_inf.src1_1,
				mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_16_SSE2(mt_data_inf.src1_0,mt_data_inf.src1_1,
					mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else JPSDR_Deinterlace_Blend_64(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
				mt_data_inf.src_Y_w_32>>1,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
					(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
			else
				memcpy(mt_data_inf.dst1,(uint8_t *)mt_data_inf.dst1+mt_data_inf.dst_pitch1,
					mt_data_inf.src_Y_w_32 << 2);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
				(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w_32 << 2);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_8_MT_3(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_8_AVX2(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_8_AVX(mt_data_inf.src1_0,mt_data_inf.src1_1,
				mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_8_SSE2(mt_data_inf.src1_0,mt_data_inf.src1_1,
					mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else JPSDR_Deinterlace_Blend_8(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
				mt_data_inf.src_Y_w,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
					(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w);
			else
				memcpy(mt_data_inf.dst1,(uint8_t *)mt_data_inf.dst1+mt_data_inf.dst_pitch1,
					mt_data_inf.src_Y_w);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
				(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_16_MT_3(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_16_AVX2(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_16_AVX(mt_data_inf.src1_0,mt_data_inf.src1_1,
				mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_16_SSE2(mt_data_inf.src1_0,mt_data_inf.src1_1,
					mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else JPSDR_Deinterlace_Blend_16(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
				mt_data_inf.src_Y_w,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
					(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w);
			else
				memcpy(mt_data_inf.dst1,(uint8_t *)mt_data_inf.dst1+mt_data_inf.dst_pitch1,
					mt_data_inf.src_Y_w);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*h),
				(uint8_t *)mt_data_inf.dst1+(mt_data_inf.dst_pitch1*(h-1)),mt_data_inf.src_Y_w);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_8_MT_4(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_8_AVX2(mt_data_inf.src2_0,mt_data_inf.src2_1,
			mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_8_AVX(mt_data_inf.src2_0,mt_data_inf.src2_1,
				mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_8_SSE2(mt_data_inf.src2_0,mt_data_inf.src2_1,
					mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else JPSDR_Deinterlace_Blend_8(mt_data_inf.src2_0,mt_data_inf.src2_1,mt_data_inf.dst2,
				mt_data_inf.src_UV_w,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*h),
					(uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*(h-1)),mt_data_inf.src_UV_w);
			else
				memcpy(mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst2+mt_data_inf.dst_pitch2,
					mt_data_inf.src_UV_w);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*h),
				(uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*(h-1)),mt_data_inf.src_UV_w);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_16_MT_4(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_16_AVX2(mt_data_inf.src2_0,mt_data_inf.src2_1,
			mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_16_AVX(mt_data_inf.src2_0,mt_data_inf.src2_1,
				mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_16_SSE2(mt_data_inf.src2_0,mt_data_inf.src2_1,
					mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else JPSDR_Deinterlace_Blend_16(mt_data_inf.src2_0,mt_data_inf.src2_1,mt_data_inf.dst2,
				mt_data_inf.src_UV_w,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*h),
					(uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*(h-1)),mt_data_inf.src_UV_w);
			else
				memcpy(mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst2+mt_data_inf.dst_pitch2,
					mt_data_inf.src_UV_w);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*h),
				(uint8_t *)mt_data_inf.dst2+(mt_data_inf.dst_pitch2*(h-1)),mt_data_inf.src_UV_w);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_8_MT_5(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_8_AVX2(mt_data_inf.src3_0,mt_data_inf.src3_1,
			mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_8_AVX(mt_data_inf.src3_0,mt_data_inf.src3_1,
				mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_8_SSE2(mt_data_inf.src3_0,mt_data_inf.src3_1,
					mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else JPSDR_Deinterlace_Blend_8(mt_data_inf.src3_0,mt_data_inf.src3_1,mt_data_inf.dst3,
				mt_data_inf.src_UV_w,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*h),
					(uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*(h-1)),mt_data_inf.src_UV_w);
			else
				memcpy(mt_data_inf.dst3,(uint8_t *)mt_data_inf.dst3+mt_data_inf.dst_pitch3,
					mt_data_inf.src_UV_w);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*h),
				(uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*(h-1)),mt_data_inf.src_UV_w);
	}
}


void JPSDR_Deinterlace::Deinterlace_Blend_16_MT_5(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.div_h)
	{
		h>>=1;
		if (mt_data_inf.dec_h) h--;
	}
	else
	{
		if (mt_data_inf.bottom) h--;
	}

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
		JPSDR_Deinterlace_Blend_16_AVX2(mt_data_inf.src3_0,mt_data_inf.src3_1,
			mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
	else
#endif
	{
		if (AVX_Enable)
			JPSDR_Deinterlace_Blend_16_AVX(mt_data_inf.src3_0,mt_data_inf.src3_1,
				mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		else
		{
			if (SSE2_Enable)
				JPSDR_Deinterlace_Blend_16_SSE2(mt_data_inf.src3_0,mt_data_inf.src3_1,
					mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else JPSDR_Deinterlace_Blend_16(mt_data_inf.src3_0,mt_data_inf.src3_1,mt_data_inf.dst3,
				mt_data_inf.src_UV_w,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		}
	}

	if (mt_data_inf.div_h)
	{
		if (mt_data_inf.dec_h)
		{
			if (mt_data_inf.bottom)
				memcpy((uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*h),
					(uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*(h-1)),mt_data_inf.src_UV_w);
			else
				memcpy(mt_data_inf.dst3,(uint8_t *)mt_data_inf.dst3+mt_data_inf.dst_pitch3,
					mt_data_inf.src_UV_w);
		}
	}
	else
	{
		if (mt_data_inf.bottom)
			memcpy((uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*h),
				(uint8_t *)mt_data_inf.dst3+(mt_data_inf.dst_pitch3*(h-1)),mt_data_inf.src_UV_w);
	}
}


void JPSDR_Deinterlace::Deinterlace_8_Tri_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_8_Tri_AVX2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_8_Tri_AVX_a(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_8_Tri_AVX_c(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
					JPSDR_Deinterlace_Blend_8_Tri_AVX_b(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_8_Tri_SSE2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_RGB32a(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,h,
						mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_RGB32c(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						Blend_Tri_RGB32b(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_16_Tri_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_16_Tri_AVX2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_16_Tri_AVX_a(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_16_Tri_AVX_c(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
					JPSDR_Deinterlace_Blend_16_Tri_AVX_b(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_16_Tri_SSE2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_RGB64a(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32>>1,h,
						mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_RGB64c(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32>>1,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						Blend_Tri_RGB64b(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32>>1,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_8_Tri_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_8_Tri_AVX2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_8_Tri_AVX_a(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_8_Tri_AVX_c(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
					JPSDR_Deinterlace_Blend_8_Tri_AVX_b(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_8_Tri_SSE2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_YUV32a(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,h,
						mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_YUV32c(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						Blend_Tri_YUV32b(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_16_Tri_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_16_Tri_AVX2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_16_Tri_AVX_a(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_16_Tri_AVX_c(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
					JPSDR_Deinterlace_Blend_16_Tri_AVX_b(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_16_Tri_SSE2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_YUV64a(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32>>1,h,
						mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_YUV64c(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32>>1,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						Blend_Tri_YUV64b(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32>>1,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_8_Tri_MT_3(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_8_Tri_AVX2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_8_Tri_AVX_a(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_8_Tri_AVX_c(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
					JPSDR_Deinterlace_Blend_8_Tri_AVX_b(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_8_Tri_SSE2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_Planar8a(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w,h,
						mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_Planar8c(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						Blend_Tri_Planar8b(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_16_Tri_MT_3(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_16_Tri_AVX2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+7)>>3,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_16_Tri_AVX_a(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_16_Tri_AVX_c(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
					JPSDR_Deinterlace_Blend_16_Tri_AVX_b(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_16_Tri_SSE2_a(mt_data_inf.src1_0,mt_data_inf.dst1,
						(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_c(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_b(mt_data_inf.src1_0,mt_data_inf.dst1,
							(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_Planar16a(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w,h,
						mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_Planar16c(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
					else
						Blend_Tri_Planar16b(mt_data_inf.src1_0,mt_data_inf.dst1,mt_data_inf.src_Y_w,h,
							mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_8_Tri_MT_4(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_8_Tri_AVX2_a(mt_data_inf.src2_0,mt_data_inf.dst2,
				(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_c(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_b(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_8_Tri_AVX_a(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_8_Tri_AVX_c(mt_data_inf.src2_0,mt_data_inf.dst2,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				else
					JPSDR_Deinterlace_Blend_8_Tri_AVX_b(mt_data_inf.src2_0,mt_data_inf.dst2,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_8_Tri_SSE2_a(mt_data_inf.src2_0,mt_data_inf.dst2,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_c(mt_data_inf.src2_0,mt_data_inf.dst2,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
					else
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_b(mt_data_inf.src2_0,mt_data_inf.dst2,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_Planar8a(mt_data_inf.src2_0,mt_data_inf.dst2,mt_data_inf.src_UV_w,h,
						mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_Planar8c(mt_data_inf.src2_0,mt_data_inf.dst2,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
					else
						Blend_Tri_Planar8b(mt_data_inf.src2_0,mt_data_inf.dst2,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_16_Tri_MT_4(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_16_Tri_AVX2_a(mt_data_inf.src2_0,mt_data_inf.dst2,
				(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_c(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_b(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_16_Tri_AVX_a(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_16_Tri_AVX_c(mt_data_inf.src2_0,mt_data_inf.dst2,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				else
					JPSDR_Deinterlace_Blend_16_Tri_AVX_b(mt_data_inf.src2_0,mt_data_inf.dst2,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_16_Tri_SSE2_a(mt_data_inf.src2_0,mt_data_inf.dst2,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_c(mt_data_inf.src2_0,mt_data_inf.dst2,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
					else
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_b(mt_data_inf.src2_0,mt_data_inf.dst2,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_Planar16a(mt_data_inf.src2_0,mt_data_inf.dst2,mt_data_inf.src_UV_w,h,
						mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_Planar16c(mt_data_inf.src2_0,mt_data_inf.dst2,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
					else
						Blend_Tri_Planar16b(mt_data_inf.src2_0,mt_data_inf.dst2,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_8_Tri_MT_5(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_8_Tri_AVX2_a(mt_data_inf.src3_0,mt_data_inf.dst3,
				(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_c(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else
				JPSDR_Deinterlace_Blend_8_Tri_AVX2_b(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_8_Tri_AVX_a(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_8_Tri_AVX_c(mt_data_inf.src3_0,mt_data_inf.dst3,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				else
					JPSDR_Deinterlace_Blend_8_Tri_AVX_b(mt_data_inf.src3_0,mt_data_inf.dst3,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_8_Tri_SSE2_a(mt_data_inf.src3_0,mt_data_inf.dst3,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_c(mt_data_inf.src3_0,mt_data_inf.dst3,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
					else
						JPSDR_Deinterlace_Blend_8_Tri_SSE2_b(mt_data_inf.src3_0,mt_data_inf.dst3,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_Planar8a(mt_data_inf.src3_0,mt_data_inf.dst3,mt_data_inf.src_UV_w,h,
						mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_Planar8c(mt_data_inf.src3_0,mt_data_inf.dst3,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
					else
						Blend_Tri_Planar8b(mt_data_inf.src3_0,mt_data_inf.dst3,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::Deinterlace_16_Tri_MT_5(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

#ifdef AVX2_BUILD_POSSIBLE
	if (AVX2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_16_Tri_AVX2_a(mt_data_inf.src3_0,mt_data_inf.dst3,
				(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_c(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else
				JPSDR_Deinterlace_Blend_16_Tri_AVX2_b(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+7)>>3,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		}
	}
	else
#endif
	{
		if (AVX_Enable)
		{
			if (mt_data_inf.top)
				JPSDR_Deinterlace_Blend_16_Tri_AVX_a(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else
			{
				if (mt_data_inf.bottom)
					JPSDR_Deinterlace_Blend_16_Tri_AVX_c(mt_data_inf.src3_0,mt_data_inf.dst3,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				else
					JPSDR_Deinterlace_Blend_16_Tri_AVX_b(mt_data_inf.src3_0,mt_data_inf.dst3,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			}
		}
		else
		{
			if (SSE2_Enable)
			{
				if (mt_data_inf.top)
					JPSDR_Deinterlace_Blend_16_Tri_SSE2_a(mt_data_inf.src3_0,mt_data_inf.dst3,
						(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				else
				{
					if (mt_data_inf.bottom)
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_c(mt_data_inf.src3_0,mt_data_inf.dst3,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
					else
						JPSDR_Deinterlace_Blend_16_Tri_SSE2_b(mt_data_inf.src3_0,mt_data_inf.dst3,
							(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				}
			}
			else
			{
				if (mt_data_inf.top)
					Blend_Tri_Planar16a(mt_data_inf.src3_0,mt_data_inf.dst3,mt_data_inf.src_UV_w,h,
						mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				else
				{
					if (mt_data_inf.bottom)
						Blend_Tri_Planar16c(mt_data_inf.src3_0,mt_data_inf.dst3,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
					else
						Blend_Tri_Planar16b(mt_data_inf.src3_0,mt_data_inf.dst3,mt_data_inf.src_UV_w,h,
							mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
				}
			}
		}
	}
}


void JPSDR_Deinterlace::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_Deinterlace *ptrClass=(JPSDR_Deinterlace *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->Deinterlace_Blend_8_MT_1(data->thread_Id); break;
		case 2 : ptrClass->Deinterlace_Blend_8_MT_2(data->thread_Id); break;
		case 3 : ptrClass->Deinterlace_Blend_8_MT_3(data->thread_Id); break;
		case 7 : ptrClass->Deinterlace_Blend_8_MT_4(data->thread_Id); break;
		case 8 : ptrClass->Deinterlace_Blend_8_MT_5(data->thread_Id); break;
		case 4 : ptrClass->Deinterlace_8_Tri_MT_1(data->thread_Id); break;
		case 5 : ptrClass->Deinterlace_8_Tri_MT_2(data->thread_Id); break;
		case 6 : ptrClass->Deinterlace_8_Tri_MT_3(data->thread_Id); break;
		case 9 : ptrClass->Deinterlace_8_Tri_MT_4(data->thread_Id); break;
		case 10 : ptrClass->Deinterlace_8_Tri_MT_5(data->thread_Id); break;
		case 11 : ptrClass->Deinterlace_Blend_16_MT_1(data->thread_Id); break;
		case 12 : ptrClass->Deinterlace_Blend_16_MT_2(data->thread_Id); break;
		case 13 : ptrClass->Deinterlace_Blend_16_MT_3(data->thread_Id); break;
		case 17 : ptrClass->Deinterlace_Blend_16_MT_4(data->thread_Id); break;
		case 18 : ptrClass->Deinterlace_Blend_16_MT_5(data->thread_Id); break;
		case 14 : ptrClass->Deinterlace_16_Tri_MT_1(data->thread_Id); break;
		case 15 : ptrClass->Deinterlace_16_Tri_MT_2(data->thread_Id); break;
		case 16 : ptrClass->Deinterlace_16_Tri_MT_3(data->thread_Id); break;
		case 19 : ptrClass->Deinterlace_16_Tri_MT_4(data->thread_Id); break;
		case 20 : ptrClass->Deinterlace_16_Tri_MT_5(data->thread_Id); break;
		default : break;
	}
}



void JPSDR_Deinterlace::Run()
{
	int32_t h0,h1,h2;
	const void *src1_0,*src2_0,*src1_1,*src2_1,*src1_2,*src2_2;
	void *dst0_0,*dst0_1,*dst0_2;
	bool deinterlace,transfert_buffer,swap_buffer;
	uint8_t current_mode;
	uint16_t i;
	uint8_t Offset_MT=0;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXFBitmap& pxsrc2 = *fa->mpSourceFrames[0];
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->mpSourceFrames[0]->mpPixmap;
	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->mpOutputFrames[0]->mpPixmap;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	swap_buffer=(!mData.file_mode) && ((mData.mode==2) ||
		(mData.mt_mode && (mData.mode==1)));
	current_mode=mData.mode;

	transfert_buffer=false;
	if (mData.file_mode)
	{
		deinterlace=false;
		i=0;
		while ((i<nbre_file_mode) && (!deinterlace))
		{
			if ( (((pxsrc2.mFrameNumber+1)==debut[i]) && (tab_mode[i]==0)) ||
				(((i+1)<nbre_file_mode) && (((pxsrc2.mFrameNumber+1)==debut[i+1])
				&& (tab_mode[i+1]==0)))  )
				transfert_buffer=true;
			if ((pxsrc2.mFrameNumber>=debut[i]) && (pxsrc2.mFrameNumber<=fin[i]))
			{
				deinterlace=true;
				current_mode=tab_mode[i];
			}
			i++;
		}
	}
	else deinterlace=true;

	if (!deinterlace)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				if (swap_buffer)
				{
					Move_Full(image_data.src_plane0,image_data.dst_plane0,image_data.src_line0,image_data.src_h0,
						image_data.src_pitch0,image_data.dst_pitch0);
				}
				if (transfert_buffer) Move_Full(image_data.src_plane0,buffer0,image_data.src_line0,
					image_data.src_h0,image_data.src_pitch0,buffer0_pitch);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				if (swap_buffer)
				{
					Move_Full(image_data.src_plane0,image_data.dst_plane0,image_data.src_line0,image_data.src_h0,
						image_data.src_pitch0,image_data.dst_pitch0);
					Move_Full(image_data.src_plane1,image_data.dst_plane1,image_data.src_line1,image_data.src_h1,
						image_data.src_pitch1,image_data.dst_pitch1);
					Move_Full(image_data.src_plane2,image_data.dst_plane2,image_data.src_line2,image_data.src_h2,
						image_data.src_pitch2,image_data.dst_pitch2);
				}
				if (transfert_buffer)
				{
					Move_Full(image_data.src_plane0,buffer0,image_data.src_line0,image_data.src_h0,image_data.src_pitch0,buffer0_pitch);
					Move_Full(image_data.src_plane1,buffer1,image_data.src_line1,image_data.src_h1,image_data.src_pitch1,buffer1_pitch);
					Move_Full(image_data.src_plane2,buffer2,image_data.src_line2,image_data.src_h2,image_data.src_pitch2,buffer2_pitch);
				}
				break;
		}
		return;
	}

	if (image_data.src_bits_pixel>8) Offset_MT=10;

	if (threads_number>1)
	{
		for (uint8_t i=0; i<threads_number; i++)
		{
			switch(current_mode)
			{
				case 1 :
					MT_Data[i].src1_0=(void *)((uint8_t *)image_data.src_plane0
						+(image_data.src_pitch0*MT_Data[i].src_Y_h_min));
					MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+image_data.src_pitch0);
					MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0
						+(image_data.dst_pitch0*MT_Data[i].dst_Y_h_min));

					MT_Data[i].src2_0=(void *)((uint8_t *)image_data.src_plane1
						+(image_data.src_pitch1*MT_Data[i].src_UV_h_min));
					MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+image_data.src_pitch1);
					MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1
						+(image_data.dst_pitch1*MT_Data[i].dst_UV_h_min));

					MT_Data[i].src3_0=(void *)((uint8_t *)image_data.src_plane2
						+(image_data.src_pitch2*MT_Data[i].src_UV_h_min));
					MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+image_data.src_pitch2);
					MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2
						+(image_data.dst_pitch2*MT_Data[i].dst_UV_h_min));

					MT_Data[i].src_pitch1=image_data.src_pitch0;
					MT_Data[i].src_pitch2=image_data.src_pitch1;
					MT_Data[i].src_pitch3=image_data.src_pitch2;
					MT_Data[i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[i].dst_pitch2=image_data.dst_pitch1;
					MT_Data[i].dst_pitch3=image_data.dst_pitch2;
					MT_Data[i].div_h=false;
					break;
				case 2 :
					if (MT_Data[i].top)
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)image_data.src_plane0
							+(image_data.src_pitch0*MT_Data[i].src_Y_h_min));
						MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0
							+(image_data.dst_pitch0*MT_Data[i].dst_Y_h_min));

						MT_Data[i].src2_0=(void *)((uint8_t *)image_data.src_plane1
							+(image_data.src_pitch1*MT_Data[i].src_UV_h_min));
						MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1
							+(image_data.dst_pitch1*MT_Data[i].dst_UV_h_min));

						MT_Data[i].src3_0=(void *)((uint8_t *)image_data.src_plane2
							+(image_data.src_pitch2*MT_Data[i].src_UV_h_min));
						MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2
							+(image_data.dst_pitch2*MT_Data[i].dst_UV_h_min));
					}
					else
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)image_data.src_plane0
							+(image_data.src_pitch0*(MT_Data[i].src_Y_h_min-1)));
						MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0
							+(image_data.dst_pitch0*MT_Data[i].dst_Y_h_min));

						MT_Data[i].src2_0=(void *)((uint8_t *)image_data.src_plane1
							+(image_data.src_pitch1*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1
							+(image_data.dst_pitch1*MT_Data[i].dst_UV_h_min));

						MT_Data[i].src3_0=(void *)((uint8_t *)image_data.src_plane2
							+(image_data.src_pitch2*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2
							+(image_data.dst_pitch2*MT_Data[i].dst_UV_h_min));
					}

					MT_Data[i].src_pitch1=image_data.src_pitch0;
					MT_Data[i].src_pitch2=image_data.src_pitch1;
					MT_Data[i].src_pitch3=image_data.src_pitch2;
					MT_Data[i].src_modulo1=image_data.src_modulo0;
					MT_Data[i].src_modulo2=image_data.src_modulo1;
					MT_Data[i].src_modulo3=image_data.src_modulo2;

					MT_Data[i].dst_pitch1=image_data.dst_pitch0;
					MT_Data[i].dst_pitch2=image_data.dst_pitch1;
					MT_Data[i].dst_pitch3=image_data.dst_pitch2;
					MT_Data[i].dst_modulo1=image_data.dst_modulo0;
					MT_Data[i].dst_modulo2=image_data.dst_modulo1;
					MT_Data[i].dst_modulo3=image_data.dst_modulo2;

					MT_Data[i].div_h=false;
					break;
				case 3 :
					if (MT_Data[i].top)
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)image_data.src_plane0
							+(image_data.src_pitch0*(MT_Data[i].src_Y_h_min+1)));
						MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+2*image_data.src_pitch0);
						MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0
							+(image_data.dst_pitch0*(MT_Data[i].dst_Y_h_min+2)));

						MT_Data[i].src2_0=(void *)((uint8_t *)image_data.src_plane1
							+(image_data.src_pitch1*(MT_Data[i].src_UV_h_min+1)));
						MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+2*image_data.src_pitch1);
						MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1
							+(image_data.dst_pitch1*(MT_Data[i].dst_UV_h_min+2)));

						MT_Data[i].src3_0=(void *)((uint8_t *)image_data.src_plane2
							+(image_data.src_pitch2*(MT_Data[i].src_UV_h_min+1)));
						MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+2*image_data.src_pitch2);
						MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2
							+(image_data.dst_pitch2*(MT_Data[i].dst_UV_h_min+2)));

						MT_Data[i].dec_h=true;
					}
					else
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)image_data.src_plane0
							+(image_data.src_pitch0*(MT_Data[i].src_Y_h_min-1)));
						MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+2*image_data.src_pitch0);
						MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0
							+(image_data.dst_pitch0*MT_Data[i].dst_Y_h_min));

						MT_Data[i].src2_0=(void *)((uint8_t *)image_data.src_plane1
							+(image_data.src_pitch1*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+2*image_data.src_pitch1);
						MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1
							+(image_data.dst_pitch1*MT_Data[i].dst_UV_h_min));

						MT_Data[i].src3_0=(void *)((uint8_t *)image_data.src_plane2
							+(image_data.src_pitch2*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+2*image_data.src_pitch2);
						MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2
							+(image_data.dst_pitch2*MT_Data[i].dst_UV_h_min));

						MT_Data[i].dec_h=false;
					}

					MT_Data[i].src_pitch1=image_data.src_pitch0 << 1;
					MT_Data[i].src_pitch2=image_data.src_pitch1 << 1;
					MT_Data[i].src_pitch3=image_data.src_pitch2 << 1;
					MT_Data[i].dst_pitch1=image_data.dst_pitch0 << 1;
					MT_Data[i].dst_pitch2=image_data.dst_pitch1 << 1;
					MT_Data[i].dst_pitch3=image_data.dst_pitch2 << 1;
					MT_Data[i].div_h=true;
					break;
				case 4 :
					MT_Data[i].src1_0=(void *)((uint8_t *)image_data.src_plane0
						+(image_data.src_pitch0*MT_Data[i].src_Y_h_min));
					MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+2*image_data.src_pitch0);
					MT_Data[i].dst1=(void *)((uint8_t *)image_data.dst_plane0
						+(image_data.dst_pitch0*(MT_Data[i].dst_Y_h_min+1)));

					MT_Data[i].src2_0=(void *)((uint8_t *)image_data.src_plane1
						+(image_data.src_pitch1*MT_Data[i].src_UV_h_min));
					MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+2*image_data.src_pitch1);
					MT_Data[i].dst2=(void *)((uint8_t *)image_data.dst_plane1
						+(image_data.dst_pitch1*(MT_Data[i].dst_UV_h_min+1)));

					MT_Data[i].src3_0=(void *)((uint8_t *)image_data.src_plane2
						+(image_data.src_pitch2*MT_Data[i].src_UV_h_min));
					MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+2*image_data.src_pitch2);
					MT_Data[i].dst3=(void *)((uint8_t *)image_data.dst_plane2
						+(image_data.dst_pitch2*(MT_Data[i].dst_UV_h_min+1)));

					if (MT_Data[i].bottom) MT_Data[i].dec_h=true;
					else MT_Data[i].dec_h=false;

					MT_Data[i].src_pitch1=image_data.src_pitch0 << 1;
					MT_Data[i].src_pitch2=image_data.src_pitch1 << 1;
					MT_Data[i].src_pitch3=image_data.src_pitch2 << 1;
					MT_Data[i].dst_pitch1=image_data.dst_pitch0 << 1;
					MT_Data[i].dst_pitch2=image_data.dst_pitch1 << 1;
					MT_Data[i].dst_pitch3=image_data.dst_pitch2 << 1;
					MT_Data[i].div_h=true;
					break;
			}
		}

		uint8_t f_proc=0;

		switch(current_mode)
		{
			case 1 :
			case 3 :
			case 4 :
				switch(image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
						f_proc=1;
						break;
					case VMODE_YUYV :
					case VMODE_UYVY :
						f_proc=2;
						break;
					case VMODE_Y :
						f_proc=3;
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						f_proc=7;
						break;
					default : ;
				}
				break;
			case 2 :
				switch(image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
						f_proc=4;
						break;
					case VMODE_YUYV :
					case VMODE_UYVY :
						f_proc=5;
						break;
					case VMODE_Y :
						f_proc=6;
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						f_proc=8;
						break;
					default : ;
				}
				break;
			default : break;
		}

		if (f_proc!=0)
		{
			if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
			{
				if (f_proc<7)
				{
					for(uint8_t i=0; i<threads_number; i++)
						MT_Thread[i].f_process=f_proc+Offset_MT;

					if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

					for(uint8_t i=0; i<threads_number; i++)
						MT_Thread[i].f_process=0;
				}
				else
				{
					switch (f_proc)
					{
						case 7 :
							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=3+Offset_MT;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=7+Offset_MT;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=8+Offset_MT;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=0;
							break;
						case 8 :
							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=6+Offset_MT;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=9+Offset_MT;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=10+Offset_MT;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=0;
							break;
						default : break;
					}
				}
				poolInterface->ReleaseThreadPool(UserId,false);
			}
		}
	}
	else
	{
		ptrdiff_t srcPitch0,srcPitch1,srcPitch2;
		ptrdiff_t dstPitch0,dstPitch1,dstPitch2;

		srcPitch0=image_data.src_pitch0;
		srcPitch1=image_data.src_pitch1;
		srcPitch2=image_data.src_pitch2;
		dstPitch0=image_data.dst_pitch0;
		dstPitch1=image_data.dst_pitch1;
		dstPitch2=image_data.dst_pitch2;

		switch(current_mode)
		{
		case 1 :
			src1_0 = image_data.src_plane0;
			src2_0 = (void *) ((uint8_t *)src1_0+image_data.src_pitch0);
			dst0_0 = image_data.dst_plane0;
			h0=image_data.src_h0-1;
			src1_1 = image_data.src_plane1;
			src2_1 = (void *) ((uint8_t *)src1_1+image_data.src_pitch1);
			dst0_1 = image_data.dst_plane1;
			h1=image_data.src_h1-1;
			src1_2 = image_data.src_plane2;
			src2_2 = (void *) ((uint8_t *)src1_2+image_data.src_pitch2);
			dst0_2 = image_data.dst_plane2;
			h2=image_data.src_h2-1;
			break;
		case 2 :
			src1_0=image_data.src_plane0;
			dst0_0=image_data.dst_plane0;
			h0=image_data.src_h0-2;
			src1_1=image_data.src_plane1;
			dst0_1=image_data.dst_plane1;
			h1=image_data.src_h1-2;
			src1_2=image_data.src_plane2;
			dst0_2=image_data.dst_plane2;
			h2=image_data.src_h2-2;
			break;
		case 3 :
			src1_0 = (void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
			src2_0 = (void *)((uint8_t *)src1_0+2*image_data.src_pitch0);
			dst0_0 = (void *)((uint8_t *)image_data.dst_plane0+2*image_data.dst_pitch0);
			h0=(image_data.src_h0 >> 1)-1;
			srcPitch0<<=1;
			dstPitch0<<=1;
			src1_1 = (void *)((uint8_t *)image_data.src_plane1+image_data.src_pitch1);
			src2_1 = (void *)((uint8_t *)src1_1+2*image_data.src_pitch1);
			dst0_1 = (void *)((uint8_t *)image_data.dst_plane1+2*image_data.dst_pitch1);
			h1=(image_data.src_h1 >> 1)-1;
			srcPitch1<<=1;
			dstPitch1<<=1;
			src1_2 = (void *)((uint8_t *)image_data.src_plane2+image_data.src_pitch2);
			src2_2 = (void *)((uint8_t *)src1_2+2*image_data.src_pitch2);
			dst0_2 = (void *)((uint8_t *)image_data.dst_plane2+2*image_data.dst_pitch2);
			h2=(image_data.src_h2 >> 1)-1;
			srcPitch2<<=1;
			dstPitch2<<=1;
			break;
		case 4 :
			src1_0 = image_data.src_plane0;
			src2_0 = (void *)((uint8_t *)src1_0+2*image_data.src_pitch0);
			dst0_0 = (void *)((uint8_t *)image_data.dst_plane0+image_data.dst_pitch0);
			h0=(image_data.src_h0 >> 1)-1;
			srcPitch0<<=1;
			dstPitch0<<=1;
			src1_1 = image_data.src_plane1;
			src2_1 = (void *)((uint8_t *)src1_1+2*image_data.src_pitch1);
			dst0_1 = (void *)((uint8_t *)image_data.dst_plane1+image_data.dst_pitch1);
			h1=(image_data.src_h1 >> 1)-1;
			srcPitch1<<=1;
			dstPitch1<<=1;
			src1_2 = image_data.src_plane2;
			src2_2 = (void *)((uint8_t *)src1_2+2*image_data.src_pitch2);
			dst0_2 = (void *)((uint8_t *)image_data.dst_plane2+image_data.dst_pitch2);
			h2=(image_data.src_h2 >> 1)-1;
			srcPitch2<<=1;
			dstPitch2<<=1;
			break;
		}
	
		switch(current_mode)
		{
			case 0 :
				switch(image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						Move_Full(buffer0,image_data.dst_plane0,image_data.src_line0,image_data.src_h0,buffer0_pitch,dstPitch0);
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						Move_Full(buffer0,image_data.dst_plane0,image_data.src_line0,image_data.src_h0,buffer0_pitch,dstPitch0);
						Move_Full(buffer1,image_data.dst_plane1,image_data.src_line1,image_data.src_h1,buffer1_pitch,dstPitch1);
						Move_Full(buffer2,image_data.dst_plane2,image_data.src_line2,image_data.src_h2,buffer2_pitch,dstPitch2);
						break;
				}
				break;
			case 1 :
			case 3 :
			case 4 :
#ifdef AVX2_BUILD_POSSIBLE
				if (AVX2_Enable)
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_AVX2(src1_0,src2_0,dst0_0,(image_data.src_w0+7)>>3,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_AVX2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+7)>>3,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_AVX2(src1_0,src2_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_AVX2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8_AVX2(src1_0,src2_0,dst0_0,(image_data.src_w0+31)>>5,
									h0,srcPitch0,dstPitch0);								
								JPSDR_Deinterlace_Blend_8_AVX2(src1_1,src2_1,dst0_1,(image_data.src_w1+31)>>5,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8_AVX2(src1_2,src2_2,dst0_2,(image_data.src_w2+31)>>5,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16_AVX2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+31)>>5,
									h0,srcPitch0,dstPitch0);								
								JPSDR_Deinterlace_Blend_16_AVX2(src1_1,src2_1,dst0_1,((image_data.src_w1<<1)+31)>>5,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16_AVX2(src1_2,src2_2,dst0_2,((image_data.src_w2<<1)+31)>>5,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_AVX2(src1_0,src2_0,dst0_0,(image_data.src_w0+31)>>5,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_AVX2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+31)>>5,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}
				else
#endif
				{
				if (AVX_Enable)
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_AVX(src1_0,src2_0,dst0_0,(image_data.src_w0+3)>>2,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_AVX(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+3)>>2,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_AVX(src1_0,src2_0,dst0_0,(image_data.src_w0+7)>>3,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_AVX(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+7)>>3,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8_AVX(src1_0,src2_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);								
								JPSDR_Deinterlace_Blend_8_AVX(src1_1,src2_1,dst0_1,(image_data.src_w1+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8_AVX(src1_2,src2_2,dst0_2,(image_data.src_w2+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16_AVX(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);								
								JPSDR_Deinterlace_Blend_16_AVX(src1_1,src2_1,dst0_1,((image_data.src_w1<<1)+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16_AVX(src1_2,src2_2,dst0_2,((image_data.src_w2<<1)+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_AVX(src1_0,src2_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_AVX(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}
				else
				{

				if (SSE2_Enable)
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_SSE2(src1_0,src2_0,dst0_0,(image_data.src_w0+3)>>2,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_SSE2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+3)>>2,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_SSE2(src1_0,src2_0,dst0_0,(image_data.src_w0+7)>>3,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_SSE2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+7)>>3,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8_SSE2(src1_0,src2_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);								
								JPSDR_Deinterlace_Blend_8_SSE2(src1_1,src2_1,dst0_1,(image_data.src_w1+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8_SSE2(src1_2,src2_2,dst0_2,(image_data.src_w2+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16_SSE2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);								
								JPSDR_Deinterlace_Blend_16_SSE2(src1_1,src2_1,dst0_1,((image_data.src_w1<<1)+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16_SSE2(src1_2,src2_2,dst0_2,((image_data.src_w2<<1)+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_SSE2(src1_0,src2_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_SSE2(src1_0,src2_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}
				else
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_24(src1_0,src2_0,dst0_0,image_data.src_w0,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_48(src1_0,src2_0,dst0_0,image_data.src_w0,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_32(src1_0,src2_0,dst0_0,(image_data.src_w0+1)>>1,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_64(src1_0,src2_0,dst0_0,(image_data.src_w0+1)>>1,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8(src1_0,src2_0,dst0_0,image_data.src_w0,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_8(src1_1,src2_1,dst0_1,image_data.src_w1,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8(src1_2,src2_2,dst0_2,image_data.src_w2,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16(src1_0,src2_0,dst0_0,image_data.src_w0,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_16(src1_1,src2_1,dst0_1,image_data.src_w1,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16(src1_2,src2_2,dst0_2,image_data.src_w2,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8(src1_0,src2_0,dst0_0,image_data.src_w0,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16(src1_0,src2_0,dst0_0,image_data.src_w0,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}

				}
				}

				switch (current_mode)
				{
					case 1 :
					case 4 :
						switch(image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
							case VMODE_Y :
								memcpy((uint8_t *)dst0_0+(h0*dstPitch0),
									(uint8_t *)dst0_0+((h0-1)*dstPitch0),image_data.src_line0);
								break;
							case VMODE_PLANAR_YUV444 :
							case VMODE_PLANAR_YUV422 :
							case VMODE_PLANAR_YUV420 :
							case VMODE_PLANAR_YUV411 :
							case VMODE_PLANAR_YUV410 :
								memcpy((uint8_t *)dst0_0+(h0*dstPitch0),
									(uint8_t *)dst0_0+((h0-1)*dstPitch0),image_data.src_line0);
								memcpy((uint8_t *)dst0_1+(h1*dstPitch1),
									(uint8_t *)dst0_1+((h1-1)*dstPitch1),image_data.src_line1);
								memcpy((uint8_t *)dst0_2+(h2*dstPitch2),
									(uint8_t *)dst0_2+((h2-1)*dstPitch2),image_data.src_line2);
								break;
						}
						break;
					case 3 :
						switch(image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
							case VMODE_Y :
								memcpy(dst0_0,(uint8_t *)dst0_0+dstPitch0,image_data.src_line0);
								break;
							case VMODE_PLANAR_YUV444 :
							case VMODE_PLANAR_YUV422 :
							case VMODE_PLANAR_YUV420 :
							case VMODE_PLANAR_YUV411 :
							case VMODE_PLANAR_YUV410 :
								memcpy(dst0_0,(uint8_t *)dst0_0+dstPitch0,image_data.src_line0);
								memcpy(dst0_1,(uint8_t *)dst0_1+dstPitch1,image_data.src_line1);
								memcpy(dst0_2,(uint8_t *)dst0_2+dstPitch2,image_data.src_line2);
								break;
						}
						break;
					default : break;
				}
				break;
			case 2 :
#ifdef AVX2_BUILD_POSSIBLE
				if (AVX2_Enable && swap_buffer)
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_AVX2(src1_0,dst0_0,(image_data.src_w0+7)>>3,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_AVX2(src1_0,dst0_0,((image_data.src_w0<<1)+7)>>3,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_AVX2(src1_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_AVX2(src1_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8_Tri_AVX2(src1_0,dst0_0,(image_data.src_w0+31)>>5,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_8_Tri_AVX2(src1_1,dst0_1,(image_data.src_w1+31)>>5,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8_Tri_AVX2(src1_2,dst0_2,(image_data.src_w2+31)>>5,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16_Tri_AVX2(src1_0,dst0_0,((image_data.src_w0<<1)+31)>>5,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_16_Tri_AVX2(src1_1,dst0_1,((image_data.src_w1<<1)+31)>>5,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16_Tri_AVX2(src1_2,dst0_2,((image_data.src_w2<<1)+31)>>5,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_AVX2(src1_0,dst0_0,(image_data.src_w0+31)>>5,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_AVX2(src1_0,dst0_0,((image_data.src_w0<<1)+31)>>5,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}
				else
#endif
				{
				if (AVX_Enable && swap_buffer)
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_AVX(src1_0,dst0_0,(image_data.src_w0+3)>>2,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_AVX(src1_0,dst0_0,((image_data.src_w0<<1)+3)>>2,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_AVX(src1_0,dst0_0,(image_data.src_w0+7)>>3,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_AVX(src1_0,dst0_0,((image_data.src_w0<<1)+7)>>3,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8_Tri_AVX(src1_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_8_Tri_AVX(src1_1,dst0_1,(image_data.src_w1+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8_Tri_AVX(src1_2,dst0_2,(image_data.src_w2+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16_Tri_AVX(src1_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_16_Tri_AVX(src1_1,dst0_1,((image_data.src_w1<<1)+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16_Tri_AVX(src1_2,dst0_2,((image_data.src_w2<<1)+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_AVX(src1_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_AVX(src1_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}
				else
				{

				if (SSE2_Enable && swap_buffer)
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_SSE2(src1_0,dst0_0,(image_data.src_w0+3)>>2,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_SSE2(src1_0,dst0_0,((image_data.src_w0<<1)+3)>>2,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_SSE2(src1_0,dst0_0,(image_data.src_w0+7)>>3,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_SSE2(src1_0,dst0_0,((image_data.src_w0<<1)+7)>>3,
									h0,srcPitch0,dstPitch0);
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								JPSDR_Deinterlace_Blend_8_Tri_SSE2(src1_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_8_Tri_SSE2(src1_1,dst0_1,(image_data.src_w1+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_8_Tri_SSE2(src1_2,dst0_2,(image_data.src_w2+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_16_Tri_SSE2(src1_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
								JPSDR_Deinterlace_Blend_16_Tri_SSE2(src1_1,dst0_1,((image_data.src_w1<<1)+15)>>4,
									h1,srcPitch1,dstPitch1);
								JPSDR_Deinterlace_Blend_16_Tri_SSE2(src1_2,dst0_2,((image_data.src_w2<<1)+15)>>4,
									h2,srcPitch2,dstPitch2);
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
								JPSDR_Deinterlace_Blend_8_Tri_SSE2(src1_0,dst0_0,(image_data.src_w0+15)>>4,
									h0,srcPitch0,dstPitch0);
							else
								JPSDR_Deinterlace_Blend_16_Tri_SSE2(src1_0,dst0_0,((image_data.src_w0<<1)+15)>>4,
									h0,srcPitch0,dstPitch0);
							break;
					}
				}
				else
				{
					switch(image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							if (image_data.src_bits_pixel==8)
							{
								if (swap_buffer)
									Blend_Tri_RGB32(src1_0,dst0_0,image_data.src_w0,h0,srcPitch0,dstPitch0);
								else
									Blend_Tri_RGB32(src1_0,dst0_0,buffer[0],image_data.src_w0,h0,srcPitch0,dstPitch0);
							}
							else
							{
								if (swap_buffer)
									Blend_Tri_RGB64(src1_0,dst0_0,image_data.src_w0,h0,srcPitch0,dstPitch0);
								else
									Blend_Tri_RGB64(src1_0,dst0_0,buffer[0],image_data.src_w0,h0,srcPitch0,dstPitch0);
							}
							break;
						case VMODE_YUYV :
						case VMODE_UYVY :
							if (image_data.src_bits_pixel==8)
							{
								if (swap_buffer)
									Blend_Tri_YUV32(src1_0,dst0_0,(image_data.src_w0+1)>>1,h0,srcPitch0,dstPitch0);
								else
									Blend_Tri_YUV32(src1_0,dst0_0,buffer[0],(image_data.src_w0+1)>>1,h0,srcPitch0,dstPitch0);
							}
							else
							{
								if (swap_buffer)
									Blend_Tri_YUV64(src1_0,dst0_0,(image_data.src_w0+1)>>1,h0,srcPitch0,dstPitch0);
								else
									Blend_Tri_YUV64(src1_0,dst0_0,buffer[0],(image_data.src_w0+1)>>1,h0,srcPitch0,dstPitch0);
							}
							break;
						case VMODE_PLANAR_YUV444 :
						case VMODE_PLANAR_YUV422 :
						case VMODE_PLANAR_YUV420 :
						case VMODE_PLANAR_YUV411 :
						case VMODE_PLANAR_YUV410 :
							if (image_data.src_bits_pixel==8)
							{
								if (swap_buffer)
								{
									Blend_Tri_Planar8(src1_0,dst0_0,image_data.src_w0,h0,srcPitch0,dstPitch0);
									Blend_Tri_Planar8(src1_1,dst0_1,image_data.src_w1,h1,srcPitch1,dstPitch1);
									Blend_Tri_Planar8(src1_2,dst0_2,image_data.src_w2,h2,srcPitch2,dstPitch2);
								}
								else
								{
									Blend_Tri_Planar8(src1_0,dst0_0,buffer[0],image_data.src_w0,h0,srcPitch0,dstPitch0);
									Blend_Tri_Planar8(src1_1,dst0_1,buffer[1],image_data.src_w1,h1,srcPitch1,dstPitch1);
									Blend_Tri_Planar8(src1_2,dst0_2,buffer[2],image_data.src_w2,h2,srcPitch2,dstPitch2);
								}
							}
							else
							{
								if (swap_buffer)
								{
									Blend_Tri_Planar16(src1_0,dst0_0,image_data.src_w0,h0,srcPitch0,dstPitch0);
									Blend_Tri_Planar16(src1_1,dst0_1,image_data.src_w1,h1,srcPitch1,dstPitch1);
									Blend_Tri_Planar16(src1_2,dst0_2,image_data.src_w2,h2,srcPitch2,dstPitch2);
								}
								else
								{
									Blend_Tri_Planar16(src1_0,dst0_0,buffer[0],image_data.src_w0,h0,srcPitch0,dstPitch0);
									Blend_Tri_Planar16(src1_1,dst0_1,buffer[1],image_data.src_w1,h1,srcPitch1,dstPitch1);
									Blend_Tri_Planar16(src1_2,dst0_2,buffer[2],image_data.src_w2,h2,srcPitch2,dstPitch2);
								}
							}
							break;
						case VMODE_Y :
							if (image_data.src_bits_pixel==8)
							{
								if (swap_buffer)
									Blend_Tri_Planar8(src1_0,dst0_0,image_data.src_w0,h0,srcPitch0,dstPitch0);
								else
									Blend_Tri_Planar8(src1_0,dst0_0,buffer[0],image_data.src_w0,h0,srcPitch0,dstPitch0);
							}
							else
							{
								if (swap_buffer)
									Blend_Tri_Planar16(src1_0,dst0_0,image_data.src_w0,h0,srcPitch0,dstPitch0);
								else
									Blend_Tri_Planar16(src1_0,dst0_0,buffer[0],image_data.src_w0,h0,srcPitch0,dstPitch0);
							}
							break;
					}
				}

				}
				}
				break;
		}
	}

	if (transfert_buffer) 
	{
		switch(image_data.video_mode)
		{
			case 0 :
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				Move_Full(image_data.dst_plane0,buffer0,image_data.dst_line0,
					image_data.dst_h0,image_data.dst_pitch0,buffer0_pitch);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				Move_Full(image_data.dst_plane0,buffer0,image_data.dst_line0,
					image_data.dst_h0,image_data.dst_pitch0,buffer0_pitch);
				Move_Full(image_data.dst_plane1,buffer1,image_data.dst_line1,
					image_data.dst_h1,image_data.dst_pitch1,buffer1_pitch);
				Move_Full(image_data.dst_plane2,buffer2,image_data.dst_line2,
					image_data.dst_h2,image_data.dst_pitch2,buffer2_pitch);
				break;
		}
	}
}


bool JPSDR_Deinterlace::Configure(VDXHWND hwnd)
{
	JPSDR_DeinterlaceDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}

void JPSDR_Deinterlace::GetSettingString(char *buf, int maxlen)
{
	char str_out[1200];

	if (!mData.file_mode)
	{
		strcpy_s(str_out,1200," Mode : %d");
/*		if ((mData.mode==5) || (mData.mode==6))
		{
			if (mData.TFF) strcat_s(str_out,1200," (TFF)");
			else strcat_s(str_out,1200," (BFF)");
		}*/
		SafePrintf(buf,maxlen,str_out,mData.mode);
	}
	else
	{
		strcpy_s(str_out,1200," File mode : %s");
		SafePrintf(buf,maxlen,str_out,mData.filename);
	}
}

void JPSDR_Deinterlace::GetScriptString(char *buf, int maxlen)
{
    char save[1200];
	int i, j;

	i = j = 0;
	while((mData.filename[i]!='\0') && (j<1198))
	{
		if (mData.filename[i] == '\\')
		{
			save[j++] = '\\';
			save[j++] = '\\';
			i++;
		}
		else
		{
			save[j++] = mData.filename[i++];
		}
	}
	save[j] = '\0';

	SafePrintf(buf, maxlen, "Config(%d, %d, %d, \"%s\")",mData.mode,mData.file_mode,mData.mt_mode,save);
}


void JPSDR_Deinterlace::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.mode=argv[0].asInt();
	mData.file_mode=!!argv[1].asInt();
	mData.mt_mode=!!argv[2].asInt();
	strcpy_s(mData.filename,1024,*argv[3].asString());
}


		
extern VDXFilterDefinition2 filterDef_JPSDR_Deinterlace=
VDXVideoFilterDefinition<JPSDR_Deinterlace>("JPSDR","Deinterlace v6.1.1","Deinterlace blending frames. [SSE2][AVX][AVX2] Optimised.");



