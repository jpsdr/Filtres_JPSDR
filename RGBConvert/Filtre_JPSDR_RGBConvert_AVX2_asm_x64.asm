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

.code


;JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2 proc public frame

	.endprolog
		
	vpcmpeqb ymm3,ymm3,ymm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax	
	mov ecx,r9d	
	mov r11,32
	
Convert_Planar420_to_Planar422_8_AVX2_1:
	vmovdqa ymm0,YMMWORD ptr[r10+rax]
	vmovdqa ymm1,YMMWORD ptr[rdx+rax]
	vpxor ymm2,ymm0,ymm3
	vpxor ymm1,ymm1,ymm3
	vpavgb ymm2,ymm2,ymm1
	vpxor ymm2,ymm2,ymm3
	vpavgb ymm2,ymm2,ymm0
	
	vmovdqa YMMWORD ptr[r8+rax],ymm2
	add rax,r11
	loop Convert_Planar420_to_Planar422_8_AVX2_1
	
	vzeroupper
	
	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX2 endp


;JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2 proc public frame

	.endprolog
		
	vpcmpeqb ymm3,ymm3,ymm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax	
	mov ecx,r9d	
	mov r11,32
	
Convert_Planar420_to_Planar422_16_AVX2_1:
	vmovdqa ymm0,YMMWORD ptr[r10+rax]
	vmovdqa ymm1,YMMWORD ptr[rdx+rax]
	vpxor ymm2,ymm0,ymm3
	vpxor ymm1,ymm1,ymm3
	vpavgw ymm2,ymm2,ymm1
	vpxor ymm2,ymm2,ymm3
	vpavgw ymm2,ymm2,ymm0
	
	vmovdqa YMMWORD ptr[r8+rax],ymm2
	add rax,r11
	loop Convert_Planar420_to_Planar422_16_AVX2_1
	
	vzeroupper
	
	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_16_AVX2 endp


;JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2 proc src1:dword,src2:dword,dst:dword,w32:dword,h:dword,src_pitch2:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w32 = r9d

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2 proc public frame

h equ dword ptr[rbp+48]
src_pitch2 equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog		
	
	mov rsi,rcx
	mov r10d,h
	mov rbx,32
	mov r11,src_pitch2
	mov r12,dst_pitch
	xor rcx,rcx

Convert_Planar422_to_Planar420_8_AVX2_1:
	xor rax,rax
	mov ecx,r9d

Convert_Planar422_to_Planar420_8_AVX2_2:
	vmovdqa ymm0,YMMWORD ptr[rsi+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rdx+rax]
	
	vmovdqa YMMWORD ptr[r8+rax],ymm0
	add rax,rbx
	loop Convert_Planar422_to_Planar420_8_AVX2_2
	
	add rsi,r11
	add rdx,r11
	add r8,r12
	dec r10d
	jnz short Convert_Planar422_to_Planar420_8_AVX2_1
	
	vzeroupper

	pop r12
	pop rbx
	pop rsi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX2 endp


;JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2 proc src1:dword,src2:dword,dst:dword,w16:dword,h:dword,src_pitch2:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w16 = r9d

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2 proc public frame

h equ dword ptr[rbp+48]
src_pitch2 equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog		
	
	mov rsi,rcx
	mov r10d,h
	mov rbx,32
	mov r11,src_pitch2
	mov r12,dst_pitch
	xor rcx,rcx

Convert_Planar422_to_Planar420_16_AVX2_1:
	xor rax,rax
	mov ecx,r9d

Convert_Planar422_to_Planar420_16_AVX2_2:
	vmovdqa ymm0,YMMWORD ptr[rsi+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rdx+rax]
	
	vmovdqa YMMWORD ptr[r8+rax],ymm0
	add rax,rbx
	loop Convert_Planar422_to_Planar420_16_AVX2_2
	
	add rsi,r11
	add rdx,r11
	add r8,r12
	dec r10d
	jnz short Convert_Planar422_to_Planar420_16_AVX2_1

	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_16_AVX2 endp


end
