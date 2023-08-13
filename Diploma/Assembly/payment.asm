.model small
.stack 64

extrn round: proc
extrn printInt: proc
extrn newline: byte
extrn gKGConverter: word
extrn printAsKg: proc
extrn printAsRM: proc
extrn choiceErrorMessage: byte
extrn notPositiveNumberErrorMessage: byte
extrn broccoliG: word
extrn cabbageG: word
extrn carrotG: word
extrn garlicG: word
extrn mushroomG: word
extrn parseInt: proc
extrn pow: proc

.data

broccoliGBought dw 1234
cabbageGBought dw 1234
carrotGBought dw 1234
garlicGBought dw 1234
mushroomGBought dw 1234

broccoliCentsPerKG dw 1399
cabbageCentsPerKG dw 549
carrotCentsPerKG dw 779
garlicCentsPerKG dw 1529
mushroomCentsPerKG dw 2149

broccoliCentscostPerKG dw 600
cabbageCentscostPerKG dw 200
carrotCentscostPerKG dw 300
garlicCentscostPerKG dw 500
mushroomCentscostPerKG dw 800

broccoliGSold dw 0
cabbageGSold dw 0
carrotGSold dw 0
garlicGSold dw 0
mushroomGSold dw 0

broccoliSubtotal dw ?
cabbageSubtotal dw ?
carrotSubtotal dw ?
garlicSubtotal dw ?
mushroomSubtotal dw ?

choice db ?
subtotal dw 0
discountAmount dw 0
sst dw ?
total dw ?
payment dw ?
changed dw ?

receipt0 db "                   TCS Life Farm Sdn Bhd$"
receipt1 db "                          Receipt$"
receipt2 db "$"
receipt3 db "Receipt No.: $"
receipt4 db "                   Date: $"
receipt5 db "No.    Description    kg Bought    RM per kg    Amount(RM)$"
receipt6 db " 1     Broccoli       $"
receipt7 db " 2     Cabbage        $"
receipt8 db " 3     Carrot         $"
receipt9 db " 4     Garlic         $"
receipt10 db " 5     Mushroom       $"
receipt11 db "                                                ----------$"
receipt12 db "                                       Subtotal:$"
receipt13 db "                                       Discount:$"
receipt14 db "                                            SST:$"
receipt15 db "                                          Total:$"
receipt16 db "                                        Payment:   $"
receipt17 db "                                        Changed:$"

confirm db "Please select your choice", 0ah, 0dh
        db "1. Make Payment", 0ah, 0dh
        db "2. Cancel Order", 0ah, 0dh
        db "Enter a choice > $"

insufficientPaymentErrorMessage db "Insufficient Payment! Please try again.$"
welcome db "Thanks for your purchase, see you next time.", 0ah, 0dh, "$"
receiptNo dw 1
seperator db 4 dup(" "), "$"
tooManyIntegerDigitErrorMessage db "Number must have less than 4 integer digits! Please try again.$"
tooManyFractionDigitErrorMessage db "Number must have less than 3 decimal points! Please try again.$"

paymentInput label byte
paymentInputMaxSize db 7
paymentInputActualSize db ?
paymentInputHolder db 7 dup("$")

.code
main proc near
    ; init data segment
    mov ax, @data
    mov ds, ax

    ; calculate broccoli subtotal
    mov ax, broccoliGBought
    mov bx, broccoliCentsPerKG
    call calculateSubtotal
    mov broccoliSubtotal, ax    

    ; calculate cabbage subtotal
    mov ax, cabbageGBought
    mov bx, cabbageCentsPerKG
    call calculateSubtotal
    mov cabbageSubtotal, ax

    ; calculate carrot subtotal
    mov ax, carrotGBought
    mov bx, carrotCentsPerKG
    call calculateSubtotal
    mov carrotSubtotal, ax

    ; calculate garlic subtotal
    mov ax, garlicGBought
    mov bx, garlicCentsPerKG
    call calculateSubtotal
    mov garlicSubtotal, ax

    ; calculate mushroom subtotal
    mov ax, mushroomGBought
    mov bx, mushroomCentsPerKG
    call calculateSubtotal
    mov mushroomSubtotal, ax

    ; calculate subtotal
    mov ax, broccoliSubtotal
    add subtotal, ax
    mov ax, cabbageSubtotal
    add subtotal, ax
    mov ax, carrotSubtotal
    add subtotal, ax
    mov ax, garlicSubtotal
    add subtotal, ax
    mov ax, mushroomSubtotal
    add subtotal, ax

    ; subtotal >= 20000 ?
    mov ax, 20000
    cmp subtotal, ax
    jge .bigDiscount

    ; subtotal >= 10000 ?
    mov ax, 10000
    cmp subtotal, ax
    jge .smallDiscount

    jmp .calculateSST

    .bigDiscount:
        mov ax, 15; 15%
        call calculateDiscountAmount
        mov discountAmount, ax

        jmp .calculateSST

    .smallDiscount:
        mov ax, 5; 5%
        call calculateDiscountAmount
        mov discountAmount, ax

        jmp .calculateSST

    .calculateSST:
        mov ax, subtotal
        sub ax, discountAmount
        mov bx, 5; SST=5%
        mul bx
        mov bx, 100
        div bx

        mov bx, dx
        mov cx, 100
        call round
        mov sst, ax

    .calculateTotal:
        mov ax, subtotal
        sub ax, discountAmount
        mov bx, 105; (1 + SST=5%)
        mul bx
        mov bx, 100
        div bx

        mov bx, dx
        mov cx, 100
        call round
        mov total, ax
		
;--------------------------------------------------------------------------------------------------------
    .printReceipt:
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

        mov ah, 09h
        lea dx, receipt0
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        lea dx, receipt1
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        lea dx, receipt2
        int 21h

        ; print new line
        lea dx, newline
        int 21h

        lea dx, receipt3
        int 21h

        mov ax, receiptNo
        mov bx, 3
        mov cl, "0"
        call printInt

        mov ah, 09h
        lea dx, receipt4
        int 21h

        mov ah, 2ah
        int 21h

        mov si, cx; year

        xor ah, ah; ah = 0
        mov al, dl; day
        xor bx, bx; bx = 0
        mov cl, " "
        call printInt

        ; print "/"
        mov ah, 02h
        mov dl, "/"
        int 21h

        xor ah, ah; ah = 0
        mov al, dh; month
        xor bx, bx; bx = 0
        mov cl, " "
        call printInt

        ; print "/"
        mov ah, 02h
        mov dl, "/"
        int 21h

        mov ax, si; year
        xor bx, bx; bx = 0
        mov cl, " "
        call printInt

        ; print 2 new line
        mov ah, 09h
        lea dx, newline
        int 21h
        int 21h

;------------------------------------------------------------------------
        lea dx, receipt5
        int 21h

        ; print new line
        lea dx, newline
        int 21h

;-------------------------------Broccoli---------------------------------
        lea dx, receipt6
        int 21h

        mov ax, broccoliGBought
        mov bx, 9
        call printAsKg

        mov ah, 09h
        lea dx, seperator
        int 21h

        mov ax, broccoliCentsPerKG
        mov bx, 9
        call printAsRM
		
        mov ah, 09h
        lea dx, seperator
        int 21h

		mov ax, broccoliSubtotal
		mov bx, 9
		call printAsRM

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h
		
;-------------------------------Cabbage-----------------------------------
        lea dx, receipt7
        int 21h
		
		mov ax, cabbageGBought
        mov bx, 9
        call printAsKg

        mov ah, 09h
        lea dx, seperator
        int 21h
		
		mov ax, cabbageCentsPerKG
        mov bx, 9
        call printAsRM
		
        mov ah, 09h
        lea dx, seperator
        int 21h
		
		mov ax,cabbageSubtotal
		mov bx,9
		call printAsRM

        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

;--------------------------------Carrot-----------------------------------
        lea dx, receipt8
        int 21h
		
		mov ax, carrotGBought;
        mov bx, 9
        call printAsKg

        mov ah, 09h
        lea dx, seperator
        int 21h
		
		mov ax, carrotCentsPerKG
        mov bx, 9
        call printAsRM
		
		
        mov ah, 09h
        lea dx, seperator
        int 21h
		
		mov ax,carrotSubtotal
		mov bx,9
		call printAsRM
		
	    ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

;--------------------------------Garlic-----------------------------------
        lea dx, receipt9
        int 21h
		
		mov ax, garlicGBought;
        mov bx, 9
        call printAsKg

        mov ah, 09h
        lea dx, seperator
        int 21h

		mov ax, garlicCentsPerKG
        mov bx, 9
        call printAsRM
		
        mov ah, 09h
        lea dx, seperator
        int 21h
		
		mov ax,garlicSubtotal
		mov bx,9
		call printAsRM
		
        ; print new line
        mov ah, 09h
        lea dx, newline
        int 21h

;-------------------------------Mushroom----------------------------------
        lea dx, receipt10
        int 21h
		
		mov ax, mushroomGBought;
        mov bx, 9
        call printAsKg

        mov ah, 09h
        lea dx, seperator
        int 21h

		mov ax, mushroomCentsPerKG
        mov bx, 9
        call printAsRM
		
        mov ah, 09h
        lea dx, seperator
        int 21h
		
		mov ax,mushroomSubtotal
		mov bx,9
		call printAsRM
		
		; print new line
        mov ah, 09h
        lea dx, newline
        int 21h	
;-----------------------------------------------------------------------
		lea dx, receipt11
		int 21h
		
		; print new line
        lea dx, newline
        int 21h	
;-----------------------------------------------------------------------
		lea dx, receipt12
		int 21h
		
		mov ax, subtotal
		mov bx, 9
		call printAsRM
		
		; print new line
        mov ah, 09h
        lea dx, newline
        int 21h	
;-----------------------------------------------------------------------	
		lea dx, receipt13
		int 21h
		
		mov ax, discountAmount
		mov bx, 9
		call printAsRM
		
		; print new line
        mov ah, 09h
        lea dx, newline
        int 21h		
;-----------------------------SST----------------------------------------
		lea dx, receipt14
		int 21h
		
		mov ax, sst
		mov bx, 9
		call printAsRM
		
		; print new line
        mov ah, 09h
        lea dx, newline
        int 21h		
;------------------------------------------------------------------------
		lea dx, receipt11
		int 21h
		
		; print new line
        mov ah, 09h
        lea dx, newline
        int 21h	
;------------------------------------------------------------------------  
		lea dx, receipt15
		int 21h
		
		mov ax, total
		mov bx, 9
		call printAsRM
		
		; print new line
        mov ah, 09h
        lea dx, newline
        int 21h		      		
;-------------------------------------------------------------------------
        receiptMenu:
            ; clear confirmation menu
            mov ax, 0606h; ah = 06h (clear)
                         ; al = 06h (6 lines)
            mov bh, 07h; black background (0h)
                       ; white foreground (7h)
            mov cx, 1100h; row index 17, column index 0
            mov dx, 184fh; lower right
            int 10h

            ; adjust cursor
            mov ah, 02h
            mov bh, 00h; set page number 0
            mov dx, 1100h; row index 17, column index 0
            int 10h

            ; print confirmation menu
            mov ah, 09h
		    lea dx, confirm
		    int 21h
    
            ; read choice
	        mov ah, 01h
		    int 21h

		    mov choice, al
	        cmp choice, "1"
	        je .transaction
    
    	    cmp choice, "2"
		    je finish

            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h
    
            ; print invalid choice error message
            mov ah, 09h
            lea dx, choiceErrorMessage
            int 21h
    
            ; read key
            mov ah, 07h
            int 21h

            jmp receiptMenu

		.transaction:
            call transaction
            inc receiptNo

	    finish:
            ; exit(0)
            mov ax, 4c00h
            int 21h
main endp

; param ax: gram of vegetable bought
; param bx: cents per kg of vegetable
; return ax: subtotal
calculateSubtotal proc near
    ; save registers
    push bx
    push cx
    push dx

    xor dx, dx; dx = 0
    mul bx
    mov bx, gKGConverter
    div bx

    mov bx, dx
    mov cx, gKGConverter
    call round

    ; restore registers
    pop dx
    pop cx
    pop bx

    ret
calculateSubtotal endp

; param ax: discount percentage
; return ax: discont amount
calculateDiscountAmount proc near
    ; save registers
    push bx
    push cx
    push dx

    mov bx, ax; discount percentage

    xor dx, dx; dx = 0
    mov ax, subtotal
    mul bx
    mov bx, 100
    div bx

    mov bx, dx
    mov cx, 100
    call round

    ; restore registers
    pop dx
    pop cx
    pop bx

    ret
calculateDiscountAmount endp

transaction proc near
    ; setup stack frame
    push bp
    mov bp, sp
    sub sp, 2

    ; save registers
    push ax
    push bx
    push cx
    push dx
    push di

    .transactionInner:
        ; clear confirmation menu
        mov ax, 0606h; ah = 06h (clear)
                     ; al = 06h (6 lines)
        mov bh, 07h; black background (0h)
                   ; white foreground (7h)
        mov cx, 1100h; row index 17, column index 0
        mov dx, 184fh; lower right
        int 10h

        ; adjust cursor
        mov ah, 02h
        mov bh, 00h; set page number 0
        mov dx, 1100h; row index 17, column index 0
        int 10h

        mov ah, 09h
        lea dx, receipt16
        int 21h

        ; read payment input
        mov ah, 0ah
        lea dx, paymentInput
        int 21h

        xor ch, ch; ch = 0
        mov cl, paymentInputActualSize
        xor si, si; si = 0; index
        mov bx, 0ffffh; dotI = -1

        xor ax, ax
        mov [bp - 2], ax; fractionDigitCount

        .isCharValid:
            ; payment input char == '.' ?
            cmp [paymentInputHolder + si], "."
            je .isDotValid

            ; payment input char >= '0' ?
            cmp [paymentInputHolder + si], "0"
            jl .paymentNotPositiveNumberError

            ; payment input char <= '9' ?
            cmp [paymentInputHolder + si], "9"
            jg .paymentNotPositiveNumberError

            ; dotI != -1 ?
            cmp bx, 0ffffh
            jne .incrementFractionDigitCount

            ; index < 3 ?
            cmp si, 3
            je .tooManyIntegerDigitError

        .isCharValidDone:
            inc si
            loop .isCharValid
            jmp .transactionDone

        .isDotValid:
            ; dotI == -1 ?
            cmp bx, 0ffffh
            jne .paymentNotPositiveNumberError

            mov bx, si
            jmp .isCharValidDone

        .incrementFractionDigitCount:
            ; fraction digit count < 2 ?
            mov ax, 2
            cmp [bp - 2], ax
            je .tooManyFractionDigitError

            mov ax, [bp - 2]
            inc ax
            mov [bp - 2], ax
            jmp .isCharValidDone

        .paymentNotPositiveNumberError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            lea dx, notPositiveNumberErrorMessage
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .transactionInner

        .tooManyFractionDigitError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            lea dx, tooManyFractionDigitErrorMessage
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .transactionInner

        .tooManyIntegerDigitError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            lea dx, tooManyIntegerDigitErrorMessage
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .transactionInner

        .removeDot:
            xor ch, ch; ch = 0
            mov cl, paymentInputActualSize
            sub cx, bx
            mov di, bx; index

            .moveLeft:
                mov al, [paymentInputHolder + di + 1]
                mov [paymentInputHolder + di], al
                inc di
                loop .moveLeft

            mov bx, 0ffffh; dotI = -1
            dec si
            jmp .transactionDone

        .insufficientPayment:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            lea dx, insufficientPaymentErrorMessage
            int 21h

            ; read key
            mov ah, 07h
            int 21h

            jmp .transactionInner

        .transactionDone:
            ; dotI != -1 ?
            cmp bx, 0ffffh
            jne .removeDot

            lea ax, paymentInputHolder
            mov bx, si
            call parseInt
            mov payment, ax

            mov ax, 10
            mov bx, 2
            sub bx, [bp - 2]
            call pow
            mov bx, ax

            mov ax, payment
            mul bx
            mov payment, ax

            mov ax, payment
            mov bx, total
            cmp ax, bx
            jl .insufficientPayment

            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            lea dx, receipt11
            int 21h

            ; print new line
            lea dx, newline
            int 21h

            lea dx, receipt17
            int 21h

            mov ax, payment
            sub ax, total
            mov bx, 9
            call printAsRM

            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            lea dx, receipt11
            int 21h

            ; print new line
            lea dx, newline
            int 21h

            ; print welcome message
            lea dx, welcome
            int 21h

            mov ax, broccoliGBought
            add broccoliGSold, ax
            sub broccoliG, ax

            mov ax, cabbageGBought
            add cabbageGSold, ax
            sub cabbageG, ax

            mov ax, carrotGBought
            add carrotGSold, ax
            sub carrotG, ax

            mov ax, garlicGBought
            add garlicGSold, ax
            sub garlicG, ax

            mov ax, mushroomGBought
            add mushroomGSold, ax
            sub mushroomG, ax

            ; restore registers
            pop di
            pop dx
            pop cx
            pop bx
            pop ax

            ; remove stack frame
            add sp, 2
            mov sp, bp
            pop bp

            ret
transaction endp
end main