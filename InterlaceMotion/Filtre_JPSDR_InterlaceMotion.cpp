#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"

extern int g_VFVAPIVersion;


class JPSDR_InterlaceMotion : public JPSDRVDXVideoFilter
{
public:
	virtual bool Init();
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual void End();

protected:
	Image_Data image_data;
	void *mem;
	uint32_t nbre_images;
	bool VDub2_Enable;
};


bool JPSDR_InterlaceMotion::Init()
{
	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));
	mem=NULL;
	return(true);
}


uint32 JPSDR_InterlaceMotion::GetParams()
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

	fa->dst.offset = fa->src.offset;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
}


static inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
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


void JPSDR_InterlaceMotion::Run()
{
	int32_t k,w_test;
	const void *src;
	const uint32_t *src_32,*s_32;
	bool same_picture;
	void *s,*dst;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	const int32_t w=image_data.src_w0,h=image_data.src_h0;
	const ptrdiff_t src_pitch=image_data.src_pitch0,dst_pitch=image_data.dst_pitch0,buffer_pitch=image_data.src_pitch0_al;
	const uint32_t line0=image_data.src_line0;

	src = image_data.src_plane0;
    dst = image_data.dst_plane0;
	s= mem;

	if (nbre_images==0)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
				Move_Full(src,s,line0,h,src_pitch,buffer_pitch);
				Move_Full(s,dst,line0,h,buffer_pitch,dst_pitch);
				break;
		}
		nbre_images++;
		return;
	}
	
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
	s_32=(uint32_t *)s;

	k=0;
	do
	{
		int32_t j=0;
		do
		{
			same_picture=(src_32[j]==s_32[j]);
			j+=4;
		}
		while (same_picture && (j<w_test));
		k+=4;
        s_32 = (uint32_t *)((uint8_t *)s_32 + (buffer_pitch << 2));
        src_32 = (uint32_t *)((uint8_t *)src_32 + (src_pitch << 2));
	}
	while (same_picture && (k<h));

	if (!same_picture)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
				Move_Full(src,dst,line0,h,src_pitch,dst_pitch);
				break;
		}
		nbre_images++;
		return;
	}

	for (int32_t i=0; i<h; i+=2)
	{
		memcpy(dst,s,line0);
		s = (void *)((uint8_t *)s + (buffer_pitch << 1));
		dst = (void *)((uint8_t *)dst + (dst_pitch << 1));
	}
	dst=(void *)((uint8_t *)image_data.dst_plane0 + dst_pitch);
	src=(void *)((uint8_t *)src + src_pitch);
	for (int32_t i=1; i<h; i+=2)
	{
		memcpy(dst,src,line0);
		src = (void *)((uint8_t *)src + (src_pitch << 1));
		dst = (void *)((uint8_t *)dst + (dst_pitch << 1));
	}

	src = image_data.src_plane0;
	s= mem;
	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
		case VMODE_YUYV :
		case VMODE_UYVY :
			Move_Full(src,s,line0,h,src_pitch,buffer_pitch);
			break;
	}
	nbre_images++;
}



void JPSDR_InterlaceMotion::Start()
{
	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}

	SetImageData(image_data);

	mem=(void *)malloc(image_data.src_size0_al);
	if (mem==NULL)
	{
		ff->ExceptOutOfMemory();
		return;
	}
	nbre_images=0;
}



void JPSDR_InterlaceMotion::End()

{
	myfree(mem);
}


extern VDXFilterDefinition2 filterDef_JPSDR_InterlaceMotion=
VDXVideoFilterDefinition<JPSDR_InterlaceMotion>("JPSDR","InterlaceMotion v2.4.0",
	"Interlace inter frame after x2 frame rate.");
