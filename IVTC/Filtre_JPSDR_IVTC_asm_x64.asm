.code


;JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,dst:dword,histo:dword,
;	repart:dword,w:dword,h:dword,pitch:dword,modulo:dword,w_map:dword,error_motion_map:dword
; src = rcx
; map = rdx
; dst = r8
; histo = r9

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 proc public frame

repart equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]
pitch equ qword ptr[rbp+72]
modulo equ qword ptr[rbp+80]
w_map equ dword ptr[rbp+88]
error_motion_map equ qword ptr[rbp+96]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	cld
	xor rax,rax
	mov rbx,rdx				; rbx = map
	mov rsi,rcx				; rsi = src
	mov rdx,r9				; rdx = histo
	xor rcx,rcx
		
	mov rdi,r9
	mov ecx,128
	rep stosq
	
	mov rdi,r8				;rdi = dst
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	mov r10,repart
	mov r11,pitch
	mov r12,modulo
	mov ecx,w_map
	mov r13,error_motion_map
	mov r14,4

	pxor mm1,mm1	; return value (s)
	pxor mm2,mm2	; error_motion_map
	pxor mm3,mm3

Loop_A_1:
	mov r15d,r8d
	xor rax,rax
Loop_A_2:
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]
	movq mm4,mm0
	movq mm6,mm0
	psadbw mm4,mm3
	paddd mm1,mm4
	punpcklbw mm0,mm3
	pextrw eax,mm0,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,1
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,2
	inc dword ptr[rdx+4*rax]
	mov al,byte ptr[rbx]
	or al,al
	jz short Next_1
	movq mm5,mm6
	paddd mm2,mm4
Next_1:
	mov eax,r8d
	movd dword ptr[rdi],mm5

	pxor mm5,mm5
	movd mm0,dword ptr[rsi+4*rax]
	movq mm4,mm0
	punpcklbw mm0,mm3
	pextrw eax,mm0,0
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,1
	inc dword ptr[rdx+4*rax]
	pextrw eax,mm0,2
	inc dword ptr[rdx+4*rax]
	mov al,byte ptr[rbx+rcx]
	or al,al
	jz short Next_2
	movq mm5,mm4
	psadbw mm4,mm3
	paddd mm2,mm4
Next_2:
	mov rax,r11
	movd dword ptr[rdi+rax],mm5

	add rdi,r14
	add rsi,r14
	inc rbx
	dec r15
	jnz Loop_A_2
	add rdi,r12
	xor rax,rax
	add rdi,r11
	mov eax,ecx
	shl rax,1
	sub rax,r8
	add rbx,rax
	xor rax,rax
	mov eax,r8d
	shl rax,2
	add rsi,rax
	dec r9d
	jnz Loop_A_1

	mov rsi,rdx
	mov rdi,r10
	add rsi,1020
	add rdi,1020
	xor rbx,rbx
	std
	mov ecx,256
Loop_A_3:
	lodsd
	add ebx,eax
	mov eax,ebx
	stosd
	loop Loop_A_3
	cld
	mov rdi,r13
	movd dword ptr[rdi],mm2
	movd eax,mm1

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Histogramme_DeltaPicture_Motion_Map_SSE_RGB32 endp




;JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 proc src:dword,map:dword,w:dword,h:dword,w_map:dword,
;	error_motion_map:dword
; src = rcx
; map = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 proc public frame

w_map equ dword ptr[rbp+48]
error_motion_map equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rbx,rdx
	mov rdi,error_motion_map
	mov r10,4
	mov r11d,w_map
	pxor mm1,mm1
	pxor mm2,mm2
	pxor mm3,mm3
	
Loop_A1_1:
	xor rax,rax
	xor rcx,rcx
	mov r12d,r8d
	mov eax,r8d
	mov ecx,r11d
Loop_A1_2:
	movd mm0,dword ptr[rsi]
	psadbw mm0,mm3
	paddd mm1,mm0
	mov dl,byte ptr[rbx]
	or dl,dl
	jz short Next_A1_1
	paddd mm2,mm0
Next_A1_1:
	mov dl,byte ptr[rbx+rcx]
	or dl,dl
	jz short Next_A1_2
	movd mm0,dword ptr[rsi+4*rax]
	psadbw mm0,mm3
	paddd mm2,mm0
Next_A1_2:
	add rsi,r10
	inc rbx
	dec r12d
	jnz short Loop_A1_2
	shl rcx,1
	sub rcx,rax
	add rbx,rcx
	shl rax,2
	add rsi,rax
	dec r9d
	jnz short Loop_A1_1

	movd dword ptr[rdi],mm2
	movd eax,mm1

	emms
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_DeltaPicture_Motion_Map_SSE_RGB32 endp



;JPSDR_IVTC_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,h:dword,
;	pitch:dword,modulo:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Motion_Map_SSE_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
thr equ dword ptr[rbp+80]
w_map equ dword ptr[rbp+88]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	mov eax,00FCFCFCh
	movd mm7,eax
	
	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r13,4
	xor rcx,rcx
	
	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,r12d
	shr ecx,2
	rep stosd
	mov rbx,rdi
	mov rdi,rdx
	mov rcx,r8
	xor r8,r8
	xor r9,r9
	mov r8d,w
	mov r9d,h
	xor r15,r15

Loop_A0_1:
	mov r14d,r8d
Loop_A0_2:
	mov rdx,r10
	pxor mm5,mm5
	shl rdx,1
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
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
	movd dword ptr[rcx],mm3
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
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
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
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
	movd dword ptr[rcx+4*rdx],mm3
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A0_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	xor rdx,rdx
	mov edx,r8d
	shl rdx,2
	add rcx,rdx
	mov rdx,r11
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A0_1

	xor rax,rax
	mov r15,rcx
	xor rcx,rcx
	xor rdx,rdx
	mov ecx,r8d
	mov edx,ecx
	shl rdx,2
	add rdx,r15
Loop_A0_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	movq mm2,mm0
	pminub mm0,mm1
	pmaxub mm2,mm1
	psubb mm2,mm0
	pand mm2,mm7
	movd dword ptr[r15+4*rax],mm2
	movd dword ptr[rdx+4*rax],mm2
	inc rax
	loop Loop_A0_3

	xor rcx,rcx
	mov ecx,r12d
	xor rax,rax
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Motion_Map_SSE_RGB32 endp




;JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,w:dword,
;	h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
thr equ dword ptr[rbp+80]
w_map equ dword ptr[rbp+88]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7

	mov rsi,rcx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov r13,4
	xor rcx,rcx

	mov rdi,r9
	xor rax,rax
	cld
	mov ecx,r12d
	shr ecx,2
	rep stosd
	mov rbx,rdi
	mov rdi,rdx
	mov rdx,r8
	xor r8,r8
	mov r8d,w
	mov r9d,h

	mov ecx,r8d
	xor rax,rax
Loop_A2_0:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdi+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdx+4*rax],mm0
	inc rax
	loop Loop_A2_0
	shl rax,2
	add rdx,rax
	
	mov rcx,rdx				;rcx=buffer + w
	
Loop_A2_1:
	mov r14d,r8d
Loop_A2_2:
	mov rdx,r10
	shl rdx,1	
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
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
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
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
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
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
	movd dword ptr[rcx+4*rdx],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_A2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	xor rdx,rdx
	mov edx,r8d
	shl rdx,2
	add rcx,rdx
	mov rdx,r11
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_A2_1

	mov rsi,rdi
	mov rdi,rcx
	xor rcx,rcx
	mov ecx,r8d
	cld
	rep movsd

	xor rax,rax
	mov ecx,r12d
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Motion_Map_SSE_RGB32 endp




;JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc src1:dword,src2:dword,buffer:dword,dst:dword,
;	w:dword,h:dword,pitch:dword,modulo:dword,thr:dword,w_map:dword
; src1 = rcx
; src2 = rdx
; buffer = r8
; dst = r9


JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
pitch equ qword ptr[rbp+64]
modulo equ qword ptr[rbp+72]
thr equ dword ptr[rbp+80]
w_map equ dword ptr[rbp+88]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog

	movd mm6,thr
	pxor mm7,mm7
	punpcklbw mm6,mm7
	
	mov r13,rcx
	mov r14,rdx
	mov r10,pitch
	mov r11,modulo
	mov r12d,w_map
	mov rdx,r8
	mov rbx,r9
	xor r8,r8
	mov r8d,w
	mov r9d,h
	xor rcx,rcx

	cld
	xor rax,rax
	mov rdi,rbx
	mov ecx,r12d
	shr ecx,2
	rep stosd
	mov rbx,rdi

	mov rsi,r13
	mov rdi,rdx
	mov ecx,r8d
	rep movsd

	mov rsi,r13
	mov rdi,r14
	mov eax,r8d

	shl rax,2
	add rdx,rax
	
	mov rcx,rdx				;rcx=buffer + w
	mov r13,4
	xor rax,rax

Loop_E2_1:
	mov r14d,r8d
Loop_E2_2:
	mov rdx,r10
	shl rdx,1
	pxor mm5,mm5
	movd mm0,dword ptr[rsi]				;mm0=src1
	movd mm1,dword ptr[rdi]				;mm1=src2
	movd mm2,dword ptr[rsi+rdx]			;mm2=src3
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
	movd dword ptr[rcx],mm7
	mov byte ptr[rbx],al

	pxor mm5,mm5
	movd mm0,dword ptr[rdi+rdx]			;Ici : mm0=src4, mm1=src2, mm2=src3
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
	xor rdx,rdx
	psubb mm3,mm1				; mm3=abs(src2-src3)
	and eax,00FFFFFFh			; lève les 8 bits les plus élevés
	mov edx,r8d
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
	movd dword ptr[rcx+4*rdx],mm7
	mov edx,r12d
	mov byte ptr[rbx+rdx],al

	add rsi,r13
	add rdi,r13
	add rcx,r13
	inc rbx

	dec r14d
	jnz Loop_E2_2

	shl rdx,1
	sub rdx,r8
	add rbx,rdx
	xor rdx,rdx
	mov edx,r8d
	shl rdx,2
	add rcx,rdx
	mov rdx,r11
	add rdx,r10
	add rsi,rdx
	add rdi,rdx

	dec r9d
	jnz Loop_E2_1

	mov rsi,rdi
	mov rdi,rcx
	xor rcx,rcx
	mov ecx,r8d
	cld
	rep movsd

	xor rax,rax
	mov ecx,r12d
	shr ecx,2
	mov rdi,rbx
	rep stosd

	emms
	
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Smart_Deinterlace_Tri_Motion_Map_SSE_RGB32 endp



;JPSDR_IVTC_Norme1_SSE_1_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword
; src1 = rcx
; src2 = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Norme1_SSE_1_RGB32 proc public frame

pitch equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	shl r10,1
	
	xor rcx,rcx
	xor rax,rax
	pxor mm3,mm3
	mov eax,00FC00FCh
	movd mm5,eax
	mov eax,0FCFCFCFCh
	movd mm4,eax
	punpcklbw mm4,mm5
	pxor mm5,mm5
	
Loop_B_1:
	mov ecx,r8d
	xor rdx,rdx
Loop_B_2:
	movd mm1,dword ptr[rsi+4*rdx]
	movd mm0,dword ptr[rdi+4*rdx]
	movq mm2,mm1
	pmaxub mm2,mm0
	pminub mm1,mm0
	psubb mm2,mm1
	pand mm2,mm4
	psadbw mm2,mm3
	inc rdx
	paddd mm5,mm2
	loop loop_B_2
	add rsi,r10
	add rdi,r10
	dec r9d
	jnz short Loop_B_1
	movd eax,mm5

	emms
	
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Norme1_SSE_1_RGB32 endp



;JPSDR_IVTC_Norme1_SSE_2_RGB32 proc src1:dword,src2,w:dword,h:dword,pitch:dword
; src1 = rcx
; src2 = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Norme1_SSE_2_RGB32 proc public frame

pitch equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,pitch
	shl r10,1

	xor rcx,rcx
	xor rax,rax
	pxor mm3,mm3
	mov eax,00FC00FCh
	movd mm5,eax
	mov eax,0FCFCFCFCh
	movd mm4,eax
	punpcklbw mm4,mm5
	pxor mm5,mm5
	
Loop_C_1:
	mov ecx,r8d
	xor rdx,rdx
Loop_C_2:
	movq mm1,qword ptr[rsi+8*rdx]
	movq mm0,qword ptr[rdi+8*rdx]
	movq mm2,mm1
	pmaxub mm2,mm0
	pminub mm1,mm0
	psubb mm2,mm1
	pand mm2,mm4
	psadbw mm2,mm3
	inc rdx
	paddd mm5,mm2
	loop loop_C_2
	add rsi,r10
	add rdi,r10
	dec r9d
	jnz short Loop_C_1
	movd eax,mm5

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Norme1_SSE_2_RGB32 endp





;JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10,r8
	mov r8d,r9d
	mov r9d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rdx,rdx
	mov edx,000000FFh
	xor rcx,rcx
	xor rbx,rbx
	
loop_1:
	mov r13d,r8d
	xor rax,rax
loop_2_1:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc rax
	dec r13d
	jnz short loop_2_1
	add rsi,r11
	add r10,r12

	mov r13d,r8d
	xor rax,rax
loop_2_2:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_2
	add rdi,r11
	add r10,r12

	dec r9d
	jnz loop_1

	mov r13d,r8d
	xor rax,rax
loop_2_3:
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	mov r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr bx,8
	shr cx,8
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl bx,8
	or r14d,ebx
	mov ebx,dword ptr[rsi+4*rax]
	mov ecx,dword ptr[rdi+4*rax]
	shr ebx,16
	shr ecx,16
	and ebx,edx
	and cx,dx
	add bx,cx
	shr bx,1
	shl ebx,16
	or ebx,r14d
	mov dword ptr[r10+4*rax],ebx
	inc eax
	dec r13d
	jnz short loop_2_3
	add r10,r12

	cld
	mov ecx,r8d
	mov rsi,rdi
	mov rdi,r10
	rep movsd

	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_Non_MMX_RGB32 endp




;JPSDR_IVTC_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_c:
	xor rax,rax
	mov ecx,r8d
loop_2_c_1:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_c_2:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_c

	xor rax,rax
	mov ecx,r8d
loop_2_c_3:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm1,dword ptr[rdx+4*rax]
	pavgb mm0,mm1
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_2_c_3
	add rdi,r11

	cld
	mov ecx,r8d
	mov rsi,rdx
	rep movsd

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE endp



;JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_2 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx

loop_1_d:
	xor rax,rax
	mov ecx,r8d
loop_2_d_1:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_d_2:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_d

	xor rax,rax
	mov ecx,r8d
loop_2_d_3:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_2_d_3
	add rdi,r11

	cld
	mov ecx,r8d
	mov rsi,rdx
	rep movsq

	emms
	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_2 endp



;JPSDR_IVTC_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Blend_SSE_3 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,r8
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	mov rbx,16

loop_1_e:
	xor rax,rax
	mov ecx,r8d
loop_2_e_1:
	movdqa xmm0,[rsi+rax]
	pavgb xmm0,[rdx+rax]
	movdqa [rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_1
	add rsi,r10
	add rdi,r11

	xor rax,rax
	mov ecx,r8d
loop_2_e_2:
	movdqa xmm0,[rsi+rax]
	pavgb xmm0,[rdx+rax]
	movdqa [rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_2
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz short loop_1_e

	xor rax,rax
	mov ecx,r8d
loop_2_e_3:
	movdqa xmm0,[rsi+rax]
	pavgb xmm0,[rdx+rax]
	movdqa [rdi+rax],xmm0
	add rax,rbx
	loop loop_2_e_3
	add rdi,r11

	cld
	mov ecx,r8d
	shl rcx,1
	mov rsi,rdx
	rep movsq
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Blend_SSE_3 endp



;JPSDR_IVTC_Deinterlace_Tri_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	cld
	mov r10,rcx
	mov rsi,rcx
	xor rcx,rcx
	mov rdi,r8
	mov ecx,r9d
	rep movsq
	
	mov rdi,r8
	mov rbx,r10	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;ebx=ln-1   esi=ln    edx=ln+1
	xor rcx,rcx
	
loop_1_f:
	mov ecx,r8d
	xor rax,rax
loop_1_f_a:
	movq mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	pavgb mm0,qword ptr[rsi+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_1_f_a

	mov rbx,rsi
	add rbx,r10
	add rdi,r11				;esi=ln-1	edx=ln	ebx=ln+1
	
	mov ecx,r8d
	xor rax,rax
loop_1_f_b:
	movq mm0,qword ptr[rsi+8*rax]
	pavgb mm0,qword ptr[rbx+8*rax]
	pavgb mm0,qword ptr[rdx+8*rax]
	movq qword ptr[rdi+8*rax],mm0
	inc rax
	loop loop_1_f_b

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_f

	mov ecx,r8d
	rep movsq

	emms
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_SSE endp





;JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	cld
	mov r10,rcx
	mov rsi,rcx
	xor rcx,rcx
	mov rdi,r8
	mov ecx,r9d
	rep movsd


	mov eax,02020202h
	pxor mm3,mm3
	movd mm4,eax
	punpcklbw mm4,mm3

	mov rdi,r8
	mov rbx,r10	
	mov rsi,rdx
	mov r8d,r9d
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	mov rdx,rbx
	add rdx,r10
	add rdi,r11				;ebx=ln-1   esi=ln    edx=ln+1
	xor rcx,rcx

loop_1_g:
	mov ecx,r8d
	xor rax,rax
loop_1_g_a:
	movd mm0,dword ptr[rbx+4*rax]
	movd mm2,dword ptr[rdx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rsi+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_a

	mov rbx,rsi
	add rbx,r10				;esi=ln-1	edx=ln	ebx=ln+1
	add rdi,r11

	mov ecx,r8d
	xor rax,rax
loop_1_g_b:
	movd mm0,dword ptr[rsi+4*rax]
	movd mm2,dword ptr[rbx+4*rax]
	punpcklbw mm0,mm3
	movd mm1,dword ptr[rdx+4*rax]	;mm0=ln-1	mm1=ln	mm2=ln+1
	punpcklbw mm2,mm3
	punpcklbw mm1,mm3
	paddw mm0,mm2
	psllw mm1,1
	paddw mm0,mm4
	paddw mm0,mm1
	psrlw mm0,2
	packuswb mm0,mm3
	movd dword ptr[rdi+4*rax],mm0
	inc rax
	loop loop_1_g_b

	mov rsi,rbx
	mov rbx,rdx
	add rdx,r10
	add rdi,r11

	dec r9d
	jnz loop_1_g

	mov ecx,r8d
	rep movsd

	emms
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Deinterlace_Tri_Blend_MMX endp



;JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2 proc src:dword,dst:dword,lookup:dword,_size:dword
; src = rcx
; dst = rdx
; lookup = r8
; _size = r9d

JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2 proc public frame

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov rdx,r8
	xor rcx,rcx
	xor rax,rax
	mov ecx,r9d
	mov r8,8
	cld
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_I:
	lodsb				; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	lodsb				; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[rdi],xmm0
	add rdi,r8
	dec rcx
	jnz Loop_I
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2 endp




;JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword
; src = rcx
; dst = rdx
; lookup = r8
; w = r9d

JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 proc public frame

h equ dword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov rdx,r8
	xor rcx,rcx
	xor rax,rax
	mov r8,8
	mov r10d,h
	mov r11,src_modulo
	cld
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
Loop_I_2:
	mov ecx,r9d
Loop_I_2a:	
	lodsb				; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	lodsb				; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	lodsb				; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[rdi],xmm0
	add rdi,r8
	dec rcx
	jnz Loop_I_2a
	add rsi,r11
	dec r10d
	jnz Loop_I_2
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret
	
JPSDR_IVTC_Convert_YUYV_to_RGB32_SSE2_2 endp



;JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2 proc src:dword,dst:dword,lookup:dword,_size:dword
; src = rcx
; dst = rdx
; lookup = r8
; _size = r9d

JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2 proc public frame

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rdx,r8
	mov ecx,r9d
	mov r8,4
	mov r9,8
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	xor rax,rax
	
Loop_J:
	movzx eax,byte ptr[rsi+1]		; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi]		; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	movzx eax,byte ptr[rsi+3]		; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi+2]		; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[rdi],xmm0
	add rsi,r8
	add rdi,r9
	dec rcx
	jnz Loop_J

	pop rbx
	pop rsi	
	pop rdi
	pop rbp
		
	ret
	
JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2 endp



;JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 proc src:dword,dst:dword,lookup:dword,w:dword,h:dword,src_modulo:dword
; src = rcx
; dst = rdx
; lookup = r8
; w = r9d

JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 proc public frame

h equ dword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rdx,r8
	mov r8,4
	mov r10,8
	mov r11d,h
	mov r12,src_modulo
	
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	xor rax,rax
	
Loop_J_2:
	mov ecx,r9d
Loop_J_2a:
	movzx eax,byte ptr[rsi+1]		; al=Y1
	pxor xmm0,xmm0
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi]		; al=U
	pinsrw xmm0,ebx,0
	pinsrw xmm0,ebx,1
	pinsrw xmm0,ebx,2
	movzx ebx,word ptr[rdx+2*rax+1536]
	pinsrw xmm1,ebx,1
	pinsrw xmm1,ebx,5
	movzx ebx,word ptr[rdx+2*rax+2048]
	movzx eax,byte ptr[rsi+3]		; al=Y2
	pinsrw xmm1,ebx,0
	pinsrw xmm1,ebx,4
	movzx ebx,word ptr[rdx+2*rax]
	movzx eax,byte ptr[rsi+2]		; al=V
	pinsrw xmm0,ebx,4
	pinsrw xmm0,ebx,5
	pinsrw xmm0,ebx,6
	movzx ebx,word ptr[rdx+2*rax+512]
	pinsrw xmm2,ebx,2
	pinsrw xmm2,ebx,6
	movzx ebx,word ptr[rdx+2*rax+1024]
	pinsrw xmm2,ebx,1
	pinsrw xmm2,ebx,5
	paddsw xmm0,xmm1
	paddsw xmm0,xmm2
	psraw xmm0,6
	packuswb xmm0,xmm3
	movq qword ptr[rdi],xmm0
	add rsi,r8
	add rdi,r10
	dec rcx
	jnz Loop_J_2a
	add rsi,r12
	dec r11d
	jnz Loop_J_2

	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
		
	ret
	
JPSDR_IVTC_Convert_UYVY_to_RGB32_SSE2_2 endp



;JPSDR_IVTC_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_IVTC_Convert420_to_YUY2_1 proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog


	mov rsi,rcx
	mov rbx,rdx
	xor rax,rax
	mov rdx,r8
	xor rcx,rcx
	mov rdi,r9
	mov ecx,w
	cld

SSE2_1_a:
	movzx eax,byte ptr[rsi+1]		;al=y2
	mov ah,byte ptr[rdx]		;ah=v
	inc rdx
	shl eax,16
	lodsw				;al=y1 ah=y2
	mov ah,byte ptr[rbx]		;ah=u
	inc rbx
	stosd
	loop SSE2_1_a
	
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Convert420_to_YUY2_1 endp


;JPSDR_IVTC_Rebuild_Frame proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; bottom_src = rcx
; top_src = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Rebuild_Frame proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	cld
	or r9d,r9d
	jz short fin_F4	
	mov rsi,rcx
	mov rdi,r8
	mov rax,src_modulo
	mov rbx,dst_modulo
	mov r10,r8
	mov r8d,r9d
	mov r9d,h
	mov r11,dst_pitch
	mov r12d,r9d
	add rax,src_pitch
	xor rcx,rcx
	add rbx,r11
		
Loop_D_1:
	mov ecx,r8d
	shr ecx,1
	jnc short Loop_D_1a
	movsd
Loop_D_1a:	
	rep movsq
	add rsi,rax
	add rdi,rbx
	dec r12d
	jnz short Loop_D_1
	mov rsi,rdx
	mov rdi,r10
	add rdi,r11
Loop_D_2:
	mov ecx,r8d
	shr ecx,1
	jnc short Loop_D_2a
	movsd
Loop_D_2a:	
	rep movsq
	add rsi,rax
	add rdi,rbx
	dec r9d
	jnz short Loop_D_2

fin_F4:	
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Rebuild_Frame endp


;JPSDR_IVTC_Rebuild_Frame_2 proc src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Rebuild_Frame_2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
dst_modulo equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog	
	
	cld
	mov rdi,rdx
	mov rsi,rcx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_F3
	or r9d,r9d
	jz short fin_F3		
	mov rax,src_pitch
	mov rdx,dst_pitch
	add rax,src_modulo
	add rdx,dst_modulo
		
Loop_D_2_1:
	mov ecx,r8d
	shr ecx,1
	jnc short Loop_D_2_1a
	movsd
Loop_D_2_1a:	
	rep movsq
	add rsi,rax
	add rdi,rdx
	dec r9d
	jnz short Loop_D_2_1
	
fin_F3:	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Rebuild_Frame_2 endp


;JPSDR_IVTC_Rebuild_Frame8 proc bottom_src:dword,top_src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; bottom_src = rcx
; top_src = rdx
; dst = r8
; w = r9d

JPSDR_IVTC_Rebuild_Frame8 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,r8
	mov rax,src_modulo
	mov rbx,dst_modulo
	mov r10,r8
	or r9d,r9d
	jz short fin_F2	
	mov r8d,r9d
	mov r9d,h
	mov r11,dst_pitch
	mov r12d,r9d
	add rax,src_pitch
	xor rcx,rcx
	add rbx,r11
	mov r13d,r8d
	and r13d,7
	shr r8d,3
	
Loop_D8_1:
	or r8d,r8d
	jz short loop_D8_suite1
	mov ecx,r8d
	rep movsq
loop_D8_suite1:
	or r13d,r13d
	jz short loop_D8_suite2
	mov ecx,r13d
	rep movsb
loop_D8_suite2:
	add rsi,rax
	add rdi,rbx
	dec r12d
	jnz short Loop_D8_1
	mov rsi,rdx
	mov rdi,r10
	add rdi,r11
	
Loop_D8_2:
	or r8d,r8d
	jz short loop_D8_suite3
	mov ecx,r8d
	rep movsq
loop_D8_suite3:
	or r13d,r13d
	jz short loop_D8_suite4
	mov ecx,r13d
	rep movsb
loop_D8_suite4:
	add rsi,rax
	add rdi,rbx
	dec r9d
	jnz short Loop_D8_2

fin_F2:	
	pop r13
	pop r12
	pop rbx
	pop rsi	
	pop rdi
	pop rbp
		
	ret

JPSDR_IVTC_Rebuild_Frame8 endp



;JPSDR_IVTC_Rebuild_Frame8_2 proc src:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword,src_modulo:dword,dst_modulo:dword;
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Rebuild_Frame8_2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
dst_modulo equ qword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog	
	
	cld
	mov rdi,rdx
	mov rsi,rcx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_F1
	or r9d,r9d
	jz short fin_F1	
	
	mov rax,src_pitch
	mov rdx,dst_pitch
	add rax,src_modulo
	add rdx,dst_modulo
	mov r10d,r8d
	and r10d,7
	shr r8d,3
	
Loop_D8_2_1:
	or r8d,r8d
	jz short loop_D8_2_suite1
	mov ecx,r8d
	rep movsq
loop_D8_2_suite1:
	or r10d,r10d
	jz short loop_D8_2_suite2
	mov ecx,r10d
	rep movsb
loop_D8_2_suite2:
	add rsi,rax
	add rdi,rdx
	dec r9d
	jnz short Loop_D8_2_1
	
fin_F1:	
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Rebuild_Frame8_2 endp



;JPSDR_IVTC_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Move32_Full proc public frame

src_modulo equ qword ptr[rbp+48]
dst_modulo equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_F
	or r9d,r9d
	jz short fin_F	
	mov r10,src_modulo
	mov r11,dst_modulo
	
loop_F:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_Fa
	movsd
loop_Fa:	
	rep movsq
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_F
	
fin_F:
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move32_Full endp


;JPSDR_IVTC_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Move8_Full proc public frame

src_modulo equ qword ptr[rbp+48]
dst_modulo equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	.endprolog
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_F8
	or r9d,r9d
	jz short fin_F8
	
	mov r10,src_modulo
	mov r11,dst_modulo
	mov ebx,r8d
	and ebx,7
	shr r8d,3
		
loop_F8:
	or r8d,r8d
	jz short loop_F8_suite1
	mov ecx,r8d
	rep movsq
loop_F8_suite1:
	or ebx,ebx
	jz short loop_F8_suite2
	mov ecx,ebx
	rep movsb
loop_F8_suite2:
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short loop_F8
	
fin_F8:
	pop rbx
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move8_Full endp


;JPSDR_IVTC_Move32_Full_src proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Move32_Full_src proc public frame

src_modulo equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_G
	or r9d,r9d
	jz short fin_G
	
	mov rax,src_modulo
		
loop_G:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_Ga
	movsd
loop_Ga:	
	rep movsq
	add rsi,rax
	dec r9d
	jnz short loop_G
	
fin_G:
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move32_Full_src endp


;JPSDR_IVTC_Move8_Full_src proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Move8_Full_src proc public frame

src_modulo equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_G8
	or r9d,r9d
	jz short fin_G8
	
	mov r10,src_modulo
	mov eax,r8d
	and eax,7
	shr r8d,3
	
loop_G8:
	or r8d,r8d
	jz short loop_G8_suite1
	mov ecx,r8d
	rep movsq
loop_G8_suite1:
	or eax,eax
	jz short loop_G8_suite2
	mov ecx,eax
	rep movsb
loop_G8_suite2:
	add rsi,r10
	dec r9d
	jnz short loop_G8
	
fin_G8:
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move8_Full_src endp



;JPSDR_IVTC_Move32_Full_dst proc src:dword,dst:dword,w:dword,h:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Move32_Full_dst proc public frame

dst_modulo equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_H
	or r9d,r9d
	jz short fin_H	
	mov rax,dst_modulo
	
loop_H:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_Ha
	movsd
loop_Ha:	
	rep movsq
	add rdi,rax
	dec r9d
	jnz short loop_H
	
fin_H:
	pop rsi	
	pop rdi
	pop rbp
	
	ret

JPSDR_IVTC_Move32_Full_dst endp


;JPSDR_IVTC_Move8_Full_dst proc src:dword,dst:dword,w:dword,h:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_IVTC_Move8_Full_dst proc public frame

dst_modulo equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	or r8d,r8d
	jz short fin_H8
	or r9d,r9d
	jz short fin_H8
	
	mov r10,dst_modulo
	mov eax,r8d
	and eax,7
	shr r8d,3
	
loop_H8:
	or r8d,r8d
	jz short loop_H8_suite1
	mov ecx,r8d
	rep movsq
loop_H8_suite1:
	or eax,eax
	jz short loop_H8_suite2
	mov ecx,eax
	rep movsb
loop_H8_suite2:	
	add rdi,r10
	dec r9d
	jnz short loop_H8
	
fin_H8:
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move8_Full_dst endp


;JPSDR_IVTC_Move32 proc src:dword,dst:dword,_size:dword
; src = rcx
; dst = rdx
; _size = r8d

JPSDR_IVTC_Move32 proc public frame

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog
	
	cld
	mov rsi,rcx
	xor rcx,rcx
	mov rdi,rdx
	or r8d,r8d
	jz short Move32_fin
	
	mov ecx,r8d
	shr ecx,1
	jnc short move_xx
	movsd
move_xx:	
	rep movsq
	
Move32_fin:	
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move32 endp


;JPSDR_IVTC_Move8 proc src:dword,dst:dword,_size:dword
; src = rcx
; dst = rdx
; _size = r8d

JPSDR_IVTC_Move8 proc public frame

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog
	
	cld
	mov rsi,rcx
	xor rcx,rcx
	mov rdi,rdx
	or r8d,r8d
	jz short Move8_fin

	mov ecx,r8d
	shr ecx,3
	jz short Move8_suite
	rep movsq	
Move8_suite:	
	mov ecx,r8d
	and ecx,7
	jz short Move8_fin
	rep movsb
	
Move8_fin:	
	pop rsi	
	pop rdi
	pop rbp

	ret

JPSDR_IVTC_Move8 endp


end





