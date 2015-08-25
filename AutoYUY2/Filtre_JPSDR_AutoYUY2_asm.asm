.586
.model flat,c

.data

align 16

uw_2_0 dword 4 dup(00000002h)
uw_3_0 dword 4 dup(00000003h)
uw_4_0 dword 4 dup(00000004h)
uw_5_0 dword 4 dup(00000005h)
uw_7_0 dword 4 dup(00000007h)

uw_2_1 dword 4 dup(00020000h)
uw_3_1 dword 4 dup(00030000h)
uw_4_1 dword 4 dup(00040000h)
uw_5_1 dword 4 dup(00050000h)
uw_7_1 dword 4 dup(00070000h)

.code


JPSDR_AutoYUY2_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_AutoYUY2_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
	mov ebx,w
	or ebx,ebx
	jz short fin_F	
	mov edx,h
	or edx,edx
	jz short fin_F	
loop_F:
	mov ecx,ebx
	rep movsd
	add esi,eax
	add edi,dst_modulo
	dec edx
	jnz short loop_F
	
fin_F:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Move32_Full endp


JPSDR_AutoYUY2_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_AutoYUY2_Move8_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov eax,src_modulo
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
	add esi,eax
	add edi,dst_modulo
	dec edx
	jnz short loop_F8
	
fin_F8:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Move8_Full endp


JPSDR_AutoYUY2_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_1

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
	mov al,byte ptr[esi+1]		;al=y2
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

JPSDR_AutoYUY2_Convert420_to_YUY2_1 endp


JPSDR_AutoYUY2_Convert420_to_UYVY_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_1

	push esi
	push edi
	push ebx

	mov esi,src_y
	mov ebx,src_u
	mov edx,src_v
	mov edi,dst
	mov ecx,w
	cld

SSE2_1_b:
	lodsw						;al=y1 ah=y2
	mov al,byte ptr[edx]		;al=v
	inc edx
	shl eax,16
	mov ah,byte ptr[esi-2]		;ah=y1
	mov al,byte ptr[ebx]		;al=u
	inc ebx
	stosd
	loop SSE2_1_b
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_1 endp


.xmm

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	cld
	
	movdqa xmm6,oword ptr uw_4_1
	movdqa xmm5,oword ptr uw_3_1
	movdqa xmm4,oword ptr uw_5_1

	xor eax,eax

SSE2_2_a:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,0
	lodsb
	pinsrw xmm0,eax,2
	lodsb
	mov ebx,src1_u
	pinsrw xmm0,eax,4
	lodsb
	mov edx,src2_u	
	pinsrw xmm0,eax,6
	movzx eax,byte ptr[ebx]					;al=u1 (1)
	add src1_u,2
	pinsrw xmm1,eax,1
	movzx eax,byte ptr[ebx+1]					;al=u1 (2)
	add src2_u,2
	pinsrw xmm1,eax,5
	movzx eax,byte ptr[edx]					;al=u2 (1)
	mov ebx,src1_v
	pinsrw xmm2,eax,1
	movzx eax,byte ptr[edx+1]					;al=u2 (2)
	mov edx,src2_v
	pinsrw xmm2,eax,5
	movzx eax,byte ptr[ebx]					;al=v1 (1)
	add src1_v,2
	pinsrw xmm1,eax,3
	movzx eax,byte ptr[ebx+1]					;al=v1 (2)
	add src2_v,2
	pinsrw xmm1,eax,7
	movzx eax,byte ptr[edx]					;al=v2 (1)
	pinsrw xmm2,eax,3
	movzx eax,byte ptr[edx+1]					;al=v2 (2)
	pinsrw xmm2,eax,7
	
	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz SSE2_2_a
	
	pop ebx
	pop edi
	pop esi

	ret	

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2 endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2

	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm2,xmm2
	pxor xmm1,xmm1
		
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	cld
	
	movdqa xmm6,oword ptr uw_4_0
	movdqa xmm5,oword ptr uw_3_0
	movdqa xmm4,oword ptr uw_5_0
	
	xor eax,eax

SSE2_2_b:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,1
	lodsb
	pinsrw xmm0,eax,3
	lodsb
	mov ebx,src1_u
	pinsrw xmm0,eax,5
	lodsb
	mov edx,src2_u	
	pinsrw xmm0,eax,7
	movzx eax,byte ptr[ebx]					;al=u1 (1)
	add src1_u,2
	pinsrw xmm1,eax,0
	movzx eax,byte ptr[ebx+1]					;al=u1 (2)
	add src2_u,2
	pinsrw xmm1,eax,4
	movzx eax,byte ptr[edx]					;al=u2 (1)
	mov ebx,src1_v
	pinsrw xmm2,eax,0
	movzx eax,byte ptr[edx+1]					;al=u2 (2)
	mov edx,src2_v
	pinsrw xmm2,eax,4
	movzx eax,byte ptr[ebx]					;al=v1 (1)
	add src1_v,2
	pinsrw xmm1,eax,2
	movzx eax,byte ptr[ebx+1]					;al=v1 (2)
	add src2_v,2
	pinsrw xmm1,eax,6
	movzx eax,byte ptr[edx]					;al=v2 (1)
	pinsrw xmm2,eax,2
	movzx eax,byte ptr[edx+1]					;al=v2 (2)
	pinsrw xmm2,eax,6
	
	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz SSE2_2_b
	
	pop ebx
	pop edi
	pop esi

	ret	

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2 endp



JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3

	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm1,xmm1
	pxor xmm2,xmm2
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	cld
	
	movdqa xmm6,oword ptr uw_4_1
	movdqa xmm5,oword ptr uw_7_1

	xor eax,eax	

SSE2_3_a:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,0
	lodsb
	pinsrw xmm0,eax,2
	lodsb
	mov ebx,src1_u
	pinsrw xmm0,eax,4
	lodsb
	mov edx,src2_u	
	pinsrw xmm0,eax,6
	movzx eax,byte ptr[ebx]					;al=u1 (1)
	add src1_u,2
	pinsrw xmm1,eax,1
	movzx eax,byte ptr[ebx+1]					;al=u1 (2)
	add src2_u,2
	pinsrw xmm1,eax,5
	movzx eax,byte ptr[edx]					;al=u2 (1)
	mov ebx,src1_v
	pinsrw xmm2,eax,1
	movzx eax,byte ptr[edx+1]					;al=u2 (2)
	mov edx,src2_v
	pinsrw xmm2,eax,5
	movzx eax,byte ptr[ebx]					;al=v1 (1)
	add src1_v,2
	pinsrw xmm1,eax,3
	movzx eax,byte ptr[ebx+1]					;al=v1 (2)
	add src2_v,2
	pinsrw xmm1,eax,7
	movzx eax,byte ptr[edx]					;al=v2 (1)
	pinsrw xmm2,eax,3
	movzx eax,byte ptr[edx+1]					;al=v2 (2)
	pinsrw xmm2,eax,7
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz SSE2_3_a
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3 endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm1,xmm1
	pxor xmm2,xmm2
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	cld
	
	movdqa xmm6,oword ptr uw_4_0
	movdqa xmm5,oword ptr uw_7_0
	
	xor eax,eax		

SSE2_3_b:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,1
	lodsb
	pinsrw xmm0,eax,3
	lodsb
	mov ebx,src1_u
	pinsrw xmm0,eax,5
	lodsb
	mov edx,src2_u	
	pinsrw xmm0,eax,7
	movzx eax,byte ptr[ebx]					;al=u1 (1)
	add src1_u,2
	pinsrw xmm1,eax,0
	movzx eax,byte ptr[ebx+1]					;al=u1 (2)
	add src2_u,2
	pinsrw xmm1,eax,4
	movzx eax,byte ptr[edx]					;al=u2 (1)
	mov ebx,src1_v
	pinsrw xmm2,eax,0
	movzx eax,byte ptr[edx+1]					;al=u2 (2)
	mov edx,src2_v
	pinsrw xmm2,eax,4
	movzx eax,byte ptr[ebx]					;al=v1 (1)
	add src1_v,2
	pinsrw xmm1,eax,2
	movzx eax,byte ptr[ebx+1]					;al=v1 (2)
	add src2_v,2
	pinsrw xmm1,eax,6
	movzx eax,byte ptr[edx]					;al=v2 (1)
	pinsrw xmm2,eax,2
	movzx eax,byte ptr[edx+1]					;al=v2 (2)
	pinsrw xmm2,eax,6
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz SSE2_3_b
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3 endp


JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	cld
	
	movdqa xmm6,oword ptr uw_2_1
	movdqa xmm5,oword ptr uw_3_1
	
	xor eax,eax	

SSE2_4_a:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,0
	lodsb
	pinsrw xmm0,eax,2
	lodsb
	mov ebx,src1_u
	pinsrw xmm0,eax,4
	lodsb
	mov edx,src2_u	
	pinsrw xmm0,eax,6
	movzx eax,byte ptr[ebx]					;al=u1 (1)
	add src1_u,2
	pinsrw xmm1,eax,1
	movzx eax,byte ptr[ebx+1]					;al=u1 (2)
	add src2_u,2
	pinsrw xmm1,eax,5
	movzx eax,byte ptr[edx]					;al=u2 (1)
	mov ebx,src1_v
	pinsrw xmm2,eax,1
	movzx eax,byte ptr[edx+1]					;al=u2 (2)
	mov edx,src2_v
	pinsrw xmm2,eax,5
	movzx eax,byte ptr[ebx]					;al=v1 (1)
	add src1_v,2
	pinsrw xmm1,eax,3
	movzx eax,byte ptr[ebx+1]					;al=v1 (2)
	add src2_v,2
	pinsrw xmm1,eax,7
	movzx eax,byte ptr[edx]					;al=v2 (1)
	pinsrw xmm2,eax,3
	movzx eax,byte ptr[edx+1]					;al=v2 (2)
	pinsrw xmm2,eax,7
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,2
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz SSE2_4_a
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4 endp


JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4
	
	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	
	mov edi,dst
	mov esi,src_y
	mov ecx,w
	cld
	
	movdqa xmm6,oword ptr uw_2_0
	movdqa xmm5,oword ptr uw_3_0
	
	xor eax,eax	

SSE2_4_b:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,1
	lodsb
	pinsrw xmm0,eax,3
	lodsb
	mov ebx,src1_u
	pinsrw xmm0,eax,5
	lodsb
	mov edx,src2_u	
	pinsrw xmm0,eax,7
	movzx eax,byte ptr[ebx]					;al=u1 (1)
	add src1_u,2
	pinsrw xmm1,eax,0
	movzx eax,byte ptr[ebx+1]					;al=u1 (2)
	add src2_u,2
	pinsrw xmm1,eax,4
	movzx eax,byte ptr[edx]					;al=u2 (1)
	mov ebx,src1_v
	pinsrw xmm2,eax,0
	movzx eax,byte ptr[edx+1]					;al=u2 (2)
	mov edx,src2_v
	pinsrw xmm2,eax,4
	movzx eax,byte ptr[ebx]					;al=v1 (1)
	add src1_v,2
	pinsrw xmm1,eax,2
	movzx eax,byte ptr[ebx+1]					;al=v1 (2)
	add src2_v,2
	pinsrw xmm1,eax,6
	movzx eax,byte ptr[edx]					;al=v2 (1)
	pinsrw xmm2,eax,2
	movzx eax,byte ptr[edx+1]					;al=v2 (2)
	pinsrw xmm2,eax,6
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,2
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[edi],xmm0
	add edi,8
	dec ecx
	jnz SSE2_4_b
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4 endp



JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2 proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2

	push esi
	push edi
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ecx,w
	
	mov eax,8
	
SSE2_2_c:
	movq xmm0,qword ptr[edx]
	movq xmm1,qword ptr[esi]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add esi,eax
	pavgb xmm0,xmm1
	add edx,eax
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movq qword ptr[edi],xmm0
	add edi,eax
	loop SSE2_2_c
	
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2 endp



JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b

	push esi
	push edi
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ecx,w
	
	mov eax,8
	
SSE2_2_cb:
	movq xmm0,qword ptr[edx]
	movq xmm1,qword ptr[esi]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add esi,eax
	pavgb xmm0,xmm1
	add edx,eax
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movq qword ptr[edi],xmm0
	add edi,eax
	loop SSE2_2_cb

	movd xmm0,dword ptr[edx]
	movd xmm1,dword ptr[esi]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movd dword ptr[edi],xmm0
			
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b endp



JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3 proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3
	
	push esi
	push edi
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ecx,w
	
	mov eax,8	
	
SSE2_3_c:
	movq xmm0,qword ptr[esi]
	movq xmm1,qword ptr[edx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	add esi,eax
	pavgb xmm1,xmm0
	add edx,eax
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movq qword ptr[edi],xmm1
	add edi,eax
	loop SSE2_3_c
	
	pop edi
	pop esi
	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3 endp



JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b
	
	push esi
	push edi
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ecx,w
	
	mov eax,8	
	
SSE2_3_cb:
	movq xmm0,qword ptr[esi]
	movq xmm1,qword ptr[edx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	add esi,eax
	pavgb xmm1,xmm0
	add edx,eax
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movq qword ptr[edi],xmm1
	add edi,eax
	loop SSE2_3_cb

	movd xmm0,dword ptr[esi]
	movd xmm1,dword ptr[edx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm1,xmm0
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movd dword ptr[edi],xmm1	
		
	pop edi
	pop esi
	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b endp


JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4 proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4
	
	push esi
	push edi
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ecx,w
	
	mov eax,8	
	
SSE2_4_c:
	movq xmm0,qword ptr[esi]
	movq xmm1,qword ptr[edx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add esi,eax
	pxor xmm0,xmm3
	add edx,eax
	pavgb xmm0,xmm2
	
	movq qword ptr[edi],xmm0
	add edi,eax
	loop SSE2_4_c
	
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4 endp


JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b proc src1:dword,src2:dword,dst:dword,w:dword

	public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b
	
	push esi
	push edi
	
	pcmpeqb xmm3,xmm3
	
	mov edi,dst
	mov esi,src1
	mov edx,src2
	mov ecx,w
	
	mov eax,8	
	
SSE2_4_cb:
	movq xmm0,qword ptr[esi]
	movq xmm1,qword ptr[edx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add esi,eax
	pxor xmm0,xmm3
	add edx,eax
	pavgb xmm0,xmm2
	
	movq qword ptr[edi],xmm0
	add edi,eax
	loop SSE2_4_cb

	movd xmm0,dword ptr[esi]
	movd xmm1,dword ptr[edx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movd dword ptr[edi],xmm0
	
		
	pop edi
	pop esi

	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b endp

end





