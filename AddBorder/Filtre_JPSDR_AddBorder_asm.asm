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



JPSDR_AddBorder_Fill8 proc dst:dword,taille:dword,data:byte

	public JPSDR_AddBorder_Fill8

	push edi
	
	cld
	mov edi,dst
	mov edx,taille
	mov al,data
	or edx,edx
	jz short fin_Fill8
	mov ah,al
	mov ecx,edx
	shl eax,16
	mov al,data
	mov ah,al
	and ecx,3
	jz short suite_1_Fill8
	rep stosb
suite_1_Fill8:
	mov ecx,edx
	shr ecx,2
	jz short fin_Fill8
	rep stosd
	
fin_Fill8:
	pop edi
	
	ret		
	
JPSDR_AddBorder_Fill8 endp


end





