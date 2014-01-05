.586
.mmx
.xmm
.model flat,c

.code

JPSDR_BlurMotion_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_BlurMotion_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
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

JPSDR_BlurMotion_Move32_Full endp




JPSDR_BlurMotion_MMX proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_MMX

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,w
	mov edx,s
	mov edi,dst
	pxor mm2,mm2

loop_1_b:
	xor eax,eax
	mov ecx,ebx
loop_2_b:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	punpcklbw mm0,mm2
	punpcklbw mm1,mm2
	paddusw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm2
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_b
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_b

	emms

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_MMX endp



JPSDR_BlurMotion_SSE proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,w
	mov edx,s
	mov edi,dst

loop_1_c:
	xor eax,eax
	mov ecx,ebx
loop_2_c:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_c

	emms

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE endp



JPSDR_BlurMotion_SSE_2 proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_2

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,w
	mov edx,s
	mov edi,dst
loop_1_d:
	xor eax,eax
	mov ecx,ebx
loop_2_d:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_d

	emms

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_2 endp



JPSDR_BlurMotion_SSE_3 proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,16
	mov edx,s
	mov edi,dst

loop_1_e:
	xor eax,eax
	mov ecx,w
loop_2_e:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_e
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_3 endp


JPSDR_BlurMotion_SSE_3_A_U proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3_A_U

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,16
	mov edx,s
	mov edi,dst

loop_1_f:
	xor eax,eax
	mov ecx,w
loop_2_f:
	movdqa xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqu [edi+eax],xmm0
	add eax,ebx
	loop loop_2_f
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_3_A_U endp


JPSDR_BlurMotion_SSE_3_U_A proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3_U_A

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,16
	mov edx,s
	mov edi,dst

loop_1_g:
	xor eax,eax
	mov ecx,w
loop_2_g:
	movdqu xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqa [edi+eax],xmm0
	add eax,ebx
	loop loop_2_g
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_3_U_A endp



JPSDR_BlurMotion_SSE_3_U proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3_U

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,16
	mov edx,s
	mov edi,dst

loop_1_h:
	xor eax,eax
	mov ecx,w
loop_2_h:
	movdqu xmm0,[esi+eax]
	pavgb xmm0,[edx+eax]
	movdqu [edi+eax],xmm0
	add eax,ebx
	loop loop_2_h
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_h

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_3_U endp




end





