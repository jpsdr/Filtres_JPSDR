.code

;JPSDR_Saturation_SSE2_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_R:word,offset_G:word,
;offset_B:word,lookup:dword,src_modulo:dword,dst_modulo:dword
;public JPSDR_Saturation_SSE2_RGB24
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_SSE2_RGB24 proc public frame

offset_R equ word ptr[rbp+48]
offset_G equ word ptr[rbp+56]
offset_B equ word ptr[rbp+64]
lookup equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_modulo equ qword ptr[rbp+88]

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
	mov r11,lookup
	mov r12,8
	mov r15,4
	mov r13,src_modulo
	mov r14,dst_modulo
	xor rax,rax
	xor rdx,rdx
	xor rcx,rcx
	xor rbx,rbx
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_R
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6	
	movzx eax,offset_G
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_B
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4

Boucle0_2:
	mov r10d,r8d
	shr r10d,1
	jz Suite1_2
Boucle1_2:
	movzx edx,byte ptr[rsi]
	movzx ecx,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R ecx=G edx=B
	movzx eax,word ptr[r11+2*rbx]
	add ax,word ptr[r11+2*rcx+512]
	add ax,word ptr[r11+2*rdx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r11+2*rbx+1536]
	add ax,word ptr[r11+2*rcx+2048]
	add ax,word ptr[r11+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r11+2*rbx+3072]
	add ax,word ptr[r11+2*rcx+3584]
	add ax,word ptr[r11+2*rdx+4096]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+4]
	movzx ecx,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R ecx=G edx=B
	movzx eax,word ptr[r11+2*rbx]
	add ax,word ptr[r11+2*rcx+512]
	add ax,word ptr[r11+2*rdx+1024]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r11+2*rbx+1536]
	add ax,word ptr[r11+2*rcx+2048]
	add ax,word ptr[r11+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r11+2*rbx+3072]
	add ax,word ptr[r11+2*rcx+3584]
	add ax,word ptr[r11+2*rdx+4096]
	pinsrw xmm0,eax,4
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movq qword ptr[rdi],xmm0
	add rsi,r12
	add rdi,r12
	dec r10d
	jnz Boucle1_2
	
Suite1_2:
	mov r10d,r8d
	and r10d,1
	jz short Suite2_2

	movzx edx,byte ptr[rsi]
	movzx ecx,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R ecx=G edx=B
	movzx eax,word ptr[r11+2*rbx]
	add ax,word ptr[r11+2*rcx+512]
	add ax,word ptr[r11+2*rdx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r11+2*rbx+1536]
	add ax,word ptr[r11+2*rcx+2048]
	add ax,word ptr[r11+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r11+2*rbx+3072]
	add ax,word ptr[r11+2*rcx+3584]
	add ax,word ptr[r11+2*rdx+4096]
	pinsrw xmm0,eax,0
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movd dword ptr[rdi],xmm0
	add rsi,r15
	add rdi,r15
	
Suite2_2:	
	add rsi,r13
	add rdi,r14
	dec r9d
	jnz Boucle0_2

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_SSE2_RGB24 endp


;JPSDR_Saturation_YUYV proc src:dword,dst:dword,w:dword,h:dword,lookup:dword,src_modulo:dword,dst_modulo:dword
;public JPSDR_Saturation_YUYV
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_YUYV proc public frame

lookup equ qword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

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


	cld
	mov r11,rcx
	mov rdi,rdx
	mov rsi,lookup
	mov r12,4
	mov r13,src_modulo
	mov r14,dst_modulo

Boucle0_3:
	mov r10d,r8d
Boucle1_3:
	movzx rax,byte ptr[r11]
	movzx rbx,byte ptr[r11+1]
	movzx rcx,byte ptr[r11+2]
	movzx rdx,byte ptr[r11+3] ; eax=Y1, ebx=U, ecx =Y2, edx =V
	mov r15d,eax
	mov al,byte ptr[rsi+2*rcx]
	mov ah,byte ptr[rsi+2*rdx+1024]
	shl eax,16
	mov ecx,r15d
	mov ah,byte ptr[rsi+2*rbx+512]
	mov al,byte ptr[rsi+2*rcx]
	stosd
	add r11,r12
	dec r10d
	jnz short Boucle1_3
	add r11,r13
	add rdi,r14
	dec r9d
	jnz short Boucle0_3
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_Saturation_YUYV endp


;JPSDR_Saturation_UYVY proc src:dword,dst:dword,w:dword,h:dword,lookup:dword,src_modulo:dword,dst_modulo:dword
;public JPSDR_Saturation_UYVY
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_UYVY proc public frame

lookup equ qword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

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

	cld
	mov r11,rcx
	mov rdi,rdx
	mov rsi,lookup
	mov r12,4
	mov r13,src_modulo
	mov r14,dst_modulo

Boucle0_4:
	mov r10d,r8d
Boucle1_4:
	movzx rax,byte ptr[r11]	
	movzx rbx,byte ptr[r11+1]
	movzx rcx,byte ptr[r11+2]
	movzx rdx,byte ptr[r11+3] ; eax=U, ebx=Y1, ecx =V, edx =Y2
	mov r15d,eax
	mov al,byte ptr[rsi+2*rcx+1024]
	mov ah,byte ptr[rsi+2*rdx]
	shl eax,16
	mov ecx,r15d
	mov ah,byte ptr[rsi+2*rbx]
	mov al,byte ptr[rsi+2*rcx+512]
	stosd
	add r11,r12
	dec r10d
	jnz short Boucle1_4
	add r11,r13
	add rdi,r14
	dec r9d
	jnz short Boucle0_4

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_UYVY endp



;JPSDR_Saturation_Planar_YUV proc src:dword,dst:dword,w:dword,h:dword,lookup:dword,
;src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_Planar_YUV proc public frame

lookup equ qword ptr[rbp+48]
src_modulo equ qword ptr[rbp+56]
dst_modulo equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi
	.endprolog

	cld
	mov rdi,rdx
	mov rsi,rcx
	mov rdx,lookup
	mov r10,src_modulo
	mov r11,dst_modulo
	xor rcx,rcx
	xor rax,rax

Boucle0_5:
	mov ecx,r8d
Boucle1_5:	
	lodsb
	movzx rax,byte ptr[rdx+2*rax]
	stosb
	loop Boucle1_5
	add rsi,r10
	add rdi,r11
	dec r9d
	jnz short Boucle0_5

	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_Planar_YUV endp


;JPSDR_Saturation_Hue_Planar_YUV proc src_cb:dword,src_cr:dword,dst_cb:dword,dst_cr:dword,
;	w:dword,h:dword,lookup_cbcr:dword,src_cb_modulo:dword,src_cr_modulo:dword,
;	dst_cb_modulo:dword,dst_cr_modulo:dword
;public JPSDR_Saturation_Hue_Planar_YUV
; src_cb = rcx
; src_cr = rdx
; dst_cb = r8
; dst_cr = r9

JPSDR_Saturation_Hue_Planar_YUV proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
lookup_cbcr equ qword ptr[rbp+64]
src_cb_modulo equ qword ptr[rbp+72]
src_cr_modulo equ qword ptr[rbp+80]
dst_cb_modulo equ qword ptr[rbp+88]
dst_cr_modulo equ qword ptr[rbp+96]

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
	mov r8,rbx
	mov rbx,rdx				; r8=src_cr		rbx=dst_cb
	mov rdi,r9
	mov r9,lookup_cbcr		; r9=lookup_cb_cr	rdi=dst_cr
	mov r10d,w
	mov r11d,h
	mov r12,src_cb_modulo
	mov r13,src_cr_modulo
	mov r14,dst_cb_modulo
	mov r15,dst_cr_modulo
	xor rcx,rcx
	xor rax,rax

Boucle_hue0:
	mov ecx,r10d
Boucle_hue1:
	movzx eax,byte ptr[r8]
	mov ah,byte ptr[rsi]
	inc r8
	mov dx,word ptr[r9+2*rax]
	inc rsi
	mov byte ptr[rbx],dh
	inc rbx
	mov byte ptr[rdi],dl
	inc rdi
	loop Boucle_hue1
	add rbx,r14
	add rdi,r15
	add rsi,r12
	add r8,r13
	dec r11d
	jnz short Boucle_hue0
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_Hue_Planar_YUV endp



;JPSDR_Saturation_Y_YUYV proc src:dword,dst:dword,w:dword,h:dword,offset_U:byte,offset_V:byte,
;	lookup:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_Y_YUYV proc public frame

offset_U equ byte ptr[rbp+48]
offset_V equ byte ptr[rbp+56]
lookup equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]
	
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
	
	cld
	mov rdi,rdx
	mov rsi,rcx
	xor rcx,rcx
	xor rax,rax
	xor rbx,rbx
	movzx r10d,offset_U
	movzx r11d,offset_V
	shl r10d,8
	shl r11d,8
	mov rdx,lookup
	mov r12,src_modulo
	mov r13,dst_modulo
	mov r14,4
	
BoucleY_0_3:
	mov ecx,r8d
BoucleY_1_3:
	movzx eax,byte ptr[rsi]
	movzx ebx,byte ptr[rsi+2] ; eax=Y1, ebx=Y2
	mov r15d,eax
	movzx eax,byte ptr[rdx+2*rbx]
	or eax,r11d
	shl eax,16
	mov ebx,r15d
	mov al,byte ptr[rdx+2*rbx]
	or eax,r10d
	stosd
	add rsi,r14
	loop BoucleY_1_3
	add rsi,r12
	add rdi,r13
	dec r9d
	jnz short BoucleY_0_3

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_Y_YUYV endp


;JPSDR_Saturation_Y_UYVY proc src:dword,dst:dword,w:dword,h:dword,offset_U:byte,offset_V:byte,
;	lookup:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_Y_UYVY proc public frame

offset_U equ byte ptr[rbp+48]
offset_V equ byte ptr[rbp+56]
lookup equ qword ptr[rbp+64]
src_modulo equ qword ptr[rbp+72]
dst_modulo equ qword ptr[rbp+80]
	
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


	cld
	mov rdi,rdx
	mov rsi,rcx
	xor rcx,rcx
	xor rax,rax
	xor rbx,rbx
	movzx r10d,offset_U
	movzx r11d,offset_V
	mov rdx,lookup
	mov r12,src_modulo
	mov r13,dst_modulo
	mov r14,4
	

BoucleY_0_4:
	mov ecx,r8d
BoucleY_1_4:
	movzx eax,byte ptr[rsi+1] 
	movzx ebx,byte ptr[rsi+3] ; eax=Y1, ebx=Y2
	mov r15d,eax
	mov eax,r11d
	mov ah,byte ptr[rdx+2*rbx]
	shl eax,16
	mov ebx,r15d
	or eax,r10d
	mov ah,byte ptr[rdx+2*rbx]
	stosd
	add rsi,r14
	loop BoucleY_1_4
	add rsi,r12
	add rdi,r13
	dec r9d
	jnz short BoucleY_0_4

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_Y_UYVY endp



;JPSDR_Saturation_Y_SSE2_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,lookup:dword,
;	src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_Y_SSE2_RGB24 proc public frame

offset_Y equ word ptr[rbp+48]
lookup equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
dst_modulo equ qword ptr[rbp+72]

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
	xor rax,rax
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,4		
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,0	
	mov r10,8
	mov r15,4
	mov r12,lookup
	mov r13,src_modulo
	mov r14,dst_modulo

BoucleY_0_2:
	mov r11d,r8d
	shr r11d,1
	jz short SuiteY1_1
BoucleY_1_2:
	movzx edx,byte ptr[rsi]
	movzx ecx,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R ecx=G edx=B
	movzx eax,word ptr[r12+2*rbx]
	add ax,word ptr[r12+2*rcx+512]
	add ax,word ptr[r12+2*rdx+1024]
	pinsrw xmm0,eax,2
	pinsrw xmm0,eax,1
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+4]
	movzx ecx,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R ecx=G edx=B
	movzx eax,word ptr[r12+2*rbx]
	add ax,word ptr[r12+2*rcx+512]
	add ax,word ptr[r12+2*rdx+1024]
	pinsrw xmm0,eax,6
	pinsrw xmm0,eax,5
	pinsrw xmm0,eax,4
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movq qword ptr[rdi],xmm0
	add rsi,r10
	add rdi,r10
	dec r11d
	jnz short BoucleY_1_2
	
SuiteY1_1:
	mov r11d,r8d
	and r11d,1
	jz short SuiteY1_2
	
	movzx edx,byte ptr[rsi]
	movzx ecx,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R ecx=G edx=B
	movzx eax,word ptr[r12+2*rbx]
	add ax,word ptr[r12+2*rcx+512]
	add ax,word ptr[r12+2*rdx+1024]
	pinsrw xmm0,eax,2
	pinsrw xmm0,eax,1
	pinsrw xmm0,eax,0
	paddsw xmm0,xmm1
	psraw xmm0,4
	packuswb xmm0,xmm2
	movd dword ptr[rdi],xmm0
	add rsi,r15
	add rdi,r15

SuiteY1_2:	
	add rsi,r13
	add rdi,r14
	dec r9d
	jnz BoucleY_0_2
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_Saturation_Y_SSE2_RGB24 endp



;JPSDR_Saturation_Non_SSE_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_R:word,offset_G:word,
;	offset_B:word,lookup:dword,src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_Non_SSE_RGB24 proc public frame

offset_R equ word ptr[rbp+48]
offset_G equ word ptr[rbp+56]
offset_B equ word ptr[rbp+64]
lookup equ qword ptr[rbp+72]
src_modulo equ qword ptr[rbp+80]
dst_modulo equ qword ptr[rbp+88]

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
	xor rax,rax
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	mov r10,4
	mov r12,lookup
	movzx r13d,offset_R
	movzx r14d,offset_G
	movzx r15d,offset_B
	
Boucle0_2n:
	mov r11d,r8d
Boucle_b:
	mov eax,dword ptr[rsi]
	mov ebx,eax
	mov ecx,eax
	mov edx,eax
	shr ebx,16
	mov eax,0FFh
	shr ecx,8
	and ebx,eax
	and ecx,eax
	and edx,eax			; ebx=R ecx=G edx=B
	movzx eax,word ptr[r12+2*rbx]
	add ax,word ptr[r12+2*rcx+512]
	add ax,word ptr[r12+2*rdx+1024]
	add eax,r13d
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_r_1_b
	xor ax,ax
	jmp short stk_r_b
suite_r_1_b:
	cmp ax,255
	jbe short stk_r_b
	mov ax,255
stk_r_b:
	shl eax,16
	mov dword ptr[rdi],eax
	movzx eax,word ptr[r12+2*rbx+1536]
	add ax,word ptr[r12+2*rcx+2048]
	add ax,word ptr[r12+2*rdx+2560]
	add eax,r14d
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_g_1_b
	xor ax,ax
	jmp short stk_g_b
suite_g_1_b:
	cmp ax,255
	jbe short stk_g_b
	mov ax,255
stk_g_b:
	shl ax,8
	or dword ptr[rdi],eax
	movzx eax,word ptr[r12+2*rbx+3072]
	add ax,word ptr[r12+2*rcx+3584]
	add ax,word ptr[r12+2*rdx+4096]
	add eax,r15d
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_b_1_b
	xor ax,ax
	jmp short stk_b_b
suite_b_1_b:
	cmp ax,255
	jbe short stk_b_b
	mov ax,255
stk_b_b:
	or dword ptr[rdi],eax
	add rsi,r10
	add rdi,r10
	dec r11d
	jnz Boucle_b
	add rsi,src_modulo
	add rdi,dst_modulo
	dec r9d
	jnz Boucle0_2n

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_Non_SSE_RGB24 endp


;JPSDR_Saturation_Y_Non_SSE_RGB24 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,lookup:dword,
;	src_modulo:dword,dst_modulo:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Saturation_Y_Non_SSE_RGB24 proc public frame

offset_Y equ word ptr[rbp+48]
lookup equ qword ptr[rbp+56]
src_modulo equ qword ptr[rbp+64]
dst_modulo equ qword ptr[rbp+72]

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
	xor rax,rax
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	mov r10,4
	mov r12,lookup
	mov r13,src_modulo
	mov r14,dst_modulo
	movzx r15d,offset_Y

BoucleY_0_2n:
	mov r11d,r8d
BoucleY_b:
	mov eax,dword ptr[rsi]
	mov ebx,eax
	mov ecx,eax
	mov edx,eax
	shr ebx,16
	mov eax,0FFh
	shr ecx,8
	and ebx,eax
	and ecx,eax
	and edx,eax			; ebx=R ecx=G edx=B
	movzx eax,word ptr[r12+2*rbx]
	add ax,word ptr[r12+2*rcx+512]
	add ax,word ptr[r12+2*rdx+1024]
	add eax,r15d
	sar ax,4
	cmp ax,07FFFh
	jbe short suite_Y_1_b
	xor ax,ax
	jmp short stk_Y_b
suite_Y_1_b:
	cmp ax,255
	jbe short stk_Y_b
	mov ax,255
stk_Y_b:
	mov ebx,eax
	mov ecx,eax
	shl ebx,16
	shl ecx,8
	or eax,ebx
	or eax,ecx
	mov dword ptr[rdi],eax
	add rsi,r10
	add rdi,r10
	dec r11d
	jnz short BoucleY_b
	add rsi,r13
	add rdi,r14
	dec r9d
	jnz BoucleY_0_2n
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Saturation_Y_Non_SSE_RGB24 endp

end
