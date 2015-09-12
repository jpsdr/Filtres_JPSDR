.code


;JPSDR_AddBorder_Fill32 proc dst:dword,taille:dword,data:dword
;public JPSDR_AddBorder_Fill32
; rcx : dst
; rdx : taille
; r8d : data

JPSDR_AddBorder_Fill32 proc public frame

	push rdi
	.pushreg rdi
	.endprolog
	
	cld
	xor rax,rax
	mov rdi,rcx
	mov eax,r8d
	shl r8,32
	mov rcx,rdx
	or rax,r8
	shr rcx,1
	jz short suite_1_Fill32
	rep stosq
	
suite_1_Fill32:	
	and rdx,1
	jz short fin_Fill32	
	stosd

fin_Fill32:
	pop rdi

	ret

JPSDR_AddBorder_Fill32 endp



end





