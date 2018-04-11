#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"

extern int g_VFVAPIVersion;

extern "C" void JPSDR_BlurMotion_SSE_3_A(const void *src,const void *mem,void *dst,int32_t w,
		int32_t h,ptrdiff_t src_pitch,ptrdiff_t buffer_pitch,ptrdiff_t dst_pitch);


class JPSDR_BlurMotion : public JPSDRVDXVideoFilter
{
public:
	virtual bool Init();
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual bool Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher);

protected:
	Image_Data image_data;
	bool SSE2_Enable,VDub2_Enable;

	void Blend_RGB32(const void *src, const void *buffer,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void Blend_YUV32(const void *src, const void *buffer,void *dst,const int32_t w,const int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);

};


bool JPSDR_BlurMotion::Init()
{
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);
	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));

	return(true);
}


uint32 JPSDR_BlurMotion::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;

	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :*/
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :*/
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
/*		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_VDXA_RGB :
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

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	fa->dst.offset = fa->src.offset;
	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
}


bool JPSDR_BlurMotion::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	prefetcher->PrefetchFrame(0,frame,0);
	prefetcher->PrefetchFrame(0,frame-1,0);
	prefetcher->PrefetchFrame(0,frame+1,0);

	return(true);
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


void JPSDR_BlurMotion::Blend_RGB32(const void *_src,const void *_buffer,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_modulo, ptrdiff_t dst_modulo)
{
	const RGB32BMP *src,*buffer;
	RGB32BMP *dst;

	src=(RGB32BMP*)_src;
	buffer=(RGB32BMP*)_buffer;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->b=(uint8_t)(((uint16_t)src->b+(uint16_t)buffer->b)>>1);
			dst->g=(uint8_t)(((uint16_t)src->g+(uint16_t)buffer->g)>>1);
			dst->r=(uint8_t)(((uint16_t)src->r+(uint16_t)buffer->r)>>1);
			dst->alpha=buffer->alpha;
			dst++;
			buffer++;
			src++;
		}
		src = (RGB32BMP *)((uint8_t *)src + src_modulo);
		dst = (RGB32BMP *)((uint8_t *)dst + dst_modulo);
	}

}


void JPSDR_BlurMotion::Blend_YUV32(const void *_src,const void *_buffer,void *_dst,const int32_t w,const int32_t h,
	ptrdiff_t src_modulo, ptrdiff_t dst_modulo)
{
	const YUYV *src,*buffer;
	YUYV *dst;

	src=(YUYV*)_src;
	buffer=(YUYV*)_buffer;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->y1=(uint8_t)(((uint16_t)src->y1+(uint16_t)buffer->y1)>>1);
			dst->u=(uint8_t)(((uint16_t)src->u+(uint16_t)buffer->u)>>1);
			dst->y2=(uint8_t)(((uint16_t)src->y2+(uint16_t)buffer->y2)>>1);
			dst->v=(uint8_t)(((uint16_t)src->v+(uint16_t)buffer->v)>>1);
			dst++;
			buffer++;
			src++;
		}
		src = (YUYV *)((uint8_t *)src + src_modulo);
		dst = (YUYV *)((uint8_t *)dst + dst_modulo);
	}

}



void JPSDR_BlurMotion::Run()
{
	const VDXPixmapAlpha* tab_pxsrc[3]={
			(const VDXPixmapAlpha*)fa->mpSourceFrames[0]->mpPixmap,
			(const VDXPixmapAlpha*)fa->mpSourceFrames[1]->mpPixmap,
			(const VDXPixmapAlpha*)fa->mpSourceFrames[2]->mpPixmap
	};
	const VDXPixmapAlpha& pxdst =(const VDXPixmapAlpha&)*fa->mpOutputFrames[0]->mpPixmap;

	int32_t w,h,i,j,w_test;
	const void *src;
	void *s,*dst;
	uint32_t *s_32,*src_32;
	ptrdiff_t src_pitch,dst_pitch,src_modulo,dst_modulo;
	bool same_picture;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	image_data.src_plane0=tab_pxsrc[0]->data;
	image_data.src_plane1=tab_pxsrc[0]->data2;
	image_data.src_plane2=tab_pxsrc[0]->data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	w=image_data.src_w0;
	h=image_data.src_h0;
	src = image_data.src_plane0;
    dst = image_data.dst_plane0;
	src_pitch=image_data.src_pitch0;
	dst_pitch=image_data.dst_pitch0;
	src_modulo=image_data.src_modulo0;
	dst_modulo=image_data.dst_modulo0;
	s=tab_pxsrc[2]->data;

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			w_test=w;
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			w_test=(w+1)>>1;
			break;
	}
	
	src_32=(uint32_t *)src;
	s_32=(uint32_t *)tab_pxsrc[1]->data;
	i=0;
	do
	{
		j=0;
		do
		{
			same_picture=(src_32[j]==s_32[j]);
			j+=4;
		}
		while (same_picture && (j<w_test));
		i+=4;
        s_32 = (uint32_t *)((uint8_t *)s_32 + (src_pitch << 2));
        src_32 = (uint32_t *)((uint8_t *)src_32 + (src_pitch << 2));
	}
	while (same_picture && (i<h));

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			if (same_picture)
			{
				if (SSE2_Enable) JPSDR_BlurMotion_SSE_3_A(src,s,dst,(w+3)>>2,h,src_pitch,src_pitch,dst_pitch);
				else Blend_RGB32(src,s,dst,w,h,src_modulo,dst_modulo);
			}
			else Move_Full(src,dst,w<<2,h,src_pitch,dst_pitch);
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			if (same_picture)
			{
				if (SSE2_Enable) JPSDR_BlurMotion_SSE_3_A(src,s,dst,(w+7)>>3,h,src_pitch,src_pitch,dst_pitch);
				else Blend_YUV32(src,s,dst,(w+1)>>1,h,src_modulo,dst_modulo);
			}
			else Move_Full(src,dst,w<<1,h,src_pitch,dst_pitch);
			break;
	}
}


void JPSDR_BlurMotion::Start()
{
	if (g_VFVAPIVersion<14)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
	}

	SetImageData(image_data);
}


extern VDXFilterDefinition2 filterDef_JPSDR_BlurMotion=
VDXVideoFilterDefinition<JPSDR_BlurMotion>("JPSDR","Correct Frame Rate Blur v3.2.0",
	"Blur added frames after changing frame rate. [SSE2] Optimized.");

