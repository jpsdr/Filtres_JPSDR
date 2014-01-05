;	VirtualDub - Video processing and capture application
;	System library component
;	Copyright (C) 1998-2004 Avery Lee, All Rights Reserved.
;
;	Beginning with 1.6.0, the VirtualDub system library is licensed
;	differently than the remainder of VirtualDub.  This particular file is
;	thus licensed as follows (the "zlib" license):
;
;	This software is provided 'as-is', without any express or implied
;	warranty.  In no event will the authors be held liable for any
;	damages arising from the use of this software.
;
;	Permission is granted to anyone to use this software for any purpose,
;	including commercial applications, and to alter it and redistribute it
;	freely, subject to the following restrictions:
;
;	1.	The origin of this software must not be misrepresented; you must
;		not claim that you wrote the original software. If you use this
;		software in a product, an acknowledgment in the product
;		documentation would be appreciated but is not required.
;	2.	Altered source versions must be plainly marked as such, and must
;		not be misrepresented as being the original software.
;	3.	This notice may not be removed or altered from any source
;		distribution.

.code
vdasm_uint128_add proc public frame
		.endprolog

		mov		rax, [rdx]
		add		rax, [r8]
		mov		[rcx], rax
		mov		rax, [rdx+8]
		adc		rax, [r8+8]
		mov		[rcx+8], rax
		ret

vdasm_uint128_add endp


vdasm_uint128_sub proc public frame
		.endprolog
		
		mov		rax, [rdx]
		sub		rax, [r8]
		mov		[rcx], rax
		mov		rax, [rdx+8]
		sbb		rax, [r8+8]
		mov		[rcx+8], rax
		ret

vdasm_uint128_sub endp


vdasm_uint128_mul proc public frame

		push rbx
		.pushreg rbx
		push rsi
		.pushreg rsi
		.endprolog

		mov		rbx, rdx			;rbx = src1
		mov		rax, [rdx]			;rax = src1a
		mov		rsi, [r8]			;rsi = src2a
		mul		rsi					;rdx:rax = src1a*src2a
		mov		[rcx], rax			;write low result
		mov		r9, rdx				;r9 = (src1a*src2a).hi
		mov		rax, [rbx+8]		;rax = src1b
		mul		rsi					;rdx:rax = src1b*src2a
		add		r9, rax				;r9 = (src1a*src2a).hi + (src1b*src2a).lo
		mov		rax, [rbx]			;rax = src1a
		mul		qword ptr[r8+8]		;rdx:rax = src1a*src2b
		add		rax, r9				;rax = (src1a*src2b).lo + (src1b*src2a).lo + (src1a*src2a).hi
		mov		[rcx+8], rax		;write high result
		
		pop rsi
		pop rbx
		ret
		
vdasm_uint128_mul endp

end