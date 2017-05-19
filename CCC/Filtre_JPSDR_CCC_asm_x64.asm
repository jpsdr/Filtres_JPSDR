.data

align 16

uw_2 word 8 dup(2)
uw_3 word 8 dup(3)
uw_11 word 8 dup(11)
uw_16 word 8 dup(16)
uw_22 word 8 dup(22)
uw_32 word 8 dup(32)
uw_53 word 8 dup(53)
uw_235 word 8 dup(235)

.code



;JPSDR_CCC_Planar_SSE_FR proc src:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword
; src = rcx
; dst = rdx
; src_pitch = r8
; dst_picth = r9

JPSDR_CCC_Planar_SSE_FR proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	sub rsp,128
	.allocstack 128
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	movdqa XMMWORD ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqa XMMWORD ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqa XMMWORD ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	movdqa XMMWORD ptr[rsp+80],xmm11
	.savexmm128 xmm11,80
	movdqa XMMWORD ptr[rsp+96],xmm12
	.savexmm128 xmm12,96
	movdqa XMMWORD ptr[rsp+112],xmm13
	.savexmm128 xmm13,112
	.endprolog
	
	pxor xmm7,xmm7
	
	mov r10,rcx		; r10 = src_orig
	mov r11,rdx		; r11 = dst_orig
	add r11,r9
	xor rcx,rcx
	
	movdqa xmm9,XMMWORD ptr uw_2
	movdqa xmm8,XMMWORD ptr uw_32
	movdqa xmm13,XMMWORD ptr uw_11
	movdqa xmm12,XMMWORD ptr uw_22
	movdqa xmm11,XMMWORD ptr uw_53
	movdqa xmm10,XMMWORD ptr uw_3
	
	mov rax,8
	
	mov ecx,w
	shr ecx,1
	jz suite1
	
boucle1:	
	mov rsi,r10
	sub rsi,r8
	movq xmm0,qword ptr[rsi]				
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movq xmm2,qword ptr[rsi]
	add rsi,r8
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movq xmm1,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movq xmm3,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add rsi,r8
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movq xmm4,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movq xmm0,qword ptr[rsi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movq xmm4,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movq xmm5,qword ptr[rsi]
	psubsw xmm3,xmm4						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]
	punpcklbw xmm5,xmm7						;xmm5=[N+7]
	paddsw xmm3,xmm9						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]+2
	pmullw xmm4,xmm12						;xmm4=43x[N+6]
	psraw xmm3,2							;xmm3=(3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2
	pmullw xmm5,xmm13						;xmm5=21x[N+7]
	movdqa xmm6,xmm3
	psubsw xmm0,xmm5						;xmm0=106x[N+5]-21x[N+7]
	mov rdi,r11
	packuswb xmm3,xmm7						;xmm3=xmm3[0->255]=calc5
	paddsw xmm2,xmm1						;xmm2=calc3+calc1
	pmullw xmm6,xmm12						;xmm6=43x((3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2)
	paddsw xmm2,xmm8						;xmm2=calc3+calc1+64
	paddsw xmm1,xmm6						;xmm1=calc1+calc6
	psraw xmm2,6							;xmm2=(calc3+calc1+64)>>7
	paddsw xmm1,xmm8						;xmm1=calc1+calc6+64
	paddsw xmm6,xmm0						;xmm6=calc6+calc2
	packuswb xmm2,xmm7						;xmm2=xmm2[0->255]
	paddsw xmm4,xmm0						;xmm4=calc4+calc2
	movq qword ptr[rdi],xmm2
	psraw xmm1,6							;xmm1=(calc1+calc6+64)>>7
	add rdi,r9
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	movq qword ptr[rdi],xmm1
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	add rdi,r9
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	movq qword ptr[rdi],xmm3
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	add rdi,r9
	movq qword ptr[rdi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add rdi,r9
	movq qword ptr[rdi],xmm4
	
	add r10,rax
	add r11,rax
	
	dec ecx
	jnz boucle1
	
suite1:
	mov ecx,w
	and ecx,1
	jz fin1

	mov rsi,r10
	sub rsi,r8
	movd xmm0,dword ptr[rsi]				
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movd xmm2,dword ptr[rsi]
	add rsi,r8
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movd xmm1,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movd xmm3,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add rsi,r8
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movd xmm4,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movd xmm0,dword ptr[rsi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movd xmm4,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movd xmm5,dword ptr[rsi]
	psubsw xmm3,xmm4						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]
	punpcklbw xmm5,xmm7						;xmm5=[N+7]
	paddsw xmm3,xmm9						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]+2
	pmullw xmm4,xmm12						;xmm4=43x[N+6]
	psraw xmm3,2							;xmm3=(3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2
	pmullw xmm5,xmm13						;xmm5=21x[N+7]
	movdqa xmm6,xmm3						;xmm6=calc5
	psubsw xmm0,xmm5						;xmm0=106x[N+5]-21x[N+7]
	packuswb xmm3,xmm7						;xmm3=xmm3[0->255]=calc5
	mov rdi,r11
	paddsw xmm2,xmm1						;xmm2=calc3+calc1
	pmullw xmm6,xmm12						;xmm6=43x((3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2)=calc6
	paddsw xmm2,xmm8						;xmm2=calc3+calc1+64
	paddsw xmm1,xmm6						;xmm1=calc1+calc6
	psraw xmm2,6							;xmm2=(calc3+calc1+64)>>7
	paddsw xmm1,xmm8						;xmm1=calc1+calc6+64
	paddsw xmm6,xmm0						;xmm6=calc6+calc2
	packuswb xmm2,xmm7						;xmm2=xmm2[0->255]
	paddsw xmm4,xmm0						;xmm4=calc4+calc2
	movd dword ptr[rdi],xmm2
	psraw xmm1,6							;xmm1=(calc1+calc6+64)>>7
	add rdi,r9
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	movd dword ptr[rdi],xmm1
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	add rdi,r9
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	movd dword ptr[rdi],xmm3
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	add rdi,r9
	movd dword ptr[rdi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add rdi,r9
	movd dword ptr[rdi],xmm4	
	
fin1:	
	movdqa xmm13,XMMWORD ptr[rsp+112]
	movdqa xmm12,XMMWORD ptr[rsp+96]
	movdqa xmm11,XMMWORD ptr[rsp+80]
	movdqa xmm10,XMMWORD ptr[rsp+64]
	movdqa xmm9,XMMWORD ptr[rsp+48]
	movdqa xmm8,XMMWORD ptr[rsp+32]
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,128
	pop rdi
	pop rsi
	pop rbp

	ret	

JPSDR_CCC_Planar_SSE_FR endp





;JPSDR_CCC_Planar_SSE proc src:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword
; src = rcx
; dst = rdx
; src_pitch = r8
; dst_picth = r9

JPSDR_CCC_Planar_SSE proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	sub rsp,160
	.allocstack 160
	movdqa oword ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa oword ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	movdqa oword ptr[rsp+32],xmm8
	.savexmm128 xmm8,32
	movdqa oword ptr[rsp+48],xmm9
	.savexmm128 xmm9,48
	movdqa oword ptr[rsp+64],xmm10
	.savexmm128 xmm10,64
	movdqa oword ptr[rsp+80],xmm11
	.savexmm128 xmm11,80
	movdqa oword ptr[rsp+96],xmm12
	.savexmm128 xmm12,96
	movdqa oword ptr[rsp+112],xmm13
	.savexmm128 xmm13,112
	movdqa oword ptr[rsp+128],xmm14
	.savexmm128 xmm14,128
	movdqa oword ptr[rsp+144],xmm15
	.savexmm128 xmm15,144
	.endprolog
	
	pxor xmm7,xmm7
	
	mov r10,rcx		; r10 = src_orig
	mov r11,rdx		; r11 = dst_orig
	add r11,r9
	xor rcx,rcx
	
	movdqa xmm9,oword ptr uw_2
	movdqa xmm8,oword ptr uw_32
	movdqa xmm13,oword ptr uw_11
	movdqa xmm12,oword ptr uw_22
	movdqa xmm11,oword ptr uw_53
	movdqa xmm10,oword ptr uw_3
	movdqa xmm14,oword ptr uw_16
	movdqa xmm15,oword ptr uw_235
	
	mov rax,8
	
	mov ecx,w
	shr ecx,1
	jz suite3
	
boucle3:	
	mov rsi,r10
	sub rsi,r8
	movq xmm0,qword ptr[rsi]				
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movq xmm2,qword ptr[rsi]
	add rsi,r8
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movq xmm1,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movq xmm3,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add rsi,r8
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movq xmm4,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movq xmm0,qword ptr[rsi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movq xmm4,qword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movq xmm5,qword ptr[rsi]
	psubsw xmm3,xmm4						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]
	punpcklbw xmm5,xmm7						;xmm5=[N+7]
	paddsw xmm3,xmm9						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]+2
	pmullw xmm4,xmm12						;xmm4=43x[N+6]
	psraw xmm3,2							;xmm3=(3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2
	pmullw xmm5,xmm13						;xmm5=21x[N+7]
	movdqa xmm6,xmm3
	pminsw xmm3,xmm15						;xmm3=min(xmm3,235)
	psubsw xmm0,xmm5						;xmm0=106x[N+5]-21x[N+7]
	pmaxsw xmm3,xmm14						;xmm3=max(xmm3,16)
	mov rdi,r11
	packuswb xmm3,xmm7						;xmm3=xmm3[0->255]=calc5
	paddsw xmm2,xmm1						;xmm2=calc3+calc1
	pmullw xmm6,xmm12						;xmm6=43x((3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2)
	paddsw xmm2,xmm8						;xmm2=calc3+calc1+64
	paddsw xmm1,xmm6						;xmm1=calc1+calc6
	psraw xmm2,6							;xmm2=(calc3+calc1+64)>>7
	paddsw xmm1,xmm8						;xmm1=calc1+calc6+64
	pminsw xmm2,xmm15						;xmm2=min(xmm2,235)
	paddsw xmm6,xmm0						;xmm6=calc6+calc2
	pmaxsw xmm2,xmm14						;xmm2=max(xmm2,16)
	paddsw xmm4,xmm0						;xmm4=calc4+calc2
	packuswb xmm2,xmm7						;xmm2=xmm2[0->255]
	psraw xmm1,6							;xmm1=(calc1+calc6+64)>>7
	movq qword ptr[rdi],xmm2
	pminsw xmm1,xmm15						;xmm1=min(xmm1,235)	
	add rdi,r9
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	pmaxsw xmm1,xmm14						;xmm1=max(xmm1,16)		
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	movq qword ptr[rdi],xmm1
	pminsw xmm6,xmm15						;xmm6=min(xmm6,235)		
	add rdi,r9
	pmaxsw xmm6,xmm14						;xmm6=max(xmm6,16)		
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	movq qword ptr[rdi],xmm3
	pminsw xmm4,xmm15						;xmm4=min(xmm4,235)		
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	add rdi,r9
	pmaxsw xmm4,xmm14						;xmm4=max(xmm4,16)		
	movq qword ptr[rdi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add rdi,r9
	movq qword ptr[rdi],xmm4
	
	add r10,rax
	add r11,rax
	
	dec ecx
	jnz boucle3
	
suite3:
	mov ecx,w
	and ecx,1
	jz fin3
	
	mov rsi,r10
	sub rsi,r8
	movd xmm0,dword ptr[rsi]				
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movd xmm2,dword ptr[rsi]
	add rsi,r8
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movd xmm1,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movd xmm3,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add rsi,r8
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movd xmm4,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movd xmm0,dword ptr[rsi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add rsi,r8
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movd xmm4,dword ptr[rsi]
	add rsi,r8
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movd xmm5,dword ptr[rsi]
	psubsw xmm3,xmm4						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]
	punpcklbw xmm5,xmm7						;xmm5=[N+7]
	paddsw xmm3,xmm9						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]+2
	pmullw xmm4,xmm12						;xmm4=43x[N+6]
	psraw xmm3,2							;xmm3=(3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2
	pmullw xmm5,xmm13						;xmm5=21x[N+7]
	movdqa xmm6,xmm3						;xmm6=calc5
	pminsw xmm3,xmm15						;xmm3=min(xmm3,235)
	psubsw xmm0,xmm5						;xmm0=106x[N+5]-21x[N+7]
	pmaxsw xmm3,xmm14						;xmm3=max(xmm3,16)
	mov rdi,r11
	packuswb xmm3,xmm7						;xmm3=xmm3[0->255]
	paddsw xmm2,xmm1						;xmm2=calc3+calc1
	pmullw xmm6,xmm12						;xmm6=43x((3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2)=calc6
	paddsw xmm2,xmm8						;xmm2=calc3+calc1+64
	paddsw xmm1,xmm6						;xmm1=calc1+calc6
	psraw xmm2,6							;xmm2=(calc3+calc1+64)>>7
	paddsw xmm1,xmm8						;xmm1=calc1+calc6+64
	pminsw xmm2,xmm15						;xmm2=min(xmm2,235)	
	paddsw xmm6,xmm0						;xmm6=calc6+calc2
	pmaxsw xmm2,xmm14						;xmm2=max(xmm2,16)	
	paddsw xmm4,xmm0						;xmm4=calc4+calc2
	packuswb xmm2,xmm7						;xmm2=xmm2[0->255]
	psraw xmm1,6							;xmm1=(calc1+calc6+64)>>7
	movd dword ptr[rdi],xmm2
	pminsw xmm1,xmm15						;xmm1=min(xmm1,235)	
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	pmaxsw xmm1,xmm14						;xmm1=max(xmm1,16)	
	add rdi,r9
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	movd dword ptr[rdi],xmm1
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	add rdi,r9
	pminsw xmm6,xmm15						;xmm6=min(xmm6,235)	
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	pmaxsw xmm6,xmm14						;xmm6=max(xmm6,16)	
	movd dword ptr[rdi],xmm3
	pminsw xmm4,xmm15						;xmm4=min(xmm4,235)	
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	pmaxsw xmm4,xmm14						;xmm4=max(xmm4,16)	
	add rdi,r9
	movd dword ptr[rdi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add rdi,r9
	movd dword ptr[rdi],xmm4		

fin3:	
	movdqa xmm15,oword ptr[rsp+144]
	movdqa xmm14,oword ptr[rsp+128]
	movdqa xmm13,oword ptr[rsp+112]
	movdqa xmm12,oword ptr[rsp+96]
	movdqa xmm11,oword ptr[rsp+80]
	movdqa xmm10,oword ptr[rsp+64]
	movdqa xmm9,oword ptr[rsp+48]
	movdqa xmm8,oword ptr[rsp+32]
	movdqa xmm7,oword ptr[rsp+16]
	movdqa xmm6,oword ptr[rsp]
	add rsp,160
	pop rdi
	pop rsi
	pop rbp

	ret	

JPSDR_CCC_Planar_SSE endp


end





