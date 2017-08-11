.data

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
	push r13
	.pushreg r13
	push r14
	.pushreg r14
	.endprolog	
	
	mov rsi,rcx
	mov rdi,r9			; rdi=planar_B
	mov r10,rdx			; r10=planar_R
	mov rbx,r8			; rbx=planar_G
	mov edx,w
	mov r8d,h
	mov r9,src_pitch
	mov r11,dst_pitch
	mov r12,8
	mov r13d,1
	mov r14d,edx
	shr r14d,1
	xor rcx,rcx
	
loop_1_g_2:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_g_2

loop_2_g_2:
	movdqa xmm0,XMMWORD ptr[rsi+4*rax] ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	movdqa xmm1,XMMWORD ptr[rsi+4*rax+16] ;xR8G8B8xR7G7B7xR6G6B6xR5G5B5	
	movdqa xmm2,xmm0                 ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	punpcklbw xmm0,xmm1              ;xxR6R2G6G2B6B2xxR5R1G5G1B5B1
	punpckhbw xmm2,xmm1              ;xxR8R4G8G4B8B4xxR7R3G7G3B7B3
	movdqa xmm1,xmm0                 ;xxR6R2G6G2B6B2xxR5R1G5G1B5B1
	punpcklbw xmm0,xmm2              ;xxxxR7R5R3R1G7G5G3G1B7B5B3B1
	punpckhbw xmm1,xmm2              ;xxxxR8R6R4R2G8G6G4G2B8B6B4B2
	movdqa xmm2,xmm0                 ;xxxxR7R5R3R1G7G5G3G1B7B5B3B1
	punpcklbw xmm0,xmm1              ;G8G7G6G5G4G3G2G1B8B7B6B5B4B3B2B1
	punpckhbw xmm2,xmm1              ;xxxxxxxxR8R7R6R5R4R3R2R1
	
	movq qword ptr[rdi+rax],xmm0
	movhps qword ptr[rbx+rax],xmm0
	movq qword ptr[r10+rax],xmm2
	add rax,r12
	loop loop_2_g_2
	
suite1_g_2:
	mov ecx,edx
	and ecx,r13d
	jz short suite2_g_2
	
	movdqa xmm0,XMMWORD ptr[rsi+4*rax] ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	movhlps xmm1,xmm0                  ;xR4G4B4xR3G3B3xR4G4B4xR3G3B3
	punpcklbw xmm0,xmm1                ;xxR4R2G4G2B4B2xxR3R1G3G1B3B1
	movhlps xmm1,xmm0                  ;xxR4R2G4G2B4B2xxR4R2G4G2B4B2
	punpcklbw xmm0,xmm1                ;xxxxR4R3R2R1G4G3G2G1B4B3B2B1
	movhlps xmm1,xmm0                  ;xxxxR4R3R2R1xxxxR4R3R2R1
	movd dword ptr[rdi+rax],xmm0
	psrlq xmm0,32
	movd dword ptr[r10+rax],xmm1
	movd dword ptr[rbx+rax],xmm0
	
suite2_g_2:			
	add rsi,r9
	add rdi,r11
	add rbx,r11
	add r10,r11
	dec r8d
	jnz loop_1_g_2
			
fin_g_2:
    pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_src_SSE_2 endp


;JPSDR_Median_RGB32_Move_src_AVX_2 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword
; src = rcx
; planar_R = rdx
; planar_G = r8
; planar_B = r9

JPSDR_Median_RGB32_Move_src_AVX_2 proc public frame

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
	mov rdi,r9			; rdi=planar_B
	mov r10,rdx			; r10=planar_R
	mov rbx,r8			; rbx=planar_G
	mov edx,w
	mov r8d,h
	mov r9,src_pitch
	mov r11,dst_pitch
	mov r12,8
	mov r13d,1
	mov r14d,edx
	shr r14d,1
	xor rcx,rcx
	
loop_1_g_2_AVX:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_g_2_AVX

loop_2_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+4*rax]    ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	vmovdqa xmm1,XMMWORD ptr[rsi+4*rax+16] ;xR8G8B8xR7G7B7xR6G6B6xR5G5B5	
	vpunpckhbw xmm2,xmm0,xmm1              ;xxR8R4G8G4B8B4xxR7R3G7G3B7B3
	vpunpcklbw xmm0,xmm0,xmm1              ;xxR6R2G6G2B6B2xxR5R1G5G1B5B1
	vpunpckhbw xmm1,xmm0,xmm2              ;xxxxR8R6R4R2G8G6G4G2B8B6B4B2
	vpunpcklbw xmm0,xmm0,xmm2              ;xxxxR7R5R3R1G7G5G3G1B7B5B3B1
	vpunpckhbw xmm2,xmm0,xmm1              ;xxxxxxxxR8R7R6R5R4R3R2R1
	vpunpcklbw xmm0,xmm0,xmm1              ;G8G7G6G5G4G3G2G1B8B7B6B5B4B3B2B1
	
	vmovq qword ptr[rdi+rax],xmm0
	vmovhps qword ptr[rbx+rax],xmm0
	vmovq qword ptr[r10+rax],xmm2
	add rax,r12
	loop loop_2_g_2_AVX
	
suite1_g_2_AVX:
	mov ecx,edx
	and ecx,r13d
	jz short suite2_g_2_AVX
	
	vmovdqa xmm0,XMMWORD ptr[rsi+4*rax]      ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	vmovhlps xmm1,xmm1,xmm0                  ;xR4G4B4xR3G3B3xR4G4B4xR3G3B3
	vpunpcklbw xmm0,xmm0,xmm1                ;xxR4R2G4G2B4B2xxR3R1G3G1B3B1
	vmovhlps xmm1,xmm1,xmm0                  ;xxR4R2G4G2B4B2xxR4R2G4G2B4B2
	vpunpcklbw xmm0,xmm0,xmm1                ;xxxxR4R3R2R1G4G3G2G1B4B3B2B1
	vmovhlps xmm1,xmm1,xmm0                  ;xxxxR4R3R2R1xxxxR4R3R2R1
	vmovd dword ptr[rdi+rax],xmm0
	vpsrlq xmm0,xmm0,32
	vmovd dword ptr[r10+rax],xmm1
	vmovd dword ptr[rbx+rax],xmm0
	
suite2_g_2_AVX:			
	add rsi,r9
	add rdi,r11
	add rbx,r11
	add r10,r11
	dec r8d
	jnz loop_1_g_2_AVX
			
fin_g_2_AVX:
    pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_src_AVX_2 endp


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
	mov r13,8
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
	xor rcx,rcx
	
loop_1_h_2:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_h_2

loop_2_h_2:
	movdqa xmm0,XMMWORD ptr[rsi+4*rax]   ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	movdqa xmm1,XMMWORD ptr[rsi+4*rax+16]   ;V8Y16U8Y15V7Y14U7Y13 V6Y12U6Y11V5Y10U5Y9
	movdqa xmm2,xmm0                      ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	punpcklbw xmm0,xmm1                  ;V6V2Y12Y4U6U2Y11Y3 V5V1Y10Y2U5U1Y9Y1
	punpckhbw xmm2,xmm1                  ;V8V4Y16Y8U8U4Y15Y7 V7V3Y14Y6U7U3Y13Y5
	movdqa xmm1,xmm0
	punpcklbw xmm0,xmm2                  ;V7V5V3V1Y14Y10Y6Y2 U7U5U3U1Y13Y9Y5Y1
	punpckhbw xmm1,xmm2                  ;V8V6V4V2Y16Y12Y8Y4 U8U6U4U2Y15Y11Y7Y3
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm1                  ;U8U7U6U5U4U3U2U1 Y15Y13Y11Y9Y5Y3Y1
	punpckhbw xmm2,xmm1                  ;V8V7V6V5V4V3V2V1 Y16Y14Y12Y10Y8Y6Y4Y2
	movhps qword ptr [rbx+rax],xmm0
	punpcklbw xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	movhps qword ptr [rdx+rax],xmm2
	movdqa XMMWORD ptr[rdi+2*rax],xmm0
	add rax,r13
	loop loop_2_h_2
	
suite1_h_2:
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_h_2
    
	movdqa xmm0,XMMWORD ptr[rsi+4*rax]   ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	movhlps xmm1,xmm0                    ;V4Y8U4Y7V3Y6U3Y5 V4Y8U4Y7V3Y6U3Y5
	punpcklbw xmm0,xmm1                  ;V4V2Y8Y4U4U2Y7Y3 V3V1Y6Y2U3U1Y5Y1
	movhlps xmm1,xmm0                    ;V4V2Y8Y4U4U2Y7Y3 V4V2Y8Y4U4U2Y7Y3
	punpcklbw xmm0,xmm1                  ;V4V3V2V1Y8Y6Y4Y2 U4U3U2U1Y7Y5Y3Y1
	movhlps xmm2,xmm0                    ;xxxxxxxx V4V3V2V1Y8Y6Y4Y2
	movdqa xmm1,xmm0
	psrlq xmm0,32                        ;0000V4V3V2V1 0000U4U3U2U1
	punpcklbw xmm1,xmm2                  ; xxxxxxxx Y8Y7Y6Y5Y4Y3Y2Y1
	movd dword ptr[rbx+rax],xmm0
	movhlps xmm2,xmm0
	movq qword ptr[rdi+2*rax],xmm1
	movd dword ptr[rdx+rax],xmm2

suite2_h_2:		
	add rsi,r10
	add rdi,r11
	add rbx,r12
	add rdx,r12
	dec r9d
	jnz loop_1_h_2
	
fin_h_2:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_src_SSE_2 endp


;JPSDR_Median_YUYV_Move_src_AVX_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9

JPSDR_Median_YUYV_Move_src_AVX_2 proc public frame

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
	mov r13,8
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
	xor rcx,rcx
	
loop_1_h_2_AVX:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_h_2_AVX

loop_2_h_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+4*rax]        ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	vmovdqa xmm1,XMMWORD ptr[rsi+4*rax+16]     ;V8Y16U8Y15V7Y14U7Y13 V6Y12U6Y11V5Y10U5Y9
	vpunpckhbw xmm2,xmm0,xmm1                  ;V8V4Y16Y8U8U4Y15Y7 V7V3Y14Y6U7U3Y13Y5
	vpunpcklbw xmm0,xmm0,xmm1                  ;V6V2Y12Y4U6U2Y11Y3 V5V1Y10Y2U5U1Y9Y1
	vpunpckhbw xmm1,xmm0,xmm2                  ;V8V6V4V2Y16Y12Y8Y4 U8U6U4U2Y15Y11Y7Y3
	vpunpcklbw xmm0,xmm0,xmm2                  ;V7V5V3V1Y14Y10Y6Y2 U7U5U3U1Y13Y9Y5Y1
	vpunpckhbw xmm2,xmm0,xmm1                  ;V8V7V6V5V4V3V2V1 Y16Y14Y12Y10Y8Y6Y4Y2
	vpunpcklbw xmm0,xmm0,xmm1                  ;U8U7U6U5U4U3U2U1 Y15Y13Y11Y9Y5Y3Y1
	vmovhps qword ptr [rbx+rax],xmm0
	vpunpcklbw xmm0,xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	vmovhps qword ptr [rdx+rax],xmm2
	vmovdqa XMMWORD ptr[rdi+2*rax],xmm0
	add rax,r13
	loop loop_2_h_2_AVX
	
suite1_h_2_AVX:
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_h_2_AVX
    
	vmovdqa xmm0,XMMWORD ptr[rsi+4*rax]        ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	vmovhlps xmm1,xmm1,xmm0                    ;V4Y8U4Y7V3Y6U3Y5 V4Y8U4Y7V3Y6U3Y5
	vpunpcklbw xmm0,xmm0,xmm1                  ;V4V2Y8Y4U4U2Y7Y3 V3V1Y6Y2U3U1Y5Y1
	vmovhlps xmm1,xmm1,xmm0                    ;V4V2Y8Y4U4U2Y7Y3 V4V2Y8Y4U4U2Y7Y3
	vpunpcklbw xmm0,xmm0,xmm1                  ;V4V3V2V1Y8Y6Y4Y2 U4U3U2U1Y7Y5Y3Y1
	vmovhlps xmm2,xmm2,xmm0                    ;xxxxxxxx V4V3V2V1Y8Y6Y4Y2
	vpunpcklbw xmm1,xmm0,xmm2                  ; xxxxxxxx Y8Y7Y6Y5Y4Y3Y2Y1
	vpsrlq xmm0,xmm0,32                        ;0000V4V3V2V1 0000U4U3U2U1
	vmovd dword ptr[rbx+rax],xmm0
	vmovhlps xmm2,xmm2,xmm0
	vmovq qword ptr[rdi+2*rax],xmm1
	vmovd dword ptr[rdx+rax],xmm2

suite2_h_2_AVX:		
	add rsi,r10
	add rdi,r11
	add rbx,r12
	add rdx,r12
	dec r9d
	jnz loop_1_h_2_AVX
	
fin_h_2_AVX:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_src_AVX_2 endp


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
	mov r11,8
	mov r12,dst_pitch_Y
	mov r13,dst_pitch_UV	
	xor rcx,rcx
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
		
loop_1_i_2:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_i_2
	
loop_2_i_2:
	movdqa xmm0,XMMWORD ptr[rsi+4*rax]   ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	movdqa xmm1,XMMWORD ptr[rsi+4*rax+16]   ;Y16V8Y15U8Y14V7Y13U7 Y12V6Y11U6Y10V5Y9U5
	movdqa xmm2,xmm0                      ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	punpcklbw xmm0,xmm1                  ;Y12Y4V6V2Y11Y3U6U2 Y10Y2V5V1Y9Y1U5U1
	punpckhbw xmm2,xmm1                  ;Y16Y8V8V4Y15Y7U8U4 Y14Y6V7V3Y13Y5U7U3
	movdqa xmm1,xmm0
	punpcklbw xmm0,xmm2                  ;Y14Y10Y6Y2V7V5V3V1 Y13Y9Y5Y1U7U5U3U1
	punpckhbw xmm1,xmm2                  ;Y16Y12Y8Y4V8V6V4V2 Y15Y11Y7Y3U8U6U4U2
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm1                  ;Y15Y13Y11Y9Y7Y5Y3Y1 U8U7U6U5U4U3U2U1
	punpckhbw xmm2,xmm1                  ;Y16Y14Y12Y10Y8Y6Y4Y2 V8V7V6V5V4v3V2V1
	movq qword ptr [rbx+rax],xmm0
	punpckhbw xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	movq qword ptr [rdx+rax],xmm2
	movdqa XMMWORD ptr[rdi+2*rax],xmm0
	add rax,r11
	loop loop_2_i_2
	
suite1_i_2:
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_i_2

	movdqa xmm0,XMMWORD ptr[rsi+4*rax]   ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	movhlps xmm1,xmm0                    ;Y8V4Y7U4Y6V3Y5U3 Y8V4Y7U4Y6V3Y5U3
	punpcklbw xmm0,xmm1                  ;Y8Y4V4V2Y7Y3U4U2 Y6Y2V3V1Y5Y1U3U1
	movhlps xmm1,xmm0                    ;Y8Y4V4V2Y7Y3U4U2 Y8Y4V4V2Y7Y3U4U2
	punpcklbw xmm0,xmm1                  ;Y8Y6Y4Y2V4V3V2V1 Y7Y5Y3Y1U4U3U2U1
	movhlps xmm2,xmm0                    ;xxxxxxxx Y8Y6Y4Y2V4V3V2V1	
	movd dword ptr [rbx+rax],xmm0
	punpcklbw xmm0,xmm2                  ;Y8Y7Y6Y5Y4Y3Y2Y1 xxxxxxxx
	movd dword ptr [rdx+rax],xmm2
	movhps qword ptr[rdi+2*rax],xmm0
	
suite2_i_2:
	add rsi,r10
	add rdi,r12
	add rbx,r13
	add rdx,r13	
	dec r9d
	jnz loop_1_i_2
	
fin_i_2:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_src_SSE_2 endp


;JPSDR_Median_UYVY_Move_src_AVX_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9

JPSDR_Median_UYVY_Move_src_AVX_2 proc public frame
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
	mov r11,8
	mov r12,dst_pitch_Y
	mov r13,dst_pitch_UV	
	xor rcx,rcx
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
		
loop_1_i_2_AVX:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_i_2_AVX
	
loop_2_i_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+4*rax]        ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	vmovdqa xmm1,XMMWORD ptr[rsi+4*rax+16]     ;Y16V8Y15U8Y14V7Y13U7 Y12V6Y11U6Y10V5Y9U5
	vpunpckhbw xmm2,xmm0,xmm1                  ;Y16Y8V8V4Y15Y7U8U4 Y14Y6V7V3Y13Y5U7U3
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y12Y4V6V2Y11Y3U6U2 Y10Y2V5V1Y9Y1U5U1
	vpunpckhbw xmm1,xmm0,xmm2                  ;Y16Y12Y8Y4V8V6V4V2 Y15Y11Y7Y3U8U6U4U2
	vpunpcklbw xmm0,xmm0,xmm2                  ;Y14Y10Y6Y2V7V5V3V1 Y13Y9Y5Y1U7U5U3U1
	vpunpckhbw xmm2,xmm0,xmm1                  ;Y16Y14Y12Y10Y8Y6Y4Y2 V8V7V6V5V4v3V2V1
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y15Y13Y11Y9Y7Y5Y3Y1 U8U7U6U5U4U3U2U1
	vmovq qword ptr [rbx+rax],xmm0
	vpunpckhbw xmm0,xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	vmovq qword ptr [rdx+rax],xmm2
	vmovdqa XMMWORD ptr[rdi+2*rax],xmm0
	add rax,r11
	loop loop_2_i_2_AVX
	
suite1_i_2_AVX:
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_i_2_AVX

	vmovdqa xmm0,XMMWORD ptr[rsi+4*rax]        ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	vmovhlps xmm1,xmm1,xmm0                    ;Y8V4Y7U4Y6V3Y5U3 Y8V4Y7U4Y6V3Y5U3
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y8Y4V4V2Y7Y3U4U2 Y6Y2V3V1Y5Y1U3U1
	vmovhlps xmm1,xmm1,xmm0                    ;Y8Y4V4V2Y7Y3U4U2 Y8Y4V4V2Y7Y3U4U2
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y8Y6Y4Y2V4V3V2V1 Y7Y5Y3Y1U4U3U2U1
	vmovhlps xmm2,xmm2,xmm0                    ;xxxxxxxx Y8Y6Y4Y2V4V3V2V1	
	vmovd dword ptr [rbx+rax],xmm0
	vpunpcklbw xmm0,xmm0,xmm2                  ;Y8Y7Y6Y5Y4Y3Y2Y1 xxxxxxxx
	vmovd dword ptr [rdx+rax],xmm2
	vmovhps qword ptr[rdi+2*rax],xmm0
	
suite2_i_2_AVX:
	add rsi,r10
	add rdi,r12
	add rbx,r13
	add rdx,r13	
	dec r9d
	jnz loop_1_i_2_AVX
	
fin_i_2_AVX:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_src_AVX_2 endp


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
	mov r12,8
	mov r13d,1
	mov r14d,r8d
	shr r14d,1	
	
loop_1_j_2:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_j_2
	
loop_2_j_2:
    movq xmm0,qword ptr[rsi+rax]  ;00000000B8B7B6B5B4B3B2B1
	movq xmm2,qword ptr[rdx+rax]  ;00000000R8R7R6R5R4R3R2R1
	movq xmm1,qword ptr[rbx+rax]  ;00000000G8G7G6G5G4G3G2G1
	punpcklbw xmm0,xmm2             ;R8B8R7B7R6B6R5B5R4B4R3B3R2B2R1B1
	punpcklbw xmm1,xmm3             ;0G80G70G60G50G40G30G20G1
	movdqa xmm2,xmm0                ;R8B8R7B7R6B6R5B5R4B4R3B3R2B2R1B1
	punpcklbw xmm0,xmm1             ;0R4G4B40R3G3B30R2G2B20R1G1B1
	punpckhbw xmm2,xmm1             ;0R8G8B80R7G7B70R6G6B60R5G5B5
	movdqa XMMWORD ptr[rdi+4*rax],xmm0
	movdqa XMMWORD ptr[rdi+4*rax+16],xmm2
	add rax,r12
	loop loop_2_j_2

suite1_j_2:
	mov ecx,r8d
	and ecx,r13d
	jz short suite2_j_2

    movd xmm0,dword ptr[rsi+rax]  ;000000000000BBBB
	movd xmm1,dword ptr[rbx+rax]  ;000000000000GGGG
	movd xmm2,dword ptr[rdx+rax]  ;000000000000RRRR
	punpcklbw xmm1,xmm3             ;000000000G0G0G0G
	punpcklbw xmm0,xmm2             ;00000000RBRBRBRB
	punpcklbw xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	movdqa XMMWORD ptr[rdi+4*rax],xmm0

suite2_j_2:	
	add rsi,r10
	add rdx,r10
	add rbx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_j_2
		
fin_j_2:
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_dst_SSE_2 endp


;JPSDR_Median_RGB32_Move_dst_AVX_2 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword
; planar_R = rcx
; planar_G = rdx
; planar_B = r8
; dst = r9

JPSDR_Median_RGB32_Move_dst_AVX_2 proc public frame

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
	vpxor xmm3,xmm3,xmm3
	mov r12,8
	mov r13d,1
	mov r14d,r8d
	shr r14d,1	
	
loop_1_j_2_AVX:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_j_2_AVX
	
loop_2_j_2_AVX:
    vmovq xmm0,qword ptr[rsi+rax]         ;00000000B8B7B6B5B4B3B2B1
	vmovq xmm2,qword ptr[rdx+rax]         ;00000000R8R7R6R5R4R3R2R1
	vmovq xmm1,qword ptr[rbx+rax]         ;00000000G8G7G6G5G4G3G2G1
	vpunpcklbw xmm0,xmm0,xmm2             ;R8B8R7B7R6B6R5B5R4B4R3B3R2B2R1B1
	vpunpcklbw xmm1,xmm1,xmm3             ;0G80G70G60G50G40G30G20G1
	vpunpckhbw xmm2,xmm0,xmm1             ;0R8G8B80R7G7B70R6G6B60R5G5B5
	vpunpcklbw xmm0,xmm0,xmm1             ;0R4G4B40R3G3B30R2G2B20R1G1B1
	vmovdqa XMMWORD ptr[rdi+4*rax],xmm0
	vmovdqa XMMWORD ptr[rdi+4*rax+16],xmm2
	add rax,r12
	loop loop_2_j_2_AVX

suite1_j_2_AVX:
	mov ecx,r8d
	and ecx,r13d
	jz short suite2_j_2_AVX

    vmovd xmm0,dword ptr[rsi+rax]         ;000000000000BBBB
	vmovd xmm1,dword ptr[rbx+rax]         ;000000000000GGGG
	vmovd xmm2,dword ptr[rdx+rax]         ;000000000000RRRR
	vpunpcklbw xmm1,xmm1,xmm3             ;000000000G0G0G0G
	vpunpcklbw xmm0,xmm0,xmm2             ;00000000RBRBRBRB
	vpunpcklbw xmm0,xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	vmovdqa XMMWORD ptr[rdi+4*rax],xmm0

suite2_j_2_AVX:	
	add rsi,r10
	add rdx,r10
	add rbx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_j_2_AVX
		
fin_j_2_AVX:
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_dst_AVX_2 endp


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
	mov r13,8
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
			
loop_1_k_2:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_k_2
	
loop_2_k_2:
	movq xmm1,qword ptr[rbx+rax]  ;00000000UUUUUUUU
	movq xmm2,qword ptr[rdx+rax]  ;00000000VVVVVVVV
	movdqa xmm0,XMMWORD ptr[rsi+2*rax]  ;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	movdqa xmm2,xmm0                ;YYYYYYYYYYYYYYYY
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	punpckhbw xmm2,xmm1             ;VYUYVYUYVYUYVYUY	
	movdqa XMMWORD ptr[rdi+4*rax],xmm0
	movdqa XMMWORD ptr[rdi+4*rax+16],xmm2
	add rax,r13
	loop loop_2_k_2
	
suite1_k_2:	
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_k_2

	movd xmm1,dword ptr[rbx+rax]  ;000000000000UUUU
	movd xmm2,dword ptr[rdx+rax]  ;000000000000VVVV
	movq xmm0,qword ptr[rsi+2*rax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[rdi+4*rax],xmm0
	
suite2_k_2:	
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_k_2
		
fin_k_2:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_dst_SSE_2 endp


;JPSDR_Median_YUYV_Move_dst_AVX_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_YUYV_Move_dst_AVX_2 proc public frame

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
	mov r13,8
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
			
loop_1_k_2_AVX:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_k_2_AVX
	
loop_2_k_2_AVX:
	vmovq xmm1,qword ptr[rbx+rax]         ;00000000UUUUUUUU
	vmovq xmm2,qword ptr[rdx+rax]         ;00000000VVVVVVVV
	vmovdqa xmm0,XMMWORD ptr[rsi+2*rax]   ;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	vpunpckhbw xmm2,xmm0,xmm1             ;VYUYVYUYVYUYVYUY	
	vpunpcklbw xmm0,xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[rdi+4*rax],xmm0
	vmovdqa XMMWORD ptr[rdi+4*rax+16],xmm2
	add rax,r13
	loop loop_2_k_2_AVX
	
suite1_k_2_AVX:	
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_k_2_AVX

	vmovd xmm1,dword ptr[rbx+rax]         ;000000000000UUUU
	vmovd xmm2,dword ptr[rdx+rax]         ;000000000000VVVV
	vmovq xmm0,qword ptr[rsi+2*rax]       ;00000000YYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;00000000VUVUVUVU
	vpunpcklbw xmm0,xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[rdi+4*rax],xmm0
	
suite2_k_2_AVX:	
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_k_2_AVX
		
fin_k_2_AVX:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_dst_AVX_2 endp


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
	mov r13,8
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
			
loop_1_l_2:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_l_2	
	
loop_2_l_2:
	movq xmm1,qword ptr[rbx+rax]  ;00000000UUUUUUUU
	movq xmm2,qword ptr[rdx+rax]  ;00000000VVVVVVVV
	movdqa xmm0,XMMWORD ptr[rsi+2*rax]  ;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	movdqa xmm2,xmm1                ;VUVUVUVUVUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	punpckhbw xmm2,xmm0             ;YVYUYVYUYVYUYVYU
	movdqa XMMWORD ptr[rdi+4*rax],xmm1
	movdqa XMMWORD ptr[rdi+4*rax+16],xmm2
	add rax,r13
	loop loop_2_l_2
	
suite1_l_2:
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_l_2

	movd xmm1,dword ptr[rbx+rax]  ;000000000000UUUU
	movd xmm2,dword ptr[rdx+rax]  ;000000000000VVVV
	movq xmm0,qword ptr[rsi+2*rax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	movdqa XMMWORD ptr[rdi+4*rax],xmm1
	
suite2_l_2:
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_l_2
		
fin_l_2:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_dst_SSE_2 endp


;JPSDR_Median_UYVY_Move_dst_AVX_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_UYVY_Move_dst_AVX_2 proc public frame

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
	mov r13,8
	mov r14d,r8d
	shr r14d,1
	mov r15d,1
			
loop_1_l_2_AVX:
    xor rax,rax
	mov ecx,r14d
	or ecx,ecx
	jz short suite1_l_2_AVX
	
loop_2_l_2_AVX:
	vmovq xmm1,qword ptr[rbx+rax]  ;00000000UUUUUUUU
	vmovq xmm2,qword ptr[rdx+rax]  ;00000000VVVVVVVV
	vmovdqa xmm0,XMMWORD ptr[rsi+2*rax]  ;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	vpunpckhbw xmm2,xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	vpunpcklbw xmm1,xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	vmovdqa XMMWORD ptr[rdi+4*rax],xmm1
	vmovdqa XMMWORD ptr[rdi+4*rax+16],xmm2
	add rax,r13
	loop loop_2_l_2_AVX
	
suite1_l_2_AVX:
	mov ecx,r8d
	and ecx,r15d
	jz short suite2_l_2_AVX

	vmovd xmm1,dword ptr[rbx+rax]  ;000000000000UUUU
	vmovd xmm2,dword ptr[rdx+rax]  ;000000000000VVVV
	vmovq xmm0,qword ptr[rsi+2*rax]  ;00000000YYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	vmovdqa XMMWORD ptr[rdi+4*rax],xmm1
	
suite2_l_2_AVX:
	add rsi,r11
	add rdx,r12
	add rbx,r12
	add rdi,r10
	dec r9d
	jnz short loop_1_l_2_AVX
		
fin_l_2_AVX:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_dst_AVX_2 endp


end





