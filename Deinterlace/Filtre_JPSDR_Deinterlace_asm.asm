.586
.model flat,c

.code


JPSDR_Deinterlace_Move32_Half proc src:dword,dst:dword,w:dword,h:dword,src_modulo_pitch:dword,dst_modulo_pitch:dword

	public JPSDR_Deinterlace_Move32_Half

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_1_f:
	mov ecx,ebx
	rep movsd
	add esi,src_modulo_pitch
	add edi,dst_modulo_pitch
	dec edx
	jnz short loop_1_f
	
fin_f:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Move32_Half endp


JPSDR_Deinterlace_Move8_Half proc src:dword,dst:dword,w:dword,h:dword,src_modulo_pitch:dword,dst_modulo_pitch:dword

	public JPSDR_Deinterlace_Move8_Half

	push esi
	push edi
	push ebx

	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_1_f_2:
	mov ecx,ebx
	and ecx,3
	jz short loop_2_f_2
	rep movsb
loop_2_f_2:
	mov ecx,ebx	
	shr ecx,2
	rep movsd
	add esi,src_modulo_pitch
	add edi,dst_modulo_pitch
	dec edx
	jnz short loop_1_f_2
	
fin_f_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Move8_Half endp



JPSDR_Deinterlace_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Deinterlace_Move32_Full

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

JPSDR_Deinterlace_Move32_Full endp



JPSDR_Deinterlace_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_Deinterlace_Move8_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_1_f_4:
	mov ecx,ebx
	and ecx,3
	jz short loop_2_f_4
	rep movsb
loop_2_f_4:
	mov ecx,ebx	
	shr ecx,2
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_1_f_4
	
fin_f_4:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Move8_Full endp



JPSDR_Deinterlace_Blend_Non_MMX_24 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Non_MMX_24

	local res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2

loop_1:
	mov ecx,w
	xor eax,eax
loop_2:
	movzx edx,byte ptr[edi+4*eax]
	movzx ebx,byte ptr[esi+4*eax]
	add bx,dx
	shr bx,1
	mov res,ebx
	movzx edx,byte ptr[edi+4*eax+1]
	movzx ebx,byte ptr[esi+4*eax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or res,ebx
	movzx edx,byte ptr[edi+4*eax+2]
	movzx ebx,byte ptr[esi+4*eax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	loop loop_2
	add esi,src_pitch
	add edi,src_pitch
	add edx,dst_pitch
	mov dst,edx
	dec h
	jnz short loop_1
	
	
fin_:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Non_MMX_24 endp


JPSDR_Deinterlace_Blend_Non_MMX_32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Non_MMX_32

	local res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2

loop_1_i:
	mov ecx,w
	xor eax,eax
loop_2_i:
	movzx edx,byte ptr[edi+4*eax]
    movzx ebx,byte ptr[esi+4*eax]
	add bx,dx
	shr bx,1
	mov res,ebx
	movzx edx,byte ptr[edi+4*eax+1]
	movzx ebx,byte ptr[esi+4*eax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or res,ebx
	movzx edx,byte ptr[edi+4*eax+2]
	movzx ebx,byte ptr[esi+4*eax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or res,ebx
	movzx edx,byte ptr[edi+4*eax+3]
	movzx ebx,byte ptr[esi+4*eax+3]
	add bx,dx
	shr bx,1
	shl ebx,24
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+4*eax],ebx
	inc eax
	loop loop_2_i
	add esi,src_pitch
	add edi,src_pitch
	add edx,dst_pitch
	mov dst,edx
	dec h
	jnz short loop_1_i
	
fin_i:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Non_MMX_32 endp


JPSDR_Deinterlace_Blend_Non_MMX_8 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Non_MMX_8

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2
	xor ebx,ebx

loop_1_j:
	mov ecx,w
	xor eax,eax
loop_2_j:
    movzx ebx,byte ptr[esi+eax]
	movzx edx,byte ptr[edi+eax]
	add bx,dx
	shr bx,1
	mov edx,dst
	mov byte ptr[edx+eax],bl
	inc eax
	loop loop_2_j
	add esi,src_pitch
	add edi,src_pitch
	add edx,dst_pitch
	mov dst,edx
	dec h
	jnz short loop_1_j
	
fin_j:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Non_MMX_8 endp

.mmx

JPSDR_Deinterlace_Blend_MMX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_MMX

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst
	pxor mm2,mm2

loop_1_b:
	xor eax,eax
	mov ecx,w
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
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_b

	emms
	
fin_b:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_MMX endp


JPSDR_Deinterlace_Blend_Tri_MMX proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_MMX

	push esi
	push edi
	push ebx
	
	pxor mm3,mm3
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_h:	
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[ebx+4*eax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddusw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_0_h
	
	mov eax,02020202h
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	movd mm4,eax
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1
	punpcklbw mm4,mm3
	
loop_1_h:
	xor eax,eax
	mov ecx,w
loop_2_h:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	punpcklbw mm0,mm3
	movd mm2,dword ptr[esi+4*eax]
	punpcklbw mm1,mm3
	punpcklbw mm2,mm3		;mm0=ln-1 mm1=ln+1 mm2=ln
	paddw mm1,mm0
	psllw mm2,1
	paddw mm1,mm2
	paddw mm1,mm4
	psrlw mm1,2
	packuswb mm1,mm3
	movd dword ptr[edi+4*eax],mm1
	inc eax
	loop loop_2_h
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_h

	mov ecx,w
	xor eax,eax
loop_3_h:	
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[esi+4*eax]
	punpcklbw mm0,mm3
	punpcklbw mm1,mm3
	paddusw mm0,mm1
	psrlw mm0,1
	packuswb mm0,mm3
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_3_h

	emms
	
fin_h:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_MMX endp

.xmm





JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc src_a:dword,src_b:dword,dst_abs:dword,dst_avg:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword

	public JPSDR_Deinterlace_YadifAbsDiffAvg_SSE
	
	push esi
	push edi
	push ebx
	
	mov esi,src_a
	mov edi,src_b
	mov ebx,dst_abs
	mov edx,dst_avg
	
loop_1_Yadif1:
	xor eax,eax
	mov ecx,w
loop_2_Yadif1:
	movdqa xmm0,[esi+eax]		; xmm0=src_a
	movdqa xmm1,[edi+eax]		; xmm1=src_b
	movdqa xmm2,xmm0
	movdqa xmm3,xmm0
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	pminub xmm3,xmm1			; xmm3=min(src_a,src_b)
	pavgb xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	psubb xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	movdqa [edx+eax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	movdqa [ebx+eax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add eax,16
	loop loop_2_Yadif1
	add esi,src_pitch
	add edi,src_pitch
	add ebx,dst_pitch
	add edx,dst_pitch
	dec h
	jnz short loop_1_Yadif1
	
	pop ebx
	pop edi
	pop esi
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiffAvg_SSE endp


JPSDR_Deinterlace_YadifAbsDiff_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword

	public JPSDR_Deinterlace_YadifAbsDiff_SSE
	
	push esi
	push edi
	push ebx
	
	mov esi,src_a
	mov ebx,src_b
	mov edi,dst
	mov edx,16
	
loop_1_Yadif2:
	xor eax,eax
	mov ecx,w
loop_2_Yadif2:
	movdqa xmm0,[esi+eax]		; xmm0=src_a
	movdqa xmm1,[ebx+eax]		; xmm1=src_b
	movdqa xmm2,xmm0
	pminub xmm0,xmm1			; xmm0=min(src_a,src_b)	
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	psubb xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	movdqa [edi+eax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add eax,edx
	loop loop_2_Yadif2
	add esi,src_pitch
	add ebx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_Yadif2
	
	pop ebx
	pop edi
	pop esi
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiff_SSE endp
	


JPSDR_Deinterlace_YadifAvg_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword

	public JPSDR_Deinterlace_YadifAvg_SSE
	
	push esi
	push edi
	push ebx
	
	mov esi,src_a
	mov ebx,src_b
	mov edi,dst
	mov edx,16
	
loop_1_Yadif3:
	xor eax,eax
	mov ecx,w
loop_2_Yadif3:
	movdqa xmm0,[esi+eax]		; xmm0=src_a
	pavgb xmm0,[ebx+eax]		; xmm0=(src_a+src_b+1)>>1
	movdqa [edi+eax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
	add eax,edx
	loop loop_2_Yadif3
	add esi,src_pitch
	add ebx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_Yadif3
	
	pop ebx
	pop edi
	pop esi
	
	ret
	
JPSDR_Deinterlace_YadifAvg_SSE endp




JPSDR_Deinterlace_Blend_SSE proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE

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
loop_2_c:
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_c
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_c

	emms
	
fin_c:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE endp



JPSDR_Deinterlace_Blend_SSE_2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_2

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
loop_2_d:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_d

	emms
	
fin_d:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_2 endp



JPSDR_Deinterlace_Blend_SSE_2b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_2b

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
loop_2_d_2:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_2
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_d_2

	emms
	
fin_d_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_2b endp


JPSDR_Deinterlace_Blend_SSE_2c proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_2c

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst

loop_1_d_3:
	xor eax,eax
	mov ecx,w
loop_2_d_3:
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_d_3
	
	movd mm0,dword ptr[esi+8*eax]
	movd mm1,dword ptr[edx+8*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+8*eax],mm0
	
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_d_3
	
	emms

fin_d_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_2c endp


JPSDR_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3

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
loop_2_e:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e
	
fin_e:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3 endp



JPSDR_Deinterlace_Blend_SSE_3b proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3b

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
loop_2_e_2:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_2

	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[edx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0	
	
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_2
	
fin_e_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3b endp


JPSDR_Deinterlace_Blend_SSE_3_A_U proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_A_U

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
loop_2_e_1:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_1
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_1
	
fin_e_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_A_U endp



JPSDR_Deinterlace_Blend_SSE_3_A_Ub proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_A_Ub

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst

loop_1_e_1_2:
	xor eax,eax
	mov ecx,w
loop_2_e_1_2:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_1_2

	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[edx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_1_2
	
fin_e_1_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_A_Ub endp


JPSDR_Deinterlace_Blend_SSE_3_U_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_U_A

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
loop_2_e_2:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[edx+eax]
	pavgb xmm0,xmm1
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_2
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_2
	
fin_e_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_U_A endp



JPSDR_Deinterlace_Blend_SSE_3_U_Ab proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_U_Ab

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst

loop_1_e_2_2:
	xor eax,eax
	mov ecx,w
loop_2_e_2_2:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[edx+eax]
	pavgb xmm0,xmm1
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_2_2

	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[edx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_2_2
	
fin_e_2_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_U_Ab endp


JPSDR_Deinterlace_Blend_SSE_3_U proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_U

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst

loop_1_e_3:
	xor eax,eax
	mov ecx,w
loop_2_e_3:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[edx+eax]
	pavgb xmm0,xmm1
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_3
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_3
	
fin_e_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_U endp



JPSDR_Deinterlace_Blend_SSE_3_Ub proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_Ub

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,src_pitch
	mov edx,src2
	mov edi,dst

loop_1_e_3_2:
	xor eax,eax
	mov ecx,w
loop_2_e_3_2:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[edx+eax]
	pavgb xmm0,xmm1
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_e_3_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[edx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	add esi,ebx
	add edi,dst_pitch
	add edx,ebx
	dec h
	jnz short loop_1_e_3_2
	
fin_e_3_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_Ub endp



JPSDR_Deinterlace_Blend_Tri_SSE proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g:	
	movd mm0,dword ptr[esi+4*eax]
	movd mm1,dword ptr[ebx+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_0_g

	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g:
	xor eax,eax
	mov ecx,w
loop_2_g:
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[edx+4*eax]
	movd mm2,dword ptr[esi+4*eax]
	pavgb mm0,mm1
	pavgb mm0,mm2
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_2_g
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g

	mov ecx,w
	xor eax,eax
loop_3_g:	
	movd mm0,dword ptr[ebx+4*eax]
	movd mm1,dword ptr[esi+4*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+4*eax],mm0
	inc eax
	loop loop_3_g
	
fin_g:
	emms
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE endp


JPSDR_Deinterlace_Blend_Tri_SSE_2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_2

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_1:	
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_0_g_1
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_1:
	xor eax,eax
	mov ecx,w
loop_2_g_1:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_g_1
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_1

	mov ecx,w
	xor eax,eax
loop_3_g_1:	
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_3_g_1
	
fin_g_1:
	emms
	
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_2 endp



JPSDR_Deinterlace_Blend_Tri_SSE_2b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_2b

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_1_2:	
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_0_g_1_2
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_1_2:
	xor eax,eax
	mov ecx,w
loop_2_g_1_2:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_g_1_2
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_1_2

	mov ecx,w
	xor eax,eax
loop_3_g_1_2:	
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_3_g_1_2
	
	emms
	
fin_g_1_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_2b endp



JPSDR_Deinterlace_Blend_Tri_SSE_2c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_2c

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_1_3:	
	movq mm0,qword ptr[esi+8*eax]
	pavgb mm0,qword ptr[ebx+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_0_g_1_3

	movd mm0,dword ptr[esi+8*eax]
	movd mm1,dword ptr[ebx+8*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+8*eax],mm0
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_1_3:
	xor eax,eax
	mov ecx,w
loop_2_g_1_3:
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[edx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_2_g_1_3
	
	movd mm0,dword ptr[ebx+8*eax]
	movd mm1,dword ptr[edx+8*eax]
	movd mm2,dword ptr[esi+8*eax]
	pavgb mm0,mm1
	pavgb mm0,mm2
	movd dword ptr[edi+8*eax],mm0
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_1_3

	mov ecx,w
	xor eax,eax
loop_3_g_1_3:	
	movq mm0,qword ptr[ebx+8*eax]
	pavgb mm0,qword ptr[esi+8*eax]
	movq qword ptr[edi+8*eax],mm0
	inc eax
	loop loop_3_g_1_3
	
	movd mm0,dword ptr[ebx+8*eax]
	movd mm1,dword ptr[esi+8*eax]
	pavgb mm0,mm1
	movd dword ptr[edi+8*eax],mm0
	
	emms
		
fin_g_1_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_2c endp


JPSDR_Deinterlace_Blend_Tri_SSE_3 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ecx,w
	mov ebx,esi
	add ebx,src_pitch
	xor eax,eax
loop_0_g_2:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[ebx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2:
	xor eax,eax
	mov ecx,w
loop_2_g_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2

	mov ecx,w
	xor eax,eax
loop_3_g_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2
	
fin_g_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3 endp



JPSDR_Deinterlace_Blend_Tri_SSE_3b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3b

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ecx,w
	mov ebx,esi
	add ebx,src_pitch
	xor eax,eax
loop_0_g_2_2:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[ebx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0	
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16	
	loop loop_2_g_2_2
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2

	mov ecx,w
	xor eax,eax
loop_3_g_2_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_2
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0	
	
fin_g_2_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3b endp



JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_3:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[ebx+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_3
		
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_3:
	xor eax,eax
	mov ecx,w
loop_2_g_3:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16		
	loop loop_2_g_3	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_3

	mov ecx,w
loop_3_g_3:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_3
	
fin_g_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_U endp


JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_3_2:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[ebx+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_3_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0	
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_3_2:
	xor eax,eax
	mov ecx,w
loop_2_g_3_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16		
	loop loop_2_g_3_2

	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_3_2

	mov ecx,w
loop_3_g_3_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_3_2
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0	
	
fin_g_3_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_Ub endp



JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_4:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_4
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_4:
	xor eax,eax
	mov ecx,w
loop_2_g_4:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[edx+eax]
	movdqu xmm2,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_4
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_4

	mov ecx,w
	xor eax,eax
loop_3_g_4:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_4
	
fin_g_4:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_U_A endp




JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_4_2:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_4_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0		
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_4_2:
	xor eax,eax
	mov ecx,w
loop_2_g_4_2:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[edx+eax]
	movdqu xmm2,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_4_2
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_4_2

	mov ecx,w
	xor eax,eax
loop_3_g_4_2:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_4_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0		
		
fin_g_4_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_U_Ab endp




JPSDR_Deinterlace_Blend_Tri_SSE_3_U proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_U

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_5:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_5
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_5:
	xor eax,eax
	mov ecx,w
loop_2_g_5:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[edx+eax]
	movdqu xmm2,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_5
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_5

	mov ecx,w
	xor eax,eax
loop_3_g_5:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_5
	
fin_g_5:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_U endp



JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	mov ecx,w
	add ebx,src_pitch
	xor eax,eax
loop_0_g_5_2:
	movdqu xmm0,oword ptr[esi+eax]
	movdqu xmm1,oword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_5_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0		
		
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_5_2:
	xor eax,eax
	mov ecx,w
loop_2_g_5_2:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[edx+eax]
	movdqu xmm2,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_5_2
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
		
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_5_2

	mov ecx,w
	xor eax,eax
loop_3_g_5_2:
	movdqu xmm0,oword ptr[ebx+eax]
	movdqu xmm1,oword ptr[esi+eax]
	pavgb xmm0,xmm1
	movdqu oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_5_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0			
	
fin_g_5_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_Ub endp



end





