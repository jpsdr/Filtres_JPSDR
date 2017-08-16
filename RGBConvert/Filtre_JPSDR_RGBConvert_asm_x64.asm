.code


;JPSDR_RGBConvert_RGB32toYV24_SSE proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9

JPSDR_RGBConvert_RGB32toYV24_SSE proc public frame

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
	
Boucle0_2:
	or r8d,r8d
	jz Suite0_2
	mov ecx,r8d
Boucle1_2:
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
	jnz Boucle1_2
	
Suite0_2:
	mov eax,r9d
	and eax,1
	jz Suite1_2
	
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
		
Suite1_2:	
	add rsi,src_modulo
	add rdi,dst_modulo_y
	add r11,dst_modulo_u
	add r12,dst_modulo_v
	dec h
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

JPSDR_RGBConvert_RGB32toYV24_SSE endp


;JPSDR_RGBConvert_RGB32toYV24_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9

JPSDR_RGBConvert_RGB32toYV24_AVX proc public frame

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
	
Boucle0_2_AVX:
	or r8d,r8d
	jz Suite0_2_AVX
	
	mov ecx,r8d
Boucle1_2_AVX:
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
	jnz Boucle1_2_AVX
	
Suite0_2_AVX:
	mov eax,r9d
	and eax,1
	jz Suite1_2_AVX
	
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
		
Suite1_2_AVX:	
	add rsi,src_modulo
	add rdi,dst_modulo_y
	add r11,dst_modulo_u
	add r12,dst_modulo_v
	dec h
	jnz Boucle0_2_AVX

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV24_AVX endp


;JPSDR_RGBConvert_RGB32toYV16_SSE proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9

JPSDR_RGBConvert_RGB32toYV16_SSE proc public frame

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
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	movzx eax,offset_V
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
	mov rsi,rcx
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13,16
	mov r14,2
	mov r9,4
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,w
	shr r8d,2				;r8d=w0
	
Boucle0_6:
	or r8d,r8d
	jz Suite0_6
	mov ecx,r8d
Boucle1_6:
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1

	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
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
	pinsrw xmm0,eax,3

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,2
	add rsi,r13
	mov word ptr[r11],ax
	pextrw eax,xmm0,3
	add r11,r14
	mov word ptr[r12],ax
	movd dword ptr[rdi],xmm0
	add r12,r14
	add rdi,r9
	
	dec ecx
	jnz Boucle1_6
	
Suite0_6:	
	mov eax,w
	and eax,3
	jz Suite2_6
	and eax,2
	jz Suite1_6

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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1

	mov eax,w
	and eax,1
	jz Suite0_6b

	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,7

	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	add rsi,12
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	mov byte ptr[rdi+2],al
	pextrw eax,xmm0,2
	add rdi,3
	mov word ptr[r11],ax
	pextrw eax,xmm0,3
	add r11,r14
	mov word ptr[r12],ax
	add r12,r14
	
	jmp Suite2_6
	
Suite0_6b:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	add rsi,8
	mov word ptr[rdi],ax
	pextrw eax,xmm0,2
	add rdi,r14
	mov byte ptr[r11],al
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite2_6
	
Suite1_6:	
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
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3	
	
	pextrw eax,xmm0,0
	add rsi,r9
	mov byte ptr[rdi],al
	pextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	pextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite2_6:	
	add rsi,src_modulo
	add rdi,dst_modulo_y
	add r11,dst_modulo_u
	add r12,dst_modulo_v
	dec h
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

JPSDR_RGBConvert_RGB32toYV16_SSE endp


;JPSDR_RGBConvert_RGB32toYV16_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo_y:dword,dst_modulo_u:dword,dst_modulo_v:dword
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9

JPSDR_RGBConvert_RGB32toYV16_AVX proc public frame

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
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5	
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6
	vpinsrw xmm3,xmm3,eax,7		
	
	mov rsi,rcx
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13,16
	mov r14,2
	mov r9,4
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,w
	shr r8d,2				;r8d=w0
	
Boucle0_6_AVX:
	or r8d,r8d
	jz Suite0_6_AVX
	
	mov ecx,r8d
Boucle1_6_AVX:
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
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1

	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
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
	vpinsrw xmm0,xmm0,eax,3

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,2
	add rsi,r13
	mov word ptr[r11],ax
	vpextrw eax,xmm0,3
	add r11,r14
	mov word ptr[r12],ax
	vmovd dword ptr[rdi],xmm0
	add r12,r14
	add rdi,r9
	
	dec ecx
	jnz Boucle1_6_AVX
	
Suite0_6_AVX:	
	mov eax,w
	and eax,3
	jz Suite2_6_AVX
	and eax,2
	jz Suite1_6_AVX

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
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1

	mov eax,w
	and eax,1
	jz Suite0_6b_AVX

	movzx edx,byte ptr[rsi+8]
	movzx r15d,byte ptr[rsi+9]
	movzx ebx,byte ptr[rsi+10] ; rbx=R r15=G rdx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,7

	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	add rsi,12
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	mov byte ptr[rdi+2],al
	vpextrw eax,xmm0,2
	add rdi,3
	mov word ptr[r11],ax
	vpextrw eax,xmm0,3
	add r11,r14
	mov word ptr[r12],ax
	add r12,r14
	
	jmp Suite2_6_AVX
	
Suite0_6b_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	add rsi,8
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,2
	add rdi,r14
	mov byte ptr[r11],al
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite2_6_AVX
	
Suite1_6_AVX:	
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
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3	
	
	vpextrw eax,xmm0,0
	add rsi,r9
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	vpextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite2_6_AVX:	
	add rsi,src_modulo
	add rdi,dst_modulo_y
	add r11,dst_modulo_u
	add r12,dst_modulo_v
	dec h
	jnz Boucle0_6_AVX

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV16_AVX endp


;JPSDR_RGBConvert_RGB32toYUYV_SSE proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,
; offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,dst_modulo:dword,
;Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; w = r8d
; h = r9d

JPSDR_RGBConvert_RGB32toYUYV_SSE proc public frame

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

JPSDR_RGBConvert_RGB32toYUYV_SSE endp


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


;JPSDR_RGBConvert_RGB32toYV12_SSE proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
;	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9
JPSDR_RGBConvert_RGB32toYV12_SSE proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_pitch equ qword ptr[rbp+96]
src_modulo equ qword ptr[rbp+104]
dst_pitch_y equ qword ptr[rbp+112]
dst_modulo_y equ qword ptr[rbp+120]
dst_modulo_u equ qword ptr[rbp+128]
dst_modulo_v equ qword ptr[rbp+136]
Min_Y equ word ptr[rbp+144]
Max_Y equ word ptr[rbp+152]
Min_U equ word ptr[rbp+160]
Max_U equ word ptr[rbp+168]
Min_V equ word ptr[rbp+176]
Max_V equ word ptr[rbp+184]

; local data
h0 equ dword ptr[rbp+40]

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
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,4
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	
	mov rsi,rcx             ;rsi=src
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13d,w
	
	mov r14,src_pitch
	mov r9,dst_pitch_y
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r13d
	inc r8d
	shr r8d,1
	dec r8d					;r8d=w0
	
	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9	
	mov h0,eax
	
Boucle0_9:
	or r8d,r8d
	jz Suite0_9
	mov ecx,r8d	
Boucle1_9:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	dec ecx
	jnz Boucle1_9
	
Suite0_9:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6

	mov eax,r13d
	and eax,1
	jnz Suite1_9a

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite1_9b
	
Suite1_9a:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,2
	add rsi,4
	mov byte ptr[rdi+r9],al
	
	pextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite1_9b:	
	add rsi,src_modulo
	add rsi,r14
	
	add rdi,dst_modulo_y
	add rdi,r9
	
	add r11,dst_modulo_u
	
	add r12,dst_modulo_v
	
	dec h0
	jnz Boucle0_9

Suite4_9:	
	mov eax,h
	and eax,1
	jnz Suite2_9

	or r8d,r8d
	jz Suite5_9
	mov ecx,r8d
Boucle2_9:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle2_9
	
Suite5_9:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz Suite3_9a
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b
	
Suite3_9a:		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,2
	mov byte ptr[rdi+r9],al
	
	pextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	mov byte ptr[r12],al
	
	jmp Suite3_9b
	
	
Suite2_9:	
	or r8d,r8d
	jz Suite6_9
	mov ecx,r8d
Boucle3_9:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	add rsi,8
	mov word ptr[rdi],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle3_9
	
Suite6_9:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz short Suite3_9c
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
	pextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b
		
Suite3_9c:			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	
	pextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	mov byte ptr[r12],al
	
Suite3_9b:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV12_SSE endp


;JPSDR_RGBConvert_RGB32toYV12_AVX proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
;	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9
JPSDR_RGBConvert_RGB32toYV12_AVX proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_pitch equ qword ptr[rbp+96]
src_modulo equ qword ptr[rbp+104]
dst_pitch_y equ qword ptr[rbp+112]
dst_modulo_y equ qword ptr[rbp+120]
dst_modulo_u equ qword ptr[rbp+128]
dst_modulo_v equ qword ptr[rbp+136]
Min_Y equ word ptr[rbp+144]
Max_Y equ word ptr[rbp+152]
Min_U equ word ptr[rbp+160]
Max_U equ word ptr[rbp+168]
Min_V equ word ptr[rbp+176]
Max_V equ word ptr[rbp+184]

; local data
h0 equ dword ptr[rbp+40]

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
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6	
	
	mov rsi,rcx             ;rsi=src
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13d,w
	
	mov r14,src_pitch
	mov r9,dst_pitch_y
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r13d
	inc r8d
	shr r8d,1
	dec r8d					;r8d=w0
	
	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9_AVX
	mov h0,eax
	
Boucle0_9_AVX:
	or r8d,r8d
	jz Suite0_9_AVX
	
	mov ecx,r8d	
Boucle1_9_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	dec ecx
	jnz Boucle1_9_AVX
	
Suite0_9_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6

	mov eax,r13d
	and eax,1
	jnz Suite1_9a_AVX

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite1_9b_AVX
	
Suite1_9a_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,2
	add rsi,4
	mov byte ptr[rdi+r9],al
	
	vpextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite1_9b_AVX:	
	add rsi,src_modulo
	add rsi,r14
	
	add rdi,dst_modulo_y
	add rdi,r9
	
	add r11,dst_modulo_u
	
	add r12,dst_modulo_v
	
	dec h0
	jnz Boucle0_9_AVX

Suite4_9_AVX:	
	mov eax,h
	and eax,1
	jnz Suite2_9_AVX

	or r8d,r8d
	jz Suite5_9_AVX
	
	mov ecx,r8d
Boucle2_9_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle2_9_AVX
	
Suite5_9_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz Suite3_9a_AVX
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b_AVX
	
Suite3_9a_AVX:		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,2
	mov byte ptr[rdi+r9],al
	
	vpextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	mov byte ptr[r12],al
	
	jmp Suite3_9b_AVX
	
	
Suite2_9_AVX:	
	or r8d,r8d
	jz Suite6_9_AVX
	
	mov ecx,r8d
Boucle3_9_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	add rsi,8
	mov word ptr[rdi],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle3_9_AVX
	
Suite6_9_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz short Suite3_9c_AVX
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
	vpextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b_AVX
		
Suite3_9c_AVX:			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	
	vpextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	mov byte ptr[r12],al
	
Suite3_9b_AVX:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV12_AVX endp


;JPSDR_RGBConvert_RGB32toYV12_SSE_1 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
;	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9
JPSDR_RGBConvert_RGB32toYV12_SSE_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_pitch equ qword ptr[rbp+96]
src_modulo equ qword ptr[rbp+104]
dst_pitch_y equ qword ptr[rbp+112]
dst_modulo_y equ qword ptr[rbp+120]
dst_modulo_u equ qword ptr[rbp+128]
dst_modulo_v equ qword ptr[rbp+136]
Min_Y equ word ptr[rbp+144]
Max_Y equ word ptr[rbp+152]
Min_U equ word ptr[rbp+160]
Max_U equ word ptr[rbp+168]
Min_V equ word ptr[rbp+176]
Max_V equ word ptr[rbp+184]

; local data
h0 equ dword ptr[rbp+40]

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
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,4
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	
	mov rsi,rcx             ;rsi=src
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13d,w
	
	mov r14,src_pitch
	mov r9,dst_pitch_y
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r13d
	inc r8d
	shr r8d,1
	dec r8d					;r8d=w0
	
	mov eax,h
	inc eax
	shr eax,1
	jz Suite4_9_1
	mov h0,eax
	
Boucle0_9_1:
	or r8d,r8d
	jz Suite0_9_1
	mov ecx,r8d	
Boucle1_9_1:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	dec ecx
	jnz Boucle1_9_1
	
Suite0_9_1:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6

	mov eax,r13d
	and eax,1
	jnz Suite1_9a_1

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite1_9b_1
	
Suite1_9a_1:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,2
	add rsi,4
	mov byte ptr[rdi+r9],al
	
	pextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite1_9b_1:	
	add rsi,src_modulo
	add rsi,r14
	
	add rdi,dst_modulo_y
	add rdi,r9
	
	add r11,dst_modulo_u
	
	add r12,dst_modulo_v
	
	dec h0
	jnz Boucle0_9_1

Suite4_9_1:	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV12_SSE_1 endp


;JPSDR_RGBConvert_RGB32toYV12_AVX_1 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
;	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9
JPSDR_RGBConvert_RGB32toYV12_AVX_1 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_pitch equ qword ptr[rbp+96]
src_modulo equ qword ptr[rbp+104]
dst_pitch_y equ qword ptr[rbp+112]
dst_modulo_y equ qword ptr[rbp+120]
dst_modulo_u equ qword ptr[rbp+128]
dst_modulo_v equ qword ptr[rbp+136]
Min_Y equ word ptr[rbp+144]
Max_Y equ word ptr[rbp+152]
Min_U equ word ptr[rbp+160]
Max_U equ word ptr[rbp+168]
Min_V equ word ptr[rbp+176]
Max_V equ word ptr[rbp+184]

; local data
h0 equ dword ptr[rbp+40]

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
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6	
	
	mov rsi,rcx             ;rsi=src
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13d,w
	
	mov r14,src_pitch
	mov r9,dst_pitch_y
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r13d
	inc r8d
	shr r8d,1
	dec r8d					;r8d=w0
	
	mov eax,h
	inc eax
	shr eax,1
	jz Suite4_9_1_AVX
	mov h0,eax
	
Boucle0_9_1_AVX:
	or r8d,r8d
	jz Suite0_9_1_AVX
	
	mov ecx,r8d	
Boucle1_9_1_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	dec ecx
	jnz Boucle1_9_1_AVX
	
Suite0_9_1_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6

	mov eax,r13d
	and eax,1
	jnz Suite1_9a_1_AVX

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite1_9b_1_AVX
	
Suite1_9a_1_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,2
	add rsi,4
	mov byte ptr[rdi+r9],al
	
	vpextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite1_9b_1_AVX:	
	add rsi,src_modulo
	add rsi,r14
	
	add rdi,dst_modulo_y
	add rdi,r9
	
	add r11,dst_modulo_u
	
	add r12,dst_modulo_v
	
	dec h0
	jnz Boucle0_9_1_AVX

Suite4_9_1_AVX:	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV12_AVX_1 endp


;JPSDR_RGBConvert_RGB32toYV12_SSE_2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
;	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9
JPSDR_RGBConvert_RGB32toYV12_SSE_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_pitch equ qword ptr[rbp+96]
src_modulo equ qword ptr[rbp+104]
dst_pitch_y equ qword ptr[rbp+112]
dst_modulo_y equ qword ptr[rbp+120]
dst_modulo_u equ qword ptr[rbp+128]
dst_modulo_v equ qword ptr[rbp+136]
Min_Y equ word ptr[rbp+144]
Max_Y equ word ptr[rbp+152]
Min_U equ word ptr[rbp+160]
Max_U equ word ptr[rbp+168]
Min_V equ word ptr[rbp+176]
Max_V equ word ptr[rbp+184]

; local data
h0 equ dword ptr[rbp+40]

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
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	movzx eax,offset_U
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	movzx eax,Min_U
	pinsrw xmm2,eax,4
	movzx eax,Max_U
	pinsrw xmm3,eax,4
	movzx eax,Min_V
	pinsrw xmm2,eax,6
	movzx eax,Max_V
	pinsrw xmm3,eax,6
	
	mov rsi,rcx             ;rsi=src
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13d,w
	
	mov r14,src_pitch
	mov r9,dst_pitch_y
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r13d
	inc r8d
	shr r8d,1
	dec r8d					;r8d=w0
	
	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9_2
	mov h0,eax
	
Boucle0_9_2:
	or r8d,r8d
	jz Suite0_9_2
	mov ecx,r8d	
Boucle1_9_2:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	dec ecx
	jnz Boucle1_9_2
	
Suite0_9_2:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6

	mov eax,r13d
	and eax,1
	jnz Suite1_9a_2

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite1_9b_2
	
Suite1_9a_2:	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,2
	add rsi,4
	mov byte ptr[rdi+r9],al
	
	pextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite1_9b_2:	
	add rsi,src_modulo
	add rsi,r14
	
	add rdi,dst_modulo_y
	add rdi,r9
	
	add r11,dst_modulo_u
	
	add r12,dst_modulo_v
	
	dec h0
	jnz Boucle0_9_2

Suite4_9_2:	
	mov eax,h
	and eax,1
	jnz Suite2_9_2

	or r8d,r8d
	jz Suite5_9_2
	mov ecx,r8d
Boucle2_9_2:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle2_9_2
	
Suite5_9_2:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz Suite3_9a_2
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	pextrw eax,xmm0,1
	mov word ptr[rdi+r9],ax
	
	pextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b_2
	
Suite3_9a_2:		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	pextrw eax,xmm0,2
	mov byte ptr[rdi+r9],al
	
	pextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	mov byte ptr[r12],al
	
	jmp Suite3_9b_2
	
	
Suite2_9_2:	
	or r8d,r8d
	jz Suite6_9_2
	mov ecx,r8d
Boucle3_9_2:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	add rsi,8
	mov word ptr[rdi],ax
	
	pextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle3_9_2
	
Suite6_9_2:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz short Suite3_9c_2
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
	pextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b_2
		
Suite3_9c_2:			
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	
	pextrw eax,xmm0,0
	mov byte ptr[rdi],al
	
	pextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	pextrw eax,xmm0,6
	mov byte ptr[r12],al
	
Suite3_9b_2:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV12_SSE_2 endp


;JPSDR_RGBConvert_RGB32toYV12_AVX_2 proc src:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_pitch:dword,src_modulo:dword,dst_pitch_y:dword,dst_modulo_y:dword,
;	dst_modulo_u:dword,dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst_y = rdx
; dst_u = r8
; dst_v = r9
JPSDR_RGBConvert_RGB32toYV12_AVX_2 proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
offset_U equ word ptr[rbp+72]
offset_V equ word ptr[rbp+80]
lookup equ qword ptr[rbp+88]
src_pitch equ qword ptr[rbp+96]
src_modulo equ qword ptr[rbp+104]
dst_pitch_y equ qword ptr[rbp+112]
dst_modulo_y equ qword ptr[rbp+120]
dst_modulo_u equ qword ptr[rbp+128]
dst_modulo_v equ qword ptr[rbp+136]
Min_Y equ word ptr[rbp+144]
Max_Y equ word ptr[rbp+152]
Min_U equ word ptr[rbp+160]
Max_U equ word ptr[rbp+168]
Min_V equ word ptr[rbp+176]
Max_V equ word ptr[rbp+184]

; local data
h0 equ dword ptr[rbp+40]

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
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,6	
	
	mov rsi,rcx             ;rsi=src
	mov r10,lookup
	mov rdi,rdx				;rdi=dst_y
	mov r11,r8				;r11=dst_u
	mov r12,r9				;r12=dst_v
	mov r13d,w
	
	mov r14,src_pitch
	mov r9,dst_pitch_y
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15
	
	mov r8d,r13d
	inc r8d
	shr r8d,1
	dec r8d					;r8d=w0
	
	mov eax,h
	inc eax
	shr eax,1
	dec eax
	jz Suite4_9_2_AVX
	mov h0,eax
	
Boucle0_9_2_AVX:
	or r8d,r8d
	jz Suite0_9_2_AVX
	
	mov ecx,r8d	
Boucle1_9_2_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	dec ecx
	jnz Boucle1_9_2_AVX
	
Suite0_9_2_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6

	mov eax,r13d
	and eax,1
	jnz Suite1_9a_2_AVX

	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
	
	jmp Suite1_9b_2_AVX
	
Suite1_9a_2_AVX:	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,2
	add rsi,4
	mov byte ptr[rdi+r9],al
	
	vpextrw eax,xmm0,4
	inc rdi
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	inc r11
	mov byte ptr[r12],al
	inc r12
	
Suite1_9b_2_AVX:	
	add rsi,src_modulo
	add rsi,r14
	
	add rdi,dst_modulo_y
	add rdi,r9
	
	add r11,dst_modulo_u
	
	add r12,dst_modulo_v
	
	dec h0
	jnz Boucle0_9_2_AVX

Suite4_9_2_AVX:	
	mov eax,h
	and eax,1
	jnz Suite2_9_2_AVX

	or r8d,r8d
	jz Suite5_9_2_AVX
	
	mov ecx,r8d
Boucle2_9_2_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	add rsi,8
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle2_9_2_AVX
	
Suite5_9_2_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	push rdx
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	push r15
	push rbx
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx edx,byte ptr[rsi+r14]
	movzx r15d,byte ptr[rsi+r14+1]
	movzx ebx,byte ptr[rsi+r14+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	pop rax
	add ebx,eax
	shr ebx,1
	pop rax
	add r15d,eax
	shr r15d,1
	pop rax
	add edx,eax
	shr edx,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz Suite3_9a_2_AVX
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx edx,byte ptr[rsi+r14+4]
	movzx r15d,byte ptr[rsi+r14+5]
	movzx ebx,byte ptr[rsi+r14+6] ; ebx=R r15d=G edx=B		
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	vpextrw eax,xmm0,1
	mov word ptr[rdi+r9],ax
	
	vpextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b_2_AVX
	
Suite3_9a_2_AVX:		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	vpextrw eax,xmm0,2
	mov byte ptr[rdi+r9],al
	
	vpextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	mov byte ptr[r12],al
	
	jmp Suite3_9b_2_AVX	
	
Suite2_9_2_AVX:	
	or r8d,r8d
	jz Suite6_9_2_AVX
	
	mov ecx,r8d
Boucle3_9_2_AVX:
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	add rsi,8
	mov word ptr[rdi],ax
	
	vpextrw eax,xmm0,2
	add rdi,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	inc r11
	mov byte ptr[r12],al
	inc r12
		
	dec ecx
	jnz Boucle3_9_2_AVX
	
Suite6_9_2_AVX:	
	movzx edx,byte ptr[rsi]
	movzx r15d,byte ptr[rsi+1]
	movzx ebx,byte ptr[rsi+2] ; ebx=R r15d=G edx=B
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*r15+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*r15+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	mov eax,r13d
	and eax,1
	jnz short Suite3_9c_2_AVX
	
	movzx edx,byte ptr[rsi+4]
	movzx r15d,byte ptr[rsi+5]
	movzx ebx,byte ptr[rsi+6] ; ebx=R r15d=G edx=B	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
	vpextrw eax,xmm0,2
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,3
	mov byte ptr[r12],al
	
	jmp Suite3_9b_2_AVX
		
Suite3_9c_2_AVX:			
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	
	vpextrw eax,xmm0,0
	mov byte ptr[rdi],al
	
	vpextrw eax,xmm0,4
	mov byte ptr[r11],al
	
	vpextrw eax,xmm0,6
	mov byte ptr[r12],al
	
Suite3_9b_2_AVX:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_RGBConvert_RGB32toYV12_AVX_2 endp


;JPSDR_RGBConvert_YV24toRGB32_SSE proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
; offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_RGBConvert_YV24toRGB32_SSE proc public frame

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
	pxor xmm2,xmm2
	pxor xmm1,xmm1
	pxor xmm0,xmm0
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
	mov r13,2
	mov r14,8
	
	mov r8d,r9d
	shr r8d,1					;r8d=w0
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15

Boucle0_4:
	or r8d,r8d
	jz Suite0_4
	mov ecx,r8d
Boucle1_4:
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
	add rsi,r13
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	add r11,r13
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	add r12,r13
	pinsrw xmm0,eax,4
	
	paddsw xmm0,xmm1
	psraw xmm0,5
	packuswb xmm0,xmm2
	
	movq qword ptr[rdi],xmm0
	
	add rdi,r14
	
	dec ecx
	jnz Boucle1_4
	
Suite0_4:	
	mov eax,r9d
	and eax,1
	jz Suite1_4
	
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
	packuswb xmm0,xmm2
	
	movd dword ptr[rdi],xmm0
	
	add rdi,4
	
Suite1_4:	
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo
	dec h
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

JPSDR_RGBConvert_YV24toRGB32_SSE endp


;JPSDR_RGBConvert_YV24toRGB32_AVX proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword,h:dword,offset_R:word,
; offset_G:word,offset_B:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_RGBConvert_YV24toRGB32_AVX proc public frame

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
	mov r13,2
	mov r14,8
	
	mov r8d,r9d
	shr r8d,1					;r8d=w0
	
	xor rcx,rcx
	xor rdx,rdx
	xor rbx,rbx
	xor r15,r15

Boucle0_4_AVX:
	or r8d,r8d
	jz Suite0_4_AVX
	
	mov ecx,r8d
Boucle1_4_AVX:
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
	add rsi,r13
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+1024]
	add ax,word ptr[r10+2*rdx+1536]
	add r11,r13
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*r15+2048]
	add r12,r13
	vpinsrw xmm0,xmm0,eax,4
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,5
	vpackuswb xmm0,xmm0,xmm2
	
	vmovq qword ptr[rdi],xmm0
	
	add rdi,r14
	
	dec ecx
	jnz Boucle1_4_AVX
	
Suite0_4_AVX:	
	mov eax,r9d
	and eax,1
	jz Suite1_4_AVX
	
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
	vpackuswb xmm0,xmm0,xmm2
	
	vmovd dword ptr[rdi],xmm0
	
	add rdi,4
	
Suite1_4_AVX:	
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo
	dec h
	jnz Boucle0_4_AVX

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_RGBConvert_YV24toRGB32_AVX endp


end
