.model small
.stack 64

extrn newline: byte
extrn choiceErrorMessage: byte
extrn notIntegerErrorMessage: byte
extrn continuePrompt: byte
extrn printInt: proc
extrn parseInt: proc

public broccoliG
public cabbageG
public carrotG
public garlicG
public mushroomG

.data

stockMenu db "Stock Menu", 0dh, 0ah
          db "1. Add Stock", 0dh, 0ah
          db "2. Show Current Stock", 0dh, 0ah
          db "3. Back", 0dh, 0ah
          db "Enter a choice > $"

currentStockTitle db "Current Stock", 0dh, 0ah, "$"
currentBroccoli   db "Broccoli: $"
currentCabbage    db "Cabbage : $"
currentCarrot     db "Carrot  : $"
currentGarlic     db "Garlic  : $"
currentMushroom   db "Mushroom: $"

replenishVegetableMenu db "Replenish Vegetable Menu", 0dh, 0ah
                       db "1. Broccoli", 0dh, 0ah
                       db "2. Cabbage", 0dh, 0ah
                       db "3. Carrot", 0dh, 0ah
                       db "4. Garlic", 0dh, 0ah
                       db "5. Mushroom", 0dh, 0ah
                       db "6. Back", 0dh, 0ah
                       db "Enter a choice > $"

broccoliGPrompt db "Gram of Broccoli to be added > $"
cabbageGPrompt db "Gram of Cabbage to be added > $"
carrotGPrompt db "Gram of Carrot to be added > $"
garlicGPrompt db "Gram of Garlic to be added > $"
mushroomGPrompt db "Gram of Mushroom to be added > $"

vegetableGInput label byte
vegetableGInputMaxSize db 6
vegetableGInputActualSize db ?
vegetableGInputHolder db 6 dup("$")

broccoliG dw 10000
cabbageG dw 10000
carrotG dw 10000
garlicG dw 10000
mushroomG dw 10000

.code
main proc near
    ; init data segment
    mov ax, @data
    mov ds, ax

    .stock:
        ; clear screen
        mov ax, 0600h; ah = 06h (scroll)
                     ; al = 00h (full screen)
        mov bh, 07h; black background (0h)
                   ; white foreground (7h)
        mov cx, 0000h; upper left
        mov dx, 184fh; lower right
        int 10h

        ; adjust cursor
        mov ah, 02h
        mov bh, 00h; set page number 0
        mov dx, 0000h; upper left
        int 10h

        ; show stock menu
        mov ah, 09h
        lea dx, stockMenu
        int 21h

        ; read stock choice
        mov ah, 01h
        int 21h

        ; stock choice == '1' ?
        cmp al, 31h
        je .addStock

        ; stock choice == '2' ?
        cmp al, 32h
        je .showCurrentStock

        ; stock choice == '3' ?
        cmp al, 33h
        je .stockBack

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print invalid choice error message
        lea dx, choiceErrorMessage
        int 21h

        ; read key
        mov ah, 07h
        int 21h

        jmp .stock

        .addStock:
            call addStock
            jmp .stock

        .showCurrentStock:
            call showCurrentStock
            jmp .stock

        .stockBack:
            ; exit(0)
            mov ax, 4c00h
            int 21h
main endp

addStock proc near
    ; save registers
    push ax
    push bx
    push cx
    push dx

    .addVegetable:
        ; clear screen
        mov ax, 0600h; ah = 06h (scroll)
                     ; al = 00h (full screen)
        mov bh, 07h; black background (0h)
                   ; white foreground (7h)
        mov cx, 0000h; upper left
        mov dx, 184fh; lower right
        int 10h

        ; adjust cursor
        mov ah, 02h
        mov bh, 00h; set page number 0
        mov dx, 0000h; upper left
        int 10h

        ; show replenish vegetable menu
        mov ah, 09h
        lea dx, replenishVegetableMenu
        int 21h

        ; read vegetable choice
        mov ah, 01h
        int 21h

        ; vegetable choice == 1 ?
        cmp al, 31h
        je .addBroccoli

        ; vegetable choice == 2 ?
        cmp al, 32h
        je .addCabbage

        ; vegetable choice == 3 ?
        cmp al, 33h
        je .addCarrot

        ; vegetable choice == 4 ?
        cmp al, 34h
        je .addGarlic

        ; vegetable choice == 5 ?
        cmp al, 35h
        je .addMushroom

        ; vegetable choice == 6 ?
        cmp al, 36h
        je .addStockBack

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print invalid choice error message
        lea dx, choiceErrorMessage
        int 21h

        ; read key
        mov ah, 07h
        int 21h

        jmp .addVegetable

        .addBroccoli:
            call addBroccoli
            jmp .addVegetable

        .addCabbage:
            call addCabbage
            jmp .addVegetable

        .addCarrot:
            call addCarrot
            jmp .addVegetable

        .addGarlic:
            call addGarlic
            jmp .addVegetable

        .addMushroom:
            call addMushroom
            jmp .addVegetable

        .addStockBack:
            ; restore registers
            pop dx
            pop cx
            pop bx
            pop ax

            ret
addStock endp

addBroccoli proc near
    ; save registers
    push ax
    push cx
    push dx
    push di

    .addBroccoliInner:
        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print broccoli gram prompt
        lea dx, broccoliGPrompt
        int 21h

        ; read vegetable gram input
        mov ah, 0ah
        lea dx, vegetableGInput
        int 21h

        xor ch, ch; ch = 0
        mov cl, vegetableGInputActualSize
        xor si, si; si = 0; index

        .addBroccoliIsDigit:
            ; digit >= '0' ?
            cmp [vegetableGInputHolder + si], "0"
            jl .addBroccoliPrintNotIntegerErrorMessage

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addBroccoliPrintNotIntegerErrorMessage

            inc si
            loop .addBroccoliIsDigit

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        .addBroccoliProcessInt:
            lea ax, vegetableGInputHolder
            mov bx, si
            call parseInt

            add broccoliG, ax

            ; print current broccoli
            mov ah, 09h
            lea dx, currentBroccoli
            int 21h

            mov ax, broccoliG
            xor bx, bx; bx = 0
            mov cl, " "
            call printInt

            ; print ' '
            mov ah, 02h
            mov dl, 20h
            int 21h

            ; print 'G'
            mov dl, 47h
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .addBroccoliBack

        .addBroccoliPrintNotIntegerErrorMessage:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not integer error message
            lea dx, notIntegerErrorMessage
            int 21h

            jmp .addBroccoliInner

        .addBroccoliBack:
            ; restore registers
            pop di
            pop dx
            pop cx
            pop ax

            ret
addBroccoli endp

addCabbage proc near
    ; save registers
    push ax
    push cx
    push dx
    push di

    .addCabbageInner:
        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print cabbage gram prompt
        lea dx, cabbageGPrompt
        int 21h

        ; read vegetable gram input
        mov ah, 0ah
        lea dx, vegetableGInput
        int 21h

        xor ch, ch; ch = 0
        mov cl, vegetableGInputActualSize
        xor si, si; si = 0; index

        .addCabbageIsDigit:
            ; digit >= '0' ?
            cmp [vegetableGInputHolder + si], "0"
            jl .addCabbagePrintNotIntegerErrorMessage

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addCabbagePrintNotIntegerErrorMessage

            inc si
            loop .addCabbageIsDigit

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        .addCabbageProcessInt:
            lea ax, vegetableGInputHolder
            mov bx, si
            call parseInt

            add cabbageG, ax

            ; print current cabbage
            mov ah, 09h
            lea dx, currentCabbage
            int 21h

            mov ax, cabbageG
            xor bx, bx; bx = 0
            mov cl, " "
            call printInt

            ; print ' '
            mov ah, 02h
            mov dl, 20h
            int 21h

            ; print 'G'
            mov dl, 47h
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .addCabbageBack

        .addCabbagePrintNotIntegerErrorMessage:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not integer error message
            lea dx, notIntegerErrorMessage
            int 21h

            jmp .addCabbageInner

        .addCabbageBack:
            ; restore registers
            pop di
            pop dx
            pop cx
            pop ax

            ret
addCabbage endp

addCarrot proc near
    ; save registers
    push ax
    push cx
    push dx
    push di

    .addCarrotInner:
        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print carrot gram prompt
        lea dx, carrotGPrompt
        int 21h

        ; read vegetable gram input
        mov ah, 0ah
        lea dx, vegetableGInput
        int 21h

        xor ch, ch; ch = 0
        mov cl, vegetableGInputActualSize
        xor si, si; si = 0; index

        .addCarrotIsDigit:
            ; digit >= '0' ?
            cmp [vegetableGInputHolder + si], "0"
            jl .addCarrotPrintNotIntegerErrorMessage

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addCarrotPrintNotIntegerErrorMessage

            inc si
            loop .addCarrotIsDigit

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        .addCarrotProcessInt:
            lea ax, vegetableGInputHolder
            mov bx, si
            call parseInt

            add carrotG, ax

            ; print current carrot
            mov ah, 09h
            lea dx, currentCarrot
            int 21h

            mov ax, carrotG
            xor bx, bx; bx = 0
            mov cl, " "
            call printInt

            ; print ' '
            mov ah, 02h
            mov dl, 20h
            int 21h

            ; print 'G'
            mov dl, 47h
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .addCarrotBack

        .addCarrotPrintNotIntegerErrorMessage:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not integer error message
            lea dx, notIntegerErrorMessage
            int 21h

            jmp .addCarrotInner

        .addCarrotBack:
            ; restore registers
            pop di
            pop dx
            pop cx
            pop ax

            ret
addCarrot endp

addGarlic proc near
    ; save registers
    push ax
    push cx
    push dx
    push di

    .addGarlicInner:
        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print garlic gram prompt
        lea dx, garlicGPrompt
        int 21h

        ; read vegetable gram input
        mov ah, 0ah
        lea dx, vegetableGInput
        int 21h

        xor ch, ch; ch = 0
        mov cl, vegetableGInputActualSize
        xor si, si; si = 0; index

        .addGarlicIsDigit:
            ; digit >= '0' ?
            cmp [vegetableGInputHolder + si], "0"
            jl .addGarlicPrintNotIntegerErrorMessage

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addGarlicPrintNotIntegerErrorMessage

            inc si
            loop .addGarlicIsDigit

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        .addGarlicProcessInt:
            lea ax, vegetableGInputHolder
            mov bx, si
            call parseInt

            add garlicG, ax

            ; print current garlic
            mov ah, 09h
            lea dx, currentGarlic
            int 21h

            mov ax, garlicG
            xor bx, bx; bx = 0
            mov cl, " "
            call printInt

            ; print ' '
            mov ah, 02h
            mov dl, 20h
            int 21h

            ; print 'G'
            mov dl, 47h
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .addGarlicBack

        .addGarlicPrintNotIntegerErrorMessage:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not integer error message
            lea dx, notIntegerErrorMessage
            int 21h

            jmp .addGarlicInner

        .addGarlicBack:
            ; restore registers
            pop di
            pop dx
            pop cx
            pop ax

            ret
addGarlic endp

addMushroom proc near
    ; save registers
    push ax
    push cx
    push dx
    push di

    .addMushroomInner:
        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        ; print mushroom gram prompt
        lea dx, mushroomGPrompt
        int 21h

        ; read vegetable gram input
        mov ah, 0ah
        lea dx, vegetableGInput
        int 21h

        xor ch, ch; ch = 0
        mov cl, vegetableGInputActualSize
        xor si, si; si = 0; index

        .addMushroomIsDigit:
            ; digit >= '0' ?
            cmp [vegetableGInputHolder + si], "0"
            jl .addMushroomPrintNotIntegerErrorMessage

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addMushroomPrintNotIntegerErrorMessage

            inc si
            loop .addMushroomIsDigit

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

        .addMushroomProcessInt:
            lea ax, vegetableGInputHolder
            mov bx, si
            call parseInt

            add mushroomG, ax

            ; print current mushroom
            mov ah, 09h
            lea dx, currentMushroom
            int 21h

            mov ax, mushroomG
            xor bx, bx; bx = 0
            mov cl, " "
            call printInt

            ; print ' '
            mov ah, 02h
            mov dl, 20h
            int 21h

            ; print 'G'
            mov dl, 47h
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .addMushroomBack

        .addMushroomPrintNotIntegerErrorMessage:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not integer error message
            lea dx, notIntegerErrorMessage
            int 21h

            jmp .addMushroomInner

        .addMushroomBack:
            ; restore registers
            pop di
            pop dx
            pop cx
            pop ax

            ret
addMushroom endp

showCurrentStock proc near
    ; save registers
    push ax
    push bx
    push cx
    push dx

    ; clear screen
    mov ax, 0600h; ah = 06h (scroll)
                 ; al = 00h (full screen)
    mov bh, 07h; black background (0h)
               ; white foreground (7h)
    mov cx, 0000h; upper left
    mov dx, 184fh; lower right
    int 10h

    ; adjust cursor
    mov ah, 02h
    mov bh, 00h; set page number 0
    mov dx, 0000h; upper left
    int 10h

    ; print current stock title
    mov ah, 09h
    lea dx, currentStockTitle
    int 21h

    ; print current broccoli
    lea dx, currentBroccoli
    int 21h

    mov ax, broccoliG
    mov bx, 5
    mov cl, " "
    call printInt

    ; print ' '
    mov ah, 02h
    mov dl, 20h
    int 21h

    ; print 'G'
    mov dl, 47h
    int 21h

    ; print new line
    mov ah, 09h
    lea dx, newline
    int 21h

    ; print current cabbage
    lea dx, currentCabbage
    int 21h

    mov ax, cabbageG
    mov bx, 5
    mov cl, " "
    call printInt

    ; print ' '
    mov ah, 02h
    mov dl, 20h
    int 21h

    ; print 'G'
    mov dl, 47h
    int 21h

    ; print new line
    mov ah, 09h
    lea dx, newline
    int 21h

    ; print current carrot
    lea dx, currentCarrot
    int 21h

    mov ax, carrotG
    mov bx, 5
    mov cl, " "
    call printInt

    ; print ' '
    mov ah, 02h
    mov dl, 20h
    int 21h

    ; print 'G'
    mov dl, 47h
    int 21h

    ; print new line
    mov ah, 09h
    lea dx, newline
    int 21h

    ; print current garlic
    lea dx, currentGarlic
    int 21h

    mov ax, garlicG
    mov bx, 5
    mov cl, " "
    call printInt

    ; print ' '
    mov ah, 02h
    mov dl, 20h
    int 21h

    ; print 'G'
    mov dl, 47h
    int 21h

    ; print new line
    mov ah, 09h
    lea dx, newline
    int 21h

    ; print current mushroom
    lea dx, currentMushroom
    int 21h

    mov ax, mushroomG
    mov bx, 5
    mov cl, " "
    call printInt

    ; print ' '
    mov ah, 02h
    mov dl, 20h
    int 21h

    ; print 'G'
    mov dl, 47h
    int 21h

    ; print new line
    mov ah, 09h
    lea dx, newline
    int 21h

    ; print continue prompt
    lea dx, continuePrompt
    int 21h

    ; read key
    mov ah, 07h
    int 21h

    ; restore registers
    pop dx
    pop cx
    pop bx
    pop ax

    ret
showCurrentStock endp

end main