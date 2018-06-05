.code

;JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2 proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2 proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax	
	mov ecx,r9d	
	mov r11,16
	
Convert_Planar420_to_Planar422_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[r10+rax]
	movdqa xmm1,XMMWORD ptr[rdx+rax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2
	
	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,r11
	loop Convert_Planar420_to_Planar422_8_SSE2_1
	
	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_SSE2 endp


;JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX proc public frame

	.endprolog
		
	vpcmpeqb xmm3,xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax	
	mov ecx,r9d	
	mov r11,16
	
Convert_Planar420_to_Planar422_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[r10+rax]
	vmovdqa xmm1,XMMWORD ptr[rdx+rax]
	vpxor xmm2,xmm0,xmm3
	vpxor xmm1,xmm1,xmm3
	vpavgb xmm2,xmm2,xmm1
	vpxor xmm2,xmm2,xmm3
	vpavgb xmm2,xmm2,xmm0
	
	vmovdqa XMMWORD ptr[r8+rax],xmm2
	add rax,r11
	loop Convert_Planar420_to_Planar422_8_AVX_1
	
	ret

JPSDR_RGBConvert_Convert_Planar420_to_Planar422_8_AVX endp


;JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2 proc src:dword,dst:dword,w:dword
; src = rcx
; dst = rdx
; w = r8d

JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2 proc public frame

	.endprolog
		
	mov r9,rcx				; r9=src
	xor rcx,rcx
	xor rax,rax	
	mov ecx,r8d	
	mov r10,16
	
Convert_Planar422_to_Planar444_8_SSE2_1:
	movdqa xmm0,XMMWORD ptr[r9+rax]
	movdqu xmm1,XMMWORD ptr[r9+rax+1]
	movdqa xmm2,xmm0
	pavgb xmm1,xmm0
	movdqa xmm3,xmm0
	punpcklbw xmm2,xmm1
	punpckhbw xmm3,xmm1	
	
	movdqa XMMWORD ptr[rdx+2*rax],xmm2
	movdqa XMMWORD ptr[rdx+2*rax+16],xmm3
	add rax,r10
	loop Convert_Planar422_to_Planar444_8_SSE2_1
	
	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_SSE2 endp


;JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX proc src:dword,dst:dword,w:dword
; src = rcx
; dst = rdx
; w = r8d

JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX proc public frame

	.endprolog
		
	mov r9,rcx				; r9=src
	xor rcx,rcx
	xor rax,rax	
	mov ecx,r8d	
	mov r10,16
	
Convert_Planar422_to_Planar444_8_AVX_1:
	vmovdqa xmm0,XMMWORD ptr[r9+rax]
	vmovdqu xmm1,XMMWORD ptr[r9+rax+1]
	vpavgb xmm1,xmm1,xmm0
	vpunpcklbw xmm2,xmm0,xmm1
	vpunpckhbw xmm3,xmm0,xmm1
	
	vmovdqa XMMWORD ptr[rdx+2*rax],xmm2
	vmovdqa XMMWORD ptr[rdx+2*rax+16],xmm3
	add rax,r10
	loop Convert_Planar422_to_Planar444_8_AVX_1
	
	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar444_8_AVX endp


;JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8 proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8 proc public frame

src_modulo equ qword ptr[rbp+48]
dst_modulo equ qword ptr[rbp+56]


	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	.endprolog
	
	mov rsi,rcx
	xor rax,rax	
	xor rcx,rcx
	mov rdi,rdx
	mov rdx,src_modulo
	mov r10,dst_modulo
	
Convert_Planar444_to_Planar422_8_1:
	mov ecx,r8d
	
Convert_Planar444_to_Planar422_8_2:
	lodsw
	stosb
	loop Convert_Planar444_to_Planar422_8_2
	
	add rsi,rdx
	add rdi,r10
	dec r9d
	jnz short Convert_Planar444_to_Planar422_8_1

	pop rdi
	pop rsi
	pop rbp

	ret
	
JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8 endp


;JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2 proc src:dword,dst:dword,w16:dword,h:dword,src_pitch:dword,dst_pitch:dword
; src = rcx
; dst = rdx
; w16 = r8d
; h = r9d

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2 proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog		
	
	mov rsi,rcx
	mov rdi,rdx
	mov rdx,rsi
	mov r10d,r8d
	shr r10d,1
	mov r11,src_pitch
	mov r12,dst_pitch
	mov rbx,16
	xor rcx,rcx
	add rdx,rbx

Convert_Planar444_to_Planar422_8_SSE2_1:
	xor rax,rax
	or r10d,r10d
	jz short Convert_Planar444_to_Planar422_8_SSE2_3
	
	mov ecx,r10d
Convert_Planar444_to_Planar422_8_SSE2_2:
	movdqa xmm0,XMMWORD ptr[rsi+2*rax]
	movdqa xmm1,XMMWORD ptr[rdx+2*rax]
	psrlw xmm0,8
	psrlw xmm1,8
	packuswb xmm0,xmm1
	
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop Convert_Planar444_to_Planar422_8_SSE2_2
	
Convert_Planar444_to_Planar422_8_SSE2_3:	
	test r8d,1
	jz short Convert_Planar444_to_Planar422_8_SSE2_4
	
	movdqa xmm0,XMMWORD ptr[rsi+2*rax]
	psrlw xmm0,8
	packuswb xmm0,xmm0
	
	movq qword ptr[rdi+rax],xmm0

Convert_Planar444_to_Planar422_8_SSE2_4:	
	add rsi,r11
	add rdx,r11
	add rdi,r12
	dec r9d
	jnz short Convert_Planar444_to_Planar422_8_SSE2_1

	pop r12
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_SSE2 endp


;JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX proc src:dword,dst:dword,w16:dword,h:dword,src_pitch:dword,dst_pitch:dword
; src = rcx
; dst = rdx
; w16 = r8d
; h = r9d

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog		
	
	mov rsi,rcx
	mov rdi,rdx
	mov rdx,rsi
	mov r10d,r8d
	shr r10d,1
	mov r11,src_pitch
	mov r12,dst_pitch
	mov rbx,16
	xor rcx,rcx
	add rdx,rbx

Convert_Planar444_to_Planar422_8_AVX_1:
	xor rax,rax
	or r10d,r10d
	jz short Convert_Planar444_to_Planar422_8_AVX_3
	
	mov ecx,r10d
Convert_Planar444_to_Planar422_8_AVX_2:
	vmovdqa xmm0,XMMWORD ptr[rsi+2*rax]
	vmovdqa xmm1,XMMWORD ptr[rdx+2*rax]
	vpsrlw xmm0,xmm0,8
	vpsrlw xmm1,xmm1,8
	vpackuswb xmm0,xmm0,xmm1
	
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,rbx
	loop Convert_Planar444_to_Planar422_8_AVX_2
	
Convert_Planar444_to_Planar422_8_AVX_3:	
	test r8d,1
	jz short Convert_Planar444_to_Planar422_8_AVX_4
	
	vmovdqa xmm0,XMMWORD ptr[rsi+2*rax]
	vpsrlw xmm0,xmm0,8
	vpackuswb xmm0,xmm0,xmm0
	
	vmovq qword ptr[rdi+rax],xmm0

Convert_Planar444_to_Planar422_8_AVX_4:	
	add rsi,r11
	add rdx,r11
	add rdi,r12
	dec r9d
	jnz short Convert_Planar444_to_Planar422_8_AVX_1

	pop r12
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_Planar444_to_Planar422_8_AVX endp


;JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2 proc src1:dword,src2:dword,dst:dword,w16:dword,h:dword,src_pitch2:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w16 = r9d

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2 proc public frame

h equ dword ptr[rbp+48]
src_pitch2 equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog		
	
	mov rsi,rcx
	mov r10d,h
	mov rbx,16
	mov r11,src_pitch2
	mov r12,dst_pitch
	xor rcx,rcx

Convert_Planar422_to_Planar420_8_SSE2_1:
	xor rax,rax
	mov ecx,r9d

Convert_Planar422_to_Planar420_8_SSE2_2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	
	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop Convert_Planar422_to_Planar420_8_SSE2_2
	
	add rsi,r11
	add rdx,r11
	add r8,r12
	dec r10d
	jnz short Convert_Planar422_to_Planar420_8_SSE2_1

	pop r12
	pop rbx
	pop rsi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_SSE2 endp


;JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX proc src1:dword,src2:dword,dst:dword,w16:dword,h:dword,src_pitch2:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w16 = r9d

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX proc public frame

h equ dword ptr[rbp+48]
src_pitch2 equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog		
	
	mov rsi,rcx
	mov r10d,h
	mov rbx,16
	mov r11,src_pitch2
	mov r12,dst_pitch
	xor rcx,rcx

Convert_Planar422_to_Planar420_8_AVX_1:
	xor rax,rax
	mov ecx,r9d

Convert_Planar422_to_Planar420_8_AVX_2:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	
	vmovdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop Convert_Planar422_to_Planar420_8_AVX_2
	
	add rsi,r11
	add rdx,r11
	add r8,r12
	dec r10d
	jnz short Convert_Planar422_to_Planar420_8_AVX_1

	pop r12
	pop rbx
	pop rsi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_Planar422_to_Planar420_8_AVX endp


;JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9

JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_modulo equ qword ptr[rbp+96]
dst_modulo_y equ qword ptr[rbp+104]
dst_modulo_u equ qword ptr[rbp+112]
dst_modulo_v equ qword ptr[rbp+120]
Min_Y equ word ptr[rbp+128]
Max_Y equ word ptr[rbp+136]
Min_U equ word ptr[rbp+144]
Max_U equ word ptr[rbp+152]
Min_V equ word ptr[rbp+160]
Max_V equ word ptr[rbp+168]

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

	xor rax,rax
	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2		
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	movzx eax,offset_U
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_V
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	movzx eax,Min_U
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_U
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_V
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	movzx eax,Max_V
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5

	mov rsi,rcx
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13,2
	mov r14,8
	mov r9d,w
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r9d
	shr r8d,1				;r8d=w0
	
Convert_RGB32toYV24_SSE2_1:
	or r8d,r8d
	jz Convert_RGB32toYV24_SSE2_3
	
	mov ecx,r8d
Convert_RGB32toYV24_SSE2_2:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,4
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,5

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	add rsi,r14
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rdi,r13
	mov word ptr[r11],ax
	pextrw eax,xmm0,2
	add r11,r13
	mov word ptr[r12],ax	
	add r12,r13
	
	dec ecx
	jnz Convert_RGB32toYV24_SSE2_2
	
Convert_RGB32toYV24_SSE2_3:
	test r9d,1
	jz Convert_RGB32toYV24_SSE2_4
	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,4
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm0
	
	pextrw eax,xmm0,0
	add rsi,4
	mov byte ptr[rdi],al
	pextrw eax,xmm0,2
	inc rdi
	mov byte ptr[r11],al
	pextrw eax,xmm0,4
	inc r11
	mov byte ptr[r12],al
	inc r12
		
Convert_RGB32toYV24_SSE2_4:	
	add rsi,src_modulo
	add rdi,dst_modulo_y
	add r11,dst_modulo_u
	add r12,dst_modulo_v
	dec h
	jnz Convert_RGB32toYV24_SSE2_1

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_RGB32toYV24_SSE2 endp


;JPSDR_RGBConvert_Convert_RGB32toYV24_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9

JPSDR_RGBConvert_Convert_RGB32toYV24_AVX proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_modulo equ qword ptr[rbp+96]
dst_modulo_y equ qword ptr[rbp+104]
dst_modulo_u equ qword ptr[rbp+112]
dst_modulo_v equ qword ptr[rbp+120]
Min_Y equ word ptr[rbp+128]
Max_Y equ word ptr[rbp+136]
Min_U equ word ptr[rbp+144]
Max_U equ word ptr[rbp+152]
Min_V equ word ptr[rbp+160]
Max_V equ word ptr[rbp+168]

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

	xor rax,rax
	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1	
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5	

	mov rsi,rcx
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13,2
	mov r14,8
	mov r9d,w
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r9d
	shr r8d,1				;r8d=w0
	
Convert_RGB32toYV24_AVX_1:
	or r8d,r8d
	jz Convert_RGB32toYV24_AVX_3
	
	mov ecx,r8d
Convert_RGB32toYV24_AVX_2:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,4
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,5

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	add rsi,r14
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rdi,r13
	mov word ptr[r11],ax
	vpextrw eax,xmm0,2
	add r11,r13
	mov word ptr[r12],ax	
	add r12,r13
	
	dec ecx
	jnz Convert_RGB32toYV24_AVX_2
	
Convert_RGB32toYV24_AVX_3:
	test r9d,1
	jz Convert_RGB32toYV24_AVX_4
	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,4
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm0
	
	vpextrw eax,xmm0,0
	add rsi,4
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,2
	inc rdi
	mov byte ptr[r11],al
	vpextrw eax,xmm0,4
	inc r11
	mov byte ptr[r12],al
	inc r12
		
Convert_RGB32toYV24_AVX_4:	
	add rsi,src_modulo
	add rdi,dst_modulo_y
	add r11,dst_modulo_u
	add r12,dst_modulo_v
	dec h
	jnz Convert_RGB32toYV24_AVX_1

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_Convert_RGB32toYV24_AVX endp


;JPSDR_RGBConvert_RGB32toYUYV_SSE2 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo:dword,
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; w = r8d
; h = r9d

JPSDR_RGBConvert_RGB32toYUYV_SSE2 proc public frame

offset_Y equ word ptr[rbp+48]
offset_U equ word ptr[rbp+56]
offset_V equ word ptr[rbp+64]
lookup equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_modulo equ qword ptr[rbp+88]
Min_Y equ word ptr[rbp+96]
Max_Y equ word ptr[rbp+104]
Min_U equ word ptr[rbp+112]
Max_U equ word ptr[rbp+120]
Min_V equ word ptr[rbp+128]
Max_V equ word ptr[rbp+136]

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

	xor rax,rax
	pxor xmm4,xmm4
	pxor xmm3,xmm3
	pxor xmm2,xmm2	
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,6
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,6
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,6
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,7
	movzx eax,Max_V
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,7
	
	mov rsi,rcx
	mov r10,lookup
	mov rdi,rdx				;rdi=dst
	mov r13d,r8d			;r13d=w
	mov r11d,r9d			;r11d=h
	
	shr r8d,2				;r8d=w0

	mov r12,8
	mov r14,16	
	mov r9,4
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15

Boucle0_8:
	or r8d,r8d
	jz Suite0_8
	mov ecx,r8d
Boucle1_8:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,3

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2

	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,7

	movzx edx,byte ptr[rsi+12]
	movzx r15d,byte ptr[rsi+13]
	movzx ebx,byte ptr[rsi+14] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,6

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3		
	packuswb xmm0,xmm4
	
	add rsi,r14
	
	movq qword ptr[rdi],xmm0
	
	add rdi,r12
	
	dec ecx
	jnz Boucle1_8
	
Suite0_8:	
	mov eax,r13d
	and eax,3
	jz Suite2_8
	and eax,2
	jz Suite1_8
	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,3

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2

	mov eax,r13d
	and eax,1
	jz Suite0_8b
	
	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,7

	xor eax,eax
	pinsrw xmm0,eax,6

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	add rsi,12
	
	movq qword ptr[rdi],xmm0
	
	add rdi,r12
	
	jmp Suite2_8
	
Suite0_8b:		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	add rsi,r12
	
	movd dword ptr[rdi],xmm0
	
	add rdi,r9
	
	jmp Suite2_8

Suite1_8:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,3
	
	xor eax,eax
	pinsrw xmm0,eax,2
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	add rsi,r9
	
	movd dword ptr[rdi],xmm0
	
	add rdi,r9
	
Suite2_8:	
	add rsi,src_modulo
	add rdi,dst_modulo
	dec r11d
	jnz Boucle0_8

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYUYV_SSE2 endp


;JPSDR_RGBConvert_RGB32toYUYV_AVX proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo:dword,
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; w = r8d
; h = r9d

JPSDR_RGBConvert_RGB32toYUYV_AVX proc public frame

offset_Y equ word ptr[rbp+48]
offset_U equ word ptr[rbp+56]
offset_V equ word ptr[rbp+64]
lookup equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_modulo equ qword ptr[rbp+88]
Min_Y equ word ptr[rbp+96]
Max_Y equ word ptr[rbp+104]
Min_U equ word ptr[rbp+112]
Max_U equ word ptr[rbp+120]
Min_V equ word ptr[rbp+128]
Max_V equ word ptr[rbp+136]

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

	xor rax,rax
	vpxor xmm4,xmm4,xmm4
	vpxor xmm3,xmm3,xmm3
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,5	
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,3
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,6
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,5
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,3
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,3
	vpinsrw xmm3,xmm3,eax,7	
	
	mov rsi,rcx
	mov r10,lookup
	mov rdi,rdx				;rdi=dst
	mov r13d,r8d			;r13d=w
	mov r11d,r9d			;r11d=h
	
	shr r8d,2				;r8d=w0

	mov r12,8
	mov r14,16	
	mov r9,4
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15

Boucle0_8_AVX:
	or r8d,r8d
	jz Suite0_8_AVX
	
	mov ecx,r8d
Boucle1_8_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,3

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2

	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,7

	movzx edx,byte ptr[rsi+12]
	movzx r15d,byte ptr[rsi+13]
	movzx ebx,byte ptr[rsi+14] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,6

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3		
	vpackuswb xmm0,xmm0,xmm4
	
	add rsi,r14
	
	vmovq qword ptr[rdi],xmm0
	
	add rdi,r12
	
	dec ecx
	jnz Boucle1_8_AVX
	
Suite0_8_AVX:	
	mov eax,r13d
	and eax,3
	jz Suite2_8_AVX
	and eax,2
	jz Suite1_8_AVX
	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,3

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2

	mov eax,r13d
	and eax,1
	jz Suite0_8b_AVX
	
	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,7

	xor eax,eax
	vpinsrw xmm0,xmm0,eax,6

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	add rsi,12
	
	vmovq qword ptr[rdi],xmm0
	
	add rdi,r12
	
	jmp Suite2_8_AVX
	
Suite0_8b_AVX:		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	add rsi,r12
	
	vmovd dword ptr[rdi],xmm0
	
	add rdi,r9
	
	jmp Suite2_8_AVX

Suite1_8_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,3
	
	xor eax,eax
	vpinsrw xmm0,xmm0,eax,2
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	add rsi,r9
	
	vmovd dword ptr[rdi],xmm0
	
	add rdi,r9
	
Suite2_8_AVX:	
	add rsi,src_modulo
	add rdi,dst_modulo
	dec r11d
	jnz Boucle0_8_AVX

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYUYV_AVX endp


;JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
; offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_R equ word ptr[rbp+64]
offset_G equ word ptr[rbp+72]
offset_B equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_modulo_y equ qword ptr[rbp+96]
src_modulo_u equ qword ptr[rbp+104]
src_modulo_v equ qword ptr[rbp+112]
dst_modulo equ qword ptr[rbp+120]

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

	xor rax,rax
	pxor xmm1,xmm1
	movzx eax,offset_B
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4
	movzx eax,offset_G
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_R
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	mov rsi,rcx					;rsi=src_y
	mov r11,rdx					;r11=src_u
	mov r12,r8					;r12=src_v
	mov rdi,r9
	mov r9d,w
	mov r10,lookup
	mov r13,4
	mov r14,16
	
	mov r8d,r9d
	shr r8d,2					;r8d=w0
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15

Convert_YV24toRGB32_SSE2_1:
	or r8d,r8d
	jz Convert_YV24toRGB32_SSE2_3
	
	mov ecx,r8d
Convert_YV24toRGB32_SSE2_2:
	pxor xmm2,xmm2
	pxor xmm0,xmm0

	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	pinsrw xmm0,eax,0
	
	movzx ebx,byte ptr[rsi+1]
	movzx r15d,byte ptr[r11+1]
	movzx edx,byte ptr[r12+1] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	pinsrw xmm0,eax,4

	movzx ebx,byte ptr[rsi+2]
	movzx r15d,byte ptr[r11+2]
	movzx edx,byte ptr[r12+2] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	pinsrw xmm2,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	pinsrw xmm2,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	pinsrw xmm2,eax,0
	
	movzx ebx,byte ptr[rsi+3]
	movzx r15d,byte ptr[r11+3]
	movzx edx,byte ptr[r12+3] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	add rsi,r13
	pinsrw xmm2,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	add r11,r13
	pinsrw xmm2,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	add r12,r13
	pinsrw xmm2,eax,4
	
	paddsw xmm0,xmm1
	paddsw xmm2,xmm1
	psraw xmm0,5
	psraw xmm2,5
	packuswb xmm0,xmm2
	
	movdqa XMMWORD ptr[rdi],xmm0
	
	add rdi,r14
	
	dec ecx
	jnz Convert_YV24toRGB32_SSE2_2
	
Convert_YV24toRGB32_SSE2_3:	
	test r9d,3
	jz Convert_YV24toRGB32_SSE2_5
	
	pxor xmm0,xmm0
	
	test r9d,2
	jnz short Convert_YV24toRGB32_SSE2_4
	
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	inc rsi
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	inc r11
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	inc r12
	pinsrw xmm0,eax,0
	
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm0
	
	movd dword ptr[rdi],xmm0
	
	add rdi,r13
	
	jmp Convert_YV24toRGB32_SSE2_5
	
Convert_YV24toRGB32_SSE2_4:
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	pinsrw xmm0,eax,0
	
	movzx ebx,byte ptr[rsi+1]
	movzx r15d,byte ptr[r11+1]
	movzx edx,byte ptr[r12+1] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	add rsi,2
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	add r11,2
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	add r12,2
	pinsrw xmm0,eax,4
	
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm0
	
	movq qword ptr[rdi],xmm0
	
	add rdi,8
	
	test r9d,1
	jz short Convert_YV24toRGB32_SSE2_5
	
	pxor xmm0,xmm0
	
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	inc rsi
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	inc r11
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	inc r12
	pinsrw xmm0,eax,0
	
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm0
	
	movd dword ptr[rdi],xmm0
	
	add rdi,r13
	
Convert_YV24toRGB32_SSE2_5:	
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo
	dec h
	jnz Convert_YV24toRGB32_SSE2_1

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_RGBConvert_Convert_YV24toRGB32_SSE2 endp


;JPSDR_RGBConvert_Convert_YV24toRGB32_AVX proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
; offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_RGBConvert_Convert_YV24toRGB32_AVX proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_R equ word ptr[rbp+64]
offset_G equ word ptr[rbp+72]
offset_B equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_modulo_y equ qword ptr[rbp+96]
src_modulo_u equ qword ptr[rbp+104]
src_modulo_v equ qword ptr[rbp+112]
dst_modulo equ qword ptr[rbp+120]

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

	xor rax,rax
	vpxor xmm2,xmm2,xmm2
	vpxor xmm1,xmm1,xmm1
	vpxor xmm0,xmm0,xmm0
	movzx eax,offset_B
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,4	
	movzx eax,offset_G
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,5
	movzx eax,offset_R
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,6
	mov rsi,rcx					;rsi=src_y
	mov r11,rdx					;r11=src_u
	mov r12,r8					;r12=src_v
	mov rdi,r9
	mov r9d,w
	mov r10,lookup
	mov r13,4
	mov r14,16
	
	mov r8d,r9d
	shr r8d,2					;r8d=w0
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15

Convert_YV24toRGB32_AVX_1:
	or r8d,r8d
	jz Convert_YV24toRGB32_AVX_3
	
	mov ecx,r8d
Convert_YV24toRGB32_AVX_2:
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx ebx,byte ptr[rsi+1]
	movzx r15d,byte ptr[r11+1]
	movzx edx,byte ptr[r12+1] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	vpinsrw xmm0,xmm0,eax,4

	movzx ebx,byte ptr[rsi+2]
	movzx r15d,byte ptr[r11+2]
	movzx edx,byte ptr[r12+2] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	vpinsrw xmm2,xmm2,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	vpinsrw xmm2,xmm2,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	vpinsrw xmm2,xmm2,eax,0
	
	movzx ebx,byte ptr[rsi+3]
	movzx r15d,byte ptr[r11+3]
	movzx edx,byte ptr[r12+3] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	add rsi,r13
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	add r11,r13
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	add r12,r13
	vpinsrw xmm2,xmm2,eax,4
	
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm2,xmm2,xmm1
	vpsraw xmm0,xmm0,5
	vpsraw xmm2,xmm2,5
	vpackuswb xmm3,xmm0,xmm2
	
	vmovdqa XMMWORD ptr[rdi],xmm3
	
	add rdi,r14
	
	dec ecx
	jnz Convert_YV24toRGB32_AVX_2
	
Convert_YV24toRGB32_AVX_3:	
	test r9d,3
	jz Convert_YV24toRGB32_AVX_5
	
	test r9d,2
	jnz short Convert_YV24toRGB32_AVX_4

	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	inc rsi
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	inc r11
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	inc r12
	vpinsrw xmm0,xmm0,eax,0
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,5
	vpackuswb xmm3,xmm0,xmm0
	
	vmovd dword ptr[rdi],xmm3
	
	add rdi,r13
	
	jmp Convert_YV24toRGB32_AVX_5
	
Convert_YV24toRGB32_AVX_4:	
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx ebx,byte ptr[rsi+1]
	movzx r15d,byte ptr[r11+1]
	movzx edx,byte ptr[r12+1] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	add rsi,2
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	add r11,2
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	add r12,2
	vpinsrw xmm0,xmm0,eax,4
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,5
	vpackuswb xmm3,xmm0,xmm0
	
	vmovq qword ptr[rdi],xmm3
	
	add rdi,8
	
	test r9d,1
	jz short Convert_YV24toRGB32_AVX_5
	
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11]
	movzx edx,byte ptr[r12] ; rbx=Y r15=U rdx=V
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rdx+512]
	inc rsi
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	inc r11
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	inc r12
	vpinsrw xmm0,xmm0,eax,0
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,5
	vpackuswb xmm3,xmm0,xmm0
	
	vmovd dword ptr[rdi],xmm3
	
	add rdi,r13
	
Convert_YV24toRGB32_AVX_5:	
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo
	dec h
	jnz Convert_YV24toRGB32_AVX_1

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_RGBConvert_Convert_YV24toRGB32_AVX endp


end
