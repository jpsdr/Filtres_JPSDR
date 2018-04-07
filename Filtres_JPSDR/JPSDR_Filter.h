/* 
WARNING !!!!!!!!!!!!!!!!
RGB32, YUYV and UYVY struct must be compiled with alined data on 1 byte.
*/ 

#ifndef _JPSDR_FILTER_H
#define _JPSDR_FILTER_H

#include <stdint.h>

#pragma pack(push,1)		// Compiler option, alined data on 1 byte, equal /Zp1

/*
typedef struct _RGB32
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t alpha;
} RGB32;

typedef union _URGB32
{
	RGB32 rgb32;
	uint32_t data32;
} URGB32;


typedef struct _RGB64
{
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t alpha;
} RGB64;

typedef union _URGB64
{
	RGB64 rgb64;
	uint64_t data64;
} URGB64;
*/

typedef struct _RGB32BMP
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t alpha;
} RGB32BMP;

typedef union _URGB32BMP
{
	RGB32BMP rgb32bmp;
	uint32_t data32;
} URGB32BMP;


typedef struct _YUYV
{
	uint8_t y1;
	uint8_t u;
	uint8_t y2;
	uint8_t v;
} YUYV;

typedef union _UYUYV
{
	YUYV yuyv;
	uint32_t data32;
} UYUYV;


typedef struct _UYVY
{
	uint8_t u;
	uint8_t y1;
	uint8_t v;
	uint8_t y2;
} UYVY;

typedef union _UUYVY
{
	UYVY uyvy;
	uint32_t data32;
} UUYVY;


typedef struct _RGB64BMP
{
	uint16_t b;
	uint16_t g;
	uint16_t r;
	uint16_t alpha;
} RGB64BMP;

typedef union _URGB64BMP
{
	RGB64BMP rgb64bmp;
	uint64_t data64;
} URGB64BMP;


typedef struct _YUYV16
{
	uint16_t y1;
	uint16_t u;
	uint16_t y2;
	uint16_t v;
} YUYV16;

typedef union _UYUYV16
{
	YUYV16 yuyv;
	uint64_t data64;
} UYUYV16;

#pragma pack(pop)		// Put back project alined data setting

#define MATRIX_NONE 0
#define MATRIX_BT601 1
#define MATRIX_BT709 2
#define MATRIX_BT2020 3
#define MATRIX_BT2100 4
#define INTERLACED_NONE 0
#define PROGRESSIVE 1
#define INTERLACED 2
#define INTERLACED_TFF 3
#define INTERLACED_BFF 4
#define VMODE_NONE 0
#define VMODE_RGBABMP 1
#define VMODE_YUYV 2
#define VMODE_UYVY 3
#define VMODE_YUV444 4
#define VMODE_YUV422 5
#define VMODE_YUV420 6
#define VMODE_YUV411 7
#define VMODE_YUV410 8
#define VMODE_Y 9

typedef struct _Image_Data
{
	uint8_t video_mode,src_video_mode,dst_video_mode;
	bool alpha_planar,src_alpha_planar,dst_alpha_planar;

	const void *src_plane0,*src_plane1,*src_plane2,*src_plane3;
	int32_t src_w0,src_w1,src_w2,src_w3;
	int32_t src_h0,src_h1,src_h2,src_h3;
	uint32_t src_line0,src_line1,src_line2,src_line3;
	ptrdiff_t src_pitch0,src_pitch1,src_pitch2,src_pitch3;
	ptrdiff_t src_pitch0_al,src_pitch1_al,src_pitch2_al,src_pitch3_al;
	ptrdiff_t src_modulo0,src_modulo1,src_modulo2,src_modulo3;
	ptrdiff_t src_modulo0_al,src_modulo1_al,src_modulo2_al,src_modulo3_al;
	size_t src_size0,src_size1,src_size2,src_size3;
	size_t src_size0_al,src_size1_al,src_size2_al,src_size3_al;
	uint16_t src_Y_Min,src_Y_Max,src_U_Min,src_U_Max,src_V_Min,src_V_Max;
	bool src_full_mode;
	uint8_t src_color_matrix,src_interlaced,src_bits_pixel;
	double src_Kr,src_Kg,src_Kb,src_Ur,src_Ug,src_Ub,src_Vr,src_Vg,src_Vb;
	int16_t src_Off_Y,src_Off_U,src_Off_V;
	double src_Coeff_Y,src_Coeff_U,src_Coeff_V,src_Rv,src_Gu,src_Gv,src_Bu;
	double src_Off_R,src_Off_G,src_Off_B;

	void *dst_plane0,*dst_plane1,*dst_plane2,*dst_plane3;
	int32_t dst_w0,dst_w1,dst_w2,dst_w3;
	int32_t dst_h0,dst_h1,dst_h2,dst_h3;
	uint32_t dst_line0,dst_line1,dst_line2,dst_line3;
	ptrdiff_t dst_pitch0,dst_pitch1,dst_pitch2,dst_pitch3;
	ptrdiff_t dst_pitch0_al,dst_pitch1_al,dst_pitch2_al,dst_pitch3_al;
	ptrdiff_t dst_modulo0,dst_modulo1,dst_modulo2,dst_modulo3;
	ptrdiff_t dst_modulo0_al,dst_modulo1_al,dst_modulo2_al,dst_modulo3_al;
	size_t dst_size0,dst_size1,dst_size2,dst_size3;
	size_t dst_size0_al,dst_size1_al,dst_size2_al,dst_size3_al;
	uint16_t dst_Y_Min,dst_Y_Max,dst_U_Min,dst_U_Max,dst_V_Min,dst_V_Max;
	bool dst_full_mode;
	uint8_t dst_color_matrix,dst_interlaced,dst_bits_pixel;
	double dst_Kr,dst_Kg,dst_Kb,dst_Ur,dst_Ug,dst_Ub,dst_Vr,dst_Vg,dst_Vb;
	int16_t dst_Off_Y,dst_Off_U,dst_Off_V;
	double dst_Coeff_Y,dst_Coeff_U,dst_Coeff_V,dst_Rv,dst_Gu,dst_Gv,dst_Bu;
	double dst_Off_R,dst_Off_G,dst_Off_B;

	_Image_Data(void)
	{
		video_mode=VMODE_NONE; src_video_mode=VMODE_NONE; dst_video_mode=VMODE_NONE;
		alpha_planar=false; src_alpha_planar=false; dst_alpha_planar=false;

		src_plane0=NULL; src_plane1=NULL; src_plane2=NULL; src_plane3=NULL;
		src_w0=0; src_w1=0; src_w2=0; src_w3=0;
		src_h0=0; src_h1=0; src_h2=0; src_h3=0;
		src_line0=0; src_line1=0; src_line2=0; src_line3=0;
		src_pitch0=0; src_pitch1=0; src_pitch2=0; src_pitch3=0;
		src_pitch0_al=0; src_pitch1_al=0; src_pitch2_al=0; src_pitch3_al=0;
		src_modulo0=0; src_modulo1=0; src_modulo2=0; src_modulo3=0;
		src_modulo0_al=0; src_modulo1_al=0; src_modulo2_al=0; src_modulo3_al=0;
		src_size0=0; src_size1=0; src_size2=0; src_size3=0;
		src_size0_al=0; src_size1_al=0; src_size2_al=0; src_size3_al=0;
		src_full_mode=false;
		src_color_matrix=MATRIX_NONE; src_interlaced=INTERLACED_NONE; src_bits_pixel=8;

		dst_plane0=NULL; dst_plane1=NULL; dst_plane2=NULL; dst_plane3=NULL;
		dst_w0=0; dst_w1=0; dst_w2=0; dst_w3=0;
		dst_h0=0; dst_h1=0; dst_h2=0; dst_h3=0;
		dst_line0=0; dst_line1=0; dst_line2=0; dst_line3=0;
		dst_pitch0=0; dst_pitch1=0; dst_pitch2=0; dst_pitch3=0;
		dst_pitch0_al=0; dst_pitch1_al=0; dst_pitch2_al=0; dst_pitch3_al=0;
		dst_modulo0=0; dst_modulo1=0; dst_modulo2=0; dst_modulo3=0;
		dst_modulo0_al=0; dst_modulo1_al=0; dst_modulo2_al=0; dst_modulo3_al=0;
		dst_size0=0; dst_size1=0; dst_size2=0; dst_size3=0;
		dst_size0_al=0; dst_size1_al=0; dst_size2_al=0; dst_size3_al=0;
		dst_full_mode=false;
		dst_color_matrix=MATRIX_NONE; dst_interlaced=INTERLACED_NONE; dst_bits_pixel=8;
	};

} Image_Data;

#define myfree(ptr) if (ptr!=NULL) { free(ptr); ptr=NULL;}
#define myCloseHandle(ptr) if (ptr!=NULL) { CloseHandle(ptr); ptr=NULL;}

#define ALIGN_SIZE 64
#define ALIGN_SHIFT 6
#define my_aligned_free(ptr) if (ptr!=NULL) { _aligned_free(ptr); ptr=NULL;}

/*
#define abs64(x) (x<0)?-x:x
*/
inline ptrdiff_t abs64(ptrdiff_t x)
{
	if (x<0) return(-x); else return(x);
}

#endif