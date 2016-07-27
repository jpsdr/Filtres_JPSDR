.586
.mmx
.xmm
.model flat,c

.code


JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
	repart:dword,w:dword,h:dword,pitch:dword,modulo:dword,w_map:dword,error_motion_map:dword

	public JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32

	local w_loop:dword

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
	mov ebx,map
	mov ecx,w_map

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
	mov eax,w
	movd dword ptr[edi],mm5

	pxor mm5,mm5
	movd mm0,dword ptr[esi+4*eax]
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
	mov eax,pitch
	movd dword ptr[edi+eax],mm5

	add edi,4
	add esi,4
	inc ebx
	dec w_loop
	jnz Loop_A_2
	add edi,modulo
	add edi,pitch
	mov eax,ecx
	shl eax,1
	sub eax,w
	add ebx,eax
	mov eax,w
	shl eax,2
	add esi,eax
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




JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
	error_motion_map:dword

	public JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32

	local w_loop:dword

	push esi
	push edi
	push ebx

	mov esi,src
	pxor mm1,mm1
	pxor mm2,mm2
	pxor mm3,mm3
	mov ebx,map
Loop_A1_1:
	mov eax,w
	mov w_loop,eax
	mov ecx,w_map
Loop_A1_2:
	movd mm0,dword ptr[esi]
	psadbw mm0,mm3
	paddd mm1,mm0
	mov dl,byte ptr[ebx]
	or dl,dl
	jz short Next_A1_1
	paddd mm2,mm0
Next_A1_1:
	mov dl,byte ptr[ebx+ecx]
	or dl,dl
	jz short Next_A1_2
	movd mm0,dword ptr[esi+4*eax]
	psadbw mm0,mm3
	paddd mm2,mm0
Next_A1_2:
	add esi,4
	inc ebx
	dec w_loop
	jnz short Loop_A1_2
	shl ecx,1
	sub ecx,eax
	add ebx,ecx
	shl eax,2
	add esi,eax
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



JPSDR_IVTC_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	jz short Next_A0_1			; Si tous négatif, pas la peine continuer, EAX=0 => AL=0
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
	mov edx,w
	xor eax,00FFFFFFh			; Si tous négatif, pas la peine continuer.
	jz short Next_A0_2
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
Next_A0_2:
	pand mm3,mm7
	movd dword ptr[ecx+4*edx],mm3
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
	add esi,edx
	add edi,edx

	dec h
	jnz Loop_A0_1

	xor eax,eax
	push ebx
	mov ebx,ecx
	mov ecx,w
	mov edx,ecx
	shl edx,2
	add edx,ebx
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




JPSDR_IVTC_Motion_Map_SSE_RGB32_a proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	pand mm3,mm7
	movd dword ptr[ecx+4*edx],mm3
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
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


JPSDR_IVTC_Motion_Map_SSE_RGB32_b proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	pand mm3,mm7
	movd dword ptr[ecx+4*edx],mm3
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
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



JPSDR_IVTC_Motion_Map_SSE_RGB32_c proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
	pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	pand mm3,mm7
	movd dword ptr[ecx+4*edx],mm3
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
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
	mov edx,w
	pand mm3,mm7
	movd dword ptr[ecx+4*edx],mm3	

	add esi,4
	add edi,4
	add ecx,4
	inc ebx

	dec w_loop
	jnz Loop_A0_2_3_c
	
	mov edx,w_map
	sub edx,w
	add ebx,edx
	
	mov edx,ecx	
	xor eax,eax
	mov ecx,w_map
	shr ecx,2
	mov edi,ebx
	rep stosd	
	mov ecx,edx
	
	mov esi,ecx
	mov edx,w
	shl edx,2
	add ecx,edx	
	mov edi,ecx
	mov ecx,w
	rep stosd

	emms
	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32_c endp



JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
	h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	shl eax,2
	add edx,eax
	
	mov ecx,edx				;ecx=buffer + w
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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	shl eax,2
	add edx,eax
	
	mov ecx,edx				;ecx=buffer + w
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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	
	add ebx,w_map
	sub ebx,w

	mov esi,ecx
	mov eax,w
	shl eax,2
	sub esi,eax
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
	w:dword,h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	shl eax,2
	add edx,eax

	mov ecx,edx				;ecx=buffer + w
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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	w:dword,h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	shl eax,2
	add edx,eax
	
	mov ecx,edx				;ecx=buffer + w
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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	w:dword,h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	w:dword,h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword

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
	mov edx,w
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
	movd dword ptr[ecx+4*edx],mm7
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
	mov edx,w
	shl edx,2
	add ecx,edx
	mov edx,modulo
	add edx,pitch
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
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_e:
	xor eax,eax
	mov ecx,w
loop_2_e_1:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
	loop loop_2_e_1
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_e_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
	loop loop_2_e_2
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_e

	xor eax,eax
	mov ecx,w
loop_2_e_3:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
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
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_e_1:
	xor eax,eax
	mov ecx,w
loop_2_e_1_1:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
	loop loop_2_e_1_1
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_e_2_1:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
	loop loop_2_e_2_1
	add edx,ebx
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
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
loop_1_e_2:
	xor eax,eax
	mov ecx,w
loop_2_e_1_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
	loop loop_2_e_1_2
	add esi,ebx
	add edi,dst_pitch

	xor eax,eax
	mov ecx,w
loop_2_e_2_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
	loop loop_2_e_2_2
	add edx,ebx
	add edi,dst_pitch

	dec h
	jnz short loop_1_e_2

	xor eax,eax
	mov ecx,w
loop_2_e_3_2:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,16
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
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_c
	
	mov edx,ebx
	mov eax,src_pitch
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1
	mov eax,dst_pitch
	add edi,eax

loop_1_f:
	xor eax,eax
	mov ecx,w
loop_1_f_a:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]	
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_a

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_f_b:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_b

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_f
	
	xor eax,eax
	mov ecx,w
loop_1_f_d:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_d

	emms
	
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
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_d_1	

	mov edx,ebx
	mov eax,src_pitch
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1
	mov eax,dst_pitch
	add edi,eax

loop_1_f_1:
	xor eax,eax
	mov ecx,w
loop_1_f_a_1:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]	
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_a_1

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_f_b_1:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_b_1

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax             
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_f_1
	
	xor eax,eax
	mov ecx,w
loop_1_f_c_1:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]	
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_c_1
	
	emms
	
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
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]	
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_a_2

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_f_b_2:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_b_2

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_f_2

	emms
	
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
	mov eax,src_pitch
	mov edi,dst
	add edx,eax				;ebx=ln-1   esi=ln    edx=ln+1

loop_1_f_3:
	xor eax,eax
	mov ecx,w
loop_1_f_a_3:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]	
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_a_3

	mov eax,src_pitch
	mov ebx,esi
	add ebx,eax				;esi=ln-1	edx=ln	ebx=ln+1
	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_f_b_3:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_b_3

	mov eax,src_pitch
	mov esi,ebx
	mov ebx,edx
	add edx,eax
	mov eax,dst_pitch
	add edi,eax

	dec h
	jnz loop_1_f_3
	
	xor eax,eax
	mov ecx,w
loop_1_f_c_3:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]	
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_c_3

	mov eax,dst_pitch
	add edi,eax

	xor eax,eax
	mov ecx,w
loop_1_f_d_3:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_1_f_d_3
	
	emms
	
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



JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2 proc src:dword,dst:dword,lookup:dword,_size:dword

	public JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2

	push esi
	push edi
	push ebx

	xor eax,eax
	mov esi,src
	mov edi,dst
	mov edx,lookup
	mov ecx,_size
	cld
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_I:
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
	jnz Loop_I
	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2 endp



JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword

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
	dec h
	jnz Loop_I_2_1
	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 endp



JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2 proc src:dword,dst:dword,lookup:dword,_size:dword

	public JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2

	push esi
	push edi
	push ebx

	mov esi,src
	mov edi,dst
	mov edx,lookup
	mov ecx,_size
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_J:
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
	jnz Loop_J
	
	pop ebx
	pop edi
	pop esi

	ret
	
JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2 endp


JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword

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

JPSDR_IVTC_Rebuild_Frame proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Rebuild_Frame

	push esi
	push edi
	push ebx

	cld
	mov esi,bottom_src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
Loop_D_1:
	mov ecx,w
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D_1
	mov esi,top_src
	mov edi,dst
	mov edx,h
	add edi,dst_pitch
Loop_D_2:
	mov ecx,w
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D_2

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Rebuild_Frame endp



JPSDR_IVTC_Rebuild_Frame_2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword,
	src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Rebuild_Frame_2

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
Loop_D_2_1:
	mov ecx,w
	rep movsd
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D_2_1

	pop ebx
	pop edi
	pop esi
	ret

JPSDR_IVTC_Rebuild_Frame_2 endp


JPSDR_IVTC_Rebuild_Frame8 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Rebuild_Frame8

	push esi
	push edi
	push ebx

	cld
	mov esi,bottom_src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
	
Loop_D8_1:
	mov ecx,w
	shr ecx,2
	jz short loop_D8_suite1
	rep movsd
loop_D8_suite1:
	mov ecx,w
	and ecx,3
	jz short loop_D8_suite2
	rep movsb
loop_D8_suite2:
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D8_1
	mov esi,top_src
	mov edi,dst
	mov edx,h
	add edi,dst_pitch
	
Loop_D8_2:
	mov ecx,w
	shr ecx,2
	jz short loop_D8_suite3
	rep movsd
loop_D8_suite3:
	mov ecx,w
	and ecx,3
	jz short loop_D8_suite4
	rep movsb
loop_D8_suite4:
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D8_2

	pop ebx
	pop edi
	pop esi
	
	ret

JPSDR_IVTC_Rebuild_Frame8 endp


JPSDR_IVTC_Rebuild_Frame8_2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword,
	src_modulo:dword,dst_modulo:dword;

	public JPSDR_IVTC_Rebuild_Frame8_2

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,dst_modulo
	add eax,src_pitch
	add ebx,dst_pitch
	mov edx,h
	
Loop_D8_2_1:
	mov ecx,w
	shr ecx,2
	jz short loop_D8_2_suite1
	rep movsd
loop_D8_2_suite1:
	mov ecx,w
	and ecx,3
	jz short loop_D8_2_suite2
	rep movsb
loop_D8_2_suite2:
	add esi,eax
	add edi,ebx
	dec edx
	jnz short Loop_D8_2_1

	pop ebx
	pop edi
	pop esi
	
	ret

JPSDR_IVTC_Rebuild_Frame8_2 endp


JPSDR_IVTC_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_IVTC_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	or ebx,ebx
	jz short fin_F			
	mov edx,h
	or edx,edx
	jz short fin_F		
	
loop_F:
	mov ecx,ebx
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_F
	
fin_F:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move32_Full endp


JPSDR_IVTC_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_IVTC_Move8_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
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
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_F8
	
fin_F8:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move8_Full endp


JPSDR_IVTC_Move32_Full_src proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword

	public JPSDR_IVTC_Move32_Full_src

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	or ebx,ebx
	jz short fin_G	
	mov edx,h
	or edx,edx
	jz short fin_G	
	mov eax,src_modulo
	
loop_G:
	mov ecx,ebx
	rep movsd
	add esi,eax
	dec edx
	jnz short loop_G
	
fin_G:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move32_Full_src endp


JPSDR_IVTC_Move8_Full_src proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword

	public JPSDR_IVTC_Move8_Full_src

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	or ebx,ebx
	jz short fin_G8
	mov edx,h
	or edx,edx
	jz short fin_G8
	mov eax,src_modulo
	
loop_G8:
	mov ecx,ebx
	shr ecx,2
	jz short loop_G8_suite1
	rep movsd
loop_G8_suite1:	
	mov ecx,ebx
	and ecx,3
	jz short loop_G8_suite2
	rep movsb
loop_G8_suite2:	
	add esi,eax
	dec edx
	jnz short loop_G8
	
fin_G8:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move8_Full_src endp



JPSDR_IVTC_Move32_Full_dst proc src:dword,dst:dword,w:dword,h:dword,dst_modulo:dword

	public JPSDR_IVTC_Move32_Full_dst

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	or ebx,ebx
	jz short fin_H
	mov edx,h
	or edx,edx
	jz short fin_H
	mov eax,dst_modulo
	
loop_H:
	mov ecx,ebx
	rep movsd
	add edi,eax
	dec edx
	jnz short loop_H
	
fin_H:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move32_Full_dst endp


JPSDR_IVTC_Move8_Full_dst proc src:dword,dst:dword,w:dword,h:dword,dst_modulo:dword

	public JPSDR_IVTC_Move8_Full_dst

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	or ebx,ebx
	jz short fin_H8
	mov edx,h
	or edx,edx
	jz short fin_H8
	mov eax,dst_modulo
	
loop_H8:
	mov ecx,ebx
	shr ecx,2
	jz short loop_H8_suite1
	rep movsd	
loop_H8_suite1:
	mov ecx,ebx
	and ecx,3
	jz short loop_H8_suite2
	rep movsb
loop_H8_suite2:
	add edi,eax
	dec edx
	jnz short loop_H8
	
fin_H8:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move8_Full_dst endp


JPSDR_IVTC_Move32 proc src:dword,dst:dword,_size:dword

	public JPSDR_IVTC_Move32

	push esi
	push edi
	
	cld
	mov esi,src
	mov edi,dst
	mov ecx,_size
	or ecx,ecx
	jz short Move32_fin
	
	rep movsd
	
Move32_fin:	
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move32 endp


JPSDR_IVTC_Move8 proc src:dword,dst:dword,_size:dword

	public JPSDR_IVTC_Move8

	push esi
	push edi
	
	mov edx,_size
	or edx,edx
	jz short Move8_fin
	
	cld
	mov esi,src
	mov edi,dst
	
	mov ecx,edx
	shr ecx,2	
	jz short Move8_suite
	rep movsd
Move8_suite:	
	mov ecx,edx
	and ecx,3
	jz short Move8_fin
	rep movsb
	
Move8_fin:	
	pop edi
	pop esi

	ret

JPSDR_IVTC_Move8 endp


end





