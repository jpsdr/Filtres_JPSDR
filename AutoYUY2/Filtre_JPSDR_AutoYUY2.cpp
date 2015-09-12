#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\asmlib\asmlib.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

extern int g_VFVAPIVersion;

extern "C" int IInstrSet;

extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_1(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);

extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_1(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(const void *scr_y,const void *src_u,const void *src_v,
		void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(const void *scr_y,const void *src_u1,const void *src_u2,
		const void *src_v1,const void *src_v2,void *dst,int32_t w);

extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(const void *scr_1,const void *src_2,void *dst,int32_t w);



#define Interlaced_Tab_Size 3

class JPSDR_AutoYUY2Data
{
public:
	uint8_t output_format;
	uint8_t convert_mode;
	uint8_t threshold;

	JPSDR_AutoYUY2Data(void);
};


JPSDR_AutoYUY2Data::JPSDR_AutoYUY2Data(void)
{
	output_format=2;
	convert_mode=0;
	threshold=4;
}



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

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
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
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_AUTOMATIC :
		case IDC_TEST :
			EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD),true);
			EnableWindow(GetDlgItem(mhdlg,IDC_THRESHOLD_TEXT),true);
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_THRESHOLD :
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_YUY2 :
		case IDC_UYVY :
		case IDC_PLANAR_422 :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



class JPSDR_AutoYUY2 : public VDXVideoFilter
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
	bool *interlaced_tab[Interlaced_Tab_Size];
	Image_Data image_data;
	uint16_t lookup[768];
	bool SSE2_Enable;
	size_t CPU_Cache_Size,Cache_Setting;


	inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	inline void Convert_Progressive_to_Planar422(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V);
	void Convert_Progressive_to_Planar422_SSE(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V);
	void Convert_Progressive_to_Planar422_NonSSE(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V);

	inline void Convert_Interlaced_to_Planar422(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V);
	void Convert_Interlaced_to_Planar422_SSE(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V);
	void Convert_Interlaced_to_Planar422_NonSSE(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V);

	void Convert_Automatic_to_Planar422(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_UV,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,const uint8_t thrs);
	void Convert_Test_to_Planar422(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_UV,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,const uint8_t thrs);

	inline void Convert_Progressive_to_YUY2(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Progressive_to_YUY2_SSE(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Progressive_to_YUY2_NonSSE(const void *_src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);

	inline void Convert_Interlaced_to_YUY2(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Interlaced_to_YUY2_SSE(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Interlaced_to_YUY2_NonSSE(const void *_src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);

	void Convert_Automatic_to_YUY2(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs);
	void Convert_Test_to_YUY2(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs);

	inline void Convert_Progressive_to_UYVY(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Progressive_to_UYVY_SSE(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Progressive_to_UYVY_NonSSE(const void *_src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);

	inline void Convert_Interlaced_to_UYVY(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Interlaced_to_UYVY_SSE(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		int32_t w_Y,int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);
	void Convert_Interlaced_to_UYVY_NonSSE(const void *_src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch);

	void Convert_Automatic_to_UYVY(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs);
	void Convert_Test_to_UYVY(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_AutoYUY2Data mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_AutoYUY2)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_AutoYUY2,ScriptConfig,"iii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_AutoYUY2::Init()
{
	uint16_t i;

	for (i=0; i<Interlaced_Tab_Size; i++)
	{
		interlaced_tab[i]=NULL;
	}

	for (i=0; i<256; i++)
	{
		lookup[i]=(uint16_t)(3*i);
		lookup[i+256]=(uint16_t)(5*i);
		lookup[i+512]=(uint16_t)(7*i);
	}

	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	if (IInstrSet<0) InstructionSet();
	CPU_Cache_Size=DataCacheSize(0)>>2;

	return(true);
}



uint32 JPSDR_AutoYUY2::GetParams()
{
	bool full_mode,bt709;
	
	if (g_VFVAPIVersion<12) return(FILTERPARAM_NOT_SUPPORTED);

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

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
			full_mode=false;
			bt709=false;
			break;
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
			full_mode=false;
			bt709=true;
			break;
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
			full_mode=true;
			bt709=false;
			break;
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
		case nsVDXPixmap::kPixFormat_VDXA_YUV :				*/
			full_mode=true;
			bt709=true;
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

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
	pxdst.pitch=0;

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM);

}


void JPSDR_AutoYUY2::Start()
{
	Image_Data idata;
	int8_t i;
	bool ok;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter !");
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

	if ((idata.src_h0<8)||(idata.src_w0<4))
	{
		ff->Except("Size must be at least 4x8 !");
		return;
	}

	for (i=0; i<Interlaced_Tab_Size; i++)
	{
		interlaced_tab[i]=(bool*)malloc(idata.dst_w0*sizeof(bool));
	}

	ok=true;
	for (i=0; i<Interlaced_Tab_Size; i++)
	{
		ok=ok && (interlaced_tab[i]!=NULL);
	}

	if (!ok)
	{
		for (i=Interlaced_Tab_Size-1; i>=0; i--)
		{
			myfree(interlaced_tab[i]);
		}
		ff->ExceptOutOfMemory();
		return;
	}

	const size_t img_size=idata.dst_size0+idata.dst_size1+idata.dst_size2;

	if (img_size<=MAX_CACHE_SIZE)
	{
		if (CPU_Cache_Size>=img_size) Cache_Setting=img_size;
		else Cache_Setting=16;
	}
	else Cache_Setting=16;

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



inline void JPSDR_AutoYUY2::Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src=(uint8_t*)src_;
	uint8_t *dst=(uint8_t*)dst_;

	if ((src_pitch==dst_pitch) && (abs(src_pitch)==w))
	{
		if (src_pitch<0)
		{
			src+=(h-1)*src_pitch;
			dst+=(h-1)*dst_pitch;
		}
		A_memcpy(dst,src,(size_t)h*(size_t)w);
	}
	else
	{
		for(int32_t i=0; i<h; i++)
		{
			A_memcpy(dst,src,w);
			src+=src_pitch;
			dst+=dst_pitch;
		}
	}
}


inline void JPSDR_AutoYUY2::Convert_Interlaced_to_Planar422(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V)
{
	if (((w_U & 0x03)==0) && ((w_V & 0x03)==0) && (SSE2_Enable))
		Convert_Interlaced_to_Planar422_SSE(src_Y,src_U,src_V,dst_Y,dst_U,dst_V,w_Y,w_U,w_V,h_Y,src_pitch_Y,src_pitch_U,
			src_pitch_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V);
	else
		Convert_Interlaced_to_Planar422_NonSSE(src_Y,src_U,src_V,dst_Y,dst_U,dst_V,w_Y,w_U,w_V,h_Y,src_pitch_Y,src_pitch_U,
			src_pitch_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V);
}


inline void JPSDR_AutoYUY2::Convert_Progressive_to_Planar422(const void *src_Y,const void *src_U,const void *src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V)
{
	if (((w_U & 0x03)==0) && ((w_V & 0x03)==0) && (SSE2_Enable))
		Convert_Progressive_to_Planar422_SSE(src_Y,src_U,src_V,dst_Y,dst_U,dst_V,w_Y,w_U,w_V,h_Y,src_pitch_Y,src_pitch_U,
			src_pitch_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V);
	else
		Convert_Progressive_to_Planar422_NonSSE(src_Y,src_U,src_V,dst_Y,dst_U,dst_V,w_Y,w_U,w_V,h_Y,src_pitch_Y,src_pitch_U,
			src_pitch_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V);
}


void JPSDR_AutoYUY2::Convert_Interlaced_to_Planar422_SSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	bool _align_U=false,_align_V=false;
	const int32_t h_4=h_Y-4,w_U4=w_U>>2,w_V4=w_V>>2;

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

	if ((((size_t)src_U & 0x0F)==0) && (((size_t)dst_U & 0x0F)==0) && ((abs(src_pitch_U) & 0x0F)==0) 
		&& ((abs(dst_pitch_U) & 0x0F)==0)) _align_U=true;
	if ((((size_t)src_V & 0x0F)==0) && (((size_t)dst_V & 0x0F)==0) && ((abs(src_pitch_V) & 0x0F)==0) 
		&& ((abs(dst_pitch_V) & 0x0F)==0)) _align_V=true;

	Move_Full(src_Y,dst_Y,w_Y,h_Y,src_pitch_Y,dst_pitch_Y);

	for(int32_t i=0; i<4; i+=4)
	{
		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		A_memcpy(dst_U,src_Un,w_U);
		A_memcpy(dst_V,src_Vn,w_V);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Unn,src_U,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Unn,src_U,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vnn,src_V,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Vnn,src_V,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Un,src_Unnn,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_Un,src_Unnn,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Vn,src_Vnnn,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_Vn,src_Vnnn,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

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

	for(int32_t i=4; i<h_4; i+=4)
	{
		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_U,src_Upp,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_U,src_Upp,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_V,src_Vpp,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_V,src_Vpp,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Up,src_Un,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Up,src_Un,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vp,src_Vn,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Vp,src_Vn,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Unn,src_U,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Unn,src_U,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vnn,src_V,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Vnn,src_V,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Un,src_Unnn,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_Un,src_Unnn,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Vn,src_Vnnn,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_Vn,src_Vnnn,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_U,src_Upp,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_U,src_Upp,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_V,src_Vpp,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3(src_V,src_Vpp,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Up,src_Un,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Up,src_Un,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vp,src_Vn,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2(src_Vp,src_Vn,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		A_memcpy(dst_U,src_Un,w_U);
		A_memcpy(dst_V,src_Vn,w_V);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

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


void JPSDR_AutoYUY2::Convert_Interlaced_to_Planar422_NonSSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst_Y,void *_dst_U,
		void *_dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_V,*dst_U;
	ptrdiff_t pitch_U_2,pitch_V_2;
	const int32_t h_4=h_Y-4;

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

	Move_Full(src_Y,dst_Y,w_Y,h_Y,src_pitch_Y,dst_pitch_Y);

	for(int32_t i=0; i<4; i+=4)
	{
		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_Un,w_U);
		A_memcpy(dst_V,src_Vn,w_V);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[src_U[j]+256]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[src_V[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

	for(int32_t i=4; i<h_4; i+=4)
	{
		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[src_Un[j]+256]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[src_Vn[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[src_U[j]+256]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[src_V[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[src_Un[j]+256]+4)>>3);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[src_Vn[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_Un,w_U);
		A_memcpy(dst_V,src_Vn,w_V);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

void JPSDR_AutoYUY2::Convert_Progressive_to_Planar422_SSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst_Y,void *dst_U,
		void *dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V)
{
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	bool _align_U=false,_align_V=false;
	const int32_t h_2=h_Y-2,w_U4=w_U>>2,w_V4=w_V>>2;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if ((((size_t)src_U & 0x0F)==0) && (((size_t)dst_U & 0x0F)==0) && ((abs(src_pitch_U) & 0x0F)==0) 
		&& ((abs(dst_pitch_U) & 0x0F)==0)) _align_U=true;
	if ((((size_t)src_V & 0x0F)==0) && (((size_t)dst_V & 0x0F)==0) && ((abs(src_pitch_V) & 0x0F)==0) 
		&& ((abs(dst_pitch_V) & 0x0F)==0)) _align_V=true;

	Move_Full(src_Y,dst_Y,w_Y,h_Y,src_pitch_Y,dst_pitch_Y);

	for(int32_t i=0; i<2; i+=2)
	{
		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Un,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_U,src_Un,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vn,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_V,src_Vn,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=2; i<h_2; i+=2)
	{
		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Up,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_U,src_Up,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vp,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_V,src_Vp,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Un,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_U,src_Un,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vn,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_V,src_Vn,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=h_2; i<h_Y; i+=2)
	{
		if (_align_U) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Up,dst_U,w_U4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_U,src_Up,dst_U,w_U4);
		if (_align_V) JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vp,dst_V,w_V4);
		else JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4(src_V,src_Vp,dst_V,w_V4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);


		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}
}



void JPSDR_AutoYUY2::Convert_Progressive_to_Planar422_NonSSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst_Y,void *_dst_U,
		void *_dst_V,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V)
{
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	uint8_t *dst_U,*dst_V;
	const int32_t h_2=h_Y-2;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;
	dst_U=(uint8_t *)_dst_U;
	dst_V=(uint8_t *)_dst_V;

	Move_Full(src_Y,dst_Y,w_Y,h_Y,src_pitch_Y,dst_pitch_Y);

	for(int32_t i=0; i<2; i+=2)
	{
		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=2; i<h_2; i+=2)
	{
		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=h_2; i<h_Y; i+=2)
	{
		for(int32_t j=0; j<w_U; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2)>>2);
		}
		for(int32_t j=0; j<w_V; j++)
		{
			dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2)>>2);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_U,w_U);
		A_memcpy(dst_V,src_V,w_V);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}
}


inline void JPSDR_AutoYUY2::Convert_Progressive_to_YUY2(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	if (((w_Y&0x03)==0) && (SSE2_Enable))
		Convert_Progressive_to_YUY2_SSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
	else
		Convert_Progressive_to_YUY2_NonSSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
}


void JPSDR_AutoYUY2::Convert_Progressive_to_YUY2_SSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	bool _align=false;
	const int32_t h_2=h_Y-2,w_Y4=w_Y>>2;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if ((((size_t)dst & 0x0F)==0) && ((abs(dst_pitch) & 0x0F)==0)) _align=true;

	for(int32_t i=0; i<2; i+=2)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=2; i<h_2; i+=2)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=h_2; i<h_Y; i+=2)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
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


void JPSDR_AutoYUY2::Convert_Progressive_to_YUY2_NonSSE(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_Y;
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	YUYV *dst;
	const int32_t h_2=h_Y-2,w_Y2=(w_Y+1)>>1;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	src_Y=(uint8_t *)_src_Y;
	dst=(YUYV *)_dst;

	for(int32_t i=0; i<2; i+=2)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			j_UV++;
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

	for(int32_t i=2; i<h_2; i+=2)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2)>>2);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			j_UV++;
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

	for(int32_t i=h_2; i<h_Y; i+=2)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2)>>2);
			j_UV++;
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


inline void JPSDR_AutoYUY2::Convert_Interlaced_to_YUY2(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	if (((w_Y&0x03)==0) && (SSE2_Enable))
		Convert_Interlaced_to_YUY2_SSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
	else
		Convert_Interlaced_to_YUY2_NonSSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
}

void JPSDR_AutoYUY2::Convert_Interlaced_to_YUY2_SSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	bool _align=false;
	const int32_t h_4=h_Y-4,w_Y4=w_Y>>2;

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

	if ((((size_t)dst & 0x0F)==0) && ((abs(dst_pitch) & 0x0F)==0)) _align=true;

	for(int32_t i=0; i<4; i+=4)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(src_Y,src_Un,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
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

	for(int32_t i=4; i<h_4; i+=4)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1(src_Y,src_Un,src_Vn,dst,w_Y4);
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


void JPSDR_AutoYUY2::Convert_Interlaced_to_YUY2_NonSSE(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	YUYV *dst;
	const int32_t h_4=h_Y-4,w_Y2=(w_Y+1)>>1;

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

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			j_UV++;
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

	for(int32_t i=4; i<h_4; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			j_UV++;
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
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


inline void JPSDR_AutoYUY2::Convert_Progressive_to_UYVY(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	if (((w_Y&0x03)==0) && (SSE2_Enable))
		Convert_Progressive_to_UYVY_SSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
	else
		Convert_Progressive_to_UYVY_NonSSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
}

void JPSDR_AutoYUY2::Convert_Progressive_to_UYVY_SSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	bool _align=false;
	const int32_t h_2=h_Y-2,w_Y4=w_Y>>2;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	if ((((size_t)dst & 0x0F)==0) && ((abs(dst_pitch) & 0x0F)==0)) _align=true;

	for(int32_t i=0; i<2; i+=2)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=2; i<h_2; i+=2)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4(src_Y,src_U,src_Un,src_V,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	for(int32_t i=h_2; i<h_Y; i+=2)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4(src_Y,src_U,src_Up,src_V,src_Vp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((char *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
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


void JPSDR_AutoYUY2::Convert_Progressive_to_UYVY_NonSSE(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_Y;
	const uint8_t *src_U,*src_Up,*src_Un;
	const uint8_t *src_V,*src_Vp,*src_Vn;
	UYVY *dst;
	const int32_t h_2=h_Y-2,w_Y2=(w_Y+1)>>1;

	src_U=(uint8_t *)_src_U;
	src_V=(uint8_t *)_src_V;
	src_Up=src_U-src_pitch_U;
	src_Un=src_U+src_pitch_U;
	src_Vp=src_V-src_pitch_V;
	src_Vn=src_V+src_pitch_V;

	src_Y=(uint8_t *)_src_Y;
	dst=(UYVY *)_dst;

	for(int32_t i=0; i<2; i+=2)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			j_UV++;
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

	for(int32_t i=2; i<h_2; i+=2)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2)>>2);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			j_UV++;
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

	for(int32_t i=h_2; i<h_Y; i+=2)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2)>>2);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2)>>2);
			j_UV++;
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


inline void JPSDR_AutoYUY2::Convert_Interlaced_to_UYVY(const void *src_Y,const void *src_U,const void *src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	if (((w_Y&0x03)==0) && (SSE2_Enable))
		Convert_Interlaced_to_UYVY_SSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
	else
		Convert_Interlaced_to_UYVY_NonSSE(src_Y,src_U,src_V,dst,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,dst_pitch);
}


void JPSDR_AutoYUY2::Convert_Interlaced_to_UYVY_SSE(const void *src_Y,const void *_src_U,const void *_src_V,void *dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	ptrdiff_t pitch_U_2,pitch_V_2;
	bool _align=false;
	const int32_t h_4=h_Y-4,w_Y4=w_Y>>2;

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

	if ((((size_t)dst & 0x0F)==0) && ((abs(dst_pitch) & 0x0F)==0)) _align=true;

	for(int32_t i=0; i<4; i+=4)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(src_Y,src_Un,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
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

	for(int32_t i=4; i<h_4; i+=4)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2(src_Y,src_Unn,src_U,src_Vnn,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3(src_Y,src_Un,src_Unnn,src_Vn,src_Vnnn,dst,w_Y4);
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3(src_Y,src_U,src_Upp,src_V,src_Vpp,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2(src_Y,src_Up,src_Un,src_Vp,src_Vn,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_U,src_V,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(src_Y,src_U,src_V,dst,w_Y4);
		dst=(void *)((uint8_t *)dst+dst_pitch);
		src_Y=(void *)((uint8_t *)src_Y+src_pitch_Y);

		if (_align) JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b(src_Y,src_Un,src_Vn,dst,w_Y4);
		else JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1(src_Y,src_Un,src_Vn,dst,w_Y4);
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


void JPSDR_AutoYUY2::Convert_Interlaced_to_UYVY_NonSSE(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	UYVY *dst;
	const int32_t h_4=h_Y-4,w_Y2=(w_Y+1)>>1;

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

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			j_UV++;
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

	for(int32_t i=4; i<h_4; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			j_UV++;
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
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



void JPSDR_AutoYUY2::Convert_Automatic_to_YUY2(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	YUYV *dst;
	const int32_t h_4=h_Y-4,w_Y2=(w_Y+1)>>1;
	const int16_t threshold=thrs;

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

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
	
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[1][j_UV]=true;
			else interlaced_tab[1][j_UV]=false;
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[0][j_UV]=true;
			else interlaced_tab[0][j_UV]=false;

			j_UV++;
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

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=4; i<h_4; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4) >> 3);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4) >> 3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2) >> 2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2) >> 2);
			}
			j_UV++;
		}
		dst=(YUYV *)((char *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[index_tab_2][j_UV]=true;
			else interlaced_tab[index_tab_2][j_UV]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_U[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_V[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[index_tab_0][j_UV]=true;
			else interlaced_tab[index_tab_0][j_UV]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_Unn[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_Vnn[j_UV]+2)>>2);
			}
			j_UV++;
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
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


void JPSDR_AutoYUY2::Convert_Test_to_YUY2(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	YUYV *dst;
	const int32_t h_4=h_Y-4,w_Y2=(w_Y+1)>>1;
	const int16_t threshold=thrs;

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

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_AutoYUY2_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
	
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[1][j_UV]=true;
			else interlaced_tab[1][j_UV]=false;
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[0][j_UV]=true;
			else interlaced_tab[0][j_UV]=false;

			j_UV++;
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

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=4; i<h_4; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2) >> 2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2) >> 2);
			}
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[index_tab_2][j_UV]=true;
			else interlaced_tab[index_tab_2][j_UV]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_U[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_V[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[index_tab_0][j_UV]=true;
			else interlaced_tab[index_tab_0][j_UV]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_Unn[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_Vnn[j_UV]+2)>>2);
			}
			j_UV++;
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
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



void JPSDR_AutoYUY2::Convert_Automatic_to_UYVY(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	UYVY *dst;
	const int32_t h_4=h_Y-4,w_Y2=(w_Y+1)>>1;
	const int16_t threshold=thrs;

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

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
	
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[1][j_UV]=true;
			else interlaced_tab[1][j_UV]=false;
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[0][j_UV]=true;
			else interlaced_tab[0][j_UV]=false;

			j_UV++;
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

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=4; i<h_4; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4) >> 3);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4) >> 3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2) >> 2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2) >> 2);
			}
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[index_tab_2][j_UV]=true;
			else interlaced_tab[index_tab_2][j_UV]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_U[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_V[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(UYVY *)((char *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];

			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[index_tab_0][j_UV]=true;
			else interlaced_tab[index_tab_0][j_UV]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_Unn[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_Vnn[j_UV]+2)>>2);
			}
			j_UV++;
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
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


void JPSDR_AutoYUY2::Convert_Test_to_UYVY(const void *_src_Y,const void *_src_U,const void *_src_V,void *_dst,
		const int32_t w_Y,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		const uint8_t thrs)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	const uint8_t *src_Y;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	UYVY *dst;
	const int32_t h_4=h_Y-4,w_Y2=(w_Y+1)>>1;
	const int16_t threshold=thrs;

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

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_AutoYUY2_Convert420_to_UYVY_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Unn[j_UV]]+lookup[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vnn[j_UV]]+lookup[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
	
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[1][j_UV]=true;
			else interlaced_tab[1][j_UV]=false;
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[0][j_UV]=true;
			else interlaced_tab[0][j_UV]=false;

			j_UV++;
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

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=4; i<h_4; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2) >> 2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2) >> 2);
			}
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV])) ||
				((src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV])))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV])) ||
				((src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV])))))
				interlaced_tab[index_tab_2][j_UV]=true;
			else interlaced_tab[index_tab_2][j_UV]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_U[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_V[j_UV]+2)>>2);
			}
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV])) ||
				((src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV])))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV])) ||
				((src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV])))))
				interlaced_tab[index_tab_0][j_UV]=true;
			else interlaced_tab[index_tab_0][j_UV]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].y1=180;
				dst[j_UV].y2=180;
				dst[j_UV].u=128;
				dst[j_UV].v=128;
			}
			else
			{
				dst[j_UV].y1=src_Y[j];
				dst[j_UV].y2=src_Y[j+1];
				dst[j_UV].u=(uint8_t)((lookup[src_Un[j_UV]]+(uint16_t)src_Unn[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup[src_Vn[j_UV]]+(uint16_t)src_Vnn[j_UV]+2)>>2);
			}
			j_UV++;
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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		int32_t j_UV;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(UYVY *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup[src_Up[j_UV]]+lookup[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup[src_Vp[j_UV]]+lookup[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
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


void JPSDR_AutoYUY2::Convert_Automatic_to_Planar422(const void *src_Y,const void *_src_U,const void *_src_V,void *dst_Y,
		void *_dst_U,void *_dst_V,const int32_t w_Y,const int32_t w_UV,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,const uint8_t thrs)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_U,*dst_V;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int32_t h_4=h_Y-4;
	const int16_t threshold=thrs;

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

	Move_Full(src_Y,dst_Y,w_Y,h_Y,src_pitch_Y,dst_pitch_Y);

	for(int32_t i=0; i<4; i+=4)
	{
		A_memcpy(dst_U,src_U,w_UV);
		A_memcpy(dst_V,src_V,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_Un,w_UV);
		A_memcpy(dst_V,src_Vn,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[src_U[j]+256]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[src_V[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
				(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
				(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
				((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))) ||
				((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
				(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
				(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
				((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
				interlaced_tab[1][j]=true;
			else interlaced_tab[1][j]=false;
			if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
				(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
				(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
				((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))) ||
				((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
				(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
				(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
				((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
				interlaced_tab[0][j]=true;
			else interlaced_tab[0][j]=false;

			dst_U[j]=(uint8_t)((lookup[src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=4; i<h_4; i+=4)
	{
		for(int32_t j=0; j<w_UV; j++)
		{
			if ((interlaced_tab[index_tab_1][j]) || (interlaced_tab[index_tab_0][j]))
			{
				dst_U[j]=(uint8_t)((lookup[src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
				dst_V[j]=(uint8_t)((lookup[src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
				(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
				(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
				((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))) ||
				((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
				(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
				(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
				((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
				interlaced_tab[index_tab_2][j]=true;
			else interlaced_tab[index_tab_2][j]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j]) || (interlaced_tab[index_tab_2][j]))
			{
				dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[src_Un[j]+256]+4)>>3);
				dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[src_Vn[j]+256]+4)>>3);
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if ((interlaced_tab[index_tab_1][j]) || (interlaced_tab[index_tab_2][j]))
			{
				dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[src_U[j]+256]+4)>>3);
				dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[src_V[j]+256]+4)>>3);
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
				(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
				(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
				((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))) ||
				((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
				(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
				(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
				((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
				interlaced_tab[index_tab_0][j]=true;
			else interlaced_tab[index_tab_0][j]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j]) || (interlaced_tab[index_tab_0][j]))
			{
				dst_U[j]=(uint8_t)((lookup[src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
				dst_V[j]=(uint8_t)((lookup[src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		for(int32_t j=0; j<w_UV; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[src_Un[j]+256]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[src_Vn[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_U,w_UV);
		A_memcpy(dst_V,src_V,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_Un,w_UV);
		A_memcpy(dst_V,src_Vn,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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


void JPSDR_AutoYUY2::Convert_Test_to_Planar422(const void *src_Y,const void *_src_U,const void *_src_V,void *dst_Y,
		void *_dst_U,void *_dst_V,const int32_t w_Y,const int32_t w_UV,const int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,
		ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,const uint8_t thrs)
{
	const uint8_t *src_U,*src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_V,*src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t *dst_U,*dst_V;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int32_t h_4=h_Y-4;
	const int16_t threshold=thrs;

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

	Move_Full(src_Y,dst_Y,w_Y,h_Y,src_pitch_Y,dst_pitch_Y);

	for(int32_t i=0; i<4; i+=4)
	{
		A_memcpy(dst_U,src_U,w_UV);
		A_memcpy(dst_V,src_V,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_Un,w_UV);
		A_memcpy(dst_V,src_Vn,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[src_U[j]+256]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_Vnn[j]]+lookup[src_V[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
				(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
				(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
				((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))) ||
				((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
				(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
				(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
				((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
				interlaced_tab[1][j]=true;
			else interlaced_tab[1][j]=false;
			if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
				(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
				(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
				((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))) ||
				((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
				(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
				(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
				((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
				interlaced_tab[0][j]=true;
			else interlaced_tab[0][j]=false;

			dst_U[j]=(uint8_t)((lookup[src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_Vn[j]+512]+(uint16_t)src_Vnnn[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=4; i<h_4; i+=4)
	{
		for(int32_t j=0; j<w_UV; j++)
		{
			if ((interlaced_tab[index_tab_1][j]) || (interlaced_tab[index_tab_0][j]))
			{
				dst_U[j]=239;
				dst_V[j]=239;
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Up[j]+2) >> 2);
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vp[j]+2) >> 2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
				(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
				(((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j])) ||
				((src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j])))) ||
				((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
				(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
				(((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j])) ||
				((src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j])))))
				interlaced_tab[index_tab_2][j]=true;
			else interlaced_tab[index_tab_2][j]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j]) || (interlaced_tab[index_tab_2][j]))
			{
				dst_U[j]=239;
				dst_V[j]=239;
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_U[j]]+(uint16_t)src_Un[j]+2)>>2);
				dst_V[j]=(uint8_t)((lookup[src_V[j]]+(uint16_t)src_Vn[j]+2)>>2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if ((interlaced_tab[index_tab_1][j]) || (interlaced_tab[index_tab_2][j]))
			{
				dst_U[j]=239;
				dst_V[j]=239;
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_U[j]+2)>>2);
				dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_V[j]+2)>>2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			if (((abs((uint8_t)src_Un[j]-(uint8_t)src_Unn[j])>=threshold) &&
				(abs((uint8_t)src_Unnn[j]-(uint8_t)src_Unn[j])>=threshold) &&
				(((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j])) ||
				((src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j])))) ||
				((abs((uint8_t)src_Vn[j]-(uint8_t)src_Vnn[j])>=threshold) &&
				(abs((uint8_t)src_Vnnn[j]-(uint8_t)src_Vnn[j])>=threshold) &&
				(((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j])) ||
				((src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j])))))
				interlaced_tab[index_tab_0][j]=true;
			else interlaced_tab[index_tab_0][j]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j]) || (interlaced_tab[index_tab_0][j]))
			{
				dst_U[j]=239;
				dst_V[j]=239;
			}
			else
			{
				dst_U[j]=(uint8_t)((lookup[src_Un[j]]+(uint16_t)src_Unn[j]+2)>>2);
				dst_V[j]=(uint8_t)((lookup[src_Vn[j]]+(uint16_t)src_Vnn[j]+2)>>2);
			}
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

	for(int32_t i=h_4; i<h_Y; i+=4)
	{
		for(int32_t j=0; j<w_UV; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_U[j]+512]+(uint16_t)src_Upp[j]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_V[j]+512]+(uint16_t)src_Vpp[j]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		for(int32_t j=0; j<w_UV; j++)
		{
			dst_U[j]=(uint8_t)((lookup[src_Up[j]]+lookup[src_Un[j]+256]+4)>>3);
			dst_V[j]=(uint8_t)((lookup[src_Vp[j]]+lookup[src_Vn[j]+256]+4)>>3);
		}
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_U,w_UV);
		A_memcpy(dst_V,src_V,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

		A_memcpy(dst_U,src_Un,w_UV);
		A_memcpy(dst_V,src_Vn,w_UV);
		dst_U+=dst_pitch_U;
		dst_V+=dst_pitch_V;

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

void JPSDR_AutoYUY2::Run()
{
	Image_Data idata;

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

	SetMemcpyCacheLimit(Cache_Setting);
	SetMemsetCacheLimit(Cache_Setting);

	switch(idata.dst_video_mode)
	{
		case 2 :
			switch(mData.convert_mode)
			{
				case 0 :
					Convert_Progressive_to_YUY2(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0);
					break;
				case 1 :
					Convert_Interlaced_to_YUY2(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0);
					break;
				case 2 :
					Convert_Automatic_to_YUY2(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,mData.threshold);
					break;
				case 3 :
					Convert_Test_to_YUY2(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,mData.threshold);
					break;
			}
			break;
		case 3 :
			switch(mData.convert_mode)
			{
				case 0 :
					Convert_Progressive_to_UYVY(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0);
					break;
				case 1 :
					Convert_Interlaced_to_UYVY(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0);
					break;
				case 2 :
					Convert_Automatic_to_UYVY(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,mData.threshold);
					break;
				case 3 :
					Convert_Test_to_UYVY(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.src_w0,
						idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,mData.threshold);
					break;
			}
			break;
		case 5 :
			switch(mData.convert_mode)
			{
				case 0 :
					Convert_Progressive_to_Planar422(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,
						idata.dst_plane1,idata.dst_plane2,idata.src_w0,idata.src_w1,idata.src_w2,idata.src_h0,idata.src_pitch0,
						idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
					break;
				case 1 :
					Convert_Interlaced_to_Planar422(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,
						idata.dst_plane1,idata.dst_plane2,idata.src_w0,idata.src_w1,idata.src_w2,idata.src_h0,idata.src_pitch0,
						idata.src_pitch1,idata.src_pitch2,idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2);
					break;
				case 2 :
					Convert_Automatic_to_Planar422(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,
						idata.dst_plane2,idata.src_w0,idata.src_w1,idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,
						idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2,mData.threshold);
					break;
				case 3 :
					Convert_Test_to_Planar422(idata.src_plane0,idata.src_plane1,idata.src_plane2,idata.dst_plane0,idata.dst_plane1,
						idata.dst_plane2,idata.src_w0,idata.src_w1,idata.src_h0,idata.src_pitch0,idata.src_pitch1,idata.src_pitch2,
						idata.dst_pitch0,idata.dst_pitch1,idata.dst_pitch2,mData.threshold);
					break;
			}
			break;
		default : break;
	}

}




void JPSDR_AutoYUY2::End()
{
	int8_t i;

	for (i=Interlaced_Tab_Size-1; i>=0; i--)
	{
		myfree(interlaced_tab[i]);
	}
}



void JPSDR_AutoYUY2::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.output_format=argv[0].asInt();
	mData.convert_mode=argv[1].asInt();
	mData.threshold=argv[2].asInt();
}

bool JPSDR_AutoYUY2::Configure(VDXHWND hwnd)
{
	JPSDR_AutoYUY2Dialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_AutoYUY2::GetScriptString(char *buf, int maxlen)
{
 
  
    SafePrintf(buf, maxlen, "Config(%d, %d, %d)",
		mData.output_format,mData.convert_mode,mData.threshold);
}



extern VDXFilterDefinition filterDef_JPSDR_AutoYUY2=
VDXVideoFilterDefinition<JPSDR_AutoYUY2>("JPSDR","Auto YUY2 v1.6.0","Convert Planar4:2:0 to severals 4:2:2 modes. [SSE2] Optimised.");

