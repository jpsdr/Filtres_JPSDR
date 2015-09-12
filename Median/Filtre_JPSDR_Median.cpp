#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\JPSDR_Filter.h"

#include "..\asmlib\asmlib.h"

#define Max_Median_Size 50

extern int g_VFVAPIVersion;

extern "C" int IInstrSet;

extern "C" void JPSDR_Median_Planar_Move_src(const void *src, void *dst, int32_t w,int32_t h,ptrdiff_t src_offset);
extern "C" void JPSDR_Median_Planar_Move_dst(const void *src, void *dst, int32_t w,int32_t h,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_RGB32_Move_src(const void *src, void *planar_R, void *planar_G,void *planar_B,
	int32_t w,int32_t h,ptrdiff_t src_offset);
extern "C" void JPSDR_Median_YUYV_Move_src(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_offset);
extern "C" void JPSDR_Median_UYVY_Move_src(const void *src, void *planar_Y, void *planar_U,void *planar_V,
	int32_t w,int32_t h,ptrdiff_t src_offset);
extern "C" void JPSDR_Median_RGB32_Move_dst(const void *planar_R,const void *planar_G,const void *planar_B,void *dst,
	int32_t w,int32_t h,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_YUYV_Move_dst(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t dst_offset);
extern "C" void JPSDR_Median_UYVY_Move_dst(const void *planar_Y,const void *planar_U,const void *planar_V,void *dst,
	int32_t w,int32_t h,ptrdiff_t dst_offset);



class JPSDR_MedianData
{
public :
	uint8_t threshold[3];
	int32_t median_size[3];
	bool setting_mode,interlace_mode;
	bool filter_disable[3];
	uint8_t filter_mode;

	JPSDR_MedianData(void);
};

JPSDR_MedianData::JPSDR_MedianData(void)
{
	uint8_t i;

	for (i=0; i<3; i++)
	{
		median_size[i]=1;
		filter_disable[i]=false;
	}
	threshold[0]=17;
	threshold[1]=11;
	threshold[2]=8;
	filter_mode=1;
	interlace_mode=false;
	setting_mode=false;
}


class JPSDR_MedianDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_MedianDialog(JPSDR_MedianData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_MEDIAN),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_MedianData& mData;
	JPSDR_MedianData moldData;
	IVDXFilterPreview2 *const mifp;
};



INT_PTR JPSDR_MedianDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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



bool JPSDR_MedianDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_DISABLE_0,mData.filter_disable[0]?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_DISABLE_1,mData.filter_disable[1]?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_DISABLE_2,mData.filter_disable[2]?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_MODE,mData.setting_mode?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(mhdlg,IDC_INTERLACE,mData.interlace_mode?BST_CHECKED:BST_UNCHECKED);
	switch (mData.filter_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_SQUARE,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_HORIZONTAL,1); break;
		case 3 : CheckDlgButton(mhdlg,IDC_VERTICAL,1); break;
	}
	SetDlgItemInt(mhdlg,IDC_0_MEDIAN_SIZE,mData.median_size[0],FALSE);
	SetDlgItemInt(mhdlg,IDC_0_THRESHOLD,mData.threshold[0],FALSE);
	SetDlgItemInt(mhdlg,IDC_1_MEDIAN_SIZE,mData.median_size[1],FALSE);
	SetDlgItemInt(mhdlg,IDC_1_THRESHOLD,mData.threshold[1],FALSE);
	SetDlgItemInt(mhdlg,IDC_2_MEDIAN_SIZE,mData.median_size[2],FALSE);
	SetDlgItemInt(mhdlg,IDC_2_THRESHOLD,mData.threshold[2],FALSE);

	EnableWindow(GetDlgItem(mhdlg,IDC_0_MEDIAN_SIZE),!mData.filter_disable[0]);
	EnableWindow(GetDlgItem(mhdlg,IDC_0_THRESHOLD),!mData.filter_disable[0]);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_0),!mData.filter_disable[0]);
	EnableWindow(GetDlgItem(mhdlg,IDC_1_MEDIAN_SIZE),!mData.filter_disable[1]);
	EnableWindow(GetDlgItem(mhdlg,IDC_1_THRESHOLD),!mData.filter_disable[1]);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_1),!mData.filter_disable[1]);
	EnableWindow(GetDlgItem(mhdlg,IDC_2_MEDIAN_SIZE),!mData.filter_disable[2]);
	EnableWindow(GetDlgItem(mhdlg,IDC_2_THRESHOLD),!mData.filter_disable[2]);
	EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_2),!mData.filter_disable[2]);

	HWND hwndPreview = GetDlgItem(mhdlg, IDC_PREVIEW);
	if (hwndPreview && mifp) {
		EnableWindow(hwndPreview, TRUE);
		mifp->InitButton((VDXHWND)hwndPreview);
	}

	return false;
}



void JPSDR_MedianDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}



bool JPSDR_MedianDialog::SaveToData()
{
	int32_t data;
	BOOL success;
	
	mData.filter_disable[0]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_0);
	mData.filter_disable[1]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_1);
	mData.filter_disable[2]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_2);

	mData.setting_mode=!!IsDlgButtonChecked(mhdlg,IDC_MODE);
	mData.interlace_mode=!!IsDlgButtonChecked(mhdlg,IDC_INTERLACE);

	if (!!IsDlgButtonChecked(mhdlg,IDC_SQUARE)) mData.filter_mode=1;
	if (!!IsDlgButtonChecked(mhdlg,IDC_HORIZONTAL)) mData.filter_mode=2;
	if (!!IsDlgButtonChecked(mhdlg,IDC_VERTICAL)) mData.filter_mode=3;

	data=GetDlgItemInt(mhdlg,IDC_0_MEDIAN_SIZE,&success,FALSE);
	if (success)
	{
		if (data<=0) data=1;
		if (data>Max_Median_Size) data=Max_Median_Size;
		mData.median_size[0]=data;
	}
	data=GetDlgItemInt(mhdlg,IDC_1_MEDIAN_SIZE,&success,FALSE);
	if (success)
	{
		if (data<=0) data=1;
		if (data>Max_Median_Size) data=Max_Median_Size;
		mData.median_size[1]=data;
	}
	data=GetDlgItemInt(mhdlg,IDC_2_MEDIAN_SIZE,&success,FALSE);
	if (success)
	{
		if (data<=0) data=1;
		if (data>Max_Median_Size) data=Max_Median_Size;
		mData.median_size[2]=data;
	}
	data=GetDlgItemInt(mhdlg,IDC_0_THRESHOLD,&success,FALSE);
	if (success)
	{
		if (data>255) data=255;
		mData.threshold[0]=(uint8_t)data;
	}
	data=GetDlgItemInt(mhdlg,IDC_1_THRESHOLD,&success,FALSE);
	if (success)
	{
		if (data>255) data=255;
		mData.threshold[1]=(uint8_t)data;
	}
	data=GetDlgItemInt(mhdlg,IDC_2_THRESHOLD,&success,FALSE);
	if (success)
	{
		if (data>255) data=255;
		mData.threshold[2]=(uint8_t)data;
	}
	
	return true;
}


bool JPSDR_MedianDialog::OnCommand(int cmd)
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
		case IDC_DISABLE_0 :
			mData.filter_disable[0]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_0);
			if (mifp) mifp->RedoFrame();
			EnableWindow(GetDlgItem(mhdlg,IDC_0_MEDIAN_SIZE),!mData.filter_disable[0]);
			EnableWindow(GetDlgItem(mhdlg,IDC_0_THRESHOLD),!mData.filter_disable[0]);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_0),!mData.filter_disable[0]);
			return true;
		case IDC_DISABLE_1 :
			mData.filter_disable[1]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_1);
			if (mifp) mifp->RedoFrame();
			EnableWindow(GetDlgItem(mhdlg,IDC_1_MEDIAN_SIZE),!mData.filter_disable[1]);
			EnableWindow(GetDlgItem(mhdlg,IDC_1_THRESHOLD),!mData.filter_disable[1]);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_1),!mData.filter_disable[1]);
			return true;
		case IDC_DISABLE_2 :
			mData.filter_disable[2]=!!IsDlgButtonChecked(mhdlg,IDC_DISABLE_2);
			if (mifp) mifp->RedoFrame();
			EnableWindow(GetDlgItem(mhdlg,IDC_2_MEDIAN_SIZE),!mData.filter_disable[2]);
			EnableWindow(GetDlgItem(mhdlg,IDC_2_THRESHOLD),!mData.filter_disable[2]);
			EnableWindow(GetDlgItem(mhdlg,IDC_TEXT_2),!mData.filter_disable[2]);
			return true;
		case IDC_0_THRESHOLD :
		case IDC_1_THRESHOLD :
		case IDC_2_THRESHOLD :
		case IDC_MODE :
		case IDC_INTERLACE :
			if (mifp && SaveToData()) mifp->RedoFrame();
			return true;
		case IDC_SQUARE :
		case IDC_HORIZONTAL :
		case IDC_VERTICAL :
		case IDC_0_MEDIAN_SIZE :
		case IDC_1_MEDIAN_SIZE :
		case IDC_2_MEDIAN_SIZE :
			if (mifp && SaveToData()) mifp->RedoSystem();
			return true;
		case IDC_PREVIEW:
			if (mifp) mifp->Toggle((VDXHWND)mhdlg);
			return true;
	}
		
	return false;
}



class JPSDR_Median : public VDXVideoFilter
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
	uint8_t *buffer_in[3],*buffer_out[3];
	ptrdiff_t buffer_size[3];
	int16_t *data[3];
	bool SSE2_Enable;
	size_t CPU_Cache_Size,Cache_Setting;

	void square_median_filter(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
		const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data);
	void square_median_filter_2(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
		const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data);
	void horizontal_median_filter(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
		const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data);
	void horizontal_median_filter_2(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
		const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data);
	void vertical_median_filter(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
		const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data);
	void vertical_median_filter_2(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
		const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data);

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_MedianData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_Median)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_Median,ScriptConfig,"iiiiiiiiiiii")
VDXVF_END_SCRIPT_METHODS()


bool JPSDR_Median::Init()
{
	uint8_t i;

	for (i=0; i<3; i++)
	{
		buffer_in[i]=NULL;
		buffer_out[i]=NULL;
		data[i]=NULL;
	}

	SSE2_Enable=((ff->getCPUFlags() & CPUF_SUPPORTS_SSE2)!=0);

	if (IInstrSet<0) InstructionSet();
	CPU_Cache_Size=DataCacheSize(0)>>2;

	return(true);
}


uint32 JPSDR_Median::GetParams()
{
	if (g_VFVAPIVersion<12) return FILTERPARAM_NOT_SUPPORTED;

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;
	
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
		case nsVDXPixmap::kPixFormat_VDXA_YUV :			*/						
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;
	
	if (g_VFVAPIVersion<14) return(FILTERPARAM_SUPPORTS_ALTFORMATS);
	else return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_PURE_TRANSFORM);
}



void JPSDR_Median::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];
	signed long size[3];
	unsigned char i;

	for (i=0; i<3; i++)
	{
		if (mData.filter_disable[i]) size[i]=0;
		else size[i]=mData.median_size[i];
	}
	strcpy_s(output_string,255," Sz:(%d,%d,%d) Thrs:(%d,%d,%d) Mode:%d");
	if (mData.setting_mode) strcat_s(output_string,255," Test");
	if (mData.interlace_mode) strcat_s(output_string,255," Intrl");
	SafePrintf(buf,maxlen,output_string,size[0],size[1],size[2],mData.threshold[0],mData.threshold[1],
		mData.threshold[2],mData.filter_mode);
}



void JPSDR_Median::square_median_filter(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
  const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data)
{
	const uint32_t _size=m_size*(w+1);	
	const uint32_t m_2=m_size << 1;
	const uint32_t m_3=(size_data+1) >> 1;
	const uint32_t m_4=m_2+1;
	const uint32_t m_6=w-m_4;
	const uint32_t h_1=h-m_2;
	const uint32_t w_1=w-m_2;

	A_memmove(dst,src,_size);
	dst+=_size;
	src+=_size;

	for (uint32_t i=0; i<h_1; i++)
	{
		for (uint32_t j=0; j<w_1; j++)
		{
			int16_t *p=data;
			int16_t max;
			const uint8_t *src_2=src-_size;

			for (uint32_t k=0; k<m_4; k++)
			{
				for (uint32_t l=0; l<m_4; l++)
					*p++=*src_2++;
				src_2+=m_6;
			}
			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
					if (max<data[l])
					{
						max=data[l];
						indice=l;
					}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)*src)<=threshold) *dst++=*src++;
			else
			{
				*dst++=(uint8_t)max;
				src++;
			}
		}

		A_memmove(dst,src,m_2);
		dst+=m_2;
		src+=m_2; 
	}
	A_memmove(dst,src,_size-m_2);
}

		

void JPSDR_Median::square_median_filter_2(const uint8_t *src, uint8_t *dst, const int32_t w, const int32_t h,
  const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data)
{
	const uint32_t size=m_size*(w+1);	
	const uint32_t m_2=m_size << 1;
	const uint32_t m_3=(size_data+1) >> 1;
	const uint32_t m_4=m_2+1;
	const uint32_t m_6=w-m_4;
	const uint32_t h_1=h-m_2;
	const uint32_t w_1=w-m_2;

	A_memmove(dst,src,size);
	dst+=size;
	src+=size;

	for (uint32_t i=0; i<h_1; i++)
	{
		for (uint32_t j=0; j<w_1; j++)
		{
			int16_t *p=data;
			int16_t max;
			const uint8_t *src_2=src-size;

			for (uint32_t k=0; k<m_4; k++)
			{
				for (uint32_t l=0; l<m_4; l++)
					*p++=*src_2++;
				src_2+=m_6;
			}
			for (uint32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (uint32_t l=k+1; l<size_data; l++)
					if (max<data[l])
					{
						max=data[l];
						indice=l;
					}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)*src)<=threshold) *dst++=*src++;
			else
			{
				*dst++=255;
				src++;
			}
		}
		A_memmove(dst,src,m_2);
		dst+=m_2;
		src+=m_2;
	}

	A_memmove(dst,src,size-m_2);
}



void JPSDR_Median::horizontal_median_filter(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
  const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data)
{
	const int32_t m_2=m_size << 1;
	const int32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=m_2+1;
	const int32_t w_1=w-m_2;

	for (int32_t i=0; i<h; i++)
	{
		A_memmove(dst,src,m_size);
		dst+=m_size;
		src+=m_size;

		for (int32_t j=0; j<w_1; j++)
		{
			int16_t *p=data;
			int16_t max;
			const uint8_t *src_2=src-m_size;

			for (int32_t k=0; k<m_4; k++)
				*p++=*src_2++;
			for (int32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (int32_t l=k+1; l<(int32_t)size_data; l++)
					if (max<data[l])
					{
						max=data[l];
						indice=l;
					}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)*src)<=threshold) *dst++=*src++;
			else
			{
				*dst++=(uint8_t)max;
				src++;
			}
		}

		A_memmove(dst,src,m_size);
		dst+=m_size;
		src+=m_size;		
	}
}

		



void JPSDR_Median::horizontal_median_filter_2(const uint8_t *src, uint8_t *dst, const int32_t w,
  const int32_t h,const int32_t m_size, const int16_t threshold, int16_t *data,
  const uint32_t size_data)
{
	const int32_t size=m_size*(w+1);	
	const int32_t m_2=m_size << 1;
	const int32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=m_2+1;
	const int32_t w_1=w-m_2;

	for (int32_t i=0; i<h; i++)
	{
		A_memmove(dst,src,m_size);
		dst+=m_size;
		src+=m_size;

		for (int32_t j=0; j<w_1; j++)
		{
			int16_t *p=data;
			int16_t max;
			const uint8_t *src_2=src-m_size;

			for (int32_t k=0; k<m_4; k++)
				*p++=*src_2++;
			for (int32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (int32_t l=k+1; l<(int32_t)size_data; l++)
					if (max<data[l])
					{
						max=data[l];
						indice=l;
					}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)*src)<=threshold) *dst++=*src++;
			else
			{
				*dst++=255;
				src++;
			}
		}

		A_memmove(dst,src,m_size);
		dst+=m_size;
		src+=m_size;
	}
}





void JPSDR_Median::vertical_median_filter(const uint8_t *src,uint8_t *dst, const int32_t w, const int32_t h,
  const int32_t m_size, const int16_t threshold, int16_t *data, const uint32_t size_data)
{
	const int32_t _size=m_size*w;	
	const int32_t m_2=m_size << 1;
	const int32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=m_2+1;
	const int32_t h_1=h-m_2;
	const int32_t w_1=w-1;

	A_memmove(dst,src,_size);
	dst+=_size;
	src+=_size;
	
	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t *p=data;
			int16_t max;
			const uint8_t *src_2=src-_size;

			for (int32_t k=0; k<m_4; k++)
			{
				*p++=*src_2;
				src_2+=w;
			}
			for (int32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (int32_t l=k+1; l<(int32_t)size_data; l++)
					if (max<data[l])
					{
						max=data[l];
						indice=l;
					}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)*src)<=threshold) *dst++=*src++;
			else
			{
				*dst++=(uint8_t)max;
				src++;
			}
		}
	}

	A_memmove(dst,src,_size);
}

		



void JPSDR_Median::vertical_median_filter_2(const uint8_t *src, uint8_t *dst, const int32_t w,
  const int32_t h,const int32_t m_size, const int16_t threshold, int16_t *data,
  const uint32_t size_data)
{
	const int32_t size=m_size*w;	
	const int32_t m_2=m_size << 1;
	const int32_t m_3=(size_data+1) >> 1;
	const int32_t m_4=m_2+1;
	const int32_t h_1=h-m_2;

	A_memmove(dst,src,size);
	dst+=size;
	src+=size;

	for (int32_t i=0; i<h_1; i++)
	{
		for (int32_t j=0; j<w; j++)
		{
			int16_t *p=data;
			int16_t max;
			const uint8_t *src_2=src-size;

			for (int32_t k=0; k<m_4; k++)
			{
				*p++=*src_2;
				src_2+=w;
			}
			for (int32_t k=0; k<m_3; k++)
			{
				uint32_t indice=k;

				max=data[k];
				for (int32_t l=k+1; l<(int32_t)size_data; l++)
					if (max<data[l])
					{
						max=data[l];
						indice=l;
					}
				if (indice!=k) data[indice]=data[k];
			}
			if (abs(max-(int16_t)*src)<=threshold) *dst++=*src++;
			else
			{
				*dst++=255;
				src++;
			}
		}
	}

	A_memmove(dst,src,size);
}







void JPSDR_Median::Run()
{
	Image_Data idata;
	int32_t w,h;
	int16_t i;
	uint32_t size_data[3];
	uint8_t *p0,*p1,*p2,*p3;

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

	if (mData.interlace_mode)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				JPSDR_Median_RGB32_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					idata.src_w0,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				JPSDR_Median_RGB32_Move_src((void *)((uint8_t *)idata.src_plane0+idata.src_pitch0),buffer_in[0]
					+(buffer_size[0] >>1),buffer_in[1]+(buffer_size[1] >>1),buffer_in[2]
					+(buffer_size[2] >>1),idata.src_w0,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				break;
			case 2 :
				JPSDR_Median_YUYV_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+1)>>1,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				JPSDR_Median_YUYV_Move_src((void *)((uint8_t *)idata.src_plane0+idata.src_pitch0),buffer_in[0]
					+(buffer_size[0] >>1),buffer_in[1]+(buffer_size[1] >>1),buffer_in[2]
					+(buffer_size[2] >>1),(idata.src_w0+1)>>1,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				break;
			case 3 :
				JPSDR_Median_UYVY_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+1)>>1,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				JPSDR_Median_UYVY_Move_src((void *)((uint8_t *)idata.src_plane0+idata.src_pitch0),buffer_in[0]
					+(buffer_size[0] >>1),buffer_in[1]+(buffer_size[1] >>1),buffer_in[2]
					+(buffer_size[2] >>1),(idata.src_w0+1)>>1,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				JPSDR_Median_Planar_Move_src(idata.src_plane0,buffer_in[0],idata.src_w0,idata.src_h0>>1,
					idata.src_modulo0+idata.src_pitch0);
				JPSDR_Median_Planar_Move_src((void *)((uint8_t *)idata.src_plane0+idata.src_pitch0),buffer_in[0]
					+(buffer_size[0] >>1),idata.src_w0,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				JPSDR_Median_Planar_Move_src(idata.src_plane1,buffer_in[1],idata.src_w1,idata.src_h1>>1,
					idata.src_modulo1+idata.src_pitch1);
				JPSDR_Median_Planar_Move_src((void *)((uint8_t *)idata.src_plane1+idata.src_pitch1),buffer_in[1]
					+(buffer_size[1] >>1),idata.src_w1,idata.src_h1>>1,idata.src_modulo1+idata.src_pitch1);
				JPSDR_Median_Planar_Move_src(idata.src_plane2,buffer_in[2],idata.src_w2,idata.src_h2>>1,
					idata.src_modulo2+idata.src_pitch2);
				JPSDR_Median_Planar_Move_src((void *)((uint8_t *)idata.src_plane2+idata.src_pitch2),buffer_in[2]
					+(buffer_size[2] >>1),idata.src_w2,idata.src_h2>>1,idata.src_modulo2+idata.src_pitch2);
				break;
			case 9 :
				JPSDR_Median_Planar_Move_src(idata.src_plane0,buffer_in[0],idata.src_w0,idata.src_h0>>1,
					idata.src_modulo0+idata.src_pitch0);
				JPSDR_Median_Planar_Move_src((void *)((uint8_t *)idata.src_plane0+idata.src_pitch0),buffer_in[0]
					+(buffer_size[0] >>1),idata.src_w0,idata.src_h0>>1,idata.src_modulo0+idata.src_pitch0);
				break;
		}
	}
	else
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				JPSDR_Median_RGB32_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					idata.src_w0,idata.src_h0,idata.src_modulo0);
				break;
			case 2 :
				JPSDR_Median_YUYV_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+1)>>1,idata.src_h0,idata.src_modulo0);
				break;
			case 3 :
				JPSDR_Median_UYVY_Move_src(idata.src_plane0,buffer_in[0],buffer_in[1],buffer_in[2],
					(idata.src_w0+1)>>1,idata.src_h0,idata.src_modulo0);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				JPSDR_Median_Planar_Move_src(idata.src_plane0,buffer_in[0],idata.src_w0,idata.src_h0,
					idata.src_modulo0);
				JPSDR_Median_Planar_Move_src(idata.src_plane1,buffer_in[1],idata.src_w1,idata.src_h1,
					idata.src_modulo1);
				JPSDR_Median_Planar_Move_src(idata.src_plane2,buffer_in[2],idata.src_w2,idata.src_h2,
					idata.src_modulo2);
				break;
			case 9 :
				JPSDR_Median_Planar_Move_src(idata.src_plane0,buffer_in[0],idata.src_w0,idata.src_h0,
					idata.src_modulo0);
				break;
		}
	}

	switch(mData.filter_mode)
	{
	case 1 :	for (i=0; i<3; i++)
				{
					size_data[i]=2*mData.median_size[i]+1;
					size_data[i]*=size_data[i];
				}
				break;
	case 2 :
	case 3 :	for (i=0; i<3; i++)
					size_data[i]=2*mData.median_size[i]+1;
				break;
	}

	if (mData.interlace_mode)
	{
		for (i=0; i<3; i++)
		{
			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					w=idata.src_w0;
					h=idata.src_h0;
					break;
				case 2 :
				case 3 :
					switch(i)
					{
						case 0 :
							w=2*((idata.src_w0+1)>>1);
							h=idata.src_h0;
							break;
						case 1 :
						case 2 :
							w=(idata.src_w0+1)>>1;
							h=idata.src_h0;
							break;
					}
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					switch(i)
					{
						case 0 :
							w=idata.src_w0;
							h=idata.src_h0;
							break;
						case 1 :
							w=idata.src_w1;
							h=idata.src_h1;
							break;
						case 2 :
							w=idata.src_w2;
							h=idata.src_h2;
							break;
					}
					break;
				case 9 :
					switch(i)
					{
						case 0 :
							w=idata.src_w0;
							h=idata.src_h0;
							break;
						case 1 :
						case 2 :
							continue;
							break;
					}
					break;
			}
			p0=buffer_in[i];
			p2=buffer_out[i];
			p1=p0+(buffer_size[i] >>1);
			p3=p2+(buffer_size[i] >>1);
			if (!mData.filter_disable[i])
			{
				if (mData.setting_mode)
				{
					switch(mData.filter_mode)
					{
						case 1 :
							square_median_filter_2(p0,p2,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							square_median_filter_2(p1,p3,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							break;
						case 2 :
							horizontal_median_filter_2(p0,p2,w,h>>1,mData.median_size[i],
								mData.threshold[i],data[i],size_data[i]);
							horizontal_median_filter_2(p1,p3,w,h>>1,mData.median_size[i],
								mData.threshold[i],data[i],size_data[i]);
							break;
						case 3 :
							vertical_median_filter_2(p0,p2,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							vertical_median_filter_2(p1,p3,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							break;
					}
				}
				else
				{
					switch(mData.filter_mode)
					{
						case 1 :
							square_median_filter(p0,p2,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							square_median_filter(p1,p3,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							break;
						case 2 :
							horizontal_median_filter(p0,p2,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							horizontal_median_filter(p1,p3,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							break;
						case 3 :
							vertical_median_filter(p0,p2,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							vertical_median_filter(p1,p3,w,h>>1,mData.median_size[i],mData.threshold[i],
								data[i],size_data[i]);
							break;
					}
				}
			}
			else
			{
				memcpy(buffer_out[i],buffer_in[i],buffer_size[i]);
			}
		}
	}
	else		// Fin partie interlace
	{
		for (i=0; i<3; i++)
		{
			switch(idata.video_mode)
			{
				case 0 :
				case 1 :
					w=idata.src_w0;
					h=idata.src_h0;
					break;
				case 2 :
				case 3 :
					switch(i)
					{
						case 0 :
							w=2*((idata.src_w0+1)>>1);
							h=idata.src_h0;
							break;
						case 1 :
						case 2 :
							w=(idata.src_w0+1)>>1;
							h=idata.src_h0;
							break;
					}
					break;
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					switch(i)
					{
						case 0 :
							w=idata.src_w0;
							h=idata.src_h0;
							break;
						case 1 :
							w=idata.src_w1;
							h=idata.src_h1;
							break;
						case 2 :
							w=idata.src_w2;
							h=idata.src_h2;
							break;
					}
					break;
				case 9 :
					switch(i)
					{
						case 0 :
							w=idata.src_w0;
							h=idata.src_h0;
							break;
						case 1 :
						case 2 :
							continue;
							break;
					}
					break;
			}
			if (!mData.filter_disable[i])
			{
				if (mData.setting_mode)
				{
						switch(mData.filter_mode)
						{
							case 1 : square_median_filter_2(buffer_in[i],buffer_out[i],w,h,mData.median_size[i],
								mData.threshold[i],data[i],size_data[i]); break;
							case 2 : horizontal_median_filter_2(buffer_in[i],buffer_out[i],w,h,mData.median_size[i],
								mData.threshold[i],data[i],size_data[i]); break;
							case 3 : vertical_median_filter_2(buffer_in[i],buffer_out[i],w,h,mData.median_size[i],
								mData.threshold[i],data[i],size_data[i]); break;
						}
				}
				else
				{
					switch(mData.filter_mode)
					{
						case 1 : square_median_filter(buffer_in[i],buffer_out[i],w,h,mData.median_size[i],
							mData.threshold[i],data[i],size_data[i]); break;
						case 2 : horizontal_median_filter(buffer_in[i],buffer_out[i],w,h,mData.median_size[i],
							mData.threshold[i],data[i],size_data[i]); break;
						case 3 : vertical_median_filter(buffer_in[i],buffer_out[i],w,h,mData.median_size[i],
							mData.threshold[i],data[i],size_data[i]); break;
					}
				}
			}
			else
			{
				memcpy(buffer_out[i],buffer_in[i],buffer_size[i]);
			}
		}
	}

	if (mData.interlace_mode)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				JPSDR_Median_RGB32_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					idata.src_w0,idata.src_h0>>1,idata.dst_modulo0+idata.dst_pitch0);
				JPSDR_Median_RGB32_Move_dst(buffer_out[0]+(buffer_size[0] >>1),buffer_out[1]
					+(buffer_size[1] >>1),buffer_out[2]+(buffer_size[2] >>1),
					(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0),idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				break;
			case 2 :
				JPSDR_Median_YUYV_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+1)>>1,idata.src_h0>>1,idata.dst_modulo0+idata.dst_pitch0);
				JPSDR_Median_YUYV_Move_dst(buffer_out[0]+(buffer_size[0] >>1),buffer_out[1]
					+(buffer_size[1] >>1),buffer_out[2]+(buffer_size[2] >>1),
					(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0),(idata.src_w0+1)>>1,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				break;
			case 3 :
				JPSDR_Median_UYVY_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+1)>>1,idata.src_h0>>1,idata.dst_modulo0+idata.dst_pitch0);
				JPSDR_Median_UYVY_Move_dst(buffer_out[0]+(buffer_size[0] >>1),buffer_out[1]
					+(buffer_size[1] >>1),buffer_out[2]+(buffer_size[2] >>1),
					(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0),(idata.src_w0+1)>>1,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				JPSDR_Median_Planar_Move_dst(buffer_out[0],idata.dst_plane0,idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				JPSDR_Median_Planar_Move_dst(buffer_out[0]+(buffer_size[0] >>1),
					(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0),idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				JPSDR_Median_Planar_Move_dst(buffer_out[1],idata.dst_plane1,idata.src_w1,idata.src_h1>>1,
					idata.dst_modulo1+idata.dst_pitch1);
				JPSDR_Median_Planar_Move_dst(buffer_out[1]+(buffer_size[1] >>1),
					(void *)((uint8_t *)idata.dst_plane1+idata.dst_pitch1),idata.src_w1,idata.src_h1>>1,
					idata.dst_modulo1+idata.dst_pitch1);
				JPSDR_Median_Planar_Move_dst(buffer_out[2],idata.dst_plane2,idata.src_w2,idata.src_h2>>1,
					idata.dst_modulo2+idata.dst_pitch2);
				JPSDR_Median_Planar_Move_dst(buffer_out[2]+(buffer_size[2] >>1),
					(void *)((uint8_t *)idata.dst_plane2+idata.dst_pitch2),idata.src_w2,idata.src_h2>>1,
					idata.dst_modulo2+idata.dst_pitch2);
				break;
			case 9 :
				JPSDR_Median_Planar_Move_dst(buffer_out[0],idata.dst_plane0,idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				JPSDR_Median_Planar_Move_dst(buffer_out[0]+(buffer_size[0] >>1),
					(void *)((uint8_t *)idata.dst_plane0+idata.dst_pitch0),idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0+idata.dst_pitch0);
				break;
		}
	}
	else
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				JPSDR_Median_RGB32_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					idata.src_w0,idata.src_h0,idata.dst_modulo0);
				break;
			case 2 :
				JPSDR_Median_YUYV_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+1)>>1,idata.src_h0,idata.dst_modulo0);
				break;
			case 3 :
				JPSDR_Median_UYVY_Move_dst(buffer_out[0],buffer_out[1],buffer_out[2],idata.dst_plane0,
					(idata.src_w0+1)>>1,idata.src_h0,idata.dst_modulo0);
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				JPSDR_Median_Planar_Move_dst(buffer_out[0],idata.dst_plane0,idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0);
				JPSDR_Median_Planar_Move_dst(buffer_out[1],idata.dst_plane1,idata.src_w1,idata.src_h1>>1,
					idata.dst_modulo1);
				JPSDR_Median_Planar_Move_dst(buffer_out[2],idata.dst_plane2,idata.src_w2,idata.src_h2>>1,
					idata.dst_modulo2);
				break;
			case 9 :
				JPSDR_Median_Planar_Move_dst(buffer_out[0],idata.dst_plane0,idata.src_w0,idata.src_h0>>1,
					idata.dst_modulo0);
				break;
		}
	}

}





void JPSDR_Median::Start()
{
	Image_Data idata;
	bool ok;
	int32_t i,j;
	uint32_t size_data[3];
	int32_t h,w;

	if (g_VFVAPIVersion<12)
	{
		ff->Except("This virtualdub version doesn't support this filter !");
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
				idata.src_modulo0=idata.src_pitch0-4*idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :
				idata.src_video_mode=2;
				idata.video_mode=2;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :
				idata.src_video_mode=3;
				idata.video_mode=3;
				idata.src_modulo0=idata.src_pitch0-4*((idata.src_w0+1)>>1);
				idata.src_size0=(size_t)idata.src_h0*(size_t)((idata.src_w0+1)>>1)*4;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :
				idata.src_video_mode=4;
				idata.video_mode=4;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=idata.src_w0;
				idata.src_w2=idata.src_w0;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :
				idata.src_video_mode=5;
				idata.video_mode=5;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;				
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_BFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
				idata.src_video_mode=6;
				idata.video_mode=6;
				idata.src_h1=(idata.src_h0+1)>>1;
				idata.src_h2=(idata.src_h0+1)>>1;
				idata.src_w1=(idata.src_w0+1)>>1;
				idata.src_w2=(idata.src_w0+1)>>1;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_TFF;								
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :
				idata.src_video_mode=7;
				idata.video_mode=7;
				idata.src_h1=idata.src_h0;
				idata.src_h2=idata.src_h0;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=INTERLACED_NONE;												
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT601;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :
				idata.src_video_mode=8;
				idata.video_mode=8;
				idata.src_h1=(idata.src_h0+3)>>2;
				idata.src_h2=(idata.src_h0+3)>>2;
				idata.src_w1=(idata.src_w0+3)>>2;
				idata.src_w2=(idata.src_w0+3)>>2;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_modulo1=idata.src_pitch1-idata.src_w1;
				idata.src_modulo2=idata.src_pitch2-idata.src_w2;
				idata.src_size0=(size_t)idata.src_w0*(size_t)idata.src_h0;
				idata.src_size1=(size_t)idata.src_w1*(size_t)idata.src_h1;
				idata.src_size2=(size_t)idata.src_w2*(size_t)idata.src_h2;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_BT709;
				idata.src_interlaced=PROGRESSIVE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=false;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;																
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :			
				idata.src_video_mode=9;
				idata.video_mode=9;
				idata.src_modulo0=idata.src_pitch0-idata.src_w0;
				idata.src_size0=(size_t)idata.src_h0*(size_t)idata.src_w0;
				idata.src_h1=0;
				idata.src_h2=0;
				idata.src_w1=0;
				idata.src_w2=0;
				idata.src_pitch1=0;
				idata.src_pitch2=0;
				idata.src_modulo1=0;
				idata.src_modulo2=0;
				idata.src_size1=0;
				idata.src_size2=0;
				idata.src_full_mode=true;
				idata.src_color_matrix=MATRIX_NONE;
				idata.src_interlaced=INTERLACED_NONE;																
				break;
		}
		switch(pxdst.format)
		{
			case nsVDXPixmap::kPixFormat_XRGB8888 :
				idata.dst_video_mode=1;
				idata.dst_modulo0=idata.dst_pitch0-4*idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																				
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709 :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_FR :
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_YUYV_709_FR :			
				idata.dst_video_mode=2;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																								
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709 :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_FR :
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV422_UYVY_709_FR :			
				idata.dst_video_mode=3;
				idata.dst_modulo0=idata.dst_pitch0-4*((idata.dst_w0+1)>>1);
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)((idata.dst_w0+1)>>1)*4;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																												
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709 :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_FR :
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV444_Planar_709_FR :			
				idata.dst_video_mode=4;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=idata.dst_w0;
				idata.dst_w2=idata.dst_w0;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709 :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_FR :
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV422_Planar_709_FR :			
				idata.dst_video_mode=5;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_BFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :			
				idata.dst_video_mode=6;
				idata.dst_h1=(idata.dst_h0+1)>>1;
				idata.dst_h2=(idata.dst_h0+1)>>1;
				idata.dst_w1=(idata.dst_w0+1)>>1;
				idata.dst_w2=(idata.dst_w0+1)>>1;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_TFF;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709 :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_FR :
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV411_Planar_709_FR :			
				idata.dst_video_mode=7;
				idata.dst_h1=idata.dst_h0;
				idata.dst_h2=idata.dst_h0;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709 :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_FR :
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT601;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_YUV410_Planar_709_FR :			
				idata.dst_video_mode=8;
				idata.dst_h1=(idata.dst_h0+3)>>2;
				idata.dst_h2=(idata.dst_h0+3)>>2;
				idata.dst_w1=(idata.dst_w0+3)>>2;
				idata.dst_w2=(idata.dst_w0+3)>>2;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_modulo1=idata.dst_pitch1-idata.dst_w1;
				idata.dst_modulo2=idata.dst_pitch2-idata.dst_w2;
				idata.dst_size0=(size_t)idata.dst_w0*(size_t)idata.dst_h0;
				idata.dst_size1=(size_t)idata.dst_w1*(size_t)idata.dst_h1;
				idata.dst_size2=(size_t)idata.dst_w2*(size_t)idata.dst_h2;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_BT709;
				idata.dst_interlaced=PROGRESSIVE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8 :
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=false;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
			case nsVDXPixmap::kPixFormat_Y8_FR :						
				idata.dst_video_mode=9;
				idata.dst_modulo0=idata.dst_pitch0-idata.dst_w0;
				idata.dst_size0=(size_t)idata.dst_h0*(size_t)idata.dst_w0;
				idata.dst_h1=0;
				idata.dst_h2=0;
				idata.dst_w1=0;
				idata.dst_w2=0;
				idata.dst_pitch1=0;
				idata.dst_pitch2=0;
				idata.dst_modulo1=0;
				idata.dst_modulo2=0;
				idata.dst_size1=0;
				idata.dst_size2=0;
				idata.dst_full_mode=true;
				idata.dst_color_matrix=MATRIX_NONE;
				idata.dst_interlaced=INTERLACED_NONE;																																
				break;
		}

	image_data=idata;

	switch(idata.video_mode)
	{
		case 0 :
		case 1 :
			buffer_size[0]=idata.src_h0*idata.src_w0;
			buffer_size[1]=buffer_size[0];
			buffer_size[2]=buffer_size[0];
			break;
		case 2 :
		case 3 :
			buffer_size[0]=idata.src_h0*2*((idata.src_w0+1)>>1);
			buffer_size[1]=idata.src_h0*((idata.src_w0+1)>>1);
			buffer_size[2]=buffer_size[1];
			break;
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
		case 9 :
			buffer_size[0]=idata.src_size0;
			buffer_size[1]=idata.src_size1;
			buffer_size[2]=idata.src_size2;
			break;
	}

	for (i=0; i<3; i++)
	{
		switch(idata.video_mode)
		{
			case 0 :
			case 1 :
				w=idata.src_w0;
				h=idata.src_h0;
				break;
			case 2 :
			case 3 :
				switch(i)
				{
					case 0 :
						w=2*((idata.src_w0+1)>>1);
						h=idata.src_h0;
						break;
					case 1 :
					case 2 :
						w=(idata.src_w0+1)>>1;
						h=idata.src_h0;
						break;
				}
				break;
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
				switch(i)
				{
					case 0 :
						w=idata.src_w0;
						h=idata.src_h0;
						break;
					case 1 :
						w=idata.src_w1;
						h=idata.src_h1;
						break;
					case 2 :
						w=idata.src_w2;
						h=idata.src_h2;
						break;
				}
				break;
			case 9 :
				switch(i)
				{
					case 0 :
						w=idata.src_w0;
						h=idata.src_h0;
						break;
					case 1 :
					case 2 :
						continue;
						break;
				}
		}
		if (mData.median_size[i]>=(h >> 1)) mData.median_size[i]=(h >> 1)-1;
		if (mData.median_size[i]>=(w >> 1)) mData.median_size[i]=(w >> 1)-1;
		size_data[i]=2*mData.median_size[i]+1;
		size_data[i]*=size_data[i];
	}
	for (i=0; i<3; i++)
	{
		if ((idata.video_mode!=9) || (i==0)) buffer_in[i]=(uint8_t *)malloc(buffer_size[i]);
		else buffer_in[i]=buffer_in[0];
	}
	for (i=0; i<3; i++)
	{
		if ((idata.video_mode!=9) || (i==0)) buffer_out[i]=(uint8_t *)malloc(buffer_size[i]);
		else buffer_out[i]=buffer_out[0];
	}
	for (i=0; i<3; i++)
	{
		if ((idata.video_mode!=9) || (i==0)) data[i]=(int16_t *)malloc(size_data[i]*sizeof(int16_t));
		else data[i]=data[0];
	}

	ok=true;
	for (i=0; i<3; i++)
		ok=ok && (buffer_in[i]!=NULL);
	for (i=0; i<3; i++)
		ok=ok && (buffer_out[i]!=NULL);
	for (i=0; i<3; i++)
		ok=ok && (data[i]!=NULL);
	if (idata.video_mode==9)
	{
		buffer_in[1]=NULL;
		buffer_in[2]=NULL;
		buffer_out[1]=NULL;
		buffer_out[2]=NULL;
		data[1]=NULL;
		data[2]=NULL;
	}
	if (!ok)
	{
		for (i=2; i>=0; i--)
			myfree(data[i]);
		for (i=2; i>=0; i--)
			myfree(buffer_out[i]);
		for (i=2; i>=0; i--)
			myfree(buffer_in[i]);
		ff->ExceptOutOfMemory();
		return;
	}

	for (i=0; i<3; i++)
	{
		if ((idata.video_mode!=9) || (i==0))
		{
			for (j=0; j<(int32_t)size_data[i]; j++)
				data[i][j]=0;
		}
		else continue;
	}

	const size_t img_size=idata.dst_size0+idata.dst_size1+idata.dst_size2;

	if (img_size<=MAX_CACHE_SIZE)
	{
		if (CPU_Cache_Size>=img_size) Cache_Setting=img_size;
		else Cache_Setting=16;
	}
	else Cache_Setting=16;

}



void JPSDR_Median::End()
{
	int16_t i;

	for (i=2; i>=0; i--)
		myfree(data[i]);
	for (i=2; i>=0; i--)
		myfree(buffer_out[i]);
	for (i=2; i>=0; i--)
		myfree(buffer_in[i]);
}



bool JPSDR_Median::Configure(VDXHWND hwnd)
{
	JPSDR_MedianDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}




void JPSDR_Median::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
    mData.median_size[0]=argv[0].asInt();
	mData.median_size[1]=argv[1].asInt();
	mData.median_size[2]=argv[2].asInt();
    mData.threshold[0]=argv[3].asInt();
	mData.threshold[1]=argv[4].asInt();
	mData.threshold[2]=argv[5].asInt();
	mData.setting_mode=!!argv[6].asInt();
	mData.filter_mode=argv[7].asInt();
	mData.interlace_mode=!!argv[8].asInt();
	mData.filter_disable[0]=!!argv[9].asInt();
	mData.filter_disable[1]=!!argv[10].asInt();
	mData.filter_disable[2]=!!argv[11].asInt();
}



void JPSDR_Median::GetScriptString(char *buf, int maxlen)
{
    SafePrintf(buf, maxlen,"Config(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
        mData.median_size[0],mData.median_size[1],mData.median_size[2],
        mData.threshold[0],mData.threshold[1],mData.threshold[2],mData.setting_mode,mData.filter_mode,
		mData.interlace_mode,mData.filter_disable[0],mData.filter_disable[1],mData.filter_disable[2]);
}

extern VDXFilterDefinition filterDef_JPSDR_Median=
VDXVideoFilterDefinition<JPSDR_Median>("JPSDR","Median v2.5.0","Median filter with threshold.");
