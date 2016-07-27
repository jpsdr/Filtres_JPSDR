.586
.xmm
.model flat,c

.data

align 16

Ymask qword 2 dup(00FF00FF00FF00FFh)

.code


JPSDR_Median_RGB32_Move_src proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_offset:dword,dst_offset:dword

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
	add edi,dst_offset
	add ebx,dst_offset
	add edx,dst_offset
	dec h
	jnz short loop_1_g
	
fin_g:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src endp


JPSDR_Median_RGB32_Move_src_SSE_1 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_src_SSE_1
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	movdqa xmm3,oword ptr Ymask
	pxor xmm0,xmm0
	
loop_1_g_1:
	mov ecx,w
	xor eax,eax
	shr ecx,2
	jz short suite_g_1
loop_2_g_1:
	movq xmm0,qword ptr[esi+8*eax]   ;00000000xRGBxRGB
	movq xmm2,qword ptr[esi+8*eax+8] ;00000000xRGBxRGB
	pslldq xmm2,8                    ;xRGBxRGB00000000
	por xmm0,xmm2                    ;xRGBxRGBxRGBxRGB
	movdqa xmm1,xmm0                 ;xRGBxRGBxRGBxRGB
	pand xmm0,xmm3                   ;0R0B0R0B0R0B0R0B
	psrlw xmm1,8                     ;0x0G0x0G0x0G0x0G
	packuswb xmm0,xmm0               ;00000000RBRBRBRB
	packuswb xmm1,xmm1               ;00000000xGxGxGxG
	movdqa xmm2,xmm0                 ;00000000RBRBRBRB
	pand xmm1,xmm3                   ;000000000G0G0G0G
	pand xmm0,xmm3                   ;000000000B0B0B0B
	packuswb xmm1,xmm1               ;000000000000GGGG
	psrlw xmm2,8                     ;000000000R0R0R0R
	packuswb xmm0,xmm0               ;000000000000BBBB
	packuswb xmm2,xmm2               ;000000000000RRRR
	movd dword ptr[edi+2*eax],xmm0
	movd dword ptr[ebx+2*eax],xmm1
	movd dword ptr[edx+2*eax],xmm2
	pxor xmm0,xmm0
	add eax,2
	loop loop_2_g_1

suite_g_1:
    mov ecx,w
	and ecx,3
	jz short suite_g_2
    push edx
	mov dx,word ptr[esi+8*eax]
	mov byte ptr[edi+2*eax],dl
	mov byte ptr[ebx+2*eax],dh
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+2]
	mov byte ptr[edx+2*eax],bl
	pop ebx
	dec ecx
	jz short suite_g_2
    push edx
	mov dx,word ptr[esi+8*eax+4]
	mov byte ptr[edi+2*eax+1],dl
	mov byte ptr[ebx+2*eax+1],dh
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+6]
	mov byte ptr[edx+2*eax+1],bl
	pop ebx
	dec ecx
	jz short suite_g_2
    push edx
	mov dx,word ptr[esi+8*eax+8]
	mov byte ptr[edi+2*eax+2],dl
	mov byte ptr[ebx+2*eax+2],dh
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+10]
	mov byte ptr[edx+2*eax+2],bl
	pop ebx
	
suite_g_2:	
	add esi,src_pitch
	add edi,dst_pitch
	add ebx,dst_pitch
	add edx,dst_pitch
	dec h
	jnz loop_1_g_1
	
fin_g_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src_SSE_1 endp


JPSDR_Median_RGB32_Move_src_SSE_2 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_src_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	movdqa xmm3,oword ptr Ymask
	
loop_1_g_2:
    xor eax,eax
	mov ecx,w
loop_2_g_2:
	movdqa xmm0,oword ptr[esi+8*eax] ;xRGBxRGBxRGBxRGB
	movdqa xmm1,xmm0                 ;xRGBxRGBxRGBxRGB
	pand xmm0,xmm3                   ;0R0B0R0B0R0B0R0B
	psrlw xmm1,8                     ;0x0G0x0G0x0G0x0G
	packuswb xmm0,xmm0               ;00000000RBRBRBRB
	packuswb xmm1,xmm1               ;00000000xGxGxGxG
	movdqa xmm2,xmm0                 ;00000000RBRBRBRB
	pand xmm1,xmm3                   ;000000000G0G0G0G
	pand xmm0,xmm3                   ;000000000B0B0B0B
	packuswb xmm1,xmm1               ;000000000000GGGG
	psrlw xmm2,8                     ;000000000R0R0R0R
	packuswb xmm0,xmm0               ;000000000000BBBB
	packuswb xmm2,xmm2               ;000000000000RRRR
	movd dword ptr[edi+2*eax],xmm0
	movd dword ptr[ebx+2*eax],xmm1
	movd dword ptr[edx+2*eax],xmm2
	add eax,2
	loop loop_2_g_2
	
	add esi,src_pitch
	add edi,dst_pitch
	add ebx,dst_pitch
	add edx,dst_pitch
	dec h
	jnz short loop_1_g_2
	
fin_g_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src_SSE_2 endp


JPSDR_Median_YUYV_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword,dst_offset_Y:dword,dst_offset_UV:dword

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
	add edi,dst_offset_Y
	add ebx,dst_offset_UV
	add edx,dst_offset_UV
	dec h
	jnz short loop_1_h
	
fin_h:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src endp


JPSDR_Median_YUYV_Move_src_SSE_1 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_YUYV_Move_src_SSE_1
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	movdqa xmm3,oword ptr Ymask
	pxor xmm0,xmm0
	
loop_1_h_1:
	mov ecx,w
	xor eax,eax
	shr ecx,2
	jz short suite_h_1	
loop_2_h_1:
	movq xmm0,qword ptr[esi+8*eax]   ;00000000VYUYVYUY
	movq xmm2,qword ptr[esi+8*eax+8] ;00000000VYUYVYUY
	pslldq xmm2,8                    ;VYUYVYUY00000000
	por xmm0,xmm2                    ;VYUYVYUYVYUYVYUY
	movdqa xmm1,xmm0                 ;VYUYVYUYVYUYVYUY
	pand xmm0,xmm3                   ;0Y0Y0Y0Y0Y0Y0Y0Y
	psrlw xmm1,8                     ;0V0U0V0U0V0U0V0U
	packuswb xmm0,xmm0               ;00000000YYYYYYYY
	packuswb xmm1,xmm1               ;00000000VUVUVUVU
	movdqa xmm2,xmm1                 ;00000000VUVUVUVU
	pand xmm1,xmm3                   ;000000000U0U0U0U
	psrlw xmm2,8                     ;000000000V0V0V0V
	packuswb xmm1,xmm1               ;000000000000UUUU
	packuswb xmm2,xmm2               ;000000000000VVVV
	movq qword ptr[edi+4*eax],xmm0
	movd dword ptr[ebx+2*eax],xmm1
	movd dword ptr[edx+2*eax],xmm2
	pxor xmm0,xmm0
	add eax,2
	loop loop_2_h_1
	
suite_h_1:
    mov ecx,w
	and ecx,3
	jz short suite_h_2
    push edx
	mov dx,word ptr[esi+8*eax]
	mov byte ptr[edi+4*eax],dl
	mov byte ptr[ebx+2*eax],dh
	mov dl,byte ptr[esi+8*eax+2]
	mov byte ptr[edi+4*eax+1],dl
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+3]
	mov byte ptr[edx+2*eax],bl
	pop ebx
	dec ecx
	jz short suite_h_2
    push edx
	mov dx,word ptr[esi+8*eax+4]
	mov byte ptr[edi+4*eax+2],dl
	mov byte ptr[ebx+2*eax+1],dh
	mov dl,byte ptr[esi+8*eax+6]
	mov byte ptr[edi+4*eax+3],dl
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+7]
	mov byte ptr[edx+2*eax+1],bl
	pop ebx
	dec ecx	
	jz short suite_h_2
    push edx
	mov dx,word ptr[esi+8*eax+8]
	mov byte ptr[edi+4*eax+4],dl
	mov byte ptr[ebx+2*eax+2],dl
	mov dl,byte ptr[esi+8*eax+10]
	mov byte ptr[edi+4*eax+5],dl
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+11]
	mov byte ptr[edx+2*eax+2],bl
	pop ebx
	
suite_h_2:			
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV
	dec h
	jnz loop_1_h_1
	
fin_h_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src_SSE_1 endp


JPSDR_Median_YUYV_Move_src_SSE_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_YUYV_Move_src_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	movdqa xmm3,oword ptr Ymask
	
loop_1_h_2:
    xor eax,eax    
	mov ecx,w
loop_2_h_2:
	movdqa xmm0,oword ptr[esi+8*eax]   ;VYUYVYUYVYUYVYUY
	movdqa xmm1,xmm0                   ;VYUYVYUYVYUYVYUY
	pand xmm0,xmm3                     ;0Y0Y0Y0Y0Y0Y0Y0Y
	psrlw xmm1,8                       ;0V0U0V0U0V0U0V0U
	packuswb xmm0,xmm0                 ;00000000YYYYYYYY
	packuswb xmm1,xmm1                 ;00000000VUVUVUVU
	movdqa xmm2,xmm1                   ;00000000VUVUVUVU
	pand xmm1,xmm3                     ;000000000U0U0U0U
	psrlw xmm2,8                       ;000000000V0V0V0V
	packuswb xmm1,xmm1                 ;000000000000UUUU
	packuswb xmm2,xmm2                 ;000000000000VVVV
	movq qword ptr[edi+4*eax],xmm0
	movd dword ptr[ebx+2*eax],xmm1
	movd dword ptr[edx+2*eax],xmm2
	add eax,2
	loop loop_2_h_2
	
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV
	dec h
	jnz short loop_1_h_2
	
fin_h_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src_SSE_2 endp


JPSDR_Median_UYVY_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword,dst_offset_Y:dword,dst_offset_UV:dword

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
	add edi,dst_offset_Y
	add ebx,dst_offset_UV
	add edx,dst_offset_UV	
	dec h
	jnz short loop_1_i
	
fin_i:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src endp




JPSDR_Median_UYVY_Move_src_SSE_1 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_UYVY_Move_src_SSE_1
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	movdqa xmm3,oword ptr Ymask
	pxor xmm0,xmm0
	
loop_1_i_1:
	mov ecx,w
	xor eax,eax
	shr ecx,2
	jz short suite_i_1		
loop_2_i_1:
	movq xmm0,qword ptr[esi+8*eax]   ;00000000YVYUYVYU
	movq xmm2,qword ptr[esi+8*eax+8] ;00000000YVYUYVYU
	pslldq xmm2,8                    ;YVYUYVYU00000000
	por xmm0,xmm2                    ;YVYUYVYUYVYUYVYU
	movdqa xmm1,xmm0                 ;YVYUYVYUYVYUYVYU
	pand xmm0,xmm3                   ;0V0U0V0U0V0U0V0U
	psrlw xmm1,8                     ;0Y0Y0Y0Y0Y0Y0Y0Y
	packuswb xmm0,xmm0               ;00000000VUVUVUVU
	packuswb xmm1,xmm1               ;00000000YYYYYYYY
	movdqa xmm2,xmm0                 ;00000000VUVUVUVU
	pand xmm0,xmm3                   ;000000000U0U0U0U
	psrlw xmm2,8                     ;000000000V0V0V0V
	packuswb xmm0,xmm0               ;000000000000UUUU
	packuswb xmm2,xmm2               ;000000000000VVVV
	movq qword ptr[edi+4*eax],xmm1
	movd dword ptr[ebx+2*eax],xmm0
	movd dword ptr[edx+2*eax],xmm2
	pxor xmm0,xmm0
	add eax,2
	loop loop_2_i_1
	
suite_i_1:
    mov ecx,w
	and ecx,3
	jz short suite_i_2
    push edx
	mov dx,word ptr[esi+8*eax]
	mov byte ptr[ebx+2*eax],dl
	mov byte ptr[edi+4*eax],dh
	mov dl,byte ptr[esi+8*eax+3]
	mov byte ptr[edi+4*eax+1],dl
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+2]
	mov byte ptr[edx+2*eax],bl
	pop ebx
	dec ecx
	jz short suite_i_2
    push edx
	mov dx,word ptr[esi+8*eax+4]
	mov byte ptr[ebx+2*eax+1],dl
	mov byte ptr[edi+4*eax+2],dh
	mov dl,byte ptr[esi+8*eax+7]
	mov byte ptr[edi+4*eax+3],dl
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+6]
	mov byte ptr[edx+2*eax+1],bl
	pop ebx
	dec ecx	
	jz short suite_i_2
    push edx
	mov dx,word ptr[esi+8*eax+8]
	mov byte ptr[ebx+2*eax+2],dl
	mov byte ptr[edi+4*eax+4],dl
	mov dl,byte ptr[esi+8*eax+11]
	mov byte ptr[edi+4*eax+5],dl
	pop edx
	push ebx
	mov bl,byte ptr[esi+8*eax+10]
	mov byte ptr[edx+2*eax+2],bl
	pop ebx
	
suite_i_2:				
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV	
	dec h
	jnz loop_1_i_1
	
fin_i_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src_SSE_1 endp



JPSDR_Median_UYVY_Move_src_SSE_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_UYVY_Move_src_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	movdqa xmm3,oword ptr Ymask
	
loop_1_i_2:
    xor eax,eax
	mov ecx,w
loop_2_i_2:
	movdqa xmm0,oword ptr[esi+8*eax]   ;YVYUYVYUYVYUYVYU
	movdqa xmm1,xmm0                   ;YVYUYVYUYVYUYVYU
	pand xmm0,xmm3                     ;0V0U0V0U0V0U0V0U
	psrlw xmm1,8                       ;0Y0Y0Y0Y0Y0Y0Y0Y
	packuswb xmm0,xmm0                 ;00000000VUVUVUVU
	packuswb xmm1,xmm1                 ;00000000YYYYYYYY
	movdqa xmm2,xmm0                   ;00000000VUVUVUVU
	pand xmm0,xmm3                     ;000000000U0U0U0U
	psrlw xmm2,8                       ;000000000V0V0V0V
	packuswb xmm0,xmm0                 ;000000000000UUUU
	packuswb xmm2,xmm2                 ;000000000000VVVV
	movq qword ptr[edi+4*eax],xmm1
	movd dword ptr[ebx+2*eax],xmm0
	movd dword ptr[edx+2*eax],xmm2
	add eax,2
	loop loop_2_i_2
	
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV	
	dec h
	jnz short loop_1_i_2
	
fin_i_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src_SSE_2 endp



JPSDR_Median_RGB32_Move_dst proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_offset:dword,dst_offset:dword

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
	
	add esi,src_offset
	add ebx,src_offset
	add edx,src_offset
	add edi,dst_offset
	dec h
	jnz short loop_1_j
		
fin_j:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst endp



JPSDR_Median_RGB32_Move_dst_SSE_1 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_dst_SSE_1
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	mov edi,dst
	pxor xmm3,xmm3
	
loop_1_j_1:
	mov ecx,w
	xor eax,eax
	shr ecx,2
	jz short suite_j_1	
loop_2_j_1:
    movd xmm0,dword ptr[esi+2*eax]  ;000000000000BBBB
	movd xmm1,dword ptr[ebx+2*eax]  ;000000000000GGGG
	movd xmm2,dword ptr[edx+2*eax]  ;000000000000RRRR
	punpcklbw xmm1,xmm3             ;000000000G0G0G0G
	punpcklbw xmm0,xmm2             ;00000000RBRBRBRB
	punpcklbw xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	movq qword ptr[edi+8*eax],xmm0
	psrldq xmm0,8
	movq qword ptr[edi+8*eax+8],xmm0
	add eax,2
	loop loop_2_j_1
	
suite_j_1:
    mov ecx,w
	and ecx,3
	jz short suite_j_2

    push edx
    mov dl,byte ptr[esi+2*eax]
	mov dh,byte ptr[ebx+2*eax]
	mov word ptr[edi+8*eax],dx
	pop edx
	push ebx
	mov bl,byte ptr[edx+2*eax]
	xor bh,bh
	mov word ptr[edi+8*eax+2],bx
	pop ebx
	dec ecx
	jz short suite_j_2
    push edx
    mov dl,byte ptr[esi+2*eax+1]
	mov dh,byte ptr[ebx+2*eax+1]
	mov word ptr[edi+8*eax+4],dx
	pop edx
	push ebx
	mov bl,byte ptr[edx+2*eax+1]
	xor bh,bh
	mov word ptr[edi+8*eax+6],bx
	pop ebx
	dec ecx
	jz short suite_j_2
    push edx
    mov dl,byte ptr[esi+2*eax+2]
	mov dh,byte ptr[ebx+2*eax+2]
	mov word ptr[edi+8*eax+8],dx
	pop edx
	push ebx
	mov bl,byte ptr[edx+2*eax+2]
	xor bh,bh
	mov word ptr[edi+8*eax+10],bx
	pop ebx
	
suite_j_2:	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz loop_1_j_1
		
fin_j_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst_SSE_1 endp



JPSDR_Median_RGB32_Move_dst_SSE_2 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_dst_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	mov edi,dst
	pxor xmm3,xmm3
	
loop_1_j_2:
    xor eax,eax
	mov ecx,w
loop_2_j_2:
    movd xmm0,dword ptr[esi+2*eax]  ;000000000000BBBB
	movd xmm1,dword ptr[ebx+2*eax]  ;000000000000GGGG
	movd xmm2,dword ptr[edx+2*eax]  ;000000000000RRRR
	punpcklbw xmm1,xmm3             ;000000000G0G0G0G
	punpcklbw xmm0,xmm2             ;00000000RBRBRBRB
	punpcklbw xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	movdqa oword ptr[edi+8*eax],xmm0
	add eax,2
	loop loop_2_j_2
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_j_2
		
fin_j_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst_SSE_2 endp



JPSDR_Median_YUYV_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_offset_Y:dword,src_offset_UV:dword,dst_offset:dword

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
	add esi,src_offset_Y
	add ebx,src_offset_UV
	add edx,src_offset_UV
	add edi,dst_offset
	dec h
	jnz short loop_1_k
		
fin_k:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst endp



JPSDR_Median_YUYV_Move_dst_SSE_1 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_YUYV_Move_dst_SSE_1
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_k_1:
	mov ecx,w
	xor eax,eax
	shr ecx,2
	jz short suite_k_1		
loop_2_k_1:
	movd xmm1,dword ptr[ebx+2*eax]  ;000000000000UUUU
	movd xmm2,dword ptr[edx+2*eax]  ;000000000000VVVV
	movq xmm0,qword ptr[esi+4*eax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	movq qword ptr[edi+8*eax],xmm0
	psrldq xmm0,8
	movq qword ptr[edi+8*eax+8],xmm0
	add eax,2
	loop loop_2_k_1
	
suite_k_1:
    mov ecx,w
	and ecx,3
	jz short suite_k_2

    push edx
    mov dl,byte ptr[esi+4*eax]
	mov dh,byte ptr[ebx+2*eax]
	mov word ptr[edi+8*eax],dx
	pop edx
	push ebx
	mov bl,byte ptr[esi+4*eax+1]
	mov bh,byte ptr[edx+2*eax]
	mov word ptr[edi+8*eax+2],bx
	pop ebx
	dec ecx
	jz short suite_k_2
    push edx
    mov dl,byte ptr[esi+4*eax+2]
	mov dh,byte ptr[ebx+2*eax+1]
	mov word ptr[edi+8*eax+4],dx
	pop edx
	push ebx
	mov bl,byte ptr[esi+4*eax+3]
	mov bh,byte ptr[edx+2*eax+1]
	mov word ptr[edi+8*eax+6],bx
	pop ebx
	dec ecx
	jz short suite_k_2
    push edx
    mov dl,byte ptr[esi+4*eax+4]
	mov dh,byte ptr[ebx+2*eax+2]
	mov word ptr[edi+8*eax+8],dx
	pop edx
	push ebx
	mov bl,byte ptr[esi+4*eax+5]
	mov bh,byte ptr[edx+2*eax+2]
	mov word ptr[edi+8*eax+10],bx
	pop ebx
	
suite_k_2:		
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz loop_1_k_1
		
fin_k_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst_SSE_1 endp


JPSDR_Median_YUYV_Move_dst_SSE_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_YUYV_Move_dst_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_k_2:
    xor eax,eax
	mov ecx,w
loop_2_k_2:
	movd xmm1,dword ptr[ebx+2*eax]  ;000000000000UUUU
	movd xmm2,dword ptr[edx+2*eax]  ;000000000000VVVV
	movq xmm0,qword ptr[esi+4*eax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	movdqa oword ptr[edi+8*eax],xmm0
	add eax,2
	loop loop_2_k_2
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz short loop_1_k_2
		
fin_k_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst_SSE_2 endp



JPSDR_Median_UYVY_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_offset_Y:dword,src_offset_UV:dword,dst_offset:dword

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
	
	add esi,src_offset_Y
	add ebx,src_offset_UV
	add edx,src_offset_UV
	add edi,dst_offset
	dec h
	jnz short loop_1_l
		
fin_l:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst endp


JPSDR_Median_UYVY_Move_dst_SSE_1 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_UYVY_Move_dst_SSE_1
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_l_1:
	mov ecx,w
	xor eax,eax
	shr ecx,2
	jz short suite_l_1			
loop_2_l_1:
	movd xmm1,dword ptr[ebx+2*eax]  ;000000000000UUUU
	movd xmm2,dword ptr[edx+2*eax]  ;000000000000VVVV
	movq xmm0,qword ptr[esi+4*eax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	movq qword ptr[edi+8*eax],xmm1
	psrldq xmm1,8
	movq qword ptr[edi+8*eax+8],xmm1
	add eax,2
	loop loop_2_l_1	
suite_l_1:
    mov ecx,w
	and ecx,3
	jz short suite_l_2

    push edx
	mov dl,byte ptr[ebx+2*eax]
    mov dh,byte ptr[esi+4*eax]
	mov word ptr[edi+8*eax],dx
	pop edx
	push ebx
	mov bl,byte ptr[edx+2*eax]
	mov bh,byte ptr[esi+4*eax+1]
	mov word ptr[edi+8*eax+2],bx
	pop ebx
	dec ecx
	jz short suite_l_2
    push edx
	mov dl,byte ptr[ebx+2*eax+1]
    mov dh,byte ptr[esi+4*eax+2]
	mov word ptr[edi+8*eax+4],dx
	pop edx
	push ebx
	mov bl,byte ptr[edx+2*eax+1]
	mov bh,byte ptr[esi+4*eax+3]
	mov word ptr[edi+8*eax+6],bx
	pop ebx
	dec ecx
	jz short suite_l_2
    push edx
	mov dl,byte ptr[ebx+2*eax+2]
    mov dh,byte ptr[esi+4*eax+4]
	mov word ptr[edi+8*eax+8],dx
	pop edx
	push ebx
	mov bl,byte ptr[edx+2*eax+2]
	mov bh,byte ptr[esi+4*eax+5]
	mov word ptr[edi+8*eax+10],bx
	pop ebx
	
suite_l_2:			
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz loop_1_l_1
		
fin_l_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst_SSE_1 endp


JPSDR_Median_UYVY_Move_dst_SSE_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_UYVY_Move_dst_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_l_2:
    xor eax,eax
	mov ecx,w
loop_2_l_2:
	movd xmm1,dword ptr[ebx+2*eax]  ;000000000000UUUU
	movd xmm2,dword ptr[edx+2*eax]  ;000000000000VVVV
	movq xmm0,qword ptr[esi+4*eax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	movdqa oword ptr[edi+8*eax],xmm1
	add eax,2
	loop loop_2_l_2
	
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz short loop_1_l_2
		
fin_l_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst_SSE_2 endp



end





