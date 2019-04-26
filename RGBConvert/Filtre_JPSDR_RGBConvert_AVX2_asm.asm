;
;  RGBConvert
;
;  RGB to YUV convert filter.
;  Copyright (C) 2005 JPSDR
;	
;  RGBConvert is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  RGBConvert is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;   
;  You should have received a copy of the GNU General Public License
;  along with GNU Make; see the file COPYING.  If not, write to
;  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
;
;

.586
.xmm
.model flat,c

.code


JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb ymm3,ymm3,ymm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax
	mov ecx,w	
	mov ebx,32
	
Convert_Planar420_to_Planar422_8_AVX2_1:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vmovdqa ymm1,YMMWORD ptr[edx+eax]
	vpxor ymm2,ymm0,ymm3
	vpxor ymm1,ymm1,ymm3
	vpavgb ymm2,ymm2,ymm1
	vpxor ymm2,ymm2,ymm3
	vpavgb ymm2,ymm2,ymm0
	
	vmovdqa YMMWORD ptr[edi+eax],ymm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_8_AVX2_1
	
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2 endp


JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb ymm3,ymm3,ymm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w	
	mov ebx,32
	
Convert_Planar420_to_Planar422_16_AVX2_1:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vmovdqa ymm1,YMMWORD ptr[edx+eax]
	vpxor ymm2,ymm0,ymm3
	vpxor ymm1,ymm1,ymm3
	vpavgw ymm2,ymm2,ymm1
	vpxor ymm2,ymm2,ymm3
	vpavgw ymm2,ymm2,ymm0
	
	vmovdqa YMMWORD ptr[edi+eax],ymm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_16_AVX2_1
	
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2 endp


JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2 proc src1:dword,src2:dword,dst:dword,w32:dword,h:dword,src_pitch2:dword,dst_pitch:dword

	public JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ebx,32
	
Convert_Planar422_to_Planar420_8_AVX2_1:
	xor eax,eax
	mov ecx,w32

Convert_Planar422_to_Planar420_8_AVX2_2:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[edx+eax]
	
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,ebx
	loop Convert_Planar422_to_Planar420_8_AVX2_2
	
	add esi,src_pitch2
	add edx,src_pitch2
	add edi,dst_pitch
	dec h
	jnz short Convert_Planar422_to_Planar420_8_AVX2_1
	
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2 endp


JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2 proc src1:dword,src2:dword,dst:dword,w16:dword,h:dword,src_pitch2:dword,dst_pitch:dword

	public JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ebx,32
	
Convert_Planar422_to_Planar420_16_AVX2_1:
	xor eax,eax
	mov ecx,w16

Convert_Planar422_to_Planar420_16_AVX2_2:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[edx+eax]
	
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,ebx
	loop Convert_Planar422_to_Planar420_16_AVX2_2
	
	add esi,src_pitch2
	add edx,src_pitch2
	add edi,dst_pitch
	dec h
	jnz short Convert_Planar422_to_Planar420_16_AVX2_1
	
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2 endp


end





