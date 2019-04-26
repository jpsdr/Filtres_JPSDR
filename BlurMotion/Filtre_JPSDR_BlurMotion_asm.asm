;
;  BlurMotion
;
;  Replace duplicated frame by blend between previous and next frame.
;  Copyright (C) 2000 JPSDR
;	
;  BlurMotion is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  BlurMotion is distributed in the hope that it will be useful,
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

.code


JPSDR_BlurMotion_SSE_3_A proc src:dword,s:dword,dst:dword,w:dword,h:dword,src_pitch:dword,buffer_pitch:dword,
	dst_pitch:dword

	public JPSDR_BlurMotion_SSE_3_A

	push esi
	push edi
	push ebx

	mov esi,src
	mov ebx,16
	mov edx,s
	mov edi,dst

loop_1_f:
	xor eax,eax
	mov ecx,w
	
loop_2_f:
	movdqa xmm0,oword ptr[esi+eax]
	pavgb xmm0,oword ptr[edx+eax]
	movdqa oword ptr[edi+eax],xmm0
	add eax,ebx
	loop loop_2_f

	add esi,src_pitch
	add edx,buffer_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_f

	pop ebx
	pop edi
	pop esi

	ret

JPSDR_BlurMotion_SSE_3_A endp



end





