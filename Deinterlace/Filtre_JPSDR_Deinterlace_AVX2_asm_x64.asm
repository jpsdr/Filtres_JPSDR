.code


;JPSDR_Deinterlace_Blend_8_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_8_AVX2 proc public frame

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
	mov rbx,32
	xor rcx,rcx

loop_1_e_AVX2:
	xor rax,rax
	mov ecx,r9d
loop_2_e_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rsi+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rdi+rax]
	vmovdqa YMMWORD ptr[r8+rax],ymm0
	add rax,rbx
	loop loop_2_e_AVX2

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_AVX2
	
fin_e_AVX2:
	vzeroupper

	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_AVX2 endp


;JPSDR_Deinterlace_Blend_16_AVX2 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_16_AVX2 proc public frame

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
	mov rbx,32
	xor rcx,rcx

loop_1_e_AVX2_16:
	xor rax,rax
	mov ecx,r9d
loop_2_e_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rsi+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rdi+rax]
	vmovdqa YMMWORD ptr[r8+rax],ymm0
	add rax,rbx
	loop loop_2_e_AVX2_16

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_AVX2_16
	
fin_e_AVX2_16:
	vzeroupper
	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_AVX2 endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX2 proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_0_g_2_AVX2

	add rdi,r11

loop_1_g_2_AVX2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_AVX2
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_AVX2

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_3_g_2_AVX2
	
fin_g_2_AVX2:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2 endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX2 proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX2 proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_0_g_2_AVX2_16

	add rdi,r11

loop_1_g_2_AVX2_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_AVX2_16
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_AVX2_16

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_3_g_2_AVX2_16
	
fin_g_2_AVX2_16:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2 endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX2_a proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_0_g_2_1_AVX2

	add rdi,r11

loop_1_g_2_1_AVX2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_1_AVX2

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1_AVX2

	vzeroupper
	
fin_g_2_1_AVX2:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2_a endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX2_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX2_a proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_0_g_2_1_AVX2_16

	add rdi,r11

loop_1_g_2_1_AVX2_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_1_AVX2_16

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1_AVX2_16

fin_g_2_1_AVX2_16:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2_a endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX2_b proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2_AVX2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_2_AVX2
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2_AVX2
	
fin_g_2_2_AVX2:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2_b endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX2_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX2_b proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2_AVX2_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_2_AVX2_16
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2_AVX2_16
	
fin_g_2_2_AVX2_16:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2_b endp


;JPSDR_Deinterlace_Blend_8_Tri_AVX2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_8_Tri_AVX2_c proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3_AVX2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_3_AVX2

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3_AVX2

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3_AVX2:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgb ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_3_g_2_3_AVX2
	
fin_g_2_3_AVX2:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_8_Tri_AVX2_c endp


;JPSDR_Deinterlace_Blend_16_Tri_AVX2_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_16_Tri_AVX2_c proc public frame

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
	mov r12,32
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3_AVX2_16:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rdx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_2_g_2_3_AVX2_16

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3_AVX2_16

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3_AVX2_16:
	vmovdqa ymm0,YMMWORD ptr[rbx+rax]
	vpavgw ymm0,ymm0,YMMWORD ptr[rsi+rax]
	vmovdqa YMMWORD ptr[rdi+rax],ymm0
	add rax,r12
	loop loop_3_g_2_3_AVX2_16
	
fin_g_2_3_AVX2_16:
	vzeroupper
	
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_16_Tri_AVX2_c endp


end





