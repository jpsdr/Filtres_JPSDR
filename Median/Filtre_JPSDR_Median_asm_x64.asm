.data

align 16

Ymask qword 2 dup(00FF00FF00FF00FFh)

.code


;JPSDR_Median_RGB32_Move_src proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_offset:dword,dst_offset:dword
; src = rcx
; planar_R = rdx
; planar_G = r8
; planar_B = r9

JPSDR_Median_RGB32_Move_src proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]
dst_offset equ qword ptr[rbp+72]

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
	mov rdi,r9			; rdi=planar_b
	mov r10,rdx			; r10=planar_R
	mov rbx,r8			; rbx=planar_G
	mov edx,w
	mov r8d,h
	mov r9,src_offset
	mov r11,dst_offset
	xor rcx,rcx
		
loop_1_g:
	mov ecx,edx
loop_2_g:
	lodsd
	stosb
	mov byte ptr[rbx],ah
	shr eax,16
	inc rbx
	mov byte ptr[r10],al
	inc r10
	loop loop_2_g
	
	add rsi,r9
	add rdi,r11
	add rbx,r11
	add r10,r11
	dec r8d
	jnz short loop_1_g
	
fin_g:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_src endp



;JPSDR_Median_RGB32_Move_src_SSE_1 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword
; src = rcx
; planar_R = rdx
; planar_G = r8
; planar_B = r9

JPSDR_Median_RGB32_Move_src_SSE_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	.endprolog	
	
	mov rsi,rcx
	mov rdi,r9			; rdi=planar_b
	mov rbx,r8			; rbx=planar_G  rdx=planar_R
	mov r10d,w
	mov r8d,h
	mov r9,src_pitch
	mov r11,dst_pitch
	mov r12,2
	xor rcx,rcx
	movdqa xmm3,oword ptr Ymask
	pxor xmm0,xmm0
	
loop_1_g_1:
	mov ecx,r10d
	xor r13,r13
	shr ecx,2
	jz short suite_g_1
loop_2_g_1:
	movq xmm0,qword ptr[rsi+8*r13]   ;00000000xRGBxRGB
	movq xmm4,qword ptr[rsi+8*r13+8] ;00000000xRGBxRGB
	pslldq xmm4,8                    ;xRGBxRGB00000000
	por xmm0,xmm4                    ;xRGBxRGBxRGBxRGB
	movdqa xmm1,xmm0                 ;xRGBxRGBxRGBxRGB
	pand xmm0,xmm3                   ;0R0B0R0B0R0B0R0B
	psrlw xmm1,8                     ;0x0G0x0G0x0G0x0G
	packuswb xmm0,xmm0               ;00000000RBRBRBRB
	packuswb xmm1,xmm1               ;00000000xGxGxGxG
	movdqa xmm2,xmm0                 ;00000000RBRBRBRB
	pand xmm1,xmm3                   ;000000000G0G0G0G
	pand xmm0,xmm3                   ;000000000B0B0B0B
	packuswb xmm1,xmm1               ;000000000000GGGG
	psrlw xmm2,8                     ;000000000R0R0R0R
	packuswb xmm0,xmm0               ;000000000000BBBB
	packuswb xmm2,xmm2               ;000000000000RRRR
	movd dword ptr[rdi+2*r13],xmm0
	movd dword ptr[rbx+2*r13],xmm1
	movd dword ptr[rdx+2*r13],xmm2
	pxor xmm0,xmm0
	add r13,r12
	loop loop_2_g_1
suite_g_1:	
    mov r14d,r10d
	and r14d,3
	jz short suite_g_2
	mov rcx,r13
	
	mov eax,dword ptr[rsi+8*rcx]
	mov byte ptr[rdi+2*rcx],al
	mov byte ptr[rbx+2*rcx],ah
	shr eax,16
	mov byte ptr[rdx+2*rcx],al
	dec r14d
	jz short suite_g_2
	mov eax,dword ptr[rsi+8*rcx+4]
	mov byte ptr[rdi+2*rcx+1],al
	mov byte ptr[rbx+2*rcx+1],ah
	shr eax,16
	mov byte ptr[rdx+2*rcx+1],al
	dec r14d
	jz short suite_g_2
	mov eax,dword ptr[rsi+8*rcx+8]
	mov byte ptr[rdi+2*rcx+2],al
	mov byte ptr[rbx+2*rcx+2],ah
	shr eax,16
	mov byte ptr[rdx+2*rcx+2],al
	
suite_g_2:
    xor rcx,rcx
	add rsi,r9
	add rdi,r11
	add rbx,r11
	add rdx,r11
	dec r8d
	jnz loop_1_g_1	
	
fin_g_1:
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_src_SSE_1 endp



;JPSDR_Median_RGB32_Move_src_SSE_2 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword
; src = rcx
; planar_R = rdx
; planar_G = r8
; planar_B = r9

JPSDR_Median_RGB32_Move_src_SSE_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]

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
	mov rdi,r9			; rdi=planar_b
	mov r10,rdx			; r10=planar_R
	mov rbx,r8			; rbx=planar_G
	mov edx,w
	mov r8d,h
	mov r9,src_pitch
	mov r11,dst_pitch
	mov r12,2
	xor rcx,rcx
	movdqa xmm3,oword ptr Ymask
	
loop_1_g_2:
    xor rax,rax
	mov ecx,edx
loop_2_g_2:
	movdqa xmm0,oword ptr[rsi+8*rax]   ;xRGBxRGBxRGBxRGB
	movdqa xmm1,xmm0                   ;xRGBxRGBxRGBxRGB
	pand xmm0,xmm3                     ;0R0B0R0B0R0B0R0B
	psrlw xmm1,8                       ;0x0G0x0G0x0G0x0G
	packuswb xmm0,xmm0                 ;00000000RBRBRBRB
	packuswb xmm1,xmm1                 ;00000000xGxGxGxG
	movdqa xmm2,xmm0                   ;00000000RBRBRBRB
	pand xmm1,xmm3                     ;000000000G0G0G0G
	pand xmm0,xmm3                     ;000000000B0B0B0B
	packuswb xmm1,xmm1                 ;000000000000GGGG
	psrlw xmm2,8                       ;000000000R0R0R0R
	packuswb xmm0,xmm0                 ;000000000000BBBB
	packuswb xmm2,xmm2                 ;000000000000RRRR
	movd dword ptr[rdi+2*rax],xmm0
	movd dword ptr[rbx+2*rax],xmm1
	movd dword ptr[r10+2*rax],xmm2
	add rax,r12
	loop loop_2_g_2
	
	add rsi,r9
	add rdi,r11
	add rbx,r11
	add r10,r11
	dec r8d
	jnz short loop_1_g_2
			
fin_g_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_src_SSE_2 endp


;JPSDR_Median_YUYV_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword,dst_offset_Y:dword,dst_offset_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9

JPSDR_Median_YUYV_Move_src proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]
dst_offset_Y equ qword ptr[rbp+72]
dst_offset_UV equ qword ptr[rbp+80]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_offset
	mov r11,dst_offset_Y
	mov r12,dst_offset_UV
	xor rcx,rcx
	
loop_1_h:
	mov ecx,r8d
loop_2_h:
	lodsd
	stosb
	mov byte ptr[rbx],ah
	shr eax,16
	inc rbx
	stosb
	mov byte ptr[rdx],ah
	inc rdx
	loop loop_2_h
	
	add rsi,r10
	add rdi,r11
	add rbx,r12
	add rdx,r12
	dec r9d
	jnz short loop_1_h
	
fin_h:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_src endp



;JPSDR_Median_YUYV_Move_src_SSE_1 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9

JPSDR_Median_YUYV_Move_src_SSE_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch_Y equ qword ptr[rbp+72]
dst_pitch_UV equ qword ptr[rbp+80]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch_Y
	mov r12,dst_pitch_UV
	mov r13,2
	xor rcx,rcx
	movdqa xmm3,oword ptr Ymask
	pxor xmm0,xmm0
	
loop_1_h_1:
	mov ecx,r8d
	xor r14,r14
	shr ecx,2
	jz short suite_h_1	
loop_2_h_1:
	movq xmm0,qword ptr[rsi+8*r14]   ;00000000VYUYVYUY
	movq xmm2,qword ptr[rsi+8*r14+8] ;00000000VYUYVYUY
	pslldq xmm2,8                    ;VYUYVYUY00000000
	por xmm0,xmm2                    ;VYUYVYUYVYUYVYUY
	movdqa xmm1,xmm0                 ;VYUYVYUYVYUYVYUY
	pand xmm0,xmm3                   ;0Y0Y0Y0Y0Y0Y0Y0Y
	psrlw xmm1,8                     ;0V0U0V0U0V0U0V0U
	packuswb xmm0,xmm0               ;00000000YYYYYYYY
	packuswb xmm1,xmm1               ;00000000VUVUVUVU
	movdqa xmm2,xmm1                 ;00000000VUVUVUVU
	pand xmm1,xmm3                   ;000000000U0U0U0U
	psrlw xmm2,8                     ;000000000V0V0V0V
	packuswb xmm1,xmm1               ;000000000000UUUU
	packuswb xmm2,xmm2               ;000000000000VVVV
	movq qword ptr[rdi+4*r14],xmm0
	movd dword ptr[rbx+2*r14],xmm1
	movd dword ptr[rdx+2*r14],xmm2
	pxor xmm0,xmm0
	add r14,r13
	loop loop_2_h_1
suite_h_1:	
    mov r15d,r8d
	and r15d,3	
	jz short suite_h_2
	mov rcx,r14
	
	mov eax,dword ptr[rsi+8*rcx]
	mov byte ptr[rdi+4*rcx],al
	mov byte ptr[rbx+2*rcx],ah
	shr eax,16
	mov byte ptr[rdi+4*rcx+1],al
	mov byte ptr[rdx+2*rcx],ah
	dec r15d
	jz short suite_h_2
	mov eax,dword ptr[rsi+8*rcx+4]
	mov byte ptr[rdi+4*rcx+2],al
	mov byte ptr[rbx+2*rcx+1],ah
	shr eax,16
	mov byte ptr[rdi+4*rcx+3],al
	mov byte ptr[rdx+2*rcx+1],ah
	dec r15d
	jz short suite_h_2
	mov eax,dword ptr[rsi+8*rcx+8]
	mov byte ptr[rdi+4*rcx+4],al
	mov byte ptr[rbx+2*rcx+2],ah
	shr eax,16
	mov byte ptr[rdi+4*rcx+5],al
	mov byte ptr[rdx+2*rcx+2],ah
	
suite_h_2:			
    xor rcx,rcx
	add rsi,r10
	add rdi,r11
	add rbx,r12
	add rdx,r12
	dec r9d
	jnz loop_1_h_1
	
fin_h_1:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_src_SSE_1 endp



;JPSDR_Median_YUYV_Move_src_SSE_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9

JPSDR_Median_YUYV_Move_src_SSE_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch_Y equ qword ptr[rbp+72]
dst_pitch_UV equ qword ptr[rbp+80]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch_Y
	mov r12,dst_pitch_UV
	mov r13,2
	xor rcx,rcx
	movdqa xmm3,oword ptr Ymask
	
loop_1_h_2:
    xor rax,rax
	mov ecx,r8d
loop_2_h_2:
	movdqa xmm0,oword ptr[rsi+8*rax]   ;00000000VYUYVYUY
	movdqa xmm1,xmm0                   ;VYUYVYUYVYUYVYUY
	pand xmm0,xmm3                     ;0Y0Y0Y0Y0Y0Y0Y0Y
	psrlw xmm1,8                       ;0V0U0V0U0V0U0V0U
	packuswb xmm0,xmm0                 ;00000000YYYYYYYY
	packuswb xmm1,xmm1                 ;00000000VUVUVUVU
	movdqa xmm2,xmm1                   ;00000000VUVUVUVU
	pand xmm1,xmm3                     ;000000000U0U0U0U
	psrlw xmm2,8                       ;000000000V0V0V0V
	packuswb xmm1,xmm1                 ;000000000000UUUU
	packuswb xmm2,xmm2                 ;000000000000VVVV
	movq qword ptr[rdi+4*rax],xmm0
	movd dword ptr[rbx+2*rax],xmm1
	movd dword ptr[rdx+2*rax],xmm2
	add rax,r13
	loop loop_2_h_2
	
	add rsi,r10
	add rdi,r11
	add rbx,r12
	add rdx,r12
	dec r9d
	jnz short loop_1_h_2
	
fin_h_2:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_src_SSE_2 endp



;JPSDR_Median_UYVY_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword,dst_offset_Y:dword,dst_offset_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9


JPSDR_Median_UYVY_Move_src proc public frame
w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]
dst_offset_Y equ qword ptr[rbp+72]
dst_offset_UV equ qword ptr[rbp+80]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_offset
	mov r11,2
	mov r12,dst_offset_Y
	mov r13,dst_offset_UV	
	xor rcx,rcx
		
loop_1_i:
	mov ecx,r8d
loop_2_i:
	lodsd
	mov byte ptr[rdi],ah		
	mov byte ptr[rbx],al
	shr eax,16
	inc rbx
	mov byte ptr[rdi+1],ah
	mov byte ptr[rdx],al
	add rdi,r11
	inc rdx
	loop loop_2_i
	
	add rsi,r10
	add rdi,r12
	add rbx,r13
	add rdx,r13	
	dec r9d
	jnz short loop_1_i
	
fin_i:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_src endp



;JPSDR_Median_UYVY_Move_src_SSE_1 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9


JPSDR_Median_UYVY_Move_src_SSE_1 proc public frame
w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch_Y equ qword ptr[rbp+72]
dst_pitch_UV equ qword ptr[rbp+80]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_pitch
	mov r11,2
	mov r12,dst_pitch_Y
	mov r13,dst_pitch_UV	
	xor rcx,rcx
	movdqa xmm3,oword ptr Ymask
	pxor xmm0,xmm0
		
loop_1_i_1:
	mov ecx,r8d
	xor r14,r14
	shr ecx,2
	jz short suite_i_1		
loop_2_i_1:
	movq xmm0,qword ptr[rsi+8*r14]   ;00000000YVYUYVYU
	movq xmm2,qword ptr[rsi+8*r14+8] ;00000000YVYUYVYU
	pslldq xmm2,8                    ;YVYUYVYU00000000
	por xmm0,xmm2                    ;YVYUYVYUYVYUYVYU
	movdqa xmm1,xmm0                 ;YVYUYVYUYVYUYVYU
	pand xmm0,xmm3                   ;0V0U0V0U0V0U0V0U
	psrlw xmm1,8                     ;0Y0Y0Y0Y0Y0Y0Y0Y
	packuswb xmm0,xmm0               ;00000000VUVUVUVU
	packuswb xmm1,xmm1               ;00000000YYYYYYYY
	movdqa xmm2,xmm0                 ;00000000VUVUVUVU
	pand xmm0,xmm3                   ;000000000U0U0U0U
	psrlw xmm2,8                     ;000000000V0V0V0V
	packuswb xmm0,xmm0               ;000000000000UUUU
	packuswb xmm2,xmm2               ;000000000000VVVV
	movq qword ptr[rdi+4*r14],xmm1
	movd dword ptr[rbx+2*r14],xmm0
	movd dword ptr[rdx+2*r14],xmm2
	pxor xmm0,xmm0
	add r14,r11
	loop loop_2_i_1
	
suite_i_1:	
    mov r15d,r8d
	and r15d,3	
	jz short suite_i_2
	mov rcx,r14
	
	mov eax,dword ptr[rsi+8*rcx]
	mov byte ptr[rbx+2*rcx],al
	mov byte ptr[rdi+4*rcx],ah
	shr eax,16
	mov byte ptr[rdx+2*rcx],al
	mov byte ptr[rdi+4*rcx+1],ah
	dec r15d
	jz short suite_i_2
	mov eax,dword ptr[rsi+8*rcx+4]
	mov byte ptr[rbx+2*rcx+1],al
	mov byte ptr[rdi+4*rcx+2],ah
	shr eax,16
	mov byte ptr[rdx+2*rcx+1],al
	mov byte ptr[rdi+4*rcx+3],ah
	dec r15d
	jz short suite_i_2
	mov eax,dword ptr[rsi+8*rcx+8]
	mov byte ptr[rbx+2*rcx+2],al
	mov byte ptr[rdi+4*rcx+4],ah
	shr eax,16
	mov byte ptr[rdx+2*rcx+2],al
	mov byte ptr[rdi+4*rcx+5],ah
	
suite_i_2:			
    xor rcx,rcx
	add rsi,r10
	add rdi,r12
	add rbx,r13
	add rdx,r13	
	dec r9d
	jnz loop_1_i_1
	
fin_i_1:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_src_SSE_1 endp



;JPSDR_Median_UYVY_Move_src_SSE_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9


JPSDR_Median_UYVY_Move_src_SSE_2 proc public frame
w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch_Y equ qword ptr[rbp+72]
dst_pitch_UV equ qword ptr[rbp+80]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_pitch
	mov r11,2
	mov r12,dst_pitch_Y
	mov r13,dst_pitch_UV	
	xor rcx,rcx
	movdqa xmm3,oword ptr Ymask
		
loop_1_i_2:
    xor rax,rax
	mov ecx,r8d
loop_2_i_2:
	movdqa xmm0,oword ptr[rsi+8*rax]   ;00000000YVYUYVYU
	movdqa xmm1,xmm0                   ;YVYUYVYUYVYUYVYU
	pand xmm0,xmm3                     ;0V0U0V0U0V0U0V0U
	psrlw xmm1,8                       ;0Y0Y0Y0Y0Y0Y0Y0Y
	packuswb xmm0,xmm0                 ;00000000VUVUVUVU
	packuswb xmm1,xmm1                 ;00000000YYYYYYYY
	movdqa xmm2,xmm0                   ;00000000VUVUVUVU
	pand xmm0,xmm3                     ;000000000U0U0U0U
	psrlw xmm2,8                       ;000000000V0V0V0V
	packuswb xmm0,xmm0                 ;000000000000UUUU
	packuswb xmm2,xmm2                 ;000000000000VVVV
	movq qword ptr[rdi+4*rax],xmm1
	movd dword ptr[rbx+2*rax],xmm0
	movd dword ptr[rdx+2*rax],xmm2
	add rax,r11
	loop loop_2_i_2
	add rsi,r10
	add rdi,r12
	add rbx,r13
	add rdx,r13	
	dec r9d
	jnz short loop_1_i_2
	
fin_i_2:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_src_SSE_2 endp



;JPSDR_Median_RGB32_Move_dst proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_offset:dword,dst_offset:dword
; planar_R = rcx
; planar_G = rdx
; planar_B = r8
; dst = r9

JPSDR_Median_RGB32_Move_dst proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]
dst_offset equ qword ptr[rbp+72]
	
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
	
	mov rsi,r8
	mov rbx,rdx
	mov rdx,rcx
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_offset
	mov r11,dst_offset
	xor rcx,rcx
	
loop_1_j:
	mov ecx,r8d
loop_2_j:
	movzx eax,byte ptr[rdx]
	inc rdx
	shl eax,16
	lodsb
	mov ah,byte ptr[rbx]
	inc rbx
	stosd
	loop loop_2_j
	
	add rsi,r10
	add rdx,r10
	add rbx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_j
		
fin_j:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_dst endp




;JPSDR_Median_RGB32_Move_dst_SSE_1 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword
; planar_R = rcx
; planar_G = rdx
; planar_B = r8
; dst = r9

JPSDR_Median_RGB32_Move_dst_SSE_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]
	
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
	.endprolog	
	
	mov rsi,r8
	mov rbx,rdx
	mov rdx,rcx
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	pxor xmm3,xmm3
	mov r12,2
	
loop_1_j_1:
	mov ecx,r8d
	xor r13,r13
	shr ecx,2
	jz short suite_j_1			
loop_2_j_1:
    movd xmm0,dword ptr[rsi+2*r13]  ;000000000000BBBB
	movd xmm1,dword ptr[rbx+2*r13]  ;000000000000GGGG
	movd xmm2,dword ptr[rdx+2*r13]  ;000000000000RRRR
	punpcklbw xmm1,xmm3             ;000000000G0G0G0G
	punpcklbw xmm0,xmm2             ;00000000RBRBRBRB
	punpcklbw xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	movq qword ptr[rdi+8*r13],xmm0
	psrldq xmm0,8
	movq qword ptr[rdi+8*r13+8],xmm0
	add r13,r12
	loop loop_2_j_1
suite_j_1:
    mov r14d,r8d
    and r14d,3
    jz short suite_j_2	
	mov rcx,r13
	
	xor eax,eax
	mov al,byte ptr[rdx+2*rcx]
	shl eax,16
	mov al,byte ptr[rsi+2*rcx]
	mov ah,byte ptr[rbx+2*rcx]
	mov dword ptr[rdi+8*rcx],eax
	dec r14d
	jz short suite_j_2
	xor eax,eax
	mov al,byte ptr[rdx+2*rcx+1]
	shl eax,16
	mov al,byte ptr[rsi+2*rcx+1]
	mov ah,byte ptr[rbx+2*rcx+1]
	mov dword ptr[rdi+8*rcx+4],eax
	dec r14d
	jz short suite_j_2
	xor eax,eax
	mov al,byte ptr[rdx+2*rcx+2]
	shl eax,16
	mov al,byte ptr[rsi+2*rcx+2]
	mov ah,byte ptr[rbx+2*rcx+2]
	mov dword ptr[rdi+8*rcx+8],eax
	
suite_j_2:	
    xor rcx,rcx
	add rsi,r10
	add rdx,r10
	add rbx,r10
	add rdi,r11
	dec r9d
	jnz loop_1_j_1
		
fin_j_1:
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_dst_SSE_1 endp



;JPSDR_Median_RGB32_Move_dst_SSE_2 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword
; planar_R = rcx
; planar_G = rdx
; planar_B = r8
; dst = r9

JPSDR_Median_RGB32_Move_dst_SSE_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch equ qword ptr[rbp+64]
dst_pitch equ qword ptr[rbp+72]
	
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
	
	mov rsi,r8
	mov rbx,rdx
	mov rdx,rcx
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_pitch
	mov r11,dst_pitch
	xor rcx,rcx
	pxor xmm3,xmm3
	mov r12,2
	
loop_1_j_2:
    xor rax,rax
	mov ecx,r8d
loop_2_j_2:
    movd xmm0,dword ptr[rsi+2*rax]  ;000000000000BBBB
	movd xmm1,dword ptr[rbx+2*rax]  ;000000000000GGGG
	movd xmm2,dword ptr[rdx+2*rax]  ;000000000000RRRR
	punpcklbw xmm1,xmm3             ;000000000G0G0G0G
	punpcklbw xmm0,xmm2             ;00000000RBRBRBRB
	punpcklbw xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	movdqa oword ptr[rdi+8*rax],xmm0
	add rax,r12
	loop loop_2_j_2
	
	add rsi,r10
	add rdx,r10
	add rbx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_j_2
		
fin_j_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_dst_SSE_2 endp



;JPSDR_Median_YUYV_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_offset_Y:dword,src_offset_UV:dword,dst_offset:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_YUYV_Move_dst proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset_Y equ qword ptr[rbp+64]
src_offset_UV equ qword ptr[rbp+72]
dst_offset equ qword ptr[rbp+80]
	
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
	mov rbx,rdx
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_offset
	mov r11,src_offset_Y
	mov r12,src_offset_UV
	xor rcx,rcx
			
loop_1_k:
	mov ecx,r8d
loop_2_k:
	mov ah,byte ptr[rdx]
	mov al,byte ptr[rsi+1]
	inc rdx
	shl eax,16
	lodsb
	mov ah,byte ptr[rbx]
	inc rsi
	stosd
	inc rbx
	loop loop_2_k
	
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_k
		
fin_k:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_dst endp



;JPSDR_Median_YUYV_Move_dst_SSE_1 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_YUYV_Move_dst_SSE_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch_Y equ qword ptr[rbp+64]
src_pitch_UV equ qword ptr[rbp+72]
dst_pitch equ qword ptr[rbp+80]
	
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
	mov rbx,rdx
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_pitch
	mov r11,src_pitch_Y
	mov r12,src_pitch_UV
	xor rcx,rcx
	mov r13,2
			
loop_1_k_1:
	mov ecx,r8d
	xor r14,r14
	shr ecx,2
	jz short suite_k_1				
loop_2_k_1:
	movd xmm1,dword ptr[rbx+2*r14]  ;000000000000UUUU
	movd xmm2,dword ptr[rdx+2*r14]  ;000000000000VVVV
	movq xmm0,qword ptr[rsi+4*r14]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	movq qword ptr[rdi+8*r14],xmm0
	psrldq xmm0,8
	movq qword ptr[rdi+8*r14+8],xmm0
	add r14,r13
	loop loop_2_k_1
suite_k_1:
    mov r15d,r8d
    and r15d,3
    jz short suite_k_2	
	mov rcx,r14
	
	mov al,byte ptr[rsi+4*rcx+1]
	mov ah,byte ptr[rdx+2*rcx]
	shl eax,16
	mov al,byte ptr[rsi+4*rcx]
	mov ah,byte ptr[rbx+2*rcx]
	mov dword ptr[rdi+8*rcx],eax
	dec r15d
	jz short suite_k_2
	mov al,byte ptr[rsi+4*rcx+3]
	mov ah,byte ptr[rdx+2*rcx+1]
	shl eax,16
	mov al,byte ptr[rsi+4*rcx+2]
	mov ah,byte ptr[rbx+2*rcx+1]
	mov dword ptr[rdi+8*rcx+4],eax
	dec r15d
	jz short suite_k_2
	mov al,byte ptr[rsi+4*rcx+5]
	mov ah,byte ptr[rdx+2*rcx+2]
	shl eax,16
	mov al,byte ptr[rsi+4*rcx+4]
	mov ah,byte ptr[rbx+2*rcx+2]
	mov dword ptr[rdi+8*rcx+8],eax
	
suite_k_2:		
    xor rcx,rcx
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz loop_1_k_1
		
fin_k_1:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_dst_SSE_1 endp



;JPSDR_Median_YUYV_Move_dst_SSE_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_YUYV_Move_dst_SSE_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch_Y equ qword ptr[rbp+64]
src_pitch_UV equ qword ptr[rbp+72]
dst_pitch equ qword ptr[rbp+80]
	
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
	mov rbx,rdx
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_pitch
	mov r11,src_pitch_Y
	mov r12,src_pitch_UV
	xor rcx,rcx
	mov r13,2
			
loop_1_k_2:
    xor rax,rax
	mov ecx,r8d
loop_2_k_2:
	movd xmm1,dword ptr[rbx+2*rax]  ;000000000000UUUU
	movd xmm2,dword ptr[rdx+2*rax]  ;000000000000VVVV
	movq xmm0,qword ptr[rsi+4*rax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	movdqa oword ptr[rdi+8*rax],xmm0
	add rax,r13
	loop loop_2_k_2
	
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_k_2
		
fin_k_2:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_dst_SSE_2 endp



;JPSDR_Median_UYVY_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_offset_Y:dword,src_offset_UV:dword,dst_offset:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9


JPSDR_Median_UYVY_Move_dst proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset_Y equ qword ptr[rbp+64]
src_offset_UV equ qword ptr[rbp+72]
dst_offset equ qword ptr[rbp+80]
	
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
	mov rbx,rdx
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_offset
	mov r11,2
	mov r12,src_offset_Y
	mov r13,src_offset_UV
	xor rcx,rcx
	
loop_1_l:
	mov ecx,r8d
loop_2_l:
	mov al,byte ptr[rdx]
	mov ah,byte ptr[rsi+1]
	inc rdx
	shl eax,16
	mov ah,byte ptr[rsi]
	mov al,byte ptr[rbx]
	add rsi,r11
	stosd
	inc rbx
	loop loop_2_l
	
	add rsi,r12
	add rdx,r13
	add rbx,r13
	add rdi,r10
	dec r9d
	jnz short loop_1_l
		
fin_l:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_dst endp


;JPSDR_Median_UYVY_Move_dst_SSE_1 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_UYVY_Move_dst_SSE_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch_Y equ qword ptr[rbp+64]
src_pitch_UV equ qword ptr[rbp+72]
dst_pitch equ qword ptr[rbp+80]
	
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
	mov rbx,rdx
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_pitch
	mov r11,src_pitch_Y
	mov r12,src_pitch_UV
	xor rcx,rcx
	mov r13,2
			
loop_1_l_1:
	mov ecx,r8d
	xor r14,r14
	shr ecx,2
	jz short suite_l_1					
loop_2_l_1:
	movd xmm1,dword ptr[rbx+2*r14]  ;000000000000UUUU
	movd xmm2,dword ptr[rdx+2*r14]  ;000000000000VVVV
	movq xmm0,qword ptr[rsi+4*r14]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	movq qword ptr[rdi+8*r14],xmm1
	psrldq xmm1,8
	movq qword ptr[rdi+8*r14+8],xmm1
	add r14,r13
	loop loop_2_l_1
suite_l_1:
    mov r15d,r8d
    and r15d,3
    jz short suite_l_2	
	mov rcx,r14
	
	mov al,byte ptr[rdx+2*rcx]
	mov ah,byte ptr[rsi+4*rcx+1]
	shl eax,16
	mov al,byte ptr[rbx+2*rcx]
	mov ah,byte ptr[rsi+4*rcx]
	mov dword ptr[rdi+8*rcx],eax
	dec r15d
	jz short suite_l_2
	mov al,byte ptr[rdx+2*rcx+1]
	mov ah,byte ptr[rsi+4*rcx+3]
	shl eax,16
	mov al,byte ptr[rbx+2*rcx+1]
	mov ah,byte ptr[rsi+4*rcx+2]
	mov dword ptr[rdi+8*rcx+4],eax
	dec r15d
	jz short suite_l_2
	mov al,byte ptr[rdx+2*rcx+2]
	mov ah,byte ptr[rsi+4*rcx+5]
	shl eax,16
	mov al,byte ptr[rbx+2*rcx+2]
	mov ah,byte ptr[rsi+4*rcx+4]
	mov dword ptr[rdi+8*rcx+8],eax
	
suite_l_2:
    xor rcx,rcx
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz loop_1_l_1
		
fin_l_1:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_dst_SSE_1 endp



;JPSDR_Median_UYVY_Move_dst_SSE_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_UYVY_Move_dst_SSE_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_pitch_Y equ qword ptr[rbp+64]
src_pitch_UV equ qword ptr[rbp+72]
dst_pitch equ qword ptr[rbp+80]
	
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
	mov rbx,rdx
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_pitch
	mov r11,src_pitch_Y
	mov r12,src_pitch_UV
	xor rcx,rcx
	mov r13,2
			
loop_1_l_2:
    xor rax,rax
	mov ecx,r8d
loop_2_l_2:
	movd xmm1,dword ptr[rbx+2*rax]  ;000000000000UUUU
	movd xmm2,dword ptr[rdx+2*rax]  ;000000000000VVVV
	movq xmm0,qword ptr[rsi+4*rax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	movdqa oword ptr[rdi+8*rax],xmm1
	add rax,r13
	loop loop_2_l_2
	
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_l_2
		
fin_l_2:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_dst_SSE_2 endp

end





