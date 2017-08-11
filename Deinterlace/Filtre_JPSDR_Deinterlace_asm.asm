.586
.model flat,c

.code

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
	movdqa xmm0,XMMWORD ptr[esi+eax]		; xmm0=src_a
	movdqa xmm1,XMMWORD ptr[edi+eax]		; xmm1=src_b
	movdqa xmm2,xmm0
	movdqa xmm3,xmm0
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	pminub xmm3,xmm1			; xmm3=min(src_a,src_b)
	pavgb xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	psubb xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	movdqa XMMWORD ptr[edx+eax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	movdqa XMMWORD ptr[ebx+eax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
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


JPSDR_Deinterlace_YadifAbsDiffAvg_AVX proc src_a:dword,src_b:dword,dst_abs:dword,dst_avg:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword

	public JPSDR_Deinterlace_YadifAbsDiffAvg_AVX
	
	push esi
	push edi
	push ebx
	
	mov esi,src_a
	mov edi,src_b
	mov ebx,dst_abs
	mov edx,dst_avg
	
loop_1_Yadif1_AVX:
	xor eax,eax
	mov ecx,w
loop_2_Yadif1_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]		; xmm0=src_a
	vmovdqa xmm1,XMMWORD ptr[edi+eax]		; xmm1=src_b
	vpmaxub xmm2,xmm0,xmm1			; xmm2=max(src_a,src_b)
	vpminub xmm3,xmm0,xmm1			; xmm3=min(src_a,src_b)
	vpavgb xmm0,xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	vpsubb xmm2,xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	vmovdqa XMMWORD ptr[edx+eax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	vmovdqa XMMWORD ptr[ebx+eax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add eax,16
	loop loop_2_Yadif1_AVX
	add esi,src_pitch
	add edi,src_pitch
	add ebx,dst_pitch
	add edx,dst_pitch
	dec h
	jnz short loop_1_Yadif1_AVX
	
	pop ebx
	pop edi
	pop esi
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiffAvg_AVX endp


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
	movdqa xmm0,XMMWORD ptr[esi+eax]		; xmm0=src_a
	movdqa xmm1,XMMWORD ptr[ebx+eax]		; xmm1=src_b
	movdqa xmm2,xmm0
	pminub xmm0,xmm1			; xmm0=min(src_a,src_b)	
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	psubb xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	movdqa XMMWORD ptr[edi+eax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
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


JPSDR_Deinterlace_YadifAbsDiff_AVX proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword

	public JPSDR_Deinterlace_YadifAbsDiff_AVX
	
	push esi
	push edi
	push ebx
	
	mov esi,src_a
	mov ebx,src_b
	mov edi,dst
	mov edx,16
	
loop_1_Yadif2_AVX:
	xor eax,eax
	mov ecx,w
loop_2_Yadif2_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]		; xmm0=src_a
	vmovdqa xmm1,XMMWORD ptr[ebx+eax]		; xmm1=src_b
	vpmaxub xmm2,xmm0,xmm1			; xmm2=max(src_a,src_b)
	vpminub xmm0,xmm0,xmm1			; xmm0=min(src_a,src_b)	
	vpsubb xmm2,xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	vmovdqa XMMWORD ptr[edi+eax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add eax,edx
	loop loop_2_Yadif2_AVX
	add esi,src_pitch
	add ebx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_Yadif2_AVX
	
	pop ebx
	pop edi
	pop esi
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiff_AVX endp


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
	movdqa xmm0,XMMWORD ptr[esi+eax]		; xmm0=src_a
	pavgb xmm0,XMMWORD ptr[ebx+eax]		; xmm0=(src_a+src_b+1)>>1
	movdqa XMMWORD ptr[edi+eax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
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


JPSDR_Deinterlace_YadifAvg_AVX proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword

	public JPSDR_Deinterlace_YadifAvg_AVX
	
	push esi
	push edi
	push ebx
	
	mov esi,src_a
	mov ebx,src_b
	mov edi,dst
	mov edx,16
	
loop_1_Yadif3_AVX:
	xor eax,eax
	mov ecx,w
loop_2_Yadif3_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]		; xmm0=src_a
	vpavgb xmm0,xmm0,XMMWORD ptr[ebx+eax]		; xmm0=(src_a+src_b+1)>>1
	vmovdqa XMMWORD ptr[edi+eax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
	add eax,edx
	loop loop_2_Yadif3_AVX
	add esi,src_pitch
	add ebx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_Yadif3_AVX
	
	pop ebx
	pop edi
	pop esi
	
	ret
	
JPSDR_Deinterlace_YadifAvg_AVX endp


JPSDR_Deinterlace_Blend_SSE_3_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3_A

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
loop_2_e:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_e
	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e
	
fin_e:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3_A endp


JPSDR_Deinterlace_Blend_AVX_3_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_AVX_3_A

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,16
	mov edx,src2
	mov edi,dst

loop_1_e_AVX:
	xor eax,eax
	mov ecx,w
loop_2_e_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[edx+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_AVX
	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e_AVX
	
fin_e_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_AVX_3_A endp


JPSDR_Deinterlace_Blend_Tri_SSE_3_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[ebx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
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
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2

	xor eax,eax
	mov ecx,w
loop_3_g_2:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2
	
fin_g_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A endp


JPSDR_Deinterlace_Blend_Tri_AVX_3_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_AVX_3_A

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[ebx+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_AVX
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_AVX:
	xor eax,eax
	mov ecx,w
loop_2_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_AVX
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_AVX

	xor eax,eax
	mov ecx,w
loop_3_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_AVX
	
fin_g_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A endp


JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_1:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgb xmm0,XMMWORD ptr[ebx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_1
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_1:
	xor eax,eax
	mov ecx,w
loop_2_g_2_1:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_1

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_1

fin_g_2_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a endp


JPSDR_Deinterlace_Blend_Tri_AVX_3_A_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_AVX_3_A_a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[ebx+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_1_AVX
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_1_AVX:
	xor eax,eax
	mov ecx,w
loop_2_g_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_1_AVX

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_1_AVX

fin_g_2_1_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_a endp


JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_2
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2
	
fin_g_2_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b endp


JPSDR_Deinterlace_Blend_Tri_AVX_3_A_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_AVX_3_A_b

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2_AVX:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_2_AVX
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2_AVX
	
fin_g_2_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_b endp


JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3:
	xor eax,eax
	mov ecx,w
loop_2_g_2_3:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[edx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_3

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_3

	xor eax,eax
	mov ecx,w
loop_3_g_2_3:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgb xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_3
	
fin_g_2_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c endp


JPSDR_Deinterlace_Blend_Tri_AVX_3_A_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_AVX_3_A_c

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3_AVX:
	xor eax,eax
	mov ecx,w
loop_2_g_2_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_3_AVX

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_3_AVX

	xor eax,eax
	mov ecx,w
loop_3_g_2_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgb xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_3_AVX
	
fin_g_2_3_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_c endp


end





