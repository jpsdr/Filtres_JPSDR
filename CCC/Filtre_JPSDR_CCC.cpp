#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "CCC_resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Pascal.h"
#include "..\Filtres_JPSDR\JPSDR_Filter.h"

extern int g_VFVAPIVersion;

extern "C" void JPSDR_CCC_Planar_SSE(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w);
extern "C" void JPSDR_CCC_Planar_SSE_FR(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w);

#define CLP_FR(x) ((x > 255) ? 255 : x)
#define ZRC_FR(x) ((x < 0) ? 0 : x)

#define CLP(x) ((x > 235) ? 235 : x)
#define ZRC(x) ((x < 16) ? 16 : x)


class JPSDR_CCCData
{
public:
	uint8_t phase;

	JPSDR_CCCData(void);
};


JPSDR_CCCData::JPSDR_CCCData(void)
{
	phase=0;
}



class JPSDR_CCCDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_CCCDialog(JPSDR_CCCData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_CCC),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_CCCData& mData;
	JPSDR_CCCData moldData;
	IVDXFilterPreview2 *const mifp;
};


INT_PTR JPSDR_CCCDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_CCCDialog::OnInit()
{
	moldData=mData;

	SetDlgItemInt(mhdlg,IDC_PHASE,mData.phase,FALSE);
	
	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}


void JPSDR_CCCDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}

bool JPSDR_CCCDialog::SaveToData()
{
	uint8_t data2;
	BOOL success;

	data2=GetDlgItemInt(mhdlg,IDC_PHASE,&success,FALSE);
	if (success) mData.phase=data2;
	if (mData.phase>5) mData.phase=5;

	return true;
}



bool JPSDR_CCCDialog::OnCommand(int cmd)
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
		case IDC_PHASE :
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;			
	}
		
	return false;
}



class JPSDR_CCC : public VDXVideoFilter
{
public:
	JPSDR_CCC(){}
	JPSDR_CCC(const JPSDR_CCC& a)
	{
		SSE2_Enable = a.SSE2_Enable;
		mData=a.mData;
	}
	virtual bool Init();
	virtual uint32 GetParams();
	virtual void Start();
	virtual void Run();
	virtual bool Configure(VDXHWND hwnd);
	virtual void GetSettingString(char *buf, int maxlen);
	virtual void GetScriptString(char *buf, int maxlen);
	
	VDXVF_DECLARE_SCRIPT_METHODS();
	
protected:
	Image_Data image_data;
	int16_t lookup[1535];
	bool SSE2_Enable;

	void CCC_Planar(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_YUY2(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_UYVY(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_Planar_FR(const void *src, void *dst, const ptrdiff_t src_pitch,const  ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_YUY2_FR(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_UYVY_FR(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_Planar_SSE(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	void CCC_Planar_SSE_FR(const void *src, void *dst, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h, const uint8_t phase);
	
	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_CCCData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_CCC)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_CCC,ScriptConfig,"i")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_CCC::Init()
{
	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	return(true);
}


uint32 JPSDR_CCC::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;
	
	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;
	
	switch(pxsrc.format)
	{
/*		case nsVDXPixmap::kPixFormat_XRGB1555 :
		case nsVDXPixmap::kPixFormat_RGB565 :
		case nsVDXPixmap::kPixFormat_RGB888 :			
		case nsVDXPixmap::kPixFormat_XRGB8888 :*/
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
		case nsVDXPixmap::kPixFormat_VDXA_YUV :			*/			
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_NONE;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_Y8 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_NONE;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			image_data.src_full_mode=false;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT601;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :				
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			image_data.src_full_mode=true;
			image_data.src_color_matrix=MATRIX_BT709;
			image_data.src_interlaced=INTERLACED_TFF;
			break;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;

	switch(pxdst.format)
	{
		case nsVDXPixmap::kPixFormat_XRGB8888 :
		case nsVDXPixmap::kPixFormat_Y8_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_NONE;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_Y8 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_NONE;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY :
		case nsVDXPixmap::kPixFormat_YUV444_Planar :
		case nsVDXPixmap::kPixFormat_YUV422_Planar :
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV411_Planar :
		case nsVDXPixmap::kPixFormat_YUV410_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :				
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			image_data.dst_full_mode=false;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT601;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
		case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :				
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_NONE;
			break;
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED;
			break;
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_BFF;
			break;
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			image_data.dst_full_mode=true;
			image_data.dst_color_matrix=MATRIX_BT709;
			image_data.dst_interlaced=INTERLACED_TFF;
			break;
	}

	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM|FILTERPARAM_SWAP_BUFFERS|FILTERPARAM_ALIGN_SCANLINES);

}



void JPSDR_CCC::CCC_Planar(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const uint8_t *src,*src_orig;
	uint8_t *dst,*dst_orig;
	const ptrdiff_t src2=2*src_pitch,src4=4*src_pitch,src5=5*src_pitch,src6=6*src_pitch,src7=7*src_pitch;
	const ptrdiff_t dst2=2*dst_pitch,dst3=3*dst_pitch,dst4=4*dst_pitch,dst5=5*dst_pitch;
	const int32_t trackbar=phase;
	const int32_t h_7=h-7+trackbar;
	
	src_orig=(uint8_t *)src_;
	dst_orig=(uint8_t *)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=src_orig+((i-trackbar)*src_pitch);
		dst=dst_orig+((i-trackbar)*dst_pitch);
		memcpy(dst,src,w);
		for (int32_t j=0; j<w; j++)
		{
			int16_t calc1,calc2,calc3,calc4,calc5,calc6;
			uint16_t src_0,src_2,src_3,src_4,src_5,src_7;

			src_0=*(src-src_pitch);
			calc3=(int16_t)*src;
			src_2=*(src+src_pitch);
			src_3=*(src+src2);
			src_4=*(src+src4);
			src_5=*(src+src5);
			calc4=(int16_t)*(src+src6);
			src_7=*(src+src7);

			calc1=lookup[src_2+256]-lookup[src_0+512];
			calc2=lookup[src_5+256]-lookup[src_7+512];
			calc5=(lookup[src_3]+lookup[src_4]-calc3-calc4+2) >> 2;
			calc6=lookup[calc5+1151];
			calc3=lookup[(uint16_t)calc3+768];
			calc4=lookup[(uint16_t)calc4+768];
			
			*(dst+dst_pitch)=(uint8_t)ZRC(CLP((calc1+calc3+32) >> 6));
			*(dst+dst2)=(uint8_t)ZRC(CLP((calc1+calc6+32) >> 6));
			*(dst+dst3)=(uint8_t)ZRC(CLP(calc5));
			*(dst+dst4)=(uint8_t)ZRC(CLP((calc2+calc6+32) >> 6));
			*(dst+dst5)=(uint8_t)ZRC(CLP((calc2+calc4+32) >> 6));
			
			src++;
			dst++;
		}
	}

	src=src_orig+((i-trackbar)*src_pitch);
	dst=dst_orig+((i-trackbar)*dst_pitch);
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

}


void JPSDR_CCC::CCC_Planar_SSE(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const uint8_t *src,*src_orig;
	uint8_t *dst,*dst_orig;
	const int32_t trackbar=phase,w2=(w+3)>>2;
	const int32_t h_7=h-7+trackbar;

	src_orig=(uint8_t *)src_;
	dst_orig=(uint8_t *)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=src_orig+((i-trackbar)*src_pitch);
		dst=dst_orig+((i-trackbar)*dst_pitch);
		memcpy(dst,src,w);
		JPSDR_CCC_Planar_SSE(src,dst,src_pitch,dst_pitch,w2);
	}


	src=src_orig+((i-trackbar)*src_pitch);
	dst=dst_orig+((i-trackbar)*dst_pitch);
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

}



void JPSDR_CCC::CCC_YUY2(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const YUYV *src,*src_orig;
	YUYV *dst,*dst_orig;
	const ptrdiff_t src2=2*src_pitch,src3=3*src_pitch,src4=4*src_pitch,src5=5*src_pitch,src6=6*src_pitch,src7=7*src_pitch;
	const ptrdiff_t dst2=2*dst_pitch,dst3=3*dst_pitch,dst4=4*dst_pitch,dst5=5*dst_pitch;
	const int32_t trackbar=phase;
	const int32_t h_7=h-7+trackbar;
	const int32_t w2=4*w;

	src_orig=(YUYV*)src_;
	dst_orig=(YUYV*)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w2);
		src=(YUYV*)((uint8_t *)src+src_pitch);
		dst=(YUYV*)((uint8_t *)dst+dst_pitch);
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=(YUYV*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
		dst=(YUYV*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
		memcpy(dst,src,w2);
		for (int32_t j=0; j<w; j++)
		{
			YUYV *src0,*src1,*dst0;
			int16_t calc1_1,calc2_1,calc3_1,calc4_1,calc5_1,calc6_1;
			int16_t calc1_2,calc2_2,calc3_2,calc4_2,calc5_2,calc6_2;
			uint8_t u1,u2,u3,u4,u5;
			uint8_t v1,v2,v3,v4,v5;

			src0=(YUYV*)((uint8_t *)src+src_pitch);
			src1=(YUYV*)((uint8_t *)src-src_pitch);
			calc1_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			u1=src0->u;
			calc1_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			v1=src0->v;
			calc3_1=(int16_t)src->y1;
			calc3_2=(int16_t)src->y2;
			src0=(YUYV*)((uint8_t *)src+src5);
			src1=(YUYV*)((uint8_t *)src+src7);
			calc2_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			u5=src0->u;
			calc2_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			v5=src0->v;
			src0=(YUYV*)((uint8_t *)src+src6);
			calc4_1=(int16_t)src0->y1;
			calc4_2=(int16_t)src0->y2;
			src0=(YUYV*)((uint8_t *)src+src2);
			src1=(YUYV*)((uint8_t *)src+src4);
			calc5_1=(lookup[src0->y1]+lookup[src1->y1]-calc3_1-calc4_1+2) >> 2;
			u2=src0->u;
			v2=src0->v;
			u4=src1->u;
			v4=src1->v;
			calc5_2=(lookup[src0->y2]+lookup[src1->y2]-calc3_2-calc4_2+2) >> 2;
			src0=(YUYV*)((uint8_t *)src+src3);
			u3=src0->u;
			v3=src0->v;
			calc6_1=lookup[calc5_1+1151];
			calc3_1=lookup[(uint16_t)calc3_1+768];
			calc4_1=lookup[(uint16_t)calc4_1+768];
			calc6_2=lookup[calc5_2+1151];
			calc3_2=lookup[(uint16_t)calc3_2+768];
			calc4_2=lookup[(uint16_t)calc4_2+768];
			
			dst0=(YUYV*)((uint8_t *)dst+dst_pitch);
			dst0->y1=(uint8_t)ZRC(CLP((calc1_1+calc3_1+32) >> 6));
			dst0->u=u1;
			dst0->y2=(uint8_t)ZRC(CLP((calc1_2+calc3_2+32) >> 6));
			dst0->v=v1;
			dst0=(YUYV*)((uint8_t *)dst+dst2);
			dst0->y1=(uint8_t)ZRC(CLP((calc1_1+calc6_1+32) >> 6));
			dst0->u=u2;
			dst0->y2=(uint8_t)ZRC(CLP((calc1_2+calc6_2+32) >> 6));
			dst0->v=v2;
			dst0=(YUYV*)((uint8_t *)dst+dst3);
			dst0->y1=(uint8_t)ZRC(CLP(calc5_1));
			dst0->u=u3;
			dst0->y2=(uint8_t)ZRC(CLP(calc5_2));
			dst0->v=v3;
			dst0=(YUYV*)((uint8_t *)dst+dst4);
			dst0->y1=(uint8_t)ZRC(CLP((calc2_1+calc6_1+32) >> 6));
			dst0->u=u4;
			dst0->y2=(uint8_t)ZRC(CLP((calc2_2+calc6_2+32) >> 6));
			dst0->v=v4;
			dst0=(YUYV*)((uint8_t *)dst+dst5);
			dst0->y1=(uint8_t)ZRC(CLP((calc2_1+calc4_1+32) >> 6));
			dst0->u=u5;
			dst0->y2=(uint8_t)ZRC(CLP((calc2_2+calc4_2+32) >> 6));
			dst0->v=v5;
			src++;
			dst++;
		}
	}


	src=(YUYV*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
	dst=(YUYV*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w2);
		src=(YUYV*)((uint8_t *)src+src_pitch);
		dst=(YUYV*)((uint8_t *)dst+dst_pitch);
	}

}


void JPSDR_CCC::CCC_UYVY(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const UYVY *src,*src_orig;
	UYVY *dst,*dst_orig;
	const ptrdiff_t src2=2*src_pitch,src3=3*src_pitch,src4=4*src_pitch,src5=5*src_pitch,src6=6*src_pitch,src7=7*src_pitch;
	const ptrdiff_t dst2=2*dst_pitch,dst3=3*dst_pitch,dst4=4*dst_pitch,dst5=5*dst_pitch;
	const int32_t trackbar=phase;
	const int32_t h_7=h-7+trackbar;
	const int32_t w2=4*w;
	
	src_orig=(UYVY*)src_;
	dst_orig=(UYVY*)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w2);
		src=(UYVY*)((uint8_t *)src+src_pitch);
		dst=(UYVY*)((uint8_t *)dst+dst_pitch);
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=(UYVY*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
		dst=(UYVY*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
		memcpy(dst,src,w2);
		for (int32_t j=0; j<w; j++)
		{
			UYVY *src0,*src1,*dst0;
			int16_t calc1_1,calc2_1,calc3_1,calc4_1,calc5_1,calc6_1;
			int16_t calc1_2,calc2_2,calc3_2,calc4_2,calc5_2,calc6_2;
			uint8_t u1,u2,u3,u4,u5;
			uint8_t v1,v2,v3,v4,v5;

			src0=(UYVY*)((uint8_t *)src+src_pitch);
			src1=(UYVY*)((uint8_t *)src-src_pitch);
			u1=src0->u;
			calc1_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			v1=src0->v;
			calc1_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			calc3_1=(int16_t)src->y1;
			calc3_2=(int16_t)src->y2;
			src0=(UYVY*)((uint8_t *)src+src5);
			src1=(UYVY*)((uint8_t *)src+src7);
			u5=src0->u;
			calc2_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			v5=src0->v;
			calc2_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			src0=(UYVY*)((uint8_t *)src+src6);
			calc4_1=(int16_t)src0->y1;
			calc4_2=(int16_t)src0->y2;
			src0=(UYVY*)((uint8_t *)src+src2);
			src1=(UYVY*)((uint8_t *)src+src4);
			u2=src0->u;
			v2=src0->v;
			u4=src1->u;
			v4=src1->v;
			calc5_1=(lookup[src0->y1]+lookup[src1->y1]-calc3_1-calc4_1+2) >> 2;
			calc5_2=(lookup[src0->y2]+lookup[src1->y2]-calc3_2-calc4_2+2) >> 2;
			src0=(UYVY*)((uint8_t *)src+src3);
			u3=src0->u;
			v3=src0->v;
			calc6_1=lookup[calc5_1+1151];
			calc3_1=lookup[(uint16_t)calc3_1+768];
			calc4_1=lookup[(uint16_t)calc4_1+768];
			calc6_2=lookup[calc5_2+1151];
			calc3_2=lookup[(uint16_t)calc3_2+768];
			calc4_2=lookup[(uint16_t)calc4_2+768];
			
			dst0=(UYVY*)((uint8_t *)dst+dst_pitch);
			dst0->u=u1;
			dst0->y1=(uint8_t)ZRC(CLP((calc1_1+calc3_1+32) >> 6));
			dst0->v=v1;
			dst0->y2=(uint8_t)ZRC(CLP((calc1_2+calc3_2+32) >> 6));
			dst0=(UYVY*)((uint8_t *)dst+dst2);
			dst0->u=u2;
			dst0->y1=(uint8_t)ZRC(CLP((calc1_1+calc6_1+32) >> 6));
			dst0->v=v2;
			dst0->y2=(uint8_t)ZRC(CLP((calc1_2+calc6_2+32) >> 6));
			dst0=(UYVY*)((uint8_t *)dst+dst3);
			dst0->u=u3;
			dst0->y1=(uint8_t)ZRC(CLP(calc5_1));
			dst0->v=v3;
			dst0->y2=(uint8_t)ZRC(CLP(calc5_2));
			dst0=(UYVY*)((uint8_t *)dst+dst4);
			dst0->u=u4;
			dst0->y1=(uint8_t)ZRC(CLP((calc2_1+calc6_1+32) >> 6));
			dst0->v=v4;
			dst0->y2=(uint8_t)ZRC(CLP((calc2_2+calc6_2+32) >> 6));
			dst0=(UYVY*)((uint8_t *)dst+dst5);
			dst0->u=u5;
			dst0->y1=(uint8_t)ZRC(CLP((calc2_1+calc4_1+32) >> 6));
			dst0->v=v5;
			dst0->y2=(uint8_t)ZRC(CLP((calc2_2+calc4_2+32) >> 6));
			src++;
			dst++;
		}
	}


	src=(UYVY*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
	dst=(UYVY*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w2);
		src=(UYVY*)((uint8_t *)src+src_pitch);
		dst=(UYVY*)((uint8_t *)dst+dst_pitch);
	}

}


void JPSDR_CCC::CCC_Planar_FR(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const uint8_t *src,*src_orig;
	uint8_t *dst,*dst_orig;
	const ptrdiff_t src2=2*src_pitch,src4=4*src_pitch,src5=5*src_pitch,src6=6*src_pitch,src7=7*src_pitch;
	const ptrdiff_t dst2=2*dst_pitch,dst3=3*dst_pitch,dst4=4*dst_pitch,dst5=5*dst_pitch;
	const int32_t trackbar=phase;
	const int32_t h_7=h-7+trackbar;
	
	src_orig=(uint8_t*)src_;
	dst_orig=(uint8_t*)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=src_orig+((i-trackbar)*src_pitch);
		dst=dst_orig+((i-trackbar)*dst_pitch);
		memcpy(dst,src,w);
		for (int32_t j=0; j<w; j++)
		{
			int16_t calc1,calc2,calc3,calc4,calc5,calc6;
			uint16_t src_0,src_2,src_3,src_4,src_5,src_7;

			src_0=*(src-src_pitch);
			calc3=(int16_t)*src;
			src_2=*(src+src_pitch);
			src_3=*(src+src2);
			src_4=*(src+src4);
			src_5=*(src+src5);
			calc4=(int16_t)*(src+src6);
			src_7=*(src+src7);

			calc1=lookup[src_2+256]-lookup[src_0+512];
			calc2=lookup[src_5+256]-lookup[src_7+512];
			calc5=(lookup[src_3]+lookup[src_4]-calc3-calc4+2) >> 2;
			calc6=lookup[calc5+1151];
			calc3=lookup[(uint16_t)calc3+768];
			calc4=lookup[(uint16_t)calc4+768];
			
			*(dst+dst_pitch)=(uint8_t)ZRC_FR(CLP_FR((calc1+calc3+32) >> 6));
			*(dst+dst2)=(uint8_t)ZRC_FR(CLP_FR((calc1+calc6+32) >> 6));
			*(dst+dst3)=(uint8_t)ZRC_FR(CLP_FR(calc5));
			*(dst+dst4)=(uint8_t)ZRC_FR(CLP_FR((calc2+calc6+32) >> 6));
			*(dst+dst5)=(uint8_t)ZRC_FR(CLP_FR((calc2+calc4+32) >> 6));
			
			src++;
			dst++;
		}
	}


	src=src_orig+((i-trackbar)*src_pitch);
	dst=dst_orig+((i-trackbar)*dst_pitch);
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

}


void JPSDR_CCC::CCC_Planar_SSE_FR(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const uint8_t *src,*src_orig;
	uint8_t *dst,*dst_orig;
	const int32_t trackbar=phase,w2=(w+3)>>2;
	const int32_t h_7=h-7+trackbar;
	
	src_orig=(uint8_t *)src_;
	dst_orig=(uint8_t *)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=src_orig+((i-trackbar)*src_pitch);
		dst=dst_orig+((i-trackbar)*dst_pitch);
		memcpy(dst,src,w);
		JPSDR_CCC_Planar_SSE_FR(src,dst,src_pitch,dst_pitch,w2);
	}

	src=src_orig+((i-trackbar)*src_pitch);
	dst=dst_orig+((i-trackbar)*dst_pitch);
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w);
		src+=src_pitch;
		dst+=dst_pitch;
	}

}



void JPSDR_CCC::CCC_YUY2_FR(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const YUYV *src,*src_orig;
	YUYV *dst,*dst_orig;
	const ptrdiff_t src2=2*src_pitch,src3=3*src_pitch,src4=4*src_pitch,src5=5*src_pitch,src6=6*src_pitch,src7=7*src_pitch;
	const ptrdiff_t dst2=2*dst_pitch,dst3=3*dst_pitch,dst4=4*dst_pitch,dst5=5*dst_pitch;
	const int32_t trackbar=phase;
	const int32_t h_7=h-7+trackbar;
	const int32_t w2=4*w;

	src_orig=(YUYV*)src_;
	dst_orig=(YUYV*)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w2);
		src=(YUYV*)((uint8_t *)src+src_pitch);
		dst=(YUYV*)((uint8_t *)dst+dst_pitch);
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=(YUYV*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
		dst=(YUYV*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
		memcpy(dst,src,w2);
		for (int32_t j=0; j<w; j++)
		{
			YUYV *src0,*src1,*dst0;
			int16_t calc1_1,calc2_1,calc3_1,calc4_1,calc5_1,calc6_1;
			int16_t calc1_2,calc2_2,calc3_2,calc4_2,calc5_2,calc6_2;
			uint8_t u1,u2,u3,u4,u5;
			uint8_t v1,v2,v3,v4,v5;

			src0=(YUYV*)((uint8_t *)src+src_pitch);
			src1=(YUYV*)((uint8_t *)src-src_pitch);
			calc1_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			u1=src0->u;
			calc1_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			v1=src0->v;
			calc3_1=(int16_t)src->y1;
			calc3_2=(int16_t)src->y2;
			src0=(YUYV*)((uint8_t *)src+src5);
			src1=(YUYV*)((uint8_t *)src+src7);
			calc2_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			u5=src0->u;
			calc2_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			v5=src0->v;
			src0=(YUYV*)((uint8_t *)src+src6);
			calc4_1=(int16_t)src0->y1;
			calc4_2=(int16_t)src0->y2;
			src0=(YUYV*)((uint8_t *)src+src2);
			src1=(YUYV*)((uint8_t *)src+src4);
			calc5_1=(lookup[src0->y1]+lookup[src1->y1]-calc3_1-calc4_1+2) >> 2;
			u2=src0->u;
			v2=src0->v;
			u4=src1->u;
			v4=src1->v;
			calc5_2=(lookup[src0->y2]+lookup[src1->y2]-calc3_2-calc4_2+2) >> 2;
			src0=(YUYV*)((uint8_t *)src+src3);
			u3=src0->u;
			v3=src0->v;
			calc6_1=lookup[calc5_1+1151];
			calc3_1=lookup[(uint16_t)calc3_1+768];
			calc4_1=lookup[(uint16_t)calc4_1+768];
			calc6_2=lookup[calc5_2+1151];
			calc3_2=lookup[(uint16_t)calc3_2+768];
			calc4_2=lookup[(uint16_t)calc4_2+768];
			
			dst0=(YUYV*)((uint8_t *)dst+dst_pitch);
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc1_1+calc3_1+32) >> 6));
			dst0->u=u1;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc1_2+calc3_2+32) >> 6));
			dst0->v=v1;
			dst0=(YUYV*)((uint8_t *)dst+dst2);
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc1_1+calc6_1+32) >> 6));
			dst0->u=u2;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc1_2+calc6_2+32) >> 6));
			dst0->v=v2;
			dst0=(YUYV*)((uint8_t *)dst+dst3);
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR(calc5_1));
			dst0->u=u3;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR(calc5_2));
			dst0->v=v3;
			dst0=(YUYV*)((uint8_t *)dst+dst4);
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc2_1+calc6_1+32) >> 6));
			dst0->u=u4;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc2_2+calc6_2+32) >> 6));
			dst0->v=v4;
			dst0=(YUYV*)((uint8_t *)dst+dst5);
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc2_1+calc4_1+32) >> 6));
			dst0->u=u5;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc2_2+calc4_2+32) >> 6));
			dst0->v=v5;
			src++;
			dst++;
		}
	}


	src=(YUYV*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
	dst=(YUYV*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w2);
		src=(YUYV*)((uint8_t *)src+src_pitch);
		dst=(YUYV*)((uint8_t *)dst+dst_pitch);
	}

}



void JPSDR_CCC::CCC_UYVY_FR(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h,
	const uint8_t phase)
{
	const UYVY *src,*src_orig;
	UYVY *dst,*dst_orig;
	const ptrdiff_t src2=2*src_pitch,src3=3*src_pitch,src4=4*src_pitch,src5=5*src_pitch,src6=6*src_pitch,src7=7*src_pitch;
	const ptrdiff_t dst2=2*dst_pitch,dst3=3*dst_pitch,dst4=4*dst_pitch,dst5=5*dst_pitch;
	const int32_t trackbar=phase;
	const int32_t h_7=h-7+trackbar;
	const int32_t w2=4*w;
	
	src_orig=(UYVY*)src_;
	dst_orig=(UYVY*)dst_;
	src=src_orig;
	dst=dst_orig;
	
	for (int32_t i=0; i<(6-trackbar); i++)
	{
		memcpy(dst,src,w2);
		src=(UYVY*)((uint8_t *)src+src_pitch);
		dst=(UYVY*)((uint8_t *)dst+dst_pitch);
	}

	int32_t i;
	for (i=6; i<h_7; i+=6)
	{
		src=(UYVY*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
		dst=(UYVY*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
		memcpy(dst,src,w2);
		for (int32_t j=0; j<w; j++)
		{
			UYVY *src0,*src1,*dst0;
			int16_t calc1_1,calc2_1,calc3_1,calc4_1,calc5_1,calc6_1;
			int16_t calc1_2,calc2_2,calc3_2,calc4_2,calc5_2,calc6_2;
			uint8_t u1,u2,u3,u4,u5;
			uint8_t v1,v2,v3,v4,v5;

			src0=(UYVY*)((uint8_t *)src+src_pitch);
			src1=(UYVY*)((uint8_t *)src-src_pitch);
			u1=src0->u;
			calc1_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			v1=src0->v;
			calc1_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			calc3_1=(int16_t)src->y1;
			calc3_2=(int16_t)src->y2;
			src0=(UYVY*)((uint8_t *)src+src5);
			src1=(UYVY*)((uint8_t *)src+src7);
			u5=src0->u;
			calc2_1=lookup[(uint16_t)src0->y1+256]-lookup[(uint16_t)src1->y1+512];
			v5=src0->v;
			calc2_2=lookup[(uint16_t)src0->y2+256]-lookup[(uint16_t)src1->y2+512];
			src0=(UYVY*)((uint8_t *)src+src6);
			calc4_1=(int16_t)src0->y1;
			calc4_2=(int16_t)src0->y2;
			src0=(UYVY*)((uint8_t *)src+src2);
			src1=(UYVY*)((uint8_t *)src+src4);
			u2=src0->u;
			v2=src0->v;
			u4=src1->u;
			v4=src1->v;
			calc5_1=(lookup[src0->y1]+lookup[src1->y1]-calc3_1-calc4_1+2) >> 2;
			calc5_2=(lookup[src0->y2]+lookup[src1->y2]-calc3_2-calc4_2+2) >> 2;
			src0=(UYVY*)((uint8_t *)src+src3);
			u3=src0->u;
			v3=src0->v;
			calc6_1=lookup[calc5_1+1151];
			calc3_1=lookup[(uint16_t)calc3_1+768];
			calc4_1=lookup[(uint16_t)calc4_1+768];
			calc6_2=lookup[calc5_2+1151];
			calc3_2=lookup[(uint16_t)calc3_2+768];
			calc4_2=lookup[(uint16_t)calc4_2+768];
			
			dst0=(UYVY*)((uint8_t *)dst+dst_pitch);
			dst0->u=u1;
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc1_1+calc3_1+32) >> 6));
			dst0->v=v1;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc1_2+calc3_2+32) >> 6));
			dst0=(UYVY*)((uint8_t *)dst+dst2);
			dst0->u=u2;
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc1_1+calc6_1+32) >> 6));
			dst0->v=v2;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc1_2+calc6_2+32) >> 6));
			dst0=(UYVY*)((uint8_t *)dst+dst3);
			dst0->u=u3;
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR(calc5_1));
			dst0->v=v3;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR(calc5_2));
			dst0=(UYVY*)((uint8_t *)dst+dst4);
			dst0->u=u4;
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc2_1+calc6_1+32) >> 6));
			dst0->v=v4;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc2_2+calc6_2+32) >> 6));
			dst0=(UYVY*)((uint8_t *)dst+dst5);
			dst0->u=u5;
			dst0->y1=(uint8_t)ZRC_FR(CLP_FR((calc2_1+calc4_1+32) >> 6));
			dst0->v=v5;
			dst0->y2=(uint8_t)ZRC_FR(CLP_FR((calc2_2+calc4_2+32) >> 6));
			src++;
			dst++;
		}
	}

	src=(UYVY*)((uint8_t *)src_orig+((i-trackbar)*src_pitch));
	dst=(UYVY*)((uint8_t *)dst_orig+((i-trackbar)*dst_pitch));
	for (int32_t j=i-trackbar; j<h; j++)
	{
		memcpy(dst,src,w2);
		src=(UYVY*)((uint8_t *)src+src_pitch);
		dst=(UYVY*)((uint8_t *)dst+dst_pitch);
	}

}

	
static inline void MovePlane(const void *src_, void *dst_, const ptrdiff_t src_pitch, const ptrdiff_t dst_pitch, const int32_t w, const int32_t h)
{
	const uint8_t *src=(uint8_t *)src_;
	uint8_t *dst=(uint8_t *)dst_;

	if ((src_pitch==dst_pitch) && (abs(src_pitch)==w))
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
		
	

void JPSDR_CCC::Start()
{
	Image_Data idata;
	int16_t i;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
		return;
	}
	
	const VDXPixmapLayout& pxdst=*fa->dst.mpPixmapLayout;
	const VDXPixmapLayout& pxsrc=*fa->src.mpPixmapLayout;

		idata.src_h0=pxsrc.h;
		idata.src_w0=pxsrc.w;
		idata.src_pitch0=pxsrc.pitch;
		idata.src_pitch1=pxsrc.pitch2;
		idata.src_pitch2=pxsrc.pitch3;

		idata.dst_h0=pxdst.h;
		idata.dst_w0=pxdst.w;
		idata.dst_pitch0=pxdst.pitch;
		idata.dst_pitch1=pxdst.pitch2;
		idata.dst_pitch2=pxdst.pitch3;
		
		switch(pxsrc.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.src_video_mode=1;
				idata.video_mode=1;
				idata.src_line0=4*idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_line0=4*((idata.src_w0+1)>>1);
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_line0=idata.src_w0;
				idata.src_line1=idata.src_w1;
				idata.src_line2=idata.src_w2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_line0=idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :			
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_line0=idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_line1=0;
				idata.src_line2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				idata.src_bits_pixel=8;
				break;
		}
		idata.src_modulo0=idata.src_pitch0-idata.src_line0;
		idata.src_pitch0_al=(((ptrdiff_t)idata.src_line0+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		idata.src_modulo0_al=idata.src_pitch0_al-idata.src_line0;
		idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_line0;
		idata.src_size0_al=(size_t)idata.src_h0*(size_t)idata.src_pitch0_al;
		idata.src_modulo1=idata.src_pitch1-idata.src_line1;
		idata.src_pitch1_al=(((ptrdiff_t)idata.src_line1+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		idata.src_modulo1_al=idata.src_pitch1_al-idata.src_line1;
		idata.src_size1=(size_t)idata.src_h1*(size_t)idata.src_line1;
		idata.src_size1_al=(size_t)idata.src_h1*(size_t)idata.src_pitch1_al;
		idata.src_modulo2=idata.src_pitch2-idata.src_line2;
		idata.src_pitch2_al=(((ptrdiff_t)idata.src_line2+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
		idata.src_modulo2_al=idata.src_pitch2_al-idata.src_line2;
		idata.src_size2=(size_t)idata.src_h2*(size_t)idata.src_line2;
		idata.src_size2_al=(size_t)idata.src_h2*(size_t)idata.src_pitch2_al;

		switch(pxdst.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.dst_video_mode=1;
				idata.dst_line0=4*idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :			
				idata.dst_video_mode=2;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :			
				idata.dst_video_mode=3;
				idata.dst_line0=4*((idata.dst_w0+1)>>1);
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :			
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :			
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :			
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :			
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :			
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_line0=idata.dst_w0;
				idata.dst_line1=idata.dst_w1;
				idata.dst_line2=idata.dst_w2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.dst_video_mode=9;
				idata.dst_line0=idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :						
				idata.dst_video_mode=9;
				idata.dst_line0=idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_line1=0;
				idata.dst_line2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;
				idata.dst_bits_pixel=8;
				break;
		}
	idata.dst_modulo0=idata.dst_pitch0-idata.dst_line0;
	idata.dst_pitch0_al=(((ptrdiff_t)idata.dst_line0+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	idata.dst_modulo0_al=idata.dst_pitch0_al-idata.dst_line0;
	idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_line0;
	idata.src_size0_al=(size_t)idata.dst_h0*(size_t)idata.dst_pitch0_al;
	idata.dst_modulo1=idata.dst_pitch1-idata.dst_line1;
	idata.dst_pitch1_al=(((ptrdiff_t)idata.dst_line1+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	idata.dst_modulo1_al=idata.dst_pitch1_al-idata.dst_line1;
	idata.dst_size1=(size_t)idata.dst_h1*(size_t)idata.dst_line1;
	idata.src_size1_al=(size_t)idata.dst_h1*(size_t)idata.dst_pitch1_al;
	idata.dst_modulo2=idata.dst_pitch2-idata.dst_line2;
	idata.dst_pitch2_al=(((ptrdiff_t)idata.dst_line2+ALIGN_SIZE-1)>>ALIGN_SHIFT)<<ALIGN_SHIFT;
	idata.dst_modulo2_al=idata.dst_pitch2_al-idata.dst_line2;
	idata.dst_size2=(size_t)idata.dst_h2*(size_t)idata.dst_line2;
	idata.src_size2_al=(size_t)idata.dst_h2*(size_t)idata.dst_pitch2_al;

	if (idata.src_bits_pixel<=16)
	{
		if (idata.src_full_mode)
		{
			idata.src_Y_Min=0;
			idata.src_U_Min=0;
			idata.src_V_Min=0;

			idata.src_Y_Max=(uint16_t)(((int)1 << idata.src_bits_pixel)-1);
			idata.src_U_Max=(uint16_t)(((int)1 << idata.src_bits_pixel)-1);
			idata.src_V_Max=(uint16_t)(((int)1 << idata.src_bits_pixel)-1);
		}
		else
		{
			idata.src_Y_Min=(uint16_t)((int)16 << (idata.src_bits_pixel-8));
			idata.src_U_Min=(uint16_t)((int)16 << (idata.src_bits_pixel-8));
			idata.src_V_Min=(uint16_t)((int)16 << (idata.src_bits_pixel-8));
			idata.src_Y_Max=(uint16_t)((int)235 << (idata.src_bits_pixel-8));
			idata.src_U_Max=(uint16_t)((int)240 << (idata.src_bits_pixel-8));
			idata.src_V_Max=(uint16_t)((int)240 << (idata.src_bits_pixel-8));
		}
	}
	if (idata.dst_bits_pixel<=16)
	{
		if (idata.dst_full_mode)
		{
			idata.dst_Y_Min=0;
			idata.dst_U_Min=0;
			idata.dst_V_Min=0;

			idata.dst_Y_Max=(uint16_t)(((int)1 << idata.dst_bits_pixel)-1);
			idata.dst_U_Max=(uint16_t)(((int)1 << idata.dst_bits_pixel)-1);
			idata.dst_V_Max=(uint16_t)(((int)1 << idata.dst_bits_pixel)-1);
		}
		else
		{
			idata.dst_Y_Min=(uint16_t)((int)16 << (idata.dst_bits_pixel-8));
			idata.dst_U_Min=(uint16_t)((int)16 << (idata.dst_bits_pixel-8));
			idata.dst_V_Min=(uint16_t)((int)16 << (idata.dst_bits_pixel-8));
			idata.dst_Y_Max=(uint16_t)((int)235 << (idata.dst_bits_pixel-8));
			idata.dst_U_Max=(uint16_t)((int)240 << (idata.dst_bits_pixel-8));
			idata.dst_V_Max=(uint16_t)((int)240 << (idata.dst_bits_pixel-8));
		}
	}

	switch(idata.src_color_matrix)
	{
		case MATRIX_BT601 :
			idata.src_Kr=0.299;
			idata.src_Kb=0.114;
			break;
		case MATRIX_BT709 :
			idata.src_Kr=0.2126;
			idata.src_Kb=0.0722;
			break;
		case MATRIX_BT2020 :
			idata.src_Kr=0.2627;
			idata.src_Kb=0.0593;
			break;
		default :
			idata.src_Kr=0.299;
			idata.src_Kb=0.114;
			break;
	}
	idata.src_Kg=1.0-idata.src_Kr-idata.src_Kb;
	if ((idata.src_full_mode) || (idata.src_bits_pixel>16))
	{
		idata.src_Coeff_Y=1.0;
		idata.src_Coeff_U=1.0;
		idata.src_Coeff_V=1.0;
	}
	else
	{
		idata.src_Coeff_Y=219.0/255.0;
		idata.src_Coeff_U=224.0/255.0;
		idata.src_Coeff_V=224.0/255.0;
	}
	idata.src_Ur=-0.5*idata.src_Kr/(1.0-idata.src_Kb);
	idata.src_Ug=-0.5*idata.src_Kg/(1.0-idata.src_Kb);
	idata.src_Ub=0.5;
	idata.src_Vr=0.5;
	idata.src_Vg=-0.5*idata.src_Kg/(1.0-idata.src_Kr);
	idata.src_Vb=-0.5*idata.src_Kb/(1.0-idata.src_Kr);
	idata.src_Rv=2.0*(1.0-idata.src_Kr);
	idata.src_Gu=-2.0*idata.src_Kb*(1.0-idata.src_Kb)/idata.src_Kg;
	idata.src_Gv=-2.0*idata.src_Kr*(1.0-idata.src_Kr)/idata.src_Kg;
	idata.src_Bu=2.0*(1.0-idata.src_Kb);
	if (idata.src_bits_pixel<=16)
	{
		idata.src_Off_Y=idata.src_Y_Min;
		idata.src_Off_U=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.src_Off_V=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.src_Off_R=-((double)idata.src_Off_Y/idata.src_Coeff_Y+idata.src_Rv*(double)idata.src_Off_V/idata.src_Coeff_V);
		idata.src_Off_G=-((double)idata.src_Off_Y/idata.src_Coeff_Y+idata.src_Gu*(double)idata.src_Off_U/idata.src_Coeff_U
			+idata.src_Gv*(double)idata.src_Off_V/idata.src_Coeff_V);
		idata.src_Off_B=-((double)idata.src_Off_Y/idata.src_Coeff_Y+idata.src_Bu*(double)idata.src_Off_U/idata.src_Coeff_U);
	}
	else
	{
		idata.src_Off_Y=0;
		idata.src_Off_U=0;
		idata.src_Off_V=0;
		idata.src_Coeff_Y=1.0;
		idata.src_Coeff_U=1.0;
		idata.src_Coeff_V=1.0;
		idata.src_Off_R=0.0;
		idata.src_Off_G=0.0;
		idata.src_Off_B=0.0;
	}

	switch(idata.dst_color_matrix)
	{
		case MATRIX_BT601 :
			idata.dst_Kr=0.299;
			idata.dst_Kb=0.114;
			break;
		case MATRIX_BT709 :
			idata.dst_Kr=0.2126;
			idata.dst_Kb=0.0722;
			break;
		case MATRIX_BT2020 :
			idata.dst_Kr=0.2627;
			idata.dst_Kb=0.0593;
			break;
		default :
			idata.dst_Kr=0.299;
			idata.dst_Kb=0.114;
			break;
	}
	idata.dst_Kg=1.0-idata.dst_Kr-idata.dst_Kb;
	if ((idata.dst_full_mode) || (idata.dst_bits_pixel>16))
	{
		idata.dst_Coeff_Y=1.0;
		idata.dst_Coeff_U=1.0;
		idata.dst_Coeff_V=1.0;
	}
	else
	{
		idata.dst_Coeff_Y=219.0/255.0;
		idata.dst_Coeff_U=224.0/255.0;
		idata.dst_Coeff_V=224.0/255.0;
	}
	idata.dst_Ur=-0.5*idata.dst_Kr/(1.0-idata.dst_Kb);
	idata.dst_Ug=-0.5*idata.dst_Kg/(1.0-idata.dst_Kb);
	idata.dst_Ub=0.5;
	idata.dst_Vr=0.5;
	idata.dst_Vg=-0.5*idata.dst_Kg/(1.0-idata.dst_Kr);
	idata.dst_Vb=-0.5*idata.dst_Kb/(1.0-idata.dst_Kr);
	idata.dst_Rv=2.0*(1.0-idata.dst_Kr);
	idata.dst_Gu=-2.0*idata.dst_Kb*(1.0-idata.dst_Kb)/idata.dst_Kg;
	idata.dst_Gv=-2.0*idata.dst_Kr*(1.0-idata.dst_Kr)/idata.dst_Kg;
	idata.dst_Bu=2.0*(1.0-idata.dst_Kb);
	if (idata.dst_bits_pixel<=16)
	{
		idata.dst_Off_Y=idata.dst_Y_Min;
		idata.dst_Off_U=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.dst_Off_V=(uint16_t)((int)128 << (idata.dst_bits_pixel-8));
		idata.dst_Off_R=-((double)idata.dst_Off_Y/idata.dst_Coeff_Y+idata.dst_Rv*(double)idata.dst_Off_V/idata.dst_Coeff_V);
		idata.dst_Off_G=-((double)idata.dst_Off_Y/idata.dst_Coeff_Y+idata.dst_Gu*(double)idata.dst_Off_U/idata.dst_Coeff_U
			+idata.dst_Gv*(double)idata.dst_Off_V/idata.dst_Coeff_V);
		idata.dst_Off_B=-((double)idata.dst_Off_Y/idata.dst_Coeff_Y+idata.dst_Bu*(double)idata.dst_Off_U/idata.dst_Coeff_U);
	}
	else
	{
		idata.dst_Off_Y=0;
		idata.dst_Off_U=0;
		idata.dst_Off_V=0;
		idata.dst_Coeff_Y=1.0;
		idata.dst_Coeff_U=1.0;
		idata.dst_Coeff_V=1.0;
		idata.dst_Off_R=0.0;
		idata.dst_Off_G=0.0;
		idata.dst_Off_B=0.0;
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

	image_data=idata;
	
	if (idata.src_h0!=1080)
	{
		ff->Except("Vertical size must be 1080 !");
		return;
	}	
	
	for (i=0; i<256; i++)
	{
		lookup[i]=(int16_t)(i*3);
		lookup[i+256]=(int16_t)round((i*5.0*64.0)/6.0);
		lookup[i+512]=(int16_t)round((i*64.0)/6.0);
		lookup[i+768]=(int16_t)round((i*2.0*64.0)/6.0);
	}
	for (i=0; i<511; i++)
	{
		lookup[i+1024]=(int16_t)round(((i-127)*2.0*64.0)/6.0);
	}

}





void JPSDR_CCC::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];

	strcpy_s(output_string,255," Phase : %d");
	SafePrintf(buf,maxlen,output_string,mData.phase);
}


void JPSDR_CCC::Run()
{
	Image_Data idata;

	idata=image_data;

	if ((idata.src_h0==0) || (idata.src_w0==0)) return;

	const VDXPixmap& pxdst=*fa->dst.mpPixmap;
	const VDXPixmap& pxsrc=*fa->src.mpPixmap;

	idata.src_plane0=pxsrc.data;
	idata.src_plane1=pxsrc.data2;
	idata.src_plane2=pxsrc.data3;
	idata.dst_plane0=pxdst.data;
	idata.dst_plane1=pxdst.data2;
	idata.dst_plane2=pxdst.data3;

	switch(idata.video_mode)
	{
		case 2 :
			if (idata.dst_full_mode)
			{
				CCC_YUY2_FR(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,(idata.src_w0+1)>>1,
					idata.src_h0,mData.phase);
			}
			else CCC_YUY2(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,(idata.src_w0+1)>>1,
					idata.src_h0,mData.phase);
			break;
		case 3 :
			if (idata.dst_full_mode)
			{
				CCC_UYVY_FR(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,(idata.src_w0+1)>>1,
					idata.src_h0,mData.phase);
			}
			else CCC_UYVY(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,(idata.src_w0+1)>>1,
					idata.src_h0,mData.phase);
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
			if (idata.dst_full_mode) 
			{
				if (SSE2_Enable)
					CCC_Planar_SSE_FR(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);
				else
					CCC_Planar_FR(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);
			}
			else
			{
				if (SSE2_Enable)
					CCC_Planar_SSE(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);
				else
					CCC_Planar(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);
			}
			MovePlane(idata.src_plane1,idata.dst_plane1,idata.src_pitch1,idata.dst_pitch1,idata.src_w1,idata.src_h1);
			MovePlane(idata.src_plane2,idata.dst_plane2,idata.src_pitch2,idata.dst_pitch2,idata.src_w2,idata.src_h2);
			break;
		case 9 :
			if (idata.dst_full_mode)
			{
				if (SSE2_Enable)
					CCC_Planar_SSE_FR(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);
				else
					CCC_Planar_FR(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);		
			}
			else
			{
				if (SSE2_Enable)
					CCC_Planar_SSE(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);
				else
					CCC_Planar(idata.src_plane0,idata.dst_plane0,idata.src_pitch0,idata.dst_pitch0,idata.src_w0,
						idata.src_h0,mData.phase);		
			}
			break;
	}

}





void JPSDR_CCC::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.phase=argv[0].asInt();
}

bool JPSDR_CCC::Configure(VDXHWND hwnd)
{
	JPSDR_CCCDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}


void JPSDR_CCC::GetScriptString(char *buf, int maxlen)
{
 
  
    SafePrintf(buf, maxlen, "Config(%d)",mData.phase);
}


extern VDXFilterDefinition2 filterDef_JPSDR_CCC=
VDXVideoFilterDefinition<JPSDR_CCC>("JPSDR","Cross-Conversion Correction v1.3.2","Restore interlaced 720->1080 upscale. SSE optimized.");

