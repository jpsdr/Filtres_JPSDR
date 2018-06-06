//	VDXFrame - Helper library for VirtualDub plugins
//	Copyright (C) 2008 Avery Lee
//
//	The plugin headers in the VirtualDub plugin SDK are licensed differently
//	differently than VirtualDub and the Plugin SDK themselves.  This
//	particular file is thus licensed as follows (the "zlib" license):
//
//	This software is provided 'as-is', without any express or implied
//	warranty.  In no event will the authors be held liable for any
//	damages arising from the use of this software.
//
//	Permission is granted to anyone to use this software for any purpose,
//	including commercial applications, and to alter it and redistribute it
//	freely, subject to the following restrictions:
//
//	1.	The origin of this software must not be misrepresented; you must
//		not claim that you wrote the original software. If you use this
//		software in a product, an acknowledgment in the product
//		documentation would be appreciated but is not required.
//	2.	Altered source versions must be plainly marked as such, and must
//		not be misrepresented as being the original software.
//	3.	This notice may not be removed or altered from any source
//		distribution.

#include "VideoFilter.h"

#ifndef _JPSDRVIDEOFILTER_H
#define _JPSDRVIDEOFILTER_H

#include <stdint.h>
#include "JPSDR_Filter.h"

class JPSDRVDXVideoFilter : public VDXVideoFilter
{
public:
	uint8_t GetVideoMode(sint32 format);
	uint8_t GetColorMode(sint32 format);
	uint8_t GetInterlaceMode(sint32 format);
	uint8_t GetBitDepth(sint32 format);
	bool CheckFullRangeMode(sint32 format);
	bool CheckAlphaPlanarMode(sint32 format);

	uint8_t GetColorMode(const VDXFBitmap* bitmap);
	uint8_t GetInterlaceMode(const VDXFBitmap* bitmap);
	uint8_t GetBitDepth(const VDXFBitmap* bitmap);
	bool CheckFullRangeMode(const VDXFBitmap* bitmap);

	void SetImageData(Image_Data& iData,bool modeBMP=false);
};


#endif
