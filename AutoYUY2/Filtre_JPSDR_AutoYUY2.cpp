#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "AUTOYUY2_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

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

extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(const void *scr_1,const void *src_2,void *dst,int32_t w);
extern "C" void JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(const void *scr_1,const void *src_2,void *dst,int32_t w);



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



class JPSDR_AutoYUY2 : public VDXVideoFilter
{
public:
	virtual ~JPSDR_AutoYUY2();
	JPSDR_AutoYUY2(){}
	JPSDR_AutoYUY2(const JPSDR_AutoYUY2& a)
	{
		SSE2_Enable = a.SSE2_Enable;
		AVX_Enable=a.AVX_Enable;
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
	uint16_t lookup_Upsc[768];
	bool SSE2_Enable,AVX_Enable;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	uint8_t CreateMTData(uint8_t max_threads,int32_t size_x,int32_t size_y);

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	inline void Convert_Progressive_to_Planar422(uint8_t thread_num);
	void Convert_Progressive_to_Planar422_SSE(uint8_t thread_num);
	void Convert_Progressive_to_Planar422_NonSSE(uint8_t thread_num);

	inline void Convert_Interlaced_to_Planar422(uint8_t thread_num);
	void Convert_Interlaced_to_Planar422_SSE(uint8_t thread_num);
	void Convert_Interlaced_to_Planar422_NonSSE(uint8_t thread_num);

	void Convert_Automatic_to_Planar422(uint8_t thread_num);
	void Convert_Test_to_Planar422(uint8_t thread_num);

	inline void Convert_Progressive_to_YUY2(uint8_t thread_num);
	void Convert_Progressive_to_YUY2_SSE(uint8_t thread_num);
	void Convert_Progressive_to_YUY2_NonSSE(uint8_t thread_num);

	inline void Convert_Interlaced_to_YUY2(uint8_t thread_num);
	void Convert_Interlaced_to_YUY2_SSE(uint8_t thread_num);
	void Convert_Interlaced_to_YUY2_NonSSE(uint8_t thread_num);

	void Convert_Automatic_to_YUY2(uint8_t thread_num);
	void Convert_Test_to_YUY2(uint8_t thread_num);

	inline void Convert_Progressive_to_UYVY(uint8_t thread_num);
	void Convert_Progressive_to_UYVY_SSE(uint8_t thread_num);
	void Convert_Progressive_to_UYVY_NonSSE(uint8_t thread_num);

	inline void Convert_Interlaced_to_UYVY(uint8_t thread_num);
	void Convert_Interlaced_to_UYVY_SSE(uint8_t thread_num);
	void Convert_Interlaced_to_UYVY_NonSSE(uint8_t thread_num);

	void Convert_Automatic_to_UYVY(uint8_t thread_num);
	void Convert_Test_to_UYVY(uint8_t thread_num);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_AutoYUY2Data mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_AutoYUY2)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_AutoYUY2,ScriptConfig,"iiii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_AutoYUY2::Init()
{
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
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

	for (i=0; i<256; i++)
	{
		lookup_Upsc[i]=(uint16_t)(3*i);
		lookup_Upsc[i+256]=(uint16_t)(5*i);
		lookup_Upsc[i+512]=(uint16_t)(7*i);
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

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);

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
	Image_Data idata;
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

	if ((idata.src_h0 & 3)!=0)
	{
		ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
		return;
	}

	if ((idata.src_h0<8) || (idata.src_w0<4))
	{
		ff->Except("Size must be at least 4x8!");
		return;
	}

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if (mData.mt_mode && ((idata.src_h0>=32) && (idata.dst_h0>=32))) threads_number=total_cpu;
	else threads_number=1;

	threads_number=CreateMTData(threads_number,idata.src_w0,idata.src_h0);

	if (mData.convert_mode>1)
	{
		for (j=0; j<threads_number; j++)
		{
			for (i=0; i<Interlaced_Tab_Size; i++)
			{
				interlaced_tab_U[j][i]=(bool*)malloc(idata.src_w1*sizeof(bool));
				interlaced_tab_V[j][i]=(bool*)malloc(idata.src_w2*sizeof(bool));
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




inline void JPSDR_AutoYUY2::Convert_Interlaced_to_Planar422(uint8_t thread_num)
{
	if (SSE2_Enable) Convert_Interlaced_to_Planar422_SSE(thread_num);
	else Convert_Interlaced_to_Planar422_NonSSE(thread_num);
}


inline void JPSDR_AutoYUY2::Convert_Progressive_to_Planar422(uint8_t thread_num)
{
	if (SSE2_Enable) Convert_Progressive_to_Planar422_SSE(thread_num);
	else Convert_Progressive_to_Planar422_NonSSE(thread_num);
}



void JPSDR_AutoYUY2::Convert_Interlaced_to_Planar422_SSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Unn,src_U,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Un,src_Unnn,dst_U,w_U4);
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
		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_U,src_Upp,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Up,src_Un,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Unn,src_U,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Un,src_Unnn,dst_U,w_U4);
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
			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_U,src_Upp,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Up,src_Un,dst_U,w_U4);
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

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vnn,src_V,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Vn,src_Vnnn,dst_V,w_V4);
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
		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_V,src_Vpp,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vp,src_Vn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vnn,src_V,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_Vn,src_Vnnn,dst_V,w_V4);
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
			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b(src_V,src_Vpp,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b(src_Vp,src_Vn,dst_V,w_V4);
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



void JPSDR_AutoYUY2::Convert_Interlaced_to_Planar422_NonSSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const uint16_t *lookup=lookup_Upsc;
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



void JPSDR_AutoYUY2::Convert_Progressive_to_Planar422_SSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Un,dst_U,w_U4);
			dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

			src_U+=src_pitch_U;
			src_Up+=src_pitch_U;
			src_Un+=src_pitch_U;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Up,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Un,dst_U,w_U4);
		dst_U=(void *)((uint8_t *)dst_U+dst_pitch_U);

		src_U+=src_pitch_U;
		src_Up+=src_pitch_U;
		src_Un+=src_pitch_U;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_U,src_Up,dst_U,w_U4);
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

			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vn,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}

	for(int32_t i=h_0; i<h_2; i+=2)
	{
		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vp,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vn,dst_V,w_V4);
		dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

		src_V+=src_pitch_V;
		src_Vp+=src_pitch_V;
		src_Vn+=src_pitch_V;
	}

	if (mt_data_inf.bottom)
	{
		for(int32_t i=h_2; i<h_Y_max; i+=2)
		{
			JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b(src_V,src_Vp,dst_V,w_V4);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);


			memcpy(dst_V,src_V,w_V);
			dst_V=(void *)((uint8_t *)dst_V+dst_pitch_V);

			src_V+=src_pitch_V;
			src_Vp+=src_pitch_V;
			src_Vn+=src_pitch_V;
		}
	}
}



void JPSDR_AutoYUY2::Convert_Progressive_to_Planar422_NonSSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const uint16_t *lookup=lookup_Upsc;
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



inline void JPSDR_AutoYUY2::Convert_Progressive_to_YUY2(uint8_t thread_num)
{
	if (SSE2_Enable) Convert_Progressive_to_YUY2_SSE(thread_num);
	else Convert_Progressive_to_YUY2_NonSSE(thread_num);
}



void JPSDR_AutoYUY2::Convert_Progressive_to_YUY2_SSE(uint8_t thread_num)

{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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


void JPSDR_AutoYUY2::Convert_Progressive_to_YUY2_NonSSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const uint16_t *lookup=lookup_Upsc;
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




inline void JPSDR_AutoYUY2::Convert_Interlaced_to_YUY2(uint8_t thread_num)
{
	if (SSE2_Enable) Convert_Interlaced_to_YUY2_SSE(thread_num);
	else Convert_Interlaced_to_YUY2_NonSSE(thread_num);
}



void JPSDR_AutoYUY2::Convert_Interlaced_to_YUY2_SSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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


void JPSDR_AutoYUY2::Convert_Interlaced_to_YUY2_NonSSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const uint16_t *lookup=lookup_Upsc;
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




inline void JPSDR_AutoYUY2::Convert_Progressive_to_UYVY(uint8_t thread_num)
{
	if (SSE2_Enable) Convert_Progressive_to_UYVY_SSE(thread_num);
	else Convert_Progressive_to_UYVY_NonSSE(thread_num);
}



void JPSDR_AutoYUY2::Convert_Progressive_to_UYVY_SSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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


void JPSDR_AutoYUY2::Convert_Progressive_to_UYVY_NonSSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const uint16_t *lookup=lookup_Upsc;
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



inline void JPSDR_AutoYUY2::Convert_Interlaced_to_UYVY(uint8_t thread_num)
{
	if (SSE2_Enable) Convert_Interlaced_to_UYVY_SSE(thread_num);
	else Convert_Interlaced_to_UYVY_NonSSE(thread_num);
}




void JPSDR_AutoYUY2::Convert_Interlaced_to_UYVY_SSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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


void JPSDR_AutoYUY2::Convert_Interlaced_to_UYVY_NonSSE(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const uint16_t *lookup=lookup_Upsc;
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



void JPSDR_AutoYUY2::Convert_Automatic_to_YUY2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const int16_t threshold=mData.threshold;
	const uint16_t *lookup=lookup_Upsc;
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
				bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
				bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
		bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
		bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
			const bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabu1=interlaced_tab_U[thread_num][index_tab_1];
			const bool *itabv0=interlaced_tab_V[thread_num][index_tab_0],*itabv1=interlaced_tab_V[thread_num][index_tab_1];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabv1=interlaced_tab_V[thread_num][index_tab_1];
			bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabu2=interlaced_tab_U[thread_num][index_tab_2];
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabv0=interlaced_tab_V[thread_num][index_tab_0];
			const bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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



void JPSDR_AutoYUY2::Convert_Test_to_YUY2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const int16_t threshold=mData.threshold;
	const uint16_t *lookup=lookup_Upsc;
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
				bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
				bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
		bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
		bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
			const bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabu1=interlaced_tab_U[thread_num][index_tab_1];
			const bool *itabv0=interlaced_tab_V[thread_num][index_tab_0],*itabv1=interlaced_tab_V[thread_num][index_tab_1];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabv1=interlaced_tab_V[thread_num][index_tab_1];
			bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabu2=interlaced_tab_U[thread_num][index_tab_2];
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabv0=interlaced_tab_V[thread_num][index_tab_0];
			const bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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


void JPSDR_AutoYUY2::Convert_Automatic_to_UYVY(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const int16_t threshold=mData.threshold;
	const uint16_t *lookup=lookup_Upsc;
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
				bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
				bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
		bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
		bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
			const bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabu1=interlaced_tab_U[thread_num][index_tab_1];
			const bool *itabv0=interlaced_tab_V[thread_num][index_tab_0],*itabv1=interlaced_tab_V[thread_num][index_tab_1];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabv1=interlaced_tab_V[thread_num][index_tab_1];
			bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabu2=interlaced_tab_U[thread_num][index_tab_2];
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabv0=interlaced_tab_V[thread_num][index_tab_0];
			const bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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


void JPSDR_AutoYUY2::Convert_Test_to_UYVY(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

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
	const int16_t threshold=mData.threshold;
	const uint16_t *lookup=lookup_Upsc;
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
				bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
				bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
		bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];
		bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

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
			const bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabu1=interlaced_tab_U[thread_num][index_tab_1];
			const bool *itabv0=interlaced_tab_V[thread_num][index_tab_0],*itabv1=interlaced_tab_V[thread_num][index_tab_1];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabv1=interlaced_tab_V[thread_num][index_tab_1];
			bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabu2=interlaced_tab_U[thread_num][index_tab_2];
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabv0=interlaced_tab_V[thread_num][index_tab_0];
			const bool *itabu2=interlaced_tab_U[thread_num][index_tab_2],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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




void JPSDR_AutoYUY2::Convert_Automatic_to_Planar422(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *_dst_U=mt_data_inf.dst2;
	void *_dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_UV=mt_data_inf.src_UV_w;
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
	uint8_t *dst_U,*dst_V;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int16_t threshold=mData.threshold;
	const uint16_t *lookup=lookup_Upsc;
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
			memcpy(dst_U,src_U,w_UV);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			{
				bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];

				for(int32_t j=0; j<w_UV; j++)
				{
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
		bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];

		for(int32_t j=0; j<w_UV; j++)
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
		}

	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabu1=interlaced_tab_U[thread_num][index_tab_1];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1];
			bool *itabu2=interlaced_tab_U[thread_num][index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabu2=interlaced_tab_U[thread_num][index_tab_2];

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
			const bool *itabu2=interlaced_tab_U[thread_num][index_tab_2];
			bool *itabu0=interlaced_tab_U[thread_num][index_tab_0];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
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
				bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

				for(int32_t j=0; j<w_UV; j++)
				{
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
		bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

		for(int32_t j=0; j<w_UV; j++)
		{
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

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabv0=interlaced_tab_V[thread_num][index_tab_0],*itabv1=interlaced_tab_V[thread_num][index_tab_1];

			// Upsample as needed.
			for(int32_t j=0; j<w_UV; j++)
			{
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
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1];
			bool *itabv2=interlaced_tab_V[thread_num][index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
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
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			const bool *itabv2=interlaced_tab_V[thread_num][index_tab_2];
			bool *itabv0=interlaced_tab_V[thread_num][index_tab_0];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
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



void JPSDR_AutoYUY2::Convert_Test_to_Planar422(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src_Y=mt_data_inf.src1;
	const void *_src_U=mt_data_inf.src2;
	const void *_src_V=mt_data_inf.src3;
	void *dst_Y=mt_data_inf.dst1;
	void *_dst_U=mt_data_inf.dst2;
	void *_dst_V=mt_data_inf.dst3;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t w_UV=mt_data_inf.src_UV_w;
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
	uint8_t *dst_U,*dst_V;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const int16_t threshold=mData.threshold;
	const uint16_t *lookup=lookup_Upsc;
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
			memcpy(dst_U,src_U,w_UV);
			dst_U+=dst_pitch_U;

			memcpy(dst_U,src_Un,w_UV);
			dst_U+=dst_pitch_U;

			for(int32_t j=0; j<w_UV; j++)
				dst_U[j]=(uint8_t)((lookup[src_Unn[j]]+lookup[(uint16_t)src_U[j]+256]+4)>>3);
			dst_U+=dst_pitch_U;

			{
				bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];

				for(int32_t j=0; j<w_UV; j++)
				{
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
		bool *itabu0=interlaced_tab_U[thread_num][0],*itabu1=interlaced_tab_U[thread_num][1];

		for(int32_t j=0; j<w_UV; j++)
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
		}

	}

	index_tab_0=0;
	index_tab_1=1;
	index_tab_2=2;

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabu0=interlaced_tab_U[thread_num][index_tab_0],*itabu1=interlaced_tab_U[thread_num][index_tab_1];

			for(int32_t j=0; j<w_UV; j++)
			{
				// Upsample as needed.
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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1];
			bool *itabu2=interlaced_tab_U[thread_num][index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
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
			const bool *itabu1=interlaced_tab_U[thread_num][index_tab_1],*itabu2=interlaced_tab_U[thread_num][index_tab_2];

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
			const bool *itabu2=interlaced_tab_U[thread_num][index_tab_2];
			bool *itabu0=interlaced_tab_U[thread_num][index_tab_0];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
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
				bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

				for(int32_t j=0; j<w_UV; j++)
				{
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
		bool *itabv0=interlaced_tab_V[thread_num][0],*itabv1=interlaced_tab_V[thread_num][1];

		for(int32_t j=0; j<w_UV; j++)
		{
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

	for(int32_t i=h_0; i<h_4; i+=4)
	{
		{
			const bool *itabv0=interlaced_tab_V[thread_num][index_tab_0],*itabv1=interlaced_tab_V[thread_num][index_tab_1];

			// Upsample as needed.
			for(int32_t j=0; j<w_UV; j++)
			{
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
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1];
			bool *itabv2=interlaced_tab_V[thread_num][index_tab_2];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
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
			const bool *itabv1=interlaced_tab_V[thread_num][index_tab_1],*itabv2=interlaced_tab_V[thread_num][index_tab_2];

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
			const bool *itabv2=interlaced_tab_V[thread_num][index_tab_2];
			bool *itabv0=interlaced_tab_V[thread_num][index_tab_0];

			for(int32_t j=0; j<w_UV; j++)
			{
				if (((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
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




void JPSDR_AutoYUY2::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_AutoYUY2 *ptrClass=(JPSDR_AutoYUY2 *)data->pClass;
	
	switch(data->f_process)
	{
		case 1 : ptrClass->Convert_Automatic_to_YUY2(data->thread_Id);
			break;
		case 2 : ptrClass->Convert_Test_to_YUY2(data->thread_Id);
			break;
		case 3 : ptrClass->Convert_Automatic_to_UYVY(data->thread_Id);
			break;
		case 4 : ptrClass->Convert_Test_to_UYVY(data->thread_Id);
			break;
		case 5 : ptrClass->Convert_Automatic_to_Planar422(data->thread_Id);
			break;
		case 6 : ptrClass->Convert_Test_to_Planar422(data->thread_Id);
			break;
		case 7 : ptrClass->Convert_Progressive_to_Planar422(data->thread_Id);
			break;
		case 8 : ptrClass->Convert_Interlaced_to_Planar422(data->thread_Id);
			break;
		case 9 : ptrClass->Convert_Progressive_to_YUY2(data->thread_Id);
			break;
		case 10 : ptrClass->Convert_Interlaced_to_YUY2(data->thread_Id);
			break;
		case 11 : ptrClass->Convert_Progressive_to_UYVY(data->thread_Id);
			break;
		case 12 : ptrClass->Convert_Interlaced_to_UYVY(data->thread_Id);
			break;
		default : ;
	}
}


void JPSDR_AutoYUY2::Run()
{
	Image_Data idata;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;

	const VDXPixmap& pxdst=*fa->dst.mpPixmap;
	const VDXPixmap& pxsrc=*fa->src.mpPixmap;
	uint8_t f_proc=0;

	idata.src_plane0=pxsrc.data;
	idata.src_plane1=pxsrc.data2;
	idata.src_plane2=pxsrc.data3;
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	for(uint8_t i=0; i<threads_number; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)idata.src_plane0+(MT_Data[i].src_Y_h_min*idata.src_pitch0));
		MT_Data[i].src2=(void *)((uint8_t *)idata.src_plane1+(MT_Data[i].src_UV_h_min*idata.src_pitch1));
		MT_Data[i].src3=(void *)((uint8_t *)idata.src_plane2+(MT_Data[i].src_UV_h_min*idata.src_pitch2));
		MT_Data[i].src_pitch1=idata.src_pitch0;
		MT_Data[i].src_pitch2=idata.src_pitch1;
		MT_Data[i].src_pitch3=idata.src_pitch2;
		MT_Data[i].dst1=(void *)((uint8_t *)idata.dst_plane0+(MT_Data[i].dst_Y_h_min*idata.dst_pitch0));
		MT_Data[i].dst2=(void *)((uint8_t *)idata.dst_plane1+(MT_Data[i].dst_UV_h_min*idata.dst_pitch1));
		MT_Data[i].dst3=(void *)((uint8_t *)idata.dst_plane2+(MT_Data[i].dst_UV_h_min*idata.dst_pitch2));
		MT_Data[i].dst_pitch1=idata.dst_pitch0;
		MT_Data[i].dst_pitch2=idata.dst_pitch1;
		MT_Data[i].dst_pitch3=idata.dst_pitch2;
	}

	switch(idata.dst_video_mode)
	{
		case 2 :
			switch(mData.convert_mode)
			{
				case 0 : f_proc=9; break;
				case 1 : f_proc=10; break;
				case 2 : f_proc=1; break;
				case 3 : f_proc=2; break;
				default : ;
			}
			break;
		case 3 :
			switch(mData.convert_mode)
			{
				case 0 : f_proc=11; break;
				case 1 : f_proc=12; break;
				case 2 : f_proc=3; break;
				case 3 : f_proc=4; break;
				default : ;
			}
			break;
		case 5 :
			switch(mData.convert_mode)
			{
				case 0 : f_proc=7; break;
				case 1 : f_proc=8; break;
				case 2 : f_proc=5; break;
				case 3 : f_proc=6; break;
				default : ;
			}
			break;
		default : ;
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

			poolInterface->ReleaseThreadPool(UserId,true);
		}
	}
	else
	{
		switch(f_proc)
		{
			case 1 : Convert_Automatic_to_YUY2(0); break;
			case 2 : Convert_Test_to_YUY2(0); break;
			case 3 : Convert_Automatic_to_UYVY(0); break;
			case 4 : Convert_Test_to_UYVY(0); break;
			case 5 : Convert_Automatic_to_Planar422(0); break;
			case 6 : Convert_Test_to_Planar422(0); break;
			case 7 : Convert_Progressive_to_Planar422(0); break;
			case 8 : Convert_Interlaced_to_Planar422(0); break;
			case 9 : Convert_Progressive_to_YUY2(0); break;
			case 10 : Convert_Interlaced_to_YUY2(0); break;
			case 11 : Convert_Progressive_to_UYVY(0); break;
			case 12 : Convert_Interlaced_to_UYVY(0); break;
			default : ;
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



extern VDXFilterDefinition filterDef_JPSDR_AutoYUY2=
VDXVideoFilterDefinition<JPSDR_AutoYUY2>("JPSDR","AutoYUY2 v3.2.5","Convert Planar4:2:0 to severals 4:2:2 modes. [SSE2] Optimised.");

