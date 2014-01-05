.code



;JPSDR_BlurMotion_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_BlurMotion_Move32_Full proc public frame

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
	
loop_F:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_Fa
	movsd
loop_Fa:	
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_F
	
fin_F:
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_Move32_Full endp


;JPSDR_BlurMotion_MMX proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_MMX proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
buffer_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rbx,buffer_pitch
	mov rdx,dst_pitch
	xor rcx,rcx

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
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_b

	emms

	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_MMX endp



;JPSDR_BlurMotion_SSE proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
buffer_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rbx,buffer_pitch
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
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_c

	emms
	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE endp



;JPSDR_BlurMotion_SSE_2 proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE_2 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
buffer_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rbx,buffer_pitch
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
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_d

	emms

	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE_2 endp



;JPSDR_BlurMotion_SSE_3 proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE_3 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
buffer_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rbx,buffer_pitch
	mov rdx,dst_pitch
	mov r12,16
	xor rcx,rcx

loop_1_e:
	xor rax,rax
	mov ecx,r9d
loop_2_e:
	movdqa xmm0,[rsi+rax]
	pavgb xmm0,[rdi+rax]
	movdqa [r8+rax],xmm0
	add rax,r12
	loop loop_2_e
	add rsi,r11
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_e

	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE_3 endp


;JPSDR_BlurMotion_SSE_3_A_U proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE_3_A_U proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
buffer_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rbx,buffer_pitch
	mov rdx,dst_pitch
	mov r12,16
	xor rcx,rcx

loop_1_f:
	xor rax,rax
	mov ecx,r9d
loop_2_f:
	movdqa xmm0,[rsi+rax]
	pavgb xmm0,[rdi+rax]
	movdqu [r8+rax],xmm0
	add rax,r12
	loop loop_2_f
	add rsi,r11
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_f

	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE_3_A_U endp


;JPSDR_BlurMotion_SSE_3_U_A proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE_3_U_A proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
buffer_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rbx,buffer_pitch
	mov rdx,dst_pitch
	mov r12,16
	xor rcx,rcx

loop_1_g:
	xor rax,rax
	mov ecx,r9d
loop_2_g:
	movdqu xmm0,[rsi+rax]
	pavgb xmm0,[rdi+rax]
	movdqa [r8+rax],xmm0
	add rax,r12
	loop loop_2_g
	add rsi,r11
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_g

	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE_3_U_A endp



;JPSDR_BlurMotion_SSE_3_U proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE_3_U proc public frame

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
	mov rbx,buffer_pitch
	mov rdx,dst_pitch
	mov r12,16
	xor rcx,rcx

loop_1_h:
	xor rax,rax
	mov ecx,r9d
loop_2_h:
	movdqu xmm0,[rsi+rax]
	pavgb xmm0,[rdi+rax]
	movdqu [r8+rax],xmm0
	add rax,r12
	loop loop_2_h
	add rsi,r11
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_h

	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE_3_U endp




end





