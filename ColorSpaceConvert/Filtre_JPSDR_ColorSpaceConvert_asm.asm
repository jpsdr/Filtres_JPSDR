.586
.xmm
.model flat,c

.data

align 16

uw_2 word 8 dup(0002h)
uw_3 word 8 dup(0003h)


.code


JPSDR_ColorSpaceConvert_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_ColorSpaceConvert_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,w
	or ebx,ebx
	jz short fin_F	
	mov edx,h
	or edx,edx
	jz short fin_F	
loop_F:
	mov ecx,ebx
	rep movsd
	add esi,eax
	add edi,dst_modulo
	dec edx
	jnz short loop_F
	
fin_F:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_Move32_Full endp


JPSDR_ColorSpaceConvert_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_ColorSpaceConvert_Move8_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,w
	or ebx,ebx
	jz short fin_F8
	mov edx,h
	or edx,edx
	jz short fin_F8	
loop_F8:
	mov ecx,ebx
	shr ecx,2
	jz short loop_F8_suite1
	rep movsd
loop_F8_suite1:	
	mov ecx,ebx
	and ecx,3
	jz short loop_F8_suite2
	rep movsb
loop_F8_suite2:		
	add esi,eax
	add edi,dst_modulo
	dec edx
	jnz short loop_F8
	
fin_F8:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_Move8_Full endp



JPSDR_ColorSpaceConvert_YV24_SSE proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV24_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm4,xmm4					
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	movzx eax,offset_U
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_V
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	movzx eax,Min_U
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_U
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_V
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	movzx eax,Max_V
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	
	mov eax,w
	shr eax,1
	mov w0,eax
	
	mov esi,src_y

Boucle0_2:
	mov eax,w0
	mov i,eax
Boucle1_2:
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]
	add src_y,2
	mov esi,src_u
	push eax
	movzx ecx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]
	add src_u,2
	mov esi,src_v
	push eax
	movzx edx,byte ptr[esi] ; edx=V ecx=U ebx=Y
	movzx eax,byte ptr[esi+1]
	add src_v,2
	mov esi,lookup
	push eax
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,4
	
	pop edx
	pop ecx
	pop ebx				; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,3
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,5
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4	
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	mov edi,dst_u
	add dst_y,2
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	mov edi,dst_v
	add dst_u,2
	pextrw eax,xmm0,2
	mov word ptr[edi],ax
	add dst_v,2
	
	mov esi,src_y
	
	dec i
	jnz Boucle1_2		
	
	mov eax,w
	and eax,1
	jz Suite1_2
	
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	inc src_y
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	inc src_u
	movzx edx,byte ptr[esi] ; edx=V ecx=U ebx=Y
	mov esi,lookup
	inc src_v
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	mov edi,dst_u
	inc dst_y
	mov byte ptr[edi],ah
	mov edi,dst_v
	inc dst_u
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src_y		
	
Suite1_2:	
	add esi,src_modulo_y
	mov eax,src_u
	add eax,src_modulo_u
	mov src_u,eax
	mov eax,src_v
	add eax,src_modulo_v
	mov src_v,eax	
	mov eax,dst_y
	add eax,dst_modulo_y
	mov dst_y,eax
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	mov src_y,esi
	dec h
	jnz Boucle0_2

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YV24_SSE endp



JPSDR_ColorSpaceConvert_YUY2_SSE proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YUY2_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,6
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,6
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,6
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,7
	movzx eax,Max_V
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,7
	
	mov eax,w
	dec eax
	shr eax,1
	mov w0,eax	
	
	mov esi,src
	mov edi,dst

Boucle0_4:
	movzx ecx,byte ptr[esi+1]
	movzx edx,byte ptr[esi+3]

	mov eax,w0
	or eax,eax
	jz Suite0_4
	mov i,eax
Boucle1_4:
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+5]
	push eax
	movzx eax,byte ptr[esi+7]
	push eax
	movzx eax,byte ptr[esi+2]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2

	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,3
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2

	mov esi,src
	pop edx
	pop ecx
	
	movzx ebx,byte ptr[esi+4]
	movzx eax,byte ptr[esi+9]
	push eax
	movzx eax,byte ptr[esi+11]
	push eax
	movzx eax,byte ptr[esi+6]	
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	mov esi,lookup
	add src,8
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,7
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[edi],xmm0
	
	mov esi,src
	add edi,8
	
	pop edx
	pop ecx
	
	dec i
	jnz Boucle1_4
	
Suite0_4:	
	mov eax,w
	dec eax
	and eax,1
	jz Suite1_4
	
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+5]
	push eax
	movzx eax,byte ptr[esi+7]
	push eax
	movzx eax,byte ptr[esi+2]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,3
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop edx
	pop ecx
	
	mov esi,src
	movzx ebx,byte ptr[esi+4]
	movzx eax,byte ptr[esi+6]
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2
	
	mov esi,lookup
	add src,8
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pop ebx
	pinsrw xmm0,eax,7
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	movq qword ptr[edi],xmm0
	
	mov esi,src
	add edi,8

	jmp Suite2_4
	
Suite1_4:	
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+2]	
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2
	
	mov esi,lookup
	add src,4
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pop ebx
	pinsrw xmm0,eax,3
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	movd dword ptr[edi],xmm0
	
	mov esi,src
	add edi,4	
	
Suite2_4:	
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_4

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YUY2_SSE endp


JPSDR_ColorSpaceConvert_UYVY_SSE proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_UYVY_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,4
	movzx eax,Max_U
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	
	mov eax,w
	dec eax
	shr eax,1
	mov w0,eax	
	
	mov esi,src
	mov edi,dst

Boucle0_6:
	movzx ecx,byte ptr[esi]
	movzx edx,byte ptr[esi+2]

	mov eax,w0
	or eax,eax
	jz Suite0_6
	mov i,eax
Boucle1_6:
	movzx ebx,byte ptr[esi+1]
	movzx eax,byte ptr[esi+4]
	push eax
	movzx eax,byte ptr[esi+6]
	push eax
	movzx eax,byte ptr[esi+3]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2

	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3

	mov esi,src
	pop edx
	pop ecx
	
	movzx ebx,byte ptr[esi+5]
	movzx eax,byte ptr[esi+8]
	push eax
	movzx eax,byte ptr[esi+10]
	push eax
	movzx eax,byte ptr[esi+7]	
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	mov esi,lookup
	add src,8
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,7

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[edi],xmm0
	
	mov esi,src
	add edi,8
	
	pop edx
	pop ecx
	
	dec i
	jnz Boucle1_6
	
Suite0_6:	
	mov eax,w
	dec eax
	and eax,1
	jz Suite1_6
	
	movzx ebx,byte ptr[esi+1]
	movzx eax,byte ptr[esi+4]
	push eax
	movzx eax,byte ptr[esi+6]
	push eax
	movzx eax,byte ptr[esi+3]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	pop edx
	pop ecx
	
	mov esi,src
	movzx ebx,byte ptr[esi+5]
	movzx eax,byte ptr[esi+7]
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2
	
	mov esi,lookup
	add src,8
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pop ebx
	pinsrw xmm0,eax,6
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	movq qword ptr[edi],xmm0
	
	mov esi,src
	add edi,8

	jmp Suite2_6
	
Suite1_6:	
	movzx ebx,byte ptr[esi+1]
	movzx eax,byte ptr[esi+3]	
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2
	
	mov esi,lookup
	add src,4
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pop ebx
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	movd dword ptr[edi],xmm0
	
	mov esi,src
	add edi,4	
	
Suite2_6:	
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_6

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_UYVY_SSE endp



JPSDR_ColorSpaceConvert_YV16_SSE proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV16_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
	mov eax,w
	dec eax
	shr eax,1
	mov w0,eax	

Boucle0_8:
	mov esi,src_v
	movzx edx,byte ptr[esi]
	
	mov esi,src_u
	movzx ecx,byte ptr[esi]

	mov eax,w0
	or eax,eax
	jz Suite0_8
	mov i,eax
Boucle1_8:
	movzx eax,byte ptr[esi+1]
	mov esi,src_v
	push eax
	movzx eax,byte ptr[esi+1]
	mov esi,src_y
	push eax
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	mov esi,src_u
	movzx eax,byte ptr[esi+2]
	add src_u,2
	mov esi,src_v
	push eax
	movzx eax,byte ptr[esi+2]
	add src_v,2
	mov esi,src_y
	push eax
	
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	add src_y,4
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,7
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	mov edi,dst_u
	pextrw eax,xmm0,2
	mov word ptr[edi],ax
	mov edi,dst_v
	add dst_u,2
	pextrw eax,xmm0,3
	mov word ptr[edi],ax
	
	mov edi,dst_y
	add dst_v,2
	movd dword ptr[edi],xmm0
	add dst_y,4
	
	pop edx
	pop ecx
	
	mov esi,src_u
		
	dec i
	jnz Boucle1_8
	
Suite0_8:	
	mov eax,w
	dec eax
	and eax,1
	jz Suite1_8
	
	movzx eax,byte ptr[esi+1]
	add src_u,2
	mov esi,src_v
	push eax
	movzx eax,byte ptr[esi+1]
	add src_v,2
	mov esi,src_y
	push eax
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	mov esi,src_y
	pop edx
	pop ecx
	
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	add src_y,4
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pop ebx
	pinsrw xmm0,eax,7
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	mov edi,dst_u
	pextrw eax,xmm0,2
	mov word ptr[edi],ax
	mov edi,dst_v
	add dst_u,2
	pextrw eax,xmm0,3
	mov word ptr[edi],ax
	
	mov edi,dst_y
	add dst_v,2
	movd dword ptr[edi],xmm0
	add dst_y,4

	jmp Suite2_8
	
Suite1_8:	
	mov esi,src_y
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2
	
	add src_y,2
	inc src_u
	inc src_v
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pop ebx
	pinsrw xmm0,eax,6
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add dst_y,2
	mov word ptr[edi],ax
	mov edi,dst_u
	pextrw eax,xmm0,2
	inc dst_u
	mov byte ptr[edi],al
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_v
	mov byte ptr[edi],al
	
Suite2_8:
	mov eax,src_y
	add eax,src_modulo_y
	mov src_y,eax
	mov eax,src_u
	add eax,src_modulo_u
	mov src_u,eax
	mov eax,src_v
	add eax,src_modulo_v
	mov src_v,eax	
	mov eax,dst_y
	add eax,dst_modulo_y
	mov dst_y,eax
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	dec h
	jnz Boucle0_8

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YV16_SSE endp


JPSDR_ColorSpaceConvert_YV12_SSE2_1 proc src_Y:dword,src_U:dword,src_V:dword,dst_Y:dword,lookup:dword,w:dword,offset_Y:word,Min_Y:word,Max_Y:word

	public JPSDR_ColorSpaceConvert_YV12_SSE2_1
	
	local i:dword
	
	push esi
	push edi
	push ebx
	
	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
	mov edi,dst_Y
	
	mov esi,src_V
	movzx edx,byte ptr[esi]
	
	mov esi,src_U
	movzx ecx,byte ptr[esi]

	mov eax,w
	dec eax
	shr eax,2
	jz Suite0_9
	mov i,eax
	
Boucle0_9:	
	movzx eax,byte ptr[esi+1]
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+1]
	mov esi,src_Y
	push eax
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	mov esi,src_U
	movzx eax,byte ptr[esi+2]
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+2]
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	pop edx
	pop ecx
	
	mov esi,src_U
	movzx eax,byte ptr[esi+3]
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+3]
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+4]
	movzx eax,byte ptr[esi+5]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,5
	
	pop edx
	pop ecx
	
	mov esi,src_U
	movzx eax,byte ptr[esi+4]
	add src_U,4
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+4]
	add src_V,4
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+6]
	movzx eax,byte ptr[esi+7]
	add src_Y,8
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[edi],xmm0
	
	pop edx
	pop ecx
	add edi,8
	
	mov esi,src_U
		
	dec i
	jnz Boucle0_9
	
Suite0_9:
	mov eax,w
	dec eax
	and eax,3
	jz Suite2_9
	and eax,2
	jz suite1_9
	
	movzx eax,byte ptr[esi+1]
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+1]
	mov esi,src_Y
	push eax
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	mov esi,src_U
	movzx eax,byte ptr[esi+2]
	add src_U,2
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+2]
	add src_V,2
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	add src_Y,4
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[edi],xmm0	
	
	pop edx
	pop ecx	
	add edi,4
	mov esi,src_U
	
	mov eax,w
	dec eax	
	and eax,1
	jz Suite2_9
	
suite1_9:
	movzx eax,byte ptr[esi+1]
	mov esi,src_V
	push eax
	movzx eax,byte ptr[esi+1]
	mov esi,src_Y
	push eax
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	mov esi,src_Y
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pop ebx
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[edi],xmm0		
	
	jmp suite3_9

suite2_9:	
	mov esi,src_Y
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pop ebx
	pinsrw xmm0,eax,0
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	
suite3_9:	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_ColorSpaceConvert_YV12_SSE2_1 endp	



JPSDR_ColorSpaceConvert_YV12_SSE2_2 proc src_Y:dword,src_U1:dword,src_U2:dword,src_V1:dword,src_V2:dword,dst_Y:dword,lookup:dword,w:dword,
	offset_Y:word,Min_Y:word,Max_Y:word

	public JPSDR_ColorSpaceConvert_YV12_SSE2_2
	
	local i:dword
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm5,xmm5
	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
	movdqa xmm6,oword ptr uw_2
	
	mov edi,dst_Y
	
	mov eax,w
	dec eax
	shr eax,2
	jz Suite0_10
	mov i,eax
	
Boucle0_10:	
	mov esi,src_U1
	mov ebx,src_U2
	movd xmm0,dword ptr[esi]
	movd xmm4,dword ptr[ebx]
	punpcklbw xmm0,xmm7
	movzx eax,byte ptr[esi+4]
	punpcklbw xmm4,xmm7
	pinsrw xmm0,eax,4
	movzx eax,byte ptr[ebx+4]
	pmullw xmm0,oword ptr uw_3
	pinsrw xmm4,eax,4
	paddsw xmm0,xmm6
	paddsw xmm4,xmm0
	psraw xmm4,2
	
	mov esi,src_V1
	mov ebx,src_V2
	movd xmm0,dword ptr[esi]
	movd xmm5,dword ptr[ebx]
	punpcklbw xmm0,xmm7
	movzx eax,byte ptr[esi+4]
	punpcklbw xmm5,xmm7
	pinsrw xmm0,eax,4
	movzx eax,byte ptr[ebx+4]
	pmullw xmm0,oword ptr uw_3
	pinsrw xmm5,eax,4
	paddsw xmm0,xmm6
	paddsw xmm5,xmm0
	psraw xmm5,2
	
	pextrw eax,xmm4,1
	pextrw ecx,xmm4,0
	push eax
	pextrw eax,xmm5,1
	pextrw edx,xmm5,0
	push eax
		
	mov esi,src_Y
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	pextrw eax,xmm4,2
	push eax
	pextrw eax,xmm5,2
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	pop edx
	pop ecx
	
	pextrw eax,xmm4,3
	push eax
	pextrw eax,xmm5,3
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+4]
	movzx eax,byte ptr[esi+5]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,5
	
	pop edx
	pop ecx
	
	mov esi,src_Y
	movzx ebx,byte ptr[esi+6]
	movzx eax,byte ptr[esi+7]
	add src_Y,8
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6
	
	pextrw eax,xmm4,4
	add ecx,eax
	pextrw eax,xmm5,4
	add edx,eax
	shr ecx,1
	pop ebx
	shr edx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[edi],xmm0
	
	add src_U1,4
	add src_V1,4
	add src_U2,4
	add src_V2,4
	add edi,8
	
	dec i
	jnz Boucle0_10
	
Suite0_10:
	mov esi,src_U1
	mov ebx,src_U2
	movd xmm0,dword ptr[esi]
	movd xmm4,dword ptr[ebx]
	punpcklbw xmm0,xmm7
	punpcklbw xmm4,xmm7
	pmullw xmm0,oword ptr uw_3
	paddsw xmm0,xmm6
	paddsw xmm4,xmm0
	psraw xmm4,2
	
	mov esi,src_V1
	mov ebx,src_V2
	movd xmm0,dword ptr[esi]
	movd xmm5,dword ptr[ebx]
	punpcklbw xmm0,xmm7
	punpcklbw xmm5,xmm7
	pmullw xmm0,oword ptr uw_3
	paddsw xmm0,xmm6
	paddsw xmm5,xmm0
	psraw xmm5,2
	
	pextrw ecx,xmm4,0
	pextrw edx,xmm5,0
	
	mov eax,w
	dec eax
	and eax,3
	jz Suite2_10
	and eax,2
	jz suite1_10
	
	pextrw eax,xmm4,1
	push eax
	pextrw eax,xmm5,1
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	pextrw eax,xmm4,2
	push eax
	pextrw eax,xmm5,2
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	add src_Y,4
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[edi],xmm0	
	
	pop edx
	pop ecx	
	add edi,4
	
	mov eax,w
	dec eax	
	and eax,1
	jz Suite2_10
	
	pextrw eax,xmm4,3
	push eax
	pextrw eax,xmm5,3
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	mov esi,src_Y
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pop ebx
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[edi],xmm0		
	
	jmp suite3_10
			
suite1_10:
	pextrw eax,xmm4,1
	push eax
	pextrw eax,xmm5,1
	mov esi,src_Y
	push eax
	
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	
	add edx,dword ptr[esp+4]
	add ecx,dword ptr[esp+8]
	shr edx,1
	pop ebx
	shr ecx,1
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	pop edx
	pop ecx
	
	mov esi,src_Y
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	mov esi,lookup
	push eax					; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pop ebx
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[edi],xmm0		
	
	jmp suite3_10

suite2_10:	
	mov esi,src_Y
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]	
	mov esi,lookup
	push eax							; edx=V ecx=U ebx=Y1 [pop eax]=Y2 V2 U2
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pop ebx
	pinsrw xmm0,eax,0
	
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	
suite3_10:	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_ColorSpaceConvert_YV12_SSE2_2 endp	



JPSDR_ColorSpaceConvert_YV12_SSE2_3 proc src_Y:dword,src_U:dword,src_V:dword,dst_U:dword,dst_V:dword,lookup:dword,w:dword,h:dword,
	src_pitch_y:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_u:dword,dst_modulo_v:dword,offset_U:word,Min_U:word,
	Max_U:word,offset_V:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV12_SSE2_3
	
	local i:dword
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_U
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_V
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7	
	movzx eax,Min_U
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Min_V
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7	
	movzx eax,Max_U
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Max_V
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
Boucle0_11:
	mov eax,w
	shr eax,2
	jz Suite0_11
	mov i,eax
	
Boucle1_11:	
	mov esi,src_Y
	movq xmm4,qword ptr[esi]
	add esi,src_pitch_y
	add src_Y,8
	movq xmm0,qword ptr[esi]
	pavgb xmm4,xmm0
	punpcklbw xmm4,xmm7
	
	mov esi,src_U
	mov ebx,src_V
	movzx ecx,byte ptr[esi+3]
	movzx edx,byte ptr[ebx+3]
	push ecx
	push edx
	movzx ecx,byte ptr[esi+2]
	movzx edx,byte ptr[ebx+2]
	push ecx
	push edx
	movzx ecx,byte ptr[esi+1]
	movzx edx,byte ptr[ebx+1]
	push ecx
	push edx
	movzx ecx,byte ptr[esi]
	movzx edx,byte ptr[ebx]
	add src_U,4
	pextrw ebx,xmm4,0			; edx=V ecx=U ebx=Y
	mov esi,lookup
	add src_V,4

	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,4
	
	pextrw ebx,xmm4,2
	pop edx
	pop ecx	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,5
	
	pextrw ebx,xmm4,4
	pop edx
	pop ecx	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	pextrw ebx,xmm4,6
	pop edx
	pop ecx	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,3
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,7
	mov edi,dst_U
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm7
	
	movd dword ptr[edi],xmm0
	add dst_U,4
	mov edi,dst_V
	psrldq xmm0,4
	add dst_V,4
	movd dword ptr[edi],xmm0
	
	dec i
	jnz Boucle1_11
	
Suite0_11:
	mov eax,w
	and eax,2
	jz suite1_11
	
	mov esi,src_Y
	movd xmm4,dword ptr[esi]
	add esi,src_pitch_y
	add src_Y,4
	movd xmm0,dword ptr[esi]
	pavgb xmm4,xmm0
	punpcklbw xmm4,xmm7
	
	mov esi,src_U
	mov ebx,src_V
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[ebx+1]
	push ecx
	push edx
	movzx ecx,byte ptr[esi]
	movzx edx,byte ptr[ebx]
	add src_U,2
	pextrw ebx,xmm4,0			; edx=V ecx=U ebx=Y
	mov esi,lookup
	add src_V,2

	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,4
	
	pextrw ebx,xmm4,2
	pop edx
	pop ecx	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,5
	mov edi,dst_U
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm7
	
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add dst_U,2
	mov edi,dst_V
	pextrw eax,xmm0,2
	add dst_V,2
	mov word ptr[edi],ax
	
suite1_11:	
	mov eax,w
	and eax,1
	jz suite2_11
	
	mov esi,src_Y
	movzx eax,byte ptr[esi]
	add esi,src_pitch_y
	add src_Y,2
	movzx ebx,byte ptr[esi]
	add ebx,eax
	shr ebx,1
	
	mov esi,src_U
	mov edi,src_V
	movzx ecx,byte ptr[esi]
	movzx ebx,byte ptr[edi] ; edx=V ecx=U ebx=Y
	inc src_U
	mov esi,lookup
	inc src_V
	mov edi,dst_U
	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,4
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm7
	
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	inc dst_U
	mov edi,dst_V
	pextrw eax,xmm0,2
	inc dst_V
	mov byte ptr[edi],al
	
suite2_11:		
	mov eax,src_Y
	add eax,src_modulo_y
	add eax,src_pitch_y
	mov src_Y,eax
	mov eax,src_U
	add eax,src_modulo_u
	mov src_U,eax
	mov eax,src_V
	add eax,src_modulo_v
	mov src_V,eax
	mov eax,dst_U
	add eax,dst_modulo_u
	mov dst_U,eax
	mov eax,dst_V
	add eax,dst_modulo_v
	mov dst_V,eax
	
	dec h
	jnz Boucle0_11
	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_ColorSpaceConvert_YV12_SSE2_3 endp


end
