#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"
#include "..\Filtres_JPSDR\Pascal.h"

extern int g_VFVAPIVersion;

extern "C" uint32_t JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32(const void *src,
		uint8_t *map,void *dst,uint32_t *histo,uint32_t *repart,int32_t w,
		int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,
		uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32(const void *src,uint8_t *map,
		int32_t w,int32_t h,int32_t w_map,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_Norme1_SSE_1_RGB32(const void *src1,const void *src2,int32_t w,
		int32_t h,ptrdiff_t pitch);
extern "C" uint32_t JPSDR_IVTC_Norme1_SSE_2_RGB32(const void *src1,const void *src2,int32_t w,
		int32_t h,ptrdiff_t pitch);
extern "C" void JPSDR_IVTC_Motion_Map_SSE_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		uint32_t thr,int32_t w_map);

extern "C" void JPSDR_IVTC_Rebuild_Frame(const void *bottom_src,const void *top_src,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,ptrdiff_t src_modulo,
		ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Rebuild_Frame_2(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Move32_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Move32(const void *src, void *dst, uint32_t size);
extern "C" void JPSDR_IVTC_Move32_Full_src(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo);
extern "C" void JPSDR_IVTC_Move32_Full_dst(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t dst_modulo);

extern "C" void JPSDR_IVTC_Rebuild_Frame8(const void *bottom_src,const void *top_src,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,ptrdiff_t src_modulo,
		ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Rebuild_Frame8_2(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Move8_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Move8(const void *src, void *dst, uint32_t size);
extern "C" void JPSDR_IVTC_Move8_Full_src(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo);
extern "C" void JPSDR_IVTC_Move8_Full_dst(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t dst_modulo);

extern "C" void JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_2(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_3(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2(const void *src,void *dst,void *lookup,
		uint32_t size);
extern "C" void JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2(const void *src,void *dst,void *lookup,
		uint32_t size);
extern "C" void JPSDR_IVTC_Convert420_to_YUY2_1(const uint8_t *scr_y,const uint8_t *src_u,
		const uint8_t *src_v,void *dst,int32_t w);

extern "C" void JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2(const void *src,void *dst,void *lookup,
		int32_t w, int32_t h, ptrdiff_t src_modulo);
extern "C" void JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2(const void *src,void *dst,void *lookup,
		int32_t w, int32_t h, ptrdiff_t src_modulo);
		
		

#define Buffer_Size_Manual 10
#define Max_Sequence 2048
#define Data_Buffer_Size 10
#define Error_Fields_Size 2
#define Read_Index_Size 10
#define Error_Index_Size 2
#define Write_Index_Size 2
#define Flag_Index_Size 10
#define Filter_Lag 7
#define DTopBot 30000
// v5.0.2 : Pb avec DVDs Jap Hack sign : Valeurs v4 sur scènes statiques parfois de l'ordre de 290000
#define Static2 350000
#define Static 65000  //80000
#define ChgSc_1 10000000
#define ChgSc_2 18000000
//#define ChgSc_1 25000000
//#define ChgSc_2 50000000
//#define ChgSc_1 12000000
//#define ChgSc_2 25000000
#define IVTC_MIN_1 45000
#define IVTC_MIN_2 3000
#define IVTC_MIN_3 1000
#define IVTC_MIN_4 25000
#define IVTC_MIN_5 20000
#define IVTC_MIN_6 200000
#define IVTC_MIN_7 60000
#define IVTC_MIN_8 700000
#define IVTC 300000
#define Deinterlace_Min 50

#define Interlaced_Tab_Size 3
#define Threshold_Convert 4


typedef struct _IVTC_Data_Flags
{
	bool ivtc,ivtc_map,pic,error,interlace,rev,first,last,chg_sc,chg_sc_p1;
	bool ivtc_valid,Z,dtopbot,p1_f,m1_f;
	bool interlace_p1,interlace_m1;
} IVTC_Data_Flags;


typedef struct _IVTC_Data_Buffer
{
	void *frameRGB32,*_frameRGB32;
	void *frameRGB32Resize,*_frameRGB32Resize;
	uint32_t error,error_IVTC_M1,error_IVTC_P1;
	uint32_t error_Motion_Map,error_IVTC_M1_Motion_Map,error_IVTC_P1_Motion_Map;
	uint32_t Delta_Top_P1,Delta_Bottom_P1,Delta_Top_M1,Delta_Bottom_M1;
	uint32_t Delta_error_P1,Delta_error_M1;
	IVTC_Data_Flags flags;
	uint32_t repartition[256];
} IVTC_Data_Buffer;


typedef struct _JPSDR_IVTC_Manual
{
	void *bufferRGB32[Buffer_Size_Manual];
	uint16_t data_out[5];
	uint8_t write_index,read_index,ivtc_index;
	uint32_t debut[Max_Sequence],fin[Max_Sequence];
	uint8_t ivtc_offset[Max_Sequence];
	bool modif_polar[Max_Sequence];
	uint16_t nbre_sequence,last_sequence;
	uint8_t last_offset,first_offset;
	bool sec_prec_ok;
} JPSDR_IVTC_Manual;

typedef struct _IVTC_Flags
{
	bool ivtc,ivtc_rev,chg_sequence,chg_sequence_p1,inv_polar;
} IVTC_Flags;


class JPSDR_IVTCData
{
public:
	bool output_filter,inv_polar,first_last_mode,disable_deinterlace,blend_anime,tri_linear,Y_only;
	bool disable_Z_mode,disable_motion_map_filtering,manual_mode;
	uint8_t output_mode,threshold_ivtc,threshold_filter,threshold_deinterlace,line_removed,
		deinterlace_mode;
	char filename[1024];

	JPSDR_IVTCData(void);
};


JPSDR_IVTCData::JPSDR_IVTCData(void)
{
	output_mode=1;
	first_last_mode=false;
	disable_deinterlace=true;
	manual_mode=false;
	disable_motion_map_filtering=false;
	disable_Z_mode=false;
	output_filter=false;
	Y_only=true;
	inv_polar=false;
	blend_anime=false;
	tri_linear=false;
	line_removed=0;
	strcpy_s(filename,1024,"");
	deinterlace_mode=1;
	threshold_ivtc=15;
	threshold_filter=4;
	threshold_deinterlace=25;
}


class JPSDR_IVTCDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_IVTCDialog(JPSDR_IVTCData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_IVTC),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_IVTCData& mData;
	JPSDR_IVTCData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_IVTCDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_IVTCDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_MANUAL_MODE,mData.manual_mode?BST_CHECKED:BST_UNCHECKED);	
	CheckDlgButton(mhdlg,IDC_OUTPUT_FILTER,mData.output_filter?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_Y_ONLY,mData.Y_only?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_INV_POLAR,mData.inv_polar?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_FIRST_LAST,mData.first_last_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_DEINTERLACE,mData.disable_deinterlace?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_BLEND,mData.blend_anime?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_Z_MODE,mData.disable_Z_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_MOTION_MAP,
		mData.disable_motion_map_filtering?BST_CHECKED:BST_UNCHECKED);
	switch (mData.output_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_PROGRESSIVE,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_TELECINE,1); break;
	}
	switch (mData.deinterlace_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_SMART,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_STANDARD,1); break;
	}
	if (mData.tri_linear) CheckDlgButton(mhdlg,IDC_TRILINEAR,1);
	else CheckDlgButton(mhdlg,IDC_BILINEAR,1);
	SetDlgItemInt(mhdlg,IDC_THRESHOLD_IVTC,mData.threshold_ivtc,FALSE);
	SetDlgItemInt(mhdlg,IDC_THRESHOLD_FILTER,mData.threshold_filter,FALSE);
	SetDlgItemInt(mhdlg,IDC_THRESHOLD_DEINTERLACE,mData.threshold_deinterlace,FALSE);
	SetDlgItemInt(mhdlg,IDC_LINE_REMOVED,mData.line_removed,FALSE);
	EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),mData.manual_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_FILENAME),mData.manual_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_Y_ONLY),mData.output_filter);
	if (mData.manual_mode)
	{
		SetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename);
	}	

/*	if (mData.output_filter)
	{
		EnableWindow(GetDlgItem(hdlg,IDC_OUTPUT_TEXT),TRUE);
		EnableWindow(GetDlgItem(hdlg,IDC_THRESHOLD_FILTER),TRUE);
		SetDlgItemInt(hdlg,IDC_THRESHOLD_FILTER,mData.threshold_filter,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hdlg,IDC_OUTPUT_TEXT),FALSE);
		EnableWindow(GetDlgItem(hdlg,IDC_THRESHOLD_FILTER),FALSE);
	}*/

	return false;
}


void JPSDR_IVTCDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}

bool JPSDR_IVTCDialog::SaveToData()
{
	uint8_t thrs;
	BOOL success;

	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD_IVTC,&success,FALSE);
	if (success) mData.threshold_ivtc=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD_FILTER,&success,FALSE);
	if (success) mData.threshold_filter=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD_DEINTERLACE,&success,FALSE);
	if (success) mData.threshold_deinterlace=thrs;
	thrs=GetDlgItemInt(mhdlg,IDC_LINE_REMOVED,&success,FALSE);
	if (success) mData.line_removed=thrs;
	mData.output_filter=!!IsDlgButtonChecked(mhdlg, IDC_OUTPUT_FILTER);
	mData.Y_only=!!IsDlgButtonChecked(mhdlg, IDC_Y_ONLY);
	mData.inv_polar=!!IsDlgButtonChecked(mhdlg, IDC_INV_POLAR);
	mData.first_last_mode=!!IsDlgButtonChecked(mhdlg, IDC_FIRST_LAST);
	mData.manual_mode=!!IsDlgButtonChecked(mhdlg, IDC_MANUAL_MODE);
	mData.disable_deinterlace=!!IsDlgButtonChecked(mhdlg, IDC_DEINTERLACE);
	mData.blend_anime=!!IsDlgButtonChecked(mhdlg, IDC_BLEND);
	mData.disable_Z_mode=!!IsDlgButtonChecked(mhdlg, IDC_Z_MODE);
	mData.disable_motion_map_filtering=!!IsDlgButtonChecked(mhdlg, IDC_MOTION_MAP);
	if (!!IsDlgButtonChecked(mhdlg, IDC_PROGRESSIVE)) mData.output_mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_TELECINE)) mData.output_mode=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_SMART)) mData.deinterlace_mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_STANDARD)) mData.deinterlace_mode=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_TRILINEAR)) mData.tri_linear=true;
	else mData.tri_linear=false;
	if (mData.manual_mode) GetWindowText(GetDlgItem(mhdlg, IDC_FILENAME),mData.filename,1024);

	return true;
}



bool JPSDR_IVTCDialog::OnCommand(int cmd)
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
		case IDC_MANUAL_MODE :
			mData.manual_mode=!!IsDlgButtonChecked(mhdlg, IDC_MANUAL_MODE);
			EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),mData.manual_mode);
			EnableWindow(GetDlgItem(mhdlg,IDC_FILENAME),mData.manual_mode);
			if (mData.manual_mode) GetWindowText(GetDlgItem(mhdlg, IDC_FILENAME),mData.filename,1024);
			return true;
		case IDC_OUTPUT_FILTER :
			mData.output_filter=!!IsDlgButtonChecked(mhdlg,IDC_OUTPUT_FILTER);
			EnableWindow(GetDlgItem(mhdlg,IDC_Y_ONLY),mData.output_filter);
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
				SetWindowText(GetDlgItem(mhdlg, IDC_FILENAME),mData.filename);
			}
			return true;		
	}
		
	return false;
}



class JPSDR_IVTC : public VDXVideoFilter
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
	void *error_Fields[Error_Fields_Size];
	IVTC_Data_Buffer buffer[Data_Buffer_Size];
	JPSDR_IVTC_Manual manual;
	uint8_t read_index[Read_Index_Size],error_index[Error_Index_Size];
	uint8_t write_index[Write_Index_Size],flag_index[Flag_Index_Size];
	void *_buffer_delta,*buffer_delta,*buffer_frame[2],*_buffer_frame[2];
	void *_buffer_frameRGB32[2],*buffer_frameRGB32[2];
	void *_buffer_resize_Y,*_buffer_resize_U,*_buffer_resize_V;
	void *buffer_resize_Y,*buffer_resize_U,*buffer_resize_V;
	uint8_t *buffer_frame_Y[2],*buffer_frame_U[2],*buffer_frame_V[2];
	uint8_t *buffer_delta_Y,*buffer_delta_U,*buffer_delta_V;
	uint8_t *buffer_map,*buffer_2;
	uint8_t first,last,last_removed_1,last_removed_2;
	bool out,previous_manual;
	uint16_t data_out[5];
	uint8_t index_out;
	bool invalid_first_last;
	bool *interlaced_tab[Interlaced_Tab_Size];
	bool resize_720x480,swap_buffer;
	sint32 resize_w0[720],resize_h0[240],resize_w1[360],resize_h1[120];
//	FILE *fic1,*fic2,*fic3;

	int16_t lookup[1280];
	uint16_t lookup_420[768];
	bool Integer_SSE_Enable,SSE2_Enable,MMX_Enable;

	void Deinterlace_Tri_Blend_RGB32(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_RGB32(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,bool tri_linear,bool align_data);
	void Motion_Map_RGB32(const void *src1,const void *src2,void *buffer,uint8_t *dst,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs);
	void Motion_Map_RGB32(const void *src1,const void *src2,uint8_t *dst,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs);
	void Motion_Map_YUYV_Y(const void *src1,const void *src2,uint8_t *dst,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs);
	void Motion_Map_UYVY_Y(const void *src1,const void *src2,uint8_t *dst,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs);
	void Motion_Map_Planar420_Y(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs);
	void Smart_Deinterlace_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer, uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs);
	void Smart_Deinterlace_Tri_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,
		uint8_t thrs);
	void Motion_Map_Filter(uint8_t *map,uint8_t *buffer,int32_t w,
		int32_t h,int32_t w_map);
	uint32_t Norme1_Histogramme_DeltaPicture_RGB32(const void *src1,const void *src2,
		void *dst,void *buffer,uint8_t *map,uint8_t *buffer_map,
		uint32_t *histo,uint32_t *repart,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,
		uint32_t &error_motion_map,uint8_t thrs);
	void Smart_Deinterlace_RGB32(const void *src1,const void *src2,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thrs,
		bool tri_linear,bool map_filter);
	uint32_t Norme1_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t pitch,
		ptrdiff_t modulo,uint32_t &error_motion_map,uint8_t thrs);
	uint32_t Norme1_RGB32(const void *src1,const void *src2,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo);

	void Convert_YUYV_to_RGB32(const void *src, void *dst, uint32_t src_size);
	void Convert_UYVY_to_RGB32(const void *src, void *dst, uint32_t src_size);
	void Convert_YUYV_to_RGB32(const void *src, void *dst, int32_t w, int32_t h,ptrdiff_t src_modulo);
	void Convert_UYVY_to_RGB32(const void *src, void *dst, int32_t w, int32_t h,ptrdiff_t src_modulo);

	void Convert_Planar420_to_RGB32(const uint8_t *src_Y,const uint8_t *src_U,const uint8_t *src_V,
		void *buffer,void *dst,int32_t w,int32_t h,ptrdiff_t pitch_Y,ptrdiff_t pitch_U,ptrdiff_t pitch_V,ptrdiff_t buffer_pitch);
	void Convert_Automatic_Planar420_to_YUY2(const uint8_t *src_Y,const uint8_t *src_U,const uint8_t *src_V,
		void *dst,int32_t w_Y,int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		uint8_t thrs);

	void Deinterlace_Tri_Blend_YUYV(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_YUYV(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_YUYV_Y(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_YUYV_Y(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Smart_Deinterlace_YUYV(const void *src1,const void *src2,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thrs,
		bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		ptrdiff_t RGBBuff_pitch,bool Y_only);
	void Deinterlace_YUYV(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,bool tri_linear,bool align_data,bool Y_only);

	void Deinterlace_Tri_Blend_UYVY(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_UYVY(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_UYVY_Y(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_UYVY_Y(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Smart_Deinterlace_UYVY(const void *src1,const void *src2,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thrs,
		bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		ptrdiff_t RGBBuff_pitch, bool Y_only);
	void Deinterlace_UYVY(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,bool tri_linear,bool align_data, bool Y_only);

	void Deinterlace_Tri_Blend_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Smart_Deinterlace_Planar420(const uint8_t *src1_Y,const uint8_t *src2_Y,
		const uint8_t *src1_U,const uint8_t *src2_U,const uint8_t *src1_V,const uint8_t *src2_V,
		uint8_t *dst_Y,uint8_t *dst_U,uint8_t *dst_V,uint8_t *buffer_Y,uint8_t *buffer_U,
		uint8_t *buffer_V,uint8_t *map,uint8_t *buffer_map,int32_t w_Y,int32_t w_U,int32_t w_V,int32_t h_Y,
		int32_t h_U,int32_t h_V,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t src_modulo_Y,
		ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,
		ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,uint8_t thrs,bool tri_linear,
		bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,ptrdiff_t RGBBuff_pitch, bool Y_only);
		void Deinterlace_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data);

	void Resize_Planar420(const void *src_y, const void *src_u, const void *src_v, void *dst_y, void *dst_u, void *dst_v,
		ptrdiff_t src_pitch_y, ptrdiff_t src_pitch_u, ptrdiff_t src_pitch_v);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_IVTCData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_IVTC)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_IVTC,ScriptConfig,"iiiiiiiiiiiiiiis")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_IVTC::Init()
{
	int16_t i;

	resize_720x480=false;
	_buffer_resize_Y=NULL;
	_buffer_resize_U=NULL;
	_buffer_resize_V=NULL;
	_buffer_delta=NULL;
	buffer_map=NULL;
	buffer_2=NULL;
	for (i=0; i<2; i++)
	{
		_buffer_frame[i]=NULL;
		_buffer_frameRGB32[i]=NULL;
	}
	for (i=0; i<Error_Fields_Size; i++)
		error_Fields[i]=NULL;
	for (i=0; i<Data_Buffer_Size; i++)
	{
		buffer[i]._frameRGB32=NULL;
		buffer[i]._frameRGB32Resize=NULL;
	}
	for (i=0; i<Interlaced_Tab_Size; i++)
	{
		interlaced_tab[i]=NULL;
	}

	for (i=0; i<256; i++)
	{
		lookup[i]=(int16_t)round(1.164*(i-16)*64.0+32.0);
		lookup[i+256]=(int16_t)round(1.596*(i-128)*64.0);
		lookup[i+512]=(int16_t)round(-0.813*(i-128)*64.0);
		lookup[i+768]=(int16_t)round(-0.391*(i-128)*64.0);
		lookup[i+1024]=(int16_t)round(2.018*(i-128)*64.0);
		lookup_420[i]=(uint16_t)(i*3);
		lookup_420[i+256]=(uint16_t)(i*5);
		lookup_420[i+512]=(uint16_t)(i*7);
	}

	return(true);
}



uint32 JPSDR_IVTC::GetParams()
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
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :*/
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :*/				
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :*/
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
/*		case nsVDXPixmap::kPixFormat_VDXA_RGB :
		case nsVDXPixmap::kPixFormat_VDXA_YUV :			*/						
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_709;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_FR;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_FR;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR;
			break;
	}
	
	fa->dst.offset = fa->src.offset;

	if (mData.output_filter || !mData.disable_deinterlace)
	{
		swap_buffer=true;
		return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS);
	}
	else
	{
		swap_buffer=false;
		return(FILTERPARAM_SUPPORTS_ALTFORMATS);
	}
}


bool JPSDR_IVTC::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	if (frame==0)
	{
		prefetcher->PrefetchFrame(0,frame,0);
		prefetcher->PrefetchFrame(0,frame+1,0);
		prefetcher->PrefetchFrame(0,frame+2,0);
		prefetcher->PrefetchFrame(0,frame+3,0);
		prefetcher->PrefetchFrame(0,frame+4,0);
		prefetcher->PrefetchFrame(0,frame+5,0);
		prefetcher->PrefetchFrame(0,frame+6,0);
		prefetcher->PrefetchFrame(0,frame+7,0);
	}
	else
	{
		prefetcher->PrefetchFrame(0,frame,0);
		prefetcher->PrefetchFrame(0,frame-2,0);
		prefetcher->PrefetchFrame(0,frame-1,0);
		prefetcher->PrefetchFrame(0,frame+1,0);
		prefetcher->PrefetchFrame(0,frame+2,0);
		prefetcher->PrefetchFrame(0,frame+6,0);
		prefetcher->PrefetchFrame(0,frame+7,0);
	}
	
	return(true);
}


void JPSDR_IVTC::Start()
{
	Image_Data idata;
	int32_t w_map;
	bool test;
	FILE *file_in;
	char buffer_in[1024];
	uint32_t u,v;
	ptrdiff_t offset_y,offset_u;
	uint16_t w,line_file;
	int32_t offset;
	int16_t i,j;
	double factor;

	if (g_VFVAPIVersion<14)
	{
		ff->Except("This virtualdub version doesn't support this filter !");
	}
	
	const VDXPixmapLayout& pxdst=*fa->dst.mpPixmapLayout;
	const VDXPixmapLayout& pxsrc=*fa->src.mpPixmapLayout;

		idata.src_h0=pxsrc.h;
		idata.src_w0=pxsrc.w;
		idata.src_pitch0=-pxsrc.pitch;
		idata.src_pitch1=-pxsrc.pitch2;
		idata.src_pitch2=-pxsrc.pitch3;

		idata.dst_h0=pxdst.h;
		idata.dst_w0=pxdst.w;
		idata.dst_pitch0=-pxdst.pitch;
		idata.dst_pitch1=-pxdst.pitch2;
		idata.dst_pitch2=-pxdst.pitch3;

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

	switch (idata.video_mode)
	{
		case 6 :
			if ((idata.src_h0%4)!=0)
			{
				ff->Except("Vertical size must be multiple of 4 for 4:2:0 !");
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

	if ((idata.src_h0<8)||(idata.src_w0<4))
	{
		ff->Except("Size must be at least 4x8 !");
		return;
	}

	if ((4*mData.line_removed+2)>=idata.src_h0)
	{
		ff->Except("More line removed than image size !");
		return;
	}

	Integer_SSE_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_INTEGER_SSE)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	MMX_Enable=ff->isMMXEnabled();

	if ((idata.src_w0%4)==0) w_map=idata.src_w0+4;
	else w_map=(((idata.src_w0 >> 2)+1)<< 2)+4;
	
	manual.nbre_sequence=0;
	offset=0;
	line_file=0;

	if ((idata.video_mode==6) && ((idata.src_h0>=600) || (idata.src_w0>=800))) resize_720x480=true;
	else resize_720x480=false;

	if (mData.manual_mode)
	{
		if (strlen(mData.filename)==0)
		{
			ff->Except("No file selected for manual mode !");
			return;
		}

		if (fopen_s(&file_in,mData.filename,"rt")!=0)
		{
			strcpy_s(buffer_in,1024,"Manual mode, unable to open file : ");
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
					sprintf_s(buffer_in,1024,"Error in manual file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
			}
			else
			{
				if (sscanf_s(buffer_in,"%lu %lu %hu",&u,&v,&w)==EOF)
				{
					fclose(file_in);
					sprintf_s(buffer_in,1024,"Error in manual file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
				else
				{
					if (w>9)
					{
						fclose(file_in);
						sprintf_s(buffer_in,1024,"Wrong ITVC seq. in manual file on line %ld.",line_file);
						ff->Except(buffer_in);
						return;
					}
					if ((u+offset)>(v+offset))
					{
						fclose(file_in);
						sprintf_s(buffer_in,1024,"Error on frame position in manual file on line %ld.",line_file);
						ff->Except(buffer_in);
						return;
					}
					manual.debut[manual.nbre_sequence]=u+offset;
					manual.fin[manual.nbre_sequence]=v+offset;
					if (w>4)
					{
						manual.ivtc_offset[manual.nbre_sequence]=(uint8_t)(w-5);
						manual.modif_polar[manual.nbre_sequence]=true;
					}
					else
					{
						manual.ivtc_offset[manual.nbre_sequence]=(uint8_t)w;
						manual.modif_polar[manual.nbre_sequence]=false;
					}
					manual.nbre_sequence++;
				}
			}
		}
		fclose(file_in);

		if (manual.nbre_sequence==0)
		{
			strcpy_s(buffer_in,1024,"No line in file : ");
			strcat_s(buffer_in,1024,mData.filename);
			ff->Except(buffer_in);
			return;
		}

		for(i=0; i<(manual.nbre_sequence-1); i++)
		{
			for(j=i+1; j<manual.nbre_sequence; j++)
			{
				if (((manual.debut[j]>=manual.debut[i]) && (manual.debut[j]<=manual.fin[i]))
					|| ((manual.fin[j]>=manual.debut[i]) && (manual.fin[j]<=manual.fin[i])))
				{
					sprintf_s(buffer_in,1024,"Overlap around lines %ld and %ld.",i+1,j+1);
					ff->Except(buffer_in);
					return;
				}
			}
		}
	}
	
	_buffer_delta=(void *)malloc(idata.src_h0*idata.src_w0*4+16); // Taille RGB32
	buffer_map=(uint8_t *)malloc(idata.src_h0*w_map*sizeof(uint8_t));
	buffer_2=(uint8_t *)malloc(idata.src_h0*w_map*sizeof(uint8_t));

	for (i=0; i<2; i++)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
				_buffer_frame[i]=(void *)malloc(idata.src_size0+16);
				break;
			case 6 :
				_buffer_frame[i]=(void *)malloc(idata.src_size0+idata.src_size1+idata.src_size2+48);
				break;
		}
		switch (idata.video_mode)
		{
			case 2 :
			case 3 :
			case 6 :
				_buffer_frameRGB32[i]=(void *)malloc(idata.src_h0*idata.src_w0*4+16);
				break;
		}
	}
	if (resize_720x480)
	{
		for (i=0; i<Error_Fields_Size; i++)
			error_Fields[i]=(void *)malloc(720*480*4); // Taille RGB32
	}
	else
	{
		for (i=0; i<Error_Fields_Size; i++)
			error_Fields[i]=(void *)malloc(idata.src_h0*idata.src_w0*4); // Taille RGB32
	}
	for (i=0; i<Data_Buffer_Size; i++)
	{
		switch (idata.video_mode)
		{
			case 2 :
			case 3 :
			case 6 :
				buffer[i]._frameRGB32=(void *)malloc(idata.src_h0*idata.src_w0*4+16);
				break;
		}
	}

	for (i=0; i<Interlaced_Tab_Size; i++)
	{
		interlaced_tab[i]=(bool*)malloc(idata.src_w0*sizeof(bool));
	}

	if (resize_720x480)
	{
		factor=720.0/(double)idata.src_w0;
		for (i=0; i<720; i++)
		{
			resize_w0[i]=(sint32)trunc((i+0.5)/factor);
		}
		factor=360.0/(double)idata.src_w1;
		for (i=0; i<360; i++)
		{
			resize_w1[i]=(sint32)trunc((i+0.5)/factor);
		}
		factor=240.0/(double)(idata.src_h0>>1);
		for (i=0; i<240; i++)
		{
			resize_h0[i]=(sint32)trunc((i+0.5)/factor);
		}
		factor=120.0/(double)(idata.src_h1>>1);
		for (i=0; i<120; i++)
		{
			resize_h1[i]=(sint32)trunc((i+0.5)/factor);
		}
		_buffer_resize_Y=(void *)malloc(720*480+16);
		_buffer_resize_U=(void *)malloc(368*240+16);
		_buffer_resize_V=(void *)malloc(368*240+16);
		for (i=0; i<Data_Buffer_Size; i++)
		{
			buffer[i]._frameRGB32Resize=(void *)malloc(720*480*4+16);
		}
	}

	test=((_buffer_delta!=NULL) && (buffer_map!=NULL) && (buffer_2!=NULL));
	for (i=0; i<2; i++)
	{
		test=test && (_buffer_frame[i]!=NULL);
		switch (idata.video_mode)
		{
			case 2 :
			case 3 :
			case 6 :
				test=test && (_buffer_frameRGB32[i]!=NULL);
				break;
		}
	}
	for (i=0; i<Error_Fields_Size; i++)
		test=test && (error_Fields[i]!=NULL);
	for (i=0; i<Data_Buffer_Size; i++)
	{
		switch (idata.video_mode)
		{
			case 2 :
			case 3 :
			case 6 :
				test=test && (buffer[i]._frameRGB32!=NULL);
				break;
		}
	}
	for (i=0; i<Interlaced_Tab_Size; i++)
	{
		test=test && (interlaced_tab[i]!=NULL);
	}
	if (resize_720x480)
	{
		test=test && (_buffer_resize_Y!=NULL);
		test=test && (_buffer_resize_U!=NULL);
		test=test && (_buffer_resize_V!=NULL);
		for (i=0; i<Data_Buffer_Size; i++)
		{
			test=test && (buffer[i]._frameRGB32Resize!=NULL);
		}
	}

	if (!test)
	{
		for (i=Data_Buffer_Size-1; i>=0; i--)
		{
			myfree(buffer[i]._frameRGB32Resize);
		}
		myfree(_buffer_resize_V);
		myfree(_buffer_resize_U);
		myfree(_buffer_resize_Y);
		for (i=Interlaced_Tab_Size-1; i>=0; i--)
		{
			myfree(interlaced_tab[i]);
		}
		for (i=Data_Buffer_Size-1; i>=0; i--)
		{
			myfree(buffer[i]._frameRGB32);
		}
		for (i=Error_Fields_Size-1; i>=0; i--)
			myfree(error_Fields[i]);
		for (i=1; i>=0; i--)
		{
			myfree(_buffer_frameRGB32[i]);
			myfree(_buffer_frame[i]);
		}
		myfree(buffer_2);
		myfree(buffer_map);
		myfree(_buffer_delta);
		ff->ExceptOutOfMemory();
		return;
	}

	for (i=0; i<Error_Index_Size; i++)
		error_index[i]=(uint8_t)i;
	for (i=0; i<Write_Index_Size; i++)
		write_index[i]=(uint8_t)(i+Filter_Lag+1);
	for (i=0; i<Read_Index_Size; i++)
		read_index[i]=(uint8_t)i;
	for (i=0; i<Flag_Index_Size; i++)
		flag_index[i]=(uint8_t)i;
	first=5;
	last=5;
	last_removed_1=5;
	last_removed_2=5;
	out=false;
	invalid_first_last=false;
	previous_manual=false;
	
	manual.read_index=0;
	manual.write_index=0;
	manual.last_sequence=9999;
	manual.last_offset=2;
	manual.first_offset=2;
	manual.ivtc_index=0;
	manual.sec_prec_ok=false;

	switch(idata.video_mode)
	{
		case 6 :
			if ((idata.src_size0&0x0F)==0) offset_y=idata.src_size0;
			else offset_y=((idata.src_size0+15)>>4)<<4;
			if ((idata.src_size1&0x0F)==0) offset_u=idata.src_size1;
			else offset_u=((idata.src_size1+15)>>4)<<4;
			break;
		default : offset_y=0; offset_u=0;
			break;
	}

	buffer_delta=(void *)(((size_t)_buffer_delta + 15) & ~(size_t)15);
	switch(idata.video_mode)
	{
		case 6 :
			buffer_delta_Y=(uint8_t *)buffer_delta;
			buffer_delta_U=buffer_delta_Y+offset_y;
			buffer_delta_V=buffer_delta_U+offset_u;
			break;
		default :
			buffer_delta_Y=(uint8_t *)buffer_delta;
			buffer_delta_U=buffer_delta_Y;
			buffer_delta_V=buffer_delta_Y;
			break;
	}
	for (i=0; i<2; i++)
	{
		buffer_frame[i]=(void *)(((size_t)_buffer_frame[i] + 15) & ~(size_t)15);
		switch (idata.video_mode)
		{
			case 6 :
				buffer_frame_Y[i]=(uint8_t *)buffer_frame[i];
				buffer_frame_U[i]=buffer_frame_Y[i]+offset_y;
				buffer_frame_V[i]=buffer_frame_U[i]+offset_u;
				break;
			default :
				buffer_frame_Y[i]=(uint8_t *)buffer_frame[i];
				buffer_frame_U[i]=buffer_frame_Y[i];
				buffer_frame_V[i]=buffer_frame_Y[i];
				break;
		}
		switch (idata.video_mode)
		{
			case 2 :
			case 3 :
			case 6 :
				buffer_frameRGB32[i]=(void *)(((size_t)_buffer_frameRGB32[i] + 15) & ~(size_t)15);
				break;
		}
	}
	for (i=0; i<Data_Buffer_Size; i++)
	{
		switch (idata.video_mode)
		{
			case 2 :
			case 3 :
			case 6 :
				buffer[i].frameRGB32=(void *)(((size_t)buffer[i]._frameRGB32 + 15) & ~(size_t)15);
				break;
		}
	}
	if (mData.manual_mode)
	{
		for (i=0; i<Buffer_Size_Manual; i++)
		{
			switch (idata.video_mode)
			{
				case 2 :
				case 3 :
				case 6 :
					manual.bufferRGB32[i]=buffer[i].frameRGB32;
					break;
			}
		}
	}
	if (resize_720x480)
	{
		for (i=0; i<Data_Buffer_Size; i++)
		{
			buffer[i].frameRGB32Resize=(void *)(((size_t)buffer[i]._frameRGB32Resize + 15) & ~(size_t)15);
		}
		buffer_resize_Y=(void *)(((size_t)_buffer_resize_Y + 15) & ~(size_t)15);
		buffer_resize_U=(void *)(((size_t)_buffer_resize_U + 15) & ~(size_t)15);
		buffer_resize_V=(void *)(((size_t)_buffer_resize_V + 15) & ~(size_t)15);
	}

/*	fic1=NULL;
	fic2=NULL;
	fic3=NULL;*/
}





void JPSDR_IVTC::GetSettingString(char *buf, int maxlen)
{
	char output_string[1400];

	if (mData.output_mode==1) strcpy_s(output_string,1400," Progressive [%d]");
	else strcpy_s(output_string,1400," Telecine [%d]");
	strcat_s(output_string,1400," Out [%d]");
	if (mData.output_filter) strcat_s(output_string,1400," (On)");
	else strcat_s(output_string,1400," (Off)");
	if (mData.line_removed>0) strcat_s(output_string,1400," Removed : %d");
	if (mData.inv_polar) strcat_s(output_string,1400," Inv. Pol.");
	if (mData.manual_mode)
	{
		strcat_s(output_string,1400," Manual mode : %s");
		if (mData.line_removed>0) SafePrintf(buf,maxlen,output_string,mData.threshold_ivtc,mData.threshold_filter,
			mData.line_removed,mData.filename);
		else SafePrintf(buf,maxlen,output_string,mData.threshold_ivtc,mData.threshold_filter,mData.filename);
	}
	else
	{
		if (mData.line_removed>0) SafePrintf(buf,maxlen,output_string,mData.threshold_ivtc,mData.threshold_filter,mData.line_removed);
		else SafePrintf(buf,maxlen,output_string,mData.threshold_ivtc,mData.threshold_filter);
	}
}




void JPSDR_IVTC::Resize_Planar420(const void *_src_y, const void *_src_u, const void *_src_v, void *_dst_y, void *_dst_u,
	void *_dst_v, ptrdiff_t src_pitch_y, ptrdiff_t src_pitch_u, ptrdiff_t src_pitch_v)

{
	for (int32_t i=0; i<240; i++)
	{
		const uint8_t *src_y;
		uint8_t *dst_y;
		
		src_y=(uint8_t *)_src_y+(resize_h0[i]*(src_pitch_y<<1));
		dst_y=(uint8_t *)_dst_y+(i*1440);

		for (int32_t j=0; j<720; j++)
		{
			*dst_y++=src_y[resize_w0[j]];
		}
	}
	for (int32_t i=0; i<240; i++)
	{
		const uint8_t *src_y;
		uint8_t *dst_y;
		
		src_y=(uint8_t *)_src_y+(src_pitch_y+resize_h0[i]*(src_pitch_y<<1));
		dst_y=(uint8_t *)_dst_y+(720+i*1440);

		for (int32_t j=0; j<720; j++)
		{
			*dst_y++=src_y[resize_w0[j]];
		}
	}
	for (int32_t i=0; i<120; i++)
	{
		const uint8_t *src_u;
		uint8_t *dst_u;
		
		src_u=(uint8_t *)_src_u+(resize_h1[i]*(src_pitch_u<<1));
		dst_u=(uint8_t *)_dst_u+(i*736);

		for (int32_t j=0; j<360; j++)
		{
			*dst_u++=src_u[resize_w1[j]];
		}
	}
	for (int32_t i=0; i<120; i++)
	{
		const uint8_t *src_u;
		uint8_t *dst_u;
		
		src_u=(uint8_t *)_src_u+(src_pitch_u+resize_h1[i]*(src_pitch_u<<1));
		dst_u=(uint8_t *)_dst_u+(368+i*736);

		for (int32_t j=0; j<360; j++)
		{
			*dst_u++=src_u[resize_w1[j]];
		}
	}
	for (int32_t i=0; i<120; i++)
	{
		const uint8_t *src_v;
		uint8_t *dst_v;
		
		src_v=(uint8_t *)_src_v+(resize_h1[i]*(src_pitch_v<<1));
		dst_v=(uint8_t *)_dst_v+(i*736);

		for (int32_t j=0; j<360; j++)
		{
			*dst_v++=src_v[resize_w1[j]];
		}
	}
	for (int32_t i=0; i<120; i++)
	{
		const uint8_t *src_v;
		uint8_t *dst_v;
		
		src_v=(uint8_t *)_src_v+(src_pitch_v+resize_h1[i]*(src_pitch_v<<1));
		dst_v=(uint8_t *)_dst_v+(368+i*736);

		for (int32_t j=0; j<360; j++)
		{
			*dst_v++=src_v[resize_w1[j]];
		}
	}

}

void JPSDR_IVTC::Convert_YUYV_to_RGB32(const void *src_0, void *dst_0, int32_t w, int32_t h,ptrdiff_t src_modulo)
{		
	int32_t w1;
	const YUYV *src;
	RGB32BMP *dst;

	src=(YUYV *)src_0;
	dst=(RGB32BMP *)dst_0;
	w1=(w+1)>>1;

	if (SSE2_Enable)
	{
		JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2(src_0,dst_0,lookup,w1,h,src_modulo);
	}
	else
	{
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w1; j++)
			{
				int16_t r,g,b;
				uint16_t y1,y2,u,v;

				y1=src->y1;
				u=src->u;
				y2=src->y2;
				v=src->v;
				src++; 

				r=(lookup[y1]+lookup[256+v])>>6;
				g=(lookup[y1]+lookup[512+v]+lookup[768+u])>>6;
				b=(lookup[y1]+lookup[1024+u])>>6;
				if (r<0) r=0;
				else
				{
					if (r>255) r=255;
				}
				if (g<0) g=0;
				else
				{
					if (g>255) g=255;
				}
				if (b<0) b=0;
				else
				{
					if (b>255) b=255;
				}
				dst->b=(uint8_t)b;
				dst->g=(uint8_t)g;
				dst->r=(uint8_t)r;
				dst->alpha=0;
				dst++;

				r=(lookup[y2]+lookup[256+v])>>6;
				g=(lookup[y2]+lookup[512+v]+lookup[768+u])>>6;
				b=(lookup[y2]+lookup[1024+u])>>6;
				if (r<0) r=0;
				else
				{
					if (r>255) r=255;
				}
				if (g<0) g=0;
				else
				{
					if (g>255) g=255;
				}
				if (b<0) b=0;
				else
				{
					if (b>255) b=255;
				}
				dst->b=(uint8_t)b;
				dst->g=(uint8_t)g;
				dst->r=(uint8_t)r;
				dst->alpha=0;
				dst++;
			}
			src=(YUYV *)((uint8_t *)src+src_modulo);
		}
	}
}


void JPSDR_IVTC::Convert_YUYV_to_RGB32(const void *src_0, void *dst_0, uint32_t src_size)
{
	const YUYV *src;
	RGB32BMP *dst;

	src=(YUYV *)src_0;
	dst=(RGB32BMP *)dst_0;

	if (SSE2_Enable)
	{
		JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2(src_0,dst_0,lookup,src_size);
	}
	else
	{
		for (uint32_t i=0; i<src_size; i++)
		{
			int16_t r,g,b;
			uint16_t y1,y2,u,v;

			y1=src->y1;
			u=src->u;
			y2=src->y2;
			v=src->v;
			src++; 

			r=(lookup[y1]+lookup[256+v])>>6;
			g=(lookup[y1]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y1]+lookup[1024+u])>>6;
			if (r<0) r=0;
			else
			{
				if (r>255) r=255;
			}
			if (g<0) g=0;
			else
			{
				if (g>255) g=255;
			}
			if (b<0) b=0;
			else
			{
				if (b>255) b=255;
			}
			dst->b=(uint8_t)b;
			dst->g=(uint8_t)g;
			dst->r=(uint8_t)r;
			dst->alpha=0;
			dst++;

			r=(lookup[y2]+lookup[256+v])>>6;
			g=(lookup[y2]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y2]+lookup[1024+u])>>6;
			if (r<0) r=0;
			else
			{
				if (r>255) r=255;
			}
			if (g<0) g=0;
			else
			{
				if (g>255) g=255;
			}
			if (b<0) b=0;
			else
			{
				if (b>255) b=255;
			}
			dst->b=(uint8_t)b;
			dst->g=(uint8_t)g;
			dst->r=(uint8_t)r;
			dst->alpha=0;
			dst++;
		}
	}
}


void JPSDR_IVTC::Convert_UYVY_to_RGB32(const void *src_0, void *dst_0, int32_t w, int32_t h,ptrdiff_t src_modulo)
{
	int32_t w1;
	const UYVY *src;
	RGB32BMP *dst;

	src=(UYVY *)src_0;
	dst=(RGB32BMP *)dst_0;
	w1=(w+1)>>1;

	if (SSE2_Enable)
	{
		JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2(src_0,dst_0,lookup,w1,h,src_modulo);
	}
	else
	{
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w1; j++)
			{
				int16_t r,g,b;
				uint16_t y1,y2,u,v;

				u=src->u;
				y1=src->y1;
				v=src->v;
				y2=src->y2;
				src++;

				r=(lookup[y1]+lookup[256+v])>>6;
				g=(lookup[y1]+lookup[512+v]+lookup[768+u])>>6;
				b=(lookup[y1]+lookup[1024+u])>>6;
				if (r<0) r=0;
				else
				{
					if (r>255) r=255;
				}
				if (g<0) g=0;
				else
				{
					if (g>255) g=255;
				}
				if (b<0) b=0;
				else
				{
					if (b>255) b=255;
				}
				dst->b=(uint8_t)b;
				dst->g=(uint8_t)g;
				dst->r=(uint8_t)r;
				dst->alpha=0;
				dst++;

				r=(lookup[y2]+lookup[256+v])>>6;
				g=(lookup[y2]+lookup[512+v]+lookup[768+u])>>6;
				b=(lookup[y2]+lookup[1024+u])>>6;
				if (r<0) r=0;
				else
				{
					if (r>255) r=255;
				}
				if (g<0) g=0;
				else
				{
					if (g>255) g=255;
				}
				if (b<0) b=0;
				else
				{
					if (b>255) b=255;
				}
				dst->b=(uint8_t)b;
				dst->g=(uint8_t)g;
				dst->r=(uint8_t)r;
				dst->alpha=0;
				dst++;
			}
			src=(UYVY *)((uint8_t *)src+src_modulo);
		}
	}
}



void JPSDR_IVTC::Convert_UYVY_to_RGB32(const void *src_0, void *dst_0, uint32_t src_size)
{
	const UYVY *src;
	RGB32BMP *dst;

	src=(UYVY *)src_0;
	dst=(RGB32BMP *)dst_0;

	if (SSE2_Enable)
	{
		JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2(src_0,dst_0,lookup,src_size);
	}
	else
	{
		for (uint32_t i=0; i<src_size; i++)
		{
			int16_t r,g,b;
			uint16_t y1,y2,u,v;

			u=src->u;
			y1=src->y1;
			v=src->v;
			y2=src->y2;
			src++;

			r=(lookup[y1]+lookup[256+v])>>6;
			g=(lookup[y1]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y1]+lookup[1024+u])>>6;
			if (r<0) r=0;
			else
			{
				if (r>255) r=255;
			}
			if (g<0) g=0;
			else
			{
				if (g>255) g=255;
			}
			if (b<0) b=0;
			else
			{
				if (b>255) b=255;
			}
			dst->b=(uint8_t)b;
			dst->g=(uint8_t)g;
			dst->r=(uint8_t)r;
			dst->alpha=0;
			dst++;

			r=(lookup[y2]+lookup[256+v])>>6;
			g=(lookup[y2]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y2]+lookup[1024+u])>>6;
			if (r<0) r=0;
			else
			{
				if (r>255) r=255;
			}
			if (g<0) g=0;
			else
			{
				if (g>255) g=255;
			}
			if (b<0) b=0;
			else
			{
				if (b>255) b=255;
			}
			dst->b=(uint8_t)b;
			dst->g=(uint8_t)g;
			dst->r=(uint8_t)r;
			dst->alpha=0;
			dst++;
		}
	}
}


void JPSDR_IVTC::Convert_Planar420_to_RGB32(const uint8_t *src_Y,const uint8_t *src_U,const uint8_t *src_V,
		void *buffer,void *dst,int32_t w,int32_t h,ptrdiff_t pitch_Y,ptrdiff_t pitch_U,ptrdiff_t pitch_V,ptrdiff_t buffer_pitch)
{
	Convert_Automatic_Planar420_to_YUY2(src_Y,src_U,src_V,buffer,w,h,pitch_Y,pitch_U,pitch_V,buffer_pitch,Threshold_Convert);
	Convert_YUYV_to_RGB32(buffer,dst,((w+1)>>1)*h);
}

void JPSDR_IVTC::Convert_Automatic_Planar420_to_YUY2(const uint8_t *src_Y,const uint8_t *src_U,const uint8_t *src_V,
		void *_dst,int32_t w_Y,int32_t h_Y,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t dst_pitch,
		uint8_t thrs)
{
	int32_t h_4,w_Y2;
	const uint8_t *src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	int16_t threshold;
	ptrdiff_t pitch_U_2,pitch_V_2;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	YUYV *dst;

	pitch_U_2=2*src_pitch_U;
	pitch_V_2=2*src_pitch_V;
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
	dst=(YUYV *)_dst;
	threshold=thrs;

	h_4=h_Y-4;
	w_Y2=(w_Y+1)>>1;

	for(int32_t i=0; i<4; i+=4)
	{
		int32_t j_UV;

		JPSDR_IVTC_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_IVTC_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup_420[src_Unn[j_UV]]+lookup_420[src_U[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup_420[src_Vnn[j_UV]]+lookup_420[src_V[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup_420[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup_420[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
	
			if (((abs((int16_t)src_Un[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				(abs((int16_t)src_Unnn[j_UV]-(int16_t)src_Unn[j_UV])>=threshold) &&
				((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV]) ||
				(src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV]))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV]) ||
				(src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV]))))
				interlaced_tab[1][j_UV]=true;
			else interlaced_tab[1][j_UV]=false;
			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV]) ||
				(src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV]))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV]) ||
				(src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV]))))
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
				dst[j_UV].u=(uint8_t)((lookup_420[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4) >> 3);
				dst[j_UV].v=(uint8_t)((lookup_420[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4) >> 3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup_420[src_U[j_UV]]+(uint16_t)src_Up[j_UV]+2) >> 2);
				dst[j_UV].v=(uint8_t)((lookup_420[src_V[j_UV]]+(uint16_t)src_Vp[j_UV]+2) >> 2);
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

			if (((abs((int16_t)src_U[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				(abs((int16_t)src_Unn[j_UV]-(int16_t)src_Un[j_UV])>=threshold) &&
				((src_U[j_UV]>src_Un[j_UV]) && (src_Unn[j_UV]>src_Un[j_UV]) ||
				(src_U[j_UV]<src_Un[j_UV]) && (src_Unn[j_UV]<src_Un[j_UV]))) ||
				((abs((int16_t)src_V[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnn[j_UV]-(int16_t)src_Vn[j_UV])>=threshold) &&
				((src_V[j_UV]>src_Vn[j_UV]) && (src_Vnn[j_UV]>src_Vn[j_UV]) ||
				(src_V[j_UV]<src_Vn[j_UV]) && (src_Vnn[j_UV]<src_Vn[j_UV]))))
				interlaced_tab[index_tab_2][j_UV]=true;
			else interlaced_tab[index_tab_2][j_UV]=false;			

			// Upsample as needed.
			if ((interlaced_tab[index_tab_1][j_UV]) || (interlaced_tab[index_tab_2][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup_420[src_Up[j_UV]]+lookup_420[src_Un[j_UV]+256]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup_420[src_Vp[j_UV]]+lookup_420[src_Vn[j_UV]+256]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup_420[src_U[j_UV]]+(uint16_t)src_Un[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup_420[src_V[j_UV]]+(uint16_t)src_Vn[j_UV]+2)>>2);
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
				dst[j_UV].u=(uint8_t)((lookup_420[src_Unn[j_UV]]+lookup_420[src_U[j_UV]+256]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup_420[src_Vnn[j_UV]]+lookup_420[src_V[j_UV]+256]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup_420[src_Un[j_UV]]+(uint16_t)src_U[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup_420[src_Vn[j_UV]]+(uint16_t)src_V[j_UV]+2)>>2);
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
				((src_Un[j_UV]>src_Unn[j_UV]) && (src_Unnn[j_UV]>src_Unn[j_UV]) ||
				(src_Un[j_UV]<src_Unn[j_UV]) && (src_Unnn[j_UV]<src_Unn[j_UV]))) ||
				((abs((int16_t)src_Vn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				(abs((int16_t)src_Vnnn[j_UV]-(int16_t)src_Vnn[j_UV])>=threshold) &&
				((src_Vn[j_UV]>src_Vnn[j_UV]) && (src_Vnnn[j_UV]>src_Vnn[j_UV]) ||
				(src_Vn[j_UV]<src_Vnn[j_UV]) && (src_Vnnn[j_UV]<src_Vnn[j_UV]))))
				interlaced_tab[index_tab_0][j_UV]=true;
			else interlaced_tab[index_tab_0][j_UV]=false;

			// Upsample as needed.
			if ((interlaced_tab[index_tab_2][j_UV]) || (interlaced_tab[index_tab_0][j_UV]))
			{
				dst[j_UV].u=(uint8_t)((lookup_420[src_Un[j_UV]+512]+(uint16_t)src_Unnn[j_UV]+4)>>3);
				dst[j_UV].v=(uint8_t)((lookup_420[src_Vn[j_UV]+512]+(uint16_t)src_Vnnn[j_UV]+4)>>3);
			}
			else
			{
				dst[j_UV].u=(uint8_t)((lookup_420[src_Un[j_UV]]+(uint16_t)src_Unn[j_UV]+2)>>2);
				dst[j_UV].v=(uint8_t)((lookup_420[src_Vn[j_UV]]+(uint16_t)src_Vnn[j_UV]+2)>>2);
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
			dst[j_UV].u=(uint8_t)((lookup_420[src_U[j_UV]+512]+(uint16_t)src_Upp[j_UV]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup_420[src_V[j_UV]+512]+(uint16_t)src_Vpp[j_UV]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		j_UV=0;
		for (int32_t j=0; j<w_Y; j+=2)
		{
			dst[j_UV].y1=src_Y[j];
			dst[j_UV].y2=src_Y[j+1];
			dst[j_UV].u=(uint8_t)((lookup_420[src_Up[j_UV]]+lookup_420[src_Un[j_UV]+256]+4)>>3);
			dst[j_UV].v=(uint8_t)((lookup_420[src_Vp[j_UV]]+lookup_420[src_Vn[j_UV]+256]+4)>>3);
			j_UV++;
		}
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_IVTC_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch);
		src_Y+=src_pitch_Y;

		JPSDR_IVTC_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
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


void JPSDR_IVTC::Deinterlace_Tri_Blend_RGB32(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *dst,*dst1,*dst2;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	dst=(RGB32BMP *)dst_0;
	src3=(RGB32BMP *)((uint8_t *)src1+ (src_pitch << 1));
	src4=(RGB32BMP *)((uint8_t *)src2+ (src_pitch << 1));

	memcpy(dst,src1,w*4);

	dst1=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	dst2=(RGB32BMP *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;
			uint8_t alpha1,alpha2;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			alpha1=src2[j].alpha;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			alpha2=src3[j].alpha;
			b=((b1+b3)+(b2 << 1)+2) >> 2;
			g=((g1+g3)+(g2 << 1)+2) >> 2;
			r=((r1+r3)+(r2 << 1)+2) >> 2;
			dst1[j].b=(uint8_t)b;
			dst1[j].g=(uint8_t)g;
			dst1[j].r=(uint8_t)r;
			dst1[j].alpha=alpha1;
			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			b=((b1+b2)+(b3 << 1)+2) >> 2;
			g=((g1+g2)+(g3 << 1)+2) >> 2;
			r=((r1+r2)+(r3 << 1)+2) >> 2;
			dst2[j].b=(uint8_t)b;
			dst2[j].g=(uint8_t)g;
			dst2[j].r=(uint8_t)r;
			dst2[j].alpha=alpha2;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+(src_pitch << 1));
		src2=(RGB32BMP *)((uint8_t *)src2+(src_pitch << 1));
		src3=(RGB32BMP *)((uint8_t *)src3+(src_pitch << 1));
		src4=(RGB32BMP *)((uint8_t *)src4+(src_pitch << 1));
		dst1=(RGB32BMP *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(RGB32BMP *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	memcpy(dst1,src2,w*4);

}




void JPSDR_IVTC::Deinterlace_Tri_Blend_YUYV(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3,*src4;
	YUYV *dst,*dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst=(YUYV *)dst_0;
	src3=(YUYV *)((uint8_t *)src1+ (src_pitch << 1));
	src4=(YUYV *)((uint8_t *)src2+ (src_pitch << 1));

	memcpy(dst,src1,w*4);

	dst1=(YUYV *)((uint8_t *)dst+dst_pitch);
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,u1,v1,y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;
			uint16_t y1,y2,u,v;

			y1_1=src1[j].y1;
			u1=src1[j].u;
			y1_2=src1[j].y2;
			v1=src1[j].v;
			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			y3_1=src3[j].y1;
			u3=src3[j].u;
			y3_2=src3[j].y2;
			v3=src3[j].v;
			y1=((y1_1+y3_1)+(y2_1 << 1)+2) >> 2;
			u=((u1+u3)+(u2 << 1)+2) >> 2;
			y2=((y1_2+y3_2)+(y2_2 << 1)+2) >> 2;
			v=((v1+v3)+(v2 << 1)+2) >> 2;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].u=(uint8_t)u;
			dst1[j].y2=(uint8_t)y2;
			dst1[j].v=(uint8_t)v;
			y1_1=src4[j].y1;
			u1=src4[j].u;
			y1_2=src4[j].y2;
			v1=src4[j].v;
			y1=((y1_1+y2_1)+(y3_1 << 1)+2) >> 2;
			u=((u1+u2)+(u3 << 1)+2) >> 2;
			y2=((y1_2+y2_2)+(y3_2 << 1)+2) >> 2;
			v=((v1+v2)+(v3 << 1)+2) >> 2;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].u=(uint8_t)u;
			dst2[j].y2=(uint8_t)y2;
			dst2[j].v=(uint8_t)v;
		}
		src1=(YUYV *)((uint8_t *)src1+(src_pitch << 1));
		src2=(YUYV *)((uint8_t *)src2+(src_pitch << 1));
		src3=(YUYV *)((uint8_t *)src3+(src_pitch << 1));
		src4=(YUYV *)((uint8_t *)src4+(src_pitch << 1));
		dst1=(YUYV *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(YUYV *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	memcpy(dst1,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Tri_Blend_YUYV_Y(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3,*src4;
	YUYV *dst,*dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst=(YUYV *)dst_0;
	src3=(YUYV *)((uint8_t *)src1+ (src_pitch << 1));
	src4=(YUYV *)((uint8_t *)src2+ (src_pitch << 1));

	memcpy(dst,src1,w*4);

	dst1=(YUYV *)((uint8_t *)dst+dst_pitch);
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,y2_1,y2_2,y3_1,y3_2;
			uint16_t y1,y2;
			uint8_t u1,v1,u2,v2;

			y1_1=src1[j].y1;
			y1_2=src1[j].y2;
			y2_1=src2[j].y1;
			u1=src2[j].u;
			y2_2=src2[j].y2;
			v1=src2[j].v;
			y3_1=src3[j].y1;
			u2=src2[j].u;
			y3_2=src3[j].y2;
			v2=src2[j].v;
			y1=((y1_1+y3_1)+(y2_1 << 1)+2) >> 2;
			y2=((y1_2+y3_2)+(y2_2 << 1)+2) >> 2;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].u=u1;
			dst1[j].y2=(uint8_t)y2;
			dst1[j].v=v1;
			y1_1=src4[j].y1;
			y1_2=src4[j].y2;
			y1=((y1_1+y2_1)+(y3_1 << 1)+2) >> 2;
			y2=((y1_2+y2_2)+(y3_2 << 1)+2) >> 2;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].u=u2;
			dst2[j].y2=(uint8_t)y2;
			dst2[j].v=v2;
		}
		src1=(YUYV *)((uint8_t *)src1+(src_pitch << 1));
		src2=(YUYV *)((uint8_t *)src2+(src_pitch << 1));
		src3=(YUYV *)((uint8_t *)src3+(src_pitch << 1));
		src4=(YUYV *)((uint8_t *)src4+(src_pitch << 1));
		dst1=(YUYV *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(YUYV *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	memcpy(dst1,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Tri_Blend_UYVY(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3,*src4;
	UYVY *dst,*dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst=(UYVY *)dst_0;
	src3=(UYVY *)((uint8_t *)src1+ (src_pitch << 1));
	src4=(UYVY *)((uint8_t *)src2+ (src_pitch << 1));

	memcpy(dst,src1,w*4);

	dst1=(UYVY *)((uint8_t *)dst+dst_pitch);
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,u1,v1,y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;
			uint16_t y1,y2,u,v;

			u1=src1[j].u;
			y1_1=src1[j].y1;
			v1=src1[j].v;
			y1_2=src1[j].y2;
			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			u3=src3[j].u;
			y3_1=src3[j].y1;
			v3=src3[j].v;
			y3_2=src3[j].y2;
			y1=((y1_1+y3_1)+(y2_1 << 1)+2) >> 2;
			u=((u1+u3)+(u2 << 1)+2) >> 2;
			y2=((y1_2+y3_2)+(y2_2 << 1)+2) >> 2;
			v=((v1+v3)+(v2 << 1)+2) >> 2;
			dst1[j].u=(uint8_t)u;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].v=(uint8_t)v;
			dst1[j].y2=(uint8_t)y2;
			u1=src4[j].u;
			y1_1=src4[j].y1;
			v1=src4[j].v;
			y1_2=src4[j].y2;
			y1=((y1_1+y2_1)+(y3_1 << 1)+2) >> 2;
			u=((u1+u2)+(u3 << 1)+2) >> 2;
			y2=((y1_2+y2_2)+(y3_2 << 1)+2) >> 2;
			v=((v1+v2)+(v3 << 1)+2) >> 2;
			dst2[j].u=(uint8_t)u;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].v=(uint8_t)v;
			dst2[j].y2=(uint8_t)y2;
		}
		src1=(UYVY *)((uint8_t *)src1+(src_pitch << 1));
		src2=(UYVY *)((uint8_t *)src2+(src_pitch << 1));
		src3=(UYVY *)((uint8_t *)src3+(src_pitch << 1));
		src4=(UYVY *)((uint8_t *)src4+(src_pitch << 1));
		dst1=(UYVY *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(UYVY *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	memcpy(dst1,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Tri_Blend_UYVY_Y(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3,*src4;
	UYVY *dst,*dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst=(UYVY *)dst_0;
	src3=(UYVY *)((uint8_t *)src1+ (src_pitch << 1));
	src4=(UYVY *)((uint8_t *)src2+ (src_pitch << 1));

	memcpy(dst,src1,w*4);

	dst1=(UYVY *)((uint8_t *)dst+dst_pitch);
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,y2_1,y2_2,y3_1,y3_2;
			uint16_t y1,y2;
			uint8_t u1,v1,u2,v2;

			y1_1=src1[j].y1;
			y1_2=src1[j].y2;
			u1=src2[j].u;
			y2_1=src2[j].y1;
			v1=src2[j].v;
			y2_2=src2[j].y2;
			u2=src3[j].u;
			y3_1=src3[j].y1;
			v2=src3[j].v;
			y3_2=src3[j].y2;
			y1=((y1_1+y3_1)+(y2_1 << 1)+2) >> 2;
			y2=((y1_2+y3_2)+(y2_2 << 1)+2) >> 2;
			dst1[j].u=u1;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].v=v1;
			dst1[j].y2=(uint8_t)y2;
			y1_1=src4[j].y1;
			y1_2=src4[j].y2;
			y1=((y1_1+y2_1)+(y3_1 << 1)+2) >> 2;
			y2=((y1_2+y2_2)+(y3_2 << 1)+2) >> 2;
			dst2[j].u=u2;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].v=v2;
			dst2[j].y2=(uint8_t)y2;
		}
		src1=(UYVY *)((uint8_t *)src1+(src_pitch << 1));
		src2=(UYVY *)((uint8_t *)src2+(src_pitch << 1));
		src3=(UYVY *)((uint8_t *)src3+(src_pitch << 1));
		src4=(UYVY *)((uint8_t *)src4+(src_pitch << 1));
		dst1=(UYVY *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(UYVY *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	memcpy(dst1,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Tri_Blend_Planar(const uint8_t *src1,const uint8_t *src2,
	uint8_t *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src3,*src4;
	uint8_t *dst1,*dst2;

	src3=src1+(src_pitch << 1);
	src4=src2+(src_pitch << 1);

	memcpy(dst,src1,w);

	dst1=dst+dst_pitch;
	dst2=dst1+dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t a1,a2,a3;

			a1=src1[j];
			a2=src2[j];
			a3=src3[j];
			dst1[j]=(uint8_t)(((a1+a3)+(a2 << 1)+2) >> 2);
			a1=src4[j];
			dst2[j]=(uint8_t)(((a1+a2)+(a3 << 1)+2) >> 2);
		}
		src1+=(src_pitch << 1);
		src2+=(src_pitch << 1);
		src3+=(src_pitch << 1);
		src4+=(src_pitch << 1);
		dst1+=(dst_pitch << 1);
		dst2+=(dst_pitch << 1);
	}

	memcpy(dst1,src2,w);

}




void JPSDR_IVTC::Deinterlace_Blend_YUYV(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+ (src_pitch << 1));

	dst1=(YUYV *)dst_0;
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,u1,v1,y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;
			uint16_t y1,y2,u,v;

			y1_1=src1[j].y1;
			u1=src1[j].u;
			y1_2=src1[j].y2;
			v1=src1[j].v;
			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			y3_1=src3[j].y1;
			u3=src3[j].u;
			y3_2=src3[j].y2;
			v3=src3[j].v;
			y1=(y1_1+y2_1) >> 1;
			u=(u1+u2) >> 1;
			y2=(y1_2+y2_2) >> 1;
			v=(v1+v2) >> 1;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].u=(uint8_t)u;
			dst1[j].y2=(uint8_t)y2;
			dst1[j].v=(uint8_t)v;
			y1=(y2_1+y3_1) >> 1;
			u=(u2+u3) >> 1;
			y2=(y2_2+y3_2) >> 1;
			v=(v2+v3) >> 1;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].u=(uint8_t)u;
			dst2[j].y2=(uint8_t)y2;
			dst2[j].v=(uint8_t)v;
		}
		src1=(YUYV *)((uint8_t *)src1+(src_pitch << 1));
		src2=(YUYV *)((uint8_t *)src2+(src_pitch << 1));
		src3=(YUYV *)((uint8_t *)src3+(src_pitch << 1));
		dst1=(YUYV *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(YUYV *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	for (int32_t j=0; j<w; j++)
	{
		uint16_t y1_1,y1_2,u1,v1,y2_1,y2_2,u2,v2,y1,y2,u,v;

		y1_1=src1[j].y1;
		u1=src1[j].u;
		y1_2=src1[j].y2;
		v1=src1[j].v;
		y2_1=src2[j].y1;
		u2=src2[j].u;
		y2_2=src2[j].y2;
		v2=src2[j].v;
		y1=(y1_1+y2_1) >> 1;
		u=(u1+u2) >> 1;
		y2=(y1_2+y2_2) >> 1;
		v=(v1+v2) >> 1;
		dst1[j].y1=(uint8_t)y1;
		dst1[j].u=(uint8_t)u;
		dst1[j].y2=(uint8_t)y2;
		dst1[j].v=(uint8_t)v;
	}
	memcpy(dst2,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Blend_YUYV_Y(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+ (src_pitch << 1));

	dst1=(YUYV *)dst_0;
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,y2_1,y2_2,y3_1,y3_2;
			uint16_t y1,y2;
			uint8_t u1,v1,u2,v2;

			y1_1=src1[j].y1;
			u1=src1[j].u;
			y1_2=src1[j].y2;
			v1=src1[j].v;
			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			y3_1=src3[j].y1;
			y3_2=src3[j].y2;
			y1=(y1_1+y2_1) >> 1;
			y2=(y1_2+y2_2) >> 1;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].u=u1;
			dst1[j].y2=(uint8_t)y2;
			dst1[j].v=v1;
			y1=(y2_1+y3_1) >> 1;
			y2=(y2_2+y3_2) >> 1;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].u=u2;
			dst2[j].y2=(uint8_t)y2;
			dst2[j].v=v2;
		}
		src1=(YUYV *)((uint8_t *)src1+(src_pitch << 1));
		src2=(YUYV *)((uint8_t *)src2+(src_pitch << 1));
		src3=(YUYV *)((uint8_t *)src3+(src_pitch << 1));
		dst1=(YUYV *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(YUYV *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	for (int32_t j=0; j<w; j++)
	{
		uint16_t y1_1,y1_2,y2_1,y2_2,y1,y2;
		uint8_t u1,v1;

		y1_1=src1[j].y1;
		u1=src1[j].u;
		y1_2=src1[j].y2;
		v1=src1[j].v;
		y2_1=src2[j].y1;
		y2_2=src2[j].y2;
		y1=(y1_1+y2_1) >> 1;
		y2=(y1_2+y2_2) >> 1;
		dst1[j].y1=(uint8_t)y1;
		dst1[j].u=u1;
		dst1[j].y2=(uint8_t)y2;
		dst1[j].v=v1;
	}
	memcpy(dst2,src2,w*4);

}

void JPSDR_IVTC::Deinterlace_Blend_UYVY(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3;
	UYVY *dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	src3=(UYVY *)((uint8_t *)src1+ (src_pitch << 1));

	dst1=(UYVY *)dst_0;
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,u1,v1,y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;
			uint16_t y1,y2,u,v;

			u1=src1[j].u;
			y1_1=src1[j].y1;
			v1=src1[j].v;
			y1_2=src1[j].y2;
			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			u3=src3[j].u;
			y3_1=src3[j].y1;
			v3=src3[j].v;
			y3_2=src3[j].y2;
			y1=(y1_1+y2_1) >> 1;
			u=(u1+u2) >> 1;
			y2=(y1_2+y2_2) >> 1;
			v=(v1+v2) >> 1;
			dst1[j].u=(uint8_t)u;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].v=(uint8_t)v;
			dst1[j].y2=(uint8_t)y2;
			y1=(y2_1+y3_1) >> 1;
			u=(u2+u3) >> 1;
			y2=(y2_2+y3_2) >> 1;
			v=(v2+v3) >> 1;
			dst2[j].u=(uint8_t)u;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].v=(uint8_t)v;
			dst2[j].y2=(uint8_t)y2;
		}
		src1=(UYVY *)((uint8_t *)src1+(src_pitch << 1));
		src2=(UYVY *)((uint8_t *)src2+(src_pitch << 1));
		src3=(UYVY *)((uint8_t *)src3+(src_pitch << 1));
		dst1=(UYVY *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(UYVY *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	for (int32_t j=0; j<w; j++)
	{
		uint16_t y1_1,y1_2,u1,v1,y2_1,y2_2,u2,v2,y1,y2,u,v;

		u1=src1[j].u;
		y1_1=src1[j].y1;
		v1=src1[j].v;
		y1_2=src1[j].y2;
		u2=src2[j].u;
		y2_1=src2[j].y1;
		v2=src2[j].v;
		y2_2=src2[j].y2;
		y1=(y1_1+y2_1) >> 1;
		u=(u1+u2) >> 1;
		y2=(y1_2+y2_2) >> 1;
		v=(v1+v2) >> 1;
		dst1[j].u=(uint8_t)u;
		dst1[j].y1=(uint8_t)y1;
		dst1[j].v=(uint8_t)v;
		dst1[j].y2=(uint8_t)y2;
	}
	memcpy(dst2,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Blend_UYVY_Y(const void *src1_0,const void *src2_0,void *dst_0,int32_t w,int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3;
	UYVY *dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	src3=(UYVY *)((uint8_t *)src1+ (src_pitch << 1));

	dst1=(UYVY *)dst_0;
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y1_1,y1_2,y2_1,y2_2,y3_1,y3_2;
			uint16_t y1,y2;
			uint8_t u1,v1,u2,v2;

			u1=src1[j].u;
			y1_1=src1[j].y1;
			v1=src1[j].v;
			y1_2=src1[j].y2;
			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			y3_1=src3[j].y1;
			y3_2=src3[j].y2;
			y1=(y1_1+y2_1) >> 1;
			y2=(y1_2+y2_2) >> 1;
			dst1[j].u=u1;
			dst1[j].y1=(uint8_t)y1;
			dst1[j].v=v1;
			dst1[j].y2=(uint8_t)y2;
			y1=(y2_1+y3_1) >> 1;
			y2=(y2_2+y3_2) >> 1;
			dst2[j].u=u2;
			dst2[j].y1=(uint8_t)y1;
			dst2[j].v=v2;
			dst2[j].y2=(uint8_t)y2;
		}
		src1=(UYVY *)((uint8_t *)src1+(src_pitch << 1));
		src2=(UYVY *)((uint8_t *)src2+(src_pitch << 1));
		src3=(UYVY *)((uint8_t *)src3+(src_pitch << 1));
		dst1=(UYVY *)((uint8_t *)dst1+(dst_pitch << 1));
		dst2=(UYVY *)((uint8_t *)dst2+(dst_pitch << 1));
	}

	for (int32_t j=0; j<w; j++)
	{
		uint16_t y1_1,y1_2,y2_1,y2_2,y1,y2;
		uint8_t u1,v1;

		u1=src1[j].u;
		y1_1=src1[j].y1;
		v1=src1[j].v;
		y1_2=src1[j].y2;
		y2_1=src2[j].y1;
		y2_2=src2[j].y2;
		y1=(y1_1+y2_1) >> 1;
		y2=(y1_2+y2_2) >> 1;
		dst1[j].u=u1;
		dst1[j].y1=(uint8_t)y1;
		dst1[j].v=v1;
		dst1[j].y2=(uint8_t)y2;
	}
	memcpy(dst2,src2,w*4);

}


void JPSDR_IVTC::Deinterlace_Blend_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src3;
	uint8_t *dst1,*dst2;

	src3=src1+(src_pitch << 1);

	dst1=dst;
	dst2=dst1+dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t a1,a2,a3;

			a1=src1[j];
			a2=src2[j];
			a3=src3[j];
			dst1[j]=(uint8_t)((a1+a2)>>1);
			dst2[j]=(uint8_t)((a2+a3)>>1);
		}
		src1+=(src_pitch << 1);
		src2+=(src_pitch << 1);
		src3+=(src_pitch << 1);
		dst1+=(dst_pitch << 1);
		dst2+=(dst_pitch << 1);
	}

	for (int32_t j=0; j<w; j++)
	{
		uint16_t a1,a2;

		a1=src1[j];
		a2=src2[j];
		dst1[j]=(uint8_t)((a1+a2)>>1);
	}

	memcpy(dst2,src2,w);
}



void JPSDR_IVTC::Deinterlace_RGB32(const void *src1,const void *src2,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data)
{
	h--;
	if (tri_linear)
	{
		if (SSE2_Enable && ((w&0x01)==0))
		{
			JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,w>>1,h,src_pitch<<1,dst_pitch);
		}
		else
			if (MMX_Enable)
				JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,w,h,src_pitch<<1,dst_pitch);
			else
				Deinterlace_Tri_Blend_RGB32(src1,src2,dst,w,h,src_pitch,dst_pitch);
	}
	else
	{
		if (SSE2_Enable && ((w&0x03)==0) && align_data)
		{
				JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,w>>2,h,src_pitch<<1,dst_pitch);
		}
		else
		{
			if (Integer_SSE_Enable)
			{
				if (w&0x01)
					JPSDR_IVTC_Deinterlace_Blend_SSE(src1,src2,dst,w,h,src_pitch<<1,dst_pitch);
				else
					JPSDR_IVTC_Deinterlace_Blend_SSE_2(src1,src2,dst,w>>1,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32(src1,src2,dst,w,h,src_pitch<<1,dst_pitch);
			}
		}
	}
}




void JPSDR_IVTC::Deinterlace_YUYV(const void *src1,const void *src2,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data,bool Y_only)
{
	h--;

	if (Y_only)
	{
		if (tri_linear) Deinterlace_Tri_Blend_YUYV_Y(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
		else Deinterlace_Blend_YUYV_Y(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);

		return;
	}

	if (tri_linear)
	{
		if (SSE2_Enable && ((w&0x03)==0))
		{
			JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,((w+1)>>1)>>1,h,src_pitch<<1,dst_pitch);
		}
		else
			if (MMX_Enable)
				JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,(w+1)>>1,h,src_pitch<<1,dst_pitch);
			else
				Deinterlace_Tri_Blend_YUYV(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
	}
	else
	{
		if (SSE2_Enable && ((w&0x07)==0) && align_data)
		{
				JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,((w+1)>>1)>>2,h,src_pitch<<1,dst_pitch);
		}
		else
		{
			if (Integer_SSE_Enable)
			{
				if (w&0x03)
					JPSDR_IVTC_Deinterlace_Blend_SSE(src1,src2,dst,(w+1)>>1,h,src_pitch<<1,dst_pitch);
				else
					JPSDR_IVTC_Deinterlace_Blend_SSE_2(src1,src2,dst,((w+1)>>1)>>1,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				Deinterlace_Blend_YUYV(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
			}
		}
	}
}



void JPSDR_IVTC::Deinterlace_UYVY(const void *src1,const void *src2,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data, bool Y_only)
{
	h--;

	if (Y_only)
	{
		if (tri_linear) Deinterlace_Tri_Blend_UYVY_Y(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
		else Deinterlace_Blend_UYVY_Y(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);

		return;
	}

	if (tri_linear)
	{
		if (SSE2_Enable && ((w&0x03)==0))
		{
			JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,((w+1)>>1)>>1,h,src_pitch<<1,dst_pitch);
		}
		else
			if (MMX_Enable)
				JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,(w+1)>>1,h,src_pitch<<1,dst_pitch);
			else
				Deinterlace_Tri_Blend_UYVY(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
	}
	else
	{
		if (SSE2_Enable && ((w&0x07)==0) && align_data)
		{
				JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,((w+1)>>1)>>2,h,src_pitch<<1,dst_pitch);
		}
		else
		{
			if (Integer_SSE_Enable)
			{
				if (w&0x03)
					JPSDR_IVTC_Deinterlace_Blend_SSE(src1,src2,dst,(w+1)>>1,h,src_pitch<<1,dst_pitch);
				else
					JPSDR_IVTC_Deinterlace_Blend_SSE_2(src1,src2,dst,((w+1)>>1)>>1,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				Deinterlace_Blend_UYVY(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
			}
		}
	}
}



void JPSDR_IVTC::Deinterlace_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data)
{
	h--;
	if (tri_linear)
	{
		if (SSE2_Enable && ((w&0x07)==0))
		{
			JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,w>>3,h,src_pitch<<1,dst_pitch);
		}
		else
		{
			if ((MMX_Enable) && ((w&0x03)==0))
				JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,w>>2,h,src_pitch<<1,dst_pitch);
			else
				Deinterlace_Tri_Blend_Planar(src1,src2,dst,w,h,src_pitch,dst_pitch);
		}
	}
	else
	{
		if (SSE2_Enable && ((w&0x0F)==0) && align_data)
		{
				JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,w>>4,h,src_pitch<<1,dst_pitch);
		}
		else
		{
			if ((Integer_SSE_Enable) && ((w&0x03)==0))
			{
				if (w&0x07)
					JPSDR_IVTC_Deinterlace_Blend_SSE(src1,src2,dst,w>>2,h,src_pitch<<1,dst_pitch);
				else
					JPSDR_IVTC_Deinterlace_Blend_SSE_2(src1,src2,dst,w>>3,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				Deinterlace_Blend_Planar(src1,src2,dst,w,h,src_pitch,dst_pitch);
			}
		}
	}
}



void JPSDR_IVTC::Motion_Map_RGB32(const void *src1_0,const void *src2_0,void *buffer_0,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs)
{
	int32_t delta_map;
	uint16_t thr;
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer;
	uint8_t *dst1,*dst2;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	buffer=(RGB32BMP *)buffer_0;
	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
		*dst1++=0;
	dst1+=delta_map;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+(pitch<<1));
	src4=(RGB32BMP *)((uint8_t *)src2+(pitch<<1));
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1->b;
			g1=src1->g;
			r1=src1->r;
			b2=src2->b;
			g2=src2->g;
			r2=src2->r;
			b3=src3->b;
			g3=src3->g;
			r3=src3->r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			buffer->b=(uint8_t)(b&0xFC);
			buffer->g=(uint8_t)(g&0xFC);
			buffer->r=(uint8_t)(r&0xFC);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) *dst1++=1;
			else *dst1++=0;

			b1=src4->b;
			g1=src4->g;
			r1=src4->r;
			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			buffer[w].b=(uint8_t)(b&0xFC);
			buffer[w].g=(uint8_t)(g&0xFC);
			buffer[w].r=(uint8_t)(r&0xFC);
			if ( (!((g3>g1)^(g3>g2)) && ((g>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((r>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((b>thr) && (abs(b3-b1)>thr))) ) *dst2++=1;
			else *dst2++=0;
			src1++;
			src2++;
			src3++;
			src4++;
			buffer++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+(modulo+pitch));
		src2=(RGB32BMP *)((uint8_t *)src2+(modulo+pitch));
		src3=(RGB32BMP *)((uint8_t *)src3+(modulo+pitch));
		src4=(RGB32BMP *)((uint8_t *)src4+(modulo+pitch));
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
		buffer+=w;
	}

	for (int32_t i=0; i<w; i++)
	{
		int16_t r,r1,r2,g,g1,g2,b,b1,b2;

		b1=src1->b;
		g1=src1->g;
		r1=src1->r;
		b2=src2->b;
		g2=src2->g;
		r2=src2->r;
		b=(int16_t)abs(b2-b1)&0xFC;
		g=(int16_t)abs(g2-g1)&0xFC;
		r=(int16_t)abs(r2-r1)&0xFC;
		buffer->b=(uint8_t)b;
		buffer->g=(uint8_t)g;
		buffer->r=(uint8_t)r;
		buffer[w]=*buffer;
		*dst1++=0;
		buffer++;
		src1++;
		src2++;
	}
}



void JPSDR_IVTC::Motion_Map_RGB32(const void *src1_0,const void *src2_0,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs)
{
	int32_t delta_map;
	int16_t thr;
	const RGB32BMP *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
		*dst1++=0;
	dst1+=delta_map;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+(pitch<<1));
	src4=(RGB32BMP *)((uint8_t *)src2+(pitch<<1));
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1->b;
			g1=src1->g;
			r1=src1->r;
			b2=src2->b;
			g2=src2->g;
			r2=src2->r;
			b3=src3->b;
			g3=src3->g;
			r3=src3->r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) *dst1++=1;
			else *dst1++=0;

			b1=src4->b;
			g1=src4->g;
			r1=src4->r;
			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			if ( (!((g3>g1)^(g3>g2)) && ((g>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((r>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((b>thr) && (abs(b3-b1)>thr))) ) *dst2++=1;
			else *dst2++=0;
			src1++;
			src2++;
			src3++;
			src4++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+(modulo+pitch));
		src2=(RGB32BMP *)((uint8_t *)src2+(modulo+pitch));
		src3=(RGB32BMP *)((uint8_t *)src3+(modulo+pitch));
		src4=(RGB32BMP *)((uint8_t *)src4+(modulo+pitch));
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
	}

	for (int32_t i=0; i<w; i++)
	{
		*dst1++=0;
	}
}


void JPSDR_IVTC::Motion_Map_YUYV_Y(const void *src1_0,const void *src2_0,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs)
{
	int32_t delta_map;
	int16_t thr;
	const YUYV *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
		*dst1++=0;
	dst1+=delta_map;

	dst2=dst1+w_map;
	src3=(YUYV *)((uint8_t *)src1+(pitch<<1));
	src4=(YUYV *)((uint8_t *)src2+(pitch<<1));
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j+=2)
		{
			int16_t y,y1,y2,y3;

			y1=src1->y1;
			y2=src2->y1;
			y3=src3->y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) *dst1++=1;
			else *dst1++=0;

			y1=src4->y1;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) *dst2++=1;
			else *dst2++=0;

			y1=src1->y2;
			y2=src2->y2;
			y3=src3->y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) *dst1++=1;
			else *dst1++=0;

			y1=src4->y2;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) *dst2++=1;
			else *dst2++=0;

			src1++;
			src2++;
			src3++;
			src4++;
		}
		src1=(YUYV *)((uint8_t *)src1+(modulo+pitch));
		src2=(YUYV *)((uint8_t *)src2+(modulo+pitch));
		src3=(YUYV *)((uint8_t *)src3+(modulo+pitch));
		src4=(YUYV *)((uint8_t *)src4+(modulo+pitch));
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
	}

	for (int32_t i=0; i<w; i++)
	{
		*dst1++=0;
	}
}



void JPSDR_IVTC::Motion_Map_UYVY_Y(const void *src1_0,const void *src2_0,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs)
{
	int32_t delta_map;
	int16_t thr;
	const UYVY *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
		*dst1++=0;
	dst1+=delta_map;

	dst2=dst1+w_map;
	src3=(UYVY *)((uint8_t *)src1+(pitch<<1));
	src4=(UYVY *)((uint8_t *)src2+(pitch<<1));
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j+=2)
		{
			int16_t y,y1,y2,y3;

			y1=src1->y1;
			y2=src2->y1;
			y3=src3->y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) *dst1++=1;
			else *dst1++=0;

			y1=src4->y1;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) *dst2++=1;
			else *dst2++=0;

			y1=src1->y2;
			y2=src2->y2;
			y3=src3->y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) *dst1++=1;
			else *dst1++=0;

			y1=src4->y2;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) *dst2++=1;
			else *dst2++=0;

			src1++;
			src2++;
			src3++;
			src4++;
		}
		src1=(UYVY *)((uint8_t *)src1+(modulo+pitch));
		src2=(UYVY *)((uint8_t *)src2+(modulo+pitch));
		src3=(UYVY *)((uint8_t *)src3+(modulo+pitch));
		src4=(UYVY *)((uint8_t *)src4+(modulo+pitch));
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
	}

	for (int32_t i=0; i<w; i++)
	{
		*dst1++=0;
	}
}


void JPSDR_IVTC::Motion_Map_Planar420_Y(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thrs)
{
	int32_t delta_map;
	int16_t thr;
	const uint8_t*src3,*src4;
	uint8_t *dst1,*dst2;

	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
		*dst1++=0;
	dst1+=delta_map;

	dst2=dst1+w_map;
	src3=src1+(pitch<<1);
	src4=src2+(pitch<<1);
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,y1,y2,y3;

			y1=*src1++;
			y2=*src2++;
			y3=*src3++;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) *dst1++=1;
			else *dst1++=0;

			y1=*src4++;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) *dst2++=1;
			else *dst2++=0;
		}
		src1+=(modulo+pitch);
		src2+=(modulo+pitch);
		src3+=(modulo+pitch);
		src4+=(modulo+pitch);
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
	}

	for (int32_t i=0; i<w; i++)
	{
		*dst1++=0;
	}
}



void JPSDR_IVTC::Smart_Deinterlace_Motion_Map_RGB32(const void *src1_0,const void *src2_0,void *buffer_0,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,
		uint8_t thrs)
{
	int32_t delta_map;
	int16_t thr;
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer;
	uint8_t *dst1,*dst2;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	buffer=(RGB32BMP *)buffer_0;

	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
	{
		int16_t r1,r2,g1,g2,b1,b2;

		b1=src1[i].b;
		g1=src1[i].g;
		r1=src1[i].r;
		b2=src2[i].b;
		g2=src2[i].g;
		r2=src2[i].r;
		buffer[i].b=(uint8_t)((b1+b2)>>1);
		buffer[i].g=(uint8_t)((g1+g2)>>1);
		buffer[i].r=(uint8_t)((r1+r2)>>1);
		*dst1++=0;
	}
	dst1+=delta_map;
	buffer+=w;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+(pitch<<1));
	src4=(RGB32BMP *)((uint8_t *)src2+(pitch<<1));
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[i].b;
			g1=src1[i].g;
			r1=src1[i].r;
			b2=src2[i].b;
			g2=src2[i].g;
			r2=src2[i].r;
			b3=src3[i].b;
			g3=src3[i].g;
			r3=src3[i].r;
			buffer->b=(uint8_t)((b3+b2)>>1);
			buffer->g=(uint8_t)((g3+g2)>>1);
			buffer->r=(uint8_t)((r3+r2)>>1);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) *dst1++=1;
			else *dst1++=0;
			b1=src4[i].b;
			g1=src4[i].g;
			r1=src4[i].r;
			buffer[w].b=(uint8_t)((b1+b3)>>1);
			buffer[w].g=(uint8_t)((g1+g3)>>1);
			buffer[w].r=(uint8_t)((r1+r3)>>1);
			if ( (!((g3>g1)^(g3>g2)) && ((abs(g3-g2)>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((abs(r3-r2)>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((abs(b3-b2)>thr) && (abs(b3-b1)>thr))) ) *dst2++=1;
			else *dst2++=0;
			src1++;
			src2++;
			src3++;
			src4++;
			buffer++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+(modulo+pitch));
		src2=(RGB32BMP *)((uint8_t *)src2+(modulo+pitch));
		src3=(RGB32BMP *)((uint8_t *)src3+(modulo+pitch));
		src4=(RGB32BMP *)((uint8_t *)src4+(modulo+pitch));
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
		buffer+=w;
	}

	for (int32_t i=0; i<w; i++)
	{
		*buffer++=*src2++;
		*dst1++=0;
	}

}



void JPSDR_IVTC::Smart_Deinterlace_Tri_Motion_Map_RGB32(const void *src1_0,const void *src2_0,void *buffer_0,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,
		uint8_t thrs)
{
	int32_t delta_map;
	int16_t thr;
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer;
	uint8_t *dst1,*dst2;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	buffer=(RGB32BMP *)buffer_0;

	dst1=dst;
	thr=thrs;
	delta_map=w_map-w;

	for (int32_t i=0; i<w; i++)
	{
		*buffer++=src1[i];
		*dst1++=0;
	}
	dst1+=delta_map;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+(pitch<<1));
	src4=(RGB32BMP *)((uint8_t *)src2+(pitch<<1));
	h--;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[i].b;
			g1=src1[i].g;
			r1=src1[i].r;
			b2=src2[i].b;
			g2=src2[i].g;
			r2=src2[i].r;
			b3=src3[i].b;
			g3=src3[i].g;
			r3=src3[i].r;
			buffer->b=(uint8_t)(((b1+b3)+(b2 << 1)+2)>>2);
			buffer->g=(uint8_t)(((g1+g3)+(g2 << 1)+2)>>2);
			buffer->r=(uint8_t)(((r1+r3)+(r2 << 1)+2)>>2);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) *dst1++=1;
			else *dst1++=0;
			b1=src4[i].b;
			g1=src4[i].g;
			r1=src4[i].r;
			buffer[w].b=(uint8_t)(((b1+b2)+(b3 << 1)+2)>>2);
			buffer[w].g=(uint8_t)(((g1+g2)+(g3 << 1)+2)>>2);
			buffer[w].r=(uint8_t)(((r1+r2)+(r3 << 1)+2)>>2);
			if ( (!((g3>g1)^(g3>g2)) && ((abs(g3-g2)>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((abs(r3-r2)>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((abs(b3-b2)>thr) && (abs(b3-b1)>thr))) ) *dst2++=1;
			else *dst2++=0;
			src1++;
			src2++;
			src3++;
			src4++;
			buffer++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+(modulo+pitch));
		src2=(RGB32BMP *)((uint8_t *)src2+(modulo+pitch));
		src3=(RGB32BMP *)((uint8_t *)src3+(modulo+pitch));
		src4=(RGB32BMP *)((uint8_t *)src4+(modulo+pitch));
		dst1+=(w_map+delta_map);
		dst2+=(w_map+delta_map);
		buffer+=w;
	}

	for (int32_t i=0; i<w; i++)
	{
		*buffer++=*src2++;
		*dst1++=0;
	}

}




void JPSDR_IVTC::Motion_Map_Filter(uint8_t *map,uint8_t *buffer,int32_t w,
	int32_t h,int32_t w_map)
{
	int32_t delta,h_4,w_2;
	uint8_t s,*tab;
	uint8_t *dst,*src;
	uint32_t *tab_2,w_map_4;

	w_map_4=w_map >> 2;
	h_4=h-4;
	w_2=w-2;

	tab=map;
	dst=buffer;
	for (int32_t i=0; i<w; i++)
	{
		dst[i]=0;
		dst[i+w_map]=0;
		dst[i+(h-2)*w_map]=0;
		dst[i+(h-1)*w_map]=0;
	}
	dst+=(w_map<<1);
	src=map+(w_map<<1);
	for (int32_t i=0; i<h_4; i++)
	{
		for (int32_t j=0; j<2; j++)
		{
			dst[j]=0;
			dst[w-1-j]=0;
		}
		for (int32_t j=2; j<w_2; j++)
		{
			if (src[j])
			{
				int32_t j_m2,j_p2;

				j_m2=j-2;
				j_p2=j+2;
				s=0;
				delta=0;
				for (int32_t k=0; k<5; k++)
				{
					for (int32_t l=j_m2; l<=j_p2; l++)
						s+=tab[delta+l];
					delta+=w_map;
				}
				if (s<=9) dst[j]=0;
				else dst[j]=1;
			}
			else dst[j]=0;
		}
		src+=w_map;
		dst+=w_map;
		tab+=w_map;
	}

	tab_2=(uint32_t *)map;
	delta=h*w_map_4;
	for (int32_t i=0; i<delta; i++)
		*tab_2++=0;
	tab=map;
	src=buffer+(w_map<<1);
	for (int32_t i=0; i<h_4; i++)
	{
		for (int32_t j=2; j<w_2; j++)
		{
			if (src[j])
			{
				tab_2=(uint32_t *)(tab+j-1);
				for (int32_t k=0; k<5; k++)
				{
					const uint32_t mask=0x01010101;

					*tab_2=mask;
					tab_2+=w_map_4;
				}
			}
		}
		tab+=w_map;
		src+=w_map;
	}
}



uint32_t JPSDR_IVTC::Norme1_Histogramme_DeltaPicture_RGB32(const void *src1,const void *src2,
		void *dst_0,void *buffer_0,uint8_t *map,uint8_t *buffer_map,
		uint32_t *histo,uint32_t *repart,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,
		uint32_t &error_motion_map,uint8_t thrs)
{
	int32_t w_map,delta_map;
	RGB32BMP *dst,*dst2,*buffer,zero_RGB;
	uint32_t s,thr,s_motion_map;

	dst=(RGB32BMP *)dst_0;
	buffer=(RGB32BMP *)buffer_0;
	zero_RGB.r=0;
	zero_RGB.g=0;
	zero_RGB.b=0;
	zero_RGB.alpha=0;

	if ((w%4)==0) w_map=w+4;
	else w_map=(((w >> 2)+1)<< 2)+4;
	delta_map=w_map-w;

	if (Integer_SSE_Enable)
	{
		thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;
		JPSDR_IVTC_Motion_Map_SSE_RGB32(src1,src2,buffer,map,w,h-1,src_pitch,src_modulo,thr,w_map);
	}
	else
		Motion_Map_RGB32(src1,src2,buffer,map,w,h,src_pitch,src_modulo,w_map,thrs);

	Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	if (Integer_SSE_Enable)
			s=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32(buffer,map,dst,histo,repart,w,
				h,dst_pitch,dst_modulo,w_map,s_motion_map);
	else
	{
		for (int32_t i=0; i<256; i++)
			histo[i]=0;
		s_motion_map=0;
		s=0;
		dst2=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				uint16_t r,g,b;
				uint16_t s_int;

				b=buffer->b;
				g=buffer->g;
				r=buffer->r;
				histo[r]++;
				histo[g]++;
				histo[b]++;
				s_int=r+g+b;
				s+=s_int;
				if (*map)
				{
					s_motion_map+=s_int;
					*dst++=*buffer;
				}
				else *dst++=zero_RGB;
				b=buffer[w].b;
				g=buffer[w].g;
				r=buffer[w].r;
				histo[r]++;
				histo[g]++;
				histo[b]++;
				if (map[w_map])
				{
					s_motion_map+=(r+g+b);
					*dst2++=buffer[w];
				}
				else *dst2++=zero_RGB;
				buffer++;
				map++;
			}
			map+=(w_map+delta_map);
			buffer+=w;
			dst=(RGB32BMP *)((uint8_t *)dst+(dst_modulo+dst_pitch));
			dst2=(RGB32BMP *)((uint8_t *)dst2+(dst_modulo+dst_pitch));
		}

		repart[255]=histo[255];
		for (int32_t i=254; i>=0; i--)
			repart[i]=histo[i]+repart[i+1];
	}

	error_motion_map=s_motion_map;

	return(s);

}




void JPSDR_IVTC::Smart_Deinterlace_RGB32(const void *src1_0,const void *src2_0,void *dst_0,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thrs,
		bool tri_linear,bool map_filter)
{
	int32_t w_map,delta_map;
	const RGB32BMP *src1,*src2;
	RGB32BMP *dst,*dst2,*buffer;
	uint32_t thr;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	dst=(RGB32BMP *)dst_0;
	buffer=(RGB32BMP *)buffer_0;

	if ((w%4)==0) w_map=w+4;
	else w_map=(((w >> 2)+1)<< 2)+4;
	delta_map=w_map-w;

	if (Integer_SSE_Enable)
	{
		thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;
		if (tri_linear)
			JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32(src1,src2,buffer,map,w,h-1,src_pitch,
				src_modulo,thr,w_map);
		else
			JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32(src1,src2,buffer,map,w,h-1,src_pitch,
				src_modulo,thr,w_map);
	}
	else
	{
		if (tri_linear)
			Smart_Deinterlace_Tri_Motion_Map_RGB32(src1,src2,buffer,map,w,h,src_pitch,src_modulo,
				w_map,thrs);
		else
			Smart_Deinterlace_Motion_Map_RGB32(src1,src2,buffer,map,w,h,src_pitch,src_modulo,
				w_map,thrs);
	}

	if (map_filter) Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	dst2=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			if (*map) *dst++=*buffer;
			else *dst++=*src1;
			if (map[w_map]) *dst2++=buffer[w];
			else *dst2++=*src2;
			buffer++;
			map++;
			src1++;
			src2++;
		}
		map+=(w_map+delta_map);
		buffer+=w;
		dst=(RGB32BMP *)((uint8_t *)dst+(dst_modulo+dst_pitch));
		dst2=(RGB32BMP *)((uint8_t *)dst2+(dst_modulo+dst_pitch));
		src1=(RGB32BMP *)((uint8_t *)src1+(src_modulo+src_pitch));
		src2=(RGB32BMP *)((uint8_t *)src2+(src_modulo+src_pitch));
	}

}



void JPSDR_IVTC::Smart_Deinterlace_YUYV(const void *src1_0,const void *src2_0,void *dst_0,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thrs,
		bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		ptrdiff_t RGBBuff_pitch,bool Y_only)
{
	int32_t w_map,delta_map;
	const YUYV *src1,*src2;
	YUYV *dst,*dst2,*buffer;
	int32_t w_image;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst=(YUYV *)dst_0;
	buffer=(YUYV *)buffer_0;
	w_image=(w+1)>>1;

	if ((w%4)==0) w_map=w+4;
	else w_map=(((w >> 2)+1)<< 2)+4;
	delta_map=w_map-w;

	Deinterlace_YUYV(src1,src2,buffer,w,h,src_pitch,(w_image << 2),tri_linear,true,Y_only);

	if (Y_only)
	{
		Motion_Map_YUYV_Y(src1_0,src2_0,map,w,h,src_pitch,src_modulo,w_map,thrs);
	}
	else
	{
		Motion_Map_RGB32(RGBBuff_1,RGBBuff_2,map,w,h,RGBBuff_pitch,0,w_map,thrs);
	}

	if (map_filter) Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	dst2=(YUYV *)((uint8_t *)dst+dst_pitch);
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w_image; j++)
		{
			if ((*map) || map[1]) *dst++=*buffer;
			else *dst++=*src1;
			if (map[w_map] || map[w_map+1]) *dst2++=buffer[w_image];
			else *dst2++=*src2;
			buffer++;
			map+=2;
			src1++;
			src2++;
		}
		map+=(w_map+delta_map);
		buffer+=w_image;
		dst=(YUYV *)((uint8_t *)dst+(dst_modulo+dst_pitch));
		dst2=(YUYV *)((uint8_t *)dst2+(dst_modulo+dst_pitch));
		src1=(YUYV *)((uint8_t *)src1+(src_modulo+src_pitch));
		src2=(YUYV *)((uint8_t *)src2+(src_modulo+src_pitch));
	}
}



void JPSDR_IVTC::Smart_Deinterlace_UYVY(const void *src1_0,const void *src2_0,void *dst_0,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thrs,
		bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		ptrdiff_t RGBBuff_pitch,bool Y_only)
{
	int32_t w_map,delta_map;
	const UYVY *src1,*src2;
	UYVY *dst,*dst2,*buffer;
	int32_t w_image;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst=(UYVY *)dst_0;
	buffer=(UYVY *)buffer_0;
	w_image=(w+1)>>1;

	if ((w%4)==0) w_map=w+4;
	else w_map=(((w >> 2)+1)<< 2)+4;
	delta_map=w_map-w;

	Deinterlace_UYVY(src1,src2,buffer,w,h,src_pitch,(w_image << 2),tri_linear,true,Y_only);

	if (Y_only)
	{
		Motion_Map_UYVY_Y(src1_0,src2_0,map,w,h,src_pitch,src_modulo,w_map,thrs);
	}
	else
	{
		Motion_Map_RGB32(RGBBuff_1,RGBBuff_2,map,w,h,RGBBuff_pitch,0,w_map,thrs);
	}

	if (map_filter) Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	dst2=(UYVY *)((uint8_t *)dst+dst_pitch);
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w_image; j++)
		{
			if ((*map) || map[1]) *dst++=*buffer;
			else *dst++=*src1;
			if (map[w_map] || map[w_map+1]) *dst2++=buffer[w_image];
			else *dst2++=*src2;
			buffer++;
			map+=2;
			src1++;
			src2++;
		}
		map+=(w_map+delta_map);
		buffer+=w_image;
		dst=(UYVY *)((uint8_t *)dst+(dst_modulo+dst_pitch));
		dst2=(UYVY *)((uint8_t *)dst2+(dst_modulo+dst_pitch));
		src1=(UYVY *)((uint8_t *)src1+(src_modulo+src_pitch));
		src2=(UYVY *)((uint8_t *)src2+(src_modulo+src_pitch));
	}

}


void JPSDR_IVTC::Smart_Deinterlace_Planar420(const uint8_t *src1_Y,const uint8_t *src2_Y,
	const uint8_t *src1_U,const uint8_t *src2_U,const uint8_t *src1_V,const uint8_t *src2_V,
	uint8_t *dst_Y,uint8_t *dst_U,uint8_t *dst_V,uint8_t *buffer_Y,uint8_t *buffer_U,
	uint8_t *buffer_V,uint8_t *map,uint8_t *buffer_map,int32_t w_Y,int32_t w_U,int32_t w_V,int32_t h_Y,
	int32_t h_U,int32_t h_V,ptrdiff_t src_pitch_Y,ptrdiff_t src_pitch_U,ptrdiff_t src_pitch_V,ptrdiff_t src_modulo_Y,
	ptrdiff_t src_modulo_U,ptrdiff_t src_modulo_V,ptrdiff_t dst_pitch_Y,ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,
	ptrdiff_t dst_modulo_Y,ptrdiff_t dst_modulo_U,ptrdiff_t dst_modulo_V,uint8_t thrs,bool tri_linear,
	bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,ptrdiff_t RGBBuff_pitch,bool Y_only)
{
	int32_t w_map,delta_map,w_map2,w_map3;
	uint8_t *dst2;
	uint8_t *map_filtre;

	if ((w_Y%4)==0) w_map=w_Y+4;
	else w_map=(((w_Y >> 2)+1) << 2)+4;
	delta_map=w_map-w_Y;
	w_map2=w_map << 1;
	w_map3=3*w_map;

	Deinterlace_Planar(src1_Y,src2_Y,buffer_Y,w_Y,h_Y,src_pitch_Y,w_Y,tri_linear,true);
	if (!Y_only)
	{
		Deinterlace_Planar(src1_U,src2_U,buffer_U,w_U,h_U,src_pitch_U,w_U,tri_linear,true);
		Deinterlace_Planar(src1_V,src2_V,buffer_V,w_V,h_V,src_pitch_V,w_V,tri_linear,true);
	}

	if (Y_only)
	{
		Motion_Map_Planar420_Y(src1_Y,src2_Y,map,w_Y,h_Y,src_pitch_Y,src_modulo_Y,w_map,thrs);
	}
	else
	{
		Motion_Map_RGB32(RGBBuff_1,RGBBuff_2,map,w_Y,h_Y,RGBBuff_pitch,0,w_map,thrs);
	}

	if (map_filter) Motion_Map_Filter(map,buffer_map,w_Y,h_Y<<1,w_map);

	dst2=dst_Y+dst_pitch_Y;
	map_filtre=map;
	for (int32_t i=0; i<h_Y; i++)
	{
		for (int32_t j=0; j<w_Y; j++)
		{
			if (*map_filtre) *dst_Y++=*buffer_Y;
			else *dst_Y++=*src1_Y;
			if (map_filtre[w_map]) *dst2++=buffer_Y[w_Y];
			else *dst2++=*src2_Y;
			buffer_Y++;
			map_filtre++;
			src1_Y++;
			src2_Y++;
		}
		map_filtre+=(w_map+delta_map);
		buffer_Y+=w_Y;
		dst_Y+=(dst_modulo_Y+dst_pitch_Y);
		dst2+=(dst_modulo_Y+dst_pitch_Y);
		src1_Y+=(src_modulo_Y+src_pitch_Y);
		src2_Y+=(src_modulo_Y+src_pitch_Y);
	}

	if (!Y_only)
	{
		dst2=dst_U+dst_pitch_U;
		map_filtre=map;
		for (int32_t i=0; i<h_U; i++)
		{
			for (int32_t j=0; j<w_U; j++)
			{
				if ((*map_filtre)||(map_filtre[1])||(map_filtre[w_map2])||(map_filtre[w_map2+1])) *dst_U++=*buffer_U;
				else *dst_U++=*src1_U;
				if ((map_filtre[w_map])||(map_filtre[1+w_map])||(map_filtre[w_map2+w_map])
					||(map_filtre[w_map2+w_map+1])) *dst2++=buffer_U[w_U];
				else *dst2++=*src2_U;
				buffer_U++;
				map_filtre+=2;
				src1_U++;
				src2_U++;
			}
			map_filtre+=(w_map3+delta_map);
			buffer_U+=w_U;
			dst_U+=(dst_modulo_U+dst_pitch_U);
			dst2+=(dst_modulo_U+dst_pitch_U);
			src1_U+=(src_modulo_U+src_pitch_U);
			src2_U+=(src_modulo_U+src_pitch_U);
		}

		dst2=dst_V+dst_pitch_V;
		map_filtre=map;
		for (int32_t i=0; i<h_V; i++)
		{
			for (int32_t j=0; j<w_V; j++)
			{
				if ((*map_filtre)||(map_filtre[1])||(map_filtre[w_map2])||(map_filtre[w_map2+1])) *dst_V++=*buffer_V;
				else *dst_V++=*src1_V;
				if ((map_filtre[w_map])||(map_filtre[1+w_map])||(map_filtre[w_map2+w_map])
					||(map_filtre[w_map2+w_map+1])) *dst2++=buffer_V[w_V];
				else *dst2++=*src2_V;
				buffer_V++;
				map_filtre+=2;
				src1_V++;
				src2_V++;
			}
			map_filtre+=(w_map3+delta_map);
			buffer_V+=w_V;
			dst_V+=(dst_modulo_V+dst_pitch_V);
			dst2+=(dst_modulo_V+dst_pitch_V);
			src1_V+=(src_modulo_V+src_pitch_V);
			src2_V+=(src_modulo_V+src_pitch_V);
		}
	}
	else
	{
		dst2=dst_U+dst_pitch_U;
		for (int32_t i=0; i<h_U; i++)
		{
			memcpy(dst_U,src1_U,w_U);
			memcpy(dst2,src2_U,w_U);
			dst_U+=(dst_pitch_U << 1);
			dst2+=(dst_pitch_U << 1);
			src1_U+=(src_pitch_U << 1);
			src2_U+=(src_pitch_U << 1);
		}

		dst2=dst_V+dst_pitch_V;
		for (int32_t i=0; i<h_V; i++)
		{
			memcpy(dst_V,src1_V,w_V);
			memcpy(dst2,src2_V,w_V);
			dst_V+=(dst_pitch_V << 1);
			dst2+=(dst_pitch_V << 1);
			src1_V+=(src_pitch_V << 1);
			src2_V+=(src_pitch_V << 1);
		}
	}

}




uint32_t JPSDR_IVTC::Norme1_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t pitch,
		ptrdiff_t modulo,uint32_t &error_motion_map,uint8_t thrs)

{
	int32_t w_map,delta_map;
	uint32_t s,thr,s_motion_map;
	RGB32BMP *buffer;

	buffer=(RGB32BMP *)buffer_0;

	if ((w%4)==0) w_map=w+4;
	else w_map=(((w >> 2)+1)<< 2)+4;
	delta_map=w_map-w;

	if (Integer_SSE_Enable)
	{
		thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;
		JPSDR_IVTC_Motion_Map_SSE_RGB32(src1,src2,buffer,map,w,h-1,pitch,modulo,thr,w_map);
	}
	else
		Motion_Map_RGB32(src1,src2,buffer,map,w,h,pitch,modulo,w_map,thrs);

	Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	if (Integer_SSE_Enable)
		s=JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32(buffer,map,w,h,w_map,s_motion_map);
	else
	{
		s=0;
		s_motion_map=0;
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				uint16_t r,g,b;
				uint32_t s_int;

				b=buffer->b;
				g=buffer->g;
				r=buffer->r;
				s_int=r+g+b;
				s+=s_int;
				if (*map) s_motion_map+=s_int;
				if (map[w_map])
				{
					b=buffer[w].b;
					g=buffer[w].g;
					r=buffer[w].r;
					s_motion_map+=(r+g+b);
				}
				buffer++;
				map++;
			}
			map+=(w_map+delta_map);
			buffer+=w;
		}
	}

	error_motion_map=s_motion_map;

	return(s);
}



uint32_t JPSDR_IVTC::Norme1_RGB32(const void *src1_0,const void *src2_0,int32_t w,int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo)
{
	uint32_t s;
	const RGB32BMP *src1,*src2;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;

	s=0;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,g,g1,g2,b,b1,b2;

			b1=src1->b;
			g1=src1->g;
			r1=src1->r;
			b2=src2->b;
			g2=src2->g;
			r2=src2->r;
			r=abs(r1-r2)&0xFC;				// Noise removal
			g=abs(g1-g2)&0xFC;
			b=abs(b1-b2)&0xFC;
			s+=(r+g+b);
			src1++;
			src2++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1 + (modulo+pitch));
		src2=(RGB32BMP *)((uint8_t *)src2 + (modulo+pitch));
	}

	return(s);
}


void JPSDR_IVTC::Run()
{
	const VDXPixmap *tab_pxsrc[8];

	const VDXPixmap& pxdst = *fa->mpOutputFrames[0]->mpPixmap;
	const VDXFBitmap& pxsrc = *fa->mpSourceFrames[0];
	
	Image_Data idata;
	const void *src1,*src2;
	const void *src1_Y,*src2_Y,*src1_U,*src2_U,*src1_V,*src2_V;
	const void *src1_RGB,*src2_RGB;
	void *dst1_Y,*dst1_U,*dst1_V,*dst2_Y,*dst2_U,*dst2_V;
	int32_t h,w,h_removed,h_U,w_U,h_V,w_V,h_Y,w_Y,h_removed_resize;
	ptrdiff_t src_pitch,src_modulo,dst_pitch,dst_modulo,buffer_removed_pitch,frame_removed_pitch;
	ptrdiff_t buffer_resize_removed_pitch;
	ptrdiff_t src_modulo_Y,src_modulo_U,src_modulo_V,src_pitch_Y,src_pitch_U,src_pitch_V;
	ptrdiff_t dst_modulo_Y,dst_modulo_U,dst_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V;
	ptrdiff_t frame_buffer_pitch,buffer_pitch;
	ptrdiff_t frame_buffer_pitch_Y,frame_buffer_pitch_U,frame_buffer_pitch_V;
	ptrdiff_t buffer_pitch_planar420;
	ptrdiff_t pitch1_Y,pitch2_Y,pitch1_U,pitch2_U,pitch1_V,pitch2_V;
	ptrdiff_t modulo1_Y,modulo2_Y,modulo1_U,modulo2_U,modulo1_V,modulo2_V;
	uint8_t i_rd_manual,i_rd,i_rd_m1,i_rd_m2,i_rd_p1,i_rd_p2;
	uint16_t j_manual;
	uint8_t i,j,k,l,i_err,i_err_m1,i_wrt,i_wrt_m1,i_tab,j_tab;
	uint8_t thr_ivtc,thr_flt,thr_dtl,output_mode,deinterlace_mode,thr_out;
	uint8_t old_first,old_last;
	bool filter_output,inv_polar,disable_deinterlace,blend_anime,tri_linear,disable_Z_mode,
		map_filter,Y_only;
	uint32_t histogramme[256];
	bool ivtc_f,prog_f,rep128_f,eg_ivtc_p1_f,eg_ivtc_m1_f,z_dtop_p1_f,z_dbot_m1_f;
	bool z_dbot_p1_f,z_dtop_m1_f;
	bool dtop_m1_f,dbot_m1_f,dtop_p1_f,dbot_p1_f,pic_f,error_f,chg_sc_f;
	bool dtop_p1_m_f,dtop_m1_m_f,dbot_m1_p_f,dbot_p1_p_f;
	bool dbot_p1_m_f,dbot_m1_m_f,dtop_m1_p_f,dtop_p1_p_f;
	bool dtopbot_f,chg_sc_m1_f;
	bool static_f,static_m1_f,static_p1_f;
	bool ivtc_detect,ivtc_ok_p1,ivtc_ok_m1,ivtc_a,ivtc_ok;
	bool ivtc_motion_map_f,ivtc_motion_map_m1_f,ivtc_motion_map_p1_f,ivtc_p1_f,ivtc_m1_f,
		interlace_f,interlace_p1_f,interlace_m1_f;
	bool test_ok,stable_ok,manual_current;
	bool modif_top,modif_bottom;
	uint32_t v,v0,v1,v2,v3,v4,v_b,v2_b,v3_b,tab_error[5];
	double s;
	uint32_t index_image;
	IVTC_Flags tab_ivtc[5];
	bool chg_seq;
	bool w_Y4,w_U4,w_V4,hw_Y4;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;
	
	if (pxsrc.mFrameNumber==0)
	{
		tab_pxsrc[0]=fa->mpSourceFrames[0]->mpPixmap;
		tab_pxsrc[1]=fa->mpSourceFrames[1]->mpPixmap;
		tab_pxsrc[2]=fa->mpSourceFrames[2]->mpPixmap;
		tab_pxsrc[3]=fa->mpSourceFrames[3]->mpPixmap;
		tab_pxsrc[4]=fa->mpSourceFrames[4]->mpPixmap;
		tab_pxsrc[5]=fa->mpSourceFrames[5]->mpPixmap;
		tab_pxsrc[6]=fa->mpSourceFrames[6]->mpPixmap;
		tab_pxsrc[7]=fa->mpSourceFrames[7]->mpPixmap;
	}
	else
	{
		tab_pxsrc[0]=fa->mpSourceFrames[1]->mpPixmap;
		tab_pxsrc[1]=fa->mpSourceFrames[2]->mpPixmap;
		tab_pxsrc[2]=fa->mpSourceFrames[0]->mpPixmap;
		tab_pxsrc[3]=fa->mpSourceFrames[3]->mpPixmap;
		tab_pxsrc[4]=fa->mpSourceFrames[4]->mpPixmap;
		tab_pxsrc[5]=fa->mpSourceFrames[5]->mpPixmap;
		tab_pxsrc[6]=fa->mpSourceFrames[5]->mpPixmap;
		tab_pxsrc[7]=fa->mpSourceFrames[6]->mpPixmap;
	}
		
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	w=idata.src_w0;
	h=idata.src_h0;
	w_Y=w;
	h_Y=h;
	w_U=idata.src_w1;
	w_V=idata.src_w2;
	h_U=idata.src_h1;
	h_V=idata.src_h2;
	h_removed=h-4*mData.line_removed;
	h_removed_resize=480-4*round((480.0/(double)idata.src_h0)*mData.line_removed);
	w_Y4=((w_Y&0x03)==0);
	w_U4=((w_U&0x03)==0);
	w_V4=((w_V&0x03)==0);
	hw_Y4=(((w_Y*h_Y)&0x03)==0);

	switch (idata.video_mode)
	{
		case 1 :
		case 2 :
		case 3 :
			dst1_Y=(void *)((uint8_t *)idata.dst_plane0-((idata.dst_h0-1)*idata.dst_pitch0));
			dst2_Y=(void *)((uint8_t *)idata.dst_plane0-((idata.dst_h0-2)*idata.dst_pitch0));
			break;
		case 6 :
			dst1_Y=(void *)((uint8_t *)idata.dst_plane0-((idata.dst_h0-1)*idata.dst_pitch0));
			dst2_Y=(void *)((uint8_t *)idata.dst_plane0-((idata.dst_h0-2)*idata.dst_pitch0));
			dst1_U=(void *)((uint8_t *)idata.dst_plane1-((idata.dst_h1-1)*idata.dst_pitch1));
			dst2_U=(void *)((uint8_t *)idata.dst_plane1-((idata.dst_h1-2)*idata.dst_pitch1));
			dst1_V=(void *)((uint8_t *)idata.dst_plane2-((idata.dst_h2-1)*idata.dst_pitch2));
			dst2_V=(void *)((uint8_t *)idata.dst_plane2-((idata.dst_h2-2)*idata.dst_pitch2));
			break;
	}

	switch (idata.video_mode)
	{
		case 0 :
		case 1 :
			frame_buffer_pitch=4*w;
			break;
		case 2 :
		case 3 :
			frame_buffer_pitch=4*((w+1)>>1);
			break;
		case 6 :
			frame_buffer_pitch=w_Y;
			break;
	}
	frame_buffer_pitch_Y=frame_buffer_pitch;
	frame_buffer_pitch_U=w_U;
	frame_buffer_pitch_V=w_V;
	buffer_pitch=4*w;
	buffer_pitch_planar420=4*((w_Y+1)>>1);

	src_pitch=idata.src_pitch0;
	src_modulo=idata.src_modulo0;
	dst_pitch=idata.dst_pitch0;
	dst_modulo=idata.dst_modulo0;
	src_pitch_Y=src_pitch;
	src_modulo_Y=src_modulo;
	dst_pitch_Y=dst_pitch;
	dst_modulo_Y=dst_modulo;
	src_pitch_U=idata.src_pitch1;
	src_modulo_U=idata.src_modulo1;
	dst_pitch_U=idata.dst_pitch1;
	dst_modulo_U=idata.dst_modulo1;
	src_pitch_V=idata.src_pitch2;
	src_modulo_V=idata.src_modulo2;
	dst_pitch_V=idata.dst_pitch2;
	dst_modulo_V=idata.dst_modulo2;
	frame_removed_pitch=2*src_pitch*mData.line_removed;
	buffer_removed_pitch=2*buffer_pitch*mData.line_removed;
	buffer_resize_removed_pitch=2*2880*round((480.0/(double)idata.src_h0)*mData.line_removed);
	thr_ivtc=mData.threshold_ivtc;
	thr_flt=mData.threshold_filter;
	thr_dtl=mData.threshold_deinterlace;
	filter_output=mData.output_filter;
	Y_only=(mData.Y_only && filter_output);
	output_mode=mData.output_mode;
	inv_polar=mData.inv_polar;
	disable_deinterlace=mData.disable_deinterlace;
	deinterlace_mode=mData.deinterlace_mode;
	blend_anime=mData.blend_anime;
	tri_linear=mData.tri_linear;
	disable_Z_mode=mData.disable_Z_mode;
	map_filter=!mData.disable_motion_map_filtering;

	if (filter_output) thr_out=thr_flt;
	else thr_out=thr_dtl;

	if (pxsrc.mFrameNumber==0)
	{
		for (i=0; i<7; i++)
		{		
/*			if (i==0)
			{
				if (fic1) fclose(fic1);
				fic1=fopen("D:\\Data-1.txt","wt");
				if (fic2) fclose(fic2);
				fic2=fopen("D:\\Data-2.txt","wt");
				if (fic3) fclose(fic3);
				fic3=fopen("D:\\Data-3.txt","wt");
			}*/

			if (i<Error_Fields_Size)
			{
				i_err=(uint8_t)i;
				i_err_m1=i_err-1;
			}
			else
			{
				i_err=error_index[Error_Index_Size-1];
				i_err_m1=error_index[Error_Index_Size-2];
			}
			
			if (i<Data_Buffer_Size)
			{
				i_wrt=(uint8_t)i;
				i_wrt_m1=i_wrt-1;
			}
			else
			{
				i_wrt=write_index[Write_Index_Size-1];
				i_wrt_m1=write_index[Write_Index_Size-2];
			}
		
			idata.src_plane0=tab_pxsrc[i]->data;
			idata.src_plane1=tab_pxsrc[i]->data2;
			idata.src_plane2=tab_pxsrc[i]->data3;
		
			switch (idata.video_mode)
			{
				case 1 :
				case 2 :
				case 3 :
					src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
					break;
				case 6 :
					src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
					src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
					src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
					break;
			}	

			switch(idata.video_mode)
			{
				case 2 :
					Convert_YUYV_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_modulo);
					break;
				case 3 :
					Convert_UYVY_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_modulo);
					break;
				case 6 :
					Convert_Planar420_to_RGB32((uint8_t *)src1_Y,(uint8_t *)src1_U,(uint8_t *)src1_V,
						buffer_delta,buffer[i_wrt].frameRGB32,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,buffer_pitch_planar420);
					if (resize_720x480)
					{
						Resize_Planar420(src1_Y,src1_U,src1_V,buffer_resize_Y,buffer_resize_U,buffer_resize_V,
							src_pitch_Y,src_pitch_U,src_pitch_V);
						Convert_Planar420_to_RGB32((uint8_t *)buffer_resize_Y,(uint8_t *)buffer_resize_U,
							(uint8_t *)buffer_resize_V,buffer_delta,buffer[i_wrt].frameRGB32Resize,720,480,720,368,368,
							1440);
					}
					break;
			}
			
			switch (idata.video_mode)
			{
				case 0 :
				case 1 :
					src1=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
					src2=(void *)((uint8_t *)src1+src_pitch);
					buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
						error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
						buffer[i_wrt].repartition,w,(h_removed>>1),src_pitch,buffer_pitch,src_modulo,0,
						buffer[i_wrt].error_Motion_Map,thr_ivtc);
					break;
				case 2 :
				case 3 :
				case 6 :
					if (resize_720x480)
					{
						src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
						src2=(void *)((uint8_t *)src1+2880);
						buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
							error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
							buffer[i_wrt].repartition,720,(h_removed_resize>>1),2880,2880,0,0,
							buffer[i_wrt].error_Motion_Map,thr_ivtc);
					}
					else
					{
						src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
						src2=(void *)((uint8_t *)src1+buffer_pitch);
						buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
							error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
							buffer[i_wrt].repartition,w,(h_removed>>1),buffer_pitch,buffer_pitch,0,0,
							buffer[i_wrt].error_Motion_Map,thr_ivtc);
					}
					break;
			}
			
			if (i==0) continue;			
		
			idata.src_plane0=tab_pxsrc[i-1]->data;
			idata.src_plane1=tab_pxsrc[i-1]->data2;
			idata.src_plane2=tab_pxsrc[i-1]->data3;
		
			switch (idata.video_mode)
			{
				case 1 :
				case 2 :
				case 3 :
					src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
					break;
				case 6 :
					src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
					src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
					src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
					break;
			}	
			
			switch (idata.video_mode)
			{
				case 0 :
				case 1 :
					if (inv_polar)
					{
						src1=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
						src2=(void *)((uint8_t *)src2_Y+(frame_removed_pitch+src_pitch));
					}
					else
					{
						src1=(void *)((uint8_t *)src2_Y+frame_removed_pitch);
						src2=(void *)((uint8_t *)src1_Y+(frame_removed_pitch+src_pitch));
					}
					buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,
						buffer_map,buffer_2,w,(h_removed>>1),src_pitch,src_modulo,
						buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
					break;
				case 2 :
				case 3 :
				case 6 :
					if (resize_720x480)
					{
						if (inv_polar)
						{
							src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(2880+buffer_resize_removed_pitch));
						}
							else
						{
							src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(2880+buffer_resize_removed_pitch));
						}
						buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
							720,h_removed_resize>>1,2880,0,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);		
					}
					else
					{
						if (inv_polar)
						{
							src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_pitch));
						}
						else
						{
							src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_pitch));
						}
						buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
							w,(h_removed>>1),buffer_pitch,0,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
					}
					break;
			}

			if (i==1)
			{
				buffer[0].error_IVTC_M1=buffer[0].error_IVTC_P1;
				buffer[0].error_IVTC_M1_Motion_Map=buffer[0].error_IVTC_P1_Motion_Map;
			}

			buffer[i_wrt].error_IVTC_M1=buffer[i_wrt_m1].error_IVTC_P1;
			buffer[i_wrt].error_IVTC_M1_Motion_Map=buffer[i_wrt_m1].error_IVTC_P1_Motion_Map;

			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					if (inv_polar)
						src2=(void *)((uint8_t *)src2_Y+src_pitch);
					else
						src2=(void *)((uint8_t *)src1_Y+src_pitch);
					if (Integer_SSE_Enable)
					{
						if (w&0x01)
							buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),src_pitch);
						else
							buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),src_pitch);
					}
					else
						buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),src_pitch,src_modulo);
					break;
				case 2 :
				case 3 :
				case 6 :
					if (resize_720x480)
					{
						if (inv_polar)
							src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
						else
							src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
						if (Integer_SSE_Enable)
							buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,360,h_removed_resize>>1,2880);
						else buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,2880,0);
					}
					else
					{
						if (inv_polar)
							src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
						else
							src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
						if (Integer_SSE_Enable)
						{
							if (w&0x01)
								buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),
									buffer_pitch);
							else
								buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),
									buffer_pitch);
						}
						else
							buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),buffer_pitch,0);
					}
					break;
			}
			
			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					src1=(void *)((uint8_t *)src2_Y+(frame_removed_pitch+src_pitch));
					src2=(void *)((uint8_t *)src1_Y+(frame_removed_pitch+src_pitch));
					if (Integer_SSE_Enable)
					{
						if (w&0x01)
							buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),src_pitch);
						else
							buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),src_pitch);
					}
					else
						buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),src_pitch,src_modulo);
					break;
				case 2 :
				case 3 :
				case 6 :
					if (resize_720x480)
					{
						src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(buffer_resize_removed_pitch+2880));
						src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(buffer_resize_removed_pitch+2880));
						if (Integer_SSE_Enable)
							buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,360,h_removed_resize>>1,2880);
						else buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,2880,0);
					}
					else
					{
						src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_pitch));
						src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_pitch));
						if (Integer_SSE_Enable)
						{
							if (w&0x01)
								buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),
									buffer_pitch);
							else
								buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),
									buffer_pitch);
						}
						else
							buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),buffer_pitch,0);
					}
					break;
			}

			if (i==1)
			{
				buffer[0].Delta_Bottom_M1=buffer[0].Delta_Bottom_P1;
				buffer[0].Delta_Top_M1=buffer[0].Delta_Top_P1;
			}

			buffer[i_wrt].Delta_Bottom_M1=buffer[i_wrt_m1].Delta_Bottom_P1;
			buffer[i_wrt].Delta_Top_M1=buffer[i_wrt_m1].Delta_Top_P1;

			src1=error_Fields[i_err_m1];
			src2=error_Fields[i_err];
			if (resize_720x480)
			{
				if (Integer_SSE_Enable)
					buffer[i_wrt_m1].Delta_error_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,360,h_removed_resize>>1,2880);
				else buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,2880,0);
			}
			else
			{
				if (Integer_SSE_Enable)
				{
					if (w&0x01)
						buffer[i_wrt_m1].Delta_error_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),
							buffer_pitch);
					else
						buffer[i_wrt_m1].Delta_error_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),
							buffer_pitch);
				}
				else
					buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),buffer_pitch,0);
			}

			if (i==1)
				buffer[0].Delta_error_M1=buffer[0].Delta_error_P1;

			buffer[i_wrt].Delta_error_M1=buffer[i_wrt_m1].Delta_error_P1;

			if (i>=Error_Fields_Size-1)
			{
				for (j=0; j<Error_Index_Size; j++)
					error_index[j]=(error_index[j]+1)%Error_Fields_Size;
			}

			if (i>=Data_Buffer_Size-1)
			{
				for (j=0; j<Write_Index_Size; j++)
					write_index[j]=(write_index[j]+1)%Data_Buffer_Size;
			}

			if (i>=3)
			{
				ivtc_f=prog_f=rep128_f=eg_ivtc_p1_f=eg_ivtc_m1_f=z_dtop_p1_f=z_dbot_m1_f=false;
				z_dtop_m1_f=z_dbot_p1_f=false;
				dtop_m1_f=dbot_m1_f=dtop_p1_f=dbot_p1_f=pic_f=error_f=chg_sc_f=false;
				dtop_p1_m_f=dtop_m1_m_f=dbot_m1_p_f=dbot_p1_p_f=dtopbot_f=false;
				dbot_p1_m_f=dbot_m1_m_f=dtop_m1_p_f=dtop_p1_p_f=false;
				static_f=static_m1_f=static_p1_f=false;
				ivtc_m1_f=chg_sc_m1_f=false;

				i_rd=flag_index[0];
				i_rd_m1=flag_index[Flag_Index_Size-1];
				i_rd_m2=flag_index[Flag_Index_Size-2];
				i_rd_p1=flag_index[1];
				i_rd_p2=flag_index[2];
		
				v0=buffer[i_rd].error;
				v1=buffer[i_rd].error_IVTC_M1;
				v2=buffer[i_rd].error_IVTC_P1;

				if (v0<v1) v=v0;
				else v=v1;
				ivtc_p1_f=((1.3*v>v2) && (v>IVTC));

				if (v0<v2) v=v0;
				else v=v2;
				ivtc_m1_f=((1.3*v>v1) && (v>IVTC));

				if (v0>IVTC) s=2.0;
				else s=4.0;
				if (blend_anime) s*=2.0;
				ivtc_ok_m1=(v1<=s*v0);
				ivtc_ok_p1=(v2<=s*v0);

				if (v2>v1) v=v2;
				else v=v1;
				if (v0<IVTC) s=0.9;
				else s=0.85;
				ivtc_f=(v<s*v0); //0.65

/*				if (v1<v2) v=v1;
				else v=v2;
				prog_f=(2.0*v0<v);*/

/*				if (v0>v1) v=v0;
				else v=v1;
				chg_sc_f=((1.8*v<v2) && (v2>ChgSc_1));

				if (v0>v2) v=v0;
				else v=v2;
				chg_sc_m1_f=((1.8*v<v1) && (v1>ChgSc_1));*/

				chg_sc_f=((1.8*v0<v2) && (v2>ChgSc_1));

				chg_sc_m1_f=((1.8*v0<v1) && (v1>ChgSc_1));


				v0=buffer[i_rd_p1].error;
				v1=buffer[i_rd_p1].error_IVTC_M1;
				v2=buffer[i_rd_p1].error_IVTC_P1;

/*				if (v0>v2) v=v0;
				else v=v2;
				chg_sc_f=chg_sc_f && ((1.8*v<v1) && (v1>ChgSc_1));*/

				chg_sc_f=chg_sc_f && ((1.8*v0<v1) && (v1>ChgSc_1));

				v0=buffer[i_rd].error_Motion_Map;
				v1=buffer[i_rd].error_IVTC_M1_Motion_Map;
				v2=buffer[i_rd].error_IVTC_P1_Motion_Map;

				if (v2>v1) v=v2;
				else v=v1;
				if (disable_Z_mode) s=0.90;
				else s=0.7;
				ivtc_motion_map_f=((v<s*v0) && (v0>IVTC_MIN_2)) ||
					((v1==0) && (v2==0) && (v0>IVTC_MIN_3));
				ivtc_motion_map_m1_f=((v1<s*v0) && (v0>IVTC_MIN_2));
				ivtc_motion_map_p1_f=((v2<s*v0) && (v0>IVTC_MIN_2));

				prog_f=(v0==0);
				static_f=(prog_f && (v1==0) && (v2==0));

/*				if (v0>v1) v=v0;
				else v=v1;
				chg_sc_f=chg_sc_f && ((2.0*v<v2) && (v2>ChgSc_2));

				if (v0>v2) v=v0;
				else v=v2;
				chg_sc_m1_f=chg_sc_m1_f && ((2.0*v<v1) && (v1>ChgSc_2));*/

				chg_sc_f=chg_sc_f && ((2.0*v0<v2) && (v2>ChgSc_2));

				chg_sc_m1_f=chg_sc_m1_f && ((2.0*v0<v1) && (v1>ChgSc_2));

				if (v0<v1) v=v0;
				else v=v1;
				ivtc_p1_f=ivtc_p1_f && (((v>15.0*v2) && (v>IVTC_MIN_4)) || ((v2==0) && (v>IVTC_MIN_5)));

				if (v0<v2) v=v0;
				else v=v2;
				ivtc_m1_f=ivtc_m1_f && (((v>15.0*v1) && (v>IVTC_MIN_4)) || ((v1==0) && (v>IVTC_MIN_5)));

				if (v0>IVTC_MIN_2) s=7.0;
				else s=10.0;

				if (v0>IVTC_MIN_3)
				{
					/*ivtc_ok_p1=ivtc_ok_p1 && (((v2<=s*v0) && (v2<IVTC_MIN_6)) || (v2<0.8*v0));
					ivtc_ok_m1=ivtc_ok_m1 && ((v1<=s*v0) && (v1<IVTC_MIN_6)) || (v1<0.8*v0));*/
					ivtc_ok_p1=ivtc_ok_p1 && (v2<=s*v0);
					ivtc_ok_m1=ivtc_ok_m1 && (v1<=s*v0);
					if ((v0>IVTC_MIN_6) && (v0>2.8*v1) && (v0>2.8*v2))
					{
						ivtc_ok_p1=true;
						ivtc_ok_m1=true;
					}
				}
				else
				{
					if (v1<IVTC_MIN_1) ivtc_ok_m1=true;
					if (v2<IVTC_MIN_1) ivtc_ok_p1=true;
				}
				interlace_f=(v0>IVTC_MIN_7);
				interlace_m1_f=(v1>IVTC_MIN_7);
				interlace_p1_f=(v2>IVTC_MIN_7);
				ivtc_a=true;
				if ( ((20.0*v1<v2) && (6.0*v2>v0) && (v0>IVTC_MIN_1)) && !ivtc_f) ivtc_a=false;
/*				if ((v2>IVTC_MIN_1) && ((v0<IVTC_MIN_2) && (v1<IVTC_MIN_2))
					&& (v0>5.0*v1)) ivtc_a=false;*/
				if (ivtc_m1_f && !(ivtc_f || ivtc_motion_map_f)) ivtc_a=false;

				v0=buffer[i_rd_p1].error_Motion_Map;
				v1=buffer[i_rd_p1].error_IVTC_M1_Motion_Map;
				v2=buffer[i_rd_p1].error_IVTC_P1_Motion_Map;

/*				if (v0>v2) v=v0;
				else v=v2;
				chg_sc_f=chg_sc_f && ((2.0*v<v1) && (v1>ChgSc_2));*/

				chg_sc_f=chg_sc_f && ((2.0*v0<v1) && (v1>ChgSc_2));

				eg_ivtc_m1_f=((buffer[i_rd_p1].error==buffer[i_rd_p1].error_IVTC_M1) && (v0>0)
					&& (v1>0));

				rep128_f=(buffer[i_rd].repartition[128]>=Deinterlace_Min);

				interlace_f=interlace_f || rep128_f;

				if (i>=4)
				{
					v0=buffer[i_rd_m1].Delta_Top_P1;
					v1=buffer[i_rd_m1].Delta_Top_M1;
					v2=buffer[i_rd_m1].Delta_Bottom_M1;
					v3=buffer[i_rd_m1].Delta_Bottom_P1;
			
					if (inv_polar)
					{
						z_dbot_p1_f=(v3==0);
						z_dbot_m1_f=(v2==0);
					}
					else
					{
						z_dtop_p1_f=(v0==0);
						z_dtop_m1_f=(v1==0);
					}

					if (inv_polar)
					{
						if (v0<v1) v=v0;
						else v=v1;
						if (v2<v) v=v2;
						dbot_p1_f=((v3<0.8*v) && (v>DTopBot)); // 0.6

						if (v0<v1) v=v0;
						else v=v1;
						if (v3<v) v=v3;
						dbot_m1_m_f=((v2<0.8*v) && (v>DTopBot)); // 0.6
					}
					else
					{
						if (v1<v2) v=v1;
						else v=v2;
						if (v3<v) v=v3;
						dtop_p1_f=((v0<0.8*v) && (v>DTopBot)); // 0.6

						if (v0<v2) v=v0;
						else v=v2;
						if (v3<v) v=v3;
						dtop_m1_m_f=((v1<0.8*v) && (v>DTopBot)); // 0.6
					}

					v0=buffer[i_rd_m1].error;
					v1=buffer[i_rd_m1].error_IVTC_P1;
					v2=buffer[i_rd_m1].error_Motion_Map;
					v3=buffer[i_rd_m1].error_IVTC_P1_Motion_Map;
					eg_ivtc_p1_f=((v0==v1) && (v2>0) && (v3>0));
					
					v0=buffer[i_rd_m1].error_Motion_Map;
					v1=buffer[i_rd_m1].error_IVTC_M1_Motion_Map;
					v2=buffer[i_rd_m1].error_IVTC_P1_Motion_Map;

/*					if (v0>v1) v=v0;
					else v=v1;
					chg_sc_m1_f=chg_sc_m1_f && ((2.0*v<v2) && (v2>ChgSc_2));*/

					chg_sc_m1_f=chg_sc_m1_f && ((2.0*v0<v2) && (v2>ChgSc_2));
					
					v0=buffer[i_rd_m1].error;
					v1=buffer[i_rd_m1].error_IVTC_M1;
					v2=buffer[i_rd_m1].error_IVTC_P1;

/*					if (v0>v1) v=v0;
					else v=v1;
					chg_sc_m1_f=chg_sc_m1_f && ((1.8*v<v2) && (v2>ChgSc_1));*/

					chg_sc_m1_f=chg_sc_m1_f && ((1.8*v0<v2) && (v2>ChgSc_1));

				}

				if (i>=5)
				{
					if (inv_polar)
					{
						v0=buffer[i_rd_m1].Delta_Top_M1;
						v1=buffer[i_rd].Delta_Top_M1;
						v2=buffer[i_rd_p1].Delta_Top_M1;
						pic_f=((v1>1.4*v0) && (v1>1.4*v2)); //3.0
						v0=buffer[i_rd_m1].Delta_Bottom_P1;
						v1=buffer[i_rd].Delta_Bottom_P1;
						v2=buffer[i_rd_p1].Delta_Bottom_P1;
						pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
						v0=buffer[i_rd_m2].Delta_Top_P1;
						v1=buffer[i_rd_m1].Delta_Top_P1;
						v2=buffer[i_rd].Delta_Top_P1;
						pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
						v0=buffer[i_rd].Delta_Bottom_M1;
						v1=buffer[i_rd_p1].Delta_Bottom_M1;
						v2=buffer[i_rd_p2].Delta_Bottom_M1;
						pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); //3.0
					}
					else
					{
						v0=buffer[i_rd_m1].Delta_Bottom_M1;
						v1=buffer[i_rd].Delta_Bottom_M1;
						v2=buffer[i_rd_p1].Delta_Bottom_M1;
						pic_f=((v1>1.4*v0) && (v1>1.4*v2)); //3.0
						v0=buffer[i_rd_m1].Delta_Top_P1;
						v1=buffer[i_rd].Delta_Top_P1;
						v2=buffer[i_rd_p1].Delta_Top_P1;
						pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
						v0=buffer[i_rd_m2].Delta_Bottom_P1;
						v1=buffer[i_rd_m1].Delta_Bottom_P1;
						v2=buffer[i_rd].Delta_Bottom_P1;
						pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
						v0=buffer[i_rd].Delta_Top_M1;
						v1=buffer[i_rd_p1].Delta_Top_M1;
						v2=buffer[i_rd_p2].Delta_Top_M1;
						pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); //3.0
					}

					v0=buffer[i_rd_m1].Delta_error_M1;
					v1=buffer[i_rd_p1].Delta_error_P1;
					if (v0>v1) v2=v0;
					else v2=v1;
					v0=buffer[i_rd].Delta_error_M1;
					v1=buffer[i_rd].Delta_error_P1;
					if (v0<v1) v3=v0;
					else v3=v1;
					error_f=(v2<0.8*v3); //0.6

					v0=buffer[i_rd_m2].Delta_Top_P1;
					v1=buffer[i_rd_m2].Delta_Bottom_M1;
					v2=buffer[i_rd_m2].Delta_Bottom_P1;
					v3=buffer[i_rd_m2].Delta_Top_M1;
					if (v1<v2) v=v1;
					else v=v2;
					if (v3<v) v=v3;
					dtop_p1_m_f=((v0<0.8*v) && (v>DTopBot)); //0.6
					if (v0<v1) v=v0;
					else v=v1;
					if (v3<v) v=v3;
					dbot_p1_m_f=((v2<0.8*v) && (v>DTopBot)); //0.6
				}

				v0=buffer[i_rd].Delta_Top_M1;
				v1=buffer[i_rd].Delta_Bottom_P1;
				v2=buffer[i_rd].Delta_Top_P1;
				v3=buffer[i_rd].Delta_Bottom_M1;

				if (inv_polar)
				{
					if (v0<v1) v=v0;
					else v=v1;
					if (v2<v) v=v2;
					dbot_m1_f=((v3<0.8*v) && (v>DTopBot)); //0.6

					if (v0<v1) v=v0;
					else v=v1;
					if (v3<v) v=v3;
					dtop_p1_f=((v2<0.8*v)&& (v>DTopBot)); //0.6
				}
				else
				{
					if (v1<v2) v=v1;
					else v=v2;
					if (v3<v) v=v3;
					dtop_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6

					if (v0<v2) v=v0;
					else v=v2;
					if (v3<v) v=v3;
					dbot_p1_f=((v1<0.8*v)&& (v>DTopBot)); //0.6
				}

				if (v1<v2) v=v1;
				else v=v2;
				if (v3>v0) v4=v3;
				else v4=v0;
				static_m1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));

				if (v3<v0) v=v3;
				else v=v0;
				if (v1>v2) v4=v1;
				else v4=v2;
				static_p1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));

//				static_f=((v0<Static) && (v1<Static) && (v2<Static) && (v3<Static));

				v0=buffer[i_rd_p1].Delta_Bottom_M1;
				v1=buffer[i_rd_p1].Delta_Bottom_P1;
				v2=buffer[i_rd_p1].Delta_Top_M1;
				v3=buffer[i_rd_p1].Delta_Top_P1;

				if (inv_polar)
				{
					z_dtop_m1_f=(v2==0);
					z_dtop_p1_f=(v3==0);
				}
				else
				{
					z_dbot_m1_f=(v0==0);
					z_dbot_p1_f=(v1==0);
				}

				if (inv_polar)
				{
					if (v0<v1) v=v0;
					else v=v1;
					if (v3<v) v=v3;
					dtop_m1_f=((v2<0.8*v) && (v>DTopBot)); //0.6

					if (v0<v1) v=v0;
					else v=v1;
					if (v2<v) v=v2;
					dtop_p1_p_f=((v3<0.8*v) && (v>DTopBot));  //0.6
				}
				else
				{
					if (v1<v2) v=v1;
					else v=v2;
					if (v3<v) v=v3;
					dbot_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6

					if (v0<v2) v=v0;
					else v=v2;
					if (v3<v) v=v3;
					dbot_p1_p_f=((v1<0.8*v) && (v>DTopBot));  //0.6
				}

				v0=buffer[i_rd_p2].Delta_Bottom_M1;
				v1=buffer[i_rd_p2].Delta_Bottom_P1;
				v2=buffer[i_rd_p2].Delta_Top_M1;
				v3=buffer[i_rd_p2].Delta_Top_P1;

				if (v1<v2) v=v1;
				else v=v2;
				if (v3<v) v=v3;
				dbot_m1_p_f=((v0<0.8*v) && (v>DTopBot)); //0.6*/
				if (v0<v1) v=v0;
				else v=v1;
				if (v3<v) v=v3;
				dtop_m1_p_f=((v2<0.8*v) && (v>DTopBot)); //0.6*/

				if (inv_polar)
					dtopbot_f=( ((dbot_m1_f && dbot_p1_f) && (dtop_m1_p_f || dtop_p1_p_f)) &&
						!static_m1_f && !chg_sc_f) || ( ((dtop_p1_f && dtop_m1_f) &&
						(dbot_m1_m_f || dbot_p1_m_f)) && !static_p1_f && !chg_sc_m1_f);
				else
					dtopbot_f=( ((dtop_m1_f && dtop_p1_f) && (dbot_m1_p_f || dbot_p1_p_f)) &&
						!static_m1_f && !chg_sc_f) || ( ((dbot_p1_f && dbot_m1_f) &&
						(dtop_m1_m_f || dtop_p1_m_f)) && !static_p1_f && !chg_sc_m1_f);

/*				if (inv_polar)
				{
					ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && (dbot_p1_f && dbot_m1_f) && chg_sc_f;
					ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && (dtop_p1_f && dtop_m1_f) && chg_sc_m1_f;
				}
				else
				{
					ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && (dtop_p1_f && dtop_m1_f) && chg_sc_f;
					ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && (dbot_p1_f && dbot_m1_f) && chg_sc_m1_f;
				}*/

/*				if (inv_polar)
				{
					ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && dbot_m1_f && chg_sc_f;
					ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && dtop_p1_f && chg_sc_m1_f;
				}
				else
				{
					ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && dtop_m1_f && chg_sc_f;
					ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && dbot_p1_f && chg_sc_m1_f;
				}*/

				ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && chg_sc_f;
				ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && chg_sc_m1_f;

				if (inv_polar)
					ivtc_detect=(ivtc_motion_map_f || eg_ivtc_p1_f || eg_ivtc_m1_f ||
						(z_dbot_p1_f && !static_m1_f) || (z_dtop_m1_f && !static_p1_f) ||
						(z_dbot_m1_f && !static_p1_f && !chg_sc_m1_f) ||
						(z_dtop_p1_f && !static_m1_f && !chg_sc_f) || dtopbot_f || ivtc_motion_map_p1_f
						|| ivtc_motion_map_m1_f || ivtc_p1_f || ivtc_m1_f);
				else
					ivtc_detect=(ivtc_motion_map_f || eg_ivtc_p1_f || eg_ivtc_m1_f ||
						(z_dtop_p1_f && !static_m1_f) || (z_dbot_m1_f && !static_p1_f) ||
						(z_dtop_m1_f && !static_p1_f && !chg_sc_m1_f) ||
						(z_dbot_p1_f && !static_m1_f && !chg_sc_f) || dtopbot_f || ivtc_motion_map_p1_f
						|| ivtc_motion_map_m1_f || ivtc_p1_f || ivtc_m1_f);

				if (ivtc_motion_map_m1_f) ivtc_a=false;
				if (ivtc_a) ivtc_ok=ivtc_ok_p1 && ivtc_detect;
				else ivtc_ok=ivtc_ok_m1 & ivtc_detect;

/*				fprintf(fic1,"Image : %d\n",i-3);
				if (dbot_m1_f) fprintf(fic1,"DBOT-1 ");
				if (dbot_p1_f) fprintf(fic1,"DBOT+1 ");
				if (dtop_p1_f) fprintf(fic1,"DTOP+1 ");
				if (dtop_m1_f) fprintf(fic1,"DTOP-1 ");
				if (z_dtop_p1_f) fprintf(fic1,"ZDTOP+1 ");
				if (z_dbot_m1_f) fprintf(fic1,"ZDBOT-1 ");
				if (z_dtop_m1_f) fprintf(fic1,"ZDTOP-1 ");
				if (z_dbot_p1_f) fprintf(fic1,"ZDBOT+1 ");
				if (dtopbot_f) fprintf(fic1,"DTopBot ");
				if (eg_ivtc_p1_f) fprintf(fic1,"ZIVTC+1 ");
				if (eg_ivtc_m1_f) fprintf(fic1,"ZIVTC-1 ");
				if (ivtc_f) fprintf(fic1,"IVTC ");
				if (ivtc_motion_map_f) fprintf(fic1,"IVTCMap ");
				if (ivtc_p1_f) fprintf(fic1,"IVTC+1 ");
				if (ivtc_m1_f) fprintf(fic1,"IVTC-1 ");
				if (chg_sc_f) fprintf(fic1,"ChgSc ");
				if (chg_sc_m1_f) fprintf(fic1,"ChgSc-1 ");
				if (ivtc_detect) fprintf(fic1,"IVTCDetect ");
				if (!ivtc_ok_p1) fprintf(fic1,"IVTCNul ");*/

				buffer[i_rd].flags.ivtc=ivtc_f;
				buffer[i_rd].flags.ivtc_map=ivtc_motion_map_f;
				buffer[i_rd].flags.pic=pic_f;
				buffer[i_rd].flags.error=error_f;
				buffer[i_rd].flags.rev=!ivtc_a;
				buffer[i_rd].flags.interlace=interlace_f;
				buffer[i_rd].flags.interlace_m1=interlace_m1_f;
				buffer[i_rd].flags.interlace_p1=interlace_p1_f;
				if (inv_polar)
				{
					buffer[i_rd].flags.first=(eg_ivtc_p1_f || z_dbot_p1_f || (z_dtop_p1_f && !chg_sc_f)
						|| (((dbot_m1_f && dbot_p1_f) && (dtop_m1_p_f || dtop_p1_p_f)) && !static_m1_f && !chg_sc_f));
					buffer[i_rd].flags.last=(eg_ivtc_m1_f || z_dtop_m1_f || (z_dbot_m1_f && !chg_sc_m1_f)
						|| (((dtop_p1_f && dtop_m1_f) && (dbot_m1_m_f || dbot_p1_m_f)) && !static_p1_f && !chg_sc_m1_f));
					buffer[i_rd].flags.Z=(eg_ivtc_p1_f || eg_ivtc_m1_f || z_dbot_p1_f || z_dtop_m1_f ||
						(z_dtop_p1_f && !chg_sc_f) || (z_dbot_m1_f && !chg_sc_m1_f));
				}
				else
				{
					buffer[i_rd].flags.first=(eg_ivtc_p1_f || z_dtop_p1_f || (z_dbot_p1_f && !chg_sc_f)
						|| (((dtop_m1_f && dtop_p1_f) && (dbot_m1_p_f || dbot_p1_p_f)) && !static_m1_f && !chg_sc_f));
					buffer[i_rd].flags.last=(eg_ivtc_m1_f || z_dbot_m1_f || (z_dtop_m1_f && !chg_sc_m1_f) || 
						(((dbot_p1_f && dbot_m1_f) && (dtop_m1_m_f || dtop_p1_m_f)) && !static_p1_f && !chg_sc_m1_f));
					buffer[i_rd].flags.Z=(eg_ivtc_p1_f || eg_ivtc_m1_f || z_dtop_p1_f || z_dbot_m1_f
						|| (z_dbot_p1_f && !chg_sc_f) || (z_dtop_m1_f && !chg_sc_m1_f));
				}
				buffer[i_rd].flags.chg_sc_p1=chg_sc_f;
				buffer[i_rd].flags.chg_sc=chg_sc_m1_f;
				buffer[i_rd].flags.dtopbot=dtopbot_f;
				buffer[i_rd].flags.ivtc_valid=ivtc_ok;
				buffer[i_rd].flags.p1_f=ivtc_ok_p1;
				buffer[i_rd].flags.m1_f=ivtc_ok_m1;

/*				if (buffer[i_rd].flags.Z) fprintf(fic1,"Z ");
				if (buffer[i_rd].flags.first) fprintf(fic1,"First ");
				if (buffer[i_rd].flags.last) fprintf(fic1,"Last ");
				if ((first)!=5) fprintf(fic1,"F:(%d) ",first);
				if ((last)!=5) fprintf(fic1,"L:(%d) ",last);
				fprintf(fic1,"\n\n");*/
			}

			if (i>=3)
			{
				for (j=0; j<Flag_Index_Size; j++)
					flag_index[j]=(flag_index[j]+1)%Data_Buffer_Size;
			}
		}		
	}
	/////////////////// Fin num image =0
	
	
	idata.src_plane0=tab_pxsrc[7]->data;
	idata.src_plane1=tab_pxsrc[7]->data2;
	idata.src_plane2=tab_pxsrc[7]->data3;

	switch (idata.video_mode)
	{
		case 1 :
		case 2 :
		case 3 :
			src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
			break;
		case 6 :
			src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
			src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
			src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
			break;
	}	

	idata.src_plane0=tab_pxsrc[6]->data;
	idata.src_plane1=tab_pxsrc[6]->data2;
	idata.src_plane2=tab_pxsrc[6]->data3;

	switch (idata.video_mode)
	{
		case 1 :
		case 2 :
		case 3 :
			src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
			break;
		case 6 :
			src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
			src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
			src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
			break;
	}	


	
	i_err=error_index[Error_Index_Size-1];
	i_err_m1=error_index[Error_Index_Size-2];

	if ((pxsrc.mFrameNumber+7)<Data_Buffer_Size)
	{
		i_wrt=(uint8_t)(pxsrc.mFrameNumber+7);
		i_wrt_m1=i_wrt-1;
	}
	else
	{
		i_wrt=write_index[Write_Index_Size-1];
		i_wrt_m1=write_index[Write_Index_Size-2];
	}
	
	switch(idata.video_mode)
	{
		case 2 :
			Convert_YUYV_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_modulo);
			break;
		case 3 :
			Convert_UYVY_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_modulo);
			break;
		case 6 :
				Convert_Planar420_to_RGB32((uint8_t *)src1_Y,(uint8_t *)src1_U,(uint8_t *)src1_V,buffer_delta,
					buffer[i_wrt].frameRGB32,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,buffer_pitch_planar420);
			if (resize_720x480)
			{
				Resize_Planar420(src1_Y,src1_U,src1_V,buffer_resize_Y,buffer_resize_U,buffer_resize_V,src_pitch_Y,src_pitch_U,
					src_pitch_V);
				Convert_Planar420_to_RGB32((uint8_t *)buffer_resize_Y,(uint8_t *)buffer_resize_U,
					(uint8_t *)buffer_resize_V,buffer_delta,buffer[i_wrt].frameRGB32Resize,720,480,720,368,368,
					1440);
			}
			break;
	}

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			src1=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
			src2=(void *)((uint8_t *)src1+src_pitch);
			buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
				error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
				buffer[i_wrt].repartition,w,(h_removed>>1),src_pitch,buffer_pitch,src_modulo,0,
				buffer[i_wrt].error_Motion_Map,thr_ivtc);
			break;
		case 2 :
		case 3 :
		case 6 :
			if (resize_720x480)
			{
				src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
				src2=(void *)((uint8_t *)src1+2880);
				buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
					error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
					buffer[i_wrt].repartition,720,h_removed_resize>>1,2880,2880,0,0,
					buffer[i_wrt].error_Motion_Map,thr_ivtc);
			}
			else
			{
				src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
				src2=(void *)((uint8_t *)src1+buffer_pitch);
				buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
					error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
					buffer[i_wrt].repartition,w,(h_removed>>1),buffer_pitch,buffer_pitch,0,0,
					buffer[i_wrt].error_Motion_Map,thr_ivtc);
			}
			break;
	}


	switch (idata.video_mode)
	{
		case 0 :
		case 1 :
			if (inv_polar)
			{
				src1=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
				src2=(void *)((uint8_t *)src2_Y+(frame_removed_pitch+src_pitch));
			}
			else
			{
				src1=(void *)((uint8_t *)src2_Y+frame_removed_pitch);
				src2=(void *)((uint8_t *)src1_Y+(frame_removed_pitch+src_pitch));
			}
			buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,
				buffer_map,buffer_2,w,(h_removed>>1),src_pitch,src_modulo,
				buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
			break;
		case 2 :
		case 3 :
		case 6 :
			if (resize_720x480)
			{
				if (inv_polar)
				{
					src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(2880+buffer_resize_removed_pitch));
				}
				else
				{
					src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(2880+buffer_resize_removed_pitch));
				}
				buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
					720,h_removed_resize>>1,2880,0,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);		
			}
			else
			{
				if (inv_polar)
				{
					src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_pitch));
				}
				else
				{
					src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_pitch));
				}
				buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
					w,(h_removed>>1),buffer_pitch,0,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
			}
			break;
	}

	buffer[i_wrt].error_IVTC_M1=buffer[i_wrt_m1].error_IVTC_P1;
	buffer[i_wrt].error_IVTC_M1_Motion_Map=buffer[i_wrt_m1].error_IVTC_P1_Motion_Map;

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			if (inv_polar)
				src2=(void *)((uint8_t *)src2_Y+frame_removed_pitch);
			else
				src2=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
			if (Integer_SSE_Enable)
			{
				if (w&0x01)
					buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),src_pitch);
				else
					buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),src_pitch);
			}
			else
				buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),src_pitch,src_modulo);
			break;
		case 2 :
		case 3 :
		case 6 :
			if (resize_720x480)
			{
				if (inv_polar)
					src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
				else
					src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
				if (Integer_SSE_Enable)
					buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,360,h_removed_resize>>1,2880);
				else buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,2880,0);
			}
			else
			{
				if (inv_polar)
					src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
				else
					src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
				if (Integer_SSE_Enable)
				{
					if (w&0x01)
						buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),
							buffer_pitch);
					else
						buffer[i_wrt_m1].Delta_Bottom_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),
							buffer_pitch);
				}
				else
					buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),buffer_pitch,0);
			}
			break;
	}



	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			src1=(void *)((uint8_t *)src2_Y+(frame_removed_pitch+src_pitch));
			src2=(void *)((uint8_t *)src1_Y+(frame_removed_pitch+src_pitch));
			if (Integer_SSE_Enable)
			{
				if (w&0x01)
					buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),src_pitch);
				else
					buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),src_pitch);
			}
			else
				buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),src_pitch,src_modulo);
			break;
		case 2 :
		case 3 :
		case 6 :
			if (resize_720x480)
			{
				src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(buffer_resize_removed_pitch+2880));
				src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(buffer_resize_removed_pitch+2880));
				if (Integer_SSE_Enable)
					buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,360,h_removed_resize>>1,2880);
				else buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,2880,0);
			}
			else
			{
				src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_pitch));
				src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_pitch));
				if (Integer_SSE_Enable)
				{
					if (w&0x01)
						buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),
							buffer_pitch);
					else
						buffer[i_wrt_m1].Delta_Top_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),
							buffer_pitch);
				}
				else
					buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),buffer_pitch,0);
			}
			break;
	}


	buffer[i_wrt].Delta_Bottom_M1=buffer[i_wrt_m1].Delta_Bottom_P1;
	buffer[i_wrt].Delta_Top_M1=buffer[i_wrt_m1].Delta_Top_P1;

	src1=error_Fields[i_err_m1];
	src2=error_Fields[i_err];
	if (resize_720x480)
	{
		if (Integer_SSE_Enable)
			buffer[i_wrt_m1].Delta_error_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,360,h_removed_resize>>1,2880);
		else buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,2880,0);
	}
	else
	{
		if (Integer_SSE_Enable)
		{
			if (w&0x01)
				buffer[i_wrt_m1].Delta_error_P1=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1,src2,w,(h_removed>>1),
					buffer_pitch);
			else
				buffer[i_wrt_m1].Delta_error_P1=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1,src2,(w>>1),(h_removed>>1),
					buffer_pitch);
		}
		else
			buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,w,(h_removed>>1),buffer_pitch,0);
	}

	buffer[i_wrt].Delta_error_M1=buffer[i_wrt_m1].Delta_error_P1;

	for (i=0; i<Error_Index_Size; i++)
		error_index[i]=(error_index[i]+1)%Error_Fields_Size;

	if ((pxsrc.mFrameNumber+7)>=(Data_Buffer_Size-1))
	{
		for (i=0; i<Write_Index_Size; i++)
			write_index[i]=(write_index[i]+1)%Data_Buffer_Size;
	}


	ivtc_f=prog_f=rep128_f=eg_ivtc_p1_f=eg_ivtc_m1_f=z_dtop_p1_f=z_dbot_m1_f=false;
	z_dtop_m1_f=z_dbot_p1_f=false;
	dtop_m1_f=dbot_m1_f=dtop_p1_f=dbot_p1_f=pic_f=error_f=chg_sc_f=false;
	dtop_p1_m_f=dtop_m1_m_f=dbot_m1_p_f=dbot_p1_p_f=dtopbot_f=false;
	dbot_p1_m_f=dbot_m1_m_f=dtop_m1_p_f=dtop_p1_p_f=false;
	static_f=static_m1_f=static_p1_f=false;
	ivtc_m1_f=chg_sc_m1_f=false;

	i_rd=flag_index[0];
	i_rd_m1=flag_index[Flag_Index_Size-1];
	i_rd_m2=flag_index[Flag_Index_Size-2];
	i_rd_p1=flag_index[1];
	i_rd_p2=flag_index[2];
		
	v0=buffer[i_rd].error;
	v1=buffer[i_rd].error_IVTC_M1;
	v2=buffer[i_rd].error_IVTC_P1;

	if (v0<v1) v=v0;
	else v=v1;
	ivtc_p1_f=((1.3*v>v2) && (v>IVTC));

	if (v0<v2) v=v0;
	else v=v2;
	ivtc_m1_f=((1.3*v>v1) && (v>IVTC));

	if (v0>IVTC) s=2.0;
	else s=4.0;
	if (blend_anime) s*=2.0;
	ivtc_ok_m1=(v1<=s*v0);
	ivtc_ok_p1=(v2<=s*v0);

	if (v2>v1) v=v2;
	else v=v1;
	if (v0<IVTC) s=0.9;
	else s=0.85;
	ivtc_f=(v<s*v0); //0.65

/*	if (v1<v2) v=v1;
	else v=v2;
	prog_f=(2.0*v0<v);*/

/*	if (v0>v1) v=v0;
	else v=v1;
	chg_sc_f=((1.8*v<v2) && (v2>ChgSc_1));

	if (v0>v2) v=v0;
	else v=v2;
	chg_sc_m1_f=((1.8*v<v1) && (v1>ChgSc_1));*/

	chg_sc_f=((1.8*v0<v2) && (v2>ChgSc_1));

	chg_sc_m1_f=((1.8*v0<v1) && (v1>ChgSc_1));

	v0=buffer[i_rd_p1].error;
	v1=buffer[i_rd_p1].error_IVTC_M1;
	v2=buffer[i_rd_p1].error_IVTC_P1;

/*	if (v0>v2) v=v0;
	else v=v2;
	chg_sc_f=chg_sc_f && ((1.8*v<v1) && (v1>ChgSc_1));*/

	chg_sc_f=chg_sc_f && ((1.8*v0<v1) && (v1>ChgSc_1));
	
	v0=buffer[i_rd].error_Motion_Map;
	v1=buffer[i_rd].error_IVTC_M1_Motion_Map;
	v2=buffer[i_rd].error_IVTC_P1_Motion_Map;

	if (v2>v1) v=v2;
	else v=v1;
	if (disable_Z_mode) s=0.90;
	else s=0.7;
	ivtc_motion_map_f=((v<s*v0) && (v0>IVTC_MIN_2)) ||
		((v1==0) && (v2==0) && (v0>IVTC_MIN_3));
	ivtc_motion_map_m1_f=((v1<s*v0) && (v0>IVTC_MIN_2));
	ivtc_motion_map_p1_f=((v2<s*v0) && (v0>IVTC_MIN_2));

	prog_f=(v0==0);
	static_f=(prog_f && (v1==0) && (v2==0));

/*	if (v0>v1) v=v0;
	else v=v1;
	chg_sc_f=chg_sc_f && ((2.0*v<v2) && (v2>ChgSc_2));

	if (v0>v2) v=v0;
	else v=v2;
	chg_sc_m1_f=chg_sc_m1_f && ((2.0*v<v1) && (v1>ChgSc_2));*/

	chg_sc_f=chg_sc_f && ((2.0*v0<v2) && (v2>ChgSc_2));

	chg_sc_m1_f=chg_sc_m1_f && ((2.0*v0<v1) && (v1>ChgSc_2));

	if (v0<v1) v=v0;
	else v=v1;
	ivtc_p1_f=ivtc_p1_f && (((v>15.0*v2) && (v>IVTC_MIN_4)) || ((v2==0) && (v>IVTC_MIN_5)));

	if (v0<v2) v=v0;
	else v=v2;
	ivtc_m1_f=ivtc_m1_f && (((v>15.0*v1) && (v>IVTC_MIN_4)) || ((v1==0) && (v>IVTC_MIN_5)));

	if (v0>IVTC_MIN_2) s=7.0;
	else s=10.0;

	if (v0>IVTC_MIN_3)
	{
		/*ivtc_ok_p1=ivtc_ok_p1 && (((v2<=s*v0) && (v2<IVTC_MIN_6)) || (v2<0.8*v0));
		ivtc_ok_m1=ivtc_ok_m1 && ((v1<=s*v0) && (v1<IVTC_MIN_6)) || (v1<0.8*v0));*/
		ivtc_ok_p1=ivtc_ok_p1 && (v2<=s*v0);
		ivtc_ok_m1=ivtc_ok_m1 && (v1<=s*v0);
		if ((v0>IVTC_MIN_6) && (v0>2.8*v1) && (v0>2.8*v2))
		{
			ivtc_ok_p1=true;
			ivtc_ok_m1=true;
		}
	}
	else
	{
		if (v1<IVTC_MIN_1) ivtc_ok_m1=true;
		if (v2<IVTC_MIN_1) ivtc_ok_p1=true;
	}
	interlace_f=(v0>IVTC_MIN_7);
	interlace_m1_f=(v1>IVTC_MIN_7);
	interlace_p1_f=(v2>IVTC_MIN_7);
	ivtc_a=true;
	if ( ((20.0*v1<v2) && (6.0*v2>v0) && (v0>IVTC_MIN_1)) && !ivtc_f) ivtc_a=false;
/*	if ((v2>IVTC_MIN_1) && ((v0<IVTC_MIN_2) && (v1<IVTC_MIN_2))
		&& (v0>5.0*v1)) ivtc_a=false;*/
	if (ivtc_m1_f && !(ivtc_f || ivtc_motion_map_f)) ivtc_a=false;
	
	v0=buffer[i_rd_p1].error_Motion_Map;
	v1=buffer[i_rd_p1].error_IVTC_M1_Motion_Map;
	v2=buffer[i_rd_p1].error_IVTC_P1_Motion_Map;

/*	if (v0>v2) v=v0;
	else v=v2;
	chg_sc_f=chg_sc_f && ((2.0*v<v1) && (v1>ChgSc_2));*/

	chg_sc_f=chg_sc_f && ((2.0*v0<v1) && (v1>ChgSc_2));

	eg_ivtc_m1_f=((buffer[i_rd_p1].error==buffer[i_rd_p1].error_IVTC_M1) && (v0>0)
		&& (v1>0));

	rep128_f=(buffer[i_rd].repartition[128]>=Deinterlace_Min);

	interlace_f=interlace_f || rep128_f;

	v0=buffer[i_rd_m1].Delta_Top_P1;
	v1=buffer[i_rd_m1].Delta_Top_M1;
	v2=buffer[i_rd_m1].Delta_Bottom_M1;
	v3=buffer[i_rd_m1].Delta_Bottom_P1;
		
	if (inv_polar)
	{
		z_dbot_p1_f=(v3==0);
		z_dbot_m1_f=(v2==0);
	}
	else
	{
		z_dtop_p1_f=(v0==0);
		z_dtop_m1_f=(v1==0);
	}

	if (inv_polar)
	{
		if (v0<v1) v=v0;
		else v=v1;
		if (v2<v) v=v2;
		dbot_p1_f=((v3<0.8*v) && (v>DTopBot)); // 0.6

		if (v0<v1) v=v0;
		else v=v1;
		if (v3<v) v=v3;
		dbot_m1_m_f=((v2<0.8*v) && (v>DTopBot)); // 0.6
	}
	else
	{
		if (v1<v2) v=v1;
		else v=v2;
		if (v3<v) v=v3;
		dtop_p1_f=((v0<0.8*v) && (v>DTopBot)); // 0.6

		if (v0<v2) v=v0;
		else v=v2;
		if (v3<v) v=v3;
		dtop_m1_m_f=((v1<0.8*v) && (v>DTopBot)); // 0.6
	}

	v0=buffer[i_rd_m1].error;
	v1=buffer[i_rd_m1].error_IVTC_P1;
	v2=buffer[i_rd_m1].error_Motion_Map;
	v3=buffer[i_rd_m1].error_IVTC_P1_Motion_Map;
	eg_ivtc_p1_f=((v0==v1) && (v2>0) && (v3>0));

	v0=buffer[i_rd_m1].error_Motion_Map;
	v1=buffer[i_rd_m1].error_IVTC_M1_Motion_Map;
	v2=buffer[i_rd_m1].error_IVTC_P1_Motion_Map;

/*	if (v0>v1) v=v0;
	else v=v1;
	chg_sc_m1_f=chg_sc_m1_f && ((2.0*v<v2) && (v2>ChgSc_2));*/

	chg_sc_m1_f=chg_sc_m1_f && ((2.0*v0<v2) && (v2>ChgSc_2));

	v0=buffer[i_rd_m1].error;
	v1=buffer[i_rd_m1].error_IVTC_M1;
	v2=buffer[i_rd_m1].error_IVTC_P1;

/*	if (v0>v1) v=v0;
	else v=v1;
	chg_sc_m1_f=chg_sc_m1_f && ((1.8*v<v2) && (v2>ChgSc_1));*/

	chg_sc_m1_f=chg_sc_m1_f && ((1.8*v0<v2) && (v2>ChgSc_1));

	
	if (inv_polar)
	{
		v0=buffer[i_rd_m1].Delta_Top_M1;
		v1=buffer[i_rd].Delta_Top_M1;
		v2=buffer[i_rd_p1].Delta_Top_M1;
		pic_f=((v1>1.4*v0) && (v1>1.4*v2)); //3.0
		v0=buffer[i_rd_m1].Delta_Bottom_P1;
		v1=buffer[i_rd].Delta_Bottom_P1;
		v2=buffer[i_rd_p1].Delta_Bottom_P1;
		pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
		v0=buffer[i_rd_m2].Delta_Top_P1;
		v1=buffer[i_rd_m1].Delta_Top_P1;
		v2=buffer[i_rd].Delta_Top_P1;
		pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
		v0=buffer[i_rd].Delta_Bottom_M1;
		v1=buffer[i_rd_p1].Delta_Bottom_M1;
		v2=buffer[i_rd_p2].Delta_Bottom_M1;
		pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); //3.0
	}
	else
	{
		v0=buffer[i_rd_m1].Delta_Bottom_M1;
		v1=buffer[i_rd].Delta_Bottom_M1;
		v2=buffer[i_rd_p1].Delta_Bottom_M1;
		pic_f=((v1>1.4*v0) && (v1>1.4*v2)); //3.0
		v0=buffer[i_rd_m1].Delta_Top_P1;
		v1=buffer[i_rd].Delta_Top_P1;
		v2=buffer[i_rd_p1].Delta_Top_P1;
		pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
		v0=buffer[i_rd_m2].Delta_Bottom_P1;
		v1=buffer[i_rd_m1].Delta_Bottom_P1;
		v2=buffer[i_rd].Delta_Bottom_P1;
		pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); // 3.0
		v0=buffer[i_rd].Delta_Top_M1;
		v1=buffer[i_rd_p1].Delta_Top_M1;
		v2=buffer[i_rd_p2].Delta_Top_M1;
		pic_f=pic_f && ((v1>1.4*v0) && (v1>1.4*v2)); //3.0
	}

	v0=buffer[i_rd_m1].Delta_error_M1;
	v1=buffer[i_rd_p1].Delta_error_P1;
	if (v0>v1) v2=v0;
	else v2=v1;
	v0=buffer[i_rd].Delta_error_M1;
	v1=buffer[i_rd].Delta_error_P1;
	if (v0<v1) v3=v0;
	else v3=v1;
	error_f=(v2<0.8*v3); //0.6

	v0=buffer[i_rd_m2].Delta_Top_P1;
	v1=buffer[i_rd_m2].Delta_Bottom_M1;
	v2=buffer[i_rd_m2].Delta_Bottom_P1;
	v3=buffer[i_rd_m2].Delta_Top_M1;
	if (v1<v2) v=v1;
	else v=v2;
	if (v3<v) v=v3;
	dtop_p1_m_f=((v0<0.8*v) && (v>DTopBot)); //0.6
	if (v0<v1) v=v0;
	else v=v1;
	if (v3<v) v=v3;
	dbot_p1_m_f=((v2<0.8*v) && (v>DTopBot)); //0.6


	v0=buffer[i_rd].Delta_Top_M1;
	v1=buffer[i_rd].Delta_Bottom_P1;
	v2=buffer[i_rd].Delta_Top_P1;
	v3=buffer[i_rd].Delta_Bottom_M1;

	if (inv_polar)
	{
		if (v0<v1) v=v0;
		else v=v1;
		if (v2<v) v=v2;
		dbot_m1_f=((v3<0.8*v) && (v>DTopBot)); //0.6

		if (v0<v1) v=v0;
		else v=v1;
		if (v3<v) v=v3;
		dtop_p1_f=((v2<0.8*v)&& (v>DTopBot)); //0.6
	}
	else
	{
		if (v1<v2) v=v1;
		else v=v2;
		if (v3<v) v=v3;
		dtop_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6

		if (v0<v2) v=v0;
		else v=v2;
		if (v3<v) v=v3;
		dbot_p1_f=((v1<0.8*v)&& (v>DTopBot)); //0.6
	}

	if (v1<v2) v=v1;
	else v=v2;
	if (v3>v0) v4=v3;
	else v4=v0;
	static_m1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));

	if (v3<v0) v=v3;
	else v=v0;
	if (v1>v2) v4=v1;
	else v4=v2;
	static_p1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));

//	static_f=((v0<Static) && (v1<Static) && (v2<Static) && (v3<Static));

	v0=buffer[i_rd_p1].Delta_Bottom_M1;
	v1=buffer[i_rd_p1].Delta_Bottom_P1;
	v2=buffer[i_rd_p1].Delta_Top_M1;
	v3=buffer[i_rd_p1].Delta_Top_P1;

	if (inv_polar)
	{
		z_dtop_m1_f=(v2==0);
		z_dtop_p1_f=(v3==0);
	}
	else
	{
		z_dbot_m1_f=(v0==0);
		z_dbot_p1_f=(v1==0);
	}

	if (inv_polar)
	{
		if (v0<v1) v=v0;
		else v=v1;
		if (v3<v) v=v3;
		dtop_m1_f=((v2<0.8*v) && (v>DTopBot)); //0.6

		if (v0<v1) v=v0;
		else v=v1;
		if (v2<v) v=v2;
		dtop_p1_p_f=((v3<0.8*v) && (v>DTopBot));  //0.6
	}
	else
	{
		if (v1<v2) v=v1;
		else v=v2;
		if (v3<v) v=v3;
		dbot_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6

		if (v0<v2) v=v0;
		else v=v2;
		if (v3<v) v=v3;
		dbot_p1_p_f=((v1<0.8*v) && (v>DTopBot));  //0.6
	}

	v0=buffer[i_rd_p2].Delta_Bottom_M1;
	v1=buffer[i_rd_p2].Delta_Bottom_P1;
	v2=buffer[i_rd_p2].Delta_Top_M1;
	v3=buffer[i_rd_p2].Delta_Top_P1;

	if (v1<v2) v=v1;
	else v=v2;
	if (v3<v) v=v3;
	dbot_m1_p_f=((v0<0.8*v) && (v>DTopBot)); //0.6*/
	if (v0<v1) v=v0;
	else v=v1;
	if (v3<v) v=v3;
	dtop_m1_p_f=((v2<0.8*v) && (v>DTopBot)); //0.6*/

	if (inv_polar)
		dtopbot_f=( ((dbot_m1_f && dbot_p1_f) && (dtop_m1_p_f || dtop_p1_p_f)) &&
			!static_m1_f && !chg_sc_f) || ( ((dtop_p1_f && dtop_m1_f) &&
			(dbot_m1_m_f || dbot_p1_m_f)) && !static_p1_f && !chg_sc_m1_f);
	else
		dtopbot_f=( ((dtop_m1_f && dtop_p1_f) && (dbot_m1_p_f || dbot_p1_p_f)) &&
			!static_m1_f && !chg_sc_f) || ( ((dbot_p1_f && dbot_m1_f) &&
			(dtop_m1_m_f || dtop_p1_m_f)) && !static_p1_f && !chg_sc_m1_f);

/*	if (inv_polar)
	{
		ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && (dbot_p1_f && dbot_m1_f) && chg_sc_f;
		ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && (dtop_p1_f && dtop_m1_f) && chg_sc_m1_f;
	}
	else
	{
		ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && (dtop_p1_f && dtop_m1_f) && chg_sc_f;
		ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && (dbot_p1_f && dbot_m1_f) && chg_sc_m1_f;
	}*/

/*	if (inv_polar)
	{
		ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && dbot_m1_f && chg_sc_f;
		ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && dtop_p1_f && chg_sc_m1_f;
	}
	else
	{
		ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && dtop_m1_f && chg_sc_f;
		ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && dbot_p1_f && chg_sc_m1_f;
	}*/

	ivtc_motion_map_m1_f=ivtc_motion_map_m1_f && chg_sc_f;
	ivtc_motion_map_p1_f=ivtc_motion_map_p1_f && chg_sc_m1_f;

	if (inv_polar)
		ivtc_detect=(ivtc_motion_map_f || eg_ivtc_p1_f || eg_ivtc_m1_f ||
			(z_dbot_p1_f && !static_m1_f) || (z_dtop_m1_f && !static_p1_f) ||
			(z_dbot_m1_f && !static_p1_f && !chg_sc_m1_f) ||
			(z_dtop_p1_f && !static_m1_f && !chg_sc_f) || dtopbot_f || ivtc_motion_map_p1_f
			|| ivtc_motion_map_m1_f || ivtc_p1_f || ivtc_m1_f);
	else
		ivtc_detect=(ivtc_motion_map_f || eg_ivtc_p1_f || eg_ivtc_m1_f ||
			(z_dtop_p1_f && !static_m1_f) || (z_dbot_m1_f && !static_p1_f) ||
			(z_dtop_m1_f && !static_p1_f && !chg_sc_m1_f) ||
			(z_dbot_p1_f && !static_m1_f && !chg_sc_f) || dtopbot_f || ivtc_motion_map_p1_f
			|| ivtc_motion_map_m1_f || ivtc_p1_f || ivtc_m1_f);

	if (ivtc_motion_map_m1_f) ivtc_a=false;
	if (ivtc_a) ivtc_ok=ivtc_ok_p1 && ivtc_detect;
	else ivtc_ok=ivtc_ok_m1 & ivtc_detect;

/*	fprintf(fic1,"Image : %d\n",pxsrc.mFrameNumber+7-3);
	if (dbot_m1_f) fprintf(fic1,"DBOT-1 ");
	if (dbot_p1_f) fprintf(fic1,"DBOT+1 ");
	if (dtop_p1_f) fprintf(fic1,"DTOP+1 ");
	if (dtop_m1_f) fprintf(fic1,"DTOP-1 ");
	if (z_dtop_p1_f) fprintf(fic1,"ZDTOP+1 ");
	if (z_dbot_m1_f) fprintf(fic1,"ZDBOT-1 ");
	if (z_dtop_m1_f) fprintf(fic1,"ZDTOP-1 ");
	if (z_dbot_p1_f) fprintf(fic1,"ZDBOT+1 ");
	if (dtopbot_f) fprintf(fic1,"DTopBot ");
	if (eg_ivtc_p1_f) fprintf(fic1,"ZIVTC+1 ");
	if (eg_ivtc_m1_f) fprintf(fic1,"ZIVTC-1 ");
	if (ivtc_f) fprintf(fic1,"IVTC ");
	if (ivtc_motion_map_f) fprintf(fic1,"IVTCMap ");
	if (ivtc_p1_f) fprintf(fic1,"IVTC+1 ");
	if (ivtc_m1_f) fprintf(fic1,"IVTC-1 ");
	if (chg_sc_f) fprintf(fic1,"ChgSc ");
	if (chg_sc_m1_f) fprintf(fic1,"ChgSc-1 ");
	if (ivtc_detect) fprintf(fic1,"IVTCDetect ");
	if (!ivtc_ok_p1) fprintf(fic1,"IVTCNul ");*/

	buffer[i_rd].flags.ivtc=ivtc_f;
	buffer[i_rd].flags.ivtc_map=ivtc_motion_map_f;
	buffer[i_rd].flags.pic=pic_f;
	buffer[i_rd].flags.error=error_f;
	buffer[i_rd].flags.rev=!ivtc_a;
	buffer[i_rd].flags.interlace=interlace_f;
	buffer[i_rd].flags.interlace_m1=interlace_m1_f;
	buffer[i_rd].flags.interlace_p1=interlace_p1_f;
	if (inv_polar)
	{
		buffer[i_rd].flags.first=(eg_ivtc_p1_f || z_dbot_p1_f || (z_dtop_p1_f && !chg_sc_f)
			|| (((dbot_m1_f && dbot_p1_f) && (dtop_m1_p_f || dtop_p1_p_f)) && !static_m1_f && !chg_sc_f));
		buffer[i_rd].flags.last=(eg_ivtc_m1_f || z_dtop_m1_f || (z_dbot_m1_f && !chg_sc_m1_f)
			|| (((dtop_p1_f && dtop_m1_f) && (dbot_m1_m_f || dbot_p1_m_f)) && !static_p1_f && !chg_sc_m1_f));
		buffer[i_rd].flags.Z=(eg_ivtc_p1_f || eg_ivtc_m1_f || z_dbot_p1_f || z_dtop_m1_f ||
			(z_dtop_p1_f && !chg_sc_f) || (z_dbot_m1_f && !chg_sc_m1_f));
	}
	else
	{
		buffer[i_rd].flags.first=(eg_ivtc_p1_f || z_dtop_p1_f || (z_dbot_p1_f && !chg_sc_f)
			|| (((dtop_m1_f && dtop_p1_f) && (dbot_m1_p_f || dbot_p1_p_f)) && !static_m1_f && !chg_sc_f));
		buffer[i_rd].flags.last=(eg_ivtc_m1_f || z_dbot_m1_f || (z_dtop_m1_f && !chg_sc_m1_f) || 
			(((dbot_p1_f && dbot_m1_f) && (dtop_m1_m_f || dtop_p1_m_f)) && !static_p1_f && !chg_sc_m1_f));
		buffer[i_rd].flags.Z=(eg_ivtc_p1_f || eg_ivtc_m1_f || z_dtop_p1_f || z_dbot_m1_f
			|| (z_dbot_p1_f && !chg_sc_f) || (z_dtop_m1_f && !chg_sc_m1_f));
	}
	buffer[i_rd].flags.chg_sc_p1=chg_sc_f;
	buffer[i_rd].flags.chg_sc=chg_sc_m1_f;
	buffer[i_rd].flags.dtopbot=dtopbot_f;
	buffer[i_rd].flags.ivtc_valid=ivtc_ok;
	buffer[i_rd].flags.p1_f=ivtc_ok_p1;
	buffer[i_rd].flags.m1_f=ivtc_ok_m1;

/*	if (buffer[i_rd].flags.Z) fprintf(fic1,"Z ");
	if (buffer[i_rd].flags.first) fprintf(fic1,"First ");
	if (buffer[i_rd].flags.last) fprintf(fic1,"Last ");
	if ((first)!=5) fprintf(fic1,"F:(%d) ",first);
	if ((last)!=5) fprintf(fic1,"L:(%d) ",last);
	fprintf(fic1,"\n\n");*/
	
	for (i=0; i<Flag_Index_Size; i++)
		flag_index[i]=(flag_index[i]+1)%Data_Buffer_Size;

	if (pxsrc.mFrameNumber==0) out=true;

	if (out)
	{
		v1=0;	// v1 : Nombre de changement de scène
		for (i=0; i<5; i++)
		{
			//tab_error[i]=buffer[read_index[i]].error_Motion_Map;
			tab_error[i]=buffer[read_index[i]].error;
			if (buffer[read_index[i]].flags.chg_sc) v1++;
		}
		for (i=0; i<5; i++)
		{
			v=tab_error[i];
			for (j=i+1; j<5; j++)
				if (v>tab_error[j])
				{
					v=tab_error[j];
					tab_error[j]=tab_error[i];
					tab_error[i]=v;
				}
		}
		i_rd_p1=0;	//i_rd_p1 = indice ayant l'erreur de corrélation maximale
		//while (tab_error[4]!=buffer[read_index[i_rd_p1]].error_Motion_Map) i_rd_p1++;
		while (tab_error[4]!=buffer[read_index[i_rd_p1]].error) i_rd_p1++;

		i_rd=5;	//Trame qui sera suprimé. La valeur 5 => Trame non déterminée.

/*		v=0;
		if ((tab_error[4]>4.0*tab_error[2]) && (tab_error[4]>IVTC_MIN_2)) v++;
		if ((tab_error[3]>4.0*tab_error[2]) && (tab_error[3]>IVTC_MIN_2)) v++;
		if ((v==1) && (v1==0)) i_rd=i_rd_p1;
		if ((v==2) && (v1==0))
		{
			i=4;
			while (tab_error[3]!=buffer[read_index[i]].error_Motion_Map) i--;
			if (((i_rd_p1==0) && (i==4)) || ((i_rd_p1==4) && (i==0))) i_rd=0;
			else
			{
				if (i<i_rd_p1) i_rd=i_rd_p1;
				else i_rd=i;
			}
		}*/
	
		stable_ok=false;

		v=0;
		v2=0;
		v3=0;
		v4=0;
		for (i=0; i<5; i++)
		{
			if (buffer[read_index[i]].flags.ivtc_valid) v++;
			if (buffer[read_index[i]].flags.first) v2++;
			if (buffer[read_index[i]].flags.last) v3++;
			if (buffer[read_index[i]].flags.Z) v4++;
		}

		// Si : pas de changement de scène, et debut et/ou fin pas encore trouvé,
		// et trame correspondante détectée, de façon unique.
		if ((v1==0) && ((v2<=1) && (v3<=1)) && ((first==5) || (last==5)))
		{
			if ((v2==1) && (first==5))
			{
				i=0;
				while (!buffer[read_index[i]].flags.first) i++;
				first=i;
			}
			if ((v3==1) && (last==5))
			{
				i=0;
				while (!buffer[read_index[i]].flags.last) i++;
				last=i;
			}
		}

		// Si on n'est pas en mode séquence stable, mais détection d'un début ou d'une fin,
		// pas de chg scène.
		if ((!mData.first_last_mode) && (v1==0) && ((v2<=1) && (v3<=1)))
		{
			if (v2==1)
			{
				i=0;
				while (!buffer[read_index[i]].flags.first) i++;
				first=i;
			}
			if (v3==1)
			{
				i=0;
				while (!buffer[read_index[i]].flags.last) i++;
				last=i;
			}
		}


		// Si : pas de changement de scène, et les trames détectées sont assez
		// robustes : 1 seule de début, une seule de fin, 2 trames maximum satisfont
		// le zéro [et 2 la validation IVTC], et si présentes, ne sont que sur les trames
		// détectées début/fin, et bien sûr, cohérance du début et de la fin.
		// Alors, séquence reforcée sur nouvelle détection.
		// Ceci permet de recaler même en mode séquence stable en cas de ratage
		// de détection de changement de séquence (principalement un fondu).
//		if ( (v1==0) && ((v2==1) && (v3==1) && (v4<=2) && (v<=2)) )
//		if ( (v1==0) && ((v2==1) && (v3==1) && (v4<=2)) )
		if ((i_rd==5) && (v1==0) && ((v2==1) && (v3==1) && (v4<=2)))
		{
			i=0;
			while (!buffer[read_index[i]].flags.first) i++;
			j=0;
			while (!buffer[read_index[j]].flags.last) j++;
			test_ok=(j==((i+1)%5));
			for (k=0; k<5; k++)
			{
				if (k==i) continue;
				if (k==j) continue;
/*				test_ok=test_ok && (!buffer[read_index[k]].flags.Z)
					&& (!buffer[read_index[k]].flags.ivtc_valid);*/
				test_ok=test_ok && (!buffer[read_index[k]].flags.Z);
			}

			if (test_ok)
			{
				if (mData.first_last_mode)
				{
					for (k=0; k<5; k++)
						buffer[read_index[k]].flags.ivtc_valid=false;
				}
				first=i;
				last=j;
				i_rd=j;
				buffer[read_index[i]].flags.ivtc_valid=true;
				buffer[read_index[j]].flags.ivtc_valid=true;
				stable_ok=true;
				invalid_first_last=false;

//				fprintf(fic3,"Image : %d  [Recalage forcé]\n",pxsrc.mFrameNumber);

			}
		}




		// Si changement de scène ou pas en mode séquence stable, et plus de 2 trames
		// satisfont le zéro ou plus d'une début ou plus d'une fin,
		// RAZ de la détection début/fin.
		if ((v1!=0) || ( (!mData.first_last_mode) && ((v2>1) || (v3>1) || (v4>2)) ) )
		{
			old_first=first;
			old_last=last;
			first=5;
			last=5;
			invalid_first_last=false;
		}


/*			
		// Si : pas de changement de scène, et détection d'un débout et/ou fin,
		// et trame correspondante détectée, de façon unique au plus.

		if ((v1==0) && ((v2<=1) && (v3<=1)))
		{
			if (v2==1)
			{
				i=0;
				while (!buffer[read_index[i]].flags.first) i++;
			}
			else i=5;

			if (v3==1)
			{
				j=0;
				while (!buffer[read_index[j]].flags.last) j++;
			}
			else j=5;

			if (j==5)
			{
				if (first!=5)
				{
					if (first!=i)
					{
						first=i;
						last=5;
					}
				}
				else
				{
					if (last!=5)
					{
						if (last!=((i+1)%5)) last=5;
					}
					first=i;
				}
			}
			else
			{
				if (i!=5)
				{
					if (j==((i+1)%5))
					{
						first=i;
						last=j;
					}
				}
				else
				{
					if (last!=5)
					{
						if (last!=j)
						{
							first=5;
							last=j;
						}
					}
					else
					{
						if (first!=5)
						{
							if (first!=((j+4)%5)) first=5;
							last=j;
						}
					}
				}
			}
		}
		*/


		// si pas de changement de scène, et détection d'un début et d'une fin.
		if ((v1==0) && (v2==1) && (v3==1)) i_rd_p2=1;
		else i_rd_p2=0;


		// Si changement de scène, possibilité de détecter tout de même un début ou une fin.
		// La trame enlevée sera, si possible, placée avant le changement de scène,
		// si l'ancienne séquence téléciné le permet.
		if ((v1!=0) && (i_rd==5))
		{
			// k : Ici, indice du 1er changement de scène.
			k=0;
			while (!buffer[read_index[k]].flags.chg_sc) k++;

			if ( (k>0) && ((old_first!=5) || (old_last!=5)) )
			{
				if (mData.first_last_mode)
				{
					for (i=0; i<k; i++)
						buffer[read_index[i]].flags.ivtc_valid=false;
				}

				if ((old_last==0) || (old_first==4))
				{
					buffer[read_index[0]].flags.ivtc_valid=true;
					i_rd=0;
				}
				else
				{
					if (old_last!=5)
					{
						if ((old_last-1)<k)
						{
							 buffer[read_index[old_last-1]].flags.ivtc_valid=true;
/*							 if (old_last==k)
								 buffer[read_index[old_last-1]].flags.rev=true;*/
						}
						if (old_last<k)
						{
							 buffer[read_index[old_last]].flags.ivtc_valid=true;
							 i_rd=old_last;
						}
					}
					else
					{
						if (old_first<k)
						{
							 buffer[read_index[old_first]].flags.ivtc_valid=true;
/*							 if ((old_first+1)==k)
								 buffer[read_index[old_first]].flags.rev=true;*/
						}
						if ((old_first+1)<k)
						{
							 buffer[read_index[old_first+1]].flags.ivtc_valid=true;
							 i_rd=old_first+1;
						}
					}
				}

//				if (i_rd!=5) fprintf(fic3,"Image : %d  [ChgSc(%d),IVTC f/l(%d) avant]\n",pxsrc.mFrameNumber,k,i_rd);
			}

			// k : Ici, indice du dernier changement de scène.
			k=4;
			while (!buffer[read_index[k]].flags.chg_sc) k--;

			v_b=0;
			v2_b=0;
			v3_b=0;
			for (i=k; i<5; i++)
			{
				if (buffer[read_index[i]].flags.ivtc_valid) v_b++;
				if (buffer[read_index[i]].flags.first) v2_b++;
				if (buffer[read_index[i]].flags.last) v3_b++;
			}

			if ((v2_b<=1) && (v3_b<=1) && ((v2_b+v3_b)>0))
			{
				test_ok=true;
				for (i=k; i<5; i++)
				{
					if ((!buffer[read_index[i]].flags.first) &&
						(!buffer[read_index[i]].flags.last))
						test_ok=test_ok && (!buffer[read_index[i]].flags.Z);
				}

				if (test_ok)
				{
					if (v2_b==1)
					{
						l=k;
						while (!buffer[read_index[l]].flags.first) l++;
						first=l;
					}
					if (v3_b==1)
					{
						l=k;
						while (!buffer[read_index[l]].flags.last) l++;
						last=l;
					}

					if ((first!=5) && (last!=5))
						test_ok=test_ok && (last==((first+1)%5));

					if (!test_ok)
					{
						first=5;
						last=5;
					}
				}
			}
			else
			{
				if (v_b==2)
				{
					l=k;
					while (!buffer[read_index[l]].flags.ivtc_valid) l++;
					i=l;
					l=4;
					while (!buffer[read_index[l]].flags.ivtc_valid) l--;
					j=l;
					test_ok=( (j==(i+1)) || ((i==k) && (j==4)) );
					for (l=k; l<5; l++)
					{
						if ((l==i) || (l==j)) continue;
						test_ok=test_ok && !buffer[read_index[l]].flags.Z;
					}
				}
			}

			if (test_ok)
			{
				if ((first!=5) || (last!=5))
				{
					if (mData.first_last_mode)
					{
						for (l=k; l<5; l++)
							buffer[read_index[l]].flags.ivtc_valid=false;
					}

					if (last!=5)
					{
						buffer[read_index[last]].flags.ivtc_valid=true;
						l=(last+4)%5;
						if (k<=l)
							buffer[read_index[l]].flags.ivtc_valid=true;
					}
					else
					{
						buffer[read_index[first]].flags.ivtc_valid=true;
						l=(first+1)%5;
						if (k<=l)
							buffer[read_index[l]].flags.ivtc_valid=true;
					}

					if (i_rd==5)
					{
						if (last!=5) i_rd=last;
						else
						{
							l=(first+1)%5;
							if (k<=l) i_rd=l;
							else i_rd=first;
						}

//						fprintf(fic3,"Image : %d  [Chgsc(%d),IVTC f/l(%d) après]\n",pxsrc.mFrameNumber,k,i_rd);
					}

				}
				else
				{
					if (i_rd==5)
					{
						if ((j==4) && (i!=3)) i_rd=i;
						else i_rd=j;

//						fprintf(fic3,"Image : %d  [Chgsc(%d),IVTC(%d) après\n",pxsrc.mFrameNumber,k,i_rd);
					}

				}

			}

			if (i_rd!=5) stable_ok=true;
		}
		

		// Si détection d'un début ET d'une fin, validation de la cohérance.
		if ((first!=5) && (last!=5))
		{
			if (last!=(first+1)%5)
			{
				first=5;
				last=5;
				invalid_first_last=false;
			}
		}

		// Si changement de scène, RAZ de la mémorisation.
		if (v1!=0)
		{
			last_removed_1=5;
			last_removed_2=5;
		}

		// Si : pas chg scène, mode séquence stable, et IVTC détectés
		// non cohérant avec les first/last.
		// Pour les chg scènes non détectés.......
		if ((i_rd==5) && (v1==0) && (v<=2) && ((v2==0) && (v3==0)) 
			&& ((first!=5) || (last!=5)))
		{
			test_ok=true;

			for (i=0; i<5; i++)
			{
				if (buffer[read_index[i]].flags.ivtc_valid) continue;
				test_ok=test_ok && (!buffer[read_index[i]].flags.Z);
			}

			if (v==0)
			{
				if (first!=5)
				{
					i=first;
					j=(first+1)%5;
				}
				else
				{
					i=(last+4)%5;
					j=last;
				}

				if ((!buffer[read_index[i]].flags.p1_f) ||
					(!buffer[read_index[j]].flags.p1_f))
					invalid_first_last=true;

				if (invalid_first_last)
				{
					if (last_removed_1!=5) i_rd=last_removed_1;
					else i_rd=j;

//					fprintf(fic3,"Image : %d [IVTC(0) déphasé F/L]\n",pxsrc.mFrameNumber);
				}
			}

			if (v==1)
			{
				i=0;
				while (!buffer[read_index[i]].flags.ivtc_valid) i++;

				if (first!=5)
					test_ok=test_ok && ((i!=first) && (i!=((first+1)%5)));
				else
					test_ok=test_ok && ((i!=last) && (i!=((last+4)%5)));
						
				if (test_ok)
				{
					i_rd=i;
					invalid_first_last=true;

//					fprintf(fic3,"Image : %d [IVTC(1) déphasé F/L]\n",pxsrc.mFrameNumber);
				}
			}
				
			if (v==2)
			{
				i=0;
				while (!buffer[read_index[i]].flags.ivtc_valid) i++;
				j=4;
				while (!buffer[read_index[j]].flags.ivtc_valid) j--;
				if ((i==0) && (j==4))
				{
					i=4;
					j=0;
				}

				test_ok=test_ok && (j==((i+1)%5));
				if (first!=5)
					test_ok=test_ok && ((i!=first) && (j!=((first+1)%5)));
				else
					test_ok=test_ok && ((i!=(last+4)%5) && (j!=last));

				if (test_ok)
				{
					i_rd=j;
					stable_ok=true;
					invalid_first_last=true;

//					fprintf(fic3,"Image : %d [IVTC(2) déphasé F/L]\n",pxsrc.mFrameNumber);
				}
			}
		}

		// Si mode séquence stable, pas de changement de scène, et au moins une trame
		// a été détectée comme début ou fin, on fixe quelle sera la séquence téléciné.
		if ((mData.first_last_mode) && (v1==0) && (i_rd==5) && ((first!=5) || (last!=5)))
		{
			stable_ok=true;

			if (first==5) i=(last+4)%5;
			else i=first;
			if (last==5) j=(first+1)%5;
			else j=last;

			for (k=0; k<5; k++)
				buffer[read_index[k]].flags.ivtc_valid=false;

			i_rd=j;
/*			if ((!buffer[read_index[i]].flags.rev &&
				buffer[read_index[i]].flags.p1_f) ||
				(buffer[read_index[i]].flags.rev &&
				buffer[read_index[i]].flags.m1_f) ||
				buffer[read_index[i]].flags.Z)*/

			buffer[read_index[i]].flags.ivtc_valid=true;
			buffer[read_index[j]].flags.ivtc_valid=true;

//			fprintf(fic3,"Image : %d [First/Last]\n",pxsrc.mFrameNumber);

		}

		// Si la trame a lever n'est pas déterminée, qu'on a détecté au moins une
		// trame téléciné, mais plus d'un début, ou de fin, ou de Zéro.
		if ((i_rd==5) && ((v2>1) || (v3>1) || (v4>2)) && (v>0))
		{
			i=0;
			while ((i<5) && ((buffer[read_index[i]].flags.first ||
				buffer[read_index[i]].flags.last || buffer[read_index[i]].flags.Z)
				|| !buffer[read_index[i]].flags.ivtc_valid)) i++;
			if (i==5)
			{
				i=0;
				while ((i<5) && (buffer[read_index[i]].flags.Z ||
					!buffer[read_index[i]].flags.ivtc_valid)) i++;
				if (i==5)
				{
					i=0;
					while ((i<5) && ((buffer[read_index[i]].flags.first || 
						buffer[read_index[i]].flags.last) ||
						!buffer[read_index[i]].flags.ivtc_valid)) i++;
					if (i<5) i_rd=i;
				}
				else i_rd=i;
			}
			else i_rd=i;

//			if (i_rd!=5) fprintf(fic3,"Image : %d  [f/l,Z > 2]\n",pxsrc.mFrameNumber);

		}

		// Si le mode Zéro est actif, rien trouvé, 2 zéros, pas de chang.scène.
		if ((!disable_Z_mode) && (i_rd==5) && (v1==0) && (v4==2))
		{
			i=0;
			while (!buffer[read_index[i]].flags.Z) i++;
			j=4;
			while (!buffer[read_index[j]].flags.Z) j--;

			if (((i==0) && (j==4)) || (j==i+1))
			{
				if ((j==4) && (i==0))
				{
					i_rd=0;
					i=4;
				}
				else i_rd=j;

				for (k=0; k<5; k++)
					if (k!=i) buffer[read_index[k]].flags.ivtc_valid=false;

				buffer[read_index[i]].flags.ivtc_valid=true;

				stable_ok=true;

//				fprintf(fic3,"Image : %d  [Z=2]\n",pxsrc.mFrameNumber);

			}
		}

		// Si la trame à lever n'est pas trouvée, mais détection d'une (ou +) trame téléciné.
		if ((i_rd==5) && (v>=1))
		{
			if (v==1)
			{
				i=0;
				while (!buffer[read_index[i]].flags.ivtc_valid) i++;

				if ((first!=5) || (last!=5))
				{
					if (first!=5)
					{
						if ((i==first) || (i==((first+1)%5))) i_rd=(first+1)%5;
					}
					else
					{
						if ((i==last) || (i==((last+4)%5))) i_rd=last;
					}
				}

				if (i_rd==5) i_rd=i;
			}

			if (v==2)
			{
				if ((last!=5) || (first!=5))
				{
					if (last!=5)
					{
						if ((buffer[read_index[last]].flags.ivtc_valid) &&
							(buffer[read_index[(last+4)%5]].flags.ivtc_valid))
							i_rd=last;
					}
					else
					{
						if ((buffer[read_index[first]].flags.ivtc_valid) &&
							(buffer[read_index[(first+1)%5]].flags.ivtc_valid))
							i_rd=(first+1)%5;
					}
				}

				if (i_rd==5)
				{
					i=0;
					while(!buffer[read_index[i]].flags.ivtc_valid) i++;
					j=4;
					while(!buffer[read_index[j]].flags.ivtc_valid) j--;

					if ((i==0) && (j==4)) i_rd=0;
					else i_rd=j;

				}
			}

			if (v>2)
			{
				if ((last!=5) && buffer[read_index[last]].flags.ivtc_valid)
					i_rd=last;
				if (i_rd==5)
				{
					i=0;
					while ((i<5) && (!buffer[read_index[i]].flags.rev)) i++;
					if ((i<5) && (!buffer[read_index[i]].flags.ivtc_valid)) i=5;
					if (i==5)
					{
						i_rd=0;
						while (!buffer[read_index[i_rd]].flags.ivtc_valid) i_rd++;
					}
					else i_rd=i;
				}
			}

//			if (i_rd!=5) fprintf(fic3,"Image : %d  [Trames IVTC>0]\n",pxsrc.mFrameNumber);
		}

//		if (i_rd==5) fprintf(fic3,"Image : %d  [Rien, plan B]\n",pxsrc.mFrameNumber);

		if ((i_rd==5) && (v==0))
		{
			v0=0;
			v1=0;
			v2=0;
			v3=0;
			v4=0;
			for (i=0; i<5; i++)
			{
				if (buffer[read_index[i]].flags.interlace) v0++;
				if (buffer[read_index[i]].flags.pic) v1++;
				if (buffer[read_index[i]].flags.error) v2++;
				if (buffer[read_index[i]].flags.error &&
					buffer[read_index[i]].flags.pic) v3++;
				if (buffer[read_index[i]].flags.ivtc ||
					buffer[read_index[i]].flags.ivtc_map) v4++;
			}

			if ((i_rd==5) && (last!=5)) i_rd=last;
			if ((i_rd==5) && (first!=5)) i_rd=(first+1)%5;

			if ((i_rd==5) && (v4>0))
			{
				i_rd=0;
				while ((i_rd<5) && !buffer[read_index[i_rd]].flags.ivtc_map) i_rd++;
			}
			if ((i_rd==5) && (v4>0))
			{
				i_rd=0;
				while ((i_rd<5) && !buffer[read_index[i_rd]].flags.ivtc) i_rd++;
			}

			if (!disable_deinterlace && (i_rd==5) && (v0>0))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.interlace) i_rd++;
			}

			if ((i_rd==5) && ((last_removed_1!=5) || (last_removed_2!=5)))
			{
				if ((last_removed_1!=5) && (last_removed_2!=5))
				{
					if (last_removed_1<last_removed_2)
					{
						i=last_removed_1;
						j=last_removed_2;
					}
					else
					{
						j=last_removed_1;
						i=last_removed_2;
					}
					if (((i==0) && (j==4)) || (j==(i+1)%5))
					{
						if ((i==0) && (j==4)) i_rd=i;
						else i_rd=j;
					}
					if (i_rd==5) i_rd=last_removed_1;
				}
				else
				{
					if (last_removed_1!=5) i_rd=last_removed_1;
					else i_rd=last_removed_2;
				}
			}

			if ((i_rd==5) && (v1==2))
			{
				if (buffer[read_index[0]].flags.pic &&
					buffer[read_index[4]].flags.pic)
				{
					i_rd=0;
					buffer[read_index[4]].flags.ivtc_valid=true;
				}
			}
			if ((i_rd==5) && (v3==1))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.error || 
					!buffer[read_index[i_rd]].flags.pic) i_rd++;
			}
			if ((i_rd==5) && (v3>1) && (last!=5))
			{
				i_rd=0;
				while ((i_rd<5) && (!buffer[read_index[i_rd]].flags.error ||
					!buffer[read_index[i_rd]].flags.pic) && (i_rd!=last)) i_rd++;
			}
			if ((i_rd==5) && (v3>1) && (first!=5))
			{
				i_rd=0;
				while ((i_rd<5) && (!buffer[read_index[i_rd]].flags.error ||
					!buffer[read_index[i_rd]].flags.pic) && (i_rd!=first)) i_rd++;
			}
			if ((i_rd==5) && (v3>1))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.error ||
					!buffer[read_index[i_rd]].flags.pic) i_rd++;
			}

			if ((i_rd==5) && (v1==1))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.pic) i_rd++;
			}
			if ((i_rd==5) && (v1>1) && (last!=5))
			{
				i_rd=0;
				while ((i_rd<5) && !buffer[read_index[i_rd]].flags.pic 
					&& (i_rd!=last)) i_rd++;
			}
			if ((i_rd==5) && (v1>1) && (first!=5))
			{
				i_rd=0;
				while ((i_rd<5) && !buffer[read_index[i_rd]].flags.pic &&
					(i_rd!=first)) i_rd++;
			}
			if ((i_rd==5) && (v1>1))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.pic) i_rd++;
			}

			if ((i_rd==5) && (v2==1))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.error) i_rd++;
			}
			if ((i_rd==5) && (v2>1) && (last!=5))
			{
				i_rd=0;
				while ((i_rd<5) && !buffer[read_index[i_rd]].flags.error 
					&& (i_rd!=last)) i_rd++;
			}
			if ((i_rd==5) && (v2>1) && (first!=5))
			{
				i_rd=0;
				while ((i_rd<5) && !buffer[read_index[i_rd]].flags.error &&
					(i_rd!=first)) i_rd++;
			}
			if ((i_rd==5) && (v2>1))
			{
				i_rd=0;
				while (!buffer[read_index[i_rd]].flags.error) i_rd++;
			}

			if (i_rd==5) i_rd=i_rd_p1;

/*			{
				if (last<5) i_rd=last;
				if ((i_rd==5) && (first<5)) i_rd=first;
				if (i_rd==5) i_rd--;
			}*/
		}

		if (!stable_ok)
		{
			if (((first!=5) || (last!=5)) && (!invalid_first_last))
			{
				if (first!=5)
				{
					if ((first==i_rd) || (first==(i_rd+4)%5) &&
						(buffer[read_index[first]].flags.p1_f))
						buffer[read_index[first]].flags.ivtc_valid=true;
				}
				else
				{
					if (last!=5)
					{
						if ((last==i_rd) || (last==(i_rd+1)%5)
							&& (buffer[read_index[(last+4)%5]].flags.p1_f))
							buffer[read_index[(last+4)%5]].flags.ivtc_valid=true;
					}
				}
			}
			else
			{
				if ((last_removed_1!=5) && (last_removed_2!=5))
				{
					if (last_removed_1<last_removed_2)
					{
						i=last_removed_1;
						j=last_removed_2;
					}
					else
					{
						j=last_removed_1;
						i=last_removed_2;
					}
					if ((((i==0) && (j==4)) || (j==(i+1)%5)) && ((i_rd==j) || (i_rd==i)))
					{
						if ((i==0) && (j==4))
						{
							if (buffer[read_index[4]].flags.p1_f)
								buffer[read_index[4]].flags.ivtc_valid=true;
						}
						else
						{
							if (buffer[read_index[i]].flags.p1_f)
								buffer[read_index[i]].flags.ivtc_valid=true;
						}
					}
				}
			}
		}

		if (last_removed_1==5) last_removed_1=i_rd;
		else
		{
			if ((last_removed_2==5) && (last_removed_1!=i_rd))
			{
				last_removed_2=i_rd;
				if (last_removed_1<last_removed_2)
				{
					i=last_removed_1;
					j=last_removed_2;
				}
				else
				{
					j=last_removed_1;
					i=last_removed_2;
				}
				if (!(((i==0) && (j==4)) || (j==(i+1)%5)))
				{
					last_removed_1=5;
					last_removed_2=5;
				}
			}
		}

		if ((i_rd!=last_removed_1) && (i_rd!=last_removed_2))
		{
			last_removed_1=i_rd;
			last_removed_2=5;
		}

		// Si on valide une IVTC juste avant un chg de scène, il faut faire IVTC inverse
		for (i=0; i<5; i++)
		{
			if (buffer[read_index[i]].flags.ivtc_valid &&
				buffer[read_index[i]].flags.chg_sc_p1)
				buffer[read_index[i]].flags.rev=true;
		}

		i=0;
		j=0;
		do
		{
			if (i==i_rd) i++;
			v1=read_index[i];
			if (buffer[read_index[i]].flags.ivtc_valid &&
				buffer[read_index[i]].flags.ivtc_map &&
				!buffer[read_index[i]].flags.interlace &&
				!buffer[read_index[i]].flags.ivtc &&
				!buffer[read_index[i]].flags.Z && 
				!buffer[read_index[i]].flags.dtopbot && (v>2))
			{
				v0=0x0100|v1;
				data_out[j]=(uint16_t)v0;
				i++;
				j++;
				continue;
			}
			if (buffer[read_index[i]].flags.ivtc_valid)
			{
				if (buffer[read_index[i]].flags.rev)
				{
					i_rd_m1=read_index[(i+Read_Index_Size-1)%Read_Index_Size];
					if (inv_polar)
						v0=(i_rd_m1 << 4)|v1;
					else
						v0=(v1 << 4)|i_rd_m1;
					if (buffer[read_index[i]].flags.interlace_m1 &&
						!disable_deinterlace) v0|=0x0400;
					data_out[j]=(uint16_t)v0;
				}
				else
				{
					i_rd_p1=read_index[(i+1)%Read_Index_Size];
					if (inv_polar)
						v0=(v1 << 4)|i_rd_p1;
					else
						v0=(i_rd_p1 << 4)|v1;
					if (buffer[read_index[i]].flags.interlace_p1 &&
						!disable_deinterlace) v0|=0x0400;
					data_out[j]=(uint16_t)v0;
				}
				i++;
				j++;
				continue;
			}
			if (buffer[read_index[i]].flags.interlace && !disable_deinterlace)
			{
				v0=0x0200|v1;
				data_out[j]=(uint16_t)v0;
				i++;
				j++;
				continue;
			}
			v0=0x0100|v1;
			data_out[j]=(uint16_t)v0;
			i++;
			j++;
		}
		while (j<4);
		data_out[4]=data_out[3];

		index_out=0;
		out=false;

		for (i=0; i<Read_Index_Size; i++)
			read_index[i]=(read_index[i]+5)%Data_Buffer_Size;
	}

// Début gestion manuelle.		

	if ((mData.manual_mode) && (pxsrc.mFrameNumber%5==0))
	{
		index_image=(uint32_t)pxsrc.mFrameNumber;
		for(i=0; i<5; i++)
		{
			j_manual=0;
			while ((j_manual<manual.nbre_sequence) && 
				!((index_image>=manual.debut[j_manual]) && (index_image<=manual.fin[j_manual])))
				j_manual++;
			if (j_manual<manual.nbre_sequence)
			{
				if (manual.last_sequence==9999)
				{
					manual.first_offset=manual.ivtc_offset[j_manual];
				}
				manual.last_offset=manual.ivtc_offset[j_manual];
				if (j_manual!=manual.last_sequence)
				{
					manual.last_sequence=j_manual;
					chg_seq=true;
				}
				else chg_seq=false;
				manual.sec_prec_ok=true;
				if (manual.modif_polar[j_manual])
					tab_ivtc[i].inv_polar=!inv_polar;
				else
					tab_ivtc[i].inv_polar=inv_polar;
			}
			else
			{
				if (manual.sec_prec_ok) chg_seq=true;
				else chg_seq=false;
				manual.sec_prec_ok=false;
				tab_ivtc[i].inv_polar=inv_polar;
			}
			tab_ivtc[i].ivtc=(i==manual.last_offset);
			tab_ivtc[i].chg_sequence=chg_seq;
			tab_ivtc[i].chg_sequence_p1=false;
			tab_ivtc[i].ivtc_rev=false;
			index_image++;
		}

		j_manual=0;
		while ((j_manual<manual.nbre_sequence) && 
			!((index_image>=manual.debut[j_manual]) && (index_image<=manual.fin[j_manual])))
			j_manual++;
		if ((j_manual!=manual.last_sequence) && (manual.sec_prec_ok))
		{
			tab_ivtc[4].chg_sequence_p1=true;
			tab_ivtc[4].ivtc_rev=true;
		}
	
		j_manual=0;
		while ((j_manual<5) && !tab_ivtc[j_manual].chg_sequence) j_manual++;

		if (!tab_ivtc[0].chg_sequence)
		{
			i=manual.first_offset;
			if (tab_ivtc[(i+1)%5].chg_sequence) i_rd_manual=i;
			else
			{
				if ((i+1)%5<j_manual) i_rd_manual=(i+1)%5;
				else
				{
					i=manual.last_offset;
					if (((i+1)%5)>=j_manual) i_rd_manual=(i+1)%5;
					else i_rd_manual=i;
				}
			}
		}
		else i_rd_manual=(manual.last_offset+1)%5;

		if ((j_manual>0) && (j_manual<5))
		{
			if (((manual.last_offset+1)%5)>=j_manual) tab_ivtc[(manual.last_offset+1)%5].ivtc=true;
		}
	
		manual.first_offset=manual.last_offset;

		j_manual=0;
		for (i=0; i<4 ;i++)
		{
			if (j_manual==i_rd_manual) j_manual++;
			if (tab_ivtc[j_manual].ivtc)
			{
				if (tab_ivtc[j_manual].inv_polar)
				{
					if (tab_ivtc[j_manual].ivtc_rev) v1=((j_manual-1) << 4) | j_manual;
					else v1=(j_manual << 4) | (j_manual+1);	
				}
				else
				{
					if (tab_ivtc[j_manual].ivtc_rev) v1=(j_manual << 4) | (j_manual-1);
					else v1=((j_manual+1) << 4) | j_manual;
				}
			}
			else v1=(0x0100 | ((j_manual << 4) | j_manual));
			manual.data_out[i]=(uint16_t)v1;
			j_manual++;
		}
		manual.data_out[4]=manual.data_out[3];

		manual.ivtc_index=0;
	}

	if (mData.manual_mode)
	{
		index_image=(uint32_t)pxsrc.mFrameNumber;
		j_manual=0;
		while ((j_manual<manual.nbre_sequence) && 
			!((index_image>=manual.debut[j_manual]) && (index_image<=manual.fin[j_manual])))
			j_manual++;
		if (j_manual<manual.nbre_sequence) manual_current=true;
		else manual_current=false;
	}
	else manual_current=false;
	
// Fin gestion manuelle		
				
		if (manual_current)
		{
			v=manual.data_out[manual.ivtc_index];
			
			if ((manual.ivtc_index<2) || (manual.ivtc_index==4) || (output_mode==1))
			{
				if ((v&0x0100)==0x0100)
				{
					i=(uint8_t)(v&0x000F);
					if (pxsrc.mFrameNumber!=0) i_tab=i+2-manual.ivtc_index;
					else i_tab=i;
					
					if ((i_tab!=2) || swap_buffer)
					{
						idata.src_plane0=tab_pxsrc[i_tab]->data;
						idata.src_plane1=tab_pxsrc[i_tab]->data2;
						idata.src_plane2=tab_pxsrc[i_tab]->data3;
		
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
								break;
							case 6 :
								src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
								src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
								src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
								break;
						}	

						switch(idata.video_mode)
						{
							case 0 :
							case 1 :
								JPSDR_IVTC_Move32_Full(src1_Y,dst1_Y,w,h,src_modulo,dst_modulo);
								break;
							case 2 :
							case 3 :
								JPSDR_IVTC_Move32_Full(src1_Y,dst1_Y,(w+1)>>1,h,src_modulo,dst_modulo);
								break;
							case 6 :
								if (w_Y4) JPSDR_IVTC_Move32_Full(src1_Y,dst1_Y,(w_Y >> 2),h_Y,src_modulo_Y,dst_modulo_Y);
								else JPSDR_IVTC_Move8_Full(src1_Y,dst1_Y,w_Y,h_Y,src_modulo_Y,dst_modulo_Y);
								if (w_U4) JPSDR_IVTC_Move32_Full(src1_U,dst1_U,(w_U >> 2),h_U,src_modulo_U,dst_modulo_U);
								else JPSDR_IVTC_Move8_Full(src1_U,dst1_U,w_U,h_U,src_modulo_U,dst_modulo_U);
								if (w_U4) JPSDR_IVTC_Move32_Full(src1_V,dst1_V,(w_V >> 2),h_V,src_modulo_V,dst_modulo_V);
								else JPSDR_IVTC_Move8_Full(src1_V,dst1_V,w_V,h_V,src_modulo_V,dst_modulo_V);
								break;
								
						}
					}
				}								
				if ((v&0xFF00)==0)
				{
					i=(uint8_t)(v&0x000F);
					if (pxsrc.mFrameNumber!=0) i_tab=i+2-manual.ivtc_index;
					else i_tab=i;
					
					modif_top=(i_tab!=2) || swap_buffer;
					
					src1_RGB=manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual];

					idata.src_plane0=tab_pxsrc[i_tab]->data;
					idata.src_plane1=tab_pxsrc[i_tab]->data2;
					idata.src_plane2=tab_pxsrc[i_tab]->data3;
		
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							break;
						case 6 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
							src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
							break;
					}	

					j=(uint8_t)((v&0x00F0) >> 4);
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;
					
					modif_bottom=(j_tab!=2) || swap_buffer;

					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual]
						+buffer_pitch);
					
					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
		
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	

					if ((manual.ivtc_index!=1) || (output_mode==1))
					{
						if (filter_output)
						{
							switch(deinterlace_mode)
							{
							case 1 :
								switch(idata.video_mode)
								{
									case 0 :
									case 1 :
										Smart_Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
											buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,
											thr_out,tri_linear,map_filter);
										break;
									case 2 :
										Smart_Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
											buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,
											thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
										break;
									case 3 :
										Smart_Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
											buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,
											thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
										break;
									case 6 :
										Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,
											(uint8_t *)src1_U,(uint8_t *)src2_U,(uint8_t *)src1_V,
											(uint8_t *)src2_V,(uint8_t *)dst1_Y,(uint8_t *)dst1_U,
											(uint8_t *)dst1_V,buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,buffer_2,
											w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,
											src_modulo_U,src_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V,dst_modulo_Y,dst_modulo_U,
											dst_modulo_V,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
										break;
								}
								break;
							case 2 :
								switch (idata.video_mode)
								{
									case 0 :
									case 1 :
										Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false);
										break;
									case 2 :
										Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,Y_only);
										break;
									case 3 :
										Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,Y_only);
										break;
									case 6 :
										Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)dst1_Y,
											w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,tri_linear,false);
										if (!Y_only)
										{
											Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,(uint8_t *)dst1_U,
												w_U,h_U>>1,src_pitch_U,dst_pitch_U,tri_linear,false);
											Deinterlace_Planar((uint8_t *)src1_V,(uint8_t*)src2_V,(uint8_t *)dst1_V,
												w_V,h_V>>1,src_pitch_V,dst_pitch_V,tri_linear,false);
										}
										else
										{
											if (w_U4)
												JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,dst1_U,w_U>>2,h_U>>1,
													src_pitch_U,dst_pitch_U,src_modulo_U,dst_modulo_U);
											else
												JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,dst1_U,w_U,h_U>>1,
													src_pitch_U,dst_pitch_U,src_modulo_U,dst_modulo_U);
											if (w_V4)
												JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,dst1_V,w_V>>2,h_V>>1,
													src_pitch_V,dst_pitch_V,src_modulo_V,dst_modulo_V);
											else
												JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,dst1_V,w_V,h_V>>1,
													src_pitch_V,dst_pitch_V,src_modulo_V,dst_modulo_V);
										}
										break;
								}
								break;
							}
						}
						else
						{
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									if (modif_top && modif_bottom)
									{
										JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									else
									{
										if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
										if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									break;
								case 2 :
								case 3 :
									if (modif_top && modif_bottom)
									{
										JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);									
									}
									else
									{
										if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
										if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									break;
								case 6 :
									if (modif_top && modif_bottom)
									{
										if (w_Y4)
											JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,
												dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_Y,src2_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,
												dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,
												dst_pitch_U,src_modulo_U,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,dst1_U,w_U,h_U>>1,src_pitch_U,
												dst_pitch_U,src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,
												dst_pitch_V,src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,dst1_V,w_V,h_V>>1,src_pitch_V,
												dst_pitch_V,src_modulo_V,dst_modulo_V);									
									}
									else
									{
										if (modif_top)
										{
											if (w_Y4)
												JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											if (w_U4)
												JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											if (w_V4)
												JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
										}
										if (modif_bottom)
										{
											if (w_Y4)
												JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											if (w_U4)
												JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											if (w_V4)
												JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
										}
									}
									break;
							}
						}
					}
					else
					{
						if (filter_output)
						{
							switch(deinterlace_mode)
							{
							case 1 :
								switch (idata.video_mode)
								{
									case 0 :
									case 1 :
										Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
											buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
											src_modulo,0,thr_out,tri_linear,map_filter);
										break;
									case 2 :
										Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
											buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
											src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
										Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
										break;
									case 3 :
										Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
											buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
											src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
										Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
										break;
									case 6 :
										Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,
											(uint8_t *)src1_U,(uint8_t *)src2_U,(uint8_t *)src1_V,
											(uint8_t *)src2_V,buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
											buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,
											buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,
											src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,frame_buffer_pitch_U,
											frame_buffer_pitch_V,0,0,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
										Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
											buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
											frame_buffer_pitch_V,buffer_pitch_planar420);
										break;
								}
								break;
							case 2 : 
								switch (idata.video_mode)
								{
									case 0 :
									case 1 :
										Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,frame_buffer_pitch,
											tri_linear,true);
										break;
									case 2 :
										Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,frame_buffer_pitch,
											tri_linear,true,Y_only);
										Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
										break;
									case 3 :
										Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,frame_buffer_pitch,
											tri_linear,true,Y_only);
										Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
										break;
									case 6 :
										Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],
											w_Y,h_Y>>1,src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
										if (!Y_only)
										{
											Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],
												w_U,h_U>>1,src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
											Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],
												w_V,h_V>>1,src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
										}
										else
										{
											if (w_U4)
												JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],w_U>>2,
													h_U>>1,src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
											else
												JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,buffer_frame_U[0],w_U,h_U>>1,
													src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
											if (w_V4)
												JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],w_V>>2,
													h_V>>1,src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
											else
												JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,buffer_frame_V[0],w_V,h_V>>1,
													src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
										}
										Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],
											buffer_frame_V[0],buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,
											frame_buffer_pitch_U,frame_buffer_pitch_V,buffer_pitch_planar420);
										break;
								}
								break;
							}
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									JPSDR_IVTC_Move32_Full_dst(buffer_frame[0],dst1_Y,w,h,dst_modulo);
									break;
								case 2 :
								case 3 :
									JPSDR_IVTC_Move32_Full_dst(buffer_frame[0],dst1_Y,(w+1)>>1,h,dst_modulo);
									break;
								case 6 :
									if (w_Y4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_Y[0],dst1_Y,w_Y>>2,h_Y,dst_modulo_Y);
									else JPSDR_IVTC_Move8_Full_dst(buffer_frame_Y[0],dst1_Y,w_Y,h_Y,dst_modulo_Y);
									if (w_U4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_U[0],dst1_U,w_U>>2,h_U,dst_modulo_U);
									else JPSDR_IVTC_Move8_Full_dst(buffer_frame_U[0],dst1_U,w_U,h_U,dst_modulo_U);
									if (w_V4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_V[0],dst1_V,w_V>>2,h_V,dst_modulo_V);
									else JPSDR_IVTC_Move8_Full_dst(buffer_frame_V[0],dst1_V,w_V,h_V,dst_modulo_V);
									break;
							}
						}
						else
						{
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									if (modif_top && modif_bottom)
									{
										JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									else
									{
										if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
										if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									break;
								case 2 :
								case 3 :
									if (modif_top && modif_bottom)
									{
										JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									else
									{
										if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
										if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
											src_modulo,dst_modulo);
									}
									break;
								case 6 :
									if (modif_top && modif_bottom)
									{
										if (w_Y4)
											JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,
												dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_Y,src2_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,
												dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,
												dst_pitch_U,src_modulo_U,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,dst1_U,w_U,h_U>>1,src_pitch_U,
												dst_pitch_U,src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,
												dst_pitch_V,src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,dst1_V,w_V,h_V>>1,src_pitch_V,
												dst_pitch_V,src_modulo_V,dst_modulo_V);									
									}
									else
									{
										if (modif_top)
										{
											if (w_Y4)
												JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											if (w_U4)
												JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											if (w_V4)
												JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
										}
										if (modif_bottom)
										{
											if (w_Y4)
												JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,src_pitch_Y,
													dst_pitch_Y,src_modulo_Y,dst_modulo_Y);
											if (w_U4)
												JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,src_pitch_U,
													dst_pitch_U,src_modulo_U,dst_modulo_U);
											if (w_V4)
												JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
											else
												JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,src_pitch_V,
													dst_pitch_V,src_modulo_V,dst_modulo_V);
										}
									}
									break;
							}
						}
					}
				}
			}
			
			if ((manual.ivtc_index==2) && (output_mode==2))
			{
				if (previous_manual)
				{
					v1=manual.data_out[1];
					if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
					if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0) >> 4);
					if (pxsrc.mFrameNumber!=0) i_tab=i+2-manual.ivtc_index;
					else i_tab=i;
				}
				else
				{
					v1=data_out[1];
					if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
					if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0) >> 4);
					if (pxsrc.mFrameNumber!=0)
						i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-manual.ivtc_index)%Read_Index_Size;
					else i_tab=i;
				}
				
				j=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
				else j_tab=j;
				
				if (((v&0xFF00)==0) && (filter_output))
				{
					j=(uint8_t)((v&0x00F0) >> 4);			
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;

					modif_bottom=(j_tab!=2) || swap_buffer;
					
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual]
						+buffer_pitch);

					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
		
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	
											
					j=(uint8_t)(v&0x000F);
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;

					modif_top=(j_tab!=2) || swap_buffer;
					
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];

					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
		
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							break;
						case 6 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
							src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
							break;
					}	
															
					switch(deinterlace_mode)
					{
					case 1 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter);
								break;
							case 2 :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 3 :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 6 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[1],
									buffer_frame_U[1],buffer_frame_V[1],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_out,tri_linear,map_filter,src1_RGB,
									src2_RGB,buffer_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],
									buffer_frame_V[1],buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					case 2 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true);
								break;
							case 2 :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 3 :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 6 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[1],w_Y,h_Y>>1,
									src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[1],w_U,h_U>>1,
										src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[1],w_V,h_V>>1,
										src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
								}
								else
								{
									if (w_U4)
										JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,buffer_frame_U[1],w_U>>2,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,buffer_frame_U[1],w_U,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									if (w_V4)
										JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,buffer_frame_V[1],w_V>>2,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,buffer_frame_V[1],w_V,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],
									buffer_frame_V[1],buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					}
				}

				if (previous_manual)
				{
					if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
					{
						modif_bottom=(i_tab!=2) || swap_buffer;
						
						idata.src_plane0=tab_pxsrc[i_tab]->data;
						idata.src_plane1=tab_pxsrc[i_tab]->data2;
						idata.src_plane2=tab_pxsrc[i_tab]->data3;

						src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
							+buffer_pitch);
						
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								break;
							case 6 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
								src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
								break;
						}
						pitch2_Y=src_pitch_Y;
						pitch2_U=src_pitch_U;
						pitch2_V=src_pitch_V;
						modulo2_Y=src_modulo_Y;
						modulo2_U=src_modulo_U;
						modulo2_V=src_modulo_V;
										
					}
					else
					{
						modif_bottom=true;
						
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_pitch);
						
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)buffer_frame[0]+frame_buffer_pitch);
								break;
							case 6 :
								src2_Y=(void *)(buffer_frame_Y[0]+frame_buffer_pitch_Y);
								src2_U=(void *)(buffer_frame_U[0]+frame_buffer_pitch_U);
								src2_V=(void *)(buffer_frame_V[0]+frame_buffer_pitch_V);
								break;
						}	
						pitch2_Y=frame_buffer_pitch_Y;
						pitch2_U=frame_buffer_pitch_U;
						pitch2_V=frame_buffer_pitch_V;
						modulo2_Y=0;
						modulo2_U=0;
						modulo2_V=0;
						
					}
				}
				else
				{
					if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
					{
					
						modif_bottom=(i_tab!=2) || swap_buffer;
						
						idata.src_plane0=tab_pxsrc[i_tab]->data;
						idata.src_plane1=tab_pxsrc[i_tab]->data2;
						idata.src_plane2=tab_pxsrc[i_tab]->data3;
						
						src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_pitch);
					
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								break;
							case 6 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
								src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
								break;
						}	
						pitch2_Y=src_pitch_Y;
						pitch2_U=src_pitch_U;
						pitch2_V=src_pitch_V;
						modulo2_Y=src_modulo_Y;
						modulo2_U=src_modulo_U;
						modulo2_V=src_modulo_V;
											
					}
					else
					{
						modif_bottom=true;
						
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_pitch);
						
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)buffer_frame[0]+frame_buffer_pitch);
								break;
							case 6 :
								src2_Y=(void *)(buffer_frame_Y[0]+frame_buffer_pitch_Y);
								src2_U=(void *)(buffer_frame_U[0]+frame_buffer_pitch_U);
								src2_V=(void *)(buffer_frame_V[0]+frame_buffer_pitch_V);
								break;
						}
						pitch2_Y=frame_buffer_pitch_Y;
						pitch2_U=frame_buffer_pitch_U;
						pitch2_V=frame_buffer_pitch_V;
						modulo2_Y=0;
						modulo2_U=0;
						modulo2_V=0;
						
					}
				}					
				if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
				{
					modif_top=(j_tab!=2) || swap_buffer;
					
					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
					
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];
					
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							break;
						case 6 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
							src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
							break;
					}
					pitch1_Y=src_pitch_Y;
					pitch1_U=src_pitch_U;
					pitch1_V=src_pitch_V;
					modulo1_Y=src_modulo_Y;
					modulo1_U=src_modulo_U;
					modulo1_V=src_modulo_V;
					
				}
				else
				{
					modif_top=true;
					
					src1_RGB=buffer_frameRGB32[1];
					
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=buffer_frame[1];
							break;
						case 6 :
							src1_Y=(void *)buffer_frame_Y[1];
							src1_U=(void *)buffer_frame_U[1];
							src1_V=(void *)buffer_frame_V[1];
							break;
					}								
					pitch1_Y=frame_buffer_pitch_Y;
					pitch1_U=frame_buffer_pitch_U;
					pitch1_V=frame_buffer_pitch_V;
					modulo1_Y=0;
					modulo1_U=0;
					modulo1_V=0;
					
				}

				switch (idata.video_mode)
				{
					case 0 :
					case 1 :
						if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,pitch1_Y,dst_pitch,modulo1_Y,dst_modulo);
						if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,pitch2_Y,dst_pitch,modulo2_Y,dst_modulo);
						break;
					case 2 :
					case 3 :
						if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,pitch1_Y,dst_pitch,modulo1_Y,dst_modulo);
						if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,pitch2_Y,dst_pitch,modulo2_Y,dst_modulo);
						break;
					case 6 :
						if (modif_top)
						{
							if (w_Y4)
								JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,pitch1_Y,dst_pitch_Y,
									modulo1_Y,dst_modulo_Y);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,pitch1_Y,dst_pitch_Y,
									modulo1_Y,dst_modulo_Y);
							if (w_U4)
								JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,pitch1_U,dst_pitch_U,
									modulo1_U,dst_modulo_U);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,pitch1_U,dst_pitch_U,
									modulo1_U,dst_modulo_U);
							if (w_V4)
								JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,pitch1_V,dst_pitch_V,
									modulo1_V,dst_modulo_V);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,pitch1_V,dst_pitch_V,
									modulo1_V,dst_modulo_V);
						}
						if (modif_bottom)
						{
							if (w_Y4)
								JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,pitch2_Y,dst_pitch_Y,
									modulo2_Y,dst_modulo_Y);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,pitch2_Y,dst_pitch_Y,
									modulo2_Y,dst_modulo_Y);
							if (w_U4)
								JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,pitch2_U,dst_pitch_U,
									modulo2_U,dst_modulo_U);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,pitch2_U,dst_pitch_U,
									modulo2_U,dst_modulo_U);
							if (w_V4)
								JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,pitch2_V,dst_pitch_V,
									modulo2_V,dst_modulo_V);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,pitch2_V,dst_pitch_V,
									modulo2_V,dst_modulo_V);
						}
						break;
				}
			}

			if ((manual.ivtc_index==3) && (output_mode==2))
			{
				if (previous_manual)
				{
					v1=manual.data_out[2];
					if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
					if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0)>>4);
					if (pxsrc.mFrameNumber!=0) i_tab=i+2-manual.ivtc_index;
					else i_tab=i;									
				}
				else
				{
					v1=data_out[2];
					if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
					if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0)>>4);
					if (pxsrc.mFrameNumber!=0)
						i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-manual.ivtc_index)%Read_Index_Size;
					else i_tab=i;					
				}					
				
				j=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
				else j_tab=j;
				
				if (((v&0xFF00)==0) && (filter_output))
				{
					j=(uint8_t)((v&0x00F0) >> 4);
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;

					modif_bottom=(j_tab!=2) || swap_buffer;
					
					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
						
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual]
						+buffer_pitch);					
					
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	
																					
					j=(uint8_t)(v&0x000F);				
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;

					modif_top=(j_tab!=2) || swap_buffer;
					
					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
						
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];
					
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							break;
						case 6 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
							src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
							break;
					}	
										
					switch(deinterlace_mode)
					{
					case 1 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter);
								break;
							case 2 :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 3 :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 6 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
									buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_out,tri_linear,map_filter,src1_RGB,
									src2_RGB,buffer_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					case 2 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true);
								break;
							case 2 :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 3 :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 6 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
									src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
										src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
										src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
								}
								else
								{
									if (w_U4)
										JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],w_U>>2,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,buffer_frame_U[0],w_U,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									if (w_V4)
										JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],w_V>>2,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,buffer_frame_V[0],w_V,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					}
				}

				if (previous_manual)
				{
					if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
					{

						modif_bottom=(i_tab!=2) || swap_buffer;
					
						idata.src_plane0=tab_pxsrc[i_tab]->data;
						idata.src_plane1=tab_pxsrc[i_tab]->data2;
						idata.src_plane2=tab_pxsrc[i_tab]->data3;
						
						src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
							+buffer_pitch);
					
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								break;
							case 6 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
								src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
								break;
						}
						pitch2_Y=src_pitch_Y;
						pitch2_U=src_pitch_U;
						pitch2_V=src_pitch_V;
						modulo2_Y=src_modulo_Y;
						modulo2_U=src_modulo_U;
						modulo2_V=src_modulo_V;										
					}
					else 
					{
						modif_bottom=true;
											
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_pitch);
					
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)buffer_frame[1]+frame_buffer_pitch);
								break;
							case 6 :
								src2_Y=(void *)(buffer_frame_Y[1]+frame_buffer_pitch_Y);
								src2_U=(void *)(buffer_frame_U[1]+frame_buffer_pitch_U);
								src2_V=(void *)(buffer_frame_V[1]+frame_buffer_pitch_V);
								break;
						}
						pitch2_Y=frame_buffer_pitch_Y;
						pitch2_U=frame_buffer_pitch_U;
						pitch2_V=frame_buffer_pitch_V;
						modulo2_Y=0;
						modulo2_U=0;
						modulo2_V=0;
					}
				}
				else
				{
					if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
					{
						modif_bottom=(i_tab!=2) || swap_buffer;
					
						idata.src_plane0=tab_pxsrc[i_tab]->data;
						idata.src_plane1=tab_pxsrc[i_tab]->data2;
						idata.src_plane2=tab_pxsrc[i_tab]->data3;
						
						src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_pitch);
					
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								break;
							case 6 :
								src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
								src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
								src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
								break;
						}
						pitch2_Y=src_pitch_Y;
						pitch2_U=src_pitch_U;
						pitch2_V=src_pitch_V;
						modulo2_Y=src_modulo_Y;
						modulo2_U=src_modulo_U;
						modulo2_V=src_modulo_V;								
					
					}
					else
					{
						modif_bottom=true;
											
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_pitch);
					
						switch (idata.video_mode)
						{
							case 1 :
							case 2 :
							case 3 :
								src2_Y=(void *)((uint8_t *)buffer_frame[1]+frame_buffer_pitch);
								break;
							case 6 :
								src2_Y=(void *)(buffer_frame_Y[1]+frame_buffer_pitch_Y);
								src2_U=(void *)(buffer_frame_U[1]+frame_buffer_pitch_U);
								src2_V=(void *)(buffer_frame_V[1]+frame_buffer_pitch_V);
								break;
						}
						pitch2_Y=frame_buffer_pitch_Y;
						pitch2_U=frame_buffer_pitch_U;
						pitch2_V=frame_buffer_pitch_V;
						modulo2_Y=0;
						modulo2_U=0;
						modulo2_V=0;
						
					}
				}					
				if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
				{
					modif_top=(j_tab!=2) || swap_buffer;
				
					idata.src_plane0=tab_pxsrc[j_tab]->data;
					idata.src_plane1=tab_pxsrc[j_tab]->data2;
					idata.src_plane2=tab_pxsrc[j_tab]->data3;
						
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];
					
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							break;
						case 6 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
							src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
							break;
					}
					pitch1_Y=src_pitch_Y;
					pitch1_U=src_pitch_U;
					pitch1_V=src_pitch_V;
					modulo1_Y=src_modulo_Y;
					modulo1_U=src_modulo_U;
					modulo1_V=src_modulo_V;								
								
				}
				else
				{
					modif_top=true;
										
					src1_RGB=buffer_frameRGB32[0];
					
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=buffer_frame[0];;
							break;
						case 6 :
							src1_Y=(void *)buffer_frame_Y[0];
							src1_U=(void *)buffer_frame_U[0];
							src1_V=(void *)buffer_frame_V[0];
							break;
					}
					pitch1_Y=frame_buffer_pitch_Y;
					pitch1_U=frame_buffer_pitch_U;
					pitch1_V=frame_buffer_pitch_V;
					modulo1_Y=0;
					modulo1_U=0;
					modulo1_V=0;												
				}

				switch (idata.video_mode)
				{
					case 0 :
					case 1 :
						if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,pitch1_Y,dst_pitch,modulo1_Y,
							dst_modulo);
						if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,pitch2_Y,dst_pitch,modulo2_Y,
							dst_modulo);
						break;
					case 2 :
					case 3 :
						if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,pitch1_Y,dst_pitch,modulo1_Y,
							dst_modulo);
						if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,pitch2_Y,dst_pitch,modulo2_Y,
							dst_modulo);
						break;
					case 6 :
						if (modif_top)
						{
							if (w_Y4)
								JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,pitch1_Y,dst_pitch_Y,modulo1_Y,
									dst_modulo_Y);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,pitch1_Y,dst_pitch_Y,modulo1_Y,
									dst_modulo_Y);
							if (w_U4)
								JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,pitch1_U,dst_pitch_U,modulo1_U,
									dst_modulo_U);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,pitch1_U,dst_pitch_U,modulo1_U,
									dst_modulo_U);
							if (w_V4)
								JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,pitch1_V,dst_pitch_V,modulo1_V,
									dst_modulo_V);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,pitch1_V,dst_pitch_V,modulo1_V,
									dst_modulo_V);
						}
						if (modif_bottom)
						{
							if (w_Y4)
								JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,pitch2_Y,dst_pitch_Y,modulo2_Y,
									dst_modulo_Y);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,pitch2_Y,dst_pitch_Y,modulo2_Y,
									dst_modulo_Y);
							if (w_U4)
								JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,pitch2_U,dst_pitch_U,modulo2_U,
									dst_modulo_U);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,pitch2_U,dst_pitch_U,modulo2_U,
									dst_modulo_U);
							if (w_V4)
								JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,pitch2_V,dst_pitch_V,modulo2_V,
									dst_modulo_V);
							else
								JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,pitch2_V,dst_pitch_V,modulo2_V,
									dst_modulo_V);
						}
						break;
				}
			}
		}

/*		fprintf(fic2,"Image : %u , %u\n",mFrameNumber,pxsrc.mFrameNumber);
		fprintf(fic2,"Process : %X\n\n",data_out[index_out]);*/

		v=data_out[index_out];

		if ((!manual_current) && ((index_out<2) || (index_out==4) || (output_mode==1)))
		{
			if ((v&0x0100)==0x0100)
			{
				i=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;
								
				if ((i_tab!=2) || swap_buffer)
				{
					idata.src_plane0=tab_pxsrc[i_tab]->data;
					idata.src_plane1=tab_pxsrc[i_tab]->data2;
					idata.src_plane2=tab_pxsrc[i_tab]->data3;
		
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							break;
						case 6 :
							src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
							src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
							src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
							break;
					}	
								
					switch (idata.video_mode)
					{
						case 0 :
						case 1 :
							JPSDR_IVTC_Move32_Full(src1_Y,dst1_Y,w,h,src_modulo,dst_modulo);
							break;
						case 2 :
						case 3 :
							JPSDR_IVTC_Move32_Full(src1_Y,dst1_Y,(w+1)>>1,h,src_modulo,dst_modulo);
							break;
						case 6 :
							if (w_Y4) JPSDR_IVTC_Move32_Full(src1_Y,dst1_Y,w_Y>>2,h_Y,src_modulo_Y,dst_modulo_Y);
							else JPSDR_IVTC_Move8_Full(src1_Y,dst1_Y,w_Y,h_Y,src_modulo_Y,dst_modulo_Y);
							if (w_U4) JPSDR_IVTC_Move32_Full(src1_U,dst1_U,w_U>>2,h_U,src_modulo_U,dst_modulo_U);
							else JPSDR_IVTC_Move8_Full(src1_U,dst1_U,w_U,h_U,src_modulo_U,dst_modulo_U);
							if (w_V4) JPSDR_IVTC_Move32_Full(src1_V,dst1_V,w_V>>2,h_V,src_modulo_V,dst_modulo_V);
							else JPSDR_IVTC_Move8_Full(src1_V,dst1_V,w_V,h_V,src_modulo_V,dst_modulo_V);
							break;
					}
				}
			}
			
			if ((v&0x0200)==0x0200)
			{
				i=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;
				
				idata.src_plane0=tab_pxsrc[i_tab]->data;
				idata.src_plane1=tab_pxsrc[i_tab]->data2;
				idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
				src1_RGB=buffer[i].frameRGB32;
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}	

				src2_RGB=(void *)((uint8_t *)src1_RGB+buffer_pitch);
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						break;
					case 6 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
						src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
						break;
				}					
								
				if ((index_out!=1) || (output_mode==1))
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
									buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,thr_dtl,
									tri_linear,map_filter);
								break;
							case 2 :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
									buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,thr_dtl,
									tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
								break;
							case 3 :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
									buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,thr_dtl,
									tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
								break;
							case 6 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,(uint8_t *)dst1_Y,
									(uint8_t *)dst1_U,(uint8_t *)dst1_V,buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,
									buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,
									src_modulo_U,src_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V,dst_modulo_Y,dst_modulo_U,
									dst_modulo_V,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
								break;
						}
						break;
					case 2 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false);
								break;
							case 2 :
								Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,false);
								break;
							case 3 :
								Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,false);
								break;
							case 6 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)dst1_Y,
									w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,tri_linear,false);
								Deinterlace_Planar((uint8_t *)src1_U,(uint8_t*)src2_U,(uint8_t *)dst1_U,
									w_U,h_U>>1,src_pitch_U,dst_pitch_U,tri_linear,false);
								Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,(uint8_t *)dst1_V,
									w_V,h_V>>1,src_pitch_V,dst_pitch_V,tri_linear,false);
								break;
						}
						break;
					}
				}
				else
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_dtl,tri_linear,map_filter);
								break;
							case 2 :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 3 :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 6 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
									buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_dtl,tri_linear,map_filter,src1_RGB,
									src2_RGB,buffer_pitch,false);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					case 2 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true);
								break;
							case 2 :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,false);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 3 :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,false);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 6 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
									src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
								Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
									src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
								Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
									src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					}
					switch (idata.video_mode)
					{
						case 0 :
						case 1 :
							JPSDR_IVTC_Move32_Full_dst(buffer_frame[0],dst1_Y,w,h,dst_modulo);
							break;
						case 2 :
						case 3 :
							JPSDR_IVTC_Move32_Full_dst(buffer_frame[0],dst1_Y,(w+1)>>1,h,dst_modulo);
							break;
						case 6 :
							if (w_Y4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_Y[0],dst1_Y,w_Y>>2,h_Y,dst_modulo_Y);
							else JPSDR_IVTC_Move8_Full_dst(buffer_frame_Y[0],dst1_Y,w_Y,h_Y,dst_modulo_Y);
							if (w_U4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_U[0],dst1_U,w_U>>2,h_U,dst_modulo_U);
							else JPSDR_IVTC_Move8_Full_dst(buffer_frame_U[0],dst1_U,w_U,h_U,dst_modulo_U);
							if (w_V4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_V[0],dst1_V,w_V>>2,h_V,dst_modulo_V);
							else JPSDR_IVTC_Move8_Full_dst(buffer_frame_V[0],dst1_V,w_V,h_V,dst_modulo_V);
							break;
					}
				}
			}

			if (((v&0xFF00)==0) || ((v&0x0400)==0x0400))
			{
				i=(uint8_t)(v&0x000F);				
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;

				modif_top=(i_tab!=2) || swap_buffer;

				idata.src_plane0=tab_pxsrc[i_tab]->data;
				idata.src_plane1=tab_pxsrc[i_tab]->data2;
				idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
				src1_RGB=buffer[i].frameRGB32;
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}	
							
				i=(uint8_t)((v&0x00F0) >> 4);		
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;
							
				modif_bottom=(i_tab!=2) || swap_buffer;

				idata.src_plane0=tab_pxsrc[i_tab]->data;
				idata.src_plane1=tab_pxsrc[i_tab]->data2;
				idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
				src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_pitch);
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						break;
					case 6 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
						src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
						break;
				}					
								
				if ((index_out!=1) || (output_mode==1))
				{
					if ((filter_output) || ((v&0x0400)==0x0400))
					{
						switch(deinterlace_mode)
						{
						case 1 :
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									Smart_Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
										buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,
										thr_out,tri_linear,map_filter);
									break;
								case 2 :
									Smart_Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
										buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,
										thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
									break;
								case 3 :
									Smart_Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
										buffer_2,w,h>>1,src_pitch,dst_pitch,src_modulo,dst_modulo,
										thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
									break;
								case 6 :
									Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
										(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,(uint8_t *)dst1_Y,
										(uint8_t *)dst1_U,(uint8_t*)dst1_V,buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,
										buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,
										src_modulo_Y,src_modulo_U,src_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V,dst_modulo_Y,dst_modulo_U,
										dst_modulo_V,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
									break;
							}
							break;
						case 2 :
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										tri_linear,false);
									break;
								case 2 :
									Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										tri_linear,false,Y_only);
									break;
								case 3 :
									Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										tri_linear,false,Y_only);
									break;
								case 6 :
									Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)dst1_Y,
										w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,tri_linear,false);
									if (!Y_only)
									{
										Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,(uint8_t *)dst1_U,
											w_U,h_U>>1,src_pitch_U,dst_pitch_U,tri_linear,false);
										Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,(uint8_t *)dst1_V,
											w_V,h_V>>1,src_pitch_V,dst_pitch_V,tri_linear,false);
									}
									else
									{
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,
												dst_pitch_U,src_modulo_Y,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,dst1_U,w_U,h_U>>1,src_pitch_U,
												dst_pitch_U,src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,
												dst_pitch_V,src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,dst1_V,w_V,h_V>>1,src_pitch_V,
												dst_pitch_V,src_modulo_V,dst_modulo_V);
									}
									break;
							}
							break;
						}
					}
					else
					{
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								if (modif_top && modif_bottom)
								{
									JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								else
								{
									if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
									if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								break;
							case 2 :
							case 3 :
								if (modif_top && modif_bottom)
								{
									JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								else
								{
									if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
									if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								break;
							case 6 :
								if (modif_top && modif_bottom)
								{
									if (w_Y4)
										JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,dst_pitch_Y,
											src_modulo_Y,dst_modulo_Y);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_Y,src2_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,
											src_modulo_Y,dst_modulo_Y);
									if (w_U4)
										JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,dst_pitch_U,
											src_modulo_U,dst_modulo_U);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,dst1_U,w_U,h_U>>1,src_pitch_U,dst_pitch_U,
											src_modulo_U,dst_modulo_U);
									if (w_V4)
										JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,dst_pitch_V,
											src_modulo_V,dst_modulo_V);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,dst1_V,w_V,h_V>>1,src_pitch_V,dst_pitch_V,
											src_modulo_V,dst_modulo_V);
								}
								else
								{
									if (modif_top)
									{
										if (w_Y4)
											JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);									
									}
									if (modif_bottom)
									{
										if (w_Y4)
											JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);									
									}
								}
								break;
						}
					}
				}
				else
				{
					if ((filter_output) || ((v&0x0400)==0x0400))
					{
						switch(deinterlace_mode)
						{
						case 1 :
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
										buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
										src_modulo,0,thr_out,tri_linear,map_filter);
									break;
								case 2 :
									Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
										buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
										src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
									Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
									break;
								case 3 :
									Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
										buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
										src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
									Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
									break;
								case 6 :
									Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
										(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
										buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
										buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
										src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
										frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_out,tri_linear,map_filter,src1_RGB,
										src2_RGB,buffer_pitch,Y_only);
									Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
										buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
										frame_buffer_pitch_V,buffer_pitch_planar420);
									break;
							}
							break;
						case 2 :
							switch (idata.video_mode)
							{
								case 0 :
								case 1 :
									Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
										frame_buffer_pitch,tri_linear,true);
									break;
								case 2 :
									Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
										frame_buffer_pitch,tri_linear,true,Y_only);
									Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
									break;
								case 3 :
									Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
										frame_buffer_pitch,tri_linear,true,Y_only);
									Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
									break;
								case 6 :
									Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
										src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
									if (!Y_only)
									{
										Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
										Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
									}
									else
									{
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],w_U>>2,h_U>>1,
												src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,buffer_frame_U[0],w_U,h_U>>1,
												src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],w_V>>2,h_V>>1,
												src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
										else
											JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,buffer_frame_V[0],w_V,h_V>>1,
												src_pitch_V,frame_buffer_pitch_V,src_modulo_U,0);
									}
									Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
										buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
										frame_buffer_pitch_V,buffer_pitch_planar420);
									break;
							}
							break;
						}
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								JPSDR_IVTC_Move32_Full_dst(buffer_frame[0],dst1_Y,w,h,dst_modulo);
								break;
							case 2 :
							case 3 :
								JPSDR_IVTC_Move32_Full_dst(buffer_frame[0],dst1_Y,(w+1)>>1,h,dst_modulo);
								break;
							case 6 :
								if (w_Y4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_Y[0],dst1_Y,w_Y>>2,h_Y,dst_modulo_Y);
								else JPSDR_IVTC_Move8_Full_dst(buffer_frame_Y[0],dst1_Y,w_Y,h_Y,dst_modulo_Y);
								if (w_U4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_U[0],dst1_U,w_U>>2,h_U,dst_modulo_U);
								else JPSDR_IVTC_Move8_Full_dst(buffer_frame_U[0],dst1_U,w_U,h_U,dst_modulo_U);
								if (w_V4) JPSDR_IVTC_Move32_Full_dst(buffer_frame_V[0],dst1_V,w_V>>2,h_V,dst_modulo_V);
								else JPSDR_IVTC_Move8_Full_dst(buffer_frame_V[0],dst1_V,w_V,h_V,dst_modulo_V);
								break;
						}
					}
					else
					{
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								if (modif_top && modif_bottom)
								{
									JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								else
								{
									if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
									if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								break;
							case 2 :
							case 3 :
								if (modif_top && modif_bottom)
								{
									JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								else
								{
									if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
									if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,src_pitch,dst_pitch,
										src_modulo,dst_modulo);
								}
								break;
							case 6 :
								if (modif_top && modif_bottom)
								{
									if (w_Y4)
										JPSDR_IVTC_Rebuild_Frame(src1_Y,src2_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,dst_pitch_Y,
											src_modulo_Y,dst_modulo_Y);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_Y,src2_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,
											src_modulo_Y,dst_modulo_Y);
									if (w_U4)
										JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,dst_pitch_U,
											src_modulo_U,dst_modulo_U);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,dst1_U,w_U,h_U>>1,src_pitch_U,dst_pitch_U,
											src_modulo_U,dst_modulo_U);
									if (w_V4)
										JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,dst_pitch_V,
											src_modulo_V,dst_modulo_V);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,dst1_V,w_V,h_V>>1,src_pitch_V,dst_pitch_V,
											src_modulo_V,dst_modulo_V);
								}
								else
								{
									if (modif_top)
									{
										if (w_Y4)
											JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);								
									}
									if (modif_bottom)
									{
										if (w_Y4)
											JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,src_pitch_Y,dst_pitch_Y,
												src_modulo_Y,dst_modulo_Y);
										if (w_U4)
											JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,src_pitch_U,dst_pitch_U,
												src_modulo_U,dst_modulo_U);
										if (w_V4)
											JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);
										else
											JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,src_pitch_V,dst_pitch_V,
												src_modulo_V,dst_modulo_V);								
									}
								}
								break;
						}
					}
				}
			}			
		}
		
		if ((!manual_current) && ((index_out==2) && (output_mode==2)))
		{
			if (!previous_manual)
			{
				v1=data_out[1];
				if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
				if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0) >> 4);
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;				
			}
			else
			{
				v1=manual.data_out[1];
				if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
				if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0) >> 4);
				if (pxsrc.mFrameNumber!=0) i_tab=i+2-index_out;
				else i_tab=i;				
			}
			
			j=(unsigned char)(v&0x000F);
			if (pxsrc.mFrameNumber!=0)
				j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
			else j_tab=j;		
			
			if ((v&0x0200)==0x0200)
			{			
				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}					

				src2_RGB=(void *)((uint8_t *)src1_RGB+buffer_pitch);

				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						break;
					case 6 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
						src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
						break;
				}					
							
				switch(deinterlace_mode)
				{
				case 1 :
					switch (idata.video_mode)
					{
						case 0 :
						case 1 :
							Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
								src_modulo,0,thr_dtl,tri_linear,map_filter);
							break;
						case 2 :
							Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
								src_modulo,0,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
							Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
							break;
						case 3 :
							Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
								src_modulo,0,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
							Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
							break;
						case 6 :
							Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
								(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[1],
								buffer_frame_U[1],buffer_frame_V[1],buffer_delta_Y,buffer_delta_U,
								buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>2,
								src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
								frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_dtl,tri_linear,map_filter,src1_RGB,
								src2_RGB,buffer_pitch,false);
							Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
								buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
								frame_buffer_pitch_V,buffer_pitch_planar420);
							break;
					}
					break;
				case 2 :
					switch (idata.video_mode)
					{
						case 0 :
						case 1 :
							Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,frame_buffer_pitch,
								tri_linear,true);
							break;
						case 2 :
							Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,frame_buffer_pitch,
								tri_linear,true,false);
							Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
							break;
						case 3 :
							Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,frame_buffer_pitch,
								tri_linear,true,false);
							Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
							break;
						case 6 :
							Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[1],w_Y,h_Y>>1,src_pitch_Y,
								frame_buffer_pitch_Y,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[1],w_U,h_U>>1,src_pitch_U,
								frame_buffer_pitch_U,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[1],w_V,h_V>>1,src_pitch_V,
								frame_buffer_pitch_V,tri_linear,true);
							Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
								buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
								frame_buffer_pitch_V,buffer_pitch_planar420);
							break;
					}
					break;
				}
			}
			if (((v&0xFF00)==0) || ((v&0x0400)==0x0400))
			{
			
				j=(uint8_t)((v&0x00F0) >> 4);
				if (pxsrc.mFrameNumber!=0)
					j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else j_tab=j;
				
				modif_bottom=(j_tab!=2) || swap_buffer;
				
				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src2_RGB=(void *)((uint8_t *)buffer[j].frameRGB32+buffer_pitch);
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						break;
					case 6 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
						src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
						break;
				}					
								
				j=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0)
					j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else j_tab=j;
				
				modif_top=(j_tab!=2) || swap_buffer;
				
				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}					
								
				if ((filter_output) || ((v&0x0400)==0x0400))
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter);
								break;
							case 2 :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 3 :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 6 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[1],
									buffer_frame_U[1],buffer_frame_V[1],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_out,tri_linear,map_filter,src1_RGB,
									src2_RGB,buffer_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
									buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					case 2 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true);
								break;
							case 2 :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 3 :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],((w+1)>>1)*h);
								break;
							case 6 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[1],
									w_Y,h_Y>>1,src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[1],
										w_U,h_U>>1,src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[1],
										w_V,h_V>>1,src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
								}
								else
								{
									if (w_U4)
										JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,buffer_frame_U[1],w_U>>2,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,buffer_frame_U[1],w_U,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									if (w_V4)
										JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,buffer_frame_V[1],w_V>>2,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,buffer_frame_V[1],w_V,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
									buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					}
				}
			}

			if (!previous_manual)
			{
				if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
				{
					modif_bottom=(i_tab!=2) || swap_buffer;
				
					idata.src_plane0=tab_pxsrc[i_tab]->data;
					idata.src_plane1=tab_pxsrc[i_tab]->data2;
					idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_pitch);
				
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	
					
					pitch2_Y=src_pitch_Y;
					pitch2_U=src_pitch_U;
					pitch2_V=src_pitch_V;
					modulo2_Y=src_modulo_Y;
					modulo2_U=src_modulo_U;
					modulo2_V=src_modulo_V;				
				}
				else
				{
					modif_bottom=true;
					
					src2_Y=buffer_frame_Y[0]+frame_buffer_pitch_Y;
					src2_U=buffer_frame_U[0]+frame_buffer_pitch_U;
					src2_V=buffer_frame_V[0]+frame_buffer_pitch_V;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_pitch);
					
					pitch2_Y=frame_buffer_pitch_Y;
					pitch2_U=frame_buffer_pitch_U;
					pitch2_V=frame_buffer_pitch_V;
					modulo2_Y=0;
					modulo2_U=0;
					modulo2_V=0;					
				}
			}
			else
			{
				if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
				{
					modif_bottom=(i_tab!=2) || swap_buffer;
				
					idata.src_plane0=tab_pxsrc[i_tab]->data;
					idata.src_plane1=tab_pxsrc[i_tab]->data2;
					idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
						+buffer_pitch);
				
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	
					
					pitch2_Y=src_pitch_Y;
					pitch2_U=src_pitch_U;
					pitch2_V=src_pitch_V;
					modulo2_Y=src_modulo_Y;
					modulo2_U=src_modulo_U;
					modulo2_V=src_modulo_V;							
				}
				else
				{
					modif_bottom=true;
					
					src2_Y=buffer_frame_Y[0]+frame_buffer_pitch_Y;
					src2_U=buffer_frame_U[0]+frame_buffer_pitch_U;
					src2_V=buffer_frame_V[0]+frame_buffer_pitch_V;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_pitch);
					
					pitch2_Y=frame_buffer_pitch_Y;
					pitch2_U=frame_buffer_pitch_U;
					pitch2_V=frame_buffer_pitch_V;
					modulo2_Y=0;
					modulo2_U=0;
					modulo2_V=0;										
				}
			}
			if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
			{
				modif_top=(j_tab!=2) || swap_buffer;
			
				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;				
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}
				
				pitch1_Y=src_pitch_Y;
				pitch1_U=src_pitch_U;
				pitch1_V=src_pitch_V;
				modulo1_Y=src_modulo_Y;
				modulo1_U=src_modulo_U;
				modulo1_V=src_modulo_V;							
			}
			else
			{
				modif_top=true;
				
				src1_Y=buffer_frame_Y[1];
				src1_U=buffer_frame_U[1];
				src1_V=buffer_frame_V[1];
				src1_RGB=buffer_frameRGB32[1];
				
				pitch1_Y=frame_buffer_pitch_Y;
				pitch1_U=frame_buffer_pitch_U;
				pitch1_V=frame_buffer_pitch_V;
				modulo1_Y=0;
				modulo1_U=0;
				modulo1_V=0;														
			}

			switch (idata.video_mode)
			{
				case 0 :
				case 1 :
					if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,pitch1_Y,dst_pitch,modulo1_Y,dst_modulo);
					if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,pitch2_Y,dst_pitch,modulo2_Y,dst_modulo);
					break;
				case 2 :
				case 3 :
					if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,pitch1_Y,dst_pitch,modulo1_Y,dst_modulo);
					if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,pitch2_Y,dst_pitch,modulo2_Y,dst_modulo);
					break;
				case 6 :
					if (modif_top)
					{
						if (w_Y4)
							JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,pitch1_Y,dst_pitch_Y,modulo1_Y,dst_modulo_Y);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,pitch1_Y,dst_pitch_Y,modulo1_Y,dst_modulo_Y);
						if (w_U4)
							JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,pitch1_U,dst_pitch_U,modulo1_U,dst_modulo_U);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,pitch1_U,dst_pitch_U,modulo1_U,dst_modulo_U);
						if (w_V4)
							JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,pitch1_V,dst_pitch_V,modulo1_V,dst_modulo_V);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,pitch1_V,dst_pitch_V,modulo1_V,dst_modulo_V);
					}
					if (modif_bottom)
					{
						if (w_Y4)
							JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,pitch2_Y,dst_pitch_Y,modulo2_Y,dst_modulo_Y);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,pitch2_Y,dst_pitch_Y,modulo2_Y,dst_modulo_Y);
						if (w_U4)
							JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,pitch2_U,dst_pitch_U,modulo2_U,dst_modulo_U);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,pitch2_U,dst_pitch_U,modulo2_U,dst_modulo_U);
						if (w_V4)
							JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,pitch2_V,dst_pitch_V,modulo2_V,dst_modulo_V);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,pitch2_V,dst_pitch_V,modulo2_V,dst_modulo_V);
					}					
					break;
			}
		}

		if ((!manual_current) && ((index_out==3) && (output_mode==2)))
		{
			if (!previous_manual)
			{
				v1=data_out[2];
				if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
				if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0)>>4);
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;				
			}
			else
			{
				v1=manual.data_out[2];
				if ((v1&0x0100)==0x0100) i=(uint8_t)(v1&0x000F);
				if (((v1&0xFF00)==0) && !filter_output) i=(uint8_t)((v1&0x00F0)>>4);
				if (pxsrc.mFrameNumber!=0) i_tab=i+2-index_out;
				else i_tab=i;
			}

			j=(unsigned char)(v&0x000F);
			if (pxsrc.mFrameNumber!=0)
				j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
			else j_tab=j;
			
			if ((v&0x0200)==0x0200)
			{			
				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}					

				src2_RGB=(void *)((uint8_t *)src1_RGB+buffer_pitch);

				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						break;
					case 6 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
						src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
						break;
				}							
							
				switch(deinterlace_mode)
				{
				case 1 :
					switch (idata.video_mode)
					{
						case 0 :
						case 1 :
							Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
								src_modulo,0,thr_dtl,tri_linear,map_filter);
							break;
						case 2 :
							Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
								src_modulo,0,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
							Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
							break;
						case 3 :
							Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
								src_modulo,0,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,false);
							Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
							break;
						case 6 :
							Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
								(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
								buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
								buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
								src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
								frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_dtl,tri_linear,map_filter,src1_RGB,
								src2_RGB,buffer_pitch,false);
							Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
								buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
								frame_buffer_pitch_V,buffer_pitch_planar420);
							break;
					}
					break;
				case 2 :
					switch (idata.video_mode)
					{
						case 0 :
						case 1 :
							Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,frame_buffer_pitch,
								tri_linear,true);
							break;
						case 2 :
							Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,frame_buffer_pitch,
								tri_linear,true,false);
							Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
							break;
						case 3 :
							Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,frame_buffer_pitch,
								tri_linear,true,false);
							Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
							break;
						case 6 :
							Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
								src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
								src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
								src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
							Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
								buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
								frame_buffer_pitch_V,buffer_pitch_planar420);
							break;
					}
					break;
				}
			}
			
			if (((v&0xFF00)==0) || ((v&0x0400)==0x0400))
			{
				j=(uint8_t)((v&0x00F0) >> 4);
				if (pxsrc.mFrameNumber!=0)
					j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else j_tab=j;
				
				modif_bottom=(j_tab!=2) || swap_buffer;

				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src2_RGB=(void *)((uint8_t *)buffer[j].frameRGB32+buffer_pitch);
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						break;
					case 6 :
						src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
						src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
						src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
						break;
				}					
											
				j=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0)
					j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else j_tab=j;

				modif_top=(j_tab!=2) || swap_buffer;

				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}						
								
				if ((filter_output) || ((v&0x0400)==0x0400))
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter);
								break;
							case 2 :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 3 :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,frame_buffer_pitch,
									src_modulo,0,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 6 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
									buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,frame_buffer_pitch_Y,
									frame_buffer_pitch_U,frame_buffer_pitch_V,0,0,0,thr_out,tri_linear,map_filter,src1_RGB,
									src2_RGB,buffer_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					case 2 :
						switch (idata.video_mode)
						{
							case 0 :
							case 1 :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true);
								break;
							case 2 :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 3 :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									frame_buffer_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],((w+1)>>1)*h);
								break;
							case 6 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
									src_pitch_Y,frame_buffer_pitch_Y,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
										src_pitch_U,frame_buffer_pitch_U,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
										src_pitch_V,frame_buffer_pitch_V,tri_linear,true);
								}
								else
								{
									if (w_U4)
										JPSDR_IVTC_Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],w_U>>2,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_U,src2_U,buffer_frame_U[0],w_U,h_U>>1,
											src_pitch_U,frame_buffer_pitch_U,src_modulo_U,0);
									if (w_V4)
										JPSDR_IVTC_Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],w_V>>2,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
									else
										JPSDR_IVTC_Rebuild_Frame8(src1_V,src2_V,buffer_frame_V[0],w_V,h_V>>1,
											src_pitch_V,frame_buffer_pitch_V,src_modulo_V,0);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,frame_buffer_pitch_Y,frame_buffer_pitch_U,
									frame_buffer_pitch_V,buffer_pitch_planar420);
								break;
						}
						break;
					}
				}
			}

			if (!previous_manual)
			{
				if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
				{
					modif_bottom=(i_tab!=2) || swap_buffer;
				
					idata.src_plane0=tab_pxsrc[i_tab]->data;
					idata.src_plane1=tab_pxsrc[i_tab]->data2;
					idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_pitch);
				
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	
					
					pitch2_Y=src_pitch_Y;
					pitch2_U=src_pitch_U;
					pitch2_V=src_pitch_V;
					modulo2_Y=src_modulo_Y;
					modulo2_U=src_modulo_U;
					modulo2_V=src_modulo_V;
					
				}
				else
				{
					modif_bottom=true;
					
					src2_Y=buffer_frame_Y[1]+frame_buffer_pitch_Y;
					src2_U=buffer_frame_U[1]+frame_buffer_pitch_U;
					src2_V=buffer_frame_V[1]+frame_buffer_pitch_V;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_pitch);
					
					pitch2_Y=frame_buffer_pitch_Y;
					pitch2_U=frame_buffer_pitch_U;
					pitch2_V=frame_buffer_pitch_V;
					modulo2_Y=0;
					modulo2_U=0;
					modulo2_V=0;										
				}
			}
			else
			{
				if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
				{			
					modif_bottom=(i_tab!=2) || swap_buffer;
				
					idata.src_plane0=tab_pxsrc[i_tab]->data;
					idata.src_plane1=tab_pxsrc[i_tab]->data2;
					idata.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
						+buffer_pitch);
				
					switch (idata.video_mode)
					{
						case 1 :
						case 2 :
						case 3 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							break;
						case 6 :
							src2_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-2)*idata.src_pitch0));
							src2_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-2)*idata.src_pitch1));
							src2_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-2)*idata.src_pitch2));
							break;
					}	
					
					pitch2_Y=src_pitch_Y;
					pitch2_U=src_pitch_U;
					pitch2_V=src_pitch_V;
					modulo2_Y=src_modulo_Y;
					modulo2_U=src_modulo_U;
					modulo2_V=src_modulo_V;
				
				}
				else
				{
					modif_bottom=true;
					
					src2_Y=buffer_frame_Y[1]+frame_buffer_pitch_Y;
					src2_U=buffer_frame_U[1]+frame_buffer_pitch_U;
					src2_V=buffer_frame_V[1]+frame_buffer_pitch_V;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_pitch);

					pitch2_Y=frame_buffer_pitch_Y;
					pitch2_U=frame_buffer_pitch_U;
					pitch2_V=frame_buffer_pitch_V;
					modulo2_Y=0;
					modulo2_U=0;
					modulo2_V=0;															
				}
			}				
			if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
			{
				modif_top=(j_tab!=2) || swap_buffer;
			
				idata.src_plane0=tab_pxsrc[j_tab]->data;
				idata.src_plane1=tab_pxsrc[j_tab]->data2;
				idata.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;				
				
				switch (idata.video_mode)
				{
					case 1 :
					case 2 :
					case 3 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						break;
					case 6 :
						src1_Y=(void *)((uint8_t *)idata.src_plane0-((idata.src_h0-1)*idata.src_pitch0));
						src1_U=(void *)((uint8_t *)idata.src_plane1-((idata.src_h1-1)*idata.src_pitch1));
						src1_V=(void *)((uint8_t *)idata.src_plane2-((idata.src_h2-1)*idata.src_pitch2));
						break;
				}
				
				pitch1_Y=src_pitch_Y;
				pitch1_U=src_pitch_U;
				pitch1_V=src_pitch_V;
				modulo1_Y=src_modulo_Y;
				modulo1_U=src_modulo_U;
				modulo1_V=src_modulo_V;			
			
			}
			else
			{
				modif_top=true;
				
				src1_Y=buffer_frame_Y[0];
				src1_U=buffer_frame_U[0];
				src1_V=buffer_frame_V[0];
				src1_RGB=buffer_frameRGB32[0];

				pitch1_Y=frame_buffer_pitch_Y;
				pitch1_U=frame_buffer_pitch_U;
				pitch1_V=frame_buffer_pitch_V;
				modulo1_Y=0;
				modulo1_U=0;
				modulo1_V=0;																		
			}

			switch (idata.video_mode)
			{
				case 0 :
				case 1 :
					if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w,h>>1,pitch1_Y,dst_pitch,modulo1_Y,dst_modulo);
					if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w,h>>1,pitch2_Y,dst_pitch,modulo2_Y,dst_modulo);
					break;
				case 2 :
				case 3 :
					if (modif_top) JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,(w+1)>>1,h>>1,pitch1_Y,dst_pitch,modulo1_Y,dst_modulo);
					if (modif_bottom) JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,(w+1)>>1,h>>1,pitch2_Y,dst_pitch,modulo2_Y,dst_modulo);
					break;
				case 6 :
					if (modif_top)
					{
						if (w_Y4)
							JPSDR_IVTC_Rebuild_Frame_2(src1_Y,dst1_Y,w_Y>>2,h_Y>>1,pitch1_Y,dst_pitch_Y,modulo1_Y,dst_modulo_Y);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src1_Y,dst1_Y,w_Y,h_Y>>1,pitch1_Y,dst_pitch_Y,modulo1_Y,dst_modulo_Y);
						if (w_U4)
							JPSDR_IVTC_Rebuild_Frame_2(src1_U,dst1_U,w_U>>2,h_U>>1,pitch1_U,dst_pitch_U,modulo1_U,dst_modulo_U);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src1_U,dst1_U,w_U,h_U>>1,pitch1_U,dst_pitch_U,modulo1_U,dst_modulo_U);
						if (w_V4)
							JPSDR_IVTC_Rebuild_Frame_2(src1_V,dst1_V,w_V>>2,h_V>>1,pitch1_V,dst_pitch_V,modulo1_V,dst_modulo_V);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src1_V,dst1_V,w_V,h_V>>1,pitch1_V,dst_pitch_V,modulo1_V,dst_modulo_V);
					}
					if (modif_bottom)
					{
						if (w_Y4)
							JPSDR_IVTC_Rebuild_Frame_2(src2_Y,dst2_Y,w_Y>>2,h_Y>>1,pitch2_Y,dst_pitch_Y,modulo2_Y,dst_modulo_Y);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src2_Y,dst2_Y,w_Y,h_Y>>1,pitch2_Y,dst_pitch_Y,modulo2_Y,dst_modulo_Y);
						if (w_U4)
							JPSDR_IVTC_Rebuild_Frame_2(src2_U,dst2_U,w_U>>2,h_U>>1,pitch2_U,dst_pitch_U,modulo2_U,dst_modulo_U);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src2_U,dst2_U,w_U,h_U>>1,pitch2_U,dst_pitch_U,modulo2_U,dst_modulo_U);
						if (w_V4)
							JPSDR_IVTC_Rebuild_Frame_2(src2_V,dst2_V,w_V>>2,h_V>>1,pitch2_V,dst_pitch_V,modulo2_V,dst_modulo_V);
						else
							JPSDR_IVTC_Rebuild_Frame8_2(src2_V,dst2_V,w_V,h_V>>1,pitch2_V,dst_pitch_V,modulo2_V,dst_modulo_V);
					}
					break;	
			}
		}

		if (mData.manual_mode)
		{
			manual.ivtc_index++;
			if (manual.ivtc_index==5) manual.read_index=(manual.read_index+5)%Buffer_Size_Manual;
		}
		
		previous_manual=manual_current;
					
		if (index_out==4) out=true;
		else index_out++;	

//		mFrameNumber++;
}




void JPSDR_IVTC::End()
{
	int8_t i;

/*	if (fic1) fclose(fic1);
	if (fic2) fclose(fic2);
	if (fic3) fclose(fic3);*/

	for (i=Data_Buffer_Size-1; i>=0; i--)
	{
		myfree(buffer[i]._frameRGB32Resize);
	}
	myfree(_buffer_resize_V);
	myfree(_buffer_resize_U);
	myfree(_buffer_resize_Y);

	for (i=Interlaced_Tab_Size-1; i>=0; i--)
	{
		myfree(interlaced_tab[i]);
	}
	for (i=Data_Buffer_Size-1; i>=0; i--)
	{
		myfree(buffer[i]._frameRGB32);
	}
	for (i=Error_Fields_Size-1; i>=0; i--)
		myfree(error_Fields[i]);
	for (i=1; i>=0; i--)
	{
		myfree(_buffer_frameRGB32[i]);
		myfree(_buffer_frame[i]);
	}
	myfree(buffer_2);
	myfree(buffer_map);
	myfree(_buffer_delta);
}



void JPSDR_IVTC::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	int out_filter;

	mData.output_mode=argv[0].asInt();
	mData.threshold_ivtc=argv[1].asInt();
	out_filter=argv[2].asInt();
	mData.threshold_filter=argv[3].asInt();
	mData.threshold_deinterlace=argv[4].asInt();
	mData.inv_polar=!!argv[5].asInt();
	mData.line_removed=argv[6].asInt();
	mData.deinterlace_mode=argv[7].asInt();
	mData.disable_deinterlace=!!argv[8].asInt();
	mData.blend_anime=!!argv[9].asInt();
	mData.tri_linear=!!argv[10].asInt();
	mData.disable_Z_mode=!!argv[11].asInt();
	mData.disable_motion_map_filtering=!!argv[12].asInt();
	mData.first_last_mode=!!argv[13].asInt();
	mData.manual_mode=!!argv[14].asInt();
	strcpy_s(mData.filename,1024,*argv[15].asString());

	mData.output_filter=((out_filter & 0x01)!=0);
	mData.Y_only=((out_filter & 0x02)!=0);
}

bool JPSDR_IVTC::Configure(VDXHWND hwnd)
{
	JPSDR_IVTCDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_IVTC::GetScriptString(char *buf, int maxlen)
{
 	char save[1400];
	int i,j;
	int out_filter;

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

	if (mData.output_filter) out_filter=0x01;
	else out_filter=0x00;
	if (mData.Y_only) out_filter|=0x02;
    
    SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \"%s\")",
		mData.output_mode,mData.threshold_ivtc,out_filter,mData.threshold_filter,
		mData.threshold_deinterlace,mData.inv_polar,mData.line_removed,mData.deinterlace_mode,
		mData.disable_deinterlace,mData.blend_anime,mData.tri_linear,mData.disable_Z_mode,
		mData.disable_motion_map_filtering,mData.first_last_mode,mData.manual_mode,save);
}



extern VDXFilterDefinition filterDef_JPSDR_IVTC=
VDXVideoFilterDefinition<JPSDR_IVTC>("JPSDR","IVTC v5.1.2","IVTC Filter. [MMX][SSE] Optimised.");

