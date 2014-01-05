.code


;JPSDR_AutoYUY2_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
;public JPSDR_AutoYUY2_Move32_Full
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_AutoYUY2_Move32_Full proc public frame

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
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rax,src_modulo
	mov r10,dst_modulo
loop_F:
	mov ecx,r8d
	shr ecx,1
	jnc short loop_Fa
	movsd
loop_Fa:	
	rep movsq
	add rsi,rax
	add rdi,r10
	dec r9d
	jnz short loop_F
	
fin_F:
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Move32_Full endp


;JPSDR_AutoYUY2_Move8_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword
;public JPSDR_AutoYUY2_Move8_Full
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_AutoYUY2_Move8_Full proc public frame

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
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rax,src_modulo
	mov r10,dst_modulo
	mov r11d,7
loop_F8:
	mov ecx,r8d
	and ecx,r11d
	jz short loop_F8_suite
	rep movsb
loop_F8_suite:	
	mov ecx,r8d
	shr ecx,3
	rep movsq
	add rsi,rax
	add rdi,r10
	dec r9d
	jnz short loop_F8
	
fin_F8:
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Move8_Full endp



;JPSDR_AutoYUY2_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_YUY2_1
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_1 proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx				; rsi=src_y
	xor rcx,rcx
	mov rdi,r9				; rdi=dst
	mov rbx,r8
	mov ecx,w
	cld

SSE2_1_a:
	mov al,byte ptr[rsi+1]		;al=y2
	mov ah,byte ptr[rbx]		;ah=v
	inc rbx
	shl eax,16
	lodsw				;al=y1 ah=y2
	mov ah,byte ptr[rdx]		;ah=u
	inc rdx
	stosd
	loop SSE2_1_a
	
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_1 endp



;JPSDR_AutoYUY2_Convert420_to_UYVY_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_UYVY_1
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_AutoYUY2_Convert420_to_UYVY_1 proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	.endprolog

	mov rsi,rcx				; rsi=src_y
	xor rcx,rcx
	mov rdi,r9				; rdi=dst
	mov ecx,w
	cld
		
SSE2_1_b:
	lodsw						;al=y1 ah=y2
	mov al,byte ptr[r8]		;al=v
	inc r8
	shl eax,16
	mov ah,byte ptr[rsi-2]		;ah=y1
	mov al,byte ptr[rdx]		;al=u
	inc rdx
	stosd
	loop SSE2_1_b
	
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_1 endp



;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2 proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	sub rsp,32
	.allocstack 32
	movdqu [rsp],xmm6
	.savexmm128 xmm6,0
	movdqu [rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	pxor xmm6,xmm6
	pxor xmm5,xmm5
	pxor xmm4,xmm4
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	xor rax,rax
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,2
	mov rbx,8
	mov ecx,w
	cld
	
	mov eax,4
	pinsrw xmm6,eax,1
	pinsrw xmm6,eax,3
	pinsrw xmm6,eax,5
	pinsrw xmm6,eax,7
	mov eax,3
	pinsrw xmm5,eax,1
	pinsrw xmm5,eax,3
	pinsrw xmm5,eax,5
	pinsrw xmm5,eax,7
	mov eax,5
	pinsrw xmm4,eax,1
	pinsrw xmm4,eax,3
	pinsrw xmm4,eax,5
	pinsrw xmm4,eax,7

	xor eax,eax

SSE2_2_a:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,0
	lodsb
	pinsrw xmm0,eax,2
	lodsb
	pinsrw xmm0,eax,4
	lodsb
	pinsrw xmm0,eax,6
	movzx eax,byte ptr[r10]					;al=u1 (1)
	pinsrw xmm1,eax,1
	movzx eax,byte ptr[r10+1]					;al=u1 (2)
	add r10,rdx
	pinsrw xmm1,eax,5
	movzx eax,byte ptr[r8]					;al=u2 (1)
	pinsrw xmm2,eax,1
	movzx eax,byte ptr[r8+1]					;al=u2 (2)
	add r8,rdx
	pinsrw xmm2,eax,5
	movzx eax,byte ptr[r9]					;al=v1 (1)
	pinsrw xmm1,eax,3
	movzx eax,byte ptr[r9+1]					;al=v1 (2)
	add r9,rdx
	pinsrw xmm1,eax,7
	movzx eax,byte ptr[r11]					;al=v2 (1)
	pinsrw xmm2,eax,3
	movzx eax,byte ptr[r11+1]					;al=v2 (2)
	add r11,rdx
	pinsrw xmm2,eax,7
	
	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[rdi],xmm0
	add rdi,rbx
	dec ecx
	jnz SSE2_2_a
	
	movdqu xmm7,[rsp+16]
	movdqu xmm6,[rsp]
	add rsp,32
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret	

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2 endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2 proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	sub rsp,32
	.allocstack 32
	movdqu [rsp],xmm6
	.savexmm128 xmm6,0
	movdqu [rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	pxor xmm6,xmm6
	pxor xmm5,xmm5
	pxor xmm4,xmm4
	pxor xmm2,xmm2
	pxor xmm1,xmm1
			
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov r11,src2_v
	mov rdi,dst
	xor rcx,rcx
	mov rdx,2
	mov rbx,8
	mov ecx,w
	cld
		
	mov eax,4
	pinsrw xmm6,eax,0
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,4
	pinsrw xmm6,eax,6
	mov eax,3
	pinsrw xmm5,eax,0
	pinsrw xmm5,eax,2
	pinsrw xmm5,eax,4
	pinsrw xmm5,eax,6
	mov eax,5
	pinsrw xmm4,eax,0
	pinsrw xmm4,eax,2
	pinsrw xmm4,eax,4
	pinsrw xmm4,eax,6

	xor eax,eax

SSE2_2_b:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,1
	lodsb
	pinsrw xmm0,eax,3
	lodsb
	pinsrw xmm0,eax,5
	lodsb
	pinsrw xmm0,eax,7
	movzx eax,byte ptr[r10]					;al=u1 (1)
	pinsrw xmm1,eax,0
	movzx eax,byte ptr[r10+1]					;al=u1 (2)
	add r10,rdx
	pinsrw xmm1,eax,4
	movzx eax,byte ptr[r8]					;al=u2 (1)
	pinsrw xmm2,eax,0
	movzx eax,byte ptr[r8+1]					;al=u2 (2)
	add r8,rdx
	pinsrw xmm2,eax,4
	movzx eax,byte ptr[r9]					;al=v1 (1)
	pinsrw xmm1,eax,2
	movzx eax,byte ptr[r9+1]					;al=v1 (2)
	add r9,rdx
	pinsrw xmm1,eax,6
	movzx eax,byte ptr[r11]					;al=v2 (1)
	pinsrw xmm2,eax,2
	movzx eax,byte ptr[r11+1]					;al=v2 (2)
	add r11,rdx
	pinsrw xmm2,eax,6
	
	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[rdi],xmm0
	add rdi,rbx
	dec ecx
	jnz SSE2_2_b
	
	movdqu xmm7,[rsp+16]
	movdqu xmm6,[rsp]
	add rsp,32
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret	

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2 endp



;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3 proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	sub rsp,32
	.allocstack 32
	movdqu [rsp],xmm6
	.savexmm128 xmm6,0
	movdqu [rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog

	pxor xmm7,xmm7
	pxor xmm6,xmm6
	pxor xmm5,xmm5
	pxor xmm1,xmm1
	pxor xmm2,xmm2
	
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov r11,src2_v
	mov rdi,dst
	xor rcx,rcx
	mov rdx,2
	mov rbx,8
	mov ecx,w
	cld
	
	mov eax,4
	pinsrw xmm6,eax,1
	pinsrw xmm6,eax,3
	pinsrw xmm6,eax,5
	pinsrw xmm6,eax,7
	
	mov eax,7
	pinsrw xmm5,eax,1
	pinsrw xmm5,eax,3
	pinsrw xmm5,eax,5
	pinsrw xmm5,eax,7

	xor eax,eax	

SSE2_3_a:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,0
	lodsb
	pinsrw xmm0,eax,2
	lodsb
	pinsrw xmm0,eax,4
	lodsb
	pinsrw xmm0,eax,6
	movzx eax,byte ptr[r10]					;al=u1 (1)
	pinsrw xmm1,eax,1
	movzx eax,byte ptr[r10+1]					;al=u1 (2)
	add r10,rdx
	pinsrw xmm1,eax,5
	movzx eax,byte ptr[r8]					;al=u2 (1)
	pinsrw xmm2,eax,1
	movzx eax,byte ptr[r8+1]					;al=u2 (2)
	add r8,rdx
	pinsrw xmm2,eax,5
	movzx eax,byte ptr[r9]					;al=v1 (1)
	pinsrw xmm1,eax,3
	movzx eax,byte ptr[r9+1]					;al=v1 (2)
	add r9,rdx
	pinsrw xmm1,eax,7
	movzx eax,byte ptr[r11]					;al=v2 (1)
	pinsrw xmm2,eax,3
	movzx eax,byte ptr[r11+1]					;al=v2 (2)
	add r11,rdx
	pinsrw xmm2,eax,7
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[rdi],xmm0
	add rdi,rbx
	dec ecx
	jnz SSE2_3_a
	
	movdqu xmm7,[rsp+16]
	movdqu xmm6,[rsp]
	add rsp,32
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3 endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9
	
JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3 proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	sub rsp,32
	.allocstack 32
	movdqu [rsp],xmm6
	.savexmm128 xmm6,0
	movdqu [rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog

	pxor xmm7,xmm7
	pxor xmm6,xmm6
	pxor xmm5,xmm5
	pxor xmm1,xmm1
	pxor xmm2,xmm2
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov r11,src2_v
	mov rdi,dst
	xor rcx,rcx
	mov rdx,2
	mov rbx,8
	mov ecx,w
	cld
	
	mov eax,4
	pinsrw xmm6,eax,0
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,4
	pinsrw xmm6,eax,6
	
	mov eax,7
	pinsrw xmm5,eax,0
	pinsrw xmm5,eax,2
	pinsrw xmm5,eax,4
	pinsrw xmm5,eax,6
	
	xor eax,eax		

SSE2_3_b:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,1
	lodsb
	pinsrw xmm0,eax,3
	lodsb
	pinsrw xmm0,eax,5
	lodsb
	pinsrw xmm0,eax,7
	movzx eax,byte ptr[r10]					;al=u1 (1)
	pinsrw xmm1,eax,0
	movzx eax,byte ptr[r10+1]					;al=u1 (2)
	add r10,rdx
	pinsrw xmm1,eax,4
	movzx eax,byte ptr[r8]					;al=u2 (1)
	pinsrw xmm2,eax,0
	movzx eax,byte ptr[r8+1]					;al=u2 (2)
	add r8,rdx
	pinsrw xmm2,eax,4
	movzx eax,byte ptr[r9]					;al=v1 (1)
	pinsrw xmm1,eax,2
	movzx eax,byte ptr[r9+1]					;al=v1 (2)
	add r9,rdx
	pinsrw xmm1,eax,6
	movzx eax,byte ptr[r11]					;al=v2 (1)
	pinsrw xmm2,eax,2
	movzx eax,byte ptr[r11+1]					;al=v2 (2)
	add r11,rdx
	pinsrw xmm2,eax,6
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,3
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[rdi],xmm0
	add rdi,rbx
	dec ecx
	jnz SSE2_3_b
	
	movdqu xmm7,[rsp+16]
	movdqu xmm6,[rsp]
	add rsp,32
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3 endp


;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9
	
JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4 proc public frame	

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	sub rsp,32
	.allocstack 32
	movdqu [rsp],xmm6
	.savexmm128 xmm6,0
	movdqu [rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	pxor xmm6,xmm6
	pxor xmm5,xmm5
	pxor xmm2,xmm2
	pxor xmm1,xmm1

	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov r11,src2_v
	mov rdi,dst
	xor rcx,rcx
	mov rdx,2
	mov rbx,8
	mov ecx,w
	cld
	
	mov eax,2
	pinsrw xmm6,eax,1
	pinsrw xmm6,eax,3
	pinsrw xmm6,eax,5
	pinsrw xmm6,eax,7
	
	mov eax,3
	pinsrw xmm5,eax,1
	pinsrw xmm5,eax,3
	pinsrw xmm5,eax,5
	pinsrw xmm5,eax,7

	xor eax,eax	

SSE2_4_a:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,0
	lodsb
	pinsrw xmm0,eax,2
	lodsb
	pinsrw xmm0,eax,4
	lodsb
	pinsrw xmm0,eax,6
	movzx eax,byte ptr[r10]					;al=u1 (1)
	pinsrw xmm1,eax,1
	movzx eax,byte ptr[r10+1]					;al=u1 (2)
	add r10,rdx
	pinsrw xmm1,eax,5
	movzx eax,byte ptr[r8]					;al=u2 (1)
	pinsrw xmm2,eax,1
	movzx eax,byte ptr[r8+1]					;al=u2 (2)
	add r8,rdx
	pinsrw xmm2,eax,5
	movzx eax,byte ptr[r9]					;al=v1 (1)
	pinsrw xmm1,eax,3
	movzx eax,byte ptr[r9+1]					;al=v1 (2)
	add r9,rdx
	pinsrw xmm1,eax,7
	movzx eax,byte ptr[r11]					;al=v2 (1)
	pinsrw xmm2,eax,3
	movzx eax,byte ptr[r11+1]					;al=v2 (2)
	add r11,rdx
	pinsrw xmm2,eax,7
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,2
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[rdi],xmm0
	add rdi,rbx
	dec ecx
	jnz SSE2_4_a

	movdqu xmm7,[rsp+16]
	movdqu xmm6,[rsp]
	add rsp,32
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4 endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4 proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9
	
JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4 proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	sub rsp,32
	.allocstack 32
	movdqu [rsp],xmm6
	.savexmm128 xmm6,0
	movdqu [rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog

	pxor xmm7,xmm7
	pxor xmm6,xmm6
	pxor xmm5,xmm5
	pxor xmm2,xmm2
	pxor xmm1,xmm1

	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov r11,src2_v
	mov rdi,dst
	xor rcx,rcx
	mov rdx,2
	mov rbx,8
	mov ecx,w
	cld
	
	mov eax,2
	pinsrw xmm6,eax,0
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,4
	pinsrw xmm6,eax,6
	
	mov eax,3
	pinsrw xmm5,eax,0
	pinsrw xmm5,eax,2
	pinsrw xmm5,eax,4
	pinsrw xmm5,eax,6

	xor eax,eax	

SSE2_4_b:
	pxor xmm0,xmm0
	lodsb
	pinsrw xmm0,eax,1
	lodsb
	pinsrw xmm0,eax,3
	lodsb
	pinsrw xmm0,eax,5
	lodsb
	pinsrw xmm0,eax,7
	movzx eax,byte ptr[r10]					;al=u1 (1)
	pinsrw xmm1,eax,0
	movzx eax,byte ptr[r10+1]					;al=u1 (2)
	add r10,rdx
	pinsrw xmm1,eax,4
	movzx eax,byte ptr[r8]					;al=u2 (1)
	pinsrw xmm2,eax,0
	movzx eax,byte ptr[r8+1]					;al=u2 (2)
	add r8,rdx
	pinsrw xmm2,eax,4
	movzx eax,byte ptr[r9]					;al=v1 (1)
	pinsrw xmm1,eax,2
	movzx eax,byte ptr[r9+1]					;al=v1 (2)
	add r9,rdx
	pinsrw xmm1,eax,6
	movzx eax,byte ptr[r11]					;al=v2 (1)
	pinsrw xmm2,eax,2
	movzx eax,byte ptr[r11+1]					;al=v2 (2)
	add r11,rdx
	pinsrw xmm2,eax,6
	
	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	paddsw xmm1,xmm2
	psraw xmm1,2
	paddsw xmm0,xmm1
	packuswb xmm0,xmm7
	movq qword ptr[rdi],xmm0
	add rdi,rbx
	dec ecx
	jnz SSE2_4_b
	
	movdqu xmm7,[rsp+16]
	movdqu xmm6,[rsp]
	add rsp,32
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4 endp



;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2 proc src1:dword,src2:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d
JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2 proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	mov rax,8
	mov ecx,r9d
		
SSE2_2_c:
	movq xmm0,qword ptr[rdx]
	movq xmm1,qword ptr[r10]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add r10,rax
	pavgb xmm0,xmm1
	add rdx,rax
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movq qword ptr[r8],xmm0
	add r8,rax
	loop SSE2_2_c
	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2 endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b proc src1:dword,src2:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d
JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	mov rax,8
	mov ecx,r9d
		
SSE2_2_cb:
	movq xmm0,qword ptr[rdx]
	movq xmm1,qword ptr[r10]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add r10,rax
	pavgb xmm0,xmm1
	add rdx,rax
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movq qword ptr[r8],xmm0
	add r8,rax
	loop SSE2_2_cb

	movd xmm0,dword ptr[rdx]
	movd xmm1,dword ptr[r10]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movd dword ptr[r8],xmm0	
		
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3 proc src1:dword,src2:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d
	
JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3 proc public frame	
	
	.endprolog
	
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	mov rax,8
	mov ecx,r9d
	
SSE2_3_c:
	movq xmm0,qword ptr[r10]
	movq xmm1,qword ptr[rdx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	add r10,rax
	pavgb xmm1,xmm0
	add rdx,rax
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movq qword ptr[r8],xmm1
	add r8,rax
	loop SSE2_3_c
	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3 endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b proc src1:dword,src2:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d
	
JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b proc public frame	
	
	.endprolog
	
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	mov rax,8
	mov ecx,r9d
	
SSE2_3_cb:
	movq xmm0,qword ptr[r10]
	movq xmm1,qword ptr[rdx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	add r10,rax
	pavgb xmm1,xmm0
	add rdx,rax
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movq qword ptr[r8],xmm1
	add r8,rax
	loop SSE2_3_cb
	
	movd xmm0,dword ptr[r10]
	movd xmm1,dword ptr[rdx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm1,xmm0
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movd dword ptr[r8],xmm1	
	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4 proc src1:dword,src2:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4 proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	mov rax,8
	mov ecx,r9d

SSE2_4_c:
	movq xmm0,qword ptr[r10]
	movq xmm1,qword ptr[rdx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add r10,rax
	pxor xmm0,xmm3
	add rdx,rax
	pavgb xmm0,xmm2
	
	movq qword ptr[r8],xmm0
	add r8,rax
	loop SSE2_4_c
	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4 endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b proc src1:dword,src2:dword,dst:dword,w:dword
;public JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	mov rax,8
	mov ecx,r9d

SSE2_4_cb:
	movq xmm0,qword ptr[r10]
	movq xmm1,qword ptr[rdx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add r10,rax
	pxor xmm0,xmm3
	add rdx,rax
	pavgb xmm0,xmm2
	
	movq qword ptr[r8],xmm0
	add r8,rax
	loop SSE2_4_cb
	
	movd xmm0,dword ptr[r10]
	movd xmm1,dword ptr[rdx]
	movq xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	add r10,rax
	pxor xmm0,xmm3
	add rdx,rax
	pavgb xmm0,xmm2
	
	movd dword ptr[r8],xmm0
		
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b endp

end





