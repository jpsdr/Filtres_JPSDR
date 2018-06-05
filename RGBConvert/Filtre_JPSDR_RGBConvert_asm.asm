.586
.xmm
.model flat,c

.code

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2 proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2
	
	push esi
	push edi
	push ebx
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w	
	mov ebx,16
	
Convert_Planar420_to_Planar422_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	movdqa xmm1,XMMWORD ptr[edx+eax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar420_to_Planar422_8_SSE2_1
	

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2 endp


JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX
	
	push esi
	push edi
	push ebx
	
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	xor eax,eax	
	mov ecx,w	
	mov ebx,16
	
Convert_Planar420_to_Planar422_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vmovdqa xmm1,XMMWORD ptr[edx+eax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgb xmm2,xmm2,xmm1
	vpxor xmm2,xmm2,xmm3
	vpavgb xmm2,xmm2,xmm0
	
	vmovdqa XMMWORD ptr[edi+eax],xmm2
	add eax,ebx
	loop Convert_Planar420_to_Planar422_8_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX endp


JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2 proc src:dword,dst:dword,w:dword

	public JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2
	
	push edi
	push ebx
	
	mov edx,src
	mov edi,dst
	xor eax,eax	
	mov ecx,w	
	mov ebx,16
	
Convert_Planar422_to_Planar444_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[edx+eax]
	movdqu xmm1,XMMWORD ptr[edx+eax+1]
	movdqa xmm2,xmm0
	pavgb xmm1,xmm0
	movdqa xmm3,xmm0
	punpcklbw xmm2,xmm1
	punpckhbw xmm3,xmm1	
	
	movdqa XMMWORD ptr[edi+2*eax],xmm2
	movdqa XMMWORD ptr[edi+2*eax+16],xmm3
	add eax,ebx
	loop Convert_Planar422_to_Planar444_8_SSE2_1
	
	pop ebx
	pop edi

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2 endp


JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX proc src:dword,dst:dword,w:dword

	public JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX
	
	push edi
	push ebx
	
	mov edx,src
	mov edi,dst
	xor eax,eax	
	mov ecx,w	
	mov ebx,16
	
Convert_Planar422_to_Planar444_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[edx+eax]
	vmovdqu xmm1,XMMWORD ptr[edx+eax+1]
	vpavgb xmm1,xmm1,xmm0
	vpunpcklbw xmm2,xmm0,xmm1
	vpunpckhbw xmm3,xmm0,xmm1
	
	vmovdqa XMMWORD ptr[edi+2*eax],xmm2
	vmovdqa XMMWORD ptr[edi+2*eax+16],xmm3
	add eax,ebx
	loop Convert_Planar422_to_Planar444_8_AVX_1
	
	pop ebx
	pop edi

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX endp


JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8 proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src
	mov ebx,src_modulo
	xor edx,edx
	xor eax,eax

Convert_Planar444_to_Planar422_8_1:
	mov ecx,w
	
Convert_Planar444_to_Planar422_8_2:
	lodsb
	mov dl,al
	lodsb
	add ax,dx
	shr ax,1
	stosb
	loop Convert_Planar444_to_Planar422_8_2
	
	add esi,ebx
	add edi,dst_modulo
	dec h
	jnz short Convert_Planar444_to_Planar422_8_1


	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8 endp


JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2 proc src:dword,dst:dword,w16:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src
	mov ebx,16
	mov edx,esi
	add edx,ebx

Convert_Planar444_to_Planar422_8_SSE2_1:
	mov ecx,w16
	xor eax,eax	
	shr ecx,1
	jz short Convert_Planar444_to_Planar422_8_SSE2_3
	
Convert_Planar444_to_Planar422_8_SSE2_2:
	movdqa xmm0,XMMWORD ptr[esi+2*eax]
	movdqa xmm2,XMMWORD ptr[edx+2*eax]
	movdqa xmm1,xmm0
	movdqa xmm3,xmm2
	psllw xmm1,8
	psllw xmm3,8
	pavgb xmm0,xmm1
	pavgb xmm2,xmm3
	psrlw xmm0,8
	psrlw xmm2,8
	packuswb xmm0,xmm2
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar444_to_Planar422_8_SSE2_2
	
Convert_Planar444_to_Planar422_8_SSE2_3:	
	test w16,1
	jz short Convert_Planar444_to_Planar422_8_SSE2_4
	
	movdqa xmm0,XMMWORD ptr[esi+2*eax]
	movdqa xmm1,xmm0
	psllw xmm1,8
	pavgb xmm0,xmm1
	psrlw xmm0,8
	packuswb xmm0,xmm0
	
	movq qword ptr[edi+eax],xmm0

Convert_Planar444_to_Planar422_8_SSE2_4:	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short Convert_Planar444_to_Planar422_8_SSE2_1


	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2 endp


JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX proc src:dword,dst:dword,w16:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src
	mov ebx,16
	mov edx,esi
	add edx,ebx

Convert_Planar444_to_Planar422_8_AVX_1:
	mov ecx,w16
	xor eax,eax	
	shr ecx,1
	jz short Convert_Planar444_to_Planar422_8_AVX_3
	
Convert_Planar444_to_Planar422_8_AVX_2:
	vmovdqa xmm0,XMMWORD ptr[esi+2*eax]
	vmovdqa xmm2,XMMWORD ptr[edx+2*eax]
	vpsllw xmm1,xmm0,8
	vpsllw xmm3,xmm2,8
	vpavgb xmm0,xmm0,xmm1
	vpavgb xmm2,xmm2,xmm3
	vpsrlw xmm0,xmm0,8
	vpsrlw xmm2,xmm2,8
	vpackuswb xmm0,xmm0,xmm2
	
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar444_to_Planar422_8_AVX_2
	
Convert_Planar444_to_Planar422_8_AVX_3:	
	test w16,1
	jz short Convert_Planar444_to_Planar422_8_AVX_4
	
	vmovdqa xmm0,XMMWORD ptr[esi+2*eax]
	vpsllw xmm1,xmm0,8
	vpavgb xmm0,xmm0,xmm1
	vpsrlw xmm0,xmm0,8
	vpackuswb xmm0,xmm0,xmm0
	
	vmovq qword ptr[edi+eax],xmm0

Convert_Planar444_to_Planar422_8_AVX_4:	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short Convert_Planar444_to_Planar422_8_AVX_1


	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX endp


JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2 proc src1:dword,src2:dword,dst:dword,w16:dword,h:dword,src_pitch2:dword,dst_pitch:dword

	public JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ebx,16
	
Convert_Planar422_to_Planar420_8_SSE2_1:
	xor eax,eax
	mov ecx,w16

Convert_Planar422_to_Planar420_8_SSE2_2:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar422_to_Planar420_8_SSE2_2
	
	add esi,src_pitch2
	add edx,src_pitch2
	add edi,dst_pitch
	dec h
	jnz short Convert_Planar422_to_Planar420_8_SSE2_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2 endp


JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX proc src1:dword,src2:dword,dst:dword,w16:dword,h:dword,src_pitch2:dword,dst_pitch:dword

	public JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX
	
	push esi
	push edi
	push ebx
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ebx,16
	
Convert_Planar422_to_Planar420_8_AVX_1:
	xor eax,eax
	mov ecx,w16

Convert_Planar422_to_Planar420_8_AVX_2:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[edx+eax]
	
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop Convert_Planar422_to_Planar420_8_AVX_2
	
	add esi,src_pitch2
	add edx,src_pitch2
	add edi,dst_pitch
	dec h
	jnz short Convert_Planar422_to_Planar420_8_AVX_1
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX endp


JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2

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

Convert_RGB32toYV24_SSE2_1:
	mov eax,w0
	or eax,eax
	jz Convert_RGB32toYV24_SSE2_3
	
	mov i,eax
Convert_RGB32toYV24_SSE2_2:
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
	jnz Convert_RGB32toYV24_SSE2_2
	
Convert_RGB32toYV24_SSE2_3:	
	test w,1
	jz Convert_RGB32toYV24_SSE2_4
	
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
	packuswb xmm0,xmm0
	
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
	
Convert_RGB32toYV24_SSE2_4:	
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
	jnz Convert_RGB32toYV24_SSE2_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2 endp


JPSDR_RGBConvert_Convert_RGB32toYV24_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
	offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword,
	Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_Convert_RGB32toYV24_AVX

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

Convert_RGB32toYV24_AVX_1:
	mov eax,w0
	or eax,eax
	jz Convert_RGB32toYV24_AVX_3
	
	mov i,eax
Convert_RGB32toYV24_AVX_2:
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
	jnz Convert_RGB32toYV24_AVX_2
	
Convert_RGB32toYV24_AVX_3:	
	test w,1
	jz Convert_RGB32toYV24_AVX_4
	
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
	vpackuswb xmm0,xmm0,xmm0
	
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
	
Convert_RGB32toYV24_AVX_4:	
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
	jnz Convert_RGB32toYV24_AVX_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_RGB32toYV24_AVX endp


JPSDR_RGBConvert_RGB32toYUYV_SSE2 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,
	lookup:dword,src_modulo:dword,dst_modulo,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word

	public JPSDR_RGBConvert_RGB32toYUYV_SSE2

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

JPSDR_RGBConvert_RGB32toYUYV_SSE2 endp


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


JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
	offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword

	public JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2

	local i,w0:dword

	push esi
	push edi
	push ebx

	pxor xmm1,xmm1
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
	shr eax,2
	mov w0,eax
	
	mov edi,dst
	
Convert_YV24toRGB32_SSE2_1:
	mov eax,w0
	or eax,eax
	jz Convert_YV24toRGB32_SSE2_3
	
	mov i,eax
Convert_YV24toRGB32_SSE2_2:
	pxor xmm2,xmm2
	pxor xmm0,xmm0

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
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	mov esi,src_y
	pinsrw xmm0,eax,4
	
	movzx ebx,byte ptr[esi+2]
	mov esi,src_u
	movzx ecx,byte ptr[esi+2]
	mov esi,src_v
	movzx edx,byte ptr[esi+2] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	pinsrw xmm2,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	pinsrw xmm2,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	mov esi,src_y
	pinsrw xmm2,eax,0
	
	movzx ebx,byte ptr[esi+3]
	mov esi,src_u
	movzx ecx,byte ptr[esi+3]
	mov esi,src_v
	movzx edx,byte ptr[esi+3] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	add src_y,4
	pinsrw xmm2,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	add src_u,4
	pinsrw xmm2,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	add src_v,4
	pinsrw xmm2,eax,4	
	
	paddsw xmm0,xmm1
	paddsw xmm2,xmm1
	psraw xmm0,5
	psraw xmm2,5
	packuswb xmm0,xmm2
	
	movdqa XMMWORD ptr[edi],xmm0
	
	add edi,16
	
	dec i
	jnz Convert_YV24toRGB32_SSE2_2
	
Convert_YV24toRGB32_SSE2_3:
	mov eax,w
	test eax,3
	jz Convert_YV24toRGB32_SSE2_5
	
	pxor xmm0,xmm0
	
	test eax,2
	jnz short Convert_YV24toRGB32_SSE2_4

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
	packuswb xmm0,xmm0
	
	movd dword ptr[edi],xmm0
	
	add edi,4
	
	jmp Convert_YV24toRGB32_SSE2_5
	
Convert_YV24toRGB32_SSE2_4:
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
	packuswb xmm0,xmm0
	
	movq qword ptr[edi],xmm0
	
	add edi,8
	
	test w,1
	jz short Convert_YV24toRGB32_SSE2_5

	pxor xmm0,xmm0
	
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
	packuswb xmm0,xmm0
	
	movd dword ptr[edi],xmm0
	
	add edi,4
	
Convert_YV24toRGB32_SSE2_5:	
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
	jnz Convert_YV24toRGB32_SSE2_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2 endp


JPSDR_RGBConvert_Convert_YV24toRGB32_AVX proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
	offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword

	public JPSDR_RGBConvert_Convert_YV24toRGB32_AVX

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
	shr eax,2
	mov w0,eax
	
	mov edi,dst
	
Convert_YV24toRGB32_AVX_1:
	mov eax,w0
	or eax,eax
	jz Convert_YV24toRGB32_AVX_3
	
	mov i,eax
Convert_YV24toRGB32_AVX_2:
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
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	mov esi,src_y
	vpinsrw xmm0,xmm0,eax,4
	
	movzx ebx,byte ptr[esi+2]
	mov esi,src_u
	movzx ecx,byte ptr[esi+2]
	mov esi,src_v
	movzx edx,byte ptr[esi+2] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	vpinsrw xmm2,xmm2,eax,2
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	vpinsrw xmm2,xmm2,eax,1
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	mov esi,src_y
	vpinsrw xmm2,xmm2,eax,0
	
	movzx ebx,byte ptr[esi+3]
	mov esi,src_u
	movzx ecx,byte ptr[esi+3]
	mov esi,src_v
	movzx edx,byte ptr[esi+3] ; ebx=Y ecx=U edx=V
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*edx+512]
	add src_y,4
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+1024]
	add ax,word ptr[esi+2*edx+1536]
	add src_u,4
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+2048]
	add src_v,4
	vpinsrw xmm2,xmm2,eax,4	
	
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm2,xmm2,xmm1
	vpsraw xmm0,xmm0,5
	vpsraw xmm2,xmm2,5
	vpackuswb xmm3,xmm0,xmm2
	
	vmovdqa XMMWORD ptr[edi],xmm3
	
	add edi,16
	
	dec i
	jnz Convert_YV24toRGB32_AVX_2
	
Convert_YV24toRGB32_AVX_3:
	mov eax,w
	test eax,3
	jz Convert_YV24toRGB32_AVX_5
	
	test eax,2
	jnz short Convert_YV24toRGB32_AVX_4
	
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
	vpackuswb xmm3,xmm0,xmm0
	
	vmovd dword ptr[edi],xmm3
	
	add edi,4
	
	jmp Convert_YV24toRGB32_AVX_5
	
Convert_YV24toRGB32_AVX_4:
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
	vpackuswb xmm3,xmm0,xmm0
	
	vmovq qword ptr[edi],xmm3
	
	add edi,8
	
	test w,1
	jz short Convert_YV24toRGB32_AVX_5
	
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
	vpackuswb xmm3,xmm0,xmm0
	
	vmovd dword ptr[edi],xmm3
	
	add edi,4
	
Convert_YV24toRGB32_AVX_5:	
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
	jnz Convert_YV24toRGB32_AVX_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_RGBConvert_Convert_YV24toRGB32_AVX endp


end
