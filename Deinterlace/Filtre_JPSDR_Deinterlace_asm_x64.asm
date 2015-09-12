.code


;JPSDR_Deinterlace_Blend_Non_MMX_24 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_24 proc public frame

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

JPSDR_Deinterlace_Blend_Non_MMX_24 endp



;JPSDR_Deinterlace_Blend_Non_MMX_32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_32 proc public frame

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

JPSDR_Deinterlace_Blend_Non_MMX_32 endp


;JPSDR_Deinterlace_Blend_Non_MMX_8 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_8 proc public frame

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

JPSDR_Deinterlace_Blend_Non_MMX_8 endp


;JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc src_a:dword,src_b:dword,dst_abs:dword,dst_avg:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; src_b = rdx
; dst_abs = r8
; dst_avg = r9

JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]

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
	mov r10d,w
	mov r11d,h
	mov r12,src_pitch
	mov r13,dst_pitch
	mov rbx,16
	xor rcx,rcx
	
loop_1_Yadif1:
	xor rax,rax
	mov ecx,r10d
loop_2_Yadif1:
	movdqa xmm0,[rsi+rax]		; xmm0=src_a
	movdqa xmm1,[rdi+rax]		; xmm1=src_b
	movdqa xmm2,xmm0
	movdqa xmm3,xmm0
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	pminub xmm3,xmm1			; xmm3=min(src_a,src_b)
	pavgb xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	psubb xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	movdqa [r9+rax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	movdqa [r8+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,rbx
	loop loop_2_Yadif1
	add rsi,r12
	add rdi,r12
	add r8,r13
	add r9,r13
	dec r11d
	jnz short loop_1_Yadif1

	pop r13
	pop r12	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_Deinterlace_YadifAbsDiffAvg_SSE endp


;JPSDR_Deinterlace_YadifAbsDiff_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAbsDiff_SSE proc public frame

dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

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
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
	
loop_1_Yadif2:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif2:
	movdqa xmm0,[rsi+rax]		; xmm0=src_a
	movdqa xmm1,[rbx+rax]		; xmm1=src_b
	movdqa xmm2,xmm0
	pminub xmm0,xmm1			; xmm0=min(src_a,src_b)	
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	psubb xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	movdqa [rdi+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,r8
	loop loop_2_Yadif2
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif2

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiff_SSE endp
	


;JPSDR_Deinterlace_YadifAvg_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAvg_SSE proc public frame
	
dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

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
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
		
loop_1_Yadif3:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif3:
	movdqa xmm0,[rsi+rax]		; xmm0=src_a
	pavgb xmm0,[rbx+rax]		; xmm0=(src_a+src_b+1)>>1
	movdqa [rdi+rax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
	add rax,r8
	loop loop_2_Yadif3
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif3

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAvg_SSE endp



;JPSDR_Deinterlace_Blend_SSE_3 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3 proc public frame

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
	mov ebx,1
	xor rcx,rcx

loop_1_d:
	xor rax,rax
	mov ecx,r9d
	shr ecx,1
	jz short Suite_d_1
loop_2_d:
	movq xmm0,qword ptr[rsi+8*rax]
	movq xmm1,qword ptr[rdi+8*rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+8*rax],xmm0
	inc rax
	loop loop_2_d
	
Suite_d_1:
	mov ecx,r9d
	and ecx,ebx
	jz short Suite_d_2
	
	movd xmm0,dword ptr[rsi+8*rax]
	movd xmm0,dword ptr[rdi+8*rax]
	pavgb xmm0,xmm1
	movd dword ptr[r8+8*rax],xmm0
	
Suite_d_2:	
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_d
	
fin_d:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3 endp



;JPSDR_Deinterlace_Blend_SSE_3_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_A proc public frame

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
	push r15
	.pushreg r15
	.endprolog
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	mov r12d,3
	mov r13d,2
	mov r14d,1
	mov r15,8
	xor rcx,rcx

loop_1_e:
	xor rax,rax
	mov ecx,r9d
	shr ecx,2
	jz short Suite_e_1
loop_2_e:
	movdqa xmm0,oword ptr[rsi+rax]
	pavgb xmm0,oword ptr[rdi+rax]
	movdqa oword ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e
	
Suite_e_1:
	mov ecx,r9d
	and ecx,r12d
	jz short Suite_e_3
	and ecx,r13d
	jz short Suite_e_2
	
	movq xmm0,qword ptr[rsi+rax]
	movq xmm0,qword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[r8+rax],xmm0
	
	mov ecx,r9d
	and ecx,r14d
	jz short Suite_e_3
	add rax,r15
	
Suite_e_2:	
	movd xmm0,dword ptr[rsi+rax]
	movd xmm0,dword ptr[rdi+rax]
	pavgb xmm0,xmm1
	movd dword ptr[r8+rax],xmm0
	
Suite_e_3:	
	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e
	
fin_e:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3_A endp




;JPSDR_Deinterlace_Blend_Tri_SSE_3 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3 proc public frame

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
	mov r12d,1
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
	shr ecx,1
	jz short Suite_1_f	
loop_0_f_2:
	movq xmm0,qword ptr[rbx+8*rax]
	movq xmm1,qword ptr[rsi+8*rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+8*rax],xmm0
	inc rax
	loop loop_0_f_2
	
Suite_1_f:	
	mov ecx,r8d
	and ecx,r12d
	jz short Suite_2_f
	
	movd xmm0,dword ptr[rbx+8*rax]
	movd xmm1,dword ptr[rsi+8*rax]
	pavgb xmm0,xmm1
	movd dword ptr[rdi+8*rax],xmm0
	
Suite_2_f:	
	add rdi,r11

loop_1_f_2:
	xor rax,rax
	mov ecx,r8d
	shr ecx,1
	jz short Suite_4_f
loop_2_f_2:
	movq xmm0,qword ptr[rbx+8*rax]
	movq xmm1,qword ptr[rdx+8*rax]
	movq xmm2,qword ptr[rsi+8*rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[rdi+8*rax],xmm0
	inc rax
	loop loop_2_f_2
	
Suite_4_f:
	mov ecx,r8d
	and ecx,r12d
	jz short Suite_5_f
	
	movd xmm0,dword ptr[rbx+8*rax]
	movd xmm1,dword ptr[rdx+8*rax]
	movd xmm2,dword ptr[rsi+8*rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[rdi+8*rax],xmm0
	
Suite_5_f:	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_f_2

	xor rax,rax
	mov ecx,r8d
	shr ecx,1
	jz short Suite_7_f	
loop_3_f_2:
	movq xmm0,qword ptr[rbx+8*rax]
	movq xmm1,qword ptr[rsi+8*rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+8*rax],xmm0
	inc rax
	loop loop_3_f_2
	
Suite_7_f:
	mov ecx,r8d
	and ecx,r12d
	jz short fin_f_2
	
	movd xmm0,dword ptr[rbx+8*rax]
	movd xmm1,dword ptr[rsi+8*rax]
	pavgb xmm0,xmm1
	movd dword ptr[rdi+8*rax],xmm0
	
fin_f_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3 endp



;JPSDR_Deinterlace_Blend_Tri_SSE_3_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A proc public frame

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
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	push r15
	.pushreg r15
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov r13d,3
	mov r14d,2
	mov r15d,1
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
	shr ecx,2
	jz short Suite_1_g	
loop_0_g_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2
	
Suite_1_g:	
	mov ecx,r8d
	and ecx,r13d
	jz short Suite_3_g
	and ecx,r14d
	jz short Suite_2_g

	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0
	
	mov ecx,r8d
	and ecx,r15d
	jz short Suite_3_g
	add rax,8
	
Suite_2_g:	
	movd xmm0,dword ptr[rbx+rax]
	movd xmm1,dword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movd dword ptr[rdi+rax],xmm0
	
Suite_3_g:	
	add rdi,r11

loop_1_g_2:
	xor rax,rax
	mov ecx,r8d
	shr ecx,2
	jz short Suite_4_g		
loop_2_g_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rdx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2
	
Suite_4_g:
	mov ecx,r8d
	and ecx,r13d
	jz short Suite_6_g
	and ecx,r14d
	jz short Suite_5_g
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rdx+rax]
	movq xmm2,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movq qword ptr[rdi+rax],xmm0
	
	mov ecx,r8d
	and ecx,r15d
	jz short Suite_6_g
	add rax,8

Suite_5_g:		
	movd xmm0,dword ptr[rbx+rax]
	movd xmm1,dword ptr[rdx+rax]
	movd xmm2,dword ptr[rsi+rax]
	pavgb xmm0,xmm1
	pavgb xmm0,xmm2
	movd dword ptr[rdi+rax],xmm0
	
Suite_6_g:	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz loop_1_g_2

	xor rax,rax
	mov ecx,r8d
	shr ecx,2
	jz short Suite_7_g	
loop_3_g_2:
	movdqa xmm0,oword ptr[rbx+rax]
	pavgb xmm0,oword ptr[rsi+rax]
	movdqa oword ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2
	
Suite_7_g:
	mov ecx,r8d
	and ecx,r13d
	jz short fin_g_2
	and ecx,r14d
	jz short Suite_8_g
	
	movq xmm0,qword ptr[rbx+rax]
	movq xmm1,qword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movq qword ptr[rdi+rax],xmm0

	mov ecx,r8d
	and ecx,r15d
	jz short fin_g_2
	add rax,8
	
Suite_8_g:	
	movd xmm0,dword ptr[rbx+rax]
	movd xmm1,dword ptr[rsi+rax]
	pavgb xmm0,xmm1
	movd dword ptr[rdi+rax],xmm0
	
fin_g_2:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A endp





end





