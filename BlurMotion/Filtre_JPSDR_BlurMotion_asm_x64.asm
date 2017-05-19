.code


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
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqa [r8+rax],xmm0
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

JPSDR_BlurMotion_SSE_3_A endp




end





