.data

align 16

uw_2 word 8 dup(0002h)
uw_3 word 8 dup(0003h)


.code



;JPSDR_ColorSpaceConvert_YV24_SSE proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV24_SSE proc public frame

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
	mov rdi,dst_u
	mov r13,r9
	mov r14,dst_v				;rsi=src_y r11=src_u r12=src_v rdi=dst_u r13=dst_y r14=dst_v
	mov r8d,w
	shr r8d,1
	mov r10,lookup	
	mov r9,2
	
	pxor xmm4,xmm4						
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
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,4

	movzx ebx,byte ptr[rsi+1]
	add rsi,r9
	movzx ecx,byte ptr[r11+1]
	add r11,r9
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	add r12,r9
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,5
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4		
	
	pextrw eax,xmm0,0
	mov word ptr[r13],ax
	add r13,r9
	pextrw eax,xmm0,1
	mov word ptr[rdi],ax
	add rdi,r9
	pextrw eax,xmm0,2
	mov word ptr[r14],ax
	add r14,r9
	
	dec r15d
	jnz Boucle1_2
	
	mov eax,w
	and eax,1
	jz Suite1_2
	
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
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov byte ptr[r13],al
	inc r13
	mov byte ptr[rdi],ah
	inc rdi
	pextrw eax,xmm0,1
	mov byte ptr[r14],al
	inc r14	
	
Suite1_2:	
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add r13,dst_modulo_y
	add rdi,dst_modulo_u
	add r14,dst_modulo_v
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

JPSDR_ColorSpaceConvert_YV24_SSE endp


;JPSDR_ColorSpaceConvert_YV24_AVX proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV24_AVX proc public frame

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
	mov rdi,dst_u
	mov r13,r9
	mov r14,dst_v				;rsi=src_y r11=src_u r12=src_v rdi=dst_u r13=dst_y r14=dst_v
	mov r8d,w
	shr r8d,1
	mov r10,lookup	
	mov r9,2
	
	vpxor xmm4,xmm4,xmm4						
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
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx

Boucle0_2_AVX:
	mov r15d,r8d
Boucle1_2_AVX:
	movzx ebx,byte ptr[rsi]
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,4

	movzx ebx,byte ptr[rsi+1]
	add rsi,r9
	movzx ecx,byte ptr[r11+1]
	add r11,r9
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	add r12,r9
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,5
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4		
	
	vpextrw eax,xmm0,0
	mov word ptr[r13],ax
	add r13,r9
	vpextrw eax,xmm0,1
	mov word ptr[rdi],ax
	add rdi,r9
	vpextrw eax,xmm0,2
	mov word ptr[r14],ax
	add r14,r9
	
	dec r15d
	jnz Boucle1_2_AVX
	
	mov eax,w
	and eax,1
	jz Suite1_2_AVX
	
	movzx ebx,byte ptr[rsi]
	inc rsi
	movzx ecx,byte ptr[r11]
	inc r11
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y
	inc r12
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,2
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov byte ptr[r13],al
	inc r13
	mov byte ptr[rdi],ah
	inc rdi
	vpextrw eax,xmm0,1
	mov byte ptr[r14],al
	inc r14	
	
Suite1_2_AVX:	
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add r13,dst_modulo_y
	add rdi,dst_modulo_u
	add r14,dst_modulo_v
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

JPSDR_ColorSpaceConvert_YV24_AVX endp


;JPSDR_ColorSpaceConvert_YUY2_SSE proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_YUY2_SSE proc public frame

h equ dword ptr[rbp+40]
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
	mov r13d,r8d
	mov h,r9d

	dec r8d
	shr r8d,1					; r8d=w0
	
	pxor xmm4,xmm4	
	pxor xmm0,xmm0
	xor rax,rax	
	
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
	
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r9,r9

Boucle0_4:
	movzx ecx,byte ptr[rsi+1]
	movzx edx,byte ptr[rsi+3]

	or r8d,r8d
	jz Suite0_4
	mov r15d,r8d
Boucle1_4:
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[rsi+5]
	movzx r9d,byte ptr[rsi+7]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

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
	pinsrw xmm0,eax,3
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+2]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2

	mov edx,r9d
	mov ecx,r14d
	
	movzx ebx,byte ptr[rsi+4]
	movzx r14d,byte ptr[rsi+9]
	movzx r9d,byte ptr[rsi+11] ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

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
	pinsrw xmm0,eax,7
	
	add edx,r9d
	add ecx,r14d
	shr edx,1
	shr ecx,1
	
	movzx ebx,byte ptr[rsi+6]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,6
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[rdi],xmm0
	
	add rdi,8	

	mov edx,r9d
	mov ecx,r14d
	
	dec r15d
	jnz Boucle1_4
	
Suite0_4:	
	mov eax,r13d
	dec eax
	and eax,1
	jz Suite1_4
	
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[rsi+5]
	movzx r9d,byte ptr[rsi+7]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

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
	pinsrw xmm0,eax,3
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+2]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2

	movzx ebx,byte ptr[rsi+4]
	mov edx,r9d
	mov ecx,r14d				; edx=V ecx=U ebx=Y1
	
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
	pinsrw xmm0,eax,7
	
	movzx ebx,byte ptr[rsi+6]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,6
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[rdi],xmm0
	
	add rdi,8	

	jmp Suite2_4
	
Suite1_4:	
	movzx ebx,byte ptr[rsi]

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
	pinsrw xmm0,eax,3
	
	movzx ebx,byte ptr[rsi+2]
	add rsi,4
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	movd dword ptr[rdi],xmm0
	
	add rdi,4	
	
Suite2_4:	
	add rsi,r11
	add rdi,r12
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

JPSDR_ColorSpaceConvert_YUY2_SSE endp


;JPSDR_ColorSpaceConvert_YUY2_AVX proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_YUY2_AVX proc public frame

h equ dword ptr[rbp+40]
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
	mov r13d,r8d
	mov h,r9d

	dec r8d
	shr r8d,1					; r8d=w0
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm0,xmm0,xmm0
	xor rax,rax	

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
	
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r9,r9

Boucle0_4_AVX:
	movzx ecx,byte ptr[rsi+1]
	movzx edx,byte ptr[rsi+3]

	or r8d,r8d
	jz Suite0_4_AVX
	
	mov r15d,r8d
Boucle1_4_AVX:
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[rsi+5]
	movzx r9d,byte ptr[rsi+7]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,3
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+2]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2

	mov edx,r9d
	mov ecx,r14d
	
	movzx ebx,byte ptr[rsi+4]
	movzx r14d,byte ptr[rsi+9]
	movzx r9d,byte ptr[rsi+11] ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,7
	
	add edx,r9d
	add ecx,r14d
	shr edx,1
	shr ecx,1
	
	movzx ebx,byte ptr[rsi+6]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,6
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovq qword ptr[rdi],xmm0
	
	add rdi,8	

	mov edx,r9d
	mov ecx,r14d
	
	dec r15d
	jnz Boucle1_4_AVX
	
Suite0_4_AVX:	
	mov eax,r13d
	dec eax
	and eax,1
	jz Suite1_4_AVX
	
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[rsi+5]
	movzx r9d,byte ptr[rsi+7]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,3
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+2]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2

	movzx ebx,byte ptr[rsi+4]
	mov edx,r9d
	mov ecx,r14d				; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,7
	
	movzx ebx,byte ptr[rsi+6]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,6
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovq qword ptr[rdi],xmm0
	
	add rdi,8	

	jmp Suite2_4_AVX
	
Suite1_4_AVX:	
	movzx ebx,byte ptr[rsi]

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,3
	
	movzx ebx,byte ptr[rsi+2]
	add rsi,4
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vmovd dword ptr[rdi],xmm0
	
	add rdi,4	
	
Suite2_4_AVX:	
	add rsi,r11
	add rdi,r12
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

JPSDR_ColorSpaceConvert_YUY2_AVX endp



;JPSDR_ColorSpaceConvert_UYVY_SSE proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_UYVY_SSE proc public frame

h equ dword ptr[rbp+40]
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
	mov r13d,r8d
	mov h,r9d

	dec r8d
	shr r8d,1					; r8d=w0
	
	pxor xmm4,xmm4	
	pxor xmm0,xmm0
	xor rax,rax	
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,7
	movzx eax,offset_U
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,4
	movzx eax,offset_V
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,6
	movzx eax,Min_Y
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,4
	movzx eax,Max_U
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,4
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
	xor r9,r9

Boucle0_6:
	movzx ecx,byte ptr[rsi]
	movzx edx,byte ptr[rsi+2]

	or r8d,r8d
	jz Suite0_6
	mov r15d,r8d
Boucle1_6:
	movzx ebx,byte ptr[rsi+1]
	movzx r14d,byte ptr[rsi+4]
	movzx r9d,byte ptr[rsi+6]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+3]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3

	mov edx,r9d
	mov ecx,r14d
	
	movzx ebx,byte ptr[rsi+5]
	movzx r14d,byte ptr[rsi+8]
	movzx r9d,byte ptr[rsi+10] ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	add edx,r9d
	add ecx,r14d
	shr edx,1
	shr ecx,1
	
	movzx ebx,byte ptr[rsi+7]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[rdi],xmm0
	
	add rdi,8	

	mov edx,r9d
	mov ecx,r14d
	
	dec r15d
	jnz Boucle1_6
	
Suite0_6:	
	mov eax,r13d
	dec eax
	and eax,1
	jz Suite1_6
	
	movzx ebx,byte ptr[rsi+1]
	movzx r14d,byte ptr[rsi+4]
	movzx r9d,byte ptr[rsi+6]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+3]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3

	movzx ebx,byte ptr[rsi+5]
	mov edx,r9d
	mov ecx,r14d					; edx=V ecx=U ebx=Y1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx ebx,byte ptr[rsi+7]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[rdi],xmm0
	
	add rdi,8	

	jmp Suite2_6
	
Suite1_6:	
	movzx ebx,byte ptr[rsi+1]

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,2
	
	movzx ebx,byte ptr[rsi+3]
	add rsi,4
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	movd dword ptr[rdi],xmm0
	
	add rdi,4	
	
Suite2_6:	
	add rsi,r11
	add rdi,r12
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

JPSDR_ColorSpaceConvert_UYVY_SSE endp


;JPSDR_ColorSpaceConvert_UYVY_AVX proc src:dword,dst:dword,w:dword,h:dword,offset_Y:word,offset_U:word,offset_V:word,lookup:dword,src_modulo:dword,
;	dst_modulo:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_ColorSpaceConvert_UYVY_AVX proc public frame

h equ dword ptr[rbp+40]
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
	mov r13d,r8d
	mov h,r9d

	dec r8d
	shr r8d,1					; r8d=w0
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm0,xmm0,xmm0
	xor rax,rax	
	
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,3
	vpinsrw xmm1,xmm1,eax,5
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,4
	movzx eax,offset_V
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,6
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,3
	vpinsrw xmm2,xmm2,eax,5
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,3
	vpinsrw xmm3,xmm3,eax,5
	vpinsrw xmm3,xmm3,eax,7
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,4
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,4
	movzx eax,Min_V
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,6
	movzx eax,Max_V
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,6
	
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r9,r9

Boucle0_6_AVX:
	movzx ecx,byte ptr[rsi]
	movzx edx,byte ptr[rsi+2]

	or r8d,r8d
	jz Suite0_6_AVX
	
	mov r15d,r8d
Boucle1_6_AVX:
	movzx ebx,byte ptr[rsi+1]
	movzx r14d,byte ptr[rsi+4]
	movzx r9d,byte ptr[rsi+6]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,2
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+3]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3

	mov edx,r9d
	mov ecx,r14d
	
	movzx ebx,byte ptr[rsi+5]
	movzx r14d,byte ptr[rsi+8]
	movzx r9d,byte ptr[rsi+10] ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	add edx,r9d
	add ecx,r14d
	shr edx,1
	shr ecx,1
	
	movzx ebx,byte ptr[rsi+7]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,7
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovq qword ptr[rdi],xmm0
	
	add rdi,8	

	mov edx,r9d
	mov ecx,r14d
	
	dec r15d
	jnz Boucle1_6_AVX
	
Suite0_6_AVX:	
	mov eax,r13d
	dec eax
	and eax,1
	jz Suite1_6_AVX
	
	movzx ebx,byte ptr[rsi+1]
	movzx r14d,byte ptr[rsi+4]
	movzx r9d,byte ptr[rsi+6]  ; edx=V ecx=U ebx=Y1 r14=U2 r9=V2

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,2
	
	add edx,r9d
	add ecx,r14d
	movzx ebx,byte ptr[rsi+3]
	shr edx,1
	shr ecx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3

	movzx ebx,byte ptr[rsi+5]
	mov edx,r9d
	mov ecx,r14d					; edx=V ecx=U ebx=Y1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx ebx,byte ptr[rsi+7]
	add rsi,8
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,7
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovq qword ptr[rdi],xmm0
	
	add rdi,8	

	jmp Suite2_6_AVX
	
Suite1_6_AVX:	
	movzx ebx,byte ptr[rsi+1]

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,2
	
	movzx ebx,byte ptr[rsi+3]
	add rsi,4
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vmovd dword ptr[rdi],xmm0
	
	add rdi,4	
	
Suite2_6_AVX:	
	add rsi,r11
	add rdi,r12
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

JPSDR_ColorSpaceConvert_UYVY_AVX endp


;JPSDR_ColorSpaceConvert_YV16_SSE proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV16_SSE proc public frame

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
; Local data
w0 equ dword ptr [rbp+40]

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
	mov rdi,dst_v
	mov r13,dst_u
	mov r14,r9				;rsi=src_y r11=src_u r12=src_v r14=dst_y r13=dst_u rdi=dst_v
	mov r10,lookup	
	
	mov r9d,w
	dec r9d
	shr r9d,1
	mov w0,r9d
	
	xor rax,rax
	pxor xmm4,xmm4				
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
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r15,r15
	xor r9,r9
		
Boucle0_8:
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12]

	mov r8d,w0
	or r8d,r8d
	jz Suite0_8
Boucle1_8:
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11+1]
	movzx r9d,byte ptr[r12+1] ; edx=V1 ecx=U1 ebx=Y1 r15=U2 r9=V2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	add ecx,r15d
	add edx,r9d
	movzx ebx,byte ptr[rsi+1] ; ebx=Y2
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1

	mov ecx,r15d
	mov edx,r9d
	movzx ebx,byte ptr[rsi+2]
	movzx r15d,byte ptr[r11+2]
	movzx r9d,byte ptr[r12+2] ; edx=V1 ecx=U1 ebx=Y1 r15=U2 r9=V2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,7
	
	add ecx,r15d
	add edx,r9d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	pextrw eax,xmm0,2
	add rsi,4
	mov word ptr[r13],ax
	add r11,2
	pextrw eax,xmm0,3
	add r12,2
	mov word ptr[rdi],ax
	add r13,2
	movd dword ptr[r14],xmm0
	
	mov ecx,r15d
	mov edx,r9d
	
	add rdi,2
	add r14,4
		
	dec r8d
	jnz Boucle1_8
	
Suite0_8:	
	mov eax,w
	dec eax
	and eax,1
	jz Suite1_8
	
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11+1]
	movzx r9d,byte ptr[r12+1] ; edx=V1 ecx=U1 ebx=Y1 r15=U2 r9=V2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	add ecx,r15d
	add edx,r9d
	movzx ebx,byte ptr[rsi+1] ; ebx=Y2
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1

	mov ecx,r15d
	mov edx,r9d
	movzx ebx,byte ptr[rsi+2] ; edx=V1 ecx=U1 ebx=Y1
	add r11,2
	add r12,2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	movzx ebx,byte ptr[rsi+3]
	pinsrw xmm0,eax,7

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	add rsi,4
	pinsrw xmm0,eax,3
		
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	pextrw eax,xmm0,2
	mov word ptr[r13],ax
	add r13,2
	pextrw eax,xmm0,3
	mov word ptr[rdi],ax
	add rdi,2
	movd dword ptr[r14],xmm0
	add r14,4	
	
	jmp Suite2_8
	
Suite1_8:	
	movzx ebx,byte ptr[rsi] ; edx=V1 ecx=U1 ebx=Y1
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm0,eax,6
	
	movzx ebx,byte ptr[rsi+1] ; ebx=Y2
	inc r11
	inc r12
	add rsi,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1	
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[r14],ax
	add r14,2
	pextrw eax,xmm0,2
	mov byte ptr[r13],al
	pextrw eax,xmm0,3
	inc r13
	mov byte ptr[rdi],al
	inc rdi	
	
Suite2_8:
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add r14,dst_modulo_y
	add r13,dst_modulo_u
	add rdi,dst_modulo_v
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

JPSDR_ColorSpaceConvert_YV16_SSE endp


;JPSDR_ColorSpaceConvert_YV16_AVX proc src_y:dword,src_u:dword,src_v:dword,dst_y:dword,dst_u:dword,dst_v:dword,w:dword,h:dword,offset_Y:word,
;	offset_U:word,offset_V:word,lookup:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_y:dword,dst_modulo_u:dword,
;	dst_modulo_v:dword,Min_Y:word,Max_Y:word,Min_U:word,Max_U:word,Min_V:word,Max_V:word
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst_y = r9

JPSDR_ColorSpaceConvert_YV16_AVX proc public frame

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
; Local data
w0 equ dword ptr [rbp+40]

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
	mov rdi,dst_v
	mov r13,dst_u
	mov r14,r9				;rsi=src_y r11=src_u r12=src_v r14=dst_y r13=dst_u rdi=dst_v
	mov r10,lookup	
	
	mov r9d,w
	dec r9d
	shr r9d,1
	mov w0,r9d
	
	xor rax,rax
	vpxor xmm4,xmm4,xmm4
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

	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r15,r15
	xor r9,r9
		
Boucle0_8_AVX:
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12]

	mov r8d,w0
	or r8d,r8d
	jz Suite0_8_AVX
	
Boucle1_8_AVX:
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11+1]
	movzx r9d,byte ptr[r12+1] ; edx=V1 ecx=U1 ebx=Y1 r15=U2 r9=V2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	add ecx,r15d
	add edx,r9d
	movzx ebx,byte ptr[rsi+1] ; ebx=Y2
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1

	mov ecx,r15d
	mov edx,r9d
	movzx ebx,byte ptr[rsi+2]
	movzx r15d,byte ptr[r11+2]
	movzx r9d,byte ptr[r12+2] ; edx=V1 ecx=U1 ebx=Y1 r15=U2 r9=V2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,7
	
	add ecx,r15d
	add edx,r9d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vpextrw eax,xmm0,2
	add rsi,4
	mov word ptr[r13],ax
	add r11,2
	vpextrw eax,xmm0,3
	add r12,2
	mov word ptr[rdi],ax
	add r13,2
	vmovd dword ptr[r14],xmm0
	
	mov ecx,r15d
	mov edx,r9d
	
	add rdi,2
	add r14,4
		
	dec r8d
	jnz Boucle1_8_AVX
	
Suite0_8_AVX:	
	mov eax,w
	dec eax
	and eax,1
	jz Suite1_8_AVX
	
	movzx ebx,byte ptr[rsi]
	movzx r15d,byte ptr[r11+1]
	movzx r9d,byte ptr[r12+1] ; edx=V1 ecx=U1 ebx=Y1 r15=U2 r9=V2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	add ecx,r15d
	add edx,r9d
	movzx ebx,byte ptr[rsi+1] ; ebx=Y2
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1

	mov ecx,r15d
	mov edx,r9d
	movzx ebx,byte ptr[rsi+2] ; edx=V1 ecx=U1 ebx=Y1
	add r11,2
	add r12,2
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	movzx ebx,byte ptr[rsi+3]
	vpinsrw xmm0,xmm0,eax,7

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	add rsi,4
	vpinsrw xmm0,xmm0,eax,3
		
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vpextrw eax,xmm0,2
	mov word ptr[r13],ax
	add r13,2
	vpextrw eax,xmm0,3
	mov word ptr[rdi],ax
	add rdi,2
	vmovd dword ptr[r14],xmm0
	add r14,4	
	
	jmp Suite2_8_AVX
	
Suite1_8_AVX:	
	movzx ebx,byte ptr[rsi] ; edx=V1 ecx=U1 ebx=Y1
			
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm0,xmm0,eax,6
	
	movzx ebx,byte ptr[rsi+1] ; ebx=Y2
	inc r11
	inc r12
	add rsi,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1	
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[r14],ax
	add r14,2
	vpextrw eax,xmm0,2
	mov byte ptr[r13],al
	vpextrw eax,xmm0,3
	inc r13
	mov byte ptr[rdi],al
	inc rdi	
	
Suite2_8_AVX:
	add rsi,src_modulo_y
	add r11,src_modulo_u
	add r12,src_modulo_v
	add r14,dst_modulo_y
	add r13,dst_modulo_u
	add rdi,dst_modulo_v
	dec h
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

JPSDR_ColorSpaceConvert_YV16_AVX endp


;JPSDR_ColorSpaceConvert_YV12_SSE2_1 proc src_Y:dword,src_U:dword,src_V:dword,dst_Y:dword,lookup:dword,w:dword,offset_Y:word,Min_Y:word,Max_Y:word
; src_Y = rcx
; src_U = rdx
; src_V = r8
; dst_Y = r9

JPSDR_ColorSpaceConvert_YV12_SSE2_1 proc public frame	

lookup equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
Min_Y equ word ptr[rbp+72]
Max_Y equ word ptr[rbp+80]
	
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
	
	mov rsi,rcx				; rsi=src_Y
	mov r11,rdx				; r11=src_U
	mov r12,r8				; r12=src_V
	mov rdi,r9				; rdi=dst_Y
	mov r10,lookup
	mov r8,8
	mov r13,4
	
	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	xor rax,rax
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r15,r15
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12]
	
	mov eax,w
	dec eax
	shr eax,2
	jz Suite0_9
	mov r9d,eax
	
Boucle0_9:	
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[r11+1]
	movzx r15d,byte ptr[r12+1]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	movzx r14d,byte ptr[r11+2]
	movzx r15d,byte ptr[r12+2]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+4]
	movzx r14d,byte ptr[r11+3]
	movzx r15d,byte ptr[r12+3]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+5]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,5
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+6]
	movzx r14d,byte ptr[r11+4]
	movzx r15d,byte ptr[r12+4]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,6

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+7]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[rdi],xmm0
	
	mov ecx,r14d
	mov edx,r15d
	
	add rsi,r8
	add rdi,r8
	add r11,r13
	add r12,r13
		
	dec r9d
	jnz Boucle0_9
	
Suite0_9:
	mov eax,w
	dec eax
	and eax,3
	jz Suite2_9
	and eax,2
	jz suite1_9
	
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[r11+1]
	movzx r15d,byte ptr[r12+1]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	movzx r14d,byte ptr[r11+2]
	movzx r15d,byte ptr[r12+2]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[rdi],xmm0	
	
	mov ecx,r14d
	mov edx,r15d
	
	add rsi,r13
	add rdi,r13
	add r11,2
	add r12,2
	
	mov eax,w
	dec eax	
	and eax,1
	jz Suite2_9
	
suite1_9:
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[r11+1]
	movzx r15d,byte ptr[r12+1]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	movzx ebx,byte ptr[rsi+2]
	mov ecx,r14d
	mov edx,r15d
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+3]
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[rdi],xmm0		
	
	jmp suite3_9

suite2_9:	
	movzx ebx,byte ptr[rsi]
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+1]
	pinsrw xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
suite3_9:	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_ColorSpaceConvert_YV12_SSE2_1 endp	


;JPSDR_ColorSpaceConvert_YV12_AVX_1 proc src_Y:dword,src_U:dword,src_V:dword,dst_Y:dword,lookup:dword,w:dword,offset_Y:word,Min_Y:word,Max_Y:word
; src_Y = rcx
; src_U = rdx
; src_V = r8
; dst_Y = r9

JPSDR_ColorSpaceConvert_YV12_AVX_1 proc public frame	

lookup equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
offset_Y equ word ptr[rbp+64]
Min_Y equ word ptr[rbp+72]
Max_Y equ word ptr[rbp+80]
	
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
	
	mov rsi,rcx				; rsi=src_Y
	mov r11,rdx				; r11=src_U
	mov r12,r8				; r12=src_V
	mov rdi,r9				; rdi=dst_Y
	mov r10,lookup
	mov r8,8
	mov r13,4
	
	vpxor xmm4,xmm4,xmm4
	vpxor xmm0,xmm0,xmm0
	
	xor rax,rax
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r15,r15
	
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5
	vpinsrw xmm1,xmm1,eax,6
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	vpinsrw xmm2,xmm2,eax,6
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5
	vpinsrw xmm3,xmm3,eax,6
	vpinsrw xmm3,xmm3,eax,7	
	
	movzx ecx,byte ptr[r11]
	movzx edx,byte ptr[r12]
	
	mov eax,w
	dec eax
	shr eax,2
	jz Suite0_9_AVX
	mov r9d,eax
	
Boucle0_9_AVX:	
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[r11+1]
	movzx r15d,byte ptr[r12+1]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	movzx r14d,byte ptr[r11+2]
	movzx r15d,byte ptr[r12+2]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+4]
	movzx r14d,byte ptr[r11+3]
	movzx r15d,byte ptr[r12+3]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,4

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+5]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,5
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+6]
	movzx r14d,byte ptr[r11+4]
	movzx r15d,byte ptr[r12+4]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,6

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+7]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,7
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovq qword ptr[rdi],xmm0
	
	mov ecx,r14d
	mov edx,r15d
	
	add rsi,r8
	add rdi,r8
	add r11,r13
	add r12,r13
		
	dec r9d
	jnz Boucle0_9_AVX
	
Suite0_9_AVX:
	mov eax,w
	dec eax
	and eax,3
	jz Suite2_9_AVX
	and eax,2
	jz suite1_9_AVX
	
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[r11+1]
	movzx r15d,byte ptr[r12+1]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	movzx r14d,byte ptr[r11+2]
	movzx r15d,byte ptr[r12+2]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2

	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovd dword ptr[rdi],xmm0	
	
	mov ecx,r14d
	mov edx,r15d
	
	add rsi,r13
	add rdi,r13
	add r11,2
	add r12,2
	
	mov eax,w
	dec eax	
	and eax,1
	jz Suite2_9_AVX
	
suite1_9_AVX:
	movzx ebx,byte ptr[rsi]
	movzx r14d,byte ptr[r11+1]
	movzx r15d,byte ptr[r12+1]	; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	movzx ebx,byte ptr[rsi+2]
	mov ecx,r14d
	mov edx,r15d
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+3]
	vpinsrw xmm0,xmm0,eax,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovd dword ptr[rdi],xmm0		
	
	jmp suite3_9_AVX

suite2_9_AVX:	
	movzx ebx,byte ptr[rsi]
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+1]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
suite3_9_AVX:	
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_ColorSpaceConvert_YV12_AVX_1 endp	


;JPSDR_ColorSpaceConvert_YV12_SSE2_2 proc src_Y:dword,src_U1:dword,src_U2:dword,src_V1:dword,src_V2:dword,dst_Y:dword,lookup:dword,w:dword,
;	offset_Y:word,Min_Y:word,Max_Y:word
; src_Y = rcx
; src_U1 = rdx
; src_U2 = r8
; src_V1 = r9
	
JPSDR_ColorSpaceConvert_YV12_SSE2_2 proc public frame	

src_V2 equ qword ptr[rbp+48]
dst_Y equ qword ptr[rbp+56]
lookup equ qword ptr[rbp+64]
w equ dword ptr[rbp+72]
offset_Y equ word ptr[rbp+80]
Min_Y equ word ptr[rbp+88]
Max_Y equ word ptr[rbp+96]
	
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
	sub rsp,48
	.allocstack 48
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	movdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	.endprolog
	
	mov rsi,rcx			; rsi=src_Y
	mov rdi,dst_Y
	mov r10,lookup
	mov r11,rdx			; r11=src_U1
	mov r12,src_V2
	
	pxor xmm7,xmm7
	pxor xmm5,xmm5
	pxor xmm4,xmm4
	pxor xmm0,xmm0
	
	movzx eax,offset_Y
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_Y
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7
	movzx eax,Max_Y
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	
	movdqa xmm6,XMMWORD ptr uw_2
	movdqa xmm8,XMMWORD ptr uw_3
	
	xor rax,rax
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r15,r15
	
	mov r13d,w
	dec r13d
	shr r13d,2
	jz Suite0_10
	
Boucle0_10:	
	movd xmm0,dword ptr[r11]
	movd xmm4,dword ptr[r8]
	punpcklbw xmm0,xmm7
	movzx eax,byte ptr[r11+4]
	punpcklbw xmm4,xmm7
	pinsrw xmm0,eax,4
	movzx eax,byte ptr[r8+4]
	pmullw xmm0,xmm8
	pinsrw xmm4,eax,4
	paddsw xmm0,xmm6
	paddsw xmm4,xmm0
	psraw xmm4,2
	
	movd xmm0,dword ptr[r9]
	movd xmm5,dword ptr[r12]
	punpcklbw xmm0,xmm7
	movzx eax,byte ptr[r9+4]
	punpcklbw xmm5,xmm7
	pinsrw xmm0,eax,4
	movzx eax,byte ptr[r12+4]
	pmullw xmm0,xmm8
	pinsrw xmm5,eax,4
	paddsw xmm0,xmm6
	paddsw xmm5,xmm0
	psraw xmm5,2
	
	pextrw ecx,xmm4,0
	pextrw r14d,xmm4,1
	movzx ebx,byte ptr[rsi]
	pextrw edx,xmm5,0
	pextrw r15d,xmm5,1					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	pextrw r14d,xmm4,2
	pextrw r15d,xmm5,2			; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3

	mov ecx,r14d
	mov edx,r15d	
	movzx ebx,byte ptr[rsi+4]
	pextrw r14d,xmm4,3
	pextrw r15d,xmm5,3			; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,4
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+5]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,5

	movzx ebx,byte ptr[rsi+6]
	mov ecx,r14d
	mov edx,r15d					; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,6
	
	pextrw eax,xmm4,4
	add ecx,eax
	pextrw eax,xmm5,4
	add edx,eax
	movzx ebx,byte ptr[rsi+7]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,7
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movq qword ptr[rdi],xmm0
	
	add r11,4
	add r9,4
	add r8,4
	add r12,4
	add rdi,8
	add rsi,8
	
	dec r13d
	jnz Boucle0_10
	
Suite0_10:
	movd xmm0,dword ptr[r11]
	movd xmm4,dword ptr[r8]
	punpcklbw xmm0,xmm7
	punpcklbw xmm4,xmm7
	pmullw xmm0,xmm8
	paddsw xmm0,xmm6
	paddsw xmm4,xmm0
	psraw xmm4,2
	
	movd xmm0,dword ptr[r9]
	movd xmm5,dword ptr[r12]
	punpcklbw xmm0,xmm7
	punpcklbw xmm5,xmm7
	pmullw xmm0,xmm8
	paddsw xmm0,xmm6
	paddsw xmm5,xmm0
	psraw xmm5,2
	
	pextrw ecx,xmm4,0
	pextrw edx,xmm5,0
	
	mov eax,w
	dec eax
	and eax,3
	jz Suite2_10
	and eax,2
	jz suite1_10
	
	movzx ebx,byte ptr[rsi]
	pextrw r14d,xmm4,1
	pextrw r15d,xmm5,1					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	pextrw r14d,xmm4,2
	pextrw r15d,xmm5,2			; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,2
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[rdi],xmm0	

	mov ecx,r14d
	mov edx,r15d
	add rsi,4
	add rdi,4
	
	mov eax,w
	dec eax	
	and eax,1
	jz Suite2_10
	
	movzx ebx,byte ptr[rsi]
	pextrw r14d,xmm4,3
	pextrw r15d,xmm5,3					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	movzx ebx,byte ptr[rsi+2]
	mov ecx,r14d
	mov edx,r15d					; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+3]
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[rdi],xmm0		
	
	jmp suite3_10
			
suite1_10:
	movzx ebx,byte ptr[rsi]
	pextrw r14d,xmm4,1
	pextrw r15d,xmm5,1					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	movzx ebx,byte ptr[rsi+2]
	mov ecx,r14d
	mov edx,r15d					; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+3]
	pinsrw xmm0,eax,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,3
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4

	movd dword ptr[rdi],xmm0		
	
	jmp suite3_10

suite2_10:	
	movzx ebx,byte ptr[rsi]
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+1]
	pinsrw xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	pinsrw xmm0,eax,1
	
	paddsw xmm0,xmm1
	psraw xmm0,6
	pmaxsw xmm0,xmm2
	pminsw xmm0,xmm3
	packuswb xmm0,xmm4
	
	pextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
suite3_10:	
	movdqu xmm8,XMMWORD ptr[rsp+32]
	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]
	add rsp,48

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_ColorSpaceConvert_YV12_SSE2_2 endp	


;JPSDR_ColorSpaceConvert_YV12_AVX_2 proc src_Y:dword,src_U1:dword,src_U2:dword,src_V1:dword,src_V2:dword,dst_Y:dword,lookup:dword,w:dword,
;	offset_Y:word,Min_Y:word,Max_Y:word
; src_Y = rcx
; src_U1 = rdx
; src_U2 = r8
; src_V1 = r9
	
JPSDR_ColorSpaceConvert_YV12_AVX_2 proc public frame	

src_V2 equ qword ptr[rbp+48]
dst_Y equ qword ptr[rbp+56]
lookup equ qword ptr[rbp+64]
w equ dword ptr[rbp+72]
offset_Y equ word ptr[rbp+80]
Min_Y equ word ptr[rbp+88]
Max_Y equ word ptr[rbp+96]
	
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
	sub rsp,48
	.allocstack 48
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	vmovdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	.endprolog
	
	mov rsi,rcx			; rsi=src_Y
	mov rdi,dst_Y
	mov r10,lookup
	mov r11,rdx			; r11=src_U1
	mov r12,src_V2
	
	vpxor xmm7,xmm7,xmm7
	vpxor xmm5,xmm5,xmm5
	vpxor xmm4,xmm4,xmm4
	vpxor xmm0,xmm0,xmm0
	
	movzx eax,offset_Y
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5
	vpinsrw xmm1,xmm1,eax,6
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_Y
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	vpinsrw xmm2,xmm2,eax,6
	vpinsrw xmm2,xmm2,eax,7
	movzx eax,Max_Y
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5
	vpinsrw xmm3,xmm3,eax,6
	vpinsrw xmm3,xmm3,eax,7
	
	vmovdqa xmm6,XMMWORD ptr uw_2
	vmovdqa xmm8,XMMWORD ptr uw_3
	
	xor rax,rax
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	xor r14,r14
	xor r15,r15
	
	mov r13d,w
	dec r13d
	shr r13d,2
	jz Suite0_10_AVX
	
Boucle0_10_AVX:	
	vmovd xmm0,dword ptr[r11]
	vmovd xmm4,dword ptr[r8]
	vpunpcklbw xmm0,xmm0,xmm7
	movzx eax,byte ptr[r11+4]
	vpunpcklbw xmm4,xmm4,xmm7
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,byte ptr[r8+4]
	vpmullw xmm0,xmm0,xmm8
	vpinsrw xmm4,xmm4,eax,4
	vpaddsw xmm0,xmm0,xmm6
	vpaddsw xmm4,xmm4,xmm0
	vpsraw xmm4,xmm4,2
	
	vmovd xmm0,dword ptr[r9]
	vmovd xmm5,dword ptr[r12]
	vpunpcklbw xmm0,xmm0,xmm7
	movzx eax,byte ptr[r9+4]
	vpunpcklbw xmm5,xmm5,xmm7
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,byte ptr[r12+4]
	vpmullw xmm0,xmm0,xmm8
	vpinsrw xmm5,xmm5,eax,4
	vpaddsw xmm0,xmm0,xmm6
	vpaddsw xmm5,xmm5,xmm0
	vpsraw xmm5,xmm5,2
	
	vpextrw ecx,xmm4,0
	vpextrw r14d,xmm4,1
	movzx ebx,byte ptr[rsi]
	vpextrw edx,xmm5,0
	vpextrw r15d,xmm5,1					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	vpextrw r14d,xmm4,2
	vpextrw r15d,xmm5,2			; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3

	mov ecx,r14d
	mov edx,r15d	
	movzx ebx,byte ptr[rsi+4]
	vpextrw r14d,xmm4,3
	vpextrw r15d,xmm5,3			; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,4
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+5]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,5

	movzx ebx,byte ptr[rsi+6]
	mov ecx,r14d
	mov edx,r15d					; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,6
	
	vpextrw eax,xmm4,4
	add ecx,eax
	vpextrw eax,xmm5,4
	add edx,eax
	movzx ebx,byte ptr[rsi+7]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,7
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovq qword ptr[rdi],xmm0
	
	add r11,4
	add r9,4
	add r8,4
	add r12,4
	add rdi,8
	add rsi,8
	
	dec r13d
	jnz Boucle0_10_AVX
	
Suite0_10_AVX:
	vmovd xmm0,dword ptr[r11]
	vmovd xmm4,dword ptr[r8]
	vpunpcklbw xmm0,xmm0,xmm7
	vpunpcklbw xmm4,xmm4,xmm7
	vpmullw xmm0,xmm0,xmm8
	vpaddsw xmm0,xmm0,xmm6
	vpaddsw xmm4,xmm4,xmm0
	vpsraw xmm4,xmm4,2
	
	vmovd xmm0,dword ptr[r9]
	vmovd xmm5,dword ptr[r12]
	vpunpcklbw xmm0,xmm0,xmm7
	vpunpcklbw xmm5,xmm5,xmm7
	vpmullw xmm0,xmm0,xmm8
	vpaddsw xmm0,xmm0,xmm6
	vpaddsw xmm5,xmm5,xmm0
	vpsraw xmm5,xmm5,2
	
	vpextrw ecx,xmm4,0
	vpextrw edx,xmm5,0
	
	mov eax,w
	dec eax
	and eax,3
	jz Suite2_10_AVX
	and eax,2
	jz suite1_10_AVX
	
	movzx ebx,byte ptr[rsi]
	vpextrw r14d,xmm4,1
	vpextrw r15d,xmm5,1					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	mov ecx,r14d
	mov edx,r15d
	movzx ebx,byte ptr[rsi+2]
	vpextrw r14d,xmm4,2
	vpextrw r15d,xmm5,2			; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,2
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+3]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovd dword ptr[rdi],xmm0	

	mov ecx,r14d
	mov edx,r15d
	add rsi,4
	add rdi,4
	
	mov eax,w
	dec eax	
	and eax,1
	jz Suite2_10_AVX
	
	movzx ebx,byte ptr[rsi]
	vpextrw r14d,xmm4,3
	vpextrw r15d,xmm5,3					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	movzx ebx,byte ptr[rsi+2]
	mov ecx,r14d
	mov edx,r15d					; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+3]
	vpinsrw xmm0,xmm0,eax,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovd dword ptr[rdi],xmm0		
	
	jmp suite3_10_AVX
			
suite1_10_AVX:
	movzx ebx,byte ptr[rsi]
	vpextrw r14d,xmm4,1
	vpextrw r15d,xmm5,1					; edx=V ecx=U ebx=Y1 r15d=V2 r14d=U2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,0
	
	add ecx,r14d
	add edx,r15d
	movzx ebx,byte ptr[rsi+1]
	shr ecx,1
	shr edx,1

	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	movzx ebx,byte ptr[rsi+2]
	mov ecx,r14d
	mov edx,r15d					; edx=V ecx=U ebx=Y1
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+3]
	vpinsrw xmm0,xmm0,eax,2
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,3
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4

	vmovd dword ptr[rdi],xmm0		
	
	jmp suite3_10_AVX

suite2_10_AVX:	
	movzx ebx,byte ptr[rsi]
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	movzx ebx,byte ptr[rsi+1]
	vpinsrw xmm0,xmm0,eax,0
	
	movzx eax,word ptr[r10+2*rbx]
	add ax,word ptr[r10+2*rcx+512]
	add ax,word ptr[r10+2*rdx+1024]
	vpinsrw xmm0,xmm0,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpsraw xmm0,xmm0,6
	vpmaxsw xmm0,xmm0,xmm2
	vpminsw xmm0,xmm0,xmm3
	vpackuswb xmm0,xmm0,xmm4
	
	vpextrw eax,xmm0,0
	mov word ptr[rdi],ax
	
suite3_10_AVX:	
	vmovdqu xmm8,XMMWORD ptr[rsp+32]
	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]
	add rsp,48

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_ColorSpaceConvert_YV12_AVX_2 endp


;JPSDR_ColorSpaceConvert_YV12_SSE2_3 proc src_Y:dword,src_U:dword,src_V:dword,dst_U:dword,dst_V:dword,lookup:dword,w:dword,h:dword,
;	src_pitch_y:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_u:dword,dst_modulo_v:dword,offset_U:word,Min_U:word,
;	Max_U:word,offset_V:word,Min_V:word,Max_V:word

; src_Y = rcx
; src_U = rdx
; src_V = r8
; dst_U = r9
	
JPSDR_ColorSpaceConvert_YV12_SSE2_3 proc public frame	

dst_V equ qword ptr[rbp+48]
lookup equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]
src_pitch_y equ qword ptr[rbp+80]
src_modulo_y equ qword ptr[rbp+88]
src_modulo_u equ qword ptr[rbp+96]
src_modulo_v equ qword ptr[rbp+104]
dst_modulo_u equ qword ptr[rbp+112]
dst_modulo_v equ qword ptr[rbp+120]
offset_U equ word ptr[rbp+128]
Min_U equ word ptr[rbp+136]
Max_U equ word ptr[rbp+144]
offset_V equ word ptr[rbp+152]
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
	sub rsp,80
	.allocstack 80
	movdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	movdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog

	mov rsi,rcx				; rsi=src_y
	mov r11,rdx				; r11=src_u
	mov r12,r8				; r12=src_v
	mov rdi,r9				; rdi=dst_u
	mov r13,dst_V
	
	xor rax,rax
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	
	pxor xmm7,xmm7
	
	movzx eax,offset_U
	pinsrw xmm1,eax,0
	pinsrw xmm1,eax,1
	pinsrw xmm1,eax,2
	pinsrw xmm1,eax,3
	pinsrw xmm1,eax,4
	pinsrw xmm1,eax,5
	pinsrw xmm1,eax,6
	pinsrw xmm1,eax,7
	movzx eax,Min_U
	pinsrw xmm2,eax,0
	pinsrw xmm2,eax,1
	pinsrw xmm2,eax,2
	pinsrw xmm2,eax,3
	pinsrw xmm2,eax,4
	pinsrw xmm2,eax,5
	pinsrw xmm2,eax,6
	pinsrw xmm2,eax,7	
	movzx eax,Max_U
	pinsrw xmm3,eax,0
	pinsrw xmm3,eax,1
	pinsrw xmm3,eax,2
	pinsrw xmm3,eax,3
	pinsrw xmm3,eax,4
	pinsrw xmm3,eax,5
	pinsrw xmm3,eax,6
	pinsrw xmm3,eax,7
	movzx eax,offset_V
	pinsrw xmm4,eax,0
	pinsrw xmm4,eax,1
	pinsrw xmm4,eax,2
	pinsrw xmm4,eax,3	
	pinsrw xmm4,eax,4
	pinsrw xmm4,eax,5
	pinsrw xmm4,eax,6
	pinsrw xmm4,eax,7	
	movzx eax,Min_V
	pinsrw xmm5,eax,0
	pinsrw xmm5,eax,1
	pinsrw xmm5,eax,2
	pinsrw xmm5,eax,3	
	pinsrw xmm5,eax,4
	pinsrw xmm5,eax,5
	pinsrw xmm5,eax,6
	pinsrw xmm5,eax,7	
	movzx eax,Max_V
	pinsrw xmm6,eax,0
	pinsrw xmm6,eax,1
	pinsrw xmm6,eax,2
	pinsrw xmm6,eax,3
	pinsrw xmm6,eax,4
	pinsrw xmm6,eax,5
	pinsrw xmm6,eax,6
	pinsrw xmm6,eax,7
	
	mov r8,8
	mov r15,4
	mov r14,src_pitch_y
	mov r10,lookup
	
Boucle0_11:
	mov eax,w
	shr eax,3
	jz Suite0_11
	mov r9d,eax
	
Boucle1_11:	
	movq xmm8,qword ptr[rsi]
	movq xmm10,qword ptr[rsi+r14]
	add rsi,r8
	pavgb xmm8,xmm10
	punpcklbw xmm8,xmm7
	
	movzx ecx,byte ptr[r11]
	pextrw ebx,xmm8,0
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,0
	
	movzx ecx,byte ptr[r11+1]
	pextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,1
	
	movzx ecx,byte ptr[r11+2]
	pextrw ebx,xmm8,4
	movzx edx,byte ptr[r12+2] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,2
	
	movzx ecx,byte ptr[r11+3]
	pextrw ebx,xmm8,6
	movzx edx,byte ptr[r12+3] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,3
	
	movq xmm8,qword ptr[rsi]
	movq xmm10,qword ptr[rsi+r14]
	add rsi,r8
	pavgb xmm8,xmm10
	punpcklbw xmm8,xmm7
	
	movzx ecx,byte ptr[r11+4]
	pextrw ebx,xmm8,0
	movzx edx,byte ptr[r12+4] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,4
	
	movzx ecx,byte ptr[r11+5]
	pextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+5] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,5
	
	movzx ecx,byte ptr[r11+6]
	pextrw ebx,xmm8,4
	movzx edx,byte ptr[r12+6] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,6
	
	movzx ecx,byte ptr[r11+7]
	pextrw ebx,xmm8,6
	movzx edx,byte ptr[r12+7] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,7
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,7
	
	paddsw xmm0,xmm1
	paddsw xmm9,xmm4
	psraw xmm0,6
	psraw xmm9,6
	pmaxsw xmm0,xmm2
	pmaxsw xmm9,xmm5
	pminsw xmm0,xmm3
	pminsw xmm9,xmm6
	packuswb xmm0,xmm7
	packuswb xmm9,xmm7
	
	movq qword ptr[rdi],xmm0
	movq qword ptr[r13],xmm9
	
	add r11,r8
	add r12,r8
	add rdi,r8
	add r13,r8
	
	dec r9d
	jnz Boucle1_11
	
Suite0_11:
	mov eax,w
	and eax,7
	jz suite3_11
	and eax,r15d
	jz suite1_11
	
	movq xmm8,qword ptr[rsi]
	movq xmm0,qword ptr[rsi+r14]
	add rsi,r8
	pavgb xmm8,xmm0
	punpcklbw xmm8,xmm7
	
	movzx ecx,byte ptr[r11]
	pextrw ebx,xmm8,0
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,0
	
	movzx ecx,byte ptr[r11+1]
	pextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,1
	
	movzx ecx,byte ptr[r11+2]
	pextrw ebx,xmm8,4
	movzx edx,byte ptr[r12+2] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,2
	
	movzx ecx,byte ptr[r11+3]
	pextrw ebx,xmm8,6
	movzx edx,byte ptr[r12+3] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,3

	paddsw xmm0,xmm1
	paddsw xmm9,xmm4
	psraw xmm0,6
	psraw xmm9,6
	pmaxsw xmm0,xmm2
	pmaxsw xmm9,xmm5
	pminsw xmm0,xmm3
	pminsw xmm9,xmm6
	packuswb xmm0,xmm7
	packuswb xmm9,xmm7
	
	movd dword ptr[rdi],xmm0
	movd dword ptr[r13],xmm9
	
	add r11,r15
	add r12,r15
	add rdi,r15
	add r13,r15
	
suite1_11:	
	mov eax,w
	and eax,2
	jz suite2_11
	
	movd xmm8,dword ptr[rsi]
	movd xmm0,dword ptr[rsi+r14]
	add rsi,r15
	pavgb xmm8,xmm0
	punpcklbw xmm8,xmm7
	
	movzx ecx,byte ptr[r11]
	pextrw ebx,xmm8,0
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,0
	
	movzx ecx,byte ptr[r11+1]
	pextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,1
	
	paddsw xmm0,xmm1
	paddsw xmm9,xmm4
	psraw xmm0,6
	psraw xmm9,6
	pmaxsw xmm0,xmm2
	pmaxsw xmm9,xmm5
	pminsw xmm0,xmm3
	pminsw xmm9,xmm6
	packuswb xmm0,xmm7
	packuswb xmm9,xmm7
	
	pextrw eax,xmm0,0
	add r11,2
	mov word ptr[rdi],ax
	add r12,2
	pextrw eax,xmm9,0
	add rdi,2
	mov word ptr[r13],ax
	add r13,2
	
suite2_11:	
	mov eax,w
	and eax,1
	jz suite3_11
	
	movzx eax,byte ptr[rsi]
	movzx ebx,byte ptr[rsi+r14]
	add rsi,2
	add ebx,eax
	shr ebx,1
	
	movzx ecx,byte ptr[r11]
	movzx ebx,byte ptr[r12] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	pinsrw xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	pinsrw xmm9,eax,0
	
	paddsw xmm0,xmm1
	paddsw xmm9,xmm4
	psraw xmm0,6
	psraw xmm9,6
	pmaxsw xmm0,xmm2
	pmaxsw xmm9,xmm5
	pminsw xmm0,xmm3
	pminsw xmm9,xmm6
	packuswb xmm0,xmm7
	packuswb xmm9,xmm7
	
	pextrw eax,xmm0,0
	inc r11
	mov byte ptr[rdi],al
	inc r12
	pextrw eax,xmm9,0
	inc rdi
	mov byte ptr[r13],al
	inc r13
	
suite3_11:		
	add rsi,src_modulo_y
	add rsi,r14
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo_u
	add r13,dst_modulo_v
	
	dec h
	jnz Boucle0_11
	
	movdqu xmm10,XMMWORD ptr[rsp+64]
	movdqu xmm9,XMMWORD ptr[rsp+48]
	movdqu xmm8,XMMWORD ptr[rsp+32]
	movdqu xmm7,XMMWORD ptr[rsp+16]
	movdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_ColorSpaceConvert_YV12_SSE2_3 endp


;JPSDR_ColorSpaceConvert_YV12_AVX_3 proc src_Y:dword,src_U:dword,src_V:dword,dst_U:dword,dst_V:dword,lookup:dword,w:dword,h:dword,
;	src_pitch_y:dword,src_modulo_y:dword,src_modulo_u:dword,src_modulo_v:dword,dst_modulo_u:dword,dst_modulo_v:dword,offset_U:word,Min_U:word,
;	Max_U:word,offset_V:word,Min_V:word,Max_V:word

; src_Y = rcx
; src_U = rdx
; src_V = r8
; dst_U = r9
	
JPSDR_ColorSpaceConvert_YV12_AVX_3 proc public frame	

dst_V equ qword ptr[rbp+48]
lookup equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]
src_pitch_y equ qword ptr[rbp+80]
src_modulo_y equ qword ptr[rbp+88]
src_modulo_u equ qword ptr[rbp+96]
src_modulo_v equ qword ptr[rbp+104]
dst_modulo_u equ qword ptr[rbp+112]
dst_modulo_v equ qword ptr[rbp+120]
offset_U equ word ptr[rbp+128]
Min_U equ word ptr[rbp+136]
Max_U equ word ptr[rbp+144]
offset_V equ word ptr[rbp+152]
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
	sub rsp,80
	.allocstack 80
	vmovdqu XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	vmovdqu XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	vmovdqu XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	vmovdqu XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	vmovdqu XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	.endprolog

	mov rsi,rcx				; rsi=src_y
	mov r11,rdx				; r11=src_u
	mov r12,r8				; r12=src_v
	mov rdi,r9				; rdi=dst_u
	mov r13,dst_V
	
	xor rax,rax
	xor rbx,rbx
	xor rcx,rcx
	xor rdx,rdx
	
	vpxor xmm7,xmm7,xmm7
	
	movzx eax,offset_U
	vpinsrw xmm1,xmm1,eax,0
	vpinsrw xmm1,xmm1,eax,1
	vpinsrw xmm1,xmm1,eax,2
	vpinsrw xmm1,xmm1,eax,3
	vpinsrw xmm1,xmm1,eax,4
	vpinsrw xmm1,xmm1,eax,5
	vpinsrw xmm1,xmm1,eax,6
	vpinsrw xmm1,xmm1,eax,7
	movzx eax,Min_U
	vpinsrw xmm2,xmm2,eax,0
	vpinsrw xmm2,xmm2,eax,1
	vpinsrw xmm2,xmm2,eax,2
	vpinsrw xmm2,xmm2,eax,3
	vpinsrw xmm2,xmm2,eax,4
	vpinsrw xmm2,xmm2,eax,5
	vpinsrw xmm2,xmm2,eax,6
	vpinsrw xmm2,xmm2,eax,7	
	movzx eax,Max_U
	vpinsrw xmm3,xmm3,eax,0
	vpinsrw xmm3,xmm3,eax,1
	vpinsrw xmm3,xmm3,eax,2
	vpinsrw xmm3,xmm3,eax,3
	vpinsrw xmm3,xmm3,eax,4
	vpinsrw xmm3,xmm3,eax,5
	vpinsrw xmm3,xmm3,eax,6
	vpinsrw xmm3,xmm3,eax,7
	movzx eax,offset_V
	vpinsrw xmm4,xmm4,eax,0
	vpinsrw xmm4,xmm4,eax,1
	vpinsrw xmm4,xmm4,eax,2
	vpinsrw xmm4,xmm4,eax,3	
	vpinsrw xmm4,xmm4,eax,4
	vpinsrw xmm4,xmm4,eax,5
	vpinsrw xmm4,xmm4,eax,6
	vpinsrw xmm4,xmm4,eax,7	
	movzx eax,Min_V
	vpinsrw xmm5,xmm5,eax,0
	vpinsrw xmm5,xmm5,eax,1
	vpinsrw xmm5,xmm5,eax,2
	vpinsrw xmm5,xmm5,eax,3	
	vpinsrw xmm5,xmm5,eax,4
	vpinsrw xmm5,xmm5,eax,5
	vpinsrw xmm5,xmm5,eax,6
	vpinsrw xmm5,xmm5,eax,7	
	movzx eax,Max_V
	vpinsrw xmm6,xmm6,eax,0
	vpinsrw xmm6,xmm6,eax,1
	vpinsrw xmm6,xmm6,eax,2
	vpinsrw xmm6,xmm6,eax,3
	vpinsrw xmm6,xmm6,eax,4
	vpinsrw xmm6,xmm6,eax,5
	vpinsrw xmm6,xmm6,eax,6
	vpinsrw xmm6,xmm6,eax,7
	
	mov r8,8
	mov r15,4
	mov r14,src_pitch_y
	mov r10,lookup
	
Boucle0_11_AVX:
	mov eax,w
	shr eax,3
	jz Suite0_11_AVX
	mov r9d,eax
	
Boucle1_11_AVX:	
	vmovq xmm8,qword ptr[rsi]
	vmovq xmm10,qword ptr[rsi+r14]
	add rsi,r8
	vpavgb xmm8,xmm8,xmm10
	vpunpcklbw xmm8,xmm8,xmm7
	
	movzx ecx,byte ptr[r11]
	vpextrw ebx,xmm8,0
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,0
	
	movzx ecx,byte ptr[r11+1]
	vpextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,1
	
	movzx ecx,byte ptr[r11+2]
	vpextrw ebx,xmm8,4
	movzx edx,byte ptr[r12+2] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,2
	
	movzx ecx,byte ptr[r11+3]
	vpextrw ebx,xmm8,6
	movzx edx,byte ptr[r12+3] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,3
	
	vmovq xmm8,qword ptr[rsi]
	vmovq xmm10,qword ptr[rsi+r14]
	add rsi,r8
	vpavgb xmm8,xmm8,xmm10
	vpunpcklbw xmm8,xmm8,xmm7
	
	movzx ecx,byte ptr[r11+4]
	vpextrw ebx,xmm8,0
	movzx edx,byte ptr[r12+4] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,4
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,4
	
	movzx ecx,byte ptr[r11+5]
	vpextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+5] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,5
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,5
	
	movzx ecx,byte ptr[r11+6]
	vpextrw ebx,xmm8,4
	movzx edx,byte ptr[r12+6] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,6
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,6
	
	movzx ecx,byte ptr[r11+7]
	vpextrw ebx,xmm8,6
	movzx edx,byte ptr[r12+7] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,7
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,7
	
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm9,xmm9,xmm4
	vpsraw xmm0,xmm0,6
	vpsraw xmm9,xmm9,6
	vpmaxsw xmm0,xmm0,xmm2
	vpmaxsw xmm9,xmm9,xmm5
	vpminsw xmm0,xmm0,xmm3
	vpminsw xmm9,xmm9,xmm6
	vpackuswb xmm0,xmm0,xmm7
	vpackuswb xmm9,xmm9,xmm7
	
	vmovq qword ptr[rdi],xmm0
	vmovq qword ptr[r13],xmm9
	
	add r11,r8
	add r12,r8
	add rdi,r8
	add r13,r8
	
	dec r9d
	jnz Boucle1_11_AVX
	
Suite0_11_AVX:
	mov eax,w
	and eax,7
	jz suite3_11_AVX
	and eax,r15d
	jz suite1_11_AVX
	
	vmovq xmm8,qword ptr[rsi]
	vmovq xmm0,qword ptr[rsi+r14]
	add rsi,r8
	vpavgb xmm8,xmm8,xmm0
	vpunpcklbw xmm8,xmm8,xmm7
	
	movzx ecx,byte ptr[r11]
	vpextrw ebx,xmm8,0
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,0
	
	movzx ecx,byte ptr[r11+1]
	vpextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,1
	
	movzx ecx,byte ptr[r11+2]
	vpextrw ebx,xmm8,4
	movzx edx,byte ptr[r12+2] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,2
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,2
	
	movzx ecx,byte ptr[r11+3]
	vpextrw ebx,xmm8,6
	movzx edx,byte ptr[r12+3] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,3
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,3

	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm9,xmm9,xmm4
	vpsraw xmm0,xmm0,6
	vpsraw xmm9,xmm9,6
	vpmaxsw xmm0,xmm0,xmm2
	vpmaxsw xmm9,xmm9,xmm5
	vpminsw xmm0,xmm0,xmm3
	vpminsw xmm9,xmm9,xmm6
	vpackuswb xmm0,xmm0,xmm7
	vpackuswb xmm9,xmm9,xmm7
	
	vmovd dword ptr[rdi],xmm0
	vmovd dword ptr[r13],xmm9
	
	add r11,r15
	add r12,r15
	add rdi,r15
	add r13,r15
	
suite1_11_AVX:	
	mov eax,w
	and eax,2
	jz suite2_11_AVX
	
	vmovd xmm8,dword ptr[rsi]
	vmovd xmm0,dword ptr[rsi+r14]
	add rsi,r15
	vpavgb xmm8,xmm8,xmm0
	vpunpcklbw xmm8,xmm8,xmm7
	
	movzx ecx,byte ptr[r11]
	vpextrw ebx,xmm8,0
	movzx edx,byte ptr[r12] ; edx=V ecx=U ebx=Y

	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,0
	
	movzx ecx,byte ptr[r11+1]
	vpextrw ebx,xmm8,2
	movzx edx,byte ptr[r12+1] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,1
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,1
	
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm9,xmm9,xmm4
	vpsraw xmm0,xmm0,6
	vpsraw xmm9,xmm9,6
	vpmaxsw xmm0,xmm0,xmm2
	vpmaxsw xmm9,xmm9,xmm5
	vpminsw xmm0,xmm0,xmm3
	vpminsw xmm9,xmm9,xmm6
	vpackuswb xmm0,xmm0,xmm7
	vpackuswb xmm9,xmm9,xmm7
	
	vpextrw eax,xmm0,0
	add r11,2
	mov word ptr[rdi],ax
	add r12,2
	vpextrw eax,xmm9,0
	add rdi,2
	mov word ptr[r13],ax
	add r13,2
	
suite2_11_AVX:	
	mov eax,w
	and eax,1
	jz suite3_11_AVX
	
	movzx eax,byte ptr[rsi]
	movzx ebx,byte ptr[rsi+r14]
	add rsi,2
	add ebx,eax
	shr ebx,1
	
	movzx ecx,byte ptr[r11]
	movzx ebx,byte ptr[r12] ; edx=V ecx=U ebx=Y
	
	movzx eax,word ptr[r10+2*rbx+1536]
	add ax,word ptr[r10+2*rcx+2048]
	add ax,word ptr[r10+2*rdx+2560]
	vpinsrw xmm0,xmm0,eax,0
	movzx eax,word ptr[r10+2*rbx+3072]
	add ax,word ptr[r10+2*rcx+3584]
	add ax,word ptr[r10+2*rdx+4096]
	vpinsrw xmm9,xmm9,eax,0
	
	vpaddsw xmm0,xmm0,xmm1
	vpaddsw xmm9,xmm9,xmm4
	vpsraw xmm0,xmm0,6
	vpsraw xmm9,xmm9,6
	vpmaxsw xmm0,xmm0,xmm2
	vpmaxsw xmm9,xmm9,xmm5
	vpminsw xmm0,xmm0,xmm3
	vpminsw xmm9,xmm9,xmm6
	vpackuswb xmm0,xmm0,xmm7
	vpackuswb xmm9,xmm9,xmm7
	
	vpextrw eax,xmm0,0
	inc r11
	mov byte ptr[rdi],al
	inc r12
	vpextrw eax,xmm9,0
	inc rdi
	mov byte ptr[r13],al
	inc r13
	
suite3_11_AVX:		
	add rsi,src_modulo_y
	add rsi,r14
	add r11,src_modulo_u
	add r12,src_modulo_v
	add rdi,dst_modulo_u
	add r13,dst_modulo_v
	
	dec h
	jnz Boucle0_11_AVX
	
	vmovdqu xmm10,XMMWORD ptr[rsp+64]
	vmovdqu xmm9,XMMWORD ptr[rsp+48]
	vmovdqu xmm8,XMMWORD ptr[rsp+32]
	vmovdqu xmm7,XMMWORD ptr[rsp+16]
	vmovdqu xmm6,XMMWORD ptr[rsp]
	add rsp,80

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_ColorSpaceConvert_YV12_AVX_3 endp


end
