#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

#define Number_Max_Lines 2048


extern int g_VFVAPIVersion;


extern "C" void JPSDR_Deinterlace_Blend_Non_MMX_8(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Non_MMX_24(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Non_MMX_32(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);	
extern "C" void JPSDR_Deinterlace_Blend_MMX(const void *src1,const void *src2, void *dst,
		sint32 w,sint32 h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_2(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_2b(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_2c(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3b(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		
extern "C" void JPSDR_Deinterlace_Blend_SSE_3_A_U(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3_U_A(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3_U(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3_A_Ub(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3_U_Ab(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_SSE_3_Ub(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_2(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_2b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_2c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_U(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_Ab(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		
		
extern "C" void JPSDR_Deinterlace_Blend_Tri_MMX(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_Deinterlace_Move32_Half(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo_pitch,ptrdiff_t dst_modulo_pitch);
extern "C" void JPSDR_Deinterlace_Move8_Half(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo_pitch,ptrdiff_t dst_modulo_pitch);
extern "C" void JPSDR_Deinterlace_Move32_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_Deinterlace_Move8_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);

extern "C" void JPSDR_Deinterlace_YadifAbsDiffAvg_SSE(const void *src_a, const void *scr_b, void *dst_abs, void *dst_avg,
		ptrdiff_t src_pitch, ptrdiff_t dst_pitch, int32_t w, int32_t h);
extern "C" void JPSDR_Deinterlace_YadifAbsDiff_SSE(const void *src_a, const void *scr_b, void *dst,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch, int32_t w, int32_t h);
extern "C" void JPSDR_Deinterlace_YadifAvg_SSE(const void *src_a, const void *scr_b, void *dst,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch, int32_t w, int32_t h);


typedef struct _Yadif_Buffer
{
	void *data[3],*_data[3];
	void *absdiff[3],*_absdiff[3];
} Yadif_Buffer;



class JPSDR_DeinterlaceData
{
public:
	uint8_t mode;
	bool keepTop,TFF;
	bool file_mode;
	char filename[1024];

	JPSDR_DeinterlaceData(void);
};



JPSDR_DeinterlaceData::JPSDR_DeinterlaceData(void)
{
	mode=1;
	keepTop=true;
	TFF=true;
	strcpy_s(filename,1024,"");
	file_mode=false;
}



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
	switch (mData.mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_BLEND,BST_CHECKED); break;
		case 2 : CheckDlgButton(mhdlg,IDC_BLEND_TRI,BST_CHECKED); break;
		case 3 :
		case 4 :
			CheckDlgButton(mhdlg,IDC_INTERPOLATE,BST_CHECKED);
			break;
		case 5 :
		case 6 :
			CheckDlgButton(mhdlg,IDC_YADIF,BST_CHECKED);
			break;
	}
	if (mData.TFF) CheckDlgButton(mhdlg,IDC_TFF,BST_CHECKED);
	else CheckDlgButton(mhdlg,IDC_BFF,BST_CHECKED);
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
		EnableWindow(GetDlgItem(mhdlg,IDC_YADIF),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_STATIC_MODE),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_TOP),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);
//		EnableWindow(GetDlgItem(mhdlg,IDC_PREVIEW),false);
	}
	else
	{
		EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_FILENAME),false);
		EnableWindow(GetDlgItem(mhdlg,IDC_BLEND),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_BLEND_TRI),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_INTERPOLATE),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_YADIF),true);
		EnableWindow(GetDlgItem(mhdlg,IDC_STATIC_MODE),true);
//		EnableWindow(GetDlgItem(mhdlg,IDC_PREVIEW),true);
		switch(mData.mode)
		{
			case 3 :
			case 4 :
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);
				break;
			case 5 :
			case 6 :
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),true);
				break;
			default :
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);
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
	mData.keepTop=!!IsDlgButtonChecked(mhdlg,IDC_BOTTOM);
	mData.TFF=!!IsDlgButtonChecked(mhdlg,IDC_TFF);
	if ((mData.mode==3) && (mData.keepTop)) mData.mode=4;
	if ((mData.mode==4) && (!mData.keepTop)) mData.mode=3;
	if ((mData.mode==5) && (!mData.keepTop)) mData.mode=6;
	if ((mData.mode==6) && (mData.keepTop)) mData.mode=5;
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
		case IDC_YADIF :
			mData.mode=5;
			if (mifp && SaveToData()) mifp->RedoSystem();
			RefreshDisplay();
			return true;
		case IDC_BOTTOM :
		case IDC_TOP :
		case IDC_TFF :
		case IDC_BFF :
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


class JPSDR_Deinterlace : public VDXVideoFilter
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
	virtual bool Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	Yadif_Buffer tab_Yadif[3];
	ptrdiff_t Yadif_pitch[3],Yadif_modulo[3];
	void *spatial_Yadif_buffer,*_spatial_Yadif_buffer;
	void *_avg1[3],*_avg2[3],*_avg3[3],*_abs1[3];
	void *avg1[3],*avg2[3],*avg3[3],*abs1[3];
	uint8_t write_index_Yadif,current_frame_Yadif,previous_frame_Yadif,next_frame_Yadif;
	void *buffer0,*buffer1,*buffer2,*_buffer_SSE,*buffer_SSE;
	void *buffer[3][2];
	uint32_t debut[Number_Max_Lines],fin[Number_Max_Lines];
	uint8_t tab_mode[Number_Max_Lines];
	uint16_t nbre_file_mode;
	bool MMX_Enable,SSE_Integer_Enable,SSE2_Enable;

	void Blend_Tri_RGB32(const void *src, void *dst,int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_RGB32(const void *src, void *dst,void *buffer[],int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32(const void *src, void *dst,int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32(const void *src, void *dst,void *buffer[],int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_Planar8(const void *src, void *dst,int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_Planar8(const void *src, void *dst,void *buffer[],int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void YadifAbsDiff(const void *src_a, const void *scr_b, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifAbsDiffAvg(const void *src_a, const void *scr_b, void *dst_abs, void *dst_avg, ptrdiff_t src_pitch,
		ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifAvgKeepTop(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifAvgKeepBottom(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifSpatialPlanarKeepTop(void *buffer, void *dst, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifSpatialPlanarKeepBottom(void *buffer, void *dst, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifTemporalPlanarKeepBottom(const void *src, void *avg1, void *avg2, void *avg3, void *abs1, void *dst, ptrdiff_t src_pitch,
		ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo,
		int32_t w, int32_t h);
	void YadifTemporalPlanarKeepTop(const void *src, void *avg1, void *avg2, void *avg3, void *abs1, void *dst, ptrdiff_t src_pitch,
		ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo,
		int32_t w, int32_t h);
	void YadifSpatiafBufferPlanarKeepTop(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void YadifSpatiafBufferPlanarKeepBottom(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h);
	void AddBorderRGB(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h);
	void AddBorderYUYV(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h);
	void AddBorderUYVY(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h);
	void AddBorderPlanar(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_DeinterlaceData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Deinterlace)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Deinterlace,ScriptConfig,"iis")
VDXVF_END_SCRIPT_METHODS()



bool JPSDR_Deinterlace::Init()
{
	int8_t i,j;	

	buffer0=NULL;
	buffer1=NULL;
	buffer2=NULL;
	for (i=0; i<3; i++)
	{
		for (j=0; j<2; j++)
		{
			buffer[i][j]=NULL;
		}
	}
	for (i=0; i<3; i++)
	{
		Yadif_pitch[i]=0;
		Yadif_modulo[i]=0;
		_avg1[i]=NULL;
		_avg2[i]=NULL;
		_avg3[i]=NULL;
		_abs1[i]=NULL;
		for (j=0; j<3; j++)
		{
			tab_Yadif[i]._data[j]=NULL;
			tab_Yadif[i]._absdiff[j]=NULL;
		}
	}
	_spatial_Yadif_buffer=NULL;
	_buffer_SSE=NULL;

	return(true);
}


uint32 JPSDR_Deinterlace::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

	if ((mData.mode==5) || (mData.mode==6))
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
	else
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
	
	if (!mData.file_mode)
	{
		switch(mData.mode)
		{
			case 2 :
				if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS);
				else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM);
				break;
			case 5 :
			case 6 :
				return(FILTERPARAM_SUPPORTS_ALTFORMATS);
				break;
			default :
				if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS);
				else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM);
				break;
		}
	}
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS);
}


bool JPSDR_Deinterlace::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	if ((!mData.file_mode) && ((mData.mode==5) || (mData.mode==6)))
	{
		prefetcher->PrefetchFrame(0,frame,0);
		prefetcher->PrefetchFrame(0,frame+1,0);
	}
	else prefetcher->PrefetchFrame(0,frame,0);

	return(true);
}


void JPSDR_Deinterlace::Start()
{
	uint32_t u,v;
	uint16_t w,line_file;
	int16_t i,j;
	int32_t offset;
	Image_Data idata;
	FILE *file_in;
	char buffer_in[1024];
	bool buff_ok;

	if ((g_VFVAPIVersion<12) || ((g_VFVAPIVersion<14) && ((mData.mode==5) || (mData.mode==6))))
	{
		ff->Except("This virtualdub version doesn't support this configuration filter !");
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

	if ((idata.src_h0<8) || (idata.src_w0<8))
	{
		ff->Except("Size must be at least 8x8 !");
		return;
	}

	switch (idata.video_mode)
	{
		case 6 :
			if ((idata.src_h0%4)!=0)
			{
				ff->Except("Vertical size must be multiple of 4 for 4:2:0 !");
				return;
			}
			break;
		case 8 :
			if ((idata.src_h0%8)!=0)
			{
				ff->Except("Vertical size must be multiple of 8 for 4:1:0 !");
				return;
			}
			break;
		default :
			if ((idata.src_h0%2)!=0)
			{
				ff->Except("Vertical size must be multiple of 2 !");
				return;
			}
			break;
	}

	MMX_Enable=ff->isMMXEnabled();
	SSE_Integer_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_INTEGER_SSE)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	if (mData.file_mode)
	{
		if (strlen(mData.filename)==0)
		{
			ff->Except("No file selected !");
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

	buff_ok=true;
	for (i=0; i<2; i++)
	{
		switch (idata.video_mode)
		{
			case 0 :
			case 1 :
				buffer[0][i]=(void *)malloc(4*idata.src_w0);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				break;
			case 2 :
			case 3 :
				buffer[0][i]=(void *)malloc(4*((idata.src_w0+1)>>1));
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				buffer[0][i]=(void *)malloc(idata.src_w0);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				buffer[1][i]=(void *)malloc(idata.src_w1);
				buff_ok=buff_ok && (buffer[1][i]!=NULL);
				buffer[2][i]=(void *)malloc(idata.src_w2);
				buff_ok=buff_ok && (buffer[2][i]!=NULL);
				break;
			case 9 :
				buffer[0][i]=(void *)malloc(idata.src_w0);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				break;
		}
	}
	if (!buff_ok)
	{
		for (i=2; i>=0; i--)
		{
			for (j=1; j>=0; j--)
			{
				myfree(buffer[i][j]);
			}
		}
		ff->ExceptOutOfMemory();
		return;
	}

	if (mData.file_mode)
	{
		buffer0=(void *)malloc(idata.src_size0);
		if (idata.src_size1!=0) buffer1=(void *)malloc(idata.src_size1);
		else buffer1=NULL;
		if (idata.src_size2!=0) buffer2=(void *)malloc(idata.src_size2);
		else buffer2=NULL;
		buff_ok=(buffer0!=NULL);
		if (idata.src_size1!=0) buff_ok=buff_ok && (buffer1!=NULL);
		if (idata.src_size2!=0) buff_ok=buff_ok && (buffer2!=NULL);
		if (!buff_ok)
		{
			myfree(buffer2);
			myfree(buffer1);
			myfree(buffer0);
			for (i=2; i>=0; i--)
			{
				for (j=1; j>=0; j--)
				{
					myfree(buffer[i][j]);
				}
			}
			ff->ExceptOutOfMemory();
			return;
		}
	}

	if ((!mData.file_mode) && ((mData.mode==5) || (mData.mode==6)))
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				Yadif_pitch[0]=((((idata.src_w0<<2)+15)>>4)<<4)+32;
				Yadif_modulo[0]=Yadif_pitch[0]-(idata.src_w0<<2);
				for (i=0; i<3; i++)
				{
					tab_Yadif[i]._data[0]=(void *)malloc(idata.src_h0*Yadif_pitch[0]+16);
					tab_Yadif[i]._absdiff[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				}
				_avg1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg2[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg3[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_abs1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_spatial_Yadif_buffer=(void *)malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10)+16);
				break;
			case 2 :
			case 3 :
				Yadif_pitch[0]=((((((idata.src_w0+1)>>1)<<2)+15)>>4)<<4)+32;
				Yadif_modulo[0]=Yadif_pitch[0]-(((idata.src_w0+1)>>1)<<2);
				for (i=0; i<3; i++)
				{
					tab_Yadif[i]._data[0]=(void *)malloc(idata.src_h0*Yadif_pitch[0]+16);
					tab_Yadif[i]._absdiff[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				}
				_avg1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg2[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg3[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_abs1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_spatial_Yadif_buffer=(void *)malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10)+16);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				Yadif_pitch[0]=(((idata.src_w0+15)>>4)<<4)+32;
				Yadif_pitch[1]=(((idata.src_w1+15)>>4)<<4)+32;
				Yadif_pitch[2]=(((idata.src_w2+15)>>4)<<4)+32;
				Yadif_modulo[0]=Yadif_pitch[0]-idata.src_w0;
				Yadif_modulo[1]=Yadif_pitch[1]-idata.src_w1;
				Yadif_modulo[2]=Yadif_pitch[2]-idata.src_w2;
				for (i=0; i<3; i++)
				{
					tab_Yadif[i]._data[0]=(void *)malloc(idata.src_h0*Yadif_pitch[0]+16);
					tab_Yadif[i]._absdiff[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
					tab_Yadif[i]._data[1]=(void *)malloc(idata.src_h1*Yadif_pitch[1]+16);
					tab_Yadif[i]._absdiff[1]=(void *)malloc((idata.src_h1>>1)*Yadif_pitch[1]+16);
					tab_Yadif[i]._data[2]=(void *)malloc(idata.src_h2*Yadif_pitch[2]+16);
					tab_Yadif[i]._absdiff[2]=(void *)malloc((idata.src_h2>>1)*Yadif_pitch[2]+16);
				}
				_avg1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg2[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg3[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_abs1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg1[1]=(void *)malloc((idata.src_h1>>1)*Yadif_pitch[1]+16);
				_avg2[1]=(void *)malloc((idata.src_h1>>1)*Yadif_pitch[1]+16);
				_avg3[1]=(void *)malloc((idata.src_h1>>1)*Yadif_pitch[1]+16);
				_abs1[1]=(void *)malloc((idata.src_h1>>1)*Yadif_pitch[1]+16);
				_avg1[2]=(void *)malloc((idata.src_h2>>1)*Yadif_pitch[2]+16);
				_avg2[2]=(void *)malloc((idata.src_h2>>1)*Yadif_pitch[2]+16);
				_avg3[2]=(void *)malloc((idata.src_h2>>1)*Yadif_pitch[2]+16);
				_abs1[2]=(void *)malloc((idata.src_h2>>1)*Yadif_pitch[2]+16);
				_spatial_Yadif_buffer=(void *)malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10)+16);
				break;
			case 9 :
				Yadif_pitch[0]=(((idata.src_w0+15)>>4)<<4)+32;
				Yadif_modulo[0]=Yadif_pitch[0]-idata.src_w0;
				for (i=0; i<3; i++)
				{
					tab_Yadif[i]._data[0]=(void *)malloc(idata.src_h0*Yadif_pitch[0]+16);
					tab_Yadif[i]._absdiff[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				}
				_avg1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg2[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_avg3[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_abs1[0]=(void *)malloc((idata.src_h0>>1)*Yadif_pitch[0]+16);
				_spatial_Yadif_buffer=(void *)malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10)+16);
				break;
		}
		_buffer_SSE=(void *)malloc(160+16);

		buff_ok=true;
		for (i=0; i<3; i++)
		{
			for (j=0; j<3; j++)
			{
				if (Yadif_pitch[j]!=0)
				{
					buff_ok=buff_ok && (tab_Yadif[i]._data[j]!=NULL);
					buff_ok=buff_ok && (tab_Yadif[i]._absdiff[j]!=NULL);
				}
			}
		}
		for (i=0; i<3; i++)
		{
			if (Yadif_pitch[i]!=0)
			{
				buff_ok=buff_ok && (_avg1[i]!=NULL);
				buff_ok=buff_ok && (_avg2[i]!=NULL);
				buff_ok=buff_ok && (_avg3[i]!=NULL);
				buff_ok=buff_ok && (_abs1[i]!=NULL);
			}
		}
		buff_ok=buff_ok && (_spatial_Yadif_buffer!=NULL);
		buff_ok=buff_ok && (_buffer_SSE!=NULL);

		if (!buff_ok)
		{
			myfree(_buffer_SSE);
			myfree(_spatial_Yadif_buffer);
			for (i=2; i>=0; i--)
			{
				myfree(_abs1[i]);
				myfree(_avg3[i]);
				myfree(_avg2[i]);
				myfree(_avg1[i]);
			}

			for (i=2; i>=0; i--)
			{
				for (j=2; j>=0; j--)
				{
					myfree(tab_Yadif[i]._absdiff[j]);
					myfree(tab_Yadif[i]._data[j]);
				}
			}
			myfree(buffer2);
			myfree(buffer1);
			myfree(buffer0);
			for (i=2; i>=0; i--)
			{
				for (j=1; j>=0; j--)
				{
					myfree(buffer[i][j]);
				}
			}
			ff->ExceptOutOfMemory();
			return;
		}
		for (i=0; i<3; i++)
		{
			for (j=0; j<3; j++)
			{
				if (Yadif_pitch[j]!=0)
				{
					tab_Yadif[i].data[j]=(void *)(((size_t)tab_Yadif[i]._data[j]+15) & ~(size_t)15);
					tab_Yadif[i].data[j]=(void *)((size_t)tab_Yadif[i].data[j]+16);
					tab_Yadif[i].absdiff[j]=(void *)(((size_t)tab_Yadif[i]._absdiff[j]+15) & ~(size_t)15);
					tab_Yadif[i].absdiff[j]=(void *)((size_t)tab_Yadif[i].absdiff[j]+16);
				}
			}
		}
		for (i=0; i<3; i++)
		{
			if (Yadif_pitch[i]!=0)
			{
				avg1[i]=(void *)(((size_t)_avg1[i]+15) & ~(size_t)15);
				avg2[i]=(void *)(((size_t)_avg2[i]+15) & ~(size_t)15);
				avg3[i]=(void *)(((size_t)_avg3[i]+15) & ~(size_t)15);
				abs1[i]=(void *)(((size_t)_abs1[i]+15) & ~(size_t)15);
				avg1[i]=(void *)((size_t)avg1[i]+16);
				avg2[i]=(void *)((size_t)avg2[i]+16);
				avg3[i]=(void *)((size_t)avg3[i]+16);
				abs1[i]=(void *)((size_t)abs1[i]+16);
			}
		}
		spatial_Yadif_buffer=(void *)(((size_t)_spatial_Yadif_buffer+15) & ~(size_t)15);
		buffer_SSE=(void *)(((size_t)_buffer_SSE+15) & ~(size_t)15);
	}

	write_index_Yadif=0;
	current_frame_Yadif=1;
	previous_frame_Yadif=0;
	next_frame_Yadif=2;
}
	

void JPSDR_Deinterlace::End()
{
	int8_t i,j;

	myfree(_buffer_SSE);
	myfree(_spatial_Yadif_buffer);
	for (i=2; i>=0; i--)
	{
		myfree(_abs1[i]);
		myfree(_avg3[i]);
		myfree(_avg2[i]);
		myfree(_avg1[i]);
	}

	for (i=2; i>=0; i--)
	{
		Yadif_pitch[i]=0;
		Yadif_modulo[i]=0;
		for (j=2; j>=0; j--)
		{
			myfree(tab_Yadif[i]._absdiff[j]);
			myfree(tab_Yadif[i]._data[j]);
		}
	}
	myfree(buffer2);
	myfree(buffer1);
	myfree(buffer0);
	for (i=2; i>=0; i--)
	{
		for (j=1; j>=0; j--)
		{
			myfree(buffer[i][j]);
		}
	}
}



#define fastmin(a, b) ((a) + (((b) - (a)) & (((b) - (a)) >> 31)))
#define fastmax(a, b) ((b) - (((b) - (a)) & (((b) - (a)) >> 31)))
#define fastminmax(a, b, mn, mx) ((t = (b) - (a)), (t &= t >> 31), ((mn) = (a) + t), ((mx) = (b) - t))


void JPSDR_Deinterlace::YadifTemporalPlanarKeepBottom(const void *src, void *_avg1, void *_avg2, void *_avg3,void *_abs1, void *_dst,
	ptrdiff_t src_pitch, ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,ptrdiff_t dst_pitch,
	ptrdiff_t dst_modulo, int32_t w, int32_t _h)
{
	const uint8_t *srcT,*srcB;
	uint8_t *avgT,*avgC,*avgB;
	uint8_t *avg2,*avg3,*abs1;
	uint8_t *dst;
	int32_t h;

	srcT=(uint8_t *)src;
	srcB=srcT+(src_pitch<<1);
	avgT=(uint8_t *)_avg1;
	avgC=avgT+buffer_pitch;
	avgB=avgC+buffer_pitch;
	avg2=(uint8_t *)_avg2+buffer_pitch;
	avg3=(uint8_t *)_avg3+buffer_pitch;
	abs1=(uint8_t *)_abs1+buffer_pitch;
	dst=(uint8_t *)_dst+(dst_pitch<<1);
	h=_h-2;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int32_t p0,p1,p2,p3,p4;
			int32_t tdiff,tdiff0,tdiff1,tdiff2;
			int32_t d12,d10,d32,d34,min_32_12,max_32_12,max_10_34,min_10_34,lo,hi,t;
			int32_t min,max,spatial_pred;

			p0=*avgT++;
			p1=*srcT++;
			p2=*avgC++;
			p3=*srcB++;
			p4=*avgB++;
			tdiff0=*abs1++;
			tdiff1=*avg2++;
			tdiff2=*avg3++;
			d12=p2-p1;
			d10=p0-p1;
			d32=p2-p3;
			d34=p4-p3;

			tdiff=fastmax(tdiff0,fastmax(tdiff1,tdiff2));
			fastminmax(d32, d12, min_32_12, max_32_12);
			fastminmax(d10, d34, min_10_34, max_10_34);
			max=fastmax(max_32_12, min_10_34);
			min=fastmin(min_32_12, max_10_34);
			tdiff=fastmax(fastmax(tdiff, min),-max);

			lo=p2-tdiff;
			hi=p2+tdiff;
			spatial_pred=*dst;
			spatial_pred=fastmin(hi,spatial_pred);
			spatial_pred=fastmax(lo,spatial_pred);

			*dst++=(uint8_t)spatial_pred;
		}
		srcT+=(src_pitch+src_modulo);
		srcB+=(src_pitch+src_modulo);
		dst+=(dst_pitch+dst_modulo);
		avgT+=buffer_modulo;
		avgC+=buffer_modulo;
		avgB+=buffer_modulo;
		avg2+=buffer_modulo;
		avg3+=buffer_modulo;
		abs1+=buffer_modulo;
	}
}



void JPSDR_Deinterlace::YadifTemporalPlanarKeepTop(const void *src, void *_avg1, void *_avg2, void *_avg3,void *_abs1, void *_dst,
	ptrdiff_t src_pitch, ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,ptrdiff_t dst_pitch,
	ptrdiff_t dst_modulo, int32_t w, int32_t _h)
{
	const uint8_t *srcT,*srcB;
	uint8_t *avgT,*avgC,*avgB;
	uint8_t *avg2,*avg3,*abs1;
	uint8_t *dst;
	int32_t h;

	srcT=(uint8_t *)src+(src_pitch<<1);
	srcB=srcT+(src_pitch<<1);
	avgT=(uint8_t *)_avg1;
	avgC=avgT+buffer_pitch;
	avgB=avgC+buffer_pitch;
	avg2=(uint8_t *)_avg2+buffer_pitch;
	avg3=(uint8_t *)_avg3+buffer_pitch;
	abs1=(uint8_t *)_abs1+buffer_pitch;
	dst=(uint8_t *)_dst+(dst_pitch<<1);
	h=_h-2;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int32_t p0,p1,p2,p3,p4;
			int32_t tdiff,tdiff0,tdiff1,tdiff2;
			int32_t d12,d10,d32,d34,max_32_12,max_10_34,min_10_34,min_32_12,lo,hi,t;
			int32_t min,max,spatial_pred;

			p0=*avgT++;
			p1=*srcT++;
			p2=*avgC++;
			p3=*srcB++;
			p4=*avgB++;
			tdiff0=*abs1++;
			tdiff1=*avg2++;
			tdiff2=*avg3++;
			d12=p2-p1;
			d10=p0-p1;
			d32=p2-p3;
			d34=p4-p3;

			tdiff=fastmax(tdiff0,fastmax(tdiff1,tdiff2));
			fastminmax(d32, d12, min_32_12, max_32_12);
			fastminmax(d10, d34, min_10_34, max_10_34);
			max=fastmax(max_32_12, min_10_34);
			min=fastmin(min_32_12, max_10_34);
			tdiff=fastmax(fastmax(tdiff, min),-max);

			lo=p2-tdiff;
			hi=p2+tdiff;
			spatial_pred=*dst;
			spatial_pred=fastmin(hi,spatial_pred);
			spatial_pred=fastmax(lo,spatial_pred);

			*dst++=(uint8_t)spatial_pred;
		}
		srcT+=(src_pitch+src_modulo);
		srcB+=(src_pitch+src_modulo);
		dst+=(dst_pitch+dst_modulo);
		avgT+=buffer_modulo;
		avgC+=buffer_modulo;
		avgB+=buffer_modulo;
		avg2+=buffer_modulo;
		avg3+=buffer_modulo;
		abs1+=buffer_modulo;
	}
}

#undef fastminmax
#undef fastmax
#undef fastmin




void JPSDR_Deinterlace::YadifSpatiafBufferPlanarKeepTop(const void *src, void *_dst, ptrdiff_t src_pitch,
	ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	int32_t h_1,w_2;
	const uint8_t *src1,*src2;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+(src_pitch<<1);
	h_1=h-1;
	w_2=w+2;
	dst=(uint8_t *)_dst;

	for (int32_t i=0; i<h_1; i++)
	{
		uint32_t offset;

		offset=0;
		for (int32_t j=-2; j<w_2; j++)
		{
			dst[offset]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
			dst[offset+1]=(uint8_t)abs((int16_t)src1[j]-(int16_t)src2[j]);
			dst[offset+2]=(uint8_t)(((uint16_t)src1[j-1]+(uint16_t)src2[j+1]+1)>>1);
			dst[offset+3]=(uint8_t)abs((int16_t)src1[j-1]-(int16_t)src2[j+1]);
			dst[offset+4]=(uint8_t)(((uint16_t)src1[j-2]+(uint16_t)src2[j+2]+1)>>1);
			dst[offset+5]=(uint8_t)abs((int16_t)src1[j-2]-(int16_t)src2[j+2]);
			dst[offset+6]=(uint8_t)(((uint16_t)src1[j+1]+(uint16_t)src2[j-1]+1)>>1);
			dst[offset+7]=(uint8_t)abs((int16_t)src1[j+1]-(int16_t)src2[j-1]);
			dst[offset+8]=(uint8_t)(((uint16_t)src1[j+2]+(uint16_t)src2[j-2]+1)>>1);
			dst[offset+9]=(uint8_t)abs((int16_t)src1[j+2]-(int16_t)src2[j-2]);
			offset+=10;
		}
		src1+=(src_pitch<<1);
		src2+=(src_pitch<<1);
		dst+=dst_pitch;
	}
}


void JPSDR_Deinterlace::YadifSpatiafBufferPlanarKeepBottom(const void *src, void *_dst, ptrdiff_t src_pitch,
	ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	int32_t h_1,w_2;
	const uint8_t *src1,*src2;
	uint8_t *dst;

	src1=(uint8_t *)src+src_pitch;
	src2=src1+(src_pitch<<1);
	h_1=h-1;
	w_2=w+2;
	dst=(uint8_t *)_dst;

	for (int32_t i=0; i<h_1; i++)
	{
		uint32_t offset;

		offset=0;
		for (int32_t j=-2; j<w_2; j++)
		{
			dst[offset]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
			dst[offset+1]=(uint8_t)abs((int16_t)src1[j]-(int16_t)src2[j]);
			dst[offset+2]=(uint8_t)(((uint16_t)src1[j-1]+(uint16_t)src2[j+1]+1)>>1);
			dst[offset+3]=(uint8_t)abs((int16_t)src1[j-1]-(int16_t)src2[j+1]);
			dst[offset+4]=(uint8_t)(((uint16_t)src1[j-2]+(uint16_t)src2[j+2]+1)>>1);
			dst[offset+5]=(uint8_t)abs((int16_t)src1[j-2]-(int16_t)src2[j+2]);
			dst[offset+6]=(uint8_t)(((uint16_t)src1[j+1]+(uint16_t)src2[j-1]+1)>>1);
			dst[offset+7]=(uint8_t)abs((int16_t)src1[j+1]-(int16_t)src2[j-1]);
			dst[offset+8]=(uint8_t)(((uint16_t)src1[j+2]+(uint16_t)src2[j-2]+1)>>1);
			dst[offset+9]=(uint8_t)abs((int16_t)src1[j+2]-(int16_t)src2[j-2]);
			offset+=10;
		}
		src1+=(src_pitch<<1);
		src2+=(src_pitch<<1);
		dst+=dst_pitch;
	}
}



void JPSDR_Deinterlace::YadifSpatialPlanarKeepTop(void *_buffer, void *_dst, ptrdiff_t buffer_pitch,
	ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	int32_t h_1;
	uint8_t *buffer,*dst;
	uint32_t offset;

	dst=(uint8_t *)_dst;
	buffer=(uint8_t *)_buffer;
	h_1=h-1;

	for (int32_t i=0; i<h_1; i++)
	{
		offset=20;
		for (int32_t j=0; j<w; j++)
		{
			uint16_t spatial_pred, spatial_score,score;

			spatial_pred=buffer[offset];
			spatial_score=buffer[offset+1-10]+buffer[offset+1]+buffer[offset+1+10];
			score=buffer[offset+3-10]+buffer[offset+3]+buffer[offset+3+10];
			if (score<spatial_score)
			{
				spatial_pred=buffer[offset+2];
				spatial_score=score;
				score=buffer[offset+5-10]+buffer[offset+5]+buffer[offset+5+10];
				if (score<spatial_score)
				{
					spatial_pred=buffer[offset+4];
					spatial_score=score;
				}
			}
			score=buffer[offset+7-10]+buffer[offset+7]+buffer[offset+7+10];
			if (score<spatial_score)
			{
				spatial_pred=buffer[offset+6];
				spatial_score=score;
				score=buffer[offset+9-10]+buffer[offset+9]+buffer[offset+9+10];
				if (score<spatial_score) spatial_pred=buffer[offset+8];
			}
			dst[j]=(uint8_t)spatial_pred;
			offset+=10;
		}
		dst+=(dst_pitch<<1);
		buffer+=buffer_pitch;
	}

	buffer-=buffer_pitch;
	offset=20;
	for (int32_t j=0; j<w; j++)
	{
		dst[j]=(uint8_t)(((uint16_t)dst[j]+(uint16_t)buffer[offset]+1)>>1);
		offset+=10;
	}
}


void JPSDR_Deinterlace::YadifSpatialPlanarKeepBottom(void *_buffer, void *_dst, ptrdiff_t buffer_pitch,
	ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	int32_t h_1;
	uint8_t *buffer,*dst;
	uint32_t offset;

	dst=(uint8_t *)_dst;
	buffer=(uint8_t *)_buffer;
	h_1=h-1;

	offset=20;
	for (int32_t j=0; j<w; j++)
	{
		dst[j]=(uint8_t)(((uint16_t)dst[j]+(uint16_t)buffer[offset]+1)>>1);
		offset+=10;
	}
	dst+=(dst_pitch<<1);

	for (int32_t i=0; i<h_1; i++)
	{
		offset=20;
		for (int32_t j=0; j<w; j++)
		{
			uint16_t spatial_pred, spatial_score,score;

			spatial_pred=buffer[offset];
			spatial_score=buffer[offset+1-10]+buffer[offset+1]+buffer[offset+1+10];
			score=buffer[offset+3-10]+buffer[offset+3]+buffer[offset+3+10];
			if (score<spatial_score)
			{
				spatial_pred=buffer[offset+2];
				spatial_score=score;
				score=buffer[offset+5-10]+buffer[offset+5]+buffer[offset+5+10];
				if (score<spatial_score)
				{
					spatial_pred=buffer[offset+4];
					spatial_score=score;
				}
			}
			score=buffer[offset+7-10]+buffer[offset+7]+buffer[offset+7+10];
			if (score<spatial_score)
			{
				spatial_pred=buffer[offset+6];
				spatial_score=score;
				score=buffer[offset+9-10]+buffer[offset+9]+buffer[offset+9+10];
				if (score<spatial_score) spatial_pred=buffer[offset+8];
			}
			dst[j]=(uint8_t)spatial_pred;
			offset+=10;
		}
		dst+=(dst_pitch<<1);
		buffer+=buffer_pitch;
	}
}



void JPSDR_Deinterlace::YadifAbsDiffAvg(const void *src_a, const void *src_b, void *_dst_abs, void *_dst_avg,
	ptrdiff_t src_pitch, ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	const uint8_t *src1,*src2;
	uint8_t *dst_avg,*dst_abs;

	if (SSE2_Enable)
	{
		JPSDR_Deinterlace_YadifAbsDiffAvg_SSE(src_a,src_b,_dst_abs,_dst_avg,src_pitch<<1,dst_pitch,(w+15)>>4,h);
		return;
	}

	src1=(uint8_t *)src_a;
	src2=(uint8_t *)src_b;
	dst_avg=(uint8_t *)_dst_avg;
	dst_abs=(uint8_t *)_dst_abs;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t d1,d2;

			d1=(int16_t)src1[j];
			d2=(int16_t)src2[j];
			dst_avg[j]=(uint8_t)((d1+d2+1)>>1);
			dst_abs[j]=(uint8_t)abs(d1-d2);
		}
		dst_avg+=dst_pitch;
		dst_abs+=dst_pitch;
		src1+=(src_pitch<<1);
		src2+=(src_pitch<<1);
	}
}



void JPSDR_Deinterlace::YadifAbsDiff(const void *src_a, const void *src_b, void *_dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	const uint8_t *src1,*src2;
	uint8_t *dst;

	if (SSE2_Enable)
	{
		JPSDR_Deinterlace_YadifAbsDiff_SSE(src_a,src_b,_dst,src_pitch<<1,dst_pitch,(w+15)>>4,h);
		return;
	}

	src1=(uint8_t *)src_a;
	src2=(uint8_t *)src_b;
	dst=(uint8_t *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j]=(uint8_t)abs((int16_t)src1[j]-(int16_t)src2[j]);
		}
		dst+=dst_pitch;
		src1+=(src_pitch<<1);
		src2+=(src_pitch<<1);
	}
}


void JPSDR_Deinterlace::YadifAvgKeepBottom(const void *src, void *_dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	const uint8_t *src1,*src2;
	uint8_t *dst;
	int32_t h_1;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	dst=(uint8_t *)_dst;
	h_1=h-1;

	memcpy(dst,src1,w);
	dst+=dst_pitch;

	if (SSE2_Enable)
	{
		JPSDR_Deinterlace_YadifAvg_SSE(src1,src2,dst,src_pitch,dst_pitch,(w+15)>>4,h_1);
		return;
	}

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
		}
		dst+=dst_pitch;
		src1+=src_pitch;
		src2+=src_pitch;
	}
}


void JPSDR_Deinterlace::YadifAvgKeepTop(const void *src, void *_dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
	ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, int32_t w, int32_t h)
{
	const uint8_t *src1,*src2;
	uint8_t *dst;
	int32_t h_1;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	dst=(uint8_t *)_dst;
	h_1=h-1;

	if (SSE2_Enable)
	{
		JPSDR_Deinterlace_YadifAvg_SSE(src1,src2,dst,src_pitch,dst_pitch,(w+15)>>4,h_1);
		dst+=h_1*dst_pitch;
		src1+=h_1*src_pitch;
		memcpy(dst,src1,w);
		return;
	}

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
		}
		dst+=dst_pitch;
		src1+=src_pitch;
		src2+=src_pitch;
	}
	memcpy(dst,src1,w);
}


void JPSDR_Deinterlace::AddBorderRGB(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h)
{
	RGB32BMP *ptr;

	ptr=(RGB32BMP *)dst;
	for (int32_t i=0; i<h; i++)
	{
		RGB32BMP data;

		data=*ptr;
		ptr[-1]=data;
		ptr[-2]=data;
		ptr[-3]=data;
		ptr[-4]=data;
		data=ptr[w-1];
		ptr[w]=data;
		ptr[w+1]=data;
		ptr[w+2]=data;
		ptr[w+3]=data;
		ptr=(RGB32BMP *)((uint8_t *)ptr + dst_pitch);
	}
}


void JPSDR_Deinterlace::AddBorderYUYV(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h)
{
	YUYV *ptr;
	int32_t w0;

	w0=(w+1)>>1;
	ptr=(YUYV *)dst;
	for (int32_t i=0; i<h; i++)
	{
		YUYV data;

		data=*ptr;
		data.y2=data.y1;
		ptr[-1]=data;
		ptr[-2]=data;
		ptr[-3]=data;
		ptr[-4]=data;
		data=ptr[w0-1];
		data.y1=data.y2;
		ptr[w0]=data;
		ptr[w0+1]=data;
		ptr[w0+2]=data;
		ptr[w0+3]=data;
		ptr=(YUYV *)((uint8_t *)ptr + dst_pitch);
	}
}


void JPSDR_Deinterlace::AddBorderUYVY(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h)
{
	UYVY *ptr;
	int32_t w0;

	w0=(w+1)>>1;
	ptr=(UYVY *)dst;
	for (int32_t i=0; i<h; i++)
	{
		UYVY data;

		data=*ptr;
		data.y2=data.y1;
		ptr[-1]=data;
		ptr[-2]=data;
		ptr[-3]=data;
		ptr[-4]=data;
		data=ptr[w0-1];
		data.y1=data.y2;
		ptr[w0]=data;
		ptr[w0+1]=data;
		ptr[w0+2]=data;
		ptr[w0+3]=data;
		ptr=(UYVY *)((uint8_t *)ptr + dst_pitch);
	}
}


void JPSDR_Deinterlace::AddBorderPlanar(void *dst,ptrdiff_t dst_pitch, int32_t w, int32_t h)
{
	uint8_t *ptr;

	ptr=(uint8_t *)dst;
	for (int32_t i=0; i<h; i++)
	{
		uint8_t data;

		data=*ptr;
		ptr[-1]=data;
		ptr[-2]=data;
		ptr[-3]=data;
		ptr[-4]=data;
		ptr[-5]=data;
		ptr[-6]=data;
		ptr[-7]=data;
		ptr[-8]=data;
		ptr[-9]=data;
		ptr[-10]=data;
		ptr[-11]=data;
		ptr[-12]=data;
		ptr[-13]=data;
		ptr[-14]=data;
		ptr[-15]=data;
		ptr[-16]=data;
		data=ptr[w-1];
		ptr[w]=data;
		ptr[w+1]=data;
		ptr[w+2]=data;
		ptr[w+3]=data;
		ptr[w+4]=data;
		ptr[w+5]=data;
		ptr[w+6]=data;
		ptr[w+7]=data;
		ptr[w+8]=data;
		ptr[w+9]=data;
		ptr[w+10]=data;
		ptr[w+11]=data;
		ptr[w+12]=data;
		ptr[w+13]=data;
		ptr[w+14]=data;
		ptr[w+15]=data;
		ptr+=dst_pitch;
	}
}

		
void JPSDR_Deinterlace::Blend_Tri_RGB32(const void *src,void *_dst,int32_t w,int32_t h,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst->r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst->g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst->b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst->alpha=src1[j].alpha;
		dst++;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->r=(((uint16_t)src1->r+(uint16_t)src3->r+(((uint16_t)src2->r) << 1))+2) >> 2;
			dst->g=(((uint16_t)src1->g+(uint16_t)src3->g+(((uint16_t)src2->g) << 1))+2) >> 2;
			dst->b=(((uint16_t)src1->b+(uint16_t)src3->b+(((uint16_t)src2->b) << 1))+2) >> 2;
			dst->alpha=src2->alpha;
			src1++;
			src2++;
			src3++;
			dst++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_modulo);
		src2=(RGB32BMP *)((uint8_t *)src2+src_modulo);
		src3=(RGB32BMP *)((uint8_t*)src3+src_modulo);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst->r=((uint16_t)src1->r+(uint16_t)src2->r+1)>>1;
		dst->g=((uint16_t)src1->g+(uint16_t)src2->g+1)>>1;
		dst->b=((uint16_t)src1->b+(uint16_t)src2->b+1)>>1;
		dst->alpha=src2->alpha;
		src1++;
		src2++;
		dst++;
	}

}


void JPSDR_Deinterlace::Blend_Tri_RGB32(const void *src,void *_dst,void *buffer[],int32_t w,int32_t h,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;
	uint8_t indice_buffer;

	src2=(RGB32BMP *)((uint8_t *)src+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	memcpy(buffer[0],src,w << 2);
	indice_buffer=1;

	src1=(RGB32BMP*)buffer[0];
	for (int32_t j=0; j<w; j++)
	{
		dst->r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst->g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst->b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst->alpha=src1[j].alpha;
		dst++;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 2);
		indice_buffer=(indice_buffer+1)%2;
		src1=(RGB32BMP*)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst->r=(((uint16_t)src1->r+(uint16_t)src3->r+(((uint16_t)src2->r) << 1))+2) >> 2;
			dst->g=(((uint16_t)src1->g+(uint16_t)src3->g+(((uint16_t)src2->g) << 1))+2) >> 2;
			dst->b=(((uint16_t)src1->b+(uint16_t)src3->b+(((uint16_t)src2->b) << 1))+2) >> 2;
			dst->alpha=src2->alpha;
			src1++;
			src2++;
			src3++;
			dst++;
		}
		src2=(RGB32BMP *)((uint8_t *)src2+src_modulo);
		src3=(RGB32BMP *)((uint8_t*)src3+src_modulo);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(RGB32BMP*)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst->r=((uint16_t)src1->r+(uint16_t)src2->r+1)>>1;
		dst->g=((uint16_t)src1->g+(uint16_t)src2->g+1)>>1;
		dst->b=((uint16_t)src1->b+(uint16_t)src2->b+1)>>1;
		dst->alpha=src2->alpha;
		src1++;
		src2++;
		dst++;
	}

}


void JPSDR_Deinterlace::Blend_Tri_YUV32(const void *src,void *_dst,int32_t w,int32_t h,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst->y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst->u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst->y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst->v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
		dst++;
	}
	dst=(YUYV *)((uint8_t *)dst+dst_modulo);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->y1=(((uint16_t)src1->y1+(uint16_t)src3->y1+(((uint16_t)src2->y1) << 1))+2) >> 2;
			dst->u=(((uint16_t)src1->u+(uint16_t)src3->u+(((uint16_t)src2->u) << 1))+2) >> 2;
			dst->y2=(((uint16_t)src1->y2+(uint16_t)src3->y2+(((uint16_t)src2->y2) << 1))+2) >> 2;
			dst->v=(((uint16_t)src1->v+(uint16_t)src3->v+(((uint16_t)src2->v) << 1))+2) >> 2;
			src1++;
			src2++;
			src3++;
			dst++;
		}
		src1=(YUYV *)((uint8_t *)src1+src_modulo);
		src2=(YUYV *)((uint8_t *)src2+src_modulo);
		src3=(YUYV *)((uint8_t*)src3+src_modulo);
		dst=(YUYV *)((uint8_t *)dst+dst_modulo);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst->y1=((uint16_t)src1->y1+(uint16_t)src2->y1+1)>>1;
		dst->u=((uint16_t)src1->u+(uint16_t)src2->u+1)>>1;
		dst->y2=((uint16_t)src1->y2+(uint16_t)src2->y2+1)>>1;
		dst->v=((uint16_t)src1->v+(uint16_t)src2->v+1)>>1;
		src1++;
		src2++;
		dst++;
	}

}



void JPSDR_Deinterlace::Blend_Tri_YUV32(const void *src,void *_dst,void *buffer[],int32_t w,int32_t h,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst;
	uint8_t indice_buffer;

	src2=(YUYV *)((uint8_t *)src+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	dst=(YUYV *)_dst;
	memcpy(buffer[0],src,w << 2);
	indice_buffer=1;

	src1=(YUYV*)buffer[0];
	for (int32_t j=0; j<w; j++)
	{
		dst->y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst->u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst->y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst->v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
		dst++;
	}
	dst=(YUYV *)((uint8_t *)dst+dst_modulo);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 2);
		indice_buffer=(indice_buffer+1)%2;
		src1=(YUYV*)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst->y1=(((uint16_t)src1->y1+(uint16_t)src3->y1+(((uint16_t)src2->y1) << 1))+2) >> 2;
			dst->u=(((uint16_t)src1->u+(uint16_t)src3->u+(((uint16_t)src2->u) << 1))+2) >> 2;
			dst->y2=(((uint16_t)src1->y2+(uint16_t)src3->y2+(((uint16_t)src2->y2) << 1))+2) >> 2;
			dst->v=(((uint16_t)src1->v+(uint16_t)src3->v+(((uint16_t)src2->v) << 1))+2) >> 2;
			src1++;
			src2++;
			src3++;
			dst++;
		}
		src2=(YUYV *)((uint8_t *)src2+src_modulo);
		src3=(YUYV *)((uint8_t*)src3+src_modulo);
		dst=(YUYV *)((uint8_t *)dst+dst_modulo);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(YUYV*)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst->y1=((uint16_t)src1->y1+(uint16_t)src2->y1+1)>>1;
		dst->u=((uint16_t)src1->u+(uint16_t)src2->u+1)>>1;
		dst->y2=((uint16_t)src1->y2+(uint16_t)src2->y2+1)>>1;
		dst->v=((uint16_t)src1->v+(uint16_t)src2->v+1)>>1;
		src1++;
		src2++;
		dst++;
	}

}



void JPSDR_Deinterlace::Blend_Tri_Planar8(const void *src,void *_dst,int32_t w,int32_t h,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		*dst++=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;
	}
	dst+=dst_modulo;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*dst++=(((uint16_t)(*src1++)+(uint16_t)(*src3++)+(((uint16_t)(*src2++)) << 1))+2) >> 2;
		}
		src1+=src_modulo;
		src2+=src_modulo;
		src3+=src_modulo;
		dst+=dst_modulo;
	}

	for (int32_t j=0; j<w; j++)
	{
		*dst++=((uint16_t)(*src1++)+(uint16_t)(*src2++)+1)>>1;
	}

}


void JPSDR_Deinterlace::Blend_Tri_Planar8(const void *src,void *_dst,void *buffer[],int32_t w,int32_t h,
	ptrdiff_t src_modulo,ptrdiff_t dst_modulo,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
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
	{
		*dst++=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;
	}
	dst+=dst_modulo;

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w);
		indice_buffer=(indice_buffer+1)%2;
		src1=(uint8_t *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			*dst++=(((uint16_t)(*src1++)+(uint16_t)(*src3++)+(((uint16_t)(*src2++)) << 1))+2) >> 2;
		}
		src2+=src_modulo;
		src3+=src_modulo;
		dst+=dst_modulo;
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(uint8_t *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		*dst++=((uint16_t)(*src1++)+(uint16_t)(*src2++)+1)>>1;
	}

}


void JPSDR_Deinterlace::Run()
{
	Image_Data idata;
	const VDXPixmap *tab_pxsrc[2];
	int32_t h0,h1,h2;
	const void *src1_0,*src2_0,*src1_1,*src2_1,*src1_2,*src2_2;
	void *dst0_0,*dst0_1,*dst0_2;
	void *dst0,*dst1,*dst2;
	bool deinterlace,transfert_buffer,swap_buffer;
	bool src_a0,src_a1,src_a2,dst_a0,dst_a1,dst_a2;
	bool n_src_a0,n_src_a1,n_src_a2,n_dst_a0,n_dst_a1,n_dst_a2;
	uint8_t current_mode;
	uint8_t discardP,discardN;
	uint16_t i;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;

	if ((!mData.file_mode) && ((mData.mode==5) || (mData.mode==6)))
	{
		tab_pxsrc[0]=fa->mpSourceFrames[0]->mpPixmap;
		tab_pxsrc[1]=fa->mpSourceFrames[1]->mpPixmap;
	}
	else
	{
		tab_pxsrc[0]=fa->mpSourceFrames[0]->mpPixmap;
		tab_pxsrc[1]=fa->mpSourceFrames[0]->mpPixmap;
	}

	const VDXPixmap& pxdst=*fa->mpOutputFrames[0]->mpPixmap;
	const VDXPixmap& pxsrc=*tab_pxsrc[0];
	const VDXPixmap& pxsrc1=*tab_pxsrc[1];
	const VDXFBitmap& pxsrc2 = *fa->mpSourceFrames[0];

	idata.src_plane0=pxsrc.data;
	idata.src_plane1=pxsrc.data2;
	idata.src_plane2=pxsrc.data3;
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	swap_buffer=(!mData.file_mode) && (mData.mode==2);
	current_mode=mData.mode;

	if ((!mData.file_mode) && ((mData.mode==5) || (mData.mode==6)))
	{
		if (pxsrc2.mFrameNumber==0)
		{
			write_index_Yadif=0;
			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderRGB(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderRGB(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
				case 2 :
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],(idata.src_w0+1)>>1,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderYUYV(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],(idata.src_w0+1)>>1,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderYUYV(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
				case 3 :
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],(idata.src_w0+1)>>1,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderUYVY(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],(idata.src_w0+1)>>1,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderUYVY(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane1,tab_Yadif[write_index_Yadif].data[1],idata.src_w1,
						idata.src_h1,idata.src_modulo1,Yadif_modulo[1]);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane2,tab_Yadif[write_index_Yadif].data[2],idata.src_w2,
						idata.src_h2,idata.src_modulo2,Yadif_modulo[2]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[1],Yadif_pitch[1],idata.src_w1,idata.src_h1);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[2],Yadif_pitch[2],idata.src_w2,idata.src_h2);
					write_index_Yadif++;
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane1,tab_Yadif[write_index_Yadif].data[1],idata.src_w1,
						idata.src_h1,idata.src_modulo1,Yadif_modulo[1]);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane2,tab_Yadif[write_index_Yadif].data[2],idata.src_w2,
						idata.src_h2,idata.src_modulo2,Yadif_modulo[2]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[1],Yadif_pitch[1],idata.src_w1,idata.src_h1);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[2],Yadif_pitch[2],idata.src_w2,idata.src_h2);
					write_index_Yadif++;
					break;
				case 9 :
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
			}
			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
				case 2 :
				case 3 :
				case 9 :
					src1_0=tab_Yadif[0].data[0];
					src2_0=tab_Yadif[1].data[0];
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					src1_0=tab_Yadif[0].data[0];
					src2_0=tab_Yadif[1].data[0];
					src1_1=tab_Yadif[0].data[1];
					src2_1=tab_Yadif[1].data[1];
					src1_2=tab_Yadif[0].data[2];
					src2_2=tab_Yadif[1].data[2];
					break;
			}

			if (mData.mode==6) // Keep bottom field
			{
				switch(idata.video_mode)
				{
					case 1 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
					case 0 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						break;
					case 2 :
					case 3 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
						src1_1=(void *) ((uint8_t *)src1_1+Yadif_pitch[1]);
						src2_1=(void *) ((uint8_t *)src2_1+Yadif_pitch[1]);
						src1_2=(void *) ((uint8_t *)src1_2+Yadif_pitch[2]);
						src2_2=(void *) ((uint8_t *)src2_2+Yadif_pitch[2]);
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[0].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[0].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						break;
					case 9 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						break;
				}
			}
			else // Keep top field
			{
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
					case 1 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						break;
					case 2 :
					case 3 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[0].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[0].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						break;
					case 9 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						break;
				}
			}
			current_frame_Yadif=1;
			previous_frame_Yadif=0;
			next_frame_Yadif=2;
		}
	// Au-delà de la trame 0.
		idata.src_plane0=pxsrc1.data;
		idata.src_plane1=pxsrc1.data2;
		idata.src_plane2=pxsrc1.data3;

			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderRGB(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
				case 2 :
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],(idata.src_w0+1)>>1,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderYUYV(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
				case 3 :
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],(idata.src_w0+1)>>1,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderUYVY(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane1,tab_Yadif[write_index_Yadif].data[1],idata.src_w1,
						idata.src_h1,idata.src_modulo1,Yadif_modulo[1]);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane2,tab_Yadif[write_index_Yadif].data[2],idata.src_w2,
						idata.src_h2,idata.src_modulo2,Yadif_modulo[2]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[1],Yadif_pitch[1],idata.src_w1,idata.src_h1);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[2],Yadif_pitch[2],idata.src_w2,idata.src_h2);
					break;
				case 9 :
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_w0,
						idata.src_h0,idata.src_modulo0,Yadif_modulo[0]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
			}

			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
				case 2 :
				case 3 :
				case 9 :
					src1_0=tab_Yadif[current_frame_Yadif].data[0];
					src2_0=tab_Yadif[next_frame_Yadif].data[0];
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					src1_0=tab_Yadif[current_frame_Yadif].data[0];
					src2_0=tab_Yadif[next_frame_Yadif].data[0];
					src1_1=tab_Yadif[current_frame_Yadif].data[1];
					src2_1=tab_Yadif[next_frame_Yadif].data[1];
					src1_2=tab_Yadif[current_frame_Yadif].data[2];
					src2_2=tab_Yadif[next_frame_Yadif].data[2];
					break;
			}

			if (mData.mode==6) // Keep Bottom
			{
				switch(idata.video_mode)
				{
					case 1 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
					case 0 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						break;
					case 2 :
					case 3 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
						src1_1=(void *) ((uint8_t *)src1_1+Yadif_pitch[1]);
						src2_1=(void *) ((uint8_t *)src2_1+Yadif_pitch[1]);
						src1_2=(void *) ((uint8_t *)src1_2+Yadif_pitch[2]);
						src2_2=(void *) ((uint8_t *)src2_2+Yadif_pitch[2]);
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[current_frame_Yadif].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[current_frame_Yadif].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						break;
					case 9 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=tab_Yadif[current_frame_Yadif].data[0];
						dst0=idata.dst_plane0;
						break;
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src1_0=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[0]+Yadif_pitch[0]);
						dst0=(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[0]+Yadif_pitch[0]);
						src1_1=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[1]+Yadif_pitch[1]);
						src1_2=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[2]+Yadif_pitch[2]);
						dst0=(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
						dst1=(void *)((uint8_t *)idata.dst_plane1+idata.dst_pitch1);
						dst2=(void *)((uint8_t *)idata.dst_plane2+idata.dst_pitch2);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						JPSDR_Deinterlace_Move32_Half(src1_0,dst0,idata.src_w0,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						break;
					case 2 :
					case 3 :
						JPSDR_Deinterlace_Move32_Half(src1_0,dst0,(idata.src_w0+1)>>1,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						JPSDR_Deinterlace_Move8_Half(src1_0,dst0,idata.src_w0,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						JPSDR_Deinterlace_Move8_Half(src1_1,dst1,idata.src_w1,idata.src_h1>>1,Yadif_pitch[1]+Yadif_modulo[1],
							idata.dst_pitch1+idata.dst_modulo1);
						JPSDR_Deinterlace_Move8_Half(src1_2,dst2,idata.src_w2,idata.src_h2>>1,Yadif_pitch[2]+Yadif_modulo[2],
							idata.dst_pitch2+idata.dst_modulo2);
						break;
					case 9 :
						JPSDR_Deinterlace_Move8_Half(src1_0,dst0,idata.src_w0,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						break;
				}
				// Here selection of discarded frames, different from TFF/BFF
				if (mData.TFF)
				{
					discardP=current_frame_Yadif;
					discardN=next_frame_Yadif;
				}
				else
				{
					discardP=previous_frame_Yadif;
					discardN=current_frame_Yadif;
				}
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=(void *)((uint8_t *)tab_Yadif[discardP].data[0]+Yadif_pitch[0]);
						src2_0=(void *)((uint8_t *)tab_Yadif[discardN].data[0]+Yadif_pitch[0]);
						dst0=(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
						break;
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src1_0=tab_Yadif[discardP].data[0];
						src2_0=tab_Yadif[discardN].data[0];
						dst0=idata.dst_plane0;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=tab_Yadif[discardP].data[0];
						src2_0=tab_Yadif[discardN].data[0];
						src1_1=tab_Yadif[discardP].data[1];
						src2_1=tab_Yadif[discardN].data[1];
						src1_2=tab_Yadif[discardP].data[2];
						src2_2=tab_Yadif[discardN].data[2];
						dst0=idata.dst_plane0;
						dst1=idata.dst_plane1;
						dst2=idata.dst_plane2;
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						break;
					case 2 :
					case 3 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAbsDiffAvg(src1_1,src2_1,abs1[1],avg1[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[1],avg2[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[1],avg3[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAbsDiffAvg(src1_2,src2_2,abs1[2],avg1[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[2],avg2[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[2],avg3[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_w0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_w0,idata.dst_h0>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[1],spatial_Yadif_buffer,
							Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1]*10,Yadif_modulo[1],idata.src_w1,idata.dst_h1>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst1,Yadif_pitch[1]*10,Yadif_modulo[1],idata.dst_pitch1,
							idata.dst_modulo1,idata.src_w1,idata.dst_h1>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[2],spatial_Yadif_buffer,
							Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2]*10,Yadif_modulo[2],idata.src_w2,idata.dst_h2>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst2,Yadif_pitch[2]*10,Yadif_modulo[2],idata.dst_pitch2,
							idata.dst_modulo2,idata.src_w2,idata.dst_h2>>1);
						break;
					case 9 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_w0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_w0,idata.dst_h0>>1);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=tab_Yadif[current_frame_Yadif].data[0];
						break;
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src1_0=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[0]+Yadif_pitch[0]);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[0]+Yadif_pitch[0]);
						src1_1=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[1]+Yadif_pitch[1]);
						src1_2=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[2]+Yadif_pitch[2]);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
					case 2 :
					case 3 :
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifTemporalPlanarKeepBottom(src1_0,avg1[0],avg2[0],avg3[0],abs1[0],dst0,Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_w0,idata.src_h0>>1);
						YadifTemporalPlanarKeepBottom(src1_1,avg1[1],avg2[1],avg3[1],abs1[1],dst1,Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.dst_pitch1,idata.dst_modulo1,idata.src_w1,idata.src_h1>>1);
						YadifTemporalPlanarKeepBottom(src1_2,avg1[2],avg2[2],avg3[2],abs1[2],dst2,Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.dst_pitch2,idata.dst_modulo2,idata.src_w2,idata.src_h2>>1);
						break;
					case 9 :
						YadifTemporalPlanarKeepBottom(src1_0,avg1[0],avg2[0],avg3[0],abs1[0],dst0,Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_w0,idata.src_h0>>1);
						break;
				}
			}
			else
			{  // Keep Top
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=(void *) ((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *) ((uint8_t *)src2_0+Yadif_pitch[0]);
					case 1 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						break;
					case 2 :
					case 3 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[current_frame_Yadif].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[current_frame_Yadif].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						break;
					case 9 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[0]+Yadif_pitch[0]);
						dst0=(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
						break;
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src1_0=tab_Yadif[current_frame_Yadif].data[0];
						dst0=idata.dst_plane0;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=tab_Yadif[current_frame_Yadif].data[0];
						src1_1=tab_Yadif[current_frame_Yadif].data[1];
						src1_2=tab_Yadif[current_frame_Yadif].data[2];
						dst0=idata.dst_plane0;
						dst1=idata.dst_plane1;
						dst2=idata.dst_plane2;
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						JPSDR_Deinterlace_Move32_Half(src1_0,dst0,idata.src_w0,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						break;
					case 2 :
					case 3 :
						JPSDR_Deinterlace_Move32_Half(src1_0,dst0,(idata.src_w0+1)>>1,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						JPSDR_Deinterlace_Move8_Half(src1_0,dst0,idata.src_w0,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						JPSDR_Deinterlace_Move8_Half(src1_1,dst1,idata.src_w1,idata.src_h1>>1,Yadif_pitch[1]+Yadif_modulo[1],
							idata.dst_pitch1+idata.dst_modulo1);
						JPSDR_Deinterlace_Move8_Half(src1_2,dst2,idata.src_w2,idata.src_h2>>1,Yadif_pitch[2]+Yadif_modulo[2],
							idata.dst_pitch2+idata.dst_modulo2);
						break;
					case 9 :
						JPSDR_Deinterlace_Move8_Half(src1_0,dst0,idata.src_w0,idata.src_h0>>1,Yadif_pitch[0]+Yadif_modulo[0],
							idata.dst_pitch0+idata.dst_modulo0);
						break;
				}
				// Here selection of discarded frames, is different form TFF/BFF
				if (mData.TFF)
				{
					discardP=previous_frame_Yadif;
					discardN=current_frame_Yadif;
				}
				else
				{
					discardP=current_frame_Yadif;
					discardN=next_frame_Yadif;
				}
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=tab_Yadif[discardP].data[0];
						src2_0=tab_Yadif[discardN].data[0];
						dst0=idata.dst_plane0;
						break;
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src1_0=(void *)((uint8_t *)tab_Yadif[discardP].data[0]+Yadif_pitch[0]);
						src2_0=(void *)((uint8_t *)tab_Yadif[discardN].data[0]+Yadif_pitch[0]);
						dst0=(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=(void *)((uint8_t *)tab_Yadif[discardP].data[0]+Yadif_pitch[0]);
						src2_0=(void *)((uint8_t *)tab_Yadif[discardN].data[0]+Yadif_pitch[0]);
						src1_1=(void *)((uint8_t *)tab_Yadif[discardP].data[1]+Yadif_pitch[1]);
						src2_1=(void *)((uint8_t *)tab_Yadif[discardN].data[1]+Yadif_pitch[1]);
						src1_2=(void *)((uint8_t *)tab_Yadif[discardP].data[2]+Yadif_pitch[2]);
						src2_2=(void *)((uint8_t *)tab_Yadif[discardN].data[2]+Yadif_pitch[2]);
						dst0=(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
						dst1=(void *)((uint8_t *)idata.dst_plane1+idata.dst_pitch1);
						dst2=(void *)((uint8_t *)idata.dst_plane2+idata.dst_pitch2);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<2,idata.src_h0>>1);
						break;
					case 2 :
					case 3 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0<<1,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAbsDiffAvg(src1_1,src2_1,abs1[1],avg1[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[1],avg2[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[1],avg3[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_w1,idata.src_h1>>1);
						YadifAbsDiffAvg(src1_2,src2_2,abs1[2],avg1[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[2],avg2[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[2],avg3[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_w2,idata.src_h2>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_w0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_w0,idata.dst_h0>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[1],spatial_Yadif_buffer,
							Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1]*10,Yadif_modulo[1],idata.src_w1,idata.dst_h1>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst1,Yadif_pitch[1]*10,Yadif_modulo[1],idata.dst_pitch1,
							idata.dst_modulo1,idata.src_w1,idata.dst_h1>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[2],spatial_Yadif_buffer,
							Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2]*10,Yadif_modulo[2],idata.src_w2,idata.dst_h2>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst2,Yadif_pitch[2]*10,Yadif_modulo[2],idata.dst_pitch2,
							idata.dst_modulo2,idata.src_w2,idata.dst_h2>>1);
						break;
					case 9 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_w0,idata.src_h0>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_w0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_w0,idata.dst_h0>>1);
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
						src1_0=(void *)((uint8_t *)tab_Yadif[current_frame_Yadif].data[0]+Yadif_pitch[0]);
						break;
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src1_0=tab_Yadif[current_frame_Yadif].data[0];
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src1_0=tab_Yadif[current_frame_Yadif].data[0];
						src1_1=tab_Yadif[current_frame_Yadif].data[1];
						src1_2=tab_Yadif[current_frame_Yadif].data[2];
						break;
				}
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
					case 2 :
					case 3 :
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifTemporalPlanarKeepTop(src1_0,avg1[0],avg2[0],avg3[0],abs1[0],dst0,Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_w0,idata.src_h0>>1);
						YadifTemporalPlanarKeepTop(src1_1,avg1[1],avg2[1],avg3[1],abs1[1],dst1,Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.dst_pitch1,idata.dst_modulo1,idata.src_w1,idata.src_h1>>1);
						YadifTemporalPlanarKeepTop(src1_2,avg1[2],avg2[2],avg3[2],abs1[2],dst2,Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.dst_pitch2,idata.dst_modulo2,idata.src_w2,idata.src_h2>>1);
						break;
					case 9 :
						YadifTemporalPlanarKeepTop(src1_0,avg1[0],avg2[0],avg3[0],abs1[0],dst0,Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_w0,idata.src_h0>>1);
						break;
				}
			}
		current_frame_Yadif=(current_frame_Yadif+1)%3;
		next_frame_Yadif=(next_frame_Yadif+1)%3;
		previous_frame_Yadif=(previous_frame_Yadif+1)%3;
		write_index_Yadif=(write_index_Yadif+1)%3;
		return;
	}

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
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				if (swap_buffer)
				{
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,idata.dst_plane0,idata.src_w0,idata.src_h0,
						idata.src_modulo0,idata.dst_modulo0);
				}
				if (transfert_buffer) JPSDR_Deinterlace_Move32_Full(idata.src_plane0,buffer0,idata.src_w0,
					idata.src_h0,idata.src_modulo0,0);
				break;
			case 2 :
			case 3 :
				if (swap_buffer)
				{
					JPSDR_Deinterlace_Move32_Full(idata.src_plane0,idata.dst_plane0,(idata.src_w0+1)>>1,idata.src_h0,
						idata.src_modulo0,idata.dst_modulo0);
				}
				if (transfert_buffer) JPSDR_Deinterlace_Move32_Full(idata.src_plane0,buffer0,(idata.src_w0+1)>>1,
					idata.src_h0,idata.src_modulo0,0);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (swap_buffer)
				{
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,idata.dst_plane0,idata.src_w0,idata.src_h0,
						idata.src_modulo0,idata.dst_modulo0);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane1,idata.dst_plane1,idata.src_w1,idata.src_h1,
						idata.src_modulo1,idata.dst_modulo1);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane2,idata.dst_plane2,idata.src_w2,idata.src_h2,
						idata.src_modulo2,idata.dst_modulo2);
				}
				if (transfert_buffer)
				{
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,buffer0,idata.src_w0,
					idata.src_h0,idata.src_modulo0,0);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane1,buffer1,idata.src_w1,
					idata.src_h1,idata.src_modulo1,0);
					JPSDR_Deinterlace_Move8_Full(idata.src_plane2,buffer2,idata.src_w2,
					idata.src_h2,idata.src_modulo2,0);
				}
				break;
			case 9 :
				if (swap_buffer)
				{
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,idata.dst_plane0,idata.src_w0,idata.src_h0,
						idata.src_modulo0,idata.dst_modulo0);
				}
				if (transfert_buffer)
				{
					JPSDR_Deinterlace_Move8_Full(idata.src_plane0,buffer0,idata.src_w0,
					idata.src_h0,idata.src_modulo0,0);
				}
				break;
		}
		return;
	}

	switch(current_mode)
	{
	case 1 :
		src1_0 = idata.src_plane0;
		src2_0 = (void *) ((uint8_t *)src1_0+idata.src_pitch0);
		dst0_0 = idata.dst_plane0;
		h0=idata.src_h0-1;
		src1_1 = idata.src_plane1;
		src2_1 = (void *) ((uint8_t *)src1_1+idata.src_pitch1);
		dst0_1 = idata.dst_plane1;
		h1=idata.src_h1-1;
		src1_2 = idata.src_plane2;
		src2_2 = (void *) ((uint8_t *)src1_2+idata.src_pitch2);
		dst0_2 = idata.dst_plane2;
		h2=idata.src_h2-1;
		break;
	case 2 :
		src1_0=idata.src_plane0;
		dst0_0=idata.dst_plane0;
		h0=idata.src_h0-2;
		src1_1=idata.src_plane1;
		dst0_1=idata.dst_plane1;
		h1=idata.src_h1-2;
		src1_2=idata.src_plane2;
		dst0_2=idata.dst_plane2;
		h2=idata.src_h2-2;
		break;
	case 3 :
		src1_0 = (void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
		src2_0 = (void *)((uint8_t *)src1_0+2*idata.src_pitch0);
		dst0_0 = (void *)((uint8_t *)idata.dst_plane0+2*idata.dst_pitch0);
		h0=(idata.src_h0 >> 1)-1;
		idata.src_pitch0<<=1;
		idata.dst_pitch0<<=1;
		src1_1 = (void *)((uint8_t *)idata.src_plane1+idata.src_pitch1);
		src2_1 = (void *)((uint8_t *)src1_1+2*idata.src_pitch1);
		dst0_1 = (void *)((uint8_t *)idata.dst_plane1+2*idata.dst_pitch1);
		h1=(idata.src_h1 >> 1)-1;
		idata.src_pitch1<<=1;
		idata.dst_pitch1<<=1;
		src1_2 = (void *)((uint8_t *)idata.src_plane2+idata.src_pitch2);
		src2_2 = (void *)((uint8_t *)src1_2+2*idata.src_pitch2);
		dst0_2 = (void *)((uint8_t *)idata.dst_plane2+2*idata.dst_pitch2);
		h2=(idata.src_h2 >> 1)-1;
		idata.src_pitch2<<=1;
		idata.dst_pitch2<<=1;
		break;
	case 4 :
		src1_0 = idata.src_plane0;
		src2_0 = (void *)((uint8_t *)src1_0+2*idata.src_pitch0);
		dst0_0 = (void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0);
		h0=(idata.src_h0 >> 1)-1;
		idata.src_pitch0<<=1;
		idata.dst_pitch0<<=1;
		src1_1 = idata.src_plane1;
		src2_1 = (void *)((uint8_t *)src1_1+2*idata.src_pitch1);
		dst0_1 = (void *)((uint8_t *)idata.dst_plane1+idata.dst_pitch1);
		h1=(idata.src_h1 >> 1)-1;
		idata.src_pitch1<<=1;
		idata.dst_pitch1<<=1;
		src1_2 = idata.src_plane2;
		src2_2 = (void *)((uint8_t *)src1_2+2*idata.src_pitch2);
		dst0_2 = (void *)((uint8_t *)idata.dst_plane2+idata.dst_pitch2);
		h2=(idata.src_h2 >> 1)-1;
		idata.src_pitch2<<=1;
		idata.dst_pitch2<<=1;
		break;
	}

	src_a0=((((size_t)src1_0&0x0F)==0) && ((abs64(idata.src_pitch0)&0x0F)==0));
	dst_a0=((((size_t)dst0_0&0x0F)==0) && ((abs64(idata.dst_pitch0)&0x0F)==0));
	src_a1=((((size_t)src1_1&0x0F)==0) && ((abs64(idata.src_pitch1)&0x0F)==0));
	dst_a1=((((size_t)dst0_1&0x0F)==0) && ((abs64(idata.dst_pitch1)&0x0F)==0));
	src_a2=((((size_t)src1_2&0x0F)==0) && ((abs64(idata.src_pitch2)&0x0F)==0));
	dst_a2=((((size_t)dst0_2&0x0F)==0) && ((abs64(idata.dst_pitch2)&0x0F)==0));
	n_src_a0=((((size_t)src1_0&0x0F)!=0) && ((abs64(idata.src_pitch0)&0x0F)==0));
	n_dst_a0=((((size_t)dst0_0&0x0F)!=0) && ((abs64(idata.dst_pitch0)&0x0F)==0));
	n_src_a1=((((size_t)src1_1&0x0F)!=0) && ((abs64(idata.src_pitch1)&0x0F)==0));
	n_dst_a1=((((size_t)dst0_1&0x0F)!=0) && ((abs64(idata.dst_pitch1)&0x0F)==0));
	n_src_a2=((((size_t)src1_2&0x0F)!=0) && ((abs64(idata.src_pitch2)&0x0F)==0));
	n_dst_a2=((((size_t)dst0_2&0x0F)!=0) && ((abs64(idata.dst_pitch2)&0x0F)==0));
	
	switch(current_mode)
	{
		case 0 :
			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					JPSDR_Deinterlace_Move32_Full(buffer0,idata.dst_plane0,idata.src_w0,idata.src_h0,
						0,idata.dst_modulo0);
					break;
				case 2 :
				case 3 :
					JPSDR_Deinterlace_Move32_Full(buffer0,idata.dst_plane0,(idata.src_w0+1)>>1,idata.src_h0,
						0,idata.dst_modulo0);
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					JPSDR_Deinterlace_Move8_Full(buffer0,idata.dst_plane0,idata.src_w0,idata.src_h0,
						0,idata.dst_modulo0);
					JPSDR_Deinterlace_Move8_Full(buffer1,idata.dst_plane1,idata.src_w1,idata.src_h1,
						0,idata.dst_modulo1);
					JPSDR_Deinterlace_Move8_Full(buffer2,idata.dst_plane2,idata.src_w2,idata.src_h2,
						0,idata.dst_modulo2);
					break;
				case 9 :
					JPSDR_Deinterlace_Move8_Full(buffer0,idata.dst_plane0,idata.src_w0,idata.src_h0,
						0,idata.dst_modulo0);
					break;
			}
			break;
		case 1 :
		case 3 :
		case 4 :
			if (SSE2_Enable)
			{
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						if (((idata.src_w0&0x01)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x03)==0)
								{
									JPSDR_Deinterlace_Blend_SSE_3(src1_0,src2_0,dst0_0,idata.src_w0>>2,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_3b(src1_0,src2_0,dst0_0,idata.src_w0>>2,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x03)==0)
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_U(src1_0,src2_0,dst0_0,
											idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_Ub(src1_0,src2_0,dst0_0,
											idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
									}									
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x03)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_A(src1_0,src2_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_Ab(src1_0,src2_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x03)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U(src1_0,src2_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_Ub(src1_0,src2_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x01)
								JPSDR_Deinterlace_Blend_SSE_2c(src1_0,src2_0,dst0_0,idata.src_w0>>1,h0,
									idata.src_pitch0,idata.dst_pitch0);
							else
								JPSDR_Deinterlace_Blend_SSE_2b(src1_0,src2_0,dst0_0,idata.src_w0>>1,
									h0,idata.src_pitch0,idata.dst_pitch0);
						}
						break;
					case 2 :
					case 3 :
						if (((idata.src_w0&0x03)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x07)==0)
								{
									JPSDR_Deinterlace_Blend_SSE_3(src1_0,src2_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_3b(src1_0,src2_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x07)==0)
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_U(src1_0,src2_0,dst0_0,
											idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_Ub(src1_0,src2_0,dst0_0,
											idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x07)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_A(src1_0,src2_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_Ab(src1_0,src2_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x07)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U(src1_0,src2_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_Ub(src1_0,src2_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x03)
								JPSDR_Deinterlace_Blend_SSE_2c(src1_0,src2_0,dst0_0,idata.src_w0>>2,h0,
									idata.src_pitch0,idata.dst_pitch0);
							else
								JPSDR_Deinterlace_Blend_SSE_2b(src1_0,src2_0,dst0_0,idata.src_w0>>2,
									h0,idata.src_pitch0,idata.dst_pitch0);
						}
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						if (((idata.src_w0&0x07)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_SSE_3(src1_0,src2_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_3b(src1_0,src2_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_U(src1_0,src2_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_Ub(src1_0,src2_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_A(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_Ab(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_Ub(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x07)
							{
								if (idata.src_w0&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_2c(src1_0,src2_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2b(src1_0,src2_0,dst0_0,idata.src_w0>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							}
						}
						if (((idata.src_w1&0x07)==0) && (src_a1 || n_src_a1) && (dst_a1 || n_dst_a1))
						{
							if (src_a1 && dst_a1)
							{
								if ((idata.src_w1&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_SSE_3(src1_1,src2_1,dst0_1,idata.src_w1>>4,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_3b(src1_1,src2_1,dst0_1,idata.src_w1>>4,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
							}
							else
							{
								if (src_a1)
								{
									if ((idata.src_w1&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_U(src1_1,src2_1,dst0_1,
											idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
									}
									else
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_Ub(src1_1,src2_1,dst0_1,
											idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
									}
								}
								else
								{
									if (dst_a1)
									{
										if ((idata.src_w1&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_A(src1_1,src2_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_Ab(src1_1,src2_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
									}
									else
									{
										if ((idata.src_w1&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U(src1_1,src2_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_Ub(src1_1,src2_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w1&0x07)
							{
								if (idata.src_w1&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_1,src2_1,dst0_1,idata.src_w1,h1,
										idata.src_pitch1,idata.dst_pitch1);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_2c(src1_1,src2_1,dst0_1,idata.src_w1>>3,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2b(src1_1,src2_1,dst0_1,idata.src_w1>>3,
									h1,idata.src_pitch1,idata.dst_pitch1);
							}
						}
						if (((idata.src_w2&0x07)==0) && (src_a2 || n_src_a2) && (dst_a2 || n_dst_a2))
						{
							if (src_a2 && dst_a2)
							{
								if ((idata.src_w2&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_SSE_3(src1_2,src2_2,dst0_2,idata.src_w2>>4,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_3b(src1_2,src2_2,dst0_2,idata.src_w2>>4,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
							}
							else
							{
								if (src_a2)
								{
									if ((idata.src_w2&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_U(src1_2,src2_2,dst0_2,
											idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
									}
									else
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_Ub(src1_2,src2_2,dst0_2,
											idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
									}
								}
								else
								{
									if (dst_a2)
									{
										if ((idata.src_w2&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_A(src1_2,src2_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_Ab(src1_2,src2_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
									}
									else
									{
										if ((idata.src_w2&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U(src1_2,src2_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_Ub(src1_2,src2_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w2&0x07)
							{
								if (idata.src_w2&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_2,src2_2,dst0_2,idata.src_w2,h2,
										idata.src_pitch2,idata.dst_pitch2);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_2c(src1_2,src2_2,dst0_2,idata.src_w2>>3,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2b(src1_2,src2_2,dst0_2,idata.src_w2>>3,
									h2,idata.src_pitch2,idata.dst_pitch2);
							}
						}
						break;
					case 9 :
						if (((idata.src_w0&0x07)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_SSE_3(src1_0,src2_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_3b(src1_0,src2_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_U(src1_0,src2_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_SSE_3_A_Ub(src1_0,src2_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_A(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_U_Ab(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_SSE_3_U(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_SSE_3_Ub(src1_0,src2_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x07)
							{
								if (idata.src_w0&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE_2c(src1_0,src2_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2b(src1_0,src2_0,dst0_0,idata.src_w0>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							}
						}
						break;
				}
			}
			else
			{
				if (SSE_Integer_Enable)
				{
					switch(idata.video_mode)
					{
						case 0 :
						case 1 :
							if (idata.src_w0&0x01)
								JPSDR_Deinterlace_Blend_SSE(src1_0,src2_0,dst0_0,idata.src_w0,
									h0,idata.src_pitch0,idata.dst_pitch0);
							else
								JPSDR_Deinterlace_Blend_SSE_2(src1_0,src2_0,dst0_0,idata.src_w0>>1,
									h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 2 :
						case 3 :
							if (idata.src_w0&0x03)
								JPSDR_Deinterlace_Blend_SSE(src1_0,src2_0,dst0_0,(idata.src_w0+1)>>1,
									h0,idata.src_pitch0,idata.dst_pitch0);
							else
								JPSDR_Deinterlace_Blend_SSE_2(src1_0,src2_0,dst0_0,idata.src_w0>>2,
									h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 4 :
						case 5 :
						case 6 :
						case 7 :
						case 8 :
							if (idata.src_w0&0x07)
							{
								if (idata.src_w0&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE(src1_0,src2_0,dst0_0,idata.src_w0>>2,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2(src1_0,src2_0,dst0_0,idata.src_w0>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							}
							if (idata.src_w1&0x07)
							{
								if (idata.src_w1&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_1,src2_1,dst0_1,idata.src_w1,h1,
										idata.src_pitch1,idata.dst_pitch1);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE(src1_1,src2_1,dst0_1,idata.src_w1>>2,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2(src1_1,src2_1,dst0_1,idata.src_w1>>3,
									h1,idata.src_pitch1,idata.dst_pitch1);
							}
							if (idata.src_w2&0x07)
							{
								if (idata.src_w2&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_2,src2_2,dst0_2,idata.src_w2,h2,
										idata.src_pitch2,idata.dst_pitch2);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE(src1_2,src2_2,dst0_2,idata.src_w2>>2,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2(src1_2,src2_2,dst0_2,idata.src_w2>>3,
									h2,idata.src_pitch2,idata.dst_pitch2);
							}
							break;
						case 9 :
							if (idata.src_w0&0x07)
							{
								if (idata.src_w0&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_SSE(src1_0,src2_0,dst0_0,idata.src_w0>>2,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_SSE_2(src1_0,src2_0,dst0_0,idata.src_w0>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							}
							break;
					}
				}
				else
				{
					if (MMX_Enable)
					{
						switch(idata.video_mode)
						{
							case 0 :
							case 1 :
								JPSDR_Deinterlace_Blend_MMX(src1_0,src2_0,dst0_0,idata.src_w0,h0,
									idata.src_pitch0,idata.dst_pitch0);
								break;
							case 2 :
							case 3 :
								JPSDR_Deinterlace_Blend_MMX(src1_0,src2_0,dst0_0,(idata.src_w0+1)>>1,h0,
									idata.src_pitch0,idata.dst_pitch0);
								break;
							case 4 :
							case 5 :
							case 6 :
							case 7 :
							case 8 :
								if (idata.src_w0&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_MMX(src1_0,src2_0,dst0_0,idata.src_w0>>2,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								if (idata.src_w1&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_1,src2_1,dst0_1,idata.src_w1,h1,
										idata.src_pitch1,idata.dst_pitch1);
								}
								else
								{
									JPSDR_Deinterlace_Blend_MMX(src1_1,src2_1,dst0_1,idata.src_w1>>2,h1,
										idata.src_pitch1,idata.dst_pitch1);
								}
								if (idata.src_w2&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_2,src2_2,dst0_2,idata.src_w2,h2,
										idata.src_pitch2,idata.dst_pitch2);
								}
								else
								{
									JPSDR_Deinterlace_Blend_MMX(src1_2,src2_2,dst0_2,idata.src_w2>>2,h2,
										idata.src_pitch2,idata.dst_pitch2);
								}
								break;
							case 9 :
								if (idata.src_w0&0x03)
								{
									JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_MMX(src1_0,src2_0,dst0_0,idata.src_w0>>2,h0,
										idata.src_pitch0,idata.dst_pitch0);
								}
								break;
						}
					}
					else
					{
						switch(idata.video_mode)
						{
							case 0 :
							case 1 :
								JPSDR_Deinterlace_Blend_Non_MMX_24(src1_0,src2_0,dst0_0,idata.src_w0,
									h0,idata.src_pitch0,idata.dst_pitch0);
								break;
							case 2 :
							case 3 :
								JPSDR_Deinterlace_Blend_Non_MMX_32(src1_0,src2_0,dst0_0,(idata.src_w0+1)>>1,
									h0,idata.src_pitch0,idata.dst_pitch0);
								break;
							case 4 :
							case 5 :
							case 6 :
							case 7 :
							case 8 :
								JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,
									h0,idata.src_pitch0,idata.dst_pitch0);
								JPSDR_Deinterlace_Blend_Non_MMX_8(src1_1,src2_1,dst0_1,idata.src_w1,
									h1,idata.src_pitch1,idata.dst_pitch1);
								JPSDR_Deinterlace_Blend_Non_MMX_8(src1_2,src2_2,dst0_2,idata.src_w2,
									h2,idata.src_pitch2,idata.dst_pitch2);
								break;
							case 9 :
								JPSDR_Deinterlace_Blend_Non_MMX_8(src1_0,src2_0,dst0_0,idata.src_w0,
									h0,idata.src_pitch0,idata.dst_pitch0);
								break;
						}
					}
				}
			}
			break;
		case 2 :
			if ((SSE2_Enable) && (swap_buffer))
			{
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						if (((idata.src_w0&0x01)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x03)==0)
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3(src1_0,dst0_0,idata.src_w0>>2,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3b(src1_0,dst0_0,idata.src_w0>>2,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x03)==0)
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(src1_0,dst0_0,
											idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(src1_0,dst0_0,
											idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x03)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(src1_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(src1_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x03)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U(src1_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(src1_0,dst0_0,
												idata.src_w0>>2,h0,idata.src_pitch0,idata.dst_pitch0);
										}										
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x01)
								JPSDR_Deinterlace_Blend_Tri_SSE_2c(src1_0,dst0_0,idata.src_w0>>1,h0,
									idata.src_pitch0,idata.dst_pitch0);
							else
								JPSDR_Deinterlace_Blend_Tri_SSE_2b(src1_0,dst0_0,idata.src_w0>>1,
									h0,idata.src_pitch0,idata.dst_pitch0);
						}
						break;
					case 2 :
					case 3 :
						if (((idata.src_w0&0x03)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x07)==0)
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3(src1_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3b(src1_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x07)==0)
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(src1_0,dst0_0,
											idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(src1_0,dst0_0,
											idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x07)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(src1_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(src1_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x07)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U(src1_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(src1_0,dst0_0,
												idata.src_w0>>3,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x03)
								JPSDR_Deinterlace_Blend_Tri_SSE_2c(src1_0,dst0_0,idata.src_w0>>2,h0,
									idata.src_pitch0,idata.dst_pitch0);
							else
								JPSDR_Deinterlace_Blend_Tri_SSE_2b(src1_0,dst0_0,idata.src_w0>>2,
									h0,idata.src_pitch0,idata.dst_pitch0);
						}
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						if (((idata.src_w0&0x07)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3(src1_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3b(src1_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(src1_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(src1_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}										
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x07)
							{
								if (idata.src_w0&0x03)
								{
									Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,
										idata.src_modulo0,idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_2c(src1_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_SSE_2b(src1_0,dst0_0,idata.src_w0>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							}
						}
						if (((idata.src_w1&0x07)==0) && (src_a1 || n_src_a1) && (dst_a1 || n_dst_a1))
						{
							if (src_a1 && dst_a1)
							{
								if ((idata.src_w1&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3(src1_1,dst0_1,idata.src_w1>>4,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3b(src1_1,dst0_1,idata.src_w1>>4,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
							}
							else
							{
								if (src_a1)
								{
									if ((idata.src_w1&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(src1_1,dst0_1,
											idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
									}
									else
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(src1_1,dst0_1,
											idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
									}
								}
								else
								{
									if (dst_a1)
									{
										if ((idata.src_w1&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(src1_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(src1_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
									}
									else
									{
										if ((idata.src_w1&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U(src1_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(src1_1,dst0_1,
												idata.src_w1>>4,h1,idata.src_pitch1,idata.dst_pitch1);
										}										
									}
								}
							}
						}
						else
						{
							if (idata.src_w1&0x07)
							{
								if (idata.src_w1&0x03)
								{
									Blend_Tri_Planar8(src1_1,dst0_1,idata.src_w1,h1,
										idata.src_modulo1,idata.dst_modulo1,idata.src_pitch1,idata.dst_pitch1);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_2c(src1_1,dst0_1,idata.src_w1>>3,
										h1,idata.src_pitch1,idata.dst_pitch1);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_SSE_2b(src1_1,dst0_1,idata.src_w1>>3,
									h1,idata.src_pitch1,idata.dst_pitch1);
							}
						}
						if (((idata.src_w2&0x07)==0) && (src_a2 || n_src_a2) && (dst_a2 || n_dst_a2))
						{
							if (src_a2 && dst_a2)
							{
								if ((idata.src_w2&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3(src1_2,dst0_2,idata.src_w2>>4,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3b(src1_2,dst0_2,idata.src_w2>>4,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
							}
							else
							{
								if (src_a2)
								{
									if ((idata.src_w2&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(src1_2,dst0_2,
											idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
									}
									else
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(src1_2,dst0_2,
											idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
									}
								}
								else
								{
									if (dst_a2)
									{
										if ((idata.src_w2&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(src1_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(src1_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
									}
									else
									{
										if ((idata.src_w2&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U(src1_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(src1_2,dst0_2,
												idata.src_w2>>4,h2,idata.src_pitch2,idata.dst_pitch2);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w2&0x07)
							{
								if (idata.src_w2&0x03)
								{
									Blend_Tri_Planar8(src1_2,dst0_2,idata.src_w2,h2,
										idata.src_modulo2,idata.dst_modulo2,idata.src_pitch2,idata.dst_pitch2);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_2c(src1_2,dst0_2,idata.src_w2>>3,
										h2,idata.src_pitch2,idata.dst_pitch2);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_SSE_2b(src1_2,dst0_2,idata.src_w2>>3,
									h2,idata.src_pitch2,idata.dst_pitch2);
							}
						}
						break;
					case 9 :
						if (((idata.src_w0&0x07)==0) && (src_a0 || n_src_a0) && (dst_a0 || n_dst_a0))
						{
							if (src_a0 && dst_a0)
							{
								if ((idata.src_w0&0x0F)==0)
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3(src1_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_3b(src1_0,dst0_0,idata.src_w0>>4,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								if (src_a0)
								{
									if ((idata.src_w0&0x0F)==0)
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U(src1_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
									else
									{
										JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub(src1_0,dst0_0,
											idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
									}
								}
								else
								{
									if (dst_a0)
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
									else
									{
										if ((idata.src_w0&0x0F)==0)
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_U(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
										else
										{
											JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub(src1_0,dst0_0,
												idata.src_w0>>4,h0,idata.src_pitch0,idata.dst_pitch0);
										}
									}
								}
							}
						}
						else
						{
							if (idata.src_w0&0x07)
							{
								if (idata.src_w0&0x03)
								{
									Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,
										idata.src_modulo0,idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
								}
								else
								{
									JPSDR_Deinterlace_Blend_Tri_SSE_2c(src1_0,dst0_0,idata.src_w0>>3,
										h0,idata.src_pitch0,idata.dst_pitch0);
								}
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_SSE_2b(src1_0,dst0_0,idata.src_w0>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							}
						}
						break;
				}
			}
			else
			{
				if ((MMX_Enable) && (swap_buffer))
				{
					switch(idata.video_mode)
					{
						case 0 :
						case 1 :
							JPSDR_Deinterlace_Blend_Tri_MMX(src1_0,dst0_0,idata.src_w0,
								h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 2 :
						case 3 :
							JPSDR_Deinterlace_Blend_Tri_MMX(src1_0,dst0_0,(idata.src_w0+1)>>1,
								h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 4 :
						case 5 :
						case 6 :
						case 7 :
						case 8 :
							if (idata.src_w0&0x3)
							{
								Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_MMX(src1_0,dst0_0,idata.src_w0>>2,h0,
									idata.src_pitch0,idata.dst_pitch0);
							}
							if (idata.src_w1&0x3)
							{
								Blend_Tri_Planar8(src1_1,dst0_1,idata.src_w1,h1,idata.src_modulo1,
									idata.dst_modulo1,idata.src_pitch1,idata.dst_pitch1);
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_MMX(src1_1,dst0_1,idata.src_w1>>2,h1,
									idata.src_pitch1,idata.dst_pitch1);
							}
							if (idata.src_w2&0x3)
							{
								Blend_Tri_Planar8(src1_2,dst0_2,idata.src_w2,h2,idata.src_modulo2,
									idata.dst_modulo2,idata.src_pitch2,idata.dst_pitch2);
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_MMX(src1_2,dst0_2,idata.src_w2>>2,h2,
									idata.src_pitch2,idata.dst_pitch2);
							}
							break;
						case 9 :
							if (idata.src_w0&0x3)
							{
								Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								JPSDR_Deinterlace_Blend_Tri_MMX(src1_0,dst0_0,idata.src_w0>>2,h0,
									idata.src_pitch0,idata.dst_pitch0);
							}
							break;
					}
				}
				else
				{
					switch(idata.video_mode)
					{
						case 0 :
						case 1 :
							if (swap_buffer)
							{
								Blend_Tri_RGB32(src1_0,dst0_0,idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								Blend_Tri_RGB32(src1_0,dst0_0,buffer[0],idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							break;
						case 2 :
						case 3 :
							if (swap_buffer)
							{
								Blend_Tri_YUV32(src1_0,dst0_0,(idata.src_w0+1)>>1,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								Blend_Tri_YUV32(src1_0,dst0_0,buffer[0],(idata.src_w0+1)>>1,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							break;
						case 4 :
						case 5 :
						case 6 :
						case 7 :
						case 8 :
							if (swap_buffer)
							{
								Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
								Blend_Tri_Planar8(src1_1,dst0_1,idata.src_w1,h1,idata.src_modulo1,
									idata.dst_modulo1,idata.src_pitch1,idata.dst_pitch1);
								Blend_Tri_Planar8(src1_2,dst0_2,idata.src_w2,h2,idata.src_modulo2,
									idata.dst_modulo2,idata.src_pitch2,idata.dst_pitch2);
							}
							else
							{
								Blend_Tri_Planar8(src1_0,dst0_0,buffer[0],idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
								Blend_Tri_Planar8(src1_1,dst0_1,buffer[1],idata.src_w1,h1,idata.src_modulo1,
									idata.dst_modulo1,idata.src_pitch1,idata.dst_pitch1);
								Blend_Tri_Planar8(src1_2,dst0_2,buffer[2],idata.src_w2,h2,idata.src_modulo2,
									idata.dst_modulo2,idata.src_pitch2,idata.dst_pitch2);
							}
							break;
						case 9 :
							if (swap_buffer)
							{
								Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								Blend_Tri_Planar8(src1_0,dst0_0,buffer[0],idata.src_w0,h0,idata.src_modulo0,
									idata.dst_modulo0,idata.src_pitch0,idata.dst_pitch0);
							}
							break;
					}
				}
			}
			break;
	}

/*	switch(current_mode)
	{
		case 3 :
		case 4 :
			idata.src_pitch0>>=1;
			idata.dst_pitch0>>=1;
			idata.src_pitch1>>=1;
			idata.dst_pitch1>>=1;
			idata.src_pitch2>>=1;
			idata.dst_pitch2>>=1;
			break;
	}*/

	if (transfert_buffer) 
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				JPSDR_Deinterlace_Move32_Full(idata.dst_plane0,buffer0,idata.dst_w0,
					idata.dst_h0,idata.dst_modulo0,0);
				break;
			case 2 :
			case 3 :
				JPSDR_Deinterlace_Move32_Full(idata.dst_plane0,buffer0,(idata.dst_w0+1)>>1,
					idata.dst_h0,idata.dst_modulo0,0);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				JPSDR_Deinterlace_Move8_Full(idata.dst_plane0,buffer0,idata.dst_w0,
					idata.dst_h0,idata.dst_modulo0,0);
				JPSDR_Deinterlace_Move8_Full(idata.dst_plane1,buffer1,idata.dst_w1,
					idata.dst_h1,idata.dst_modulo1,0);
				JPSDR_Deinterlace_Move8_Full(idata.dst_plane2,buffer2,idata.dst_w2,
					idata.dst_h2,idata.dst_modulo2,0);
				break;
			case 9 :
				JPSDR_Deinterlace_Move8_Full(idata.dst_plane0,buffer0,idata.dst_w0,
					idata.dst_h0,idata.dst_modulo0,0);
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
		if ((mData.mode==5) || (mData.mode==6))
		{
			if (mData.TFF) strcat_s(str_out,1200," (TFF)");
			else strcat_s(str_out,1200," (BFF)");
		}
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

    SafePrintf(buf, maxlen, "Config(%d, %d, \"%s\")",mData.mode,mData.file_mode,save);
}


void JPSDR_Deinterlace::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.mode=argv[0].asInt();
	mData.file_mode=!!argv[1].asInt();
	strcpy_s(mData.filename,1024,*argv[2].asString());
}



		
extern VDXFilterDefinition filterDef_JPSDR_Deinterlace=
VDXVideoFilterDefinition<JPSDR_Deinterlace>("JPSDR","Deinterlace v4.1.2","Deinterlace blending frames. [ASM][MMX][SSE][SSE2] Optimised.");



