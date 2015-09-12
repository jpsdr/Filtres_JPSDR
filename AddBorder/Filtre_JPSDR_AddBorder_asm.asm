.586
.model flat,c
.code


JPSDR_AddBorder_Fill32 proc dst:dword,taille:dword,data:dword

	public JPSDR_AddBorder_Fill32

	push edi
	
	cld
	mov edi,dst
	mov ecx,taille
	mov eax,data
	or ecx,ecx
	jz short fin_Fill32
	rep stosd

fin_Fill32:	
	pop edi

	ret

JPSDR_AddBorder_Fill32 endp




end





