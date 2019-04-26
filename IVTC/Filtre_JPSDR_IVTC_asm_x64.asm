;
;  IVTC
;
;  Automatic 2:3 pulldown detection.
;  Copyright (C) 2004 JPSDR
;	
;  IVTC is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  IVTC is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;   
;  You should have received a copy of the GNU General Public License
;  along with GNU Make; see the file COPYING.  If not, write to
;  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
;
;

.data

align 16

ub_FC dword 4 dup(00FCFCFCh)
ub_FF qword 2 dup(0000FFFFFFFFFFFFh)
filtre_1 dword 4 dup(00FFFFFFh)

.code


;JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
;	repart:dword,w:dword,h:dword,src_pitch:dword,scr_modulo:dword,dst_pitch:dword,dst_modulo:dword,w_map:dword,error_motion_map:dword
; src = rcx
; map = rdx
; dst = r8
; histo = r9

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 proc public frame

repart equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]
src_pitch equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_pitch equ qword ptr[rbp+88]
dst_modulo equ qword ptr[rbp+96]
w_map equ dword ptr[rbp+104]
error_motion_map equ qword ptr[rbp+112]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	cld
	xor rax,rax
	mov rbx,rdx				; rbx = map
	mov rsi,rcx				; rsi = src
	mov rdx,r9				; rdx = histo
	xor rcx,rcx
		
	mov rdi,r9
	mov ecx,128
	rep stosq
	
	mov rdi,r8				;rdi = dst
	xor r8,r8
	xor r9,r9
	xor r15,r15
	mov r8d,w
	mov r9d,h
	mov r10,repart
	mov r11,dst_pitch
	
	xor r12,r12
	xor r13,r13
	mov r12d,w_map
	mov r13d,r8d
	shl r12,1
	sub r12,r13
	
	mov ecx,w_map
	mov r13,4
	mov r14,src_pitch

	pxor mm1,mm1	; return value (s)
	pxor mm2,mm2	; error_motion_map
	pxor mm3,mm3

Loop_A_1:
	mov r15d,r8d
Loop_A_2:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]
	movq mm4,mm0
	movq mm6,mm0
	psadbw mm4,mm3
	paddd mm1,mm4
	punpcklbw mm0,mm3
	pextrw eax,mm0,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,1
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,2
	inc dword ptr[rdx+4*rax]
	mov al,byte ptr[rbx]
	or al,al
	jz short Next_1
	movq mm5,mm6
	paddd mm2,mm4
Next_1:
	movd dword ptr[rdi],mm5

	pxor mm5,mm5
	movd mm0,dword ptr[rsi+r14]
	movq mm4,mm0
	punpcklbw mm0,mm3
	pextrw eax,mm0,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,1
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,2
	inc dword ptr[rdx+4*rax]
	mov al,byte ptr[rbx+rcx]
	or al,al
	jz short Next_2
	movq mm5,mm4
	psadbw mm4,mm3
	paddd mm2,mm4
Next_2:
	movd dword ptr[rdi+r11],mm5

	add rdi,r13
	add rsi,r13
	inc rbx
	dec r15
	jnz Loop_A_2
	add rdi,dst_modulo
	add rbx,r12
	add rsi,src_modulo
	add rdi,r11
	add rsi,r14
	dec r9d
	jnz Loop_A_1

	mov rsi,rdx
	mov rdi,r10
	add rsi,1020
	add rdi,1020
	xor rbx,rbx
	std
	mov ecx,256
Loop_A_3:
	lodsd
	add ebx,eax
	mov eax,ebx
	stosd
	loop Loop_A_3
	cld
	
	mov rdi,error_motion_map
	movd dword ptr[rdi],mm2
	movd eax,mm1

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 endp


;JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
;	repart:dword,w:dword,h:dword,src_pitch:dword,scr_modulo:dword,dst_pitch:dword,dst_modulo:dword,w_map:dword,error_motion_map:dword
; src = rcx
; map = rdx
; dst = r8
; histo = r9

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32 proc public frame

repart equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]
src_pitch equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_pitch equ qword ptr[rbp+88]
dst_modulo equ qword ptr[rbp+96]
w_map equ dword ptr[rbp+104]
error_motion_map equ qword ptr[rbp+112]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,32
	.allocstack 32
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	.endprolog

	cld
	xor rax,rax
	mov rbx,rdx				; rbx = map
	mov rsi,rcx				; rsi = src
	mov rdx,r9				; rdx = histo
	xor rcx,rcx
		
	mov rdi,r9
	mov ecx,128
	rep stosq
	
	mov rdi,r8				;rdi = dst
	xor r8,r8
	xor r9,r9
	xor r15,r15
	mov r8d,w
	mov r9d,h
	mov r10,4
	mov r11,dst_pitch
	
	xor r12,r12
	xor r13,r13
	mov r12d,w_map
	mov r13d,r8d
	shl r12,1
	shl r13,2
	sub r12,r13	
	
	mov ecx,w_map
	mov r13,16
	mov r14,src_pitch

	pxor xmm1,xmm1	; return value (s)
	pxor xmm2,xmm2	; error_motion_map
	pxor xmm3,xmm3

Loop_A_1b:
	mov r15d,r8d
Loop_A_2b:
	movd xmm5,dword ptr[rbx]
	punpcklbw xmm5,xmm3
	punpcklwd xmm5,xmm3

	movdqa xmm0,XMMWORD ptr[rsi]
	pcmpgtd xmm5,xmm3
	movdqa xmm4,xmm0
	pand xmm5,xmm0
	movdqa xmm7,xmm0
	psadbw xmm4,xmm3
	movdqa xmm6,xmm5
	paddd xmm1,xmm4
	psadbw xmm5,xmm3
	punpcklbw xmm0,xmm3
	paddd xmm2,xmm5
	pextrw eax,xmm0,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm0,1
	inc dword ptr [rdx+4*rax]
	pextrw eax,xmm0,2
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm0,4
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm0,5
	inc dword ptr [rdx+4*rax]
	punpckhbw xmm7,xmm3
	pextrw eax,xmm0,6
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,1
	inc dword ptr [rdx+4*rax]
	pextrw eax,xmm7,2
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,4
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,5
	inc dword ptr [rdx+4*rax]
	pextrw eax,xmm7,6
	inc dword ptr[rdx+4*rax]
	
	movdqa XMMWORD ptr[rdi],xmm6
	
	movd xmm5,dword ptr[rbx+rcx]
	punpcklbw xmm5,xmm3
	punpcklwd xmm5,xmm3	

	movdqa xmm0,XMMWORD ptr[rsi+r14]
	pcmpgtd xmm5,xmm3
	pand xmm5,xmm0
	movdqa xmm7,xmm0
	movdqa xmm6,xmm5
	psadbw xmm5,xmm3
	punpcklbw xmm0,xmm3
	paddd xmm2,xmm5
	pextrw eax,xmm0,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm0,1
	inc dword ptr [rdx+4*rax]
	pextrw eax,xmm0,2
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm0,4
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm0,5
	inc dword ptr [rdx+4*rax]
	punpckhbw xmm7,xmm3
	pextrw eax,xmm0,6
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,1
	inc dword ptr [rdx+4*rax]
	pextrw eax,xmm7,2
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,4
	inc dword ptr[rdx+4*rax]
	pextrw eax,xmm7,5
	inc dword ptr [rdx+4*rax]
	pextrw eax,xmm7,6
	inc dword ptr[rdx+4*rax]
	
	movdqa XMMWORD ptr[rdi+r11],xmm6

	add rdi,r13
	add rsi,r13
	add rbx,r10
	dec r15
	jnz Loop_A_2b
	add rdi,dst_modulo
	add rbx,r12
	add rsi,r14
	add rdi,r11
	add rsi,src_modulo
	dec r9d
	jnz Loop_A_1b

	mov rsi,rdx
	mov rdi,repart
	add rsi,1020
	add rdi,1020
	xor rbx,rbx
	std
	mov ecx,256
Loop_A_3b:
	lodsd
	add ebx,eax
	mov eax,ebx
	stosd
	loop Loop_A_3b
	cld

	movhlps xmm0,xmm1
	movhlps xmm4,xmm2
	paddd xmm1,xmm0
	paddd xmm2,xmm4
	
	mov rdi,error_motion_map
	movd dword ptr[rdi],xmm2
	movd eax,xmm1

	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]	
	add rsp,32	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32 endp


;JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_AVX_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
;	repart:dword,w:dword,h:dword,src_pitch:dword,scr_modulo:dword,dst_pitch:dword,dst_modulo:dword,w_map:dword,error_motion_map:dword
; src = rcx
; map = rdx
; dst = r8
; histo = r9

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_AVX_RGB32 proc public frame

repart equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]
src_pitch equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_pitch equ qword ptr[rbp+88]
dst_modulo equ qword ptr[rbp+96]
w_map equ dword ptr[rbp+104]
error_motion_map equ qword ptr[rbp+112]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,32
	.allocstack 32
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	.endprolog

	cld
	xor rax,rax
	mov rbx,rdx				; rbx = map
	mov rsi,rcx				; rsi = src
	mov rdx,r9				; rdx = histo
	xor rcx,rcx
		
	mov rdi,r9
	mov ecx,128
	rep stosq
	
	mov rdi,r8				;rdi = dst
	xor r8,r8
	xor r9,r9
	xor r15,r15
	mov r8d,w
	mov r9d,h
	mov r10,4
	mov r11,dst_pitch
	
	xor r12,r12
	xor r13,r13
	mov r12d,w_map
	mov r13d,r8d
	shl r12,1
	shl r13,2
	sub r12,r13	
	
	mov ecx,w_map
	mov r13,16
	mov r14,src_pitch

	vpxor xmm1,xmm1,xmm1	; return value (s)
	vpxor xmm2,xmm2,xmm2	; error_motion_map
	vpxor xmm3,xmm3,xmm3

Loop_A_1b_AVX:
	mov r15d,r8d
Loop_A_2b_AVX:
	vmovd xmm5,dword ptr[rbx]
	vpunpcklbw xmm5,xmm5,xmm3
	vpunpcklwd xmm5,xmm5,xmm3

	vmovdqa xmm0,XMMWORD ptr[rsi]
	vpcmpgtd xmm5,xmm5,xmm3
	vpsadbw xmm4,xmm0,xmm3
	vpand xmm5,xmm5,xmm0
	vpunpckhbw xmm7,xmm0,xmm3
	vmovdqa xmm6,xmm5
	vpaddd xmm1,xmm1,xmm4
	vpsadbw xmm5,xmm5,xmm3
	vpunpcklbw xmm0,xmm0,xmm3
	vpaddd xmm2,xmm2,xmm5
	vpextrw eax,xmm0,0
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm0,1
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm0,2
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm0,4
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm0,5
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm0,6
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,0
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,1
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm7,2
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,4
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,5
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm7,6
	inc dword ptr[rdx+4*rax]
	
	vmovdqa XMMWORD ptr[rdi],xmm6
	
	vmovd xmm5,dword ptr[rbx+rcx]
	vpunpcklbw xmm5,xmm5,xmm3
	vpunpcklwd xmm5,xmm5,xmm3	

	vmovdqa xmm0,XMMWORD ptr[rsi+r14]
	vpcmpgtd xmm5,xmm5,xmm3
	vpand xmm5,xmm5,xmm0
	vpunpckhbw xmm7,xmm0,xmm3
	vmovdqa xmm6,xmm5
	vpsadbw xmm5,xmm5,xmm3
	vpunpcklbw xmm0,xmm0,xmm3
	vpaddd xmm2,xmm2,xmm5
	vpextrw eax,xmm0,0
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm0,1
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm0,2
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm0,4
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm0,5
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm0,6
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,0
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,1
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm7,2
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,4
	inc dword ptr[rdx+4*rax]
	vpextrw eax,xmm7,5
	inc dword ptr [rdx+4*rax]
	vpextrw eax,xmm7,6
	inc dword ptr[rdx+4*rax]
	
	vmovdqa XMMWORD ptr[rdi+r11],xmm6

	add rdi,r13
	add rsi,r13
	add rbx,r10
	dec r15
	jnz Loop_A_2b_AVX
	add rdi,dst_modulo
	add rbx,r12
	add rsi,r14
	add rdi,r11
	add rsi,src_modulo
	dec r9d
	jnz Loop_A_1b_AVX

	mov rsi,rdx
	mov rdi,repart
	add rsi,1020
	add rdi,1020
	xor rbx,rbx
	std
	mov ecx,256
Loop_A_3b_AVX:
	lodsd
	add ebx,eax
	mov eax,ebx
	stosd
	loop Loop_A_3b_AVX
	cld

	vmovhlps xmm0,xmm0,xmm1
	vmovhlps xmm4,xmm4,xmm2
	vpaddd xmm1,xmm1,xmm0
	vpaddd xmm2,xmm2,xmm4
	
	mov rdi,error_motion_map
	vmovd dword ptr[rdi],xmm2
	vmovd eax,xmm1

	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]	
	add rsp,32	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_AVX_RGB32 endp


;JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
;	src_pitch:dword,src_modulo:dword,error_motion_map:dword
; src = rcx
; map = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 proc public frame

w_map equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
error_motion_map equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,4
	
	xor rdx,rdx
	xor rbx,rbx
	mov edx,w_map
	mov ebx,r8d
	shl rdx,1
	sub rdx,rbx
	
	mov ebx,w_map
	mov r11,src_pitch
	mov r12,src_modulo
	pxor mm1,mm1
	pxor mm2,mm2
	pxor mm3,mm3
	
	xor rax,rax
	xor rcx,rcx
Loop_A1_1:
	mov ecx,r8d
Loop_A1_2:
	movd mm0,dword ptr[rsi]
	psadbw mm0,mm3
	paddd mm1,mm0
	mov al,byte ptr[rdi]
	or al,al
	jz short Next_A1_1
	paddd mm2,mm0
Next_A1_1:
	mov al,byte ptr[rdi+rbx]
	or al,al
	jz short Next_A1_2
	movd mm0,dword ptr[rsi+r11]
	psadbw mm0,mm3
	paddd mm2,mm0
Next_A1_2:
	add rsi,r10
	inc rdi
	loop Loop_A1_2
	add rsi,r12
	add rdi,rdx
	add rsi,r11
	dec r9d
	jnz short Loop_A1_1
	
	mov rdi,error_motion_map
	movd dword ptr[rdi],mm2
	movd eax,mm1

	emms
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 endp


;JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
;	src_pitch:dword,src_modulo:dword,error_motion_map:dword
; src = rcx
; map = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32 proc public frame

w_map equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
error_motion_map equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,16
	mov r12,4
	
	xor rbx,rbx
	xor rax,rax
	xor rdx,rdx
	mov eax,w_map
	mov edx,r8d
	shl rax,1
	shl rdx,2
	sub rax,rdx	
	
	mov ebx,w_map
	mov rdx,src_pitch
	mov r11,src_modulo
	pxor xmm1,xmm1
	pxor xmm2,xmm2
	pxor xmm3,xmm3
	
	xor rcx,rcx
Loop_A1_1:
	mov ecx,r8d
Loop_A1_2:
	movd xmm4,dword ptr[rdi]
	punpcklbw xmm4,xmm3
	punpcklwd xmm4,xmm3
	
	movdqa xmm0,XMMWORD ptr[rsi]
	pcmpgtd xmm4,xmm3
	pand xmm4,xmm0
	psadbw xmm0,xmm3
	psadbw xmm4,xmm3
	paddd xmm1,xmm0
	paddd xmm2,xmm4
	
	movd xmm4,dword ptr[rdi+rbx]
	punpcklbw xmm4,xmm3
	punpcklwd xmm4,xmm3
	
	movdqa xmm0,XMMWORD ptr[rsi+rdx]
	pcmpgtd xmm4,xmm3
	pand xmm4,xmm0
	psadbw xmm4,xmm3
	paddd xmm2,xmm4	
	
	add rsi,r10
	add rdi,r12
	loop Loop_A1_2
	add rsi,r11
	add rdi,rax
	add rsi,rdx
	dec r9d
	jnz short Loop_A1_1

	mov rdi,error_motion_map
	
	movhlps xmm0,xmm1
	movhlps xmm4,xmm2
	paddd xmm1,xmm0
	paddd xmm2,xmm4	
	
	movd dword ptr[rdi],xmm2
	movd eax,xmm1
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32 endp


;JPSDR_IVTC_DeltaPicture_Motion_Map_AVX_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
;	src_pitch:dword,src_modulo:dword,error_motion_map:dword
; src = rcx
; map = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_DeltaPicture_Motion_Map_AVX_RGB32 proc public frame

w_map equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
error_motion_map equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,16
	mov r12,4
	
	xor rbx,rbx
	xor rax,rax
	xor rdx,rdx
	mov eax,w_map
	mov edx,r8d
	shl rax,1
	shl rdx,2
	sub rax,rdx	
	
	mov ebx,w_map
	mov rdx,src_pitch
	mov r11,src_modulo
	vpxor xmm1,xmm1,xmm1
	vpxor xmm2,xmm2,xmm2
	vpxor xmm3,xmm3,xmm3
	
	xor rcx,rcx
Loop_A1_1_AVX:
	mov ecx,r8d
Loop_A1_2_AVX:
	vmovd xmm4,dword ptr[rdi]
	vpunpcklbw xmm4,xmm4,xmm3
	vpunpcklwd xmm4,xmm4,xmm3
	
	vmovdqa xmm0,XMMWORD ptr[rsi]
	vpcmpgtd xmm4,xmm4,xmm3
	vpand xmm4,xmm4,xmm0
	vpsadbw xmm0,xmm0,xmm3
	vpsadbw xmm4,xmm4,xmm3
	vpaddd xmm1,xmm1,xmm0
	vpaddd xmm2,xmm2,xmm4
	
	vmovd xmm4,dword ptr[rdi+rbx]
	vpunpcklbw xmm4,xmm4,xmm3
	vpunpcklwd xmm4,xmm4,xmm3
	
	vmovdqa xmm0,XMMWORD ptr[rsi+rdx]
	vpcmpgtd xmm4,xmm4,xmm3
	vpand xmm4,xmm4,xmm0
	vpsadbw xmm4,xmm4,xmm3
	vpaddd xmm2,xmm2,xmm4
	
	add rsi,r10
	add rdi,r12
	loop Loop_A1_2_AVX
	add rsi,r11
	add rdi,rax
	add rsi,rdx
	dec r9d
	jnz short Loop_A1_1_AVX

	mov rdi,error_motion_map
	
	vmovhlps xmm4,xmm4,xmm2
	vmovhlps xmm0,xmm0,xmm1
	vpaddd xmm2,xmm2,xmm4
	vpaddd xmm1,xmm1,xmm0	
	
	vmovd dword ptr[rdi],xmm2
	vmovd eax,xmm1
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_DeltaPicture_Motion_Map_AVX_RGB32 endp


;JPSDR_IVTC_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax
	
	xor r12,r12
	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r13,pitch_buffer
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,r12d
	shr ecx,2
	rep stosd
	
	mov rbx,rdi
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r14,r14

Loop_A0_1:
	mov r14d,r8d
Loop_A0_2:
	mov rdx,r10
	pxor mm5,mm5
	shl rdx,1
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A0_1:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2:
	pand mm3,mm7
	movd dword ptr[rcx+r13],mm3
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,4
	add rdi,4
	add rcx,4
	inc rbx

	dec r14d
	jnz Loop_A0_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r13
	mov rdx,r11
	add rcx,modulo_buffer
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A0_1

	mov r15,rcx
	mov rdx,rcx
	xor rax,rax
	add rdx,r13
	mov ecx,r8d
Loop_A0_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	movq mm2,mm0
	pminub mm0,mm1
	pmaxub mm2,mm1
	psubb mm2,mm0
	pand mm2,mm7
	movd dword ptr[r15+4*rax],mm2
	movd dword ptr[rdx+4*rax],mm2
	inc rax
	loop Loop_A0_3

	xor rcx,rcx
	mov ecx,r12d
	xor rax,rax
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32 endp


;JPSDR_IVTC_Motion_Map_SSE2_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE2_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	movdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	
	
	mov rsi,rcx
	mov r13,pitch_buffer
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r14,r14
	mov r10,8
	mov r11,2	
	
	mov rbx,rdi
	mov rdi,rdx
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r15,rdx
	mov rax,r8
	shl r15,1
	shl rax,1
	sub r15,rax	
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5	
	
	movdqa xmm8,XMMWORD ptr ub_FC
	movdqa xmm9,XMMWORD ptr ub_FF
	movdqa xmm10,XMMWORD ptr filtre_1	

	mov r12,pitch
	shl r12,1
	
Loop_A0_1:
	mov r14d,r8d
Loop_A0_2:
	movq xmm0,qword ptr[rsi]				;xmm0=src1
	movq xmm1,qword ptr[rdi]				;xmm1=src2
	movq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,xmm9              ;xmm7 = FF si produit est positif		
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,xmm10
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[rbx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	movq qword ptr[rcx],xmm3
	
	movq xmm0,qword ptr[rdi+r12]			;Ici : xmm0=src4, xmm1=src2, xmm2=src3	
	movdqa xmm7,xmm2
	movdqa xmm4,xmm1
	movdqa xmm3,xmm0
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	punpcklbw xmm3,xmm5
	pcmpgtw xmm4,xmm7
	pcmpgtw xmm3,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,xmm9              ;xmm4 = FF si produit est positif		
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,xmm10
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[rbx+rdx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	movq qword ptr[rcx+r13],xmm3	

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2

	add rcx,r13
	add rbx,r15
	add rcx,modulo_buffer
	add rsi,modulo
	add rdi,modulo
	add rsi,pitch
	add rdi,pitch

	dec r9d
	jnz Loop_A0_1

	mov r15,rcx
	mov rdx,rcx
	xor rax,rax
	add rdx,r13
	mov ecx,r8d
Loop_A0_3:
	movq xmm0,qword ptr[rsi+8*rax]
	movq xmm1,qword ptr[rdi+8*rax]
	movdqa xmm2,xmm0
	pminub xmm0,xmm1
	pmaxub xmm2,xmm1
	psubb xmm2,xmm0
	pand xmm2,xmm8
	movq qword ptr[r15+8*rax],xmm2
	movq qword ptr[rdx+8*rax],xmm2
	inc rax
	loop Loop_A0_3

	xor rcx,rcx
	mov ecx,w_map
	xor rax,rax
	shr ecx,2
	mov rdi,rbx
	rep stosd

	movdqu xmm10,XMMWORD ptr[rsp+64]
	movdqu xmm9,XMMWORD ptr[rsp+48]
	movdqu xmm8,XMMWORD ptr[rsp+32]
	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32 endp


;JPSDR_IVTC_Motion_Map_AVX_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_AVX_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	vmovdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	vmovdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	vmovdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	
	
	mov rsi,rcx
	mov r13,pitch_buffer
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r14,r14
	mov r10,8
	mov r11,2	
	
	mov rbx,rdi
	mov rdi,rdx
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r15,rdx
	mov rax,r8
	shl r15,1
	shl rax,1
	sub r15,rax	
	
	mov eax,thr
	vpxor xmm5,xmm5,xmm5
	vmovd xmm6,eax
	vpinsrw xmm6,xmm6,eax,2
	vpinsrw xmm6,xmm6,eax,3
	vpunpcklbw xmm6,xmm6,xmm5
	
	vmovdqa xmm8,XMMWORD ptr ub_FC
	vmovdqa xmm9,XMMWORD ptr ub_FF
	vmovdqa xmm10,XMMWORD ptr filtre_1	

	mov r12,pitch
	shl r12,1
	
Loop_A0_1_AVX:
	mov r14d,r8d
Loop_A0_2_AVX:
	vmovq xmm0,qword ptr[rsi]				;xmm0=src1
	vmovq xmm1,qword ptr[rdi]				;xmm1=src2
	vmovq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	vpunpcklbw xmm7,xmm0,xmm5
	vpunpcklbw xmm3,xmm1,xmm5
	vpunpcklbw xmm4,xmm2,xmm5
	vpcmpgtw xmm7,xmm7,xmm3
	vpcmpgtw xmm4,xmm4,xmm3
	vpxor xmm7,xmm7,xmm4				;xmm7 = FF si produit est négatif
	vpxor xmm7,xmm7,xmm9              ;xmm7 = FF si produit est positif		
	vpmaxub xmm3,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm3,xmm3,xmm0				; xmm3=abs(src1-src2)
	
	vpminub xmm0,xmm2,xmm1
	vpmaxub xmm4,xmm2,xmm1
	vpsubb xmm4,xmm4,xmm0				; xmm4=abs(sr3-src2)
	vpminub xmm4,xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm4,xmm4,xmm5
	vpcmpgtw xmm4,xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm4,xmm4,xmm7				; xmm4=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	vpacksswb xmm4,xmm4,xmm5
	vpand xmm4,xmm4,xmm10
	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	vpsrlq xmm4,xmm4,32
	mov byte ptr[rbx],al

	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	vpand xmm3,xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	vmovq qword ptr[rcx],xmm3
	
	vmovq xmm0,qword ptr[rdi+r12]			;Ici : xmm0=src4, xmm1=src2, xmm2=src3	
	vpunpcklbw xmm7,xmm2,xmm5
	vpunpcklbw xmm4,xmm1,xmm5
	vpunpcklbw xmm3,xmm0,xmm5
	vpcmpgtw xmm4,xmm4,xmm7
	vpcmpgtw xmm3,xmm3,xmm7
	vpxor xmm4,xmm4,xmm3				;xmm4 = FF si produit est négatif
	vpxor xmm4,xmm4,xmm9              ;xmm4 = FF si produit est positif		
	vpmaxub xmm3,xmm1,xmm2
	vpminub xmm1,xmm1,xmm2
	vpsubb xmm3,xmm3,xmm1				; xmm3=abs(src2-src3)
	
	vpminub xmm1,xmm0,xmm2
	vpmaxub xmm0,xmm0,xmm2
	vpsubb xmm0,xmm0,xmm1				; xmm0=abs(sr4-src3)
	vpminub xmm0,xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm0,xmm0,xmm5
	vpcmpgtw xmm0,xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm0,xmm0,xmm4
	vpacksswb xmm0,xmm0,xmm5
	vpand xmm0,xmm0,xmm10
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	vpsrlq xmm0,xmm0,32
	mov byte ptr[rbx+rdx],al
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	vpand xmm3,xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	vmovq qword ptr[rcx+r13],xmm3	

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_AVX

	add rcx,r13
	add rbx,r15
	add rcx,modulo_buffer
	add rsi,modulo
	add rdi,modulo
	add rsi,pitch
	add rdi,pitch

	dec r9d
	jnz Loop_A0_1_AVX

	mov r15,rcx
	mov rdx,rcx
	xor rax,rax
	add rdx,r13
	mov ecx,r8d
Loop_A0_3_AVX:
	vmovq xmm0,qword ptr[rsi+8*rax]
	vmovq xmm1,qword ptr[rdi+8*rax]
	vpmaxub xmm2,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm2,xmm2,xmm0
	vpand xmm2,xmm2,xmm8
	vmovq qword ptr[r15+8*rax],xmm2
	vmovq qword ptr[rdx+8*rax],xmm2
	inc rax
	loop Loop_A0_3_AVX

	xor rcx,rcx
	mov ecx,w_map
	xor rax,rax
	shr ecx,2
	mov rdi,rbx
	rep stosd

	vmovdqu xmm10,XMMWORD ptr[rsp+64]
	vmovdqu xmm9,XMMWORD ptr[rsp+48]
	vmovdqu xmm8,XMMWORD ptr[rsp+32]
	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_AVX_RGB32 endp


;JPSDR_IVTC_Motion_Map_SSE_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE_RGB32_a proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax
	
	xor r12,r12
	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov r13,4
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,r12d
	shr ecx,2
	rep stosd
	mov rbx,rdi
	
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r14,r14

Loop_A0_1_1:
	mov r14d,r8d
Loop_A0_2_1:
	mov rdx,r10
	pxor mm5,mm5
	shl rdx,1
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A0_1_1:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2_1
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2_1:
	pand mm3,mm7
	movd dword ptr[rcx+r15],mm3
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A0_2_1

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rcx,modulo_buffer
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A0_1_1


	mov rdx,r10
	mov r14d,r8d
	shl rdx,1
Loop_A0_2_1_a:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_1_a			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A0_1_1_a:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al	
	
	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A0_2_1_a	

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_a endp


;JPSDR_IVTC_Motion_Map_SSE2_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE2_RGB32_a proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	movdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	

	
	mov rsi,rcx
	mov r15,pitch_buffer
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov rbx,rdi
	
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,8
	mov r11,2	
	xor r14,r14
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r13,rdx
	mov rax,r8
	shl r13,1
	shl rax,1
	sub r13,rax		
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5
	
	movdqa xmm8,XMMWORD ptr ub_FC
	movdqa xmm9,XMMWORD ptr ub_FF
	movdqa xmm10,XMMWORD ptr filtre_1			
	
	mov r12,pitch
	shl r12,1	

Loop_A0_1_1:
	mov r14d,r8d
Loop_A0_2_1:
	movq xmm0,qword ptr[rsi]				;xmm0=src1
	movq xmm1,qword ptr[rdi]				;xmm1=src2
	movq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,xmm9              ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,xmm10
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[rbx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	movq qword ptr[rcx],xmm3	
	
	movq xmm0,qword ptr[rdi+r12]			;Ici : xmm0=src4, xmm1=src2, xmm2=src3
	movdqa xmm7,xmm2
	movdqa xmm4,xmm1
	movdqa xmm3,xmm0
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	punpcklbw xmm3,xmm5
	pcmpgtw xmm4,xmm7
	pcmpgtw xmm3,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,xmm9              ;xmm4 = FF si produit est positif
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,xmm10
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[rbx+rdx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	movq qword ptr[rcx+r15],xmm3

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_1

	add rcx,r15
	add rbx,r13
	add rcx,modulo_buffer
	add rsi,pitch
	add rdi,pitch
	add rsi,modulo
	add rdi,modulo

	dec r9d
	jnz Loop_A0_1_1

	mov r14d,r8d
Loop_A0_2_1_a:
	movq xmm0,qword ptr[rsi]				;xmm0=src1
	movq xmm1,qword ptr[rdi]				;xmm1=src2
	movq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,xmm9              ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; mm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; mm4=abs(sr3-src2)
	pminub xmm4,xmm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0	
	
	packsswb xmm4,xmm5
	pand xmm4,xmm10
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[rbx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	movq qword ptr[rcx],xmm3	
	
	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_1_a	

	movdqu xmm10,XMMWORD ptr[rsp+64]
	movdqu xmm9,XMMWORD ptr[rsp+48]
	movdqu xmm8,XMMWORD ptr[rsp+32]
	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80		
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32_a endp


;JPSDR_IVTC_Motion_Map_AVX_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_AVX_RGB32_a proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	vmovdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	vmovdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	vmovdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	

	
	mov rsi,rcx
	mov r15,pitch_buffer
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov rbx,rdi
	
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,8
	mov r11,2	
	xor r14,r14
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r13,rdx
	mov rax,r8
	shl r13,1
	shl rax,1
	sub r13,rax		
	
	mov eax,thr
	vpxor xmm5,xmm5,xmm5
	vmovd xmm6,eax
	vpinsrw xmm6,xmm6,eax,2
	vpinsrw xmm6,xmm6,eax,3
	vpunpcklbw xmm6,xmm6,xmm5
	
	vmovdqa xmm8,XMMWORD ptr ub_FC
	vmovdqa xmm9,XMMWORD ptr ub_FF
	vmovdqa xmm10,XMMWORD ptr filtre_1			
	
	mov r12,pitch
	shl r12,1	

Loop_A0_1_1_AVX:
	mov r14d,r8d
Loop_A0_2_1_AVX:
	vmovq xmm0,qword ptr[rsi]				;xmm0=src1
	vmovq xmm1,qword ptr[rdi]				;xmm1=src2
	vmovq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	vpunpcklbw xmm7,xmm0,xmm5
	vpunpcklbw xmm3,xmm1,xmm5
	vpunpcklbw xmm4,xmm2,xmm5	
	vpcmpgtw xmm7,xmm7,xmm3
	vpcmpgtw xmm4,xmm4,xmm3
	vpxor xmm7,xmm7,xmm4				;xmm7 = FF si produit est négatif
	vpxor xmm7,xmm7,xmm9              ;xmm7 = FF si produit est positif
	vpmaxub xmm3,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm3,xmm3,xmm0				; xmm3=abs(src1-src2)

	vpminub xmm0,xmm2,xmm1
	vpmaxub xmm4,xmm2,xmm1
	vpsubb xmm4,xmm4,xmm0				; xmm4=abs(sr3-src2)
	vpminub xmm4,xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm4,xmm4,xmm5
	vpcmpgtw xmm4,xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm4,xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	vpacksswb xmm4,xmm4,xmm5
	vpand xmm4,xmm4,xmm10
	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	vpsrlq xmm4,xmm4,32
	mov byte ptr[rbx],al

	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	vpand xmm3,xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	vmovq qword ptr[rcx],xmm3	
	
	vmovq xmm0,qword ptr[rdi+r12]			;Ici : xmm0=src4, xmm1=src2, xmm2=src3
	vpunpcklbw xmm7,xmm2,xmm5
	vpunpcklbw xmm4,xmm1,xmm5
	vpunpcklbw xmm3,xmm0,xmm5
	vpcmpgtw xmm4,xmm4,xmm7
	vpcmpgtw xmm3,xmm3,xmm7
	vpxor xmm4,xmm4,xmm3				;xmm4 = FF si produit est négatif
	vpxor xmm4,xmm4,xmm9              ;xmm4 = FF si produit est positif
	vpmaxub xmm3,xmm1,xmm2
	vpminub xmm1,xmm1,xmm2
	vpsubb xmm3,xmm3,xmm1				; xmm3=abs(src2-src3)
	
	vpminub xmm1,xmm0,xmm2
	vpmaxub xmm0,xmm0,xmm2
	vpsubb xmm0,xmm0,xmm1				; xmm0=abs(sr4-src3)
	vpminub xmm0,xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm0,xmm0,xmm5
	vpcmpgtw xmm0,xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm0,xmm0,xmm4
	vpacksswb xmm0,xmm0,xmm5
	vpand xmm0,xmm0,xmm10
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	vpsrlq xmm0,xmm0,32
	mov byte ptr[rbx+rdx],al
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	vpand xmm3,xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	vmovq qword ptr[rcx+r15],xmm3

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_1_AVX

	add rcx,r15
	add rbx,r13
	add rcx,modulo_buffer
	add rsi,pitch
	add rdi,pitch
	add rsi,modulo
	add rdi,modulo

	dec r9d
	jnz Loop_A0_1_1_AVX

	mov r14d,r8d
Loop_A0_2_1_a_AVX:
	vmovq xmm0,qword ptr[rsi]				;xmm0=src1
	vmovq xmm1,qword ptr[rdi]				;xmm1=src2
	vmovq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	vpunpcklbw xmm7,xmm0,xmm5
	vpunpcklbw xmm3,xmm1,xmm5
	vpunpcklbw xmm4,xmm2,xmm5
	vpcmpgtw xmm7,xmm7,xmm3
	vpcmpgtw xmm4,xmm4,xmm3
	vpxor xmm7,xmm7,xmm4				;xmm7 = FF si produit est négatif
	vpxor xmm7,xmm7,xmm9              ;xmm7 = FF si produit est positif
	vpmaxub xmm3,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm3,xmm3,xmm0				; mm3=abs(src1-src2)
	vpminub xmm0,xmm2,xmm1
	vpmaxub xmm4,xmm2,xmm1
	vpsubb xmm4,xmm4,xmm0				; mm4=abs(sr3-src2)
	vpminub xmm4,xmm4,xmm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm4,xmm4,xmm5
	vpcmpgtw xmm4,xmm4,xmm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm4,xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0	
	
	vpacksswb xmm4,xmm4,xmm5
	vpand xmm4,xmm4,xmm10
	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	vpsrlq xmm4,xmm4,32
	mov byte ptr[rbx],al

	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	vpand xmm3,xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	vmovq qword ptr[rcx],xmm3	
	
	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_1_a_AVX

	vmovdqu xmm10,XMMWORD ptr[rsp+64]
	vmovdqu xmm9,XMMWORD ptr[rsp+48]
	vmovdqu xmm8,XMMWORD ptr[rsp+32]
	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80		
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_AVX_RGB32_a endp


;JPSDR_IVTC_Motion_Map_SSE_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE_RGB32_b proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax
	
	xor r12,r12
	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r13,pitch_buffer
	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r14,r14

Loop_A0_1_2:
	mov r14d,r8d
Loop_A0_2_2:
	mov rdx,r10
	pxor mm5,mm5
	shl rdx,1
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_2			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A0_1_2:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2_2
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2_2:
	pand mm3,mm7
	movd dword ptr[rcx+r13],mm3
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,4
	add rdi,4
	add rcx,4
	inc rbx

	dec r14d
	jnz Loop_A0_2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r13
	mov rdx,r11
	add rcx,modulo_buffer
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A0_1_2

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_b endp


;JPSDR_IVTC_Motion_Map_SSE2_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE2_RGB32_b proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	movdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	

	mov rsi,rcx
	mov r13,pitch_buffer
	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,8
	mov r11,2	
	xor r14,r14
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r15,rdx
	mov rax,r8
	shl r15,1
	shl rax,1
	sub r15,rax	
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5
	
	movdqa xmm8,XMMWORD ptr ub_FC
	movdqa xmm9,XMMWORD ptr ub_FF
	movdqa xmm10,XMMWORD ptr filtre_1		

	mov r12,pitch
	shl r12,1
	
Loop_A0_1_2:
	mov r14d,r8d
Loop_A0_2_2:
	movq xmm0,qword ptr[rsi]				;xmm0=src1
	movq xmm1,qword ptr[rdi]				;xmm1=src2
	movq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,xmm9              ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,xmm10
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[rbx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	movq qword ptr[rcx],xmm3	

	movq xmm0,qword ptr[rdi+r12]			;Ici : xmm0=src4, xmm1=src2, xmm2=src3
	movdqa xmm7,xmm2
	movdqa xmm4,xmm1
	movdqa xmm3,xmm0
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	punpcklbw xmm3,xmm5
	pcmpgtw xmm4,xmm7
	pcmpgtw xmm3,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,xmm9              ;xmm4 = FF si produit est positif
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,xmm10
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[rbx+rdx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	movq qword ptr[rcx+r13],xmm3	

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_2

	add rcx,r13
	add rbx,r15
	add rcx,modulo_buffer
	add rsi,pitch
	add rdi,pitch
	add rsi,modulo
	add rdi,modulo

	dec r9d
	jnz Loop_A0_1_2

	movdqu xmm10,XMMWORD ptr[rsp+64]
	movdqu xmm9,XMMWORD ptr[rsp+48]
	movdqu xmm8,XMMWORD ptr[rsp+32]
	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80			
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32_b endp


;JPSDR_IVTC_Motion_Map_AVX_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_AVX_RGB32_b proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	vmovdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	vmovdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	vmovdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	

	mov rsi,rcx
	mov r13,pitch_buffer
	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,8
	mov r11,2	
	xor r14,r14
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r15,rdx
	mov rax,r8
	shl r15,1
	shl rax,1
	sub r15,rax	
	
	mov eax,thr
	vpxor xmm5,xmm5,xmm5
	vmovd xmm6,eax
	vpinsrw xmm6,xmm6,eax,2
	vpinsrw xmm6,xmm6,eax,3
	vpunpcklbw xmm6,xmm6,xmm5
	
	vmovdqa xmm8,XMMWORD ptr ub_FC
	vmovdqa xmm9,XMMWORD ptr ub_FF
	vmovdqa xmm10,XMMWORD ptr filtre_1		

	mov r12,pitch
	shl r12,1
	
Loop_A0_1_2_AVX:
	mov r14d,r8d
Loop_A0_2_2_AVX:
	vmovq xmm0,qword ptr[rsi]				;xmm0=src1
	vmovq xmm1,qword ptr[rdi]				;xmm1=src2
	vmovq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	vpunpcklbw xmm7,xmm0,xmm5
	vpunpcklbw xmm3,xmm1,xmm5
	vpunpcklbw xmm4,xmm2,xmm5
	vpcmpgtw xmm7,xmm7,xmm3
	vpcmpgtw xmm4,xmm4,xmm3
	vpxor xmm7,xmm7,xmm4				;xmm7 = FF si produit est négatif
	vpxor xmm7,xmm7,xmm9              ;xmm7 = FF si produit est positif
	vpmaxub xmm3,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm3,xmm3,xmm0				; xmm3=abs(src1-src2)
	vpminub xmm0,xmm2,xmm1
	vpmaxub xmm4,xmm2,xmm1
	vpsubb xmm4,xmm4,xmm0				; xmm4=abs(sr3-src2)
	vpminub xmm4,xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm4,xmm4,xmm5
	vpcmpgtw xmm4,xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm4,xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	vpacksswb xmm4,xmm4,xmm5
	vpand xmm4,xmm4,xmm10
	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	vpsrlq xmm4,xmm4,32
	mov byte ptr[rbx],al

	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	vpand xmm3,xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	vmovq qword ptr[rcx],xmm3	

	vmovq xmm0,qword ptr[rdi+r12]			;Ici : xmm0=src4, xmm1=src2, xmm2=src3
	vpunpcklbw xmm7,xmm2,xmm5
	vpunpcklbw xmm4,xmm1,xmm5
	vpunpcklbw xmm3,xmm0,xmm5
	vpcmpgtw xmm4,xmm4,xmm7
	vpcmpgtw xmm3,xmm3,xmm7
	vpxor xmm4,xmm4,xmm3				;xmm4 = FF si produit est négatif
	vpxor xmm4,xmm4,xmm9              ;xmm4 = FF si produit est positif
	vpmaxub xmm3,xmm1,xmm2
	vpminub xmm1,xmm1,xmm2
	vpsubb xmm3,xmm3,xmm1				; xmm3=abs(src2-src3)
	vpminub xmm1,xmm0,xmm2
	vpmaxub xmm0,xmm0,xmm2
	vpsubb xmm0,xmm0,xmm1				; xmm0=abs(sr4-src3)
	vpminub xmm0,xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm0,xmm0,xmm5
	vpcmpgtw xmm0,xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm0,xmm0,xmm4
	vpacksswb xmm0,xmm0,xmm5
	vpand xmm0,xmm0,xmm10
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	vpsrlq xmm0,xmm0,32
	mov byte ptr[rbx+rdx],al
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	vpand xmm3,xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	vmovq qword ptr[rcx+r13],xmm3	

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_2_AVX

	add rcx,r13
	add rbx,r15
	add rcx,modulo_buffer
	add rsi,pitch
	add rdi,pitch
	add rsi,modulo
	add rdi,modulo

	dec r9d
	jnz Loop_A0_1_2_AVX

	vmovdqu xmm10,XMMWORD ptr[rsp+64]
	vmovdqu xmm9,XMMWORD ptr[rsp+48]
	vmovdqu xmm8,XMMWORD ptr[rsp+32]
	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80			
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_AVX_RGB32_b endp


;JPSDR_IVTC_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE_RGB32_c proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax
	
	xor r12,r12
	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r13,pitch_buffer
	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r14,r14

Loop_A0_1_3:
	mov r14d,r8d
Loop_A0_2_3:
	mov rdx,r10
	pxor mm5,mm5
	shl rdx,1
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_3			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A0_1_3:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2_3
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2_3:
	pand mm3,mm7
	movd dword ptr[rcx+r13],mm3
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,4
	add rdi,4
	add rcx,4
	inc rbx

	dec r14d
	jnz Loop_A0_2_3

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r13
	mov rdx,r11
	add rcx,modulo_buffer
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A0_1_3
	
	
	mov rdx,r10
	mov r14d,r8d
	shl rdx,1
Loop_A0_2_3_c:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_3_c			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A0_1_3_c:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al	


	movq mm3,mm1                ;Ici : mm1=src2, mm2=src3
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	pand mm3,mm7
	movd dword ptr[rcx+r13],mm3	
	
	add rsi,4
	add rdi,4
	add rcx,4
	inc rbx

	dec r14d
	jnz Loop_A0_2_3_c		
	
	add rbx,r12
	sub rbx,r8
	
	mov rdx,rcx
	xor rcx,rcx
	mov ecx,r12d
	xor rax,rax
	shr ecx,2
	mov rdi,rbx
	rep stosd	
	
	xor rcx,rcx	
	mov rdi,rdx
	mov rsi,rdx
	add rdi,r13
	mov ecx,r8d
	rep movsd
	
	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_c endp


;JPSDR_IVTC_Motion_Map_SSE2_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE2_RGB32_c proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	movdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	

	mov rsi,rcx
	mov r13,pitch_buffer
	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,8
	mov r11,2	
	xor r14,r14
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r15,rdx
	mov rax,r8
	shl r15,1
	shl rax,1
	sub r15,rax	
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5
	
	movdqa xmm8,XMMWORD ptr ub_FC
	movdqa xmm9,XMMWORD ptr ub_FF
	movdqa xmm10,XMMWORD ptr filtre_1		

	mov r12,pitch
	shl r12,1	

Loop_A0_1_3:
	mov r14d,r8d
Loop_A0_2_3:
	movq xmm0,qword ptr[rsi]				;xmm0=src1
	movq xmm1,qword ptr[rdi]				;xmm1=src2
	movq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,xmm9              ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,xmm10
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[rbx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	movq qword ptr[rcx],xmm3	
	
	movq xmm0,qword ptr[rdi+r12]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movdqa xmm7,xmm2
	movdqa xmm4,xmm1
	movdqa xmm3,xmm0
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	punpcklbw xmm3,xmm5
	pcmpgtw xmm4,xmm7
	pcmpgtw xmm3,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,xmm9              ;xmm4 = FF si produit est positif
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,xmm10
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[rbx+rdx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	movq qword ptr[rcx+r13],xmm3	

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_3

	add rcx,r13
	add rbx,r15
	add rcx,modulo_buffer
	add rsi,pitch
	add rdi,pitch
	add rsi,modulo
	add rdi,modulo

	dec r9d
	jnz Loop_A0_1_3
		
	mov r14d,r8d
Loop_A0_2_3_c:
	movq xmm0,qword ptr[rsi]				;xmm0=src1
	movq xmm1,qword ptr[rdi]				;xmm1=src2
	movq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,xmm9              ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7
	
	packsswb xmm4,xmm5
	pand xmm4,xmm10
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[rbx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	movq qword ptr[rcx],xmm3

	movdqa xmm3,xmm1                ;Ici : xmm1=src2, xmm2=src3
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	pand xmm3,xmm8
	movq qword ptr[rcx+r13],xmm3	
	
	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_3_c		
	
	add rcx,modulo_buffer
	
	mov rax,r8
	add rbx,rdx
	shl rax,1
	sub rbx,rax
	
	mov rdx,rcx
	xor rcx,rcx
	xor rax,rax
	mov ecx,w_map
	mov rdi,rbx
	shr ecx,2
	rep stosd	
	
	xor rcx,rcx	
	mov rdi,rdx
	mov rsi,rdx
	add rdi,r13
	mov ecx,r8d
	rep movsq
	
	movdqu xmm10,XMMWORD ptr[rsp+64]
	movdqu xmm9,XMMWORD ptr[rsp+48]
	movdqu xmm8,XMMWORD ptr[rsp+32]
	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80				
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32_c endp


;JPSDR_IVTC_Motion_Map_AVX_RGB32_c proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_AVX_RGB32_c proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	sub rsp,80
	.allocstack 80
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16	
	vmovdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	vmovdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	vmovdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog	

	mov rsi,rcx
	mov r13,pitch_buffer
	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,8
	mov r11,2	
	xor r14,r14
	
	xor rdx,rdx
	mov edx,w_map
	
	mov r15,rdx
	mov rax,r8
	shl r15,1
	shl rax,1
	sub r15,rax	
	
	mov eax,thr
	vpxor xmm5,xmm5,xmm5
	vmovd xmm6,eax
	vpinsrw xmm6,xmm6,eax,2
	vpinsrw xmm6,xmm6,eax,3
	vpunpcklbw xmm6,xmm6,xmm5
	
	vmovdqa xmm8,XMMWORD ptr ub_FC
	vmovdqa xmm9,XMMWORD ptr ub_FF
	vmovdqa xmm10,XMMWORD ptr filtre_1		

	mov r12,pitch
	shl r12,1	

Loop_A0_1_3_AVX:
	mov r14d,r8d
Loop_A0_2_3_AVX:
	vmovq xmm0,qword ptr[rsi]				;xmm0=src1
	vmovq xmm1,qword ptr[rdi]				;xmm1=src2
	vmovq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	vpunpcklbw xmm7,xmm0,xmm5
	vpunpcklbw xmm3,xmm1,xmm5
	vpunpcklbw xmm4,xmm2,xmm5
	vpcmpgtw xmm7,xmm7,xmm3
	vpcmpgtw xmm4,xmm4,xmm3
	vpxor xmm7,xmm7,xmm4				;xmm7 = FF si produit est négatif
	vpxor xmm7,xmm7,xmm9              ;xmm7 = FF si produit est positif
	vpmaxub xmm3,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm3,xmm3,xmm0				; xmm3=abs(src1-src2)
	vpminub xmm0,xmm2,xmm1
	vpmaxub xmm4,xmm2,xmm1
	vpsubb xmm4,xmm4,xmm0				; xmm4=abs(sr3-src2)
	vpminub xmm4,xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm4,xmm4,xmm5
	vpcmpgtw xmm4,xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm4,xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	vpacksswb xmm4,xmm4,xmm5
	vpand xmm4,xmm4,xmm10
	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	vpsrlq xmm4,xmm4,32
	mov byte ptr[rbx],al

	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	vpand xmm3,xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	vmovq qword ptr[rcx],xmm3	
	
	vmovq xmm0,qword ptr[rdi+r12]			;Ici : mm0=src4, mm1=src2, mm2=src3
	vpunpcklbw xmm7,xmm2,xmm5
	vpunpcklbw xmm4,xmm1,xmm5
	vpunpcklbw xmm3,xmm0,xmm5
	vpcmpgtw xmm4,xmm4,xmm7
	vpcmpgtw xmm3,xmm3,xmm7
	vpxor xmm4,xmm4,xmm3				;xmm4 = FF si produit est négatif
	vpxor xmm4,xmm4,xmm9              ;xmm4 = FF si produit est positif
	vpmaxub xmm3,xmm1,xmm2
	vpminub xmm1,xmm1,xmm2
	vpsubb xmm3,xmm3,xmm1				; xmm3=abs(src2-src3)
	vpminub xmm1,xmm0,xmm2
	vpmaxub xmm0,xmm0,xmm2
	vpsubb xmm0,xmm0,xmm1				; xmm0=abs(sr4-src3)
	vpminub xmm0,xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm0,xmm0,xmm5
	vpcmpgtw xmm0,xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm0,xmm0,xmm4
	vpacksswb xmm0,xmm0,xmm5
	vpand xmm0,xmm0,xmm10
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	vpsrlq xmm0,xmm0,32
	mov byte ptr[rbx+rdx],al
	
	vmovd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	vpand xmm3,xmm3,xmm8	
	mov byte ptr[rbx+rdx+1],al
	
	vmovq qword ptr[rcx+r13],xmm3	

	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_3_AVX

	add rcx,r13
	add rbx,r15
	add rcx,modulo_buffer
	add rsi,pitch
	add rdi,pitch
	add rsi,modulo
	add rdi,modulo

	dec r9d
	jnz Loop_A0_1_3_AVX
		
	mov r14d,r8d
Loop_A0_2_3_c_AVX:
	vmovq xmm0,qword ptr[rsi]				;xmm0=src1
	vmovq xmm1,qword ptr[rdi]				;xmm1=src2
	vmovq xmm2,qword ptr[rsi+r12]			;xmm2=src3
	vpunpcklbw xmm7,xmm0,xmm5
	vpunpcklbw xmm3,xmm1,xmm5
	vpunpcklbw xmm4,xmm2,xmm5
	vpcmpgtw xmm7,xmm7,xmm3
	vpcmpgtw xmm4,xmm4,xmm3
	vpxor xmm7,xmm7,xmm4				;xmm7 = FF si produit est négatif
	vpxor xmm7,xmm7,xmm9              ;xmm7 = FF si produit est positif
	vpmaxub xmm3,xmm0,xmm1
	vpminub xmm0,xmm0,xmm1
	vpsubb xmm3,xmm3,xmm0				; xmm3=abs(src1-src2)
	vpminub xmm0,xmm2,xmm1
	vpmaxub xmm4,xmm2,xmm1
	vpsubb xmm4,xmm4,xmm0				; xmm4=abs(sr3-src2)
	vpminub xmm4,xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	vpunpcklbw xmm4,xmm4,xmm5
	vpcmpgtw xmm4,xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	vpand xmm4,xmm4,xmm7
	
	vpacksswb xmm4,xmm4,xmm5
	vpand xmm4,xmm4,xmm10
	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	vpsrlq xmm4,xmm4,32
	mov byte ptr[rbx],al

	vmovd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	vpand xmm3,xmm3,xmm8				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[rbx+1],al
	
	vmovq qword ptr[rcx],xmm3

	vpmaxub xmm3,xmm1,xmm2			;Ici : xmm1=src2, xmm2=src3
	vpminub xmm1,xmm1,xmm2
	vpsubb xmm3,xmm3,xmm1				; xmm3=abs(src2-src3)
	vpand xmm3,xmm3,xmm8
	vmovq qword ptr[rcx+r13],xmm3	
	
	add rsi,r10
	add rdi,r10
	add rcx,r10
	add rbx,r11

	dec r14d
	jnz Loop_A0_2_3_c_AVX		
	
	add rcx,modulo_buffer
	
	mov rax,r8
	add rbx,rdx
	shl rax,1
	sub rbx,rax
	
	mov rdx,rcx
	xor rcx,rcx
	xor rax,rax
	mov ecx,w_map
	mov rdi,rbx
	shr ecx,2
	rep stosd	
	
	xor rcx,rcx	
	mov rdi,rdx
	mov rsi,rdx
	add rdi,r13
	mov ecx,r8d
	rep movsq
	
	vmovdqu xmm10,XMMWORD ptr[rsp+64]
	vmovdqu xmm9,XMMWORD ptr[rsp+48]
	vmovdqu xmm8,XMMWORD ptr[rsp+32]
	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80				
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_AVX_RGB32_c endp


;JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
;	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov rsi,rcx
	mov r10,pitch
	mov r15,pitch_buffer
	mov r11,modulo
	mov r12d,w_map
	mov r13,4
	xor rcx,rcx

	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,r12d
	shr ecx,2
	rep stosd
	
	mov rbx,rdi
	mov rdi,rdx
	mov rdx,r8
	xor r8,r8
	mov r8d,w
	mov r9d,h

	mov ecx,r8d
	xor rax,rax
Loop_A2_0:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_A2_0
	add rdx,r15
	
	mov rcx,rdx				;rcx=buffer + pitch_buffer
	
Loop_A2_1:
	mov r14d,r8d
Loop_A2_2:
	mov rdx,r10
	shl rdx,1	
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]		;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A2_1

	mov rsi,rdi
	mov rdi,rcx
	xor rcx,rcx
	mov ecx,r8d
	cld
	rep movsd

	xor rax,rax
	mov ecx,r12d
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 endp


;JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
;	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov r13,4
	xor rcx,rcx

	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,r12d
	shr ecx,2
	rep stosd
	
	mov rbx,rdi
	mov rdi,rdx
	mov rdx,r8
	xor r8,r8
	mov r8d,w
	mov r9d,h

	mov ecx,r8d
	xor rax,rax
Loop_A2_0_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_A2_0_1
	add rdx,r15
	
	mov rcx,rdx				;rcx=buffer + pitch_buffer
	
Loop_A2_1_1:
	mov r14d,r8d
Loop_A2_2_1:
	mov rdx,r10
	shl rdx,1	
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_1		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1_1:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2_1
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2_1:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2_1

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A2_1_1
	
	mov r14d,r8d
	mov rdx,r10
	shl rdx,1	
Loop_A2_2_1_a:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_1_a		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1_1_a:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2_1_a

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a endp



;JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
;	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov r13,4

	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	mov r8d,w
	mov r9d,h
	
Loop_A2_1_2:
	mov r14d,r8d
Loop_A2_2_2:
	mov rdx,r10
	shl rdx,1	
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_2		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1_2:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2_2:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A2_1_2

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b endp



;JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
;	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	
	xor r12,r12
	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov r13,4

	mov rbx,r9
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	mov r8d,w
	mov r9d,h
	
Loop_A2_1_3:
	mov r14d,r8d
Loop_A2_2_3:
	mov rdx,r10
	shl rdx,1	
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_3		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1_3:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2_3
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2_3:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2_3

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A2_1_3
	
	mov r14d,r8d
	mov rdx,r10
	shl rdx,1	
Loop_A2_2_3_c:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_3_c		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1_3_c:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2_3_c	
	
	add rcx,modulo_buffer
	add rbx,r12
	sub rbx,r8
	
	xor rax,rax
	mov rsi,rcx
	sub rsi,r15
	mov rdi,rcx
	xor rcx,rcx
	cld
	mov ecx,r8d
	rep movsd
	
	xor rax,rax
	mov ecx,r12d
	shr rcx,2
	mov rdi,rbx
	rep stosd
	
	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c endp


;JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,
;	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	
	mov r13,rcx
	mov r14,rdx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov rdx,r8
	mov rbx,r9
	xor r8,r8
	mov r8d,w
	mov r9d,h
	xor rcx,rcx

	cld
	xor rax,rax
	mov rdi,rbx
	mov ecx,r12d
	shr ecx,2
	rep stosd
	mov rbx,rdi

	mov rsi,r13
	mov rdi,r14
	mov ecx,r8d
	xor rax,rax
Loop_E2_0_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_E2_0_1
	add rdx,r15
	
	mov rcx,rdx				;rcx=buffer + pitch_buffer
	mov r13,4
	xor rax,rax

Loop_E2_1:
	mov r14d,r8d
Loop_E2_2:
	mov rdx,r10
	shl rdx,1
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_E2_1:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_E2_1

	mov rdx,rcx
	xor rcx,rcx
	xor rax,rax
	mov ecx,r8d
Loop_E2_0_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_E2_0_2
	
	xor rax,rax
	mov ecx,r12d
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 endp


;JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,
;	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	
	mov r13,rcx
	mov r14,rdx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov rdx,r8
	mov rbx,r9
	xor r8,r8
	mov r8d,w
	mov r9d,h
	xor rcx,rcx

	cld
	xor rax,rax
	mov rdi,rbx
	mov ecx,r12d
	shr ecx,2
	rep stosd
	mov rbx,rdi

	mov rsi,r13
	mov rdi,r14
	mov ecx,r8d
	xor rax,rax
Loop_E2_0_1_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_E2_0_1_1
	add rdx,r15
	
	mov rcx,rdx				;rcx=buffer + pitch_buffer
	mov r13,4
	xor rax,rax

Loop_E2_1_1:
	mov r14d,r8d
Loop_E2_2_1:
	mov rdx,r10
	shl rdx,1
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_1		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_E2_1_1:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2_1
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2_1:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2_1

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_E2_1_1
	
	mov r14d,r8d
	mov rdx,r10
	shl rdx,1
Loop_E2_2_1_a:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_1_a		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_E2_1_1_a:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2_1_a	

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a endp


;JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,
;	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov rcx,r8
	mov rbx,r9
	xor r8,r8
	mov r8d,w
	mov r9d,h

	mov r13,4
	xor rax,rax

Loop_E2_1_2:
	mov r14d,r8d
Loop_E2_2_2:
	mov rdx,r10
	shl rdx,1
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_2		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_E2_1_2:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2_2:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_E2_1_2

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b endp


;JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,
;	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
pitch_buffer equ qword ptr[rbp+80]
modulo_buffer equ qword ptr[rbp+88]
thr equ dword ptr[rbp+96]
w_map equ dword ptr[rbp+104]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	
	xor r12,r12
	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r15,pitch_buffer
	mov rcx,r8
	mov rbx,r9
	xor r8,r8
	mov r9d,h
	mov r8d,w
	
	mov r13,4
	xor rax,rax

Loop_E2_1_3:
	mov r14d,r8d
Loop_E2_2_3:
	mov rdx,r10
	shl rdx,1
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_3		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_E2_1_3:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2_3
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2_3:
	movd dword ptr[rcx+r15],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2_3

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	add rcx,r15
	mov rdx,r11
	add rdx,r10
	add rcx,modulo_buffer
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_E2_1_3

	
	mov r14d,r8d
	mov rdx,r10
	shl rdx,1
Loop_E2_2_3_c:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_3_c		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_E2_1_3_c:
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2_3_c
	
	add rcx,modulo_buffer
	
	mov rdx,rcx
	xor rax,rax
	mov eax,r8d
	shl rax,2
	sub rdi,rax
	add rsi,r11
	add rsi,r10
	xor rcx,rcx
	xor rax,rax
	mov ecx,r8d
Loop_E2_0_2_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_E2_0_2_3
	
	add rbx,r12
	sub rbx,r8
	
	cld
	xor rax,rax
	mov ecx,r12d
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c endp


;JPSDR_IVTC_Norme1_SSE_1_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword
; src1 = rcx
; src2 = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Norme1_SSE_1_RGB32 proc public frame

pitch equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	shl r10,1
	
	xor rcx,rcx
	xor rax,rax
	pxor mm3,mm3
	mov eax,00FC00FCh
	movd mm5,eax
	mov eax,0FCFCFCFCh
	movd mm4,eax
	punpcklbw mm4,mm5
	pxor mm5,mm5
	
Loop_B_1:
	mov ecx,r8d
	xor rdx,rdx
Loop_B_2:
	movd mm1,dword ptr[rsi+4*rdx]
	movd mm0,dword ptr[rdi+4*rdx]
	movq mm2,mm1
	pmaxub mm2,mm0
	pminub mm1,mm0
	psubb mm2,mm1
	pand mm2,mm4
	psadbw mm2,mm3
	inc rdx
	paddd mm5,mm2
	loop loop_B_2
	add rsi,r10
	add rdi,r10
	dec r9d
	jnz short Loop_B_1
	movd eax,mm5

	emms
	
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Norme1_SSE_1_RGB32 endp


;JPSDR_IVTC_Norme1_SSE2_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword
; src1 = rcx
; src2 = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Norme1_SSE2_RGB32 proc public frame

pitch equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	shl r10,1
	mov rdx,16
	
	xor rcx,rcx	
	pxor xmm2,xmm2
	pxor xmm5,xmm5
	movdqa xmm3,XMMWORD ptr ub_FC
	
Loop_B_1:
	xor rax,rax
	mov ecx,r8d
Loop_B_2:
	movdqa xmm1,XMMWORD ptr[rsi+rax]
	movdqa xmm0,XMMWORD ptr[rdi+rax]
	movdqa xmm4,xmm1
	pminub xmm1,xmm0
	pmaxub xmm4,xmm0
	psubb xmm4,xmm1	
	pand xmm4,xmm3
	psadbw xmm4,xmm5
	add rax,rdx
	paddd xmm2,xmm4
	loop loop_B_2
	add rsi,r10
	add rdi,r10
	dec r9d
	jnz short Loop_B_1
	
	movhlps xmm0,xmm2
	paddd xmm2,xmm0
	movd eax,xmm2	
	
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Norme1_SSE2_RGB32 endp


;JPSDR_IVTC_Norme1_AVX_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword
; src1 = rcx
; src2 = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Norme1_AVX_RGB32 proc public frame

pitch equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	shl r10,1
	mov rdx,16
	
	xor rcx,rcx	
	vpxor xmm2,xmm2,xmm2
	vpxor xmm5,xmm5,xmm5
	vmovdqa xmm3,XMMWORD ptr ub_FC
	
Loop_B_1_AVX:
	xor rax,rax
	mov ecx,r8d
Loop_B_2_AVX:
	vmovdqa xmm1,XMMWORD ptr[rsi+rax]
	vmovdqa xmm0,XMMWORD ptr[rdi+rax]
	vpmaxub xmm4,xmm1,xmm0
	vpminub xmm1,xmm1,xmm0
	vpsubb xmm4,xmm4,xmm1	
	vpand xmm4,xmm4,xmm3
	vpsadbw xmm4,xmm4,xmm5
	add rax,rdx
	vpaddd xmm2,xmm2,xmm4
	loop loop_B_2_AVX
	add rsi,r10
	add rdi,r10
	dec r9d
	jnz short Loop_B_1_AVX
	
	vmovhlps xmm0,xmm0,xmm2
	vpaddd xmm2,xmm2,xmm0
	vmovd eax,xmm2	
	
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Norme1_AVX_RGB32 endp


;JPSDR_IVTC_Norme1_SSE_2_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword
; src1 = rcx
; src2 = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Norme1_SSE_2_RGB32 proc public frame

pitch equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	shl r10,1

	xor rcx,rcx
	xor rax,rax
	pxor mm3,mm3
	mov eax,00FC00FCh
	movd mm5,eax
	mov eax,0FCFCFCFCh
	movd mm4,eax
	punpcklbw mm4,mm5
	pxor mm5,mm5
	
Loop_C_1:
	mov ecx,r8d
	xor rdx,rdx
Loop_C_2:
	movq mm1,qword ptr[rsi+8*rdx]
	movq mm0,qword ptr[rdi+8*rdx]
	movq mm2,mm1
	pmaxub mm2,mm0
	pminub mm1,mm0
	psubb mm2,mm1
	pand mm2,mm4
	psadbw mm2,mm3
	inc rdx
	paddd mm5,mm2
	loop loop_C_2
	add rsi,r10
	add rdi,r10
	dec r9d
	jnz short Loop_C_1
	movd eax,mm5

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Norme1_SSE_2_RGB32 endp


;JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,r8
	mov r8d,r9d
	mov r9d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rdx,rdx
	mov edx,000000FFh
	xor rcx,rcx
	xor rbx,rbx
	
loop_1:
	mov r13d,r8d
	xor rax,rax
loop_2_1:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc rax
	dec r13d
	jnz short loop_2_1
	add rsi,r11
	add r10,r12

	mov r13d,r8d
	xor rax,rax
loop_2_2:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_2
	add rdi,r11
	add r10,r12

	dec r9d
	jnz loop_1

	mov r13d,r8d
	xor rax,rax
loop_2_3:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_3
	
	mov rsi,r10
	cld
	add r10,r12
	mov ecx,r8d
	mov rdi,r10
	rep movsd

	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 endp


;JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,r8
	mov r8d,r9d
	mov r9d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rdx,rdx
	mov edx,000000FFh
	xor rcx,rcx
	xor rbx,rbx
	
loop_1_1:
	mov r13d,r8d
	xor rax,rax
loop_2_1_1:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc rax
	dec r13d
	jnz short loop_2_1_1
	add rsi,r11
	add r10,r12

	mov r13d,r8d
	xor rax,rax
loop_2_2_1:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_2_1
	add rdi,r11
	add r10,r12

	dec r9d
	jnz loop_1_1

	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a endp


;JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,r8
	mov r8d,r9d
	mov r9d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rdx,rdx
	mov edx,000000FFh
	xor rcx,rcx
	xor rbx,rbx
	
loop_1_2:
	mov r13d,r8d
	xor rax,rax
loop_2_1_2:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc rax
	dec r13d
	jnz short loop_2_1_2
	add rsi,r11
	add r10,r12

	mov r13d,r8d
	xor rax,rax
loop_2_2_2:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_2_2
	add rdi,r11
	add r10,r12

	dec r9d
	jnz loop_1_2

	mov r13d,r8d
	xor rax,rax
loop_2_3_2:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_3_2
	
	mov rsi,r10
	add r10,r12
	cld
	mov rdi,r10
	mov ecx,r8d
	rep movsd

	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b endp


;JPSDR_IVTC_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_c:
	xor rax,rax
	mov ecx,r8d
loop_2_c_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_c_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_c

	xor rax,rax
	mov ecx,r8d
loop_2_c_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_3
	
	mov rsi,rdi
	cld
	add rdi,r11
	mov ecx,r8d
	rep movsd

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE endp


;JPSDR_IVTC_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_c_1:
	xor rax,rax
	mov ecx,r8d
loop_2_c_1_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_1_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_c_2_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_2_1
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_c_1

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_a endp


;JPSDR_IVTC_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_c_2:
	xor rax,rax
	mov ecx,r8d
loop_2_c_1_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_1_2
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_c_2_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_2_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_c_2

	xor rax,rax
	mov ecx,r8d
loop_2_c_3_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_3_2
	
	mov rsi,rdi
	cld
	add rdi,r11
	mov ecx,r8d
	rep movsd

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_b endp


;JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_d:
	xor rax,rax
	mov ecx,r8d
loop_2_d_1:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_d_2:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_d

	xor rax,rax
	mov ecx,r8d
loop_2_d_3:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_3
	
	mov rsi,rdi
	cld
	add rdi,r11
	mov ecx,r8d
	rep movsq

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2 endp


;JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_2_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_d_1:
	xor rax,rax
	mov ecx,r8d
loop_2_d_1_1:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_1_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_d_2_1:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_2_1
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_d_1

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2_a endp


;JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_2_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_d_2:
	xor rax,rax
	mov ecx,r8d
loop_2_d_1_2:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_1_2
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_d_2_2:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_2_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_d_2

	xor rax,rax
	mov ecx,r8d
loop_2_d_3_2:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_3_2
	
	mov rsi,rdi
	cld
	add rdi,r11
	mov ecx,r8d
	rep movsq

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2_b endp


;JPSDR_IVTC_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_3 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e

	xor rax,rax
	mov ecx,r8d
loop_2_e_3:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_3
	
	mov rsi,rdi
	add rdi,r11
	mov ecx,r8d
	cld
	shl rcx,1
	rep movsq
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3 endp


;JPSDR_IVTC_Deinterlace_Blend_AVX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_AVX proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1_AVX
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2_AVX
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e_AVX

	xor rax,rax
	mov ecx,r8d
loop_2_e_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_3_AVX
	
	mov rsi,rdi
	add rdi,r11
	mov ecx,r8d
	cld
	shl rcx,1
	rep movsq
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_AVX endp


;JPSDR_IVTC_Deinterlace_Blend_SSE_3_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_3_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e_1:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1_1:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2_1:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2_1
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e_1

	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3_a endp


;JPSDR_IVTC_Deinterlace_Blend_AVX_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_AVX_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e_1_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1_1_AVX
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2_1_AVX
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e_1_AVX

	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_AVX_a endp


;JPSDR_IVTC_Deinterlace_Blend_SSE_3_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_3_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e_2:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1_2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1_2
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2_2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e_2

	xor rax,rax
	mov ecx,r8d
loop_2_e_3_2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_3_2
	
	mov rsi,rdi
	add rdi,r11
	mov ecx,r8d
	cld
	shl rcx,1
	rep movsq
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3_b endp


;JPSDR_IVTC_Deinterlace_Blend_AVX_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_AVX_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e_2_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1_2_AVX
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2_2_AVX
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e_2_AVX

	xor rax,rax
	mov ecx,r8d
loop_2_e_3_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_3_2_AVX
	
	mov rsi,rdi
	add rdi,r11
	mov ecx,r8d
	cld
	shl rcx,1
	rep movsq
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_AVX_b endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rbx,rcx
	mov r12,16
	mov r10,src_pitch
	mov r11,dst_pitch	
	
	mov rsi,rcx	
	xor rcx,rcx
	mov rdi,r8
	xor rax,rax
	mov ecx,r9d
loop_1_f_c:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_c
	
	mov rdi,r8
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;rbx=ln-1   rsi=ln    rdx=ln+1
	
loop_1_f:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_d:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_d

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_AVX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rbx,rcx
	mov r12,16
	mov r10,src_pitch
	mov r11,dst_pitch	
	
	mov rsi,rcx	
	xor rcx,rcx
	mov rdi,r8
	xor rax,rax
	mov ecx,r9d
loop_1_f_c_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_c_AVX
	
	mov rdi,r8
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;rbx=ln-1   rsi=ln    rdx=ln+1
	
loop_1_f_AVX:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_AVX

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_AVX

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_AVX
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_d_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_d_AVX

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12	
	.endprolog

	mov rbx,rcx
	mov r12,16
	mov r10,src_pitch
	mov r11,dst_pitch	
	
	mov rsi,rcx	
	xor rcx,rcx
	mov rdi,r8
	xor rax,rax
	mov ecx,r9d
loop_1_f_d_1:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_d_1
	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx
	
loop_1_f_1:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_1:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_1

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_1:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_1

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_1

	xor rax,rax
	mov ecx,r8d	
loop_1_f_c_1:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_c_1	

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12	
	.endprolog

	mov rbx,rcx
	mov r12,16
	mov r10,src_pitch
	mov r11,dst_pitch	
	
	mov rsi,rcx	
	xor rcx,rcx
	mov rdi,r8
	xor rax,rax
	mov ecx,r9d
loop_1_f_d_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_d_1_AVX
	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx
	
loop_1_f_1_AVX:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_1_AVX

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_1_AVX

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_1_AVX

	xor rax,rax
	mov ecx,r8d	
loop_1_f_c_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_c_1_AVX

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_a endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12		
	.endprolog

	mov rdi,r8
	mov rbx,rcx	
	mov rsi,rdx
	mov r12,16
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10             ;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx
	
loop_1_f_2:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_2

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_2

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_2

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12		
	.endprolog

	mov rdi,r8
	mov rbx,rcx	
	mov rsi,rdx
	mov r12,16
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10             ;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx
	
loop_1_f_2_AVX:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_2_AVX

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_2_AVX

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_2_AVX

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_b endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12			
	.endprolog

	mov rdi,r8
	mov rbx,rcx
	mov r12,16
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10             ;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx
	
loop_1_f_3:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_3:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_3

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_3:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_3

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_3
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_c_3:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_c_3

	add rdi,r11
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_d_3:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_d_3
	
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_c proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_c proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12			
	.endprolog

	mov rdi,r8
	mov rbx,rcx
	mov r12,16
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10             ;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx
	
loop_1_f_3_AVX:
	xor rax,rax
	mov ecx,r8d
loop_1_f_a_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_a_3_AVX

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;rsi=ln-1	rdx=ln	rbx=ln+1
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_b_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_b_3_AVX

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f_3_AVX
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_c_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_c_3_AVX

	add rdi,r11
	
	xor rax,rax
	mov ecx,r8d
loop_1_f_d_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_1_f_d_3_AVX
	
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_AVX_c endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov r10,rcx

	mov rsi,rcx
	xor rcx,rcx
	mov rdi,r8
	mov ecx,r9d

	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3
	
	xor rax,rax
loop_1_g_c:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_c

	mov rdi,r8
	mov rbx,r10	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx

loop_1_g:
	xor rax,rax
	mov ecx,r8d
loop_1_g_a:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_a

	mov rbx,rsi
	add rbx,r10				;rsi=ln-1	rdx=ln	rbx=ln+1
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_1_g_b:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm2,dword ptr[rbx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rdx+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_b

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_g
	
	xor rax,rax
	mov ecx,r8d
loop_1_g_d:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rsi+4*rax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_d
	
	emms
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov r10,rcx

	mov rsi,rcx
	xor rcx,rcx
	mov rdi,r8
	mov ecx,r9d
	
	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3
	
	xor rax,rax
loop_1_g_d_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_d_1

	mov rdi,r8
	mov rbx,r10	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx

loop_1_g_1:
	xor rax,rax
	mov ecx,r8d
loop_1_g_a_1:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_a_1

	mov rbx,rsi
	add rbx,r10				;rsi=ln-1	rdx=ln	rbx=ln+1
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_1_g_b_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm2,dword ptr[rbx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rdx+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_b_1

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_g_1
	
	xor rax,rax
	mov ecx,r8d
loop_1_g_c_1:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_c_1
	
	emms
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3

	mov rdi,r8
	mov rbx,rcx
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10             ;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx

loop_1_g_2:
	xor rax,rax
	mov ecx,r8d
loop_1_g_a_2:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_a_2

	mov rbx,rsi
	add rbx,r10				;rsi=ln-1	rdx=ln	rbx=ln+1
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_1_g_b_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm2,dword ptr[rbx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rdx+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_b_2

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_g_2

	emms
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b endp


;JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3

	mov rdi,r8
	mov rbx,rcx	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10             ;rbx=ln-1   rsi=ln    rdx=ln+1
	xor rcx,rcx

loop_1_g_3:
	xor rax,rax
	mov ecx,r8d
loop_1_g_a_3:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_a_3

	mov rbx,rsi
	add rbx,r10				;rsi=ln-1	rdx=ln	rbx=ln+1
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_1_g_b_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm2,dword ptr[rbx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rdx+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_b_3

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_g_3
	
	xor rax,rax
	mov ecx,r8d
loop_1_g_c_3:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_c_3
	
	add rdi,r11
	
	xor rax,rax
	mov ecx,r8d
loop_1_g_d_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_d_3
	
	emms
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c endp


;JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; lookup = r8
; w = r9d

JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 proc public frame

h equ dword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov rdx,r8
	xor rcx,rcx
	xor rax,rax
	mov r8,8
	mov r10d,h
	mov r11,src_modulo
	mov r12,dst_modulo
	cld
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_I_2:
	mov ecx,r9d
Loop_I_2a:	
	lodsb				; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	lodsb				; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[rdi],xmm0
	add rdi,r8
	dec rcx
	jnz Loop_I_2a
	add rsi,r11
	add rdi,r12
	dec r10d
	jnz Loop_I_2
	
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 endp


;JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; lookup = r8
; w = r9d

JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX proc public frame

h equ dword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov rdx,r8
	xor rcx,rcx
	xor rax,rax
	mov r8,8
	mov r10d,h
	mov r11,src_modulo
	mov r12,dst_modulo
	cld
	
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	
Loop_I_2_AVX:
	mov ecx,r9d
Loop_I_2a_AVX:	
	lodsb				; al=Y1
	vpxor xmm0,xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=U
	vpinsrw xmm0,xmm0,ebx,0
	vpinsrw xmm0,xmm0,ebx,1
	vpinsrw xmm0,xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	vpinsrw xmm1,xmm1,ebx,1
	vpinsrw xmm1,xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	lodsb				; al=Y2
	vpinsrw xmm1,xmm1,ebx,0
	vpinsrw xmm1,xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=V
	vpinsrw xmm0,xmm0,ebx,4
	vpinsrw xmm0,xmm0,ebx,5
	vpinsrw xmm0,xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	vpinsrw xmm2,xmm2,ebx,2
	vpinsrw xmm2,xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	vpinsrw xmm2,xmm2,ebx,1
	vpinsrw xmm2,xmm2,ebx,5
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm0,xmm0,xmm2
	vpsraw xmm0,xmm0,6
	vpackuswb xmm0,xmm0,xmm3
	vmovq qword ptr[rdi],xmm0
	add rdi,r8
	dec rcx
	jnz Loop_I_2a_AVX
	add rsi,r11
	add rdi,r12
	dec r10d
	jnz Loop_I_2_AVX
	
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_AVX endp


;JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; lookup = r8
; w = r9d

JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 proc public frame

h equ dword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rdx,r8
	mov r8,4
	mov r10,8
	mov r11d,h
	mov r12,src_modulo
	mov r13,dst_modulo
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	xor rax,rax
	
Loop_J_2:
	mov ecx,r9d
Loop_J_2a:
	movzx eax,byte ptr[rsi+1]		; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi]		; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	movzx eax,byte ptr[rsi+3]		; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi+2]		; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[rdi],xmm0
	add rsi,r8
	add rdi,r10
	dec rcx
	jnz Loop_J_2a
	add rsi,r12
	add rdi,r13
	dec r11d
	jnz Loop_J_2

	pop r13
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
		
	ret
	
JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 endp


;JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; lookup = r8
; w = r9d

JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX proc public frame

h equ dword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rdx,r8
	mov r8,4
	mov r10,8
	mov r11d,h
	mov r12,src_modulo
	mov r13,dst_modulo
	
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	xor rax,rax
	
Loop_J_2_AVX:
	mov ecx,r9d
Loop_J_2a_AVX:
	movzx eax,byte ptr[rsi+1]		; al=Y1
	vpxor xmm0,xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi]		; al=U
	vpinsrw xmm0,xmm0,ebx,0
	vpinsrw xmm0,xmm0,ebx,1
	vpinsrw xmm0,xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	vpinsrw xmm1,xmm1,ebx,1
	vpinsrw xmm1,xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	movzx eax,byte ptr[rsi+3]		; al=Y2
	vpinsrw xmm1,xmm1,ebx,0
	vpinsrw xmm1,xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi+2]		; al=V
	vpinsrw xmm0,xmm0,ebx,4
	vpinsrw xmm0,xmm0,ebx,5
	vpinsrw xmm0,xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	vpinsrw xmm2,xmm2,ebx,2
	vpinsrw xmm2,xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	vpinsrw xmm2,xmm2,ebx,1
	vpinsrw xmm2,xmm2,ebx,5
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm0,xmm0,xmm2
	vpsraw xmm0,xmm0,6
	vpackuswb xmm0,xmm0,xmm3
	vmovq qword ptr[rdi],xmm0
	add rsi,r8
	add rdi,r10
	dec rcx
	jnz Loop_J_2a_AVX
	add rsi,r12
	add rdi,r13
	dec r11d
	jnz Loop_J_2_AVX

	pop r13
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
		
	ret
	
JPSDR_IVTC_Convert_UYVY_to_RGB32_AVX endp


;JPSDR_IVTC_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_IVTC_Convert420_to_YUY2_1 proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog


	mov rsi,rcx
	mov rbx,rdx
	xor rax,rax
	mov rdx,r8
	xor rcx,rcx
	mov rdi,r9
	mov ecx,w
	cld

SSE2_1_a:
	movzx eax,byte ptr[rsi+1]		;al=y2
	mov ah,byte ptr[rdx]		;ah=v
	inc rdx
	shl eax,16
	lodsw				;al=y1 ah=y2
	mov ah,byte ptr[rbx]		;ah=u
	inc rbx
	stosd
	loop SSE2_1_a
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Convert420_to_YUY2_1 endp


end





