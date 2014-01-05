.code


;JPSDR_Field_Shift_Rebuild_Frame32 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
;src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
;public JPSDR_Field_Shift_Rebuild_Frame32
;bottom_src = rcx
;top_src = rdx
;dst = r8
;w=r9d

JPSDR_Field_Shift_Rebuild_Frame32 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	cld
	mov rsi,rcx 			; bottom_src
	mov rdi,r8				; dst
	mov ebx,h
	mov r10,rdx				; top_src
	mov r11,src_modulo
	mov r12,dst_modulo
	add r11,src_pitch		; r11=src_modulo+src_pitch
	add r12,dst_pitch		; r12=dst_modulo+dst_pitch
	xor rcx,rcx
	mov edx,ebx
Loop_D_1:
	mov ecx,r9d
	shr ecx,1
	jnc short Loop_D_1a
	movsd
Loop_D_1a:
	rep movsq
	add rsi,r11
	add rdi,r12
	dec edx
	jnz short Loop_D_1
	mov rsi,r10
	mov rdi,r8
	mov edx,ebx
	add rdi,dst_pitch
Loop_D_2:
	mov ecx,r9d
	shr ecx,1
	jnc short Loop_D_2a
	movsd	
Loop_D_2a:	
	rep movsq
	add rsi,r11
	add rdi,r12
	dec edx
	jnz short Loop_D_2

	pop r12
	pop rbx
	pop rdi
	pop rsi
	pop rbp
	ret

JPSDR_Field_Shift_Rebuild_Frame32 endp


;JPSDR_Field_Shift_Rebuild_Frame8 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
;src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
;public JPSDR_Field_Shift_Rebuild_Frame8
;bottom_src = rcx
;top_src = rdx
;dst = r8
;w=r9d

JPSDR_Field_Shift_Rebuild_Frame8 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog
	
	cld
	mov rsi,rcx 			; bottom_src
	mov rdi,r8				; dst
	mov ebx,h
	mov r10,rdx				; top_src
	mov r11,src_modulo
	mov r12,dst_modulo
	add r11,src_pitch		; r11=src_modulo+src_pitch
	add r12,dst_pitch		; r12=dst_modulo+dst_pitch
	xor rcx,rcx
	mov r13d,7
	mov edx,ebx
Loop_E_1:
	mov ecx,r9d
	and ecx,r13d
	jz short Rebuild8_1
	rep movsb
Rebuild8_1:
	mov ecx,r9d
	shr ecx,3
	rep movsq
	add rsi,r11
	add rdi,r12
	dec edx
	jnz short Loop_E_1
	mov rsi,r10
	mov rdi,r8
	mov edx,ebx
	add rdi,dst_pitch
Loop_E_2:
	mov ecx,r9d
	and ecx,r13d
	jz short Rebuild8_2
	rep movsb
Rebuild8_2:
	mov ecx,r9d
	shr ecx,3
	rep movsq
	add rsi,r11
	add rdi,r12
	dec edx
	jnz short Loop_E_2

	pop r13
	pop r12
	pop rbx
	pop rdi
	pop rsi
	pop rbp
	ret

JPSDR_Field_Shift_Rebuild_Frame8 endp



;JPSDR_Field_Shift_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Field_Shift_Move32_Full proc public frame

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

JPSDR_Field_Shift_Move32_Full endp


;JPSDR_Field_Shift_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Field_Shift_Move8_Full proc public frame

src_modulo equ qword ptr[rbp+48]
dst_modulo equ qword ptr[rbp+56]

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
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov r10,src_modulo
	mov r11,dst_modulo
	mov rbx,7
		
loop_F8:
	mov ecx,r8d
	and ecx,ebx
	jz short loop_F8_suite
	rep movsb
loop_F8_suite:	
	mov ecx,r8d
	shr ecx,3
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_F8
	
fin_F8:
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_Field_Shift_Move8_Full endp



end





