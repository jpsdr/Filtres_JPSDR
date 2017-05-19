.586
.mmx
.xmm
.model flat,c

.data

align 16

ub_FC dword 4 dup(00FCFCFCh)
ub_FF qword 2 dup(0000FFFFFFFFFFFFh)
filtre_1 dword 4 dup(00FFFFFFh)

.code


JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
	repart:dword,w:dword,h:dword,src_pitch:dword,src_modulo:dword,dst_pitch:dword,dst_modulo:dword,w_map:dword,error_motion_map:dword

	public JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32

	local w_loop,delta_map:dword

	push esi
	push edi
	push ebx

	cld
	xor eax,eax
	mov edi,histo
	mov ecx,256
	rep stosd

	mov esi,src
	mov edi,dst
	pxor mm1,mm1	; return value (s)
	pxor mm2,mm2	; error_motion_map
	pxor mm3,mm3
	mov edx,histo
	mov ecx,w_map
	mov ebx,map
	
	mov eax,ecx
	shl eax,1
	sub eax,w
	mov delta_map,eax	

Loop_A_1:
	mov eax,w
	mov w_loop,eax
Loop_A_2:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]
	movq mm4,mm0
	movq mm6,mm0
	psadbw mm4,mm3
	paddd mm1,mm4
	punpcklbw mm0,mm3
	pextrw eax,mm0,0
	inc dword ptr[edx+4*eax]
	pextrw eax,mm0,1
	inc dword ptr [edx+4*eax]
	pextrw eax,mm0,2
	inc dword ptr[edx+4*eax]
	mov al,byte ptr[ebx]
	or al,al
	jz short Next_1
	movq mm5,mm6
	paddd mm2,mm4
Next_1:
	mov eax,src_pitch
	movd dword ptr[edi],mm5

	pxor mm5,mm5
	movd mm0,dword ptr[esi+eax]
	movq mm4,mm0
	punpcklbw mm0,mm3
	pextrw eax,mm0,0
	inc dword ptr[edx+4*eax]
	pextrw eax,mm0,1
	inc dword ptr[edx+4*eax]
	pextrw eax,mm0,2
	inc dword ptr[edx+4*eax]
	mov al,byte ptr[ebx+ecx]
	or al,al
	jz short Next_2
	movq mm5,mm4
	psadbw mm4,mm3
	paddd mm2,mm4
Next_2:
	mov eax,dst_pitch
	movd dword ptr[edi+eax],mm5

	add edi,4
	add esi,4
	inc ebx
	dec w_loop
	jnz Loop_A_2
	add edi,dst_modulo
	add ebx,delta_map
	add esi,src_pitch
	add edi,dst_pitch
	add esi,src_modulo
	dec h
	jnz Loop_A_1

	mov esi,histo
	mov edi,repart
	add esi,1020
	add edi,1020
	xor ebx,ebx
	std
	mov ecx,256
Loop_A_3:
	lodsd
	add ebx,eax
	mov eax,ebx
	stosd
	loop Loop_A_3
	cld
	mov edi,error_motion_map
	movd dword ptr[edi],mm2
	movd eax,mm1

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 endp


JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
	repart:dword,w:dword,h:dword,src_pitch:dword,src_modulo:dword,dst_pitch:dword,dst_modulo:dword,w_map:dword,error_motion_map:dword

	public JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32

	local w_loop,delta_map:dword

	push esi
	push edi
	push ebx

	cld
	xor eax,eax
	mov edi,histo
	mov ecx,256
	rep stosd

	mov esi,src
	mov edi,dst
	pxor xmm1,xmm1	; return value (s)
	pxor xmm2,xmm2	; error_motion_map
	pxor xmm3,xmm3
	mov edx,histo
	mov ecx,w_map
	
	mov eax,ecx
	mov ebx,w
	shl eax,1
	shl ebx,2
	sub eax,ebx
	mov delta_map,eax	
	
	mov ebx,map

Loop_A_1b:
	mov eax,w
	mov w_loop,eax
Loop_A_2b:
	movd xmm5,dword ptr[ebx]
	punpcklbw xmm5,xmm3
	punpcklwd xmm5,xmm3
	
	movdqa xmm0,XMMWORD ptr[esi]
	pcmpgtd xmm5,xmm3
	movdqa xmm4,xmm0
	pand xmm5,xmm0
	movdqa xmm7,xmm0
	psadbw xmm4,xmm3
	movdqa xmm6,xmm5
	paddd xmm1,xmm4
	psadbw xmm5,xmm3
	punpcklbw xmm0,xmm3
	paddd xmm2,xmm5
	pextrw eax,xmm0,0
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm0,1
	inc dword ptr [edx+4*eax]
	pextrw eax,xmm0,2
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm0,4
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm0,5
	inc dword ptr [edx+4*eax]
	punpckhbw xmm7,xmm3
	pextrw eax,xmm0,6
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,0
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,1
	inc dword ptr [edx+4*eax]
	pextrw eax,xmm7,2
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,4
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,5
	inc dword ptr [edx+4*eax]
	pextrw eax,xmm7,6
	inc dword ptr[edx+4*eax]
	
	movdqa XMMWORD ptr[edi],xmm6
	
	movd xmm5,dword ptr[ebx+ecx]
	punpcklbw xmm5,xmm3
	mov eax,src_pitch
	punpcklwd xmm5,xmm3	
	
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pcmpgtd xmm5,xmm3
	pand xmm5,xmm0
	movdqa xmm7,xmm0
	movdqa xmm6,xmm5
	psadbw xmm5,xmm3
	punpcklbw xmm0,xmm3
	paddd xmm2,xmm5
	pextrw eax,xmm0,0
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm0,1
	inc dword ptr [edx+4*eax]
	pextrw eax,xmm0,2
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm0,4
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm0,5
	inc dword ptr [edx+4*eax]
	punpckhbw xmm7,xmm3
	pextrw eax,xmm0,6
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,0
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,1
	inc dword ptr [edx+4*eax]
	pextrw eax,xmm7,2
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,4
	inc dword ptr[edx+4*eax]
	pextrw eax,xmm7,5
	inc dword ptr [edx+4*eax]
	pextrw eax,xmm7,6
	inc dword ptr[edx+4*eax]
	
	mov eax,dst_pitch
	movdqa XMMWORD ptr[edi+eax],xmm6
	
	add edi,16
	add esi,16
	add ebx,4
	dec w_loop
	jnz Loop_A_2b
	add edi,dst_modulo
	add ebx,delta_map
	add esi,src_pitch
	add edi,dst_pitch
	add esi,src_modulo
	dec h
	jnz Loop_A_1b

	mov esi,histo
	mov edi,repart
	add esi,1020
	add edi,1020
	xor ebx,ebx
	std
	mov ecx,256
Loop_A_3b:
	lodsd
	add ebx,eax
	mov eax,ebx
	stosd
	loop Loop_A_3b
	cld
	
	mov edi,error_motion_map
	
	movhlps xmm4,xmm2
	movhlps xmm0,xmm1
	paddd xmm2,xmm4
	paddd xmm1,xmm0
	
	movd dword ptr[edi],xmm2
	movd eax,xmm1

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE2_RGB32 endp


JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
	src_pitch:dword,src_modulo:dword,error_motion_map:dword

	public JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32
	
	local delta_map:dword

	push esi
	push edi
	push ebx
	
	mov eax,w_map
	shl eax,1
	sub eax,w
	mov delta_map,eax		

	mov esi,src
	pxor mm1,mm1
	pxor mm2,mm2
	pxor mm3,mm3
	mov edi,map
	mov edx,src_pitch
	mov ebx,w_map
Loop_A1_1:
    mov ecx,w
Loop_A1_2:
	movd mm0,dword ptr[esi]
	psadbw mm0,mm3
	paddd mm1,mm0
	mov al,byte ptr[edi]
	or al,al
	jz short Next_A1_1
	paddd mm2,mm0
Next_A1_1:
	mov al,byte ptr[edi+ebx]
	or al,al
	jz short Next_A1_2
	movd mm0,dword ptr[esi+edx]
	psadbw mm0,mm3
	paddd mm2,mm0
Next_A1_2:
	add esi,4
	inc edi
	loop Loop_A1_2
	add esi,src_modulo
	add edi,delta_map
	add esi,edx
	dec h
	jnz short Loop_A1_1

	mov edi,error_motion_map
	movd dword ptr[edi],mm2
	movd eax,mm1

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 endp


JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
	src_pitch:dword,src_modulo:dword,error_motion_map:dword

	public JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32

	push esi
	push edi
	push ebx

	mov esi,src
	pxor xmm1,xmm1
	pxor xmm2,xmm2
	pxor xmm3,xmm3
	mov edi,map
	
	mov eax,w_map
	mov ebx,w
	shl eax,1
	shl ebx,2
	sub eax,ebx
	
	mov ebx,w_map
	mov edx,src_pitch
Loop_A1_1:
    mov ecx,w
Loop_A1_2:
	movd xmm4,dword ptr[edi]
	punpcklbw xmm4,xmm3
	punpcklwd xmm4,xmm3
	
	movdqa xmm0,XMMWORD ptr[esi]
	pcmpgtd xmm4,xmm3
	pand xmm4,xmm0
	psadbw xmm0,xmm3
	psadbw xmm4,xmm3
	paddd xmm1,xmm0
	paddd xmm2,xmm4
	
	movd xmm4,dword ptr[edi+ebx]
	punpcklbw xmm4,xmm3
	punpcklwd xmm4,xmm3
	
	movdqa xmm0,XMMWORD ptr[esi+edx]
	pcmpgtd xmm4,xmm3
	pand xmm4,xmm0
	psadbw xmm4,xmm3
	paddd xmm2,xmm4

Next_A1_2:
	add esi,16
	add edi,4
	loop Loop_A1_2
	add esi,src_modulo
	add edi,eax
	add esi,edx
	dec h
	jnz short Loop_A1_1

	mov edi,error_motion_map
	
	movhlps xmm4,xmm2
	movhlps xmm0,xmm1
	paddd xmm2,xmm4
	paddd xmm1,xmm0	
	
	movd dword ptr[edi],xmm2
	movd eax,xmm1

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE2_RGB32 endp


JPSDR_IVTC_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE_RGB32

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax

	xor eax,eax
	mov edi,dst
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1:
	mov eax,w
	mov w_loop,eax
Loop_A0_2:
	mov edx,pitch
	pxor mm5,mm5
	shl edx,1
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A0_1:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[ecx],mm3
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2:
    mov edx,pitch_buffer
	pand mm3,mm7
	movd dword ptr[ecx+edx],mm3
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1

	push ebx
	xor eax,eax
	mov ebx,ecx
	mov edx,ecx
	mov ecx,w
	add edx,pitch_buffer
Loop_A0_3:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	movq mm2,mm0
	pminub mm0,mm1
	pmaxub mm2,mm1
	psubb mm2,mm0
	pand mm2,mm7
	movd dword ptr[ebx+4*eax],mm2
	movd dword ptr[edx+4*eax],mm2
	inc eax
	loop Loop_A0_3
	pop ebx

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

JPSDR_IVTC_Motion_Map_SSE_RGB32 endp


JPSDR_IVTC_Motion_Map_SSE2_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE2_RGB32

	local w_loop,delta_map:dword

	push esi
	push edi
	push ebx

	mov eax,w_map
	mov ebx,w
	shl eax,1
	shl ebx,1
	sub eax,ebx
	mov delta_map,eax
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5

	xor eax,eax
	mov edi,dst
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1:
	mov eax,w
	mov w_loop,eax
Loop_A0_2:
	mov edx,pitch
	movq xmm0,qword ptr[esi]				;xmm0=src1
	shl edx,1
	movq xmm1,qword ptr[edi]				;xmm1=src2
	movq xmm2,qword ptr[esi+edx]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm3,xmm5
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,XMMWORD ptr ub_FF ;xmm7 = FF si produit est positif		
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,XMMWORD ptr filtre_1
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[ebx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,XMMWORD ptr ub_FC				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[ebx+1],al
	
	movq qword ptr[ecx],xmm3

	movq xmm0,qword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movdqa xmm7,xmm2
	movdqa xmm3,xmm0
	movdqa xmm4,xmm1
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm3,xmm7
	pcmpgtw xmm4,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,XMMWORD ptr ub_FF ;xmm4 = FF si produit est positif		
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,XMMWORD ptr filtre_1
	
	mov edx,w_map
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[ebx+edx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,XMMWORD ptr ub_FC	
	mov byte ptr[ebx+edx+1],al
	
    mov edx,pitch_buffer
	movq qword ptr[ecx+edx],xmm3

	add esi,8
	add edi,8
	add ecx,8
	add ebx,2

	dec w_loop
	jnz Loop_A0_2

	add ebx,delta_map
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1

	movdqa xmm7,XMMWORD ptr ub_FC
	push ebx
	xor eax,eax
	mov ebx,ecx
	mov edx,ecx
	mov ecx,w
	add edx,pitch_buffer
Loop_A0_3:
	movq xmm0,qword ptr[esi+8*eax]
	movq xmm1,qword ptr[edi+8*eax]
	movdqa xmm2,xmm0
	pminub xmm0,xmm1
	pmaxub xmm2,xmm1
	psubb xmm2,xmm0
	pand xmm2,xmm7
	movq qword ptr[ebx+8*eax],xmm2
	movq qword ptr[edx+8*eax],xmm2
	inc eax
	loop Loop_A0_3
	pop ebx

	xor eax,eax
	mov ecx,w_map
	shr ecx,2
	mov edi,ebx
	rep stosd

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32 endp


JPSDR_IVTC_Motion_Map_SSE_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE_RGB32_a

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax

	xor eax,eax
	mov edi,dst
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1_1:
	mov eax,w
	mov w_loop,eax
Loop_A0_2_1:
	mov edx,pitch
	pxor mm5,mm5
	shl edx,1
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A0_1_1:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[ecx],mm3
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2_1
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2_1:
    mov edx,pitch_buffer
	pand mm3,mm7
	movd dword ptr[ecx+edx],mm3
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2_1

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1_1
	
	mov eax,w
	mov w_loop,eax
	mov edx,pitch
	shl edx,1
Loop_A0_2_1_a:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_1_a			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A0_1_1_a:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[ecx],mm3
	mov byte ptr[ebx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2_1_a
	
	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_a endp


JPSDR_IVTC_Motion_Map_SSE2_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE2_RGB32_a

	local w_loop,delta_map:dword

	push esi
	push edi
	push ebx

	mov eax,w_map
	mov ebx,w
	shl eax,1
	shl ebx,1
	sub eax,ebx
	mov delta_map,eax
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5

	xor eax,eax
	mov edi,dst
	cld
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1_1:
	mov eax,w
	mov w_loop,eax
Loop_A0_2_1:
	mov edx,pitch
	movq xmm0,qword ptr[esi]				;xmm0=src1
	shl edx,1
	movq xmm1,qword ptr[edi]				;xmm1=src2
	movq xmm2,qword ptr[esi+edx]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm3,xmm5
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,XMMWORD ptr ub_FF ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,XMMWORD ptr filtre_1
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[ebx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,XMMWORD ptr ub_FC				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[ebx+1],al
	
	movq qword ptr[ecx],xmm3

	movq xmm0,qword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movdqa xmm7,xmm2
	movdqa xmm3,xmm0
	movdqa xmm4,xmm1
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm3,xmm7
	pcmpgtw xmm4,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,XMMWORD ptr ub_FF ;xmm4 = FF si produit est positif
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,XMMWORD ptr filtre_1
	
	mov edx,w_map
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[ebx+edx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,XMMWORD ptr ub_FC	
	mov byte ptr[ebx+edx+1],al
	
    mov edx,pitch_buffer
	movq qword ptr[ecx+edx],xmm3

	add esi,8
	add edi,8
	add ecx,8
	add ebx,2

	dec w_loop
	jnz Loop_A0_2_1

	add ebx,delta_map
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1_1
	
	mov eax,w
	mov edx,pitch
	mov w_loop,eax
	shl edx,1
Loop_A0_2_1_a:
	movq xmm0,qword ptr[esi]				;mm0=src1
	movq xmm1,qword ptr[edi]				;mm1=src2
	movq xmm2,qword ptr[esi+edx]			;mm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm3,xmm5
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,XMMWORD ptr ub_FF ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; mm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; mm4=abs(sr3-src2)
	pminub xmm4,xmm3				; mm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; mm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0	
	
	packsswb xmm4,xmm5
	pand xmm4,XMMWORD ptr filtre_1
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[ebx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,XMMWORD ptr ub_FC				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[ebx+1],al
	
	movq qword ptr[ecx],xmm3

	add esi,8
	add edi,8
	add ecx,8
	add ebx,2

	dec w_loop
	jnz Loop_A0_2_1_a
	
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32_a endp


JPSDR_IVTC_Motion_Map_SSE_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE_RGB32_b

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1_2:
	mov eax,w
	mov w_loop,eax
Loop_A0_2_2:
	mov edx,pitch
	pxor mm5,mm5
	shl edx,1
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_2			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A0_1_2:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[ecx],mm3
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2_2
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2_2:
    mov edx,pitch_buffer
	pand mm3,mm7
	movd dword ptr[ecx+edx],mm3
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2_2

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1_2

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_b endp


JPSDR_IVTC_Motion_Map_SSE2_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE2_RGB32_b

	local w_loop,delta_map:dword

	push esi
	push edi
	push ebx

	mov eax,w_map
	mov ebx,w
	shl eax,1
	shl ebx,1
	sub eax,ebx
	mov delta_map,eax
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1_2:
	mov eax,w
	mov w_loop,eax
Loop_A0_2_2:
	mov edx,pitch
	movq xmm0,qword ptr[esi]				;xmm0=src1
	shl edx,1
	movq xmm1,qword ptr[edi]				;xmm1=src2
	movq xmm2,qword ptr[esi+edx]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm3,xmm5
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,XMMWORD ptr ub_FF ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,XMMWORD ptr filtre_1
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[ebx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,XMMWORD ptr ub_FC				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[ebx+1],al
	
	movq qword ptr[ecx],xmm3

	movq xmm0,qword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movdqa xmm7,xmm2
	movdqa xmm3,xmm0
	movdqa xmm4,xmm1
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm3,xmm7
	pcmpgtw xmm4,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,XMMWORD ptr ub_FF ;xmm4 = FF si produit est positif
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,XMMWORD ptr filtre_1
	
	mov edx,w_map
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[ebx+edx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,XMMWORD ptr ub_FC	
	mov byte ptr[ebx+edx+1],al
	
    mov edx,pitch_buffer
	movq qword ptr[ecx+edx],xmm3

	add esi,8
	add edi,8
	add ecx,8
	add ebx,2

	dec w_loop
	jnz Loop_A0_2_2

	add ebx,delta_map
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1_2

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32_b endp


JPSDR_IVTC_Motion_Map_SSE_RGB32_c proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE_RGB32_c

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1_3:
	mov eax,w
	mov w_loop,eax
Loop_A0_2_3:
	mov edx,pitch
	pxor mm5,mm5
	shl edx,1
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_3			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A0_1_3:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[ecx],mm3
	mov byte ptr[ebx],al
	
	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2_3
	pxor mm5,mm5
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A0_2_3:
    mov edx,pitch_buffer
	pand mm3,mm7
	movd dword ptr[ecx+edx],mm3
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2_3

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1_3
		
	mov eax,w
	mov w_loop,eax
Loop_A0_2_3_c:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A0_1_3_c			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A0_1_3_c:
	pand mm3,mm7				; Filtre par 0xFC, pour lever le bruit
	movd dword ptr[ecx],mm3
	mov byte ptr[ebx],al

	movq mm3,mm1                ;Ici : mm1=src2, mm2=src3
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	mov edx,pitch_buffer
	pand mm3,mm7
	movd dword ptr[ecx+edx],mm3	

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2_3_c
	
	add ecx,modulo_buffer
	
	mov edx,w_map
	sub edx,w
	add ebx,edx
		
	mov edx,ecx	
	xor eax,eax
	mov ecx,w_map
	shr ecx,2
	mov edi,ebx
	rep stosd	
	
	mov edi,edx
	mov esi,edx
	add edi,pitch_buffer
	mov ecx,w
	rep movsd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_c endp


JPSDR_IVTC_Motion_Map_SSE2_RGB32_c proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Motion_Map_SSE2_RGB32_c

	local w_loop,delta_map:dword

	push esi
	push edi
	push ebx
	
	mov eax,w_map
	mov ebx,w
	shl eax,1
	shl ebx,1
	sub eax,ebx
	mov delta_map,eax
	
	mov eax,thr
	pxor xmm5,xmm5
	movd xmm6,eax
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	punpcklbw xmm6,xmm5

	mov esi,src1
	mov edi,src2
	mov ebx,dst
	mov ecx,buffer

Loop_A0_1_3:
	mov eax,w
	mov w_loop,eax
Loop_A0_2_3:
	mov edx,pitch
	movq xmm0,qword ptr[esi]				;xmm0=src1
	shl edx,1
	movq xmm1,qword ptr[edi]				;xmm1=src2
	movq xmm2,qword ptr[esi+edx]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm3,xmm5
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,XMMWORD ptr ub_FF ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7				; xmm4=(FF) si max>Threshold & (src2-scr1)*(src2-src3)>0
	packsswb xmm4,xmm5
	pand xmm4,XMMWORD ptr filtre_1
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[ebx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,XMMWORD ptr ub_FC				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[ebx+1],al
	
	movq qword ptr[ecx],xmm3

	movq xmm0,qword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movdqa xmm7,xmm2
	movdqa xmm3,xmm0
	movdqa xmm4,xmm1
	punpcklbw xmm7,xmm5
	punpcklbw xmm3,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm3,xmm7
	pcmpgtw xmm4,xmm7
	pxor xmm4,xmm3				;xmm4 = FF si produit est négatif
	movdqa xmm3,xmm1
	pxor xmm4,XMMWORD ptr ub_FF ;xmm4 = FF si produit est positif
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	movdqa xmm1,xmm0
	pminub xmm1,xmm2
	pmaxub xmm0,xmm2
	psubb xmm0,xmm1				; xmm0=abs(sr4-src3)
	pminub xmm0,xmm3				; xmm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm0,xmm5
	pcmpgtw xmm0,xmm6				; xmm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm0,xmm4
	packsswb xmm0,xmm5
	pand xmm0,XMMWORD ptr filtre_1
	
	mov edx,w_map
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
	psrlq xmm0,32
	mov byte ptr[ebx+edx],al
	
	movd eax,xmm0
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al		
	pand xmm3,XMMWORD ptr ub_FC	
	mov byte ptr[ebx+edx+1],al
	
    mov edx,pitch_buffer
	movq qword ptr[ecx+edx],xmm3

	add esi,8
	add edi,8
	add ecx,8
	add ebx,2

	dec w_loop
	jnz Loop_A0_2_3

	add ebx,delta_map
	add ecx,pitch_buffer
	mov edx,modulo
	add ecx,modulo_buffer
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1_3
		
	mov eax,w
	mov w_loop,eax
Loop_A0_2_3_c:
	mov edx,pitch
	movq xmm0,qword ptr[esi]				;xmm0=src1
	shl edx,1
	movq xmm1,qword ptr[edi]				;xmm1=src2
	movq xmm2,qword ptr[esi+edx]			;xmm2=src3
	movdqa xmm7,xmm0
	movdqa xmm3,xmm1
	movdqa xmm4,xmm2
	punpcklbw xmm3,xmm5
	punpcklbw xmm7,xmm5
	punpcklbw xmm4,xmm5
	pcmpgtw xmm7,xmm3
	pcmpgtw xmm4,xmm3
	pxor xmm7,xmm4				;xmm7 = FF si produit est négatif
	movdqa xmm3,xmm0
	pxor xmm7,XMMWORD ptr ub_FF ;xmm7 = FF si produit est positif
	pminub xmm0,xmm1
	pmaxub xmm3,xmm1
	psubb xmm3,xmm0				; xmm3=abs(src1-src2)
	movdqa xmm0,xmm2
	movdqa xmm4,xmm2
	pminub xmm0,xmm1
	pmaxub xmm4,xmm1
	psubb xmm4,xmm0				; xmm4=abs(sr3-src2)
	pminub xmm4,xmm3				; xmm4 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw xmm4,xmm5
	pcmpgtw xmm4,xmm6				; xmm4=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	pand xmm4,xmm7
	
	packsswb xmm4,xmm5
	pand xmm4,XMMWORD ptr filtre_1
	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al
	psrlq xmm4,32
	mov byte ptr[ebx],al

	movd eax,xmm4
	or eax,eax					; si aucun ne satisfait les 2, al=0
	setnz al	
	pand xmm3,XMMWORD ptr ub_FC				; Filtre par 0xFC, pour lever le bruit
	mov byte ptr[ebx+1],al
	
	movq qword ptr[ecx],xmm3

	movdqa xmm3,xmm1                ;Ici : xmm1=src2, xmm2=src3
	pminub xmm1,xmm2
	pmaxub xmm3,xmm2
	psubb xmm3,xmm1				; xmm3=abs(src2-src3)
	mov edx,pitch_buffer
	pand xmm3,XMMWORD ptr ub_FC
	movq qword ptr[ecx+edx],xmm3	

	add esi,8
	add edi,8
	add ecx,8
	add ebx,2

	dec w_loop
	jnz Loop_A0_2_3_c
	
	add ecx,modulo_buffer
	
	mov eax,w
	add ebx,w_map
	shl eax,1
	sub ebx,eax
		
	mov edx,ecx	
	xor eax,eax
	mov ecx,w_map
	shr ecx,2
	mov edi,ebx
	rep stosd	
	
	mov edi,edx
	mov esi,edx
	add edi,pitch_buffer
	mov ecx,w
	shl ecx,1
	rep movsd

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE2_RGB32_c endp


JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	cld
	xor eax,eax
	mov edi,dst
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
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
	add edx,pitch_buffer
	
	mov ecx,edx				;ecx=buffer + pitch_buffer
	mov ebx,dst

Loop_A2_1:
	mov eax,w
	mov w_loop,eax
Loop_A2_2:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A2_2

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,modulo_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,pitch_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A2_1

	mov esi,edi
	mov edi,ecx
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

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 endp


JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	cld
	xor eax,eax
	mov edi,dst
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
	mov edx,buffer
	mov ecx,w
	xor eax,eax
Loop_A2_0_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edx+4*eax],mm0
	inc eax
	loop Loop_A2_0_1
	add edx,pitch_buffer
	
	mov ecx,edx				;ecx=buffer + pitch_buffer
	mov ebx,dst

Loop_A2_1_1:
	mov eax,w
	mov w_loop,eax
Loop_A2_2_1:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_1		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A2_1_1:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2_1
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2_1:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A2_2_1

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,modulo_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,pitch_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A2_1_1

	mov eax,w
	mov w_loop,eax
	mov edx,pitch
	shl edx,1
Loop_A2_2_1_a:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_1_a		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A2_1_1_a:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A2_2_1_a		
	
	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_a endp


JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov esi,src1
	mov edi,src2
	mov ecx,buffer
	mov ebx,dst

Loop_A2_1_2:
	mov eax,w
	mov w_loop,eax
Loop_A2_2_2:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_2		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A2_1_2:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2_2:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A2_2_2

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,modulo_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A2_1_2

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_b endp


JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
	h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov esi,src1
	mov edi,src2
	mov ecx,buffer
	mov ebx,dst

Loop_A2_1_3:
	mov eax,w
	mov w_loop,eax
Loop_A2_2_3:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_3		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A2_1_3:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm2
	pavgb mm7,mm0				;mm7=(src4+src3)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A2_2_3
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_A2_2_3:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A2_2_3

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,modulo_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A2_1_3
	
	mov eax,w
	mov w_loop,eax
	mov edx,pitch
	shl edx,1
Loop_A2_2_3_c:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	pavgb mm7,mm1				;mm7=(sr3+src2)/2

	movd eax,mm3				;EAX = FF si produit est négatif
	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_A2_1_3_c		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_A2_1_3_c:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A2_2_3_c
	
	add ecx,modulo_buffer	
	add ebx,w_map
	sub ebx,w

	mov esi,ecx
	sub esi,pitch_buffer	
	mov edi,ecx
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

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32_c endp


JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,
	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	cld
	xor eax,eax
	mov edi,dst
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi
	
	mov esi,src1
	mov edi,src2
	mov edx,buffer
	mov ecx,w
	xor eax,eax
Loop_E2_0_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edx+4*eax],mm0
	inc eax
	loop Loop_E2_0_1
	add edx,pitch_buffer

	mov ecx,edx				;ecx=buffer + pitch_buffer
	mov ebx,dst

Loop_E2_1:
	mov eax,w
	mov w_loop,eax
Loop_E2_2:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_E2_1:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_E2_2

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,modulo_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_E2_1

	mov edx,ecx
	xor eax,eax
	mov ecx,w
Loop_E2_0_2:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edx+4*eax],mm0
	inc eax
	loop Loop_E2_0_2

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

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 endp


JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,
	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	cld
	xor eax,eax
	mov edi,dst
	mov ecx,w_map
	shr ecx,2
	rep stosd
	mov dst,edi

	mov esi,src1
	mov edi,src2
	mov edx,buffer
	mov ecx,w
	xor eax,eax
Loop_E2_0_1_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edx+4*eax],mm0
	inc eax
	loop Loop_E2_0_1_1
	add edx,pitch_buffer
	
	mov ecx,edx				;ecx=buffer + pitch_buffer
	mov ebx,dst

Loop_E2_1_1:
	mov eax,w
	mov w_loop,eax
Loop_E2_2_1:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_1		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_E2_1_1:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2_1
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2_1:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_E2_2_1

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,modulo_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_E2_1_1
	
	mov eax,w
	mov w_loop,eax
	mov edx,pitch
	shl edx,1
Loop_E2_2_1_a:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_1_a		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_E2_1_1_a:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_E2_2_1_a	
	
	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_a endp


JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,
	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov esi,src1
	mov edi,src2
	mov ecx,buffer
	mov ebx,dst

Loop_E2_1_2:
	mov eax,w
	mov w_loop,eax
Loop_E2_2_2:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_2		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_E2_1_2:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2_2
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2_2:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_E2_2_2

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,modulo_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_E2_1_2

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_b endp


JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c proc src1:dword,src2:dword,buffer:dword,dst:dword,
	w:dword,h:dword,pitch:dword,modulo:dword,pitch_buffer:dword,modulo_buffer:dword,thr:dword,w_map:dword

	public JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c

	local w_loop:dword

	push esi
	push edi
	push ebx

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov esi,src1
	mov edi,src2
	mov ecx,buffer
	mov ebx,dst

Loop_E2_1_3:
	mov eax,w
	mov w_loop,eax
Loop_E2_2_3:
	mov edx,pitch
	shl edx,1
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_3		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_E2_1_3:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	pxor mm5,mm5
	movd mm0,dword ptr[edi+edx]			;Ici : mm0=src4, mm1=src2, mm2=src3
	movq mm3,mm0
	movq mm4,mm1
	punpcklbw mm2,mm5
	punpcklbw mm3,mm5
	punpcklbw mm4,mm5
	pcmpgtw mm3,mm2
	pcmpgtw mm4,mm2
	packsswb mm3,mm5
	packsswb mm4,mm5
	packuswb mm2,mm5
	pxor mm3,mm4				;mm3 = FF si produit est négatif

	movq mm7,mm1
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm2
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src2+src4)+2*src3+2)/4

	movq mm3,mm1
	pminub mm1,mm2
	pmaxub mm3,mm2
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,pitch_buffer
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_E2_2_3
	movq mm1,mm0
	pminub mm1,mm2
	pmaxub mm0,mm2
	psubb mm0,mm1				; mm0=abs(sr4-src3)
	pminub mm0,mm3				; mm0 = min(abs(sr3-src2),abs(src1-src2))
	punpcklbw mm0,mm5
	pcmpgtw mm0,mm6				; mm0=(FF) si min(abs(src3-src2),abs(src2-src1))>Threshold
	packsswb mm0,mm5
	movd mm5,eax				; mm5 = FF seulement aux endroit où produit positif
	pand mm5,mm0				; mm5=(FF) si min>Threshold & (src2-scr1)*(src2-src3)>0
	movd eax,mm5
	or eax,eax					; si aucun ne satisfait les 2...
	setnz al
Next_E2_2_3:
	movd dword ptr[ecx+edx],mm7
	mov edx,w_map
	mov byte ptr[ebx+edx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_E2_2_3

	shl edx,1
	sub edx,w
	add ebx,edx
	add ecx,pitch_buffer
	mov edx,modulo
	add edx,pitch
	add ecx,modulo_buffer
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_E2_1_3
	
	mov eax,w
	mov w_loop,eax
	mov edx,pitch
	shl edx,1
Loop_E2_2_3_c:
	pxor mm5,mm5
	movd mm0,dword ptr[esi]				;mm0=src1
	movd mm1,dword ptr[edi]				;mm1=src2
	movd mm2,dword ptr[esi+edx]			;mm2=src3
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
	movq mm4,mm0
	punpcklbw mm7,mm5
	punpcklbw mm4,mm5
	mov eax,02020202h
	paddw mm7,mm4
	movd mm4,eax
	movd eax,mm3				;EAX = FF si produit est négatif
	punpcklbw mm4,mm5
	movq mm3,mm1
	paddw mm7,mm4
	punpcklbw mm3,mm5
	psllw mm3,1
	paddw mm7,mm3
	psrlw mm7,2					
	packuswb mm7,mm5			;mm7=((src1+src3)+2*src2+2)/4

	movq mm3,mm0
	pminub mm0,mm1
	pmaxub mm3,mm1
	psubb mm3,mm0				; mm3=abs(src1-src2)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	xor eax,00FFFFFFh			; EAX = FF si produit est positif
	jz short Next_E2_1_3_c		; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
Next_E2_1_3_c:
	movd dword ptr[ecx],mm7
	mov byte ptr[ebx],al

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_E2_2_3_c
	
	add ecx,modulo_buffer
	
	mov edx,ecx
	mov eax,w
	shl eax,2
	sub edi,eax
	add esi,modulo
	add esi,pitch
	xor eax,eax
	mov ecx,w
Loop_E2_0_2_3:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edx+4*eax],mm0
	inc eax
	loop Loop_E2_0_2_3
	
	add ebx,w_map
	sub ebx,w

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

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32_c endp


JPSDR_IVTC_Norme1_SSE_1_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword

	public JPSDR_IVTC_Norme1_SSE_1_RGB32

	push esi
	push edi
	push ebx

	pxor mm3,mm3
	mov eax,00FC00FCh
	movd mm5,eax
	mov eax,0FCFCFCFCh
	movd mm4,eax
	punpcklbw mm4,mm5
	mov esi,src1
	mov edi,src2
	pxor mm5,mm5
Loop_B_1:
	mov ecx,w
	xor edx,edx
Loop_B_2:
	movd mm1,dword ptr[esi+4*edx]
	movd mm0,dword ptr[edi+4*edx]
	movq mm2,mm1
	pmaxub mm2,mm0
	pminub mm1,mm0
	psubb mm2,mm1
	pand mm2,mm4
	psadbw mm2,mm3
	inc edx
	paddd mm5,mm2
	loop loop_B_2
	mov ebx,pitch
	shl ebx,1
	add esi,ebx
	add edi,ebx
	dec h
	jnz short Loop_B_1
	movd eax,mm5

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Norme1_SSE_1_RGB32 endp


JPSDR_IVTC_Norme1_SSE2_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword

	public JPSDR_IVTC_Norme1_SSE2_RGB32

	push esi
	push edi
	push ebx

	pxor xmm2,xmm2
	pxor xmm5,xmm5
	movdqa xmm3,XMMWORD ptr ub_FC
	mov esi,src1
	mov edi,src2
	mov ebx,pitch
	shl ebx,1
	mov edx,w
	
Loop_B_1:
	xor eax,eax
	mov ecx,edx
Loop_B_2:
	movdqa xmm1,XMMWORD ptr[esi+eax]
	movdqa xmm0,XMMWORD ptr[edi+eax]
	movdqa xmm4,xmm1
	pminub xmm1,xmm0
	pmaxub xmm4,xmm0
	psubb xmm4,xmm1	
	pand xmm4,xmm3
	psadbw xmm4,xmm5
	add eax,16
	paddd xmm2,xmm4
	loop loop_B_2
	add esi,ebx
	add edi,ebx
	dec h
	jnz short Loop_B_1
	
	movhlps xmm0,xmm2
	paddd xmm2,xmm0
	movd eax,xmm2

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Norme1_SSE2_RGB32 endp


JPSDR_IVTC_Norme1_SSE_2_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword

	public JPSDR_IVTC_Norme1_SSE_2_RGB32

	push esi
	push edi
	push ebx

	pxor mm3,mm3
	mov eax,00FC00FCh
	movd mm5,eax
	mov eax,0FCFCFCFCh
	movd mm4,eax
	punpcklbw mm4,mm5
	mov esi,src1
	mov edi,src2
	pxor mm5,mm5
Loop_C_1:
	mov ecx,w
	xor edx,edx
Loop_C_2:
	movq mm1,qword ptr[esi+8*edx]
	movq mm0,qword ptr[edi+8*edx]
	movq mm2,mm1
	pmaxub mm2,mm0
	pminub mm1,mm0
	psubb mm2,mm1
	pand mm2,mm4
	psadbw mm2,mm3
	inc edx
	paddd mm5,mm2
	loop loop_C_2
	mov ebx,pitch
	shl ebx,1
	add esi,ebx
	add edi,ebx
	dec h
	jnz short Loop_C_1
	movd eax,mm5

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Norme1_SSE_2_RGB32 endp


JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32

	local boucle,res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2
loop_1:
	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_1:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_1
	add esi,src_pitch
	add edx,dst_pitch
	mov dst,edx

	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_2:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_2
	add edi,src_pitch		
	add edx,dst_pitch
	mov dst,edx

	dec h
	jnz loop_1

	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_3:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_3
	
	mov esi,edx
	cld
	add edx,dst_pitch
	mov ecx,w
	mov edi,edx
	rep movsd

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 endp


JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a

	local boucle,res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2
loop_1_1:
	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_1_1:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_1_1
	add esi,src_pitch
	add edx,dst_pitch
	mov dst,edx

	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_2_1:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_2_1
	add edi,src_pitch		
	add edx,dst_pitch
	mov dst,edx

	dec h
	jnz loop_1_1

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_a endp


JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b

	local boucle,res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2
loop_1_2:
	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_1_2:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_1_2
	add esi,src_pitch
	add edx,dst_pitch
	mov dst,edx

	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_2_2:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_2_2
	add edi,src_pitch		
	add edx,dst_pitch
	mov dst,edx

	dec h
	jnz loop_1_2

	mov eax,w
	mov boucle,eax
	xor eax,eax
loop_2_3_2:
    mov edx,000000FFh
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or res,ebx
	mov ebx,dword ptr[esi+4*eax]
	mov ecx,dword ptr[edi+4*eax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	dec boucle
	jnz short loop_2_3_2
	
	mov esi,edx
	add edx,dst_pitch
	cld
	mov edi,edx
	mov ecx,w
	rep movsd

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32_b endp


JPSDR_IVTC_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_c:
	xor eax,eax
	mov ecx,w
loop_2_c_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_1
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_c_2:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_2
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_c

	xor eax,eax
	mov ecx,w
loop_2_c_3:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_3
	
	mov esi,edi
	cld
	add edi,dst_pitch
	mov ecx,w
	rep movsd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE endp


JPSDR_IVTC_Deinterlace_Blend_SSE_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_a

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_c_1:
	xor eax,eax
	mov ecx,w
loop_2_c_1_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_1_1
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_c_2_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_2_1
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_c_1

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_a endp


JPSDR_IVTC_Deinterlace_Blend_SSE_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_b

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_c_2:
	xor eax,eax
	mov ecx,w
loop_2_c_1_2:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_1_2
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_c_2_2:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_2_2
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_c_2

	xor eax,eax
	mov ecx,w
loop_2_c_3_2:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c_3_2
	
	mov esi,edi
	cld
	add edi,dst_pitch
	mov ecx,w
	rep movsd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_b endp


JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_2

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_d:
	xor eax,eax
	mov ecx,w
loop_2_d_1:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_1
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_d_2:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_2
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_d

	xor eax,eax
	mov ecx,w
loop_2_d_3:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_3
	
	mov esi,edi
	add edi,dst_pitch
	mov ecx,w
	cld
	shl ecx,1
	rep movsd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2 endp


JPSDR_IVTC_Deinterlace_Blend_SSE_2_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_2_a

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_d_1:
	xor eax,eax
	mov ecx,w
loop_2_d_1_1:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_1_1
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_d_2_1:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_2_1
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_d_1

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2_a endp


JPSDR_IVTC_Deinterlace_Blend_SSE_2_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_2_b

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_d_2:
	xor eax,eax
	mov ecx,w
loop_2_d_1_2:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_1_2
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_d_2_2:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_2_2
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_d_2

	xor eax,eax
	mov ecx,w
loop_2_d_3_2:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_3_2
	
	mov esi,edi
	add edi,dst_pitch
	mov ecx,w
	cld
	shl ecx,1
	rep movsd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2_b endp


JPSDR_IVTC_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_3

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,16
	mov edx,src2
	mov edi,dst
loop_1_e:
	xor eax,eax
	mov ecx,w
loop_2_e_1:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_1
	add esi,src_pitch
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_e_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_2
	add edx,src_pitch
	add edi,dst_pitch

	dec h
	jnz short loop_1_e

	xor eax,eax
	mov ecx,w
loop_2_e_3:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_3
	
	mov esi,edi
	add edi,dst_pitch
	mov ecx,w
	cld
	shl ecx,2
	rep movsd

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3 endp


JPSDR_IVTC_Deinterlace_Blend_SSE_3_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_3_a

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,16
	mov edx,src2
	mov edi,dst
loop_1_e_1:
	xor eax,eax
	mov ecx,w
loop_2_e_1_1:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_1_1
	add esi,src_pitch
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_e_2_1:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_2_1
	add edx,src_pitch
	add edi,dst_pitch

	dec h
	jnz short loop_1_e_1

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3_a endp


JPSDR_IVTC_Deinterlace_Blend_SSE_3_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Blend_SSE_3_b

	push esi
	push edi
	push ebx

	mov esi,src1
	mov ebx,16
	mov edx,src2
	mov edi,dst
loop_1_e_2:
	xor eax,eax
	mov ecx,w
loop_2_e_1_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_1_2
	add esi,src_pitch
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_e_2_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_2_2
	add edx,src_pitch
	add edi,dst_pitch

	dec h
	jnz short loop_1_e_2

	xor eax,eax
	mov ecx,w
loop_2_e_3_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_3_2
	
	mov esi,edi
	add edi,dst_pitch
	mov ecx,w
	cld
	shl ecx,2
	rep movsd

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3_b endp


JPSDR_IVTC_Deinterlace_Tri_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_SSE

	push esi
	push edi
	push ebx

	mov ebx,src1
	mov esi,src2
	mov edi,dst
	
	xor eax,eax
	mov ecx,w
loop_1_f_c:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_c
	
	mov edx,ebx
	add edx,src_pitch			;ebx=ln-1   esi=ln    edx=ln+1
	add edi,dst_pitch

loop_1_f:
	xor eax,eax
	mov ecx,w
loop_1_f_a:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_a

	mov ebx,esi
	add ebx,src_pitch			;esi=ln-1	edx=ln	ebx=ln+1
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_1_f_b:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_b

	mov esi,ebx
	mov ebx,edx
	add edx,src_pitch
	add edi,dst_pitch

	dec h
	jnz loop_1_f
	
	xor eax,eax
	mov ecx,w
loop_1_f_d:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_d

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE endp


JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a

	push esi
	push edi
	push ebx

	
	mov ebx,src1
	mov esi,src2
	mov edi,dst
	
	xor eax,eax
	mov ecx,w
loop_1_f_d_1:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_d_1	

	mov edx,ebx
	add edx,src_pitch				;ebx=ln-1   esi=ln    edx=ln+1
	add edi,dst_pitch

loop_1_f_1:
	xor eax,eax
	mov ecx,w
loop_1_f_a_1:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_a_1

	mov ebx,esi
	add ebx,src_pitch			;esi=ln-1	edx=ln	ebx=ln+1
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_1_f_b_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_b_1

	mov esi,ebx
	mov ebx,edx
	add edx,src_pitch             
	add edi,dst_pitch

	dec h
	jnz loop_1_f_1
	
	xor eax,eax
	mov ecx,w
loop_1_f_c_1:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_c_1
	
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_a endp


JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b

	push esi
	push edi
	push ebx

	mov ebx,src1
	mov esi,src2
	mov edx,ebx
	mov eax,src_pitch
	mov edi,dst
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1

loop_1_f_2:
	xor eax,eax
	mov ecx,w
loop_1_f_a_2:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_a_2

	mov ebx,esi
	add ebx,src_pitch			;esi=ln-1	edx=ln	ebx=ln+1
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_1_f_b_2:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_b_2

	mov esi,ebx
	mov ebx,edx
	add edx,src_pitch
	add edi,dst_pitch

	dec h
	jnz loop_1_f_2

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_b endp


JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c

	push esi
	push edi
	push ebx

	mov ebx,src1
	mov esi,src2
	mov edx,ebx
	mov edi,dst
	add edx,src_pitch			;ebx=ln-1   esi=ln    edx=ln+1

loop_1_f_3:
	xor eax,eax
	mov ecx,w
loop_1_f_a_3:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_a_3

	mov ebx,esi
	add ebx,src_pitch			;esi=ln-1	edx=ln	ebx=ln+1
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_1_f_b_3:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_b_3

	mov esi,ebx
	mov ebx,edx
	add edx,src_pitch
	add edi,dst_pitch

	dec h
	jnz loop_1_f_3
	
	xor eax,eax
	mov ecx,w
loop_1_f_c_3:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]	
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_c_3

	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_1_f_d_3:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_1_f_d_3
	
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE_c endp


JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_MMX

	push esi
	push edi
	push ebx

	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3
	
	mov ebx,src1
	mov esi,src2
	mov edi,dst
	
	xor eax,eax
	mov ecx,w
loop_1_g_c:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[esi+4*eax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_c

	mov edx,ebx
	mov eax,src_pitch
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1
	mov eax,dst_pitch
	add edi,eax

loop_1_g:
	xor eax,eax
	mov ecx,w
loop_1_g_a:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm2,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[esi+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_a

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_g_b:
	movd mm0,dword ptr[esi+4*eax]
	movd mm2,dword ptr[ebx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[edx+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_b

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_g

	xor eax,eax
	mov ecx,w
loop_1_g_d:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[esi+4*eax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_d
	
	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX endp


JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a

	push esi
	push edi
	push ebx


	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3
	
	mov ebx,src1
	mov esi,src2
	mov edi,dst
	
	xor eax,eax
	mov ecx,w
loop_1_g_d_1:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[esi+4*eax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_d_1	

	mov edx,ebx
	mov eax,src_pitch
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1
	mov eax,dst_pitch
	add edi,eax

loop_1_g_1:
	xor eax,eax
	mov ecx,w
loop_1_g_a_1:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm2,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[esi+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_a_1

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_g_b_1:
	movd mm0,dword ptr[esi+4*eax]
	movd mm2,dword ptr[ebx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[edx+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_b_1

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_g_1
	
	xor eax,eax
	mov ecx,w
loop_1_g_c_1:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm2,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[esi+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_c_1	

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_a endp


JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b

	push esi
	push edi
	push ebx

	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3

	mov ebx,src1
	mov esi,src2
	mov edx,ebx
	mov eax,src_pitch
	mov edi,dst
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1

loop_1_g_2:
	xor eax,eax
	mov ecx,w
loop_1_g_a_2:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm2,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[esi+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_a_2

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_g_b_2:
	movd mm0,dword ptr[esi+4*eax]
	movd mm2,dword ptr[ebx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[edx+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_b_2

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_g_2

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_b endp


JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c

	push esi
	push edi
	push ebx


	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3

	mov ebx,src1
	mov esi,src2
	mov edx,ebx
	mov eax,src_pitch
	mov edi,dst
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1

loop_1_g_3:
	xor eax,eax
	mov ecx,w
loop_1_g_a_3:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm2,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[esi+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_a_3

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_g_b_3:
	movd mm0,dword ptr[esi+4*eax]
	movd mm2,dword ptr[ebx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[edx+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_b_3

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_g_3
	
	xor eax,eax
	mov ecx,w
loop_1_g_c_3:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm2,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[esi+4*eax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_c_3
	
	mov eax,dst_pitch
	add edi,eax	
	
	xor eax,eax
	mov ecx,w
loop_1_g_d_3:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_1_g_d_3

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX_c endp


JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2

	push esi
	push edi
	push ebx

	xor eax,eax
	mov esi,src
	mov edi,dst
	mov edx,lookup
	cld
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_I_2_1:
	mov ecx,w
Loop_I_2_2:	
	lodsb				; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[edx+2*eax]
	lodsb				; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[edx+2*eax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[edx+2*eax+2048]
	lodsb				; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[edx+2*eax]
	lodsb				; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[edx+2*eax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[edx+2*eax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz Loop_I_2_2
	add esi,src_modulo
	add edi,dst_modulo
	dec h
	jnz Loop_I_2_1
	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 endp


JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2

	push esi
	push edi
	push ebx

	mov esi,src
	mov edi,dst
	mov edx,lookup
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_J_2_1:
	mov ecx,w
Loop_J_2_2:	
	movzx eax,byte ptr[esi+1]		; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[edx+2*eax]
	movzx eax,byte ptr[esi]		; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[edx+2*eax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[edx+2*eax+2048]
	movzx eax,byte ptr[esi+3]		; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[edx+2*eax]
	movzx eax,byte ptr[esi+2]		; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[edx+2*eax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[edx+2*eax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[edi],xmm0
	add esi,4
	add edi,8
	dec ecx
	jnz Loop_J_2_2
	add esi,src_modulo
	add edi,dst_modulo
	dec h
	jnz Loop_J_2_1
	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 endp


JPSDR_IVTC_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_IVTC_Convert420_to_YUY2_1

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	cld

SSE2_1_a:
	movzx eax,byte ptr[esi+1]		;al=y2
	mov ah,byte ptr[edx]		;ah=v
	inc edx
	shl eax,16
	lodsw				;al=y1 ah=y2
	mov ah,byte ptr[ebx]		;ah=u
	inc ebx
	stosd
	loop SSE2_1_a
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Convert420_to_YUY2_1 endp


end





