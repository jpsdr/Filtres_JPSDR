/*
 *  Smart_Deinterlace
 *
 *  Deinterlace only on detected interlaced parts.
 *  Copyright (C) Donald A. Graft
 *	
 *  Smart_Deinterlace is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  Smart_Deinterlace is distributed in the hope that it will be useful,
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

#include "Smart_Deinterlace_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"


extern int g_VFVAPIVersion;


extern "C" void JPSDR_Smart_Deinterlace_Motion_Map_SSE_RGB32(const void *src,void *buffer,
	uint8_t *dst,int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,uint32_t thr,
	int32_t w_map);
extern "C" void JPSDR_Smart_Deinterlace_SSE_RGB32(const void *src,void *buffer,uint8_t *map,
	void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,int32_t w_map);

extern "C" void JPSDR_Smart_Deinterlace_Move32_Full(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);



class JPSDR_Smart_DeinterlaceData
{
public:
	uint8_t threshold;
	uint32_t frame_offset;
	uint8_t frame_freq;
	uint8_t filter_mode;
	bool disable_motion_map_filter;

	JPSDR_Smart_DeinterlaceData(void);
};

JPSDR_Smart_DeinterlaceData::JPSDR_Smart_DeinterlaceData(void)
{
	threshold=10;
	frame_offset=0;
	frame_freq=1;
	filter_mode=1;
	disable_motion_map_filter=false;
}


class JPSDR_Smart_DeinterlaceDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_Smart_DeinterlaceDialog(JPSDR_Smart_DeinterlaceData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_SMART_DEINTERLACE),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_Smart_DeinterlaceData& mData;
	JPSDR_Smart_DeinterlaceData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_Smart_DeinterlaceDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_Smart_DeinterlaceDialog::OnInit()
{
	moldData=mData;

	SetDlgItemInt(mhdlg,IDC_THRESHOLD,mData.threshold,FALSE);
	SetDlgItemInt(mhdlg,IDC_OFFSET,mData.frame_offset,FALSE);
	SetDlgItemInt(mhdlg,IDC_FREQ,mData.frame_freq,FALSE);

	return false;
}

void JPSDR_Smart_DeinterlaceDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_Smart_DeinterlaceDialog::SaveToData()
{
	uint8_t thrs;
	uint32_t u;
	BOOL success;

	thrs=GetDlgItemInt(mhdlg,IDC_THRESHOLD,&success,FALSE);
	if (success) mData.threshold=thrs;
	u=GetDlgItemInt(mhdlg,IDC_OFFSET,&success,FALSE);
	if (success) mData.frame_offset=u;
	thrs=GetDlgItemInt(mhdlg,IDC_FREQ,&success,FALSE);
	if ((success) && (thrs>0)) mData.frame_freq=thrs;
	
	return true;
}


bool JPSDR_Smart_DeinterlaceDialog::OnCommand(int cmd)
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
	}
		
	return false;
}



class JPSDR_Smart_Deinterlace : public JPSDRVDXVideoFilter
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
	Image_Data image_data;
	void *buffer_delta;
	uint8_t *buffer_map,*buffer_2;
	bool Integer_SSE_Enable,VDub2_Enable;

	void Deinterlace_Motion_MapRGB32(const void *src,void *buffer,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thres);
	void Motion_Map_Filter(uint8_t *map,uint8_t *buffer,int32_t w,
		int32_t h,int32_t w_map);
	void Smart_DeinterlaceRGB32(const void *src,void *dst,void *buffer,
		uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
		ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thr);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_Smart_DeinterlaceData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Smart_Deinterlace)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Smart_Deinterlace,ScriptConfig,"iiiii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_Smart_Deinterlace::Init()
{
	Integer_SSE_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_INTEGER_SSE)!=0);
	buffer_delta=NULL;
	buffer_map=NULL;
	buffer_2=NULL;

	return(true);
}



uint32 JPSDR_Smart_Deinterlace::GetParams()
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
/*		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
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

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);
}


bool JPSDR_Smart_Deinterlace::Configure(VDXHWND hwnd)
{
	JPSDR_Smart_DeinterlaceDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_Smart_Deinterlace::GetSettingString(char *buf, int maxlen)
{
	SafePrintf(buf,maxlen," Threshold : %d Offset : %d 1/N : %d",mData.threshold,mData.frame_offset,
		mData.frame_freq);
}



void JPSDR_Smart_Deinterlace::Deinterlace_Motion_MapRGB32(const void *src,void *_buffer,uint8_t *dst,
		int32_t w,int32_t h,ptrdiff_t pitch,ptrdiff_t modulo,int32_t w_map,uint8_t thres)
{
	int32_t delta_map;
	int16_t thr;
	const RGB32BMP *src1,*src2,*src3;
	RGB32BMP *buffer;

	thr=thres;
	delta_map=w_map-w;
	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+pitch);
	buffer=(RGB32BMP *)_buffer;

	for (int32_t i=0; i<w; i++)
	{
		uint16_t r1,r2,g1,g2,b1,b2;
		uint8_t alpha;

		b1=src1->b;
		g1=src1->g;
		r1=src1->r;
		alpha=src1->alpha;
		b2=src2->b;
		g2=src2->g;
		r2=src2->r;
		buffer->b=(uint8_t)((b1+b2)>>1);
		buffer->g=(uint8_t)((g1+g2)>>1);
		buffer->r=(uint8_t)((r1+r2)>>1);
		buffer->alpha=alpha;
		buffer++;
		*dst++=0;
	}
	src3=(RGB32BMP *)((uint8_t *)src2+pitch);
	for (int32_t i=0; i<h-2; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t r1,r2,r3,g1,g2,g3,b1,b2,b3;
			uint8_t alpha;

			b1=src1->b;
			g1=src1->g;
			r1=src1->r;
			b2=src2->b;
			g2=src2->g;
			r2=src2->r;
			alpha=src2->alpha;
			b3=src3->b;
			g3=src3->g;
			r3=src3->r;
			src1++;
			src2++;
			src3++;
			buffer->b=(uint8_t)((b2+b3)>>1);
			buffer->g=(uint8_t)((g2+g3)>>1);
			buffer->r=(uint8_t)((r2+r3)>>1);
			buffer->alpha=alpha;
			buffer++;
			if ( (!((g2>g1)^(g2>g3)) && ((abs(g2-g1)>thr) && (abs(g2-g3)>thr))) 
				|| (!((r2>r1)^(r2>r3)) && ((abs(r2-r1)>thr) && (abs(r2-r3)>thr)))
				|| (!((b2>b1)^(b2>b3)) && ((abs(b2-b1)>thr) && (abs(b2-b3)>thr))) ) *dst++=1;
			else *dst++=0;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+modulo);
		src2=(RGB32BMP *)((uint8_t *)src2+modulo);
		src3=(RGB32BMP *)((uint8_t *)src3+modulo);
		dst+=delta_map;
	}

	for (int32_t i=0; i<w; i++)
	{
		*buffer++=*src2++;
		*dst++=0;
	}

}




void JPSDR_Smart_Deinterlace::Motion_Map_Filter(uint8_t *map,uint8_t *buffer,int32_t w,
		int32_t h,int32_t w_map)
{
	int32_t delta0;
	uint8_t *tab;
	uint8_t *dst,*src;
	uint32_t *tab_2,w_map_4;
	const uint32_t mask=0x01010101;

	w_map_4=w_map >> 2;

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
	for (int32_t i=0; i<h-4; i++)
	{
		for (int32_t j=0; j<2; j++)
		{
			dst[j]=0;
			dst[w-1-j]=0;
		}
		for (int32_t j=2; j<w-2; j++)
		{
			if (src[j])
			{
				int32_t delta;
				uint8_t s;

				s=0;
				delta=0;
				for (int32_t k=0; k<5; k++)
				{
					for (int32_t l=j-2; l<=j+2; l++)
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
	delta0=h*w_map_4;
	for (int32_t i=0; i<delta0; i++)
		*tab_2++=0;
	tab=map;
	src=buffer+(w_map<<1);
	for (int32_t i=0; i<h-4; i++)
	{
		for (int32_t j=2; j<w-2; j++)
		{
			if (src[j])
			{
				tab_2=(uint32_t *)(tab+j-1);
				for (int32_t k=0; k<5; k++)
				{
					*tab_2=mask;
					tab_2+=w_map_4;
				}
			}
		}
		tab+=w_map;
		src+=w_map;
	}
}



void JPSDR_Smart_Deinterlace::Smart_DeinterlaceRGB32(const void *_src,void *_dst,void *_buffer,
	uint8_t *map,uint8_t *buffer_map,int32_t w,int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo,uint8_t thr)
{
	int32_t w_map,delta_map;
	uint32_t threshold;
	const RGB32BMP *src;
	RGB32BMP *dst,*buffer;

	src=(RGB32BMP *)_src;
	dst=(RGB32BMP *)_dst;
	buffer=(RGB32BMP *)_buffer;

	if ((w%4)==0) w_map=w+4;
	else w_map=(((w >> 2)+1)<< 2)+4;
	delta_map=w_map-w;

	if (Integer_SSE_Enable)
	{
		threshold=((uint32_t)thr<<16)|((uint32_t)thr<<8)|thr;
		JPSDR_Smart_Deinterlace_Motion_Map_SSE_RGB32(src,buffer,map,w,h-2,src_pitch,src_modulo,
			threshold,w_map);
	}
	else
		Deinterlace_Motion_MapRGB32(src,buffer,map,w,h,src_pitch,src_modulo,w_map,thr);

	Motion_Map_Filter(map,buffer_map,w,h,w_map);

	if (Integer_SSE_Enable)
		JPSDR_Smart_Deinterlace_SSE_RGB32(src,buffer,map,dst,w,h,src_pitch,dst_pitch,w_map);
	else
	{
		for (int32_t i=0; i<h; i++)
		{
			for (int32_t j=0; j<w; j++)
			{
				if (*map++) *dst++=*buffer;
				else *dst++=*src;
				buffer++;
				src++;
			}
			map+=delta_map;
			dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);
			src=(RGB32BMP *)((uint8_t *)src+src_modulo);
		}
	}

}


void JPSDR_Smart_Deinterlace::Run()
{
	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;
	const VDXFBitmap& pxsrc2 = *fa->mpSourceFrames[0];

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	if ((pxsrc2.mFrameNumber<mData.frame_offset) || (((pxsrc2.mFrameNumber-mData.frame_offset)%mData.frame_freq)!=0))
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
				JPSDR_Smart_Deinterlace_Move32_Full(image_data.src_plane0,image_data.dst_plane0,image_data.src_w0,image_data.src_h0,
					image_data.src_modulo0,image_data.dst_modulo0);
				break;
		}
	}
	else
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
				Smart_DeinterlaceRGB32(image_data.src_plane0,image_data.dst_plane0,buffer_delta,buffer_map,buffer_2,
					image_data.src_w0,image_data.src_h0,image_data.src_pitch0,image_data.dst_pitch0,image_data.src_modulo0,image_data.dst_modulo0,mData.threshold);
				break;
		}
	}
}



void JPSDR_Smart_Deinterlace::Start()
{
	int32_t w_map;
	bool test;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}
	
	SetImageData(image_data);

	if ((image_data.src_w0%4)==0) w_map=image_data.src_w0+4;
	else w_map=(((image_data.src_w0 >> 2)+1)<< 2)+4;

	buffer_delta=(void *)malloc(image_data.src_size0);
	buffer_map=(uint8_t *)malloc(image_data.src_h0*w_map*sizeof(uint8_t));
	buffer_2=(uint8_t *)malloc(image_data.src_h0*w_map*sizeof(uint8_t));

	test=((buffer_delta!=NULL) && (buffer_map!=NULL) && (buffer_2!=NULL));

	if (!test)
	{
		myfree(buffer_2);
		myfree(buffer_map);
		myfree(buffer_delta);
		ff->ExceptOutOfMemory();
		return;
	}
}




void JPSDR_Smart_Deinterlace::End()

{
	myfree(buffer_2);
	myfree(buffer_map);
	myfree(buffer_delta);
}




void JPSDR_Smart_Deinterlace::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.threshold=argv[0].asInt();
	mData.frame_offset=argv[1].asInt();
	mData.frame_freq=argv[2].asInt();
	mData.filter_mode=argv[3].asInt();
	mData.disable_motion_map_filter=!!argv[3].asInt();
}


void JPSDR_Smart_Deinterlace::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf,maxlen,"Config(%d, %d, %d, %d, %d)",mData.threshold,mData.frame_offset,
		mData.frame_freq,mData.filter_mode,mData.disable_motion_map_filter);
}


extern VDXFilterDefinition2 filterDef_JPSDR_Smart_Deinterlace=
VDXVideoFilterDefinition<JPSDR_Smart_Deinterlace>("JPSDR","SmartDeinterlace v2.4.1","Smart Deinterlace (Based D. Grafh). [MMX][SSE] Optimised.");


