.586
.model flat,c

.code

JPSDR_WideScreenRGB32 proc src:dword,dst:dword,w:dword,src_modulo:dword,dst_modulo:dword,top:dword,
	bottom:dword,src_offset_center:dword,dst_offset_center:dword

	public JPSDR_WideScreenRGB32


	push esi
	push edi
	push ebx

	mov esi,src
	mov edi,dst
	cld

	mov ebx,bottom
	or ebx,ebx
	jz short Center_
	mov ebx,src_modulo
	mov edx,dst_modulo
Loop_1_0:
	mov ecx,w
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
	add esi,ebx
	add edi,edx
	dec bottom
	jnz short Loop_1_0

Center_:
	add esi,src_offset_center
	add edi,dst_offset_center

Top_:
	mov edx,top
	or edx,edx
	jz short End_
	mov ebx,src_modulo
	mov edx,dst_modulo
	xor eax,eax
Loop_1_3:
	mov ecx,w
	rep stosd
	add esi,ebx
	add edi,edx
	dec top
	jnz short Loop_1_3

End_:
	pop ebx
	pop edi
	pop esi

	ret


JPSDR_WideScreenRGB32 endp

end

