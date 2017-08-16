.586
.xmm
.model flat,c

.code

JPSDR_RGBConvert_RGB32toYV24_SSE proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV24_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
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
	
	mov esi,src

Boucle0_2:
	mov eax,w0
	or eax,eax
	jz Suite0_2
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
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,4
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
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
	add src,8
	mov word ptr[edi],ax
	mov edi,dst_u
	pextrw eax,xmm0,1
	add dst_y,2
	mov word ptr[edi],ax
	mov edi,dst_v
	pextrw eax,xmm0,2
	add dst_u,2
	mov word ptr[edi],ax
	add dst_v,2
	
	mov esi,src
	
	dec i
	jnz Boucle1_2
	
Suite0_2:	
	mov eax,w
	and eax,1
	jz Suite1_2
	
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
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,4
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	mov edi,dst_u
	pextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	mov edi,dst_v
	pextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src	
	
Suite1_2:	
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

JPSDR_RGBConvert_RGB32toYV24_SSE endp


JPSDR_RGBConvert_RGB32toYV24_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV24_AVX

	local i,w0:dword

	push esi
	push edi
	push ebx

	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1	
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5

	mov eax,w
	shr eax,1
	mov w0,eax
	
	mov esi,src

Boucle0_2_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_2_AVX
	
	mov i,eax
Boucle1_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,4
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,3
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,5
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,8
	mov word ptr[edi],ax
	mov edi,dst_u
	vpextrw eax,xmm0,1
	add dst_y,2
	mov word ptr[edi],ax
	mov edi,dst_v
	vpextrw eax,xmm0,2
	add dst_u,2
	mov word ptr[edi],ax
	add dst_v,2
	
	mov esi,src
	
	dec i
	jnz Boucle1_2_AVX
	
Suite0_2_AVX:	
	mov eax,w
	and eax,1
	jz Suite1_2_AVX
	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,4
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	mov edi,dst_u
	vpextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	mov edi,dst_v
	vpextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src	
	
Suite1_2_AVX:	
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
	jnz Boucle0_2_AVX

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV24_AVX endp


JPSDR_RGBConvert_RGB32toYV16_SSE proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV16_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
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
	shr eax,2
	mov w0,eax
	
	mov esi,src

Boucle0_6:
	mov eax,w0
	or eax,eax
	jz Suite0_6
	mov i,eax
Boucle1_6:
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	pinsrw xmm0,eax,1
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
	mov esi,lookup
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
	mov esi,src
	pinsrw xmm0,eax,7
	
	movzx edx,byte ptr[esi+12]
	movzx ecx,byte ptr[esi+13]
	movzx ebx,byte ptr[esi+14] ; ebx=R ecx=G edx=B
	mov esi,lookup
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
	add src,16
	mov word ptr[edi],ax
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	add dst_u,2
	mov word ptr[edi],ax

	mov edi,dst_y
	add dst_v,2
	movd dword ptr[edi],xmm0
	add dst_y,4

	mov esi,src
	
	dec i	
	jnz Boucle1_6
	
Suite0_6:
	mov eax,w
	and eax,3
	jz Suite2_6
	and eax,2
	jz Suite1_6
	
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,1
	
	mov eax,w
	and eax,1
	jz Suite0_6b
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
	mov esi,lookup
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
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,12
	mov word ptr[edi],ax
	pextrw eax,xmm0,1
	add dst_y,3
	mov byte ptr[edi+2],al
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_u,2
	mov word ptr[edi],ax
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	add dst_v,2
	mov word ptr[edi],ax

	jmp Suite2_6
	
Suite0_6b:	
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

	add src,8
	
	jmp Suite2_6
		
Suite1_6:
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
	pinsrw xmm0,eax,4
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
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al

	add src,4
	
Suite2_6:	
	mov esi,src
	
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
	jnz Boucle0_6

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV16_SSE endp


JPSDR_RGBConvert_RGB32toYV16_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV16_AVX

	local i,w0:dword

	push esi
	push edi
	push ebx

	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5	
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6
	vpinsrw xmm3,xmm3,eax,7	

	mov eax,w
	shr eax,2
	mov w0,eax
	
	mov esi,src

Boucle0_6_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_6_AVX
	
	mov i,eax
Boucle1_6_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,1
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,7
	
	movzx edx,byte ptr[esi+12]
	movzx ecx,byte ptr[esi+13]
	movzx ebx,byte ptr[esi+14] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add src,16
	mov word ptr[edi],ax
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	add dst_u,2
	mov word ptr[edi],ax

	mov edi,dst_y
	add dst_v,2
	vmovd dword ptr[edi],xmm0
	add dst_y,4

	mov esi,src
	
	dec i	
	jnz Boucle1_6_AVX
	
Suite0_6_AVX:
	mov eax,w
	and eax,3
	jz Suite2_6_AVX
	and eax,2
	jz Suite1_6_AVX
	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	mov eax,w
	and eax,1
	jz Suite0_6b_AVX
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,7
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,12
	mov word ptr[edi],ax
	vpextrw eax,xmm0,1
	add dst_y,3
	mov byte ptr[edi+2],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_u,2
	mov word ptr[edi],ax
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	add dst_v,2
	mov word ptr[edi],ax

	jmp Suite2_6_AVX
	
Suite0_6b_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	mov edi,dst_y
	vpextrw eax,xmm0,0
	add dst_y,2
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_v
	mov byte ptr[edi],al

	add src,8
	
	jmp Suite2_6_AVX
		
Suite1_6_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3

	mov edi,dst_y
	vpextrw eax,xmm0,0
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al

	add src,4
	
Suite2_6_AVX:	
	mov esi,src
	
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
	jnz Boucle0_6_AVX

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV16_AVX endp


JPSDR_RGBConvert_RGB32toYUYV_SSE proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,
	lookup:dword,src_modulo:dword,dst_modulo,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYUYV_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx
	
	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
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
	shr eax,2
	mov w0,eax

	mov edi,dst
	mov esi,src
	
Boucle0_8:
	mov eax,w0
	or eax,eax
	jz Suite0_8
	mov i,eax
Boucle1_8:
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
	mov esi,src
	pinsrw xmm0,eax,3
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	pinsrw xmm0,eax,2
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
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
	mov esi,src
	pinsrw xmm0,eax,7
	
	movzx edx,byte ptr[esi+12]
	movzx ecx,byte ptr[esi+13]
	movzx ebx,byte ptr[esi+14] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3	
	packuswb xmm0,xmm4
	
	add src,16
	
	movq qword ptr[edi],xmm0
	
	add edi,8
	
	mov esi,src
	
	dec i	
	jnz Boucle1_8
	
Suite0_8:	
	mov eax,w
	and eax,3
	jz Suite2_8
	and eax,2
	jz Suite1_8
	
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
	mov esi,src
	pinsrw xmm0,eax,3
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2

	mov eax,w
	and eax,1
	jz Suite0_8b
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
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
	pinsrw xmm0,eax,7
	
	xor eax,eax
	pinsrw xmm0,eax,6

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	add src,12
	
	movq qword ptr[edi],xmm0
	
	add edi,8
	
	jmp Suite2_8
	
Suite0_8b:		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	add src,8
	
	movd dword ptr[edi],xmm0
	
	add edi,4
	
	jmp Suite2_8
	
Suite1_8:	
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
	pinsrw xmm0,eax,3
	
	xor eax,eax
	pinsrw xmm0,eax,2
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	add src,4
	
	movd dword ptr[edi],xmm0
	
	add edi,4
	
Suite2_8:	
	mov esi,src
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_8

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYUYV_SSE endp


JPSDR_RGBConvert_RGB32toYUYV_AVX proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,
	lookup:dword,src_modulo:dword,dst_modulo,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYUYV_AVX

	local i,w0:dword

	push esi
	push edi
	push ebx
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,5	
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,3
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,6
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,5
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,3
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,3
	vpinsrw xmm3,xmm3,eax,7
	
	mov eax,w
	shr eax,2
	mov w0,eax

	mov edi,dst
	mov esi,src
	
Boucle0_8_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_8_AVX
	
	mov i,eax
Boucle1_8_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,3
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,2
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,7
	
	movzx edx,byte ptr[esi+12]
	movzx ecx,byte ptr[esi+13]
	movzx ebx,byte ptr[esi+14] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,6
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3	
	vpackuswb xmm0,xmm0,xmm4
	
	add src,16
	
	vmovq qword ptr[edi],xmm0
	
	add edi,8
	
	mov esi,src
	
	dec i	
	jnz Boucle1_8_AVX
	
Suite0_8_AVX:	
	mov eax,w
	and eax,3
	jz Suite2_8_AVX
	and eax,2
	jz Suite1_8_AVX
	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,3
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2

	mov eax,w
	and eax,1
	jz Suite0_8b_AVX
	
	movzx edx,byte ptr[esi+8]
	movzx ecx,byte ptr[esi+9]
	movzx ebx,byte ptr[esi+10] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,7
	
	xor eax,eax
	vpinsrw xmm0,xmm0,eax,6

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	add src,12
	
	vmovq qword ptr[edi],xmm0
	
	add edi,8
	
	jmp Suite2_8_AVX
	
Suite0_8b_AVX:		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	add src,8
	
	vmovd dword ptr[edi],xmm0
	
	add edi,4
	
	jmp Suite2_8_AVX
	
Suite1_8_AVX:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,1	
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,3
	
	xor eax,eax
	vpinsrw xmm0,xmm0,eax,2
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	add src,4
	
	vmovd dword ptr[edi],xmm0
	
	add edi,4
	
Suite2_8_AVX:	
	mov esi,src
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_8_AVX

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYUYV_AVX endp


JPSDR_RGBConvert_RGB32toYV12_SSE proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV12_SSE

	local i,w0,h0:dword

	push esi
	push edi
	push ebx
	
	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,6
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
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	
	mov esi,src
	
	mov eax,w
	inc eax
	shr eax,1
	dec eax
	mov w0,eax

	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9
	mov h0,eax

Boucle0_9:
	mov eax,w0
	or eax,eax
	jz Suite0_9
	mov i,eax
Boucle1_9:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch	
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src
	
	dec i	
	jnz Boucle1_9
	
Suite0_9:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6

	mov eax,w
	and eax,1
	jnz Suite1_9a

	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	jmp Suite1_9b
	
Suite1_9a:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	pextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al
	
Suite1_9b:	
	mov esi,src
	add esi,src_modulo
	add esi,src_pitch
	
	mov eax,dst_y
	add eax,dst_modulo_y
	add eax,dst_pitch_y
	mov dst_y,eax
	
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	
	mov src,esi
	
	dec h0
	jnz Boucle0_9
	
Suite4_9:
	mov eax,h
	and eax,1
	jnz Suite2_9

	mov eax,w0
	or eax,eax
	jz Suite5_9
	mov i,eax
Boucle2_9:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src	
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src	
	add esi,src_pitch	
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;	ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,8
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
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
		
	mov esi,src
	
	dec i	
	jnz Boucle2_9
	
Suite5_9:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz Suite3_9a
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b
	
Suite3_9a:		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	mov byte ptr[edi],al
	
	jmp Suite3_9b
		
Suite2_9:	
	mov eax,w0
	or eax,eax
	jz Suite6_9	
	mov i,eax
Boucle3_9:
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
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
		
	mov esi,src
	
	dec i	
	jnz Boucle3_9
	
Suite6_9:	
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz short Suite3_9c
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
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
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b
		
Suite3_9c:			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	mov byte ptr[edi],al

	
Suite3_9b:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV12_SSE endp


JPSDR_RGBConvert_RGB32toYV12_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV12_AVX

	local i,w0,h0:dword

	push esi
	push edi
	push ebx
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6
	
	mov esi,src
	
	mov eax,w
	inc eax
	shr eax,1
	dec eax
	mov w0,eax

	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9_AVX
	mov h0,eax

Boucle0_9_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_9_AVX
	
	mov i,eax
Boucle1_9_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch	
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src
	
	dec i	
	jnz Boucle1_9_AVX
	
Suite0_9_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6

	mov eax,w
	and eax,1
	jnz Suite1_9a_AVX

	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	jmp Suite1_9b_AVX
	
Suite1_9a_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	vpextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al
	
Suite1_9b_AVX:	
	mov esi,src
	add esi,src_modulo
	add esi,src_pitch
	
	mov eax,dst_y
	add eax,dst_modulo_y
	add eax,dst_pitch_y
	mov dst_y,eax
	
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	
	mov src,esi
	
	dec h0
	jnz Boucle0_9_AVX
	
Suite4_9_AVX:
	mov eax,h
	and eax,1
	jnz Suite2_9_AVX

	mov eax,w0
	or eax,eax
	jz Suite5_9_AVX
	
	mov i,eax
Boucle2_9_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src	
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src	
	add esi,src_pitch	
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;	ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,3
			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,8
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	add dst_y,2
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_v
	mov byte ptr[edi],al
		
	mov esi,src
	
	dec i	
	jnz Boucle2_9_AVX
	
Suite5_9_AVX:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz Suite3_9a_AVX
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b_AVX
	
Suite3_9a_AVX:		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	vpextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	mov byte ptr[edi],al
	
	jmp Suite3_9b_AVX
		
Suite2_9_AVX:	
	mov eax,w0
	or eax,eax
	jz Suite6_9_AVX
	
	mov i,eax
Boucle3_9_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,1
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add dst_y,2
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_v
	mov byte ptr[edi],al
		
	mov esi,src
	
	dec i	
	jnz Boucle3_9_AVX
	
Suite6_9_AVX:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz short Suite3_9c_AVX
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b_AVX
		
Suite3_9c_AVX:			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	mov byte ptr[edi],al

	
Suite3_9b_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV12_AVX endp


JPSDR_RGBConvert_RGB32toYV12_SSE_1 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV12_SSE_1

	local i,w0,h0:dword

	push esi
	push edi
	push ebx
	
	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,6
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
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	
	mov esi,src
	
	mov eax,w
	inc eax
	shr eax,1
	dec eax
	mov w0,eax

	mov eax,h
	inc eax
	shr eax,1
	jz Suite4_9_1
	mov h0,eax

Boucle0_9_1:
	mov eax,w0
	or eax,eax
	jz Suite0_9_1
	mov i,eax
Boucle1_9_1:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch	
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src
	
	dec i	
	jnz Boucle1_9_1
	
Suite0_9_1:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6

	mov eax,w
	and eax,1
	jnz Suite1_9a_1

	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	jmp Suite1_9b_1
	
Suite1_9a_1:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	pextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al
	
Suite1_9b_1:	
	mov esi,src
	add esi,src_modulo
	add esi,src_pitch
	
	mov eax,dst_y
	add eax,dst_modulo_y
	add eax,dst_pitch_y
	mov dst_y,eax
	
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	
	mov src,esi
	
	dec h0
	jnz Boucle0_9_1
	
Suite4_9_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV12_SSE_1 endp


JPSDR_RGBConvert_RGB32toYV12_AVX_1 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV12_AVX_1

	local i,w0,h0:dword

	push esi
	push edi
	push ebx
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6
	
	mov esi,src
	
	mov eax,w
	inc eax
	shr eax,1
	dec eax
	mov w0,eax

	mov eax,h
	inc eax
	shr eax,1
	jz Suite4_9_1_AVX
	mov h0,eax

Boucle0_9_1_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_9_1_AVX
	
	mov i,eax
Boucle1_9_1_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch	
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src
	
	dec i	
	jnz Boucle1_9_1_AVX
	
Suite0_9_1_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6

	mov eax,w
	and eax,1
	jnz Suite1_9a_1_AVX

	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	jmp Suite1_9b_1_AVX
	
Suite1_9a_1_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	vpextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al
	
Suite1_9b_1_AVX:	
	mov esi,src
	add esi,src_modulo
	add esi,src_pitch
	
	mov eax,dst_y
	add eax,dst_modulo_y
	add eax,dst_pitch_y
	mov dst_y,eax
	
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	
	mov src,esi
	
	dec h0
	jnz Boucle0_9_1_AVX
	
Suite4_9_1_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV12_AVX_1 endp


JPSDR_RGBConvert_RGB32toYV12_SSE_2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV12_SSE_2

	local i,w0,h0:dword

	push esi
	push edi
	push ebx
	
	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,6
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
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	
	mov esi,src
	
	mov eax,w
	inc eax
	shr eax,1
	dec eax
	mov w0,eax

	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9_2
	mov h0,eax

Boucle0_9_2:
	mov eax,w0
	or eax,eax
	jz Suite0_9_2
	mov i,eax
Boucle1_9_2:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch	
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src
	
	dec i	
	jnz Boucle1_9_2
	
Suite0_9_2:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6

	mov eax,w
	and eax,1
	jnz Suite1_9a_2

	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	jmp Suite1_9b_2
	
Suite1_9a_2:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	pextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al
	
Suite1_9b_2:	
	mov esi,src
	add esi,src_modulo
	add esi,src_pitch
	
	mov eax,dst_y
	add eax,dst_modulo_y
	add eax,dst_pitch_y
	mov dst_y,eax
	
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	
	mov src,esi
	
	dec h0
	jnz Boucle0_9_2
	
Suite4_9_2:
	mov eax,h
	and eax,1
	jnz Suite2_9_2

	mov eax,w0
	or eax,eax
	jz Suite5_9_2
	mov i,eax
Boucle2_9_2:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src	
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src	
	add esi,src_pitch	
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;	ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	add src,8
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
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
		
	mov esi,src
	
	dec i	
	jnz Boucle2_9_2
	
Suite5_9_2:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz Suite3_9a_2
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	pextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b_2
	
Suite3_9a_2:		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	mov byte ptr[edi],al
	
	jmp Suite3_9b_2
		
Suite2_9_2:	
	mov eax,w0
	or eax,eax
	jz Suite6_9_2
	mov i,eax
Boucle3_9_2:
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
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
		
	mov esi,src
	
	dec i	
	jnz Boucle3_9_2
	
Suite6_9_2:	
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz short Suite3_9c_2
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
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
	mov word ptr[edi],ax
	
	mov edi,dst_u
	pextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b_2
		
Suite3_9c_2:			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	mov edi,dst_y
	pextrw eax,xmm0,0
	mov byte ptr[edi],al
	
	mov edi,dst_u
	pextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	pextrw eax,xmm0,6
	mov byte ptr[edi],al

	
Suite3_9b_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV12_SSE_2 endp


JPSDR_RGBConvert_RGB32toYV12_AVX_2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYV12_AVX_2

	local i,w0,h0:dword

	push esi
	push edi
	push ebx
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6
	
	mov esi,src
	
	mov eax,w
	inc eax
	shr eax,1
	dec eax
	mov w0,eax

	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9_2_AVX
	mov h0,eax

Boucle0_9_2_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_9_2_AVX
	
	mov i,eax
Boucle1_9_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch	
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	mov esi,src
	
	dec i	
	jnz Boucle1_9_2_AVX
	
Suite0_9_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6

	mov eax,w
	and eax,1
	jnz Suite1_9a_2_AVX

	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	add dst_y,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_u
	mov byte ptr[edi],al
	inc dst_v
	
	jmp Suite1_9b_2_AVX
	
Suite1_9a_2_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,4
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	vpextrw eax,xmm0,2
	inc dst_y
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	inc dst_v
	mov byte ptr[edi],al
	
Suite1_9b_2_AVX:	
	mov esi,src
	add esi,src_modulo
	add esi,src_pitch
	
	mov eax,dst_y
	add eax,dst_modulo_y
	add eax,dst_pitch_y
	mov dst_y,eax
	
	mov eax,dst_u
	add eax,dst_modulo_u
	mov dst_u,eax
	
	mov eax,dst_v
	add eax,dst_modulo_v
	mov dst_v,eax
	
	mov src,esi
	
	dec h0
	jnz Boucle0_9_2_AVX
	
Suite4_9_2_AVX:
	mov eax,h
	and eax,1
	jnz Suite2_9_2_AVX

	mov eax,w0
	or eax,eax
	jz Suite5_9_2_AVX
	
	mov i,eax
Boucle2_9_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src	
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src	
	add esi,src_pitch	
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ;	ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,3
			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add src,8
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	add dst_y,2
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_v
	mov byte ptr[edi],al
		
	mov esi,src
	
	dec i	
	jnz Boucle2_9_2_AVX
	
Suite5_9_2_AVX:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	push edx
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	push ecx
	mov esi,lookup
	push ebx
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop eax
	add ebx,eax
	shr ebx,1
	pop eax
	add ecx,eax
	shr ecx,1
	pop eax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz Suite3_9a_2_AVX
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	mov esi,src
	add esi,src_pitch
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	add edi,dst_pitch_y
	vpextrw eax,xmm0,1
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b_2_AVX
	
Suite3_9a_2_AVX:		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov byte ptr[edi],al
	add edi,dst_pitch_y
	vpextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	mov byte ptr[edi],al
	
	jmp Suite3_9b_2_AVX
		
Suite2_9_2_AVX:	
	mov eax,w0
	or eax,eax
	jz Suite6_9_2_AVX
	
	mov i,eax
Boucle3_9_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	mov esi,src
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add src,8
	vpinsrw xmm0,xmm0,eax,1
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	add dst_y,2
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	inc dst_u
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	inc dst_v
	mov byte ptr[edi],al
		
	mov esi,src
	
	dec i	
	jnz Boucle3_9_2_AVX
	
Suite6_9_2_AVX:	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,w
	and eax,1
	jnz short Suite3_9c_2_AVX
	
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B	
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov word ptr[edi],ax
	
	mov edi,dst_u
	vpextrw eax,xmm0,2
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,3
	mov byte ptr[edi],al
	
	jmp Suite3_9b_2_AVX
		
Suite3_9c_2_AVX:			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	mov edi,dst_y
	vpextrw eax,xmm0,0
	mov byte ptr[edi],al
	
	mov edi,dst_u
	vpextrw eax,xmm0,4
	mov byte ptr[edi],al
	
	mov edi,dst_v
	vpextrw eax,xmm0,6
	mov byte ptr[edi],al

	
Suite3_9b_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_RGB32toYV12_AVX_2 endp


JPSDR_RGBConvert_YV24toRGB32_SSE proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
	offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword

	public JPSDR_RGBConvert_YV24toRGB32_SSE

	local i,w0:dword

	push esi
	push edi
	push ebx

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
	
	mov eax,w
	shr eax,1
	mov w0,eax
	
	mov edi,dst
	
Boucle0_4:
	mov eax,w0
	or eax,eax
	jz Suite0_4
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
	mov esi,src_y
	pinsrw xmm0,eax,0
	
	movzx ebx,byte ptr[esi+1]
	mov esi,src_u
	movzx ecx,byte ptr[esi+1]
	mov esi,src_v
	movzx edx,byte ptr[esi+1] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	add src_y,2
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	add src_u,2
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	add src_v,2
	pinsrw xmm0,eax,4
	
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm2
	
	movq qword ptr[edi],xmm0
	
	add edi,8
	
	dec i
	jnz Boucle1_4
	
Suite0_4:
	mov eax,w
	and eax,1
	jz Suite1_4
	
	mov esi,src_y
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	movzx edx,byte ptr[esi] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	inc src_y
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	inc src_u
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	inc src_v
	pinsrw xmm0,eax,0
	
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm2
	
	movd dword ptr[edi],xmm0
	
	add edi,4
	
Suite1_4:	
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

JPSDR_RGBConvert_YV24toRGB32_SSE endp


JPSDR_RGBConvert_YV24toRGB32_AVX proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
	offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword

	public JPSDR_RGBConvert_YV24toRGB32_AVX

	local i,w0:dword

	push esi
	push edi
	push ebx

	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_B
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,4	
	movzx eax,offset_G
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,5
	movzx eax,offset_R
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,6
	
	mov eax,w
	shr eax,1
	mov w0,eax
	
	mov edi,dst
	
Boucle0_4_AVX:
	mov eax,w0
	or eax,eax
	jz Suite0_4_AVX
	
	mov i,eax
Boucle1_4_AVX:
	mov esi,src_y
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	movzx edx,byte ptr[esi] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	mov esi,src_y
	vpinsrw xmm0,xmm0,eax,0
	
	movzx ebx,byte ptr[esi+1]
	mov esi,src_u
	movzx ecx,byte ptr[esi+1]
	mov esi,src_v
	movzx edx,byte ptr[esi+1] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	add src_y,2
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	add src_u,2
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	add src_v,2
	vpinsrw xmm0,xmm0,eax,4
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,5
	vpackuswb xmm0,xmm0,xmm2
	
	vmovq qword ptr[edi],xmm0
	
	add edi,8
	
	dec i
	jnz Boucle1_4_AVX
	
Suite0_4_AVX:
	mov eax,w
	and eax,1
	jz Suite1_4_AVX
	
	mov esi,src_y
	movzx ebx,byte ptr[esi]
	mov esi,src_u
	movzx ecx,byte ptr[esi]
	mov esi,src_v
	movzx edx,byte ptr[esi] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	inc src_y
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	inc src_u
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	inc src_v
	vpinsrw xmm0,xmm0,eax,0
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,5
	vpackuswb xmm0,xmm0,xmm2
	
	vmovd dword ptr[edi],xmm0
	
	add edi,4
	
Suite1_4_AVX:	
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
	jnz Boucle0_4_AVX

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_YV24toRGB32_AVX endp


end
