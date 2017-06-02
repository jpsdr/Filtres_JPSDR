#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "Deinterlace_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

#define Number_Max_Lines 2048

extern ThreadPoolInterface *poolInterface;

extern int g_VFVAPIVersion;

extern "C" void JPSDR_Deinterlace_Blend_Non_MMX_8(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Non_MMX_24(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Non_MMX_32(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);	

extern "C" void JPSDR_Deinterlace_Blend_SSE_3_A(const void *src1,const void *src2, void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);		

extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c(const void *src,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
		
extern "C" void JPSDR_Deinterlace_YadifAbsDiffAvg_SSE(const void *src_a, const void *scr_b, void *dst_abs, void *dst_avg,
		ptrdiff_t src_pitch, ptrdiff_t dst_pitch, int32_t w, int32_t h);
extern "C" void JPSDR_Deinterlace_YadifAbsDiff_SSE(const void *src_a, const void *scr_b, void *dst,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch, int32_t w, int32_t h);
extern "C" void JPSDR_Deinterlace_YadifAvg_SSE(const void *src_a, const void *scr_b, void *dst,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch, int32_t w, int32_t h);


typedef struct _Yadif_Buffer
{
	void *data[3];
	void *absdiff[3];
} Yadif_Buffer;


class JPSDR_DeinterlaceData
{
public:
	uint8_t mode;
	bool keepTop,TFF;
	bool file_mode;
	char filename[1024];
	bool mt_mode;

	JPSDR_DeinterlaceData(void);
};



JPSDR_DeinterlaceData::JPSDR_DeinterlaceData(void)
{
	mode=1;
	keepTop=true;
	TFF=true;
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
		EnableWindow(GetDlgItem(mhdlg,IDC_YADIF),true);
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
				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),false);
				break;
			case 5 :
			case 6 :
				EnableWindow(GetDlgItem(mhdlg,IDC_ENABLE_MT),false);
				EnableWindow(GetDlgItem(mhdlg,IDC_TOP),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BOTTOM),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_DISCARD),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_TFF),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_BFF),true);
				EnableWindow(GetDlgItem(mhdlg,IDC_FIELD_ORDER),true);
				break;
			default :
				EnableWindow(GetDlgItem(mhdlg,IDC_ENABLE_MT),true);
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
	mData.mt_mode=!!IsDlgButtonChecked(mhdlg,IDC_ENABLE_MT);
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
		case IDC_ENABLE_MT :
			if (mifp && SaveToData()) mifp->RedoSystem();
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
	virtual ~JPSDR_Deinterlace();

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
	void *spatial_Yadif_buffer;
	void *avg1[3],*avg2[3],*avg3[3],*abs1[3];
	uint8_t write_index_Yadif,current_frame_Yadif,previous_frame_Yadif,next_frame_Yadif;
	void *buffer0,*buffer1,*buffer2;
	ptrdiff_t buffer0_pitch,buffer0_modulo;
	ptrdiff_t buffer1_pitch,buffer1_modulo;
	ptrdiff_t buffer2_pitch,buffer2_modulo;
	void *buffer[3][2];
	uint32_t debut[Number_Max_Lines],fin[Number_Max_Lines];
	uint8_t tab_mode[Number_Max_Lines];
	uint16_t nbre_file_mode;
	bool SSE2_Enable;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits);

	void Deinterlace_Blend_MT_1(uint8_t thread_num);
	void Deinterlace_Blend_MT_2(uint8_t thread_num);
	void Deinterlace_Blend_MT_3(uint8_t thread_num);
	void Deinterlace_Blend_MT_4(uint8_t thread_num);
	void Deinterlace_Blend_MT_5(uint8_t thread_num);

	void Deinterlace_Tri_MT_1(uint8_t thread_num);
	void Deinterlace_Tri_MT_2(uint8_t thread_num);
	void Deinterlace_Tri_MT_3(uint8_t thread_num);
	void Deinterlace_Tri_MT_4(uint8_t thread_num);
	void Deinterlace_Tri_MT_5(uint8_t thread_num);

	void Blend_Tri_RGB32(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_RGB32a(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_RGB32b(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_RGB32c(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void Blend_Tri_RGB32(const void *src, void *dst,void *buffer[],const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void Blend_Tri_YUV32(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32a(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32b(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32c(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void Blend_Tri_YUV32(const void *src, void *dst,void *buffer[],const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void Blend_Tri_Planar8(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_Planar8a(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_Planar8b(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Blend_Tri_Planar8c(const void *src, void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void Blend_Tri_Planar8(const void *src, void *dst,void *buffer[],const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void YadifAbsDiff(const void *src_a, const void *scr_b, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifAbsDiffAvg(const void *src_a, const void *scr_b, void *dst_abs, void *dst_avg, ptrdiff_t src_pitch,
		ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifAvgKeepTop(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifAvgKeepBottom(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifSpatialPlanarKeepTop(void *buffer, void *dst, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifSpatialPlanarKeepBottom(void *buffer, void *dst, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifTemporalPlanarKeepBottom(const void *src, void *avg1, void *avg2, void *avg3, void *abs1, void *dst, ptrdiff_t src_pitch,
		ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo,
		int32_t w, int32_t h);
	void YadifTemporalPlanarKeepTop(const void *src, void *avg1, void *avg2, void *avg3, void *abs1, void *dst, ptrdiff_t src_pitch,
		ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo,
		const int32_t w, const int32_t h);
	void YadifSpatiafBufferPlanarKeepTop(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void YadifSpatiafBufferPlanarKeepBottom(const void *src, void *dst, ptrdiff_t src_pitch, ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h);
	void AddBorderRGB(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h);
	void AddBorderYUYV(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h);
	void AddBorderUYVY(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h);
	void AddBorderPlanar(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_DeinterlaceData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Deinterlace)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Deinterlace,ScriptConfig,"iiis")
VDXVF_END_SCRIPT_METHODS()



bool JPSDR_Deinterlace::Init()
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
	for (i=0; i<3; i++)
	{
		Yadif_pitch[i]=0;
		Yadif_modulo[i]=0;
		avg1[i]=NULL;
		avg2[i]=NULL;
		avg3[i]=NULL;
		abs1[i]=NULL;
		for (j=0; j<3; j++)
		{
			tab_Yadif[i].data[j]=NULL;
			tab_Yadif[i].absdiff[j]=NULL;
		}
	}
	spatial_Yadif_buffer=NULL;

	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	for (i=0; i<MAX_MT_THREADS; i++)
	{
		MT_Thread[i].pClass=NULL;
		MT_Thread[i].f_process=0;
		MT_Thread[i].thread_Id=(uint8_t)i;
		MT_Thread[i].pFunc=NULL;
	}

	UserId=0;
	if (poolInterface->GetThreadPoolInterfaceStatus())
	{
		total_cpu=poolInterface->GetThreadNumber(0,true);

		if (total_cpu>0)
			threadpoolAllocated=poolInterface->AllocateThreads(total_cpu,0,0,true,false,true,-1);
		else threadpoolAllocated=false;
		if (threadpoolAllocated) poolInterface->GetUserId(UserId);
	}
	else
	{
		total_cpu=0;
		threadpoolAllocated=false;
	}

	return(true);
}


JPSDR_Deinterlace::~JPSDR_Deinterlace()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
}


uint8_t JPSDR_Deinterlace::CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y,uint8_t div_x,uint8_t div_y,uint8_t _32bits)
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
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w+3)>>2;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w+3)>>2;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w+3)>>2;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w+3)>>2;
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
				MT_Data[0].src_Y_w_32=(MT_Data[0].src_Y_w+3)>>2;
				MT_Data[0].dst_Y_w_32=(MT_Data[0].dst_Y_w+3)>>2;
				MT_Data[0].src_UV_w_32=(MT_Data[0].src_UV_w+3)>>2;
				MT_Data[0].dst_UV_w_32=(MT_Data[0].dst_UV_w+3)>>2;
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
				MT_Data[i].src_Y_w_32=(MT_Data[i].src_Y_w+3)>>2;
				MT_Data[i].dst_Y_w_32=(MT_Data[i].dst_Y_w+3)>>2;
				MT_Data[i].src_UV_w_32=(MT_Data[i].src_UV_w+3)>>2;
				MT_Data[i].dst_UV_w_32=(MT_Data[i].dst_UV_w+3)>>2;
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

	if (mData.mt_mode)
	{
		if (!mData.file_mode)
		{
			switch(mData.mode)
			{
				case 1 :
				case 2 :
					if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
					else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					break;
				case 5 :
				case 6 :
					return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
					break;
				default :
					if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
					else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					break;
			}
		}
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
	}
	else
	{
		if (!mData.file_mode)
		{
			switch(mData.mode)
			{
				case 2 :
					if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
					else return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					break;
				case 5 :
				case 6 :
					return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
					break;
				default :
					if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
					else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
					break;
			}
		}
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
	}
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
		ff->Except("This virtualdub version doesn't support this configuration filter!");
	}

	if (!poolInterface->GetThreadPoolInterfaceStatus())
	{
		ff->Except("Error with the TheadPool status!");
		return;
	}

	if (UserId==0)
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

	if ((idata.src_h0<8) || (idata.src_w0<8))
	{
		ff->Except("Size must be at least 8x8!");
		return;
	}

	switch (idata.video_mode)
	{
		case 6 :
			if ((idata.src_h0%4)!=0)
			{
				ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
				return;
			}
			break;
		case 8 :
			if ((idata.src_h0%8)!=0)
			{
				ff->Except("Vertical size must be multiple of 8 for 4:1:0!");
				return;
			}
			break;
		default :
			if ((idata.src_h0%2)!=0)
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

	if ((!mData.file_mode) && mData.mt_mode && (mData.mode<5) &&
		((idata.src_h0>=32) && (idata.dst_h0>=32))) threads_number=total_cpu;
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

	buff_ok=true;
	for (i=0; i<2; i++)
	{
		switch (idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
			case 9 :
				buffer[0][i]=(void *)_aligned_malloc(idata.src_pitch0_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				buffer[0][i]=(void *)_aligned_malloc(idata.src_pitch0_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[0][i]!=NULL);
				buffer[1][i]=(void *)_aligned_malloc(idata.src_pitch1_al,ALIGN_SIZE);
				buff_ok=buff_ok && (buffer[1][i]!=NULL);
				buffer[2][i]=(void *)_aligned_malloc(idata.src_pitch2_al,ALIGN_SIZE);
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
		buffer0=(void *)_aligned_malloc(idata.src_size0_al,ALIGN_SIZE);
		buffer0_pitch=idata.src_pitch0_al;
		buffer0_modulo=idata.src_modulo0_al;
		if (idata.src_size1!=0)
		{
			buffer1=(void *)_aligned_malloc(idata.src_size1_al,ALIGN_SIZE);
			buffer1_pitch=idata.src_pitch1_al;
			buffer1_modulo=idata.src_modulo1_al;
		}
		else buffer1=NULL;
		if (idata.src_size2!=0)
		{
			buffer2=(void *)_aligned_malloc(idata.src_size2_al,ALIGN_SIZE);
			buffer2_pitch=idata.src_pitch2_al;
			buffer2_modulo=idata.src_modulo2_al;
		}
		else buffer2=NULL;
		buff_ok=(buffer0!=NULL);
		if (idata.src_size1!=0) buff_ok=buff_ok && (buffer1!=NULL);
		if (idata.src_size2!=0) buff_ok=buff_ok && (buffer2!=NULL);
		if (!buff_ok)
		{
			ff->ExceptOutOfMemory();
			return;
		}
	}

	if ((mData.mode==5) || (mData.mode==6))
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				Yadif_pitch[0]=idata.src_pitch0_al;
				Yadif_pitch[0]+=(ALIGN_SIZE << 1);
				Yadif_modulo[0]=Yadif_pitch[0]-idata.src_line0;
				for (i=0; i<3; i++)
				{
					tab_Yadif[i].data[0]=(void *)_aligned_malloc(idata.src_h0*Yadif_pitch[0],ALIGN_SIZE);
					tab_Yadif[i].absdiff[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				}
				avg1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg2[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg3[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				abs1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				spatial_Yadif_buffer=(void *)_aligned_malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10),ALIGN_SIZE);
				break;
			case 2 :
			case 3 :
				Yadif_pitch[0]=idata.src_pitch0_al;
				Yadif_pitch[0]+=(ALIGN_SIZE << 1);
				Yadif_modulo[0]=Yadif_pitch[0]-idata.src_line0;
				for (i=0; i<3; i++)
				{
					tab_Yadif[i].data[0]=(void *)_aligned_malloc(idata.src_h0*Yadif_pitch[0],ALIGN_SIZE);
					tab_Yadif[i].absdiff[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				}
				avg1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg2[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg3[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				abs1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				spatial_Yadif_buffer=(void *)_aligned_malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10),ALIGN_SIZE);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				Yadif_pitch[0]=idata.src_pitch0_al;
				Yadif_pitch[1]=idata.src_pitch1_al;
				Yadif_pitch[2]=idata.src_pitch2_al;;
				Yadif_pitch[0]+=(ALIGN_SIZE << 1);
				Yadif_pitch[1]+=(ALIGN_SIZE << 1);
				Yadif_pitch[2]+=(ALIGN_SIZE << 1);
				Yadif_modulo[0]=Yadif_pitch[0]-idata.src_line0;
				Yadif_modulo[1]=Yadif_pitch[1]-idata.src_line1;
				Yadif_modulo[2]=Yadif_pitch[2]-idata.src_line2;
				for (i=0; i<3; i++)
				{
					tab_Yadif[i].data[0]=(void *)_aligned_malloc(idata.src_h0*Yadif_pitch[0],ALIGN_SIZE);
					tab_Yadif[i].absdiff[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
					tab_Yadif[i].data[1]=(void *)_aligned_malloc(idata.src_h1*Yadif_pitch[1],ALIGN_SIZE);
					tab_Yadif[i].absdiff[1]=(void *)_aligned_malloc((idata.src_h1>>1)*Yadif_pitch[1],ALIGN_SIZE);
					tab_Yadif[i].data[2]=(void *)_aligned_malloc(idata.src_h2*Yadif_pitch[2],ALIGN_SIZE);
					tab_Yadif[i].absdiff[2]=(void *)_aligned_malloc((idata.src_h2>>1)*Yadif_pitch[2],ALIGN_SIZE);
				}
				avg1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg2[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg3[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				abs1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg1[1]=(void *)_aligned_malloc((idata.src_h1>>1)*Yadif_pitch[1],ALIGN_SIZE);
				avg2[1]=(void *)_aligned_malloc((idata.src_h1>>1)*Yadif_pitch[1],ALIGN_SIZE);
				avg3[1]=(void *)_aligned_malloc((idata.src_h1>>1)*Yadif_pitch[1],ALIGN_SIZE);
				abs1[1]=(void *)_aligned_malloc((idata.src_h1>>1)*Yadif_pitch[1],ALIGN_SIZE);
				avg1[2]=(void *)_aligned_malloc((idata.src_h2>>1)*Yadif_pitch[2],ALIGN_SIZE);
				avg2[2]=(void *)_aligned_malloc((idata.src_h2>>1)*Yadif_pitch[2],ALIGN_SIZE);
				avg3[2]=(void *)_aligned_malloc((idata.src_h2>>1)*Yadif_pitch[2],ALIGN_SIZE);
				abs1[2]=(void *)_aligned_malloc((idata.src_h2>>1)*Yadif_pitch[2],ALIGN_SIZE);
				spatial_Yadif_buffer=(void *)_aligned_malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10),ALIGN_SIZE);
				break;
			case 9 :
				Yadif_pitch[0]=idata.src_pitch0_al;
				Yadif_pitch[0]+=(ALIGN_SIZE << 1);
				Yadif_modulo[0]=Yadif_pitch[0]-idata.src_line0;
				for (i=0; i<3; i++)
				{
					tab_Yadif[i].data[0]=(void *)_aligned_malloc(idata.src_h0*Yadif_pitch[0],ALIGN_SIZE);
					tab_Yadif[i].absdiff[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				}
				avg1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg2[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				avg3[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				abs1[0]=(void *)_aligned_malloc((idata.src_h0>>1)*Yadif_pitch[0],ALIGN_SIZE);
				spatial_Yadif_buffer=(void *)_aligned_malloc((idata.src_h0>>1)*(Yadif_pitch[0]*10),ALIGN_SIZE);
				break;
		}

		buff_ok=true;
		for (i=0; i<3; i++)
		{
			for (j=0; j<3; j++)
			{
				if (Yadif_pitch[j]!=0)
				{
					buff_ok=buff_ok && (tab_Yadif[i].data[j]!=NULL);
					buff_ok=buff_ok && (tab_Yadif[i].absdiff[j]!=NULL);
				}
			}
		}
		for (i=0; i<3; i++)
		{
			if (Yadif_pitch[i]!=0)
			{
				buff_ok=buff_ok && (avg1[i]!=NULL);
				buff_ok=buff_ok && (avg2[i]!=NULL);
				buff_ok=buff_ok && (avg3[i]!=NULL);
				buff_ok=buff_ok && (abs1[i]!=NULL);
			}
		}
		buff_ok=buff_ok && (spatial_Yadif_buffer!=NULL);

		if (!buff_ok)
		{
			ff->ExceptOutOfMemory();
			return;
		}
		for (i=0; i<3; i++)
		{
			for (j=0; j<3; j++)
			{
				if (Yadif_pitch[j]!=0)
				{
					tab_Yadif[i].data[j]=(void *)((size_t)tab_Yadif[i].data[j]+ALIGN_SIZE);
					tab_Yadif[i].absdiff[j]=(void *)((size_t)tab_Yadif[i].absdiff[j]+ALIGN_SIZE);
				}
			}
		}

	}

	if (threads_number>1)
	{
		if (!threadpoolAllocated)
		{
			ff->Except("Error with the TheadPool while allocating threadpool!");
			return;
		}

		StaticThreadpoolF=StaticThreadpool;

		for (i=0; i<threads_number; i++)
		{
			MT_Thread[i].pClass=this;
			MT_Thread[i].f_process=0;
			MT_Thread[i].thread_Id=(uint8_t)i;
			MT_Thread[i].pFunc=StaticThreadpoolF;
		}
	}

	write_index_Yadif=0;
	current_frame_Yadif=1;
	previous_frame_Yadif=0;
	next_frame_Yadif=2;
}
	

void JPSDR_Deinterlace::End()
{
	int16_t i,j;

	my_aligned_free(spatial_Yadif_buffer);
	for (i=2; i>=0; i--)
	{
		my_aligned_free(abs1[i]);
		my_aligned_free(avg3[i]);
		my_aligned_free(avg2[i]);
		my_aligned_free(avg1[i]);
	}

	for (i=2; i>=0; i--)
	{
		Yadif_pitch[i]=0;
		Yadif_modulo[i]=0;
		for (j=2; j>=0; j--)
		{
			my_aligned_free(tab_Yadif[i].absdiff[j]);
			my_aligned_free(tab_Yadif[i].data[j]);
		}
	}
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



#define fastmin(a, b) ((a) + (((b) - (a)) & (((b) - (a)) >> 31)))
#define fastmax(a, b) ((b) - (((b) - (a)) & (((b) - (a)) >> 31)))
#define fastminmax(a, b, mn, mx) ((t = (b) - (a)), (t &= t >> 31), ((mn) = (a) + t), ((mx) = (b) - t))


void JPSDR_Deinterlace::YadifTemporalPlanarKeepBottom(const void *src, void *_avg1, void *_avg2, void *_avg3,void *_abs1, void *_dst,
	ptrdiff_t src_pitch, ptrdiff_t src_modulo, ptrdiff_t buffer_pitch, ptrdiff_t buffer_modulo,ptrdiff_t dst_pitch,
	ptrdiff_t dst_modulo, const int32_t w, const int32_t _h)
{
	const uint8_t *srcT,*srcB;
	uint8_t *avgT,*avgC,*avgB;
	uint8_t *avg2,*avg3,*abs1;
	uint8_t *dst;
	const int32_t h=_h-2;

	srcT=(uint8_t *)src;
	srcB=srcT+(src_pitch<<1);
	avgT=(uint8_t *)_avg1;
	avgC=avgT+buffer_pitch;
	avgB=avgC+buffer_pitch;
	avg2=(uint8_t *)_avg2+buffer_pitch;
	avg3=(uint8_t *)_avg3+buffer_pitch;
	abs1=(uint8_t *)_abs1+buffer_pitch;
	dst=(uint8_t *)_dst+(dst_pitch<<1);

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
	ptrdiff_t dst_modulo, const int32_t w, const int32_t _h)
{
	const uint8_t *srcT,*srcB;
	uint8_t *avgT,*avgC,*avgB;
	uint8_t *avg2,*avg3,*abs1;
	uint8_t *dst;
	const int32_t h=_h-2;

	srcT=(uint8_t *)src+(src_pitch<<1);
	srcB=srcT+(src_pitch<<1);
	avgT=(uint8_t *)_avg1;
	avgC=avgT+buffer_pitch;
	avgB=avgC+buffer_pitch;
	avg2=(uint8_t *)_avg2+buffer_pitch;
	avg3=(uint8_t *)_avg3+buffer_pitch;
	abs1=(uint8_t *)_abs1+buffer_pitch;
	dst=(uint8_t *)_dst+(dst_pitch<<1);

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
	ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const int32_t h_1=h-1,w_2=w+2;
	const uint8_t *src1,*src2;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+(src_pitch<<1);
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
	ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const int32_t h_1=h-1,w_2=w+2;
	const uint8_t *src1,*src2;
	uint8_t *dst;

	src1=(uint8_t *)src+src_pitch;
	src2=src1+(src_pitch<<1);
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
	ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const int32_t h_1=h-1;
	uint8_t *buffer,*dst;
	uint32_t offset;

	dst=(uint8_t *)_dst;
	buffer=(uint8_t *)_buffer;

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
	ptrdiff_t buffer_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const int32_t h_1=h-1;
	uint8_t *buffer,*dst;
	uint32_t offset;

	dst=(uint8_t *)_dst;
	buffer=(uint8_t *)_buffer;

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
	ptrdiff_t src_pitch, ptrdiff_t src_modulo, ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
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
	ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
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
	ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const uint8_t *src1,*src2;
	uint8_t *dst;
	const int32_t h_1=h-1;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	dst=(uint8_t *)_dst;

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
	ptrdiff_t dst_pitch, ptrdiff_t dst_modulo, const int32_t w, const int32_t h)
{
	const uint8_t *src1,*src2;
	uint8_t *dst;
	const int32_t h_1=h-1;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	dst=(uint8_t *)_dst;

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


void JPSDR_Deinterlace::AddBorderRGB(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h)
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


void JPSDR_Deinterlace::AddBorderYUYV(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h)
{
	YUYV *ptr;
	const int32_t w0=(w+1)>>1;

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


void JPSDR_Deinterlace::AddBorderUYVY(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h)
{
	UYVY *ptr;
	const int32_t w0=(w+1)>>1;

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


void JPSDR_Deinterlace::AddBorderPlanar(void *dst,ptrdiff_t dst_pitch, const int32_t w, const int32_t h)
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

		
void JPSDR_Deinterlace::Blend_Tri_RGB32(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst[j].g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst[j].b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2;
			dst[j].g=(((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2;
			dst[j].b=(((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2;
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst[j].g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst[j].b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst[j].alpha=src2[j].alpha;
	}

}



void JPSDR_Deinterlace::Blend_Tri_RGB32a(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst[j].g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst[j].b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst[j].alpha=src1[j].alpha;
	}
	dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].r=(((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2;
			dst[j].g=(((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2;
			dst[j].b=(((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2;
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}
}


void JPSDR_Deinterlace::Blend_Tri_RGB32b(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
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
			dst[j].r=(((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2;
			dst[j].g=(((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2;
			dst[j].b=(((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2;
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}

}



void JPSDR_Deinterlace::Blend_Tri_RGB32c(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
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
			dst[j].r=(((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2;
			dst[j].g=(((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2;
			dst[j].b=(((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2;
			dst[j].alpha=src2[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst[j].g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst[j].b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst[j].alpha=src2[j].alpha;
	}

}

void JPSDR_Deinterlace::Blend_Tri_RGB32(const void *src,void *_dst,void *buffer[],const int32_t w,
	const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
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
		dst[j].r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst[j].g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst[j].b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
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
			dst[j].r=(((uint16_t)src1[j].r+(uint16_t)src3[j].r+(((uint16_t)src2[j].r) << 1))+2) >> 2;
			dst[j].g=(((uint16_t)src1[j].g+(uint16_t)src3[j].g+(((uint16_t)src2[j].g) << 1))+2) >> 2;
			dst[j].b=(((uint16_t)src1[j].b+(uint16_t)src3[j].b+(((uint16_t)src2[j].b) << 1))+2) >> 2;
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
		dst[j].r=((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1;
		dst[j].g=((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1;
		dst[j].b=((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1;
		dst[j].alpha=src2[j].alpha;
	}

}


void JPSDR_Deinterlace::Blend_Tri_YUV32(const void *src,void *_dst,const int32_t w,const int32_t h,
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
		dst[j].y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst[j].u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst[j].y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst[j].v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
	}
	dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2;
			dst[j].u=(((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2;
			dst[j].y2=(((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2;
			dst[j].v=(((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst[j].u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst[j].y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst[j].v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
	}

}



void JPSDR_Deinterlace::Blend_Tri_YUV32a(const void *src,void *_dst,const int32_t w,const int32_t h,
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
		dst[j].y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst[j].u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst[j].y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst[j].v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
	}
	dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2;
			dst[j].u=(((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2;
			dst[j].y2=(((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2;
			dst[j].v=(((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

}



void JPSDR_Deinterlace::Blend_Tri_YUV32b(const void *src,void *_dst,const int32_t w,const int32_t h,
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
			dst[j].y1=(((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2;
			dst[j].u=(((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2;
			dst[j].y2=(((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2;
			dst[j].v=(((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

}



void JPSDR_Deinterlace::Blend_Tri_YUV32c(const void *src,void *_dst,const int32_t w,const int32_t h,
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
			dst[j].y1=(((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2;
			dst[j].u=(((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2;
			dst[j].y2=(((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2;
			dst[j].v=(((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst[j].u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst[j].y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst[j].v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
	}

}


void JPSDR_Deinterlace::Blend_Tri_YUV32(const void *src,void *_dst,void *buffer[],const int32_t w,
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
		dst[j].y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst[j].u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst[j].y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst[j].v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
	}
	dst=(YUYV *)((uint8_t *)dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w << 2);
		indice_buffer=(indice_buffer+1)%2;
		src1=(YUYV *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
		{
			dst[j].y1=(((uint16_t)src1[j].y1+(uint16_t)src3[j].y1+(((uint16_t)src2[j].y1) << 1))+2) >> 2;
			dst[j].u=(((uint16_t)src1[j].u+(uint16_t)src3[j].u+(((uint16_t)src2[j].u) << 1))+2) >> 2;
			dst[j].y2=(((uint16_t)src1[j].y2+(uint16_t)src3[j].y2+(((uint16_t)src2[j].y2) << 1))+2) >> 2;
			dst[j].v=(((uint16_t)src1[j].v+(uint16_t)src3[j].v+(((uint16_t)src2[j].v) << 1))+2) >> 2;
		}
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		src3=(YUYV *)((uint8_t *)src3+src_pitch);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(YUYV *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
	{
		dst[j].y1=((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1;
		dst[j].u=((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1;
		dst[j].y2=((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1;
		dst[j].v=((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1;
	}

}



void JPSDR_Deinterlace::Blend_Tri_Planar8(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
		dst[j]=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;
	dst+=dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
			dst[j]=(((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2;
		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

	for (int32_t j=0; j<w; j++)
		dst[j]=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;

}



void JPSDR_Deinterlace::Blend_Tri_Planar8a(const void *src,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src1,*src2,*src3;
	uint8_t *dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	dst=(uint8_t *)_dst;

	for (int32_t j=0; j<w; j++)
		dst[j]=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;
	dst+=dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
			dst[j]=(((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2;
		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}
}



void JPSDR_Deinterlace::Blend_Tri_Planar8b(const void *src,void *_dst,const int32_t w,const int32_t h,
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
			dst[j]=(((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2;
		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

}



void JPSDR_Deinterlace::Blend_Tri_Planar8c(const void *src,void *_dst,const int32_t w,const int32_t h,
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
		{
			dst[j]=(((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2;
		}
		src1+=src_pitch;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

	for (int32_t j=0; j<w; j++)
	{
		dst[j]=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;
	}

}


void JPSDR_Deinterlace::Blend_Tri_Planar8(const void *src,void *_dst,void *buffer[],const int32_t w,
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
		dst[j]=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;
	dst+=dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		memcpy(buffer[indice_buffer],src2,w);
		indice_buffer=(indice_buffer+1)%2;
		src1=(uint8_t *)buffer[indice_buffer];
		for (int32_t j=0; j<w; j++)
			dst[j]=(((uint16_t)src1[j]+(uint16_t)src3[j]+(((uint16_t)src2[j]) << 1))+2) >> 2;
		src2+=src_pitch;
		src3+=src_pitch;
		dst+=dst_pitch;
	}

	indice_buffer=(indice_buffer+1)%2;
	src1=(uint8_t *)buffer[indice_buffer];
	for (int32_t j=0; j<w; j++)
		dst[j]=((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1;

}



void JPSDR_Deinterlace::Deinterlace_Blend_MT_1(uint8_t thread_num)
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

	if (SSE2_Enable)
		JPSDR_Deinterlace_Blend_SSE_3_A(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else JPSDR_Deinterlace_Blend_Non_MMX_24(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
		mt_data_inf.src_Y_w_32,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);

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



void JPSDR_Deinterlace::Deinterlace_Blend_MT_2(uint8_t thread_num)
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

	if (SSE2_Enable)
		JPSDR_Deinterlace_Blend_SSE_3_A(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else JPSDR_Deinterlace_Blend_Non_MMX_32(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
		mt_data_inf.src_Y_w_32,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);

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


void JPSDR_Deinterlace::Deinterlace_Blend_MT_3(uint8_t thread_num)
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

	if (SSE2_Enable)
		JPSDR_Deinterlace_Blend_SSE_3_A(mt_data_inf.src1_0,mt_data_inf.src1_1,
			mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
	else JPSDR_Deinterlace_Blend_Non_MMX_8(mt_data_inf.src1_0,mt_data_inf.src1_1,mt_data_inf.dst1,
		mt_data_inf.src_Y_w,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);

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



void JPSDR_Deinterlace::Deinterlace_Blend_MT_4(uint8_t thread_num)
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

	if (SSE2_Enable)
		JPSDR_Deinterlace_Blend_SSE_3_A(mt_data_inf.src2_0,mt_data_inf.src2_1,
			mt_data_inf.dst2,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
	else JPSDR_Deinterlace_Blend_Non_MMX_8(mt_data_inf.src2_0,mt_data_inf.src2_1,mt_data_inf.dst2,
		mt_data_inf.src_UV_w,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);

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



void JPSDR_Deinterlace::Deinterlace_Blend_MT_5(uint8_t thread_num)
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

	if (SSE2_Enable)
		JPSDR_Deinterlace_Blend_SSE_3_A(mt_data_inf.src3_0,mt_data_inf.src3_1,
			mt_data_inf.dst3,(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
	else JPSDR_Deinterlace_Blend_Non_MMX_8(mt_data_inf.src3_0,mt_data_inf.src3_1,mt_data_inf.dst3,
		mt_data_inf.src_UV_w,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);

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



void JPSDR_Deinterlace::Deinterlace_Tri_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	if (SSE2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b(mt_data_inf.src1_0,mt_data_inf.dst1,
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



void JPSDR_Deinterlace::Deinterlace_Tri_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	if (SSE2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b(mt_data_inf.src1_0,mt_data_inf.dst1,
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



void JPSDR_Deinterlace::Deinterlace_Tri_MT_3(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	if (SSE2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a(mt_data_inf.src1_0,mt_data_inf.dst1,
				(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c(mt_data_inf.src1_0,mt_data_inf.dst1,
					(mt_data_inf.src_Y_w_32+3)>>2,h,mt_data_inf.src_pitch1,mt_data_inf.dst_pitch1);
			else
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b(mt_data_inf.src1_0,mt_data_inf.dst1,
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




void JPSDR_Deinterlace::Deinterlace_Tri_MT_4(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	if (SSE2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a(mt_data_inf.src2_0,mt_data_inf.dst2,
				(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c(mt_data_inf.src2_0,mt_data_inf.dst2,
					(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch2,mt_data_inf.dst_pitch2);
			else
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b(mt_data_inf.src2_0,mt_data_inf.dst2,
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



void JPSDR_Deinterlace::Deinterlace_Tri_MT_5(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	int32_t h=mt_data_inf.src_UV_h_max-mt_data_inf.src_UV_h_min;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	if (SSE2_Enable)
	{
		if (mt_data_inf.top)
			JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a(mt_data_inf.src3_0,mt_data_inf.dst3,
				(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
		else
		{
			if (mt_data_inf.bottom)
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c(mt_data_inf.src3_0,mt_data_inf.dst3,
					(mt_data_inf.src_UV_w_32+3)>>2,h,mt_data_inf.src_pitch3,mt_data_inf.dst_pitch3);
			else
				JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b(mt_data_inf.src3_0,mt_data_inf.dst3,
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




void JPSDR_Deinterlace::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_Deinterlace *ptrClass=(JPSDR_Deinterlace *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->Deinterlace_Blend_MT_1(data->thread_Id); break;
		case 2 : ptrClass->Deinterlace_Blend_MT_2(data->thread_Id); break;
		case 3 : ptrClass->Deinterlace_Blend_MT_3(data->thread_Id); break;
		case 7 : ptrClass->Deinterlace_Blend_MT_4(data->thread_Id); break;
		case 8 : ptrClass->Deinterlace_Blend_MT_5(data->thread_Id); break;
		case 4 : ptrClass->Deinterlace_Tri_MT_1(data->thread_Id); break;
		case 5 : ptrClass->Deinterlace_Tri_MT_2(data->thread_Id); break;
		case 6 : ptrClass->Deinterlace_Tri_MT_3(data->thread_Id); break;
		case 9 : ptrClass->Deinterlace_Tri_MT_4(data->thread_Id); break;
		case 10 : ptrClass->Deinterlace_Tri_MT_5(data->thread_Id); break;
		default : ;
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

	swap_buffer=(!mData.file_mode) && ((mData.mode==2) ||
		(mData.mt_mode && (mData.mode==1)));
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
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderRGB(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderRGB(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
				case 2 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderYUYV(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderYUYV(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
				case 3 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderUYVY(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderUYVY(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					Move_Full(idata.src_plane1,tab_Yadif[write_index_Yadif].data[1],idata.src_line1,
						idata.src_h1,idata.src_pitch1,Yadif_pitch[1]);
					Move_Full(idata.src_plane2,tab_Yadif[write_index_Yadif].data[2],idata.src_line2,
						idata.src_h2,idata.src_pitch2,Yadif_pitch[2]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[1],Yadif_pitch[1],idata.src_w1,idata.src_h1);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[2],Yadif_pitch[2],idata.src_w2,idata.src_h2);
					write_index_Yadif++;
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					Move_Full(idata.src_plane1,tab_Yadif[write_index_Yadif].data[1],idata.src_line1,
						idata.src_h1,idata.src_pitch1,Yadif_pitch[1]);
					Move_Full(idata.src_plane2,tab_Yadif[write_index_Yadif].data[2],idata.src_line2,
						idata.src_h2,idata.src_pitch2,Yadif_pitch[2]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[1],Yadif_pitch[1],idata.src_w1,idata.src_h1);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[2],Yadif_pitch[2],idata.src_w2,idata.src_h2);
					write_index_Yadif++;
					break;
				case 9 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					write_index_Yadif++;
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
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
					case 2 :
					case 3 :
					case 9 :
						src1_0=(void *)((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *)((uint8_t *)src2_0+Yadif_pitch[0]);
					case 0 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
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
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[0].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[0].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
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
					case 2 :
					case 3 :
					case 9 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[0].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[0].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[0].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
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
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderRGB(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
				case 2 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderYUYV(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
				case 3 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					AddBorderUYVY(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
					Move_Full(idata.src_plane1,tab_Yadif[write_index_Yadif].data[1],idata.src_line1,
						idata.src_h1,idata.src_pitch1,Yadif_pitch[1]);
					Move_Full(idata.src_plane2,tab_Yadif[write_index_Yadif].data[2],idata.src_line2,
						idata.src_h2,idata.src_pitch2,Yadif_pitch[2]);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[0],Yadif_pitch[0],idata.src_w0,idata.src_h0);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[1],Yadif_pitch[1],idata.src_w1,idata.src_h1);
					AddBorderPlanar(tab_Yadif[write_index_Yadif].data[2],Yadif_pitch[2],idata.src_w2,idata.src_h2);
					break;
				case 9 :
					Move_Full(idata.src_plane0,tab_Yadif[write_index_Yadif].data[0],idata.src_line0,
						idata.src_h0,idata.src_pitch0,Yadif_pitch[0]);
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
					case 2 :
					case 3 :
					case 9 :
						src1_0=(void *)((uint8_t *)src1_0+Yadif_pitch[0]);
						src2_0=(void *)((uint8_t *)src2_0+Yadif_pitch[0]);
					case 0 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
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
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[current_frame_Yadif].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[current_frame_Yadif].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
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
					case 2 :
					case 3 :
					case 9 :
						Move_Half(src1_0,dst0,idata.src_line0,idata.src_h0>>1,Yadif_pitch[0],idata.dst_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						Move_Half(src1_0,dst0,idata.src_line0,idata.src_h0>>1,Yadif_pitch[0],idata.dst_pitch0);
						Move_Half(src1_1,dst1,idata.src_line1,idata.src_h1>>1,Yadif_pitch[1],idata.dst_pitch1);
						Move_Half(src1_2,dst2,idata.src_line2,idata.src_h2>>1,Yadif_pitch[2],idata.dst_pitch2);
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
					case 2 :
					case 3 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAbsDiffAvg(src1_1,src2_1,abs1[1],avg1[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[1],avg2[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[1],avg3[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAbsDiffAvg(src1_2,src2_2,abs1[2],avg1[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[2],avg2[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[2],avg3[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_line0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_line0,idata.dst_h0>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[1],spatial_Yadif_buffer,
							Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1]*10,Yadif_modulo[1],idata.src_line1,idata.dst_h1>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst1,Yadif_pitch[1]*10,Yadif_modulo[1],idata.dst_pitch1,
							idata.dst_modulo1,idata.src_line1,idata.dst_h1>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[2],spatial_Yadif_buffer,
							Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2]*10,Yadif_modulo[2],idata.src_line2,idata.dst_h2>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst2,Yadif_pitch[2]*10,Yadif_modulo[2],idata.dst_pitch2,
							idata.dst_modulo2,idata.src_line2,idata.dst_h2>>1);
						break;
					case 9 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepBottom(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifSpatiafBufferPlanarKeepBottom(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_line0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepBottom(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_line0,idata.dst_h0>>1);
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
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_line0,idata.src_h0>>1);
						YadifTemporalPlanarKeepBottom(src1_1,avg1[1],avg2[1],avg3[1],abs1[1],dst1,Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.dst_pitch1,idata.dst_modulo1,idata.src_line1,idata.src_h1>>1);
						YadifTemporalPlanarKeepBottom(src1_2,avg1[2],avg2[2],avg3[2],abs1[2],dst2,Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.dst_pitch2,idata.dst_modulo2,idata.src_line2,idata.src_h2>>1);
						break;
					case 9 :
						YadifTemporalPlanarKeepBottom(src1_0,avg1[0],avg2[0],avg3[0],abs1[0],dst0,Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_line0,idata.src_h0>>1);
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
					case 2 :
					case 3 :
					case 9 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiff(src1_0,src2_0,tab_Yadif[current_frame_Yadif].absdiff[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAbsDiff(src1_1,src2_1,tab_Yadif[current_frame_Yadif].absdiff[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAbsDiff(src1_2,src2_2,tab_Yadif[current_frame_Yadif].absdiff[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
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
					case 2 :
					case 3 :
					case 9 :
						Move_Half(src1_0,dst0,idata.src_line0,idata.src_h0>>1,Yadif_pitch[0],idata.dst_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						Move_Half(src1_0,dst0,idata.src_line0,idata.src_h0>>1,Yadif_pitch[0],idata.dst_pitch0);
						Move_Half(src1_1,dst1,idata.src_line1,idata.src_h1>>1,Yadif_pitch[1],idata.dst_pitch1);
						Move_Half(src1_2,dst2,idata.src_line2,idata.src_h2>>1,Yadif_pitch[2],idata.dst_pitch2);
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
					case 2 :
					case 3 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAbsDiffAvg(src1_1,src2_1,abs1[1],avg1[1],Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1],
							Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[1],avg2[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[1],avg3[1],Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.src_line1,idata.src_h1>>1);
						YadifAbsDiffAvg(src1_2,src2_2,abs1[2],avg1[2],Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2],
							Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[2],avg2[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[2],avg3[2],Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.src_line2,idata.src_h2>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_line0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_line0,idata.dst_h0>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[1],spatial_Yadif_buffer,
							Yadif_pitch[1],Yadif_modulo[1],Yadif_pitch[1]*10,Yadif_modulo[1],idata.src_line1,idata.dst_h1>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst1,Yadif_pitch[1]*10,Yadif_modulo[1],idata.dst_pitch1,
							idata.dst_modulo1,idata.src_line1,idata.dst_h1>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[2],spatial_Yadif_buffer,
							Yadif_pitch[2],Yadif_modulo[2],Yadif_pitch[2]*10,Yadif_modulo[2],idata.src_line2,idata.dst_h2>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst2,Yadif_pitch[2]*10,Yadif_modulo[2],idata.dst_pitch2,
							idata.dst_modulo2,idata.src_line2,idata.dst_h2>>1);
						break;
					case 9 :
						YadifAbsDiffAvg(src1_0,src2_0,abs1[0],avg1[0],Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0],
							Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[previous_frame_Yadif].absdiff[0],avg2[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifAvgKeepTop(tab_Yadif[current_frame_Yadif].absdiff[0],avg3[0],Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.src_line0,idata.src_h0>>1);
						YadifSpatiafBufferPlanarKeepTop(tab_Yadif[current_frame_Yadif].data[0],spatial_Yadif_buffer,
							Yadif_pitch[0],Yadif_modulo[0],Yadif_pitch[0]*10,Yadif_modulo[0],idata.src_line0,idata.dst_h0>>1);
						YadifSpatialPlanarKeepTop(spatial_Yadif_buffer,dst0,Yadif_pitch[0]*10,Yadif_modulo[0],idata.dst_pitch0,
							idata.dst_modulo0,idata.src_line0,idata.dst_h0>>1);
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
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_line0,idata.src_h0>>1);
						YadifTemporalPlanarKeepTop(src1_1,avg1[1],avg2[1],avg3[1],abs1[1],dst1,Yadif_pitch[1],Yadif_modulo[1],
							Yadif_pitch[1],Yadif_modulo[1],idata.dst_pitch1,idata.dst_modulo1,idata.src_line1,idata.src_h1>>1);
						YadifTemporalPlanarKeepTop(src1_2,avg1[2],avg2[2],avg3[2],abs1[2],dst2,Yadif_pitch[2],Yadif_modulo[2],
							Yadif_pitch[2],Yadif_modulo[2],idata.dst_pitch2,idata.dst_modulo2,idata.src_line2,idata.src_h2>>1);
						break;
					case 9 :
						YadifTemporalPlanarKeepTop(src1_0,avg1[0],avg2[0],avg3[0],abs1[0],dst0,Yadif_pitch[0],Yadif_modulo[0],
							Yadif_pitch[0],Yadif_modulo[0],idata.dst_pitch0,idata.dst_modulo0,idata.src_line0,idata.src_h0>>1);
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
			case 2 :
			case 3 :
			case 9 :
				if (swap_buffer)
				{
					Move_Full(idata.src_plane0,idata.dst_plane0,idata.src_line0,idata.src_h0,
						idata.src_pitch0,idata.dst_pitch0);
				}
				if (transfert_buffer) Move_Full(idata.src_plane0,buffer0,idata.src_line0,
					idata.src_h0,idata.src_pitch0,buffer0_pitch);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				if (swap_buffer)
				{
					Move_Full(idata.src_plane0,idata.dst_plane0,idata.src_line0,idata.src_h0,
						idata.src_pitch0,idata.dst_pitch0);
					Move_Full(idata.src_plane1,idata.dst_plane1,idata.src_line1,idata.src_h1,
						idata.src_pitch1,idata.dst_pitch1);
					Move_Full(idata.src_plane2,idata.dst_plane2,idata.src_line2,idata.src_h2,
						idata.src_pitch2,idata.dst_pitch2);
				}
				if (transfert_buffer)
				{
					Move_Full(idata.src_plane0,buffer0,idata.src_line0,idata.src_h0,idata.src_pitch0,buffer0_pitch);
					Move_Full(idata.src_plane1,buffer1,idata.src_line1,idata.src_h1,idata.src_pitch1,buffer1_pitch);
					Move_Full(idata.src_plane2,buffer2,idata.src_line2,idata.src_h2,idata.src_pitch2,buffer2_pitch);
				}
				break;
		}
		return;
	}

	if (threads_number>1)
	{
		for (uint8_t i=0; i<threads_number; i++)
		{
			switch(current_mode)
			{
				case 1 :
					MT_Data[i].src1_0=(void *)((uint8_t *)idata.src_plane0
						+(idata.src_pitch0*MT_Data[i].src_Y_h_min));
					MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+idata.src_pitch0);
					MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0
						+(idata.dst_pitch0*MT_Data[i].dst_Y_h_min));

					MT_Data[i].src2_0=(void *)((uint8_t *)idata.src_plane1
						+(idata.src_pitch1*MT_Data[i].src_UV_h_min));
					MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+idata.src_pitch1);
					MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1
						+(idata.dst_pitch1*MT_Data[i].dst_UV_h_min));

					MT_Data[i].src3_0=(void *)((uint8_t *)idata.src_plane2
						+(idata.src_pitch2*MT_Data[i].src_UV_h_min));
					MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+idata.src_pitch2);
					MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2
						+(idata.dst_pitch2*MT_Data[i].dst_UV_h_min));

					MT_Data[i].src_pitch1=idata.src_pitch0;
					MT_Data[i].src_pitch2=idata.src_pitch1;
					MT_Data[i].src_pitch3=idata.src_pitch2;
					MT_Data[i].dst_pitch1=idata.dst_pitch0;
					MT_Data[i].dst_pitch2=idata.dst_pitch1;
					MT_Data[i].dst_pitch3=idata.dst_pitch2;
					MT_Data[i].div_h=false;
					break;
				case 2 :
					if (MT_Data[i].top)
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)idata.src_plane0
							+(idata.src_pitch0*MT_Data[i].src_Y_h_min));
						MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0
							+(idata.dst_pitch0*MT_Data[i].dst_Y_h_min));

						MT_Data[i].src2_0=(void *)((uint8_t *)idata.src_plane1
							+(idata.src_pitch1*MT_Data[i].src_UV_h_min));
						MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1
							+(idata.dst_pitch1*MT_Data[i].dst_UV_h_min));

						MT_Data[i].src3_0=(void *)((uint8_t *)idata.src_plane2
							+(idata.src_pitch2*MT_Data[i].src_UV_h_min));
						MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2
							+(idata.dst_pitch2*MT_Data[i].dst_UV_h_min));
					}
					else
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)idata.src_plane0
							+(idata.src_pitch0*(MT_Data[i].src_Y_h_min-1)));
						MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0
							+(idata.dst_pitch0*MT_Data[i].dst_Y_h_min));

						MT_Data[i].src2_0=(void *)((uint8_t *)idata.src_plane1
							+(idata.src_pitch1*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1
							+(idata.dst_pitch1*MT_Data[i].dst_UV_h_min));

						MT_Data[i].src3_0=(void *)((uint8_t *)idata.src_plane2
							+(idata.src_pitch2*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2
							+(idata.dst_pitch2*MT_Data[i].dst_UV_h_min));
					}

					MT_Data[i].src_pitch1=idata.src_pitch0;
					MT_Data[i].src_pitch2=idata.src_pitch1;
					MT_Data[i].src_pitch3=idata.src_pitch2;
					MT_Data[i].src_modulo1=idata.src_modulo0;
					MT_Data[i].src_modulo2=idata.src_modulo1;
					MT_Data[i].src_modulo3=idata.src_modulo2;

					MT_Data[i].dst_pitch1=idata.dst_pitch0;
					MT_Data[i].dst_pitch2=idata.dst_pitch1;
					MT_Data[i].dst_pitch3=idata.dst_pitch2;
					MT_Data[i].dst_modulo1=idata.dst_modulo0;
					MT_Data[i].dst_modulo2=idata.dst_modulo1;
					MT_Data[i].dst_modulo3=idata.dst_modulo2;

					MT_Data[i].div_h=false;
					break;
				case 3 :
					if (MT_Data[i].top)
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)idata.src_plane0
							+(idata.src_pitch0*(MT_Data[i].src_Y_h_min+1)));
						MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+2*idata.src_pitch0);
						MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0
							+(idata.dst_pitch0*(MT_Data[i].dst_Y_h_min+2)));

						MT_Data[i].src2_0=(void *)((uint8_t *)idata.src_plane1
							+(idata.src_pitch1*(MT_Data[i].src_UV_h_min+1)));
						MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+2*idata.src_pitch1);
						MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1
							+(idata.dst_pitch1*(MT_Data[i].dst_UV_h_min+2)));

						MT_Data[i].src3_0=(void *)((uint8_t *)idata.src_plane2
							+(idata.src_pitch2*(MT_Data[i].src_UV_h_min+1)));
						MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+2*idata.src_pitch2);
						MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2
							+(idata.dst_pitch2*(MT_Data[i].dst_UV_h_min+2)));

						MT_Data[i].dec_h=true;
					}
					else
					{
						MT_Data[i].src1_0=(void *)((uint8_t *)idata.src_plane0
							+(idata.src_pitch0*(MT_Data[i].src_Y_h_min-1)));
						MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+2*idata.src_pitch0);
						MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0
							+(idata.dst_pitch0*MT_Data[i].dst_Y_h_min));

						MT_Data[i].src2_0=(void *)((uint8_t *)idata.src_plane1
							+(idata.src_pitch1*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+2*idata.src_pitch1);
						MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1
							+(idata.dst_pitch1*MT_Data[i].dst_UV_h_min));

						MT_Data[i].src3_0=(void *)((uint8_t *)idata.src_plane2
							+(idata.src_pitch2*(MT_Data[i].src_UV_h_min-1)));
						MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+2*idata.src_pitch2);
						MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2
							+(idata.dst_pitch2*MT_Data[i].dst_UV_h_min));

						MT_Data[i].dec_h=false;
					}

					MT_Data[i].src_pitch1=idata.src_pitch0 << 1;
					MT_Data[i].src_pitch2=idata.src_pitch1 << 1;
					MT_Data[i].src_pitch3=idata.src_pitch2 << 1;
					MT_Data[i].dst_pitch1=idata.dst_pitch0 << 1;
					MT_Data[i].dst_pitch2=idata.dst_pitch1 << 1;
					MT_Data[i].dst_pitch3=idata.dst_pitch2 << 1;
					MT_Data[i].div_h=true;
					break;
				case 4 :
					MT_Data[i].src1_0=(void *)((uint8_t *)idata.src_plane0
						+(idata.src_pitch0*MT_Data[i].src_Y_h_min));
					MT_Data[i].src1_1=(void *)((uint8_t *)MT_Data[i].src1_0+2*idata.src_pitch0);
					MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0
						+(idata.dst_pitch0*(MT_Data[i].dst_Y_h_min+1)));

					MT_Data[i].src2_0=(void *)((uint8_t *)idata.src_plane1
						+(idata.src_pitch1*MT_Data[i].src_UV_h_min));
					MT_Data[i].src2_1=(void *)((uint8_t *)MT_Data[i].src2_0+2*idata.src_pitch1);
					MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1
						+(idata.dst_pitch1*(MT_Data[i].dst_UV_h_min+1)));

					MT_Data[i].src3_0=(void *)((uint8_t *)idata.src_plane2
						+(idata.src_pitch2*MT_Data[i].src_UV_h_min));
					MT_Data[i].src3_1=(void *)((uint8_t *)MT_Data[i].src3_0+2*idata.src_pitch2);
					MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2
						+(idata.dst_pitch2*(MT_Data[i].dst_UV_h_min+1)));

					if (MT_Data[i].bottom) MT_Data[i].dec_h=true;
					else MT_Data[i].dec_h=false;

					MT_Data[i].src_pitch1=idata.src_pitch0 << 1;
					MT_Data[i].src_pitch2=idata.src_pitch1 << 1;
					MT_Data[i].src_pitch3=idata.src_pitch2 << 1;
					MT_Data[i].dst_pitch1=idata.dst_pitch0 << 1;
					MT_Data[i].dst_pitch2=idata.dst_pitch1 << 1;
					MT_Data[i].dst_pitch3=idata.dst_pitch2 << 1;
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
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						f_proc=1;
						break;
					case 2 :
					case 3 :
						f_proc=2;
						break;
					case 9 :
						f_proc=3;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						f_proc=7;
						break;
					default : ;
				}
				break;
			case 2 :
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
						f_proc=4;
						break;
					case 2 :
					case 3 :
						f_proc=5;
						break;
					case 9 :
						f_proc=6;
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						f_proc=8;
						break;
					default : ;
				}
				break;
			default : ;
		}

		if (f_proc!=0)
		{
			if (poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false))
			{
				if (f_proc<7)
				{
					for(uint8_t i=0; i<threads_number; i++)
						MT_Thread[i].f_process=f_proc;

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
								MT_Thread[i].f_process=3;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=7;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=8;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=0;
							break;
						case 8 :
							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=6;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=9;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=10;

							if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

							for(uint8_t i=0; i<threads_number; i++)
								MT_Thread[i].f_process=0;
							break;
						default : break;
					}
				}
				poolInterface->ReleaseThreadPool(UserId,true);
			}
		}
	}
	else
	{
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
	
		switch(current_mode)
		{
			case 0 :
				switch(idata.video_mode)
				{
					case 0 :
					case 1 :
					case 2 :
					case 3 :
					case 9 :
						Move_Full(buffer0,idata.dst_plane0,idata.src_line0,idata.src_h0,buffer0_pitch,idata.dst_pitch0);
						break;
					case 4 :
					case 5 :
					case 6 :
					case 7 :
					case 8 :
						Move_Full(buffer0,idata.dst_plane0,idata.src_line0,idata.src_h0,buffer0_pitch,idata.dst_pitch0);
						Move_Full(buffer1,idata.dst_plane1,idata.src_line1,idata.src_h1,buffer1_pitch,idata.dst_pitch1);
						Move_Full(buffer2,idata.dst_plane2,idata.src_line2,idata.src_h2,buffer2_pitch,idata.dst_pitch2);
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
							JPSDR_Deinterlace_Blend_SSE_3_A(src1_0,src2_0,dst0_0,(idata.src_w0+3)>>2,
								h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 2 :
						case 3 :
								JPSDR_Deinterlace_Blend_SSE_3_A(src1_0,src2_0,dst0_0,(idata.src_w0+7)>>3,
									h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 4 :
						case 5 :
						case 6 :
						case 7 :
						case 8 :
							JPSDR_Deinterlace_Blend_SSE_3_A(src1_0,src2_0,dst0_0,(idata.src_w0+15)>>4,
								h0,idata.src_pitch0,idata.dst_pitch0);								
							JPSDR_Deinterlace_Blend_SSE_3_A(src1_1,src2_1,dst0_1,(idata.src_w1+15)>>4,
								h1,idata.src_pitch1,idata.dst_pitch1);
							JPSDR_Deinterlace_Blend_SSE_3_A(src1_2,src2_2,dst0_2,(idata.src_w2+15)>>4,
								h2,idata.src_pitch2,idata.dst_pitch2);
							break;
						case 9 :
							JPSDR_Deinterlace_Blend_SSE_3_A(src1_0,src2_0,dst0_0,(idata.src_w0+15)>>4,
								h0,idata.src_pitch0,idata.dst_pitch0);
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
				switch (current_mode)
				{
					case 1 :
					case 4 :
						switch(idata.video_mode)
						{
							case 0 :
							case 1 :
							case 2 :
							case 3 :
							case 9 :
								memcpy((uint8_t *)dst0_0+(h0*idata.dst_pitch0),
									(uint8_t *)dst0_0+((h0-1)*idata.dst_pitch0),idata.src_line0);
								break;
							case 4 :
							case 5 :
							case 6 :
							case 7 :
							case 8 :
								memcpy((uint8_t *)dst0_0+(h0*idata.dst_pitch0),
									(uint8_t *)dst0_0+((h0-1)*idata.dst_pitch0),idata.src_line0);
								memcpy((uint8_t *)dst0_1+(h1*idata.dst_pitch1),
									(uint8_t *)dst0_1+((h1-1)*idata.dst_pitch1),idata.src_line1);
								memcpy((uint8_t *)dst0_2+(h2*idata.dst_pitch2),
									(uint8_t *)dst0_2+((h2-1)*idata.dst_pitch2),idata.src_line2);
								break;
						}
						break;
					case 3 :
						switch(idata.video_mode)
						{
							case 0 :
							case 1 :
							case 2 :
							case 3 :
							case 9 :
								memcpy(dst0_0,(uint8_t *)dst0_0+idata.dst_pitch0,idata.src_line0);
								break;
							case 4 :
							case 5 :
							case 6 :
							case 7 :
							case 8 :
								memcpy(dst0_0,(uint8_t *)dst0_0+idata.dst_pitch0,idata.src_line0);
								memcpy(dst0_1,(uint8_t *)dst0_1+idata.dst_pitch1,idata.src_line1);
								memcpy(dst0_2,(uint8_t *)dst0_2+idata.dst_pitch2,idata.src_line2);
								break;
						}
						break;
					default : break;
				}
				break;
			case 2 :
				if (SSE2_Enable && swap_buffer)
				{
					switch(idata.video_mode)
					{
						case 0 :
						case 1 :
							JPSDR_Deinterlace_Blend_Tri_SSE_3_A(src1_0,dst0_0,(idata.src_w0+3)>>2,
								h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 2 :
						case 3 :
							JPSDR_Deinterlace_Blend_Tri_SSE_3_A(src1_0,dst0_0,(idata.src_w0+7)>>3,
								h0,idata.src_pitch0,idata.dst_pitch0);
							break;
						case 4 :
						case 5 :
						case 6 :
						case 7 :
						case 8 :
							JPSDR_Deinterlace_Blend_Tri_SSE_3_A(src1_0,dst0_0,(idata.src_w0+15)>>4,
								h0,idata.src_pitch0,idata.dst_pitch0);
							JPSDR_Deinterlace_Blend_Tri_SSE_3_A(src1_1,dst0_1,(idata.src_w1+15)>>4,
								h1,idata.src_pitch1,idata.dst_pitch1);
							JPSDR_Deinterlace_Blend_Tri_SSE_3_A(src1_2,dst0_2,(idata.src_w2+15)>>4,
								h2,idata.src_pitch2,idata.dst_pitch2);
							break;
						case 9 :
							JPSDR_Deinterlace_Blend_Tri_SSE_3_A(src1_0,dst0_0,(idata.src_w0+15)>>4,
								h0,idata.src_pitch0,idata.dst_pitch0);
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
								Blend_Tri_RGB32(src1_0,dst0_0,idata.src_w0,h0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								Blend_Tri_RGB32(src1_0,dst0_0,buffer[0],idata.src_w0,h0,idata.src_pitch0,
									idata.dst_pitch0);
							}
							break;
						case 2 :
						case 3 :
							if (swap_buffer)
							{
								Blend_Tri_YUV32(src1_0,dst0_0,(idata.src_w0+1)>>1,h0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								Blend_Tri_YUV32(src1_0,dst0_0,buffer[0],(idata.src_w0+1)>>1,h0,idata.src_pitch0,
									idata.dst_pitch0);
							}
							break;
						case 4 :
						case 5 :
						case 6 :
						case 7 :
						case 8 :
							if (swap_buffer)
							{
								Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,idata.src_pitch0,idata.dst_pitch0);
								Blend_Tri_Planar8(src1_1,dst0_1,idata.src_w1,h1,idata.src_pitch1,idata.dst_pitch1);
								Blend_Tri_Planar8(src1_2,dst0_2,idata.src_w2,h2,idata.src_pitch2,idata.dst_pitch2);
							}
							else
							{
								Blend_Tri_Planar8(src1_0,dst0_0,buffer[0],idata.src_w0,h0,idata.src_pitch0,
									idata.dst_pitch0);
								Blend_Tri_Planar8(src1_1,dst0_1,buffer[1],idata.src_w1,h1,idata.src_pitch1,
									idata.dst_pitch1);
								Blend_Tri_Planar8(src1_2,dst0_2,buffer[2],idata.src_w2,h2,idata.src_pitch2,
									idata.dst_pitch2);
							}
							break;
						case 9 :
							if (swap_buffer)
							{
								Blend_Tri_Planar8(src1_0,dst0_0,idata.src_w0,h0,idata.src_pitch0,idata.dst_pitch0);
							}
							else
							{
								Blend_Tri_Planar8(src1_0,dst0_0,buffer[0],idata.src_w0,h0,idata.src_pitch0,
									idata.dst_pitch0);
							}
							break;
					}
				}
				break;
		}
	}

	if (transfert_buffer) 
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
			case 9 :
				Move_Full(idata.dst_plane0,buffer0,idata.dst_line0,
					idata.dst_h0,idata.dst_pitch0,buffer0_pitch);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				Move_Full(idata.dst_plane0,buffer0,idata.dst_line0,
					idata.dst_h0,idata.dst_pitch0,buffer0_pitch);
				Move_Full(idata.dst_plane1,buffer1,idata.dst_line1,
					idata.dst_h1,idata.dst_pitch1,buffer1_pitch);
				Move_Full(idata.dst_plane2,buffer2,idata.dst_line2,
					idata.dst_h2,idata.dst_pitch2,buffer2_pitch);
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

	SafePrintf(buf, maxlen, "Config(%d, %d, %d, \"%s\")",mData.mode,mData.file_mode,mData.mt_mode,save);
}


void JPSDR_Deinterlace::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.mode=argv[0].asInt();
	mData.file_mode=!!argv[1].asInt();
	mData.mt_mode=!!argv[2].asInt();
	strcpy_s(mData.filename,1024,*argv[3].asString());
}



		
extern VDXFilterDefinition filterDef_JPSDR_Deinterlace=
VDXVideoFilterDefinition<JPSDR_Deinterlace>("JPSDR","Deinterlace v5.2.3","Deinterlace blending frames. [ASM][MMX][SSE][SSE2] Optimised.");



