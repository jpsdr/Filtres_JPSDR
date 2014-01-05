.586
.model flat,c

.code


JPSDR_Field_Shift_Rebuild_Frame32 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;

	public JPSDR_Field_Shift_Rebuild_Frame32

	push esi
	push edi
	push ebx

	cld
	mov esi,bottom_src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
Loop_D_1:
	mov ecx,w
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D_1
	mov esi,top_src
	mov edi,dst
	mov edx,h
	add edi,dst_pitch
Loop_D_2:
	mov ecx,w
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D_2

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_Field_Shift_Rebuild_Frame32 endp


JPSDR_Field_Shift_Rebuild_Frame8 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;

	public JPSDR_Field_Shift_Rebuild_Frame8

	push esi
	push edi
	push ebx

	cld
	mov esi,bottom_src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
Loop_E_1:
	mov ecx,w
	and ecx,3
	jz short Rebuild8_1
	rep movsb
Rebuild8_1:
	mov ecx,w
	shr ecx,2
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_E_1
	mov esi,top_src
	mov edi,dst
	mov edx,h
	add edi,dst_pitch
Loop_E_2:
	mov ecx,w
	and ecx,3
	jz short Rebuild8_2
	rep movsb
Rebuild8_2:
	mov ecx,w
	shr ecx,2
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_E_2

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_Field_Shift_Rebuild_Frame8 endp


JPSDR_Field_Shift_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Field_Shift_Move32_Full

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

JPSDR_Field_Shift_Move32_Full endp


JPSDR_Field_Shift_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Field_Shift_Move8_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
	
loop_F8:
	mov ecx,ebx
	and ecx,3
	jz short loop_F8_suite
	rep movsb
loop_F8_suite:	
	mov ecx,ebx
	shr ecx,2
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_F8
	
fin_F8:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Field_Shift_Move8_Full endp


end





