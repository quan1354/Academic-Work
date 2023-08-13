.model small
.stack 64
.data
;-----------------------Start login variable----------------------------------
	logo        db 0ah,0dh,"_________________________________________________________________",0ah,0dh
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
	invalidInput        db 10,13,"Invalid input","$"
	mainMenu            db 0ah,0dh,0ah,0dh,"Main Menu",0ah,0dh
						db "1. Gallery",0ah,0dh
						db "2. Stock ",0ah,0dh
						db "3. Payment",0ah,0dh
						db "4. Summary",0ah,0dh
						db "5. Log Out $"
	newline             db 0dh, 0ah, "$"
	choice              db ?
	choice2             db ?
	validStatus         dw 0
	wrongData           db 0ah,0dh,"Invalid username or data,please enter again$"
	
	inputUserName label byte
	maxUserLength db 7
	userLength    db ?
	username      db 7 dup("$")
	
	
	inputPassword label byte
	maxPasswordLength db 10
	passwordLength    db ?
	password          db 10 dup ("$")
;--------------------------------End log in variable-------------------------------
	
;--------------------------------start Gallery variable---------------------------
	galleryMenu             DB "Stock Menu",0ah,0dh
						    DB "1.Broccoli",0ah,0dh
						    DB "2.Cabbage",0ah,0dh
						    DB "3.Carrot",0ah,0dh
						    DB "4.Garlic",0ah,0dh
						    DB "5.Mushroom",0ah,0dh
						    DB "6.Back",0ah,0dh
						    DB "Pelase Enter the vegetable that you would like to buy (1-5):$"
	

					 

	broccoliBuy             DW 0
	cabbageBuy              DW 0
	carrotBuy               DW 0
	garlicBuy               DW 0
	mushroomBuy             DW 0
	selection               DB ?
	gBuyDigit               DW 0
	nestedLoop              DW ?
	
	amount                  DB 0ah,0dh,"Please enter the amount you want to buy (in g)(MAXIMUM 5 digit):$"
	invalidAmountMsg        DB 0ah,0dh,"We dont have enough stock, please enter a smaller number$"
	

	
	

	broccoliG dw 10000
	cabbageG  dw 10000
	carrotG   dw 10000
	garlicG   dw 10000
	mushroomG dw 10000

	inputGBuy LABEL byte
	maxG     DB 6
	currentG DB ?
	gBuy     DB 6 dup(0)
;-----------------------end Gallery variable---------------------------	

	
	

.code
main proc far
	mov ax,@data
	mov ds,ax

Login:	
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
	jne choice_2
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
	mov ah,0ah
	lea dx,inputPassword
	int 21h
	call validatePassword
	cmp validStatus,0
	je MainFunction
	mov ah,09h
	lea dx,wrongData
	int 21h
	jmp Login
	
	
	
choice_2:
	cmp choice,'2'
	jne choice_3
	jmp endFunction
	
	
choice_3:
	mov ah,09h
	lea dx,invalidInput
	int 21h
	jmp Login
	
	
MainFunction:
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
	call payment
	jmp MainFunction
	
choice2_4:
	cmp choice2,'4'
	jne choice2_5
	call summary
	jmp MainFunction
	
choice2_5:
	cmp choice2,'5'
	jne choice2_6
	jmp Login
	
choice2_6:
	mov ah,09h
	lea dx,invalidInput
	int 21h
	jmp MainFunction
	
	
	


endFunction:
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
	mov ch,0
	mov cl,passwordLength
	cmp cl,9
	jne invalidPassword
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
;---------------------------End Login Function------------------------------------------



;----------------------start Gallery Function-------------------------------------------
gallery proc 
	
start:
	;---print gallery menu
	mov ah,09h
	lea dx,newline
	int 21h
	lea dx,newline
	int 21h
	lea dx,newline
	int 21h
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
	mov ax,broccoliG
	sub ax,gBuyDigit
	cmp ax,0	;------check stock enough or not
	jl invalidAmount
	mov broccoliG,ax
	mov ax,broccoliBuy
	add ax,gBuyDigit
	mov broccoliBuy,ax
	jmp start

option2:;--------cabbage
	cmp selection,'2'
	jne option3
	call buyInput
	mov ax,cabbageG
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov cabbageG,ax
	mov ax,cabbageG
	add ax,gBuyDigit
	mov cabbageBuy,ax
	jmp start

option3:;---carrot	
	cmp selection,'3'
	jne option4
	call buyInput
	mov ax,carrotG
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov carrotG,ax
	mov ax,carrotBuy
	add ax,gBuyDigit
	mov carrotBuy,ax
	jmp start


;-------put at the middle due jmp limit
invalidAmount:
	mov ah,09h
	lea dx,invalidAmountMsg
	int 21h
	jmp start

option4:;---garlic
	cmp selection,'4'
	jne option5
	call buyInput
	mov ax,garlicG
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov garlicG,ax
	mov ax,garlicBuy
	add ax,gBuyDigit
	mov garlicBuy,ax
	jmp start
	
option5:;----mushroom
	cmp selection,'5'
	jne option6
	call buyInput
	mov ax,mushroomG
	sub ax,gBuyDigit
	cmp ax,0    ;------check stock enough or not
	jl invalidAmount
	mov mushroomG,ax
	mov ax,mushroomBuy
	add ax,gBuyDigit
	mov mushroomBuy,ax
	jmp start

option6:
	cmp selection,'6'
	je finish	

	mov ah,09h
	lea dx,invalidInput
	int 21h
	jmp start	

finish:
	ret

gallery endp



buyInput proc
	mov ah,09h
	lea dx,amount
	int 21h
	
	mov ah,0ah
	lea dx,inputGBuy
	int 21h
	call convertToDigit
	call clearInput
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


clearInput proc
	mov cx,0
	mov cl,maxG
	mov si,0
clear:
	mov gBuy[si],"0"
	inc si
	loop clear
	ret



clearInput endp

;--------------------------End Gallery Function-------------------------------------


;--------------------------Start stock Function-------------------------------------
stock proc 

stock endp
;--------------------------End stock Function---------------------------------------


;--------------------------Start payment Function-------------------------------------
payment proc

payment endp

;--------------------------End payment Function---------------------------------------



;--------------------------Start summary Function-------------------------------------
summary proc

summary endp
;--------------------------End summary Function---------------------------------------



end main