;
;  Smart_Deinterlace
;
;  Deinterlace only on detected interlaced parts.
;  Copyright (C) Donald A. Graft
;	
;  Smart_Deinterlace is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  Smart_Deinterlace is distributed in the hope that it will be useful,
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


JPSDR_Smart_Deinterlace_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Smart_Deinterlace_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_1_f_3:
	mov ecx,ebx
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_1_f_3
	
fin_f_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Smart_Deinterlace_Move32_Full endp



JPSDR_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src:dword,buffer:dword,dst:dword,w:dword,
	h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

	public JPSDR_Smart_Deinterlace_Motion_Map_SSE_RGB32

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	xor eax,eax
	mov edi,dst
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src				;esi=src
	mov edi,esi
	add edi,pitch
	mov edx,buffer
	mov ecx,w
	xor eax,eax
Loop_A2_0:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edx+4*eax],mm0
	inc eax
	loop Loop_A2_0
	shl eax,2
	add edx,eax
	
	mov edi,edx				;edi=buffer + w
	mov ebx,dst				;ebx=map
	mov edx,pitch

Loop_A2_1:
	mov ecx,w
Loop_A2_2:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[esi+edx]			;mm1=src2
	movd mm2,dword ptr[esi+2*edx]		;mm2=src3
	movq mm3,mm0
	movq mm4,mm2
	punpcklbw mm1,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm1
	pcmpgtw mm4,mm1
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm1,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm1				;mm7=(sr2+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
	pxor mm5,mm5
	movq mm0,mm2
	movq mm4,mm2
	pminub mm0,mm1
	pmaxub mm4,mm1
	psubb mm4,mm0				; mm4=abs(sr3-src2)
	pminub mm4,mm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm4,mm5
	pcmpgtw mm4,mm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm4,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm4,mm5				; mm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
Next_A2_1:
	movd dword ptr[edi],mm7
	mov [ebx],al

	add esi,4
	add edi,4
	inc ebx

	dec ecx
	jnz Loop_A2_2

	mov eax,w_map
	sub eax,w
	add ebx,eax
	mov eax,modulo
	add esi,eax

	dec h
	jnz Loop_A2_1

	add esi,edx
	mov ecx,w
	cld
	rep movsd

	xor eax,eax
	mov ecx,w_map
	shr ecx,2
	mov edi,ebx
	rep stosd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_Smart_Deinterlace_Motion_Map_SSE_RGB32 endp





JPSDR_Smart_Deinterlace_SSE_RGB32 proc src:dword,buffer:dword,map:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,w_map:dword

	public JPSDR_Smart_Deinterlace_SSE_RGB32

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,buffer
	mov edi,dst
	mov edx,map

Loop_A1_1:
	xor eax,eax
	mov ecx,w
Loop_A1_2:
	movd mm0,dword ptr[esi+4*eax]
	bt word ptr [edx+eax],0
	jnc short Next_A1
	movd mm0,dword ptr[ebx+4*eax]
Next_A1:
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop Loop_A1_2

	add esi,src_pitch
	add edi,dst_pitch
	shl eax,2
	add ebx,eax
	add edx,w_map
	dec h
	jnz short Loop_A1_1

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_Smart_Deinterlace_SSE_RGB32 endp
	







end





