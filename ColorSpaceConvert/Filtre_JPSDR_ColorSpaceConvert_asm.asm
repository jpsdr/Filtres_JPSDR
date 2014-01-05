.586
.xmm
.model flat,c

.code

JPSDR_ColorSpaceConvert_YV24_SSE1 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV24_SSE1

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	mov esi,src_y

Boucle0_1:
	mov eax,w
	mov i,eax
Boucle1_1:
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
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	mov edi,dst_u
	inc dst_y
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	mov edi,dst_v
	inc dst_u
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	inc dst_v
	mov esi,src_y
	dec i
	jnz Boucle1_1
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
	jnz Boucle0_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YV24_SSE1 endp


JPSDR_ColorSpaceConvert_YV24_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV24_SSE2

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,4
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,4
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	mov esi,src_y

Boucle0_2:
	mov eax,w
	mov i,eax
Boucle1_2:
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	movzx edx,byte ptr[esi] ; edx=V ecx=U ebx=Y
	mov esi,lookup
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
	mov esi,src_y
	movzx ebx,byte ptr[esi+1]
	mov esi,src_u
	add src_y,2
	movzx ecx,byte ptr[esi+1]
	mov esi,src_v
	add src_u,2
	movzx edx,byte ptr[esi+1] ; edx=V ecx=U ebx=Y
	mov esi,lookup
	add src_v,2
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
	pinsrw xmm0,eax,6
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	pextrw eax,xmm0,4
	mov byte ptr[edi+1],al
	mov edi,dst_u
	add dst_y,2
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	pextrw eax,xmm0,5
	mov byte ptr[edi+1],al
	mov edi,dst_v
	add dst_u,2
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	pextrw eax,xmm0,6
	mov byte ptr[edi+1],al
	add dst_v,2
	mov esi,src_y
	dec i
	jnz Boucle1_2
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

JPSDR_ColorSpaceConvert_YV24_SSE2 endp



JPSDR_ColorSpaceConvert_YUY2_SSE1 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YUY2_SSE1

	local i,j:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	mov esi,src
	mov edi,dst

Boucle0_3:
	mov eax,w
	mov i,eax
Boucle1_3:
	movzx ebx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx eax,byte ptr[esi+2]
	movzx edx,byte ptr[esi+3] ; edx=V ecx=U ebx=Y1 j=Y2
	mov j,eax
	mov esi,lookup
	add src,4
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]	
	add ax,word ptr[esi+2*ecx+1536]	
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]	
	pinsrw xmm0,eax,3
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	pextrw eax,xmm0,1
	mov byte ptr[edi+1],al
	pextrw eax,xmm0,3
	mov byte ptr[edi+2],al
	pextrw eax,xmm0,2
	mov byte ptr[edi+3],al
	mov esi,src
	add edi,4
	dec i
	jnz Boucle1_3
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_3

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YUY2_SSE1 endp


JPSDR_ColorSpaceConvert_YUY2_SSE2 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YUY2_SSE2

	local i,j:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	mov esi,src
	mov edi,dst

Boucle0_4:
	mov eax,w
	mov i,eax
Boucle1_4:
	movzx ebx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx eax,byte ptr[esi+2]
	movzx edx,byte ptr[esi+3] ; edx=V ecx=U ebx=Y1 j=Y2
	mov j,eax
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]	
	add ax,word ptr[esi+2*ecx+1536]	
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]		
	pinsrw xmm0,eax,3

	mov esi,src
	movzx ebx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx eax,byte ptr[esi+6]
	movzx edx,byte ptr[esi+7] ; edx=V ecx=U ebx=Y1 j=Y2
	mov j,eax
	mov esi,lookup
	add src,8
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]	
	add ax,word ptr[esi+2*ecx+1536]	
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]		
	pinsrw xmm0,eax,7
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	pextrw eax,xmm0,1
	mov byte ptr[edi+1],al
	pextrw eax,xmm0,3
	mov byte ptr[edi+2],al
	pextrw eax,xmm0,2
	mov byte ptr[edi+3],al
	pextrw eax,xmm0,4
	mov byte ptr[edi+4],al
	pextrw eax,xmm0,5
	mov byte ptr[edi+5],al
	pextrw eax,xmm0,7
	mov byte ptr[edi+6],al
	pextrw eax,xmm0,6
	mov byte ptr[edi+7],al
	mov esi,src
	add edi,8
	dec i
	jnz Boucle1_4
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_4

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YUY2_SSE2 endp


JPSDR_ColorSpaceConvert_UYVY_SSE1 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_UYVY_SSE1

	local i,j:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	mov esi,src
	mov edi,dst

Boucle0_5:
	mov eax,w
	mov i,eax
Boucle1_5:
	movzx ecx,byte ptr[esi]
	movzx ebx,byte ptr[esi+1]
	movzx edx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3] ; edx=V ecx=U ebx=Y1 j=Y2
	mov esi,lookup
	mov j,eax
	add src,4
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]	
	add ax,word ptr[esi+2*ecx+1536]	
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]	
	pinsrw xmm0,eax,3
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	pextrw eax,xmm0,0
	mov byte ptr[edi+1],al
	pextrw eax,xmm0,2
	mov byte ptr[edi+2],al
	pextrw eax,xmm0,3
	mov byte ptr[edi+3],al
	mov esi,src
	add edi,4
	dec i
	jnz Boucle1_5
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_5

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_UYVY_SSE1 endp


JPSDR_ColorSpaceConvert_UYVY_SSE2 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_UYVY_SSE2

	local i,j:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	mov esi,src
	mov edi,dst

Boucle0_6:
	mov eax,w
	mov i,eax
Boucle1_6:
	movzx ecx,byte ptr[esi]
	movzx ebx,byte ptr[esi+1]
	movzx edx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3] ; edx=V ecx=U ebx=Y1 j=Y2
	mov esi,lookup
	mov j,eax
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]	
	add ax,word ptr[esi+2*ecx+1536]	
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]		
	pinsrw xmm0,eax,3

	mov esi,src
	movzx ecx,byte ptr[esi+4]
	movzx ebx,byte ptr[esi+5]
	movzx edx,byte ptr[esi+6]
	movzx eax,byte ptr[esi+7] ; edx=V ecx=U ebx=Y1 j=Y2
	mov esi,lookup
	mov j,eax
	add src,8
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]	
	add ax,word ptr[esi+2*ecx+1536]	
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]		
	pinsrw xmm0,eax,7
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	pextrw eax,xmm0,0
	mov byte ptr[edi+1],al
	pextrw eax,xmm0,2
	mov byte ptr[edi+2],al
	pextrw eax,xmm0,3
	mov byte ptr[edi+3],al
	pextrw eax,xmm0,5
	mov byte ptr[edi+4],al
	pextrw eax,xmm0,4
	mov byte ptr[edi+5],al
	pextrw eax,xmm0,6
	mov byte ptr[edi+6],al
	pextrw eax,xmm0,7
	mov byte ptr[edi+7],al
	mov esi,src
	add edi,8
	dec i
	jnz Boucle1_6
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_6

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_UYVY_SSE2 endp



JPSDR_ColorSpaceConvert_YV16_SSE1 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV16_SSE1

	local i,j:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	mov esi,src_y

Boucle0_7:
	mov eax,w
	mov i,eax
Boucle1_7:
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]
	mov esi,src_u
	mov j,eax
	add src_y,2
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	inc src_u
	movzx edx,byte ptr[esi] ; edx=V ecx=U ebx=Y1 j=Y2
	mov esi,lookup
	inc src_v
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+1536]
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	pextrw eax,xmm0,3
	mov byte ptr[edi+1],al
	mov edi,dst_u
	add dst_y,2
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	mov edi,dst_v
	inc dst_u
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	inc dst_v
	mov esi,src_y
	dec i
	jnz Boucle1_7
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
	jnz Boucle0_7

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YV16_SSE1 endp


JPSDR_ColorSpaceConvert_YV16_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_ColorSpaceConvert_YV16_SSE2

	local i,j:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	mov esi,src_y

Boucle0_8:
	mov eax,w
	mov i,eax
Boucle1_8:
	movzx ebx,byte ptr[esi]
	movzx eax,byte ptr[esi+1]
	mov esi,src_u
	mov j,eax
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	movzx edx,byte ptr[esi] ; edx=V ecx=U ebx=Y1 j=Y2
	mov esi,lookup
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,0
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+1536]
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	mov esi,src_y
	movzx ebx,byte ptr[esi+2]
	movzx eax,byte ptr[esi+3]
	mov esi,src_u
	mov j,eax
	add src_y,4
	movzx ecx,byte ptr[esi+1]
	mov esi,src_v
	add src_u,2
	movzx edx,byte ptr[esi+1] ; edx=V ecx=U ebx=Y1 j=Y2
	mov esi,lookup
	add src_v,2
			
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,4
	push ecx
	mov ecx,j
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3072]
	sar ax,1
	mov j,eax
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+1536]
	sar ax,1
	mov ebx,ecx
	pop ecx
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	mov eax,j
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	pextrw eax,xmm0,3
	mov byte ptr[edi+1],al
	pextrw eax,xmm0,4
	mov byte ptr[edi+2],al
	pextrw eax,xmm0,7
	mov byte ptr[edi+3],al
	mov edi,dst_u
	add dst_y,4
	pextrw eax,xmm0,1
	mov byte ptr[edi],al
	pextrw eax,xmm0,5
	mov byte ptr[edi+1],al
	mov edi,dst_v
	add dst_u,2
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	pextrw eax,xmm0,6
	mov byte ptr[edi+1],al
	add dst_v,2
	mov esi,src_y
	dec i
	jnz Boucle1_8
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
	jnz Boucle0_8

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_ColorSpaceConvert_YV16_SSE2 endp


end
