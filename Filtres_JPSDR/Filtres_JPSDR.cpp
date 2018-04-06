// Filtres_JPSDR.cpp : définit les fonctions exportées pour l'application DLL.
//


#include "VideoFilterEntry.h"
#include "vdvideofilt.h"
#include "ThreadPoolInterface.h"

ThreadPoolInterface *poolInterface;

VDX_DECLARE_VIDEOFILTERS_BEGIN()
	poolInterface=ThreadPoolInterface::Init(8);
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_Deinterlace)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_Saturation)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_IVTC)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_IVTC_Manual)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_Field_Shift)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_BlurMotion)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_Median)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_WideScreen)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_AutoYUY2)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_ChromaShift)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_AddBorder)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_RemoveFrames)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_RGBConvert)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_ColorSpaceConvert)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_CCC)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_InterlaceMotion)
#ifndef _M_AMD64
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_VHS)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_VHS_II)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_VHS_III)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_Smart_Deinterlace)
	VDX_DECLARE_VIDEOFILTER(filterDef_JPSDR_DeinterlaceII)
#endif
VDX_DECLARE_VIDEOFILTERS_END()

VDX_DECLARE_VFMODULE()


HINSTANCE hInstance;
int g_VFVAPIVersion;

BOOL WINAPI DllMain(HINSTANCE hModule, ULONG reason, void*)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		hInstance = hModule;
		break;
	}
	return(TRUE);
}

extern "C" int __declspec(dllexport) __cdecl VirtualdubFilterModuleInit2(VDXFilterModule *fm, const VDXFilterFunctions *ff, int& vdfd_ver, int& vdfd_compat) {
	if (VDXVideoFilterModuleInit2(fm, ff, vdfd_ver)) return 1;

	g_VFVAPIVersion = vdfd_ver;
	vdfd_ver = VIRTUALDUB_FILTERDEF_VERSION;
	vdfd_compat = 9;		// copy constructor support required
	return 0;
}

extern "C" int __declspec(dllexport) __cdecl FilterModModuleInit(VDXFilterModule *fm, const FilterModInitFunctions *ff, int& vdfd_ver, int& vdfd_compat, int& mod_ver, int& mod_min) {
	if (VDXVideoFilterModuleInitFilterMod(fm, ff, vdfd_ver, mod_ver)) return 1;

	g_VFVAPIVersion = vdfd_ver;
	vdfd_ver = VIRTUALDUB_FILTERDEF_VERSION;
	vdfd_compat = 9;		// copy constructor support required

	mod_ver = FILTERMOD_VERSION;
	mod_min = FILTERMOD_VERSION;
	return 0;
}

extern "C" void __declspec(dllexport) __cdecl VirtualdubFilterModuleDeinit(VDXFilterModule *fm, const VDXFilterFunctions *ff) {
}
