#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "IVTC_Manual_resource.h"

#include "..\Filtres_JPSDR\JPSDRVideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"


#define Max_Sequence 2048


extern int g_VFVAPIVersion;


typedef struct _IVTC_Flags
{
	bool ivtc,ivtc_rev,chg_sequence,chg_sequence_p1,inv_polar;
} IVTC_Flags;



class JPSDR_IVTC_ManualData
{
public:
	bool invert_polar;
	uint8_t output_mode;
	char filename[1024];

	JPSDR_IVTC_ManualData(void);
};

JPSDR_IVTC_ManualData::JPSDR_IVTC_ManualData(void)
{
	invert_polar=false;
	output_mode=1;
	strcpy_s(filename,1024,"");
}


class JPSDR_IVTC_ManualDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_IVTC_ManualDialog(JPSDR_IVTC_ManualData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_JPSDR_IVTC_MANUAL),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_IVTC_ManualData& mData;
	JPSDR_IVTC_ManualData moldData;
	IVDXFilterPreview2 *const mifp;
};



INT_PTR JPSDR_IVTC_ManualDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_IVTC_ManualDialog::OnInit()
{
	moldData=mData;

	CheckDlgButton(mhdlg,IDC_INV_POLAR,mData.invert_polar?BST_CHECKED:BST_UNCHECKED);
	SetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename);
	switch (mData.output_mode)
	{
		case 1 : CheckDlgButton(mhdlg,IDC_PROGRESSIVE,1); break;
		case 2 : CheckDlgButton(mhdlg,IDC_STABLE2,1); break;
	}

	return false;
}


void JPSDR_IVTC_ManualDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_IVTC_ManualDialog::SaveToData()
{
	mData.invert_polar=!!IsDlgButtonChecked(mhdlg, IDC_INV_POLAR);
	GetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename,1024);
	if (!!IsDlgButtonChecked(mhdlg, IDC_PROGRESSIVE)) mData.output_mode=1;
	if (!!IsDlgButtonChecked(mhdlg, IDC_STABLE2)) mData.output_mode=2;
	
	return true;
}



bool JPSDR_IVTC_ManualDialog::OnCommand(int cmd)
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
				SetWindowText(GetDlgItem(mhdlg, IDC_FILENAME), mData.filename);
			}
			return true;
	}
		
	return false;
}




class JPSDR_IVTC_Manual : public JPSDRVDXVideoFilter
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
	uint8_t data_out[5];
	uint8_t ivtc_index;
	uint32_t debut[Max_Sequence],fin[Max_Sequence];
	uint8_t ivtc_offset[Max_Sequence];
	bool modif_polar[Max_Sequence];
	uint16_t nbre_sequence,last_sequence;
	uint8_t last_offset,first_offset;
	bool VDub2_Enable;
//	FILE *fic;

	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_IVTC_ManualData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_IVTC_Manual)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_IVTC_Manual,ScriptConfig,"iis")
VDXVF_END_SCRIPT_METHODS()


uint32 JPSDR_IVTC_Manual::GetParams()
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

	switch(pxsrc.format)
	{
		case nsVDXPixmap::kPixFormat_YUV420_Planar :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709 :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709 :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_709;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_FR :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_FR;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_FR;
			break;
		case nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420ib_Planar_709_FR :
		case nsVDXPixmap::kPixFormat_YUV420it_Planar_709_FR :
			if (mData.output_mode==1) pxdst.format=nsVDXPixmap::kPixFormat_YUV420_Planar_709_FR;
			else pxdst.format=nsVDXPixmap::kPixFormat_YUV420i_Planar_709_FR;
			break;
	}
	
	fa->dst.offset = fa->src.offset;

	image_data.dst_full_mode=CheckFullRangeMode(&fa->dst);
	image_data.dst_color_matrix=GetColorMode(&fa->dst);
	image_data.dst_interlaced=GetInterlaceMode(&fa->dst);

	return(FILTERPARAM_SUPPORTS_ALTFORMATS|FILTERPARAM_ALIGN_SCANLINES);
}


static inline void Rebuild_Frame(const void *top_src, const void *bottom_src, void *dst_, const int32_t w,
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


static inline void Rebuild_Frame_2(const void *src_, void *dst_, const int32_t w,
		const int32_t h,const ptrdiff_t src_pitch,const ptrdiff_t dst_pitch)
{
	const uint8_t *src;
	uint8_t *dst;
	const ptrdiff_t src_pitch2=src_pitch << 1,dst_pitch2=dst_pitch << 1;

	src=(uint8_t *)src_;
	dst=(uint8_t *)dst_;

	for(int32_t i=0; i<h; i++)
	{
		memcpy(dst,src,w);
		src+=src_pitch2;
		dst+=dst_pitch2;
	}

}


void JPSDR_IVTC_Manual::GetSettingString(char *buf, int maxlen)
{
	char output_string[1200];

	if (mData.invert_polar) strcpy_s(output_string,1200," Inverse polarity : On");
	else strcpy_s(output_string,1200," Inverse polarity : Off");
	if (mData.output_mode==1) strcat_s(output_string,255," Progressive");
	else strcat_s(output_string,1200," Telecine");
	strcat_s(output_string,1200," File : %s");
	SafePrintf(buf,maxlen,output_string,mData.filename);
}


bool JPSDR_IVTC_Manual::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	prefetcher->PrefetchFrame(0,frame,0);
	prefetcher->PrefetchFrame(0,frame-2,0);
	prefetcher->PrefetchFrame(0,frame-1,0);
	prefetcher->PrefetchFrame(0,frame+1,0);
	prefetcher->PrefetchFrame(0,frame+2,0);
	
	return(true);
}



void JPSDR_IVTC_Manual::Run()
{
	const VDXPixmapAlpha* tab_pxsrc[5]={
			(const VDXPixmapAlpha*)fa->mpSourceFrames[1]->mpPixmap,
			(const VDXPixmapAlpha*)fa->mpSourceFrames[2]->mpPixmap,
			(const VDXPixmapAlpha*)fa->mpSourceFrames[0]->mpPixmap,
			(const VDXPixmapAlpha*)fa->mpSourceFrames[3]->mpPixmap,
			(const VDXPixmapAlpha*)fa->mpSourceFrames[4]->mpPixmap
	};

	const VDXPixmapAlpha& pxdst = (const VDXPixmapAlpha&)*fa->mpOutputFrames[0]->mpPixmap;
	const VDXFBitmap& pxsrc = *fa->mpSourceFrames[0];
	
	const void *src1_Y,*src2_Y,*src1_U,*src2_U,*src1_V,*src2_V;
	void *dst1_Y,*dst2_Y,*dst1_U,*dst2_U,*dst1_V,*dst2_V;
	IVTC_Flags tab_ivtc[5];
	uint8_t tab_pictures[4];
	uint16_t i,j,i_rd;
	bool chg_seq;
	int64_t index_image;

	if ((image_data.src_h0==0) || (image_data.src_w0==0)) return;

	image_data.dst_plane0=pxdst.data;
	image_data.dst_plane1=pxdst.data2;
	image_data.dst_plane2=pxdst.data3;

	const int32_t h_Y=image_data.src_h0,h_U=image_data.src_h1,h_V=image_data.src_h2;
	const uint32_t line0=image_data.src_line0,line1=image_data.src_line1,line2=image_data.src_line2;
	const ptrdiff_t src_pitch_Y=image_data.src_pitch0,src_pitch_U=image_data.src_pitch1,src_pitch_V=image_data.src_pitch2;
	const ptrdiff_t dst_pitch_Y=image_data.dst_pitch0,dst_pitch_U=image_data.dst_pitch1,dst_pitch_V=image_data.dst_pitch2;

/*	if (pxsrc.mFrameNumber==0)
	{
		if (fic!=NULL) fclose(fic);

		fic=fopen("D:\\Test.txt","wt");
	}*/
	
	if (pxsrc.mFrameNumber%5==0)
	{
		index_image=pxsrc.mFrameNumber;
		for(i=0; i<5; i++)
		{
			j=0;
			while ((j<nbre_sequence) && 
				!((index_image>=debut[j]) && (index_image<=fin[j])))
				j++;
			if (j<nbre_sequence)
			{
				if (index_image==0)
				{
					first_offset=ivtc_offset[j];
				}
				last_offset=ivtc_offset[j];				
				if (j!=last_sequence)
				{
					last_sequence=j;
					chg_seq=true;
				}
				else chg_seq=false;
				if (modif_polar[j])
					tab_ivtc[i].inv_polar=!mData.invert_polar;
				else
					tab_ivtc[i].inv_polar=mData.invert_polar;
			}
			else tab_ivtc[i].inv_polar=mData.invert_polar;
			tab_ivtc[i].ivtc=(i==last_offset);
			tab_ivtc[i].chg_sequence=chg_seq;
			tab_ivtc[i].chg_sequence_p1=false;
			tab_ivtc[i].ivtc_rev=false;
			index_image++;
		}

		j=0;
		while ((j<nbre_sequence) && 
			!((index_image>=debut[j]) && (index_image<=fin[j])))
			j++;
		if (j!=last_sequence)
		{
			tab_ivtc[4].chg_sequence_p1=true;
			tab_ivtc[4].ivtc_rev=true;
		}

		j=0;
		while ((j<5) && !tab_ivtc[j].chg_sequence) j++;

/*		fprintf(fic,"Image  %d -> %d\n",mFrameNumber+1,mFrameNumber+5);
		fprintf(fic,"ivtc (avant/après) : %d,%d\n",first_offset,last_offset);
		if (j<5) fprintf(fic,"Chg seq : %d\n",j);*/

		if (!tab_ivtc[0].chg_sequence)
		{
			i=first_offset;
			if (tab_ivtc[(i+1)%5].chg_sequence) i_rd=i;
			else
			{
				if ((i+1)%5<j) i_rd=(i+1)%5;
				else
				{
					i=last_offset;
					if (((i+1)%5)>=j) i_rd=(i+1)%5;
					else i_rd=i;
				}
			}
		}
		else i_rd=(last_offset+1)%5;

		if ((j>0) && (j<5))
		{
			if (((last_offset+1)%5)>=j) tab_ivtc[(last_offset+1)%5].ivtc=true;
		}

/*		fprintf(fic,"Image levée : %d\n",i_rd);
		for (i=0; i<5; i++)
		{
			if (tab_ivtc[i].ivtc) fprintf(fic,"IVTC : %d\n",i);
		}*/

		first_offset=last_offset;

		j=0;
		for (i=0; i<4 ;i++)
		{
			if (j==i_rd) j++;
			if (tab_ivtc[j].ivtc)
			{
				if (tab_ivtc[j].inv_polar)
				{
					if (tab_ivtc[j].ivtc_rev) tab_pictures[i]=((j-1) << 4) | j;
					else tab_pictures[i]=(j << 4) | (j+1);
				}
				else
				{
					if (tab_ivtc[j].ivtc_rev) tab_pictures[i]=(j << 4) | (j-1);
					else tab_pictures[i]=((j+1) << 4) | j;
				}
			}
			else tab_pictures[i]=(j << 4) | j;
			j++;
		}

/*		fprintf(fic,"Traitement : ");
		for(i=0; i<4; i++)
			fprintf(fic,"%x ",tab_pictures[i]);
		fprintf(fic,"\n");*/

//		fprintf(fic,"\n\n");

		data_out[0]=tab_pictures[0];
		data_out[1]=tab_pictures[1];
		data_out[4]=tab_pictures[3];
		if (mData.output_mode==1)
		{
			data_out[2]=tab_pictures[2];
			data_out[3]=tab_pictures[2];
		}
		else
		{
			data_out[2]=(tab_pictures[1]&0xF0)|(tab_pictures[2]&0x0F);
			data_out[3]=(tab_pictures[2]&0xF0)|(tab_pictures[3]&0x0F);
		}

		ivtc_index=0;
	}

	i=data_out[ivtc_index]&0x0F;
	j=(data_out[ivtc_index]&0xF0)>>4;
	
	i=i+2-ivtc_index;
	j=j+2-ivtc_index;
	
	src1_Y=tab_pxsrc[i]->data;
	src1_U=tab_pxsrc[i]->data2;
	src1_V=tab_pxsrc[i]->data3;

	src2_Y=tab_pxsrc[j]->data;
	src2_U=tab_pxsrc[j]->data2;
	src2_V=tab_pxsrc[j]->data3;
	
	src1_Y=(void *)((uint8_t *)src1_Y+src_pitch_Y);
	src1_U=(void *)((uint8_t *)src1_U+src_pitch_U);
	src1_V=(void *)((uint8_t *)src1_V+src_pitch_V);
	dst1_Y=(void *)((uint8_t *)image_data.dst_plane0+dst_pitch_Y);
	dst2_Y=image_data.dst_plane0;
	dst1_U=(void *)((uint8_t *)image_data.dst_plane1+dst_pitch_U);
	dst2_U=image_data.dst_plane1;
	dst1_V=(void *)((uint8_t *)image_data.dst_plane2+dst_pitch_V);
	dst2_V=image_data.dst_plane2;

	if ((i!=2) && (j!=2))
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				Rebuild_Frame(src2_Y,src1_Y,dst2_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				Rebuild_Frame(src2_Y,src1_Y,dst2_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				Rebuild_Frame(src2_U,src1_U,dst2_U,line1,h_U>>1,src_pitch_U,dst_pitch_U);
				Rebuild_Frame(src2_V,src1_V,dst2_V,line2,h_V>>1,src_pitch_V,dst_pitch_V);
				break;
		}
	}

	if ((i!=2) && (j==2))
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				Rebuild_Frame_2(src1_Y,dst1_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				Rebuild_Frame_2(src1_Y,dst1_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				Rebuild_Frame_2(src1_U,dst1_U,line1,h_U>>1,src_pitch_U,dst_pitch_U);
				Rebuild_Frame_2(src1_V,dst1_V,line2,h_V>>1,src_pitch_V,dst_pitch_V);
				break;
		}
	}

	if ((i==2) && (j!=2))
	{
		switch(image_data.video_mode)
		{
			case VMODE_BMP_RGBA :
			case VMODE_YUYV :
			case VMODE_UYVY :
			case VMODE_Y :
				Rebuild_Frame_2(src2_Y,dst2_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				break;
			case VMODE_PLANAR_YUV444 :
			case VMODE_PLANAR_YUV422 :
			case VMODE_PLANAR_YUV420 :
			case VMODE_PLANAR_YUV411 :
			case VMODE_PLANAR_YUV410 :
				Rebuild_Frame_2(src2_Y,dst2_Y,line0,h_Y>>1,src_pitch_Y,dst_pitch_Y);
				Rebuild_Frame_2(src2_U,dst2_U,line1,h_U>>1,src_pitch_U,dst_pitch_U);
				Rebuild_Frame_2(src2_V,dst2_V,line2,h_V>>1,src_pitch_V,dst_pitch_V);
				break;
		}
	}

	ivtc_index++;
}



void JPSDR_IVTC_Manual::Start()
{
	FILE *file_in;
	char buffer_in[1200];
	uint32_t u,v;
	uint16_t w,line_file;
	int16_t i,j;
	int32_t offset;

	if (g_VFVAPIVersion<14)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
	}

	SetImageData(image_data);

	switch(image_data.video_mode)
	{
		case 6 :
			if ((image_data.src_h0%4)!=0)
				ff->Except("Vertical size must be multiple of 4 for 4:2:0!");
			break;
		case 8 :
			if ((image_data.src_h0%8)!=0)
				ff->Except("Vertical size must be multiple of 8 for 4:1:0!");
			break;
		default :
			if ((image_data.src_h0%2)!=0)
				ff->Except("Vertical size must be multiple of 2!");
			break;
	}

	nbre_sequence=0;
	offset=0;
	line_file=0;

	if (strlen(mData.filename)==0)
	{
		ff->Except("No file selected!");
		return;
	}

	if (fopen_s(&file_in,mData.filename,"rt")!=0)
	{
		strcpy_s(buffer_in,1200,"Unable to open file : ");
		strcat_s(buffer_in,1200,mData.filename);
		ff->Except(buffer_in);
		return;
	}

	while (fgets(buffer_in,1200,file_in))
	{
		line_file++;
		if ((buffer_in[0]=='+') || (buffer_in[0]=='-'))
		{
			if (sscanf_s(buffer_in,"%ld",&offset)==EOF)
			{
				fclose(file_in);
				sprintf_s(buffer_in,1200,"Error in file on line %ld.",line_file);
				ff->Except(buffer_in);
				return;
			}
		}
		else
		{
			if (sscanf_s(buffer_in,"%lu %lu %hu",&u,&v,&w)==EOF)
			{
				fclose(file_in);
				sprintf_s(buffer_in,1200,"Error in file on line %ld.",line_file);
				ff->Except(buffer_in);
				return;
			}
			else
			{
				if (w>9)
				{
					fclose(file_in);
					sprintf_s(buffer_in,1200,"Wrong ITVC seq. in file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}
				if ((u+offset)>(v+offset))
				{
					fclose(file_in);
					sprintf_s(buffer_in,1200,"Error on frame position in file on line %ld.",line_file);
					ff->Except(buffer_in);
					return;
				}

				debut[nbre_sequence]=u+offset;
				fin[nbre_sequence]=v+offset;
				if (w>4)
				{
					ivtc_offset[nbre_sequence]=(uint8_t)(w-5);
					modif_polar[nbre_sequence]=true;
				}
				else
				{
					ivtc_offset[nbre_sequence]=(uint8_t)w;
					modif_polar[nbre_sequence]=false;
				}
				nbre_sequence++;
			}
		}
	}
	fclose(file_in);
	if (nbre_sequence==0)
	{
		strcpy_s(buffer_in,1200,"No line in file : ");
		strcat_s(buffer_in,1200,mData.filename);
		ff->Except(buffer_in);
		return;
	}

	for(i=0; i<(nbre_sequence-1); i++)
	{
		for(j=i+1; j<nbre_sequence; j++)
		{
			if (((debut[j]>=debut[i]) && (debut[j]<=fin[i])) ||
				((fin[j]>=debut[i]) && (fin[j]<=fin[i])))
			{
				sprintf_s(buffer_in,1200,"Overlap around lines %ld and %ld.",i+1,j+1);
				ff->Except(buffer_in);
				return;
			}
		}
	}

	last_sequence=9999;
	last_offset=2;
	first_offset=2;
	ivtc_index=0;
/*	if (fic!=NULL) fclose(fic);
	fic=NULL;*/

}




void JPSDR_IVTC_Manual::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.invert_polar=!!argv[0].asInt();
	mData.output_mode=argv[1].asInt();
	strcpy_s(mData.filename,1024,*argv[2].asString());
}


bool JPSDR_IVTC_Manual::Configure(VDXHWND hwnd)
{
	JPSDR_IVTC_ManualDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}



void JPSDR_IVTC_Manual::GetScriptString(char *buf, int maxlen)
{
    char save[1400];
	int i, j;

	i = j = 0;
	while(mData.filename[i] && (j<1398))
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

	SafePrintf(buf,maxlen,"Config(%d,%d, \"%s\")",mData.invert_polar,mData.output_mode,save);
}


extern VDXFilterDefinition2 filterDef_JPSDR_IVTC_Manual=
VDXVideoFilterDefinition<JPSDR_IVTC_Manual>("JPSDR","IVTC Manual v3.2.1","IVTC Filter (Manual). [ASM] Optimised.");

