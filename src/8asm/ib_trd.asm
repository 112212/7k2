; Seven Kingdoms 2: The Fryhtan War
;
; Copyright 1999 Enlight Software Ltd.
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;

;Filename    : IB_TRD.ASM
;Description : Blt a bitmap to the display surface buffer
;	       with decompression, transparency handling and colormaping


INCLUDE imgfun.inc
INCLUDE colcode.inc

.CODE


;----------- BEGIN OF FUNCTION IMGbltTransRemapDecompress ------
;
; Put a compressed bitmap on image buffer.
; It does handle color key transparency and remaping.
;
; Syntax : IMGbltTransRemapDecompress( imageBuf, pitch, x, y, bitmapBuf,
;                 colorTable)
;
; char *imageBuf - the pointer to the display surface buffer
; int  pitch     - pitch of the display surface buffer
; int  x,y       - where to put the image on the surface buffer
; char *bitmapPtr  - the pointer to the bitmap buffer
; char *colorTable - color remaping table
;
; two counters are maintained, EDX and ECX for counting no. of rows
; and no. of columns remaining to draw
; if the counter reach zero, exit the loop
;
; ESI initally points to the start of bitmap data
; EDI initally points to the top left hand cornder of the destination
;     in video memory
;
; compressed data is loaded from ESI, into AL
; If AL is non-transparent, blit the point to video memory.
; If AL is transparent, seek EDI forward. If the right side of the
; destination is passed,
;   1. seek EDI to the left side of the next line
;   2. if run-length is still very long, seek one more line
;   3. residue (of run-length) is added to EDI, ECX will count from a number
;      lower than the width of bitmap
;
;-------------------------------------------------
;
; Format of the bitmap data :
;
; <short>  width
; <short>  height
; <char..> bitmap image
;
;-------------------------------------------------

		PUBLIC    IMGbltTransRemapDecompress
IMGbltTransRemapDecompress PROC imageBuf,pitch,x,y,bitmapPtr,colorTable
		LOCAL	bitmapWidth:DWORD, bitmapHeight:DWORD
		STARTPROC

		MOV	EAX, imageBuf		; store the address of the image buffer to a variable
		MOV	image_buf, EAX

		;------ get the bitmap width and height -----;

		MOV     AX , DS
		MOV	ES , AX
		MOV     ESI, bitmapPtr
		CLD

		;------ calculate destination on the video memory ----;
		CALC_ADDR EDI, x, y, pitch	; Get the address to the destination buffer

		;------ calculate bitmapWidth - no. of points ploted on each row --;
		XOR	EAX, EAX
		LODSW                            ; get bitmap width
		MOV     bitmapWidth, EAX

		;----- calculate number of rows to be ploted on the screen
		LODSW                            ; get bitmap height
		MOV	bitmapHeight, EAX
		MOV     EDX, EAX		; EDX contains no. of rows remain

		MOV	EBX, colorTable
@@loopY:
		MOV	ECX, bitmapWidth
@@loopX:
		LODSB
		PRE_REMAP
		JUMP_IF_TRANS al, @@compressed1
@@nonTrans:
		; -----  00-F7, simply blit the point on video memory ----
		POST_REMAP
		STOSB

		; ----------  test end of line ------------
		; (pass the right of clipping window)
		LOOP	@@loopX
		ADD	EDI, pitch
		SUB	EDI, bitmapWidth
		; JMP	@@endloopY	; reduce jump
		DEC	EDX
		JNE	@@loopY
		JMP	@@end

		ALIGN	4
@@compressed1:
		JUMP_IF_NOT_MANY_TRANS al, @@compress1a
		; F8
		LODSB
		ENCODE_FEW_TRANS_CODE al
@@compress1a:
		; F7-FF
		DECODE_FEW_TRANS_CODE al
		MOVZX	AX,AL

		CMP	ECX, EAX
		JBE	@@compress2
		; ECX > EAX
		; meaning the run-length is still within output bitmap
		ADD	EDI, EAX	; skip the number of points
		SUB	ECX, EAX
		JMP	@@loopX

		ALIGN	4
@@compress2:
		; run-length is outside clip window
		; adjust EDI to point to left of next line
		SUB	EAX, ECX
		ADD	EDI, pitch
		ADD	EDI, ECX
		SUB	EDI, bitmapWidth

@@compress3a:
		; if EAX is longer than width of bitmap,
		; position to EDI one line below
		CMP	EAX, bitmapWidth
		JB	@@compress4
		ADD	EDI, pitch
		SUB	EAX, bitmapWidth
		DEC	EDX			; minus y remain by 1
		JNE	@@compress3a
		JMP	@@end

		ALIGN	4
@@compress4:
		; add remainder to EDI
		; ECX has another initial value other than bitmapWidth
		ADD	EDI, EAX
		MOV     ECX, bitmapWidth
		SUB	ECX, EAX
		DEC	EDX
		JNE	@@loopX
		JMP	@@end

		ALIGN	4
@@endloopY:
		DEC	EDX
		JNE	@@loopY

@@end:          ENDPROC
IMGbltTransRemapDecompress   	ENDP

;----------- END OF FUNCTION IMGbltTransRemapDecompress ----------

END
