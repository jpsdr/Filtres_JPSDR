;
;  AddBorder
;
;  Add border to image.
;  Copyright (C) 2000 JPSDR
;	
;  AddBorder is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  AddBorder is distributed in the hope that it will be useful,
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


JPSDR_AddBorder_Fill32 proc dst:dword,taille:dword,data:dword

	public JPSDR_AddBorder_Fill32

	push edi
	
	cld
	mov edi,dst
	mov ecx,taille
	mov eax,data
	or ecx,ecx
	jz short fin_Fill32
	rep stosd

fin_Fill32:	
	pop edi

	ret

JPSDR_AddBorder_Fill32 endp




end





