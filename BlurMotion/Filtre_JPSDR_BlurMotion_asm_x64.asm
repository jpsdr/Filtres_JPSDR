.code




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
	mov r12d,1
	xor rcx,rcx

loop_1_e:
	xor rax,rax
	mov ecx,r9d
	shr ecx,1
	jz short loop_3_e
	
loop_2_e:
	movq xmm0,qword ptr[rsi+8*rax]
	movq xmm1,qword ptr[rdi+8*rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+8*rax],xmm0
	inc rax
	loop loop_2_e
	
loop_3_e:
	mov ecx,r9d
	and ecx,r12d
	jz short loop_4_e

	movd xmm0,dword ptr[rsi+8*rax]
	movd xmm1,dword ptr[rdi+8*rax]
	pavgb xmm0,xmm1
	movd dword ptr[r8+8*rax],xmm0
	
loop_4_e:	
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



;JPSDR_BlurMotion_SSE_3_A proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
;	dst_pitch:dword
; src = rcx
; s = rdx
; dst = r8
; w = r9d

JPSDR_BlurMotion_SSE_3_A proc public frame

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
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rbx,buffer_pitch
	mov rdx,dst_pitch
	mov r12,16
	mov r13d,3
	mov r14d,2
	mov r15d,1
	xor rcx,rcx

loop_1_f:
	xor rax,rax
	mov ecx,r9d
	shr ecx,2
	jz short loop_3_f
	
loop_2_f:
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqu [r8+rax],xmm0
	add rax,r12
	loop loop_2_f
	
loop_3_f:
	mov ecx,r9d
	and ecx,r13d
	jz short loop_5_f
	and ecx,r14d
	jz short loop_4_f
	
	movq xmm0,qword ptr[rsi+rax]
	movq xmm1,qword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+rax],xmm0

	mov ecx,r9d
	and ecx,r15d
	jz short loop_5_f
	add rax,8	

loop_4_f:
	movd xmm0,dword ptr[rsi+rax]
	movd xmm1,dword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movd dword ptr[r8+rax],xmm0
	
loop_5_f:
	add rsi,r11
	add rdi,rbx
	add r8,rdx
	dec r10d
	jnz short loop_1_f

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_BlurMotion_SSE_3_A endp




end





