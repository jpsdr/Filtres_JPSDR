/*
 *  IVTC
 *
 *  Automatic 2:3 pulldown detection.
 *  Copyright (C) 2004 JPSDR
 *	
 *  IVTC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  IVTC is distributed in the hope that it will be useful,
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

#include "IVTC_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Pascal.h"

#include "..\Filtres_JPSDR\ThreadPoolInterface.h"

extern int g_VFVAPIVersion;

extern ThreadPoolInterface *poolInterface;

extern "C" uint32_t JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32(const void *src,
		uint8_t *map,void *dst,uint32_t *histo,uint32_t *repart,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch,ptrdiff_t dst_modulo,int32_t w_map,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32(const void *src,
		uint8_t *map,void *dst,uint32_t *histo,uint32_t *repart,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch,ptrdiff_t dst_modulo,int32_t w_map,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_AVX_RGB32(const void *src,
		uint8_t *map,void *dst,uint32_t *histo,uint32_t *repart,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t src_modulo,
		ptrdiff_t dst_pitch,ptrdiff_t dst_modulo,int32_t w_map,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32(const void *src,uint8_t *map,
		int32_t w,int32_t h,int32_t w_map,ptrdiff_t src_pitch,ptrdiff_t src_modulo,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32(const void *src,uint8_t *map,
		int32_t w,int32_t h,int32_t w_map,ptrdiff_t src_pitch,ptrdiff_t src_modulo,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_DeltaPicture_Motion_Map_AVX_RGB32(const void *src,uint8_t *map,
		int32_t w,int32_t h,int32_t w_map,ptrdiff_t src_pitch,ptrdiff_t src_modulo,uint32_t &error_motion_map);
extern "C" uint32_t JPSDR_IVTC_Norme1_SSE_1_RGB32(const void *src1,const void *src2,int32_t w,
		int32_t h,ptrdiff_t pitch);
extern "C" uint32_t JPSDR_IVTC_Norme1_SSE_2_RGB32(const void *src1,const void *src2,int32_t w,
		int32_t h,ptrdiff_t pitch);
extern "C" uint32_t JPSDR_IVTC_Norme1_SSE2_RGB32(const void *src1,const void *src2,int32_t w,
		int32_t h,ptrdiff_t pitch);
extern "C" uint32_t JPSDR_IVTC_Norme1_AVX_RGB32(const void *src1,const void *src2,int32_t w,
		int32_t h,ptrdiff_t pitch);

extern "C" void JPSDR_IVTC_Motion_Map_SSE_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_SSE_RGB32_a(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_SSE_RGB32_b(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_SSE_RGB32_c(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);

extern "C" void JPSDR_IVTC_Motion_Map_SSE2_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_SSE2_RGB32_a(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_SSE2_RGB32_b(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_SSE2_RGB32_c(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);

extern "C" void JPSDR_IVTC_Motion_Map_AVX_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_AVX_RGB32_a(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_AVX_RGB32_b(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Motion_Map_AVX_RGB32_c(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,ptrdiff_t pitch_buffer,
		ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);


extern "C" void JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);

extern "C" void JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);
extern "C" void JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c(const void *src1,const void *src2,
		void *buffer,uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,
		ptrdiff_t pitch_buffer,ptrdiff_t modulo_buffer,uint32_t thr,int32_t w_map);

extern "C" void JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_2(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_2_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_2_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_3(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_3_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_SSE_3_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Blend_AVX(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_AVX_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Blend_AVX_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_AVX(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_c(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
extern "C" void JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c(const void *src1,const void *src2,void *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

extern "C" void JPSDR_IVTC_Convert420_to_YUY2_1(const uint8_t *scr_y,const uint8_t *src_u,
		const uint8_t *src_v,void *dst,int32_t w);

extern "C" void JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2(const void *src,void *dst,void *lookup,
		int32_t w, int32_t h, ptrdiff_t src_modulo, ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2(const void *src,void *dst,void *lookup,
		int32_t w, int32_t h, ptrdiff_t src_modulo, ptrdiff_t dst_modulo);

extern "C" void JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX(const void *src,void *dst,void *lookup,
		int32_t w, int32_t h, ptrdiff_t src_modulo, ptrdiff_t dst_modulo);
extern "C" void JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX(const void *src,void *dst,void *lookup,
		int32_t w, int32_t h, ptrdiff_t src_modulo, ptrdiff_t dst_modulo);


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
	void *frameRGB32;
	void *frameRGB32Resize;
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


typedef struct _MT_Data_Info
{
	void *src1,*src2,*src3,*src4;
	void *dst1,*dst2,*dst3;
	ptrdiff_t src_pitch1,src_pitch2,src_pitch3,src_pitch4;
	ptrdiff_t src_modulo1,src_modulo2,src_modulo3,src_modulo4;
	ptrdiff_t dst_pitch1,dst_pitch2,dst_pitch3;
	ptrdiff_t dst_modulo1,dst_modulo2,dst_modulo3;
	int32_t src_Y_h_min,src_Y_h_max,src_Y_w,src_Y_w_32;
	int32_t src_UV_h_min,src_UV_h_max,src_UV_w,src_UV_w_32;
	int32_t dst_Y_h_min,dst_Y_h_max,dst_Y_w,dst_Y_w_32;
	int32_t dst_UV_h_min,dst_UV_h_max,dst_UV_w,dst_UV_w_32;
	uint32_t offset;
	bool top,bottom;
	uint8_t data_u8;
	uint32_t data_u32_1,data_u32_2;
	int32_t data_i32;
} MT_Data_Info;



class JPSDR_IVTCData
{
public:
	bool output_filter,inv_polar,first_last_mode,disable_deinterlace,blend_anime,tri_linear,Y_only;
	bool disable_Z_mode,disable_motion_map_filtering,manual_mode,mt_mode;
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
	mt_mode=true;
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

	CheckDlgButton(mhdlg,IDC_ENABLE_MT,mData.mt_mode?BST_CHECKED:BST_UNCHECKED);	
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
	mData.mt_mode=!!IsDlgButtonChecked(mhdlg, IDC_ENABLE_MT);
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


class JPSDR_IVTC : public JPSDRVDXVideoFilter
{
public:
	JPSDR_IVTC(){}
	JPSDR_IVTC(const JPSDR_IVTC& a)
	{
		AVX_Enable=a.AVX_Enable;
		SSE2_Enable=a.SSE2_Enable;
		Integer_SSE_Enable=a.Integer_SSE_Enable;
		MMX_Enable=a.MMX_Enable;
		mData=a.mData;
		InternalInit();
	}
	virtual ~JPSDR_IVTC();

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
	void *buffer_delta,*buffer_frame[2];
	void *buffer_frameRGB32[2];
	void *buffer_resize_Y,*buffer_resize_U,*buffer_resize_V;
	uint8_t *buffer_frame_Y[2],*buffer_frame_U[2],*buffer_frame_V[2];
	uint8_t *buffer_delta_Y,*buffer_delta_U,*buffer_delta_V;
	uint8_t *buffer_map,*buffer_2;
	ptrdiff_t buffer_frame_pitch,buffer_frame_modulo;
	ptrdiff_t buffer_delta_pitch,buffer_delta_modulo32,buffer_delta_modulo16,buffer_delta_modulo8;
	ptrdiff_t buffer_delta_moduloR32,buffer_delta_moduloR16,buffer_delta_moduloR8;
	ptrdiff_t buffer_frameRGB32_pitch,buffer_frameRGB32_modulo;
	ptrdiff_t buffer_resize_Y_pitch,buffer_resize_Y_modulo;
	ptrdiff_t buffer_resize_U_pitch,buffer_resize_U_modulo,buffer_resize_V_pitch,buffer_resize_V_modulo;
	ptrdiff_t buffer_frame_Y_pitch,buffer_frame_Y_modulo;
	ptrdiff_t buffer_frame_U_pitch,buffer_frame_U_modulo,buffer_frame_V_pitch,buffer_frame_V_modulo;
	ptrdiff_t buffer_delta_Y_pitch,buffer_delta_Y_modulo;
	ptrdiff_t buffer_delta_U_pitch,buffer_delta_U_modulo,buffer_delta_V_pitch,buffer_delta_V_modulo;
	ptrdiff_t buffer_map_pitch,buffer_map_modulo,buffer_2_pitch,buffer_2_modulo;
	ptrdiff_t frameRGB32Resize_pitch,frameRGB32Resize_modulo;
	ptrdiff_t error_field_pitch,error_field_modulo;
	uint8_t first,last,last_removed_1,last_removed_2;
	bool out,previous_manual;
	uint16_t data_out[5];
	uint8_t index_out;
	bool invalid_first_last;
	bool *interlaced_tab_U[MAX_MT_THREADS][Interlaced_Tab_Size],*interlaced_tab_V[MAX_MT_THREADS][Interlaced_Tab_Size];
	bool resize_720x480,swap_buffer;
	sint32 resize_w0[720],resize_h0[240],resize_w1[360],resize_h1[120];
//	FILE *fic1,*fic2,*fic3;

	int16_t lookup_RGB[1280];
	uint16_t lookup_420[768];
	bool Integer_SSE_Enable,SSE2_Enable,MMX_Enable,AVX_Enable,VDub2_Enable;

	Public_MT_Data_Thread MT_Thread[MAX_MT_THREADS];
	MT_Data_Info MT_Data[MAX_MT_THREADS];
	uint8_t threads_number,total_cpu;
	bool threadpoolAllocated;
	uint16_t UserId;

	uint32_t histogramme_MT[MAX_MT_THREADS][256];
	uint32_t repartition_MT[MAX_MT_THREADS][256];

	ThreadPoolFunction StaticThreadpoolF;

	static void StaticThreadpool(void *ptr);

	void InternalInit(void);

	uint8_t CreateMTData(uint8_t max_threads,int32_t src_size_x,int32_t src_size_y,int32_t dst_size_x,int32_t dst_size_y,bool src_UV_w,bool src_UV_h,bool dst_UV_w,bool dst_UV_h);
	uint8_t CreateMTData(uint8_t max_threads,uint32_t size);

	void Deinterlace_Tri_Blend_RGB32(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_RGB32_MT(uint8_t thread_num);

	void Deinterlace_RGB32(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data);

	void Deinterlace_Tri_Blend_MMX_MT(uint8_t thread_num);
	void Deinterlace_Tri_Blend_SSE_MT(uint8_t thread_num);
	void Deinterlace_Tri_Blend_AVX_MT(uint8_t thread_num);

	void Deinterlace_Blend_Non_MMX_RGB32_MT(uint8_t thread_num);
	void Deinterlace_Blend_SSE_MT(uint8_t thread_num);
	void Deinterlace_Blend_SSE_2_MT(uint8_t thread_num);
	void Deinterlace_Blend_SSE_3_MT(uint8_t thread_num);
	void Deinterlace_Blend_AVX_MT(uint8_t thread_num);

	void DeltaPicture_Motion_Map_SSE_RGB32_MT(uint8_t thread_num);
	void DeltaPicture_Motion_Map_SSE2_RGB32_MT(uint8_t thread_num);
	void DeltaPicture_Motion_Map_AVX_RGB32_MT(uint8_t thread_num);

	void Norme1_SSE_RGB32_MT_1(uint8_t thread_num);
	void Norme1_SSE_RGB32_MT_2(uint8_t thread_num);
	void Norme1_SSE2_RGB32_MT(uint8_t thread_num);
	void Norme1_AVX_RGB32_MT(uint8_t thread_num);

	void Histogramme_DeltaPicture_Motion_Map_SSE_RGB32_MT(uint8_t thread_num);
	void Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32_MT(uint8_t thread_num);
	void Histogramme_DeltaPicture_Motion_Map_AVX_RGB32_MT(uint8_t thread_num);

	void Motion_Map_SSE_RGB32_MT(uint8_t thread_num);
	void Motion_Map_SSE2_RGB32_MT(uint8_t thread_num);
	void Motion_Map_AVX_RGB32_MT(uint8_t thread_num);

	void Smart_Deinterlace_Motion_Map_SSE_RGB32_MT(uint8_t thread_num);

	void Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_MT(uint8_t thread_num);

	void Motion_Map_RGB32(const void *src1,const void *src2,void *buffer,uint8_t *dst,const int32_t w,
		const int32_t h,const ptrdiff_t pitch,const ptrdiff_t pitch_buffer,const int32_t w_map,const uint8_t thrs);
	void Motion_Map_RGB32_MT(uint8_t thread_num);
	void Motion_Map_RGB32(const void *src1,const void *src2,uint8_t *dst,const int32_t w,const int32_t h,
		const ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs);
	void Motion_Map_RGB32_2_MT(uint8_t thread_num);

	void Motion_Map_YUYV_Y(const void *src1,const void *src2,uint8_t *dst,const int32_t w,const int32_t h,
		ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs);
	void Motion_Map_YUYV_Y_MT(uint8_t thread_num);

	void Motion_Map_UYVY_Y(const void *src1,const void *src2,uint8_t *dst,const int32_t w,const int32_t h,
		ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs);
	void Motion_Map_UYVY_Y_MT(uint8_t thread_num);

	void Motion_Map_Planar420_Y(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,const int32_t w,
		const int32_t h,ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs);
	void Motion_Map_Planar420_Y_MT(uint8_t thread_num);

	void Smart_Deinterlace_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer, uint8_t *dst,
		const int32_t w,const int32_t h,const ptrdiff_t pitch,const ptrdiff_t pitch_buffer,const int32_t w_map,const uint8_t thrs);
	void Smart_Deinterlace_Motion_Map_RGB32_MT(uint8_t thread_num);

	void Smart_Deinterlace_Tri_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *dst,const int32_t w,const int32_t h,const ptrdiff_t pitch,const ptrdiff_t pitch_buffer,const int32_t w_map,
		const uint8_t thrs);
	void Smart_Deinterlace_Tri_Motion_Map_RGB32_MT(uint8_t thread_num);

	void Motion_Map_Filter(uint8_t *map,uint8_t *buffer,const int32_t w,const int32_t h,const int32_t w_map);
	void Motion_Map_Filter_MT_1(uint8_t thread_num);
	void Motion_Map_Filter_MT_2(uint8_t thread_num);

	void Histogramme_DeltaPicture_RGB32_MT(uint8_t thread_num);
	uint32_t Norme1_Histogramme_DeltaPicture_RGB32(const void *src1,const void *src2,
		void *dst,void *buffer,uint8_t *map,uint8_t *buffer_map,
		uint32_t *histo,uint32_t *repart,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t buffer_pitch, const ptrdiff_t dst_pitch,const ptrdiff_t src_modulo,const ptrdiff_t buffer_modulo,
		const ptrdiff_t dst_modulo,uint32_t &error_motion_map,const uint8_t thrs);

	void Smart_Deinterlace_RGB32(const void *src1,const void *src2,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t dst_pitch,const ptrdiff_t buffer_pitch,const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,
		const ptrdiff_t buffer_modulo,const uint8_t thrs,bool tri_linear,bool map_filter);
	void Smart_Deinterlace_RGB32_MT(uint8_t thread_num);

	uint32_t Norme1_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,ptrdiff_t pitch,ptrdiff_t buffer_pitch,
		ptrdiff_t modulo,ptrdiff_t buffer_modulo,uint32_t &error_motion_map,const uint8_t thrs);
	void Norme1_Motion_Map_RGB32_MT(uint8_t thread_num);

	uint32_t Norme1_RGB32(const void *src1,const void *src2,const int32_t w,const int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo);
	void Norme1_RGB32_MT(uint8_t thread_num);

	void Convert_YUYV_to_RGB32(const void *src, void *dst, const int32_t w, const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t src_modulo,const ptrdiff_t dst_pitch,const ptrdiff_t dst_modulo);
	void Convert_UYVY_to_RGB32(const void *src, void *dst, const int32_t w, const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t src_modulo,const ptrdiff_t dst_pitch,const ptrdiff_t dst_modulo);

	void JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX_MT(uint8_t thread_num);
	void JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2_MT(uint8_t thread_num);
	void JPSDR_IVTC_Convert_YUYV_to_RGB32_2_MT(uint8_t thread_num);

	void JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX_MT(uint8_t thread_num);
	void JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2_MT(uint8_t thread_num);
	void JPSDR_IVTC_Convert_UYVY_to_RGB32_2_MT(uint8_t thread_num);

	void Convert_Planar420_to_RGB32(const uint8_t *src_Y,const uint8_t *src_U,const uint8_t *src_V, void *buffer,void *dst,
		const int32_t w,const int32_t h,const ptrdiff_t pitch_Y,const ptrdiff_t pitch_U,const ptrdiff_t pitch_V,
		const ptrdiff_t buffer_pitch, const ptrdiff_t dst_pitch,const ptrdiff_t buffer_modulo, const ptrdiff_t dst_modulo);
	void Convert_Automatic_Planar420_to_YUY2(uint8_t thread_num);

	void Deinterlace_Tri_Blend_YUYV(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_YUYV_MT(uint8_t thread_num);

	void Deinterlace_Blend_YUYV(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_YUYV_MT(uint8_t thread_num);

	void Deinterlace_Tri_Blend_YUYV_Y(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_YUYV_Y_MT(uint8_t thread_num);

	void Deinterlace_Blend_YUYV_Y(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_YUYV_Y_MT(uint8_t thread_num);

	void Smart_Deinterlace_YUYV(const void *src1,const void *src2,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t dst_pitch,const ptrdiff_t buffer_pitch,const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,
		const ptrdiff_t buffer_modulo,const uint8_t thrs,bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		const ptrdiff_t RGBBuff_pitch,bool Y_only);
	void Smart_Deinterlace_YUYV_MT(uint8_t thread_num);


	void Deinterlace_YUYV(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,bool tri_linear,bool align_data,bool Y_only);

	void Deinterlace_Tri_Blend_UYVY(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_UYVY_MT(uint8_t thread_num);

	void Deinterlace_Blend_UYVY(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_UYVY_MT(uint8_t thread_num);

	void Deinterlace_Tri_Blend_UYVY_Y(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_UYVY_Y_MT(uint8_t thread_num);

	void Deinterlace_Blend_UYVY_Y(const void *src1,const void *src2,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_UYVY_Y_MT(uint8_t thread_num);

	void Smart_Deinterlace_UYVY(const void *src1,const void *src2,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t dst_pitch,const ptrdiff_t buffer_pitch,const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,
		const ptrdiff_t buffer_modulo,const uint8_t thrs,bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		ptrdiff_t RGBBuff_pitch, bool Y_only);
	void Smart_Deinterlace_UYVY_MT(uint8_t thread_num);

	void Deinterlace_UYVY(const void *src1,const void *src2,void *dst,int32_t w,int32_t h,
		ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data, bool Y_only);

	void Deinterlace_Tri_Blend_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Tri_Blend_Planar_MT(uint8_t thread_num);

	void Deinterlace_Blend_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch);
	void Deinterlace_Blend_Planar_MT(uint8_t thread_num);

	void Smart_Deinterlace_Planar420(const uint8_t *src1_Y,const uint8_t *src2_Y,
		const uint8_t *src1_U,const uint8_t *src2_U,const uint8_t *src1_V,const uint8_t *src2_V,
		uint8_t *dst_Y,uint8_t *dst_U,uint8_t *dst_V,uint8_t *buffer_Y,uint8_t *buffer_U,
		uint8_t *buffer_V,uint8_t *map,uint8_t *buffer_map,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,
		const int32_t h_U,const int32_t h_V,const ptrdiff_t src_pitch_Y,const ptrdiff_t src_pitch_U,const ptrdiff_t src_pitch_V,
		const ptrdiff_t src_modulo_Y,const ptrdiff_t src_modulo_U,const ptrdiff_t src_modulo_V,const ptrdiff_t dst_pitch_Y,
		const ptrdiff_t dst_pitch_U,ptrdiff_t dst_pitch_V,const ptrdiff_t dst_modulo_Y,const ptrdiff_t dst_modulo_U,
		const ptrdiff_t dst_modulo_V,const ptrdiff_t buffer_pitch_Y,const ptrdiff_t buffer_pitch_U,const ptrdiff_t buffer_pitch_V,
		const ptrdiff_t buffer_modulo_Y,const ptrdiff_t buffer_modulo_U,const ptrdiff_t buffer_modulo_V,const uint8_t thrs,
		bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,ptrdiff_t RGBBuff_pitch, bool Y_only);
	void Smart_Deinterlace_Planar420_MT_Y(uint8_t thread_num);
	void Smart_Deinterlace_Planar420_MT_UV(uint8_t thread_num);

	void Deinterlace_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data);

	void Resize_Planar420(const void *src_y, const void *src_u, const void *src_v, void *dst_y, void *dst_u, void *dst_v,
		const ptrdiff_t src_pitch_y,const ptrdiff_t src_pitch_u,const ptrdiff_t src_pitch_v,const ptrdiff_t dst_pitch_y,
		const ptrdiff_t dst_pitch_u,const ptrdiff_t dst_pitch_v);

	void Resize_Planar420_MT(uint8_t thread_num);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_IVTCData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_IVTC)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_IVTC,ScriptConfig,"iiiiiiiiiiiiiiiis")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_IVTC::Init()
{
	AVX_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_AVX)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	Integer_SSE_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_INTEGER_SSE)!=0);
	MMX_Enable=ff->isMMXEnabled();
	InternalInit();

	return(true);
}


void JPSDR_IVTC::InternalInit(void)
{
	int16_t i,j;

	resize_720x480=false;
	buffer_resize_Y=NULL;
	buffer_resize_U=NULL;
	buffer_resize_V=NULL;
	buffer_delta=NULL;
	buffer_map=NULL;
	buffer_2=NULL;
	for (i=0; i<2; i++)
	{
		buffer_frame[i]=NULL;
		buffer_frameRGB32[i]=NULL;
	}
	for (i=0; i<Error_Fields_Size; i++)
		error_Fields[i]=NULL;
	for (i=0; i<Data_Buffer_Size; i++)
	{
		buffer[i].frameRGB32=NULL;
		buffer[i].frameRGB32Resize=NULL;
	}
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
		lookup_RGB[i]=(int16_t)round(1.164*(i-16)*64.0+32.0);
		lookup_RGB[i+256]=(int16_t)round(1.596*(i-128)*64.0);
		lookup_RGB[i+512]=(int16_t)round(-0.813*(i-128)*64.0);
		lookup_RGB[i+768]=(int16_t)round(-0.391*(i-128)*64.0);
		lookup_RGB[i+1024]=(int16_t)round(2.018*(i-128)*64.0);
		lookup_420[i]=(uint16_t)(i*3);
		lookup_420[i+256]=(uint16_t)(i*5);
		lookup_420[i+512]=(uint16_t)(i*7);
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


JPSDR_IVTC::~JPSDR_IVTC()
{
	if (threadpoolAllocated)
	{
		poolInterface->RemoveUserId(UserId);
		poolInterface->DeAllocateAllThreads(true);
	}
}



uint32 JPSDR_IVTC::GetParams()
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

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	if (mData.output_filter || !mData.disable_deinterlace)
	{
		swap_buffer=true;
		return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_SWAP_BUFFERS | FILTERPARAM_ALIGN_SCANLINES);
	}
	else
	{
		swap_buffer=false;
		return(FILTERPARAM_SUPPORTS_ALTFORMATS | FILTERPARAM_ALIGN_SCANLINES);
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


static inline void Rebuild_Frame(const void *bottom_src, const void *top_src, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	const uint8_t *src;
	uint8_t *dst;
	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	src=(uint8_t *)bottom_src;
	dst=(uint8_t *)dst_;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

	src=(uint8_t *)top_src;
	dst=(uint8_t *)dst_+dst_pitch;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

}


static inline void Rebuild_Frame_2(const void *src_, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	const uint8_t *src;
	uint8_t *dst;
	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	src=(uint8_t *)src_;
	dst=(uint8_t *)dst_;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

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


uint8_t JPSDR_IVTC::CreateMTData(uint8_t max_threads,int32_t src_size_x,int32_t src_size_y,int32_t dst_size_x,int32_t dst_size_y,bool src_UV_w,bool src_UV_h,bool dst_UV_w,bool dst_UV_h)
{
	if ((max_threads<=1) || (max_threads>threads_number))
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].src_Y_h_min=0;
		MT_Data[0].dst_Y_h_min=0;
		MT_Data[0].src_Y_h_max=src_size_y;
		MT_Data[0].dst_Y_h_max=dst_size_y;
		MT_Data[0].src_UV_h_min=0;
		MT_Data[0].dst_UV_h_min=0;
		if (src_UV_h) MT_Data[0].src_UV_h_max=src_size_y >> 1;
		else MT_Data[0].src_UV_h_max=src_size_y;
		if (dst_UV_h) MT_Data[0].dst_UV_h_max=dst_size_y >> 1;
		else MT_Data[0].dst_UV_h_max=dst_size_y;
		MT_Data[0].src_Y_w=src_size_x;
		MT_Data[0].dst_Y_w=dst_size_x;
		if (src_UV_w) MT_Data[0].src_UV_w=src_size_x >> 1;
		else MT_Data[0].src_UV_w=src_size_x;
		if (dst_UV_w) MT_Data[0].dst_UV_w=dst_size_x >> 1;
		else MT_Data[0].dst_UV_w=dst_size_x;
		return(1);
	}

	int32_t _y_min,_dh;
	int32_t src_dh_Y,src_dh_UV,dst_dh_Y,dst_dh_UV;
	int32_t h_y;
	uint8_t i,max_src=1,max_dst=1,max;

	dst_dh_Y=(dst_size_y+(uint32_t)max_threads-1)/(uint32_t)max_threads;
	if (dst_dh_Y<16) dst_dh_Y=16;
	if ((dst_dh_Y & 3)!=0) dst_dh_Y=((dst_dh_Y+3) >> 2) << 2;

	if (src_size_y==dst_size_y) src_dh_Y=dst_dh_Y;
	else
	{
		src_dh_Y=trunc((((float)src_size_y)/((float)dst_size_y))*(float)dst_dh_Y);
		if (src_dh_Y<16) src_dh_Y=16;
		if ((src_dh_Y & 3)!=0) src_dh_Y=((src_dh_Y+3) >> 2) << 2;
	}

	_y_min=src_size_y;
	_dh=src_dh_Y;
	h_y=_dh;
	while (h_y<(_y_min-16))
	{
		max_src++;
		h_y+=_dh;
	}

	_y_min=dst_size_y;
	_dh=dst_dh_Y;
	h_y=_dh;
	while (h_y<(_y_min-16))
	{
		max_dst++;
		h_y+=_dh;
	}

	max=(max_src<max_dst) ? max_src:max_dst;

	if (max==1)
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].src_Y_h_min=0;
		MT_Data[0].dst_Y_h_min=0;
		MT_Data[0].src_Y_h_max=src_size_y;
		MT_Data[0].dst_Y_h_max=dst_size_y;
		MT_Data[0].src_UV_h_min=0;
		MT_Data[0].dst_UV_h_min=0;
		if (src_UV_h) MT_Data[0].src_UV_h_max=src_size_y >> 1;
		else MT_Data[0].src_UV_h_max=src_size_y;
		if (dst_UV_h) MT_Data[0].dst_UV_h_max=dst_size_y >> 1;
		else MT_Data[0].dst_UV_h_max=dst_size_y;
		MT_Data[0].src_Y_w=src_size_x;
		MT_Data[0].dst_Y_w=dst_size_x;
		if (src_UV_w) MT_Data[0].src_UV_w=src_size_x >> 1;
		else MT_Data[0].src_UV_w=src_size_x;
		if (dst_UV_w) MT_Data[0].dst_UV_w=dst_size_x >> 1;
		else MT_Data[0].dst_UV_w=dst_size_x;
		return(1);
	}

	src_dh_UV= src_UV_h ? src_dh_Y>>1 : src_dh_Y;
	dst_dh_UV= dst_UV_h ? dst_dh_Y>>1 : dst_dh_Y;

	MT_Data[0].top=true;
	MT_Data[0].bottom=false;
	MT_Data[0].src_Y_h_min=0;
	MT_Data[0].src_Y_h_max=src_dh_Y;
	MT_Data[0].dst_Y_h_min=0;
	MT_Data[0].dst_Y_h_max=dst_dh_Y;
	MT_Data[0].src_UV_h_min=0;
	MT_Data[0].src_UV_h_max=src_dh_UV;
	MT_Data[0].dst_UV_h_min=0;
	MT_Data[0].dst_UV_h_max=dst_dh_UV;

	i=1;
	while (i<max)
	{
		MT_Data[i].top=false;
		MT_Data[i].bottom=false;
		MT_Data[i].src_Y_h_min=MT_Data[i-1].src_Y_h_max;
		MT_Data[i].src_Y_h_max=MT_Data[i].src_Y_h_min+src_dh_Y;
		MT_Data[i].dst_Y_h_min=MT_Data[i-1].dst_Y_h_max;
		MT_Data[i].dst_Y_h_max=MT_Data[i].dst_Y_h_min+dst_dh_Y;
		MT_Data[i].src_UV_h_min=MT_Data[i-1].src_UV_h_max;
		MT_Data[i].src_UV_h_max=MT_Data[i].src_UV_h_min+src_dh_UV;
		MT_Data[i].dst_UV_h_min=MT_Data[i-1].dst_UV_h_max;
		MT_Data[i].dst_UV_h_max=MT_Data[i].dst_UV_h_min+dst_dh_UV;
		i++;
	}

	MT_Data[max-1].bottom=true;
	MT_Data[max-1].src_Y_h_max=src_size_y;
	MT_Data[max-1].dst_Y_h_max=dst_size_y;
	if (src_UV_h) MT_Data[max-1].src_UV_h_max=src_size_y >> 1;
	else MT_Data[max-1].src_UV_h_max=src_size_y;
	if (dst_UV_h) MT_Data[max-1].dst_UV_h_max=dst_size_y >> 1;
	else MT_Data[max-1].dst_UV_h_max=dst_size_y;

	for (i=0; i<max; i++)
	{
		MT_Data[i].src_Y_w=src_size_x;
		MT_Data[i].dst_Y_w=dst_size_x;
		if (src_UV_w) MT_Data[i].src_UV_w=src_size_x >> 1;
		else MT_Data[i].src_UV_w=src_size_x;
		if (dst_UV_w) MT_Data[i].dst_UV_w=dst_size_x >> 1;
		else MT_Data[i].dst_UV_w=dst_size_x;
	}

	return(max);
}



uint8_t JPSDR_IVTC::CreateMTData(uint8_t max_threads,uint32_t size)
{
	if ((max_threads<=1) || (max_threads>threads_number))
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].offset=size;
		return(1);
	}

	uint32_t dsize,sizef;
	uint8_t i,max=1;

	dsize=(size+(uint32_t)max_threads-1)/(uint32_t)max_threads;
	if (dsize<64) dsize=64;
	if ((dsize & 63)!=0) dsize=((dsize+63) >> 6) << 6;

	sizef=dsize;
	while (sizef<(size-64))
	{
		max++;
		sizef+=dsize;
	}

	if (max==1)
	{
		MT_Data[0].top=true;
		MT_Data[0].bottom=true;
		MT_Data[0].offset=size;
		return(1);
	}

	MT_Data[0].top=true;
	MT_Data[0].bottom=false;
	MT_Data[0].offset=dsize;

	i=1;
	while (i<max)
	{
		MT_Data[i].top=false;
		MT_Data[i].bottom=false;
		MT_Data[i].offset=dsize;
		i++;
	}

	MT_Data[max-1].bottom=true;
	MT_Data[max-1].offset=size-((uint32_t)max-1)*dsize;

	return(max);
}


void JPSDR_IVTC::Start()
{
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
		ff->Except("This virtualdub version doesn't support this filter!");
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

	SetImageData(image_data,true);

	switch (image_data.video_mode)
	{
		case VMODE_PLANAR_YUV420 :
			if ((image_data.src_h0 & 3)!=0)
			{
				ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
				return;
			}
			break;
		default :
			if ((image_data.src_h0 & 1)!=0)
			{
				ff->Except("Vertical size must be multiple of 2!");
				return;
			}
			break;
	}

	if ((image_data.src_h0<8) || (image_data.src_w0<4))
	{
		ff->Except("Size must be at least 4x8!");
		return;
	}

	if ((4*mData.line_removed+2)>=image_data.src_h0)
	{
		ff->Except("More line removed than image size!");
		return;
	}

	if ((image_data.src_w0 & 3)==0) w_map=image_data.src_w0+4;
	else w_map=(((image_data.src_w0 >> 2)+1)<< 2)+4;
	
	manual.nbre_sequence=0;
	offset=0;
	line_file=0;

	if ((image_data.video_mode==VMODE_PLANAR_YUV420)
		&& ((image_data.src_h0>=600) || (image_data.src_w0>=800))) resize_720x480=true;
	else resize_720x480=false;

	if (mData.manual_mode)
	{
		if (strlen(mData.filename)==0)
		{
			ff->Except("No file selected for manual mode!");
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

	if (total_cpu==0)
	{
		ff->Except("Error with the TheadPool while getting CPU info!");
		return;
	}

	if (mData.mt_mode && ((image_data.src_h0>=32) && (image_data.dst_h0>=32))) threads_number=total_cpu;
	else threads_number=1;

	buffer_delta_pitch=(((image_data.src_w0 << 2)+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	buffer_delta_modulo32=buffer_delta_pitch-(image_data.src_w0 << 2);
	buffer_delta_modulo16=buffer_delta_pitch-(((image_data.src_w0+1)>>1) << 2);
	buffer_delta_modulo8=buffer_delta_pitch-image_data.src_w0;
	buffer_delta_moduloR32=buffer_delta_pitch-(720*4);
	buffer_delta_moduloR16=buffer_delta_pitch-(720*2);
	buffer_delta_moduloR8=buffer_delta_pitch-720;
	buffer_delta_Y_pitch=image_data.src_pitch0_al;
	buffer_delta_Y_modulo=image_data.src_modulo0_al;
	buffer_delta_U_pitch=image_data.src_pitch1_al;
	buffer_delta_U_modulo=image_data.src_modulo1_al;
	buffer_delta_V_pitch=image_data.src_pitch2_al;
	buffer_delta_V_modulo=image_data.src_modulo2_al;
	buffer_map_pitch=w_map*sizeof(uint8_t);
	buffer_map_modulo=0;
	buffer_2_pitch=buffer_map_pitch;
	buffer_2_modulo=buffer_map_modulo;
	buffer_frame_pitch=image_data.src_pitch0_al;
	buffer_frame_modulo=image_data.src_modulo0_al;
	buffer_frame_Y_pitch=image_data.src_pitch0_al;
	buffer_frame_Y_modulo=image_data.src_modulo0_al;
	buffer_frame_U_pitch=image_data.src_pitch1_al;
	buffer_frame_U_modulo=image_data.src_modulo1_al;
	buffer_frame_V_pitch=image_data.src_pitch2_al;
	buffer_frame_V_modulo=image_data.src_modulo2_al;
	buffer_frameRGB32_pitch=(((image_data.src_w0 << 2)+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	buffer_frameRGB32_modulo=buffer_frameRGB32_pitch-(image_data.src_w0 << 2);

	buffer_delta=(void *)_aligned_malloc((size_t)image_data.src_h0*(size_t)buffer_delta_pitch,ALIGN_SIZE); // Taille RGB32
	buffer_map=(uint8_t *)malloc((size_t)image_data.src_h0*(size_t)buffer_map_pitch);
	buffer_2=(uint8_t *)malloc((size_t)image_data.src_h0*(size_t)buffer_map_pitch);

	for (i=0; i<2; i++)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
				buffer_frame[i]=(void *)_aligned_malloc(image_data.src_size0_al,ALIGN_SIZE);
				break;
			case VMODE_PLANAR_YUV420 :
				buffer_frame[i]=(void *)_aligned_malloc(image_data.src_size0_al+image_data.src_size1_al+image_data.src_size2_al,ALIGN_SIZE);
				break;
		}
		switch (image_data.video_mode)
		{
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_PLANAR_YUV420 :
				buffer_frameRGB32[i]=(void *)_aligned_malloc((size_t)image_data.src_h0*(size_t)buffer_frameRGB32_pitch,ALIGN_SIZE);
				break;
		}
	}
	if (resize_720x480)
	{
		error_field_pitch=(((720 << 2)+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		error_field_modulo=error_field_pitch-(720 << 2);
		for (i=0; i<Error_Fields_Size; i++)
			error_Fields[i]=(void *)_aligned_malloc((size_t)error_field_pitch*(size_t)480,ALIGN_SIZE); // Taille RGB32
	}
	else
	{
		error_field_pitch=(((image_data.src_w0 << 2)+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		error_field_modulo=error_field_pitch-(image_data.src_w0 << 2);
		for (i=0; i<Error_Fields_Size; i++)
			error_Fields[i]=(void *)_aligned_malloc((size_t)error_field_pitch*(size_t)image_data.src_h0,ALIGN_SIZE); // Taille RGB32
	}
	for (i=0; i<Data_Buffer_Size; i++)
	{
		switch (image_data.video_mode)
		{
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_PLANAR_YUV420 :
				buffer[i].frameRGB32=(void *)_aligned_malloc((size_t)image_data.src_h0*(size_t)buffer_frameRGB32_pitch,ALIGN_SIZE);
				break;
		}
	}
	for(j=0 ; j<threads_number; j++)
	{
		for (i=0; i<Interlaced_Tab_Size; i++)
		{
			interlaced_tab_U[j][i]=(bool*)malloc(image_data.src_w1*sizeof(bool));
			interlaced_tab_V[j][i]=(bool*)malloc(image_data.src_w2*sizeof(bool));
		}
	}

	if (resize_720x480)
	{
		factor=720.0/(double)image_data.src_w0;
		for (i=0; i<720; i++)
		{
			resize_w0[i]=(sint32)trunc((i+0.5)/factor);
		}
		factor=360.0/(double)image_data.src_w1;
		for (i=0; i<360; i++)
		{
			resize_w1[i]=(sint32)trunc((i+0.5)/factor);
		}
		factor=240.0/(double)(image_data.src_h0>>1);
		for (i=0; i<240; i++)
		{
			resize_h0[i]=(sint32)trunc((i+0.5)/factor);
		}
		factor=120.0/(double)(image_data.src_h1>>1);
		for (i=0; i<120; i++)
		{
			resize_h1[i]=(sint32)trunc((i+0.5)/factor);
		}
		buffer_resize_Y_pitch=((720+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		buffer_resize_Y_modulo=buffer_resize_Y_pitch-720;
		buffer_resize_U_pitch=((360+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		buffer_resize_U_modulo=buffer_resize_U_pitch-368;
		buffer_resize_V_pitch=((360+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		buffer_resize_V_modulo=buffer_resize_V_pitch-368;
		frameRGB32Resize_pitch=((2880+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT; //720*4
		frameRGB32Resize_modulo=frameRGB32Resize_pitch-2880;
		buffer_resize_Y=(void *)_aligned_malloc((size_t)480*(size_t)buffer_resize_Y_pitch,ALIGN_SIZE);
		buffer_resize_U=(void *)_aligned_malloc((size_t)240*(size_t)buffer_resize_U_pitch,ALIGN_SIZE);
		buffer_resize_V=(void *)_aligned_malloc((size_t)240*(size_t)buffer_resize_V_pitch,ALIGN_SIZE);
		for (i=0; i<Data_Buffer_Size; i++)
		{
			buffer[i].frameRGB32Resize=(void *)_aligned_malloc((size_t)480*(size_t)frameRGB32Resize_pitch,ALIGN_SIZE);
		}
	}


	test=((buffer_delta!=NULL) && (buffer_map!=NULL) && (buffer_2!=NULL));
	for (i=0; i<2; i++)
	{
		test=test && (buffer_frame[i]!=NULL);
		switch (image_data.video_mode)
		{
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_PLANAR_YUV420 :
				test=test && (buffer_frameRGB32[i]!=NULL);
				break;
		}
	}
	for (i=0; i<Error_Fields_Size; i++)
		test=test && (error_Fields[i]!=NULL);
	for (i=0; i<Data_Buffer_Size; i++)
	{
		switch (image_data.video_mode)
		{
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_PLANAR_YUV420 :
				test=test && (buffer[i].frameRGB32!=NULL);
				break;
		}
	}
	for (j=0; j<threads_number; j++)
	{
		for (i=0; i<Interlaced_Tab_Size; i++)
		{
			test=test && (interlaced_tab_U[j][i]!=NULL);
			test=test && (interlaced_tab_V[j][i]!=NULL);
		}
	}
	if (resize_720x480)
	{
		test=test && (buffer_resize_Y!=NULL);
		test=test && (buffer_resize_U!=NULL);
		test=test && (buffer_resize_V!=NULL);
		for (i=0; i<Data_Buffer_Size; i++)
		{
			test=test && (buffer[i].frameRGB32Resize!=NULL);
		}
	}

	if (!test)
	{
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

	switch(image_data.video_mode)
	{
		case VMODE_PLANAR_YUV420 :
			offset_y=image_data.src_size0_al;
			offset_u=image_data.src_size1_al;
			break;
		default : offset_y=0; offset_u=0;
			break;
	}

	switch(image_data.video_mode)
	{
		case VMODE_PLANAR_YUV420 :
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
		switch (image_data.video_mode)
		{
			case VMODE_PLANAR_YUV420 :
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
	}
	if (mData.manual_mode)
	{
		for (i=0; i<Buffer_Size_Manual; i++)
		{
			switch (image_data.video_mode)
			{
				case VMODE_YUYV :
				case VMODE_UYVY :
				case VMODE_PLANAR_YUV420 :
					manual.bufferRGB32[i]=buffer[i].frameRGB32;
					break;
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


void JPSDR_IVTC::Resize_Planar420_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *_src_y=mt_data_inf.src1;
	const void *_src_u=mt_data_inf.src2;
	const void *_src_v=mt_data_inf.src3;
	void *_dst_y=mt_data_inf.dst1;
	void *_dst_u=mt_data_inf.dst2;
	void *_dst_v=mt_data_inf.dst3;
	const ptrdiff_t src_pitch_y=mt_data_inf.src_pitch1,dst_pitch_y=mt_data_inf.dst_pitch1;
	const ptrdiff_t src_pitch_u=mt_data_inf.src_pitch2,dst_pitch_u=mt_data_inf.dst_pitch2;
	const ptrdiff_t src_pitch_v=mt_data_inf.src_pitch3,dst_pitch_v=mt_data_inf.dst_pitch3;

	const sint32 *rs_w0=resize_w0;
	const sint32 *rs_w1=resize_w1;
	const int32_t Y_h_min=mt_data_inf.dst_Y_h_min,Y_h_max=mt_data_inf.dst_Y_h_max;
	const int32_t UV_h_min=mt_data_inf.dst_UV_h_min,UV_h_max=mt_data_inf.dst_UV_h_max;
	const ptrdiff_t src_pitch_y_2=src_pitch_y << 1,src_pitch_u_2=src_pitch_u << 1,src_pitch_v_2=src_pitch_v << 1;
	const ptrdiff_t dst_pitch_y_2=dst_pitch_y << 1,dst_pitch_u_2=dst_pitch_u << 1,dst_pitch_v_2=dst_pitch_v << 1;

	for (int32_t i=Y_h_min; i<Y_h_max; i++)
	{
		const uint8_t *src_y;
		uint8_t *dst_y;
		
		src_y=(uint8_t *)_src_y+(resize_h0[i]*src_pitch_y_2);
		dst_y=(uint8_t *)_dst_y+((i-Y_h_min)*dst_pitch_y_2);

		for (int32_t j=0; j<720; j++)
		{
			dst_y[j]=src_y[rs_w0[j]];
		}
	}
	for (int32_t i=Y_h_min; i<Y_h_max; i++)
	{
		const uint8_t *src_y;
		uint8_t *dst_y;
		
		src_y=(uint8_t *)_src_y+(src_pitch_y+resize_h0[i]*src_pitch_y_2);
		dst_y=(uint8_t *)_dst_y+(dst_pitch_y+(i-Y_h_min)*dst_pitch_y_2);

		for (int32_t j=0; j<720; j++)
		{
			dst_y[j]=src_y[rs_w0[j]];
		}
	}
	for (int32_t i=UV_h_min; i<UV_h_max; i++)
	{
		const uint8_t *src_u;
		uint8_t *dst_u;
		
		src_u=(uint8_t *)_src_u+(resize_h1[i]*src_pitch_u_2);
		dst_u=(uint8_t *)_dst_u+((i-UV_h_min)*dst_pitch_u_2);

		for (int32_t j=0; j<360; j++)
		{
			dst_u[j]=src_u[rs_w1[j]];
		}
	}
	for (int32_t i=UV_h_min; i<UV_h_max; i++)
	{
		const uint8_t *src_u;
		uint8_t *dst_u;
		
		src_u=(uint8_t *)_src_u+(src_pitch_u+resize_h1[i]*src_pitch_u_2);
		dst_u=(uint8_t *)_dst_u+(dst_pitch_u+(i-UV_h_min)*dst_pitch_u_2);

		for (int32_t j=0; j<360; j++)
		{
			dst_u[j]=src_u[rs_w1[j]];
		}
	}
	for (int32_t i=UV_h_min; i<UV_h_max; i++)
	{
		const uint8_t *src_v;
		uint8_t *dst_v;
		
		src_v=(uint8_t *)_src_v+(resize_h1[i]*src_pitch_v_2);
		dst_v=(uint8_t *)_dst_v+((i-UV_h_min)*dst_pitch_v_2);

		for (int32_t j=0; j<360; j++)
		{
			dst_v[j]=src_v[rs_w1[j]];
		}
	}
	for (int32_t i=UV_h_min; i<UV_h_max; i++)
	{
		const uint8_t *src_v;
		uint8_t *dst_v;
		
		src_v=(uint8_t *)_src_v+(src_pitch_v+resize_h1[i]*src_pitch_v_2);
		dst_v=(uint8_t *)_dst_v+(dst_pitch_v+(i-UV_h_min)*dst_pitch_v_2);

		for (int32_t j=0; j<360; j++)
		{
			dst_v[j]=src_v[rs_w1[j]];
		}
	}

}


void JPSDR_IVTC::Resize_Planar420(const void *_src_y, const void *_src_u, const void *_src_v, void *_dst_y, void *_dst_u,
	void *_dst_v,const ptrdiff_t src_pitch_y,const ptrdiff_t src_pitch_u,const ptrdiff_t src_pitch_v,const ptrdiff_t dst_pitch_y,
	const ptrdiff_t dst_pitch_u,const ptrdiff_t dst_pitch_v)
{

	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,720,240,720,240,true,true,true,true);
	
	for(uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)_src_y;
		MT_Data[i].src2=(void *)_src_u;
		MT_Data[i].src3=(void *)_src_v;
		MT_Data[i].src_pitch1=src_pitch_y;
		MT_Data[i].src_pitch2=src_pitch_u;
		MT_Data[i].src_pitch3=src_pitch_v;
		MT_Data[i].dst1=(void *)((uint8_t *)_dst_y+((dst_pitch_y << 1)*MT_Data[i].dst_Y_h_min));
		MT_Data[i].dst2=(void *)((uint8_t *)_dst_u+((dst_pitch_u << 1)*MT_Data[i].dst_UV_h_min));
		MT_Data[i].dst3=(void *)((uint8_t *)_dst_v+((dst_pitch_v << 1)*MT_Data[i].dst_UV_h_min));
		MT_Data[i].dst_pitch1=dst_pitch_y;
		MT_Data[i].dst_pitch2=dst_pitch_u;
		MT_Data[i].dst_pitch3=dst_pitch_v;
	}

	if (Nb_Threads>1)
	{
		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=10;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else Resize_Planar420_MT(0);
}



void JPSDR_IVTC::JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2(mt_data_inf.src1,mt_data_inf.dst1,lookup_RGB,
		(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}


void JPSDR_IVTC::JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX(mt_data_inf.src1,mt_data_inf.dst1,lookup_RGB,
		(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}


void JPSDR_IVTC::JPSDR_IVTC_Convert_YUYV_to_RGB32_2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const YUYV *src=(YUYV *)mt_data_inf.src1;
	RGB32BMP *dst=(RGB32BMP *)mt_data_inf.dst1;
	const int16_t *lookup=lookup_RGB;

	const ptrdiff_t src_modulo=mt_data_inf.src_modulo1,dst_modulo=mt_data_inf.dst_modulo1;
	const int32_t w1=(mt_data_inf.src_Y_w+1)>>1;
	const uint32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	for (uint32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w1; j++)
		{
			int16_t r,g,b;
			uint16_t y1,y2,u,v;

			y1=src->y1;
			u=src->u;
			y2=src->y2;
			v=(src++)->v;

			r=(lookup[y1]+lookup[256+v])>>6;
			g=(lookup[y1]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y1]+lookup[1024+u])>>6;
			if (r<0) dst->r=0;
			else
			{
				if (r>255) dst->r=255;
				else dst->r=(uint8_t)r;
			}
			if (g<0) dst->g=0;
			else
			{
				if (g>255) dst->g=255;
				else dst->g=(uint8_t)g;
			}
			if (b<0) dst->b=0;
			else
			{
				if (b>255) dst->b=255;
				else dst->b=(uint8_t)b;
			}
			(dst++)->alpha=0;

			r=(lookup[y2]+lookup[256+v])>>6;
			g=(lookup[y2]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y2]+lookup[1024+u])>>6;
			if (r<0) dst->r=0;
			else
			{
				if (r>255) dst->r=255;
				else dst->r=(uint8_t)r;
			}
			if (g<0) dst->g=0;
			else
			{
				if (g>255) dst->g=255;
				else dst->g=(uint8_t)g;
			}
			if (b<0) dst->b=0;
			else
			{
				if (b>255) dst->b=255;
				else dst->b=(uint8_t)b;
			}
			(dst++)->alpha=0;
		}
		src=(YUYV *)((uint8_t *)src+src_modulo);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);
	}
}



void JPSDR_IVTC::Convert_YUYV_to_RGB32(const void *src_0, void *dst_0, const int32_t w, const int32_t h,const ptrdiff_t src_pitch,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_pitch,const ptrdiff_t dst_modulo)
{		
	uint8_t Nb_Threads,f_proc=0;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,true,false,false,false);
	
	for(uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)src_0+(MT_Data[i].src_Y_h_min*src_pitch));
		MT_Data[i].src_pitch1=src_pitch;
		MT_Data[i].src_modulo1=src_modulo;
		MT_Data[i].dst1=(void *)((uint8_t *)dst_0+(MT_Data[i].dst_Y_h_min*dst_pitch));
		MT_Data[i].dst_pitch1=dst_pitch;
		MT_Data[i].dst_modulo1=dst_modulo;
	}

	if (Nb_Threads>1)
	{
		if (AVX_Enable) f_proc=56;
		else
		{
			if (SSE2_Enable) f_proc=4;
			else f_proc=5;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (AVX_Enable) JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX_MT(0);
		else
		{
			if (SSE2_Enable) JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2_MT(0);
			else JPSDR_IVTC_Convert_YUYV_to_RGB32_2_MT(0);
		}
	}

}



void JPSDR_IVTC::JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2(mt_data_inf.src1,mt_data_inf.dst1,lookup_RGB,
		(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}


void JPSDR_IVTC::JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX(mt_data_inf.src1,mt_data_inf.dst1,lookup_RGB,
		(mt_data_inf.src_Y_w+1)>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
		mt_data_inf.src_modulo1,mt_data_inf.dst_modulo1);
}



void JPSDR_IVTC::JPSDR_IVTC_Convert_UYVY_to_RGB32_2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const UYVY *src=(UYVY *)mt_data_inf.src1;
	RGB32BMP *dst=(RGB32BMP *)mt_data_inf.dst1;
	const int16_t *lookup=lookup_RGB;
	const ptrdiff_t src_modulo=mt_data_inf.src_modulo1,dst_modulo=mt_data_inf.dst_modulo1;
	const int32_t w1=(mt_data_inf.src_Y_w+1)>>1;
	const uint32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	for (uint32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w1; j++)
		{
			int16_t r,g,b;
			uint16_t y1,y2,u,v;

			u=src->u;
			y1=src->y1;
			v=src->v;
			y2=(src++)->y2;

			r=(lookup[y1]+lookup[256+v])>>6;
			g=(lookup[y1]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y1]+lookup[1024+u])>>6;
			if (r<0) dst->r=0;
			else
			{
				if (r>255) dst->r=255;
				else dst->r=(uint8_t)r;
			}
			if (g<0) dst->g=0;
			else
			{
				if (g>255) dst->g=255;
				else dst->g=(uint8_t)g;
			}
			if (b<0) dst->b=0;
			else
			{
				if (b>255) dst->b=255;
				else dst->b=(uint8_t)b;
			}
			(dst++)->alpha=0;

			r=(lookup[y2]+lookup[256+v])>>6;
			g=(lookup[y2]+lookup[512+v]+lookup[768+u])>>6;
			b=(lookup[y2]+lookup[1024+u])>>6;
			if (r<0) dst->r=0;
			else
			{
				if (r>255) dst->r=255;
				else dst->r=(uint8_t)r;
			}
			if (g<0) dst->g=0;
			else
			{
				if (g>255) dst->g=255;
				else dst->g=(uint8_t)g;
			}
			if (b<0) dst->b=0;
			else
			{
				if (b>255) dst->b=255;
				else dst->b=(uint8_t)b;
			}
			(dst++)->alpha=0;
		}
		src=(UYVY *)((uint8_t *)src+src_modulo);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);
	}
}


void JPSDR_IVTC::Convert_UYVY_to_RGB32(const void *src_0, void *dst_0, const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_pitch,const ptrdiff_t dst_modulo)
{
	uint8_t Nb_Threads,f_proc=0;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,true,false,false,false);
	
	for(uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)src_0+(MT_Data[i].src_Y_h_min*src_pitch));
		MT_Data[i].src_pitch1=src_pitch;
		MT_Data[i].src_modulo1=src_modulo;
		MT_Data[i].dst1=(void *)((uint8_t *)dst_0+(MT_Data[i].dst_Y_h_min*dst_pitch));
		MT_Data[i].dst_pitch1=dst_pitch;
		MT_Data[i].dst_modulo1=dst_modulo;
	}

	if (Nb_Threads>1)
	{
		if (AVX_Enable) f_proc=57;
		else
		{
			if (SSE2_Enable) f_proc=8;
			else f_proc=9;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (AVX_Enable) JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX_MT(0);
		else
		{
			if (SSE2_Enable) JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2_MT(0);
			else JPSDR_IVTC_Convert_UYVY_to_RGB32_2_MT(0);
		}
	}
}



void JPSDR_IVTC::Convert_Planar420_to_RGB32(const uint8_t *src_Y,const uint8_t *src_U,const uint8_t *src_V,void *buffer,void *dst,
	const int32_t w,const int32_t h,const ptrdiff_t pitch_Y,const ptrdiff_t pitch_U,const ptrdiff_t pitch_V,const ptrdiff_t buffer_pitch,
	const ptrdiff_t dst_pitch,const ptrdiff_t buffer_modulo,const ptrdiff_t dst_modulo)
{
	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,true,true,true,false);

	for(uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)(src_Y+(MT_Data[i].src_Y_h_min*pitch_Y));
		MT_Data[i].src2=(void *)(src_U+(MT_Data[i].src_UV_h_min*pitch_U));
		MT_Data[i].src3=(void *)(src_V+(MT_Data[i].src_UV_h_min*pitch_V));
		MT_Data[i].src_pitch1=pitch_Y;
		MT_Data[i].src_pitch2=pitch_U;
		MT_Data[i].src_pitch3=pitch_V;
		MT_Data[i].dst1=(void *)((uint8_t *)buffer+(MT_Data[i].dst_Y_h_min*buffer_pitch));
		MT_Data[i].dst_pitch1=buffer_pitch;
		MT_Data[i].dst_modulo1=buffer_modulo;
		MT_Data[i].data_u8=Threshold_Convert;
	}

	if (Nb_Threads>1)
	{
		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=1;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else Convert_Automatic_Planar420_to_YUY2(0);
	Convert_YUYV_to_RGB32(buffer,dst,w,h,buffer_pitch,buffer_modulo,dst_pitch,dst_modulo);
}



void JPSDR_IVTC::Convert_Automatic_Planar420_to_YUY2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const uint8_t *src_Y=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src_U=(uint8_t *)mt_data_inf.src2;
	const uint8_t *src_V=(uint8_t *)mt_data_inf.src3;
	YUYV *dst=(YUYV *)mt_data_inf.dst1;
	const int32_t w_Y=mt_data_inf.src_Y_w;
	const int32_t h_Y_min=mt_data_inf.src_Y_h_min;
	const int32_t h_Y_max=mt_data_inf.src_Y_h_max;
	const ptrdiff_t src_pitch_Y=mt_data_inf.src_pitch1;
	const ptrdiff_t src_pitch_U=mt_data_inf.src_pitch2;
	const ptrdiff_t src_pitch_V=mt_data_inf.src_pitch3;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;

	const uint8_t *src_Up,*src_Upp,*src_Un,*src_Unn,*src_Unnn;
	const uint8_t *src_Vp,*src_Vpp,*src_Vn,*src_Vnn,*src_Vnnn;
	uint8_t index_tab_0,index_tab_1,index_tab_2;
	const uint16_t *lookup=lookup_420;

	const ptrdiff_t pitch_U_2=2*src_pitch_U;
	const ptrdiff_t pitch_V_2=2*src_pitch_V;
	const int16_t threshold=mt_data_inf.data_u8;
	const int32_t w_UV=w_Y>>1;
	const int32_t w_Y2=(w_Y+1)>>1;
	const int32_t h_4 = mt_data_inf.bottom ? h_Y_max-4:h_Y_max;
	const int32_t h_0 = mt_data_inf.top ? 4:h_Y_min;

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
		for(int32_t y=0; y<4; y+=4)
		{
			JPSDR_IVTC_Convert420_to_YUY2_1(src_Y,src_U,src_V,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			JPSDR_IVTC_Convert420_to_YUY2_1(src_Y,src_Un,src_Vn,dst,w_Y2);
			dst=(YUYV *)((uint8_t *)dst+dst_pitch);
			src_Y+=src_pitch_Y;

			{
				int32_t i=0;

				for (int32_t j=0; j<w_UV; j++)
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
					
				for (int32_t j=0; j<w_UV; j++)
				{
					dst[j].y1=src_Y[i];
					dst[j].y2=src_Y[i+1];
					i+=2;

					if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
						(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
						((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j]) ||
						(src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j]))))
						itabu0[j]=true;
					else itabu0[j]=false;
					if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
						(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
						((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j]) ||
						(src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j]))))
						itabu1[j]=true;
					else itabu1[j]=false;

					dst[j].u=(uint8_t)((lookup[(uint16_t)src_Un[j]+512]+(uint16_t)src_Unnn[j]+4)>>3);
						
					if	(((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
						(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
						((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j]) ||
						(src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j]))))
						itabv0[j]=true;
					else itabv0[j]=false;
					if	(((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
						((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j]) ||
						(src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j]))))
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
					
		for (int32_t j=0; j<w_UV; j++)
		{
			if (((abs((int16_t)src_Upp[j]-(int16_t)src_Up[j])>=threshold) &&
				(abs((int16_t)src_U[j]-(int16_t)src_Up[j])>=threshold) &&
				((src_Upp[j]>src_Up[j]) && (src_U[j]>src_Up[j]) ||
				(src_Upp[j]<src_Up[j]) && (src_U[j]<src_Up[j]))))
				itabu0[j]=true;
			else itabu0[j]=false;
			if (((abs((int16_t)src_Up[j]-(int16_t)src_U[j])>=threshold) &&
				(abs((int16_t)src_Un[j]-(int16_t)src_U[j])>=threshold) &&
				((src_Up[j]>src_U[j]) && (src_Un[j]>src_U[j]) ||
				(src_Up[j]<src_U[j]) && (src_Un[j]<src_U[j]))))
				itabu1[j]=true;
			else itabu1[j]=false;
			
			if	(((abs((int16_t)src_Vpp[j]-(int16_t)src_Vp[j])>=threshold) &&
				(abs((int16_t)src_V[j]-(int16_t)src_Vp[j])>=threshold) &&
				((src_Vpp[j]>src_Vp[j]) && (src_V[j]>src_Vp[j]) ||
				(src_Vpp[j]<src_Vp[j]) && (src_V[j]<src_Vp[j]))))
				itabv0[j]=true;
			else itabv0[j]=false;
			if	(((abs((int16_t)src_Vp[j]-(int16_t)src_V[j])>=threshold) &&
				(abs((int16_t)src_Vn[j]-(int16_t)src_V[j])>=threshold) &&
				((src_Vp[j]>src_V[j]) && (src_Vn[j]>src_V[j]) ||
				(src_Vp[j]<src_V[j]) && (src_Vn[j]<src_V[j]))))
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

			for (int32_t j=0; j<w_UV; j++)
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

			for (int32_t j=0; j<w_UV; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_U[j]-(int16_t)src_Un[j])>=threshold) &&
					(abs((int16_t)src_Unn[j]-(int16_t)src_Un[j])>=threshold) &&
					((src_U[j]>src_Un[j]) && (src_Unn[j]>src_Un[j]) ||
					(src_U[j]<src_Un[j]) && (src_Unn[j]<src_Un[j]))))
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

				if	(((abs((int16_t)src_V[j]-(int16_t)src_Vn[j])>=threshold) &&
					(abs((int16_t)src_Vnn[j]-(int16_t)src_Vn[j])>=threshold) &&
					((src_V[j]>src_Vn[j]) && (src_Vnn[j]>src_Vn[j]) ||
					(src_V[j]<src_Vn[j]) && (src_Vnn[j]<src_Vn[j]))))
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

			for (int32_t j=0; j<w_UV; j++)
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

			for (int32_t j=0; j<w_UV; j++)
			{
				dst[j].y1=src_Y[i];
				dst[j].y2=src_Y[i+1];
				i+=2;

				if (((abs((int16_t)src_Un[j]-(int16_t)src_Unn[j])>=threshold) &&
					(abs((int16_t)src_Unnn[j]-(int16_t)src_Unn[j])>=threshold) &&
					((src_Un[j]>src_Unn[j]) && (src_Unnn[j]>src_Unn[j]) ||
					(src_Un[j]<src_Unn[j]) && (src_Unnn[j]<src_Unn[j]))))
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

				if	(((abs((int16_t)src_Vn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					(abs((int16_t)src_Vnnn[j]-(int16_t)src_Vnn[j])>=threshold) &&
					((src_Vn[j]>src_Vnn[j]) && (src_Vnnn[j]>src_Vnn[j]) ||
					(src_Vn[j]<src_Vnn[j]) && (src_Vnnn[j]<src_Vnn[j]))))
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

				for (int32_t j=0; j<w_UV; j++)
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

				for (int32_t j=0; j<w_UV; j++)
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
}


void JPSDR_IVTC::Deinterlace_Tri_Blend_RGB32(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	dst1=(RGB32BMP *)dst_0;

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst1[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst1[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst1[j].alpha=src1[j].alpha;
	}

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	src3=(RGB32BMP *)((uint8_t *)src1+src_pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+src_pitch2);

	dst1=(RGB32BMP *)((uint8_t *)dst_0+dst_pitch);
	dst2=(RGB32BMP *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t r2,r3,g2,g3,b2,b3;

			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			dst1[j].b=(uint8_t)((((uint16_t)src1[j].b+b3)+(b2 << 1)+2) >> 2);
			dst1[j].g=(uint8_t)((((uint16_t)src1[j].g+g3)+(g2 << 1)+2) >> 2);
			dst1[j].r=(uint8_t)((((uint16_t)src1[j].r+r3)+(r2 << 1)+2) >> 2);
			dst1[j].alpha=src2[j].alpha;
			dst2[j].b=(uint8_t)((((uint16_t)src4[j].b+b2)+(b3 << 1)+2) >> 2);
			dst2[j].g=(uint8_t)((((uint16_t)src4[j].g+g2)+(g3 << 1)+2) >> 2);
			dst2[j].r=(uint8_t)((((uint16_t)src4[j].r+r2)+(r3 << 1)+2) >> 2);
			dst2[j].alpha=src3[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+src_pitch2);
		dst1=(RGB32BMP *)((uint8_t *)dst1+dst_pitch2);
		dst2=(RGB32BMP *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
		dst1[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
		dst1[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
		dst1[j].alpha=src1[j].alpha;
	}
}


void JPSDR_IVTC::Deinterlace_Blend_Non_MMX_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (mt_data_inf.bottom)
		JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,
			mt_data_inf.dst_pitch1);
	else
		JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1<<1,
			mt_data_inf.dst_pitch1);
}



void JPSDR_IVTC::Deinterlace_Blend_SSE_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (mt_data_inf.bottom)
		JPSDR_IVTC_Deinterlace_Blend_SSE_b(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			mt_data_inf.src_Y_w_32,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
		JPSDR_IVTC_Deinterlace_Blend_SSE_a(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			mt_data_inf.src_Y_w_32,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
}


void JPSDR_IVTC::Deinterlace_Blend_SSE_2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (mt_data_inf.bottom)
		JPSDR_IVTC_Deinterlace_Blend_SSE_2_b(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			mt_data_inf.src_Y_w_32>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
		JPSDR_IVTC_Deinterlace_Blend_SSE_2_a(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			mt_data_inf.src_Y_w_32>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
}


void JPSDR_IVTC::Deinterlace_Blend_SSE_3_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (mt_data_inf.bottom)
		JPSDR_IVTC_Deinterlace_Blend_SSE_3_b(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			(mt_data_inf.src_Y_w_32+3)>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
		JPSDR_IVTC_Deinterlace_Blend_SSE_3_a(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			(mt_data_inf.src_Y_w_32+3)>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
}


void JPSDR_IVTC::Deinterlace_Blend_AVX_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	if (mt_data_inf.bottom)
		JPSDR_IVTC_Deinterlace_Blend_AVX_b(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			(mt_data_inf.src_Y_w_32+3)>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
		JPSDR_IVTC_Deinterlace_Blend_AVX_a(mt_data_inf.src1,mt_data_inf.src2,mt_data_inf.dst1,
			(mt_data_inf.src_Y_w_32+3)>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,
			mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
}


void JPSDR_IVTC::Deinterlace_Tri_Blend_SSE_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0,*src2_0;

	if (mt_data_inf.top)
	{
		src1_0=mt_data_inf.src1;
		src2_0=mt_data_inf.src2;
	}
	else
	{
		src1_0=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2_0=mt_data_inf.src1;
	}
	if (mt_data_inf.top)
		JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a(src1_0,src2_0,mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c(src1_0,src2_0,mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
		else
			JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b(src1_0,src2_0,mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	}
}


void JPSDR_IVTC::Deinterlace_Tri_Blend_AVX_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0,*src2_0;

	if (mt_data_inf.top)
	{
		src1_0=mt_data_inf.src1;
		src2_0=mt_data_inf.src2;
	}
	else
	{
		src1_0=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2_0=mt_data_inf.src1;
	}
	if (mt_data_inf.top)
		JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_a(src1_0,src2_0,mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_c(src1_0,src2_0,mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
		else
			JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_b(src1_0,src2_0,mt_data_inf.dst1,(mt_data_inf.src_Y_w_32+3)>>2,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	}
}


void JPSDR_IVTC::Deinterlace_Tri_Blend_MMX_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0,*src2_0;

	if (mt_data_inf.top)
	{
		src1_0=mt_data_inf.src1;
		src2_0=mt_data_inf.src2;
	}
	else
	{
		src1_0=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2_0=mt_data_inf.src1;
	}
	if (mt_data_inf.top)
		JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a(src1_0,src2_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c(src1_0,src2_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
		else
			JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b(src1_0,src2_0,mt_data_inf.dst1,mt_data_inf.src_Y_w_32,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1<<1,mt_data_inf.dst_pitch1);
	}
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(RGB32BMP *)src1_0;
		src2=(RGB32BMP *)src2_0;
		dst1=(RGB32BMP *)dst_0;

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
			dst1[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
			dst1[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
			dst1[j].alpha=src1[j].alpha;
		}
		dst1=(RGB32BMP *)((uint8_t *)dst_0+dst_pitch);
	}
	else
	{
		src1=(RGB32BMP *)((uint8_t *)src2_0-src_pitch2);
		src2=(RGB32BMP *)src1_0;
		dst1=(RGB32BMP *)dst_0;
	}

	dst2=(RGB32BMP *)((uint8_t *)dst1+dst_pitch);
	src3=(RGB32BMP *)((uint8_t *)src1+src_pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+src_pitch2);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t r2,r3,g2,g3,b2,b3;

			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			dst1[j].b=(uint8_t)((((uint16_t)src1[j].b+b3)+(b2 << 1)+2) >> 2);
			dst1[j].g=(uint8_t)((((uint16_t)src1[j].g+g3)+(g2 << 1)+2) >> 2);
			dst1[j].r=(uint8_t)((((uint16_t)src1[j].r+r3)+(r2 << 1)+2) >> 2);
			dst1[j].alpha=src2[j].alpha;
			dst2[j].b=(uint8_t)((((uint16_t)src4[j].b+b2)+(b3 << 1)+2) >> 2);
			dst2[j].g=(uint8_t)((((uint16_t)src4[j].g+g2)+(g3 << 1)+2) >> 2);
			dst2[j].r=(uint8_t)((((uint16_t)src4[j].r+r2)+(r3 << 1)+2) >> 2);
			dst2[j].alpha=src3[j].alpha;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+src_pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+src_pitch2);
		dst1=(RGB32BMP *)((uint8_t *)dst1+dst_pitch2);
		dst2=(RGB32BMP *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].b=(uint8_t)((((uint16_t)src1[j].b+(uint16_t)src3[j].b)+((uint16_t)src2[j].b << 1)+2) >> 2);
			dst1[j].g=(uint8_t)((((uint16_t)src1[j].g+(uint16_t)src3[j].g)+((uint16_t)src2[j].g << 1)+2) >> 2);
			dst1[j].r=(uint8_t)((((uint16_t)src1[j].r+(uint16_t)src3[j].r)+((uint16_t)src2[j].r << 1)+2) >> 2);
			dst1[j].alpha=src2[j].alpha;
		}
	}

	if (mt_data_inf.bottom)
	{
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch);
		dst1=(RGB32BMP *)((uint8_t *)dst1+dst_pitch);

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].r=(uint8_t)(((uint16_t)src1[j].r+(uint16_t)src2[j].r+1)>>1);
			dst1[j].g=(uint8_t)(((uint16_t)src1[j].g+(uint16_t)src2[j].g+1)>>1);
			dst1[j].b=(uint8_t)(((uint16_t)src1[j].b+(uint16_t)src2[j].b+1)>>1);
			dst1[j].alpha=src1[j].alpha;
		}
	}

}



void JPSDR_IVTC::Deinterlace_Tri_Blend_YUYV(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3,*src4;
	YUYV *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst1=(YUYV *)dst_0;

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);
	src4=(YUYV *)((uint8_t *)src2+src_pitch2);

	dst1=(YUYV *)((uint8_t *)dst_0+dst_pitch);
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;

			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			y3_1=src3[j].y1;
			u3=src3[j].u;
			y3_2=src3[j].y2;
			v3=src3[j].v;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].u=(uint8_t)((((uint16_t)src1[j].u+u3)+(u2 << 1)+2) >> 2);
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst1[j].v=(uint8_t)((((uint16_t)src1[j].v+v3)+(v2 << 1)+2) >> 2);
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].u=(uint8_t)((((uint16_t)src4[j].u+u2)+(u3 << 1)+2) >> 2);
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
			dst2[j].v=(uint8_t)((((uint16_t)src4[j].v+v2)+(v3 << 1)+2) >> 2);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		src4=(YUYV *)((uint8_t *)src4+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
	}

}



void JPSDR_IVTC::Deinterlace_Tri_Blend_YUYV_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const YUYV *src1,*src2,*src3,*src4;
	YUYV *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(YUYV *)src1_0;
		src2=(YUYV *)src2_0;
		dst1=(YUYV *)dst_0;

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
		}

		dst1=(YUYV *)((uint8_t *)dst_0+dst_pitch);
	}
	else
	{
		src1=(YUYV *)((uint8_t *)src2_0-src_pitch2);
		src2=(YUYV *)src1_0;
		dst1=(YUYV *)dst_0;
	}

	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);
	src4=(YUYV *)((uint8_t *)src2+src_pitch2);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;

			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			y3_1=src3[j].y1;
			u3=src3[j].u;
			y3_2=src3[j].y2;
			v3=src3[j].v;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].u=(uint8_t)((((uint16_t)src1[j].u+u3)+(u2 << 1)+2) >> 2);
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst1[j].v=(uint8_t)((((uint16_t)src1[j].v+v3)+(v2 << 1)+2) >> 2);
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].u=(uint8_t)((((uint16_t)src4[j].u+u2)+(u3 << 1)+2) >> 2);
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
			dst2[j].v=(uint8_t)((((uint16_t)src4[j].v+v2)+(v3 << 1)+2) >> 2);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		src4=(YUYV *)((uint8_t *)src4+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1)+((uint16_t)src2[j].y1 << 1)+2) >> 2);
			dst1[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u)+((uint16_t)src2[j].u << 1)+2) >> 2);
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2)+((uint16_t)src2[j].y2 << 1)+2) >> 2);
			dst1[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v)+((uint16_t)src2[j].v << 1)+2) >> 2);
		}
	}

	if (mt_data_inf.bottom)
	{
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch);

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
		}
	}
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_YUYV_Y(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3,*src4;
	YUYV *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst1=(YUYV *)dst_0;

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].u=src1[j].u;
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst1[j].v=src1[j].v;
	}

	src3=(YUYV *)((uint8_t *)src1+src_pitch2);
	src4=(YUYV *)((uint8_t *)src2+src_pitch2);

	dst1=(YUYV *)((uint8_t *)dst_0+dst_pitch);
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,y3_1,y3_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			y3_1=src3[j].y1;
			y3_2=src3[j].y2;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].u=src2[j].u;
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst1[j].v=src2[j].v;
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].u=src3[j].u;
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
			dst2[j].v=src3[j].v;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		src4=(YUYV *)((uint8_t *)src4+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].u=src2[j].u;
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		dst1[j].v=src2[j].v;
	}
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_YUYV_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const YUYV *src1,*src2,*src3,*src4;
	YUYV *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(YUYV *)src1_0;
		src2=(YUYV *)src2_0;
		dst1=(YUYV *)dst_0;

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].u=src1[j].u;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
			dst1[j].v=src1[j].v;
		}

		dst1=(YUYV *)((uint8_t *)dst_0+dst_pitch);
	}
	else
	{
		src1=(YUYV *)((uint8_t *)src2_0-src_pitch2);
		src2=(YUYV *)src1_0;
		dst1=(YUYV *)dst_0;
	}

	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);
	src4=(YUYV *)((uint8_t *)src2+src_pitch2);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,y3_1,y3_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			y3_1=src3[j].y1;
			y3_2=src3[j].y2;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].u=src2[j].u;
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst1[j].v=src2[j].v;
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].u=src3[j].u;
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
			dst2[j].v=src3[j].v;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		src4=(YUYV *)((uint8_t *)src4+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1)+((uint16_t)src2[j].y1 << 1)+2) >> 2);
			dst1[j].u=src2[j].u;
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2)+((uint16_t)src2[j].y2 << 1)+2) >> 2);
			dst1[j].v=src2[j].v;
		}
	}

	if (mt_data_inf.bottom)
	{
		src1=(YUYV *)((uint8_t *)src1+src_pitch);
		src2=(YUYV *)((uint8_t *)src2+src_pitch);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch);

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].u=src2[j].u;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
			dst1[j].v=src2[j].v;
		}
	}
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_UYVY(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3,*src4;
	UYVY *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst1=(UYVY *)dst_0;

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
	}

	src3=(UYVY *)((uint8_t *)src1+src_pitch2);
	src4=(UYVY *)((uint8_t *)src2+src_pitch2);

	dst1=(UYVY *)((uint8_t *)dst_0+dst_pitch);
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;

			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			u3=src3[j].u;
			y3_1=src3[j].y1;
			v3=src3[j].v;
			y3_2=src3[j].y2;
			dst1[j].u=(uint8_t)((((uint16_t)src1[j].u+u3)+(u2 << 1)+2) >> 2);
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].v=(uint8_t)((((uint16_t)src1[j].v+v3)+(v2 << 1)+2) >> 2);
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst2[j].u=(uint8_t)((((uint16_t)src4[j].u+u2)+(u3 << 1)+2) >> 2);
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].v=(uint8_t)((((uint16_t)src4[j].v+v2)+(v3 << 1)+2) >> 2);
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		src4=(UYVY *)((uint8_t *)src4+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
	}
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_UYVY_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const UYVY *src1,*src2,*src3,*src4;
	UYVY *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(UYVY *)src1_0;
		src2=(UYVY *)src2_0;
		dst1=(UYVY *)dst_0;

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		}

		dst1=(UYVY *)((uint8_t *)dst_0+dst_pitch);
	}
	else
	{
		src1=(UYVY *)((uint8_t *)src2_0-src_pitch2);
		src2=(UYVY *)src1_0;
		dst1=(UYVY *)dst_0;
	}

	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);
	src3=(UYVY *)((uint8_t *)src1+src_pitch2);
	src4=(UYVY *)((uint8_t *)src2+src_pitch2);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2,y3_1,y3_2,u3,v3;

			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			u3=src3[j].u;
			y3_1=src3[j].y1;
			v3=src3[j].v;
			y3_2=src3[j].y2;
			dst1[j].u=(uint8_t)((((uint16_t)src1[j].u+u3)+(u2 << 1)+2) >> 2);
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].v=(uint8_t)((((uint16_t)src1[j].v+v3)+(v2 << 1)+2) >> 2);
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst2[j].u=(uint8_t)((((uint16_t)src4[j].u+u2)+(u3 << 1)+2) >> 2);
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].v=(uint8_t)((((uint16_t)src4[j].v+v2)+(v3 << 1)+2) >> 2);
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		src4=(UYVY *)((uint8_t *)src4+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=(uint8_t)((((uint16_t)src1[j].u+(uint16_t)src3[j].u)+((uint16_t)src2[j].u << 1)+2) >> 2);
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1)+((uint16_t)src2[j].y1 << 1)+2) >> 2);
			dst1[j].v=(uint8_t)((((uint16_t)src1[j].v+(uint16_t)src3[j].v)+((uint16_t)src2[j].v << 1)+2) >> 2);
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2)+((uint16_t)src2[j].y2 << 1)+2) >> 2);
		}
	}

	if (mt_data_inf.bottom)
	{
		src1=(UYVY *)((uint8_t *)src1+src_pitch);
		src2=(UYVY *)((uint8_t *)src2+src_pitch);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch);

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u+1)>>1);
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v+1)>>1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		}
	}
}


void JPSDR_IVTC::Deinterlace_Tri_Blend_UYVY_Y(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3,*src4;
	UYVY *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst1=(UYVY *)dst_0;

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].u=src1[j].u;
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].v=src1[j].v;
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
	}

	src3=(UYVY *)((uint8_t *)src1+src_pitch2);
	src4=(UYVY *)((uint8_t *)src2+src_pitch2);

	dst1=(UYVY *)((uint8_t *)dst_0+dst_pitch);
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,y3_1,y3_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			y3_1=src3[j].y1;
			y3_2=src3[j].y2;
			dst1[j].u=src2[j].u;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].v=src2[j].v;
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst2[j].u=src3[j].u;
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].v=src3[j].v;
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		src4=(UYVY *)((uint8_t *)src4+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].u=src2[j].u;
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
		dst1[j].v=src2[j].v;
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
	}

}



void JPSDR_IVTC::Deinterlace_Tri_Blend_UYVY_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const UYVY *src1,*src2,*src3,*src4;
	UYVY *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(UYVY *)src1_0;
		src2=(UYVY *)src2_0;
		dst1=(UYVY *)dst_0;

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=src1[j].u;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].v=src1[j].v;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		}

		dst1=(UYVY *)((uint8_t *)dst_0+dst_pitch);
	}
	else
	{
		src1=(UYVY *)((uint8_t *)src2_0-src_pitch2);
		src2=(UYVY *)src1_0;
		dst1=(UYVY *)dst_0;
	}

	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);
	src3=(UYVY *)((uint8_t *)src1+src_pitch2);
	src4=(UYVY *)((uint8_t *)src2+src_pitch2);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,y3_1,y3_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			y3_1=src3[j].y1;
			y3_2=src3[j].y2;
			dst1[j].u=src2[j].u;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+y3_1)+(y2_1 << 1)+2) >> 2);
			dst1[j].v=src2[j].v;
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+y3_2)+(y2_2 << 1)+2) >> 2);
			dst2[j].u=src3[j].u;
			dst2[j].y1=(uint8_t)((((uint16_t)src4[j].y1+y2_1)+(y3_1 << 1)+2) >> 2);
			dst2[j].v=src3[j].v;
			dst2[j].y2=(uint8_t)((((uint16_t)src4[j].y2+y2_2)+(y3_2 << 1)+2) >> 2);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		src4=(UYVY *)((uint8_t *)src4+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=src2[j].u;
			dst1[j].y1=(uint8_t)((((uint16_t)src1[j].y1+(uint16_t)src3[j].y1)+((uint16_t)src2[j].y1 << 1)+2) >> 2);
			dst1[j].v=src2[j].v;
			dst1[j].y2=(uint8_t)((((uint16_t)src1[j].y2+(uint16_t)src3[j].y2)+((uint16_t)src2[j].y2 << 1)+2) >> 2);
		}
	}

	if (mt_data_inf.bottom)
	{
		src1=(UYVY *)((uint8_t *)src1+src_pitch);
		src2=(UYVY *)((uint8_t *)src2+src_pitch);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch);

		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=src2[j].u;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1+1)>>1);
			dst1[j].v=src2[j].v;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2+1)>>1);
		}
	}
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_Planar(const uint8_t *src1,const uint8_t *src2,
	uint8_t *dst,const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src3,*src4;
	uint8_t *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	dst1=dst;

	for (int32_t j=0; j<w; j++)
		dst1[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);

	src3=src1+src_pitch2;
	src4=src2+src_pitch2;

	dst1=dst+dst_pitch;
	dst2=dst1+dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t a2,a3;

			a2=src2[j];
			a3=src3[j];
			dst1[j]=(uint8_t)((((uint16_t)src1[j]+a3)+(a2 << 1)+2) >> 2);
			dst2[j]=(uint8_t)((((uint16_t)src4[j]+a2)+(a3 << 1)+2) >> 2);
		}
		src1+=src_pitch2;
		src2+=src_pitch2;
		src3+=src_pitch2;
		src4+=src_pitch2;
		dst1+=dst_pitch2;
		dst2+=dst_pitch2;
	}

	for (int32_t j=0; j<w; j++)
		dst1[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
}



void JPSDR_IVTC::Deinterlace_Tri_Blend_Planar_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const uint8_t *src1=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src2=(uint8_t *)mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const uint8_t *src3,*src4;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		dst1=dst;

		for (int32_t j=0; j<w; j++)
			dst1[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);

		dst1=dst+dst_pitch;
	}
	else
	{
		src3=src2-src_pitch2;
		src2=src1;
		src1=src3;
		dst1=dst;
	}

	dst2=dst1+dst_pitch;
	src3=src1+src_pitch2;
	src4=src2+src_pitch2;

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t a2,a3;

			a2=src2[j];
			a3=src3[j];
			dst1[j]=(uint8_t)((((uint16_t)src1[j]+a3)+(a2 << 1)+2) >> 2);
			dst2[j]=(uint8_t)((((uint16_t)src4[j]+a2)+(a3 << 1)+2) >> 2);
		}
		src1+=src_pitch2;
		src2+=src_pitch2;
		src3+=src_pitch2;
		src4+=src_pitch2;
		dst1+=dst_pitch2;
		dst2+=dst_pitch2;
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
			dst1[j]=(uint8_t)((((uint16_t)src1[j]+(uint16_t)src3[j])+((uint16_t)src2[j] << 1)+2) >> 2);
	}

	if (mt_data_inf.bottom)
	{
		src1+=src_pitch;
		src2+=src_pitch;
		dst1+=dst_pitch;

		for (int32_t j=0; j<w; j++)
			dst1[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j]+1)>>1);
	}
}



void JPSDR_IVTC::Deinterlace_Blend_YUYV(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);

	dst1=(YUYV *)dst_0;
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2;

			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+u2) >> 1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+v2) >> 1);
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].u=(uint8_t)((u2+(uint16_t)src3[j].u) >> 1);
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
			dst2[j].v=(uint8_t)((v2+(uint16_t)src3[j].v) >> 1);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
		dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u) >> 1);
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
		dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v) >> 1);
	}
	memcpy(dst2,dst1,w*4);

}



void JPSDR_IVTC::Deinterlace_Blend_YUYV_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const YUYV *src1,*src2,*src3;
	YUYV *dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);

	dst1=(YUYV *)dst_0;
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	if (mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2;

			y2_1=src2[j].y1;
			u2=src2[j].u;
			y2_2=src2[j].y2;
			v2=src2[j].v;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+u2) >> 1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+v2) >> 1);
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].u=(uint8_t)((u2+(uint16_t)src3[j].u) >> 1);
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
			dst2[j].v=(uint8_t)((v2+(uint16_t)src3[j].v) >> 1);
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u) >> 1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v) >> 1);
		}
		memcpy(dst2,dst1,w*4);
	}

}



void JPSDR_IVTC::Deinterlace_Blend_YUYV_Y(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const YUYV *src1,*src2,*src3;
	YUYV *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);

	dst1=(YUYV *)dst_0;
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].u=src1[j].u;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst1[j].v=src1[j].v;
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].u=src2[j].u;
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
			dst2[j].v=src2[j].v;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
		dst1[j].u=src1[j].u;
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
		dst1[j].v=src1[j].v;
	}
	memcpy(dst2,dst1,w<<2);

}


void JPSDR_IVTC::Deinterlace_Blend_YUYV_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const YUYV *src1,*src2,*src3;
	YUYV *dst1,*dst2;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	src3=(YUYV *)((uint8_t *)src1+src_pitch2);

	dst1=(YUYV *)dst_0;
	dst2=(YUYV *)((uint8_t *)dst1+dst_pitch);

	if (mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].u=src1[j].u;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst1[j].v=src1[j].v;
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].u=src2[j].u;
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
			dst2[j].v=src2[j].v;
		}
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
		src3=(YUYV *)((uint8_t *)src3+src_pitch2);
		dst1=(YUYV *)((uint8_t *)dst1+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].y1=(uint8_t)((src1[j].y1+src2[j].y1) >> 1);
			dst1[j].u=src1[j].u;
			dst1[j].y2=(uint8_t)((src1[j].y2+src2[j].y2) >> 1);
			dst1[j].v=src1[j].v;
		}
		memcpy(dst2,dst1,w<<2);
	}
}


void JPSDR_IVTC::Deinterlace_Blend_UYVY(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3;
	UYVY *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	src3=(UYVY *)((uint8_t *)src1+ (src_pitch << 1));

	dst1=(UYVY *)dst_0;
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2;

			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+u2) >> 1);
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+v2) >> 1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst2[j].u=(uint8_t)((u2+(uint16_t)src3[j].u) >> 1);
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].v=(uint8_t)((v2+(uint16_t)src3[j].v) >> 1);
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}


	for (int32_t j=0; j<w; j++)
	{
		dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u) >> 1);
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
		dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v) >> 1);
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
	}
	memcpy(dst2,dst1,w<<2);

}




void JPSDR_IVTC::Deinterlace_Blend_UYVY_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const UYVY *src1,*src2,*src3;
	UYVY *dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	src3=(UYVY *)((uint8_t *)src1+src_pitch2);

	dst1=(UYVY *)dst_0;
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	if (mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2,u2,v2;

			u2=src2[j].u;
			y2_1=src2[j].y1;
			v2=src2[j].v;
			y2_2=src2[j].y2;
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+u2) >> 1);
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+v2) >> 1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst2[j].u=(uint8_t)((u2+(uint16_t)src3[j].u) >> 1);
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].v=(uint8_t)((v2+(uint16_t)src3[j].v) >> 1);
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=(uint8_t)(((uint16_t)src1[j].u+(uint16_t)src2[j].u) >> 1);
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
			dst1[j].v=(uint8_t)(((uint16_t)src1[j].v+(uint16_t)src2[j].v) >> 1);
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
		}
		memcpy(dst2,dst1,w<<2);
	}

}


void JPSDR_IVTC::Deinterlace_Blend_UYVY_Y(const void *src1_0,const void *src2_0,void *dst_0,const int32_t w,const int32_t h,
	ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const UYVY *src1,*src2,*src3;
	UYVY *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	src3=(UYVY *)((uint8_t *)src1+src_pitch2);

	dst1=(UYVY *)dst_0;
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			dst1[j].u=src1[j].u;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].v=src1[j].v;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst2[j].u=src2[j].u;
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].v=src2[j].v;
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	for (int32_t j=0; j<w; j++)
	{
		dst1[j].u=src1[j].u;
		dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
		dst1[j].v=src1[j].v;
		dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
	}
	memcpy(dst2,dst1,w<<2);

}


void JPSDR_IVTC::Deinterlace_Blend_UYVY_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	void *dst_0=mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const UYVY *src1,*src2,*src3;
	UYVY *dst1,*dst2;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	src3=(UYVY *)((uint8_t *)src1+src_pitch2);

	dst1=(UYVY *)dst_0;
	dst2=(UYVY *)((uint8_t *)dst1+dst_pitch);

	if (mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t y2_1,y2_2;

			y2_1=src2[j].y1;
			y2_2=src2[j].y2;
			dst1[j].u=src1[j].u;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+y2_1) >> 1);
			dst1[j].v=src1[j].v;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+y2_2) >> 1);
			dst2[j].u=src2[j].u;
			dst2[j].y1=(uint8_t)((y2_1+(uint16_t)src3[j].y1) >> 1);
			dst2[j].v=src2[j].v;
			dst2[j].y2=(uint8_t)((y2_2+(uint16_t)src3[j].y2) >> 1);
		}
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
		src3=(UYVY *)((uint8_t *)src3+src_pitch2);
		dst1=(UYVY *)((uint8_t *)dst1+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst1[j].u=src1[j].u;
			dst1[j].y1=(uint8_t)(((uint16_t)src1[j].y1+(uint16_t)src2[j].y1) >> 1);
			dst1[j].v=src1[j].v;
			dst1[j].y2=(uint8_t)(((uint16_t)src1[j].y2+(uint16_t)src2[j].y2) >> 1);
		}
		memcpy(dst2,dst1,w*4);
	}
}


void JPSDR_IVTC::Deinterlace_Blend_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch)
{
	const uint8_t *src3;
	uint8_t *dst1,*dst2;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	src3=src1+src_pitch2;

	dst1=dst;
	dst2=dst1+dst_pitch;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			const uint16_t a2=src2[j];

			dst1[j]=(uint8_t)(((uint16_t)src1[j]+a2)>>1);
			dst2[j]=(uint8_t)((a2+(uint16_t)src3[j])>>1);
		}
		src1+=src_pitch2;
		src2+=src_pitch2;
		src3+=src_pitch2;
		dst1+=dst_pitch2;
		dst2+=dst_pitch2;
	}

	for (int32_t j=0; j<w; j++)
		dst1[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j])>>1);

	memcpy(dst2,dst1,w);
}



void JPSDR_IVTC::Deinterlace_Blend_Planar_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const uint8_t *src1=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src2=(uint8_t *)mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;

	const ptrdiff_t src_pitch2=src_pitch << 1;
	const ptrdiff_t dst_pitch2=dst_pitch << 1;

	const uint8_t *src3;
	uint8_t *dst1,*dst2;

	src3=src1+src_pitch2;

	dst1=dst;
	dst2=dst1+dst_pitch;

	if (mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			const uint16_t a2=src2[j];

			dst1[j]=(uint8_t)(((uint16_t)src1[j]+a2)>>1);
			dst2[j]=(uint8_t)((a2+(uint16_t)src3[j])>>1);
		}
		src1+=src_pitch2;
		src2+=src_pitch2;
		src3+=src_pitch2;
		dst1+=dst_pitch2;
		dst2+=dst_pitch2;
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
			dst1[j]=(uint8_t)(((uint16_t)src1[j]+(uint16_t)src2[j])>>1);

		memcpy(dst2,dst1,w);
	}
}


void JPSDR_IVTC::Deinterlace_RGB32(const void *src1,const void *src2,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data)
{
	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src_pitch1=src_pitch;
			MT_Data[i].dst1=(void *)((uint8_t *)dst+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
			MT_Data[i].dst_pitch1=dst_pitch;
			MT_Data[i].src_Y_w_32=w;
			MT_Data[i].dst_Y_w_32=w;
		}
		if (tri_linear)
		{
			if (AVX_Enable && align_data) f_proc=58;
			else
			{
				if (SSE2_Enable && align_data) f_proc=12;
				else
				{
					if (MMX_Enable) f_proc=13;
					else f_proc=11;
				}
			}
		}
		else
		{
			if (AVX_Enable && align_data) f_proc=59;
			else
			{
				if (SSE2_Enable && align_data) f_proc=22;
				else
				{
					if (Integer_SSE_Enable)
					{
						if ((w&0x01)!=0) f_proc=20;
						else f_proc=21;
					}
					else f_proc=19;
				}
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		h--;
		if (tri_linear)
		{
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Tri_Blend_AVX(src1,src2,dst,(w+3)>>2,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,(w+3)>>2,h,src_pitch<<1,dst_pitch);
				}
				else
					if (MMX_Enable)
						JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,w,h,src_pitch<<1,dst_pitch);
					else
						Deinterlace_Tri_Blend_RGB32(src1,src2,dst,w,h,src_pitch,dst_pitch);
			}
		}
		else
		{
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Blend_AVX(src1,src2,dst,(w+3)>>2,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,(w+3)>>2,h,src_pitch<<1,dst_pitch);
				}
				else
				{
					if (Integer_SSE_Enable)
					{
						if ((w&0x01)!=0)
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
	}
}



void JPSDR_IVTC::Deinterlace_YUYV(const void *src1,const void *src2,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data,bool Y_only)
{

	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,true,false,true,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src_pitch1=src_pitch;
			MT_Data[i].dst1=(void *)((uint8_t *)dst+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
			MT_Data[i].dst_pitch1=dst_pitch;
			MT_Data[i].src_Y_w_32=(w+1)>>1;
			MT_Data[i].dst_Y_w_32=(w+1)>>1;
		}

		if (Y_only)
		{
			if (tri_linear) f_proc=15;
			else f_proc=24;
		}
		else
		{
			if (tri_linear)
			{
				if (AVX_Enable && align_data) f_proc=58;
				else
				{
					if (SSE2_Enable && align_data) f_proc=12;
					else
					{
						if (MMX_Enable) f_proc=13;
						else f_proc=14;
					}
				}
			}
			else
			{
				if (AVX_Enable && align_data) f_proc=59;
				else
				{
					if (SSE2_Enable && align_data) f_proc=22;
					else
					{
						if (Integer_SSE_Enable)
						{
							if ((w&0x03)!=0) f_proc=20;
							else f_proc=21;
						}
						else f_proc=23;
					}
				}
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
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
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Tri_Blend_AVX(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
				}
				else
					if (MMX_Enable)
						JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,(w+1)>>1,h,src_pitch<<1,dst_pitch);
					else
						Deinterlace_Tri_Blend_YUYV(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
			}
		}
		else
		{
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Blend_AVX(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
				}
				else
				{
					if (Integer_SSE_Enable)
					{
						if ((w&0x03)!=0)
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
	}
}



void JPSDR_IVTC::Deinterlace_UYVY(const void *src1,const void *src2,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data, bool Y_only)
{
	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,true,false,true,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src_pitch1=src_pitch;
			MT_Data[i].dst1=(void *)((uint8_t *)dst+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
			MT_Data[i].dst_pitch1=dst_pitch;
			MT_Data[i].src_Y_w_32=(w+1)>>1;
			MT_Data[i].dst_Y_w_32=(w+1)>>1;
		}

		if (Y_only)
		{
			if (tri_linear) f_proc=17;
			else f_proc=26;
		}
		else
		{
			if (tri_linear)
			{
				if (AVX_Enable && align_data) f_proc=58;
				else
				{
					if (SSE2_Enable && align_data) f_proc=12;
					else
					{
						if (MMX_Enable) f_proc=13;
						else f_proc=16;
					}
				}
			}
			else
			{
				if (AVX_Enable && align_data) f_proc=59;
				else
				{
					if (SSE2_Enable && align_data) f_proc=22;
					else
					{
						if (Integer_SSE_Enable)
						{
							if ((w&0x03)!=0) f_proc=20;
							else f_proc=21;
						}
						else f_proc=25;
					}
				}
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
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
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Tri_Blend_AVX(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
				}
				else
					if (MMX_Enable)
						JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,(w+1)>>1,h,src_pitch<<1,dst_pitch);
					else
						Deinterlace_Tri_Blend_UYVY(src1,src2,dst,(w+1)>>1,h,src_pitch,dst_pitch);
			}
		}
		else
		{
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Blend_AVX(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,(w+7)>>3,h,src_pitch<<1,dst_pitch);
				}
				else
				{
					if (Integer_SSE_Enable)
					{
						if ((w&0x03)!=0)
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
	}
}



void JPSDR_IVTC::Deinterlace_Planar(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
	int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,bool tri_linear,bool align_data)
{
	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src_pitch1=src_pitch;
			MT_Data[i].dst1=(void *)((uint8_t *)dst+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
			MT_Data[i].dst_pitch1=dst_pitch;
			MT_Data[i].src_Y_w_32=(w+3)>>2;
			MT_Data[i].dst_Y_w_32=(w+3)>>2;
		}
		if (tri_linear)
		{
			if (AVX_Enable && align_data) f_proc=58;
			else
			{
				if (SSE2_Enable && align_data) f_proc=12;
				else
				{
					if ((MMX_Enable) && ((w&0x03)==0)) f_proc=13;
					else f_proc=18;
				}
			}
		}
		else
		{
			if (AVX_Enable && align_data) f_proc=59;
			else
			{
				if (SSE2_Enable && align_data) f_proc=22;
				else
				{
					if ((Integer_SSE_Enable) && ((w&0x03)==0))
					{
						if ((w&0x07)!=0) f_proc=20;
						else f_proc=21;
					}
					else f_proc=27;
				}
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		h--;
		if (tri_linear)
		{
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Tri_Blend_AVX(src1,src2,dst,(w+15)>>4,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Tri_Blend_SSE(src1,src2,dst,(w+15)>>4,h,src_pitch<<1,dst_pitch);
				}
				else
				{
					if ((MMX_Enable) && ((w&0x03)==0))
						JPSDR_IVTC_Deinterlace_Tri_Blend_MMX(src1,src2,dst,w>>2,h,src_pitch<<1,dst_pitch);
					else
						Deinterlace_Tri_Blend_Planar(src1,src2,dst,w,h,src_pitch,dst_pitch);
				}
			}
		}
		else
		{
			if (AVX_Enable && align_data)
			{
				JPSDR_IVTC_Deinterlace_Blend_AVX(src1,src2,dst,(w+15)>>4,h,src_pitch<<1,dst_pitch);
			}
			else
			{
				if (SSE2_Enable && align_data)
				{
					JPSDR_IVTC_Deinterlace_Blend_SSE_3(src1,src2,dst,(w+15)>>4,h,src_pitch<<1,dst_pitch);
				}
				else
				{
					if ((Integer_SSE_Enable) && ((w&0x03)==0))
					{
						if ((w&0x07)!=0)
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
	}
}



void JPSDR_IVTC::Motion_Map_RGB32(const void *src1_0,const void *src2_0,void *buffer_0,uint8_t *dst,
		const int32_t w,const int32_t h,const ptrdiff_t pitch,const ptrdiff_t pitch_buffer,const int32_t w_map,const uint8_t thrs)
{
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer,*buffer2;
	uint8_t *dst1,*dst2;
	const uint16_t thr=thrs;
	const int32_t h_1=h-1;

	const ptrdiff_t pitch2=pitch << 1,pitch_buffer2=pitch_buffer << 1;
	const int32_t w_map_2=w_map << 1 ;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	buffer=(RGB32BMP *)buffer_0;
	dst1=dst;

	memset(dst1,0,w);
	dst1+=w_map;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);
	buffer2=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer);

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			buffer[j].b=(uint8_t)(b&0xFC);
			buffer[j].g=(uint8_t)(g&0xFC);
			buffer[j].r=(uint8_t)(r&0xFC);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;

			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			buffer2[j].b=(uint8_t)(b&0xFC);
			buffer2[j].g=(uint8_t)(g&0xFC);
			buffer2[j].r=(uint8_t)(r&0xFC);
			if ( (!((g3>g1)^(g3>g2)) && ((g>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((r>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((b>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+pitch_buffer2);
	}

	for (int32_t i=0; i<w; i++)
	{
		int16_t r,r1,r2,g,g1,g2,b,b1,b2;

		b1=src1[i].b;
		g1=src1[i].g;
		r1=src1[i].r;
		b2=src2[i].b;
		g2=src2[i].g;
		r2=src2[i].r;
		b=(int16_t)abs(b2-b1)&0xFC;
		g=(int16_t)abs(g2-g1)&0xFC;
		r=(int16_t)abs(r2-r1)&0xFC;
		buffer[i].b=(uint8_t)b;
		buffer[i].g=(uint8_t)g;
		buffer[i].r=(uint8_t)r;
	}
	memcpy(buffer2,buffer,w<<2);
	memset(dst1,0,w);
}



void JPSDR_IVTC::Motion_Map_SSE_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1,*src2;
	void *buffer;

	if (mt_data_inf.top)
	{
		src1=mt_data_inf.src1;
		src2=mt_data_inf.src2;
		buffer=mt_data_inf.dst2;
	}
	else
	{
		src1=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2=(void *)mt_data_inf.src1;
		buffer=(void *)((uint8_t *)mt_data_inf.dst2-mt_data_inf.dst_pitch2);
	}
	if (mt_data_inf.top)
		JPSDR_IVTC_Motion_Map_SSE_RGB32_a(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
			mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Motion_Map_SSE_RGB32_c(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
				mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
		else
			JPSDR_IVTC_Motion_Map_SSE_RGB32_b(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
				mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
}


void JPSDR_IVTC::Motion_Map_SSE2_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1,*src2;
	void *buffer;

	if (mt_data_inf.top)
	{
		src1=mt_data_inf.src1;
		src2=mt_data_inf.src2;
		buffer=mt_data_inf.dst2;
	}
	else
	{
		src1=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2=(void *)mt_data_inf.src1;
		buffer=(void *)((uint8_t *)mt_data_inf.dst2-mt_data_inf.dst_pitch2);
	}
	if (mt_data_inf.top)
		JPSDR_IVTC_Motion_Map_SSE2_RGB32_a(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w>>1,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
			mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Motion_Map_SSE2_RGB32_c(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w>>1,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
				mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
		else
			JPSDR_IVTC_Motion_Map_SSE2_RGB32_b(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w>>1,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
				mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
}


void JPSDR_IVTC::Motion_Map_AVX_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1,*src2;
	void *buffer;

	if (mt_data_inf.top)
	{
		src1=mt_data_inf.src1;
		src2=mt_data_inf.src2;
		buffer=mt_data_inf.dst2;
	}
	else
	{
		src1=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2=(void *)mt_data_inf.src1;
		buffer=(void *)((uint8_t *)mt_data_inf.dst2-mt_data_inf.dst_pitch2);
	}
	if (mt_data_inf.top)
		JPSDR_IVTC_Motion_Map_AVX_RGB32_a(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w>>1,
			mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
			mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Motion_Map_AVX_RGB32_c(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w>>1,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
				mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
		else
			JPSDR_IVTC_Motion_Map_AVX_RGB32_b(src1,src2,buffer,(uint8_t *)mt_data_inf.dst1,mt_data_inf.src_Y_w>>1,
				mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
				mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
}


void JPSDR_IVTC::Motion_Map_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	void *buffer_0=mt_data_inf.dst2;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t buffer_pitch=mt_data_inf.dst_pitch2;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1,buffer_pitch2=buffer_pitch << 1;
	const int32_t w_map_2=w_map << 1 ;

	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer,*buffer2;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(RGB32BMP *)src1_0;
		src2=(RGB32BMP *)src2_0;
		buffer=(RGB32BMP *)buffer_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;
	}
	else
	{
		src1=(RGB32BMP *)((uint8_t *)src2_0-pitch2);
		src2=(RGB32BMP *)src1_0;
		buffer=(RGB32BMP *)((uint8_t *)buffer_0-buffer_pitch);
		dst1=dst;
	}

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);
	buffer2=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			buffer[j].b=(uint8_t)(b&0xFC);
			buffer[j].g=(uint8_t)(g&0xFC);
			buffer[j].r=(uint8_t)(r&0xFC);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;

			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			buffer2[j].b=(uint8_t)(b&0xFC);
			buffer2[j].g=(uint8_t)(g&0xFC);
			buffer2[j].r=(uint8_t)(r&0xFC);
			if ( (!((g3>g1)^(g3>g2)) && ((g>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((r>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((b>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+buffer_pitch2);
	}

	if (mt_data_inf.top)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			buffer[j].b=(uint8_t)(b&0xFC);
			buffer[j].g=(uint8_t)(g&0xFC);
			buffer[j].r=(uint8_t)(r&0xFC);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			buffer[j].b=(uint8_t)(b&0xFC);
			buffer[j].g=(uint8_t)(g&0xFC);
			buffer[j].r=(uint8_t)(r&0xFC);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;

			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			buffer2[j].b=(uint8_t)(b&0xFC);
			buffer2[j].g=(uint8_t)(g&0xFC);
			buffer2[j].r=(uint8_t)(r&0xFC);
		}
		buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
		dst1+=w_map;
		memset(dst1,0,w);
		memcpy(buffer,buffer2,w<<2);
	}
}


void JPSDR_IVTC::Motion_Map_RGB32_2_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;

	const RGB32BMP *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(RGB32BMP *)src1_0;
		src2=(RGB32BMP *)src2_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;
	}
	else
	{
		src1=(RGB32BMP *)((uint8_t *)src2_0-pitch2);
		src2=(RGB32BMP *)src1_0;
		dst1=dst;
	}

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;

			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			if ( (!((g3>g1)^(g3>g2)) && ((g>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((r>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((b>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		dst1+=w_map;
		memset(dst1,0,w);
	}
}



void JPSDR_IVTC::Motion_Map_RGB32(const void *src1_0,const void *src2_0,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs)
{
	const RGB32BMP *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;
	const int32_t h_1=h-1;
	const int16_t thr=thrs;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	dst1=dst;

	memset(dst1,0,w);
	dst1+=w_map;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,r3,g,g1,g2,g3,b,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			b=(int16_t)abs(b2-b1);
			g=(int16_t)abs(g2-g1);
			r=(int16_t)abs(r2-r1);
			if ( (!((g2>g1)^(g2>g3)) && ((g>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((r>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((b>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;

			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			b=(int16_t)abs(b3-b2);
			g=(int16_t)abs(g3-g2);
			r=(int16_t)abs(r3-r2);
			if ( (!((g3>g1)^(g3>g2)) && ((g>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((r>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((b>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	memset(dst1,0,w);
}



void JPSDR_IVTC::Motion_Map_YUYV_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;
	const int32_t w_2=w >> 1 ;

	const YUYV *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(YUYV *)src1_0;
		src2=(YUYV *)src2_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;
	}
	else
	{
		src1=(YUYV *)((uint8_t *)src2_0-pitch2);
		src2=(YUYV *)src1_0;
		dst1=dst;
	}

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	dst2=dst1+w_map;
	src3=(YUYV *)((uint8_t *)src1+pitch2);
	src4=(YUYV *)((uint8_t *)src2+pitch2);

	for (int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w_2; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j].y1;
			y2=src2[j].y1;
			y3=src3[j].y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y1;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;

			y1=src1[j].y2;
			y2=src2[j].y2;
			y3=src3[j].y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y2;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;
		}
		src1=(YUYV *)((uint8_t *)src1+pitch2);
		src2=(YUYV *)((uint8_t *)src2+pitch2);
		src3=(YUYV *)((uint8_t *)src3+pitch2);
		src4=(YUYV *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w_2; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j].y1;
			y2=src2[j].y1;
			y3=src3[j].y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2++]=1;
			else dst1[j2++]=0;

			y1=src1[j].y2;
			y2=src2[j].y2;
			y3=src3[j].y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2++]=1;
			else dst1[j2++]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		dst1+=w_map;
		memset(dst1,0,w);
	}
}


void JPSDR_IVTC::Motion_Map_YUYV_Y(const void *src1_0,const void *src2_0,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs)
{
	const YUYV *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	const int32_t h_1=h-1;
	const int16_t thr=thrs;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;
	const int32_t w_2=w >> 1 ;

	src1=(YUYV *)src1_0;
	src2=(YUYV *)src2_0;
	dst1=dst;

	memset(dst1,0,w);
	dst1+=w_map;

	dst2=dst1+w_map;
	src3=(YUYV *)((uint8_t *)src1+pitch2);
	src4=(YUYV *)((uint8_t *)src2+pitch2);

	for (int32_t i=0; i<h_1; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w_2; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j].y1;
			y2=src2[j].y1;
			y3=src3[j].y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y1;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;

			y1=src1[j].y2;
			y2=src2[j].y2;
			y3=src3[j].y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y2;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;
		}
		src1=(YUYV *)((uint8_t *)src1+pitch2);
		src2=(YUYV *)((uint8_t *)src2+pitch2);
		src3=(YUYV *)((uint8_t *)src3+pitch2);
		src4=(YUYV *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	memset(dst1,0,w);
}


void JPSDR_IVTC::Motion_Map_UYVY_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;
	const int32_t w_2=w >> 1 ;

	const UYVY *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(UYVY *)src1_0;
		src2=(UYVY *)src2_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;
	}
	else
	{
		src1=(UYVY *)((uint8_t *)src2_0-pitch2);
		src2=(UYVY *)src1_0;
		dst1=dst;
	}

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	dst2=dst1+w_map;
	src3=(UYVY *)((uint8_t *)src1+pitch2);
	src4=(UYVY *)((uint8_t *)src2+pitch2);

	for (int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w_2; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j].y1;
			y2=src2[j].y1;
			y3=src3[j].y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y1;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;

			y1=src1[j].y2;
			y2=src2[j].y2;
			y3=src3[j].y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y2;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;
		}
		src1=(UYVY *)((uint8_t *)src1+pitch2);
		src2=(UYVY *)((uint8_t *)src2+pitch2);
		src3=(UYVY *)((uint8_t *)src3+pitch2);
		src4=(UYVY *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w_2; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j].y1;
			y2=src2[j].y1;
			y3=src3[j].y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2++]=1;
			else dst1[j2++]=0;

			y1=src1[j].y2;
			y2=src2[j].y2;
			y3=src3[j].y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2++]=1;
			else dst1[j2++]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		dst1+=w_map;
		memset(dst1,0,w);
	}
}



void JPSDR_IVTC::Motion_Map_UYVY_Y(const void *src1_0,const void *src2_0,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs)
{
	const UYVY *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;
	const int32_t h_1=h-1;
	const int16_t thr=thrs;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;
	const int32_t w_2=w >> 1 ;

	src1=(UYVY *)src1_0;
	src2=(UYVY *)src2_0;
	dst1=dst;

	memset(dst1,0,w);
	dst1+=w_map;

	dst2=dst1+w_map;
	src3=(UYVY *)((uint8_t *)src1+pitch2);
	src4=(UYVY *)((uint8_t *)src2+pitch2);

	for (int32_t i=0; i<h_1; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w_2; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j].y1;
			y2=src2[j].y1;
			y3=src3[j].y1;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y1;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;

			y1=src1[j].y2;
			y2=src2[j].y2;
			y3=src3[j].y2;
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j2]=1;
			else dst1[j2]=0;

			y1=src4[j].y2;
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j2++]=1;
			else dst2[j2++]=0;
		}
		src1=(UYVY *)((uint8_t *)src1+pitch2);
		src2=(UYVY *)((uint8_t *)src2+pitch2);
		src3=(UYVY *)((uint8_t *)src3+pitch2);
		src4=(UYVY *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	memset(dst1,0,w);
}


void JPSDR_IVTC::Motion_Map_Planar420_Y_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const uint8_t *src1_0=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src2_0=(uint8_t *)mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;

	const uint8_t *src1,*src2,*src3,*src4;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=src1_0;
		src2=src2_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;
	}
	else
	{
		src1=src2_0-pitch2;
		src2=src1_0;
		dst1=dst;
	}

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	dst2=dst1+w_map;
	src3=src1+pitch2;
	src4=src2+pitch2;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j];
			y2=src2[j];
			y3=src3[j];
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j]=1;
			else dst1[j]=0;

			y1=src4[j];
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j]=1;
			else dst2[j]=0;
		}
		src1+=pitch2;
		src2+=pitch2;
		src3+=pitch2;
		src4+=pitch2;
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j];
			y2=src2[j];
			y3=src3[j];
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j]=1;
			else dst1[j]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		dst1+=w_map;
		memset(dst1,0,w);
	}
}



void JPSDR_IVTC::Motion_Map_Planar420_Y(const uint8_t *src1,const uint8_t *src2,uint8_t *dst,
		const int32_t w,const int32_t h,ptrdiff_t pitch,const int32_t w_map,const uint8_t thrs)
{
	const uint8_t *src3,*src4;
	uint8_t *dst1,*dst2;
	const int32_t h_1=h-1;
	const int16_t thr=thrs;

	const ptrdiff_t pitch2=pitch << 1;
	const int32_t w_map_2=w_map << 1 ;

	dst1=dst;

	memset(dst1,0,w);
	dst1+=w_map;

	dst2=dst1+w_map;
	src3=src1+pitch2;
	src4=src2+pitch2;

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t y,y1,y2,y3;

			y1=src1[j];
			y2=src2[j];
			y3=src3[j];
			y=(int16_t)abs(y2-y1);
			if (!((y2>y1)^(y2>y3)) && ((y>thr) && (abs(y2-y3)>thr))) dst1[j]=1;
			else dst1[j]=0;

			y1=src4[j];
			y=(int16_t)abs(y3-y2);
			if (!((y3>y1)^(y3>y2)) && ((y>thr) && (abs(y3-y1)>thr))) dst2[j]=1;
			else dst2[j]=0;
		}
		src1+=pitch2;
		src2+=pitch2;
		src3+=pitch2;
		src4+=pitch2;
		dst1+=w_map_2;
		dst2+=w_map_2;
	}

	memset(dst1,0,w);
}



void JPSDR_IVTC::Smart_Deinterlace_Motion_Map_SSE_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1,*src2;

	if (mt_data_inf.top)
	{
		src1=mt_data_inf.src1;
		src2=mt_data_inf.src2;
	}
	else
	{
		src1=(void *)((uint8_t*)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2=mt_data_inf.src1;
	}

	if (mt_data_inf.top)
	{
		JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a(src1,src2,mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst1,
			mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,
			mt_data_inf.src_modulo1,mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c(src1,src2,mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst1,
				mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,
				mt_data_inf.src_modulo1,mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
		else
			JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b(src1,src2,mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst1,
				mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,
				mt_data_inf.src_modulo1,mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
}



void JPSDR_IVTC::Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1,*src2;

	if (mt_data_inf.top)
	{
		src1=mt_data_inf.src1;
		src2=mt_data_inf.src2;
	}
	else
	{
		src1=(void *)((uint8_t *)mt_data_inf.src2-(mt_data_inf.src_pitch1 << 1));
		src2=mt_data_inf.src1;
	}

	if (mt_data_inf.top)
	{
		JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a(src1,src2,mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst1,
			mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,
			mt_data_inf.src_modulo1,mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
	else
	{
		if (mt_data_inf.bottom)
			JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c(src1,src2,mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst1,
				mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min-1,mt_data_inf.src_pitch1,
				mt_data_inf.src_modulo1,mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
		else
			JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b(src1,src2,mt_data_inf.dst2,(uint8_t *)mt_data_inf.dst1,
				mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,
				mt_data_inf.src_modulo1,mt_data_inf.dst_pitch2,mt_data_inf.dst_modulo2,mt_data_inf.data_u32_1,mt_data_inf.data_i32);
	}
}



void JPSDR_IVTC::Smart_Deinterlace_Motion_Map_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	void *buffer_0=mt_data_inf.dst2;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t buffer_pitch=mt_data_inf.dst_pitch2;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1,buffer_pitch2=buffer_pitch << 1;
	const int32_t w_map_2=w_map << 1 ;

	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer,*buffer2;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(RGB32BMP *)src1_0;
		src2=(RGB32BMP *)src2_0;
		buffer=(RGB32BMP *)buffer_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;

		for (int32_t i=0; i<w; i++)
		{
			buffer[i].b=(uint8_t)(((uint16_t)src1[i].b+(uint16_t)src2[i].b)>>1);
			buffer[i].g=(uint8_t)(((uint16_t)src1[i].g+(uint16_t)src2[i].g)>>1);
			buffer[i].r=(uint8_t)(((uint16_t)src1[i].r+(uint16_t)src2[i].r)>>1);
		}
		buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch);
	}
	else
	{
		src1=(RGB32BMP *)((uint8_t *)src2_0-pitch2);
		src2=(RGB32BMP *)src1_0;
		dst1=dst;
		buffer=(RGB32BMP *)buffer_0;
	}

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);
	buffer2=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			buffer[j].b=(uint8_t)((b3+b2)>>1);
			buffer[j].g=(uint8_t)((g3+g2)>>1);
			buffer[j].r=(uint8_t)((r3+r2)>>1);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			buffer2[j].b=(uint8_t)((b1+b3)>>1);
			buffer2[j].g=(uint8_t)((g1+g3)>>1);
			buffer2[j].r=(uint8_t)((r1+r3)>>1);
			if ( (!((g3>g1)^(g3>g2)) && ((abs(g3-g2)>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((abs(r3-r2)>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((abs(b3-b2)>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+buffer_pitch2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			buffer[j].b=(uint8_t)((b3+b2)>>1);
			buffer[j].g=(uint8_t)((g3+g2)>>1);
			buffer[j].r=(uint8_t)((r3+r2)>>1);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		dst1+=w_map;
		memset(dst1,0,w);
		memcpy(buffer2,buffer,w<<2);
	}

}



void JPSDR_IVTC::Smart_Deinterlace_Motion_Map_RGB32(const void *src1_0,const void *src2_0,void *buffer_0,
		uint8_t *dst,const int32_t w,const int32_t h,const ptrdiff_t pitch,const ptrdiff_t pitch_buffer,const int32_t w_map,
		const uint8_t thrs)
{
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer,*buffer2;
	uint8_t *dst1,*dst2;
	const int32_t h_1=h-1;
	const int16_t thr=thrs;

	const ptrdiff_t pitch2=pitch << 1,pitch_buffer2=pitch_buffer << 1;
	const int32_t w_map_2=w_map << 1 ;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	buffer=(RGB32BMP *)buffer_0;

	dst1=dst;

	for (int32_t i=0; i<w; i++)
	{
		buffer[i].b=(uint8_t)(((uint16_t)src1[i].b+(uint16_t)src2[i].b)>>1);
		buffer[i].g=(uint8_t)(((uint16_t)src1[i].g+(uint16_t)src2[i].g)>>1);
		buffer[i].r=(uint8_t)(((uint16_t)src1[i].r+(uint16_t)src2[i].r)>>1);
	}
	memset(dst1,0,w);
	dst1+=w_map;
	buffer=(RGB32BMP *)((uint8_t*)buffer+pitch_buffer);

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);
	buffer2=(RGB32BMP *)((uint8_t*)buffer+pitch_buffer);

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			buffer[j].b=(uint8_t)((b3+b2)>>1);
			buffer[j].g=(uint8_t)((g3+g2)>>1);
			buffer[j].r=(uint8_t)((r3+r2)>>1);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			buffer2[j].b=(uint8_t)((b1+b3)>>1);
			buffer2[j].g=(uint8_t)((g1+g3)>>1);
			buffer2[j].r=(uint8_t)((r1+r3)>>1);
			if ( (!((g3>g1)^(g3>g2)) && ((abs(g3-g2)>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((abs(r3-r2)>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((abs(b3-b2)>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t*)buffer+pitch_buffer2);
		buffer2=(RGB32BMP *)((uint8_t*)buffer2+pitch_buffer2);
	}

	memset(dst1,0,w);
	buffer2=(RGB32BMP *)((uint8_t*)buffer-pitch_buffer);
	memcpy(buffer,buffer2,w<<2);
}



void JPSDR_IVTC::Smart_Deinterlace_Tri_Motion_Map_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const void *src1_0=mt_data_inf.src1;
	const void *src2_0=mt_data_inf.src2;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	void *buffer_0=mt_data_inf.dst2;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t pitch_buffer=mt_data_inf.dst_pitch2;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const uint16_t thr=mt_data_inf.data_u8;
	const int32_t w_map=mt_data_inf.data_i32;

	const ptrdiff_t pitch2=pitch << 1,pitch_buffer2=pitch_buffer << 1;
	const int32_t w_map_2=w_map << 1 ;

	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer,*buffer2;
	uint8_t *dst1,*dst2;

	if (mt_data_inf.top)
	{
		src1=(RGB32BMP *)src1_0;
		src2=(RGB32BMP *)src2_0;
		buffer=(RGB32BMP *)buffer_0;
		dst1=dst;

		memset(dst1,0,w);
		dst1+=w_map;

		for (int32_t i=0; i<w; i++)
		{
			buffer[i].b=(uint8_t)(((uint16_t)src1[i].b+(uint16_t)src2[i].b)>>1);
			buffer[i].g=(uint8_t)(((uint16_t)src1[i].g+(uint16_t)src2[i].g)>>1);
			buffer[i].r=(uint8_t)(((uint16_t)src1[i].r+(uint16_t)src2[i].r)>>1);
		}
		buffer=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer);
	}
	else
	{
		src1=(RGB32BMP *)((uint8_t *)src2_0-pitch2);
		src2=(RGB32BMP *)src1_0;
		dst1=dst;
		buffer=(RGB32BMP *)buffer_0;
	}

	if (mt_data_inf.top || mt_data_inf.bottom) h--;

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);
	buffer2=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			buffer[j].b=(uint8_t)(((b1+b3)+(b2 << 1)+2)>>2);
			buffer[j].g=(uint8_t)(((g1+g3)+(g2 << 1)+2)>>2);
			buffer[j].r=(uint8_t)(((r1+r3)+(r2 << 1)+2)>>2);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			buffer2[j].b=(uint8_t)(((b1+b2)+(b3 << 1)+2)>>2);
			buffer2[j].g=(uint8_t)(((g1+g2)+(g3 << 1)+2)>>2);
			buffer2[j].r=(uint8_t)(((r1+r2)+(r3 << 1)+2)>>2);
			if ( (!((g3>g1)^(g3>g2)) && ((abs(g3-g2)>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((abs(r3-r2)>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((abs(b3-b2)>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+pitch_buffer2);
	}

	if (mt_data_inf.top || mt_data_inf.bottom)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			buffer[j].b=(uint8_t)(((b1+b3)+(b2 << 1)+2)>>2);
			buffer[j].g=(uint8_t)(((g1+g3)+(g2 << 1)+2)>>2);
			buffer[j].r=(uint8_t)(((r1+r3)+(r2 << 1)+2)>>2);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
		}
	}

	if (mt_data_inf.bottom)
	{
		for (int32_t i=0; i<w; i++)
		{
			buffer2[i].b=(uint8_t)(((uint16_t)src2[i].b+(uint16_t)src3[i].b)>>1);
			buffer2[i].g=(uint8_t)(((uint16_t)src2[i].g+(uint16_t)src3[i].g)>>1);
			buffer2[i].r=(uint8_t)(((uint16_t)src2[i].r+(uint16_t)src3[i].r)>>1);
		}
		dst1+=w_map;
		memset(dst1,0,w);
	}

}



void JPSDR_IVTC::Smart_Deinterlace_Tri_Motion_Map_RGB32(const void *src1_0,const void *src2_0,void *buffer_0,
		uint8_t *dst,const int32_t w,const int32_t h,const ptrdiff_t pitch,const ptrdiff_t pitch_buffer,const int32_t w_map,
		const uint8_t thrs)
{
	const RGB32BMP *src1,*src2,*src3,*src4;
	RGB32BMP *buffer,*buffer2;
	uint8_t *dst1,*dst2;
	const int32_t delta_map=w_map-w,h_1=h-1;
	const int16_t thr=thrs;

	const ptrdiff_t pitch2=pitch << 1,pitch_buffer2=pitch_buffer << 1;
	const int32_t w_map_2=w_map << 1 ;

	src1=(RGB32BMP *)src1_0;
	src2=(RGB32BMP *)src2_0;
	buffer=(RGB32BMP *)buffer_0;

	dst1=dst;

	for (int32_t i=0; i<w; i++)
	{
		buffer[i].b=(uint8_t)(((uint16_t)src1[i].b+(uint16_t)src2[i].b)>>1);
		buffer[i].g=(uint8_t)(((uint16_t)src1[i].g+(uint16_t)src2[i].g)>>1);
		buffer[i].r=(uint8_t)(((uint16_t)src1[i].r+(uint16_t)src2[i].r)>>1);
	}
	memset(dst1,0,w);
	dst1+=w_map;
	buffer=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer);

	dst2=dst1+w_map;
	src3=(RGB32BMP *)((uint8_t *)src1+pitch2);
	src4=(RGB32BMP *)((uint8_t *)src2+pitch2);
	buffer2=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer);

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;

			b1=src1[j].b;
			g1=src1[j].g;
			r1=src1[j].r;
			b2=src2[j].b;
			g2=src2[j].g;
			r2=src2[j].r;
			b3=src3[j].b;
			g3=src3[j].g;
			r3=src3[j].r;
			buffer[j].b=(uint8_t)(((b1+b3)+(b2 << 1)+2)>>2);
			buffer[j].g=(uint8_t)(((g1+g3)+(g2 << 1)+2)>>2);
			buffer[j].r=(uint8_t)(((r1+r3)+(r2 << 1)+2)>>2);
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) dst1[j]=1;
			else dst1[j]=0;
			b1=src4[j].b;
			g1=src4[j].g;
			r1=src4[j].r;
			buffer2[j].b=(uint8_t)(((b1+b2)+(b3 << 1)+2)>>2);
			buffer2[j].g=(uint8_t)(((g1+g2)+(g3 << 1)+2)>>2);
			buffer2[j].r=(uint8_t)(((r1+r2)+(r3 << 1)+2)>>2);
			if ( (!((g3>g1)^(g3>g2)) && ((abs(g3-g2)>thr) && (abs(g3-g1)>thr))) 
				|| (!((r3>r1)^(r3>r2)) && ((abs(r3-r2)>thr) && (abs(r3-r1)>thr)))
				|| (!((b3>b1)^(b3>b2)) && ((abs(b3-b2)>thr) && (abs(b3-b1)>thr))) ) dst2[j]=1;
			else dst2[j]=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+pitch2);
		src3=(RGB32BMP *)((uint8_t *)src3+pitch2);
		src4=(RGB32BMP *)((uint8_t *)src4+pitch2);
		dst1+=w_map_2;
		dst2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t *)buffer+pitch_buffer2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+pitch_buffer2);
	}

	for (int32_t i=0; i<w; i++)
	{
		buffer[i].b=(uint8_t)(((uint16_t)src1[i].b+(uint16_t)src2[i].b)>>1);
		buffer[i].g=(uint8_t)(((uint16_t)src1[i].g+(uint16_t)src2[i].g)>>1);
		buffer[i].r=(uint8_t)(((uint16_t)src1[i].r+(uint16_t)src2[i].r)>>1);
	}
	memset(dst1,0,w);
}


void JPSDR_IVTC::Motion_Map_Filter(uint8_t *map,uint8_t *buffer,const int32_t w,
	const int32_t h,const int32_t w_map)
{
	uint8_t *tab;
	uint8_t *dst,*src;
	const int32_t h_4=h-4,w_2=w-2,w_map_4=w_map >> 2;

	tab=map;
	dst=buffer;

	memset(dst,0,w_map<<1);
	memset(dst+((h-2)*w_map),0,w_map<<1);

	dst+=(w_map<<1);
	src=map+(w_map<<1);

	for (int32_t i=0; i<h_4; i++)
	{
		for (int32_t j=0; j<2; j++)
			dst[j]=0;

		for (int32_t j=2; j<w_2; j++)
		{
			if (src[j]==1)
			{
				uint8_t s=0;
				const uint8_t *tab0=tab+j-2;

				for (int32_t k=0; k<5; k++)
				{
					for (int32_t l=0; l<5; l++)
						s+=tab0[l];
					tab0+=w_map;
				}
				if (s<=9) dst[j]=0;
				else dst[j]=1;
			}
			else dst[j]=0;
		}

		for (int32_t j=w_2; j<w; j++)
			dst[j]=0;

		src+=w_map;
		dst+=w_map;
		tab+=w_map;
	}

	memset(map,0,h*w_map);

	tab=map;
	src=buffer+(w_map<<1);

	for (int32_t i=0; i<h_4; i++)
	{
		for (int32_t j=2; j<w_2; j++)
		{
			if (src[j]==1)
			{
				uint32_t *tab_2=(uint32_t *)(tab+j-1);

				for (int32_t k=0; k<5; k++)
				{
					*tab_2=0x01010101;
					tab_2+=w_map_4;
				}
			}
		}
		tab+=w_map;
		src+=w_map;
	}
}


void JPSDR_IVTC::Motion_Map_Filter_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint8_t *map=(uint8_t *)mt_data_inf.src1;
	uint8_t *buffer=(uint8_t *)mt_data_inf.dst1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w_map=mt_data_inf.data_i32;

	uint8_t *tab;
	uint8_t *dst,*src;
	const int32_t w_2=w-2;

	dst=buffer;

	if (mt_data_inf.top)
	{
		memset(dst,0,w_map<<1);
		dst+=(w_map << 1);

		tab=map;
		src=map+(w_map<<1);
		h-=2;
	}
	else
	{
		tab=map-(w_map<<1);
		src=map;
	}

	if (mt_data_inf.bottom)
	{
		memset(dst+((h-2)*w_map),0,w_map<<1);
		h-=2;
	}

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<2; j++)
			dst[j]=0;

		for (int32_t j=2; j<w_2; j++)
		{
			if (src[j]==1)
			{
				uint8_t s=0;
				const uint8_t *tab0=tab+j-2;

				for (int32_t k=0; k<5; k++)
				{
					for (int32_t l=0; l<5; l++)
						s+=tab0[l];
					tab0+=w_map;
				}
				if (s<=9) dst[j]=0;
				else dst[j]=1;
			}
			else dst[j]=0;
		}

		for (int32_t j=w_2; j<w; j++)
			dst[j]=0;

		src+=w_map;
		dst+=w_map;
		tab+=w_map;
	}
}



void JPSDR_IVTC::Motion_Map_Filter_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint8_t *map=(uint8_t *)mt_data_inf.src1;
	uint8_t *buffer=(uint8_t *)mt_data_inf.dst1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w_map=mt_data_inf.data_i32;

	uint8_t *tab;
	uint8_t *src;
	const int32_t w_2=w-2,w_map_4=w_map >> 2;

	if (mt_data_inf.top)
	{

		tab=map;
		src=buffer+(w_map<<1);
		h-=2;
	}
	else
	{
		tab=map-(w_map<<1);
		src=buffer;
	}

	if (mt_data_inf.bottom) h-=2;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=2; j<w_2; j++)
		{
			if (src[j]==1)
			{
				uint32_t *tab_2=(uint32_t *)(tab+j-1);

				for (int32_t k=0; k<5; k++)
				{
					*tab_2=0x01010101;
					tab_2+=w_map_4;
				}
			}
		}
		tab+=w_map;
		src+=w_map;
	}
}


void JPSDR_IVTC::Histogramme_DeltaPicture_Motion_Map_SSE_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint32_t s_motion_map;

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32(mt_data_inf.src1,
		(uint8_t *)mt_data_inf.src2,mt_data_inf.dst1,(uint32_t *)mt_data_inf.dst2,(uint32_t *)mt_data_inf.dst3,
		mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
		mt_data_inf.dst_pitch1,mt_data_inf.dst_modulo1,mt_data_inf.data_i32,s_motion_map);
	MT_Data[thread_num].data_u32_2=s_motion_map;
}


void JPSDR_IVTC::Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint32_t s_motion_map;

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32(mt_data_inf.src1,
		(uint8_t *)mt_data_inf.src2,mt_data_inf.dst1,(uint32_t *)mt_data_inf.dst2,(uint32_t *)mt_data_inf.dst3,
		mt_data_inf.src_Y_w>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
		mt_data_inf.dst_pitch1,mt_data_inf.dst_modulo1,mt_data_inf.data_i32,s_motion_map);
	MT_Data[thread_num].data_u32_2=s_motion_map;
}


void JPSDR_IVTC::Histogramme_DeltaPicture_Motion_Map_AVX_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint32_t s_motion_map;

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_AVX_RGB32(mt_data_inf.src1,
		(uint8_t *)mt_data_inf.src2,mt_data_inf.dst1,(uint32_t *)mt_data_inf.dst2,(uint32_t *)mt_data_inf.dst3,
		mt_data_inf.src_Y_w>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,
		mt_data_inf.dst_pitch1,mt_data_inf.dst_modulo1,mt_data_inf.data_i32,s_motion_map);
	MT_Data[thread_num].data_u32_2=s_motion_map;
}


void JPSDR_IVTC::Histogramme_DeltaPicture_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const RGB32BMP *buffer,*buffer2;
	const uint8_t *map=(uint8_t *)mt_data_inf.src2;
	RGB32BMP *dst,*dst2,zero_RGB;;
	uint32_t *histo=(uint32_t *)mt_data_inf.dst2;
	uint32_t *repart=(uint32_t *)mt_data_inf.dst3;
	const ptrdiff_t src_pitch=mt_data_inf.src_pitch1,dst_pitch=mt_data_inf.dst_pitch1;
	const int32_t w=mt_data_inf.src_Y_w;
	int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w_map=mt_data_inf.data_i32;
	const int32_t delta_map=w_map-w;
	uint32_t s,s_motion_map;

	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	memset(histo,0,1024);

	zero_RGB.r=0;
	zero_RGB.g=0;
	zero_RGB.b=0;
	zero_RGB.alpha=0;

	s_motion_map=0;
	s=0;
	buffer=(RGB32BMP *)mt_data_inf.src1;
	buffer2=(RGB32BMP *)((uint8_t *)buffer+src_pitch);
	dst=(RGB32BMP *)mt_data_inf.dst1;
	dst2=(RGB32BMP *)((uint8_t *)dst+dst_pitch);
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t r,g,b;
			uint16_t s_int;

			b=buffer[j].b;
			g=buffer[j].g;
			r=buffer[j].r;
			histo[r]++;
			histo[g]++;
			histo[b]++;
			s_int=r+g+b;
			s+=s_int;
			if (*map)
			{
				s_motion_map+=s_int;
				dst[j]=buffer[j];
			}
			else dst[j]=zero_RGB;
			b=buffer2[j].b;
			g=buffer2[j].g;
			r=buffer2[j].r;
			histo[r]++;
			histo[g]++;
			histo[b]++;
			if (map[w_map])
			{
				s_motion_map+=(r+g+b);
				dst2[j]=buffer2[j];
			}
			else dst2[j]=zero_RGB;
			map++;
		}
		map+=(w_map+delta_map);
		buffer=(RGB32BMP *)((uint8_t *)buffer+src_pitch2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+src_pitch2);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch2);
		dst2=(RGB32BMP *)((uint8_t *)dst2+dst_pitch2);
	}

	repart[255]=histo[255];
	for (int32_t i=254; i>=0; i--)
		repart[i]=histo[i]+repart[i+1];

	MT_Data[thread_num].data_u32_1=s;
	MT_Data[thread_num].data_u32_2=s_motion_map;
}



uint32_t JPSDR_IVTC::Norme1_Histogramme_DeltaPicture_RGB32(const void *src1,const void *src2,
		void *dst_0,void *buffer_0,uint8_t *map,uint8_t *buffer_map,
		uint32_t *histo,uint32_t *repart,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t buffer_pitch,const ptrdiff_t dst_pitch,const ptrdiff_t src_modulo,const ptrdiff_t buffer_modulo,
		const ptrdiff_t dst_modulo,uint32_t &error_motion_map,const uint8_t thrs)
{
	uint32_t s,s_motion_map;
	const int32_t w_map=(((w+3)>>2)<<2)+4,delta_map=w_map-w;

	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src_pitch1=src_pitch;
			MT_Data[i].src_modulo1=src_modulo;
			MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
			MT_Data[i].dst2=(void *)((uint8_t *)buffer_0+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch);
			MT_Data[i].dst_pitch2=buffer_pitch;
			MT_Data[i].dst_modulo2=buffer_modulo;
			MT_Data[i].data_u8=thrs;
			MT_Data[i].data_u32_1=thr;
			MT_Data[i].data_i32=w_map;
		}

		if (AVX_Enable && ((w&1)==0)) f_proc=62;
		else
		{
			if (SSE2_Enable && ((w&1)==0)) f_proc=54;
			else
			{
				if (Integer_SSE_Enable) f_proc=28;
				else f_proc=29;
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (AVX_Enable && ((w&1)==0))
		{
			const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

			JPSDR_IVTC_Motion_Map_AVX_RGB32(src1,src2,buffer_0,map,w>>1,h-1,src_pitch,src_modulo,buffer_pitch,buffer_modulo,thr,w_map);
		}
		else
		{
			if (SSE2_Enable && ((w&1)==0))
			{
				const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

				JPSDR_IVTC_Motion_Map_SSE2_RGB32(src1,src2,buffer_0,map,w>>1,h-1,src_pitch,src_modulo,buffer_pitch,buffer_modulo,thr,w_map);
			}
			else
			{
				if (Integer_SSE_Enable)
				{
					const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

					JPSDR_IVTC_Motion_Map_SSE_RGB32(src1,src2,buffer_0,map,w,h-1,src_pitch,src_modulo,buffer_pitch,buffer_modulo,thr,w_map);
				}
				else
					Motion_Map_RGB32(src1,src2,buffer_0,map,w,h,src_pitch,buffer_pitch,w_map,thrs);
			}
		}
	}

	Nb_Threads=CreateMTData(threads_number,w,h<<1,w,h<<1,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)(map+(MT_Data[i].src_Y_h_min*w_map));
			MT_Data[i].dst1=(void *)(buffer_map+(MT_Data[i].dst_Y_h_min*w_map));
			MT_Data[i].data_i32=w_map;
		}

		f_proc=30;

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		memset(map,0,(h<<1)*w_map);

		f_proc=31;

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
		Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)buffer_0+(MT_Data[i].src_Y_h_min << 1)*buffer_pitch);
			MT_Data[i].src2=(void *)(map+(MT_Data[i].src_Y_h_min << 1)*w_map);
			MT_Data[i].dst1=(void *)((uint8_t *)dst_0+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
			MT_Data[i].src_pitch1=buffer_pitch;
			MT_Data[i].src_modulo1=buffer_modulo;
			MT_Data[i].dst_pitch1=dst_pitch;
			MT_Data[i].dst_modulo1=dst_modulo;
			MT_Data[i].dst2=histogramme_MT[i];
			MT_Data[i].dst3=repartition_MT[i];
			MT_Data[i].data_i32=w_map;
		}

		if (AVX_Enable && ((w&3)==0)) f_proc=60;
		else
		{
			if (SSE2_Enable && ((w&3)==0)) f_proc=52;
			else
			{
				if (Integer_SSE_Enable) f_proc=37;
				else f_proc=38;
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;

		s=0;
		s_motion_map=0;
		memset(histo,0,1024);
		memset(repart,0,1024);

		for(uint8_t i=0; i<Nb_Threads; i++)
		{
			s+=MT_Data[i].data_u32_1;
			s_motion_map+=MT_Data[i].data_u32_2;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
		{
			for(uint16_t j=0; j<256; j++)
			{
				histo[j]+=histogramme_MT[i][j];
				repart[j]+=repartition_MT[i][j];
			}
		}

	}
	else
	{
		if (AVX_Enable && ((w&3)==0))
				s=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_AVX_RGB32(buffer_0,map,dst_0,histo,repart,w>>2,
					h,buffer_pitch,buffer_modulo,dst_pitch,dst_modulo,w_map,s_motion_map);
		else
		{

		if (SSE2_Enable && ((w&3)==0))
				s=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32(buffer_0,map,dst_0,histo,repart,w>>2,
					h,buffer_pitch,buffer_modulo,dst_pitch,dst_modulo,w_map,s_motion_map);
		else
		{
			if (Integer_SSE_Enable)
					s=JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32(buffer_0,map,dst_0,histo,repart,w,
						h,buffer_pitch,buffer_modulo,dst_pitch,dst_modulo,w_map,s_motion_map);
			else
			{
				RGB32BMP *dst,*dst2,*buffer,*buffer2,zero_RGB;
				const ptrdiff_t dst_pitch2=dst_pitch << 1,buffer_pitch2=buffer_pitch << 1;

				buffer=(RGB32BMP *)buffer_0;
				buffer2=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch);
				dst=(RGB32BMP *)dst_0;
				dst2=(RGB32BMP *)((uint8_t *)dst+dst_pitch);

				zero_RGB.r=0;
				zero_RGB.g=0;
				zero_RGB.b=0;
				zero_RGB.alpha=0;

				memset(histo,0,1024);
				s_motion_map=0;
				s=0;
				for (int32_t i=0; i<h; i++)
				{
					for (int32_t j=0; j<w; j++)
					{
						uint16_t r,g,b;
						uint16_t s_int;
					
						b=buffer[j].b;
						g=buffer[j].g;
						r=buffer[j].r;
						histo[r]++;
						histo[g]++;
						histo[b]++;
						s_int=r+g+b;
						s+=s_int;
						if (*map)
						{
							s_motion_map+=s_int;
							dst[j]=buffer[j];
						}
						else dst[j]=zero_RGB;
						b=buffer2[j].b;
						g=buffer2[j].g;
						r=buffer2[j].r;
						histo[r]++;
						histo[g]++;
						histo[b]++;
						if (map[w_map])
						{
							s_motion_map+=(r+g+b);
							dst2[j]=buffer2[j];
						}
						else dst2[j]=zero_RGB;
						map++;
					}
					map+=(w_map+delta_map);
					buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
					buffer2=(RGB32BMP *)((uint8_t *)buffer2+buffer_pitch2);
					dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch2);
					dst2=(RGB32BMP *)((uint8_t *)dst2+dst_pitch2);
				}
				
				repart[255]=histo[255];
				for (int32_t i=254; i>=0; i--)
					repart[i]=histo[i]+repart[i+1];
			}
		}

		}
	}

	error_motion_map=s_motion_map;

	return(s);

}


void JPSDR_IVTC::Smart_Deinterlace_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const RGB32BMP *src1=(RGB32BMP *)mt_data_inf.src1;
	const RGB32BMP *src2=(RGB32BMP *)mt_data_inf.src2;
	const uint8_t *map=(uint8_t *)mt_data_inf.src3;
	RGB32BMP *dst=(RGB32BMP *)mt_data_inf.dst1;
	const RGB32BMP *buffer=(RGB32BMP *)mt_data_inf.dst2;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w=mt_data_inf.src_Y_w;

	const ptrdiff_t src_pitch2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t dst_pitch2=mt_data_inf.dst_pitch1 << 1;
	const ptrdiff_t buffer_pitch2=mt_data_inf.dst_pitch2 << 1;
	const int32_t w_map_2=mt_data_inf.data_i32 << 1;

	RGB32BMP *dst2=(RGB32BMP *)((uint8_t *)dst+mt_data_inf.dst_pitch1);
	const uint8_t *map2=map+mt_data_inf.data_i32;
	const RGB32BMP *buffer2=(RGB32BMP *)((uint8_t *)buffer+mt_data_inf.dst_pitch2);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			if (map[j]==1) dst[j]=buffer[j];
			else dst[j]=src1[j];
			if (map2[j]==1) dst2[j]=buffer2[j];
			else dst2[j]=src2[j];
		}
		map+=w_map_2;
		map2+=w_map_2;
		buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+buffer_pitch2);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_pitch2);
		dst2=(RGB32BMP *)((uint8_t *)dst2+dst_pitch2);
		src1=(RGB32BMP *)((uint8_t *)src1+src_pitch2);
		src2=(RGB32BMP *)((uint8_t *)src2+src_pitch2);
	}
}



void JPSDR_IVTC::Smart_Deinterlace_RGB32(const void *src1_0,const void *src2_0,void *dst_0,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t dst_pitch,const ptrdiff_t buffer_pitch,const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,
		const ptrdiff_t buffer_modulo,const uint8_t thrs,bool tri_linear,bool map_filter)
{
	uint32_t thr;
	const int32_t w_map=(((w+3)>>2)<<2)+4;

	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
			MT_Data[i].src_pitch1=src_pitch;
			MT_Data[i].src_modulo1=src_modulo;
			MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
			MT_Data[i].dst2=(void *)((uint8_t *)buffer_0+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch);
			MT_Data[i].dst_pitch2=buffer_pitch;
			MT_Data[i].dst_modulo2=buffer_modulo;
			MT_Data[i].data_u32_1=thr;
			MT_Data[i].data_u8=thrs;
			MT_Data[i].data_i32=w_map;
		}

		if (Integer_SSE_Enable)
		{
			if (tri_linear) f_proc=44;
			else f_proc=43;
		}
		else
		{
			if (tri_linear) f_proc=46;
			else f_proc=45;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (Integer_SSE_Enable)
		{
			thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;
			if (tri_linear)
				JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32(src1_0,src2_0,buffer_0,map,w,h-1,src_pitch,
					src_modulo,buffer_pitch,buffer_modulo,thr,w_map);
			else
				JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32(src1_0,src2_0,buffer_0,map,w,h-1,src_pitch,
					src_modulo,buffer_pitch,buffer_modulo,thr,w_map);
		}
		else
		{
			if (tri_linear)
				Smart_Deinterlace_Tri_Motion_Map_RGB32(src1_0,src2_0,buffer_0,map,w,h,src_pitch,buffer_pitch,
					w_map,thrs);
			else
			Smart_Deinterlace_Motion_Map_RGB32(src1_0,src2_0,buffer_0,map,w,h,src_pitch,buffer_pitch,w_map,thrs);
		}
	}

	if (map_filter)
	{
		Nb_Threads=CreateMTData(threads_number,w,h<<1,w,h<<1,false,false,false,false);

		if (Nb_Threads>1)
		{
			uint8_t f_proc;

			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)(map+(MT_Data[i].src_Y_h_min*w_map));
				MT_Data[i].dst1=(void *)(buffer_map+(MT_Data[i].dst_Y_h_min*w_map));
				MT_Data[i].data_i32=w_map;
			}

			f_proc=30;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			memset(map,0,(h<<1)*w_map);

			f_proc=31;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=0;
		}
		else
			Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);
	}

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	for (uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)src1_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
		MT_Data[i].src2=(void *)((uint8_t *)src2_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
		MT_Data[i].src3=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
		MT_Data[i].src_pitch1=src_pitch;
		MT_Data[i].dst1=(void *)((uint8_t *)dst_0+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
		MT_Data[i].dst2=(void *)((uint8_t *)buffer_0+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch);
		MT_Data[i].dst_pitch2=buffer_pitch;
		MT_Data[i].dst_modulo2=buffer_modulo;
		MT_Data[i].dst_pitch1=dst_pitch;
		MT_Data[i].data_i32=w_map;
	}

	if (Nb_Threads>1)
	{
		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=47;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else Smart_Deinterlace_RGB32_MT(0);

}


void JPSDR_IVTC::Smart_Deinterlace_YUYV_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const YUYV *src1=(YUYV *)mt_data_inf.src1;
	const YUYV *src2=(YUYV *)mt_data_inf.src2;
	const uint8_t *map=(uint8_t *)mt_data_inf.src3;
	YUYV *dst=(YUYV *)mt_data_inf.dst1;
	const YUYV *buffer=(YUYV *)mt_data_inf.dst2;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w=mt_data_inf.src_Y_w;

	const ptrdiff_t src_pitch2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t dst_pitch2=mt_data_inf.dst_pitch1 << 1;
	const ptrdiff_t buffer_pitch2=mt_data_inf.dst_pitch2 << 1;
	const int32_t w_map_2=mt_data_inf.data_i32 << 1;

	YUYV *dst2=(YUYV *)((uint8_t *)dst+mt_data_inf.dst_pitch1);
	const uint8_t *map2=map+mt_data_inf.data_i32;
	const YUYV *buffer2=(YUYV *)((uint8_t *)buffer+mt_data_inf.dst_pitch2);

	for (int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w; j++)
		{
			if ((map[j2]==1) || (map[j2+1]==1)) dst[j]=buffer[j];
			else dst[j]=src1[j];
			if ((map2[j2]==1) || (map2[j2+1]==1)) dst2[j]=buffer2[j];
			else dst2[j]=src2[j];
			j2+=2;
		}
		map+=w_map_2;
		map2+=w_map_2;
		buffer=(YUYV *)((uint8_t *)buffer+buffer_pitch2);
		buffer2=(YUYV *)((uint8_t *)buffer2+buffer_pitch2);
		dst=(YUYV *)((uint8_t *)dst+dst_pitch2);
		dst2=(YUYV *)((uint8_t *)dst2+dst_pitch2);
		src1=(YUYV *)((uint8_t *)src1+src_pitch2);
		src2=(YUYV *)((uint8_t *)src2+src_pitch2);
	}
}


void JPSDR_IVTC::Smart_Deinterlace_YUYV(const void *src1_0,const void *src2_0,void *dst_0,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t dst_pitch,const ptrdiff_t buffer_pitch,const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,
		const ptrdiff_t buffer_modulo,const uint8_t thrs,bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		const ptrdiff_t RGBBuff_pitch,bool Y_only)
{
	const int32_t w_map=(((w+3)>>2)<<2)+4,w_image=(w+1)>>1;

	uint8_t Nb_Threads;

	Deinterlace_YUYV(src1_0,src2_0,buffer_0,w,h,src_pitch,buffer_pitch,tri_linear,true,Y_only);

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		if (Y_only)
		{
			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)((uint8_t *)src1_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
				MT_Data[i].src2=(void *)((uint8_t *)src2_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
				MT_Data[i].src_pitch1=src_pitch;
				MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
				MT_Data[i].data_u8=thrs;
				MT_Data[i].data_i32=w_map;
			}
			f_proc=40;
		}
		else
		{
			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)((uint8_t *)RGBBuff_1+(MT_Data[i].src_Y_h_min << 1)*RGBBuff_pitch);
				MT_Data[i].src2=(void *)((uint8_t *)RGBBuff_2+(MT_Data[i].src_Y_h_min << 1)*RGBBuff_pitch);
				MT_Data[i].src_pitch1=RGBBuff_pitch;
				MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
				MT_Data[i].data_u8=thrs;
				MT_Data[i].data_i32=w_map;
			}
			f_proc=39;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (Y_only)
		{
			Motion_Map_YUYV_Y(src1_0,src2_0,map,w,h,src_pitch,w_map,thrs);
		}
		else
		{
			Motion_Map_RGB32(RGBBuff_1,RGBBuff_2,map,w,h,RGBBuff_pitch,w_map,thrs);
		}
	}

	if (map_filter)
	{
		Nb_Threads=CreateMTData(threads_number,w,h<<1,w,h<<1,false,false,false,false);

		if (Nb_Threads>1)
		{
			uint8_t f_proc;

			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)(map+(MT_Data[i].src_Y_h_min*w_map));
				MT_Data[i].dst1=(void *)(buffer_map+(MT_Data[i].dst_Y_h_min*w_map));
				MT_Data[i].data_i32=w_map;
			}

			f_proc=30;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			memset(map,0,(h<<1)*w_map);

			f_proc=31;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=0;
		}
		else
			Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);
	}

	Nb_Threads=CreateMTData(threads_number,w_image,h,w_image,h,false,false,false,false);

	for (uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)src1_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
		MT_Data[i].src2=(void *)((uint8_t *)src2_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
		MT_Data[i].src3=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
		MT_Data[i].src_pitch1=src_pitch;
		MT_Data[i].dst1=(void *)((uint8_t *)dst_0+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
		MT_Data[i].dst2=(void *)((uint8_t *)buffer_0+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch);
		MT_Data[i].dst_pitch1=dst_pitch;
		MT_Data[i].dst_pitch2=buffer_pitch;
		MT_Data[i].dst_modulo2=buffer_modulo;
		MT_Data[i].data_i32=w_map;
	}

	if (Nb_Threads>1)
	{
		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=48;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else Smart_Deinterlace_YUYV_MT(0);

}



void JPSDR_IVTC::Smart_Deinterlace_UYVY_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const UYVY *src1=(UYVY *)mt_data_inf.src1;
	const UYVY *src2=(UYVY *)mt_data_inf.src2;
	const uint8_t *map=(uint8_t *)mt_data_inf.src3;
	UYVY *dst=(UYVY *)mt_data_inf.dst1;
	const UYVY *buffer=(UYVY *)mt_data_inf.dst2;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w=mt_data_inf.src_Y_w;

	const ptrdiff_t src_pitch2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t dst_pitch2=mt_data_inf.dst_pitch1 << 1;
	const ptrdiff_t buffer_pitch2=mt_data_inf.dst_pitch2 << 1;
	const int32_t w_map_2=mt_data_inf.data_i32 << 1;

	UYVY *dst2=(UYVY *)((uint8_t *)dst+mt_data_inf.dst_pitch1);
	const uint8_t *map2=map+mt_data_inf.data_i32;
	const UYVY *buffer2=(UYVY *)((uint8_t *)buffer+mt_data_inf.dst_pitch2);

	for (int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w; j++)
		{
			if ((map[j2]==1) || (map[j2+1]==1)) dst[j]=buffer[j];
			else dst[j]=src1[j];
			if ((map2[j2]==1) || (map2[j2+1]==1)) dst2[j]=buffer2[j];
			else dst2[j]=src2[j];
			j2+=2;
		}
		map+=w_map_2;
		map2+=w_map_2;
		buffer=(UYVY *)((uint8_t *)buffer+buffer_pitch2);
		buffer2=(UYVY *)((uint8_t *)buffer2+buffer_pitch2);
		dst=(UYVY *)((uint8_t *)dst+dst_pitch2);
		dst2=(UYVY *)((uint8_t *)dst2+dst_pitch2);
		src1=(UYVY *)((uint8_t *)src1+src_pitch2);
		src2=(UYVY *)((uint8_t *)src2+src_pitch2);
	}
}



void JPSDR_IVTC::Smart_Deinterlace_UYVY(const void *src1_0,const void *src2_0,void *dst_0,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,const ptrdiff_t src_pitch,
		const ptrdiff_t dst_pitch,const ptrdiff_t buffer_pitch,const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,
		const ptrdiff_t buffer_modulo,const uint8_t thrs,bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,
		ptrdiff_t RGBBuff_pitch,bool Y_only)
{
	const int32_t w_map=(((w+3)>>2)<<2)+4,w_image=(w+1)>>1;

	uint8_t Nb_Threads;

	Deinterlace_UYVY(src1_0,src2_0,buffer_0,w,h,src_pitch,buffer_pitch,tri_linear,true,Y_only);

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		if (Y_only)
		{
			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)((uint8_t *)src1_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
				MT_Data[i].src2=(void *)((uint8_t *)src2_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
				MT_Data[i].src_pitch1=src_pitch;
				MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
				MT_Data[i].data_u8=thrs;
				MT_Data[i].data_i32=w_map;
			}
			f_proc=41;
		}
		else
		{
			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)((uint8_t *)RGBBuff_1+(MT_Data[i].src_Y_h_min << 1)*RGBBuff_pitch);
				MT_Data[i].src2=(void *)((uint8_t *)RGBBuff_2+(MT_Data[i].src_Y_h_min << 1)*RGBBuff_pitch);
				MT_Data[i].src_pitch1=RGBBuff_pitch;
				MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
				MT_Data[i].data_u8=thrs;
				MT_Data[i].data_i32=w_map;
			}
			f_proc=39;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (Y_only)
		{
			Motion_Map_UYVY_Y(src1_0,src2_0,map,w,h,src_pitch,w_map,thrs);
		}
		else
		{
			Motion_Map_RGB32(RGBBuff_1,RGBBuff_2,map,w,h,RGBBuff_pitch,w_map,thrs);
		}
	}

	if (map_filter)
	{
		Nb_Threads=CreateMTData(threads_number,w,h<<1,w,h<<1,false,false,false,false);

		if (Nb_Threads>1)
		{
			uint8_t f_proc;

			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)(map+(MT_Data[i].src_Y_h_min*w_map));
				MT_Data[i].dst1=(void *)(buffer_map+(MT_Data[i].dst_Y_h_min*w_map));
				MT_Data[i].data_i32=w_map;
			}

			f_proc=30;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			memset(map,0,(h<<1)*w_map);

			f_proc=31;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=0;
		}
		else
			Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);
	}

	Nb_Threads=CreateMTData(threads_number,w_image,h,w_image,h,false,false,false,false);

	for (uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)((uint8_t *)src1_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
		MT_Data[i].src2=(void *)((uint8_t *)src2_0+(MT_Data[i].src_Y_h_min << 1)*src_pitch);
		MT_Data[i].src3=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
		MT_Data[i].src_pitch1=src_pitch;
		MT_Data[i].dst1=(void *)((uint8_t *)dst_0+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch);
		MT_Data[i].dst2=(void *)((uint8_t *)buffer_0+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch);
		MT_Data[i].dst_pitch1=dst_pitch;
		MT_Data[i].dst_pitch2=buffer_pitch;
		MT_Data[i].dst_modulo2=buffer_modulo;
		MT_Data[i].data_i32=w_map;
	}

	if (Nb_Threads>1)
	{
		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=49;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else Smart_Deinterlace_UYVY_MT(0);

}


void JPSDR_IVTC::Smart_Deinterlace_Planar420_MT_Y(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const uint8_t *src1=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src2=(uint8_t *)mt_data_inf.src2;
	const uint8_t *map=(uint8_t *)mt_data_inf.src3;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const uint8_t *buffer=(uint8_t *)mt_data_inf.dst2;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w=mt_data_inf.src_Y_w;

	const ptrdiff_t src_pitch2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t dst_pitch2=mt_data_inf.dst_pitch1 << 1;
	const ptrdiff_t buffer_pitch2=mt_data_inf.dst_pitch2 << 1;
	const int32_t w_map_2=mt_data_inf.data_i32 << 1;

	uint8_t *dst2=dst+mt_data_inf.dst_pitch1;
	const uint8_t *map2=map+mt_data_inf.data_i32;
	const uint8_t *buffer2=buffer+mt_data_inf.dst_pitch2;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			if (map[j]==1) dst[j]=buffer[j];
			else dst[j]=src1[j];
			if (map2[j]==1) dst2[j]=buffer2[j];
			else dst2[j]=src2[j];
		}
		map+=w_map_2;
		map2+=w_map_2;
		buffer+=buffer_pitch2;
		buffer2+=buffer_pitch2;
		dst+=dst_pitch2;
		dst2+=dst_pitch2;
		src1+=src_pitch2;
		src2+=src_pitch2;
	}
}



void JPSDR_IVTC::Smart_Deinterlace_Planar420_MT_UV(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const uint8_t *src1=(uint8_t *)mt_data_inf.src1;
	const uint8_t *src2=(uint8_t *)mt_data_inf.src2;
	const uint8_t *map=(uint8_t *)mt_data_inf.src3;
	uint8_t *dst=(uint8_t *)mt_data_inf.dst1;
	const uint8_t *buffer=(uint8_t *)mt_data_inf.dst2;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w=mt_data_inf.src_Y_w;

	const ptrdiff_t src_pitch2=mt_data_inf.src_pitch1 << 1;
	const ptrdiff_t dst_pitch2=mt_data_inf.dst_pitch1 << 1;
	const ptrdiff_t buffer_pitch2=mt_data_inf.dst_pitch2 << 1;
	const int32_t w_map_2=mt_data_inf.data_i32 << 1;

	uint8_t *dst2=dst+mt_data_inf.dst_pitch1;
	const uint8_t *map1=map+mt_data_inf.data_i32;
	const uint8_t *map2=map+w_map_2;
	const uint8_t *map3=map+3*mt_data_inf.data_i32;
	const uint8_t *buffer2=buffer+mt_data_inf.dst_pitch2;

	for (int32_t i=0; i<h; i++)
	{
		int32_t j2=0;

		for (int32_t j=0; j<w; j++)
		{
			if ((map[j2]==1) || (map[j2+1]==1) || (map2[j2]==1) || (map2[j2+1]==1)) dst[j]=buffer[j];
			else dst[j]=src1[j];
			if ((map1[j2]==1) || (map1[j2+1]==1) || (map3[j2]==1) || (map3[j2+1]==1)) dst2[j]=buffer2[j];
			else dst2[j]=src2[j];
			j2+=2;
		}
		map+=w_map_2;
		map1+=w_map_2;
		map2+=w_map_2;
		map3+=w_map_2;
		buffer+=buffer_pitch2;
		buffer2+=buffer_pitch2;
		dst+=dst_pitch2;
		dst2+=dst_pitch2;
		src1+=src_pitch2;
		src2+=src_pitch2;
	}
}



void JPSDR_IVTC::Smart_Deinterlace_Planar420(const uint8_t *src1_Y,const uint8_t *src2_Y,
	const uint8_t *src1_U,const uint8_t *src2_U,const uint8_t *src1_V,const uint8_t *src2_V,
	uint8_t *dst_Y,uint8_t *dst_U,uint8_t *dst_V,uint8_t *buffer_Y,uint8_t *buffer_U,
	uint8_t *buffer_V,uint8_t *map,uint8_t *buffer_map,const int32_t w_Y,const int32_t w_U,const int32_t w_V,const int32_t h_Y,
	const int32_t h_U,const int32_t h_V,const ptrdiff_t src_pitch_Y,const ptrdiff_t src_pitch_U,const ptrdiff_t src_pitch_V,
	const ptrdiff_t src_modulo_Y,const ptrdiff_t src_modulo_U,const ptrdiff_t src_modulo_V,const ptrdiff_t dst_pitch_Y,
	const ptrdiff_t dst_pitch_U,const ptrdiff_t dst_pitch_V,const ptrdiff_t dst_modulo_Y,const ptrdiff_t dst_modulo_U,
	const ptrdiff_t dst_modulo_V,const ptrdiff_t buffer_pitch_Y,const ptrdiff_t buffer_pitch_U,const ptrdiff_t buffer_pitch_V,
	const ptrdiff_t buffer_modulo_Y,const ptrdiff_t buffer_modulo_U,const ptrdiff_t buffer_modulo_V,const uint8_t thrs,
	bool tri_linear,bool map_filter,const void *RGBBuff_1,const void *RGBBuff_2,ptrdiff_t RGBBuff_pitch,bool Y_only)
{
	const int32_t w_map=(((w_Y+3)>>2)<<2)+4;

	const ptrdiff_t src_pitch_U_2=src_pitch_U << 1;
	const ptrdiff_t src_pitch_V_2=src_pitch_V << 1;
	const ptrdiff_t dst_pitch_U_2=dst_pitch_U << 1;
	const ptrdiff_t dst_pitch_V_2=dst_pitch_V << 1;

	uint8_t Nb_Threads;

	Deinterlace_Planar(src1_Y,src2_Y,buffer_Y,w_Y,h_Y,src_pitch_Y,buffer_pitch_Y,tri_linear,true);
	if (!Y_only)
	{
		Deinterlace_Planar(src1_U,src2_U,buffer_U,w_U,h_U,src_pitch_U,buffer_pitch_U,tri_linear,true);
		Deinterlace_Planar(src1_V,src2_V,buffer_V,w_V,h_V,src_pitch_V,buffer_pitch_V,tri_linear,true);
	}

	Nb_Threads=CreateMTData(threads_number,w_Y,h_Y,w_Y,h_Y,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		if (Y_only)
		{
			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)((uint8_t *)src1_Y+(MT_Data[i].src_Y_h_min << 1)*src_pitch_Y);
				MT_Data[i].src2=(void *)((uint8_t *)src2_Y+(MT_Data[i].src_Y_h_min << 1)*src_pitch_Y);
				MT_Data[i].src_pitch1=src_pitch_Y;
				MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
				MT_Data[i].data_u8=thrs;
				MT_Data[i].data_i32=w_map;
			}
			f_proc=42;
		}
		else
		{
			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)((uint8_t *)RGBBuff_1+(MT_Data[i].src_Y_h_min << 1)*RGBBuff_pitch);
				MT_Data[i].src2=(void *)((uint8_t *)RGBBuff_2+(MT_Data[i].src_Y_h_min << 1)*RGBBuff_pitch);
				MT_Data[i].src_pitch1=RGBBuff_pitch;
				MT_Data[i].dst1=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
				MT_Data[i].data_u8=thrs;
				MT_Data[i].data_i32=w_map;
			}
			f_proc=39;
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (Y_only)
		{
			Motion_Map_Planar420_Y(src1_Y,src2_Y,map,w_Y,h_Y,src_pitch_Y,w_map,thrs);
		}
		else
		{
			Motion_Map_RGB32(RGBBuff_1,RGBBuff_2,map,w_Y,h_Y,RGBBuff_pitch,w_map,thrs);
		}
	}

	if (map_filter)
	{
		Nb_Threads=CreateMTData(threads_number,w_Y,h_Y<<1,w_Y,h_Y<<1,false,false,false,false);

		if (Nb_Threads>1)
		{
			uint8_t f_proc;

			for (uint8_t i=0; i<Nb_Threads; i++)
			{
				MT_Data[i].src1=(void *)(map+(MT_Data[i].src_Y_h_min*w_map));
				MT_Data[i].dst1=(void *)(buffer_map+(MT_Data[i].dst_Y_h_min*w_map));
				MT_Data[i].data_i32=w_map;
			}

			f_proc=30;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			memset(map,0,(h_Y<<1)*w_map);

			f_proc=31;

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=f_proc;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=0;
		}
		else
			Motion_Map_Filter(map,buffer_map,w_Y,h_Y<<1,w_map);
	}

	Nb_Threads=CreateMTData(threads_number,w_Y,h_Y,w_Y,h_Y,false,false,false,false);

	for (uint8_t i=0; i<Nb_Threads; i++)
	{
		MT_Data[i].src1=(void *)(src1_Y+(MT_Data[i].src_Y_h_min << 1)*src_pitch_Y);
		MT_Data[i].src2=(void *)(src2_Y+(MT_Data[i].src_Y_h_min << 1)*src_pitch_Y);
		MT_Data[i].src3=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
		MT_Data[i].src_pitch1=src_pitch_Y;
		MT_Data[i].dst1=(void *)(dst_Y+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch_Y);
		MT_Data[i].dst2=(void *)(buffer_Y+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch_Y);
		MT_Data[i].dst_pitch1=dst_pitch_Y;
		MT_Data[i].dst_pitch2=buffer_pitch_Y;
		MT_Data[i].dst_modulo2=buffer_modulo_Y;
		MT_Data[i].data_i32=w_map;
	}

	if (Nb_Threads>1)
	{
		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=50;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else Smart_Deinterlace_Planar420_MT_Y(0);

	if (!Y_only)
	{
		Nb_Threads=CreateMTData(threads_number,w_U,h_U,w_U,h_U,false,false,false,false);

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)(src1_U+(MT_Data[i].src_Y_h_min << 1)*src_pitch_U);
			MT_Data[i].src2=(void *)(src2_U+(MT_Data[i].src_Y_h_min << 1)*src_pitch_U);
			MT_Data[i].src3=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
			MT_Data[i].src_pitch1=src_pitch_U;
			MT_Data[i].dst1=(void *)(dst_U+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch_U);
			MT_Data[i].dst2=(void *)(buffer_U+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch_U);
			MT_Data[i].dst_pitch1=dst_pitch_U;
			MT_Data[i].dst_pitch2=buffer_pitch_U;
			MT_Data[i].dst_modulo2=buffer_modulo_U;
			MT_Data[i].data_i32=w_map;
		}

		if (Nb_Threads>1)
		{
			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=51;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=0;
		}
		else Smart_Deinterlace_Planar420_MT_UV(0);

		Nb_Threads=CreateMTData(threads_number,w_V,h_V,w_V,h_V,false,false,false,false);

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)(src1_V+(MT_Data[i].src_Y_h_min << 1)*src_pitch_V);
			MT_Data[i].src2=(void *)(src2_V+(MT_Data[i].src_Y_h_min << 1)*src_pitch_V);
			MT_Data[i].src3=(void *)(map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
			MT_Data[i].src_pitch1=src_pitch_V;
			MT_Data[i].dst1=(void *)(dst_V+(MT_Data[i].dst_Y_h_min << 1)*dst_pitch_V);
			MT_Data[i].dst2=(void *)(buffer_V+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch_V);
			MT_Data[i].dst_pitch1=dst_pitch_V;
			MT_Data[i].dst_pitch2=buffer_pitch_V;
			MT_Data[i].dst_modulo2=buffer_modulo_V;
			MT_Data[i].data_i32=w_map;
		}

		if (Nb_Threads>1)
		{
			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=51;

			if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

			for(uint8_t i=0; i<Nb_Threads; i++)
				MT_Thread[i].f_process=0;
		}
		else Smart_Deinterlace_Planar420_MT_UV(0);
	}
	else
	{
		uint8_t *dst2;

		dst2=dst_U+dst_pitch_U;
		for (int32_t i=0; i<h_U; i++)
		{
			memcpy(dst_U,src1_U,w_U);
			memcpy(dst2,src2_U,w_U);
			dst_U+=dst_pitch_U_2;
			dst2+=dst_pitch_U_2;
			src1_U+=src_pitch_U_2;
			src2_U+=src_pitch_U_2;
		}

		dst2=dst_V+dst_pitch_V;
		for (int32_t i=0; i<h_V; i++)
		{
			memcpy(dst_V,src1_V,w_V);
			memcpy(dst2,src2_V,w_V);
			dst_V+=dst_pitch_V_2;
			dst2+=dst_pitch_V_2;
			src1_V+=src_pitch_V_2;
			src2_V+=src_pitch_V_2;
		}
	}

}



void JPSDR_IVTC::DeltaPicture_Motion_Map_SSE_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint32_t s,s_motion_map;

	s=JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32(mt_data_inf.src1,(uint8_t *)mt_data_inf.src2,
		mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.data_i32,
		mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,s_motion_map);

	MT_Data[thread_num].data_u32_1=s;
	MT_Data[thread_num].data_u32_2=s_motion_map;
}


void JPSDR_IVTC::DeltaPicture_Motion_Map_SSE2_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint32_t s,s_motion_map;

	s=JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32(mt_data_inf.src1,(uint8_t *)mt_data_inf.src2,
		mt_data_inf.src_Y_w>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.data_i32,
		mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,s_motion_map);

	MT_Data[thread_num].data_u32_1=s;
	MT_Data[thread_num].data_u32_2=s_motion_map;
}


void JPSDR_IVTC::DeltaPicture_Motion_Map_AVX_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	uint32_t s,s_motion_map;

	s=JPSDR_IVTC_DeltaPicture_Motion_Map_AVX_RGB32(mt_data_inf.src1,(uint8_t *)mt_data_inf.src2,
		mt_data_inf.src_Y_w>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.data_i32,
		mt_data_inf.src_pitch1,mt_data_inf.src_modulo1,s_motion_map);

	MT_Data[thread_num].data_u32_1=s;
	MT_Data[thread_num].data_u32_2=s_motion_map;
}


uint32_t JPSDR_IVTC::Norme1_Motion_Map_RGB32(const void *src1,const void *src2,void *buffer_0,
		uint8_t *map,uint8_t *buffer_map,const int32_t w,const int32_t h,ptrdiff_t pitch,ptrdiff_t buffer_pitch,
		ptrdiff_t modulo,ptrdiff_t buffer_modulo,uint32_t &error_motion_map,const uint8_t thrs)
{
	uint32_t s,s_motion_map;
	const int32_t w_map=(((w+3)>>2)<<2)+4,delta_map=w_map-w;

	uint8_t Nb_Threads;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;
		const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1+(MT_Data[i].src_Y_h_min << 1)*pitch);
			MT_Data[i].src2=(void *)((uint8_t *)src2+(MT_Data[i].src_Y_h_min << 1)*pitch);
			MT_Data[i].src_pitch1=pitch;
			MT_Data[i].src_modulo1=modulo;
			MT_Data[i].dst1=(void *)((uint8_t *)map+(MT_Data[i].dst_Y_h_min << 1)*w_map);
			MT_Data[i].dst2=(void *)((uint8_t *)buffer_0+(MT_Data[i].dst_Y_h_min << 1)*buffer_pitch);
			MT_Data[i].dst_pitch2=buffer_pitch;
			MT_Data[i].dst_modulo2=buffer_modulo;
			MT_Data[i].data_u8=thrs;
			MT_Data[i].data_u32_1=thr;
			MT_Data[i].data_i32=w_map;
		}

		if (AVX_Enable && ((w&1)==0)) f_proc=62;
		else
		{
			if (SSE2_Enable && ((w&1)==0)) f_proc=54;
			{
				if (Integer_SSE_Enable) f_proc=28;
				else f_proc=29;
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
	{
		if (AVX_Enable && ((w&1)==0))
		{
			const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

			JPSDR_IVTC_Motion_Map_AVX_RGB32(src1,src2,buffer_0,map,w>>1,h-1,pitch,modulo,buffer_pitch,buffer_modulo,thr,w_map);
		}
		else
		{
			if (SSE2_Enable && ((w&1)==0))
			{
				const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

				JPSDR_IVTC_Motion_Map_SSE2_RGB32(src1,src2,buffer_0,map,w>>1,h-1,pitch,modulo,buffer_pitch,buffer_modulo,thr,w_map);
			}
			else
			{
				if (Integer_SSE_Enable)
				{
					const uint32_t thr=((uint32_t)thrs<<16)|((uint32_t)thrs<<8)|(uint32_t)thrs;

					JPSDR_IVTC_Motion_Map_SSE_RGB32(src1,src2,buffer_0,map,w,h-1,pitch,modulo,buffer_pitch,buffer_modulo,thr,w_map);
				}
				else
					Motion_Map_RGB32(src1,src2,buffer_0,map,w,h,pitch,buffer_pitch,w_map,thrs);
			}
		}
	}

	Nb_Threads=CreateMTData(threads_number,w,h<<1,w,h<<1,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)map+MT_Data[i].src_Y_h_min*w_map);
			MT_Data[i].dst1=(void *)((uint8_t *)buffer_map+MT_Data[i].dst_Y_h_min*w_map);
			MT_Data[i].data_i32=w_map;
		}

		f_proc=30;

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		memset(map,0,(h<<1)*w_map);

		f_proc=31;

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;
	}
	else
		Motion_Map_Filter(map,buffer_map,w,h<<1,w_map);

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)buffer_0+(MT_Data[i].src_Y_h_min << 1)*buffer_pitch);
			MT_Data[i].src2=(void *)((uint8_t *)map+(MT_Data[i].src_Y_h_min << 1)*w_map);
			MT_Data[i].src_pitch1=buffer_pitch;
			MT_Data[i].src_modulo1=buffer_modulo;
			MT_Data[i].data_i32=w_map;
		}

		if (AVX_Enable && ((w&3)==0)) f_proc=61;
		else
		{
			if (SSE2_Enable && ((w&3)==0)) f_proc=53;
			else
			{
				if (Integer_SSE_Enable) f_proc=32;
				else f_proc=33;
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;

		s=0;
		s_motion_map=0;
		for(uint8_t i=0; i<Nb_Threads; i++)
		{
			s+=MT_Data[i].data_u32_1;
			s_motion_map+=MT_Data[i].data_u32_2;
		}
	}
	else
	{
		if (AVX_Enable && ((w&3)==0))
			s=JPSDR_IVTC_DeltaPicture_Motion_Map_AVX_RGB32(buffer_0,map,w>>2,h,w_map,buffer_pitch,buffer_modulo,s_motion_map);
		else
		{

		if (SSE2_Enable && ((w&3)==0))
			s=JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32(buffer_0,map,w>>2,h,w_map,buffer_pitch,buffer_modulo,s_motion_map);
		else
		{
			if (Integer_SSE_Enable)
				s=JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32(buffer_0,map,w,h,w_map,buffer_pitch,buffer_modulo,s_motion_map);
			else
			{
				const RGB32BMP *buffer=(RGB32BMP *)buffer_0,*buffer2=(RGB32BMP *)((uint8_t *)buffer_0+buffer_pitch);
				const ptrdiff_t buffer_pitch2=buffer_pitch << 1;

				s=0;
				s_motion_map=0;
				for (int32_t i=0; i<h; i++)
				{
					for (int32_t j=0; j<w; j++)
					{
						uint16_t r,g,b;
						uint32_t s_int;

						b=buffer[j].b;
						g=buffer[j].g;
						r=buffer[j].r;
						s_int=r+g+b;
						s+=s_int;
						if (*map) s_motion_map+=s_int;
						if (map[w_map])
						{
							b=buffer2[j].b;
							g=buffer2[j].g;
							r=buffer2[j].r;
							s_motion_map+=(r+g+b);
						}
						map++;
					}
					map+=(w_map+delta_map);
					buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
					buffer2=(RGB32BMP *)((uint8_t *)buffer2+buffer_pitch2);
				}
			}
		}

		}
	}

	error_motion_map=s_motion_map;

	return(s);
}


void JPSDR_IVTC::Norme1_Motion_Map_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const ptrdiff_t buffer_pitch=mt_data_inf.src_pitch1,buffer_pitch2=buffer_pitch << 1;
	const RGB32BMP *buffer=(RGB32BMP *)mt_data_inf.src1,*buffer2=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch);
	const uint8_t *map=(uint8_t *)mt_data_inf.src2;
	const int32_t w=mt_data_inf.src_Y_w;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const int32_t w_map=mt_data_inf.data_i32;

	uint32_t s,s_motion_map;
	const int32_t delta_map=w_map-w;

	s=0;
	s_motion_map=0;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			uint16_t r,g,b;
			uint32_t s_int;

			b=buffer[j].b;
			g=buffer[j].g;
			r=buffer[j].r;
			s_int=r+g+b;
			s+=s_int;
			if (*map) s_motion_map+=s_int;
			if (map[w_map])
			{
				b=buffer2[j].b;
				g=buffer2[j].g;
				r=buffer2[j].r;
				s_motion_map+=(r+g+b);
			}
			map++;
		}
		map+=(w_map+delta_map);
		buffer=(RGB32BMP *)((uint8_t *)buffer+buffer_pitch2);
		buffer2=(RGB32BMP *)((uint8_t *)buffer2+buffer_pitch2);
	}

	MT_Data[thread_num].data_u32_1=s;
	MT_Data[thread_num].data_u32_2=s_motion_map;
}



void JPSDR_IVTC::Norme1_SSE_RGB32_MT_1(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Norme1_SSE_1_RGB32(mt_data_inf.src1,mt_data_inf.src2,
		mt_data_inf.src_Y_w,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1);
}



void JPSDR_IVTC::Norme1_SSE_RGB32_MT_2(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Norme1_SSE_2_RGB32(mt_data_inf.src1,mt_data_inf.src2,
		mt_data_inf.src_Y_w>>1,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1);
}


void JPSDR_IVTC::Norme1_SSE2_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Norme1_SSE2_RGB32(mt_data_inf.src1,mt_data_inf.src2,
		mt_data_inf.src_Y_w>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1);
}


void JPSDR_IVTC::Norme1_AVX_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	MT_Data[thread_num].data_u32_1=JPSDR_IVTC_Norme1_AVX_RGB32(mt_data_inf.src1,mt_data_inf.src2,
		mt_data_inf.src_Y_w>>2,mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min,mt_data_inf.src_pitch1);
}


void JPSDR_IVTC::Norme1_RGB32_MT(uint8_t thread_num)
{
	const MT_Data_Info mt_data_inf=MT_Data[thread_num];

	const RGB32BMP *src1=(RGB32BMP *)mt_data_inf.src1;
	const RGB32BMP *src2=(RGB32BMP *)mt_data_inf.src2;
	const int32_t w=mt_data_inf.src_Y_w;
	const int32_t h=mt_data_inf.src_Y_h_max-mt_data_inf.src_Y_h_min;
	const ptrdiff_t pitch=mt_data_inf.src_pitch1;
	const ptrdiff_t modulo=mt_data_inf.src_modulo1;

	uint32_t s;

	s=0;
	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r,r1,r2,g,g1,g2,b,b1,b2;

			b1=src1->b;
			g1=src1->g;
			r1=(src1++)->r;
			b2=src2->b;
			g2=src2->g;
			r2=(src2++)->r;
			r=abs(r1-r2)&0xFC;				// Noise removal
			g=abs(g1-g2)&0xFC;
			b=abs(b1-b2)&0xFC;
			s+=(r+g+b);
		}
		src1=(RGB32BMP *)((uint8_t *)src1 + (modulo+pitch));
		src2=(RGB32BMP *)((uint8_t *)src2 + (modulo+pitch));
	}

	MT_Data[thread_num].data_u32_1=s;
}


uint32_t JPSDR_IVTC::Norme1_RGB32(const void *src1_0,const void *src2_0,const int32_t w,const int32_t h,
		ptrdiff_t pitch,ptrdiff_t modulo)
{

	uint8_t Nb_Threads;
	uint32_t s;

	Nb_Threads=CreateMTData(threads_number,w,h,w,h,false,false,false,false);

	if (Nb_Threads>1)
	{
		uint8_t f_proc;

		for (uint8_t i=0; i<Nb_Threads; i++)
		{
			MT_Data[i].src1=(void *)((uint8_t *)src1_0+((MT_Data[i].src_Y_h_min<<1)*pitch));
			MT_Data[i].src2=(void *)((uint8_t *)src2_0+((MT_Data[i].src_Y_h_min<<1)*pitch));
			MT_Data[i].src_pitch1=pitch;
			MT_Data[i].src_modulo1=modulo;
		}

		if (AVX_Enable && ((w&3)==0)) f_proc=63;
		else
		{
			if (SSE2_Enable && ((w&3)==0)) f_proc=55;
			else
			{
				if (Integer_SSE_Enable)
				{
					if ((w&0x01)!=0) f_proc=35;
					else f_proc=34;
				}
				else f_proc=36;
			}
		}

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=f_proc;

		if (poolInterface->StartThreads(UserId)) poolInterface->WaitThreadsEnd(UserId);

		for(uint8_t i=0; i<Nb_Threads; i++)
			MT_Thread[i].f_process=0;

		s=0;
		for(uint8_t i=0; i<Nb_Threads; i++)
			s+=MT_Data[i].data_u32_1;
	}
	else
	{
		if (AVX_Enable && ((w&3)==0))
			s=JPSDR_IVTC_Norme1_AVX_RGB32(src1_0,src2_0,w>>2,h,pitch);
		else
		{

		if (SSE2_Enable && ((w&3)==0))
			s=JPSDR_IVTC_Norme1_SSE2_RGB32(src1_0,src2_0,w>>2,h,pitch);
		else
		{
			if (Integer_SSE_Enable)
			{
				if ((w&0x01)!=0) s=JPSDR_IVTC_Norme1_SSE_1_RGB32(src1_0,src2_0,w,h,pitch);
				else s=JPSDR_IVTC_Norme1_SSE_2_RGB32(src1_0,src2_0,w>>1,h,pitch);
			}
			else
			{
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
						r1=(src1++)->r;
						b2=src2->b;
						g2=src2->g;
						r2=(src2++)->r;
						r=abs(r1-r2)&0xFC;				// Noise removal
						g=abs(g1-g2)&0xFC;
						b=abs(b1-b2)&0xFC;
						s+=(r+g+b);
					}
					src1=(RGB32BMP *)((uint8_t *)src1 + (modulo+pitch));
					src2=(RGB32BMP *)((uint8_t *)src2 + (modulo+pitch));
				}
			}
		}

		}
	}

	return(s);
}


void JPSDR_IVTC::StaticThreadpool(void *ptr)
{
	const Public_MT_Data_Thread *data=(const Public_MT_Data_Thread *)ptr;
	JPSDR_IVTC *ptrClass=(JPSDR_IVTC *)data->pClass;

	switch(data->f_process)
	{
		case 1 : ptrClass->Convert_Automatic_Planar420_to_YUY2(data->thread_Id);
			break;
		case 4 : ptrClass->JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2_MT(data->thread_Id);
			break;
		case 5 : ptrClass->JPSDR_IVTC_Convert_YUYV_to_RGB32_2_MT(data->thread_Id);
			break;
		case 8 : ptrClass->JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2_MT(data->thread_Id);
			break;
		case 9 : ptrClass->JPSDR_IVTC_Convert_UYVY_to_RGB32_2_MT(data->thread_Id);
			break;
		case 10 : ptrClass->Resize_Planar420_MT(data->thread_Id);
			break;
		case 11 : ptrClass->Deinterlace_Tri_Blend_RGB32_MT(data->thread_Id);
			break;
		case 12 : ptrClass->Deinterlace_Tri_Blend_SSE_MT(data->thread_Id);
			break;
		case 13 : ptrClass->Deinterlace_Tri_Blend_MMX_MT(data->thread_Id);
			break;
		case 14 : ptrClass->Deinterlace_Tri_Blend_YUYV_MT(data->thread_Id);
			break;
		case 15 : ptrClass->Deinterlace_Tri_Blend_YUYV_Y_MT(data->thread_Id);
			break;
		case 16 : ptrClass->Deinterlace_Tri_Blend_UYVY_MT(data->thread_Id);
			break;
		case 17 : ptrClass->Deinterlace_Tri_Blend_UYVY_Y_MT(data->thread_Id);
			break;
		case 18 : ptrClass->Deinterlace_Tri_Blend_Planar_MT(data->thread_Id);
			break;
		case 19 : ptrClass->Deinterlace_Blend_Non_MMX_RGB32_MT(data->thread_Id);
			break;
		case 20 : ptrClass->Deinterlace_Blend_SSE_MT(data->thread_Id);
			break;
		case 21 : ptrClass->Deinterlace_Blend_SSE_2_MT(data->thread_Id);
			break;
		case 22 : ptrClass->Deinterlace_Blend_SSE_3_MT(data->thread_Id);
			break;
		case 23 : ptrClass->Deinterlace_Blend_YUYV_MT(data->thread_Id);
			break;
		case 24 : ptrClass->Deinterlace_Blend_YUYV_Y_MT(data->thread_Id);
			break;
		case 25 : ptrClass->Deinterlace_Blend_UYVY_MT(data->thread_Id);
			break;
		case 26 : ptrClass->Deinterlace_Blend_UYVY_Y_MT(data->thread_Id);
			break;
		case 27 : ptrClass->Deinterlace_Blend_Planar_MT(data->thread_Id);
			break;
		case 28 : ptrClass->Motion_Map_SSE_RGB32_MT(data->thread_Id);
			break;
		case 29 : ptrClass->Motion_Map_RGB32_MT(data->thread_Id);
			break;
		case 30 : ptrClass->Motion_Map_Filter_MT_1(data->thread_Id);
			break;
		case 31 : ptrClass->Motion_Map_Filter_MT_2(data->thread_Id);
			break;
		case 32 : ptrClass->DeltaPicture_Motion_Map_SSE_RGB32_MT(data->thread_Id);
			break;
		case 33 : ptrClass->Norme1_Motion_Map_RGB32_MT(data->thread_Id);
			break;
		case 34 : ptrClass->Norme1_SSE_RGB32_MT_2(data->thread_Id);
			break;
		case 35 : ptrClass->Norme1_SSE_RGB32_MT_1(data->thread_Id);
			break;
		case 36 : ptrClass->Norme1_RGB32_MT(data->thread_Id);
			break;
		case 37 : ptrClass->Histogramme_DeltaPicture_Motion_Map_SSE_RGB32_MT(data->thread_Id);
			break;
		case 38 : ptrClass->Histogramme_DeltaPicture_RGB32_MT(data->thread_Id);
			break;
		case 39 : ptrClass->Motion_Map_RGB32_2_MT(data->thread_Id);
			break;
		case 40 : ptrClass->Motion_Map_YUYV_Y_MT(data->thread_Id);
			break;
		case 41 : ptrClass->Motion_Map_UYVY_Y_MT(data->thread_Id);
			break;
		case 42 : ptrClass->Motion_Map_Planar420_Y_MT(data->thread_Id);
			break;
		case 43 : ptrClass->Smart_Deinterlace_Motion_Map_SSE_RGB32_MT(data->thread_Id);
			break;
		case 44 : ptrClass->Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_MT(data->thread_Id);
			break;
		case 45 : ptrClass->Smart_Deinterlace_Motion_Map_RGB32_MT(data->thread_Id);
			break;
		case 46 : ptrClass->Smart_Deinterlace_Tri_Motion_Map_RGB32_MT(data->thread_Id);
			break;
		case 47 : ptrClass->Smart_Deinterlace_RGB32_MT(data->thread_Id);
			break;
		case 48 : ptrClass->Smart_Deinterlace_YUYV_MT(data->thread_Id);
			break;
		case 49 : ptrClass->Smart_Deinterlace_UYVY_MT(data->thread_Id);
			break;
		case 50 : ptrClass->Smart_Deinterlace_Planar420_MT_Y(data->thread_Id);
			break;
		case 51 : ptrClass->Smart_Deinterlace_Planar420_MT_UV(data->thread_Id);
			break;
		case 52 : ptrClass->Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32_MT(data->thread_Id);
			break;
		case 53 : ptrClass->DeltaPicture_Motion_Map_SSE2_RGB32_MT(data->thread_Id);
			break;
		case 54 : ptrClass->Motion_Map_SSE2_RGB32_MT(data->thread_Id);
			break;
		case 55 : ptrClass->Norme1_SSE2_RGB32_MT(data->thread_Id);
			break;
		case 56 : ptrClass->JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX_MT(data->thread_Id);
			break;
		case 57 : ptrClass->JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX_MT(data->thread_Id);
			break;
		case 58 : ptrClass->Deinterlace_Tri_Blend_AVX_MT(data->thread_Id);
			break;
		case 59 : ptrClass->Deinterlace_Blend_AVX_MT(data->thread_Id);
			break;
		case 60 : ptrClass->Histogramme_DeltaPicture_Motion_Map_AVX_RGB32_MT(data->thread_Id);
			break;
		case 61 : ptrClass->DeltaPicture_Motion_Map_AVX_RGB32_MT(data->thread_Id);
			break;
		case 62 : ptrClass->Motion_Map_AVX_RGB32_MT(data->thread_Id);
			break;
		case 63 : ptrClass->Norme1_AVX_RGB32_MT(data->thread_Id);
			break;
		default : break;
	}
}


void JPSDR_IVTC::Run()
{
	const VDXPixmapAlpha* tab_pxsrc[8];

	const VDXPixmapAlpha& pxdst = (const VDXPixmapAlpha&)*fa->mpOutputFrames[0]->mpPixmap;
	const VDXFBitmap& pxsrc = *fa->mpSourceFrames[0];
	
	const void *src1,*src2;
	const void *src1_Y,*src2_Y,*src1_U,*src2_U,*src1_V,*src2_V;
	const void *src1_RGB,*src2_RGB;
	void *dst1_Y,*dst1_U,*dst1_V,*dst2_Y,*dst2_U,*dst2_V;
	int32_t h,w,h_removed,h_U,w_U,h_V,w_V,h_Y,w_Y,h_removed_resize;
	ptrdiff_t src_pitch,src_modulo,dst_pitch,dst_modulo,buffer_removed_pitch,frame_removed_pitch;
	ptrdiff_t buffer_resize_removed_pitch;
	ptrdiff_t src_modulo_Y,src_modulo_U,src_modulo_V,src_pitch_Y,src_pitch_U,src_pitch_V;
	ptrdiff_t dst_modulo_Y,dst_modulo_U,dst_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V;
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

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;
	
	if (threads_number>1)
	{
		if (!poolInterface->RequestThreadPool(UserId,threads_number,MT_Thread,-1,false)) return;

		for(uint8_t i=0; i<threads_number; i++)
			MT_Thread[i].f_process=0;
	}

	if (pxsrc.mFrameNumber==0)
	{
		tab_pxsrc[0]=(const VDXPixmapAlpha*)fa->mpSourceFrames[0]->mpPixmap;
		tab_pxsrc[1]=(const VDXPixmapAlpha*)fa->mpSourceFrames[1]->mpPixmap;
		tab_pxsrc[2]=(const VDXPixmapAlpha*)fa->mpSourceFrames[2]->mpPixmap;
		tab_pxsrc[3]=(const VDXPixmapAlpha*)fa->mpSourceFrames[3]->mpPixmap;
		tab_pxsrc[4]=(const VDXPixmapAlpha*)fa->mpSourceFrames[4]->mpPixmap;
		tab_pxsrc[5]=(const VDXPixmapAlpha*)fa->mpSourceFrames[5]->mpPixmap;
		tab_pxsrc[6]=(const VDXPixmapAlpha*)fa->mpSourceFrames[6]->mpPixmap;
		tab_pxsrc[7]=(const VDXPixmapAlpha*)fa->mpSourceFrames[7]->mpPixmap;
	}
	else
	{
		tab_pxsrc[0]=(const VDXPixmapAlpha*)fa->mpSourceFrames[1]->mpPixmap;
		tab_pxsrc[1]=(const VDXPixmapAlpha*)fa->mpSourceFrames[2]->mpPixmap;
		tab_pxsrc[2]=(const VDXPixmapAlpha*)fa->mpSourceFrames[0]->mpPixmap;
		tab_pxsrc[3]=(const VDXPixmapAlpha*)fa->mpSourceFrames[3]->mpPixmap;
		tab_pxsrc[4]=(const VDXPixmapAlpha*)fa->mpSourceFrames[4]->mpPixmap;
		tab_pxsrc[5]=(const VDXPixmapAlpha*)fa->mpSourceFrames[5]->mpPixmap;
		tab_pxsrc[6]=(const VDXPixmapAlpha*)fa->mpSourceFrames[5]->mpPixmap;
		tab_pxsrc[7]=(const VDXPixmapAlpha*)fa->mpSourceFrames[6]->mpPixmap;
	}
		
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	w=image_data.src_w0;
	h=image_data.src_h0;
	w_Y=w;
	h_Y=h;
	w_U=image_data.src_w1;
	w_V=image_data.src_w2;
	h_U=image_data.src_h1;
	h_V=image_data.src_h2;
	h_removed=h-4*mData.line_removed;
	h_removed_resize=480-4*round((480.0/(double)image_data.src_h0)*mData.line_removed);
	w_Y4=((w_Y&0x03)==0);
	w_U4=((w_U&0x03)==0);
	w_V4=((w_V&0x03)==0);
	hw_Y4=(((w_Y*h_Y)&0x03)==0);

	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
		case VMODE_YUYV :
		case VMODE_UYVY :
			dst1_Y=(void *)((uint8_t *)image_data.dst_plane0-((image_data.dst_h0-1)*image_data.dst_pitch0));
			dst2_Y=(void *)((uint8_t *)image_data.dst_plane0-((image_data.dst_h0-2)*image_data.dst_pitch0));
			break;
		case VMODE_PLANAR_YUV420 :
			dst1_Y=(void *)((uint8_t *)image_data.dst_plane0-((image_data.dst_h0-1)*image_data.dst_pitch0));
			dst2_Y=(void *)((uint8_t *)image_data.dst_plane0-((image_data.dst_h0-2)*image_data.dst_pitch0));
			dst1_U=(void *)((uint8_t *)image_data.dst_plane1-((image_data.dst_h1-1)*image_data.dst_pitch1));
			dst2_U=(void *)((uint8_t *)image_data.dst_plane1-((image_data.dst_h1-2)*image_data.dst_pitch1));
			dst1_V=(void *)((uint8_t *)image_data.dst_plane2-((image_data.dst_h2-1)*image_data.dst_pitch2));
			dst2_V=(void *)((uint8_t *)image_data.dst_plane2-((image_data.dst_h2-2)*image_data.dst_pitch2));
			break;
	}

	src_pitch=image_data.src_pitch0;
	src_modulo=image_data.src_modulo0;
	dst_pitch=image_data.dst_pitch0;
	dst_modulo=image_data.dst_modulo0;
	src_pitch_Y=src_pitch;
	src_modulo_Y=src_modulo;
	dst_pitch_Y=dst_pitch;
	dst_modulo_Y=dst_modulo;
	src_pitch_U=image_data.src_pitch1;
	src_modulo_U=image_data.src_modulo1;
	dst_pitch_U=image_data.dst_pitch1;
	dst_modulo_U=image_data.dst_modulo1;
	src_pitch_V=image_data.src_pitch2;
	src_modulo_V=image_data.src_modulo2;
	dst_pitch_V=image_data.dst_pitch2;
	dst_modulo_V=image_data.dst_modulo2;
	frame_removed_pitch=2*src_pitch*mData.line_removed;
	buffer_removed_pitch=2*buffer_frameRGB32_pitch*mData.line_removed;
	buffer_resize_removed_pitch=2*frameRGB32Resize_pitch*round((480.0/(double)image_data.src_h0)*mData.line_removed);
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
			if (i==0)
			{
/*				if (fic1) fclose(fic1);
				fic1=fopen("J:\\Data-1.txt","wt");
				if (fic2) fclose(fic2);
				fic2=fopen("D:\\Data-2.txt","wt");
				if (fic3) fclose(fic3);
				fic3=fopen("D:\\Data-3.txt","wt");*/
			}

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
		
			image_data.src_plane0=tab_pxsrc[i]->data;
			image_data.src_plane1=tab_pxsrc[i]->data2;
			image_data.src_plane2=tab_pxsrc[i]->data3;
		
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
				case VMODE_YUYV :
				case VMODE_UYVY :
					src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
					break;
				case VMODE_PLANAR_YUV420 :
					src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
					src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
					src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
					break;
			}	

			switch(image_data.video_mode)
			{
				case VMODE_YUYV :
					Convert_YUYV_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_pitch,src_modulo,buffer_frameRGB32_pitch,
						buffer_frameRGB32_modulo);
					break;
				case VMODE_UYVY :
					Convert_UYVY_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_pitch,src_modulo,buffer_frameRGB32_pitch,
						buffer_frameRGB32_modulo);
					break;
				case VMODE_PLANAR_YUV420 :
					Convert_Planar420_to_RGB32((uint8_t *)src1_Y,(uint8_t *)src1_U,(uint8_t *)src1_V,
						buffer_delta,buffer[i_wrt].frameRGB32,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,buffer_delta_pitch,
						buffer_frameRGB32_pitch,buffer_delta_modulo16,buffer_frameRGB32_modulo);
					if (resize_720x480)
					{
						Resize_Planar420(src1_Y,src1_U,src1_V,buffer_resize_Y,buffer_resize_U,buffer_resize_V,
							src_pitch_Y,src_pitch_U,src_pitch_V,buffer_resize_Y_pitch,buffer_resize_U_pitch,buffer_resize_V_pitch);
						Convert_Planar420_to_RGB32((uint8_t *)buffer_resize_Y,(uint8_t *)buffer_resize_U,
							(uint8_t *)buffer_resize_V,buffer_delta,buffer[i_wrt].frameRGB32Resize,720,480,buffer_resize_Y_pitch,
							buffer_resize_U_pitch,buffer_resize_V_pitch,buffer_delta_pitch,frameRGB32Resize_pitch,buffer_delta_moduloR16,
							frameRGB32Resize_modulo);
					}
					break;
			}
			
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					src1=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
					src2=(void *)((uint8_t *)src1+src_pitch);
					buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
						error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
						buffer[i_wrt].repartition,w,h_removed>>1,src_pitch,buffer_delta_pitch,error_field_pitch,src_modulo,
						buffer_delta_modulo32,error_field_modulo,buffer[i_wrt].error_Motion_Map,thr_ivtc);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
				case VMODE_PLANAR_YUV420 :
					if (resize_720x480)
					{
						src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
						src2=(void *)((uint8_t *)src1+frameRGB32Resize_pitch);
						buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
							error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
							buffer[i_wrt].repartition,720,h_removed_resize>>1,frameRGB32Resize_pitch,buffer_delta_pitch,
							error_field_pitch,frameRGB32Resize_modulo,buffer_delta_moduloR32,error_field_modulo,
							buffer[i_wrt].error_Motion_Map,thr_ivtc);
					}
					else
					{
						src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
						src2=(void *)((uint8_t *)src1+buffer_frameRGB32_pitch);
						buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
							error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
							buffer[i_wrt].repartition,w,h_removed>>1,buffer_frameRGB32_pitch,buffer_delta_pitch,
							error_field_pitch,buffer_frameRGB32_modulo,buffer_delta_modulo32,error_field_modulo,
							buffer[i_wrt].error_Motion_Map,thr_ivtc);
					}
					break;
			}
			
			if (i==0) continue;			
		
			image_data.src_plane0=tab_pxsrc[i-1]->data;
			image_data.src_plane1=tab_pxsrc[i-1]->data2;
			image_data.src_plane2=tab_pxsrc[i-1]->data3;
		
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
				case VMODE_YUYV :
				case VMODE_UYVY :
					src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
					break;
				case VMODE_PLANAR_YUV420 :
					src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
					src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
					src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
					break;
			}	
			
			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
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
						buffer_map,buffer_2,w,h_removed>>1,src_pitch,buffer_delta_pitch,src_modulo,
						buffer_delta_modulo32,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
				case VMODE_PLANAR_YUV420 :
					if (resize_720x480)
					{
						if (inv_polar)
						{
							src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(frameRGB32Resize_pitch+buffer_resize_removed_pitch));
						}
							else
						{
							src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(frameRGB32Resize_pitch+buffer_resize_removed_pitch));
						}
						buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
							720,h_removed_resize>>1,frameRGB32Resize_pitch,buffer_delta_pitch,frameRGB32Resize_modulo,
							buffer_delta_moduloR32,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
					}
					else
					{
						if (inv_polar)
						{
							src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
						}
						else
						{
							src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
							src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
						}
						buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
							w,h_removed>>1,buffer_frameRGB32_pitch,buffer_delta_pitch,buffer_frameRGB32_modulo,
							buffer_delta_modulo32,buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
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

			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					if (inv_polar) src2=(void *)((uint8_t *)src2_Y+src_pitch);
					else src2=(void *)((uint8_t *)src1_Y+src_pitch);
					buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,src_pitch,src_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
				case VMODE_PLANAR_YUV420 :
					if (resize_720x480)
					{
						if (inv_polar) src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
						else src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
						buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,frameRGB32Resize_pitch,
							frameRGB32Resize_modulo);
					}
					else
					{
						if (inv_polar) src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
						else src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
						buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,buffer_frameRGB32_pitch,
							buffer_frameRGB32_modulo);
					}
					break;
			}
			
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					src1=(void *)((uint8_t *)src2_Y+(frame_removed_pitch+src_pitch));
					src2=(void *)((uint8_t *)src1_Y+(frame_removed_pitch+src_pitch));
					buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,src_pitch,src_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
				case VMODE_PLANAR_YUV420 :
					if (resize_720x480)
					{
						src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(buffer_resize_removed_pitch+frameRGB32Resize_pitch));
						src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(buffer_resize_removed_pitch+frameRGB32Resize_pitch));
						buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,frameRGB32Resize_pitch,
							frameRGB32Resize_modulo);
					}
					else
					{
						src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
						src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
						buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,buffer_frameRGB32_pitch,
							buffer_frameRGB32_modulo);
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
			if (resize_720x480) buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,frameRGB32Resize_pitch,
				frameRGB32Resize_modulo);
			else buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);

			if (i==1) buffer[0].Delta_error_M1=buffer[0].Delta_error_P1;

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

				//fprintf(fic1,"Image : %d\n",i-3);

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
						//fprintf(fic1,"DTOP+1 : %u %u %u\n",v0,v,trunc(0.8*v));

						if (v0<v1) v=v0;
						else v=v1;
						if (v3<v) v=v3;
						dbot_m1_m_f=((v2<0.8*v) && (v>DTopBot)); // 0.6
						//fprintf(fic1,"DTOP-1 M : %u %u %u\n",v1,v,trunc(0.8*v));
					}
					else
					{
						if (v1<v2) v=v1;
						else v=v2;
						if (v3<v) v=v3;
						dtop_p1_f=((v0<0.8*v) && (v>DTopBot)); // 0.6
						//fprintf(fic1,"DTOP+1 : %u %u %u\n",v0,v,trunc(0.8*v));

						if (v0<v2) v=v0;
						else v=v2;
						if (v3<v) v=v3;
						dtop_m1_m_f=((v1<0.8*v) && (v>DTopBot)); // 0.6
						//fprintf(fic1,"DTOP-1 M : %u %u %u\n",v1,v,trunc(0.8*v));
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
					//fprintf(fic1,"DTOP-1 : %u %u %u\n",v0,v,trunc(0.8*v));

					if (v0<v1) v=v0;
					else v=v1;
					if (v3<v) v=v3;
					dtop_p1_f=((v2<0.8*v)&& (v>DTopBot)); //0.6
					//fprintf(fic1,"DBOT+1 : %u %u %u\n",v1,v,trunc(0.8*v));
				}
				else
				{
					if (v1<v2) v=v1;
					else v=v2;
					if (v3<v) v=v3;
					dtop_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6
					//fprintf(fic1,"DTOP-1 : %u %u %u\n",v0,v,trunc(0.8*v));

					if (v0<v2) v=v0;
					else v=v2;
					if (v3<v) v=v3;
					dbot_p1_f=((v1<0.8*v)&& (v>DTopBot)); //0.6
					//fprintf(fic1,"DBOT+1 : %u %u %u\n",v1,v,trunc(0.8*v));
				}

				if (v1<v2) v=v1;
				else v=v2;
				if (v3>v0) v4=v3;
				else v4=v0;
				static_m1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));
				//fprintf(fic1,"STATIC-1 : %u %u %u\n",v4,v,trunc(0.6*v));

				if (v3<v0) v=v3;
				else v=v0;
				if (v1>v2) v4=v1;
				else v4=v2;
				static_p1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));
				//fprintf(fic1,"STATIC+1 : %u %u %u\n",v4,v,trunc(0.6*v));

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
					//fprintf(fic1,"DBOT-1 : %u %u %u\n",v0,v,trunc(0.8*v));

					if (v0<v1) v=v0;
					else v=v1;
					if (v2<v) v=v2;
					dtop_p1_p_f=((v3<0.8*v) && (v>DTopBot));  //0.6
					//fprintf(fic1,"DBOT+1 P : %u %u %u\n",v1,v,trunc(0.8*v));
				}
				else
				{
					if (v1<v2) v=v1;
					else v=v2;
					if (v3<v) v=v3;
					dbot_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6
					//fprintf(fic1,"DBOT-1 : %u %u %u\n",v0,v,trunc(0.8*v));

					if (v0<v2) v=v0;
					else v=v2;
					if (v3<v) v=v3;
					dbot_p1_p_f=((v1<0.8*v) && (v>DTopBot));  //0.6
					//fprintf(fic1,"DBOT+1 P : %u %u %u\n",v1,v,trunc(0.8*v));
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
	
	
	image_data.src_plane0=tab_pxsrc[7]->data;
	image_data.src_plane1=tab_pxsrc[7]->data2;
	image_data.src_plane2=tab_pxsrc[7]->data3;

	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
		case VMODE_YUYV :
		case VMODE_UYVY :
			src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
			break;
		case VMODE_PLANAR_YUV420 :
			src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
			src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
			src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
			break;
	}	

	image_data.src_plane0=tab_pxsrc[6]->data;
	image_data.src_plane1=tab_pxsrc[6]->data2;
	image_data.src_plane2=tab_pxsrc[6]->data3;

	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
		case VMODE_YUYV :
		case VMODE_UYVY :
			src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
			break;
		case VMODE_PLANAR_YUV420 :
			src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
			src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
			src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
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
	
	switch(image_data.video_mode)
	{
		case VMODE_YUYV :
			Convert_YUYV_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_pitch,src_modulo,buffer_frameRGB32_pitch,
					buffer_frameRGB32_modulo);
			break;
		case VMODE_UYVY :
			Convert_UYVY_to_RGB32(src1_Y,buffer[i_wrt].frameRGB32,w,h,src_pitch,src_modulo,buffer_frameRGB32_pitch,
					buffer_frameRGB32_modulo);
			break;
		case VMODE_PLANAR_YUV420 :
			Convert_Planar420_to_RGB32((uint8_t *)src1_Y,(uint8_t *)src1_U,(uint8_t *)src1_V,buffer_delta,
				buffer[i_wrt].frameRGB32,w_Y,h_Y,src_pitch_Y,src_pitch_U,src_pitch_V,buffer_delta_pitch,buffer_frameRGB32_pitch,
				buffer_delta_modulo16,buffer_frameRGB32_modulo);
			if (resize_720x480)
			{
				Resize_Planar420(src1_Y,src1_U,src1_V,buffer_resize_Y,buffer_resize_U,buffer_resize_V,src_pitch_Y,src_pitch_U,
					src_pitch_V,buffer_resize_Y_pitch,buffer_resize_U_pitch,buffer_resize_V_pitch);
				Convert_Planar420_to_RGB32((uint8_t *)buffer_resize_Y,(uint8_t *)buffer_resize_U,
					(uint8_t *)buffer_resize_V,buffer_delta,buffer[i_wrt].frameRGB32Resize,720,480,buffer_resize_Y_pitch,
					buffer_resize_U_pitch,buffer_resize_V_pitch,buffer_delta_pitch,frameRGB32Resize_pitch,buffer_delta_moduloR16,
					frameRGB32Resize_modulo);
			}
			break;
	}

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			src1=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
			src2=(void *)((uint8_t *)src1+src_pitch);
			buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
				error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
				buffer[i_wrt].repartition,w,h_removed>>1,src_pitch,buffer_delta_pitch,error_field_pitch,src_modulo,
				buffer_delta_modulo32,error_field_modulo,buffer[i_wrt].error_Motion_Map,thr_ivtc);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_PLANAR_YUV420 :
			if (resize_720x480)
			{
				src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
				src2=(void *)((uint8_t *)src1+frameRGB32Resize_pitch);
				buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
					error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
					buffer[i_wrt].repartition,720,h_removed_resize>>1,frameRGB32Resize_pitch,buffer_delta_pitch,error_field_pitch,
					frameRGB32Resize_modulo,buffer_delta_moduloR32,error_field_modulo,buffer[i_wrt].error_Motion_Map,thr_ivtc);
			}
			else
			{
				src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
				src2=(void *)((uint8_t *)src1+buffer_frameRGB32_pitch);
				buffer[i_wrt].error=Norme1_Histogramme_DeltaPicture_RGB32(src1,src2,
					error_Fields[i_err],buffer_delta,buffer_map,buffer_2,histogramme,
					buffer[i_wrt].repartition,w,h_removed>>1,buffer_frameRGB32_pitch,buffer_delta_pitch,error_field_pitch,
					buffer_frameRGB32_modulo,buffer_delta_modulo32,error_field_modulo,buffer[i_wrt].error_Motion_Map,thr_ivtc);
			}
			break;
	}


	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
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
				buffer_map,buffer_2,w,h_removed>>1,src_pitch,buffer_delta_pitch,src_modulo,buffer_delta_modulo32,
				buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_PLANAR_YUV420 :
			if (resize_720x480)
			{
				if (inv_polar)
				{
					src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(frameRGB32Resize_pitch+buffer_resize_removed_pitch));
				}
				else
				{
					src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(frameRGB32Resize_pitch+buffer_resize_removed_pitch));
				}
				buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
					720,h_removed_resize>>1,frameRGB32Resize_pitch,buffer_delta_pitch,frameRGB32Resize_modulo,buffer_delta_moduloR32,
					buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
			}
			else
			{
				if (inv_polar)
				{
					src1=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
				}
				else
				{
					src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
					src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
				}
				buffer[i_wrt_m1].error_IVTC_P1=Norme1_Motion_Map_RGB32(src1,src2,buffer_delta,buffer_map,buffer_2,
					w,h_removed>>1,buffer_frameRGB32_pitch,buffer_delta_pitch,buffer_frameRGB32_modulo,buffer_delta_modulo32,
					buffer[i_wrt_m1].error_IVTC_P1_Motion_Map,thr_ivtc);
			}
			break;
	}

	buffer[i_wrt].error_IVTC_M1=buffer[i_wrt_m1].error_IVTC_P1;
	buffer[i_wrt].error_IVTC_M1_Motion_Map=buffer[i_wrt_m1].error_IVTC_P1_Motion_Map;

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			if (inv_polar) src2=(void *)((uint8_t *)src2_Y+frame_removed_pitch);
			else src2=(void *)((uint8_t *)src1_Y+frame_removed_pitch);
			buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,src_pitch,src_modulo);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_PLANAR_YUV420 :
			if (resize_720x480)
			{
				if (inv_polar) src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+buffer_resize_removed_pitch);
				else src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+buffer_resize_removed_pitch);
				buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,frameRGB32Resize_pitch,
					frameRGB32Resize_modulo);
			}
			else
			{
				if (inv_polar) src2=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+buffer_removed_pitch);
				else src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+buffer_removed_pitch);
				buffer[i_wrt_m1].Delta_Bottom_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
			}
			break;
	}

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			src1=(void *)((uint8_t *)src2_Y+(frame_removed_pitch+src_pitch));
			src2=(void *)((uint8_t *)src1_Y+(frame_removed_pitch+src_pitch));
			buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,src_pitch,src_modulo);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_PLANAR_YUV420 :
			if (resize_720x480)
			{
				src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32Resize+(buffer_resize_removed_pitch+frameRGB32Resize_pitch));
				src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32Resize+(buffer_resize_removed_pitch+frameRGB32Resize_pitch));
				buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,frameRGB32Resize_pitch,frameRGB32Resize_modulo);
			}
			else
			{
				src1=(void *)((uint8_t *)buffer[i_wrt_m1].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
				src2=(void *)((uint8_t *)buffer[i_wrt].frameRGB32+(buffer_removed_pitch+buffer_frameRGB32_pitch));
				buffer[i_wrt_m1].Delta_Top_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
			}
			break;
	}

	buffer[i_wrt].Delta_Bottom_M1=buffer[i_wrt_m1].Delta_Bottom_P1;
	buffer[i_wrt].Delta_Top_M1=buffer[i_wrt_m1].Delta_Top_P1;

	src1=error_Fields[i_err_m1];
	src2=error_Fields[i_err];
	if (resize_720x480) buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,720,h_removed_resize>>1,frameRGB32Resize_pitch,
		frameRGB32Resize_modulo);
	else buffer[i_wrt_m1].Delta_error_P1=Norme1_RGB32(src1,src2,w,h_removed>>1,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);

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
	
	//fprintf(fic1,"Image : %d\n",pxsrc.mFrameNumber+7-3);

	v0=buffer[i_rd].error;
	v1=buffer[i_rd].error_IVTC_M1;
	v2=buffer[i_rd].error_IVTC_P1;

	//fprintf(fic1,"Errors : %u %u %u\n",v0,v1,v2);

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

	//fprintf(fic1,"Errors Motion Map : %u %u %u\n",v0,v1,v2);

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
		//fprintf(fic1,"DTOP+1 : %u %u %u\n",v0,v,trunc(0.8*v));

		if (v0<v1) v=v0;
		else v=v1;
		if (v3<v) v=v3;
		dbot_m1_m_f=((v2<0.8*v) && (v>DTopBot)); // 0.6
		//fprintf(fic1,"DTOP-1 M : %u %u %u\n",v1,v,trunc(0.8*v));
	}
	else
	{
		if (v1<v2) v=v1;
		else v=v2;
		if (v3<v) v=v3;
		dtop_p1_f=((v0<0.8*v) && (v>DTopBot)); // 0.6
		//fprintf(fic1,"DTOP+1 : %u %u %u\n",v0,v,trunc(0.8*v));

		if (v0<v2) v=v0;
		else v=v2;
		if (v3<v) v=v3;
		dtop_m1_m_f=((v1<0.8*v) && (v>DTopBot)); // 0.6
		//fprintf(fic1,"DTOP-1 M : %u %u %u\n",v1,v,trunc(0.8*v));
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
		//fprintf(fic1,"DTOP-1 : %u %u %u\n",v0,v,trunc(0.8*v));

		if (v0<v1) v=v0;
		else v=v1;
		if (v3<v) v=v3;
		dtop_p1_f=((v2<0.8*v)&& (v>DTopBot)); //0.6
		//fprintf(fic1,"DBOT+1 : %u %u %u\n",v1,v,trunc(0.8*v));
	}
	else
	{
		if (v1<v2) v=v1;
		else v=v2;
		if (v3<v) v=v3;
		dtop_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6
		//fprintf(fic1,"DTOP-1 : %u %u %u\n",v0,v,trunc(0.8*v));

		if (v0<v2) v=v0;
		else v=v2;
		if (v3<v) v=v3;
		dbot_p1_f=((v1<0.8*v)&& (v>DTopBot)); //0.6
		//fprintf(fic1,"DBOT+1 : %u %u %u\n",v1,v,trunc(0.8*v));
	}

	if (v1<v2) v=v1;
	else v=v2;
	if (v3>v0) v4=v3;
	else v4=v0;
	static_m1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));
	//fprintf(fic1,"STATIC-1 : %u %u %u\n",v4,v,trunc(0.6*v));

	if (v3<v0) v=v3;
	else v=v0;
	if (v1>v2) v4=v1;
	else v4=v2;
	static_p1_f=((v4<0.6*v) && (v4<Static2) && (v>Static));
	//fprintf(fic1,"STATIC+1 : %u %u %u\n",v4,v,trunc(0.6*v));

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
		//fprintf(fic1,"DBOT-1 : %u %u %u\n",v0,v,trunc(0.8*v));

		if (v0<v1) v=v0;
		else v=v1;
		if (v2<v) v=v2;
		dtop_p1_p_f=((v3<0.8*v) && (v>DTopBot));  //0.6
		//fprintf(fic1,"DBOT+1 P : %u %u %u\n",v1,v,trunc(0.8*v));
	}
	else
	{
		if (v1<v2) v=v1;
		else v=v2;
		if (v3<v) v=v3;
		dbot_m1_f=((v0<0.8*v) && (v>DTopBot)); //0.6
		//fprintf(fic1,"DBOT-1 : %u %u %u\n",v0,v,trunc(0.8*v));

		if (v0<v2) v=v0;
		else v=v2;
		if (v3<v) v=v3;
		dbot_p1_p_f=((v1<0.8*v) && (v>DTopBot));  //0.6
		//fprintf(fic1,"DBOT+1 P : %u %u %u\n",v1,v,trunc(0.8*v));
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

/*	if (buffer[i_rd].flags.Z) fprintf(fic1,"Z ");  ////////////////////////////////////
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

		// Si la trame à lever n'est pas déterminée, qu'on a détecté au moins une
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
						image_data.src_plane0=tab_pxsrc[i_tab]->data;
						image_data.src_plane1=tab_pxsrc[i_tab]->data2;
						image_data.src_plane2=tab_pxsrc[i_tab]->data3;
		
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
								break;
							case VMODE_PLANAR_YUV420 :
								src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
								src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
								src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
								break;
						}	

						switch(image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								Move_Full(src1_Y,dst1_Y,image_data.src_line0,h,src_pitch,dst_pitch);
								break;
							case VMODE_PLANAR_YUV420 :
								Move_Full(src1_Y,dst1_Y,image_data.src_line0,h,src_pitch_Y,dst_pitch_Y);
								Move_Full(src1_U,dst1_U,image_data.src_line1,h,src_pitch_U,dst_pitch_U);
								Move_Full(src1_V,dst1_V,image_data.src_line2,h,src_pitch_V,dst_pitch_V);
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

					image_data.src_plane0=tab_pxsrc[i_tab]->data;
					image_data.src_plane1=tab_pxsrc[i_tab]->data2;
					image_data.src_plane2=tab_pxsrc[i_tab]->data3;
		
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
							src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
							break;
					}	

					j=(uint8_t)((v&0x00F0) >> 4);
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;
					
					modif_bottom=(j_tab!=2) || swap_buffer;

					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual]
						+buffer_frameRGB32_pitch);
					
					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
		
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
							break;
					}	

					if ((manual.ivtc_index!=1) || (output_mode==1))
					{
						if (filter_output)
						{
							switch(deinterlace_mode)
							{
							case 1 :
								switch(image_data.video_mode)
								{
									case VMODE_BMP_RGBA :
										Smart_Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
											buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,
											buffer_delta_modulo32,thr_out,tri_linear,map_filter);
										break;
									case VMODE_YUYV :
										Smart_Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
											buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,
											buffer_delta_modulo16,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,
											Y_only);
										break;
									case VMODE_UYVY :
										Smart_Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
											buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,
											buffer_delta_modulo16,thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,
											Y_only);
										break;
									case VMODE_PLANAR_YUV420 :
										Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,
											(uint8_t *)src1_U,(uint8_t *)src2_U,(uint8_t *)src1_V,
											(uint8_t *)src2_V,(uint8_t *)dst1_Y,(uint8_t *)dst1_U,
											(uint8_t *)dst1_V,buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,buffer_2,
											w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,
											src_modulo_U,src_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V,dst_modulo_Y,dst_modulo_U,
											dst_modulo_V,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
											buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,
											tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
										break;
								}
								break;
							case 2 :
								switch (image_data.video_mode)
								{
									case VMODE_BMP_RGBA :
										Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false);
										break;
									case VMODE_YUYV :
										Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,Y_only);
										break;
									case VMODE_UYVY :
										Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,Y_only);
										break;
									case VMODE_PLANAR_YUV420 :
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
											Rebuild_Frame(src1_U,src2_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
											Rebuild_Frame(src1_V,src2_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
										}
										break;
								}
								break;
							}
						}
						else
						{
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
								case VMODE_YUYV :
								case VMODE_UYVY :
									if (modif_top && modif_bottom)
									{
										Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
									}
									else
									{
										if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
										if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
									}
									break;
									break;
								case VMODE_PLANAR_YUV420 :
									if (modif_top && modif_bottom)
									{
										Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
										Rebuild_Frame(src1_U,src2_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame(src1_V,src2_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
									}
									else
									{
										if (modif_top)
										{
											Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
											Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
											Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
										}
										if (modif_bottom)
										{
											Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
											Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
											Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
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
								switch (image_data.video_mode)
								{
									case VMODE_BMP_RGBA :
										Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
											buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
											src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_out,tri_linear,map_filter);
										break;
									case VMODE_YUYV :
										Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
											buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
											src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
											src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
										Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
											buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
										break;
									case VMODE_UYVY :
										Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
											buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
											src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
											src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
										Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
											buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
										break;
									case VMODE_PLANAR_YUV420 :
										Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,
											(uint8_t *)src1_U,(uint8_t *)src2_U,(uint8_t *)src1_V,
											(uint8_t *)src2_V,buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
											buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,
											buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,
											src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,buffer_frame_U_pitch,
											buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,buffer_frame_V_modulo,
											buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,buffer_delta_Y_modulo,
											buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
											src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
										Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
											buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
											buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
											buffer_frameRGB32_modulo);
										break;
								}
								break;
							case 2 : 
								switch (image_data.video_mode)
								{
									case VMODE_BMP_RGBA :
										Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,buffer_frame_pitch,
											tri_linear,true);
										break;
									case VMODE_YUYV :
										Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,buffer_frame_pitch,
											tri_linear,true,Y_only);
										Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
											buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
										break;
									case VMODE_UYVY :
										Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,buffer_frame_pitch,
											tri_linear,true,Y_only);
										Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
											buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
										break;
									case VMODE_PLANAR_YUV420 :
										Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],
											w_Y,h_Y>>1,src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
										if (!Y_only)
										{
											Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],
												w_U,h_U>>1,src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
											Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],
												w_V,h_V>>1,src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
										}
										else
										{
											Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],image_data.src_line1,h_U>>1,src_pitch_U,
												buffer_frame_U_pitch);
											Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],image_data.src_line2,h_V>>1,src_pitch_V,
												buffer_frame_V_pitch);
										}
										Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],
											buffer_frame_V[0],buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,
											buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,
											buffer_delta_modulo16,buffer_frameRGB32_modulo);
										break;
								}
								break;
							}
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
								case VMODE_YUYV :
								case VMODE_UYVY :
									Move_Full(buffer_frame[0],dst1_Y,image_data.src_line0,h,buffer_frame_pitch,dst_pitch);
									break;
								case VMODE_PLANAR_YUV420 :
									Move_Full(buffer_frame_Y[0],dst1_Y,image_data.src_line0,h_Y,buffer_frame_Y_pitch,dst_pitch_Y);
									Move_Full(buffer_frame_U[0],dst1_U,image_data.src_line1,h_U,buffer_frame_U_pitch,dst_pitch_U);
									Move_Full(buffer_frame_V[0],dst1_V,image_data.src_line2,h_V,buffer_frame_V_pitch,dst_pitch_V);
									break;
							}
						}
						else
						{
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
								case VMODE_YUYV :
								case VMODE_UYVY :
									if (modif_top && modif_bottom)
									{
										Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
									}
									else
									{
										if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
										if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
									}
									break;
								case VMODE_PLANAR_YUV420 :
									if (modif_top && modif_bottom)
									{
										Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
										Rebuild_Frame(src1_U,src2_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame(src1_V,src2_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
									}
									else
									{
										if (modif_top)
										{
											Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
											Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
											Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
										}
										if (modif_bottom)
										{
											Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
											Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
											Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
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
						+buffer_frameRGB32_pitch);

					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
		
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
							break;
					}	
											
					j=(uint8_t)(v&0x000F);
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;

					modif_top=(j_tab!=2) || swap_buffer;
					
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];

					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
		
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
							src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
							break;
					}	
															
					switch(deinterlace_mode)
					{
					case 1 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_out,tri_linear,map_filter);
								break;
							case VMODE_YUYV :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[1],
									buffer_frame_U[1],buffer_frame_V[1],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,
									buffer_frame_V_modulo,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
									buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],
									buffer_frame_V[1],buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,
									buffer_delta_modulo16,buffer_frameRGB32_modulo);
								break;
						}
						break;
					case 2 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true);
								break;
							case VMODE_YUYV :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[1],w_Y,h_Y>>1,
									src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[1],w_U,h_U>>1,
										src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[1],w_V,h_V>>1,
										src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
								}
								else
								{
									Rebuild_Frame(src1_U,src2_U,buffer_frame_U[1],image_data.src_line1,h_U>>1,src_pitch_U,buffer_frame_U_pitch);
									Rebuild_Frame(src1_V,src2_V,buffer_frame_V[1],image_data.src_line2,h_V>>1,src_pitch_V,buffer_frame_V_pitch);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],
									buffer_frame_V[1],buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,
									buffer_delta_modulo16,buffer_frameRGB32_modulo);
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
						
						image_data.src_plane0=tab_pxsrc[i_tab]->data;
						image_data.src_plane1=tab_pxsrc[i_tab]->data2;
						image_data.src_plane2=tab_pxsrc[i_tab]->data3;

						src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
							+buffer_frameRGB32_pitch);
						
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
								src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
						
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_frameRGB32_pitch);
						
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)buffer_frame[0]+buffer_frame_pitch);
								pitch2_Y=buffer_frame_pitch;
								pitch2_U=0;
								pitch2_V=0;
								modulo2_Y=buffer_frame_modulo;
								modulo2_U=0;
								modulo2_V=0;
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)(buffer_frame_Y[0]+buffer_frame_Y_pitch);
								src2_U=(void *)(buffer_frame_U[0]+buffer_frame_U_pitch);
								src2_V=(void *)(buffer_frame_V[0]+buffer_frame_V_pitch);
								pitch2_Y=buffer_frame_Y_pitch;
								pitch2_U=buffer_frame_U_pitch;
								pitch2_V=buffer_frame_V_pitch;
								modulo2_Y=buffer_frame_Y_modulo;
								modulo2_U=buffer_frame_U_modulo;
								modulo2_V=buffer_frame_V_modulo;
								break;
						}	
					}
				}
				else
				{
					if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
					{
					
						modif_bottom=(i_tab!=2) || swap_buffer;
						
						image_data.src_plane0=tab_pxsrc[i_tab]->data;
						image_data.src_plane1=tab_pxsrc[i_tab]->data2;
						image_data.src_plane2=tab_pxsrc[i_tab]->data3;
						
						src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_frameRGB32_pitch);
					
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
								src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
						
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_frameRGB32_pitch);
						
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)buffer_frame[0]+buffer_frame_pitch);
								pitch2_Y=buffer_frame_pitch;
								pitch2_U=0;
								pitch2_V=0;
								modulo2_Y=buffer_frame_modulo;
								modulo2_U=0;
								modulo2_V=0;
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)(buffer_frame_Y[0]+buffer_frame_Y_pitch);
								src2_U=(void *)(buffer_frame_U[0]+buffer_frame_U_pitch);
								src2_V=(void *)(buffer_frame_V[0]+buffer_frame_V_pitch);
								pitch2_Y=buffer_frame_Y_pitch;
								pitch2_U=buffer_frame_U_pitch;
								pitch2_V=buffer_frame_V_pitch;
								modulo2_Y=buffer_frame_Y_modulo;
								modulo2_U=buffer_frame_U_modulo;
								modulo2_V=buffer_frame_V_modulo;
								break;
						}
					}
				}					
				if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
				{
					modif_top=(j_tab!=2) || swap_buffer;
					
					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
					
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];
					
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
							src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
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
					
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=buffer_frame[1];
							pitch1_Y=buffer_frame_pitch;
							pitch1_U=0;
							pitch1_V=0;
							modulo1_Y=buffer_frame_modulo;
							modulo1_U=0;
							modulo1_V=0;
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)buffer_frame_Y[1];
							src1_U=(void *)buffer_frame_U[1];
							src1_V=(void *)buffer_frame_V[1];
							pitch1_Y=buffer_frame_Y_pitch;
							pitch1_U=buffer_frame_U_pitch;
							pitch1_V=buffer_frame_V_pitch;
							modulo1_Y=buffer_frame_Y_modulo;
							modulo1_U=buffer_frame_U_modulo;
							modulo1_V=buffer_frame_V_modulo;
							break;
					}								
				}

				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,pitch1_Y,dst_pitch);
						if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,pitch2_Y,dst_pitch);
						break;
					case VMODE_PLANAR_YUV420 :
						if (modif_top)
						{
							Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
							Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
							Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
						}
						if (modif_bottom)
						{
							Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
							Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
							Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
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
					
					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
						
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual]
						+buffer_frameRGB32_pitch);
					
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
							break;
					}	
																					
					j=(uint8_t)(v&0x000F);				
					if (pxsrc.mFrameNumber!=0) j_tab=j+2-manual.ivtc_index;
					else j_tab=j;

					modif_top=(j_tab!=2) || swap_buffer;
					
					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
						
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];
					
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
							src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
							break;
					}	
										
					switch(deinterlace_mode)
					{
					case 1 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_out,tri_linear,map_filter);
								break;
							case VMODE_YUYV :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
									buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,
									buffer_frame_V_modulo,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
									buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
								break;
						}
						break;
					case 2 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true);
								break;
							case VMODE_YUYV :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
									src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
										src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
										src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
								}
								else
								{
									Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],image_data.src_line1,h_U>>1,src_pitch_U,buffer_frame_U_pitch);
									Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],image_data.src_line2,h_V>>1,src_pitch_V,buffer_frame_V_pitch);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
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
					
						image_data.src_plane0=tab_pxsrc[i_tab]->data;
						image_data.src_plane1=tab_pxsrc[i_tab]->data2;
						image_data.src_plane2=tab_pxsrc[i_tab]->data3;
						
						src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
							+buffer_frameRGB32_pitch);
					
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
								src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
											
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_frameRGB32_pitch);
					
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)buffer_frame[1]+buffer_frame_pitch);
								pitch2_Y=buffer_frame_pitch;
								pitch2_U=0;
								pitch2_V=0;
								modulo2_Y=buffer_frame_modulo;
								modulo2_U=0;
								modulo2_V=0;
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)(buffer_frame_Y[1]+buffer_frame_Y_pitch);
								src2_U=(void *)(buffer_frame_U[1]+buffer_frame_U_pitch);
								src2_V=(void *)(buffer_frame_V[1]+buffer_frame_V_pitch);
								pitch2_Y=buffer_frame_Y_pitch;
								pitch2_U=buffer_frame_U_pitch;
								pitch2_V=buffer_frame_V_pitch;
								modulo2_Y=buffer_frame_Y_modulo;
								modulo2_U=buffer_frame_U_modulo;
								modulo2_V=buffer_frame_V_modulo;
								break;
						}
					}
				}
				else
				{
					if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
					{
						modif_bottom=(i_tab!=2) || swap_buffer;
					
						image_data.src_plane0=tab_pxsrc[i_tab]->data;
						image_data.src_plane1=tab_pxsrc[i_tab]->data2;
						image_data.src_plane2=tab_pxsrc[i_tab]->data3;
						
						src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_frameRGB32_pitch);
					
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
								src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
								src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
											
						src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_frameRGB32_pitch);
					
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								src2_Y=(void *)((uint8_t *)buffer_frame[1]+buffer_frame_pitch);
								pitch2_Y=buffer_frame_pitch;
								pitch2_U=0;
								pitch2_V=0;
								modulo2_Y=buffer_frame_modulo;
								modulo2_U=0;
								modulo2_V=0;
								break;
							case VMODE_PLANAR_YUV420 :
								src2_Y=(void *)(buffer_frame_Y[1]+buffer_frame_Y_pitch);
								src2_U=(void *)(buffer_frame_U[1]+buffer_frame_U_pitch);
								src2_V=(void *)(buffer_frame_V[1]+buffer_frame_V_pitch);
								pitch2_Y=buffer_frame_Y_pitch;
								pitch2_U=buffer_frame_U_pitch;
								pitch2_V=buffer_frame_V_pitch;
								modulo2_Y=buffer_frame_Y_modulo;
								modulo2_U=buffer_frame_U_modulo;
								modulo2_V=buffer_frame_V_modulo;
								break;
						}
					}
				}					
				if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
				{
					modif_top=(j_tab!=2) || swap_buffer;
				
					image_data.src_plane0=tab_pxsrc[j_tab]->data;
					image_data.src_plane1=tab_pxsrc[j_tab]->data2;
					image_data.src_plane2=tab_pxsrc[j_tab]->data3;
						
					src1_RGB=manual.bufferRGB32[(manual.read_index+j)%Buffer_Size_Manual];
					
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
							src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
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
					
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=buffer_frame[0];
							pitch1_Y=buffer_frame_pitch;
							pitch1_U=0;
							pitch1_V=0;
							modulo1_Y=buffer_frame_modulo;
							modulo1_U=0;
							modulo1_V=0;
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)buffer_frame_Y[0];
							src1_U=(void *)buffer_frame_U[0];
							src1_V=(void *)buffer_frame_V[0];
							pitch1_Y=buffer_frame_Y_pitch;
							pitch1_U=buffer_frame_U_pitch;
							pitch1_V=buffer_frame_V_pitch;
							modulo1_Y=buffer_frame_Y_modulo;
							modulo1_U=buffer_frame_U_modulo;
							modulo1_V=buffer_frame_V_modulo;
							break;
					}
				}

				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,pitch1_Y,dst_pitch);
						if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,pitch2_Y,dst_pitch);
						break;
					case VMODE_PLANAR_YUV420 :
						if (modif_top)
						{
							Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
							Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
							Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
						}
						if (modif_bottom)
						{
							Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
							Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
							Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
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
					image_data.src_plane0=tab_pxsrc[i_tab]->data;
					image_data.src_plane1=tab_pxsrc[i_tab]->data2;
					image_data.src_plane2=tab_pxsrc[i_tab]->data3;
		
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
							src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
							src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
							break;
					}	
								
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							Move_Full(src1_Y,dst1_Y,image_data.src_line0,h,src_pitch,dst_pitch);
							break;
						case VMODE_PLANAR_YUV420 :
							Move_Full(src1_Y,dst1_Y,image_data.src_line0,h_Y,src_pitch_Y,dst_pitch_Y);
							Move_Full(src1_U,dst1_U,image_data.src_line1,h_U,src_pitch_U,dst_pitch_U);
							Move_Full(src1_V,dst1_V,image_data.src_line2,h_V,src_pitch_V,dst_pitch_V);
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
				
				image_data.src_plane0=tab_pxsrc[i_tab]->data;
				image_data.src_plane1=tab_pxsrc[i_tab]->data2;
				image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
				src1_RGB=buffer[i].frameRGB32;
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
						break;
				}	

				src2_RGB=(void *)((uint8_t *)src1_RGB+buffer_frameRGB32_pitch);
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
						src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
						break;
				}					
								
				if ((index_out!=1) || (output_mode==1))
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
									buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,buffer_delta_modulo32,
									thr_dtl,tri_linear,map_filter);
								break;
							case VMODE_YUYV :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
									buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,buffer_delta_modulo16,
									thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
								break;
							case VMODE_UYVY :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
									buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,buffer_delta_modulo16,
									thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
								break;
							case VMODE_PLANAR_YUV420 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,(uint8_t *)dst1_Y,
									(uint8_t *)dst1_U,(uint8_t *)dst1_V,buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,
									buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,
									src_modulo_U,src_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V,dst_modulo_Y,dst_modulo_U,
									dst_modulo_V,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,buffer_delta_Y_modulo,
									buffer_delta_U_modulo,buffer_delta_V_modulo,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,
									buffer_frameRGB32_pitch,false);
								break;
						}
						break;
					case 2 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false);
								break;
							case VMODE_YUYV :
								Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,false);
								break;
							case VMODE_UYVY :
								Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,tri_linear,false,false);
								break;
							case VMODE_PLANAR_YUV420 :
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
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_dtl,tri_linear,map_filter);
								break;
							case VMODE_YUYV :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_dtl,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_dtl,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
									buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,
									buffer_frame_V_modulo,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
									buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_dtl,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
								break;
						}
						break;
					case 2 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true);
								break;
							case VMODE_YUYV :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,false);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,false);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
									src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
								Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
									src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
								Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
									src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
								break;
						}
						break;
					}
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							Move_Full(buffer_frame[0],dst1_Y,image_data.src_line0,h,buffer_frame_pitch,dst_pitch);
							break;
						case VMODE_PLANAR_YUV420 :
							Move_Full(buffer_frame_Y[0],dst1_Y,image_data.src_line0,h_Y,buffer_frame_Y_pitch,dst_pitch_Y);
							Move_Full(buffer_frame_U[0],dst1_U,image_data.src_line1,h_U,buffer_frame_U_pitch,dst_pitch_U);
							Move_Full(buffer_frame_V[0],dst1_V,image_data.src_line2,h_V,buffer_frame_V_pitch,dst_pitch_V);
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

				image_data.src_plane0=tab_pxsrc[i_tab]->data;
				image_data.src_plane1=tab_pxsrc[i_tab]->data2;
				image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
				src1_RGB=buffer[i].frameRGB32;
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
						break;
				}	
							
				i=(uint8_t)((v&0x00F0) >> 4);		
				if (pxsrc.mFrameNumber!=0)
					i_tab=(((i+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else i_tab=i;
							
				modif_bottom=(i_tab!=2) || swap_buffer;

				image_data.src_plane0=tab_pxsrc[i_tab]->data;
				image_data.src_plane1=tab_pxsrc[i_tab]->data2;
				image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
				src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_frameRGB32_pitch);
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
						src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
						break;
				}					
								
				if ((index_out!=1) || (output_mode==1))
				{
					if ((filter_output) || ((v&0x0400)==0x0400))
					{
						switch(deinterlace_mode)
						{
						case 1 :
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
									Smart_Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
										buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,buffer_delta_modulo32,
										thr_out,tri_linear,map_filter);
									break;
								case VMODE_YUYV :
									Smart_Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
										buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,buffer_delta_modulo16,
										thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
									break;
								case VMODE_UYVY :
									Smart_Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,buffer_delta,buffer_map,
										buffer_2,w,h>>1,src_pitch,dst_pitch,buffer_delta_pitch,src_modulo,dst_modulo,buffer_delta_modulo16,
										thr_out,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
									break;
								case VMODE_PLANAR_YUV420 :
									Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
										(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,(uint8_t *)dst1_Y,
										(uint8_t *)dst1_U,(uint8_t*)dst1_V,buffer_delta_Y,buffer_delta_U,buffer_delta_V,buffer_map,
										buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,src_pitch_Y,src_pitch_U,src_pitch_V,
										src_modulo_Y,src_modulo_U,src_modulo_V,dst_pitch_Y,dst_pitch_U,dst_pitch_V,dst_modulo_Y,dst_modulo_U,
										dst_modulo_V,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
										buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
										src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
									break;
							}
							break;
						case 2 :
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
									Deinterlace_RGB32(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										tri_linear,false);
									break;
								case VMODE_YUYV :
									Deinterlace_YUYV(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										tri_linear,false,Y_only);
									break;
								case VMODE_UYVY :
									Deinterlace_UYVY(src1_Y,src2_Y,dst1_Y,w,h>>1,src_pitch,dst_pitch,
										tri_linear,false,Y_only);
									break;
								case VMODE_PLANAR_YUV420 :
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
										Rebuild_Frame(src1_U,src2_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame(src1_V,src2_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
									}
									break;
							}
							break;
						}
					}
					else
					{
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								if (modif_top && modif_bottom)
								{
									Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
								}
								else
								{
									if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
									if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
								}
								break;
							case VMODE_PLANAR_YUV420 :
								if (modif_top && modif_bottom)
								{
									Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
									Rebuild_Frame(src1_U,src2_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
									Rebuild_Frame(src1_V,src2_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
								}
								else
								{
									if (modif_top)
									{
										Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
										Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
									}
									if (modif_bottom)
									{
										Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
										Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
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
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
									Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
										buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
										src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_out,tri_linear,map_filter);
									break;
								case VMODE_YUYV :
									Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
										buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
										src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
										src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
									Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
										buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
									break;
								case VMODE_UYVY :
									Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
										buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
										src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
										src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
									Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
										buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
									break;
								case VMODE_PLANAR_YUV420 :
									Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
										(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
										buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
										buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
										src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
										buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,
										buffer_frame_V_modulo,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
										buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
										src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
									Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
										buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
										buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
										buffer_frameRGB32_modulo);
									break;
							}
							break;
						case 2 :
							switch (image_data.video_mode)
							{
								case VMODE_BMP_RGBA :
									Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
										buffer_frame_pitch,tri_linear,true);
									break;
								case VMODE_YUYV :
									Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
										buffer_frame_pitch,tri_linear,true,Y_only);
									Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
										buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
									break;
								case VMODE_UYVY :
									Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
										buffer_frame_pitch,tri_linear,true,Y_only);
									Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
										buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
									break;
								case VMODE_PLANAR_YUV420 :
									Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
										src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
									if (!Y_only)
									{
										Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
											src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
										Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
											src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
									}
									else
									{
										Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],image_data.src_line1,h_U>>1,src_pitch_U,
											buffer_frame_U_pitch);
										Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],image_data.src_line2,h_V>>1,src_pitch_V,
											buffer_frame_V_pitch);
									}
									Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
										buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
										buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
										buffer_frameRGB32_modulo);
									break;
							}
							break;
						}
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								Move_Full(buffer_frame[0],dst1_Y,image_data.src_line0,h,buffer_frame_pitch,dst_pitch);
								break;
							case VMODE_PLANAR_YUV420 :
								Move_Full(buffer_frame_Y[0],dst1_Y,image_data.src_line0,h_Y,buffer_frame_Y_pitch,dst_pitch_Y);
								Move_Full(buffer_frame_U[0],dst1_U,image_data.src_line1,h_U,buffer_frame_U_pitch,dst_pitch_U);
								Move_Full(buffer_frame_V[0],dst1_V,image_data.src_line2,h_V,buffer_frame_V_pitch,dst_pitch_V);
								break;
						}
					}
					else
					{
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
							case VMODE_YUYV :
							case VMODE_UYVY :
								if (modif_top && modif_bottom)
								{
									Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
								}
								else
								{
									if (modif_top)Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
									if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,src_pitch,dst_pitch);
								}
								break;
							case VMODE_PLANAR_YUV420 :
								if (modif_top && modif_bottom)
								{
									Rebuild_Frame(src1_Y,src2_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
									Rebuild_Frame(src1_U,src2_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
									Rebuild_Frame(src1_V,src2_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
								}
								else
								{
									if (modif_top)
									{
										Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
										Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
									}
									if (modif_bottom)
									{
										Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
										Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,src_pitch_U,dst_pitch_U);
										Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,src_pitch_V,dst_pitch_V);
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
				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
						break;
				}					

				src2_RGB=(void *)((uint8_t *)src1_RGB+buffer_frameRGB32_pitch);
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
						src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
						break;
				}					
							
				switch(deinterlace_mode)
				{
				case 1 :
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
								src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_dtl,tri_linear,map_filter);
							break;
						case VMODE_YUYV :
							Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
								src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_dtl,tri_linear,map_filter,
								src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
							Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_UYVY :
							Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
								src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_dtl,tri_linear,map_filter,
								src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
							Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_PLANAR_YUV420 :
							Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
								(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[1],
								buffer_frame_U[1],buffer_frame_V[1],buffer_delta_Y,buffer_delta_U,
								buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>2,
								src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
								buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,buffer_frame_V_modulo,
								buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,buffer_delta_Y_modulo,buffer_delta_U_modulo,
								buffer_delta_V_modulo,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
							Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
								buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
								buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
								buffer_frameRGB32_modulo);
							break;
					}
					break;
				case 2 :
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,buffer_frame_pitch,
								tri_linear,true);
							break;
						case VMODE_YUYV :
							Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,buffer_frame_pitch,
								tri_linear,true,false);
							Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_UYVY :
							Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,buffer_frame_pitch,
								tri_linear,true,false);
							Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_PLANAR_YUV420 :
							Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[1],w_Y,h_Y>>1,src_pitch_Y,
								buffer_frame_Y_pitch,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[1],w_U,h_U>>1,src_pitch_U,
								buffer_frame_U_pitch,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[1],w_V,h_V>>1,src_pitch_V,
								buffer_frame_V_pitch,tri_linear,true);
							Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
								buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
								buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
								buffer_frameRGB32_modulo);
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
				
				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src2_RGB=(void *)((uint8_t *)buffer[j].frameRGB32+buffer_frameRGB32_pitch);
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
						src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
						break;
				}					
								
				j=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0)
					j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else j_tab=j;
				
				modif_top=(j_tab!=2) || swap_buffer;
				
				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
						break;
				}					
								
				if ((filter_output) || ((v&0x0400)==0x0400))
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_out,tri_linear,map_filter);
								break;
							case VMODE_YUYV :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[1],
									buffer_frame_U[1],buffer_frame_V[1],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,
									buffer_frame_V_modulo,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
									buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
									buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
								break;
						}
						break;
					case 2 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true);
								break;
							case VMODE_YUYV :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[1],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[1],buffer_frameRGB32[1],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[1],
									w_Y,h_Y>>1,src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[1],
										w_U,h_U>>1,src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[1],
										w_V,h_V>>1,src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
								}
								else
								{
									Rebuild_Frame(src1_U,src2_U,buffer_frame_U[1],image_data.src_line1,h_U>>1,src_pitch_U,buffer_frame_U_pitch);
									Rebuild_Frame(src1_V,src2_V,buffer_frame_V[1],image_data.src_line2,h_V>>1,src_pitch_V,buffer_frame_V_pitch);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[1],buffer_frame_U[1],buffer_frame_V[1],
									buffer_delta,buffer_frameRGB32[1],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
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
				
					image_data.src_plane0=tab_pxsrc[i_tab]->data;
					image_data.src_plane1=tab_pxsrc[i_tab]->data2;
					image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_frameRGB32_pitch);
				
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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

					src2_Y=buffer_frame_Y[0]+buffer_frame_Y_pitch;
					src2_U=buffer_frame_U[0]+buffer_frame_U_pitch;
					src2_V=buffer_frame_V[0]+buffer_frame_V_pitch;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_frameRGB32_pitch);
					
					pitch2_Y=buffer_frame_Y_pitch;
					pitch2_U=buffer_frame_U_pitch;
					pitch2_V=buffer_frame_V_pitch;
					modulo2_Y=buffer_frame_Y_modulo;
					modulo2_U=buffer_frame_U_modulo;
					modulo2_V=buffer_frame_V_modulo;
				}
			}
			else
			{
				if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
				{
					modif_bottom=(i_tab!=2) || swap_buffer;
				
					image_data.src_plane0=tab_pxsrc[i_tab]->data;
					image_data.src_plane1=tab_pxsrc[i_tab]->data2;
					image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
						+buffer_frameRGB32_pitch);
				
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
					
					src2_Y=buffer_frame_Y[0]+buffer_frame_Y_pitch;
					src2_U=buffer_frame_U[0]+buffer_frame_U_pitch;
					src2_V=buffer_frame_V[0]+buffer_frame_V_pitch;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[0]+buffer_frameRGB32_pitch);
					
					pitch2_Y=buffer_frame_Y_pitch;
					pitch2_U=buffer_frame_U_pitch;
					pitch2_V=buffer_frame_V_pitch;
					modulo2_Y=buffer_frame_Y_modulo;
					modulo2_U=buffer_frame_U_modulo;
					modulo2_V=buffer_frame_V_modulo;
				}
			}
			if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
			{
				modif_top=(j_tab!=2) || swap_buffer;
			
				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;				
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
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
				
				pitch1_Y=buffer_frame_Y_pitch;
				pitch1_U=buffer_frame_U_pitch;
				pitch1_V=buffer_frame_V_pitch;
				modulo1_Y=buffer_frame_Y_modulo;
				modulo1_U=buffer_frame_U_modulo;
				modulo1_V=buffer_frame_V_modulo;
			}

			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
				case VMODE_YUYV :
				case VMODE_UYVY :
					if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,pitch1_Y,dst_pitch);
					if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,pitch2_Y,dst_pitch);
					break;
				case VMODE_PLANAR_YUV420 :
					if (modif_top)
					{
						Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
						Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
						Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
					}
					if (modif_bottom)
					{
						Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
						Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
						Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
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
				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
						break;
				}					

				src2_RGB=(void *)((uint8_t *)src1_RGB+buffer_frameRGB32_pitch);

				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
						src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
						break;
				}							
							
				switch(deinterlace_mode)
				{
				case 1 :
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
								src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_dtl,tri_linear,map_filter);
							break;
						case VMODE_YUYV :
							Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
								src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_dtl,tri_linear,map_filter,
								src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
							Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_UYVY :
							Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
								buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
								src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_dtl,tri_linear,map_filter,
								src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
							Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_PLANAR_YUV420 :
							Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
								(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
								buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
								buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
								src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
								buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,buffer_frame_V_modulo,
								buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,buffer_delta_Y_modulo,buffer_delta_U_modulo,
								buffer_delta_V_modulo,thr_dtl,tri_linear,map_filter,src1_RGB,src2_RGB,buffer_frameRGB32_pitch,false);
							Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
								buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
								buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
								buffer_frameRGB32_modulo);
							break;
					}
					break;
				case 2 :
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
							Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,buffer_frame_pitch,
								tri_linear,true);
							break;
						case VMODE_YUYV :
							Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,buffer_frame_pitch,
								tri_linear,true,false);
							Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_UYVY :
							Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,buffer_frame_pitch,
								tri_linear,true,false);
							Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
								buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
							break;
						case VMODE_PLANAR_YUV420 :
							Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
								src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
								src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
							Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
								src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
							Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
								buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
								buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
								buffer_frameRGB32_modulo);
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

				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src2_RGB=(void *)((uint8_t *)buffer[j].frameRGB32+buffer_frameRGB32_pitch);
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
						src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
						src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
						break;
				}					
											
				j=(uint8_t)(v&0x000F);
				if (pxsrc.mFrameNumber!=0)
					j_tab=(((j+Read_Index_Size-((read_index[0]+Read_Index_Size-5)%Read_Index_Size))%Read_Index_Size)+2-index_out)%Read_Index_Size;
				else j_tab=j;

				modif_top=(j_tab!=2) || swap_buffer;

				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
						break;
				}						
								
				if ((filter_output) || ((v&0x0400)==0x0400))
				{
					switch(deinterlace_mode)
					{
					case 1 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Smart_Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo32,thr_out,tri_linear,map_filter);
								break;
							case VMODE_YUYV :
								Smart_Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Smart_Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],buffer_delta,
									buffer_map,buffer_2,w,h>>1,src_pitch,buffer_frame_pitch,buffer_delta_pitch,
									src_modulo,buffer_frame_modulo,buffer_delta_modulo16,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Smart_Deinterlace_Planar420((uint8_t *)src1_Y,(uint8_t *)src2_Y,(uint8_t *)src1_U,
									(uint8_t *)src2_U,(uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_Y[0],
									buffer_frame_U[0],buffer_frame_V[0],buffer_delta_Y,buffer_delta_U,
									buffer_delta_V,buffer_map,buffer_2,w_Y,w_U,w_V,h_Y>>1,h_U>>1,h_V>>1,
									src_pitch_Y,src_pitch_U,src_pitch_V,src_modulo_Y,src_modulo_U,src_modulo_V,buffer_frame_Y_pitch,
									buffer_frame_U_pitch,buffer_frame_V_pitch,buffer_frame_Y_modulo,buffer_frame_U_modulo,
									buffer_frame_V_modulo,buffer_delta_Y_pitch,buffer_delta_U_pitch,buffer_delta_V_pitch,
									buffer_delta_Y_modulo,buffer_delta_U_modulo,buffer_delta_V_modulo,thr_out,tri_linear,map_filter,
									src1_RGB,src2_RGB,buffer_frameRGB32_pitch,Y_only);
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
								break;
						}
						break;
					case 2 :
						switch (image_data.video_mode)
						{
							case VMODE_BMP_RGBA :
								Deinterlace_RGB32(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true);
								break;
							case VMODE_YUYV :
								Deinterlace_YUYV(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_YUYV_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_UYVY :
								Deinterlace_UYVY(src1_Y,src2_Y,buffer_frame[0],w,h>>1,src_pitch,
									buffer_frame_pitch,tri_linear,true,Y_only);
								Convert_UYVY_to_RGB32(buffer_frame[0],buffer_frameRGB32[0],w,h,buffer_frame_pitch,
									buffer_frame_modulo,buffer_frameRGB32_pitch,buffer_frameRGB32_modulo);
								break;
							case VMODE_PLANAR_YUV420 :
								Deinterlace_Planar((uint8_t *)src1_Y,(uint8_t *)src2_Y,buffer_frame_Y[0],w_Y,h_Y>>1,
									src_pitch_Y,buffer_frame_Y_pitch,tri_linear,true);
								if (!Y_only)
								{
									Deinterlace_Planar((uint8_t *)src1_U,(uint8_t *)src2_U,buffer_frame_U[0],w_U,h_U>>1,
										src_pitch_U,buffer_frame_U_pitch,tri_linear,true);
									Deinterlace_Planar((uint8_t *)src1_V,(uint8_t *)src2_V,buffer_frame_V[0],w_V,h_V>>1,
										src_pitch_V,buffer_frame_V_pitch,tri_linear,true);
								}
								else
								{
									Rebuild_Frame(src1_U,src2_U,buffer_frame_U[0],image_data.src_line1,h_U>>1,src_pitch_U,buffer_frame_U_pitch);
									Rebuild_Frame(src1_V,src2_V,buffer_frame_V[0],image_data.src_line2,h_V>>1,src_pitch_V,buffer_frame_V_pitch);
								}
								Convert_Planar420_to_RGB32(buffer_frame_Y[0],buffer_frame_U[0],buffer_frame_V[0],
									buffer_delta,buffer_frameRGB32[0],w_Y,h_Y,buffer_frame_Y_pitch,buffer_frame_U_pitch,
									buffer_frame_V_pitch,buffer_delta_pitch,buffer_frameRGB32_pitch,buffer_delta_modulo16,
									buffer_frameRGB32_modulo);
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
				
					image_data.src_plane0=tab_pxsrc[i_tab]->data;
					image_data.src_plane1=tab_pxsrc[i_tab]->data2;
					image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)buffer[i].frameRGB32+buffer_frameRGB32_pitch);
				
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
					
					src2_Y=buffer_frame_Y[1]+buffer_frame_Y_pitch;
					src2_U=buffer_frame_U[1]+buffer_frame_U_pitch;
					src2_V=buffer_frame_V[1]+buffer_frame_V_pitch;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_frameRGB32_pitch);
					
					pitch2_Y=buffer_frame_Y_pitch;
					pitch2_U=buffer_frame_U_pitch;
					pitch2_V=buffer_frame_V_pitch;
					modulo2_Y=buffer_frame_Y_modulo;
					modulo2_U=buffer_frame_U_modulo;
					modulo2_V=buffer_frame_V_modulo;
				}
			}
			else
			{
				if (((v1&0x0100)==0x0100) || (((v1&0xFF00)==0) && !filter_output))
				{			
					modif_bottom=(i_tab!=2) || swap_buffer;
				
					image_data.src_plane0=tab_pxsrc[i_tab]->data;
					image_data.src_plane1=tab_pxsrc[i_tab]->data2;
					image_data.src_plane2=tab_pxsrc[i_tab]->data3;
				
					src2_RGB=(void *)((uint8_t *)manual.bufferRGB32[(manual.read_index+i)%Buffer_Size_Manual]
						+buffer_frameRGB32_pitch);
				
					switch (image_data.video_mode)
					{
						case VMODE_BMP_RGBA :
						case VMODE_YUYV :
						case VMODE_UYVY :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							break;
						case VMODE_PLANAR_YUV420 :
							src2_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-2)*image_data.src_pitch0));
							src2_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-2)*image_data.src_pitch1));
							src2_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-2)*image_data.src_pitch2));
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
					
					src2_Y=buffer_frame_Y[1]+buffer_frame_Y_pitch;
					src2_U=buffer_frame_U[1]+buffer_frame_U_pitch;
					src2_V=buffer_frame_V[1]+buffer_frame_V_pitch;
					src2_RGB=(void *)((uint8_t *)buffer_frameRGB32[1]+buffer_frameRGB32_pitch);
					
					pitch2_Y=buffer_frame_Y_pitch;
					pitch2_U=buffer_frame_U_pitch;
					pitch2_V=buffer_frame_V_pitch;
					modulo2_Y=buffer_frame_Y_modulo;
					modulo2_U=buffer_frame_U_modulo;
					modulo2_V=buffer_frame_V_modulo;
				}
			}				
			if (((v&0x0100)==0x0100) || (((v&0xFF00)==0) && !filter_output))
			{
				modif_top=(j_tab!=2) || swap_buffer;
			
				image_data.src_plane0=tab_pxsrc[j_tab]->data;
				image_data.src_plane1=tab_pxsrc[j_tab]->data2;
				image_data.src_plane2=tab_pxsrc[j_tab]->data3;
				
				src1_RGB=buffer[j].frameRGB32;				
				
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						break;
					case VMODE_PLANAR_YUV420 :
						src1_Y=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
						src1_U=(void *)((uint8_t *)image_data.src_plane1-((image_data.src_h1-1)*image_data.src_pitch1));
						src1_V=(void *)((uint8_t *)image_data.src_plane2-((image_data.src_h2-1)*image_data.src_pitch2));
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

				pitch1_Y=buffer_frame_Y_pitch;
				pitch1_U=buffer_frame_U_pitch;
				pitch1_V=buffer_frame_V_pitch;
				modulo1_Y=buffer_frame_Y_modulo;
				modulo1_U=buffer_frame_U_modulo;
				modulo1_V=buffer_frame_V_modulo;
			}

			switch (image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
				case VMODE_YUYV :
				case VMODE_UYVY :
					if (modif_top) Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h>>1,pitch1_Y,dst_pitch);
					if (modif_bottom) Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h>>1,pitch2_Y,dst_pitch);
					break;
				case VMODE_PLANAR_YUV420 :
					if (modif_top)
					{
						Rebuild_Frame_2(src1_Y,dst1_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
						Rebuild_Frame_2(src1_U,dst1_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
						Rebuild_Frame_2(src1_V,dst1_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
					}
					if (modif_bottom)
					{
						Rebuild_Frame_2(src2_Y,dst2_Y,image_data.src_line0,h_Y>>1,pitch1_Y,dst_pitch_Y);
						Rebuild_Frame_2(src2_U,dst2_U,image_data.src_line1,h_U>>1,pitch1_U,dst_pitch_U);
						Rebuild_Frame_2(src2_V,dst2_V,image_data.src_line2,h_V>>1,pitch1_V,dst_pitch_V);
					}
					break;	
			}
		}

		if (threads_number>1) poolInterface->ReleaseThreadPool(UserId,false);

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
	int8_t i,j;

/*	if (fic1) fclose(fic1);
	if (fic2) fclose(fic2);
	if (fic3) fclose(fic3);*/

	for (i=Data_Buffer_Size-1; i>=0; i--)
	{
		my_aligned_free(buffer[i].frameRGB32Resize);
	}
	my_aligned_free(buffer_resize_V);
	my_aligned_free(buffer_resize_U);
	my_aligned_free(buffer_resize_Y);
	for (j=threads_number-1; j>=0; j--)
	{
		for (i=Interlaced_Tab_Size-1; i>=0; i--)
		{
			myfree(interlaced_tab_V[j][i]);
			myfree(interlaced_tab_U[j][i]);
		}
	}
	for (i=Data_Buffer_Size-1; i>=0; i--)
	{
		my_aligned_free(buffer[i].frameRGB32);
	}
	for (i=Error_Fields_Size-1; i>=0; i--)
		my_aligned_free(error_Fields[i]);
	for (i=1; i>=0; i--)
	{
		my_aligned_free(buffer_frameRGB32[i]);
		my_aligned_free(buffer_frame[i]);
	}
	myfree(buffer_2);
	myfree(buffer_map);
	my_aligned_free(buffer_delta);
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
	mData.mt_mode=!!argv[15].asInt();
	strcpy_s(mData.filename,1024,*argv[16].asString());

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
    
    SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \"%s\")",
		mData.output_mode,mData.threshold_ivtc,out_filter,mData.threshold_filter,
		mData.threshold_deinterlace,mData.inv_polar,mData.line_removed,mData.deinterlace_mode,
		mData.disable_deinterlace,mData.blend_anime,mData.tri_linear,mData.disable_Z_mode,
		mData.disable_motion_map_filtering,mData.first_last_mode,mData.manual_mode,mData.mt_mode,save);
}



extern VDXFilterDefinition2 filterDef_JPSDR_IVTC=
VDXVideoFilterDefinition<JPSDR_IVTC>("JPSDR","IVTC v6.6.2","IVTC Filter. [SSE2][AVX] Optimised.");
