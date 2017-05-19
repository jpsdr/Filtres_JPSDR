#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "Field_Shift_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

extern int g_VFVAPIVersion;

#define Max_Sequence 2048


class JPSDR_Field_ShiftData
{
public :
	uint8_t mode;
	bool file_mode;
	char filename[1024];

	JPSDR_Field_ShiftData(void);
};


JPSDR_Field_ShiftData::JPSDR_Field_ShiftData(void)
{
	mode=1;
	strcpy_s(filename,1024,"");
	file_mode=false;
}


class JPSDR_Field_ShiftDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_Field_ShiftDialog(JPSDR_Field_ShiftData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_FIELD_SHIFT),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	void RefreshDisplay(void);
	void SetDisplay(void);
	
	bool SaveToData();
	
	JPSDR_Field_ShiftData& mData;
	JPSDR_Field_ShiftData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_Field_ShiftDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


void JPSDR_Field_ShiftDialog::SetDisplay(void)
{
	switch (mData.mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_UP_UP,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_BOT_BOT,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_BOT_UP,1); break;
		case 4 : CheckDlgButton(mhdlg,IDC_UP_BOT,1); break;
	}
}


void JPSDR_Field_ShiftDialog::RefreshDisplay(void)
{
	EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_PATH),mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_UP_UP),!mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_BOT_UP),!mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_BOT_BOT),!mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_UP_BOT),!mData.file_mode);
//	EnableWindow(GetDlgItem(mhdlg,IDC_PREVIEW),!mData.file_mode);
}


bool JPSDR_Field_ShiftDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_FILE_MODE,mData.file_mode?BST_CHECKED:BST_UNCHECKED);
	if (mData.file_mode)
	{
		SetWindowText(GetDlgItem(mhdlg,IDC_PATH),mData.filename);
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


void JPSDR_Field_ShiftDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_Field_ShiftDialog::SaveToData()
{
	if (!!IsDlgButtonChecked(mhdlg, IDC_UP_UP)) mData.mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_BOT_BOT)) mData.mode=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_BOT_UP)) mData.mode=3;
	if (!!IsDlgButtonChecked(mhdlg, IDC_UP_BOT)) mData.mode=4;
	mData.file_mode=!!IsDlgButtonChecked(mhdlg,IDC_FILE_MODE);
	if (mData.file_mode) GetWindowText(GetDlgItem(mhdlg,IDC_PATH),mData.filename,1024);
	else strcpy_s(mData.filename,1024,"");
	
	return true;
}



bool JPSDR_Field_ShiftDialog::OnCommand(int cmd)
{
	switch(cmd)
	{
		case IDC_UP_UP :
		case IDC_BOT_BOT :
		case IDC_BOT_UP :
		case IDC_UP_BOT :
			if (mifp && SaveToData()) mifp->RedoFrame();
			RefreshDisplay();
			return true;
		case IDC_FILE_MODE :
			mData.file_mode=!!IsDlgButtonChecked(mhdlg,IDC_FILE_MODE);
/*			if (mData.file_mode)
			{
				if (mifp->IsPreviewDisplayed()) mifp->Toggle((VDXHWND)mhdlg);
			}*/
			RefreshDisplay();
			if (mData.file_mode) GetWindowText(GetDlgItem(mhdlg,IDC_PATH), mData.filename, 1024);
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
				SetWindowText(GetDlgItem(mhdlg,IDC_PATH), mData.filename);
			}
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
			if (mifp)
				mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



class JPSDR_Field_Shift : public VDXVideoFilter
{
public:
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	virtual bool Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	uint32_t debut[Max_Sequence],fin[Max_Sequence];
	uint8_t tab_mode[Max_Sequence];
	uint16_t nbre_file_mode;

	inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);
	inline void Rebuild_Frame(const void *top_src, const void *bottom_src, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);

	void Blend_Tri_RGB32(const void *src,void *dst,const int32_t w,const int32_t h,const ptrdiff_t src_modulo,
		const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32(const void *src,void *dst,const int32_t w,const int32_t h,const ptrdiff_t src_modulo,
		const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);
	void Blend_Tri_Planar(const void *src,void *dst,const int32_t w,const int32_t h,const ptrdiff_t src_modulo,
		const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_Field_ShiftData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Field_Shift)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Field_Shift,ScriptConfig,"iis")
VDXVF_END_SCRIPT_METHODS()



uint32 JPSDR_Field_Shift::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

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

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
}


bool JPSDR_Field_Shift::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	prefetcher->PrefetchFrame(0,frame,0);
	prefetcher->PrefetchFrame(0,frame-1,0);

	return(true);
}


inline void JPSDR_Field_Shift::Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
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


inline void JPSDR_Field_Shift::Rebuild_Frame(const void *top_src, const void *bottom_src, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	const uint8_t *src;
	uint8_t *dst;
	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	src=(uint8_t *)top_src;
	dst=(uint8_t *)dst_;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

	src=(uint8_t *)bottom_src;
	dst=(uint8_t *)dst_+dst_pitch;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

}


void JPSDR_Field_Shift::GetSettingString(char *buf, int maxlen)
{
	char output_string[1200];
	
	if (mData.file_mode)
	{
		strcpy_s(output_string,1200," File mode : %s");
		SafePrintf(buf,maxlen,output_string,mData.filename);
	}
	else
	{
		strcpy_s(output_string,1200," Mode : %d");
		SafePrintf(buf,maxlen,output_string,mData.mode);
	}
}



void JPSDR_Field_Shift::Blend_Tri_RGB32(const void *src,void *_dst,const int32_t w,const int32_t h,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	RGB32BMP *src1,*src2,*src3,*dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	memcpy(_dst,src,w << 2);
	dst=(RGB32BMP *)((uint8_t *)_dst+dst_pitch);

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

	memcpy(dst,src2,w << 2);

}


void JPSDR_Field_Shift::Blend_Tri_YUV32(const void *src,void *_dst,const int32_t w,const int32_t h,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	YUYV *src1,*src2,*src3,*dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	memcpy(_dst,src,w << 2);
	dst=(YUYV *)((uint8_t *)_dst+dst_pitch);

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

	memcpy(dst,src2,w << 2);

}


void JPSDR_Field_Shift::Blend_Tri_Planar(const void *src,void *_dst,const int32_t w,const int32_t h,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	uint8_t *src1,*src2,*src3,*dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	memcpy(_dst,src,w);
	dst=(uint8_t *)(((uint8_t *)_dst)+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*dst++=(uint8_t)((((uint16_t)*src1++)+((uint16_t)*src3++)
				+(((uint16_t)*src2++) << 1)+2) >> 2);
		}
		src1+=src_modulo;
		src2+=src_modulo;
		src3+=src_modulo;
		dst+=dst_modulo;
	}
	memcpy(dst,src2,w);
}



void JPSDR_Field_Shift::Run()
{
	const VDXPixmap *tab_pxsrc[2]={
			fa->mpSourceFrames[0]->mpPixmap,
			fa->mpSourceFrames[1]->mpPixmap
	};
	const VDXPixmap& pxdst = *fa->mpOutputFrames[0]->mpPixmap;
	const VDXFBitmap& pxsrc = *fa->mpSourceFrames[0];

	Image_Data idata;
	int32_t w_Y,h_Y,w_U,h_U,w_V,h_V;
	uint32_t line0,line1,line2;
	ptrdiff_t src_modulo_Y,dst_modulo_Y,src_pitch_Y,dst_pitch_Y;
	ptrdiff_t src_modulo_U,dst_modulo_U,src_pitch_U,dst_pitch_U;
	ptrdiff_t src_modulo_V,dst_modulo_V,src_pitch_V,dst_pitch_V;
	const void *src_top_Y,*src_bottom_Y,*src_top_U,*src_bottom_U,*src_top_V,*src_bottom_V;
	void *dst_Y,*dst_U,*dst_V;
	uint8_t mode;
	uint16_t i;
	bool ok;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;


	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	switch (idata.video_mode)
	{
		case 1 :
		case 2 :
		case 3 :
		case 9 :
			dst_Y=idata.dst_plane0;
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
			dst_Y=idata.dst_plane0;
			dst_U=idata.dst_plane1;
			dst_V=idata.dst_plane2;
			break;
	}

	w_Y=idata.src_w0;
	h_Y=idata.src_h0;
	line0=idata.src_line0;
	src_pitch_Y=idata.src_pitch0;
	src_modulo_Y=idata.src_modulo0;
	dst_pitch_Y=idata.dst_pitch0;
	dst_modulo_Y=idata.dst_modulo0;
	w_U=idata.src_w1;
	h_U=idata.src_h1;
	line1=idata.src_line1;
	src_pitch_U=idata.src_pitch1;
	src_modulo_U=idata.src_modulo1;
	dst_pitch_U=idata.dst_pitch1;
	dst_modulo_U=idata.dst_modulo1;
	w_V=idata.src_w2;
	h_V=idata.src_h2;
	line2=idata.src_line2;
	src_pitch_V=idata.src_pitch2;
	src_modulo_V=idata.src_modulo2;
	dst_pitch_V=idata.dst_pitch2;
	dst_modulo_V=idata.dst_modulo2;


	if (mData.file_mode)
	{
		ok=false;
		i=0;
		while ((i<nbre_file_mode) && (!ok))
		{
			if ((pxsrc.mFrameNumber>=debut[i]) && (pxsrc.mFrameNumber<=fin[i]))
			{
				ok=true;
				mode=tab_mode[i];
			}
			i++;
		}
	}
	else
	{
		mode=mData.mode;
		ok=true;
	}

	if (pxsrc.mFrameNumber==0)
	{
		idata.src_plane0=tab_pxsrc[0]->data;
		idata.src_plane1=tab_pxsrc[0]->data2;
		idata.src_plane2=tab_pxsrc[0]->data3;
			
		switch (idata.video_mode)
		{
			case 1 :
			case 2 :
			case 3 :
			case 9 :
				src_top_Y=idata.src_plane0;
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				src_top_Y=idata.src_plane0;
				src_top_U=idata.src_plane1;
				src_top_V=idata.src_plane2;
				break;
		}

		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				if (ok) Blend_Tri_RGB32(src_top_Y,dst_Y,w_Y,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				break;
			case 2 :
			case 3 :
				if (ok) Blend_Tri_YUV32(src_top_Y,dst_Y,(w_Y+1)>>1,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (ok) Blend_Tri_Planar(src_top_Y,dst_Y,w_Y,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				if (ok) Blend_Tri_Planar(src_top_U,dst_U,w_U,h_U-2,src_modulo_U,dst_modulo_U,
					src_pitch_U,dst_pitch_U);
				if (ok) Blend_Tri_Planar(src_top_V,dst_V,w_V,h_V-2,src_modulo_V,dst_modulo_V,
					src_pitch_V,dst_pitch_V);
				break;
			case 9 :
				if (ok) Blend_Tri_Planar(src_top_Y,dst_Y,w_Y,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				break;
		}

//		mFrameNumber++;
		return;
	}

	if (ok)
	{
		switch(mode)
		{
			case 1 :
				idata.src_plane0=tab_pxsrc[1]->data;
				idata.src_plane1=tab_pxsrc[1]->data2;
				idata.src_plane2=tab_pxsrc[1]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_top_Y=idata.src_plane0;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_top_Y=idata.src_plane0;
						src_top_U=idata.src_plane1;
						src_top_V=idata.src_plane2;
						break;
				}

				idata.src_plane0=tab_pxsrc[0]->data;
				idata.src_plane1=tab_pxsrc[0]->data2;
				idata.src_plane2=tab_pxsrc[0]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_bottom_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_bottom_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						src_bottom_U=(void *)((uint8_t *)idata.src_plane1+idata.src_pitch1);
						src_bottom_V=(void *)((uint8_t *)idata.src_plane2+idata.src_pitch2);
						break;
				}
				break;
			case 2 :
				idata.src_plane0=tab_pxsrc[0]->data;
				idata.src_plane1=tab_pxsrc[0]->data2;
				idata.src_plane2=tab_pxsrc[0]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_top_Y=idata.src_plane0;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_top_Y=idata.src_plane0;
						src_top_U=idata.src_plane1;
						src_top_V=idata.src_plane2;
						break;
				}

				idata.src_plane0=tab_pxsrc[1]->data;
				idata.src_plane1=tab_pxsrc[1]->data2;
				idata.src_plane2=tab_pxsrc[1]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_bottom_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_bottom_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						src_bottom_U=(void *)((uint8_t *)idata.src_plane1+idata.src_pitch1);
						src_bottom_V=(void *)((uint8_t *)idata.src_plane2+idata.src_pitch2);
						break;
				}
				break;
			case 3 :
				idata.src_plane0=tab_pxsrc[1]->data;
				idata.src_plane1=tab_pxsrc[1]->data2;
				idata.src_plane2=tab_pxsrc[1]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_top_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_top_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						src_top_U=(void *)((uint8_t *)idata.src_plane1+idata.src_pitch1);
						src_top_V=(void *)((uint8_t *)idata.src_plane2+idata.src_pitch2);
						break;
				}

				idata.src_plane0=tab_pxsrc[0]->data;
				idata.src_plane1=tab_pxsrc[0]->data2;
				idata.src_plane2=tab_pxsrc[0]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_bottom_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_bottom_Y=(void *)((uint8_t *)idata.src_plane0+idata.src_pitch0);
						src_bottom_U=(void *)((uint8_t *)idata.src_plane1+idata.src_pitch1);
						src_bottom_V=(void *)((uint8_t *)idata.src_plane2+idata.src_pitch2);
						break;
				}
				break;
			case 4 :
				idata.src_plane0=tab_pxsrc[0]->data;
				idata.src_plane1=tab_pxsrc[0]->data2;
				idata.src_plane2=tab_pxsrc[0]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_top_Y=idata.src_plane0;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_top_Y=idata.src_plane0;
						src_top_U=idata.src_plane1;
						src_top_V=idata.src_plane2;
						break;
				}

				idata.src_plane0=tab_pxsrc[1]->data;
				idata.src_plane1=tab_pxsrc[1]->data2;
				idata.src_plane2=tab_pxsrc[1]->data3;
			
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						src_bottom_Y=idata.src_plane0;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						src_bottom_Y=idata.src_plane0;
						src_bottom_U=idata.src_plane1;
						src_bottom_V=idata.src_plane2;
						break;
				}
				break;
		}

		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
			case 9 :
				Rebuild_Frame(src_top_Y,src_bottom_Y,dst_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				Rebuild_Frame(src_top_Y,src_bottom_Y,dst_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				Rebuild_Frame(src_top_U,src_bottom_U,dst_U,line1,h_U>>1,src_pitch_U,dst_pitch_U);
				Rebuild_Frame(src_top_V,src_bottom_V,dst_V,line2,h_V>>1,src_pitch_V,dst_pitch_V);
				break;
		}
	}
	else
	{
		idata.src_plane0=tab_pxsrc[0]->data;
		idata.src_plane1=tab_pxsrc[0]->data2;
		idata.src_plane2=tab_pxsrc[0]->data3;
			
		switch (idata.video_mode)
		{
			case 1 :
			case 2 :
			case 3 :
			case 9 :
				src_top_Y=idata.src_plane0;
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				src_top_Y=idata.src_plane0;
				src_top_U=idata.src_plane1;
				src_top_V=idata.src_plane2;
				break;
		}

		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
			case 9 :
				Move_Full(src_top_Y,dst_Y,line0,h_Y,src_pitch_Y,dst_pitch_Y);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				Move_Full(src_top_Y,dst_Y,line0,h_Y,src_pitch_Y,dst_pitch_Y);
				Move_Full(src_top_U,dst_U,line1,h_U,src_pitch_U,dst_pitch_U);
				Move_Full(src_top_V,dst_V,line2,h_V,src_pitch_V,dst_pitch_V);
				break;
		}
	}
}



void JPSDR_Field_Shift::Start()
{
	Image_Data idata;
	uint32_t u,v;
	int32_t offset;
	uint16_t w,line_file;
	int16_t i,j;
	FILE *file_in;
	char buffer_in[1024];

	if (g_VFVAPIVersion<14)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
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
		case 6 :
			if ((idata.src_h1%2)!=0)
				ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
			break;
		case 8 :
			if ((idata.src_h1%2)!=0)
				ff->Except("Vertical size must be multiple of 8 for 4:1:0!");
			break;
		default :
			if ((idata.src_h0%2)!=0)
				ff->Except("Vertical size must be multiple of 2!");
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
					if ((w<1) || (w>4))
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

}



void JPSDR_Field_Shift::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.mode=argv[0].asInt();
	mData.file_mode=!!argv[1].asInt();
	strcpy_s(mData.filename,1024,*argv[2].asString());
}


bool JPSDR_Field_Shift::Configure(VDXHWND hwnd)
{
	JPSDR_Field_ShiftDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}



void JPSDR_Field_Shift::GetScriptString(char *buf, int maxlen)
{
    char save[1400];
	int i, j;

	i = j = 0;
	while((mData.filename[i]!='\0') && (j<1398))
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

	SafePrintf(buf,maxlen,"Config(%d, %d, \"%s\")",mData.mode,mData.file_mode,save);
}


extern VDXFilterDefinition filterDef_JPSDR_Field_Shift=
VDXVideoFilterDefinition<JPSDR_Field_Shift>("JPSDR","FieldShift v3.1.1","Shift Field from previous and current frame.");

