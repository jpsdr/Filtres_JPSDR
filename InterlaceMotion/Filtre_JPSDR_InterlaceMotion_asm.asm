.586
.model flat,c

.code

JPSDR_InterlaceMotion_Move32_Full_src proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword

	public JPSDR_InterlaceMotion_Move32_Full_src

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
	mov eax,src_modulo
loop_G:
	mov ecx,ebx
	rep movsd
	add esi,eax
	dec edx
	jnz short loop_G
	
fin_G:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_InterlaceMotion_Move32_Full_src endp

JPSDR_InterlaceMotion_Move32_Full_dst proc src:dword,dst:dword,w:dword,h:dword,dst_modulo:dword

	public JPSDR_InterlaceMotion_Move32_Full_dst

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
	mov eax,dst_modulo
loop_H:
	mov ecx,ebx
	rep movsd
	add edi,eax
	dec edx
	jnz short loop_H
	
fin_H:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_InterlaceMotion_Move32_Full_dst endp


JPSDR_InterlaceMotion_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_InterlaceMotion_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_F:
	mov ecx,ebx
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_F
	
fin_F:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_InterlaceMotion_Move32_Full endp



end





