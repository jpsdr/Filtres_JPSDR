;
;  Median
;
;  Median filter with threshold (despecle).
;  Copyright (C) 2004 JPSDR
;	
;  Median is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2, or (at your option)
;  any later version.
;   
;  Median is distributed in the hope that it will be useful,
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


JPSDR_Median_RGB32_Move_src proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_offset:dword,dst_offset:dword

	public JPSDR_Median_RGB32_Move_src
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	
loop_1_g:
	mov ecx,w
loop_2_g:
	lodsd
	stosb
	mov byte ptr[ebx],ah
	shr eax,16
	inc ebx
	mov byte ptr[edx],al
	inc edx
	loop loop_2_g
	
	add esi,src_offset
	add edi,dst_offset
	add ebx,dst_offset
	add edx,dst_offset
	dec h
	jnz short loop_1_g
	
fin_g:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src endp


JPSDR_Median_RGB32_Move_src_SSE_2 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_src_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	
loop_1_g_2:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_g_2

loop_2_g_2:
	movdqa xmm0,XMMWORD ptr[esi+4*eax] ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	movdqa xmm1,XMMWORD ptr[esi+4*eax+16] ;xR8G8B8xR7G7B7xR6G6B6xR5G5B5	
	movdqa xmm2,xmm0                 ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	punpcklbw xmm0,xmm1              ;xxR6R2G6G2B6B2xxR5R1G5G1B5B1
	punpckhbw xmm2,xmm1              ;xxR8R4G8G4B8B4xxR7R3G7G3B7B3
	movdqa xmm1,xmm0                 ;xxR6R2G6G2B6B2xxR5R1G5G1B5B1
	punpcklbw xmm0,xmm2              ;xxxxR7R5R3R1G7G5G3G1B7B5B3B1
	punpckhbw xmm1,xmm2              ;xxxxR8R6R4R2G8G6G4G2B8B6B4B2
	movdqa xmm2,xmm0                 ;xxxxR7R5R3R1G7G5G3G1B7B5B3B1
	punpcklbw xmm0,xmm1              ;G8G7G6G5G4G3G2G1B8B7B6B5B4B3B2B1
	punpckhbw xmm2,xmm1              ;xxxxxxxxR8R7R6R5R4R3R2R1
	
	movq qword ptr[edi+eax],xmm0
	movhps qword ptr[ebx+eax],xmm0
	movq qword ptr[edx+eax],xmm2
	add eax,8
	loop loop_2_g_2

suite1_g_2:
	mov ecx,w
	and ecx,1
	jz short suite2_g_2
	
	movdqa xmm0,XMMWORD ptr[esi+4*eax] ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	movhlps xmm1,xmm0                  ;xR4G4B4xR3G3B3xR4G4B4xR3G3B3
	punpcklbw xmm0,xmm1                ;xxR4R2G4G2B4B2xxR3R1G3G1B3B1
	movhlps xmm1,xmm0                  ;xxR4R2G4G2B4B2xxR4R2G4G2B4B2
	punpcklbw xmm0,xmm1                ;xxxxR4R3R2R1G4G3G2G1B4B3B2B1
	movhlps xmm1,xmm0                  ;xxxxR4R3R2R1xxxxR4R3R2R1
	movd dword ptr[edi+eax],xmm0
	psrlq xmm0,32
	movd dword ptr[edx+eax],xmm1
	movd dword ptr[ebx+eax],xmm0
	
suite2_g_2:	
	add esi,src_pitch
	add edi,dst_pitch
	add ebx,dst_pitch
	add edx,dst_pitch
	dec h
	jnz loop_1_g_2
	
fin_g_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src_SSE_2 endp


JPSDR_Median_RGB32_Move_src_AVX_2 proc src:dword,planar_R:dword,planar_G:dword,planar_B:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_src_AVX_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	
loop_1_g_2_AVX:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_g_2_AVX

loop_2_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+4*eax]    ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	vmovdqa xmm1,XMMWORD ptr[esi+4*eax+16] ;xR8G8B8xR7G7B7xR6G6B6xR5G5B5	
	vpunpckhbw xmm2,xmm0,xmm1              ;xxR8R4G8G4B8B4xxR7R3G7G3B7B3
	vpunpcklbw xmm0,xmm0,xmm1              ;xxR6R2G6G2B6B2xxR5R1G5G1B5B1
	vpunpckhbw xmm1,xmm0,xmm2              ;xxxxR8R6R4R2G8G6G4G2B8B6B4B2
	vpunpcklbw xmm0,xmm0,xmm2              ;xxxxR7R5R3R1G7G5G3G1B7B5B3B1
	vpunpckhbw xmm2,xmm0,xmm1              ;xxxxxxxxR8R7R6R5R4R3R2R1
	vpunpcklbw xmm0,xmm0,xmm1              ;G8G7G6G5G4G3G2G1B8B7B6B5B4B3B2B1
	
	vmovq qword ptr[edi+eax],xmm0
	vmovhps qword ptr[ebx+eax],xmm0
	vmovq qword ptr[edx+eax],xmm2
	add eax,8
	loop loop_2_g_2_AVX

suite1_g_2_AVX:
	mov ecx,w
	and ecx,1
	jz short suite2_g_2_AVX
	
	vmovdqa xmm0,XMMWORD ptr[esi+4*eax]      ;xR4G4B4xR3G3B3xR2G2B2xR1G1B1
	vmovhlps xmm1,xmm1,xmm0                  ;xR4G4B4xR3G3B3xR4G4B4xR3G3B3
	vpunpcklbw xmm0,xmm0,xmm1                ;xxR4R2G4G2B4B2xxR3R1G3G1B3B1
	vmovhlps xmm1,xmm1,xmm0                  ;xxR4R2G4G2B4B2xxR4R2G4G2B4B2
	vpunpcklbw xmm0,xmm0,xmm1                ;xxxxR4R3R2R1G4G3G2G1B4B3B2B1
	vmovhlps xmm1,xmm1,xmm0                  ;xxxxR4R3R2R1xxxxR4R3R2R1
	vmovd dword ptr[edi+eax],xmm0
	vpsrlq xmm0,xmm0,32
	vmovd dword ptr[edx+eax],xmm1
	vmovd dword ptr[ebx+eax],xmm0
	
suite2_g_2_AVX:	
	add esi,src_pitch
	add edi,dst_pitch
	add ebx,dst_pitch
	add edx,dst_pitch
	dec h
	jnz loop_1_g_2_AVX
	
fin_g_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_src_AVX_2 endp


JPSDR_Median_YUYV_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword,dst_offset_Y:dword,dst_offset_UV:dword

	public JPSDR_Median_YUYV_Move_src
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_h:
	mov ecx,w
loop_2_h:
	lodsd
	stosb
	mov byte ptr[ebx],ah
	shr eax,16
	inc ebx
	stosb
	mov byte ptr[edx],ah
	inc edx
	loop loop_2_h
	
	add esi,src_offset
	add edi,dst_offset_Y
	add ebx,dst_offset_UV
	add edx,dst_offset_UV
	dec h
	jnz short loop_1_h
	
fin_h:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src endp


JPSDR_Median_YUYV_Move_src_SSE_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_YUYV_Move_src_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_h_2:
    xor eax,eax    
	mov ecx,w
	shr ecx,1
	jz short suite1_h_2
	
loop_2_h_2:
	movdqa xmm0,XMMWORD ptr[esi+4*eax]   ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	movdqa xmm1,XMMWORD ptr[esi+4*eax+16]   ;V8Y16U8Y15V7Y14U7Y13 V6Y12U6Y11V5Y10U5Y9
	movdqa xmm2,xmm0                      ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	punpcklbw xmm0,xmm1                  ;V6V2Y12Y4U6U2Y11Y3 V5V1Y10Y2U5U1Y9Y1
	punpckhbw xmm2,xmm1                  ;V8V4Y16Y8U8U4Y15Y7 V7V3Y14Y6U7U3Y13Y5
	movdqa xmm1,xmm0
	punpcklbw xmm0,xmm2                  ;V7V5V3V1Y14Y10Y6Y2 U7U5U3U1Y13Y9Y5Y1
	punpckhbw xmm1,xmm2                  ;V8V6V4V2Y16Y12Y8Y4 U8U6U4U2Y15Y11Y7Y3
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm1                  ;U8U7U6U5U4U3U2U1 Y15Y13Y11Y9Y5Y3Y1
	punpckhbw xmm2,xmm1                  ;V8V7V6V5V4V3V2V1 Y16Y14Y12Y10Y8Y6Y4Y2
	movhps qword ptr [ebx+eax],xmm0
	punpcklbw xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	movhps qword ptr [edx+eax],xmm2
	movdqa XMMWORD ptr[edi+2*eax],xmm0
	add eax,8
	loop loop_2_h_2

suite1_h_2:
	mov ecx,w
	and ecx,1
	jz short suite2_h_2
    
	movdqa xmm0,XMMWORD ptr[esi+4*eax]   ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	movhlps xmm1,xmm0                    ;V4Y8U4Y7V3Y6U3Y5 V4Y8U4Y7V3Y6U3Y5
	punpcklbw xmm0,xmm1                  ;V4V2Y8Y4U4U2Y7Y3 V3V1Y6Y2U3U1Y5Y1
	movhlps xmm1,xmm0                    ;V4V2Y8Y4U4U2Y7Y3 V4V2Y8Y4U4U2Y7Y3
	punpcklbw xmm0,xmm1                  ;V4V3V2V1Y8Y6Y4Y2 U4U3U2U1Y7Y5Y3Y1
	movhlps xmm2,xmm0                    ;xxxxxxxx V4V3V2V1Y8Y6Y4Y2
	movdqa xmm1,xmm0
	psrlq xmm0,32                        ;0000V4V3V2V1 0000U4U3U2U1
	punpcklbw xmm1,xmm2                  ; xxxxxxxx Y8Y7Y6Y5Y4Y3Y2Y1
	movd dword ptr[ebx+eax],xmm0
	movhlps xmm2,xmm0
	movq qword ptr[edi+2*eax],xmm1
	movd dword ptr[edx+eax],xmm2

suite2_h_2:	
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV
	dec h
	jnz loop_1_h_2
	
fin_h_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src_SSE_2 endp


JPSDR_Median_YUYV_Move_src_AVX_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_YUYV_Move_src_AVX_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_h_2_AVX:
    xor eax,eax    
	mov ecx,w
	shr ecx,1
	jz short suite1_h_2_AVX
	
loop_2_h_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+4*eax]        ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	vmovdqa xmm1,XMMWORD ptr[esi+4*eax+16]     ;V8Y16U8Y15V7Y14U7Y13 V6Y12U6Y11V5Y10U5Y9
	vpunpckhbw xmm2,xmm0,xmm1                  ;V8V4Y16Y8U8U4Y15Y7 V7V3Y14Y6U7U3Y13Y5
	vpunpcklbw xmm0,xmm0,xmm1                  ;V6V2Y12Y4U6U2Y11Y3 V5V1Y10Y2U5U1Y9Y1
	vpunpckhbw xmm1,xmm0,xmm2                  ;V8V6V4V2Y16Y12Y8Y4 U8U6U4U2Y15Y11Y7Y3
	vpunpcklbw xmm0,xmm0,xmm2                  ;V7V5V3V1Y14Y10Y6Y2 U7U5U3U1Y13Y9Y5Y1
	vpunpckhbw xmm2,xmm0,xmm1                  ;V8V7V6V5V4V3V2V1 Y16Y14Y12Y10Y8Y6Y4Y2
	vpunpcklbw xmm0,xmm0,xmm1                  ;U8U7U6U5U4U3U2U1 Y15Y13Y11Y9Y5Y3Y1
	vmovhps qword ptr [ebx+eax],xmm0
	vpunpcklbw xmm0,xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	vmovhps qword ptr [edx+eax],xmm2
	vmovdqa XMMWORD ptr[edi+2*eax],xmm0
	add eax,8
	loop loop_2_h_2_AVX

suite1_h_2_AVX:
	mov ecx,w
	and ecx,1
	jz short suite2_h_2_AVX
    
	vmovdqa xmm0,XMMWORD ptr[esi+4*eax]        ;V4Y8U4Y7V3Y6U3Y5 V2Y4U2Y3V1Y2U1Y1
	vmovhlps xmm1,xmm1,xmm0                    ;V4Y8U4Y7V3Y6U3Y5 V4Y8U4Y7V3Y6U3Y5
	vpunpcklbw xmm0,xmm0,xmm1                  ;V4V2Y8Y4U4U2Y7Y3 V3V1Y6Y2U3U1Y5Y1
	vmovhlps xmm1,xmm1,xmm0                    ;V4V2Y8Y4U4U2Y7Y3 V4V2Y8Y4U4U2Y7Y3
	vpunpcklbw xmm0,xmm0,xmm1                  ;V4V3V2V1Y8Y6Y4Y2 U4U3U2U1Y7Y5Y3Y1
	vmovhlps xmm2,xmm2,xmm0                    ;xxxxxxxx V4V3V2V1Y8Y6Y4Y2
	vpunpcklbw xmm1,xmm0,xmm2                  ; xxxxxxxx Y8Y7Y6Y5Y4Y3Y2Y1
	vpsrlq xmm0,xmm0,32                        ;0000V4V3V2V1 0000U4U3U2U1
	vmovd dword ptr[ebx+eax],xmm0
	vmovhlps xmm2,xmm2,xmm0
	vmovq qword ptr[edi+2*eax],xmm1
	vmovd dword ptr[edx+eax],xmm2

suite2_h_2_AVX:	
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV
	dec h
	jnz loop_1_h_2_AVX
	
fin_h_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_src_AVX_2 endp


JPSDR_Median_UYVY_Move_src proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_offset:dword,dst_offset_Y:dword,dst_offset_UV:dword

	public JPSDR_Median_UYVY_Move_src
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_i:
	mov ecx,w
loop_2_i:
	lodsd
	mov byte ptr[edi],ah		
	mov byte ptr[ebx],al
	shr eax,16
	inc ebx
	mov byte ptr[edi+1],ah
	mov byte ptr[edx],al
	add edi,2
	inc edx
	loop loop_2_i
	
	add esi,src_offset
	add edi,dst_offset_Y
	add ebx,dst_offset_UV
	add edx,dst_offset_UV	
	dec h
	jnz short loop_1_i
	
fin_i:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src endp


JPSDR_Median_UYVY_Move_src_SSE_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_UYVY_Move_src_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_i_2:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_i_2
	
loop_2_i_2:
	movdqa xmm0,XMMWORD ptr[esi+4*eax]   ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	movdqa xmm1,XMMWORD ptr[esi+4*eax+16]   ;Y16V8Y15U8Y14V7Y13U7 Y12V6Y11U6Y10V5Y9U5
	movdqa xmm2,xmm0                      ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	punpcklbw xmm0,xmm1                  ;Y12Y4V6V2Y11Y3U6U2 Y10Y2V5V1Y9Y1U5U1
	punpckhbw xmm2,xmm1                  ;Y16Y8V8V4Y15Y7U8U4 Y14Y6V7V3Y13Y5U7U3
	movdqa xmm1,xmm0
	punpcklbw xmm0,xmm2                  ;Y14Y10Y6Y2V7V5V3V1 Y13Y9Y5Y1U7U5U3U1
	punpckhbw xmm1,xmm2                  ;Y16Y12Y8Y4V8V6V4V2 Y15Y11Y7Y3U8U6U4U2
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm1                  ;Y15Y13Y11Y9Y7Y5Y3Y1 U8U7U6U5U4U3U2U1
	punpckhbw xmm2,xmm1                  ;Y16Y14Y12Y10Y8Y6Y4Y2 V8V7V6V5V4v3V2V1
	movq qword ptr [ebx+eax],xmm0
	punpckhbw xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	movq qword ptr [edx+eax],xmm2
	movdqa XMMWORD ptr[edi+2*eax],xmm0
	add eax,8
	loop loop_2_i_2
	
suite1_i_2:
	mov ecx,w
	and ecx,1
	jz short suite2_i_2
	
	movdqa xmm0,XMMWORD ptr[esi+4*eax]   ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	movhlps xmm1,xmm0                    ;Y8V4Y7U4Y6V3Y5U3 Y8V4Y7U4Y6V3Y5U3
	punpcklbw xmm0,xmm1                  ;Y8Y4V4V2Y7Y3U4U2 Y6Y2V3V1Y5Y1U3U1
	movhlps xmm1,xmm0                    ;Y8Y4V4V2Y7Y3U4U2 Y8Y4V4V2Y7Y3U4U2
	punpcklbw xmm0,xmm1                  ;Y8Y6Y4Y2V4V3V2V1 Y7Y5Y3Y1U4U3U2U1
	movhlps xmm2,xmm0                    ;xxxxxxxx Y8Y6Y4Y2V4V3V2V1	
	movd dword ptr [ebx+eax],xmm0
	punpcklbw xmm0,xmm2                  ;Y8Y7Y6Y5Y4Y3Y2Y1 xxxxxxxx
	movd dword ptr [edx+eax],xmm2
	movhps qword ptr[edi+2*eax],xmm0
	
suite2_i_2:
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV	
	dec h
	jnz loop_1_i_2
	
fin_i_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src_SSE_2 endp


JPSDR_Median_UYVY_Move_src_AVX_2 proc src:dword,planar_Y:dword,planar_U:dword,planar_V:dword,w:dword,h:dword,src_pitch:dword,dst_pitch_Y:dword,dst_pitch_UV:dword

	public JPSDR_Median_UYVY_Move_src_AVX_2
	
	push esi
	push edi
	push ebx
	
	mov esi,src
	mov edi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	
loop_1_i_2_AVX:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_i_2_AVX
	
loop_2_i_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[esi+4*eax]        ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	vmovdqa xmm1,XMMWORD ptr[esi+4*eax+16]     ;Y16V8Y15U8Y14V7Y13U7 Y12V6Y11U6Y10V5Y9U5
	vpunpckhbw xmm2,xmm0,xmm1                  ;Y16Y8V8V4Y15Y7U8U4 Y14Y6V7V3Y13Y5U7U3
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y12Y4V6V2Y11Y3U6U2 Y10Y2V5V1Y9Y1U5U1
	vpunpckhbw xmm1,xmm0,xmm2                  ;Y16Y12Y8Y4V8V6V4V2 Y15Y11Y7Y3U8U6U4U2
	vpunpcklbw xmm0,xmm0,xmm2                  ;Y14Y10Y6Y2V7V5V3V1 Y13Y9Y5Y1U7U5U3U1
	vpunpckhbw xmm2,xmm0,xmm1                  ;Y16Y14Y12Y10Y8Y6Y4Y2 V8V7V6V5V4v3V2V1
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y15Y13Y11Y9Y7Y5Y3Y1 U8U7U6U5U4U3U2U1
	vmovq qword ptr [ebx+eax],xmm0
	vpunpckhbw xmm0,xmm0,xmm2                  ;Y16Y15Y14Y13Y12Y11Y10Y9Y8Y7Y6Y5Y4Y3Y2Y1
	vmovq qword ptr [edx+eax],xmm2
	vmovdqa XMMWORD ptr[edi+2*eax],xmm0
	add eax,8
	loop loop_2_i_2_AVX
	
suite1_i_2_AVX:
	mov ecx,w
	and ecx,1
	jz short suite2_i_2_AVX
	
	vmovdqa xmm0,XMMWORD ptr[esi+4*eax]        ;Y8V4Y7U4Y6V3Y5U3 Y4V2Y3U2Y2V1Y1U1
	vmovhlps xmm1,xmm1,xmm0                    ;Y8V4Y7U4Y6V3Y5U3 Y8V4Y7U4Y6V3Y5U3
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y8Y4V4V2Y7Y3U4U2 Y6Y2V3V1Y5Y1U3U1
	vmovhlps xmm1,xmm1,xmm0                    ;Y8Y4V4V2Y7Y3U4U2 Y8Y4V4V2Y7Y3U4U2
	vpunpcklbw xmm0,xmm0,xmm1                  ;Y8Y6Y4Y2V4V3V2V1 Y7Y5Y3Y1U4U3U2U1
	vmovhlps xmm2,xmm2,xmm0                    ;xxxxxxxx Y8Y6Y4Y2V4V3V2V1	
	vmovd dword ptr [ebx+eax],xmm0
	vpunpcklbw xmm0,xmm0,xmm2                  ;Y8Y7Y6Y5Y4Y3Y2Y1 xxxxxxxx
	vmovd dword ptr [edx+eax],xmm2
	vmovhps qword ptr[edi+2*eax],xmm0
	
suite2_i_2_AVX:
	add esi,src_pitch
	add edi,dst_pitch_Y
	add ebx,dst_pitch_UV
	add edx,dst_pitch_UV	
	dec h
	jnz loop_1_i_2_AVX
	
fin_i_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_src_AVX_2 endp


JPSDR_Median_RGB32_Move_dst proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_offset:dword,dst_offset:dword

	public JPSDR_Median_RGB32_Move_dst
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	mov edi,dst
	
loop_1_j:
	mov ecx,w
loop_2_j:
	movzx eax,byte ptr[edx]
	inc edx
	shl eax,16
	lodsb
	mov ah,byte ptr[ebx]
	inc ebx
	stosd
	loop loop_2_j
	
	add esi,src_offset
	add ebx,src_offset
	add edx,src_offset
	add edi,dst_offset
	dec h
	jnz short loop_1_j
		
fin_j:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst endp


JPSDR_Median_RGB32_Move_dst_SSE_2 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_dst_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	mov edi,dst
	pxor xmm3,xmm3
	
loop_1_j_2:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_j_2
	
loop_2_j_2:
    movq xmm0,qword ptr[esi+eax]  ;00000000B8B7B6B5B4B3B2B1
	movq xmm2,qword ptr[edx+eax]  ;00000000R8R7R6R5R4R3R2R1
	movq xmm1,qword ptr[ebx+eax]  ;00000000G8G7G6G5G4G3G2G1
	punpcklbw xmm0,xmm2             ;R8B8R7B7R6B6R5B5R4B4R3B3R2B2R1B1
	punpcklbw xmm1,xmm3             ;0G80G70G60G50G40G30G20G1
	movdqa xmm2,xmm0                ;R8B8R7B7R6B6R5B5R4B4R3B3R2B2R1B1
	punpcklbw xmm0,xmm1             ;0R4G4B40R3G3B30R2G2B20R1G1B1
	punpckhbw xmm2,xmm1             ;0R8G8B80R7G7B70R6G6B60R5G5B5
	movdqa XMMWORD ptr[edi+4*eax],xmm0
	movdqa XMMWORD ptr[edi+4*eax+16],xmm2
	add eax,8
	loop loop_2_j_2
	
suite1_j_2:	
	mov ecx,w
	and ecx,1
	jz short suite2_j_2
	
    movd xmm0,dword ptr[esi+eax]  ;000000000000BBBB
	movd xmm1,dword ptr[ebx+eax]  ;000000000000GGGG
	movd xmm2,dword ptr[edx+eax]  ;000000000000RRRR
	punpcklbw xmm1,xmm3             ;000000000G0G0G0G
	punpcklbw xmm0,xmm2             ;00000000RBRBRBRB
	punpcklbw xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	movdqa XMMWORD ptr[edi+4*eax],xmm0

suite2_j_2:	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_j_2
		
fin_j_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst_SSE_2 endp


JPSDR_Median_RGB32_Move_dst_AVX_2 proc planar_R:dword,planar_G:dword,planar_B:dword,dst:dword,w:dword,h:dword,src_pitch:dword,dst_pitch:dword

	public JPSDR_Median_RGB32_Move_dst_AVX_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_B
	mov ebx,planar_G
	mov edx,planar_R
	mov edi,dst
	vpxor xmm3,xmm3,xmm3
	
loop_1_j_2_AVX:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_j_2_AVX
	
loop_2_j_2_AVX:
    vmovq xmm0,qword ptr[esi+eax]         ;00000000B8B7B6B5B4B3B2B1
	vmovq xmm2,qword ptr[edx+eax]         ;00000000R8R7R6R5R4R3R2R1
	vmovq xmm1,qword ptr[ebx+eax]         ;00000000G8G7G6G5G4G3G2G1
	vpunpcklbw xmm0,xmm0,xmm2             ;R8B8R7B7R6B6R5B5R4B4R3B3R2B2R1B1
	vpunpcklbw xmm1,xmm1,xmm3             ;0G80G70G60G50G40G30G20G1
	vpunpckhbw xmm2,xmm0,xmm1             ;0R8G8B80R7G7B70R6G6B60R5G5B5
	vpunpcklbw xmm0,xmm0,xmm1             ;0R4G4B40R3G3B30R2G2B20R1G1B1
	vmovdqa XMMWORD ptr[edi+4*eax],xmm0
	vmovdqa XMMWORD ptr[edi+4*eax+16],xmm2
	add eax,8
	loop loop_2_j_2_AVX
	
suite1_j_2_AVX:	
	mov ecx,w
	and ecx,1
	jz short suite2_j_2_AVX
	
    vmovd xmm0,dword ptr[esi+eax]         ;000000000000BBBB
	vmovd xmm1,dword ptr[ebx+eax]         ;000000000000GGGG
	vmovd xmm2,dword ptr[edx+eax]         ;000000000000RRRR
	vpunpcklbw xmm1,xmm1,xmm3             ;000000000G0G0G0G
	vpunpcklbw xmm0,xmm0,xmm2             ;00000000RBRBRBRB
	vpunpcklbw xmm0,xmm0,xmm1             ;0RGB0RGB0RGB0RGB
	vmovdqa XMMWORD ptr[edi+4*eax],xmm0

suite2_j_2_AVX:	
	add esi,src_pitch
	add ebx,src_pitch
	add edx,src_pitch
	add edi,dst_pitch
	dec h
	jnz short loop_1_j_2_AVX
		
fin_j_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_RGB32_Move_dst_AVX_2 endp


JPSDR_Median_YUYV_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_offset_Y:dword,src_offset_UV:dword,dst_offset:dword

	public JPSDR_Median_YUYV_Move_dst
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_k:
	mov ecx,w
loop_2_k:
	mov ah,byte ptr[edx]
	mov al,byte ptr[esi+1]
	inc edx
	shl eax,16
	lodsb
	mov ah,byte ptr[ebx]
	inc esi
	stosd
	inc ebx
	loop loop_2_k
	add esi,src_offset_Y
	add ebx,src_offset_UV
	add edx,src_offset_UV
	add edi,dst_offset
	dec h
	jnz short loop_1_k
		
fin_k:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst endp


JPSDR_Median_YUYV_Move_dst_SSE_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_YUYV_Move_dst_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_k_2:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_k_2
	
loop_2_k_2:
	movq xmm1,qword ptr[ebx+eax]  ;00000000UUUUUUUU
	movq xmm2,qword ptr[edx+eax]  ;00000000VVVVVVVV
	movdqa xmm0,XMMWORD ptr[esi+2*eax]  ;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	movdqa xmm2,xmm0                ;YYYYYYYYYYYYYYYY
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	punpckhbw xmm2,xmm1             ;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi+4*eax],xmm0
	movdqa XMMWORD ptr[edi+4*eax+16],xmm2
	add eax,8
	loop loop_2_k_2
	
suite1_k_2:	
	mov ecx,w
	and ecx,1
	jz short suite2_k_2
	
	movd xmm1,dword ptr[ebx+eax]  ;000000000000UUUU
	movd xmm2,dword ptr[edx+eax]  ;000000000000VVVV
	movq xmm0,qword ptr[esi+2*eax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[edi+4*eax],xmm0
	
suite2_k_2:	
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz short loop_1_k_2
		
fin_k_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst_SSE_2 endp


JPSDR_Median_YUYV_Move_dst_AVX_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_YUYV_Move_dst_AVX_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_k_2_AVX:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_k_2_AVX
	
loop_2_k_2_AVX:
	vmovq xmm1,qword ptr[ebx+eax]         ;00000000UUUUUUUU
	vmovq xmm2,qword ptr[edx+eax]         ;00000000VVVVVVVV
	vmovdqa xmm0,XMMWORD ptr[esi+2*eax]   ;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	vpunpckhbw xmm2,xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	vpunpcklbw xmm0,xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi+4*eax],xmm0
	vmovdqa XMMWORD ptr[edi+4*eax+16],xmm2
	add eax,8
	loop loop_2_k_2_AVX
	
suite1_k_2_AVX:	
	mov ecx,w
	and ecx,1
	jz short suite2_k_2_AVX
	
	vmovd xmm1,dword ptr[ebx+eax]         ;000000000000UUUU
	vmovd xmm2,dword ptr[edx+eax]         ;000000000000VVVV
	vmovq xmm0,qword ptr[esi+2*eax]       ;00000000YYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;00000000VUVUVUVU
	vpunpcklbw xmm0,xmm0,xmm1             ;VYUYVYUYVYUYVYUY
	vmovdqa XMMWORD ptr[edi+4*eax],xmm0
	
suite2_k_2_AVX:	
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz short loop_1_k_2_AVX
		
fin_k_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_YUYV_Move_dst_AVX_2 endp


JPSDR_Median_UYVY_Move_dst proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_offset_Y:dword,src_offset_UV:dword,dst_offset:dword

	public JPSDR_Median_UYVY_Move_dst
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_l:
	mov ecx,w
loop_2_l:
	mov al,byte ptr[edx]
	mov ah,byte ptr[esi+1]
	inc edx
	shl eax,16
	mov ah,byte ptr[esi]
	mov al,byte ptr[ebx]
	add esi,2
	stosd
	inc ebx
	loop loop_2_l
	
	add esi,src_offset_Y
	add ebx,src_offset_UV
	add edx,src_offset_UV
	add edi,dst_offset
	dec h
	jnz short loop_1_l
		
fin_l:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst endp


JPSDR_Median_UYVY_Move_dst_SSE_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_UYVY_Move_dst_SSE_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_l_2:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_l_2
	
loop_2_l_2:
	movq xmm1,qword ptr[ebx+eax]  ;00000000UUUUUUUU
	movq xmm2,qword ptr[edx+eax]  ;00000000VVVVVVVV
	movdqa xmm0,XMMWORD ptr[esi+2*eax]  ;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	movdqa xmm2,xmm1                ;VUVUVUVUVUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	punpckhbw xmm2,xmm0             ;YVYUYVYUYVYUYVYU
	movdqa XMMWORD ptr[edi+4*eax],xmm1
	movdqa XMMWORD ptr[edi+4*eax+16],xmm2
	add eax,8
	loop loop_2_l_2

suite1_l_2:	
	mov ecx,w
	and ecx,1
	jz short suite2_l_2
	
	movd xmm1,dword ptr[ebx+eax]  ;000000000000UUUU
	movd xmm2,dword ptr[edx+eax]  ;000000000000VVVV
	movq xmm0,qword ptr[esi+2*eax]  ;00000000YYYYYYYY
	punpcklbw xmm1,xmm2             ;00000000VUVUVUVU
	punpcklbw xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	movdqa XMMWORD ptr[edi+4*eax],xmm1
	
suite2_l_2:	
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz short loop_1_l_2
		
fin_l_2:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst_SSE_2 endp


JPSDR_Median_UYVY_Move_dst_AVX_2 proc planar_Y:dword,planar_U:dword,planar_V:dword,dst:dword,w:dword,h:dword,src_pitch_Y:dword,src_pitch_UV:dword,dst_pitch:dword

	public JPSDR_Median_UYVY_Move_dst_AVX_2
	
	push esi
	push edi
	push ebx
	
	mov esi,planar_Y
	mov ebx,planar_U
	mov edx,planar_V
	mov edi,dst
	
loop_1_l_2_AVX:
    xor eax,eax
	mov ecx,w
	shr ecx,1
	jz short suite1_l_2_AVX
	
loop_2_l_2_AVX:
	vmovq xmm1,qword ptr[ebx+eax]  ;00000000UUUUUUUU
	vmovq xmm2,qword ptr[edx+eax]  ;00000000VVVVVVVV
	vmovdqa xmm0,XMMWORD ptr[esi+2*eax]  ;YYYYYYYYYYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;VUVUVUVUVUVUVUVU
	vpunpckhbw xmm2,xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	vpunpcklbw xmm1,xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	vmovdqa XMMWORD ptr[edi+4*eax],xmm1
	vmovdqa XMMWORD ptr[edi+4*eax+16],xmm2
	add eax,8
	loop loop_2_l_2_AVX

suite1_l_2_AVX:	
	mov ecx,w
	and ecx,1
	jz short suite2_l_2_AVX
	
	vmovd xmm1,dword ptr[ebx+eax]  ;000000000000UUUU
	vmovd xmm2,dword ptr[edx+eax]  ;000000000000VVVV
	vmovq xmm0,qword ptr[esi+2*eax]  ;00000000YYYYYYYY
	vpunpcklbw xmm1,xmm1,xmm2             ;00000000VUVUVUVU
	vpunpcklbw xmm1,xmm1,xmm0             ;YVYUYVYUYVYUYVYU
	vmovdqa XMMWORD ptr[edi+4*eax],xmm1
	
suite2_l_2_AVX:	
	add esi,src_pitch_Y
	add ebx,src_pitch_UV
	add edx,src_pitch_UV
	add edi,dst_pitch
	dec h
	jnz short loop_1_l_2_AVX
		
fin_l_2_AVX:
	pop ebx
	pop edi
	pop esi

	ret

JPSDR_Median_UYVY_Move_dst_AVX_2 endp


end





