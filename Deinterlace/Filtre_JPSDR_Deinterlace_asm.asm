.586
.model flat,c

.code

JPSDR_Deinterlace_Blend_24 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_24

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

JPSDR_Deinterlace_Blend_24 endp


JPSDR_Deinterlace_Blend_32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_32

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

JPSDR_Deinterlace_Blend_32 endp


JPSDR_Deinterlace_Blend_48 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_48

	local res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2

loop_1_48:
	mov ecx,w
	xor eax,eax
loop_2_48:
	movzx edx,word ptr[edi+8*eax]
	movzx ebx,word ptr[esi+8*eax]
	add ebx,edx
	shr ebx,1
	mov res,ebx
	movzx edx,word ptr[edi+8*eax+2]
	movzx ebx,word ptr[esi+8*eax+2]
	add ebx,edx
	shr ebx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+8*eax],ebx
	
	movzx edx,word ptr[edi+8*eax+4]
	movzx ebx,word ptr[esi+8*eax+4]
	add ebx,edx
	shr ebx,1
	mov edx,dst
	mov dword ptr[edx+8*eax+4],ebx
	
	inc eax
	loop loop_2_48
	
	add esi,src_pitch
	add edi,src_pitch
	add edx,dst_pitch
	mov dst,edx
	
	dec h
	jnz short loop_1_48
	
fin__48:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_48 endp


JPSDR_Deinterlace_Blend_64 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_64

	local res:dword

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2

loop_1_i_64:
	mov ecx,w
	xor eax,eax
loop_2_i_64:
	movzx edx,word ptr[edi+8*eax]
    movzx ebx,word ptr[esi+8*eax]
	add ebx,edx
	shr ebx,1
	mov res,ebx
	movzx edx,word ptr[edi+8*eax+2]
	movzx ebx,word ptr[esi+8*eax+2]
	add ebx,edx
	shr ebx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+8*eax],ebx
	
	movzx edx,word ptr[edi+8*eax+4]
	movzx ebx,word ptr[esi+8*eax+4]
	add ebx,edx
	shr ebx,1
	mov res,ebx
	movzx edx,word ptr[edi+8*eax+6]
	movzx ebx,word ptr[esi+8*eax+6]
	add ebx,edx
	shr ebx,1
	shl ebx,16
	mov edx,dst
	or ebx,res
	mov dword ptr[edx+8*eax+4],ebx
	
	inc eax
	loop loop_2_i_64
	
	add esi,src_pitch
	add edi,src_pitch
	add edx,dst_pitch
	mov dst,edx
	
	dec h
	jnz short loop_1_i_64
	
fin_i_64:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_64 endp


JPSDR_Deinterlace_Blend_8 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2

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

JPSDR_Deinterlace_Blend_8 endp


JPSDR_Deinterlace_Blend_16 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov edi,src2

loop_1_j_16:
	mov ecx,w
	xor eax,eax
loop_2_j_16:
    movzx ebx,word ptr[esi+2*eax]
	movzx edx,word ptr[edi+2*eax]
	add ebx,edx
	shr ebx,1
	mov edx,dst
	mov word ptr[edx+2*eax],bx
	
	inc eax
	loop loop_2_j_16
	
	add esi,src_pitch
	add edi,src_pitch
	add edx,dst_pitch
	mov dst,edx
	
	dec h
	jnz short loop_1_j_16
	
fin_j_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16 endp


.xmm


JPSDR_Deinterlace_Blend_8_SSE2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_SSE2

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

JPSDR_Deinterlace_Blend_8_SSE2 endp


JPSDR_Deinterlace_Blend_16_SSE2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_SSE2

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,16
	mov edx,src2
	mov edi,dst

loop_1_e_16:
	xor eax,eax
	mov ecx,w
loop_2_e_16:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgw xmm0,XMMWORD ptr[edx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_16
	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e_16
	
fin_e_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_SSE2 endp


JPSDR_Deinterlace_Blend_8_AVX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_AVX

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

JPSDR_Deinterlace_Blend_8_AVX endp


JPSDR_Deinterlace_Blend_16_AVX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
	src_pitch:dword,dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_AVX

	push esi
	push edi
	push ebx
	
	mov esi,src1
	mov ebx,16
	mov edx,src2
	mov edi,dst

loop_1_e_AVX_16:
	xor eax,eax
	mov ecx,w
loop_2_e_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[edx+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_e_AVX_16
	
	add esi,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_e_AVX_16
	
fin_e_AVX_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_AVX endp


JPSDR_Deinterlace_Blend_8_Tri_SSE2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_SSE2

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

JPSDR_Deinterlace_Blend_8_Tri_SSE2 endp


JPSDR_Deinterlace_Blend_16_Tri_SSE2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_SSE2

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_16:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgw xmm0,XMMWORD ptr[ebx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_16
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_16:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgw xmm0,XMMWORD ptr[edx+eax]
	pavgw xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_16
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_16

	xor eax,eax
	mov ecx,w
loop_3_g_2_16:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgw xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_16
	
fin_g_2_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2 endp


JPSDR_Deinterlace_Blend_8_Tri_AVX proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX

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

JPSDR_Deinterlace_Blend_8_Tri_AVX endp


JPSDR_Deinterlace_Blend_16_Tri_AVX proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[ebx+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_AVX_16
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_AVX_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_AVX_16
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_AVX_16

	xor eax,eax
	mov ecx,w
loop_3_g_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_AVX_16
	
fin_g_2_AVX_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX endp


JPSDR_Deinterlace_Blend_8_Tri_SSE2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_SSE2_a

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

JPSDR_Deinterlace_Blend_8_Tri_SSE2_a endp


JPSDR_Deinterlace_Blend_16_Tri_SSE2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_SSE2_a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_1_16:
	movdqa xmm0,XMMWORD ptr[esi+eax]
	pavgw xmm0,XMMWORD ptr[ebx+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_1_16
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_1_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_1_16:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgw xmm0,XMMWORD ptr[edx+eax]
	pavgw xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_1_16

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_1_16

fin_g_2_1_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2_a endp


JPSDR_Deinterlace_Blend_8_Tri_AVX_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX_a

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

JPSDR_Deinterlace_Blend_8_Tri_AVX_a endp


JPSDR_Deinterlace_Blend_16_Tri_AVX_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX_a

	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,dst
	mov ebx,esi
	add ebx,src_pitch
	
	xor eax,eax
	mov ecx,w
loop_0_g_2_1_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[esi+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[ebx+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_0_g_2_1_AVX_16
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	add edi,dst_pitch
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_1_AVX_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_1_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_1_AVX_16

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_1_AVX_16

fin_g_2_1_AVX_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX_a endp


JPSDR_Deinterlace_Blend_8_Tri_SSE2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_SSE2_b

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

JPSDR_Deinterlace_Blend_8_Tri_SSE2_b endp


JPSDR_Deinterlace_Blend_16_Tri_SSE2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_SSE2_b

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2_16:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgw xmm0,XMMWORD ptr[edx+eax]
	pavgw xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_2_16
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2_16
	
fin_g_2_2_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2_b endp


JPSDR_Deinterlace_Blend_8_Tri_AVX_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX_b

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

JPSDR_Deinterlace_Blend_8_Tri_AVX_b endp


JPSDR_Deinterlace_Blend_16_Tri_AVX_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX_b

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_2_AVX_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_2_AVX_16
	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_2_AVX_16
	
fin_g_2_2_AVX_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX_b endp


JPSDR_Deinterlace_Blend_8_Tri_SSE2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_SSE2_c

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

JPSDR_Deinterlace_Blend_8_Tri_SSE2_c endp


JPSDR_Deinterlace_Blend_16_Tri_SSE2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_SSE2_c

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_3_16:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgw xmm0,XMMWORD ptr[edx+eax]
	pavgw xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_3_16

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_3_16

	xor eax,eax
	mov ecx,w
loop_3_g_2_3_16:
	movdqa xmm0,XMMWORD ptr[ebx+eax]
	pavgw xmm0,XMMWORD ptr[esi+eax]
	movdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_3_16
	
fin_g_2_3_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2_c endp


JPSDR_Deinterlace_Blend_8_Tri_AVX_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_8_Tri_AVX_c

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

JPSDR_Deinterlace_Blend_8_Tri_AVX_c endp


JPSDR_Deinterlace_Blend_16_Tri_AVX_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
	dst_pitch:dword

	public JPSDR_Deinterlace_Blend_16_Tri_AVX_c

	push esi
	push edi
	push ebx
	
	mov ebx,src					;ebx=ln-1
	mov edi,dst
	mov esi,ebx
	add esi,src_pitch			;esi=ln
	mov edx,esi
	add edx,src_pitch			;edx=ln+1

loop_1_g_2_3_AVX_16:
	xor eax,eax
	mov ecx,w
loop_2_g_2_3_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[edx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_2_g_2_3_AVX_16

	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_g_2_3_AVX_16

	xor eax,eax
	mov ecx,w
loop_3_g_2_3_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[ebx+eax]
	vpavgw xmm0,xmm0,XMMWORD ptr[esi+eax]
	vmovdqa XMMWORD ptr[edi+eax],xmm0
	add eax,16
	loop loop_3_g_2_3_AVX_16
	
fin_g_2_3_AVX_16:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX_c endp


end





