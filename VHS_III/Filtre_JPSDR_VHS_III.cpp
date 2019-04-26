/*
 *  VHS_III
 *
 *  Temporal denoiser and chroma/luma sharpening for cleaning VHS.
 *  Copyright (C) 2005 JPSDR
 *	
 *  VHS_III is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  VHS_III is distributed in the hope that it will be useful,
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

#include "VHS_III_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Pascal.h"

extern int g_VFVAPIVersion;


extern "C" void JPSDR_VHS_III_convert_RGB_YUV_SSE2(const void *src,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t src_modulo,int16_t *lookup);
extern "C" void JPSDR_VHS_III_convert_RGB_YUV_SSE(const void *src,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t src_modulo,int16_t *lookup);
extern "C" void JPSDR_VHS_III_convert_YUV_RGB_SSE2(void *dst,int16_t *y,int16_t *u,
	int16_t *v,int32_t w,int32_t h,ptrdiff_t dst_modulo,int16_t *lookup);
extern "C" void JPSDR_VHS_III_convert_YUV_RGB_SSE(void *dst,int16_t *y,int16_t *u,
	int16_t *v,int32_t w,int32_t h,ptrdiff_t dst_modulo,int16_t *lookup);
extern "C" void JPSDR_VHS_III_shift_left(int16_t *in,int16_t *out,int32_t w,int32_t h,
	uint8_t delta);
extern "C" void JPSDR_VHS_III_shift_right(int16_t *in,int16_t *out,int32_t w,int32_t h,
	uint8_t delta);


#define Min_Y 16
#define Max_Y 235
#define Min_U 16
#define Max_U 240
#define Min_V 16
#define Max_V 240


class JPSDR_VHS_IIIData
{
public:
	uint16_t threshold_Y_1,threshold_Y_2,threshold_UV_1,threshold_UV_2,DCTi_thrs1,
		DCTi_thrs2,DTi_thrs1,DTi_thrs2;
	uint8_t filter_mode_Y,filter_mode_UV,DCTi_delta1,DCTi_delta2,DTi_delta1,DTi_delta2,
		chroma_shift,chroma_shift_mode;
	bool chroma_shift_enable,Y_filter_enable,UV_filter_enable,DCTi_enable,DCTi_first,DCTi_Test,
		DTi_enable,DTi_first,DTi_Test;

	JPSDR_VHS_IIIData(void);
};


JPSDR_VHS_IIIData::JPSDR_VHS_IIIData(void)
{
	threshold_Y_1=5;
	threshold_Y_2=8;
	filter_mode_Y=1;
	threshold_UV_1=14;
	threshold_UV_2=20;
	filter_mode_UV=2;
	DCTi_delta1=8;
	DCTi_delta2=4;
	DCTi_thrs1=100;
	DCTi_thrs2=25;
	DTi_delta1=5;
	DTi_delta2=3;
	DTi_thrs1=200;
	DTi_thrs2=80;
	chroma_shift_mode=1;
	chroma_shift=7;
	chroma_shift_enable=false;
	Y_filter_enable=false;
	UV_filter_enable=false;
	DCTi_enable=false;
	DCTi_first=false;
	DCTi_Test=false;
	DTi_enable=false;
	DTi_first=false;
	DTi_Test=false;
}


class JPSDR_VHS_IIIDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_VHS_IIIDialog(JPSDR_VHS_IIIData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_VHS_III),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	void Refresh_Dialog();
	
	JPSDR_VHS_IIIData& mData;
	JPSDR_VHS_IIIData moldData;
	IVDXFilterPreview2 *const mifp;
};



INT_PTR JPSDR_VHS_IIIDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


void JPSDR_VHS_IIIDialog::Refresh_Dialog()
{
	if (mData.Y_filter_enable) CheckDlgButton(mhdlg, IDC_FILTER_Y_ENABLE,BST_CHECKED);
	else CheckDlgButton(mhdlg, IDC_FILTER_Y_ENABLE,BST_UNCHECKED);
	if (mData.UV_filter_enable)CheckDlgButton(mhdlg,IDC_FILTER_UV_ENABLE,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_FILTER_UV_ENABLE,BST_UNCHECKED);
	if (mData.DCTi_enable) CheckDlgButton(mhdlg, IDC_DCTi_ENABLE,BST_CHECKED);
	else CheckDlgButton(mhdlg, IDC_DCTi_ENABLE,BST_UNCHECKED);
	if (!mData.DCTi_first) CheckDlgButton(mhdlg, IDC_DCTi_FILTER_FIRST,BST_CHECKED);
	else CheckDlgButton(mhdlg, IDC_DCTi_FILTER_FIRST,BST_UNCHECKED);
	if (mData.DCTi_Test) CheckDlgButton(mhdlg, IDC_DCTi_TEST,BST_CHECKED);
	else CheckDlgButton(mhdlg, IDC_DCTi_TEST,BST_UNCHECKED);
	if (mData.DTi_enable) CheckDlgButton(mhdlg, IDC_DTi_ENABLE,BST_CHECKED);
	else CheckDlgButton(mhdlg, IDC_DTi_ENABLE,BST_UNCHECKED);
	if (!mData.DTi_first) CheckDlgButton(mhdlg, IDC_DTi_FILTER_FIRST,BST_CHECKED);
	else CheckDlgButton(mhdlg, IDC_DTi_FILTER_FIRST,BST_UNCHECKED);
	if (mData.DTi_Test) CheckDlgButton(mhdlg, IDC_DTi_TEST,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_DTi_TEST,BST_UNCHECKED);
	if (mData.chroma_shift_enable) CheckDlgButton(mhdlg,IDC_CHROMA_SHIFT_ENABLE,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_CHROMA_SHIFT_ENABLE,BST_UNCHECKED);

	switch(mData.filter_mode_Y)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_Y_FILTER_1,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_Y_FILTER_2,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_Y_FILTER_3,1); break;
	}
	SetDlgItemInt(mhdlg,IDC_THRS_Y_1,mData.threshold_Y_1,FALSE);
	SetDlgItemInt(mhdlg,IDC_THRS_Y_2,mData.threshold_Y_2,FALSE);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_1),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_1_T),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_2),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_2_T),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_MODE),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_1),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_2),mData.Y_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_3),mData.Y_filter_enable);

	switch(mData.filter_mode_UV)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_UV_FILTER_1,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_UV_FILTER_2,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_UV_FILTER_3,1); break;
	}
	SetDlgItemInt(mhdlg,IDC_THRS_UV_1,mData.threshold_UV_1,FALSE);
	SetDlgItemInt(mhdlg,IDC_THRS_UV_2,mData.threshold_UV_2,FALSE);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_1),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_1_T),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_2),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_2_T),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_MODE),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_1),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_2),mData.UV_filter_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_3),mData.UV_filter_enable);

	SetDlgItemInt(mhdlg,IDC_DCTi_DELTA1,mData.DCTi_delta1,FALSE);
	SetDlgItemInt(mhdlg,IDC_DCTi_DELTA2,mData.DCTi_delta2,FALSE);
	SetDlgItemInt(mhdlg,IDC_DCTi_THRS1,mData.DCTi_thrs1,FALSE);
	SetDlgItemInt(mhdlg,IDC_DCTi_THRS2,mData.DCTi_thrs2,FALSE);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA1),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA1_T),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA2),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA2_T),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS1),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS1_T),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS2),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS2_T),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_FILTER_FIRST),mData.DCTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_TEST),mData.DCTi_enable);

	SetDlgItemInt(mhdlg,IDC_DTi_DELTA1,mData.DTi_delta1,FALSE);
	SetDlgItemInt(mhdlg,IDC_DTi_DELTA2,mData.DTi_delta2,FALSE);
	SetDlgItemInt(mhdlg,IDC_DTi_THRS1,mData.DTi_thrs1,FALSE);
	SetDlgItemInt(mhdlg,IDC_DTi_THRS2,mData.DTi_thrs2,FALSE);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA1),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA1_T),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA2),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA2_T),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS1),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS1_T),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS2),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS2_T),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_FILTER_FIRST),mData.DTi_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_DTi_TEST),mData.DTi_enable);

	switch(mData.chroma_shift_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_SHIFT_LEFT,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_SHIFT_RIGHT,1); break;
	}
	SetDlgItemInt(mhdlg,IDC_CHROMA_SHIFT,mData.chroma_shift,FALSE);
	EnableWindow(GetDlgItem(mhdlg,IDC_CHROMA_SHIFT),mData.chroma_shift_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_CHROMA_SHIFT_T),mData.chroma_shift_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_CHROMA_SHIFT_DIR),mData.chroma_shift_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_LEFT),mData.chroma_shift_enable);
	EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_RIGHT),mData.chroma_shift_enable);
}

bool JPSDR_VHS_IIIDialog::OnInit()
{
	moldData=mData;

	Refresh_Dialog();

	return false;
}


void JPSDR_VHS_IIIDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_VHS_IIIDialog::SaveToData()
{
	uint16_t thrs;
	uint8_t delta;
	BOOL success;

	mData.Y_filter_enable=!!IsDlgButtonChecked(mhdlg, IDC_FILTER_Y_ENABLE);
	thrs=GetDlgItemInt(mhdlg,IDC_THRS_Y_1,&success,FALSE);
	if (success) mData.threshold_Y_1=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_THRS_Y_2,&success,FALSE);
	if (success) mData.threshold_Y_2=thrs;
	if (!!IsDlgButtonChecked(mhdlg, IDC_Y_FILTER_1)) mData.filter_mode_Y=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_Y_FILTER_2)) mData.filter_mode_Y=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_Y_FILTER_3)) mData.filter_mode_Y=3;

	mData.UV_filter_enable=!!IsDlgButtonChecked(mhdlg, IDC_FILTER_UV_ENABLE);
	thrs=GetDlgItemInt(mhdlg,IDC_THRS_UV_1,&success,FALSE);
	if (success) mData.threshold_UV_1=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_THRS_UV_2,&success,FALSE);
	if (success) mData.threshold_UV_2=thrs;
	if (!!IsDlgButtonChecked(mhdlg, IDC_UV_FILTER_1)) mData.filter_mode_UV=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_UV_FILTER_2)) mData.filter_mode_UV=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_UV_FILTER_3)) mData.filter_mode_UV=3;

	mData.DCTi_enable=!!IsDlgButtonChecked(mhdlg, IDC_DCTi_ENABLE);
	thrs=GetDlgItemInt(mhdlg,IDC_DCTi_THRS1,&success,FALSE);
	if (success) mData.DCTi_thrs1=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_DCTi_THRS2,&success,FALSE);
	if (success) mData.DCTi_thrs2=thrs;
	delta=GetDlgItemInt(mhdlg,IDC_DCTi_DELTA1,&success,FALSE);
	if ((success) && (delta>=2)) mData.DCTi_delta1=delta;
	delta=GetDlgItemInt(mhdlg,IDC_DCTi_DELTA2,&success,FALSE);
	if ((success) && (delta>=2)) mData.DCTi_delta2=delta;
	if (mData.DCTi_delta1<mData.DCTi_delta2) mData.DCTi_delta2=mData.DCTi_delta1;
	mData.DCTi_first=!IsDlgButtonChecked(mhdlg, IDC_DCTi_FILTER_FIRST);
	mData.DCTi_Test=!!IsDlgButtonChecked(mhdlg, IDC_DCTi_TEST);

	mData.DTi_enable=!!IsDlgButtonChecked(mhdlg, IDC_DTi_ENABLE);
	thrs=GetDlgItemInt(mhdlg,IDC_DTi_THRS1,&success,FALSE);
	if (success) mData.DTi_thrs1=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_DTi_THRS2,&success,FALSE);
	if (success) mData.DTi_thrs2=thrs;
	delta=GetDlgItemInt(mhdlg,IDC_DTi_DELTA1,&success,FALSE);
	if ((success) && (delta>=2)) mData.DTi_delta1=delta;
	delta=GetDlgItemInt(mhdlg,IDC_DTi_DELTA2,&success,FALSE);
	if ((success) && (delta>=2)) mData.DTi_delta2=delta;
	if (mData.DTi_delta1<mData.DTi_delta2) mData.DTi_delta2=mData.DTi_delta1;
	mData.DTi_first=!IsDlgButtonChecked(mhdlg, IDC_DTi_FILTER_FIRST);
	mData.DTi_Test=!!IsDlgButtonChecked(mhdlg, IDC_DTi_TEST);

	mData.chroma_shift_enable=!!IsDlgButtonChecked(mhdlg, IDC_CHROMA_SHIFT_ENABLE);
	delta=GetDlgItemInt(mhdlg,IDC_CHROMA_SHIFT,&success,FALSE);
	if (success) mData.chroma_shift=delta;
	if (!!IsDlgButtonChecked(mhdlg, IDC_SHIFT_LEFT)) mData.chroma_shift_mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_SHIFT_RIGHT)) mData.chroma_shift_mode=2;

	return true;
}



bool JPSDR_VHS_IIIDialog::OnCommand(int cmd)
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
		case ID_DEFAULT :
			mData.threshold_Y_1=5;
			mData.threshold_Y_2=8;
			mData.filter_mode_Y=1;
			mData.threshold_UV_1=14;
			mData.threshold_UV_2=20;
			mData.filter_mode_UV=2;
			mData.DCTi_delta1=8;
			mData.DCTi_delta2=4;
			mData.DCTi_thrs1=100;
			mData.DCTi_thrs2=25;
			mData.DTi_delta1=5;
			mData.DTi_delta2=3;
			mData.DTi_thrs1=200;
			mData.DTi_thrs2=80;
			mData.chroma_shift_mode=1;
			mData.chroma_shift=7;
			mData.chroma_shift_enable=false;
			mData.Y_filter_enable=false;
			mData.UV_filter_enable=false;
			mData.DCTi_enable=false;
			mData.DCTi_first=false;
			mData.DCTi_Test=false;
			mData.DTi_enable=false;
			mData.DTi_first=false;
			mData.DTi_Test=false;

			Refresh_Dialog();

			return true;
		case IDC_FILTER_Y_ENABLE :
			mData.Y_filter_enable=!!IsDlgButtonChecked(mhdlg, IDC_FILTER_Y_ENABLE);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_1),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_1_T),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_2),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_Y_2_T),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_MODE),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_1),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_2),mData.Y_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_Y_FILTER_3),mData.Y_filter_enable);
			return true;
		case IDC_FILTER_UV_ENABLE :
			mData.UV_filter_enable=!!IsDlgButtonChecked(mhdlg, IDC_FILTER_UV_ENABLE);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_1),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_1_T),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_2),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRS_UV_2_T),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_MODE),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_1),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_2),mData.UV_filter_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_UV_FILTER_3),mData.UV_filter_enable);
			return true;
		case IDC_DCTi_ENABLE :
			mData.DCTi_enable=!!IsDlgButtonChecked(mhdlg, IDC_DCTi_ENABLE);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA1),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA1_T),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA2),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_DELTA2_T),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS1),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS1_T),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS2),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_THRS2_T),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_FILTER_FIRST),mData.DCTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DCTi_TEST),mData.DCTi_enable);
			return true;
		case IDC_DTi_ENABLE :
			mData.DTi_enable=!!IsDlgButtonChecked(mhdlg, IDC_DTi_ENABLE);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA1),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA1_T),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA2),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_DELTA2_T),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS1),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS1_T),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS2),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_THRS2_T),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_FILTER_FIRST),mData.DTi_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_DTi_TEST),mData.DTi_enable);
			return true;
		case IDC_CHROMA_SHIFT_ENABLE :
			mData.chroma_shift_enable=!!IsDlgButtonChecked(mhdlg, IDC_CHROMA_SHIFT_ENABLE);
			EnableWindow(GetDlgItem(mhdlg,IDC_CHROMA_SHIFT),mData.chroma_shift_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_CHROMA_SHIFT_T),mData.chroma_shift_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_CHROMA_SHIFT_DIR),mData.chroma_shift_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_LEFT),mData.chroma_shift_enable);
			EnableWindow(GetDlgItem(mhdlg,IDC_SHIFT_RIGHT),mData.chroma_shift_enable);
			return true;
	}
		
	return false;
}



class JPSDR_VHS_III : public JPSDRVDXVideoFilter
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
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	int16_t *Y_filtre[5],*U_filtre[5],*V_filtre[5];
	int16_t *Y_filtre_prec,*U_filtre_prec,*V_filtre_prec;
	int16_t *buffer_Y[2],*buffer_U[2],*buffer_V[2];
	int16_t lookup_RGB_Y[768],lookup_YUV_G[20403],lookup_filter_Y[44881],lookup_filter_UV[89761];
	uint8_t indice_filtre[5];
	bool SSE_Integer_Enable,SSE2_Enable,VDub2_Enable;

	void convert_RGB_YUV(const void *src,int16_t *y,int16_t *u,int16_t *v,
		int32_t w,int32_t h,ptrdiff_t src_modulo,int16_t *lookup);
	void convert_YUV_RGB(void *dst,int16_t *y,int16_t *u,int16_t *v,
		int32_t w,int32_t h,ptrdiff_t dst_modulo,int16_t *lookup);

	void convert_YUY2_YUV(const void *src,int16_t *y,int16_t *u,int16_t *v,
		int32_t w,int32_t h,ptrdiff_t src_modulo);
	void convert_UYVY_YUV(const void *src,int16_t *y,int16_t *u,int16_t *v,
		int32_t w,int32_t h,ptrdiff_t src_modulo);
	void convert_YUV_YUY2(void *dst,int16_t *y,int16_t *u,int16_t *v,
		int32_t w,int32_t h,ptrdiff_t dst_modulo);
	void convert_YUV_UYVY(void *dst,int16_t *y,int16_t *u,int16_t *v,
		int32_t w,int32_t h,ptrdiff_t dst_modulo);

	void convert_PlanarY_YUV(const void *src,int16_t *dst,int32_t w,int32_t h,ptrdiff_t src_modulo);
	void convert_PlanarUV_YUV(const void *src,int16_t *dst,int32_t w,int32_t h,ptrdiff_t src_modulo);
	void convert_YUV_PlanarY(int16_t *src,void *dst,int32_t w,int32_t h,ptrdiff_t dst_modulo);
	void convert_YUV_PlanarU(int16_t *src,void *dst,int32_t w,int32_t h,ptrdiff_t dst_modulo);
	void convert_YUV_PlanarV(int16_t *src,void *dst,int32_t w,int32_t h,ptrdiff_t dst_modulo);

	void Digital_Transition_improvement(int16_t *in,int16_t *out,int32_t w,int32_t h,
		uint8_t delta1,uint8_t delta2,int16_t thrs1,int16_t thrs2);
	void Digital_Transition_improvement_Test(int16_t *in,int16_t *out,int32_t w,
		int32_t h,uint8_t delta1,uint8_t delta2,int16_t thrs1,int16_t thrs2);
	void Init_Filter(int16_t *in1,int16_t *in2,int16_t *out,int32_t h,int32_t w);
	void VHS_Filter_Y_Mode_1(int16_t *filter_buffer_Y[],int16_t *filter_prec_Y,
		int16_t *out_Y,uint8_t *filter_indice,int32_t h,int32_t w,uint16_t thrs1,
		uint16_t thrs2);
	void VHS_Filter_Y_Mode_1_Pass_1(int16_t *filter_buffer_Y[],
		int16_t *filter_prec_Y,int16_t*out_Y,uint8_t *filter_indice,
		int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2);
	void VHS_Filter_Y_Mode_1_Pass_2(int16_t *filter_buffer_Y[],
		int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
		int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2);
	void VHS_Filter_Y_Mode_2(int16_t *filter_buffer_Y[], int16_t *filter_prec_Y,
		int16_t *out_Y,uint8_t *filter_indice,int32_t h,int32_t w,
		uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_Y_Mode_2_Pass_1(int16_t *filter_buffer_Y[],
		int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
		int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_Y_Mode_2_Pass_2(int16_t *filter_buffer_Y[],
		int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
		int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_Y_Mode_3(int16_t *filter_buffer_Y[], int16_t *filter_prec_Y,
		int16_t *out_Y,uint8_t *filter_indice,int32_t h,int32_t w,
		uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_Y_Mode_3_Pass_1(int16_t *filter_buffer_Y[],
		int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
		int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_Y_Mode_3_Pass_2(int16_t *filter_buffer_Y[],
		int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
		int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_1(int16_t *filter_buffer_U[], int16_t *filter_buffer_V[],
		int16_t *filter_prec_U, int16_t *filter_prec_V, int16_t *out_U,
		int16_t *out_V,uint8_t *filter_indice,int32_t h,int32_t w,
		uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_1_Pass_1(int16_t *filter_buffer_U[],
		int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
		int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
		int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_1_Pass_2(int16_t *filter_buffer_U[],
		int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
		int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
		int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_2(int16_t *filter_buffer_U[], int16_t *filter_buffer_V[],
		int16_t *filter_prec_U, int16_t *filter_prec_V, int16_t *out_U,
		int16_t *out_V,uint8_t *filter_indice,int32_t h,int32_t w,
		uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_2_Pass_1(int16_t *filter_buffer_U[],
		int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
		int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
		int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_2_Pass_2(int16_t *filter_buffer_U[],
		int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
		int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
		int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_3(int16_t *filter_buffer_U[], int16_t *filter_buffer_V[],
		int16_t *filter_prec_U, int16_t *filter_prec_V, int16_t *out_U,
		int16_t *out_V,uint8_t *filter_indice,int32_t h,int32_t w,
		uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_3_Pass_1(int16_t *filter_buffer_U[],
		int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
		int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
		int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);
	void VHS_Filter_UV_Mode_3_Pass_2(int16_t *filter_buffer_U[],
		int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
		int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
		int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_VHS_IIIData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_VHS_III)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_VHS_III,ScriptConfig,"iiiiiiiiiiiiiii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_VHS_III::Init()
{
	SSE_Integer_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_INTEGER_SSE)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	int32_t i;
	double a,b,c;

	for (i=0; i<5; i++)
	{
		Y_filtre[i]=NULL;
		U_filtre[i]=NULL;
		V_filtre[i]=NULL;
	}
	Y_filtre_prec=NULL;
	U_filtre_prec=NULL;
	V_filtre_prec=NULL;
	for (i=0; i<2; i++)
	{
		buffer_Y[i]=NULL;
		buffer_U[i]=NULL;
		buffer_V[i]=NULL;
	}

	a=0.3008;
	b=0.5859;
	c=0.1133;

	for (i=0; i<256; i++)
	{
		lookup_RGB_Y[i]=(int16_t)round(i*a*16.0);
		lookup_RGB_Y[i+256]=(int16_t)round(i*b*16.0);
		lookup_RGB_Y[i+512]=(int16_t)round(i*c*16.0);
	}
	for (i=0; i<=255*16; i++)
		lookup_YUV_G[i]=(int16_t)round(i*(1.0-a-c)/b);

	for (i=-255*16; i<=255*16; i++)
	{
		lookup_YUV_G[i+4080+(255*16+1)]=(int16_t)round(-1.0*i*a/b);
		lookup_YUV_G[i+4080+(255*16+1)+(2*255*16+1)]=(int16_t)round(-1.0*i*c/b);
	}

	return(true);
}


uint32 JPSDR_VHS_III::GetParams()
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
//		case nsVDXPixmap::kPixFormat_Y8 :
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
//		case nsVDXPixmap::kPixFormat_Y8_FR :
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
		case nsVDXPixmap::kPixFormat_VDXA_YUV :
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

	image_data.src_full_mode=CheckFullRangeMode(&fa->src);
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_HAS_LAG(2)|FILTERPARAM_ALIGN_SCANLINES);
}


bool JPSDR_VHS_III::Configure(VDXHWND hwnd)
{
	JPSDR_VHS_IIIDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_VHS_III::GetSettingString(char *buf, int maxlen)
{
	char out_string[200];

	strcpy_s(out_string,200," Enable :");
	if (mData.Y_filter_enable) strcat_s(out_string,200," Y filter");
	if (mData.UV_filter_enable) strcat_s(out_string,200," UV filter");
	if (mData.DCTi_enable) strcat_s(out_string,200," Dti (UV)");
	if (mData.DTi_enable) strcat_s(out_string,200," Dti (Y)");
	if (mData.chroma_shift_enable) strcat_s(out_string,200," C. Shift");

	SafePrintf(buf,maxlen,out_string);
}





void JPSDR_VHS_III::convert_RGB_YUV(const void *_src,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t src_modulo,int16_t *lookup)
{
	const RGB32BMP *src;

	src=(RGB32BMP *)_src;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,g,b;

			b=src->b;
			g=src->g;
			r=src->r;
			src++;
			*y=lookup[r]+lookup[g+256]+lookup[b+512];
			*u++=(b << 4)-*y;
			*v++=(r << 4)-*y++;
		}
		src=(RGB32BMP *)((uint8_t *)src + src_modulo);
	}
}


void JPSDR_VHS_III::convert_YUY2_YUV(const void *_src,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t src_modulo)
{
	const YUYV *src;

	src=(YUYV *)_src;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*y++=((int16_t)src->y1) << 4;
			*y++=((int16_t)src->y2) << 4;
			*u++=((int16_t)src->u-128) << 4;
			*v++=((int16_t)src->v-128) << 4;
			src++;
		}
		src=(YUYV *)((uint8_t *)src + src_modulo);
	}
}


void JPSDR_VHS_III::convert_UYVY_YUV(const void *_src,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t src_modulo)
{
	const UYVY *src;

	src=(UYVY *)_src;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*y++=((int16_t)src->y1) << 4;
			*y++=((int16_t)src->y2) << 4;
			*u++=((int16_t)src->u-128) << 4;
			*v++=((int16_t)src->v-128) << 4;
			src++;
		}
		src=(UYVY *)((uint8_t *)src + src_modulo);
	}
}


void JPSDR_VHS_III::convert_PlanarY_YUV(const void *_src,int16_t *dst,int32_t w,int32_t h,ptrdiff_t src_modulo)
{
	const uint8_t *src;

	src=(uint8_t *)_src;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*dst++=((int16_t)*src++) << 4;
		}
		src+=src_modulo;
	}
}


void JPSDR_VHS_III::convert_PlanarUV_YUV(const void *_src,int16_t *dst,int32_t w,int32_t h,ptrdiff_t src_modulo)
{
	const uint8_t *src;

	src=(uint8_t *)_src;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*dst++=(((int16_t)*src++)-128) << 4;
		}
		src+=src_modulo;
	}
}



void JPSDR_VHS_III::convert_YUV_RGB(void *_dst,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t dst_modulo,int16_t *lookup)
{
	RGB32BMP *dst;

	dst=(RGB32BMP *)_dst;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,g,b;

			r=*y+*v+8;
			if (r<0) r=0;
			if (r>4080) r=4080;
			r>>=4;
			g=lookup[*y]+lookup[*v+4080+4081]+lookup[*u+4080+(4081+8161)]+8;
			if (g<0) g=0;
			if (g>4080) g=4080;
			g>>=4;
			b=*y+*u+8;
			if (b<0) b=0;
			if (b>4080) b=4080;
			b>>=4;
			dst->b=(uint8_t)b;
			dst->g=(uint8_t)g;
			dst->r=(uint8_t)r;
			dst++;
			y++;
			u++;
			v++;
		}
		dst=(RGB32BMP *)((uint8_t *)dst + dst_modulo);
	}
}


void JPSDR_VHS_III::convert_YUV_YUY2(void *_dst,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t dst_modulo)
{
	YUYV *dst;

	dst=(YUYV *)_dst;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y1,y2,u1,v1;

			y1=((*y++)+8) >> 4;
			y2=((*y++)+8) >> 4;
			u1=(((*u++)+8) >> 4)+128;
			v1=(((*v++)+8) >> 4)+128;
			if (y1<Min_Y) y1=Min_Y;
			else
			{
				if (y1>Max_Y) y1=Max_Y;
			}
			if (y2<Min_Y) y2=Min_Y;
			else
			{
				if (y2>Max_Y) y2=Max_Y;
			}
			if (u1<Min_U) u1=Min_U;
			else
			{
				if (u1>Max_U) u1=Max_U;
			}
			if (v1<Min_V) v1=Min_V;
			else
			{
				if (v1>Max_V) v1=Max_V;
			}
			dst->y1=(uint8_t)y1;
			dst->u=(uint8_t)u1;
			dst->y2=(uint8_t)y2;
			dst->v=(uint8_t)v1;
			dst++;
		}
		dst=(YUYV *)((uint8_t *)dst + dst_modulo);
	}
}


void JPSDR_VHS_III::convert_YUV_UYVY(void *_dst,int16_t *y,int16_t *u,int16_t *v,
	int32_t w,int32_t h,ptrdiff_t dst_modulo)
{
	UYVY *dst;

	dst=(UYVY *)_dst;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y1,y2,u1,v1;

			y1=((*y++)+8) >> 4;
			y2=((*y++)+8) >> 4;
			u1=(((*u++)+8) >> 4)+128;
			v1=(((*v++)+8) >> 4)+128;
			if (y1<Min_Y) y1=Min_Y;
			else
			{
				if (y1>Max_Y) y1=Max_Y;
			}
			if (y2<Min_Y) y2=Min_Y;
			else
			{
				if (y2>Max_Y) y2=Max_Y;
			}
			if (u1<Min_U) u1=Min_U;
			else
			{
				if (u1>Max_U) u1=Max_U;
			}
			if (v1<Min_V) v1=Min_V;
			else
			{
				if (v1>Max_V) v1=Max_V;
			}
			dst->u=(uint8_t)u1;
			dst->y1=(uint8_t)y1;
			dst->v=(uint8_t)v1;
			dst->y2=(uint8_t)y2;
			dst++;
		}
		dst=(UYVY *)((uint8_t *)dst + dst_modulo);
	}
}


void JPSDR_VHS_III::convert_YUV_PlanarY(int16_t *src,void *_dst,int32_t w,int32_t h,ptrdiff_t dst_modulo)
{
	uint8_t *dst;

	dst=(uint8_t *)_dst;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y;

			y=((*src++)+8) >> 4;
			if (y<Min_Y) y=Min_Y;
			else
			{
				if (y>Max_Y) y=Max_Y;
			}
			*dst++=(uint8_t)y;
		}
		dst+=dst_modulo;
	}
}



void JPSDR_VHS_III::convert_YUV_PlanarU(int16_t *src,void *_dst,int32_t w,int32_t h,ptrdiff_t dst_modulo)
{
	uint8_t *dst;

	dst=(uint8_t *)_dst;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t u;

			u=(((*src++)+8) >> 4)+128;
			if (u<Min_U) u=Min_U;
			else
			{
				if (u>Max_U) u=Max_U;
			}
			*dst++=(uint8_t)u;
		}
		dst+=dst_modulo;
	}
}


void JPSDR_VHS_III::convert_YUV_PlanarV(int16_t *src,void *_dst,int32_t w,int32_t h,ptrdiff_t dst_modulo)
{
	uint8_t *dst;

	dst=(uint8_t *)_dst;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t v;

			v=(((*src++)+8) >> 4)+128;
			if (v<Min_V) v=Min_V;
			else
			{
				if (v>Max_V) v=Max_V;
			}
			*dst++=(uint8_t)v;
		}
		dst+=dst_modulo;
	}
}




void JPSDR_VHS_III::Digital_Transition_improvement(int16_t *in,int16_t *out,int32_t w,int32_t h,
	uint8_t delta1,uint8_t delta2,int16_t thrs1,int16_t thrs2)
{
	int32_t thrs1_a,thrs2_a,thrs1_b,thrs2_b;
	uint8_t cpt;
	bool hold;

	thrs1_a=thrs1 << 4;
	thrs2_a=thrs2 << 4;
	thrs1_b=round(((float)thrs1_a*(float)delta2)/(float)delta1);
	thrs2_b=round(((float)thrs2_a*(float)delta2)/(float)delta1);

	for (int32_t i=0; i<h; i++)
	{
		int32_t s1,s2;

		s2=in[1]-in[0];
		for (int32_t j=1; j<delta2-1; j++)
			s2+=in[j+1]-in[j];
		s1=s2;
		for (int32_t j=delta2-1; j<delta1-1; j++)
			s1+=in[j+1]-in[j];
		cpt=0;
		hold=false;
		for (int32_t j=0; j<w-delta1; j++)
		{
			int16_t clamp,d;

			s1+=in[j+delta1]-in[j+delta1-1];
			s2+=in[j+delta2]-in[j+delta2-1];
			if ((!hold) && ((abs(s1)<thrs1_a) && (abs(s1)>thrs2_a)))
			{
				hold=true;
				cpt=delta1;
				clamp=in[j];
			}
			if ((!hold) && ((abs(s2)<thrs1_b) && (abs(s2)>thrs2_b)))
			{
				hold=true;
				cpt=delta2;
				clamp=in[j];
			}
			if (hold) out[j]=clamp;
			else out[j]=in[j];
			if (cpt>0)
			{
				cpt--;
				if (cpt==0) hold=false;
			}
			d=in[j+1]-in[j];
			s1-=d;
			s2-=d;
		}
		for (int32_t j=w-delta1; j<w; j++)
			out[j]=in[j];
		out+=w;
		in+=w;
	}
}



void JPSDR_VHS_III::Digital_Transition_improvement_Test(int16_t *in,int16_t *out,sint32 w,
	sint32 h,uint8_t delta1,uint8_t delta2,int16_t thrs1,int16_t thrs2)
{
	int32_t thrs1_a,thrs2_a,thrs1_b,thrs2_b;
	uint8_t cpt;
	bool hold;

	thrs1_a=thrs1 << 4;
	thrs2_a=thrs2 << 4;
	thrs1_b=round(((float)thrs1_a*(float)delta2)/(float)delta1);
	thrs2_b=round(((float)thrs2_a*(float)delta2)/(float)delta1);

	for (int32_t i=0; i<h; i++)
	{
		int32_t s1,s2;

		s2=in[1]-in[0];
		for (int32_t j=1; j<delta2-1; j++)
			s2+=in[j+1]-in[j];
		s1=s2;
		for (int32_t j=delta2-1; j<delta1-1; j++)
			s1+=in[j+1]-in[j];
		cpt=0;
		hold=false;
		for (int32_t j=0; j<w-delta1; j++)
		{
			int16_t clamp,d;

			s1+=in[j+delta1]-in[j+delta1-1];
			s2+=in[j+delta2]-in[j+delta2-1];
			if ((!hold) && ((abs(s1)<thrs1_a) && (abs(s1)>thrs2_a)))
			{
				hold=true;
				cpt=delta1;
				clamp=2000;
			}
			if ((!hold) && ((abs(s2)<thrs1_b) && (abs(s2)>thrs2_b)))
			{
				hold=true;
				cpt=delta2;
				clamp=2000;
			}
			if (hold) out[j]=clamp;
			else out[j]=in[j];
			if (cpt>0)
			{
				cpt--;
				if (cpt==0) hold=false;
			}
			d=in[j+1]-in[j];
			s1-=d;
			s2-=d;
		}
		for (int32_t j=w-delta1; j<w; j++)
			out[j]=in[j];
		out+=w;
		in+=w;
	}
}



void JPSDR_VHS_III::Init_Filter(int16_t *in1,int16_t *in2,int16_t *out,int32_t h,int32_t w)
{
	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
			*out++=(*in1++ + *in2++)>>1;
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_1(int16_t *filter_buffer_Y[],int16_t *filter_prec_Y,
	int16_t *out_Y,uint8_t *filter_indice,int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2)
{
	uint8_t indice_m2,indice_m1,indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_m2=filter_indice[0]; indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int16_t y0,y1,y2;
			int16_t dy1,dy2;

			y2=filter_buffer_Y[indice_m2][indice];
			y1=filter_buffer_Y[indice_m1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy1=y0-y1;
			dy2=y0-y2;
			if ((abs(dy1)<=thrs1) && (abs(dy2)<=thrs1) && (abs(dy1+dy2)<=thrs2))
			{
				int16_t y;

				y=filter_prec_Y[indice];
				y=(y0+y1+y2+y+2)>>2;
				filter_prec_Y[indice]=y;
				out_Y[indice]=y;
			}
			else
			{
				int16_t y3,y4;
				int16_t dy3,dy4;

				y3=filter_buffer_Y[indice_p1][indice];
				y4=filter_buffer_Y[indice_p2][indice];
				dy3=y0-y3;
				dy4=y0-y4;
				if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
				{
					int16_t y;

					y=(y3+y4) >> 1;
					y=(y0+y3+y4+y+2)>>2;
					filter_prec_Y[indice]=y;
					out_Y[indice]=y;
				}
				else
				{
					if ((abs(dy1)<=thrs1) && (abs(dy3)<=thrs1) && (abs(dy1+dy2)<=thrs2))
					{
						int16_t y;

						y=(y1+y3) >> 1;
						y=(y0+y1+y3+y+2)>>2;
						filter_prec_Y[indice]=y;
						out_Y[indice]=y;
					}
					else
					{
						filter_prec_Y[indice]=y0;
						out_Y[indice]=y0;
					}
				}
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_1_Pass_1(int16_t *filter_buffer_Y[],
	int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
	int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2)
{
	uint8_t indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int16_t y0,y3,y4;
			int16_t dy3,dy4;

			y4=filter_buffer_Y[indice_p2][indice];
			y3=filter_buffer_Y[indice_p1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy3=y0-y3;
			dy4=y0-y4;
			if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
			{
				int16_t y;

				y=(y3+y4) >> 1;
				y=(y0+y3+y4+y+2)>>2;
				filter_prec_Y[indice]=y;
				out_Y[indice]=y;
			}
			else
			{
				filter_prec_Y[indice]=y0;
				out_Y[indice]=y0;
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_1_Pass_2(int16_t *filter_buffer_Y[],
	int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
	int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2)
{
	uint8_t indice_0,indice_p1,indice_p2,indice_m1;
	uint32_t indice;

	indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int16_t y0,y3,y4;
			int16_t dy3,dy4;

			y4=filter_buffer_Y[indice_p2][indice];
			y3=filter_buffer_Y[indice_p1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy3=y0-y3;
			dy4=y0-y4;
			if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
			{
				int16_t y;

				y=(y3+y4) >> 1;
				y=(y0+y3+y4+y+2)>>2;
				filter_prec_Y[indice]=y;
				out_Y[indice]=y;
			}
			else
			{
				int16_t y1;
				int16_t dy1;

				y1=filter_buffer_Y[indice_m1][indice];
				dy1=y0-y1;
				if ((abs(dy1)<=thrs1) && (abs(dy3)<=thrs1) && (abs(dy1+dy3)<=thrs2))
				{
					int16_t y;

					y=(y1+y3) >> 1;
					y=(y0+y1+y3+y+2)>>2;
					filter_prec_Y[indice]=y;
					out_Y[indice]=y;
				}
				else
				{
					filter_prec_Y[indice]=y0;
					out_Y[indice]=y0;
				}
			}
			indice++;
		}
}



void JPSDR_VHS_III::VHS_Filter_Y_Mode_2(int16_t *filter_buffer_Y[], int16_t *filter_prec_Y,
	int16_t *out_Y,uint8_t *filter_indice,int32_t h,int32_t w,
	uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_m2,indice_m1,indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_m2=filter_indice[0]; indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t y0,y1,y2;
			int32_t dy1,dy2;

			y2=filter_buffer_Y[indice_m2][indice];
			y1=filter_buffer_Y[indice_m1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy1=y0-y1;
			dy2=y0-y2;
			if ((abs(dy1)<=thrs1) && (abs(dy2)<=thrs1) && (abs(dy1+dy2)<=thrs2))
			{
				int32_t y;

				y=filter_prec_Y[indice];
				y=lookup[y0+y1+y2+(y << 2)];
				filter_prec_Y[indice]=(int16_t)y;
				out_Y[indice]=(int16_t)y;
			}
			else
			{
				int32_t y3,y4;
				int32_t dy3,dy4;

				y3=filter_buffer_Y[indice_p1][indice];
				y4=filter_buffer_Y[indice_p2][indice];
				dy3=y0-y3;
				dy4=y0-y4;
				if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
				{
					int32_t y;

					y=(y3+y4) >> 1;
					y=lookup[y0+y3+y4+(y << 2)];
					filter_prec_Y[indice]=(int16_t)y;
					out_Y[indice]=(int16_t)y;
				}
				else
				{
					if ((abs(dy1)<=thrs1) && (abs(dy3)<=thrs1) && (abs(dy1+dy2)<=thrs2))
					{
						int32_t y;

						y=(y1+y3) >> 1;
						y=lookup[y0+y1+y3+(y << 2)];
						filter_prec_Y[indice]=(int16_t)y;
						out_Y[indice]=(int16_t)y;
					}
					else
					{
						filter_prec_Y[indice]=(int16_t)y0;
						out_Y[indice]=(int16_t)y0;
					}
				}
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_2_Pass_1(int16_t *filter_buffer_Y[],
	int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
	int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t y0,y3,y4;
			int32_t dy3,dy4;

			y4=filter_buffer_Y[indice_p2][indice];
			y3=filter_buffer_Y[indice_p1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy3=y0-y3;
			dy4=y0-y4;
			if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
			{
				int32_t y;

				y=(y3+y4) >> 1;
				y=lookup[y0+y3+y4+(y << 2)];
				filter_prec_Y[indice]=(int16_t)y;
				out_Y[indice]=(int16_t)y;
			}
			else
			{
				filter_prec_Y[indice]=(int16_t)y0;
				out_Y[indice]=(int16_t)y0;
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_2_Pass_2(int16_t *filter_buffer_Y[],
	int16_t *filter_prec_Y,int16_t *out_Y,unsigned char *filter_indice,
	int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2,indice_m1;
	uint32_t indice;

	indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t y0,y3,y4;
			int32_t dy3,dy4;

			y4=filter_buffer_Y[indice_p2][indice];
			y3=filter_buffer_Y[indice_p1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy3=y0-y3;
			dy4=y0-y4;
			if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
			{
				int32_t y;

				y=(y3+y4) >> 1;
				y=lookup[y0+y3+y4+(y << 2)];
				filter_prec_Y[indice]=(int16_t)y;
				out_Y[indice]=(int16_t)y;
			}
			else
			{
				int32_t y1;
				int32_t dy1;

				y1=filter_buffer_Y[indice_m1][indice];
				dy1=y0-y1;
				if ((abs(dy1)<=thrs1) && (abs(dy3)<=thrs1) && (abs(dy1+dy3)<=thrs2))
				{
					int32_t y;

					y=(y1+y3) >> 1;
					y=lookup[y0+y1+y3+(y << 2)];
					filter_prec_Y[indice]=(int16_t)y;
					out_Y[indice]=(int16_t)y;
				}
				else
				{
					filter_prec_Y[indice]=(int16_t)y0;
					out_Y[indice]=(int16_t)y0;
				}
			}
			indice++;
		}
}
		


void JPSDR_VHS_III::VHS_Filter_Y_Mode_3(int16_t *filter_buffer_Y[], int16_t *filter_prec_Y,
	int16_t *out_Y,uint8_t *filter_indice,int32_t h,int32_t w,
	uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_m2,indice_m1,indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_m2=filter_indice[0]; indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t y0,y1,y2;
			int32_t dy1,dy2;

			y2=filter_buffer_Y[indice_m2][indice];
			y1=filter_buffer_Y[indice_m1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy1=y0-y1;
			dy2=y0-y2;
			if ((abs(dy1)<=thrs1) && (abs(dy2)<=thrs1) && (abs(dy1+dy2)<=thrs2))
			{
				int32_t y;

				y=filter_prec_Y[indice];
				y=lookup[y0+y1+y2+(y << 3)];
				filter_prec_Y[indice]=(int16_t)y;
				out_Y[indice]=(int16_t)y;
			}
			else
			{
				int32_t y3,y4;
				int32_t dy3,dy4;

				y3=filter_buffer_Y[indice_p1][indice];
				y4=filter_buffer_Y[indice_p2][indice];
				dy3=y0-y3;
				dy4=y0-y4;
				if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
				{
					int32_t y;

					y=(y3+y4) >> 1;
					y=lookup[y0+y3+y4+(y << 3)];
					filter_prec_Y[indice]=(int16_t)y;
					out_Y[indice]=(int16_t)y;
				}
				else
				{
					if ((abs(dy1)<=thrs1) && (abs(dy3)<=thrs1) && (abs(dy1+dy3)<=thrs2))
					{
						int32_t y;

						y=(y1+y3) >> 1;
						y=lookup[y0+y1+y3+(y << 3)];
						filter_prec_Y[indice]=(int16_t)y;
						out_Y[indice]=(int16_t)y;
					}
					else
					{
						filter_prec_Y[indice]=(int16_t)y0;
						out_Y[indice]=(int16_t)y0;
					}
				}
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_3_Pass_1(int16_t *filter_buffer_Y[],
	int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
	int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t y0,y3,y4;
			int32_t dy3,dy4;

			y4=filter_buffer_Y[indice_p2][indice];
			y3=filter_buffer_Y[indice_p1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy3=y0-y3;
			dy4=y0-y4;
			if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
			{
				int32_t y;

				y=(y3+y4) >> 1;
				y=lookup[y0+y3+y4+(y << 3)];
				filter_prec_Y[indice]=(int16_t)y;
				out_Y[indice]=(int16_t)y;
			}
			else
			{
				filter_prec_Y[indice]=(int16_t)y0;
				out_Y[indice]=(int16_t)y0;
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_Y_Mode_3_Pass_2(int16_t *filter_buffer_Y[],
	int16_t *filter_prec_Y,int16_t *out_Y,uint8_t *filter_indice,
	int32_t h,int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2,indice_m1;
	uint32_t indice;

	indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t y0,y3,y4;
			int32_t dy3,dy4;

			y4=filter_buffer_Y[indice_p2][indice];
			y3=filter_buffer_Y[indice_p1][indice];
			y0=filter_buffer_Y[indice_0][indice];
			dy3=y0-y3;
			dy4=y0-y4;
			if ((abs(dy3)<=thrs1) && (abs(dy4)<=thrs1) && (abs(dy3+dy4)<=thrs2))
			{
				int32_t y;

				y=(y3+y4) >> 1;
				y=lookup[y0+y3+y4+(y << 3)];
				filter_prec_Y[indice]=(int16_t)y;
				out_Y[indice]=(int16_t)y;
			}
			else
			{
				int32_t y1;
				int32_t dy1;

				y1=filter_buffer_Y[indice_m1][indice];
				dy1=y0-y1;
				if ((abs(dy1)<=thrs1) && (abs(dy3)<=thrs1) && (abs(dy1+dy3)<=thrs2))
				{
					int32_t y;

					y=(y1+y3) >> 1;
					y=lookup[y0+y1+y3+(y << 3)];
					filter_prec_Y[indice]=(int16_t)y;
					out_Y[indice]=(int16_t)y;
				}
				else
				{
					filter_prec_Y[indice]=(int16_t)y0;
					out_Y[indice]=(int16_t)y0;
				}
			}
			indice++;
		}
}



void JPSDR_VHS_III::VHS_Filter_UV_Mode_1(int16_t *filter_buffer_U[], int16_t *filter_buffer_V[],
	int16_t *filter_prec_U, int16_t *filter_prec_V, int16_t *out_U,
	int16_t *out_V,uint8_t *filter_indice,int32_t h,int32_t w,
	uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_m2,indice_m1,indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_m2=filter_indice[0]; indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u1,u2,v0,v1,v2;
			int32_t du1,du2,dv1,dv2;

			u2=filter_buffer_U[indice_m2][indice];
			u1=filter_buffer_U[indice_m1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v2=filter_buffer_V[indice_m2][indice];
			v1=filter_buffer_V[indice_m1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du1=u0-u1;
			du2=u0-u2;
			dv1=v0-v1;
			dv2=v0-v2;
			if ((abs(du1)<=thrs1) && (abs(du2)<=thrs1) && (abs(du1+du2)<=thrs2) &&
				(abs(dv1)<=thrs1) && (abs(dv2)<=thrs1) && (abs(dv1+dv2)<=thrs2))
			{
				int32_t u,v;

				u=filter_prec_U[indice];
				u=lookup[u0+u1+u2+u+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=filter_prec_V[indice];
				v=lookup[v0+v1+v2+v+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				int32_t u3,u4,v3,v4;
				int32_t du3,du4,dv3,dv4;

				u3=filter_buffer_U[indice_p1][indice];
				u4=filter_buffer_U[indice_p2][indice];
				v3=filter_buffer_V[indice_p1][indice];
				v4=filter_buffer_V[indice_p2][indice];
				du3=u0-u3;
				du4=u0-u4;
				dv3=v0-v3;
				dv4=v0-v4;
				if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
					(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
				{
					int32_t u,v;

					u=(u3+u4) >> 1;
					u=lookup[u0+u3+u4+u+44880];
					filter_prec_U[indice]=(int16_t)u;
					out_U[indice]=(int16_t)u;
					v=(v3+v4) >> 1;
					v=lookup[v0+v3+v4+v+44880];
					filter_prec_V[indice]=(int16_t)v;
					out_V[indice]=(int16_t)v;
				}
				else
				{
					if ((abs(du1)<=thrs1) && (abs(du3)<=thrs1) && (abs(du1+du3)<=thrs2) &&
						(abs(dv1)<=thrs1) && (abs(dv3)<=thrs1) && (abs(dv1+dv3)<=thrs2))
					{
						int32_t u,v;

						u=(u1+u3) >> 1;
						u=lookup[u0+u1+u3+u+44880];
						filter_prec_U[indice]=(int16_t)u;
						out_U[indice]=(int16_t)u;
						v=(v1+v3) >> 1;
						v=lookup[v0+v1+v3+v+44880];
						filter_prec_V[indice]=(int16_t)v;
						out_V[indice]=(int16_t)v;
					}
					else
					{
						filter_prec_U[indice]=(int16_t)u0;
						out_U[indice]=(int16_t)u0;
						filter_prec_V[indice]=(int16_t)v0;
						out_V[indice]=(int16_t)v0;
					}
				}
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_UV_Mode_1_Pass_1(int16_t *filter_buffer_U[],
	int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
	int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
	int32_t w,uint16_t thrs1, uint16_t thrs2,int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u3,u4,v0,v3,v4;
			int32_t du3,du4,dv3,dv4;

			u4=filter_buffer_U[indice_p2][indice];
			u3=filter_buffer_U[indice_p1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v4=filter_buffer_V[indice_p2][indice];
			v3=filter_buffer_V[indice_p1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du3=u0-u3;
			du4=u0-u4;
			dv3=v0-v3;
			dv4=v0-v4;
			if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
				(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
			{
				int32_t u,v;

				u=(u3+u4) >> 1;
				u=lookup[u0+u3+u4+u+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=(v3+v4) >> 1;
				v=lookup[v0+v3+v4+v+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				filter_prec_U[indice]=(int16_t)u0;
				out_U[indice]=(int16_t)u0;
				filter_prec_V[indice]=(int16_t)v0;
				out_V[indice]=(int16_t)v0;
			}
			indice++;
		}
}



void JPSDR_VHS_III::VHS_Filter_UV_Mode_1_Pass_2(int16_t *filter_buffer_U[],
	int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
	int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
	int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2,indice_m1;
	uint32_t indice;

	indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u3,u4,v0,v3,v4;
			int32_t du3,du4,dv3,dv4;

			u4=filter_buffer_U[indice_p2][indice];
			u3=filter_buffer_U[indice_p1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v4=filter_buffer_V[indice_p2][indice];
			v3=filter_buffer_V[indice_p1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du3=u0-u3;
			du4=u0-u4;
			dv3=v0-v3;
			dv4=v0-v4;
			if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
				(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
			{
				int32_t u,v;

				u=(u3+u4) >> 1;
				u=lookup[u0+u3+u4+u+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=(v3+v4) >> 1;
				v=lookup[v0+v3+v4+v+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				int32_t u1,v1;
				int32_t du1,dv1;

				u1=filter_buffer_U[indice_m1][indice];
				du1=u0-u1;
				v1=filter_buffer_V[indice_m1][indice];
				dv1=v0-v1;
				if ((abs(du1)<=thrs1) && (abs(du3)<=thrs1) && (abs(du1+du3)<=thrs2) &&
					(abs(dv1)<=thrs1) && (abs(dv3)<=thrs1) && (abs(dv1+dv3)<=thrs2))
				{
					int32_t u,v;

					u=(u1+u3) >> 1;
					u=lookup[u0+u1+u3+u+44880];
					filter_prec_U[indice]=(int16_t)u;
					out_U[indice]=(int16_t)u;
					v=(v1+v3) >> 1;
					v=lookup[v0+v1+v3+v+44880];
					filter_prec_V[indice]=(int16_t)v;
					out_V[indice]=(int16_t)v;
				}
				else
				{
					filter_prec_U[indice]=(int16_t)u0;
					out_U[indice]=(int16_t)u0;
					filter_prec_V[indice]=(int16_t)v0;
					out_V[indice]=(int16_t)v0;
				}
			}
			indice++;
		}
}



void JPSDR_VHS_III::VHS_Filter_UV_Mode_2(int16_t *filter_buffer_U[], int16_t *filter_buffer_V[],
	int16_t *filter_prec_U, int16_t *filter_prec_V, int16_t *out_U,
	int16_t *out_V,uint8_t *filter_indice,int32_t h,int32_t w,
	uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_m2,indice_m1,indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_m2=filter_indice[0]; indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u1,u2,v0,v1,v2;
			int32_t du1,du2,dv1,dv2;

			u2=filter_buffer_U[indice_m2][indice];
			u1=filter_buffer_U[indice_m1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v2=filter_buffer_V[indice_m2][indice];
			v1=filter_buffer_V[indice_m1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du1=u0-u1;
			du2=u0-u2;
			dv1=v0-v1;
			dv2=v0-v2;
			if ((abs(du1)<=thrs1) && (abs(du2)<=thrs1) && (abs(du1+du2)<=thrs2) &&
				(abs(dv1)<=thrs1) && (abs(dv2)<=thrs1) && (abs(dv1+dv2)<=thrs2))
			{
				int32_t u,v;

				u=filter_prec_U[indice];
				u=lookup[u0+u1+u2+(u << 2)+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=filter_prec_V[indice];
				v=lookup[v0+v1+v2+(v << 2)+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				int32_t u3,u4,v3,v4;
				int32_t du3,du4,dv3,dv4;

				u3=filter_buffer_U[indice_p1][indice];
				u4=filter_buffer_U[indice_p2][indice];
				v3=filter_buffer_V[indice_p1][indice];
				v4=filter_buffer_V[indice_p2][indice];
				du3=u0-u3;
				du4=u0-u4;
				dv3=v0-v3;
				dv4=v0-v4;
				if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
					(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
				{
					int32_t u,v;

					u=(u3+u4) >> 1;
					u=lookup[u0+u3+u4+(u << 2)+44880];
					filter_prec_U[indice]=(int16_t)u;
					out_U[indice]=(int16_t)u;
					v=(v3+v4) >> 1;
					v=lookup[v0+v3+v4+(v << 2)+44880];
					filter_prec_V[indice]=(int16_t)v;
					out_V[indice]=(int16_t)v;
				}
				else
				{
					if ((abs(du1)<=thrs1) && (abs(du3)<=thrs1) && (abs(du1+du3)<=thrs2) &&
						(abs(dv1)<=thrs1) && (abs(dv3)<=thrs1) && (abs(dv1+dv3)<=thrs2))
					{
						int32_t u,v;

						u=(u1+u3) >> 1;
						u=lookup[u0+u1+u3+(u << 2)+44880];
						filter_prec_U[indice]=(int16_t)u;
						out_U[indice]=(int16_t)u;
						v=(v1+v3) >> 1;
						v=lookup[v0+v1+v3+(v << 2)+44880];
						filter_prec_V[indice]=(int16_t)v;
						out_V[indice]=(int16_t)v;
					}
					else
					{
						filter_prec_U[indice]=(int16_t)u0;
						out_U[indice]=(int16_t)u0;
						filter_prec_V[indice]=(int16_t)v0;
						out_V[indice]=(int16_t)v0;
					}
				}
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_UV_Mode_2_Pass_1(int16_t *filter_buffer_U[],
	int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
	int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
	int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u3,u4,v0,v3,v4;
			int32_t du3,du4,dv3,dv4;

			u4=filter_buffer_U[indice_p2][indice];
			u3=filter_buffer_U[indice_p1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v4=filter_buffer_V[indice_p2][indice];
			v3=filter_buffer_V[indice_p1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du3=u0-u3;
			du4=u0-u4;
			dv3=v0-v3;
			dv4=v0-v4;
			if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
				(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
			{
				int32_t u,v;

				u=(u3+u4) >> 1;
				u=lookup[u0+u3+u4+(u << 2)+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=(v3+v4) >> 1;
				v=lookup[v0+v3+v4+(v << 2)+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				filter_prec_U[indice]=(int16_t)u0;
				out_U[indice]=(int16_t)u0;
				filter_prec_V[indice]=(int16_t)v0;
				out_V[indice]=(int16_t)v0;
			}
			indice++;
		}
}



void JPSDR_VHS_III::VHS_Filter_UV_Mode_2_Pass_2(int16_t *filter_buffer_U[],
	int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
	int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
	int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2,indice_m1;
	uint32_t indice;

	indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u3,u4,v0,v3,v4;
			int32_t du3,du4,dv3,dv4;

			u4=filter_buffer_U[indice_p2][indice];
			u3=filter_buffer_U[indice_p1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v4=filter_buffer_V[indice_p2][indice];
			v3=filter_buffer_V[indice_p1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du3=u0-u3;
			du4=u0-u4;
			dv3=v0-v3;
			dv4=v0-v4;
			if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
				(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
			{
				int32_t u,v;

				u=(u3+u4) >> 1;
				u=lookup[u0+u3+u4+(u << 2)+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=(v3+v4) >> 1;
				v=lookup[v0+v3+v4+(v << 2)+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				int32_t u1,v1;
				int32_t du1,dv1;

				u1=filter_buffer_U[indice_m1][indice];
				du1=u0-u1;
				v1=filter_buffer_V[indice_m1][indice];
				dv1=v0-v1;
				if ((abs(du1)<=thrs1) && (abs(du3)<=thrs1) && (abs(du1+du3)<=thrs2) &&
					(abs(dv1)<=thrs1) && (abs(dv3)<=thrs1) && (abs(dv1+dv3)<=thrs2))
				{
					int32_t u,v;

					u=(u1+u3) >> 1;
					u=lookup[u0+u1+u3+(u << 2)+44880];
					filter_prec_U[indice]=(int16_t)u;
					out_U[indice]=(int16_t)u;
					v=(v1+v3) >> 1;
					v=lookup[v0+v1+v3+(v << 2)+44880];
					filter_prec_V[indice]=(int16_t)v;
					out_V[indice]=(int16_t)v;
				}
				else
				{
					filter_prec_U[indice]=(int16_t)u0;
					out_U[indice]=(int16_t)u0;
					filter_prec_V[indice]=(int16_t)v0;
					out_V[indice]=(int16_t)v0;
				}
			}
			indice++;
		}
}




void JPSDR_VHS_III::VHS_Filter_UV_Mode_3(int16_t *filter_buffer_U[], int16_t *filter_buffer_V[],
	int16_t *filter_prec_U, int16_t *filter_prec_V, int16_t *out_U,
	int16_t *out_V,uint8_t *filter_indice,int32_t h,int32_t w,
	uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_m2,indice_m1,indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_m2=filter_indice[0]; indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u1,u2,v0,v1,v2;
			int32_t du1,du2,dv1,dv2;

			u2=filter_buffer_U[indice_m2][indice];
			u1=filter_buffer_U[indice_m1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v2=filter_buffer_V[indice_m2][indice];
			v1=filter_buffer_V[indice_m1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du1=u0-u1;
			du2=u0-u2;
			dv1=v0-v1;
			dv2=v0-v2;
			if ((abs(du1)<=thrs1) && (abs(du2)<=thrs1) && (abs(du1+du2)<=thrs2) &&
				(abs(dv1)<=thrs1) && (abs(dv2)<=thrs1) && (abs(dv1+dv2)<=thrs2))
			{
				int32_t u,v;

				u=filter_prec_U[indice];
				u=lookup[u0+u1+u2+(u << 3)+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=filter_prec_V[indice];
				v=lookup[v0+v1+v2+(v << 3)+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				int32_t u3,u4,v3,v4;
				int32_t du3,du4,dv3,dv4;

				u3=filter_buffer_U[indice_p1][indice];
				u4=filter_buffer_U[indice_p2][indice];
				v3=filter_buffer_V[indice_p1][indice];
				v4=filter_buffer_V[indice_p2][indice];
				du3=u0-u3;
				du4=u0-u4;
				dv3=v0-v3;
				dv4=v0-v4;
				if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
					(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
				{
					int32_t u,v;

					u=(u3+u4) >> 1;
					u=lookup[u0+u3+u4+(u << 3)+44880];
					filter_prec_U[indice]=(int16_t)u;
					out_U[indice]=(int16_t)u;
					v=(v3+v4) >> 1;
					v=lookup[v0+v3+v4+(v << 3)+44880];
					filter_prec_V[indice]=(int16_t)v;
					out_V[indice]=(int16_t)v;
				}
				else
				{
					if ((abs(du1)<=thrs1) && (abs(du3)<=thrs1) && (abs(du1+du3)<=thrs2) &&
						(abs(dv1)<=thrs1) && (abs(dv3)<=thrs1) && (abs(dv1+dv3)<=thrs2))
					{
						int32_t u,v;

						u=(u1+u3) >> 1;
						u=lookup[u0+u1+u3+(u << 3)+44880];
						filter_prec_U[indice]=(int16_t)u;
						out_U[indice]=(int16_t)u;
						v=(v1+v3) >> 1;
						v=lookup[v0+v1+v3+(v << 3)+44880];
						filter_prec_V[indice]=(int16_t)v;
						out_V[indice]=(int16_t)v;
					}
					else
					{
						filter_prec_U[indice]=(int16_t)u0;
						out_U[indice]=(int16_t)u0;
						filter_prec_V[indice]=(int16_t)v0;
						out_V[indice]=(int16_t)v0;
					}
				}
			}
			indice++;
		}
}


void JPSDR_VHS_III::VHS_Filter_UV_Mode_3_Pass_1(int16_t *filter_buffer_U[],
	int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
	int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
	int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2;
	uint32_t indice;

	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u3,u4,v0,v3,v4;
			int32_t du3,du4,dv3,dv4;

			u4=filter_buffer_U[indice_p2][indice];
			u3=filter_buffer_U[indice_p1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v4=filter_buffer_V[indice_p2][indice];
			v3=filter_buffer_V[indice_p1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du3=u0-u3;
			du4=u0-u4;
			dv3=v0-v3;
			dv4=v0-v4;
			if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
				(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
			{
				int32_t u,v;

				u=(u3+u4) >> 1;
				u=lookup[u0+u3+u4+(u << 3)+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=(v3+v4) >> 1;
				v=lookup[v0+v3+v4+(v << 3)+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				filter_prec_U[indice]=(int16_t)u0;
				out_U[indice]=(int16_t)u0;
				filter_prec_V[indice]=(int16_t)v0;
				out_V[indice]=(int16_t)v0;
			}
			indice++;
		}
}



void JPSDR_VHS_III::VHS_Filter_UV_Mode_3_Pass_2(int16_t *filter_buffer_U[],
	int16_t *filter_buffer_V[],int16_t *filter_prec_U, int16_t *filter_prec_V,
	int16_t *out_U,int16_t *out_V,uint8_t *filter_indice,int32_t h,
	int32_t w,uint16_t thrs1, uint16_t thrs2, int16_t *lookup)
{
	uint8_t indice_0,indice_p1,indice_p2,indice_m1;
	uint32_t indice;

	indice_m1=filter_indice[1];
	indice_0=filter_indice[2];
	indice_p1=filter_indice[3]; indice_p2=filter_indice[4];
	indice=0;
	thrs1<<=4;
	thrs2<<=4;

	for (int32_t i=0; i<h; i++)
		for (int32_t j=0; j<w; j++)
		{
			int32_t u0,u3,u4,v0,v3,v4;
			int32_t du3,du4,dv3,dv4;

			u4=filter_buffer_U[indice_p2][indice];
			u3=filter_buffer_U[indice_p1][indice];
			u0=filter_buffer_U[indice_0][indice];
			v4=filter_buffer_V[indice_p2][indice];
			v3=filter_buffer_V[indice_p1][indice];
			v0=filter_buffer_V[indice_0][indice];
			du3=u0-u3;
			du4=u0-u4;
			dv3=v0-v3;
			dv4=v0-v4;
			if ((abs(du3)<=thrs1) && (abs(du4)<=thrs1) && (abs(du3+du4)<=thrs2) &&
				(abs(dv3)<=thrs1) && (abs(dv4)<=thrs1) && (abs(dv3+dv4)<=thrs2))
			{
				int32_t u,v;

				u=(u3+u4) >> 1;
				u=lookup[u0+u3+u4+(u << 3)+44880];
				filter_prec_U[indice]=(int16_t)u;
				out_U[indice]=(int16_t)u;
				v=(v3+v4) >> 1;
				v=lookup[v0+v3+v4+(v << 3)+44880];
				filter_prec_V[indice]=(int16_t)v;
				out_V[indice]=(int16_t)v;
			}
			else
			{
				int32_t u1,v1;
				int32_t du1,dv1;

				u1=filter_buffer_U[indice_m1][indice];
				du1=u0-u1;
				v1=filter_buffer_V[indice_m1][indice];
				dv1=v0-v1;
				if ((abs(du1)<=thrs1) && (abs(du3)<=thrs1) && (abs(du1+du3)<=thrs2) &&
					(abs(dv1)<=thrs1) && (abs(dv3)<=thrs1) && (abs(dv1+dv3)<=thrs2))
				{
					int32_t u,v;

					u=(u1+u3) >> 1;
					u=lookup[u0+u1+u3+(u << 3)+44880];
					filter_prec_U[indice]=(int16_t)u;
					out_U[indice]=(int16_t)u;
					v=(v1+v3) >> 1;
					v=lookup[v0+v1+v3+(v << 3)+44880];
					filter_prec_V[indice]=(int16_t)v;
					out_V[indice]=(int16_t)v;
				}
				else
				{
					filter_prec_U[indice]=(int16_t)u0;
					out_U[indice]=(int16_t)u0;
					filter_prec_V[indice]=(int16_t)v0;
					out_V[indice]=(int16_t)v0;
				}
			}
			indice++;
		}
}



void JPSDR_VHS_III::Run()
{
	int32_t w_Y,h_Y,w_U,h_U,w_V,h_V;
	ptrdiff_t src_modulo,dst_modulo;
	ptrdiff_t src_modulo_Y,dst_modulo_Y;
	ptrdiff_t src_modulo_U,dst_modulo_U;
	ptrdiff_t src_modulo_V,dst_modulo_V;
	const void *src,*src_Y,*src_U,*src_V;
	void *dst,*dst_Y,*dst_U,*dst_V;
	uint32_t size_Y,size_U,size_V;
	uint8_t indice_in_Y,indice_out_Y,indice_in_UV,indice_out_UV;
	uint8_t c_shift;
	uint8_t i;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;
	const VDXFBitmap& pxsrc2 = fa->src;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			w_Y=image_data.src_w0;
			h_Y=image_data.src_h0;
			w_U=w_Y;
			w_V=w_Y;
			h_U=h_Y;
			h_V=h_Y;
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			w_Y=image_data.src_w0;
			h_Y=image_data.src_h0;
			w_U=(image_data.src_w0+1)>>1;
			w_V=w_U;
			h_U=h_Y;
			h_V=h_Y;
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			w_Y=image_data.src_w0;
			h_Y=image_data.src_h0;
			w_U=image_data.src_w1;
			h_U=image_data.src_h1;
			w_V=image_data.src_w2;
			h_V=image_data.src_h2;
			break;
	}

	src=image_data.src_plane0;
	dst=image_data.dst_plane0;
	src_Y=src;
	dst_Y=dst;
	src_U=image_data.src_plane1;
	dst_U=image_data.dst_plane1;
	src_V=image_data.src_plane2;
	dst_V=image_data.dst_plane2;
	src_modulo=image_data.src_modulo0;
	dst_modulo=image_data.dst_modulo0;
	src_modulo_Y=src_modulo;
	dst_modulo_Y=dst_modulo;
	src_modulo_U=image_data.src_modulo1;
	dst_modulo_U=image_data.dst_modulo1;
	src_modulo_V=image_data.src_modulo2;
	dst_modulo_V=image_data.dst_modulo2;

	size_Y=h_Y*w_Y*sizeof(int16_t);
	size_U=h_U*w_U*sizeof(int16_t);
	size_V=h_V*w_V*sizeof(int16_t);

	indice_in_Y=0; indice_out_Y=1;
	indice_in_UV=0; indice_out_UV=1;

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			if ((SSE2_Enable) && ((w_Y&0x07)==0))
			{
				JPSDR_VHS_III_convert_RGB_YUV_SSE2(src,buffer_Y[0],buffer_U[0],buffer_V[0],w_Y>>3,h_Y,src_modulo,
					lookup_RGB_Y);
			}
			else
			{
				if ((SSE_Integer_Enable) && ((w_Y&0x03)==0))
					JPSDR_VHS_III_convert_RGB_YUV_SSE(src,buffer_Y[0],buffer_U[0],buffer_V[0],w_Y>>2,h_Y,src_modulo,
						lookup_RGB_Y);
				else
					convert_RGB_YUV(src,buffer_Y[0],buffer_U[0],buffer_V[0],w_Y,h_Y,src_modulo,
						lookup_RGB_Y);
			}
			break;
		case VMODE_YUYV :
			convert_YUY2_YUV(src,buffer_Y[0],buffer_U[0],buffer_V[0],(w_Y+1)>>1,h_Y,src_modulo);
			break;
		case VMODE_UYVY :
			convert_UYVY_YUV(src,buffer_Y[0],buffer_U[0],buffer_V[0],(w_Y+1)>>1,h_Y,src_modulo);
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			convert_PlanarY_YUV(src_Y,buffer_Y[0],w_Y,h_Y,src_modulo_Y);
			convert_PlanarUV_YUV(src_U,buffer_U[0],w_U,h_U,src_modulo_U);
			convert_PlanarUV_YUV(src_V,buffer_V[0],w_V,h_V,src_modulo_V);
			break;

	}

	if ((mData.DCTi_enable) && (mData.DCTi_first))
	{
		if (mData.DCTi_Test)
		{
			Digital_Transition_improvement_Test(buffer_U[indice_in_UV],buffer_U[indice_out_UV],w_U,
				h_U,mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
			Digital_Transition_improvement_Test(buffer_V[indice_in_UV],buffer_V[indice_out_UV],w_V,
				h_V,mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
		}
		else
		{
			Digital_Transition_improvement(buffer_U[indice_in_UV],buffer_U[indice_out_UV],w_U,h_U,
				mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
			Digital_Transition_improvement(buffer_V[indice_in_UV],buffer_V[indice_out_UV],w_V,h_V,
				mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
		}
		indice_in_UV=(indice_in_UV+1)%2;
		indice_out_UV=(indice_out_UV+1)%2;
	}

	if ((mData.DTi_enable) && (mData.DTi_first))
	{
		if (mData.DTi_Test)
			Digital_Transition_improvement_Test(buffer_Y[indice_in_Y],buffer_Y[indice_out_Y],w_Y,
				h_Y,mData.DTi_delta1,mData.DTi_delta2,mData.DTi_thrs1,mData.DTi_thrs2);
		else
			Digital_Transition_improvement(buffer_Y[indice_in_Y],buffer_Y[indice_out_Y],w_Y,h_Y,
				mData.DTi_delta1,mData.DTi_delta2,mData.DTi_thrs1,mData.DTi_thrs2);
		indice_in_Y=(indice_in_Y+1)%2;
		indice_out_Y=(indice_out_Y+1)%2;
	}

	if (pxsrc2.mFrameNumber==0)
	{
		memcpy(Y_filtre[2],buffer_Y[indice_in_Y],size_Y);
		memcpy(U_filtre[2],buffer_U[indice_in_UV],size_U);
		memcpy(V_filtre[2],buffer_V[indice_in_UV],size_V);
		return;
	}

	if (pxsrc2.mFrameNumber==1)
	{
		memcpy(Y_filtre[3],buffer_Y[indice_in_Y],size_Y);
		memcpy(U_filtre[3],buffer_U[indice_in_UV],size_U);
		memcpy(V_filtre[3],buffer_V[indice_in_UV],size_V);
		Init_Filter(Y_filtre[2],Y_filtre[3],Y_filtre_prec,h_Y,w_Y);
		Init_Filter(U_filtre[2],U_filtre[3],U_filtre_prec,h_U,w_U);
		Init_Filter(V_filtre[2],V_filtre[3],V_filtre_prec,h_V,w_V);
		return;
	}

	memcpy(Y_filtre[indice_filtre[4]],buffer_Y[indice_in_Y],size_Y);
	memcpy(U_filtre[indice_filtre[4]],buffer_U[indice_in_UV],size_U);
	memcpy(V_filtre[indice_filtre[4]],buffer_V[indice_in_UV],size_V);

	if (mData.Y_filter_enable)
	{
		if (pxsrc2.mFrameNumber==2)
		{
			switch(mData.filter_mode_Y)
			{
			case 1 : VHS_Filter_Y_Mode_1_Pass_1(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2); break;
			case 2 : VHS_Filter_Y_Mode_2_Pass_1(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2,
						lookup_filter_Y); break;
			case 3 : VHS_Filter_Y_Mode_3_Pass_1(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2,
						lookup_filter_Y); break;
			}
		}
		if (pxsrc2.mFrameNumber==3)
		{
			switch(mData.filter_mode_Y)
			{
			case 1 : VHS_Filter_Y_Mode_1_Pass_2(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2); break;
			case 2 : VHS_Filter_Y_Mode_2_Pass_2(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2,
						lookup_filter_Y); break;
			case 3 : VHS_Filter_Y_Mode_3_Pass_2(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2,
						lookup_filter_Y); break;
			}
		}
		if (pxsrc2.mFrameNumber>3)
		{
			switch(mData.filter_mode_Y)
			{
			case 1 : VHS_Filter_Y_Mode_1(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2); break;
			case 2 : VHS_Filter_Y_Mode_2(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2,
						lookup_filter_Y); break;
			case 3 : VHS_Filter_Y_Mode_3(Y_filtre,Y_filtre_prec,buffer_Y[indice_out_Y],
						indice_filtre,h_Y,w_Y,mData.threshold_Y_1,mData.threshold_Y_2,
						lookup_filter_Y); break;
			}
		}
	}
	else
		memcpy(buffer_Y[indice_out_Y],Y_filtre[indice_filtre[2]],size_Y);

	if (mData.UV_filter_enable)
	{
		if (pxsrc2.mFrameNumber==2)
		{
			switch(mData.filter_mode_UV)
			{
			case 1 : VHS_Filter_UV_Mode_1_Pass_1(U_filtre,V_filtre,U_filtre_prec,
						 V_filtre_prec,buffer_U[indice_out_UV],buffer_V[indice_out_UV],
						 indice_filtre,h_U,w_U,mData.threshold_UV_1,mData.threshold_UV_2,
						 lookup_filter_UV); break;
			case 2 : VHS_Filter_UV_Mode_2_Pass_1(U_filtre,V_filtre,U_filtre_prec,
						 V_filtre_prec,buffer_U[indice_out_UV],buffer_V[indice_out_UV],
						 indice_filtre,h_U,w_U,mData.threshold_UV_1,mData.threshold_UV_2,
						 lookup_filter_UV); break;
			case 3 : VHS_Filter_UV_Mode_3_Pass_1(U_filtre,V_filtre,U_filtre_prec,
						 V_filtre_prec,buffer_U[indice_out_UV],buffer_V[indice_out_UV],
						 indice_filtre,h_U,w_U,mData.threshold_UV_1,mData.threshold_UV_2,
						 lookup_filter_UV); break;
			}
		}
		if (pxsrc2.mFrameNumber==3)
		{
			switch(mData.filter_mode_UV)
			{
			case 1 : VHS_Filter_UV_Mode_1_Pass_2(U_filtre,V_filtre,U_filtre_prec,
						 V_filtre_prec,buffer_U[indice_out_UV],buffer_V[indice_out_UV],
						 indice_filtre,h_U,w_U,mData.threshold_UV_1,mData.threshold_UV_2,
						 lookup_filter_UV); break;
			case 2 : VHS_Filter_UV_Mode_2_Pass_2(U_filtre,V_filtre,U_filtre_prec,
						 V_filtre_prec,buffer_U[indice_out_UV],buffer_V[indice_out_UV],
						 indice_filtre,h_U,w_U,mData.threshold_UV_1,mData.threshold_UV_2,
						 lookup_filter_UV); break;
			case 3 : VHS_Filter_UV_Mode_3_Pass_2(U_filtre,V_filtre,U_filtre_prec,
						 V_filtre_prec,buffer_U[indice_out_UV],buffer_V[indice_out_UV],
						 indice_filtre,h_U,w_U,mData.threshold_UV_1,mData.threshold_UV_2,
						 lookup_filter_UV); break;
			}
		}
		if (pxsrc2.mFrameNumber>3)
		{
			switch(mData.filter_mode_UV)
			{
			case 1 : VHS_Filter_UV_Mode_1(U_filtre,V_filtre,U_filtre_prec,V_filtre_prec,
						 buffer_U[indice_out_UV],buffer_V[indice_out_UV],indice_filtre,h_U,w_U,
						 mData.threshold_UV_1,mData.threshold_UV_2,lookup_filter_UV); break;
			case 2 : VHS_Filter_UV_Mode_2(U_filtre,V_filtre,U_filtre_prec,V_filtre_prec,
						 buffer_U[indice_out_UV],buffer_V[indice_out_UV],indice_filtre,h_U,w_U,
						 mData.threshold_UV_1,mData.threshold_UV_2,lookup_filter_UV); break;
			case 3 : VHS_Filter_UV_Mode_3(U_filtre,V_filtre,U_filtre_prec,V_filtre_prec,
						 buffer_U[indice_out_UV],buffer_V[indice_out_UV],indice_filtre,h_U,w_U,
						 mData.threshold_UV_1,mData.threshold_UV_2,lookup_filter_UV); break;
			}
		}
	}
	else
	{
		memcpy(buffer_U[indice_out_UV],U_filtre[indice_filtre[2]],size_U);
		memcpy(buffer_V[indice_out_UV],V_filtre[indice_filtre[2]],size_V);
	}

	indice_in_Y=(indice_in_Y+1)%2;
	indice_out_Y=(indice_out_Y+1)%2;
	indice_in_UV=(indice_in_UV+1)%2;
	indice_out_UV=(indice_out_UV+1)%2;

	if ((mData.DCTi_enable) && (!mData.DCTi_first))
	{
		if (mData.DCTi_Test)
		{
			Digital_Transition_improvement_Test(buffer_U[indice_in_UV],buffer_U[indice_out_UV],w_U,
				h_U,mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
			Digital_Transition_improvement_Test(buffer_V[indice_in_UV],buffer_V[indice_out_UV],w_V,
				h_V,mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
		}
		else
		{
			Digital_Transition_improvement(buffer_U[indice_in_UV],buffer_U[indice_out_UV],w_U,h_U,
				mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
			Digital_Transition_improvement(buffer_V[indice_in_UV],buffer_V[indice_out_UV],w_V,h_V,
				mData.DCTi_delta1,mData.DCTi_delta2,mData.DCTi_thrs1,mData.DCTi_thrs2);
		}
		indice_in_UV=(indice_in_UV+1)%2;
		indice_out_UV=(indice_out_UV+1)%2;
	}

	if ((mData.DTi_enable) && (!mData.DTi_first))
	{
		if (mData.DTi_Test)
			Digital_Transition_improvement_Test(buffer_Y[indice_in_Y],buffer_Y[indice_out_Y],w_Y,
				h_Y,mData.DTi_delta1,mData.DTi_delta2,mData.DTi_thrs1,mData.DTi_thrs2);
		else
			Digital_Transition_improvement(buffer_Y[indice_in_Y],buffer_Y[indice_out_Y],w_Y,h_Y,
				mData.DTi_delta1,mData.DTi_delta2,mData.DTi_thrs1,mData.DTi_thrs2);
		indice_in_Y=(indice_in_Y+1)%2;
		indice_out_Y=(indice_out_Y+1)%2;
	}

	if (mData.chroma_shift_enable)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_PLANAR_YUV444 :
				c_shift=mData.chroma_shift;
				break;
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
				c_shift=(mData.chroma_shift+1)>>1;
				break;
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				c_shift=(mData.chroma_shift+3)>>2;
				break;
		}
		switch(mData.chroma_shift_mode)
		{
			case 1 : 
			{
				JPSDR_VHS_III_shift_left(buffer_U[indice_in_UV],buffer_U[indice_out_UV],w_U,h_U,c_shift);
				JPSDR_VHS_III_shift_left(buffer_V[indice_in_UV],buffer_V[indice_out_UV],w_V,h_V,c_shift);
				break;
			}
			case 2 : 
			{
				JPSDR_VHS_III_shift_right(buffer_U[indice_in_UV],buffer_U[indice_out_UV],w_U,h_U,c_shift);
				JPSDR_VHS_III_shift_right(buffer_V[indice_in_UV],buffer_V[indice_out_UV],w_V,h_V,c_shift);
				break;
			}
		}
		indice_in_UV=(indice_in_UV+1)%2;
		indice_out_UV=(indice_out_UV+1)%2;
	}


	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			if ((SSE2_Enable) && ((w_Y&0x07)==0))
			{
				JPSDR_VHS_III_convert_YUV_RGB_SSE2(dst,buffer_Y[indice_in_Y],buffer_U[indice_in_UV],
					buffer_V[indice_in_UV],w_Y>>3,h_Y,dst_modulo,lookup_YUV_G);
			}
			else
			{
				if ((SSE_Integer_Enable) && ((w_Y&0x03)==0))
					JPSDR_VHS_III_convert_YUV_RGB_SSE(dst,buffer_Y[indice_in_Y],buffer_U[indice_in_UV],
						buffer_V[indice_in_UV],w_Y>>2,h_Y,dst_modulo,lookup_YUV_G);
				else
					convert_YUV_RGB(dst,buffer_Y[indice_in_Y],buffer_U[indice_in_UV],
						buffer_V[indice_in_UV],w_Y,h_Y,dst_modulo,lookup_YUV_G);
			}
			break;
		case VMODE_YUYV :
			convert_YUV_YUY2(dst,buffer_Y[indice_in_Y],buffer_U[indice_in_UV],buffer_V[indice_in_UV],
				(w_Y+1)>>1,h_Y,dst_modulo);
			break;
		case VMODE_UYVY :
			convert_YUV_UYVY(dst,buffer_Y[indice_in_Y],buffer_U[indice_in_UV],buffer_V[indice_in_UV],
				(w_Y+1)>>1,h_Y,dst_modulo);
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			convert_YUV_PlanarY(buffer_Y[indice_in_Y],dst_Y,w_Y,h_Y,dst_modulo_Y);
			convert_YUV_PlanarU(buffer_U[indice_in_UV],dst_U,w_U,h_U,dst_modulo_U);
			convert_YUV_PlanarV(buffer_V[indice_in_UV],dst_V,w_V,h_V,dst_modulo_V);
			break;
	}

	for (i=0; i<5; i++)
		indice_filtre[i]=(indice_filtre[i]+1)%5;
}




void JPSDR_VHS_III::Start()
{
	bool ok;
	int32_t i,j;
	uint32_t size_Y,size_U,size_V;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}
	
	SetImageData(image_data);

	switch(image_data.video_mode)
	{
		case 0 :
		case VMODE_BMP_RGBA :
			size_Y=(image_data.src_h0*image_data.src_w0)*sizeof(int16_t);
			size_U=(image_data.src_h0*image_data.src_w0)*sizeof(int16_t);
			size_V=(image_data.src_h0*image_data.src_w0)*sizeof(int16_t);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			size_Y=(image_data.src_h0*2*((image_data.src_w0+1)>>1))*sizeof(int16_t);
			size_U=(image_data.src_h0*((image_data.src_w0+1)>>1))*sizeof(int16_t);
			size_V=(image_data.src_h0*((image_data.src_w0+1)>>1))*sizeof(int16_t);
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			size_Y=(image_data.src_h0*image_data.src_w0)*sizeof(int16_t);
			size_U=(image_data.src_h1*image_data.src_w1)*sizeof(int16_t);
			size_V=(image_data.src_h2*image_data.src_w2)*sizeof(int16_t);
			break;
	}

	switch(image_data.video_mode)
	{
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
			mData.chroma_shift=2*((mData.chroma_shift+1)>>1);
			break;
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			mData.chroma_shift=4*((mData.chroma_shift+3)>>2);
			break;
	}

	for (i=0; i<5; i++)
	{
		Y_filtre[i]=(int16_t *)malloc(size_Y);
		U_filtre[i]=(int16_t *)malloc(size_U);
		V_filtre[i]=(int16_t *)malloc(size_V);
	}

	Y_filtre_prec=(int16_t *)malloc(size_Y);
	U_filtre_prec=(int16_t *)malloc(size_U);
	V_filtre_prec=(int16_t *)malloc(size_V);

	for (i=0; i<2; i++)
	{
		buffer_Y[i]=(int16_t *)_aligned_malloc(size_Y,ALIGN_SIZE);
		buffer_U[i]=(int16_t *)_aligned_malloc(size_U,ALIGN_SIZE);
		buffer_V[i]=(int16_t *)_aligned_malloc(size_V,ALIGN_SIZE);
	}

	ok=true;

	for (i=0; i<5; i++)
	{
		ok = ok && (Y_filtre[i]!=NULL);
		ok = ok && (U_filtre[i]!=NULL);
		ok = ok && (V_filtre[i]!=NULL);
	}

	ok=ok && (Y_filtre_prec!=NULL);
	ok=ok && (U_filtre_prec!=NULL);
	ok=ok && (V_filtre_prec!=NULL);

	for (i=0; i<2; i++)
	{
		ok=ok && (buffer_Y[i]!=NULL);
		ok=ok && (buffer_U[i]!=NULL);
		ok=ok && (buffer_V[i]!=NULL);
	}

	if (!ok)
	{
		for (i=1; i>=0; i--)
		{
			my_aligned_free(buffer_V[i]);
			my_aligned_free(buffer_U[i]);
			my_aligned_free(buffer_Y[i]);
		}
		myfree(V_filtre_prec);
		myfree(U_filtre_prec);
		myfree(Y_filtre_prec);
		for (i=4; i>=0; i--)
		{
			myfree(V_filtre[i]);
			myfree(U_filtre[i]);
			myfree(Y_filtre[i]);
		}
		ff->ExceptOutOfMemory();
		return;
	}

	switch (mData.filter_mode_Y)
	{
		case 1 : j=(255*16)*4; break;
		case 2 : j=(255*16)*7; break;
		case 3 : j=(255*16)*11; break;
	}
	for (i=0; i<=j; i++)
		switch (mData.filter_mode_Y)
		{
			case 1 : lookup_filter_Y[i]=(int16_t)round((float)i/4.0); break;
			case 2 : lookup_filter_Y[i]=(int16_t)round((float)i/7.0); break;
			case 3 : lookup_filter_Y[i]=(int16_t)round((float)i/11.0); break;
		}
	for (i=j+1; i<=(255*16)*11; i++)
		lookup_filter_Y[i]=(int16_t)(255*16);

	switch (mData.filter_mode_UV)
	{
		case 1 : j=(255*16)*4; break;
		case 2 : j=(255*16)*7; break;
		case 3 : j=(255*16)*11; break;
	}
	for (i=-j; i<=j; i++)
		switch (mData.filter_mode_UV)
		{
			case 1 : lookup_filter_UV[i+44880]=(int16_t)round((float)i/4.0); break;
			case 2 : lookup_filter_UV[i+44880]=(int16_t)round((float)i/7.0); break;
			case 3 : lookup_filter_UV[i+44880]=(int16_t)round((float)i/11.0); break;
		}
	for (i=j+1; i<=(255*16)*11; i++)
		lookup_filter_UV[i+44880]=(int16_t)(255*16);
	for (i=-(255*16)*11; i<-j; i++)
		lookup_filter_UV[i+44880]=(int16_t)(-255*16);

	for (i=0; i<5; i++)
		indice_filtre[i]=(uint8_t)i;
}



void JPSDR_VHS_III::End()
{
	int16_t i;

	for (i=1; i>=0; i--)
	{
		my_aligned_free(buffer_V[i]);
		my_aligned_free(buffer_U[i]);
		my_aligned_free(buffer_Y[i]);
	}
	myfree(V_filtre_prec);
	myfree(U_filtre_prec);
	myfree(Y_filtre_prec);
	for (i=4; i>=0; i--)
	{
		myfree(V_filtre[i]);
		myfree(U_filtre[i]);
		myfree(Y_filtre[i]);
	}
}



void JPSDR_VHS_III::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	unsigned long thrs_Y,thrs_UV,thrs_DCTi,thrs_DTi,data1,data2;

    mData.Y_filter_enable=!!argv[0].asInt();
	mData.UV_filter_enable=!!argv[1].asInt();
    mData.DCTi_enable=!!argv[2].asInt();
    mData.DCTi_first=!!argv[3].asInt();
	mData.DCTi_Test=!!argv[4].asInt();
    mData.DTi_enable=!!argv[5].asInt();
    mData.DTi_first=!!argv[6].asInt();
	mData.DTi_Test=!!argv[7].asInt();
    mData.chroma_shift_enable=!!argv[8].asInt();
	thrs_Y=argv[9].asInt();
	thrs_UV=argv[10].asInt();
	thrs_DCTi=argv[11].asInt();
	thrs_DTi=argv[12].asInt();
	data1=argv[13].asInt();
	data2=argv[14].asInt();
	mData.threshold_Y_1=(uint16_t)((thrs_Y>>16) & 0xFFFF);
	mData.threshold_Y_2=(uint16_t)(thrs_Y & 0xFFFF);
	mData.threshold_UV_1=(uint16_t)((thrs_UV>>16) & 0xFFFF);
	mData.threshold_UV_2=(uint16_t)(thrs_UV & 0xFFFF);
	mData.DCTi_thrs1=(uint16_t)((thrs_DCTi>>16)&0xFFFF);
	mData.DCTi_thrs2=(uint16_t)(thrs_DCTi & 0xFFFF);
	mData.DTi_thrs1=(uint16_t)((thrs_DTi>>16) & 0xFFFF);
	mData.DTi_thrs2=(uint16_t)(thrs_DTi & 0xFFFF);
	mData.filter_mode_Y=(uint8_t)((data1>>24) & 0xFF);
	mData.filter_mode_UV=(uint8_t)((data1>>16) & 0xFF);
	mData.DCTi_delta1=(uint8_t)((data1>>8) & 0xFF);
	mData.DCTi_delta2=(uint8_t)(data1 & 0xFF);
	mData.DTi_delta1=(uint8_t)((data2>>24) & 0xFF);
	mData.DTi_delta2=(uint8_t)((data2>>16) & 0xFF);
	mData.chroma_shift=(uint8_t)((data2>>8) & 0xFF);
	mData.chroma_shift_mode=(uint8_t)(data2 & 0xFF);

}



void JPSDR_VHS_III::GetScriptString(char *buf, int maxlen)
{
	uint32_t thrs_Y,thrs_UV,thrs_DCTi,thrs_DTi,data1,data2;

	thrs_Y=(((uint32_t)mData.threshold_Y_1) << 16) + mData.threshold_Y_2;
	thrs_UV=(((uint32_t)mData.threshold_UV_1) << 16) + mData.threshold_UV_2;
	thrs_DCTi=(((uint32_t)mData.DCTi_thrs1) << 16) + mData.DCTi_thrs2;
	thrs_DTi=(((uint32_t)mData.DTi_thrs1) << 16) + mData.DTi_thrs2;
	data1=(((uint32_t)mData.filter_mode_Y) << 24)+(((uint32_t)mData.filter_mode_UV) << 16)
		+(((uint32_t)mData.DCTi_delta1) << 8)+mData.DCTi_delta2;
	data2=(((uint32_t)mData.DTi_delta1) << 24)+(((uint32_t)mData.DTi_delta2) << 16)
		+(((uint32_t)mData.chroma_shift) << 8)+mData.chroma_shift_mode;

    SafePrintf(buf,maxlen,"Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
		mData.Y_filter_enable,mData.UV_filter_enable,mData.DCTi_enable,mData.DCTi_first,mData.DCTi_Test,
		mData.DTi_enable,mData.DTi_first,mData.DTi_Test,mData.chroma_shift_enable,
		thrs_Y,thrs_UV,thrs_DCTi,thrs_DTi,data1,data2);
}


extern VDXFilterDefinition2 filterDef_JPSDR_VHS_III=
VDXVideoFilterDefinition<JPSDR_VHS_III>("JPSDR","VHS III v2.5.1","Filter to remove VHS noise.[ASM][SSE][SSE2] Optimised. Lag 2");

