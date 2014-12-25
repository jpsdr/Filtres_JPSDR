.code

;JPSDR_IVTC_Manual_Rebuild_Frame proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; bottom_src = rcx
; top_src = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Manual_Rebuild_Frame proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]

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
	
	cld
	mov rdi,r8
	mov rsi,rcx
	mov r10d,h
	mov r11,rdx
	mov r12,src_pitch
	mov r13,dst_pitch
	mov rax,r13
	add r12,src_modulo
	add r13,dst_modulo
	xor rcx,rcx
		
	mov edx,r10d
Loop_D_1:
	mov ecx,r9d
	shr ecx,1
	jnc short Loop_D_1a
	movsd
Loop_D_1a:	
	rep movsq
	add rsi,r12
	add rdi,r13
	dec edx
	jnz short Loop_D_1
	
	mov rsi,r11
	mov rdi,r8
	mov edx,r10d
	add rdi,rax
Loop_D_2:
	mov ecx,r9d
	shr ecx,1
	jnc short Loop_D_2a
	movsd
Loop_D_2a:	
	rep movsq
	add rsi,r12
	add rdi,r13
	dec edx
	jnz short Loop_D_2

	pop r13
	pop r12	
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Manual_Rebuild_Frame endp



;JPSDR_IVTC_Manual_Rebuild_Frame8 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; bottom_src = rcx
; top_src = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Manual_Rebuild_Frame8 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]

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
	
	cld
	mov rdi,r8
	mov rsi,rcx
	mov r10d,h
	mov r11,rdx
	mov r12,src_pitch
	mov r13,dst_pitch
	mov rax,r13
	add r12,src_modulo
	add r13,dst_modulo
	mov r14d,r9d
	and r14d,7
	shr r9d,3
	xor rcx,rcx
	
	mov edx,r10d
Loop_D8_1:
	or r9d,r9d
	jz short loop_D8_suite1a
	mov ecx,r9d
	rep movsq
loop_D8_suite1a:	
	or r14d,r14d
	jz short loop_D8_suite1b
	mov ecx,r14d
	rep movsb
loop_D8_suite1b:	
	add rsi,r12
	add rdi,r13
	dec edx
	jnz short Loop_D8_1
	
	mov rsi,r11
	mov rdi,r8
	mov edx,r10d
	add rdi,rax
Loop_D8_2:
	or r9d,r9d
	jz short loop_D8_suite2a
	mov ecx,r9d
	rep movsq
loop_D8_suite2a:	
	or r14d,r14d
	jz short loop_D8_suite2b
	mov ecx,r14d
	rep movsb
loop_D8_suite2b:	
	add rsi,r12
	add rdi,r13
	dec edx
	jnz short Loop_D8_2

	pop r14
	pop r13
	pop r12	
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Manual_Rebuild_Frame8 endp



;JPSDR_IVTC_Manual_Rebuild_Frame_2 proc src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Manual_Rebuild_Frame_2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
dst_modulo equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog	
	
	cld
	mov rdi,rdx
	mov rsi,rcx
	mov rax,src_pitch
	mov rdx,dst_pitch
	add rax,src_modulo
	add rdx,dst_modulo
	xor rcx,rcx
		
Loop_E_1:
	mov ecx,r8d
	shr ecx,1
	jnc short Loop_E_1a
	movsd
Loop_E_1a:	
	rep movsq
	add rsi,rax
	add rdi,rdx
	dec r9d
	jnz short Loop_E_1
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Manual_Rebuild_Frame_2 endp



;JPSDR_IVTC_Manual_Rebuild_Frame8_2 proc src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Manual_Rebuild_Frame8_2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
dst_modulo equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog	
	
	cld
	mov rdi,rdx
	mov rsi,rcx
	mov rax,src_pitch
	mov rdx,dst_pitch
	add rax,src_modulo
	add rdx,dst_modulo
	mov r10d,r8d
	and r10d,7
	shr r8d,3
	xor rcx,rcx
	
Loop_E8_1:
	or r8d,r8d
	jz short loop_E8_suite1
	mov ecx,r8d
	rep movsq
loop_E8_suite1:	
	or r10d,r10d
	jz short loop_E8_suite2
	mov ecx,r10d
	rep movsb
loop_E8_suite2:	
	add rsi,rax
	add rdi,rdx
	dec r9d
	jnz short Loop_E8_1
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Manual_Rebuild_Frame8_2 endp



end





