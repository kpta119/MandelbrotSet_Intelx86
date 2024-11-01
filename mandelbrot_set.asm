;Arguments:
;RDI - *data
;RSI - pitch (length of one row in bytes)
;RDX (EDX) - height and width
;XMM0 - scale
;XMM1 - x_left
;XMM2 - y_up

;Calculations:
;XMM6 - current pRe
;XMM7 - current pIm
;XMM8 - oldRe
;XMM9 - oldIm
;XMM11 - moduls squared

section .text
global mandelbrot_set

mandelbrot_set:

    push rbp
    mov rbp, rsp

    mov r8d, edx         ; r8 = height
    mov r10d, 4          ; 4.0 in double
    cvtsi2sd xmm12, r10d

column_loop:
    mov r9d, edx         ; r9 = width
    movsd xmm5, xmm1     ; xmm5 = disp_x_left
    xor r10, r10         ; index pixel in row array

row_loop:
    xor rcx, rcx         ; zero iteration register
    movsd xmm6, xmm5     ; current pRe
    movsd xmm7, xmm2     ; current pIm
    pxor xmm8, xmm8 ;oldRe reset to zero at the beginning for each point
    pxor xmm9, xmm9 ;oldIm reset to zero at the beginning for each point
recursive_sequence:
    ; p = x + jy - current point
    ; newRe = oldRe * oldRe - oldIm * oldIm + x;
    ; newIm = 2 * oldRe * oldIm + y;
    ; if((newRe * newRe + newIm * newIm) > 4) break;

    movsd xmm10, xmm9 ;assing oldIm

    ; calculate newIm
    mulsd xmm9, xmm8   ; oldIm * oldRe
    addsd xmm9, xmm9   ; newIm = 2 * oldRe * oldIm
    addsd xmm9, xmm7   ; newIm = newIm + pIm

    ; calculate newRe
    mulsd xmm8, xmm8    ; oldRe * oldRe
    mulsd xmm10, xmm10  ; oldIm * oldIm
    subsd xmm8, xmm10   ; newRe = oldRe * oldRe - oldIm * oldIm
    addsd xmm8, xmm6    ; newRe = newRe + pRe

    movsd xmm10, xmm9   ; move newIm to xmm10 because in xmm9 will be oldIm
    movsd xmm11, xmm8   ; move newRe to xmm11

    ;moduls squared
    mulsd xmm11, xmm11  ; newRe * newRe
    mulsd xmm10, xmm10  ; newIm * newIm
    addsd xmm11, xmm10  ; moduls_squared = (newRe * newRe + newIm * newIm)

    inc ecx             ; increase number of iterations
    cmp ecx, 255
    jae after_sequence  ; if iterations > 255, break

    comisd xmm11, xmm12     ;Compare Double-Precision Floating Point Values and Set EFLAGS
    jbe recursive_sequence  ; if modulus_squared <= 4, continue

after_sequence:
    mov byte [rdi + r10], cl  ; draw pixels
    mov byte [rdi + r10 + 1], cl
    mov byte [rdi + r10 + 2], cl

    add r10, 3         ; increase index to next pixel


    addsd xmm5, xmm0   ; move to next x pixel
    dec r9
    jnz row_loop       ; if not end of row, continue

end_row_loop:
    add rdi, rsi       ; move to next row in bitmap
    subsd xmm2, xmm0   ; move to next y pixel

    dec r8             ; decrease height counter
    jnz column_loop    ; if not end of column, continue

epilog:
    mov rsp, rbp
    pop rbp
    ret
