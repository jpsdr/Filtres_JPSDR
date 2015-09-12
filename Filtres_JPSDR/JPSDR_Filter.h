/* 
WARNING !!!!!!!!!!!!!!!!
RGB32, YUYV and UYVY struct must be compiled with alined data on 1 byte.
*/ 

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

#pragma pack(pop)		// Put back project alined data setting

typedef struct _Image_Data
{
	uint8_t video_mode,src_video_mode,dst_video_mode;
	const void *src_plane0,*src_plane1,*src_plane2;
	int32_t src_w0,src_w1,src_w2;
	int32_t src_h0,src_h1,src_h2;
	ptrdiff_t src_pitch0,src_pitch1,src_pitch2;
	ptrdiff_t src_modulo0,src_modulo1,src_modulo2;
	size_t src_size0,src_size1,src_size2;
	bool src_full_mode;
	uint8_t src_color_matrix,src_interlaced;
	void *dst_plane0,*dst_plane1,*dst_plane2;
	int32_t dst_w0,dst_w1,dst_w2;
	int32_t dst_h0,dst_h1,dst_h2;
	ptrdiff_t dst_pitch0,dst_pitch1,dst_pitch2;
	ptrdiff_t dst_modulo0,dst_modulo1,dst_modulo2;
	size_t dst_size0,dst_size1,dst_size2;
	bool dst_full_mode;
	uint8_t dst_color_matrix,dst_interlaced;	
} Image_Data;

#define MATRIX_NONE 0
#define MATRIX_BT709 1
#define MATRIX_BT601 2
#define PROGRESSIVE 0
#define INTERLACED_NONE 0
#define INTERLACED 1
#define INTERLACED_TFF 2
#define INTERLACED_BFF 3

#define myfree(ptr) if (ptr!=NULL) { free(ptr); ptr=NULL;}

#define ALIGN_SIZE 64
#define ALIGN_SHIFT 6
#define my_aligned_free(ptr) if (ptr!=NULL) { _aligned_free(ptr); ptr=NULL;}

// Cache size for asmlib function, a little more the size of a 720p YV12 frame
#define MAX_CACHE_SIZE 1400000

/*
#define abs64(x) (x<0)?-x:x
*/
inline ptrdiff_t abs64(ptrdiff_t x)
{
	if (x<0) return(-x); else return(x);
}
