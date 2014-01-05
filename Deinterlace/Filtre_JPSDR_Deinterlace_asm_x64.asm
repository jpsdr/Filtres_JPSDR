.code


;JPSDR_Deinterlace_Move32_Half proc src:dword,dst:dword,w:dword,h:dword,src_modulo_pitch:dword,dst_modulo_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Move32_Half proc public frame

src_modulo_pitch equ qword ptr[rbp+48]
dst_modulo_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov r10,src_modulo_pitch
	mov r11,dst_modulo_pitch
	
loop_1_f:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_1_fa
	movsd
loop_1_fa:	
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_f
	
fin_f:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Move32_Half endp


; src:dword,dst:dword,w:dword,h:dword,src_modulo_pitch:dword,dst_modulo_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Move8_Half proc public frame

src_modulo_pitch equ qword ptr[rbp+48]
dst_modulo_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov r10,src_modulo_pitch
	mov r11,dst_modulo_pitch
	mov edx,7
	
loop_1_f_2:
	mov ecx,r8d
	and ecx,edx
	jz short loop_2_f_2
	rep movsb
loop_2_f_2:
	mov ecx,r8d
	shr ecx,3
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_f_2
	
fin_f_2:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Move8_Half endp



;JPSDR_Deinterlace_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d


JPSDR_Deinterlace_Move32_Full proc public frame

src_modulo equ qword ptr[rbp+48]
dst_modulo equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov r10,src_modulo
	mov r11,dst_modulo
	
loop_1_f_3:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_1_f_3a
	movsd
loop_1_f_3a:		
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_f_3
	
fin_f_3:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Move32_Full endp



;JPSDR_Deinterlace_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Move8_Full proc public frame

src_modulo equ qword ptr[rbp+48]
dst_modulo equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov r10,src_modulo
	mov r11,dst_modulo
	mov edx,7

loop_1_f_4:
	mov ecx,r8d
	and ecx,edx
	jz short loop_2_f_4
	rep movsb
loop_2_f_4:
	mov ecx,r8d	
	shr ecx,3
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_f_4
	
fin_f_4:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Move8_Full endp



;JPSDR_Deinterlace_Blend_Non_MMX_24 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_24 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1:
	mov ecx,r9d
	xor rax,rax
loop_2:
	movzx edx,byte ptr[rdi+4*rax]
	movzx ebx,byte ptr[rsi+4*rax]
	add bx,dx
	shr bx,1
	mov r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+1]
	movzx ebx,byte ptr[rsi+4*rax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+2]
	movzx ebx,byte ptr[rsi+4*rax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or ebx,r13d
	mov dword ptr[r8+4*rax],ebx
	inc rax
	loop loop_2
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1
	
fin_:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Non_MMX_24 endp



;JPSDR_Deinterlace_Blend_Non_MMX_32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_32 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_i:
	mov ecx,r9d
	xor rax,rax
loop_2_i:
	movzx edx,byte ptr[rdi+4*rax]
    movzx ebx,byte ptr[rsi+4*rax]
	add bx,dx
	shr bx,1
	mov r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+1]
	movzx ebx,byte ptr[rsi+4*rax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+2]
	movzx ebx,byte ptr[rsi+4*rax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+3]
	movzx ebx,byte ptr[rsi+4*rax+3]
	add bx,dx
	shr bx,1
	shl ebx,24
	or ebx,r13d
	mov dword ptr[r8+4*rax],ebx
	inc rax
	loop loop_2_i
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1_i
	
fin_i:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Non_MMX_32 endp


;JPSDR_Deinterlace_Blend_Non_MMX_8 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_8 proc public frame

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

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_j:
	mov ecx,r9d
	xor rax,rax
loop_2_j:
    movzx ebx,byte ptr[rsi+rax]
	movzx edx,byte ptr[rdi+rax]
	add bx,dx
	shr bx,1
	mov byte ptr[r8+rax],bl
	inc rax
	loop loop_2_j
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1_j
	
fin_j:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Non_MMX_8 endp


;JPSDR_Deinterlace_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_MMX proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	xor rcx,rcx
	pxor mm2,mm2

loop_1_b:
	xor rax,rax
	mov ecx,r9d
loop_2_b:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	punpcklbw mm0,mm2
	punpcklbw mm1,mm2
	paddusw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm2
	movd dword ptr[r8+4*rax],mm0
	inc rax
	loop loop_2_b
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_b

	emms
	
fin_b:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_MMX endp


;JPSDR_Deinterlace_Blend_Tri_MMX proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_MMX proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
	xor rax,rax
	mov ecx,r8d
loop_0_h:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rsi+4*rax]
	punpcklbw mm0,mm2
	punpcklbw mm1,mm2
	paddusw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm2
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_0_h

	mov eax,02020202h
	movd mm4,eax
	add rdi,r11
	pxor mm3,mm3
	punpcklbw mm4,mm3

loop_1_h:
	xor rax,rax
	mov ecx,r8d
loop_2_h:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm2,dword ptr[rsi+4*rax]
	punpcklbw mm1,mm3
	punpcklbw mm2,mm3		;mm0=ln-1 mm1=ln+1 mm2=ln
	paddw mm1,mm0
	psllw mm2,1
	paddw mm1,mm2
	paddw mm1,mm4
	psrlw mm1,2
	packuswb mm1,mm3
	movd dword ptr[rdi+4*rax],mm1
	inc rax
	loop loop_2_h
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_h

	xor rax,rax
	mov ecx,r8d
loop_3_h:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rsi+4*rax]
	punpcklbw mm0,mm2
	punpcklbw mm1,mm2
	paddusw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm2
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_3_h
	
	emms
	
fin_h:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_MMX endp





;JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc src_a:dword,src_b:dword,dst_abs:dword,dst_avg:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; src_b = rdx
; dst_abs = r8
; dst_avg = r9

JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]

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
	mov r10d,w
	mov r11d,h
	mov r12,src_pitch
	mov r13,dst_pitch
	mov rbx,16
	xor rcx,rcx
	
loop_1_Yadif1:
	xor rax,rax
	mov ecx,r10d
loop_2_Yadif1:
	movdqa xmm0,[rsi+rax]		; xmm0=src_a
	movdqa xmm1,[rdi+rax]		; xmm1=src_b
	movdqa xmm2,xmm0
	movdqa xmm3,xmm0
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	pminub xmm3,xmm1			; xmm3=min(src_a,src_b)
	pavgb xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	psubb xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	movdqa [r9+rax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	movdqa [r8+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,rbx
	loop loop_2_Yadif1
	add rsi,r12
	add rdi,r12
	add r8,r13
	add r9,r13
	dec r11d
	jnz short loop_1_Yadif1

	pop r13
	pop r12	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_Deinterlace_YadifAbsDiffAvg_SSE endp


;JPSDR_Deinterlace_YadifAbsDiff_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAbsDiff_SSE proc public frame

dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

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
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
	
loop_1_Yadif2:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif2:
	movdqa xmm0,[rsi+rax]		; xmm0=src_a
	movdqa xmm1,[rbx+rax]		; xmm1=src_b
	movdqa xmm2,xmm0
	pminub xmm0,xmm1			; xmm0=min(src_a,src_b)	
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	psubb xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	movdqa [rdi+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,r8
	loop loop_2_Yadif2
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif2

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiff_SSE endp
	


;JPSDR_Deinterlace_YadifAvg_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAvg_SSE proc public frame
	
dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

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
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
		
loop_1_Yadif3:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif3:
	movdqa xmm0,[rsi+rax]		; xmm0=src_a
	pavgb xmm0,[rbx+rax]		; xmm0=(src_a+src_b+1)>>1
	movdqa [rdi+rax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
	add rax,r8
	loop loop_2_Yadif3
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif3

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAvg_SSE endp




;JPSDR_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	xor rcx,rcx

loop_1_c:
	xor rax,rax
	mov ecx,r9d
loop_2_c:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[r8+4*rax],mm0
	inc rax
	loop loop_2_c
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_c

	emms
	
fin_c:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE endp



;JPSDR_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_2 proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	xor rcx,rcx

loop_1_d:
	xor rax,rax
	mov ecx,r9d
loop_2_d:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdi+8*rax]
	movq qword ptr[r8+8*rax],mm0
	inc rax
	loop loop_2_d
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_d

	emms
	
fin_d:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_2 endp



;JPSDR_Deinterlace_Blend_SSE_2b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_2b proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	xor rcx,rcx

loop_1_d_2:
	xor rax,rax
	mov ecx,r9d
loop_2_d_2:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdi+8*rax]
	movq qword ptr[r8+8*rax],mm0
	inc rax
	loop loop_2_d_2
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_d_2
	
	emms

fin_d_2:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_2b endp




;JPSDR_Deinterlace_Blend_SSE_2c proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_2c proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	xor rcx,rcx

loop_1_d_3:
	xor rax,rax
	mov ecx,r9d
loop_2_d_3:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdi+8*rax]
	movq qword ptr[r8+8*rax],mm0
	inc rax
	loop loop_2_d_3
	
	movd mm0,dword ptr[rsi+8*rax]
	movd mm1,dword ptr[rdi+8*rax]
	pavgb mm0,mm1
	movd dword ptr[r8+8*rax],mm0
	
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_d_3
	
	emms

fin_d_3:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_2c endp



;JPSDR_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3 proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e:
	xor rax,rax
	mov ecx,r9d
loop_2_e:
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqa oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e
	
fin_e:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3 endp




;JPSDR_Deinterlace_Blend_SSE_3b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3b proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_2:
	xor rax,rax
	mov ecx,r9d
loop_2_e_2:
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqa oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_2
	
	movq xmm0,qword ptr[rsi+rax]
	movq xmm1,qword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+rax],xmm0	
	
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_2
	
fin_e_2:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3b endp



;JPSDR_Deinterlace_Blend_SSE_3_A_U proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_A_U proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_1:
	xor rax,rax
	mov ecx,r9d
loop_2_e_1:
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqu oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_1
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_1
	
fin_e_1:
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_Deinterlace_Blend_SSE_3_A_U endp




;JPSDR_Deinterlace_Blend_SSE_3_A_Ub proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_A_Ub proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_1_2:
	xor rax,rax
	mov ecx,r9d
loop_2_e_1_2:
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqu oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_1_2
	
	movq xmm0,qword ptr[rsi+rax]
	movq xmm1,qword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+rax],xmm0
	
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_1_2
	
fin_e_1_2:
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_Deinterlace_Blend_SSE_3_A_Ub endp



;JPSDR_Deinterlace_Blend_SSE_3_U_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_U_A proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_2:
	xor rax,rax
	mov ecx,r9d
loop_2_e_2:
	movdqu xmm0,oword ptr[rsi+rax]
	movdqu xmm1,oword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movdqa oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_2
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_2
	
fin_e_2:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3_U_A endp



;JPSDR_Deinterlace_Blend_SSE_3_U_Ab proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_U_Ab proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_2_2:
	xor rax,rax
	mov ecx,r9d
loop_2_e_2_2:
	movdqu xmm0,oword ptr[rsi+rax]
	movdqu xmm1,oword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movdqa oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_2_2
	
	movq xmm0,qword ptr[rsi+rax]
	movq xmm1,qword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+rax],xmm0
	
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_2_2
	
fin_e_2_2:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3_U_Ab endp


;JPSDR_Deinterlace_Blend_SSE_3_U proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_U proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_3:
	xor rax,rax
	mov ecx,r9d
loop_2_e_3:
	movdqu xmm0,oword ptr[rsi+rax]
	movdqu xmm1,oword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movdqu oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_3
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_3
	
fin_e_3:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3_U endp




;JPSDR_Deinterlace_Blend_SSE_3_Ub proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_Ub proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_3_2:
	xor rax,rax
	mov ecx,r9d
loop_2_e_3_2:
	movdqu xmm0,oword ptr[rsi+rax]
	movdqu xmm1,oword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movdqu oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_3_2
	
	movq xmm0,qword ptr[rsi+rax]
	movq xmm1,qword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+rax],xmm0
		
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_3_2
	
fin_e_3_2:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3_Ub endp




;JPSDR_Deinterlace_Blend_Tri_SSE_2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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
	
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_1:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_0_g_1
	
	add rdi,r11

loop_1_g_1:
	xor rax,rax
	mov ecx,r8d
loop_2_g_1:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_g_1
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_1

	mov ecx,r8d
	xor rax,rax
loop_3_g_1:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_3_g_1
	
fin_g_1:
	emms
	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_2 endp



;JPSDR_Deinterlace_Blend_Tri_SSE_2b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_2b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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
	
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_1_2:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_0_g_1_2
	
	add rdi,r11

loop_1_g_1_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_1_2:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_g_1_2
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_1_2

	mov ecx,r8d
	xor rax,rax
loop_3_g_1_2:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_3_g_1_2
	
	emms
	
fin_g_1_2:
	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_2b endp




;JPSDR_Deinterlace_Blend_Tri_SSE_2c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_2c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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
	
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_1_3:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_0_g_1_3

	movd mm0,dword ptr[rbx+8*rax]
	movd mm1,dword ptr[rsi+8*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+8*rax],mm0
	
	add rdi,r11

loop_1_g_1_3:
	xor rax,rax
	mov ecx,r8d
loop_2_g_1_3:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_g_1_3
	
	movd mm0,dword ptr[rbx+8*rax]
	movd mm1,dword ptr[rdx+8*rax]
	movd mm2,dword ptr[rsi+8*rax]
	pavgb mm0,mm1
	pavgb mm0,mm2
	movd dword ptr[rdi+8*rax],mm0
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_1_3

	mov ecx,r8d
	xor rax,rax
loop_3_g_1_3:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_3_g_1_3
	
	movd mm0,dword ptr[rbx+8*rax]
	movd mm1,dword ptr[rsi+8*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+8*rax],mm0	
	
	emms
	
fin_g_1_3:
	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_2c endp



;JPSDR_Deinterlace_Blend_Tri_SSE proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rsi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_0_g
	
	add rdi,r11

loop_1_g:
	xor rax,rax
	mov ecx,r8d
loop_2_g:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	movd mm2,dword ptr[rsi+4*rax]
	pavgb mm0,mm1
	pavgb mm0,mm2
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_g
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g

	mov ecx,r8d
	xor rax,rax
loop_3_g:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm1,dword ptr[rsi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_3_g
	
fin_g:
	emms
	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE endp



;JPSDR_Deinterlace_Blend_Tri_SSE_3 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
		
	mov ecx,r8d
	xor rax,rax
loop_0_g_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2
	
	add rdi,r11

loop_1_g_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rdx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2

	mov ecx,r8d
	xor rax,rax
loop_3_g_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2
	
fin_g_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3 endp



;JPSDR_Deinterlace_Blend_Tri_SSE_3b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
		
	mov ecx,r8d
	xor rax,rax
loop_0_g_2_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_2

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
	add rdi,r11

loop_1_g_2_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rdx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rdx+rax]
	movq xmm2,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[rdi+rax],xmm0	
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2

	mov ecx,r8d
	xor rax,rax
loop_3_g_2_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_2
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
		
fin_g_2_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3b endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_3:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_3
	
	add rdi,r11

loop_1_g_3:
	xor rax,rax
	mov ecx,r8d
loop_2_g_3:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rdx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_3
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_3

	mov ecx,r8d
	xor rax,rax
loop_3_g_3:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_3	

fin_g_3:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U endp



;JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_3_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_3_2

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
	add rdi,r11

loop_1_g_3_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_3_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rdx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_3_2
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rdx+rax]
	movq xmm2,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[rdi+rax],xmm0
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_3_2

	mov ecx,r8d
	xor rax,rax
loop_3_g_3_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_3_2	
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
fin_g_3_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub endp



;JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_4:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_4
	
	add rdi,r11

loop_1_g_4:
	xor rax,rax
	mov ecx,r8d
loop_2_g_4:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rdx+rax]
	movdqu xmm2,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_4
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_4

	mov ecx,r8d
	xor rax,rax
loop_3_g_4:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_4
	
fin_g_4:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A endp




;JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_4_2:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_4_2

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
	add rdi,r11

loop_1_g_4_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_4_2:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rdx+rax]
	movdqu xmm2,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_4_2
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rdx+rax]
	movq xmm2,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[rdi+rax],xmm0
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_4_2

	mov ecx,r8d
	xor rax,rax
loop_3_g_4_2:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_4_2

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
fin_g_4_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_U proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_U proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_5:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_5
	
	add rdi,r11

loop_1_g_5:
	xor rax,rax
	mov ecx,r8d
loop_2_g_5:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rdx+rax]
	movdqu xmm2,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_5
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_5

	mov ecx,r8d
	xor rax,rax
loop_3_g_5:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_5

fin_g_5:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_U endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	xor rcx,rcx
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_0_g_5_2:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_5_2
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
	add rdi,r11

loop_1_g_5_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_5_2:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rdx+rax]
	movdqu xmm2,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_5_2

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rdx+rax]
	movq xmm2,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[rdi+rax],xmm0
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_5_2

	mov ecx,r8d
	xor rax,rax
loop_3_g_5_2:
	movdqu xmm0,oword ptr[rbx+rax]
	movdqu xmm1,oword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movdqu oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_5_2
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
			
fin_g_5_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub endp



end





