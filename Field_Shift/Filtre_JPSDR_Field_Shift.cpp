#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "Field_Shift_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

extern int g_VFVAPIVersion;

#define Max_Sequence 2048


class JPSDR_Field_ShiftData
{
public :
	uint8_t mode;
	bool file_mode;
	char filename[1024];

	JPSDR_Field_ShiftData(void);
};


JPSDR_Field_ShiftData::JPSDR_Field_ShiftData(void)
{
	mode=1;
	strcpy_s(filename,1024,"");
	file_mode=false;
}


class JPSDR_Field_ShiftDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_Field_ShiftDialog(JPSDR_Field_ShiftData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_FIELD_SHIFT),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	void RefreshDisplay(void);
	void SetDisplay(void);
	
	bool SaveToData();
	
	JPSDR_Field_ShiftData& mData;
	JPSDR_Field_ShiftData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_Field_ShiftDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


void JPSDR_Field_ShiftDialog::SetDisplay(void)
{
	switch (mData.mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_UP_UP,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_BOT_BOT,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_BOT_UP,1); break;
		case 4 : CheckDlgButton(mhdlg,IDC_UP_BOT,1); break;
	}
}


void JPSDR_Field_ShiftDialog::RefreshDisplay(void)
{
	EnableWindow(GetDlgItem(mhdlg,IDC_BROWSE),mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_PATH),mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_UP_UP),!mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_BOT_UP),!mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_BOT_BOT),!mData.file_mode);
	EnableWindow(GetDlgItem(mhdlg,IDC_UP_BOT),!mData.file_mode);
//	EnableWindow(GetDlgItem(mhdlg,IDC_PREVIEW),!mData.file_mode);
}


bool JPSDR_Field_ShiftDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_FILE_MODE,mData.file_mode?BST_CHECKED:BST_UNCHECKED);
	if (mData.file_mode)
	{
		SetWindowText(GetDlgItem(mhdlg,IDC_PATH),mData.filename);
	}

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	SetDisplay();
	RefreshDisplay();

	return false;
}


void JPSDR_Field_ShiftDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_Field_ShiftDialog::SaveToData()
{
	if (!!IsDlgButtonChecked(mhdlg, IDC_UP_UP)) mData.mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_BOT_BOT)) mData.mode=2;
	if (!!IsDlgButtonChecked(mhdlg, IDC_BOT_UP)) mData.mode=3;
	if (!!IsDlgButtonChecked(mhdlg, IDC_UP_BOT)) mData.mode=4;
	mData.file_mode=!!IsDlgButtonChecked(mhdlg,IDC_FILE_MODE);
	if (mData.file_mode) GetWindowText(GetDlgItem(mhdlg,IDC_PATH),mData.filename,1024);
	else strcpy_s(mData.filename,1024,"");
	
	return true;
}



bool JPSDR_Field_ShiftDialog::OnCommand(int cmd)
{
	switch(cmd)
	{
		case IDC_UP_UP :
		case IDC_BOT_BOT :
		case IDC_BOT_UP :
		case IDC_UP_BOT :
			if (mifp && SaveToData()) mifp->RedoFrame();
			RefreshDisplay();
			return true;
		case IDC_FILE_MODE :
			mData.file_mode=!!IsDlgButtonChecked(mhdlg,IDC_FILE_MODE);
/*			if (mData.file_mode)
			{
				if (mifp->IsPreviewDisplayed()) mifp->Toggle((VDXHWND)mhdlg);
			}*/
			RefreshDisplay();
			if (mData.file_mode) GetWindowText(GetDlgItem(mhdlg,IDC_PATH), mData.filename, 1024);
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
				SetWindowText(GetDlgItem(mhdlg,IDC_PATH), mData.filename);
			}
			return true;
		case IDOK :
			SaveToData();
			EndDialog(mhdlg,true);
			return true;
		case IDCANCEL :
			mData=moldData;
			EndDialog(mhdlg,false);
			return true;
		case IDC_PREVIEW:
			if (mifp)
				mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



class JPSDR_Field_Shift : public JPSDRVDXVideoFilter
{
public:
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	virtual bool Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	uint32_t debut[Max_Sequence],fin[Max_Sequence];
	uint8_t tab_mode[Max_Sequence];
	uint16_t nbre_file_mode;
	bool VDub2_Enable;

	inline void Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
		const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);
	inline void Rebuild_Frame(const void *top_src, const void *bottom_src, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);

	void Blend_Tri_RGB32(const void *src,void *dst,const int32_t w,const int32_t h,const ptrdiff_t src_modulo,
		const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);
	void Blend_Tri_YUV32(const void *src,void *dst,const int32_t w,const int32_t h,const ptrdiff_t src_modulo,
		const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch);
	void Blend_Tri_Planar(const void *src,void *dst,const int32_t w,const int32_t h,const ptrdiff_t src_modulo,
		const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,ptrdiff_t dst_pitch);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_Field_ShiftData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Field_Shift)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Field_Shift,ScriptConfig,"iis")
VDXVF_END_SCRIPT_METHODS()


uint32 JPSDR_Field_Shift::GetParams()
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

	fa->dst.offset = fa->src.offset;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
}


bool JPSDR_Field_Shift::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	prefetcher->PrefetchFrame(0,frame,0);
	prefetcher->PrefetchFrame(0,frame-1,0);

	return(true);
}


inline void JPSDR_Field_Shift::Move_Full(const void *src_, void *dst_, const int32_t w,const int32_t h,
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


inline void JPSDR_Field_Shift::Rebuild_Frame(const void *top_src, const void *bottom_src, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	const uint8_t *src;
	uint8_t *dst;
	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	src=(uint8_t *)top_src;
	dst=(uint8_t *)dst_;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

	src=(uint8_t *)bottom_src;
	dst=(uint8_t *)dst_+dst_pitch;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

}


void JPSDR_Field_Shift::GetSettingString(char *buf, int maxlen)
{
	char output_string[1200];
	
	if (mData.file_mode)
	{
		strcpy_s(output_string,1200," File mode : %s");
		SafePrintf(buf,maxlen,output_string,mData.filename);
	}
	else
	{
		strcpy_s(output_string,1200," Mode : %d");
		SafePrintf(buf,maxlen,output_string,mData.mode);
	}
}



void JPSDR_Field_Shift::Blend_Tri_RGB32(const void *src,void *_dst,const int32_t w,const int32_t h,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	RGB32BMP *src1,*src2,*src3,*dst;

	src1=(RGB32BMP *)src;
	src2=(RGB32BMP *)((uint8_t *)src1+src_pitch);
	src3=(RGB32BMP *)((uint8_t *)src2+src_pitch);
	memcpy(_dst,src,w << 2);
	dst=(RGB32BMP *)((uint8_t *)_dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->r=(((uint16_t)src1->r+(uint16_t)src3->r+(((uint16_t)src2->r) << 1))+2) >> 2;
			dst->g=(((uint16_t)src1->g+(uint16_t)src3->g+(((uint16_t)src2->g) << 1))+2) >> 2;
			dst->b=(((uint16_t)src1->b+(uint16_t)src3->b+(((uint16_t)src2->b) << 1))+2) >> 2;
			dst->alpha=src2->alpha;
			src1++;
			src2++;
			src3++;
			dst++;
		}
		src1=(RGB32BMP *)((uint8_t *)src1+src_modulo);
		src2=(RGB32BMP *)((uint8_t *)src2+src_modulo);
		src3=(RGB32BMP *)((uint8_t*)src3+src_modulo);
		dst=(RGB32BMP *)((uint8_t *)dst+dst_modulo);
	}

	memcpy(dst,src2,w << 2);

}


void JPSDR_Field_Shift::Blend_Tri_YUV32(const void *src,void *_dst,const int32_t w,const int32_t h,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	YUYV *src1,*src2,*src3,*dst;

	src1=(YUYV *)src;
	src2=(YUYV *)((uint8_t *)src1+src_pitch);
	src3=(YUYV *)((uint8_t *)src2+src_pitch);
	memcpy(_dst,src,w << 2);
	dst=(YUYV *)((uint8_t *)_dst+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			dst->y1=(((uint16_t)src1->y1+(uint16_t)src3->y1+(((uint16_t)src2->y1) << 1))+2) >> 2;
			dst->u=(((uint16_t)src1->u+(uint16_t)src3->u+(((uint16_t)src2->u) << 1))+2) >> 2;
			dst->y2=(((uint16_t)src1->y2+(uint16_t)src3->y2+(((uint16_t)src2->y2) << 1))+2) >> 2;
			dst->v=(((uint16_t)src1->v+(uint16_t)src3->v+(((uint16_t)src2->v) << 1))+2) >> 2;
			src1++;
			src2++;
			src3++;
			dst++;
		}
		src1=(YUYV *)((uint8_t *)src1+src_modulo);
		src2=(YUYV *)((uint8_t *)src2+src_modulo);
		src3=(YUYV *)((uint8_t*)src3+src_modulo);
		dst=(YUYV *)((uint8_t *)dst+dst_modulo);
	}

	memcpy(dst,src2,w << 2);

}


void JPSDR_Field_Shift::Blend_Tri_Planar(const void *src,void *_dst,const int32_t w,const int32_t h,
	const ptrdiff_t src_modulo,const ptrdiff_t dst_modulo,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	uint8_t *src1,*src2,*src3,*dst;

	src1=(uint8_t *)src;
	src2=src1+src_pitch;
	src3=src2+src_pitch;
	memcpy(_dst,src,w);
	dst=(uint8_t *)(((uint8_t *)_dst)+dst_pitch);

	for (int32_t i=0; i<h; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			*dst++=(uint8_t)((((uint16_t)*src1++)+((uint16_t)*src3++)
				+(((uint16_t)*src2++) << 1)+2) >> 2);
		}
		src1+=src_modulo;
		src2+=src_modulo;
		src3+=src_modulo;
		dst+=dst_modulo;
	}
	memcpy(dst,src2,w);
}



void JPSDR_Field_Shift::Run()
{
	const VDXPixmapAlpha* tab_pxsrc[2]={
		(const VDXPixmapAlpha*)fa->mpSourceFrames[0]->mpPixmap,
		(const VDXPixmapAlpha*)fa->mpSourceFrames[1]->mpPixmap
	};
	const VDXPixmapAlpha& pxdst = (const VDXPixmapAlpha&)*fa->mpOutputFrames[0]->mpPixmap;
	const VDXFBitmap& pxsrc = *fa->mpSourceFrames[0];

	int32_t w_Y,h_Y,w_U,h_U,w_V,h_V;
	uint32_t line0,line1,line2;
	ptrdiff_t src_modulo_Y,dst_modulo_Y,src_pitch_Y,dst_pitch_Y;
	ptrdiff_t src_modulo_U,dst_modulo_U,src_pitch_U,dst_pitch_U;
	ptrdiff_t src_modulo_V,dst_modulo_V,src_pitch_V,dst_pitch_V;
	const void *src_top_Y,*src_bottom_Y,*src_top_U,*src_bottom_U,*src_top_V,*src_bottom_V;
	void *dst_Y,*dst_U,*dst_V;
	uint8_t mode;
	uint16_t i;
	bool ok;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;


	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_Y :
			dst_Y=image_data.dst_plane0;
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			dst_Y=image_data.dst_plane0;
			dst_U=image_data.dst_plane1;
			dst_V=image_data.dst_plane2;
			break;
	}

	w_Y=image_data.src_w0;
	h_Y=image_data.src_h0;
	line0=image_data.src_line0;
	src_pitch_Y=image_data.src_pitch0;
	src_modulo_Y=image_data.src_modulo0;
	dst_pitch_Y=image_data.dst_pitch0;
	dst_modulo_Y=image_data.dst_modulo0;
	w_U=image_data.src_w1;
	h_U=image_data.src_h1;
	line1=image_data.src_line1;
	src_pitch_U=image_data.src_pitch1;
	src_modulo_U=image_data.src_modulo1;
	dst_pitch_U=image_data.dst_pitch1;
	dst_modulo_U=image_data.dst_modulo1;
	w_V=image_data.src_w2;
	h_V=image_data.src_h2;
	line2=image_data.src_line2;
	src_pitch_V=image_data.src_pitch2;
	src_modulo_V=image_data.src_modulo2;
	dst_pitch_V=image_data.dst_pitch2;
	dst_modulo_V=image_data.dst_modulo2;


	if (mData.file_mode)
	{
		ok=false;
		i=0;
		while ((i<nbre_file_mode) && (!ok))
		{
			if ((pxsrc.mFrameNumber>=debut[i]) && (pxsrc.mFrameNumber<=fin[i]))
			{
				ok=true;
				mode=tab_mode[i];
			}
			i++;
		}
	}
	else
	{
		mode=mData.mode;
		ok=true;
	}

	if (pxsrc.mFrameNumber==0)
	{
		image_data.src_plane0=tab_pxsrc[0]->data;
		image_data.src_plane1=tab_pxsrc[0]->data2;
		image_data.src_plane2=tab_pxsrc[0]->data3;
			
		switch (image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				src_top_Y=image_data.src_plane0;
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				src_top_Y=image_data.src_plane0;
				src_top_U=image_data.src_plane1;
				src_top_V=image_data.src_plane2;
				break;
		}

		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
				if (ok) Blend_Tri_RGB32(src_top_Y,dst_Y,w_Y,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_YUYV :
			case VMODE_UYVY :
				if (ok) Blend_Tri_YUV32(src_top_Y,dst_Y,(w_Y+1)>>1,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				if (ok) Blend_Tri_Planar(src_top_Y,dst_Y,w_Y,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				if (ok) Blend_Tri_Planar(src_top_U,dst_U,w_U,h_U-2,src_modulo_U,dst_modulo_U,
					src_pitch_U,dst_pitch_U);
				if (ok) Blend_Tri_Planar(src_top_V,dst_V,w_V,h_V-2,src_modulo_V,dst_modulo_V,
					src_pitch_V,dst_pitch_V);
				break;
			case VMODE_Y :
				if (ok) Blend_Tri_Planar(src_top_Y,dst_Y,w_Y,h_Y-2,src_modulo_Y,dst_modulo_Y,
					src_pitch_Y,dst_pitch_Y);
				break;
		}

//		mFrameNumber++;
		return;
	}

	if (ok)
	{
		switch(mode)
		{
			case 1 :
				image_data.src_plane0=tab_pxsrc[1]->data;
				image_data.src_plane1=tab_pxsrc[1]->data2;
				image_data.src_plane2=tab_pxsrc[1]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_top_Y=image_data.src_plane0;
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_top_Y=image_data.src_plane0;
						src_top_U=image_data.src_plane1;
						src_top_V=image_data.src_plane2;
						break;
				}

				image_data.src_plane0=tab_pxsrc[0]->data;
				image_data.src_plane1=tab_pxsrc[0]->data2;
				image_data.src_plane2=tab_pxsrc[0]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_bottom_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_bottom_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						src_bottom_U=(void *)((uint8_t *)image_data.src_plane1+image_data.src_pitch1);
						src_bottom_V=(void *)((uint8_t *)image_data.src_plane2+image_data.src_pitch2);
						break;
				}
				break;
			case 2 :
				image_data.src_plane0=tab_pxsrc[0]->data;
				image_data.src_plane1=tab_pxsrc[0]->data2;
				image_data.src_plane2=tab_pxsrc[0]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_top_Y=image_data.src_plane0;
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_top_Y=image_data.src_plane0;
						src_top_U=image_data.src_plane1;
						src_top_V=image_data.src_plane2;
						break;
				}

				image_data.src_plane0=tab_pxsrc[1]->data;
				image_data.src_plane1=tab_pxsrc[1]->data2;
				image_data.src_plane2=tab_pxsrc[1]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_bottom_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_bottom_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						src_bottom_U=(void *)((uint8_t *)image_data.src_plane1+image_data.src_pitch1);
						src_bottom_V=(void *)((uint8_t *)image_data.src_plane2+image_data.src_pitch2);
						break;
				}
				break;
			case 3 :
				image_data.src_plane0=tab_pxsrc[1]->data;
				image_data.src_plane1=tab_pxsrc[1]->data2;
				image_data.src_plane2=tab_pxsrc[1]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_top_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_top_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						src_top_U=(void *)((uint8_t *)image_data.src_plane1+image_data.src_pitch1);
						src_top_V=(void *)((uint8_t *)image_data.src_plane2+image_data.src_pitch2);
						break;
				}

				image_data.src_plane0=tab_pxsrc[0]->data;
				image_data.src_plane1=tab_pxsrc[0]->data2;
				image_data.src_plane2=tab_pxsrc[0]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_bottom_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_bottom_Y=(void *)((uint8_t *)image_data.src_plane0+image_data.src_pitch0);
						src_bottom_U=(void *)((uint8_t *)image_data.src_plane1+image_data.src_pitch1);
						src_bottom_V=(void *)((uint8_t *)image_data.src_plane2+image_data.src_pitch2);
						break;
				}
				break;
			case 4 :
				image_data.src_plane0=tab_pxsrc[0]->data;
				image_data.src_plane1=tab_pxsrc[0]->data2;
				image_data.src_plane2=tab_pxsrc[0]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_top_Y=image_data.src_plane0;
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_top_Y=image_data.src_plane0;
						src_top_U=image_data.src_plane1;
						src_top_V=image_data.src_plane2;
						break;
				}

				image_data.src_plane0=tab_pxsrc[1]->data;
				image_data.src_plane1=tab_pxsrc[1]->data2;
				image_data.src_plane2=tab_pxsrc[1]->data3;
			
				switch (image_data.video_mode)
				{
					case VMODE_BMP_RGBA :
					case VMODE_YUYV :
					case VMODE_UYVY :
					case VMODE_Y :
						src_bottom_Y=image_data.src_plane0;
						break;
					case VMODE_PLANAR_YUV444 :
					case VMODE_PLANAR_YUV422 :
					case VMODE_PLANAR_YUV420 :
					case VMODE_PLANAR_YUV411 :
					case VMODE_PLANAR_YUV410 :
						src_bottom_Y=image_data.src_plane0;
						src_bottom_U=image_data.src_plane1;
						src_bottom_V=image_data.src_plane2;
						break;
				}
				break;
		}

		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				Rebuild_Frame(src_top_Y,src_bottom_Y,dst_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				Rebuild_Frame(src_top_Y,src_bottom_Y,dst_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				Rebuild_Frame(src_top_U,src_bottom_U,dst_U,line1,h_U>>1,src_pitch_U,dst_pitch_U);
				Rebuild_Frame(src_top_V,src_bottom_V,dst_V,line2,h_V>>1,src_pitch_V,dst_pitch_V);
				break;
		}
	}
	else
	{
		image_data.src_plane0=tab_pxsrc[0]->data;
		image_data.src_plane1=tab_pxsrc[0]->data2;
		image_data.src_plane2=tab_pxsrc[0]->data3;
			
		switch (image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				src_top_Y=image_data.src_plane0;
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				src_top_Y=image_data.src_plane0;
				src_top_U=image_data.src_plane1;
				src_top_V=image_data.src_plane2;
				break;
		}

		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				Move_Full(src_top_Y,dst_Y,line0,h_Y,src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				Move_Full(src_top_Y,dst_Y,line0,h_Y,src_pitch_Y,dst_pitch_Y);
				Move_Full(src_top_U,dst_U,line1,h_U,src_pitch_U,dst_pitch_U);
				Move_Full(src_top_V,dst_V,line2,h_V,src_pitch_V,dst_pitch_V);
				break;
		}
	}
}



void JPSDR_Field_Shift::Start()
{
	uint32_t u,v;
	int32_t offset;
	uint16_t w,line_file;
	int16_t i,j;
	FILE *file_in;
	char buffer_in[1024];

	if (g_VFVAPIVersion<14)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
	}

	SetImageData(image_data);

	switch(image_data.video_mode)
	{
		case VMODE_PLANAR_YUV420 :
			if ((image_data.src_h1%2)!=0)
				ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
			break;
		case VMODE_PLANAR_YUV410 :
			if ((image_data.src_h1%2)!=0)
				ff->Except("Vertical size must be multiple of 8 for 4:1:0!");
			break;
		default :
			if ((image_data.src_h0%2)!=0)
				ff->Except("Vertical size must be multiple of 2!");
			break;
	}

	if (mData.file_mode)
	{
		if (strlen(mData.filename)==0)
		{
			ff->Except("No file selected!");
			return;
		}

		nbre_file_mode=0;
		offset=0;
		line_file=0;

		if (fopen_s(&file_in,mData.filename,"rt")!=0)
		{
			strcpy_s(buffer_in,1024,"Unable to open file : ");
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
					sprintf_s(buffer_in,1024,"Error in file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
			}
			else
			{
				if (sscanf_s(buffer_in,"%lu %lu %hu",&u,&v,&w)==EOF)
				{
					fclose(file_in);
					sprintf_s(buffer_in,1024,"Error in file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
				else
				{
					if ((w<1) || (w>4))
					{
						fclose(file_in);
						sprintf_s(buffer_in,1024,"Incorrect mode number on line %ld.",line_file);
						ff->Except(buffer_in);
						return;
					}
					if ((u+offset)>(v+offset))
					{
						fclose(file_in);
						sprintf_s(buffer_in,1024,"Error on frame position on line %ld.",line_file);
						ff->Except(buffer_in);
						return;
					}
					debut[nbre_file_mode]=u+offset;
					fin[nbre_file_mode]=v+offset;
					tab_mode[nbre_file_mode]=(uint8_t)w;
					nbre_file_mode++;
				}
			}
		}
		fclose(file_in);

		if (nbre_file_mode==0)
		{
			strcpy_s(buffer_in,1024,"No line in file : ");
			strcat_s(buffer_in,1024,mData.filename);
			ff->Except(buffer_in);
			return;
		}

		for (i=0; i<(nbre_file_mode-1); i++)
		{
			for (j=i+1; j<nbre_file_mode; j++)
			{
				if (((debut[j]>=debut[i]) && (debut[j]<=fin[i])) ||
					((fin[j]>=debut[i]) && (fin[j]<=fin[i])))
				{
					sprintf_s(buffer_in,1024,"Overlap around lines %ld and %ld.",i+1,j+1);
					ff->Except(buffer_in);
					return;
				}
			}
		}
	}
}



void JPSDR_Field_Shift::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.mode=argv[0].asInt();
	mData.file_mode=!!argv[1].asInt();
	strcpy_s(mData.filename,1024,*argv[2].asString());
}


bool JPSDR_Field_Shift::Configure(VDXHWND hwnd)
{
	JPSDR_Field_ShiftDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}



void JPSDR_Field_Shift::GetScriptString(char *buf, int maxlen)
{
    char save[1400];
	int i, j;

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

	SafePrintf(buf,maxlen,"Config(%d, %d, \"%s\")",mData.mode,mData.file_mode,save);
}


extern VDXFilterDefinition2 filterDef_JPSDR_Field_Shift=
VDXVideoFilterDefinition<JPSDR_Field_Shift>("JPSDR","FieldShift v3.2.1","Shift Field from previous and current frame.");

