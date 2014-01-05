.586
.xmm
.model flat,c

.code

JPSDR_RGBConvert_RGB32toYV24_SSE1 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV24_SSE1

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
	mov esi,src

Boucle0_1:
	mov eax,w
	mov i,eax
Boucle1_1:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
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
	add src,4
	mov esi,src
	dec i
	jnz Boucle1_1
	add esi,src_modulo
	mov eax,dst_y
	add eax,dst_modulo_y
	mov dst_y,eax
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	mov src,esi
	dec h
	jnz Boucle0_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV24_SSE1 endp


JPSDR_RGBConvert_RGB32toYV24_SSE2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV24_SSE2

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
	
	mov esi,src

Boucle0_2:
	mov eax,w
	mov i,eax
Boucle1_2:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
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
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
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
	add src,8
	mov esi,src
	dec i
	jnz Boucle1_2
	add esi,src_modulo
	mov eax,dst_y
	add eax,dst_modulo_y
	mov dst_y,eax
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	mov src,esi
	dec h
	jnz Boucle0_2

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV24_SSE2 endp


JPSDR_RGBConvert_YV24toRGB32_SSE1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
	offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword

	public JPSDR_RGBConvert_YV24toRGB32_SSE1

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_B
	pinsrw xmm1,eax,0
	movzx eax,offset_G
	pinsrw xmm1,eax,1
	movzx eax,offset_R
	pinsrw xmm1,eax,2
	mov edi,dst

Boucle0_3:
	mov eax,w
	mov i,eax
Boucle1_3:
	mov esi,src_y
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	inc src_y
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	inc src_u
	movzx edx,byte ptr[esi] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	inc src_v
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	pinsrw xmm0,eax,0
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm2
	movd dword ptr[edi],xmm0
	
	add edi,4
	dec i
	jnz short Boucle1_3
	add edi,dst_modulo
	mov eax,src_y
	add eax,src_modulo_y
	mov src_y,eax
	mov eax,src_u
	add eax,src_modulo_u
	mov src_u,eax
	mov eax,src_v
	add eax,src_modulo_v
	mov src_v,eax
	dec h
	jnz Boucle0_3

	pop ebx
	pop edi
	pop esi
	
	ret

JPSDR_RGBConvert_YV24toRGB32_SSE1 endp


JPSDR_RGBConvert_YV24toRGB32_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
	offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword

	public JPSDR_RGBConvert_YV24toRGB32_SSE2

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_B
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4	
	movzx eax,offset_G
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_R
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	mov edi,dst

Boucle0_4:
	mov eax,w
	mov i,eax
Boucle1_4:
	mov esi,src_y
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	movzx edx,byte ptr[esi] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	pinsrw xmm0,eax,0
	
	mov esi,src_y
	movzx ebx,byte ptr[esi+1]
	mov esi,src_u
	add src_y,2
	movzx ecx,byte ptr[esi+1]
	mov esi,src_v
	add src_u,2
	movzx edx,byte ptr[esi+1] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	add src_v,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	pinsrw xmm0,eax,4
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm2
	movq qword ptr[edi],xmm0
	
	add edi,8
	dec i
	jnz Boucle1_4
	add edi,dst_modulo
	mov eax,src_y
	add eax,src_modulo_y
	mov src_y,eax
	mov eax,src_u
	add eax,src_modulo_u
	mov src_u,eax
	mov eax,src_v
	add eax,src_modulo_v
	mov src_v,eax
	dec h
	jnz Boucle0_4

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_YV24toRGB32_SSE2 endp



end
