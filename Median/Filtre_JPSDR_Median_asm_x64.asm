.code


;JPSDR_Median_Planar_Move_src proc src:dword,dst:dword,w:dword,h:dword,src_offset:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Median_Planar_Move_src proc public frame

src_offset equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rax,src_offset
	mov edx,7

loop_1_f_2:
	mov ecx,r8d
	and ecx,edx
	jz short loop_2_f_2
	rep movsb
loop_2_f_2:
	mov ecx,r8d	
	shr ecx,3
	rep movsq
	add rsi,rax
	dec r9d
	jnz short loop_1_f_2
	
fin_f_2:
	pop rsi
	pop rdi
	pop rbp
	
	ret

JPSDR_Median_Planar_Move_src endp



;JPSDR_Median_Planar_Move_dst proc src:dword,dst:dword,w:dword,h:dword,dst_offset:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Median_Planar_Move_dst proc public frame

dst_offset equ qword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	.endprolog
	
	cld
	mov rsi,rcx
	mov rdi,rdx
	xor rcx,rcx
	mov rax,dst_offset
	mov edx,7

loop_1_f_3:
	mov ecx,r8d
	and ecx,edx
	jz short loop_2_f_3
	rep movsb
loop_2_f_3:
	mov ecx,r8d
	shr ecx,3
	rep movsq
	add rdi,rax
	dec r9d
	jnz short loop_1_f_3
	
fin_f_3:
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_Planar_Move_dst endp



;JPSDR_Median_RGB32_Move_src proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_offset:dword
; src = rcx
; planar_R = rdx
; planar_G = r8
; planar_B = r9

JPSDR_Median_RGB32_Move_src proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]

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
	dec r8d
	jnz short loop_1_g
	
fin_g:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_src endp


;JPSDR_Median_YUYV_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9

JPSDR_Median_YUYV_Move_src proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_offset
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
	dec r9d
	jnz short loop_1_h
	
fin_h:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_src endp


;JPSDR_Median_UYVY_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword
; src = rcx
; planar_y = rdx
; planar_u = r8
; planar_v = r9


JPSDR_Median_UYVY_Move_src proc public frame
w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
src_offset equ qword ptr[rbp+64]

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
	mov rbx,r8
	mov rdx,r9
	mov r8d,w
	mov r9d,h
	mov r10,src_offset
	mov r11,2
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
	dec r9d
	jnz short loop_1_i
	
fin_i:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_src endp


;JPSDR_Median_RGB32_Move_dst proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,dst_offset:dword
; planar_R = rcx
; planar_G = rdx
; planar_B = r8
; dst = r9

JPSDR_Median_RGB32_Move_dst proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
dst_offset equ qword ptr[rbp+64]
	
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
	mov r10,dst_offset
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
	add rdi,r10
	dec r9d
	jnz short loop_1_j
		
fin_j:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_RGB32_Move_dst endp


;JPSDR_Median_YUYV_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,dst_offset:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9

JPSDR_Median_YUYV_Move_dst proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
dst_offset equ qword ptr[rbp+64]
	
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
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_offset
	xor rcx,rcx
			
loop_1_k:
	mov ecx,r8d
loop_2_k:
	mov ah,byte ptr[rdx]
	mov al,byte ptr[rsi+1]
	inc rdx
	shl eax,16
	lodsb
	mov ah,byte ptr[ebx]
	inc rsi
	stosd
	inc rbx
	loop loop_2_k
	add rdi,r10
	dec r9d
	jnz short loop_1_k
		
fin_k:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_YUYV_Move_dst endp


;JPSDR_Median_UYVY_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,dst_offset:dword
; planar_y = rcx
; planar_u = rdx
; planar_v = r8
; dst = r9


JPSDR_Median_UYVY_Move_dst proc public frame

w equ dword ptr[rbp+48]
h equ dword ptr[rbp+56]
dst_offset equ qword ptr[rbp+64]
	
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
	mov rdx,r8
	mov rdi,r9
	mov r8d,w
	mov r9d,h
	mov r10,dst_offset
	mov r11,2
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
	add rdi,r10
	dec r9d
	jnz short loop_1_l
		
fin_l:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Median_UYVY_Move_dst endp

end





