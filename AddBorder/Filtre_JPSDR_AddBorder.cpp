/*
 *  AddBorder
 *
 *  Add border to image.
 *  Copyright (C) 2000 JPSDR
 *	
 *  AddBorder is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  AddBorder is distributed in the hope that it will be useful,
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

#include "./AddBorder_resource.h"

#include "../Filtres_JPSDR/JPSDRVideoFilter.h"
#include "../Filtres_JPSDR/VideoFilterDialog.h"

#include "../Filtres_JPSDR/Pascal.h"

extern int g_VFVAPIVersion;

extern "C" void JPSDR_AddBorder_Fill32(void *dst,ptrdiff_t size,uint32_t data);


class JPSDR_AddBorderData
{
public:
	int32_t left,right,top,bottom;
	uint8_t red,green,blue;

	JPSDR_AddBorderData(void);
};


JPSDR_AddBorderData::JPSDR_AddBorderData(void)
{
	left=0;
	right=0;
	top=0;
	bottom=0;
	red=0;
	green=0;
	blue=0;
}



class JPSDR_AddBorderDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_AddBorderDialog(JPSDR_AddBorderData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_ADD_BORDER),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_AddBorderData& mData;
	JPSDR_AddBorderData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_AddBorderDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_AddBorderDialog::OnInit()
{
	moldData=mData;

	SetDlgItemInt(mhdlg,IDC_LEFT,mData.left,FALSE);
	SetDlgItemInt(mhdlg,IDC_RIGHT,mData.right,FALSE);
	SetDlgItemInt(mhdlg,IDC_TOP,mData.top,FALSE);
	SetDlgItemInt(mhdlg,IDC_BOTTOM,mData.bottom,FALSE);
	SetDlgItemInt(mhdlg,IDC_RED,mData.red,FALSE);
	SetDlgItemInt(mhdlg,IDC_GREEN,mData.green,FALSE);
	SetDlgItemInt(mhdlg,IDC_BLUE,mData.blue,FALSE);

	return false;
}


void JPSDR_AddBorderDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}

bool JPSDR_AddBorderDialog::SaveToData()
{
	int32_t data1;
	uint8_t data2;
	BOOL success;

	data1=GetDlgItemInt(mhdlg,IDC_LEFT,&success,FALSE);
	if (success) mData.left=data1;
	data1=GetDlgItemInt(mhdlg,IDC_RIGHT,&success,FALSE);
	if (success) mData.right=data1;
	data1=GetDlgItemInt(mhdlg,IDC_TOP,&success,FALSE);
	if (success) mData.top=data1;
	data1=GetDlgItemInt(mhdlg,IDC_BOTTOM,&success,FALSE);
	if (success) mData.bottom=data1;
	data2=GetDlgItemInt(mhdlg,IDC_RED,&success,FALSE);
	if (success) mData.red=data2;
	data2=GetDlgItemInt(mhdlg,IDC_GREEN,&success,FALSE);
	if (success) mData.green=data2;
	data2=GetDlgItemInt(mhdlg,IDC_BLUE,&success,FALSE);
	if (success) mData.blue=data2;

	return true;
}



bool JPSDR_AddBorderDialog::OnCommand(int cmd)
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



class JPSDR_AddBorder : public JPSDRVDXVideoFilter
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
	int32_t top_Y,bottom_Y,left_Y,right_Y;
	int32_t top_UV,bottom_UV,left_UV,right_UV;
	uint32_t data32;
	uint8_t y_fill,u_fill,v_fill;
	bool VDub2_Enable;

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_AddBorderData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_AddBorder)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_AddBorder,ScriptConfig,"iiiiiii")
VDXVF_END_SCRIPT_METHODS()


uint32 JPSDR_AddBorder::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;
	
	VDub2_Enable=((fma!=NULL) && (fma->fmpixmap!=NULL));
	top_Y=mData.top;
	bottom_Y=mData.bottom;
	left_Y=mData.left;
	right_Y=mData.right;

	const VDXPixmapLayoutAlpha& pxsrc = (const VDXPixmapLayoutAlpha&)*fa->src.mpPixmapLayout;	
	VDXPixmapLayoutAlpha& pxdst = (VDXPixmapLayoutAlpha&)*fa->dst.mpPixmapLayout;
	
	if ((left_Y==0) && (right_Y==0) && (top_Y==0) && (bottom_Y==0))
	{
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB1555 :
			case nsVDXPixmap::kPixFormat_RGB565 :
			case nsVDXPixmap::kPixFormat_RGB888 :
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
			case nsVDXPixmap::kPixFormat_YUV422_YU64 :
				break;
			default : return FILTERPARAM_NOT_SUPPORTED;
		}
	}
	else
	{
		switch(pxsrc.format)
		{
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
				break;										
			default : return FILTERPARAM_NOT_SUPPORTED;
		}
	}

	image_data.src_video_mode=GetVideoMode(pxsrc.format);
	image_data.src_full_mode=CheckFullRangeMode(&fa->src);
	image_data.src_color_matrix=GetColorMode(&fa->src);
	image_data.src_interlaced=GetInterlaceMode(&fa->src);

	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;
	fa->dst.depth = 0;
	fa->dst.offset=fa->src.offset;
	
	if ((left_Y==0) && (right_Y==0) && (top_Y==0) && (bottom_Y==0))
	{
		if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS);
		else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM);
	}

	switch(image_data.src_video_mode)
	{
		case VMODE_YUYV :
		case VMODE_UYVY :
		case VMODE_PLANAR_YUV422 :
			left_Y=((left_Y+1)>>1)<<1;
			right_Y=((right_Y+1)>>1)<<1;
			break;
		case VMODE_PLANAR_YUV420 :
			top_Y=((top_Y+1)>>1)<<1;
			bottom_Y=((bottom_Y+1)>>1)<<1;
			left_Y=((left_Y+1)>>1)<<1;
			right_Y=((right_Y+1)>>1)<<1;
			break;
		case VMODE_PLANAR_YUV411 :
			left_Y=((left_Y+3)>>2)<<2;
			right_Y=((right_Y+3)>>2)<<2;
			break;
		case VMODE_PLANAR_YUV410 :
			top_Y=((top_Y+3)>>2)<<2;
			bottom_Y=((bottom_Y+3)>>2)<<2;
			left_Y=((left_Y+3)>>2)<<2;
			right_Y=((right_Y+3)>>2)<<2;
			break;
		default : break;
	}

	pxdst.w+=left_Y+right_Y;
	pxdst.h+=top_Y+bottom_Y;
	pxdst.pitch=0;

	image_data.dst_video_mode=GetVideoMode(pxdst.format);
	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_ALIGN_SCANLINES);

}



void JPSDR_AddBorder::Start()
{
	double y,u,v;

	if ((left_Y==0) && (right_Y==0) && (top_Y==0) && (bottom_Y==0)) return;
	
	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}

	SetImageData(image_data);

	y=image_data.src_Off_Y+(image_data.src_Kr*mData.red+image_data.src_Kg*mData.green+image_data.src_Kb*mData.blue)*image_data.src_Coeff_Y;
	u=image_data.src_Off_U+(image_data.src_Ur*mData.red+image_data.src_Ug*mData.green+image_data.src_Ub*mData.blue)*image_data.src_Coeff_U;
	v=image_data.src_Off_V+(image_data.src_Vr*mData.red+image_data.src_Vg*mData.green+image_data.src_Vb*mData.blue)*image_data.src_Coeff_V;
	if (y>image_data.src_Y_Max) y=image_data.src_Y_Max;
	else
	{
		if (y<image_data.src_Y_Min) y=image_data.src_Y_Min;
	}
	if (u>image_data.src_U_Max) u=image_data.src_U_Max;
	else
	{
		if (u<image_data.src_U_Min) u=image_data.src_U_Min;
	}
	if (v>image_data.src_V_Max) v=image_data.src_V_Max;
	else
	{
		if (v<image_data.src_V_Min) v=image_data.src_V_Min;
	}
	
	y_fill=(uint8_t)round(y);
	u_fill=(uint8_t)round(u);
	v_fill=(uint8_t)round(v);

	switch (image_data.video_mode)
	{
		case VMODE_BMP_RGBA :
			data32=((uint32_t)mData.blue) | (((uint32_t)mData.green)<<8) | (((uint32_t)mData.red)<<16);
			break;
		case VMODE_YUYV :
			data32=((uint32_t)y_fill) | (((uint32_t)u_fill) << 8) | (((uint32_t)y_fill) << 16)
				| (((uint32_t)v_fill) << 24);
			break;
		case VMODE_UYVY :
			data32=((uint32_t)u_fill) | (((uint32_t)y_fill) << 8) | (((uint32_t)v_fill) << 16)
				| (((uint32_t)y_fill) << 24);
			break;
		default : data32=0;
	}

	switch (image_data.video_mode)
	{
		case VMODE_PLANAR_YUV422 :
			top_UV=top_Y;
			bottom_UV=bottom_Y;
			left_UV=left_Y>>1;
			right_UV=right_Y>>1;
			break;
		case VMODE_PLANAR_YUV420 :
			top_UV=top_Y>>1;
			bottom_UV=bottom_Y>>1;
			left_UV=left_Y>>1;
			right_UV=right_Y>>1;
			break;
		case VMODE_PLANAR_YUV411 :
			top_UV=top_Y;
			bottom_UV=bottom_Y;
			left_UV=left_Y>>2;
			right_UV=right_Y>>2;
			break;
		case VMODE_PLANAR_YUV410 :
			top_UV=top_Y>>2;
			bottom_UV=bottom_Y>>2;
			left_UV=left_Y>>2;
			right_UV=right_Y>>2;
			break;
		default :
			top_UV=top_Y;
			bottom_UV=bottom_Y;
			left_UV=left_Y;
			right_UV=right_Y;
			break;
	}

}



void JPSDR_AddBorder::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];

	strcpy_s(output_string,255," Top : %d Bottom : %d Left : %d Right : %d");
	SafePrintf(buf,maxlen,output_string,mData.top,mData.bottom,mData.left,mData.right);
}


void JPSDR_AddBorder::Run()
{
	const void *src;
	void *dst,*dst_;

	if ((top_Y==0) && (bottom_Y==0) && (left_Y==0) && (right_Y==0)) return;

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
			src=image_data.src_plane0;
			dst=image_data.dst_plane0;
			for (int32_t i=0; i<top_Y; i++)
			{
				JPSDR_AddBorder_Fill32(dst,image_data.dst_w0,data32);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			for (int32_t i=0; i<image_data.src_h0; i++)
			{
				JPSDR_AddBorder_Fill32(dst,left_Y,data32);
				dst_=(void *)((uint8_t *)dst+(left_Y<<2));
				memcpy(dst_,src,image_data.src_w0<<2);
				src=(void *)((uint8_t *)src+image_data.src_pitch0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
				dst_=(void *)((uint8_t *)dst_+(image_data.src_w0<<2));
				JPSDR_AddBorder_Fill32(dst_,right_Y,data32);
			}
			for (int32_t i=0; i<bottom_Y; i++)
			{
				JPSDR_AddBorder_Fill32(dst,image_data.dst_w0,data32);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			break;
		case VMODE_YUYV :
		case VMODE_UYVY :
			src=image_data.src_plane0;
			dst=image_data.dst_plane0;
			for (int32_t i=0; i<top_Y; i++)
			{
				JPSDR_AddBorder_Fill32(dst,image_data.dst_w0>>1,data32);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			for (int32_t i=0; i<image_data.src_h0; i++)
			{
				JPSDR_AddBorder_Fill32(dst,left_Y>>1,data32);
				dst_=(void *)((uint8_t *)dst+(left_Y<<1));
				memcpy(dst_,src,image_data.src_w0<<1);
				src=(void *)((uint8_t *)src+image_data.src_pitch0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
				dst_=(void *)((uint8_t *)dst_+(image_data.src_w0<<1));
				JPSDR_AddBorder_Fill32(dst_,right_Y>>1,data32);
			}
			for (int32_t i=0; i<bottom_Y; i++)
			{
				JPSDR_AddBorder_Fill32(dst,image_data.dst_w0>>1,data32);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			break;
		case VMODE_PLANAR_YUV444 :
		case VMODE_PLANAR_YUV422 :
		case VMODE_PLANAR_YUV420 :
		case VMODE_PLANAR_YUV411 :
		case VMODE_PLANAR_YUV410 :
			src=image_data.src_plane0;
			dst=image_data.dst_plane0;
			for (int32_t i=0; i<top_Y; i++)
			{
				memset(dst,y_fill,image_data.dst_w0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			for (int32_t i=0; i<image_data.src_h0; i++)
			{
				memset(dst,y_fill,left_Y);
				dst_=(void *)((uint8_t *)dst+left_Y);
				memcpy(dst_,src,image_data.src_w0);
				src=(void *)((uint8_t *)src+image_data.src_pitch0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
				dst_=(void *)((uint8_t *)dst_+image_data.src_w0);
				memset(dst_,y_fill,right_Y);
			}
			for (int32_t i=0; i<bottom_Y; i++)
			{
				memset(dst,y_fill,image_data.dst_w0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}

			src=image_data.src_plane1;
			dst=image_data.dst_plane1;
			for (int32_t i=0; i<top_UV; i++)
			{
				memset(dst,u_fill,image_data.dst_w1);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch1);
			}
			for (int32_t i=0; i<image_data.src_h1; i++)
			{
				memset(dst,u_fill,left_UV);
				dst_=(void *)((uint8_t *)dst+left_UV);
				memcpy(dst_,src,image_data.src_w1);
				src=(void *)((uint8_t *)src+image_data.src_pitch1);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch1);
				dst_=(void *)((uint8_t *)dst_+image_data.src_w1);
				memset(dst_,u_fill,right_UV);
			}
			for (int32_t i=0; i<bottom_UV; i++)
			{
				memset(dst,u_fill,image_data.dst_w1);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch1);
			}

			src=image_data.src_plane2;
			dst=image_data.dst_plane2;
			for (int32_t i=0; i<top_UV; i++)
			{
				memset(dst,v_fill,image_data.dst_w2);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch2);
			}
			for (int32_t i=0; i<image_data.src_h2; i++)
			{
				memset(dst,v_fill,left_UV);
				dst_=(void *)((uint8_t *)dst+left_UV);
				memcpy(dst_,src,image_data.src_w2);
				src=(void *)((uint8_t *)src+image_data.src_pitch2);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch2);
				dst_=(void *)((uint8_t *)dst_+image_data.src_w2);
				memset(dst_,v_fill,right_UV);
			}
			for (int32_t i=0; i<bottom_UV; i++)
			{
				memset(dst,v_fill,image_data.dst_w2);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch2);
			}
			break;
		case VMODE_Y :
			src=image_data.src_plane0;
			dst=image_data.dst_plane0;
			for (int32_t i=0; i<top_Y; i++)
			{
				memset(dst,y_fill,image_data.dst_w0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			for (int32_t i=0; i<image_data.src_h0; i++)
			{
				memset(dst,y_fill,left_Y);
				dst_=(void *)((uint8_t *)dst+left_Y);
				memcpy(dst_,src,image_data.src_w0);
				src=(void *)((uint8_t *)src+image_data.src_pitch0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
				dst_=(void *)((uint8_t *)dst_+image_data.src_w0);
				memset(dst_,y_fill,right_Y);
			}
			for (int32_t i=0; i<bottom_Y; i++)
			{
				memset(dst,y_fill,image_data.dst_w0);
				dst=(void *)((uint8_t *)dst+image_data.dst_pitch0);
			}
			break;
		default : break;
	}
}





void JPSDR_AddBorder::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.top=argv[0].asInt();
	mData.bottom=argv[1].asInt();
	mData.left=argv[2].asInt();
	mData.right=argv[3].asInt();
	mData.red=argv[4].asInt();
	mData.green=argv[5].asInt();
	mData.blue=argv[6].asInt();
}

bool JPSDR_AddBorder::Configure(VDXHWND hwnd)
{
	JPSDR_AddBorderDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_AddBorder::GetScriptString(char *buf, int maxlen)
{
 
  
    SafePrintf(buf, maxlen, "Config(%d, %d, %d, %d, %d, %d, %d)",
		mData.top,mData.bottom,mData.left,mData.right,mData.red,mData.green,mData.blue);
}



extern VDXFilterDefinition2 filterDef_JPSDR_AddBorder=
VDXVideoFilterDefinition<JPSDR_AddBorder>("JPSDR","AddBorder v1.4.3","Add boder to picture.");

