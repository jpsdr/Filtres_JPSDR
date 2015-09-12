.586
.xmm
.model flat,c

.code



JPSDR_BlurMotion_SSE_3 proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3

	push esi
	push edi
	push ebx

	mov esi,src
	mov edx,s
	mov edi,dst
	mov ebx,w

loop_1_e:
	xor eax,eax
	mov ecx,ebx
	shr ecx,1
	jz short loop_3_e
	
loop_2_e:
	movq xmm0,qword ptr[esi+8*eax]
	movq xmm1,qword ptr[edx+8*eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+8*eax],xmm0
	inc eax
	loop loop_2_e
	
loop_3_e:	
	mov ecx,ebx
	and ecx,1
	jz short loop_4_e
	
	movd xmm0,dword ptr[esi+8*eax]
	movd xmm1,dword ptr[edx+8*eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+8*eax],xmm0

loop_4_e:
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




JPSDR_BlurMotion_SSE_3_A proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3_A

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
	shr ecx,2
	jz short loop_3_f
	
loop_2_f:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_f
	
loop_3_f:
	mov ecx,w
	and ecx,3
	jz short loop_5_f
	and ecx,2
	jz short loop_4_f
	
	movq xmm0,qword ptr[esi+eax]
	movq xmm1,qword ptr[edx+eax]
	pavgb xmm0,xmm1
	movq qword ptr[edi+eax],xmm0

	mov ecx,w
	and ecx,1
	jz short loop_5_f
	add eax,8

loop_4_f:
	movd xmm0,dword ptr[esi+eax]
	movd xmm1,dword ptr[edx+eax]
	pavgb xmm0,xmm1
	movd dword ptr[edi+eax],xmm0

loop_5_f:
	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_3_A endp



end





