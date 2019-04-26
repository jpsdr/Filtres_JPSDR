;
;  VHS_III
;
;  Temporal denoiser and chroma/luma sharpening for cleaning VHS.
;  Copyright (C) 2005 JPSDR
;	
;  VHS_III is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  VHS_III is distributed in the hope that it will be useful,
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
.mmx
.xmm
.model flat,c

.code


JPSDR_VHS_III_convert_RGB_YUV_SSE2 proc src:dword,y:dword,u:dword,v:dword,w:dword,h:dword,src_modulo:dword,
	lookup:dword

	public JPSDR_VHS_III_convert_RGB_YUV_SSE2

	push esi
	push edi
	push ebx

	mov edi,lookup
	mov edx,h
	xor ebx,ebx
	pxor xmm7,xmm7
	cld

Loop_1_1:
	mov ecx,w

Loop_1_2:
	mov esi,src

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,0
	pinsrw xmm3,eax,0
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,0
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,0
	pinsrw xmm4,eax,0

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,1
	pinsrw xmm3,eax,1
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,1
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,1
	pinsrw xmm4,eax,1

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,2
	pinsrw xmm3,eax,2
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,2
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,2
	pinsrw xmm4,eax,2

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,3
	pinsrw xmm3,eax,3
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,3
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,3
	pinsrw xmm4,eax,3

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,4
	pinsrw xmm3,eax,4
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,4
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,4
	pinsrw xmm4,eax,4

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,5
	pinsrw xmm3,eax,5
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,5
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,5
	pinsrw xmm4,eax,5

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,6
	pinsrw xmm3,eax,6
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,6
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,6
	pinsrw xmm4,eax,6

	lodsd
	movd xmm6,eax
	punpcklbw xmm6,xmm7
	pextrw eax,xmm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw xmm0,ebx,7
	pinsrw xmm3,eax,7
	pextrw eax,xmm6,1
	mov bx,[edi+2*eax+512]
	pinsrw xmm1,ebx,7
	pextrw eax,xmm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw xmm2,ebx,7
	pinsrw xmm4,eax,7

	mov src,esi

	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psubsw xmm3,xmm0
	psubsw xmm4,xmm0

	mov esi,y
	movdqa [esi],xmm0
	add esi,16
	mov y,esi
	mov esi,u
	movdqa [esi],xmm4
	add esi,16
	mov u,esi
	mov esi,v
	movdqa [esi],xmm3
	add esi,16
	mov v,esi

	dec ecx
	jnz Loop_1_2

	mov esi,src
	add esi,src_modulo
	mov src,esi

	dec edx
	jnz Loop_1_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_III_convert_RGB_YUV_SSE2 endp


JPSDR_VHS_III_convert_RGB_YUV_SSE proc src:dword,y:dword,u:dword,v:dword,w:dword,h:dword,src_modulo:dword,
	lookup:dword

	public JPSDR_VHS_III_convert_RGB_YUV_SSE

	push esi
	push edi
	push ebx

	mov edi,lookup
	mov edx,h
	xor ebx,ebx
	pxor mm7,mm7
	cld

Loop_2_1:
	mov ecx,w

Loop_2_2:
	mov esi,src

	lodsd
	movd mm6,eax
	punpcklbw mm6,mm7
	pextrw eax,mm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw mm0,ebx,0
	pinsrw mm3,eax,0
	pextrw eax,mm6,1
	mov bx,[edi+2*eax+512]
	pinsrw mm1,ebx,0
	pextrw eax,mm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw mm2,ebx,0
	pinsrw mm4,eax,0

	lodsd
	movd mm6,eax
	punpcklbw mm6,mm7
	pextrw eax,mm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw mm0,ebx,1
	pinsrw mm3,eax,1
	pextrw eax,mm6,1
	mov bx,[edi+2*eax+512]
	pinsrw mm1,ebx,1
	pextrw eax,mm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw mm2,ebx,1
	pinsrw mm4,eax,1

	lodsd
	movd mm6,eax
	punpcklbw mm6,mm7
	pextrw eax,mm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw mm0,ebx,2
	pinsrw mm3,eax,2
	pextrw eax,mm6,1
	mov bx,[edi+2*eax+512]
	pinsrw mm1,ebx,2
	pextrw eax,mm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw mm2,ebx,2
	pinsrw mm4,eax,2

	lodsd
	movd mm6,eax
	punpcklbw mm6,mm7
	pextrw eax,mm6,2
	mov bx,[edi+2*eax]
	shl eax,4
	pinsrw mm0,ebx,3
	pinsrw mm3,eax,3
	pextrw eax,mm6,1
	mov bx,[edi+2*eax+512]
	pinsrw mm1,ebx,3
	pextrw eax,mm6,0
	mov bx,[edi+2*eax+1024]
	shl eax,4
	pinsrw mm2,ebx,3
	pinsrw mm4,eax,3

	mov src,esi

	paddsw mm0,mm1
	paddsw mm0,mm2
	psubsw mm3,mm0
	psubsw mm4,mm0

	mov esi,y
	movq [esi],mm0
	add esi,8
	mov y,esi
	mov esi,u
	movq [esi],mm4
	add esi,8
	mov u,esi
	mov esi,v
	movq [esi],mm3
	add esi,8
	mov v,esi

	dec ecx
	jnz Loop_2_2

	mov esi,src
	add esi,src_modulo
	mov src,esi

	dec edx
	jnz Loop_2_1

	emms

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_III_convert_RGB_YUV_SSE endp


JPSDR_VHS_III_convert_YUV_RGB_SSE2 proc dst:dword,y:dword,u:dword,v:dword,w:dword,h:dword,dst_modulo:dword,
	lookup:dword

	public JPSDR_VHS_III_convert_YUV_RGB_SSE2

	local i:dword

	push esi
	push edi
	push ebx

	mov esi,lookup
	mov dx,8161
	mov cx,16322
	xor ebx,ebx
	pxor xmm7,xmm7
	mov eax,8
	pinsrw xmm6,eax,0
	pinsrw xmm6,eax,1
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	pinsrw xmm6,eax,4
	pinsrw xmm6,eax,5
	pinsrw xmm6,eax,6
	pinsrw xmm6,eax,7
	cld

Loop_3_1:
	mov eax,w
	mov i,eax
Loop_3_2:
	mov edi,y
	movdqa xmm0,[edi]
	add edi,16
	mov y,edi
	mov edi,v
	movdqa xmm1,[edi]
	add edi,16
	mov v,edi
	mov edi,u
	movdqa xmm2,[edi]
	add edi,16
	mov u,edi

	pextrw eax,xmm0,0
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,0
	pextrw eax,xmm1,0
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,0
	pextrw eax,xmm2,0
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,0

	pextrw eax,xmm0,1
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,1
	pextrw eax,xmm1,1
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,1
	pextrw eax,xmm2,1
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,1

	pextrw eax,xmm0,2
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,2
	pextrw eax,xmm1,2
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,2
	pextrw eax,xmm2,2
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,2

	pextrw eax,xmm0,3
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,3
	pextrw eax,xmm1,3
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,3
	pextrw eax,xmm2,3
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,3

	pextrw eax,xmm0,4
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,4
	pextrw eax,xmm1,4
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,4
	pextrw eax,xmm2,4
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,4

	pextrw eax,xmm0,5
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,5
	pextrw eax,xmm1,5
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,5
	pextrw eax,xmm2,5
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,5

	pextrw eax,xmm0,6
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,6
	pextrw eax,xmm1,6
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,6
	pextrw eax,xmm2,6
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,6

	pextrw eax,xmm0,7
	mov bx,[esi+2*eax]
	pinsrw xmm3,ebx,7
	pextrw eax,xmm1,7
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw xmm4,ebx,7
	pextrw eax,xmm2,7
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw xmm5,ebx,7

	paddsw xmm3,xmm4
	paddsw xmm3,xmm5 ; xmm3=G
	paddsw xmm1,xmm0 ; xmm1=R
	paddsw xmm2,xmm0 ; xmm2=B
	paddsw xmm1,xmm6
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	psraw xmm1,4
	psraw xmm2,4
	psraw xmm3,4
	packuswb xmm1,xmm7
	packuswb xmm2,xmm7
	packuswb xmm3,xmm7
	punpcklbw xmm1,xmm7
	punpcklbw xmm2,xmm7
	punpcklbw xmm3,xmm7

	mov edi,dst

	pextrw eax,xmm1,0
	shl eax,16
	pextrw ebx,xmm3,0
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,0
	or eax,ebx
	stosd

	pextrw eax,xmm1,1
	shl eax,16
	pextrw ebx,xmm3,1
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,1
	or eax,ebx
	stosd

	pextrw eax,xmm1,2
	shl eax,16
	pextrw ebx,xmm3,2
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,2
	or eax,ebx
	stosd

	pextrw eax,xmm1,3
	shl eax,16
	pextrw ebx,xmm3,3
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,3
	or eax,ebx
	stosd

	pextrw eax,xmm1,4
	shl eax,16
	pextrw ebx,xmm3,4
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,4
	or eax,ebx
	stosd

	pextrw eax,xmm1,5
	shl eax,16
	pextrw ebx,xmm3,5
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,5
	or eax,ebx
	stosd

	pextrw eax,xmm1,6
	shl eax,16
	pextrw ebx,xmm3,6
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,6
	or eax,ebx
	stosd

	pextrw eax,xmm1,7
	shl eax,16
	pextrw ebx,xmm3,7
	shl ebx,8
	or eax,ebx
	pextrw ebx,xmm2,7
	or eax,ebx
	stosd

	mov dst,edi

	dec i
	jnz Loop_3_2

	mov edi,dst
	add edi,dst_modulo
	mov dst,edi

	dec h
	jnz Loop_3_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_III_convert_YUV_RGB_SSE2 endp


JPSDR_VHS_III_convert_YUV_RGB_SSE proc dst:dword,y:dword,u:dword,v:dword,w:dword,h:dword,dst_modulo:dword,
	lookup:dword

	public JPSDR_VHS_III_convert_YUV_RGB_SSE

	local i:dword

	push esi
	push edi
	push ebx

	mov esi,lookup
	mov dx,8161
	mov cx,16322
	xor ebx,ebx
	pxor mm7,mm7
	mov eax,8
	pinsrw mm6,eax,0
	pinsrw mm6,eax,1
	pinsrw mm6,eax,2
	pinsrw mm6,eax,3
	cld

Loop_4_1:
	mov eax,w
	mov i,eax
Loop_4_2:
	mov edi,y
	movq mm0,[edi]
	add edi,8
	mov y,edi
	mov edi,v
	movq mm1,[edi]
	add edi,8
	mov v,edi
	mov edi,u
	movq mm2,[edi]
	add edi,8
	mov u,edi

	pextrw eax,mm0,0
	mov bx,[esi+2*eax]
	pinsrw mm3,ebx,0
	pextrw eax,mm1,0
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw mm4,ebx,0
	pextrw eax,mm2,0
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw mm5,ebx,0

	pextrw eax,mm0,1
	mov bx,[esi+2*eax]
	pinsrw mm3,ebx,1
	pextrw eax,mm1,1
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw mm4,ebx,1
	pextrw eax,mm2,1
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw mm5,ebx,1

	pextrw eax,mm0,2
	mov bx,[esi+2*eax]
	pinsrw mm3,ebx,2
	pextrw eax,mm1,2
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw mm4,ebx,2
	pextrw eax,mm2,2
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw mm5,ebx,2

	pextrw eax,mm0,3
	mov bx,[esi+2*eax]
	pinsrw mm3,ebx,3
	pextrw eax,mm1,3
	add ax,dx
	mov bx,[esi+2*eax]
	pinsrw mm4,ebx,3
	pextrw eax,mm2,3
	add ax,cx
	mov bx,[esi+2*eax]
	pinsrw mm5,ebx,3

	paddsw mm3,mm4
	paddsw mm3,mm5 ; mm3=G
	paddsw mm1,mm0 ; mm1=R
	paddsw mm2,mm0 ; mm2=B
	paddsw mm1,mm6
	paddsw mm2,mm6
	paddsw mm3,mm6
	psraw mm1,4
	psraw mm2,4
	psraw mm3,4
	packuswb mm1,mm7
	packuswb mm2,mm7
	packuswb mm3,mm7
	punpcklbw mm1,mm7
	punpcklbw mm2,mm7
	punpcklbw mm3,mm7

	mov edi,dst

	pextrw eax,mm1,0
	shl eax,16
	pextrw ebx,mm3,0
	shl ebx,8
	or eax,ebx
	pextrw ebx,mm2,0
	or eax,ebx
	stosd

	pextrw eax,mm1,1
	shl eax,16
	pextrw ebx,mm3,1
	shl ebx,8
	or eax,ebx
	pextrw ebx,mm2,1
	or eax,ebx
	stosd

	pextrw eax,mm1,2
	shl eax,16
	pextrw ebx,mm3,2
	shl ebx,8
	or eax,ebx
	pextrw ebx,mm2,2
	or eax,ebx
	stosd

	pextrw eax,mm1,3
	shl eax,16
	pextrw ebx,mm3,3
	shl ebx,8
	or eax,ebx
	pextrw ebx,mm2,3
	or eax,ebx
	stosd

	mov dst,edi

	dec i
	jnz Loop_4_2

	mov edi,dst
	add edi,dst_modulo
	mov dst,edi

	dec h
	jnz Loop_4_1

	emms

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_III_convert_YUV_RGB_SSE endp


JPSDR_VHS_III_shift_left proc in_:dword,out_:dword,w:dword,h:dword,delta:byte

	public JPSDR_VHS_III_shift_left

	push esi
	push edi
	push ebx

	cld
	xor ebx,ebx
	xor eax,eax
	mov bl,delta
	mov edx,w
	mov esi,in_
	sub edx,ebx
	mov edi,out_

Loop_5_1:
	add esi,ebx
	mov ecx,edx
	add esi,ebx
	rep movsw
	mov ecx,ebx
	rep stosw
	dec h
	jnz short Loop_5_1

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_III_shift_left endp


JPSDR_VHS_III_shift_right proc in_:dword,out_:dword,w:dword,h:dword,delta:byte

	public JPSDR_VHS_III_shift_right


	push esi
	push edi
	push ebx

	cld
	xor ebx,ebx
	xor eax,eax
	mov bl,delta
	mov edx,w
	mov esi,in_
	sub edx,ebx
	mov edi,out_

Loop_6_1:
	mov ecx,ebx
	rep stosw
	mov ecx,edx
	rep movsw
	add esi,ebx
	add esi,ebx
	dec h
	jnz short Loop_6_1

	pop ebx
	pop edi
	pop esi

	ret


JPSDR_VHS_III_shift_right endp

end

