.model samll
.stack 64


.data
ten                     DW 10
galleryMenu             DB 10,13,10,13,10,13,"Menu",10,13
				        DB "1.Broccoli",10,13
			            DB "2.Cabbage",10,13
			            DB "3.Carrot",10,13
			            DB "4.Garlic",10,13
			            DB "5.Mushroom",10,13
			            DB "6.Back",10,13
			            DB "Pelase Enter the vegetable that you would like to buy (1-5):$"
					 
amount                  DB 10,13,"Please enter the amount you want to buy (in g)(MAXIMUM 5 digit):$"
invalidAmountMsg        DB 10,13,"We dont have enough stock, please enter a smaller number$"
invalid                 DB 10,13,"Invalid input","$"
broccoliBuy             DW 0
cabbageBuy              DW 0
carrotBuy               DW 0
garlicBuy               DW 0
mushroomBuy             DW 0
selection               DB ?
gBuyDigit               DW 0
nestedLoop              DW ?


broccoliG dw 10000
cabbageG  dw 10000
carrotG   dw 10000
garlicG   dw 10000
mushroomG dw 10000

inputGBuy LABEL byte
maxG     DB 6
currentG DB ?
gBuy     DB 6 dup(0)




.code
main proc far
	mov ax,@data
	mov ds,ax
	
start:
	;---print galery menu
	mov ah,09h
	LEA dx,galleryMenu
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
	lea dx,invalid
	int 21h
	jmp start	

finish:
	mov ax,4c00h
	int 21h

main endp



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
	sub al,30h
	mov nestedLoop,cx
	mov cx,di  ;di stand for power of 10,first loop 10^0, second loop 10^1 ......
	
convertToBase10: 
    
    cmp cx,0
    je continueConvert
	mul ten 
	loop convertToBase10
	
continueConvert:	  
    add ax,gBuyDigit
	mov gBuyDigit,ax
	
	dec si
	inc di
	mov cx,nestedLoop
	loop startConvert
	

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



end main