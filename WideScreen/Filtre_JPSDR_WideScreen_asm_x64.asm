.code

;JPSDR_WideScreenRGB32 proc src:dword,dst:dword,w:dword,src_modulo:dword,dst_modulo:dword,top:dword,
;	bottom:dword,src_offset_center:dword,dst_offset_center:dword
; src = rcx
; dst = rdx
; w = r8d
; src_modulo = r9

JPSDR_WideScreenRGB32 proc public frame

dst_modulo equ qword ptr[rbp+48]
top equ dword ptr[rbp+56]
bottom equ dword ptr[rbp+64]
src_offset_center equ qword ptr[rbp+72]
dst_offset_center equ qword ptr[rbp+80]

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
	xor rcx,rcx
	mov r10,dst_modulo
	
	cld
	mov edx,bottom
	or edx,edx
	jz short Center_
Loop_1_0:
	mov ecx,r8d
Loop_1_1:
	lodsd
	test eax,00E00000h
	jz short Zero
	test eax,0000E000h
	jz short Zero
	test eax,000000E0h
	jnz short Next_1_0
Zero:
	xor eax,eax
Next_1_0:
	stosd
	loop Loop_1_1
	add rsi,r9
	add rdi,r10
	dec edx
	jnz short Loop_1_0

Center_:
	add rsi,src_offset_center
	add rdi,dst_offset_center

Top_:
	mov edx,top
	or edx,edx
	jz short End_
	xor rax,rax
Loop_1_3:
	mov ecx,r8d
	shr ecx,1
	jnc short Loop_1_3a
	stosd
Loop_1_3a:	
	rep stosq
	add rsi,r9
	add rdi,r10
	dec edx
	jnz short Loop_1_3

End_:
	pop rsi
	pop rdi
	pop rbp

	ret


JPSDR_WideScreenRGB32 endp

end

