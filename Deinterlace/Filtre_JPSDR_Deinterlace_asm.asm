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




JPSDR_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_SSE_3

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,w
	mov edx,src2
	mov edi,dst

loop_1_d:
	xor eax,eax
	mov ecx,ebx
	shr ecx,2
	jz short Suite_d_1
loop_2_d:
	movq xmm0,qword ptr[esi+8*eax]
	movq xmm1,qword ptr[edx+8*eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_2_d
	
Suite_d_1:
	mov ecx,ebx
	and ecx,1
	jz short Suite_d_2
	
	movd xmm0,dword ptr[esi+8*eax]
	movd xmm1,dword ptr[edx+8*eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+8*eax],xmm0	
	
Suite_d_2:	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_d
	
fin_d:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_SSE_3 endp



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
	shr ecx,2
	jz short Suite_e_1
loop_2_e:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_e
	
Suite_e_1:
	mov ecx,w
	and ecx,3
	jz short Suite_e_3
	and ecx,2
	jz short Suite_e_2
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[edx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0	
	
	mov ecx,w
	and ecx,1
	jz short Suite_e_3
	add eax,8	
	
Suite_e_2:	
	movd xmm0,dword ptr[esi+eax]
	movd xmm1,dword ptr[edx+eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+eax],xmm0	
	
Suite_e_3:	
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



JPSDR_Deinterlace_Blend_Tri_SSE_3 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_1_f
loop_0_f_2:
	movq xmm0,qword ptr[esi+8*eax]
	movq xmm1,qword ptr[ebx+8*eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_0_f_2
	
Suite_1_f:	
	mov ecx,w
	and ecx,1
	jz short Suite_2_f

	movd xmm0,dword ptr[esi+8*eax]
	movd xmm1,dword ptr[ebx+8*eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+8*eax],xmm0
	
Suite_2_f:
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_f_2:
	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_4_f
loop_2_f_2:
	movq xmm0,qword ptr[ebx+8*eax]
	movq xmm1,qword ptr[edx+8*eax]
	movq xmm2,qword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_2_f_2
	
Suite_4_f:
	mov ecx,w
	and ecx,1
	jz short Suite_5_f
	
	movd xmm0,dword ptr[ebx+8*eax]
	movd xmm1,dword ptr[edx+8*eax]
	movd xmm2,dword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+8*eax],xmm0
	
Suite_5_f:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f_2

	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_7_f	
loop_3_f_2:
	movq xmm0,qword ptr[ebx+8*eax]
	movq xmm1,qword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_3_f_2
	
Suite_7_f:
	mov ecx,w
	and ecx,1
	jz short fin_f_2
	
	movd xmm0,dword ptr[ebx+8*eax]
	movd xmm1,dword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+8*eax],xmm0
	
fin_f_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3 endp



JPSDR_Deinterlace_Blend_Tri_SSE_3a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_1_f_1
loop_0_f_2_1:
	movq xmm0,qword ptr[esi+8*eax]
	movq xmm1,qword ptr[ebx+8*eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_0_f_2_1
	
Suite_1_f_1:	
	mov ecx,w
	and ecx,1
	jz short Suite_2_f_1

	movd xmm0,dword ptr[esi+8*eax]
	movd xmm1,dword ptr[ebx+8*eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+8*eax],xmm0
	
Suite_2_f_1:
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_f_2_1:
	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_4_f_1
loop_2_f_2_1:
	movq xmm0,qword ptr[ebx+8*eax]
	movq xmm1,qword ptr[edx+8*eax]
	movq xmm2,qword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_2_f_2_1
	
Suite_4_f_1:
	mov ecx,w
	and ecx,1
	jz short Suite_5_f_1
	
	movd xmm0,dword ptr[ebx+8*eax]
	movd xmm1,dword ptr[edx+8*eax]
	movd xmm2,dword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+8*eax],xmm0
	
Suite_5_f_1:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f_2_1

	
fin_f_2_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3a endp


JPSDR_Deinterlace_Blend_Tri_SSE_3b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3b

	push esi
	push edi
	push ebx
	
	
Suite_2_f_2:
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_f_2_2:
	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_4_f_2
loop_2_f_2_2:
	movq xmm0,qword ptr[ebx+8*eax]
	movq xmm1,qword ptr[edx+8*eax]
	movq xmm2,qword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_2_f_2_2
	
Suite_4_f_2:
	mov ecx,w
	and ecx,1
	jz short Suite_5_f_2
	
	movd xmm0,dword ptr[ebx+8*eax]
	movd xmm1,dword ptr[edx+8*eax]
	movd xmm2,dword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+8*eax],xmm0
	
Suite_5_f_2:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f_2_2

fin_f_2_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3b endp


JPSDR_Deinterlace_Blend_Tri_SSE_3c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3c

	push esi
	push edi
	push ebx
	
	
Suite_2_f_3:
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_f_2_3:
	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_4_f_3
loop_2_f_2_3:
	movq xmm0,qword ptr[ebx+8*eax]
	movq xmm1,qword ptr[edx+8*eax]
	movq xmm2,qword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_2_f_2_3
	
Suite_4_f_3:
	mov ecx,w
	and ecx,1
	jz short Suite_5_f_3
	
	movd xmm0,dword ptr[ebx+8*eax]
	movd xmm1,dword ptr[edx+8*eax]
	movd xmm2,dword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+8*eax],xmm0
	
Suite_5_f_3:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f_2_3

	xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short Suite_7_f_3
loop_3_f_2_3:
	movq xmm0,qword ptr[ebx+8*eax]
	movq xmm1,qword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_3_f_2_3
	
Suite_7_f_3:
	mov ecx,w
	and ecx,1
	jz short fin_f_2_3
	
	movd xmm0,dword ptr[ebx+8*eax]
	movd xmm1,dword ptr[esi+8*eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+8*eax],xmm0
	
fin_f_2_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3c endp


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
	shr ecx,2
	jz short Suite_1_g
loop_0_g_2:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[ebx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2
	
Suite_1_g:	
	mov ecx,w
	and ecx,3
	jz short Suite_3_g
	and ecx,2
	jz short Suite_2_g

	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short Suite_3_g
	add eax,8

Suite_2_g:		
	movd xmm0,dword ptr[esi+eax]
	movd xmm1,dword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+eax],xmm0
	
Suite_3_g:	
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
	shr ecx,2
	jz short Suite_4_g	
loop_2_g_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2
	
Suite_4_g:
	mov ecx,w
	and ecx,3
	jz short Suite_6_g
	and ecx,2
	jz short Suite_5_g

	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short Suite_6_g
	add eax,8

Suite_5_g:		
	movd xmm0,dword ptr[ebx+eax]
	movd xmm1,dword ptr[edx+eax]
	movd xmm2,dword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+eax],xmm0
	
Suite_6_g:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz loop_1_g_2

	xor eax,eax
	mov ecx,w
	shr ecx,2
	jz short Suite_7_g	
loop_3_g_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2
	
Suite_7_g:
	mov ecx,w
	and ecx,3
	jz short fin_g_2
	and ecx,2
	jz short Suite_8_g
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short fin_g_2
	add eax,8

Suite_8_g:		
	movd xmm0,dword ptr[ebx+eax]
	movd xmm1,dword ptr[esi+eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+eax],xmm0
	
fin_g_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A endp




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
	shr ecx,2
	jz short Suite_1_g_1
loop_0_g_2_1:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[ebx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_1
	
Suite_1_g_1:	
	mov ecx,w
	and ecx,3
	jz short Suite_3_g_1
	and ecx,2
	jz short Suite_2_g_1

	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short Suite_3_g_1
	add eax,8

Suite_2_g_1:		
	movd xmm0,dword ptr[esi+eax]
	movd xmm1,dword ptr[ebx+eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+eax],xmm0
	
Suite_3_g_1:	
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
	shr ecx,2
	jz short Suite_4_g_1
loop_2_g_2_1:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_1
	
Suite_4_g_1:
	mov ecx,w
	and ecx,3
	jz short Suite_6_g_1
	and ecx,2
	jz short Suite_5_g_1

	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short Suite_6_g_1
	add eax,8

Suite_5_g_1:		
	movd xmm0,dword ptr[ebx+eax]
	movd xmm1,dword ptr[edx+eax]
	movd xmm2,dword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+eax],xmm0
	
Suite_6_g_1:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz loop_1_g_2_1

fin_g_2_1:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a endp




JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b

	push esi
	push edi
	push ebx
	
Suite_3_g_2:	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2:
	xor eax,eax
	mov ecx,w
	shr ecx,2
	jz short Suite_4_g_2
loop_2_g_2_2:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_2
	
Suite_4_g_2:
	mov ecx,w
	and ecx,3
	jz short Suite_6_g_2
	and ecx,2
	jz short Suite_5_g_2

	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short Suite_6_g_2
	add eax,8

Suite_5_g_2:		
	movd xmm0,dword ptr[ebx+eax]
	movd xmm1,dword ptr[edx+eax]
	movd xmm2,dword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+eax],xmm0
	
Suite_6_g_2:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz loop_1_g_2_2
	
fin_g_2_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b endp




JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c

	push esi
	push edi
	push ebx
	
Suite_3_g_3:	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3:
	xor eax,eax
	mov ecx,w
	shr ecx,2
	jz short Suite_4_g_3
loop_2_g_2_3:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[edx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_3
	
Suite_4_g_3:
	mov ecx,w
	and ecx,3
	jz short Suite_6_g_3
	and ecx,2
	jz short Suite_5_g_3

	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[edx+eax]
	movq xmm2,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short Suite_6_g_3
	add eax,8

Suite_5_g_3:		
	movd xmm0,dword ptr[ebx+eax]
	movd xmm1,dword ptr[edx+eax]
	movd xmm2,dword ptr[esi+eax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[edi+eax],xmm0
	
Suite_6_g_3:
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz loop_1_g_2_3

	xor eax,eax
	mov ecx,w
	shr ecx,2
	jz short Suite_7_g_3
loop_3_g_2_3:
	movdqa xmm0,oword ptr[ebx+eax]
	pavgb xmm0,oword ptr[esi+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_3
	
Suite_7_g_3:
	mov ecx,w
	and ecx,3
	jz short fin_g_2_3
	and ecx,2
	jz short Suite_8_g_3
	
	movq xmm0,qword ptr[ebx+eax]
	movq xmm1,qword ptr[esi+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0
	
	mov ecx,w
	and ecx,1
	jz short fin_g_2_3
	add eax,8

Suite_8_g_3:		
	movd xmm0,dword ptr[ebx+eax]
	movd xmm1,dword ptr[esi+eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+eax],xmm0
	
fin_g_2_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c endp

end





