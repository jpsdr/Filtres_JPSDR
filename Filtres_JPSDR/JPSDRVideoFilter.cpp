//	VDXFrame - Helper library for VirtualDub plugins
//	Copyright (C) 2008 Avery Lee
//
//	The plugin headers in the VirtualDub plugin SDK are licensed differently
//	differently than VirtualDub and the Plugin SDK themselves.  This
//	particular file is thus licensed as follows (the "zlib" license):
//
//	This software is provided 'as-is', without any express or implied
//	warranty.  In no event will the authors be held liable for any
//	damages arising from the use of this software.
//
//	Permission is granted to anyone to use this software for any purpose,
//	including commercial applications, and to alter it and redistribute it
//	freely, subject to the following restrictions:
//
//	1.	The origin of this software must not be misrepresented; you must
//		not claim that you wrote the original software. If you use this
//		software in a product, an acknowledgment in the product
//		documentation would be appreciated but is not required.
//	2.	Altered source versions must be plainly marked as such, and must
//		not be misrepresented as being the original software.
//	3.	This notice may not be removed or altered from any source
//		distribution.

#include "JPSDRVideoFilter.h"


uint8_t JPSDRVDXVideoFilter::GetVideoMode(sint32 format)
{
	switch(format)
	{
		// BMP RGB
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
			return(VMODE_BMP_RGBA); break;
		// YUY2
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_YUV422_YU64 :
			return(VMODE_YUYV); break;
		// UYVY
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
			return(VMODE_UYVY); break;
		// Planar YUV 4:4:4
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar16 :
			return(VMODE_PLANAR_YUV444); break;
		// Planar YUV 4:2:2
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar16 :
			return(VMODE_PLANAR_YUV422); break;
		// Planar YUV 4:2:0
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_YUV420_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar16 :
			return(VMODE_PLANAR_YUV420); break;
		// Planar YUV 4:1:1
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
			return(VMODE_PLANAR_YUV411); break;
		// Planar YUV 4:1:0
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
			return(VMODE_PLANAR_YUV410); break;
		// Planar Y
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_Y16 :
			return(VMODE_Y); break;
		// Planar RGB & RGBA VDub2
		//  VDub2
		case nsVDXPixmap::kPixFormat_RGB_Planar :
		case nsVDXPixmap::kPixFormat_RGB_Planar16 :
		case nsVDXPixmap::kPixFormat_RGB_Planar32F :
		case nsVDXPixmap::kPixFormat_RGBA_Planar :
		case nsVDXPixmap::kPixFormat_RGBA_Planar16 :
		case nsVDXPixmap::kPixFormat_RGBA_Planar32F :
			return(VMODE_PLANAR_RGB); break;
		default : return(VMODE_NONE); break;
	}
}


uint8_t JPSDRVDXVideoFilter::GetColorMode(sint32 format)
{
	switch(format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_Y8 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_Y16 :
		case nsVDXPixmap::kPixFormat_RGB_Planar :
		case nsVDXPixmap::kPixFormat_RGB_Planar16 :
		case nsVDXPixmap::kPixFormat_RGB_Planar32F :
		case nsVDXPixmap::kPixFormat_RGBA_Planar :
		case nsVDXPixmap::kPixFormat_RGBA_Planar16 :
		case nsVDXPixmap::kPixFormat_RGBA_Planar32F :
			return(MATRIX_NONE); break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
			return(MATRIX_BT601); break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
			return(MATRIX_BT709); break;
		default : return(MATRIX_NONE); break;
	}
}


uint8_t JPSDRVDXVideoFilter::GetInterlaceMode(sint32 format)
{
	switch(format)
	{
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
			return(IMODE_INTERLACED); break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			return(IMODE_TFF); break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
			return(IMODE_BFF); break;
		default : return(IMODE_PROGRESSIVE); break;
	}
}


uint8_t JPSDRVDXVideoFilter::GetBitDepth(sint32 format)
{
	switch(format)
	{
		case nsVDXPixmap::kPixFormat_YUV422_V210 :
		// VDub2
		case nsVDXPixmap::kPixFormat_YUV444_V410 :
		case nsVDXPixmap::kPixFormat_YUV444_Y410 :
		case nsVDXPixmap::kPixFormat_R210 :
		case nsVDXPixmap::kPixFormat_R10K :
		case nsVDXPixmap::kPixFormat_YUV422_P210 :
		case nsVDXPixmap::kPixFormat_YUV420_P010 :
			return(10); break;
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar16 :
		case nsVDXPixmap::kPixFormat_Y16 :
		case nsVDXPixmap::kPixFormat_YUV444_Y416 :
		case nsVDXPixmap::kPixFormat_YUV422_P216 :
		case nsVDXPixmap::kPixFormat_YUV420_P016 :
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_YU64 :
			return(16); break;
		// VDub2
		case nsVDXPixmap::kPixFormat_RGB_Planar32F :
		case nsVDXPixmap::kPixFormat_RGBA_Planar32F :
			return(32); break;
		default : return(8); break;
	}
}


bool JPSDRVDXVideoFilter::CheckFullRangeMode(sint32 format)
{
	switch(format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		// VDub2
		case nsVDXPixmap::kPixFormat_XRGB64 :
		case nsVDXPixmap::kPixFormat_RGB_Planar :
		case nsVDXPixmap::kPixFormat_RGB_Planar16 :
		case nsVDXPixmap::kPixFormat_RGB_Planar32F :
		case nsVDXPixmap::kPixFormat_RGBA_Planar :
		case nsVDXPixmap::kPixFormat_RGBA_Planar16 :
		case nsVDXPixmap::kPixFormat_RGBA_Planar32F :
			return(true); break;
		default : return(false); break;
	}
}


bool JPSDRVDXVideoFilter::CheckAlphaPlanarMode(sint32 format)
{
	switch(format)
	{
		// VDub2
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar :
		case nsVDXPixmap::kPixFormat_YUV444_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV422_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_YUV420_Alpha_Planar16 :
		case nsVDXPixmap::kPixFormat_RGBA_Planar :
		case nsVDXPixmap::kPixFormat_RGBA_Planar16 :
		case nsVDXPixmap::kPixFormat_RGBA_Planar32F :
			return(true); break;
		default : return(false); break;
	}
}


uint8_t JPSDRVDXVideoFilter::GetColorMode(const VDXFBitmap* bitmap)
{
	uint8_t ColorMode;

	if ((fma!=NULL) && (fma->fmpixmap!=NULL))
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo(bitmap->mpPixmap);

		switch(info->colorSpaceMode)
		{
			case nsVDXPixmap::kColorSpaceMode_601 : ColorMode=MATRIX_BT601; break;
			case nsVDXPixmap::kColorSpaceMode_709 : ColorMode=MATRIX_BT709; break;
			default : ColorMode=GetColorMode(bitmap->mpPixmapLayout->format); break;
		}
	}
	else ColorMode=GetColorMode(bitmap->mpPixmapLayout->format);

	return(ColorMode);
}


uint8_t JPSDRVDXVideoFilter::GetInterlaceMode(const VDXFBitmap* bitmap)
{/*
	if ((fma!=NULL) && (fma->fmpixmap!=NULL))
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo(bitmap->mpPixmap);

		switch(info->colorRangeMode)
		{
			case nsVDXPixmap::kColorSpaceMode_601 : return(MATRIX_BT601); break;
			case nsVDXPixmap::kColorSpaceMode_709 : return(MATRIX_BT709); break;
			default : break;
		}
	}*/

	return(GetInterlaceMode(bitmap->mpPixmapLayout->format));
}


uint8_t JPSDRVDXVideoFilter::GetBitDepth(const VDXFBitmap* bitmap)
{
	uint8_t BitDepth;

	BitDepth=GetBitDepth(bitmap->mpPixmapLayout->format);
	if (BitDepth!=32)
	{
		if ((fma!=NULL) && (fma->fmpixmap!=NULL))
		{
			FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo(bitmap->mpPixmap);

			if ((info->ref_r!=0) && (info->ref_g!=0) && (info->ref_b!=0) && (info->ref_a!=0))
			{
				uint32 max=info->ref_r;

				if (max<info->ref_g) max=info->ref_g;
				if (max<info->ref_b) max=info->ref_b;
				if (max<info->ref_a) max=info->ref_a;

				if (max>=0x0100)
				{
					BitDepth=10;
					if (max>=0x0400) BitDepth=12;
					if (max>=0x1000) BitDepth=14;
					if (max>=0x4000) BitDepth=16;
				}
				else BitDepth=8;
			}
		}
	}

	return(BitDepth);
}


bool JPSDRVDXVideoFilter::CheckFullRangeMode(const VDXFBitmap* bitmap)
{
	bool FullRange;

	if ((fma!=NULL) && (fma->fmpixmap!=NULL))
	{
		FilterModPixmapInfo* info = fma->fmpixmap->GetPixmapInfo(bitmap->mpPixmap);

		switch(info->colorRangeMode)
		{
			case nsVDXPixmap::kColorRangeMode_Limited : FullRange=false; break;
			case nsVDXPixmap::kColorRangeMode_Full : FullRange=true; break;
			default : FullRange=CheckFullRangeMode(bitmap->mpPixmapLayout->format); break;
		}
	}
	else FullRange=CheckFullRangeMode(bitmap->mpPixmapLayout->format);

	return(FullRange);
}


void JPSDRVDXVideoFilter::SetImageData(Image_Data& iData,bool modeBMP)
{
	const VDXPixmapLayoutAlpha& pxsrc=(VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	const VDXPixmapLayoutAlpha& pxdst=(VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;

	iData.src_video_mode=GetVideoMode(pxsrc.format);
	iData.video_mode=iData.src_video_mode;
	iData.src_full_mode=CheckFullRangeMode(&fa->src);
	iData.src_color_matrix=GetColorMode(&fa->src);
	iData.src_interlaced=GetInterlaceMode(&fa->src);
	iData.src_bits_pixel=GetBitDepth(&fa->src);
	iData.src_alpha_planar=CheckAlphaPlanarMode(pxsrc.format);
	iData.alpha_planar=iData.src_alpha_planar;

	iData.dst_video_mode=GetVideoMode(pxdst.format);
	iData.dst_full_mode=CheckFullRangeMode(&fa->dst);
	iData.dst_color_matrix=GetColorMode(&fa->dst);
	iData.dst_interlaced=GetInterlaceMode(&fa->dst);
	iData.dst_bits_pixel=GetBitDepth(&fa->dst);
	iData.dst_alpha_planar=CheckAlphaPlanarMode(pxdst.format);

	if (modeBMP)
	{
		iData.src_pitch0=-pxsrc.pitch;
		switch(iData.src_video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				iData.src_pitch1=0;
				iData.src_pitch2=0;
				break;
			default :
				iData.src_pitch1=-pxsrc.pitch2;
				iData.src_pitch2=-pxsrc.pitch3;
				break;
		}
	}
	else
	{
		iData.src_pitch0=pxsrc.pitch;
		switch(iData.src_video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				iData.src_pitch1=0;
				iData.src_pitch2=0;
				break;
			default :
				iData.src_pitch1=pxsrc.pitch2;
				iData.src_pitch2=pxsrc.pitch3;
				break;
		}
	}

	if (modeBMP)
	{
		iData.dst_pitch0=-pxdst.pitch;
		switch(iData.dst_video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				iData.dst_pitch1=0;
				iData.dst_pitch2=0;
				break;
			default :
				iData.dst_pitch1=-pxdst.pitch2;
				iData.dst_pitch2=-pxdst.pitch3;
				break;
		}
	}
	else
	{
		iData.dst_pitch0=pxdst.pitch;
		switch(iData.dst_video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				iData.dst_pitch1=0;
				iData.dst_pitch2=0;
				break;
			default :
				iData.dst_pitch1=pxdst.pitch2;
				iData.dst_pitch2=pxdst.pitch3;
				break;
		}
	}

	if (iData.src_alpha_planar)
	{
		if (modeBMP) iData.src_pitch3=-pxsrc.pitch4;
		else iData.src_pitch3=pxsrc.pitch4;
	}
	else iData.src_pitch3=0;

	if (iData.dst_alpha_planar)
	{
		if (modeBMP) iData.dst_pitch3=-pxdst.pitch4;
		else iData.dst_pitch3=pxdst.pitch4;
	}
	else iData.dst_pitch3=0;

	iData.src_h0=pxsrc.h;
	iData.src_w0=pxsrc.w;
	switch(iData.src_video_mode)
	{
		case VMODE_BMP_RGBA :
			iData.src_line0=(iData.src_bits_pixel==8) ? 4*iData.src_w0 : 8*iData.src_w0;
			iData.src_h1=0;
			iData.src_h2=0;
			iData.src_h3=0;
			iData.src_w1=0;
			iData.src_w2=0;
			iData.src_w3=0;
			iData.src_line1=0;
			iData.src_line2=0;
			iData.src_line3=0;
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			iData.src_line0=(iData.src_bits_pixel==8) ? 4*((iData.src_w0+1)>>1) : 8*((iData.src_w0+1)>>1);
			iData.src_h1=0;
			iData.src_h2=0;
			iData.src_h3=0;
			iData.src_w1=0;
			iData.src_w2=0;
			iData.src_w3=0;
			iData.src_line1=0;
			iData.src_line2=0;
			iData.src_line3=0;
			break;
		case VMODE_PLANAR_RGB :
			iData.src_h1=iData.src_h0;
			iData.src_h2=iData.src_h0;
			iData.src_w1=iData.src_w0;
			iData.src_w2=iData.src_w0;
			switch (iData.src_bits_pixel)
			{
				case 8 :
					iData.src_line0=iData.src_w0;
					iData.src_line1=iData.src_w1;
					iData.src_line2=iData.src_w2;
					break;
				case 16 :
					iData.src_line0=2*iData.src_w0;
					iData.src_line1=2*iData.src_w1;
					iData.src_line2=2*iData.src_w2;
					break;
				case 32 :
					iData.src_line0=4*iData.src_w0;
					iData.src_line1=4*iData.src_w1;
					iData.src_line2=4*iData.src_w2;
					break;
				default :
					iData.src_line0=iData.src_w0;
					iData.src_line1=iData.src_w1;
					iData.src_line2=iData.src_w2;
					break;
			}
			if (iData.src_alpha_planar)
			{
				iData.src_h3=iData.src_h0;
				iData.src_w3=iData.src_w0;
				switch (iData.src_bits_pixel)
				{
					case 8 : iData.src_line3=iData.src_w3; break;
					case 16 : iData.src_line3=2*iData.src_w3; break;
					case 32 : iData.src_line3=4*iData.src_w3; break;
					default : iData.src_line3=iData.src_w3; break;
				}
			}
			else
			{
				iData.src_h3=0;
				iData.src_w3=0;
				iData.src_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV444 :
			iData.src_h1=iData.src_h0;
			iData.src_h2=iData.src_h0;
			iData.src_w1=iData.src_w0;
			iData.src_w2=iData.src_w0;
			iData.src_line0=(iData.src_bits_pixel==8) ? iData.src_w0 : 2*iData.src_w0;
			iData.src_line1=(iData.src_bits_pixel==8) ? iData.src_w1 : 2*iData.src_w1;
			iData.src_line2=(iData.src_bits_pixel==8) ? iData.src_w2 : 2*iData.src_w2;
			if (iData.src_alpha_planar)
			{
				iData.src_h3=iData.src_h0;
				iData.src_w3=iData.src_w0;
				iData.src_line3=(iData.src_bits_pixel==8) ? iData.src_w3 : 2*iData.src_w3;
			}
			else
			{
				iData.src_h3=0;
				iData.src_w3=0;
				iData.src_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV422 :
			iData.src_h1=iData.src_h0;
			iData.src_h2=iData.src_h0;
			iData.src_w1=(iData.src_w0+1)>>1;
			iData.src_w2=(iData.src_w0+1)>>1;
			iData.src_line0=(iData.src_bits_pixel==8) ? iData.src_w0 : 2*iData.src_w0;
			iData.src_line1=(iData.src_bits_pixel==8) ? iData.src_w1 : 2*iData.src_w1;
			iData.src_line2=(iData.src_bits_pixel==8) ? iData.src_w2 : 2*iData.src_w2;
			if (iData.src_alpha_planar)
			{
				iData.src_h3=iData.src_h0;
				iData.src_w3=iData.src_w0;
				iData.src_line3=(iData.src_bits_pixel==8) ? iData.src_w3 : 2*iData.src_w3;
			}
			else
			{
				iData.src_h3=0;
				iData.src_w3=0;
				iData.src_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV420 :
			iData.src_h1=(iData.src_h0+1)>>1;
			iData.src_h2=(iData.src_h0+1)>>1;
			iData.src_w1=(iData.src_w0+1)>>1;
			iData.src_w2=(iData.src_w0+1)>>1;
			iData.src_line0=(iData.src_bits_pixel==8) ? iData.src_w0 : 2*iData.src_w0;
			iData.src_line1=(iData.src_bits_pixel==8) ? iData.src_w1 : 2*iData.src_w1;
			iData.src_line2=(iData.src_bits_pixel==8) ? iData.src_w2 : 2*iData.src_w2;
			if (iData.src_alpha_planar)
			{
				iData.src_h3=iData.src_h0;
				iData.src_w3=iData.src_w0;
				iData.src_line3=(iData.src_bits_pixel==8) ? iData.src_w3 : 2*iData.src_w3;
			}
			else
			{
				iData.src_h3=0;
				iData.src_w3=0;
				iData.src_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV411 :
			iData.src_h1=iData.src_h0;
			iData.src_h2=iData.src_h0;
			iData.src_w1=(iData.src_w0+3)>>2;
			iData.src_w2=(iData.src_w0+3)>>2;
			iData.src_line0=(iData.src_bits_pixel==8) ? iData.src_w0 : 2*iData.src_w0;
			iData.src_line1=(iData.src_bits_pixel==8) ? iData.src_w1 : 2*iData.src_w1;
			iData.src_line2=(iData.src_bits_pixel==8) ? iData.src_w2 : 2*iData.src_w2;
			if (iData.src_alpha_planar)
			{
				iData.src_h3=iData.src_h0;
				iData.src_w3=iData.src_w0;
				iData.src_line3=(iData.src_bits_pixel==8) ? iData.src_w3 : 2*iData.src_w3;
			}
			else
			{
				iData.src_h3=0;
				iData.src_w3=0;
				iData.src_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV410 :
			iData.src_h1=(iData.src_h0+3)>>2;
			iData.src_h2=(iData.src_h0+3)>>2;
			iData.src_w1=(iData.src_w0+3)>>2;
			iData.src_w2=(iData.src_w0+3)>>2;
			iData.src_line0=(iData.src_bits_pixel==8) ? iData.src_w0 : 2*iData.src_w0;
			iData.src_line1=(iData.src_bits_pixel==8) ? iData.src_w1 : 2*iData.src_w1;
			iData.src_line2=(iData.src_bits_pixel==8) ? iData.src_w2 : 2*iData.src_w2;
			if (iData.src_alpha_planar)
			{
				iData.src_h3=iData.src_h0;
				iData.src_w3=iData.src_w0;
				iData.src_line3=(iData.src_bits_pixel==8) ? iData.src_w3 : 2*iData.src_w3;
			}
			else
			{
				iData.src_h3=0;
				iData.src_w3=0;
				iData.src_line3=0;
			}
			break;
		case VMODE_Y :
			iData.src_line0=(iData.src_bits_pixel==8) ? iData.src_w0 : 2*iData.src_w0;
			iData.src_h1=0;
			iData.src_h2=0;
			iData.src_h3=0;
			iData.src_w1=0;
			iData.src_w2=0;
			iData.src_w3=0;
			iData.src_line1=0;
			iData.src_line2=0;
			iData.src_line3=0;
			break;
	}

	iData.src_modulo0=iData.src_pitch0-iData.src_line0;
	iData.src_pitch0_al=(((ptrdiff_t)iData.src_line0+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	iData.src_modulo0_al=iData.src_pitch0_al-iData.src_line0;
	iData.src_size0=(size_t)iData.src_h0*(size_t)iData.src_line0;
	iData.src_size0_al=(size_t)iData.src_h0*(size_t)iData.src_pitch0_al;
	iData.src_modulo1=iData.src_pitch1-iData.src_line1;
	iData.src_pitch1_al=(((ptrdiff_t)iData.src_line1+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	iData.src_modulo1_al=iData.src_pitch1_al-iData.src_line1;
	iData.src_size1=(size_t)iData.src_h1*(size_t)iData.src_line1;
	iData.src_size1_al=(size_t)iData.src_h1*(size_t)iData.src_pitch1_al;
	iData.src_modulo2=iData.src_pitch2-iData.src_line2;
	iData.src_pitch2_al=(((ptrdiff_t)iData.src_line2+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	iData.src_modulo2_al=iData.src_pitch2_al-iData.src_line2;
	iData.src_size2=(size_t)iData.src_h2*(size_t)iData.src_line2;
	iData.src_size2_al=(size_t)iData.src_h2*(size_t)iData.src_pitch2_al;
	if (iData.src_alpha_planar)
	{
		iData.src_modulo3=iData.src_pitch3-iData.src_line3;
		iData.src_pitch3_al=(((ptrdiff_t)iData.src_line3+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		iData.src_modulo3_al=iData.src_pitch3_al-iData.src_line3;
		iData.src_size3=(size_t)iData.src_h3*(size_t)iData.src_line3;
		iData.src_size3_al=(size_t)iData.src_h3*(size_t)iData.src_pitch3_al;
	}
	else
	{
		iData.src_modulo3=0;
		iData.src_pitch3_al=0;
		iData.src_modulo3_al=0;
		iData.src_size3=0;
		iData.src_size3_al=0;
	}

	iData.dst_h0=pxdst.h;
	iData.dst_w0=pxdst.w;
	switch(iData.dst_video_mode)
	{
		case VMODE_BMP_RGBA :
			iData.dst_line0=(iData.dst_bits_pixel==8) ? 4*iData.dst_w0 : 8*iData.dst_w0;
			iData.dst_h1=0;
			iData.dst_h2=0;
			iData.dst_h3=0;
			iData.dst_w1=0;
			iData.dst_w2=0;
			iData.dst_w3=0;
			iData.dst_line1=0;
			iData.dst_line2=0;
			iData.dst_line3=0;
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			iData.dst_line0=(iData.dst_bits_pixel==8) ? 4*((iData.dst_w0+1)>>1) : 8*((iData.dst_w0+1)>>1);
			iData.dst_h1=0;
			iData.dst_h2=0;
			iData.dst_h3=0;
			iData.dst_w1=0;
			iData.dst_w2=0;
			iData.dst_w3=0;
			iData.dst_line1=0;
			iData.dst_line2=0;
			iData.dst_line3=0;
			break;
		case VMODE_PLANAR_RGB :
			iData.dst_h1=iData.dst_h0;
			iData.dst_h2=iData.dst_h0;
			iData.dst_w1=iData.dst_w0;
			iData.dst_w2=iData.dst_w0;
			switch(iData.dst_bits_pixel)
			{
				case 8 :
					iData.dst_line0=iData.dst_w0;
					iData.dst_line1=iData.dst_w1;
					iData.dst_line2=iData.dst_w2;
					break;
				case 16 :
					iData.dst_line0=2*iData.dst_w0;
					iData.dst_line1=2*iData.dst_w1;
					iData.dst_line2=2*iData.dst_w2;
					break;
				case 32 :
					iData.dst_line0=4*iData.dst_w0;
					iData.dst_line1=4*iData.dst_w1;
					iData.dst_line2=4*iData.dst_w2;
					break;
				default :
					iData.dst_line0=iData.dst_w0;
					iData.dst_line1=iData.dst_w1;
					iData.dst_line2=iData.dst_w2;
					break;
			}
			if (iData.dst_alpha_planar)
			{
				iData.dst_h3=iData.dst_h0;
				iData.dst_w3=iData.dst_w0;
				switch(iData.dst_bits_pixel)
				{
					case 8 : iData.dst_line3=iData.dst_w3; break;
					case 16 : iData.dst_line3=2*iData.dst_w3; break;
					case 32 : iData.dst_line3=4*iData.dst_w3; break;
					default : iData.dst_line3=iData.dst_w3; break;
				}
			}
			else
			{
				iData.dst_h3=0;
				iData.dst_w3=0;
				iData.dst_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV444 :
			iData.dst_h1=iData.dst_h0;
			iData.dst_h2=iData.dst_h0;
			iData.dst_w1=iData.dst_w0;
			iData.dst_w2=iData.dst_w0;
			iData.dst_line0=(iData.dst_bits_pixel==8) ? iData.dst_w0 : 2*iData.dst_w0;
			iData.dst_line1=(iData.dst_bits_pixel==8) ? iData.dst_w1 : 2*iData.dst_w1;
			iData.dst_line2=(iData.dst_bits_pixel==8) ? iData.dst_w2 : 2*iData.dst_w2;
			if (iData.dst_alpha_planar)
			{
				iData.dst_h3=iData.dst_h0;
				iData.dst_w3=iData.dst_w0;
				iData.dst_line3=(iData.dst_bits_pixel==8) ? iData.dst_w3 : 2*iData.dst_w3;
			}
			else
			{
				iData.dst_h3=0;
				iData.dst_w3=0;
				iData.dst_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV422 :
			iData.dst_h1=iData.dst_h0;
			iData.dst_h2=iData.dst_h0;
			iData.dst_w1=(iData.dst_w0+1)>>1;
			iData.dst_w2=(iData.dst_w0+1)>>1;
			iData.dst_line0=(iData.dst_bits_pixel==8) ? iData.dst_w0 : 2*iData.dst_w0;
			iData.dst_line1=(iData.dst_bits_pixel==8) ? iData.dst_w1 : 2*iData.dst_w1;
			iData.dst_line2=(iData.dst_bits_pixel==8) ? iData.dst_w2 : 2*iData.dst_w2;
			if (iData.dst_alpha_planar)
			{
				iData.dst_h3=iData.dst_h0;
				iData.dst_w3=iData.dst_w0;
				iData.dst_line3=(iData.dst_bits_pixel==8) ? iData.dst_w3 : 2*iData.dst_w3;
			}
			else
			{
				iData.dst_h3=0;
				iData.dst_w3=0;
				iData.dst_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV420 :
			iData.dst_h1=(iData.dst_h0+1)>>1;
			iData.dst_h2=(iData.dst_h0+1)>>1;
			iData.dst_w1=(iData.dst_w0+1)>>1;
			iData.dst_w2=(iData.dst_w0+1)>>1;
			iData.dst_line0=(iData.dst_bits_pixel==8) ? iData.dst_w0 : 2*iData.dst_w0;
			iData.dst_line1=(iData.dst_bits_pixel==8) ? iData.dst_w1 : 2*iData.dst_w1;
			iData.dst_line2=(iData.dst_bits_pixel==8) ? iData.dst_w2 : 2*iData.dst_w2;
			if (iData.dst_alpha_planar)
			{
				iData.dst_h3=iData.dst_h0;
				iData.dst_w3=iData.dst_w0;
				iData.dst_line3=(iData.dst_bits_pixel==8) ? iData.dst_w3 : 2*iData.dst_w3;
			}
			else
			{
				iData.dst_h3=0;
				iData.dst_w3=0;
				iData.dst_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV411 :
			iData.dst_h1=iData.dst_h0;
			iData.dst_h2=iData.dst_h0;
			iData.dst_w1=(iData.dst_w0+3)>>2;
			iData.dst_w2=(iData.dst_w0+3)>>2;
			iData.dst_line0=(iData.dst_bits_pixel==8) ? iData.dst_w0 : 2*iData.dst_w0;
			iData.dst_line1=(iData.dst_bits_pixel==8) ? iData.dst_w1 : 2*iData.dst_w1;
			iData.dst_line2=(iData.dst_bits_pixel==8) ? iData.dst_w2 : 2*iData.dst_w2;
			if (iData.dst_alpha_planar)
			{
				iData.dst_h3=iData.dst_h0;
				iData.dst_w3=iData.dst_w0;
				iData.dst_line3=(iData.dst_bits_pixel==8) ? iData.dst_w3 : 2*iData.dst_w3;
			}
			else
			{
				iData.dst_h3=0;
				iData.dst_w3=0;
				iData.dst_line3=0;
			}
			break;
		case VMODE_PLANAR_YUV410 :
			iData.dst_h1=(iData.dst_h0+3)>>2;
			iData.dst_h2=(iData.dst_h0+3)>>2;
			iData.dst_w1=(iData.dst_w0+3)>>2;
			iData.dst_w2=(iData.dst_w0+3)>>2;
			iData.dst_line0=(iData.dst_bits_pixel==8) ? iData.dst_w0 : 2*iData.dst_w0;
			iData.dst_line1=(iData.dst_bits_pixel==8) ? iData.dst_w1 : 2*iData.dst_w1;
			iData.dst_line2=(iData.dst_bits_pixel==8) ? iData.dst_w2 : 2*iData.dst_w2;
			if (iData.dst_alpha_planar)
			{
				iData.dst_h3=iData.dst_h0;
				iData.dst_w3=iData.dst_w0;
				iData.dst_line3=(iData.dst_bits_pixel==8) ? iData.dst_w3 : 2*iData.dst_w3;
			}
			else
			{
				iData.dst_h3=0;
				iData.dst_w3=0;
				iData.dst_line3=0;
			}
			break;
		case VMODE_Y :
			iData.dst_line0=(iData.dst_bits_pixel==8) ? iData.dst_w0 : 2*iData.dst_w0;
			iData.dst_h1=0;
			iData.dst_h2=0;
			iData.dst_h3=0;
			iData.dst_w1=0;
			iData.dst_w2=0;
			iData.dst_w3=0;
			iData.dst_line1=0;
			iData.dst_line2=0;
			iData.dst_line3=0;
			break;
	}

	iData.dst_modulo0=iData.dst_pitch0-iData.dst_line0;
	iData.dst_pitch0_al=(((ptrdiff_t)iData.dst_line0+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	iData.dst_modulo0_al=iData.dst_pitch0_al-iData.dst_line0;
	iData.dst_size0=(size_t)iData.dst_h0*(size_t)iData.dst_line0;
	iData.dst_size0_al=(size_t)iData.dst_h0*(size_t)iData.dst_pitch0_al;
	iData.dst_modulo1=iData.dst_pitch1-iData.dst_line1;
	iData.dst_pitch1_al=(((ptrdiff_t)iData.dst_line1+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	iData.dst_modulo1_al=iData.dst_pitch1_al-iData.dst_line1;
	iData.dst_size1=(size_t)iData.dst_h1*(size_t)iData.dst_line1;
	iData.dst_size1_al=(size_t)iData.dst_h1*(size_t)iData.dst_pitch1_al;
	iData.dst_modulo2=iData.dst_pitch2-iData.dst_line2;
	iData.dst_pitch2_al=(((ptrdiff_t)iData.dst_line2+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	iData.dst_modulo2_al=iData.dst_pitch2_al-iData.dst_line2;
	iData.dst_size2=(size_t)iData.dst_h2*(size_t)iData.dst_line2;
	iData.dst_size2_al=(size_t)iData.dst_h2*(size_t)iData.dst_pitch2_al;
	if (iData.dst_alpha_planar)
	{
		iData.dst_modulo3=iData.dst_pitch3-iData.dst_line3;
		iData.dst_pitch3_al=(((ptrdiff_t)iData.dst_line3+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		iData.dst_modulo3_al=iData.dst_pitch3_al-iData.dst_line3;
		iData.dst_size3=(size_t)iData.dst_h3*(size_t)iData.dst_line3;
		iData.dst_size3_al=(size_t)iData.dst_h3*(size_t)iData.dst_pitch3_al;
	}
	else
	{
		iData.dst_modulo3=0;
		iData.dst_pitch3_al=0;
		iData.dst_modulo3_al=0;
		iData.dst_size3=0;
		iData.dst_size3_al=0;
	}

	if (iData.src_bits_pixel<=16)
	{
		if (iData.src_full_mode)
		{
			iData.src_Y_Min=0;
			iData.src_U_Min=0;
			iData.src_V_Min=0;

			iData.src_Y_Max=(uint16_t)(((int)1 << iData.src_bits_pixel)-1);
			iData.src_U_Max=(uint16_t)(((int)1 << iData.src_bits_pixel)-1);
			iData.src_V_Max=(uint16_t)(((int)1 << iData.src_bits_pixel)-1);
		}
		else
		{
			iData.src_Y_Min=(uint16_t)((int)16 << (iData.src_bits_pixel-8));
			iData.src_U_Min=(uint16_t)((int)16 << (iData.src_bits_pixel-8));
			iData.src_V_Min=(uint16_t)((int)16 << (iData.src_bits_pixel-8));
			iData.src_Y_Max=(uint16_t)((int)235 << (iData.src_bits_pixel-8));
			iData.src_U_Max=(uint16_t)((int)240 << (iData.src_bits_pixel-8));
			iData.src_V_Max=(uint16_t)((int)240 << (iData.src_bits_pixel-8));
		}
	}
	if (iData.dst_bits_pixel<=16)
	{
		if (iData.dst_full_mode)
		{
			iData.dst_Y_Min=0;
			iData.dst_U_Min=0;
			iData.dst_V_Min=0;

			iData.dst_Y_Max=(uint16_t)(((int)1 << iData.dst_bits_pixel)-1);
			iData.dst_U_Max=(uint16_t)(((int)1 << iData.dst_bits_pixel)-1);
			iData.dst_V_Max=(uint16_t)(((int)1 << iData.dst_bits_pixel)-1);
		}
		else
		{
			iData.dst_Y_Min=(uint16_t)((int)16 << (iData.dst_bits_pixel-8));
			iData.dst_U_Min=(uint16_t)((int)16 << (iData.dst_bits_pixel-8));
			iData.dst_V_Min=(uint16_t)((int)16 << (iData.dst_bits_pixel-8));
			iData.dst_Y_Max=(uint16_t)((int)235 << (iData.dst_bits_pixel-8));
			iData.dst_U_Max=(uint16_t)((int)240 << (iData.dst_bits_pixel-8));
			iData.dst_V_Max=(uint16_t)((int)240 << (iData.dst_bits_pixel-8));
		}
	}

	switch(iData.src_color_matrix)
	{
		case MATRIX_BT601 :
			iData.src_Kr=0.299;
			iData.src_Kb=0.114;
			break;
		case MATRIX_BT709 :
			iData.src_Kr=0.2126;
			iData.src_Kb=0.0722;
			break;
		case MATRIX_BT2020 :
			iData.src_Kr=0.2627;
			iData.src_Kb=0.0593;
			break;
		default :
			iData.src_Kr=0.299;
			iData.src_Kb=0.114;
			break;
	}
	iData.src_Kg=1.0-iData.src_Kr-iData.src_Kb;
	if ((iData.src_full_mode) || (iData.src_bits_pixel>16))
	{
		iData.src_Coeff_Y=1.0;
		iData.src_Coeff_U=1.0;
		iData.src_Coeff_V=1.0;
	}
	else
	{
		iData.src_Coeff_Y=219.0/255.0;
		iData.src_Coeff_U=224.0/255.0;
		iData.src_Coeff_V=224.0/255.0;
	}
	iData.src_Ur=-0.5*iData.src_Kr/(1.0-iData.src_Kb);
	iData.src_Ug=-0.5*iData.src_Kg/(1.0-iData.src_Kb);
	iData.src_Ub=0.5;
	iData.src_Vr=0.5;
	iData.src_Vg=-0.5*iData.src_Kg/(1.0-iData.src_Kr);
	iData.src_Vb=-0.5*iData.src_Kb/(1.0-iData.src_Kr);
	iData.src_Rv=2.0*(1.0-iData.src_Kr);
	iData.src_Gu=-2.0*iData.src_Kb*(1.0-iData.src_Kb)/iData.src_Kg;
	iData.src_Gv=-2.0*iData.src_Kr*(1.0-iData.src_Kr)/iData.src_Kg;
	iData.src_Bu=2.0*(1.0-iData.src_Kb);
	if (iData.src_bits_pixel<=16)
	{
		iData.src_Off_Y=iData.src_Y_Min;
		iData.src_Off_U=(uint16_t)((int)128 << (iData.src_bits_pixel-8));
		iData.src_Off_V=(uint16_t)((int)128 << (iData.src_bits_pixel-8));
		iData.src_Off_R=-((double)iData.src_Off_Y/iData.src_Coeff_Y+iData.src_Rv*(double)iData.src_Off_V/iData.src_Coeff_V);
		iData.src_Off_G=-((double)iData.src_Off_Y/iData.src_Coeff_Y+iData.src_Gu*(double)iData.src_Off_U/iData.src_Coeff_U
			+iData.src_Gv*(double)iData.src_Off_V/iData.src_Coeff_V);
		iData.src_Off_B=-((double)iData.src_Off_Y/iData.src_Coeff_Y+iData.src_Bu*(double)iData.src_Off_U/iData.src_Coeff_U);
	}
	else
	{
		iData.src_Off_Y=0;
		iData.src_Off_U=0;
		iData.src_Off_V=0;
		iData.src_Coeff_Y=1.0;
		iData.src_Coeff_U=1.0;
		iData.src_Coeff_V=1.0;
		iData.src_Off_R=0.0;
		iData.src_Off_G=0.0;
		iData.src_Off_B=0.0;
	}

	switch(iData.dst_color_matrix)
	{
		case MATRIX_BT601 :
			iData.dst_Kr=0.299;
			iData.dst_Kb=0.114;
			break;
		case MATRIX_BT709 :
			iData.dst_Kr=0.2126;
			iData.dst_Kb=0.0722;
			break;
		case MATRIX_BT2020 :
			iData.dst_Kr=0.2627;
			iData.dst_Kb=0.0593;
			break;
		default :
			iData.dst_Kr=0.299;
			iData.dst_Kb=0.114;
			break;
	}
	iData.dst_Kg=1.0-iData.dst_Kr-iData.dst_Kb;
	if ((iData.dst_full_mode) || (iData.dst_bits_pixel>16))
	{
		iData.dst_Coeff_Y=1.0;
		iData.dst_Coeff_U=1.0;
		iData.dst_Coeff_V=1.0;
	}
	else
	{
		iData.dst_Coeff_Y=219.0/255.0;
		iData.dst_Coeff_U=224.0/255.0;
		iData.dst_Coeff_V=224.0/255.0;
	}
	iData.dst_Ur=-0.5*iData.dst_Kr/(1.0-iData.dst_Kb);
	iData.dst_Ug=-0.5*iData.dst_Kg/(1.0-iData.dst_Kb);
	iData.dst_Ub=0.5;
	iData.dst_Vr=0.5;
	iData.dst_Vg=-0.5*iData.dst_Kg/(1.0-iData.dst_Kr);
	iData.dst_Vb=-0.5*iData.dst_Kb/(1.0-iData.dst_Kr);
	iData.dst_Rv=2.0*(1.0-iData.dst_Kr);
	iData.dst_Gu=-2.0*iData.dst_Kb*(1.0-iData.dst_Kb)/iData.dst_Kg;
	iData.dst_Gv=-2.0*iData.dst_Kr*(1.0-iData.dst_Kr)/iData.dst_Kg;
	iData.dst_Bu=2.0*(1.0-iData.dst_Kb);
	if (iData.dst_bits_pixel<=16)
	{
		iData.dst_Off_Y=iData.dst_Y_Min;
		iData.dst_Off_U=(uint16_t)((int)128 << (iData.dst_bits_pixel-8));
		iData.dst_Off_V=(uint16_t)((int)128 << (iData.dst_bits_pixel-8));
		iData.dst_Off_R=-((double)iData.dst_Off_Y/iData.dst_Coeff_Y+iData.dst_Rv*(double)iData.dst_Off_V/iData.dst_Coeff_V);
		iData.dst_Off_G=-((double)iData.dst_Off_Y/iData.dst_Coeff_Y+iData.dst_Gu*(double)iData.dst_Off_U/iData.dst_Coeff_U
			+iData.dst_Gv*(double)iData.dst_Off_V/iData.dst_Coeff_V);
		iData.dst_Off_B=-((double)iData.dst_Off_Y/iData.dst_Coeff_Y+iData.dst_Bu*(double)iData.dst_Off_U/iData.dst_Coeff_U);
	}
	else
	{
		iData.dst_Off_Y=0;
		iData.dst_Off_U=0;
		iData.dst_Off_V=0;
		iData.dst_Coeff_Y=1.0;
		iData.dst_Coeff_U=1.0;
		iData.dst_Coeff_V=1.0;
		iData.dst_Off_R=0.0;
		iData.dst_Off_G=0.0;
		iData.dst_Off_B=0.0;
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

}