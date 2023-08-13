.model small
.stack 64
.data
;-----------------------Start login variable----------------------------------
	logo                db "_________________________________________________________________",0ah,0dh
	                    db "\__    ___|_   ___ \ /   _____/ \_   _____/____ _______  _____   ",0ah,0dh
	                    db "  |    |  /    \  \/ \_____  \   |    __) \__  \\_  __ \/     \  ",0ah,0dh
	                    db "  |    |  \     \____/        \  |     \   / __ \|  | \/  y y  \ ",0ah,0dh
	                    db "  |____|   \______  /_______  /  \___  /  (____  /__|  |__|_|  / ",0ah,0dh
		                db "                  \/        \/       \/        \/            \/  ",0ah,0dh
		 	            db "_________________________________________________________________",0ah,0dh
						db "1. login",0ah,0dh,"2. exit$"
						
	choose              db 0ah,0dh,"Enter your choice > $"
	validUsername       db "staff1$"
	validPassword       db "password1$"
	userStr             db 0ah,0dh,"Username: $"
	passwordStr         db 0ah,0dh,"password: $"
	passwordIndex       db 0ah
	invalidInput        db 10,13,"Invalid input",10,13,"$"
	mainMenu            db "Main Menu",0ah,0dh
						db "1. Gallery",0ah,0dh
						db "2. Stock ",0ah,0dh
						db "3. Payment",0ah,0dh
						db "4. Summary",0ah,0dh
						db "5. Log Out $"
	choice              db ?
	choice2             db ?
	validStatus         dw 0
	wrongData           db 0ah,0dh,"Invalid username or password, please enter again",10,13,"$"
    thankMessage        db "Thank you for using this POS system. Feel free to check out our other products  at "
                        db "https://tcs.com.my/products/$"
	
	inputUserName label byte
	maxUserLength db 7
	userLength    db ?
	username      db 7 dup("$")
	
	inputPassword label byte
	maxPasswordLength db 10
	passwordLength    db ?
	password          db 10 dup ("$")

    summaryAddr dw ?
;--------------------------------End log in variable-------------------------------
	
	
	
	
;--------------------------------start Gallery variable---------------------------
	galleryMenu             DB "Gallery Menu",0ah,0dh
						    DB "1.Broccoli",0ah,0dh
						    DB "2.Cabbage",0ah,0dh
						    DB "3.Carrot",0ah,0dh
						    DB "4.Garlic",0ah,0dh
						    DB "5.Mushroom",0ah,0dh
						    DB "6.Back",0ah,0dh
						    DB "Please Enter the vegetable that you would like to buy (1-5):$"

	broccoliBuy             DW 0
	cabbageBuy              DW 0
	carrotBuy               DW 0
	garlicBuy               DW 0
	mushroomBuy             DW 0

    selection               DB ?
	gBuyDigit               DW 0
	nestedLoop              DW ?
	gBuyValid               DB 0
	amount                  DB 0ah,0dh,"Please enter the amount you want to buy (in g)(MAXIMUM 5 digit):$"
	invalidAmountMsg        DB 0ah,0dh,"We dont have enough stock, please enter a smaller number",10,13,"$"

	inputGBuy LABEL byte
	maxG     DB 6
	currentG DB ?
	gBuy     DB 6 dup("$")
;-----------------------end Gallery variable---------------------------




;-----------------------start stock variable---------------------------
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
;------------------------end stock variable----------------------------




;-----------------------start payment variable-------------------------
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
        db "3. Back", 0ah, 0dh
        db "Enter a choice > $"

insufficientPaymentErrorMessage db "Insufficient Payment! Please try again.$"
welcome db "Thanks for your purchase, see you next time.", 0ah, 0dh, "$"
cancelMessage db "Cancelled the order.$"
receiptNo dw 1
seperator db 4 dup(" "), "$"
tooManyIntegerDigitErrorMessage db "Number must have less than 4 integer digits! Please try again.$"
tooManyFractionDigitErrorMessage db "Number must have less than 3 decimal points! Please try again.$"

paymentInput label byte
paymentInputMaxSize db 7
paymentInputActualSize db ?
paymentInputHolder db 7 dup("$")
;------------------------end payment variable--------------------------




;-----------------------start summary variable-------------------------
profitbroccoliCentPerKG dw 0
profitcabbageCentPerKG dw 0
profitcarrotCentPerKG dw 0
profitgaricCentPerKG dw 0
profitmushroomCentPerKG dw 0

totalProfitbroccoli dw 0
totalProfitcabbage dw 0
totalProfitcarrot dw 0
totalProfitgaric dw 0
totalProfitmushroom dw 0

totalProfit dw 0
summary1 db "The total order received are  : $"
summary2 db "The total profit for broccoli is : RM$"
summary3 db "The total profit for cabbage  is : RM$"
summary4 db "The total profit for carrot   is : RM$"
summary5 db "The total profit for garic    is : RM$"
summary6 db "The total profit for mushroom is : RM$"
summary7 db "Total profit of all vegetables are: RM$"
;------------------------end summary variable--------------------------




;------------------------start lib variable----------------------------
newline db 0dh, 0ah, "$"
choiceErrorMessage db "Invalid choice! Please try again.$"
notDigitErrorMessage db "Invalid input! It must be a digit! Please try again.$"
notPositiveIntegerErrorMessage db "Invalid input! It must be a positive integer! Please try again.$"
notPositiveNumberErrorMessage db "Invalid input! It must be a positive number! Please try again.$"
continuePrompt db "Press any key to continue.$"
gKGConverter dw 1000
centsRMConverter dw 100
;-------------------------end lib variable-----------------------------




.code
main proc far
	mov ax,@data
	mov ds,ax

    lea ax, summary
    mov summaryAddr, ax

Login:	
	
	
	call clearscreen
	mov ah,09h
	lea dx,logo
	int 21h
	
	mov ah,09h
	lea dx,choose
	int 21h
	
	mov ah,01h
	int 21h
	mov choice,al
	
	cmp choice,'1'
	jne choice_temp
	mov ax,0
	mov validStatus,ax
	
	mov ah,09h
	lea dx,userStr
	int 21h
	mov ah,0ah
	lea dx,inputUserName
	int 21h
	call validateUser
	
	mov ah,09h
	lea dx,passwordStr
	int 21h
	

	

choice_temp:
	cmp choice,'1'
	jne choice_2
	

	mov si,0
	mov bl,0ah
	mov passwordIndex,bl
	
enterpassword:
	mov ah,07h
	int 21h
	cmp al,8
	je backspace
	inc passwordIndex
	mov password[si],al
	inc si
	mov ah,02h
	mov dl,"*"
	int 21h
	cmp password[8],"$"
	je enterpassword
	jmp govalid
backspace:
	dec passwordIndex
	cmp passwordIndex,9h
	jne passwordlimit
	inc passwordIndex
	jmp enterpassword
	

passwordlimit:	
	;----set cursor
	mov ah,02h
	mov bh,0
	mov dh,0bh  ;set row
	mov dl,passwordIndex  ;set column
	int 10h
	
	mov ah,02h
	mov dl," "
	int 21h
	
	mov ah,02h
	mov bh,0
	mov dh,0bh  ;set row
	mov dl,passwordIndex  ;set column
	int 10h
	dec si
	jmp enterpassword
	
govalid:
	call validatePassword
	cmp validStatus,0
	je MainFunction
	mov ah,09h
	lea dx,wrongData
	int 21h
	call clrPassword
	call holdon
	
	jmp Login
	
	
	
choice_2:
	cmp choice,'2'
	jne choice_3
	jmp endFunction
	
	
choice_3:
	mov ah,09h
	lea dx,invalidInput
	int 21h
	
	;----hold on to let user see error massage
	mov ah,09h
	lea dx,continuePrompt
	int 21h
	
	mov ah,01h
	int 21h
	jmp Login
	
	
MainFunction:
	call clearscreen
	mov ah,09h
	lea dx,mainMenu
	int 21h
	lea dx,choose
	int 21h
	
	mov ah,01h
	int 21h
	mov choice2,al
	
	cmp choice2,'1'
	jne choice2_2
	call gallery
	jmp MainFunction
	
choice2_2:
	cmp choice2,'2'
	jne choice2_3
	call stock
	jmp MainFunction
	
choice2_3:
	cmp choice2,'3'
	jne choice2_4
	call paymentFunc
	jmp MainFunction
	
choice2_4:
	cmp choice2,'4'
	jne choice2_5
	call cs:summaryAddr
	jmp MainFunction
	
choice2_5:
	cmp choice2,'5'
	jne choice2_6
	call clrPassword
	jmp Login
	
choice2_6:
	mov ah,09h
	lea dx,invalidInput
	int 21h
	call holdon
	jmp MainFunction

endFunction:
    ; print 2 new line
    mov ah, 09h
    lea dx, newline
    int 21h
    int 21h

    ; print thanks message
    lea dx, thankMessage
    int 21h

    mov ax,4c00h
    int 21h
main endp
;------------------End Main Function----------------------------




;------------------start Login function------------------------
validateUser proc
	mov ch,0
	mov cl,userLength
	cmp cl,6
	jne invalidUser	
	mov si,0
	
checkUser:
	mov al,validUsername[si]
	cmp username[si],al
	jne invalidUser
	inc si
	loop checkUser
	
	ret
	
invalidUser:
	mov ax,validStatus
	inc ax
	mov validStatus,ax
	ret

validateUser endp


validatePassword proc
	mov cx,9
	mov si,0
	
checkPassword:
	mov al,validPassword[si]
	cmp password[si],al
	jne invalidPassword
	inc si
	loop checkPassword
	ret
	
invalidPassword:
	mov ax,validStatus
	inc ax
	mov validStatus,ax
	ret
	

validatePassword endp


clearscreen proc
	;---clear screen
	mov ax,0600h
	mov bh,07h ;black background white foreground
	mov cx,0 ;start row
	mov dx,184fh ;end row
	int 10h

	;----set cursor
	mov ah,02h
	mov bh,0
	mov dh,0h  ;set row
	mov dl,0h  ;set column
	int 10h
	ret
	
	
clearscreen endp


holdon proc
	mov ah,09h
	lea dx,continuePrompt
	int 21h
	
	mov ah,07h
	int 21h
	
	ret
holdon endp


clrPassword proc
	mov ch,0
	mov cl,maxPasswordLength
	mov si,0
	mov al,"$"
	dec cl
clra:
	mov password[si],al
	inc si
	loop clra

	ret
clrPassword endp
;---------------------------End Login Function------------------------------------------






;----------------------start Gallery Function-------------------------------------------
gallery proc 
	
start:
	call clearscreen
	;---print gallery menu
	mov ah,09h
	lea dx,galleryMenu
	int 21h
	
	;-----read user selection and amount wanted to buy
	mov ah,01h
	int 21h
	mov selection,al
	
	
;option1--------broccoli
	cmp selection,'1'
	jne option2
	call buyInput
	cmp gBuyValid,0
	jne invalidBuy
	mov ax,broccoliG
    sub ax,broccoliBuy
	sub ax,gBuyDigit
	cmp ax,0	;------check stock enough or not
	jl invalidAmount
	mov ax,broccoliBuy
	add ax,gBuyDigit
	mov broccoliBuy,ax
	jmp start

option2:;--------cabbage
	cmp selection,'2'
	jne option3
	call buyInput
	cmp gBuyValid,0
	jne invalidBuy
	mov ax,cabbageG
    sub ax,cabbageBuy
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov ax,cabbageBuy
	add ax,gBuyDigit
	mov cabbageBuy,ax
	jmp start

option3:;---carrot	
	cmp selection,'3'
	jne option4
	call buyInput
	cmp gBuyValid,0
	jne invalidBuy
	mov ax,carrotG
    sub ax,carrotBuy
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov ax,carrotBuy
	add ax,gBuyDigit
	mov carrotBuy,ax
	jmp start
	
;-------jmp limit
invalidBuy:
	mov ah,09h
	lea dx,invalidInput
	int 21h
	call holdon
	jmp start

;-------put at the middle due jmp limit
invalidAmount:
	mov ah,09h
	lea dx,invalidAmountMsg
	int 21h
	call holdon
	jmp start



option4:;---garlic
	cmp selection,'4'
	jne option5
	call buyInput
	cmp gBuyValid,0
	jne invalidBuy
	mov ax,garlicG
    sub ax,garlicBuy
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov ax,garlicBuy
	add ax,gBuyDigit
	mov garlicBuy,ax
	jmp start
	
option5:;----mushroom
	cmp selection,'5'
	jne option6
	call buyInput
	cmp gBuyValid,0
	jne invalidBuy
	mov ax,mushroomG
    sub ax,mushroomBuy
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov ax,mushroomBuy
	add ax,gBuyDigit
	mov mushroomBuy,ax
	jmp start

option6:
	cmp selection,'6'
	je galleryFinish	
	
option7:
	mov ah,09h
	lea dx,invalidInput
	int 21h
	call holdon
	jmp start

	

galleryFinish:
	ret

gallery endp

buyInput proc
    mov ah,09h
	lea dx,amount
	int 21h
	
	mov ah,0ah
	lea dx,inputGBuy
	int 21h
	
	mov ch,0
	mov si,0
	mov gBuyValid,ch
	mov cl,currentG

;----check the value input is it digit	
checkGBuyValid:
	cmp gBuy[si],30h
	jb invalidGBuy
	cmp gBuy[si],39h
	ja invalidGBuy
	inc si
loop checkGBuyValid
	call convertToDigit
	jmp doneInput
	
invalidGBuy:
	mov al,1
	mov gBuyValid,al

doneInput:	
	ret
buyInput endp

convertToDigit proc
	mov ax,0
	mov bx,10
	mov gBuyDigit,ax
	mov al,currentG
	mov si,ax
	dec si
	mov ch,0
	mov cl,currentG
	mov di,0   
	
startConvert:
    mov dx,0
	mov ax,0
	mov al,gBuy[si]
	cmp al,30h
	jl return
	cmp al,39h
	jg return
	
	sub al,30h
	mov nestedLoop,cx
	mov cx,di  ;di stand for power of 10,first loop 10^0, second loop 10^1 ......
	
convertToBase10: 
    
    cmp cx,0
    je continueConvert
	mul bx
	loop convertToBase10
	
continueConvert:	  
    add ax,gBuyDigit
	mov gBuyDigit,ax
	
	dec si
	inc di
	mov cx,nestedLoop
	loop startConvert
	
	mov ah,09h
	lea dx,newline
	int 21h

return:
	ret

convertToDigit endp

;--------------------------End Gallery Function-------------------------------------







;--------------------------Start stock Function-------------------------------------
stock proc near
    ; save registers
    push ax
    push bx
    push cx
    push dx

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
            ; restore registers
            pop dx
            pop cx
            pop bx
            pop ax

            ret
stock endp

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
            jl .addBroccoliNotPositiveIntegerError

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addBroccoliNotPositiveIntegerError

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

        .addBroccoliNotPositiveIntegerError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not positive integer error message
            lea dx, notPositiveIntegerErrorMessage
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
            jl .addCabbageNotPositiveIntegerError

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addCabbageNotPositiveIntegerError

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

        .addCabbageNotPositiveIntegerError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not positive integer error message
            lea dx, notPositiveIntegerErrorMessage
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
            jl .addCarrotNotPositiveIntegerError

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addCarrotNotPositiveIntegerError

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

        .addCarrotNotPositiveIntegerError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not positive integer error message
            lea dx, notPositiveIntegerErrorMessage
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
            jl .addGarlicNotPositiveIntegerError

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addGarlicNotPositiveIntegerError

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

        .addGarlicNotPositiveIntegerError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not positive integer error message
            lea dx, notPositiveIntegerErrorMessage
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
            jl .addMushroomNotPositiveIntegerError

            ; digit <= '9' ?
            cmp [vegetableGInputHolder + si], "9"
            jg .addMushroomNotPositiveIntegerError

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

        .addMushroomNotPositiveIntegerError:
            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print not positive integer error message
            lea dx, notPositiveIntegerErrorMessage
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
;--------------------------End stock Function---------------------------------------

;--------------------------Start payment Function-------------------------------------
paymentFunc proc near
    ; save registers
    push ax
    push bx
    push cx
    push dx
    push si

    ; calculate broccoli subtotal
    mov ax, broccoliBuy
    mov bx, broccoliCentsPerKG
    call calculateSubtotal
    mov broccoliSubtotal, ax    

    ; calculate cabbage subtotal
    mov ax, cabbageBuy
    mov bx, cabbageCentsPerKG
    call calculateSubtotal
    mov cabbageSubtotal, ax

    ; calculate carrot subtotal
    mov ax, carrotBuy
    mov bx, carrotCentsPerKG
    call calculateSubtotal
    mov carrotSubtotal, ax

    ; calculate garlic subtotal
    mov ax, garlicBuy
    mov bx, garlicCentsPerKG
    call calculateSubtotal
    mov garlicSubtotal, ax

    ; calculate mushroom subtotal
    mov ax, mushroomBuy
    mov bx, mushroomCentsPerKG
    call calculateSubtotal
    mov mushroomSubtotal, ax

    ; calculate subtotal
    mov subtotal, 0
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

        mov ax, broccoliBuy
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
		
		mov ax, cabbageBuy
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
		
		mov ax, carrotBuy
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
		
		mov ax, garlicBuy
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
		
		mov ax, mushroomBuy
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
            mov ax, 0607h; ah = 06h (clear)
                         ; al = 07h (7 lines)
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
            je .resetPayment

    	    cmp choice, "3"
		    je .paymentFinish

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
            call resetPayment
            jmp .paymentFinish

        .resetPayment:
            call resetPayment

            ; print new line
            mov ah, 09h
            lea dx, newline
            int 21h

            ; print cancel message
            lea dx, cancelMessage
            int 21h

	    .paymentFinish:
            ; restore registers
            pop si
            pop dx
            pop cx
            pop bx
            pop ax

            ret
paymentFunc endp

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

            mov ax, broccoliBuy
            add broccoliGSold, ax
            sub broccoliG, ax

            mov ax, cabbageBuy
            add cabbageGSold, ax
            sub cabbageG, ax

            mov ax, carrotBuy
            add carrotGSold, ax
            sub carrotG, ax

            mov ax, garlicBuy
            add garlicGSold, ax
            sub garlicG, ax

            mov ax, mushroomBuy
            add mushroomGSold, ax
            sub mushroomG, ax

            ; read key
            mov ah, 07h
            int 21h

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

resetPayment proc near
    ; save registers
    push ax
    push dx

    mov broccoliBuy, 0
    mov cabbageBuy, 0
    mov carrotBuy, 0
    mov garlicBuy, 0
    mov mushroomBuy, 0

    mov broccoliSubtotal, 0
    mov cabbageSubtotal, 0
    mov carrotSubtotal, 0
    mov garlicSubtotal, 0
    mov mushroomSubtotal, 0

    mov subtotal, 0
    mov discountAmount, 0
    mov sst, 0
    mov total, 0

    ; restore registers
    pop dx
    pop ax

    ret
resetPayment endp
;--------------------------End payment Function---------------------------------------

;--------------------------Start summary Function-------------------------------------
summary proc near
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

	;------------------------------------Profit per kg = Price per kg – Cost per kg
	mov ax,broccoliCentsPerKG
	sub ax,broccoliCentscostPerKG
	mov profitbroccoliCentPerKG,ax
	
	mov ax,cabbageCentsPerKG
	sub ax,cabbageCentscostPerKG
	mov profitcabbageCentPerKG,ax
	
	mov ax,carrotCentsPerKG
	sub ax,carrotCentscostPerKG
	mov profitcarrotCentPerKG,ax
	
	mov ax,garlicCentsPerKG
	sub ax,garlicCentscostPerKG
	mov profitgaricCentPerKG,ax
	
	mov ax,mushroomCentsPerKG
	sub ax,mushroomCentscostPerKG
	mov profitmushroomCentPerKG,ax

;----------------------------------------Total Profit of a Vegetable Type = kg Sold * Profit per kg
    mov ax,profitbroccoliCentPerKG
	mov bx,broccoliGSold
	mul bx
    mov bx,gKgConverter
    div bx

    mov bx,dx
    mov cx,gKgConverter
    call round
	mov totalProfitbroccoli,ax
	
	mov ax,profitcabbageCentPerKG
	mov bx,cabbageGSold
	mul bx
    mov bx,gKgConverter
    div bx

    mov bx,dx
    mov cx,gKgConverter
    call round
	mov totalProfitcabbage,ax

	mov ax,profitcarrotCentPerKG
	mov bx,carrotGSold
	mul bx
    mov bx,gKgConverter
    div bx

    mov bx,dx
    mov cx,gKgConverter
    call round
	mov totalProfitcarrot,ax
	
	mov ax,profitgaricCentPerKG
	mov bx,garlicGSold
	mul bx
    mov bx,gKgConverter
    div bx

    mov bx,dx
    mov cx,gKgConverter
    call round
	mov totalProfitgaric,ax

	mov ax,profitmushroomCentPerKG
	mov bx,mushroomGSold
	mul bx
    mov bx,gKgConverter
    div bx

    mov bx,dx
    mov cx,gKgConverter
    call round
	mov totalProfitmushroom,ax
	
;----------------------------------------Total Profit of The Day = Total Profit of Broccolis + Total Profit of Cabbages +Total Profit of Carrots + Total Profit of Garlics + Total Profit of Mushrooms – Total Discount
   xor ax, ax; ax = 0
   add ax,totalProfitbroccoli
   add ax,totalProfitcabbage
   add ax,totalProfitcarrot
   add ax,totalProfitgaric
   add ax,totalProfitmushroom
   sub ax,discountAmount
   mov totalProfit,ax
   
;---------------------------------------display----------------------------------------
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h

   lea dx, summary1
   int 21h
   
   mov ax, receiptNo
   dec ax
   mov bx, 0
   call printInt
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h	
;------------------------------------------broccoli-----------------------------------------
   lea dx,summary2
   int 21h
   
   mov ax, totalProfitbroccoli
   mov bx, 9
   call printAsRM
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h	
;-------------------------------------------cabbage---------------------------------------
   lea dx,summary3
   int 21h
   
   mov ax, totalProfitcabbage
   mov bx, 9
   call printAsRM
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h	
;-------------------------------------------carrot----------------------------------------
   lea dx,summary4
   int 21h
   
   mov ax, totalProfitcarrot
   mov bx, 9
   call printAsRM
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h	
;---------------------------------------------garlic--------------------------------------
   lea dx,summary5
   int 21h
   
   mov ax, totalProfitgaric
   mov bx, 9
   call printAsRM
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h	
;---------------------------------------------mushroom--------------------------------------
   lea dx,summary6
   int 21h
   
   mov ax, totalProfitmushroom
   mov bx, 9
   call printAsRM
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h
;---------------------------------------------------------------------------------------------  
   lea dx,summary7
   int 21h
   
   mov ax, totalProfit
   mov bx, 9
   call printAsRM
   
   ; print new line
   mov ah, 09h
   lea dx, newline
   int 21h
;---------------------------------------------------------------------------------------------    

    ; read key
    mov ah, 07h
    int 21h

    ; restore registers
    pop dx
    pop cx
    pop bx
    pop ax

    ret
summary endp
;--------------------------End summary Function---------------------------------------

;----------------------------start lib function---------------------------------------
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
;-----------------------------end lib function----------------------------------------

end main