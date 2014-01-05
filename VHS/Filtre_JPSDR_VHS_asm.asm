.586
.mmx
.xmm
.model flat,c

.code


JPSDR_VHS_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_VHS_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_1_f_3:
	mov ecx,ebx
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_1_f_3
	
fin_f_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_Move32_Full endp



end





