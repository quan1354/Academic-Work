.model small
.stack 64

public newline
public choiceErrorMessage
public notDigitErrorMessage
public notIntegerErrorMessage
public notPositiveNumberErrorMessage
public continuePrompt
public gKGConverter
public printInt
public parseInt
public round
public printAsKg
public printAsRM
public pow

.data

newline db 0dh, 0ah, "$"
choiceErrorMessage db "Invalid choice! Please try again.$"
notDigitErrorMessage db "Invalid input! It must be a digit! Please try again.$"
notIntegerErrorMessage db "Invalid input! It must be an integer! Please try again.$"
notPositiveNumberErrorMessage db "Invalid input! It must be a positive number! Please try again.$"
continuePrompt db "Press any key to continue.$"
gKGConverter dw 1000
centsRMConverter dw 100

.code

; param ax: integer to be printed
; param bx: minimum width
; param cl: default character
printInt proc near
    ; setup stack frame
    push bp
    mov bp, sp
    sub sp, 3

    ; save registers
    push ax
    push bx
    push cx
    push dx
    push si
    push di

    mov [bp - 2], bx; minimum width
    mov [bp - 3], cl; default character
    mov si, ax; integer
    mov di, 10; place value
    call intSize
    mov cx, ax; integer size

    mov ax, 10
    mov bx, cx
    dec bx
    call pow
    mov bx, ax; divisor

    .printDefaultChar:
        cmp [bp - 2], cx
        jle .printDigit

        ; print default char
        mov ah, 02h
        mov dl, [bp - 3]
        int 21h

        mov ax, [bp - 2]
        dec ax
        mov [bp - 2], ax
        jmp .printDefaultChar

    .printDigit:
        mov ax, si; working integer
        xor dx, dx; dx = 0
        div bx
        xor dx, dx; dx = 0
        div di

        ; print digit
        mov ah, 02h
        add dx, 30h; convert int to char
        int 21h

        mov ax, bx
        xor dx, dx; dx = 0
        div di
        mov bx, ax
        loop .printDigit

    ; restore registers
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    ; remove stack frame
    add sp, 3
    mov sp, bp
    pop bp

    ret
printInt endp

; param ax: integer
; return ax: integer size
intSize proc near
    ; save registers
    push bx
    push cx
    push dx

    xor bx, bx; result = 0
    mov cx, 10; place value

    .intSizeLoop:
        xor dx, dx
        div cx
        inc bx

        test ax, 0ffffh
        jnz .intSizeLoop

    mov ax, bx

    ; restore registers
    pop dx
    pop cx
    pop bx

    ret
intSize endp

; param ax: base
; param bx: exponent
; return ax: base to the power of exponent
pow proc near
    ; save registers
    push bx
    push cx
    push dx

    mov cx, bx; exponent
    mov bx, ax; base
    mov ax, 1; result

    .powLoop:
        test cx, 0ffffh
        jz .powDone

        mul bx
        dec cx
        jmp .powLoop

    .powDone:
        ; restore registers
        pop dx
        pop cx
        pop bx

        ret
pow endp

; param ax: address of integer string
; param bx: string size
; return ax: integer
parseInt proc near
    ; setup stack frame
    push bp
    mov bp, sp
    sub sp, 2

    ; save registers
    push bx
    push cx
    push dx
    push si
    push di

    mov dx, ax
    xor ax, ax; ax = 0
    mov [bp - 2], ax; result

    mov ax, 10
    dec bx
    call pow
    
    mov bx, dx; address of integer string
    mov cx, ax; multiplier
    xor si, si; index = 0
    mov di, 10; place value

    .parseDigit:
        mov al, [bx + si]
        sub al, 30h; convert char to int
        xor ah, ah; ah = 0
        mul cx
        add [bp - 2], ax
        mov ax, cx
        xor dx, dx; dx = 0
        div di
        mov cx, ax
        inc si

        test cx, 0ffffh
        jnz .parseDigit

    mov ax, [bp - 2]

    ; restore registers
    pop di
    pop si
    pop dx
    pop cx
    pop bx

    ; remove stack frame
    add sp, 2
    mov sp, bp
    pop bp

    ret
parseInt endp

; param ax: quotient
; param bx: remainder
; param cx: divisor
; return ax: rounded quotient
round proc near
    ; save registers
    push bx
    push cx
    push dx
    push si
    push di

    mov si, ax; quotient
    mov di, 2; doubler
    mov ax, bx
    mul di
    cmp ax, cx
    jge .adjustQuotient
    jmp .roundEnd

    .adjustQuotient:
        inc si

    .roundEnd:
        mov ax, si

        ; restore registers
        pop di
        pop si
        pop dx
        pop cx
        pop bx

        ret
round endp

; param ax: gram
; param bx: minimum width
printAsKg proc near
    ; save registers
    push ax
    push bx
    push cx
    push dx
    push si

    mov cx, ax; gram
    call intSize
    mov si, ax;

    inc si; kg size
    cmp si, 5; minimum kg size
    jge .printAsKgPrintSpaces

    mov si, 5; minimum kg size

    .printAsKgPrintSpaces:
        cmp bx, si
        jle .printKg

        ; print ' '
        mov ah, 02h
        mov dl, " "
        int 21h

        dec bx
        jmp .printAsKgPrintSpaces

    .printKg:
        xor dx, dx; dx = 0
        mov ax, cx
        mov bx, gKgConverter
        div bx

        xor bx, bx; bx = 0
        mov cl, " "
        call printInt

        mov bx, dx

        ; print '.'
        mov ah, 02h
        mov dl, "."
        int 21h

        mov ax, bx
        mov bx, 3
        mov cl, "0"
        call printInt

    ; restore registers
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    ret
printAsKg endp

; param ax: cents
; param bx: minimum width
printAsRM proc near
    ; save registers
    push ax
    push bx
    push cx
    push dx
    push si

    mov cx, ax; cents
    call intSize
    mov si, ax;

    inc si; RM size
    cmp si, 4; minimum RM size
    jge .printAsRMPrintSpace

    mov si, 4; minimum RM size

    .printAsRMPrintSpace:
        cmp bx, si
        jle .printRM

        ; print ' '
        mov ah, 02h
        mov dl, " "
        int 21h

        dec bx
        jmp .printAsRMPrintSpace

    .printRM:
        xor dx, dx; dx = 0
        mov ax, cx
        mov bx, centsRMConverter
        div bx

        xor bx, bx; bx = 0
        mov cl, " "
        call printInt

        mov bx, dx

        ; print '.'
        mov ah, 02h
        mov dl, "."
        int 21h

        mov ax, bx
        mov bx, 2
        mov cl, "0"
        call printInt

    ; restore registers
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    ret
printAsRM endp
end