;
;  VHS
;
;  Temporal denoiser and chroma/luma sharpening for cleaning VHS.
;  Copyright (C) 2005 JPSDR
;	
;  VHS is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  VHS is distributed in the hope that it will be useful,
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
.mmx
.xmm
.model flat,c

.code


JPSDR_VHS_Move32_Full proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword,dst_modulo:dword

	public JPSDR_VHS_Move32_Full

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
loop_1_f_3:
	mov ecx,ebx
	rep movsd
	add esi,src_modulo
	add edi,dst_modulo
	dec edx
	jnz short loop_1_f_3
	
fin_f_3:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_VHS_Move32_Full endp



end





