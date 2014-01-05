.code

;JPSDR_ColorSpaceConvert_YV24_SSE1 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV24_SSE1 proc public frame

dst_u equ qword ptr[rbp+48]
dst_v equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]
offset_Y equ word ptr[rbp+80]
offset_U equ word ptr[rbp+88]
offset_V equ word ptr[rbp+96]
lookup equ qword ptr[rbp+104]
src_modulo_y equ qword ptr[rbp+112]
src_modulo_u equ qword ptr[rbp+120]
src_modulo_v equ qword ptr[rbp+128]
dst_modulo_y equ qword ptr[rbp+136]
dst_modulo_u equ qword ptr[rbp+144]
dst_modulo_v equ qword ptr[rbp+152]
Min_Y equ word ptr[rbp+160]
Max_Y equ word ptr[rbp+168]
Min_U equ word ptr[rbp+176]
Max_U equ word ptr[rbp+184]
Min_V equ word ptr[rbp+192]
Max_V equ word ptr[rbp+200]

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
	mov r11,rdx
	mov r12,r8
	mov rdi,r9
	mov r13,dst_u
	mov r14,dst_v				;rsi=src_y r11=src_u r12=src_v rdi=dst_y r13=dst_u r14=dst_v
	mov r8d,w
	mov r9d,h
	mov r10,lookup	
	
	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx

Boucle0_1:
	mov r15d,r8d
Boucle1_1:
	movzx ebx,byte ptr[rsi]
	inc rsi
	movzx ecx,byte ptr[r11]
	inc r11
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y
	inc r12
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	inc rdi
	pextrw eax,xmm0,1
	mov byte ptr[r13],al
	inc r13
	pextrw eax,xmm0,2
	mov byte ptr[r14],al
	inc r14
	dec r15d
	jnz Boucle1_1
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo_y
	add r13,dst_modulo_u
	add r14,dst_modulo_v
	dec r9d
	jnz Boucle0_1
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_ColorSpaceConvert_YV24_SSE1 endp




;JPSDR_ColorSpaceConvert_YV24_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV24_SSE2 proc public frame

dst_u equ qword ptr[rbp+48]
dst_v equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]
offset_Y equ word ptr[rbp+80]
offset_U equ word ptr[rbp+88]
offset_V equ word ptr[rbp+96]
lookup equ qword ptr[rbp+104]
src_modulo_y equ qword ptr[rbp+112]
src_modulo_u equ qword ptr[rbp+120]
src_modulo_v equ qword ptr[rbp+128]
dst_modulo_y equ qword ptr[rbp+136]
dst_modulo_u equ qword ptr[rbp+144]
dst_modulo_v equ qword ptr[rbp+152]
Min_Y equ word ptr[rbp+160]
Max_Y equ word ptr[rbp+168]
Min_U equ word ptr[rbp+176]
Max_U equ word ptr[rbp+184]
Min_V equ word ptr[rbp+192]
Max_V equ word ptr[rbp+200]

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
	mov r11,rdx
	mov r12,r8
	mov rdi,r9
	mov r13,dst_u
	mov r14,dst_v				;rsi=src_y r11=src_u r12=src_v rdi=dst_y r13=dst_u r14=dst_v
	mov r8d,w
	mov r9d,h
	mov r10,lookup	
	
	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,4
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,4
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx

Boucle0_2:
	mov r15d,r8d
Boucle1_2:
	movzx ebx,byte ptr[rsi]
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2

	movzx ebx,byte ptr[rsi+1]
	add rsi,2
	movzx ecx,byte ptr[r11+1]
	add r11,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	add r12,2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,4
	mov byte ptr[rdi+1],al
	add rdi,2
	pextrw eax,xmm0,1
	mov byte ptr[r13],al
	pextrw eax,xmm0,5
	mov byte ptr[r13+1],al
	add r13,2
	pextrw eax,xmm0,2
	mov byte ptr[r14],al
	pextrw eax,xmm0,6
	mov byte ptr[r14+1],al
	add r14,2
	dec r15d
	jnz Boucle1_2
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo_y
	add r13,dst_modulo_u
	add r14,dst_modulo_v
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

JPSDR_ColorSpaceConvert_YV24_SSE2 endp




;JPSDR_ColorSpaceConvert_YUY2_SSE1 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_YUY2_SSE1 proc public frame

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
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10,lookup
	mov r11,src_modulo
	mov r12,dst_modulo
	mov r13,4

	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14

Boucle0_3:
	mov r15d,r8d
Boucle1_3:
	movzx ebx,byte ptr[rsi]
	movzx ecx,byte ptr[rsi+1]
	movzx r14d,byte ptr[rsi+2]
	movzx edx,byte ptr[rsi+3] ; edx=V ecx=U ebx=Y1 r14=Y2
	add rsi,r13
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]	
	add ax,word ptr[r10+2*r14+1536]	
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r14+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*r14]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]	
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,1
	mov byte ptr[rdi+1],al
	pextrw eax,xmm0,3
	mov byte ptr[rdi+2],al
	pextrw eax,xmm0,2
	mov byte ptr[rdi+3],al
	add rdi,r13
	dec r15d
	jnz Boucle1_3
	add rsi,r11
	add rdi,r12
	dec r9d
	jnz Boucle0_3

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_ColorSpaceConvert_YUY2_SSE1 endp



;JPSDR_ColorSpaceConvert_YUY2_SSE2 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_YUY2_SSE2 proc public frame

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
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10,lookup
	mov r11,src_modulo
	mov r12,dst_modulo
	mov r13,8

	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14

Boucle0_4:
	mov r15d,r8d
Boucle1_4:
	movzx ebx,byte ptr[rsi]
	movzx ecx,byte ptr[rsi+1]
	movzx r14d,byte ptr[rsi+2]
	movzx edx,byte ptr[rsi+3] ; edx=V ecx=U ebx=Y1 r14=Y2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]	
	add ax,word ptr[r10+2*r14+1536]	
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r14+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*r14]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]	
	pinsrw xmm0,eax,3

	movzx ebx,byte ptr[rsi+4]
	movzx ecx,byte ptr[rsi+5]
	movzx r14d,byte ptr[rsi+6]
	movzx edx,byte ptr[rsi+7] ; edx=V ecx=U ebx=Y1 r14=Y2
	add rsi,r13
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]	
	add ax,word ptr[r10+2*r14+1536]	
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r14+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*r14]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]	
	pinsrw xmm0,eax,7
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,1
	mov byte ptr[rdi+1],al
	pextrw eax,xmm0,3
	mov byte ptr[rdi+2],al
	pextrw eax,xmm0,2
	mov byte ptr[rdi+3],al
	pextrw eax,xmm0,4
	mov byte ptr[rdi+4],al
	pextrw eax,xmm0,5
	mov byte ptr[rdi+5],al
	pextrw eax,xmm0,7
	mov byte ptr[rdi+6],al
	pextrw eax,xmm0,6
	mov byte ptr[rdi+7],al
	add rdi,r13
	dec r15d
	jnz Boucle1_4
	add rsi,r11
	add rdi,r12
	dec r9d
	jnz Boucle0_4

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_ColorSpaceConvert_YUY2_SSE2 endp



;JPSDR_ColorSpaceConvert_UYVY_SSE1 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_UYVY_SSE1 proc public frame

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
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10,lookup
	mov r11,src_modulo
	mov r12,dst_modulo
	mov r13,4

	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14

Boucle0_5:
	mov r15d,r8d
Boucle1_5:
	movzx ecx,byte ptr[rsi]
	movzx ebx,byte ptr[rsi+1]
	movzx edx,byte ptr[rsi+2]
	movzx r14d,byte ptr[rsi+3] ; edx=V ecx=U ebx=Y1 r14=Y2
	add rsi,r13
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]	
	add ax,word ptr[r10+2*r14+1536]	
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r14+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*r14]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]	
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,1
	mov byte ptr[rdi],al
	pextrw eax,xmm0,0
	mov byte ptr[rdi+1],al
	pextrw eax,xmm0,2
	mov byte ptr[rdi+2],al
	pextrw eax,xmm0,3
	mov byte ptr[rdi+3],al
	add rdi,r13
	dec r15d
	jnz Boucle1_5
	add rsi,r11
	add rdi,r12
	dec r9d
	jnz Boucle0_5

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_ColorSpaceConvert_UYVY_SSE1 endp




;JPSDR_ColorSpaceConvert_UYVY_SSE2 proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_UYVY_SSE2 proc public frame

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
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10,lookup
	mov r11,src_modulo
	mov r12,dst_modulo
	mov r13,8

	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14

Boucle0_6:
	mov r15d,r8d
Boucle1_6:
	movzx ecx,byte ptr[rsi]
	movzx ebx,byte ptr[rsi+1]
	movzx edx,byte ptr[rsi+2]
	movzx r14d,byte ptr[rsi+3] ; edx=V ecx=U ebx=Y1 r14=Y2
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]	
	add ax,word ptr[r10+2*r14+1536]	
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r14+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*r14]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]	
	pinsrw xmm0,eax,3

	movzx ecx,byte ptr[rsi+4]
	movzx ebx,byte ptr[rsi+5]
	movzx edx,byte ptr[rsi+6]
	movzx r14d,byte ptr[rsi+7] ; edx=V ecx=U ebx=Y1 r14=Y2
	add rsi,r13
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]	
	add ax,word ptr[r10+2*r14+1536]	
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r14+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*r14]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]	
	pinsrw xmm0,eax,7
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,1
	mov byte ptr[rdi],al
	pextrw eax,xmm0,0
	mov byte ptr[rdi+1],al
	pextrw eax,xmm0,2
	mov byte ptr[rdi+2],al
	pextrw eax,xmm0,3
	mov byte ptr[rdi+3],al
	pextrw eax,xmm0,5
	mov byte ptr[rdi+4],al
	pextrw eax,xmm0,4
	mov byte ptr[rdi+5],al
	pextrw eax,xmm0,6
	mov byte ptr[rdi+6],al
	pextrw eax,xmm0,7
	mov byte ptr[rdi+7],al
	add rdi,r13
	dec r15d
	jnz Boucle1_6
	add rsi,r11
	add rdi,r12
	dec r9d
	jnz Boucle0_6

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_ColorSpaceConvert_UYVY_SSE2 endp




;JPSDR_ColorSpaceConvert_YV16_SSE1 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV16_SSE1 proc public frame

dst_u equ qword ptr[rbp+48]
dst_v equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]
offset_Y equ word ptr[rbp+80]
offset_U equ word ptr[rbp+88]
offset_V equ word ptr[rbp+96]
lookup equ qword ptr[rbp+104]
src_modulo_y equ qword ptr[rbp+112]
src_modulo_u equ qword ptr[rbp+120]
src_modulo_v equ qword ptr[rbp+128]
dst_modulo_y equ qword ptr[rbp+136]
dst_modulo_u equ qword ptr[rbp+144]
dst_modulo_v equ qword ptr[rbp+152]
Min_Y equ word ptr[rbp+160]
Max_Y equ word ptr[rbp+168]
Min_U equ word ptr[rbp+176]
Max_U equ word ptr[rbp+184]
Min_V equ word ptr[rbp+192]
Max_V equ word ptr[rbp+200]

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
	mov r11,rdx
	mov r12,r8
	mov rdi,r9
	mov r13,dst_u
	mov r14,dst_v				;rsi=src_y r11=src_u r12=src_v rdi=dst_y r13=dst_u r14=dst_v
	mov r10,lookup	
	mov r9,2
	
	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r15,r15
	
Boucle0_7:
	mov r8d,w
Boucle1_7:
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	add rsi,r9
	movzx ecx,byte ptr[r11]
	inc r11
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y1 r15=Y2
	inc r12
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+1536]
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*r15]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,3
	mov byte ptr[rdi+1],al
	add rdi,r9
	pextrw eax,xmm0,1
	mov byte ptr[r13],al
	inc r13
	pextrw eax,xmm0,2
	mov byte ptr[r14],al
	inc r14
	dec r8d
	jnz Boucle1_7
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo_y
	add r13,dst_modulo_u
	add r14,dst_modulo_v
	dec h
	jnz Boucle0_7
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_ColorSpaceConvert_YV16_SSE1 endp



;JPSDR_ColorSpaceConvert_YV16_SSE2 proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV16_SSE2 proc public frame

dst_u equ qword ptr[rbp+48]
dst_v equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]
offset_Y equ word ptr[rbp+80]
offset_U equ word ptr[rbp+88]
offset_V equ word ptr[rbp+96]
lookup equ qword ptr[rbp+104]
src_modulo_y equ qword ptr[rbp+112]
src_modulo_u equ qword ptr[rbp+120]
src_modulo_v equ qword ptr[rbp+128]
dst_modulo_y equ qword ptr[rbp+136]
dst_modulo_u equ qword ptr[rbp+144]
dst_modulo_v equ qword ptr[rbp+152]
Min_Y equ word ptr[rbp+160]
Max_Y equ word ptr[rbp+168]
Min_U equ word ptr[rbp+176]
Max_U equ word ptr[rbp+184]
Min_V equ word ptr[rbp+192]
Max_V equ word ptr[rbp+200]

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
	mov r11,rdx
	mov r12,r8
	mov rdi,r9
	mov r13,dst_u
	mov r14,dst_v				;rsi=src_y r11=src_u r12=src_v rdi=dst_y r13=dst_u r14=dst_v
	mov r10,lookup	
	mov r9,2
	
	xor rax,rax
	pxor xmm3,xmm3
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,6
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r15,r15
	
Boucle0_8:
	mov r8d,w
Boucle1_8:
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y1 r15=Y2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+1536]
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*r15]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3

	movzx ebx,byte ptr[rsi+2]
	movzx r15d,byte ptr[rsi+3]
	add rsi,4
	movzx ecx,byte ptr[r11+1]
	add r11,r9
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y1 r15=Y2
	add r12,r9
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+1536]
	sar ax,1
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3072]
	sar ax,1
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*r15]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,7
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,3
	mov byte ptr[rdi+1],al
	pextrw eax,xmm0,4
	mov byte ptr[rdi+2],al
	pextrw eax,xmm0,7
	mov byte ptr[rdi+3],al
	add rdi,4
	pextrw eax,xmm0,1
	mov byte ptr[r13],al
	pextrw eax,xmm0,5
	mov byte ptr[r13+1],al
	add r13,r9
	pextrw eax,xmm0,2
	mov byte ptr[r14],al
	pextrw eax,xmm0,6
	mov byte ptr[r14+1],al
	add r14,r9
	dec r8d
	jnz Boucle1_8
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo_y
	add r13,dst_modulo_u
	add r14,dst_modulo_v
	dec h
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

JPSDR_ColorSpaceConvert_YV16_SSE2 endp




end
