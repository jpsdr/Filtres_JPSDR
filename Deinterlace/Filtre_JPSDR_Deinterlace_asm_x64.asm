.code


;JPSDR_Deinterlace_Blend_24 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_24 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1:
	mov ecx,r9d
	xor rax,rax
loop_2:
	movzx edx,byte ptr[rdi+4*rax]
	movzx ebx,byte ptr[rsi+4*rax]
	add bx,dx
	shr bx,1
	mov r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+1]
	movzx ebx,byte ptr[rsi+4*rax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+2]
	movzx ebx,byte ptr[rsi+4*rax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or ebx,r13d
	mov dword ptr[r8+4*rax],ebx
	
	inc rax
	loop loop_2
	
	add rsi,r11
	add rdi,r11
	add r8,r12
	
	dec r10d
	jnz short loop_1
	
fin_:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_24 endp


;JPSDR_Deinterlace_Blend_32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_32 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_i:
	mov ecx,r9d
	xor rax,rax
loop_2_i:
	movzx edx,byte ptr[rdi+4*rax]
    movzx ebx,byte ptr[rsi+4*rax]
	add bx,dx
	shr bx,1
	mov r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+1]
	movzx ebx,byte ptr[rsi+4*rax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+2]
	movzx ebx,byte ptr[rsi+4*rax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+3]
	movzx ebx,byte ptr[rsi+4*rax+3]
	add bx,dx
	shr bx,1
	shl ebx,24
	or ebx,r13d
	mov dword ptr[r8+4*rax],ebx
	
	inc rax
	loop loop_2_i
	
	add rsi,r11
	add rdi,r11
	add r8,r12
	
	dec r10d
	jnz short loop_1_i
	
fin_i:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_32 endp


;JPSDR_Deinterlace_Blend_48 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_48 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_48:
	mov ecx,r9d
	xor rax,rax
loop_2_48:
	movzx rdx,word ptr[rdi+8*rax]
	movzx rbx,word ptr[rsi+8*rax]
	add rbx,rdx
	shr rbx,1
	mov r13,rbx
	movzx rdx,word ptr[rdi+8*rax+2]
	movzx rbx,word ptr[rsi+8*rax+2]
	add rbx,rdx
	shr rbx,1
	shl rbx,16
	or r13,rbx
	movzx rdx,word ptr[rdi+8*rax+4]
	movzx rbx,word ptr[rsi+8*rax+4]
	add rbx,rdx
	shr rbx,1
	shl rbx,32
	or rbx,r13
	mov qword ptr[r8+8*rax],rbx
	
	inc rax
	loop loop_2_48
	
	add rsi,r11
	add rdi,r11
	add r8,r12
	
	dec r10d
	jnz short loop_1_48
	
fin_:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_48 endp


;JPSDR_Deinterlace_Blend_64 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_64 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_i_64:
	mov ecx,r9d
	xor rax,rax
loop_2_i_64:
	movzx rdx,word ptr[rdi+8*rax]
    movzx rbx,word ptr[rsi+8*rax]
	add rbx,rdx
	shr rbx,1
	mov r13,rbx
	movzx rdx,word ptr[rdi+8*rax+2]
	movzx rbx,word ptr[rsi+8*rax+2]
	add rbx,rdx
	shr rbx,1
	shl rbx,16
	or r13,rbx
	movzx rdx,word ptr[rdi+8*rax+4]
	movzx rbx,word ptr[rsi+8*rax+4]
	add rbx,rdx
	shr rbx,1
	shl rbx,32
	or r13,rbx
	movzx rdx,word ptr[rdi+8*rax+6]
	movzx rbx,word ptr[rsi+8*rax+6]
	add rbx,rdx
	shr rbx,1
	shl rbx,48
	or rbx,r13
	mov qword ptr[r8+8*rax],rbx
	
	inc rax
	loop loop_2_i_64
	
	add rsi,r11
	add rdi,r11
	add r8,r12
	
	dec r10d
	jnz short loop_1_i_64
	
fin_i_64:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_64 endp


;JPSDR_Deinterlace_Blend_8 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_8 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_j:
	mov ecx,r9d
	xor rax,rax
loop_2_j:
    movzx ebx,byte ptr[rsi+rax]
	movzx edx,byte ptr[rdi+rax]
	add bx,dx
	shr bx,1
	mov byte ptr[r8+rax],bl
	
	inc rax
	loop loop_2_j
	
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1_j
	
fin_j:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8 endp


;JPSDR_Deinterlace_Blend_16 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_16 proc public frame

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
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_j_16:
	mov ecx,r9d
	xor rax,rax
loop_2_j_16:
    movzx ebx,word ptr[rsi+2*rax]
	movzx edx,word ptr[rdi+2*rax]
	add ebx,edx
	shr ebx,1
	mov word ptr[r8+2*rax],bx
	
	inc rax
	loop loop_2_j_16
	
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1_j_16
	
fin_j_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16 endp


;JPSDR_Deinterlace_Blend_8_SSE2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_8_SSE2 proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e:
	xor rax,rax
	mov ecx,r9d
loop_2_e:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdi+rax]
	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e
	
fin_e:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_SSE2 endp


;JPSDR_Deinterlace_Blend_16_SSE2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_16_SSE2 proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_16:
	xor rax,rax
	mov ecx,r9d
loop_2_e_16:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgw xmm0,XMMWORD ptr[rdi+rax]
	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_16

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_16
	
fin_e_16:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_SSE2 endp


;JPSDR_Deinterlace_Blend_8_AVX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_8_AVX proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_AVX:
	xor rax,rax
	mov ecx,r9d
loop_2_e_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdi+rax]
	vmovdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_AVX

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_AVX
	
fin_e_AVX:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_AVX endp


;JPSDR_Deinterlace_Blend_16_AVX proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_16_AVX proc public frame

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
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_AVX_16:
	xor rax,rax
	mov ecx,r9d
loop_2_e_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rdi+rax]
	vmovdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_AVX_16

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_AVX_16
	
fin_e_AVX_16:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_AVX endp


;JPSDR_Deinterlace_Blend_8_Tri_SSE2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_SSE2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2

	add rdi,r11

loop_1_g_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2

	xor rax,rax
	mov ecx,r8d
loop_3_g_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2
	
fin_g_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_SSE2 endp


;JPSDR_Deinterlace_Blend_16_Tri_SSE2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_SSE2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_16

	add rdi,r11

loop_1_g_2_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rdx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_16
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_16

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_16
	
fin_g_2_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2 endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_AVX

	add rdi,r11

loop_1_g_2_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_AVX
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_AVX

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_AVX
	
fin_g_2_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_AVX_16

	add rdi,r11

loop_1_g_2_AVX_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_AVX_16
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_AVX_16

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_AVX_16
	
fin_g_2_AVX_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX endp


;JPSDR_Deinterlace_Blend_8_Tri_SSE2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_SSE2_a proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_1

	add rdi,r11

loop_1_g_2_1:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_1

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1

fin_g_2_1:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_SSE2_a endp


;JPSDR_Deinterlace_Blend_16_Tri_SSE2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_SSE2_a proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_1_16

	add rdi,r11

loop_1_g_2_1_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rdx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_1_16

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1_16

fin_g_2_1_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2_a endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX_a proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_1_AVX

	add rdi,r11

loop_1_g_2_1_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_1_AVX

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1_AVX

fin_g_2_1_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX_a endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX_a proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_1_AVX_16

	add rdi,r11

loop_1_g_2_1_AVX_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_1_AVX_16

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1_AVX_16

fin_g_2_1_AVX_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX_a endp


;JPSDR_Deinterlace_Blend_8_Tri_SSE2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_SSE2_b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2
	
fin_g_2_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_SSE2_b endp


;JPSDR_Deinterlace_Blend_16_Tri_SSE2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_SSE2_b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rdx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2_16
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2_16
	
fin_g_2_2_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2_b endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX_b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2_AVX
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2_AVX
	
fin_g_2_2_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX_b endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX_b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2_AVX_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2_AVX_16
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2_AVX_16
	
fin_g_2_2_AVX_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX_b endp


;JPSDR_Deinterlace_Blend_8_Tri_SSE2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_SSE2_c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_3

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_3
	
fin_g_2_3:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_SSE2_c endp


;JPSDR_Deinterlace_Blend_16_Tri_SSE2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_SSE2_c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rdx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_3_16

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3_16

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3_16:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgw xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_3_16
	
fin_g_2_3_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_SSE2_c endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX_c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_3_AVX

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3_AVX

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_3_AVX
	
fin_g_2_3_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX_c endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX_c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

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

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3_AVX_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_3_AVX_16

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3_AVX_16

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3_AVX_16:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgw xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_3_AVX_16
	
fin_g_2_3_AVX_16:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX_c endp


end





