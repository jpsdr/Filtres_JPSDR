#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "DeinterlaceII_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Pascal.h"


extern int g_VFVAPIVersion;

extern "C" void JPSDR_DeinterlaceII_Move32_Full_src(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t src_modulo);
extern "C" void JPSDR_DeinterlaceII_Move32_Full_dst(const void *src, void *dst, int32_t w,int32_t h,
		ptrdiff_t dst_modulo);


class JPSDR_DeinterlaceIIData
{
public:
	bool sharp;
	uint32_t frame_offset;
	uint8_t frame_freq;

	JPSDR_DeinterlaceIIData(void);
};



JPSDR_DeinterlaceIIData::JPSDR_DeinterlaceIIData(void)
{
	frame_offset=0;
	frame_freq=1;
	sharp=false;
}





class JPSDR_DeinterlaceIIDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_DeinterlaceIIDialog(JPSDR_DeinterlaceIIData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_DEINTERLACEII),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_DeinterlaceIIData& mData;
	JPSDR_DeinterlaceIIData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_DeinterlaceIIDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_DeinterlaceIIDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_SHARP,mData.sharp?BST_CHECKED:BST_UNCHECKED);
	SetDlgItemInt(mhdlg,IDC_OFFSET,mData.frame_offset,FALSE);
	SetDlgItemInt(mhdlg,IDC_FREQ,mData.frame_freq,FALSE);

	return false;
}


void JPSDR_DeinterlaceIIDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_DeinterlaceIIDialog::SaveToData()
{
	uint8_t v;
	uint32_t u;
	BOOL success;

	u=GetDlgItemInt(mhdlg,IDC_OFFSET,&success,FALSE);
	if (success) mData.frame_offset=u;
	v=GetDlgItemInt(mhdlg,IDC_FREQ,&success,FALSE);
	if ((success) && (v>0)) mData.frame_freq=v;
	mData.sharp=!!IsDlgButtonChecked(mhdlg, IDC_SHARP);
	
	return true;
}



bool JPSDR_DeinterlaceIIDialog::OnCommand(int cmd)
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


class JPSDR_DeinterlaceII : public JPSDRVDXVideoFilter
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
	void *buffer_frame[2];
	uint8_t indice_frame;
	int16_t look_up[1280];
	uint32_t nbre_images;
	bool MMX_Enable,SSE_Integer_Enable,SSE2_Enable,VDub2_Enable;

	void Blend_UpRGB32(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void Blend_UpYUV32(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void Blend_DownRGB32(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void Blend_DownYUV32(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void Move32(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,ptrdiff_t dst_pitch,
		ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void AdvancedRGB32(const void *src0,const void *src1,const void *src2,void *dst,int32_t w,
	int32_t h,ptrdiff_t src_pitch,ptrdiff_t buffer_pitch,ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);
	void AdvancedYUV32(const void *src0,const void *src1,const void *src2,void *dst,int32_t w,
	int32_t h,ptrdiff_t src_pitch,ptrdiff_t buffer_pitch,ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_DeinterlaceIIData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_DeinterlaceII)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_DeinterlaceII,ScriptConfig,"iii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_DeinterlaceII::Init()
{
	MMX_Enable=ff->isMMXEnabled();
	SSE_Integer_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_INTEGER_SSE)!=0);
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	for (int16_t j=0; j<2; j++)
		buffer_frame[j]=NULL;

	for (int16_t j=0; j<=255; j++)
	{
		look_up[j]=(int16_t)round(64.0*0.526*j);
		look_up[j+256]=(int16_t)round(64.0*0.170*j);
		look_up[j+512]=(int16_t)round(64.0*-0.116*j);
		look_up[j+768]=(int16_t)round(64.0*0.031*j);
		look_up[j+1024]=(int16_t)round(64.0*-0.026*j);
	}

	return(true);
}
	


uint32 JPSDR_DeinterlaceII::GetParams()
{

	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;

	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));

	switch(fa->src.mpPixmapLayout->format)
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

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_HAS_LAG(1)|FILTERPARAM_ALIGN_SCANLINES);
}


void JPSDR_DeinterlaceII::GetSettingString(char *buf, int maxlen)
{
	if (mData.sharp)
		SafePrintf(buf,maxlen," Sharp : On Offset : %d 1/N : %d",mData.frame_offset,mData.frame_freq);
	else
		SafePrintf(buf,maxlen," Sharp : Off Offset : %d 1/N : %d",mData.frame_offset,mData.frame_freq);
}



void JPSDR_DeinterlaceII::Blend_UpRGB32(const void *src,void *_dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)
{
	const RGB32BMP *src1,*src2;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1 + src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->b=((uint16_t)src1->b+(uint16_t)src2->b)>>1;
			dst->g=((uint16_t)src1->g+(uint16_t)src2->g)>>1;
			dst->r=((uint16_t)src1->r+(uint16_t)src2->r)>>1;
			dst->alpha=src1->alpha;
			src1++;
			src2++;
			dst++;
         }
        src1 = (RGB32BMP *)((uint8_t *)src1 + src_modulo);
		src2 = (RGB32BMP *)((uint8_t *)src2 + src_modulo);
        dst = (RGB32BMP *)((uint8_t *)dst + dst_modulo);
	}
}



void JPSDR_DeinterlaceII::Blend_UpYUV32(const void *src,void *_dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)
{
	const YUYV *src1,*src2;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1 + src_pitch);
	dst=(YUYV *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->y1=((uint16_t)src1->y1+(uint16_t)src2->y1)>>1;
			dst->u=((uint16_t)src1->u+(uint16_t)src2->u)>>1;
			dst->y2=((uint16_t)src1->y2+(uint16_t)src2->y2)>>1;
			dst->v=((uint16_t)src1->v+(uint16_t)src2->v)>>1;
			src1++;
			src2++;
			dst++;
         }
        src1 = (YUYV *)((uint8_t *)src1 + src_modulo);
		src2 = (YUYV *)((uint8_t *)src2 + src_modulo);
        dst = (YUYV *)((uint8_t *)dst + dst_modulo);
	}
}




void JPSDR_DeinterlaceII::Blend_DownRGB32(const void *src,void *_dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)

{
	const RGB32BMP *src1,*src2;
	RGB32BMP *dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1 + src_pitch);
	dst=(RGB32BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->b=((uint16_t)src1->b+(uint16_t)src2->b)>>1;
			dst->g=((uint16_t)src1->g+(uint16_t)src2->g)>>1;
			dst->r=((uint16_t)src1->r+(uint16_t)src2->r)>>1;
			dst->alpha=src1->alpha;
			src1++;
			src2++;
			dst++;
         }
        src1 = (RGB32BMP *)((uint8_t *)src1 + src_modulo);
		src2 = (RGB32BMP *)((uint8_t *)src2 + src_modulo);
        dst = (RGB32BMP *)((uint8_t *)dst + dst_modulo);
	}
	memcpy(dst,src1,4*w);
}


void JPSDR_DeinterlaceII::Blend_DownYUV32(const void *src,void *_dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)
{
	const YUYV *src1,*src2;
	YUYV *dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1 + src_pitch);
	dst=(YUYV *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->y1=((uint16_t)src1->y1+(uint16_t)src2->y1)>>1;
			dst->u=((uint16_t)src1->u+(uint16_t)src2->u)>>1;
			dst->y2=((uint16_t)src1->y2+(uint16_t)src2->y2)>>1;
			dst->v=((uint16_t)src1->v+(uint16_t)src2->v)>>1;
			src1++;
			src2++;
			dst++;
         }
        src1 = (YUYV *)((uint8_t *)src1 + src_modulo);
		src2 = (YUYV *)((uint8_t *)src2 + src_modulo);
        dst = (YUYV *)((uint8_t *)dst + dst_modulo);
	}
	memcpy(dst,src1,4*w);
}


void JPSDR_DeinterlaceII::Move32(const void *src,void *dst,int32_t w,int32_t h,ptrdiff_t src_pitch,
	ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)
{
	sint32 i;

	for (i=0; i<h; i++)
	{
		memcpy(dst,src,4*w);
		src=(void *)((uint8_t *)src+(src_pitch << 1));
		dst=(void *)((uint8_t *)dst+(dst_pitch << 1));
	}
}

    

void JPSDR_DeinterlaceII::AdvancedRGB32(const void *src0,const void *src1,const void *src2,void *_dst,int32_t w,
	int32_t h,ptrdiff_t src_pitch,ptrdiff_t buffer_pitch,ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)
{
	const RGB32BMP *src0_a,*src0_b,*src0_c,*src1_a,*src1_b,*src2_a,*src2_b,*src2_c;
	RGB32BMP *dst;

	src0_a=(RGB32BMP *)src0;
	src0_b=(RGB32BMP *)((uint8_t *)src0 + (buffer_pitch << 1));
	src0_c=(RGB32BMP *)((uint8_t *)src0 + (buffer_pitch << 2));
	src1_a=(RGB32BMP *)src1;
	src1_b=(RGB32BMP *)((uint8_t *)src1 + (buffer_pitch << 1));
	src2_a=(RGB32BMP *)src2;
	src2_b=(RGB32BMP *)((uint8_t *)src2 + (src_pitch << 1));
	src2_c=(RGB32BMP *)((uint8_t *)src2 + (src_pitch << 2));
	dst=(RGB32BMP *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int32_t r,g,b;

			b=(((int32_t)src1_a->b)<<3) + (((int32_t)src1_b->b)<<3) +
				(((int32_t)src0_b->b)<<1) + (((int32_t)src2_b->b)<<1) + 8;
			b-=( (int32_t)src0_a->b + (int32_t)src0_c->b +
				(int32_t)src2_a->b + (int32_t)src2_c->b );
			if (b<0) b=0;
			else
			{
				b>>=4;
				if (b>255) b=255;
			}
			g=(((int32_t)src1_a->g)<<3) + (((int32_t)src1_b->g)<<3) +
				(((int32_t)src0_b->g)<<1) + (((int32_t)src2_b->g)<<1) + 8;
			g-=( (int32_t)src0_a->g + (int32_t)src0_c->g +
				(int32_t)src2_a->g + (int32_t)src2_c->g );
			if (g<0) g=0;
			else
			{
				g>>=4;
				if (g>255) g=255;
			}
			r=(((int32_t)src1_a->r)<<3) + (((int32_t)src1_b->r)<<3) +
				(((int32_t)src0_b->r)<<1) + (((int32_t)src2_b->r)<<1) + 8;
			r-=( (int32_t)src0_a->r + (int32_t)src0_c->r +
				(int32_t)src2_a->r + (int32_t)src2_c->r );
			if (r<0) r=0;
			else
			{
				r>>=4;
				if (r>255) r=255;
			}

			dst->b=(uint8_t)b;
			dst->g=(uint8_t)g;
			dst->r=(uint8_t)r;
			dst->alpha=src1_a->alpha;

			src0_a++;
			src0_b++;
			src0_c++;
			src1_a++;
			src1_b++;
			src2_a++;
			src2_b++;
			src2_c++;
			dst++;

		}
		src0_a=(RGB32BMP *)((uint8_t *)src0_a + buffer_pitch);
		src0_b=(RGB32BMP *)((uint8_t *)src0_b + buffer_pitch);
		src0_c=(RGB32BMP *)((uint8_t *)src0_c + buffer_pitch);
		src1_a=(RGB32BMP *)((uint8_t *)src1_a + buffer_pitch);
		src1_b=(RGB32BMP *)((uint8_t *)src1_b + buffer_pitch);
		src2_a=(RGB32BMP *)((uint8_t *)src2_a + (src_modulo+src_pitch));
		src2_b=(RGB32BMP *)((uint8_t *)src2_b + (src_modulo+src_pitch));
		src2_c=(RGB32BMP *)((uint8_t *)src2_c + (src_modulo+src_pitch));
		dst=(RGB32BMP *)((uint8_t *)dst + (dst_modulo+dst_pitch));
	}

}




void JPSDR_DeinterlaceII::AdvancedYUV32(const void *src0,const void *src1,const void *src2,void *_dst,int32_t w,
	int32_t h,ptrdiff_t src_pitch,ptrdiff_t buffer_pitch,ptrdiff_t dst_pitch,ptrdiff_t src_modulo,ptrdiff_t dst_modulo)
{
	const YUYV *src0_a,*src0_b,*src0_c,*src1_a,*src1_b,*src2_a,*src2_b,*src2_c;
	YUYV *dst;

	src0_a=(YUYV *)src0;
	src0_b=(YUYV *)((uint8_t *)src0 + (buffer_pitch << 1));
	src0_c=(YUYV *)((uint8_t *)src0 + (buffer_pitch << 2));
	src1_a=(YUYV *)src1;
	src1_b=(YUYV *)((uint8_t *)src1 + (buffer_pitch << 1));
	src2_a=(YUYV *)src2;
	src2_b=(YUYV *)((uint8_t *)src2 + (src_pitch << 1));
	src2_c=(YUYV *)((uint8_t *)src2 + (src_pitch << 2));
	dst=(YUYV *)_dst;

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int32_t y1,u,y2,v;

			y1=(((int32_t)src1_a->y1)<<3) + (((int32_t)src1_b->y1)<<3) +
				(((int32_t)src0_b->y1)<<1) + (((int32_t)src2_b->y1)<<1) + 8;
			y1-=( (int32_t)src0_a->y1 + (int32_t)src0_c->y1 +
				(int32_t)src2_a->y1 + (int32_t)src2_c->y1 );
			y1>>=4;
			if (y1<16) y1=16;
			else
			{
				if (y1>235) y1=235;
			}
			u=(((int32_t)src1_a->u-128)<<3) + (((int32_t)src1_b->u-128)<<3) +
				(((int32_t)src0_b->u-128)<<1) + (((int32_t)src2_b->u-128)<<1) + 8;
			u-=( ((int32_t)src0_a->u-128) + ((int32_t)src0_c->u-128) +
				((int32_t)src2_a->u-128) + ((int32_t)src2_c->u-128) );
			u>>=4;
			u+=128;
			if (u<16) u=16;
			else
			{
				if (u>240) u=240;
			}
			y2=(((int32_t)src1_a->y2)<<3) + (((int32_t)src1_b->y2)<<3) +
				(((int32_t)src0_b->y2)<<1) + (((int32_t)src2_b->y2)<<1) + 8;
			y2-=( (int32_t)src0_a->y2 + (int32_t)src0_c->y2 +
				(int32_t)src2_a->y2 + (int32_t)src2_c->y2 );
			y2>>=4;
			if (y2<16) y2=16;
			else
			{
				if (y2>235) y2=235;
			}
			v=(((int32_t)src1_a->v-128)<<3) + (((int32_t)src1_b->v-128)<<3) +
				(((int32_t)src0_b->v-128)<<1) + (((int32_t)src2_b->v-128)<<1) + 8;
			v-=( ((int32_t)src0_a->v-128) + ((int32_t)src0_c->v-128) +
				((int32_t)src2_a->v-128) + ((int32_t)src2_c->v-128) );
			v>>=4;
			v+=128;
			if (v<16) v=16;
			else
			{
				if (v>240) v=240;
			}

			dst->y1=(uint8_t)y1;
			dst->u=(uint8_t)u;
			dst->y2=(uint8_t)y2;
			dst->v=(uint8_t)v;

			src0_a++;
			src0_b++;
			src0_c++;
			src1_a++;
			src1_b++;
			src2_a++;
			src2_b++;
			src2_c++;
			dst++;

		}
		src0_a=(YUYV *)((uint8_t *)src0_a + buffer_pitch);
		src0_b=(YUYV *)((uint8_t *)src0_b + buffer_pitch);
		src0_c=(YUYV *)((uint8_t *)src0_c + buffer_pitch);
		src1_a=(YUYV *)((uint8_t *)src1_a + buffer_pitch);
		src1_b=(YUYV *)((uint8_t *)src1_b + buffer_pitch);
		src2_a=(YUYV *)((uint8_t *)src2_a + (src_modulo+src_pitch));
		src2_b=(YUYV *)((uint8_t *)src2_b + (src_modulo+src_pitch));
		src2_c=(YUYV *)((uint8_t *)src2_c + (src_modulo+src_pitch));
		dst=(YUYV *)((uint8_t *)dst + (dst_modulo+dst_pitch));
	}

}



void JPSDR_DeinterlaceII::Run()
{
	int32_t w,h;
	ptrdiff_t src_modulo,dst_modulo,src_pitch,dst_pitch,buffer_pitch;
	const void *src1,*src2,*src3;
	void *dst;
	uint8_t indice_m1,indice;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	src_modulo=image_data.src_modulo0;
	src_pitch=image_data.src_pitch0;
	dst_modulo=image_data.dst_modulo0;
	dst_pitch=image_data.dst_pitch0;

	h=image_data.src_h0;
	w=image_data.src_w0;

	switch(image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			buffer_pitch=4*w;
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			buffer_pitch=4*((w+1)>>1);
			break;
	}

	if (nbre_images==0)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
				JPSDR_DeinterlaceII_Move32_Full_src(image_data.src_plane0,buffer_frame[0],w,h,src_modulo);
				break;
			case VMODE_YUYV :
			case VMODE_UYVY :
				JPSDR_DeinterlaceII_Move32_Full_src(image_data.src_plane0,buffer_frame[0],(w+1)>>1,h,src_modulo);
				break;
		}
		nbre_images++;
		return;
	}

	if (nbre_images==1)
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
				JPSDR_DeinterlaceII_Move32_Full_src(image_data.src_plane0,buffer_frame[1],w,h,src_modulo);
				break;
			case VMODE_YUYV :
			case VMODE_UYVY :
				JPSDR_DeinterlaceII_Move32_Full_src(image_data.src_plane0,buffer_frame[1],(w+1)>>1,h,src_modulo);
				break;
		}
		src1=buffer_frame[0];
		if ((h<2) || ((nbre_images<mData.frame_offset) ||
			(((nbre_images-mData.frame_offset)%mData.frame_freq)!=0)) )
		{
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					JPSDR_DeinterlaceII_Move32_Full_dst(src1,image_data.dst_plane0,w,h,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					JPSDR_DeinterlaceII_Move32_Full_dst(src1,image_data.dst_plane0,(w+1)>>1,h,dst_modulo);
					break;
			}
		}
		else
		{
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					Blend_DownRGB32(src1,image_data.dst_plane0,w,h-1,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					Blend_DownYUV32(src1,image_data.dst_plane0,(w+1)>>1,h-1,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
			}
		}
		nbre_images++;
		return;
	}

	if (nbre_images>=2)
	{
		indice=indice_frame;
		indice_m1=(indice+1)%2;
		src1=buffer_frame[indice];
		dst=image_data.dst_plane0;
		if ((h<2) || ((nbre_images<mData.frame_offset) ||
			(((nbre_images-mData.frame_offset)%mData.frame_freq)!=0)) )
		{
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					JPSDR_DeinterlaceII_Move32_Full_dst(src1,dst,w,h,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					JPSDR_DeinterlaceII_Move32_Full_dst(src1,dst,(w+1)>>1,h,dst_modulo);
					break;
			}
		}
		else
		{
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					Blend_UpRGB32(src1,dst,w,2,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					Blend_UpYUV32(src1,dst,(w+1)>>1,2,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
			}
			dst=(void *)((char *)dst + (dst_pitch << 1));
			src1=buffer_frame[indice_m1];
			src2=buffer_frame[indice];
			src2=(void *)((char *)src2 + buffer_pitch);
			src3=image_data.src_plane0;
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					AdvancedRGB32(src1,src2,src3,dst,w,(h-4)>>1,src_pitch,buffer_pitch,dst_pitch,src_modulo,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					AdvancedRGB32(src1,src2,src3,dst,(w+1)>>1,(h-4)>>1,src_pitch,buffer_pitch,dst_pitch,src_modulo,
						dst_modulo);
					break;
			}
			src2=(void *)((char *)src2 + (buffer_pitch << 1));
			dst=(void *)((char *)dst + dst_pitch);
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					Move32(src2,dst,w,(h-4)>>1,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					Move32(src2,dst,(w+1)>>1,(h-4)>>1,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
			}
			src2=(void *)((char *)src2 + ((h-5)*buffer_pitch));
			dst=(void *)((char *)dst + ((h-5)*dst_pitch));
			switch(image_data.video_mode)
			{
				case VMODE_BMP_RGBA :
					Blend_DownRGB32(src2,dst,w,1,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
				case VMODE_YUYV :
				case VMODE_UYVY :
					Blend_DownYUV32(src2,dst,(w+1)>>1,1,buffer_pitch,dst_pitch,0,dst_modulo);
					break;
			}
		}
		indice_frame=(indice_frame+1)%2;
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
				JPSDR_DeinterlaceII_Move32_Full_src(image_data.src_plane0,buffer_frame[indice_frame],
					w,h,src_modulo);
				break;
			case VMODE_YUYV :
			case VMODE_UYVY :
				JPSDR_DeinterlaceII_Move32_Full_src(image_data.src_plane0,buffer_frame[indice_frame],
					(w+1)>>1,h,src_modulo);
				break;
		}
		nbre_images++;
	}
}




void JPSDR_DeinterlaceII::Start()
{
	bool ok;
	int8_t i;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}

	SetImageData(image_data);

	for (i=0; i<2; i++)
		buffer_frame[i]=(void *)malloc(image_data.src_size0);
	ok=true;
	for (i=0; i<2; i++)
		ok=ok && (buffer_frame[i]!=NULL);
	if (!ok)
	{
		for (i=1; i>=0; i--)
			myfree(buffer_frame[i]);
		ff->ExceptOutOfMemory();
		return;
	}

	indice_frame=1;
	nbre_images=0;
}



void JPSDR_DeinterlaceII::End()

{
	int8_t i;

	for (i=1; i>=0; i--)
		myfree(buffer_frame[i]);
}


bool JPSDR_DeinterlaceII::Configure(VDXHWND hwnd)
{
	JPSDR_DeinterlaceIIDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}



void JPSDR_DeinterlaceII::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.sharp=!!argv[0].asInt();
	mData.frame_offset=argv[1].asInt();
	mData.frame_freq=argv[2].asInt();
}



void JPSDR_DeinterlaceII::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf, maxlen,"Config(%d, %d, %d)",mData.sharp,mData.frame_offset,mData.frame_freq);
}

extern VDXFilterDefinition2 filterDef_JPSDR_DeinterlaceII=
VDXVideoFilterDefinition<JPSDR_DeinterlaceII>("JPSDR","Deinterlace II v2.4.1","Deinterlace Advanced. Lag 1");
