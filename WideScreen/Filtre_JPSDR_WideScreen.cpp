/*
 *  WideScreen
 *
 *  Remplace top and bottom with pure black. Will keep subtitles on bottom.
 *  For digitized old videos with black top and bottom.
 *  Copyright (C) 2005 JPSDR
 *	
 *  WideScreen is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  WideScreen is distributed in the hope that it will be useful,
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

#include "./WideScreen_resource.h"

#include "../Filtres_JPSDR/JPSDRVideoFilter.h"
#include "../Filtres_JPSDR/VideoFilterDialog.h"

extern int g_VFVAPIVersion;

extern "C" void JPSDR_WideScreenRGB32(const void *src,void *dst,int32_t w,ptrdiff_t src_modulo,
	ptrdiff_t dst_modulo,int32_t top,int32_t bottom,ptrdiff_t src_offset_center,ptrdiff_t dst_offset_center);


class JPSDR_WideScreenData
{
public:
	unsigned short top,bottom;

	JPSDR_WideScreenData(void);
};

JPSDR_WideScreenData::JPSDR_WideScreenData(void)
{
	top=0;
	bottom=0;
}



class JPSDR_WideScreenDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_WideScreenDialog(JPSDR_WideScreenData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_WIDESCREEN),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_WideScreenData& mData;
	JPSDR_WideScreenData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_WideScreenDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_WideScreenDialog::OnInit()
{
	moldData=mData;

	SetDlgItemInt(mhdlg,IDC_TOP,mData.top,FALSE);
	SetDlgItemInt(mhdlg,IDC_BOTTOM,mData.bottom,FALSE);

	return false;
}


void JPSDR_WideScreenDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}



bool JPSDR_WideScreenDialog::SaveToData()
{
	uint16_t size;
	BOOL success;

	size=GetDlgItemInt(mhdlg,IDC_TOP,&success,FALSE);
	if (success) mData.top=size;
	size=GetDlgItemInt(mhdlg,IDC_BOTTOM,&success,FALSE);
	if (success) mData.bottom=size;

	return true;
}



bool JPSDR_WideScreenDialog::OnCommand(int cmd)
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


class JPSDR_WideScreen : public JPSDRVDXVideoFilter
{
public:
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	ptrdiff_t src_offset_center,dst_offset_center;
	bool VDub2_Enable;

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_WideScreenData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_WideScreen)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_WideScreen,ScriptConfig,"ii")
VDXVF_END_SCRIPT_METHODS()


uint32 JPSDR_WideScreen::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;
	
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



bool JPSDR_WideScreen::Configure(VDXHWND hwnd)
{
	JPSDR_WideScreenDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_WideScreen::GetSettingString(char *buf, int maxlen)
{
	SafePrintf(buf,maxlen," Top size : %d Bottom size : %d",mData.top,mData.bottom);
}



void JPSDR_WideScreen::Run()
{
	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	const VDXPixmapAlpha& pxdst=(const VDXPixmapAlpha&)*fa->dst.mpPixmap;
	const VDXPixmapAlpha& pxsrc=(const VDXPixmapAlpha&)*fa->src.mpPixmap;

	image_data.src_plane0=pxsrc.data;
	image_data.src_plane1=pxsrc.data2;
	image_data.src_plane2=pxsrc.data3;
	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			image_data.src_plane0=(void *)((uint8_t *)image_data.src_plane0-((image_data.src_h0-1)*image_data.src_pitch0));
			image_data.dst_plane0=(void *)((uint8_t *)image_data.dst_plane0-((image_data.dst_h0-1)*image_data.dst_pitch0));
			break;
	}

	JPSDR_WideScreenRGB32(image_data.src_plane0,image_data.dst_plane0,image_data.src_w0,image_data.src_modulo0,image_data.dst_modulo0,
		mData.top,mData.bottom,src_offset_center,dst_offset_center);
}



void JPSDR_WideScreen::Start()
{
	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}
	
	SetImageData(image_data,true);

	src_offset_center=(image_data.src_h0-(mData.top+mData.bottom))*image_data.src_pitch0;
	dst_offset_center=(image_data.dst_h0-(mData.top+mData.bottom))*image_data.dst_pitch0;
}



void JPSDR_WideScreen::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
    mData.top=argv[0].asInt();
	mData.bottom=argv[1].asInt();
}


void JPSDR_WideScreen::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf,maxlen,"Config(%d, %d)",mData.top,mData.bottom);
}


extern VDXFilterDefinition2 filterDef_JPSDR_WideScreen=
VDXVideoFilterDefinition<JPSDR_WideScreen>("JPSDR","WideSreen v2.4.2","Black WideScreen and left subtitles.[ASM] Optimised.");


