;
;  CCC
;
;  Cross-Conversion Correction for specific 1080p.
;  Copyright (C) 2008 JPSDR
;	
;  CCC is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  CCC is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;   
;  You should have received a copy of the GNU General Public License
;  along with GNU Make; see the file COPYING.  If not, write to
;  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
;
;

.586
.xmm
.model flat,c

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

xmm9 equ XMMWORD ptr uw_2
xmm8 equ XMMWORD ptr uw_32
xmm13 equ XMMWORD ptr uw_11
xmm12 equ XMMWORD ptr uw_22
xmm11 equ XMMWORD ptr uw_53
xmm10 equ XMMWORD ptr uw_3
xmm14 equ XMMWORD ptr uw_16
xmm15 equ XMMWORD ptr uw_235


JPSDR_CCC_Planar_SSE_FR proc src:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword

	public JPSDR_CCC_Planar_SSE_FR

	push esi
	push edi
	push ebx
	
	pxor xmm7,xmm7
	
	mov ebx,src		; ebx = src_orig
	mov edx,dst		; edx = dst_orig
	add edx,dst_pitch
	
	mov ecx,w
	shr ecx,1
	jz suite1

boucle1:	
	mov eax,src_pitch
	mov esi,ebx
	sub esi,eax
	movq xmm0,qword ptr[esi]				
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movq xmm2,qword ptr[esi]
	add esi,eax
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movq xmm1,qword ptr[esi]
	add esi,eax
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movq xmm3,qword ptr[esi]
	add esi,eax
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add esi,eax
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movq xmm4,qword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movq xmm0,qword ptr[esi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movq xmm4,qword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movq xmm5,qword ptr[esi]
	psubsw xmm3,xmm4						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]
	punpcklbw xmm5,xmm7						;xmm5=[N+7]
	paddsw xmm3,xmm9						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]+2
	pmullw xmm4,xmm12						;xmm4=43x[N+6]
	psraw xmm3,2							;xmm3=(3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2
	pmullw xmm5,xmm13						;xmm5=21x[N+7]
	movdqa xmm6,xmm3
	psubsw xmm0,xmm5						;xmm0=106x[N+5]-21x[N+7]
	mov esi,edx
	mov eax,dst_pitch
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
	movq qword ptr[edi],xmm2
	psraw xmm1,6							;xmm1=(calc1+calc6+64)>>7
	add edi,eax
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	movq qword ptr[edi],xmm1
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	add edi,eax
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	movq qword ptr[edi],xmm3
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	add edi,eax
	movq qword ptr[edi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add edi,eax
	movq qword ptr[edi],xmm4
	
	add ebx,8
	add edx,8
	
	dec ecx
	jnz boucle1
	
suite1:
	mov ecx,w
	and ecx,1
	jz fin1

	mov eax,src_pitch
	mov esi,ebx
	sub esi,eax	
	movd xmm0,dword ptr[esi]				
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movd xmm2,dword ptr[esi]
	add esi,eax
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movd xmm1,dword ptr[esi]
	add esi,eax
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movd xmm3,dword ptr[esi]
	add esi,eax
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add esi,eax
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movd xmm4,dword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movd xmm0,dword ptr[esi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movd xmm4,dword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movd xmm5,dword ptr[esi]
	psubsw xmm3,xmm4						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]
	punpcklbw xmm5,xmm7						;xmm5=[N+7]
	paddsw xmm3,xmm9						;xmm3=3x([N+2]+[N+4])-[N]-[N+6]+2
	pmullw xmm4,xmm12						;xmm4=43x[N+6]
	psraw xmm3,2							;xmm3=(3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2
	pmullw xmm5,xmm13						;xmm5=21x[N+7]
	movdqa xmm6,xmm3						;xmm6=calc5
	psubsw xmm0,xmm5						;xmm0=106x[N+5]-21x[N+7]
	packuswb xmm3,xmm7						;xmm3=xmm3[0->255]=calc5
	mov edi,edx
	mov eax,dst_pitch
	paddsw xmm2,xmm1						;xmm2=calc3+calc1
	pmullw xmm6,xmm12						;xmm6=43x((3x([N+2]+[N+4])-[N]-[N+6]+2) >> 2)=calc6
	paddsw xmm2,xmm8						;xmm2=calc3+calc1+64
	paddsw xmm1,xmm6						;xmm1=calc1+calc6
	psraw xmm2,6							;xmm2=(calc3+calc1+64)>>7
	paddsw xmm1,xmm8						;xmm1=calc1+calc6+64
	paddsw xmm6,xmm0						;xmm6=calc6+calc2
	packuswb xmm2,xmm7						;xmm2=xmm2[0->255]
	paddsw xmm4,xmm0						;xmm4=calc4+calc2
	movd dword ptr[edi],xmm2
	psraw xmm1,6							;xmm1=(calc1+calc6+64)>>7
	add edi,eax
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	movd dword ptr[edi],xmm1
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	add edi,eax
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	movd dword ptr[edi],xmm3
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	add edi,eax
	movd dword ptr[edi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add edi,eax
	movd dword ptr[edi],xmm4
	
fin1:	
	pop ebx
	pop edi
	pop esi

	ret	

JPSDR_CCC_Planar_SSE_FR endp



JPSDR_CCC_Planar_SSE proc src:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword

	public JPSDR_CCC_Planar_SSE

	push esi
	push edi
	push ebx
		
	pxor xmm7,xmm7
	
	mov ebx,src		; ebx = src_orig
	mov edx,dst		; edx = dst_orig
	add edx,dst_pitch
	
	mov ecx,w
	shr ecx,1
	jz suite3
	
boucle3:	
	mov eax,src_pitch
	mov esi,ebx
	sub esi,eax
	movq xmm0,qword ptr[esi]				
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movq xmm2,qword ptr[esi]
	add esi,eax
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movq xmm1,qword ptr[esi]
	add esi,eax
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movq xmm3,qword ptr[esi]
	add esi,eax
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add esi,eax
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movq xmm4,qword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movq xmm0,qword ptr[esi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movq xmm4,qword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movq xmm5,qword ptr[esi]
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
	mov edi,edx
	mov eax,dst_pitch
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
	movq qword ptr[edi],xmm2
	pminsw xmm1,xmm15						;xmm1=min(xmm1,235)	
	add edi,eax
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	pmaxsw xmm1,xmm14						;xmm1=max(xmm1,16)		
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	movq qword ptr[edi],xmm1
	pminsw xmm6,xmm15						;xmm6=min(xmm6,235)		
	add edi,eax
	pmaxsw xmm6,xmm14						;xmm6=max(xmm6,16)		
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	movq qword ptr[edi],xmm3
	pminsw xmm4,xmm15						;xmm4=min(xmm4,235)		
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	add edi,eax
	pmaxsw xmm4,xmm14						;xmm4=max(xmm4,16)		
	movq qword ptr[edi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add edi,eax
	movq qword ptr[edi],xmm4
	
	add ebx,8
	add edx,8
	
	dec ecx
	jnz boucle3
	
suite3:	
	mov ecx,w
	and ecx,1
	jz fin3
	
	mov eax,src_pitch
	mov esi,ebx
	sub esi,eax	
	movd xmm0,dword ptr[esi]				
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N-1]
	movd xmm2,dword ptr[esi]
	add esi,eax
	pmullw xmm0,xmm13						;xmm0=[N-1]x21 (x1)
	punpcklbw xmm2,xmm7						;xmm2=[N]
	movd xmm1,dword ptr[esi]
	add esi,eax
	punpcklbw xmm1,xmm7						;xmm1=[N+1]
	movd xmm3,dword ptr[esi]
	add esi,eax
	punpcklbw xmm3,xmm7						;xmm3=[N+2]
	add esi,eax
	pmullw xmm1,xmm11						;xmm1=[N+1]x106 (x5)
	movd xmm4,dword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+4]
	psubsw xmm1,xmm0						;xmm1=106x[N+1]-21x[N-1]
	paddsw xmm3,xmm4						;xmm3=[N+4]+[N+2]
	movd xmm0,dword ptr[esi]	
	pmullw xmm3,xmm10						;xmm3=3x([N+2]+[N+4])
	add esi,eax
	punpcklbw xmm0,xmm7						;xmm0=[N+5]
	movd xmm4,dword ptr[esi]
	add esi,eax
	punpcklbw xmm4,xmm7						;xmm4=[N+6]
	psubsw xmm3,xmm2						;xmm3=3x([N+2]+[N+4])-[N]
	pmullw xmm0,xmm11						;xmm0=106x[N+5]
	pmullw xmm2,xmm12						;xmm2=43x[N] (x2)
	movd xmm5,dword ptr[esi]
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
	mov edi,edx
	mov eax,dst_pitch
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
	movd dword ptr[edi],xmm2
	pminsw xmm1,xmm15						;xmm1=min(xmm1,235)	
	paddsw xmm6,xmm8						;xmm6=calc6+calc2+64
	pmaxsw xmm1,xmm14						;xmm1=max(xmm1,16)	
	add edi,eax
	packuswb xmm1,xmm7						;xmm1=xmm1[0->255]
	paddsw xmm4,xmm8						;xmm4=calc4+calc2+64
	movd dword ptr[edi],xmm1
	psraw xmm6,6							;xmm6=(calc6+calc2+64)>>7
	add edi,eax
	pminsw xmm6,xmm15						;xmm6=min(xmm6,235)	
	psraw xmm4,6							;xmm4=(calc4+calc2+64)>>7
	pmaxsw xmm6,xmm14						;xmm6=max(xmm6,16)	
	movd dword ptr[edi],xmm3
	pminsw xmm4,xmm15						;xmm4=min(xmm4,235)	
	packuswb xmm6,xmm7						;xmm6=xmm6[0->255]
	pmaxsw xmm4,xmm14						;xmm4=max(xmm4,16)	
	add edi,eax
	movd dword ptr[edi],xmm6
	packuswb xmm4,xmm7						;xmm4=xmm4[0->255]
	add edi,eax
	movd dword ptr[edi],xmm4		
		
fin3:		
	pop ebx
	pop edi
	pop esi

	ret	

JPSDR_CCC_Planar_SSE endp


end





