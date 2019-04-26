;
;  Deinterlace
;
;  Deinterlace video.
;  Copyright (C) 2000 JPSDR
;	
;  Deinterlace is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  Deinterlace is distributed in the hope that it will be useful,
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
.model flat,c

.code

.xmm

JPSDR_Deinterlace_Blend_8_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_AVX2

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,32
	mov edx,src2
	mov edi,dst

loop_1_e_AVX2:
	xor eax,eax
	mov ecx,w
loop_2_e_AVX2:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[edx+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,ebx
	loop loop_2_e_AVX2
	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e_AVX2
	
fin_e_AVX2:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_8_AVX2 endp


JPSDR_Deinterlace_Blend_16_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_AVX2

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,32
	mov edx,src2
	mov edi,dst

loop_1_e_AVX2_16:
	xor eax,eax
	mov ecx,w
loop_2_e_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[edx+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,ebx
	loop loop_2_e_AVX2_16
	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e_AVX2_16
	
fin_e_AVX2_16:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_AVX2 endp


JPSDR_Deinterlace_Blend_8_Tri_AVX2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX2

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[ebx+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_0_g_2_AVX2
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_AVX2:
	xor eax,eax
	mov ecx,w
loop_2_g_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_AVX2
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_AVX2

	xor eax,eax
	mov ecx,w
loop_3_g_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_3_g_2_AVX2
	
fin_g_2_AVX2:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2 endp


JPSDR_Deinterlace_Blend_16_Tri_AVX2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX2

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[ebx+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_0_g_2_AVX2_16
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_AVX2_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_AVX2_16
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_AVX2_16

	xor eax,eax
	mov ecx,w
loop_3_g_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_3_g_2_AVX2_16
	
fin_g_2_AVX2_16:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2 endp


JPSDR_Deinterlace_Blend_8_Tri_AVX2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX2_a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_1_AVX2:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[ebx+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_0_g_2_1_AVX2
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_1_AVX2:
	xor eax,eax
	mov ecx,w
loop_2_g_2_1_AVX2:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_1_AVX2

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_1_AVX2

fin_g_2_1_AVX2:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2_a endp


JPSDR_Deinterlace_Blend_16_Tri_AVX2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX2_a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_1_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[esi+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[ebx+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_0_g_2_1_AVX2_16
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_1_AVX2_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_1_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_1_AVX2_16

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_1_AVX2_16

fin_g_2_1_AVX2_16:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2_a endp


JPSDR_Deinterlace_Blend_8_Tri_AVX2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX2_b

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2_AVX2:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_2_AVX2
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2_AVX2
	
fin_g_2_2_AVX2:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2_b endp


JPSDR_Deinterlace_Blend_16_Tri_AVX2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX2_b

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2_AVX2_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_2_AVX2_16
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2_AVX2_16
	
fin_g_2_2_AVX2_16:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2_b endp


JPSDR_Deinterlace_Blend_8_Tri_AVX2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX2_c

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3_AVX2:
	xor eax,eax
	mov ecx,w
loop_2_g_2_3_AVX2:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_3_AVX2

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_3_AVX2

	xor eax,eax
	mov ecx,w
loop_3_g_2_3_AVX2:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgb ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_3_g_2_3_AVX2
	
fin_g_2_3_AVX2:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2_c endp


JPSDR_Deinterlace_Blend_16_Tri_AVX2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX2_c

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3_AVX2_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_3_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[edx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_2_g_2_3_AVX2_16

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_3_AVX2_16

	xor eax,eax
	mov ecx,w
loop_3_g_2_3_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[ebx+eax]
	vpavgw ymm0,ymm0,YMMWORD ptr[esi+eax]
	vmovdqa YMMWORD ptr[edi+eax],ymm0
	add eax,32
	loop loop_3_g_2_3_AVX2_16
	
fin_g_2_3_AVX2_16:
	vzeroupper
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2_c endp


end





