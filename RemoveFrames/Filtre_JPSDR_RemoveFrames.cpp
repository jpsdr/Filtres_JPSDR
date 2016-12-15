#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "resource.h"

#include "..\Filtres_JPSDR\VideoFilter.h"
#include "..\Filtres_JPSDR\VideoFilterDialog.h"

#include "..\Filtres_JPSDR\Fraction.h"
#include "..\Filtres_JPSDR\JPSDR_Filter.h"

extern int g_VFVAPIVersion;



class JPSDR_RemoveFramesData
{
public:
	int32_t offset_frame,frames_removed,frame_period;

	JPSDR_RemoveFramesData(void);
};

JPSDR_RemoveFramesData::JPSDR_RemoveFramesData(void)
{
	offset_frame=3;
	frames_removed=1;
	frame_period=5;
}


class JPSDR_RemoveFramesDialog : public VDXVideoFilterDialog
{
public:
	JPSDR_RemoveFramesDialog(JPSDR_RemoveFramesData& Data, IVDXFilterPreview2 *ifp) : mData(Data), mifp(ifp)
	{}
	
	bool Show(HWND parent)
	{
		return 0!=VDXVideoFilterDialog::Show(NULL,MAKEINTRESOURCE(IDD_REMOVEFRAMES),parent);
	}
	
	virtual INT_PTR DlgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInit();
	bool OnCommand(int cmd);
	void OnDestroy();
	
	bool SaveToData();
	
	JPSDR_RemoveFramesData& mData;
	JPSDR_RemoveFramesData moldData;
	IVDXFilterPreview2 *const mifp;
};



INT_PTR JPSDR_RemoveFramesDialog::DlgProc(UINT msg, WPARAM wParam, LPARAM lParam)
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


bool JPSDR_RemoveFramesDialog::OnInit()
{
	moldData=mData;

	SetDlgItemInt(mhdlg,IDC_OFFSET,mData.offset_frame,FALSE);
	SetDlgItemInt(mhdlg,IDC_FRAMES,mData.frames_removed,FALSE);
	SetDlgItemInt(mhdlg,IDC_PERIOD,mData.frame_period,FALSE);

	return false;
}


void JPSDR_RemoveFramesDialog::OnDestroy()
{
	if (mifp) mifp->InitButton(NULL);
}


bool JPSDR_RemoveFramesDialog::SaveToData()
{
	int32_t data;
	BOOL success;

	data=GetDlgItemInt(mhdlg,IDC_OFFSET,&success,FALSE);
	if (success) mData.offset_frame=data;
	data=GetDlgItemInt(mhdlg,IDC_FRAMES,&success,FALSE);
	if (success) mData.frames_removed=data;
	data=GetDlgItemInt(mhdlg,IDC_PERIOD,&success,FALSE);
	if (success) mData.frame_period=data;

	return true;
}



bool JPSDR_RemoveFramesDialog::OnCommand(int cmd)
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




class JPSDR_RemoveFrames : public VDXVideoFilter
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
	void ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc);
		
	JPSDR_RemoveFramesData mData;
};

VDXVF_BEGIN_SCRIPT_METHODS(JPSDR_RemoveFrames)
VDXVF_DEFINE_SCRIPT_METHOD(JPSDR_RemoveFrames,ScriptConfig,"iii")
VDXVF_END_SCRIPT_METHODS()




uint32 JPSDR_RemoveFrames::GetParams()
{
	int64_t frame_count_h,frame_count_l;

	if (g_VFVAPIVersion<12) return(FILTERPARAM_NOT_SUPPORTED);

	const VDXPixmapLayout& pxsrc = *fa->src.mpPixmapLayout;
	VDXPixmapLayout& pxdst = *fa->dst.mpPixmapLayout;

	VDFraction fr(fa->dst.mFrameRateHi, fa->dst.mFrameRateLo);

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
			break;
		default : return FILTERPARAM_NOT_SUPPORTED;
	}

	if ((mData.frame_period>0) && ((mData.frame_period-mData.frames_removed)>0))
	{
		fr *= VDFraction(mData.frame_period-mData.frames_removed,mData.frame_period);
		fa->dst.mFrameRateHi = fr.getHi();
		fa->dst.mFrameRateLo = fr.getLo();
		frame_count_h=(fa->dst.mFrameCount/mData.frame_period)*(mData.frame_period-mData.frames_removed);
		frame_count_l=fa->dst.mFrameCount%mData.frame_period;
		if ((frame_count_l-1)>=mData.offset_frame)
		{
			if (((frame_count_l-1)-mData.offset_frame)<mData.frames_removed) frame_count_l=mData.offset_frame;
			else frame_count_l-=mData.frames_removed;
		}
		fa->dst.mFrameCount=frame_count_h+frame_count_l;
	}

	fa->dst.depth = 0;
	pxdst.pitch=pxsrc.pitch;
	pxdst.pitch2=pxsrc.pitch2;
	pxdst.pitch3=pxsrc.pitch3;

	fa->dst.offset = fa->src.offset;

	return(FILTERPARAM_SUPPORTS_ALTFORMATS);
}



void JPSDR_RemoveFrames::GetSettingString(char *buf, int maxlen)
{
	char output_string[255];

	strcpy_s(output_string,255," Offset : %d Removed : %d Period : %d");
	SafePrintf(buf,maxlen,output_string,mData.offset_frame,mData.frames_removed,mData.frame_period);
}



// Case of IVTC (Exemple)
// Offset=0 => [0-3]=>[1-4]
// Offset=1 => [0]=>[0], [1-3]=>[2-4]
// Offset=2 => [0-1]=>[0-1], [2-3]=>[3-4]
// Offset=3 => [0-2]=>[0-2], [3]=>[4]
// Offset=4 => [0-3]=>[0-3]
bool JPSDR_RemoveFrames::Prefetch2(sint64 frame, IVDXVideoPrefetcher *prefetcher)
{
	int64_t frame_low,frame_high,i;

	frame_high=(frame/(mData.frame_period-mData.frames_removed))*mData.frame_period;
	frame_low=frame%(mData.frame_period-mData.frames_removed);
	if (frame_low>=mData.offset_frame) frame_low+=mData.frames_removed;

	frame=frame_high+frame_low;
	prefetcher->PrefetchFrame(0,frame,0);
	for (i=mData.frames_removed; i>=1; i--)
		prefetcher->PrefetchFrame(0,frame-i,0);
	for (i=1; i<=mData.frames_removed; i++)
		prefetcher->PrefetchFrame(0,frame+i,0);

	return(true);
}



void JPSDR_RemoveFrames::Run()
{
}


void JPSDR_RemoveFrames::Start()
{
	if (g_VFVAPIVersion<14)
	{
		ff->Except("This virtualdub version doesn't support this filter!");
	}

	if ((mData.frame_period<=1) || ((mData.offset_frame+mData.frames_removed)>mData.frame_period) || 
		(mData.frames_removed>=mData.frame_period))
	{
		ff->Except("Incorrect parameters!");
	}
}



void JPSDR_RemoveFrames::ScriptConfig(IVDXScriptInterpreter *isi, const VDXScriptValue *argv, int argc)
{
	mData.offset_frame=argv[0].asInt();
	mData.frames_removed=argv[1].asInt();
	mData.frame_period=argv[2].asInt();
}


bool JPSDR_RemoveFrames::Configure(VDXHWND hwnd)
{
	JPSDR_RemoveFramesDialog dlg(mData, fa->ifp2);
	
	return dlg.Show((HWND)hwnd);
}



void JPSDR_RemoveFrames::GetScriptString(char *buf, int maxlen)
{
	SafePrintf(buf,maxlen,"Config(%d, %d , %d)",mData.offset_frame,mData.frames_removed,mData.frame_period);
}


extern VDXFilterDefinition filterDef_JPSDR_RemoveFrames=
VDXVideoFilterDefinition<JPSDR_RemoveFrames>("JPSDR","RemoveFrames v1.2.4","Filter to remove frames.");

