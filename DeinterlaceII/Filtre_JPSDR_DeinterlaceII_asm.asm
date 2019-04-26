;
;  DeinterlaceII
;
;  Deinterlace video.
;  Copyright (C) 2000 JPSDR
;	
;  DeinterlaceII is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  DeinterlaceII is distributed in the hope that it will be useful,
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
.model flat,c

.code

JPSDR_DeinterlaceII_Move32_Full_src proc src:dword,dst:dword,w:dword,h:dword,src_modulo:dword

	public JPSDR_DeinterlaceII_Move32_Full_src

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
	mov eax,src_modulo
loop_G:
	mov ecx,ebx
	rep movsd
	add esi,eax
	dec edx
	jnz short loop_G
	
fin_G:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_DeinterlaceII_Move32_Full_src endp

JPSDR_DeinterlaceII_Move32_Full_dst proc src:dword,dst:dword,w:dword,h:dword,dst_modulo:dword

	public JPSDR_DeinterlaceII_Move32_Full_dst

	push esi
	push edi
	push ebx
	
	cld
	mov esi,src
	mov edi,dst
	mov ebx,w
	mov edx,h
	mov eax,dst_modulo
loop_H:
	mov ecx,ebx
	rep movsd
	add edi,eax
	dec edx
	jnz short loop_H
	
fin_H:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_DeinterlaceII_Move32_Full_dst endp


end





