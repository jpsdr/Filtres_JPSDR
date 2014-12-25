.586
.model flat,c

.code


JPSDR_Median_Planar_Move_src proc src:dword,dst:dword,w:dword,h:dword,src_offset:dword

	public JPSDR_Median_Planar_Move_src

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov eax,src_offset
	mov ebx,w
	mov edx,h
	or ebx,ebx
	jz short fin_f_2
	or edx,edx
	jz short fin_f_2
loop_1_f_2:
	mov ecx,ebx
	shr ecx,2
	jz short loop_2_f_2
	rep movsd
loop_2_f_2:
	mov ecx,ebx	
	and ecx,3
	jz short loop_3_f_2
	rep movsb
loop_3_f_2:	
	add esi,eax
	dec edx
	jnz short loop_1_f_2
	
fin_f_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_Planar_Move_src endp



JPSDR_Median_Planar_Move_dst proc src:dword,dst:dword,w:dword,h:dword,dst_offset:dword

	public JPSDR_Median_Planar_Move_dst

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov eax,dst_offset
	mov ebx,w
	mov edx,h
	or ebx,ebx
	jz short fin_f_3
	or edx,edx
	jz short fin_f_3	
loop_1_f_3:
	mov ecx,ebx
	shr ecx,2
	jz short loop_2_f_3
	rep movsd
loop_2_f_3:
	mov ecx,ebx	
	and ecx,3
	jz short loop_3_f_3
	rep movsb
loop_3_f_3:	
	add edi,eax
	dec edx
	jnz short loop_1_f_3
	
fin_f_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_Planar_Move_dst endp


JPSDR_Median_RGB32_Move_src proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_offset:dword

	public JPSDR_Median_RGB32_Move_src
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	
loop_1_g:
	mov ecx,w
loop_2_g:
	lodsd
	stosb
	mov byte ptr[ebx],ah
	shr eax,16
	inc ebx
	mov byte ptr[edx],al
	inc edx
	loop loop_2_g
	add esi,src_offset
	dec h
	jnz short loop_1_g
	
fin_g:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src endp


JPSDR_Median_YUYV_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword

	public JPSDR_Median_YUYV_Move_src
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_h:
	mov ecx,w
loop_2_h:
	lodsd
	stosb
	mov byte ptr[ebx],ah
	shr eax,16
	inc ebx
	stosb
	mov byte ptr[edx],ah
	inc edx
	loop loop_2_h
	add esi,src_offset
	dec h
	jnz short loop_1_h
	
fin_h:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src endp

JPSDR_Median_UYVY_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword

	public JPSDR_Median_UYVY_Move_src
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_i:
	mov ecx,w
loop_2_i:
	lodsd
	mov byte ptr[edi],ah		
	mov byte ptr[ebx],al
	shr eax,16
	inc ebx
	mov byte ptr[edi+1],ah
	mov byte ptr[edx],al
	add edi,2
	inc edx
	loop loop_2_i
	add esi,src_offset
	dec h
	jnz short loop_1_i
	
fin_i:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src endp


JPSDR_Median_RGB32_Move_dst proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,dst_offset:dword

	public JPSDR_Median_RGB32_Move_dst
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	mov edi,dst
	
loop_1_j:
	mov ecx,w
loop_2_j:
	movzx eax,byte ptr[edx]
	inc edx
	shl eax,16
	lodsb
	mov ah,byte ptr[ebx]
	inc ebx
	stosd
	loop loop_2_j
	add edi,dst_offset
	dec h
	jnz short loop_1_j
		
fin_j:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst endp


JPSDR_Median_YUYV_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,dst_offset:dword

	public JPSDR_Median_YUYV_Move_dst
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_k:
	mov ecx,w
loop_2_k:
	mov ah,byte ptr[edx]
	mov al,byte ptr[esi+1]
	inc edx
	shl eax,16
	lodsb
	mov ah,byte ptr[ebx]
	inc esi
	stosd
	inc ebx
	loop loop_2_k
	add edi,dst_offset
	dec h
	jnz short loop_1_k
		
fin_k:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst endp


JPSDR_Median_UYVY_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,dst_offset:dword

	public JPSDR_Median_UYVY_Move_dst
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_l:
	mov ecx,w
loop_2_l:
	mov al,byte ptr[edx]
	mov ah,byte ptr[esi+1]
	inc edx
	shl eax,16
	mov ah,byte ptr[esi]
	mov al,byte ptr[ebx]
	add esi,2
	stosd
	inc ebx
	loop loop_2_l
	add edi,dst_offset
	dec h
	jnz short loop_1_l
		
fin_l:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst endp

end





