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
	mov rdi,rcx
	xor rax,rax
	mov rcx,rdx
	mov eax,r8d
	shl r8,32
	or rdx,rdx
	jz short fin_Fill32	
	or rax,r8
	shr rcx,1
	jnc short suite_1_Fill32
	stosd
	or rcx,rcx
	jz short fin_Fill32	
suite_1_Fill32:	
	rep stosq

fin_Fill32:
	pop rdi

	ret

JPSDR_AddBorder_Fill32 endp



;JPSDR_AddBorder_Fill8 proc dst:dword,taille:dword,data:byte
;public JPSDR_AddBorder_Fill8
; rcx : dst
; rdx : taille
; r8d : data

JPSDR_AddBorder_Fill8 proc public frame

	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	.endprolog
	
	cld
	xor rax,rax
	xor ebx,ebx
	mov rdi,rcx
	mov eax,r8d
	or rdx,rdx
	jz short fin_Fill8
	mov ah,al
	mov rcx,rdx
	mov bx,ax
	shl eax,16
	or eax,ebx
	mov r8d,eax
	shl r8,32
	or rax,r8
	and rcx,7
	jz short suite_1_Fill8
	rep stosb
suite_1_Fill8:
	mov rcx,rdx
	shr rcx,3
	jz short fin_Fill8
	rep stosq
	
fin_Fill8:
	pop rbx
	pop rdi
	
	ret		
	
JPSDR_AddBorder_Fill8 endp


end





