;
;  Saturation
;
;  Allow to change saturation, brightness and contrast.
;  Copyright (C) 2005 JPSDR
;	
;  Saturation is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  Saturation is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;   
;  You should have received a copy of the GNU General Public License
;  along with GNU Make; see the file COPYING.  If not, write to
;  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
;
;

.586
.xmm
.model flat,c

.code


JPSDR_Saturation_SSE2_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_R:word,offset_G:word,
	offset_B:word,lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_SSE2_RGB24

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_R
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6	
	movzx eax,offset_G
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_B
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4
	mov edi,dst
	mov esi,src

Boucle0_2:
	mov eax,w
	shr eax,1
	jz Suite1_2
	mov i,eax
Boucle1_2:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,0
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,4
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movq qword ptr[edi],xmm0
	add src,8
	add edi,8
	mov esi,src
	dec i
	jnz Boucle1_2
	
Suite1_2:	
	mov eax,w
	and eax,1
	jz short Suite2_2
	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	pinsrw xmm0,eax,0
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movd dword ptr[edi],xmm0
	add src,4
	add edi,4
	mov esi,src
	
Suite2_2:	
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_2

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_SSE2_RGB24 endp


JPSDR_Saturation_AVX_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_R:word,offset_G:word,
	offset_B:word,lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_AVX_RGB24

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_R
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,6	
	movzx eax,offset_G
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,5
	movzx eax,offset_B
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,4
	mov edi,dst
	mov esi,src

Boucle0_2_AVX:
	mov eax,w
	shr eax,1
	jz Suite1_2_AVX
	mov i,eax
Boucle1_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,0
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,4
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,4
	vpackuswb xmm0,xmm0,xmm2
	vmovq qword ptr[edi],xmm0
	add src,8
	add edi,8
	mov esi,src
	dec i
	jnz Boucle1_2_AVX
	
Suite1_2_AVX:	
	mov eax,w
	and eax,1
	jz short Suite2_2_AVX
	
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	vpinsrw xmm0,xmm0,eax,0
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,4
	vpackuswb xmm0,xmm0,xmm2
	vmovd dword ptr[edi],xmm0
	add src,4
	add edi,4
	mov esi,src
	
Suite2_2_AVX:	
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_2_AVX

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_AVX_RGB24 endp


JPSDR_Saturation_YUYV proc src:dword,dst:dword,w:dword,h:dword,lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_YUYV

	local i:dword

	push esi
	push edi
	push ebx

	cld
	xor eax,eax
	mov edi,dst
	mov esi,src

Boucle0_3:
	mov eax,w
	mov i,eax
Boucle1_3:
	movzx eax,byte ptr[esi]
	movzx ebx,byte ptr[esi+1]	
	movzx ecx,byte ptr[esi+2]
	movzx edx,byte ptr[esi+3] ; eax=Y1, ebx=U, ecx =Y2, edx =V
	push ax
	mov esi,lookup
	mov al,byte ptr[esi+2*ecx]
	mov ah,byte ptr[esi+2*edx+1024]
	shl eax,16
	pop cx
	mov ah,byte ptr[esi+2*ebx+512]
	mov al,byte ptr[esi+2*ecx]
	stosd
	add src,4
	mov esi,src
	dec i
	jnz short Boucle1_3
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz short Boucle0_3

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_YUYV endp


JPSDR_Saturation_UYVY proc src:dword,dst:dword,w:dword,h:dword,lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_UYVY

	local i:dword

	push esi
	push edi
	push ebx

	cld
	xor eax,eax
	mov edi,dst
	mov esi,src

Boucle0_4:
	mov eax,w
	mov i,eax
Boucle1_4:
	movzx eax,byte ptr[esi]	
	movzx ebx,byte ptr[esi+1]
	movzx ecx,byte ptr[esi+2]
	movzx edx,byte ptr[esi+3] ; eax=U, ebx=Y1, ecx =V, edx =Y2
	push ax
	mov esi,lookup
	mov al,byte ptr[esi+2*ecx+1024]
	mov ah,byte ptr[esi+2*edx]
	shl eax,16
	pop cx
	mov ah,byte ptr[esi+2*ebx]
	mov al,byte ptr[esi+2*ecx+512]
	stosd
	add src,4
	mov esi,src
	dec i
	jnz short Boucle1_4
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz short Boucle0_4

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_UYVY endp



JPSDR_Saturation_Planar_YUV proc src:dword,dst:dword,w:dword,h:dword,lookup:dword,
	src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Planar_YUV
	
	push esi
	push edi
	push ebx

	cld
	xor eax,eax
	mov ebx,w
	mov edx,lookup
	mov edi,dst
	mov esi,src

Boucle0_5:
	mov ecx,ebx
Boucle1_5:	
	lodsb
	movzx eax,byte ptr[edx+2*eax]
	stosb
	loop Boucle1_5
	add esi,src_modulo
	add edi,dst_modulo
	dec h
	jnz short Boucle0_5

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Planar_YUV endp


JPSDR_Saturation_Hue_Planar_YUV proc src_cb:dword,src_cr:dword,dst_cb:dword,dst_cr:dword,
	w:dword,h:dword,lookup_cbcr:dword,src_cb_modulo:dword,src_cr_modulo:dword,
	dst_cb_modulo:dword,dst_cr_modulo:dword

	public JPSDR_Saturation_Hue_Planar_YUV
	
	local w1:dword
	
	push esi
	push edi
	push ebx
	
	mov esi,src_cb
	mov ebx,src_cr
	mov ecx,lookup_cbcr

Boucle_hue0:
	mov eax,w
	mov w1,eax
Boucle_hue1:
	mov edi,dst_cb
	movzx eax,byte ptr[ebx]
	mov ah,byte ptr[esi]
	inc ebx
	mov dx,word ptr[ecx+2*eax]
	inc esi
	mov byte ptr[edi],dh
	inc dst_cb
	mov edi,dst_cr
	inc dst_cr
	mov byte ptr[edi],dl
	dec w1
	jnz short Boucle_hue1
	mov edi,dst_cb
	mov edx,dst_cr
	add edi,dst_cb_modulo
	add edx,dst_cr_modulo
	mov dst_cb,edi
	mov dst_cr,edx
	add esi,src_cb_modulo
	add ebx,src_cr_modulo
	dec h
	jnz short Boucle_hue0
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Hue_Planar_YUV endp



JPSDR_Saturation_Y_YUYV proc src:dword,dst:dword,w:dword,h:dword,offset_U:byte,offset_V:byte,
	lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Y_YUYV
	
	local i:dword

	push esi
	push edi
	push ebx

	cld
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	mov edi,dst
	mov esi,src
	mov ch,offset_U
	mov dh,offset_V

BoucleY_0_3:
	mov eax,w
	mov i,eax
BoucleY_1_3:
	movzx eax,byte ptr[esi]
	movzx ebx,byte ptr[esi+2] ; eax=Y1, ebx=Y2
	push ax
	mov esi,lookup
	mov ah,dh
	mov al,byte ptr[esi+2*ebx]
	shl eax,16
	pop bx
	mov ah,ch
	mov al,byte ptr[esi+2*ebx]
	stosd
	add src,4
	mov esi,src
	dec i
	jnz short BoucleY_1_3
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz short BoucleY_0_3

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Y_YUYV endp


JPSDR_Saturation_Y_UYVY proc src:dword,dst:dword,w:dword,h:dword,offset_U:byte,offset_V:byte,
	lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Y_UYVY
	
	local i:dword

	push esi
	push edi
	push ebx

	cld
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	mov edi,dst
	mov esi,src
	mov cl,offset_U
	mov dl,offset_V

BoucleY_0_4:
	mov eax,w
	mov i,eax
BoucleY_1_4:
	movzx eax,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+3] ; eax=Y1, ebx=Y2
	push ax
	mov esi,lookup
	mov al,dl
	mov ah,byte ptr[esi+2*ebx]
	shl eax,16
	pop bx
	mov al,cl
	mov ah,byte ptr[esi+2*ebx]
	stosd
	add src,4
	mov esi,src
	dec i
	jnz short BoucleY_1_4
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz short BoucleY_0_4

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Y_UYVY endp



JPSDR_Saturation_Y_SSE2_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,lookup:dword,
	src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Y_SSE2_RGB24

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,4		
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,0	
	mov edi,dst
	mov esi,src

BoucleY_0_2:
	mov eax,w
	shr eax,1
	jz SuiteY1_2
	mov i,eax
BoucleY_1_2:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	pinsrw xmm0,eax,1
	pinsrw xmm0,eax,0
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,6
	pinsrw xmm0,eax,5
	pinsrw xmm0,eax,4
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movq qword ptr[edi],xmm0
	add src,8
	add edi,8
	mov esi,src
	dec i
	jnz BoucleY_1_2
	
SuiteY1_2:
	mov eax,w
	and eax,1
	jz short SuiteY2_2

	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	pinsrw xmm0,eax,2
	pinsrw xmm0,eax,1
	pinsrw xmm0,eax,0
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movd dword ptr[edi],xmm0
	add src,4
	add edi,4
	mov esi,src
	
SuiteY2_2:	
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz BoucleY_0_2

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Y_SSE2_RGB24 endp


JPSDR_Saturation_Y_AVX_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,lookup:dword,
	src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Y_AVX_RGB24

	local i:dword

	push esi
	push edi
	push ebx

	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,6
	vpinsrw xmm1,xmm1,eax,5
	vpinsrw xmm1,xmm1,eax,4		
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,0	
	mov edi,dst
	mov esi,src

BoucleY_0_2_AVX:
	mov eax,w
	shr eax,1
	jz SuiteY1_2_AVX
	mov i,eax
BoucleY_1_2_AVX:
	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	vpinsrw xmm0,xmm0,eax,1
	vpinsrw xmm0,xmm0,eax,0
	mov esi,src
	movzx edx,byte ptr[esi+4]
	movzx ecx,byte ptr[esi+5]
	movzx ebx,byte ptr[esi+6] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,6
	vpinsrw xmm0,xmm0,eax,5
	vpinsrw xmm0,xmm0,eax,4
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,4
	vpackuswb xmm0,xmm0,xmm2
	vmovq qword ptr[edi],xmm0
	add src,8
	add edi,8
	mov esi,src
	dec i
	jnz BoucleY_1_2_AVX
	
SuiteY1_2_AVX:
	mov eax,w
	and eax,1
	jz short SuiteY2_2_AVX

	movzx edx,byte ptr[esi]
	movzx ecx,byte ptr[esi+1]
	movzx ebx,byte ptr[esi+2] ; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	vpinsrw xmm0,xmm0,eax,2
	vpinsrw xmm0,xmm0,eax,1
	vpinsrw xmm0,xmm0,eax,0
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,4
	vpackuswb xmm0,xmm0,xmm2
	vmovd dword ptr[edi],xmm0
	add src,4
	add edi,4
	mov esi,src
	
SuiteY2_2_AVX:	
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz BoucleY_0_2_AVX

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Y_AVX_RGB24 endp


JPSDR_Saturation_Non_SSE_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_R:word,offset_G:word,
	offset_B:word,lookup:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Non_SSE_RGB24

	local i:dword

	push esi
	push edi
	push ebx

	mov esi,src
	mov edi,dst

Boucle0_2n:
	mov eax,w
	mov i,eax
Boucle_b:
	mov eax,dword ptr[esi]
	mov ebx,eax
	mov ecx,eax
	mov edx,eax
	shr ebx,16
	mov eax,0FFh
	shr ecx,8
	and ebx,eax
	and ecx,eax
	and edx,eax			; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add ax,offset_R
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_r_1_b
	xor ax,ax
	jmp short stk_r_b
suite_r_1_b:
	cmp ax,255
	jbe short stk_r_b
	mov ax,255
stk_r_b:
	shl eax,16
	mov dword ptr[edi],eax
	movzx eax,word ptr[esi+2*ebx+1536]
	add ax,word ptr[esi+2*ecx+2048]
	add ax,word ptr[esi+2*edx+2560]
	add ax,offset_G
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_g_1_b
	xor ax,ax
	jmp short stk_g_b
suite_g_1_b:
	cmp ax,255
	jbe short stk_g_b
	mov ax,255
stk_g_b:
	shl ax,8
	or dword ptr[edi],eax
	movzx eax,word ptr[esi+2*ebx+3072]
	add ax,word ptr[esi+2*ecx+3584]
	add ax,word ptr[esi+2*edx+4096]
	add ax,offset_B
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_b_1_b
	xor ax,ax
	jmp short stk_b_b
suite_b_1_b:
	cmp ax,255
	jbe short stk_b_b
	mov ax,255
stk_b_b:
	or dword ptr[edi],eax
	add src,4
	add edi,4
	mov esi,src
	dec i
	jnz Boucle_b
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz Boucle0_2n

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Non_SSE_RGB24 endp


JPSDR_Saturation_Y_Non_SSE_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,lookup:dword,
	src_modulo:dword,dst_modulo:dword

	public JPSDR_Saturation_Y_Non_SSE_RGB24

	local i:dword

	push esi
	push edi
	push ebx

	mov esi,src
	mov edi,dst

BoucleY_0_2n:
	mov eax,w
	mov i,eax
BoucleY_b:
	mov eax,dword ptr[esi]
	mov ebx,eax
	mov ecx,eax
	mov edx,eax
	shr ebx,16
	mov eax,0FFh
	shr ecx,8
	and ebx,eax
	and ecx,eax
	and edx,eax			; ebx=R ecx=G edx=B
	mov esi,lookup
	movzx eax,word ptr[esi+2*ebx]
	add ax,word ptr[esi+2*ecx+512]
	add ax,word ptr[esi+2*edx+1024]
	add ax,offset_Y
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_Y_1_b
	xor ax,ax
	jmp short stk_Y_b
suite_Y_1_b:
	cmp ax,255
	jbe short stk_Y_b
	mov ax,255
stk_Y_b:
	mov ebx,eax
	mov ecx,eax
	shl ebx,16
	shl ecx,8
	or eax,ebx
	or eax,ecx
	mov dword ptr[edi],eax
	add src,4
	add edi,4
	mov esi,src
	dec i
	jnz short BoucleY_b
	add esi,src_modulo
	add edi,dst_modulo
	mov src,esi
	dec h
	jnz BoucleY_0_2n

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Saturation_Y_Non_SSE_RGB24 endp

end
