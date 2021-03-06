;
;  AutoYUY2()
;
;  Adaptive YV12 upsampling. Progressive picture areas are upsampled
;  progressively and interlaced areas are upsampled interlaced.
;  Copyright (C) 2005 Donald A. Graft
;  ASM part made by JPSDR
;	
;  AutoYUY2 is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  AutoYUY2 is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;   
;  You should have received a copy of the GNU General Public License
;  along with GNU Make; see the file COPYING.  If not, write to
;  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
;

.586
.model flat,c

.data

align 16

uw_2 word 8 dup(2)
uw_3 word 8 dup(3)
uw_4 word 8 dup(4)
uw_5 word 8 dup(5)
uw_7 word 8 dup(7)

.code


JPSDR_AutoYUY2_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_1

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	cld

SSE2_1_a:
	mov al,byte ptr[esi+1]		;al=y2
	mov ah,byte ptr[edx]		;ah=v
	inc edx
	shl eax,16
	lodsw				;al=y1 ah=y2
	mov ah,byte ptr[ebx]		;ah=u
	inc ebx
	stosd
	loop SSE2_1_a
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_1 endp


JPSDR_AutoYUY2_Convert420_to_UYVY_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_1

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	cld

SSE2_1_b:
	lodsw						;al=y1 ah=y2
	mov al,byte ptr[edx]		;al=v
	inc edx
	shl eax,16
	mov ah,byte ptr[esi-2]		;ah=y1
	mov al,byte ptr[ebx]		;al=u
	inc ebx
	stosd
	loop SSE2_1_b
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_1 endp


.xmm


JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	xor eax,eax
	
	shr ecx,1
	jz short _SSE2_1b_b
	
_SSE2_1b_a:
	movq xmm1,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm0,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	movdqa xmm2,XMMWORD ptr[esi+8*eax]	;YYYYYYYYYYYYYYYY	
	punpcklbw xmm1,xmm0					;VUVUVUVUVUVUVUVU
	movdqa xmm3,xmm2
	add eax,2
	punpcklbw xmm2,xmm1     			;VYUYVYUYVYUYVYUY
	punpckhbw xmm3,xmm1     			;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[edi],xmm2
	movdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	loop _SSE2_1b_a
	
_SSE2_1b_b:
	mov ecx,w
	and ecx,1
	jz short _SSE2_1b_c

	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	movq xmm2,qword ptr[esi+8*eax]		;00000000YYYYYYYY	
	punpcklbw xmm1,xmm0					;00000000VUVUVUVU
	punpcklbw xmm2,xmm1     			;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[edi],xmm2	
	
_SSE2_1b_c:	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	xor eax,eax
	
	shr ecx,1
	jz short _AVX_1b_b
	
_AVX_1b_a:
	vmovq xmm1,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm0,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vmovdqa xmm2,XMMWORD ptr[esi+8*eax]	;YYYYYYYYYYYYYYYY	
	vpunpcklbw xmm1,xmm1,xmm0			;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpckhbw xmm3,xmm2,xmm1  			;VYUYVYUYVYUYVYUY
	vpunpcklbw xmm2,xmm2,xmm1     			;VYUYVYUYVYUYVYUY
	
	vmovdqa XMMWORD ptr[edi],xmm2
	vmovdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	loop _AVX_1b_a
	
_AVX_1b_b:
	mov ecx,w
	and ecx,1
	jz short _AVX_1b_c

	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vmovq xmm2,qword ptr[esi+8*eax]		;00000000YYYYYYYY	
	vpunpcklbw xmm1,xmm1,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm1  			;VYUYVYUYVYUYVYUY
	
	vmovdqa XMMWORD ptr[edi],xmm2	
	
_AVX_1b_c:	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_1b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	xor eax,eax
	
	shr ecx,1
	jz short _SSE2_0b_b
	
_SSE2_0b_a:
	movq xmm1,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm0,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	movdqa xmm2,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm0					;VUVUVUVUVUVUVUVU
	add eax,2
	movdqa xmm3,xmm1
	punpcklbw xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	punpckhbw xmm3,xmm2     			;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[edi],xmm1
	movdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	loop _SSE2_0b_a
	
_SSE2_0b_b:
	mov ecx,w
	and ecx,1
	jz short _SSE2_0b_c
	
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	movq xmm2,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	punpcklbw xmm1,xmm0					;00000000VUVUVUVU
	punpcklbw xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[edi],xmm1	
	
_SSE2_0b_c:	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	xor eax,eax
	
	shr ecx,1
	jz short _AVX_0b_b
	
_AVX_0b_a:
	vmovq xmm1,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm0,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vmovdqa xmm2,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm0				;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpckhbw xmm3,xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	vpunpcklbw xmm1,xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	
	vmovdqa XMMWORD ptr[edi],xmm1
	vmovdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	loop _AVX_0b_a
	
_AVX_0b_b:
	mov ecx,w
	and ecx,1
	jz short _AVX_0b_c
	
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vmovq xmm2,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm0				;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	
	vmovdqa XMMWORD ptr[edi],xmm1	
	
_AVX_0b_c:	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_1b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_3
	movdqa xmm4,XMMWORD ptr uw_5
	
	xor eax,eax
	shr ecx,1
	jz _SSE2_2b_b

_SSE2_2b_a:
	mov ebx,src1_u
	mov edx,src1_v
	movq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	movq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	pmullw xmm2,xmm4
	pmullw xmm3,xmm4	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	movdqa xmm2,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm3,xmm2
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	punpcklbw xmm2,xmm0				;VYUYVYUYVYUYVYUY
	punpckhbw xmm3,xmm0				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi],xmm2
	movdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	
	dec ecx
	jnz _SSE2_2b_a
	
_SSE2_2b_b:
	mov ecx,w
	and ecx,1
	jz short _SSE2_2b_c
		
	mov ebx,src1_u
	mov edx,src1_v
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	movd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	
	
	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	movq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[edi],xmm0	
		
_SSE2_2b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b
	
	push esi
	push edi
	push ebx
	
	vpxor xmm7,xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	vmovdqa xmm6,XMMWORD ptr uw_4
	vmovdqa xmm5,XMMWORD ptr uw_3
	vmovdqa xmm4,XMMWORD ptr uw_5
	
	xor eax,eax
	shr ecx,1
	jz _AVX_2b_b

_AVX_2b_a:
	mov ebx,src1_u
	mov edx,src1_v
	vmovq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm1,xmm1,xmm7			;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	vmovq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm2,xmm2,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm3,xmm3,xmm7			;0V0V0V0V0V0V0V0V
	
	vpmullw xmm0,xmm0,xmm5
	vpmullw xmm1,xmm1,xmm5	
	vpmullw xmm2,xmm2,xmm4
	vpmullw xmm3,xmm3,xmm4	
	vpaddsw xmm2,xmm2,xmm6
	vpaddsw xmm3,xmm3,xmm6
	vpaddsw xmm0,xmm0,xmm2
	vpaddsw xmm1,xmm1,xmm3
	vmovdqa xmm2,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	vpsraw xmm0,xmm0,3
	vpsraw xmm1,xmm1,3
	vpackuswb xmm0,xmm0,xmm7			;00000000UUUUUUUU
	vpackuswb xmm1,xmm1,xmm7			;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpckhbw xmm3,xmm2,xmm0			;VYUYVYUYVYUYVYUY
	vpunpcklbw xmm2,xmm2,xmm0			;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi],xmm2
	vmovdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	
	dec ecx
	jnz _AVX_2b_a
	
_AVX_2b_b:
	mov ecx,w
	and ecx,1
	jz short _AVX_2b_c
		
	mov ebx,src1_u
	mov edx,src1_v
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm1,xmm1,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm7			;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	vmovd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm2,xmm2,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm7			;0V0U0V0U0V0U0V0U	
	
	vpmullw xmm1,xmm1,xmm5
	vpmullw xmm2,xmm2,xmm4
	vpaddsw xmm1,xmm1,xmm6
	vmovq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpaddsw xmm1,xmm1,xmm2
	vpsraw xmm1,xmm1,3
	vpackuswb xmm1,xmm1,xmm7			;00000000VUVUVUVU
	vpunpcklbw xmm0,xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	vmovdqa XMMWORD ptr[edi],xmm0	
		
_AVX_2b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_2b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_3
	movdqa xmm4,XMMWORD ptr uw_5
	
	xor eax,eax
	shr ecx,1
	jz _SSE2_6b_b

_SSE2_6b_a:
	mov ebx,src1_u
	mov edx,src1_v
	movq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	movq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	pmullw xmm2,xmm4
	pmullw xmm3,xmm4	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm2,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	movdqa xmm3,xmm0
	punpcklbw xmm0,xmm2				;YVYUYVYUYVYUYVYU
	punpckhbw xmm3,xmm2				;YVYUYVYUYVYUYVYU
	movdqa XMMWORD ptr[edi],xmm0
	movdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	
	dec ecx
	jnz _SSE2_6b_a
	
_SSE2_6b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_6b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	movd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	
	
	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	movq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[edi],xmm1	
		
_SSE2_6b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b
	
	push esi
	push edi
	push ebx
	
	vpxor xmm7,xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	vmovdqa xmm6,XMMWORD ptr uw_4
	vmovdqa xmm5,XMMWORD ptr uw_3
	vmovdqa xmm4,XMMWORD ptr uw_5
	
	xor eax,eax
	shr ecx,1
	jz _AVX_6b_b

_AVX_6b_a:
	mov ebx,src1_u
	mov edx,src1_v
	vmovq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm1,xmm1,xmm7			;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	vmovq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm2,xmm2,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm3,xmm3,xmm7			;0V0V0V0V0V0V0V0V
	
	vpmullw xmm0,xmm0,xmm5
	vpmullw xmm1,xmm1,xmm5	
	vpmullw xmm2,xmm2,xmm4
	vpmullw xmm3,xmm3,xmm4	
	vpaddsw xmm2,xmm2,xmm6
	vpaddsw xmm3,xmm3,xmm6
	vpaddsw xmm0,xmm0,xmm2
	vpaddsw xmm1,xmm1,xmm3
	vpsraw xmm0,xmm0,3
	vpsraw xmm1,xmm1,3
	vpackuswb xmm0,xmm0,xmm7				;00000000UUUUUUUU
	vpackuswb xmm1,xmm1,xmm7				;00000000VVVVVVVV
	vmovdqa xmm2,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm0,xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpckhbw xmm3,xmm0,xmm2			;YVYUYVYUYVYUYVYU
	vpunpcklbw xmm0,xmm0,xmm2			;YVYUYVYUYVYUYVYU
	vmovdqa XMMWORD ptr[edi],xmm0
	vmovdqa XMMWORD ptr[edi+16],xmm3
	add edi,32
	
	dec ecx
	jnz _AVX_6b_a
	
_AVX_6b_b:	
	mov ecx,w
	and ecx,1
	jz short _AVX_6b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]	;000000000000VVVV
	vpunpcklbw xmm1,xmm1,xmm0				;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	vmovd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm2,xmm2,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm7			;0V0U0V0U0V0U0V0U	
	
	vpmullw xmm1,xmm1,xmm5
	vpmullw xmm2,xmm2,xmm4
	vpaddsw xmm1,xmm1,xmm6
	vmovq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpaddsw xmm1,xmm1,xmm2
	vpsraw xmm1,xmm1,3
	vpackuswb xmm1,xmm1,xmm7			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	vmovdqa XMMWORD ptr[edi],xmm1	
		
_AVX_6b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_2b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_7
	
	xor eax,eax
	shr ecx,1
	jz _SSE2_3b_b

_SSE2_3b_a:
	mov ebx,src1_u
	mov edx,src1_v
	movq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	movq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	movdqa xmm4,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm2,xmm4
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	punpcklbw xmm2,xmm0				;VYUYVYUYVYUYVYUY
	punpckhbw xmm4,xmm0				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi],xmm2
	movdqa XMMWORD ptr[edi+16],xmm4
	add edi,32
	
	loop _SSE2_3b_a
	
_SSE2_3b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_3b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	movd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[edi],xmm0	
		
_SSE2_3b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b
	
	push esi
	push edi
	push ebx
	
	vpxor xmm7,xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	vmovdqa xmm6,XMMWORD ptr uw_4
	vmovdqa xmm5,XMMWORD ptr uw_7
	
	xor eax,eax
	shr ecx,1
	jz _AVX_3b_b

_AVX_3b_a:
	mov ebx,src1_u
	mov edx,src1_v
	vmovq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm1,xmm1,xmm7			;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	vmovq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm2,xmm2,xmm7				;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm3,xmm3,xmm7				;0V0V0V0V0V0V0V0V
	vmovdqa xmm4,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	
	vpmullw xmm0,xmm0,xmm5
	vpmullw xmm1,xmm1,xmm5	
	vpaddsw xmm2,xmm2,xmm6
	vpaddsw xmm3,xmm3,xmm6
	vpaddsw xmm0,xmm0,xmm2
	vpaddsw xmm1,xmm1,xmm3
	vpsraw xmm0,xmm0,3
	vpsraw xmm1,xmm1,3
	vpackuswb xmm0,xmm0,xmm7			;00000000UUUUUUUU
	vpackuswb xmm1,xmm1,xmm7			;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpcklbw xmm2,xmm4,xmm0			;VYUYVYUYVYUYVYUY
	vpunpckhbw xmm4,xmm4,xmm0			;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi],xmm2
	vmovdqa XMMWORD ptr[edi+16],xmm4
	add edi,32
	
	loop _AVX_3b_a
	
_AVX_3b_b:	
	mov ecx,w
	and ecx,1
	jz short _AVX_3b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm1,xmm1,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm7			;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	vmovd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm2,xmm2,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm7			;0V0U0V0U0V0U0V0U	
	
	vpmullw xmm1,xmm1,xmm5
	vpaddsw xmm2,xmm2,xmm6
	vmovq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpaddsw xmm1,xmm1,xmm2
	vpsraw xmm1,xmm1,3
	vpackuswb xmm1,xmm1,xmm7			;00000000VUVUVUVU
	vpunpcklbw xmm0,xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	vmovdqa XMMWORD ptr[edi],xmm0	
		
_AVX_3b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_3b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_7
	
	xor eax,eax
	shr ecx,1
	jz _SSE2_7b_b

_SSE2_7b_a:
	mov ebx,src1_u
	mov edx,src1_v
	movq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	movq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm4,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm4				;VYUYVYUYVYUYVYUY
	punpckhbw xmm2,xmm4				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi],xmm0
	movdqa XMMWORD ptr[edi+16],xmm2
	add edi,32
	
	loop _SSE2_7b_a
	
_SSE2_7b_b:
	mov ecx,w
	and ecx,1
	jz short _SSE2_7b_c

	mov ebx,src1_u
	mov edx,src1_v
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	movd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[edi],xmm1

_SSE2_7b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b
	
	push esi
	push edi
	push ebx
	
	vpxor xmm7,xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	vmovdqa xmm6,XMMWORD ptr uw_4
	vmovdqa xmm5,XMMWORD ptr uw_7
	
	xor eax,eax
	shr ecx,1
	jz _AVX_7b_b

_AVX_7b_a:
	mov ebx,src1_u
	mov edx,src1_v
	vmovq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm1,xmm1,xmm7			;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	vmovq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm2,xmm2,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm3,xmm3,xmm7			;0V0V0V0V0V0V0V0V
	
	vpmullw xmm0,xmm0,xmm5
	vpmullw xmm1,xmm1,xmm5	
	vpaddsw xmm2,xmm2,xmm6
	vpaddsw xmm3,xmm3,xmm6
	vpaddsw xmm0,xmm0,xmm2
	vpaddsw xmm1,xmm1,xmm3
	vpsraw xmm0,xmm0,3
	vpsraw xmm1,xmm1,3
	vpackuswb xmm0,xmm0,xmm7			;00000000UUUUUUUU
	vpackuswb xmm1,xmm1,xmm7			;00000000VVVVVVVV
	vmovdqa xmm4,XMMWORD ptr[esi+8*eax]	;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm0,xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpckhbw xmm2,xmm0,xmm4			;VYUYVYUYVYUYVYUY
	vpunpcklbw xmm0,xmm0,xmm4			;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi],xmm0
	vmovdqa XMMWORD ptr[edi+16],xmm2
	add edi,32
	
	loop _AVX_7b_a
	
_AVX_7b_b:
	mov ecx,w
	and ecx,1
	jz short _AVX_7b_c

	mov ebx,src1_u
	mov edx,src1_v
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm1,xmm1,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm7			;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	vmovd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm2,xmm2,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm7			;0V0U0V0U0V0U0V0U	
	
	vpmullw xmm1,xmm1,xmm5
	vpaddsw xmm2,xmm2,xmm6
	vmovq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpaddsw xmm1,xmm1,xmm2
	vpsraw xmm1,xmm1,3
	vpackuswb xmm1,xmm1,xmm7			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	vmovdqa XMMWORD ptr[edi],xmm1

_AVX_7b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_3b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_2
	movdqa xmm5,XMMWORD ptr uw_3
	
	xor eax,eax
	shr ecx,1
	jz _SSE2_4b_b

_SSE2_4b_a:
	mov ebx,src1_u
	mov edx,src1_v
	movq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	movq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	movdqa xmm4,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,2
	psraw xmm1,2
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm2,xmm4
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	punpcklbw xmm2,xmm0				;VYUYVYUYVYUYVYUY
	punpckhbw xmm4,xmm0				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi],xmm2
	movdqa XMMWORD ptr[edi+16],xmm4
	add edi,32
	
	loop _SSE2_4b_a
	
_SSE2_4b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_4b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	movd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,2
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[edi],xmm0	
	
_SSE2_4b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b endp


JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b
	
	push esi
	push edi
	push ebx
	
	vpxor xmm7,xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	vmovdqa xmm6,XMMWORD ptr uw_2
	vmovdqa xmm5,XMMWORD ptr uw_3
	
	xor eax,eax
	shr ecx,1
	jz _AVX_4b_b

_AVX_4b_a:
	mov ebx,src1_u
	mov edx,src1_v
	vmovq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm1,xmm1,xmm7			;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	vmovq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm2,xmm2,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm3,xmm3,xmm7			;0V0V0V0V0V0V0V0V
	vmovdqa xmm4,XMMWORD ptr[esi+8*eax]	;YYYYYYYYYYYYYYYY
	
	vpmullw xmm0,xmm0,xmm5
	vpmullw xmm1,xmm1,xmm5	
	vpaddsw xmm2,xmm2,xmm6
	vpaddsw xmm3,xmm3,xmm6
	vpaddsw xmm0,xmm0,xmm2
	vpaddsw xmm1,xmm1,xmm3
	vpsraw xmm0,xmm0,2
	vpsraw xmm1,xmm1,2
	vpackuswb xmm0,xmm0,xmm7			;00000000UUUUUUUU
	vpackuswb xmm1,xmm1,xmm7			;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpcklbw xmm2,xmm4,xmm0			;VYUYVYUYVYUYVYUY
	vpunpckhbw xmm4,xmm4,xmm0			;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi],xmm2
	vmovdqa XMMWORD ptr[edi+16],xmm4
	add edi,32
	
	loop _AVX_4b_a
	
_AVX_4b_b:	
	mov ecx,w
	and ecx,1
	jz short _AVX_4b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm1,xmm1,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm7			;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	vmovd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm2,xmm2,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm7			;0V0U0V0U0V0U0V0U	
	
	vpmullw xmm1,xmm1,xmm5
	vpaddsw xmm2,xmm2,xmm6
	vmovq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpaddsw xmm1,xmm1,xmm2
	vpsraw xmm1,xmm1,2
	vpackuswb xmm1,xmm1,xmm7			;00000000VUVUVUVU
	vpunpcklbw xmm0,xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	vmovdqa XMMWORD ptr[edi],xmm0	
	
_AVX_4b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_AVX_4b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_2
	movdqa xmm5,XMMWORD ptr uw_3
	
	xor eax,eax
	shr ecx,1
	jz _SSE2_8b_b

_SSE2_8b_a:
	mov ebx,src1_u
	mov edx,src1_v
	movq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	movq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,2
	psraw xmm1,2
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm4,XMMWORD ptr[esi+8*eax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm4				;VYUYVYUYVYUYVYUY
	punpckhbw xmm2,xmm4				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi],xmm0
	movdqa XMMWORD ptr[edi+16],xmm2
	add edi,32
	
	loop _SSE2_8b_a
	
_SSE2_8b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_8b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	movd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	movd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	movd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,2
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[edi],xmm1	
		
_SSE2_8b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b endp


JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b
	
	push esi
	push edi
	push ebx
	
	vpxor xmm7,xmm7,xmm7
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	
	vmovdqa xmm6,XMMWORD ptr uw_2
	vmovdqa xmm5,XMMWORD ptr uw_3
	
	xor eax,eax
	shr ecx,1
	jz _AVX_8b_b

_AVX_8b_a:
	mov ebx,src1_u
	mov edx,src1_v
	vmovq xmm0,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm1,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm0,xmm0,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm1,xmm1,xmm7			;0V0V0V0V0V0V0V0V
	mov ebx,src2_u
	mov edx,src2_v
	vmovq xmm2,qword ptr[ebx+4*eax]		;00000000UUUUUUUU
	vmovq xmm3,qword ptr[edx+4*eax]		;00000000VVVVVVVV
	vpunpcklbw xmm2,xmm2,xmm7			;0U0U0U0U0U0U0U0U
	vpunpcklbw xmm3,xmm3,xmm7			;0V0V0V0V0V0V0V0V
	
	vpmullw xmm0,xmm0,xmm5
	vpmullw xmm1,xmm1,xmm5	
	vpaddsw xmm2,xmm2,xmm6
	vpaddsw xmm3,xmm3,xmm6
	vpaddsw xmm0,xmm0,xmm2
	vpaddsw xmm1,xmm1,xmm3
	vpsraw xmm0,xmm0,2
	vpsraw xmm1,xmm1,2
	vpackuswb xmm0,xmm0,xmm7			;00000000UUUUUUUU
	vpackuswb xmm1,xmm1,xmm7			;00000000VVVVVVVV
	vmovdqa xmm4,XMMWORD ptr[esi+8*eax]	;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm0,xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add eax,2
	vpunpckhbw xmm2,xmm0,xmm4			;VYUYVYUYVYUYVYUY
	vpunpcklbw xmm0,xmm0,xmm4			;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi],xmm0
	vmovdqa XMMWORD ptr[edi+16],xmm2
	add edi,32
	
	loop _AVX_8b_a
	
_AVX_8b_b:	
	mov ecx,w
	and ecx,1
	jz short _AVX_8b_c
	
	mov ebx,src1_u
	mov edx,src1_v
	vmovd xmm1,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm1,xmm1,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm7			;0V0U0V0U0V0U0V0U
	mov ebx,src2_u
	mov edx,src2_v
	vmovd xmm2,dword ptr[ebx+4*eax]		;000000000000UUUU
	vmovd xmm0,dword ptr[edx+4*eax]		;000000000000VVVV
	vpunpcklbw xmm2,xmm2,xmm0			;00000000VUVUVUVU
	vpunpcklbw xmm2,xmm2,xmm7			;0V0U0V0U0V0U0V0U	
	
	vpmullw xmm1,xmm1,xmm5
	vpaddsw xmm2,xmm2,xmm6
	vmovq xmm0,qword ptr[esi+8*eax]		;00000000YYYYYYYY
	vpaddsw xmm1,xmm1,xmm2
	vpsraw xmm1,xmm1,2
	vpackuswb xmm1,xmm1,xmm7			;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	vmovdqa XMMWORD ptr[edi],xmm1	
		
_AVX_8b_c:		
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_AVX_4b endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2 proc src1:dword,src2:dword,dst:dword,w16:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w16	
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x1_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x1_8_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_SSE2 endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX proc src1:dword,src2:dword,dst:dword,w16:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w16	
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x1_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgb xmm2,xmm2,xmm1
	vpxor xmm2,xmm2,xmm3
	vpavgb xmm2,xmm2,xmm0
	
	vmovdqa XMMWORD ptr[edi+eax],xmm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x1_8_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_8_AVX endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2 proc src1:dword,src2:dword,dst:dword,w8:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w8
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x1_16_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgw xmm0,xmm1
	pxor xmm0,xmm3
	pavgw xmm0,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x1_16_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_SSE2 endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX proc src1:dword,src2:dword,dst:dword,w8:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w8
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x1_16_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgw xmm2,xmm2,xmm1
	vpxor xmm2,xmm2,xmm3
	vpavgw xmm2,xmm2,xmm0
	
	vmovdqa XMMWORD ptr[edi+eax],xmm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x1_16_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x1_16_AVX endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2 proc src1:dword,src2:dword,dst:dword,w16:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w16
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x5_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x5_8_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_SSE2 endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX proc src1:dword,src2:dword,dst:dword,w16:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w16	
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x5_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgb xmm2,xmm2,xmm1
	vpavgb xmm2,xmm2,xmm1
	vpxor xmm2,xmm2,xmm3
	vpavgb xmm2,xmm2,xmm0
	
	vmovdqa XMMWORD ptr[edi+eax],xmm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x5_8_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_8_AVX endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2 proc src1:dword,src2:dword,dst:dword,w8:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w8
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x5_16_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgw xmm0,xmm1
	pavgw xmm0,xmm1
	pxor xmm0,xmm3
	pavgw xmm0,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x5_16_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_SSE2 endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX proc src1:dword,src2:dword,dst:dword,w8:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w8
	mov ebx,16
	
Convert_Planar420_to_Planar422_x3x5_16_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgw xmm2,xmm2,xmm1
	vpavgw xmm2,xmm2,xmm1
	vpxor xmm2,xmm2,xmm3
	vpavgw xmm2,xmm2,xmm0
	
	vmovdqa XMMWORD ptr[edi+eax],xmm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x3x5_16_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x3x5_16_AVX endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2 proc src1:dword,src2:dword,dst:dword,w16:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w16
	mov ebx,16
	
Convert_Planar420_to_Planar422_x7x1_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm1,xmm0
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm1
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x7x1_8_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_SSE2 endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX proc src1:dword,src2:dword,dst:dword,w16:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w16	
	mov ebx,16
	
Convert_Planar420_to_Planar422_x7x1_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgb xmm1,xmm1,xmm2
	vpavgb xmm1,xmm1,xmm2
	vpxor xmm1,xmm1,xmm3
	vpavgb xmm1,xmm1,xmm0
	vmovdqa XMMWORD ptr[edi+eax],xmm1
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x7x1_8_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_8_AVX endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2 proc src1:dword,src2:dword,dst:dword,w8:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w8
	mov ebx,16
	
Convert_Planar420_to_Planar422_x7x1_16_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgw xmm1,xmm0
	pavgw xmm1,xmm0
	pxor xmm1,xmm3
	pavgw xmm1,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm1
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x7x1_16_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_SSE2 endp


JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX proc src1:dword,src2:dword,dst:dword,w8:dword

	public JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w8
	mov ebx,16
	
Convert_Planar420_to_Planar422_x7x1_16_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgw xmm1,xmm1,xmm2
	vpavgw xmm1,xmm1,xmm2
	vpxor xmm1,xmm1,xmm3
	vpavgw xmm1,xmm1,xmm0
	vmovdqa XMMWORD ptr[edi+eax],xmm1
	add eax,ebx
	loop Convert_Planar420_to_Planar422_x7x1_16_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert_Planar420_to_Planar422_x7x1_16_AVX endp


end





