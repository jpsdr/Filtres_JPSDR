.586
.model flat,c

.code

JPSDR_IVTC_Manual_Rebuild_Frame proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Manual_Rebuild_Frame

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

JPSDR_IVTC_Manual_Rebuild_Frame endp





JPSDR_IVTC_Manual_Rebuild_Frame8 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Manual_Rebuild_Frame8

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
	
Loop_D8_1:
	mov ecx,w
	and ecx,3
	jz short loop_D8_suite1
	rep movsb
loop_D8_suite1:	
	mov ecx,w
	shr ecx,2
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D8_1
	mov esi,top_src
	mov edi,dst
	mov edx,h
	add edi,dst_pitch
Loop_D8_2:
	mov ecx,w
	and ecx,3
	jz short loop_D8_suite2
	rep movsb
loop_D8_suite2:	
	mov ecx,w
	shr ecx,2
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D8_2

	pop ebx
	pop edi
	pop esi
	
	ret

JPSDR_IVTC_Manual_Rebuild_Frame8 endp


JPSDR_IVTC_Manual_Rebuild_Frame_2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword,
	src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Manual_Rebuild_Frame_2

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
Loop_E_1:
	mov ecx,w
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_E_1

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Manual_Rebuild_Frame_2 endp





JPSDR_IVTC_Manual_Rebuild_Frame8_2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword,
	src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Manual_Rebuild_Frame8_2

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
	
Loop_E8_1:
	mov ecx,w
	and ecx,3
	jz short loop_E8_suite1
	rep movsb
loop_E8_suite1:	
	mov ecx,w
	shr ecx,2
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_E8_1

	pop ebx
	pop edi
	pop esi
	
	ret

JPSDR_IVTC_Manual_Rebuild_Frame8_2 endp



end





