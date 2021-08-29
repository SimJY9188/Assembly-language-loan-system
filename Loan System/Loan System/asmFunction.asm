.386
.model flat, stdcall

INCLUDE Irvine32.inc

.stack 4096

ExitProcess PROTO, dwExitCode: DWORD

changeFontRed PROTO C
setDefaultFont PROTO C
chkYear PROTO C, strYear: DWORD
getYear PROTO C
getValue PROTO C, a: DWORD, b: DWORD, c1: DWORD, d: DWORD, e: DWORD
storeValue PROTO C
printBalanceStatment PROTO C
chkAction PROTO C, strAction: DWORD
getID PROTO C, inputID: DWORD
searchID PROTO C, inputPass: DWORD
checkApplication PROTO C
applicationRejectAction PROTO C
getAdminLoginResult PROTO C, adminRes: DWORD
getUserLoginResult PROTO C, userRes: DWORD
getLogout PROTO C, logout: DWORD
adminApproval PROTO C
clearScreen PROTO C
getLevelSalary PROTO C
getLevel PROTO C
getSalary1 PROTO C
getSalary2 PROTO C
fill_info PROTO C
;yx

getAdminID PROTO C, inputID: DWORD
searchAdminID PROTO C, password:dword
forgottenPassword PROTO C,userType:dword
chkPaymentStatus PROTO C
resetPayBackStatus PROTO C
changePayBackStatus PROTO C
checkCardCvv PROTO C, cvv:dword
getAmount PROTO C,totalPayBackAmts:dword
checkBankCardNumber PROTO C,cardNo:dword
getAllThreeAmount PROTO C,totalPayBackAmts:dword
paymentInfoUpdate PROTO C,totalPayBackAmount:dword,monthlyPayBackAmount:dword,dueAmount:dword
enterAmount PROTO C, payAmount:dword

.data

	adminLogin DB "  ----- Login -----", 0
	enterID DB "  ID      : ", 0
	enterPass DB "  Password: ", 0
	max = 20
	input_pass DWORD max+1 DUP (?)
	input_id DWORD 12 DUP (?)
	adminID DB "AD01LOG", 0
	adminPassword DB "v|jwy~zntu", 0
	correctAdminPass DB " =======Successfully Login to Admin Account========", 0
	correctUserPass DB " =======Successfully Login to Student Account========", 0
	idNotExist DB "  ID does not exist, please enter again.", 0
	wrongPass DB "  Incorrect password, please enter again.", 0
	forgotPass DB "  *Forgotten your password?*",0
	checkForgotPass DB "  Enter 'y' for yes, press any button for no >>> ",0
	mainMenu DB "  [  MAIN MENU  ]", 0
	adminAction1 DB "  1. Application Approval", 0
	adminAction2 DB "  2. Log out", 0
	strAdminAction DB 21 DUP (?)
	userAction1 DB "  1. Application", 0
	userAction2 DB "  2. Pay Back", 0
	userAction3 DB "  3. Check Balance Statement", 0
	userAction4 DB "  4. Log out", 0
	promptAction DB "  Enter action: ", 0
	errorAction DB "  Invalid action input, please enter again.", 0
	strUserAction DB 21 DUP (?)

	noApplication DB "  No application for checking.", 0
	noStatement DB "  Your statement has not been generated.", 0
	waitApproval DB "  Waiting for ADMIN to approved your application, please come back after awhile.", 0
	rejectedMsg DB "  Your application has been rejected.", 0
	approvedMessage DB "  Your application have been approved.", 0
	promptYear DB "  Enter number of years to pay back (between 1 - 15): ", 0
	errorYear DB "  Invalid years input, please enter again.", 0
	stryear DB 21 DUP (?)
	chkDiploma DB ?, 0
	chkDegree DB ?, 0

	SALARY_GUAR1 DWORD ?, 0
	SALARY_GUAR2 DWORD ?, 0
	INTEREST_RATE DWORD ?, 0
	borrowAmount DWORD ?, 0
	interestAmount DWORD ?, 0
	paybackAmount DWORD ?, 0

	promptstatement DB " Total Amount Payback per Month : ", 0

	;yx
	userType dd ?
	divisor dd 64h
	monthlyPayBackAmount dd ?
	totalPayBackAmount dd 0
	bankNo dd 16 DUP(?)
	cvv dd ?
	payAmount dd ?
	dueAmount dd ?
	paymentStatus dd ?
	prompt0 byte "  Choose your action.",0
	prompt1 byte "  1.Payback",0
	prompt2 byte "  2.Back",0
	prompt3 byte "  Your choice>> ",0
	prompt4 byte "  Your amount to pay this month : RM",0
	prompt5 byte "  Total Amount left to pay back : RM",0
	prompt6 byte "  Proceed to payment",0
	prompt7 byte "  1.Yes",0
	prompt8 byte "  2.No",0
	prompt9 byte "  Enter your Mastercard/VISA bank card number(Enter 'x' to go back)>>> ",0
	prompt10 byte "  Enter CVV for the card(Enter '0' to go back)>>> ",0
	prompt11 byte "  Payment is done successfully.",0
	prompt12 byte "  Updated Total Amount to pay back :RM",0
	prompt13 byte "  Payment Amount : RM",0
	prompt14 byte "  You have paid back all your loan",0
	prompt15 byte "  Your amount to pay this month after adding due amount : RM",0
	prompt16 byte "  Enter amount to pay >>> ",0
	prompt17 byte "  Due amount : RM",0
	errorPrompt byte "  Invalid input.Please enter only integer 1 or 2.",0
	errorPrompt1 byte "  Invalid bank number, please try again.",0
	errorPrompt2 byte "  Invalid CVV, please try again.",0
	errorPrompt3 byte "  You have entered wrongly cvv 3 times, Please try again later.",0
	errorPrompt4 byte "  You have already paid for this month. Please wait for next month",0
	errorPrompt5 byte "  You have not borrow any money yet.", 0

.code

LoginProcess PROC C
	; prompt user ID for Login
	LOGIN: call crlf
		   mov edx, OFFSET adminLogin
		   call WriteString
		   call crlf
		   mov edx, OFFSET enterID
		   call WriteString
		   mov edx, OFFSET input_id
		   mov ecx, 11
		   call ReadString
		   invoke getAdminID,edx
		   ; compare ID input with Admin ID first
		   mov ecx, 7
		   mov ebx, 0
		   CHK_ID: mov eax, input_id[ebx]
				   cmp adminID[ebx], al
				   jne GET_PASSWORD
				   inc ebx
				   LOOP CHK_ID

		   ; prompt user Password for Login
		   mov edx, OFFSET adminPassword
		   invoke searchAdminID,edx
		   mov edx, OFFSET enterPass
		   call WriteString
		   mov edx, OFFSET input_pass
		   mov ecx, max
		   call ReadString
		   
		   ; compare Password input with Admin Password first
		   mov edx, OFFSET adminPassword
		   invoke getAdminID,edx
		   mov ecx, lengthof adminPassword
		   mov ebx, 0
		   mov userType,0
		   CHK_PASS: mov eax, input_pass[ebx]
					 cmp adminPassword[ebx], al
					 jne WRONG_PASS
					 inc ebx
					 LOOP CHK_PASS

		   ; jump to admin module after ID and Password check are valid
		   jmp ADMIN_SUCCESSFUL
			
		   ; if the ID is not the Admin ID, check with user ID
		   GET_PASSWORD: mov edx, OFFSET input_id
						 INVOKE getID, edx
						 mov edx, OFFSET enterPass
					     call WriteString
					     mov edx, OFFSET input_pass
					     mov ecx, max
					     call ReadString
					     INVOKE searchID, edx
						 mov userType,1
						 ; if the ID and Password for user both correct, jump to user module
					     cmp al, 0
					     je USER_SUCCESSFUL

						 ; if input invalid, jump to respective label
					     cmp al, 1
					     je ID_NOTEXIST
					     cmp al, 2
					     je WRONG_PASS

		   ; if the input wrong, need to print error message and loop the login process
		   WRONG_PASS: mov edx, OFFSET wrongPass
					   call WriteString
					   call crlf
					   call crlf
					   mov edx, OFFSET forgotPass
					   call WriteString
					   call crlf
					   mov edx, OFFSET checkForgotPass
					   call WriteString
					   mov eax,0
					   call readchar 
					   call writechar
					   cmp al,79h
					   je Forgot_Pass
					   call crlf
					   jmp LOGIN

			Forgot_Pass:
						call crlf
						invoke forgottenPassword,userType
						jmp LOGIN
		   ID_NOTEXIST: mov edx, OFFSET idNotExist
					    call WriteString
					    call crlf
					    call crlf
					    jmp LOGIN

	ADMIN_SUCCESSFUL: mov eax, 1
					  INVOKE getAdminLoginResult, eax
					  jmp exitP

	USER_SUCCESSFUL: mov eax, 1
					 INVOKE getUserLoginResult, eax

	exitP:

	ret

LoginProcess ENDP


; -------------------------------------------------------------------------------
; Admin Module start here

AdminModule PROC C

	; print successfully login to admin account in the screen
	INVOKE clearScreen
	call crlf
	mov edx, OFFSET correctAdminPass
	call WriteString
	call crlf

	START_ADMIN:
	call crlf
	mov edx, OFFSET mainMenu
	call WriteString
	call crlf
	mov edx, OFFSET adminAction1
	call WriteString
	call crlf
	mov edx, OFFSET adminAction2
	call WriteString
	call crlf
	call crlf
	PROMPTA_ACTION: mov edx, OFFSET promptAction
				    call WriteString

					; get admin action as string and pass to cpp for validation
				    mov edx, OFFSET strAdminAction
				    mov ecx, 20
				    call ReadString
					INVOKE chkAction, edx
					cmp al, 0
					je ERRORA_ACTION
					cmp al, 1
				    je CHK_APPLICATION
					
					; if input 3 or 4, treat as error in admin module
					cmp al, 3
				    je ERRORA_ACTION
					cmp al, 4
					je ERRORA_ACTION
					call crlf
					call crlf
					cmp al, 2
				    je LOGOUT_1

	; print error message and loop prompt action if input invalid
	ERRORA_ACTION: mov edx, OFFSET errorAction
				   call WriteString
			       call crlf
			       call crlf
			       jmp PROMPTA_ACTION

	; check if there are any application for approval
	CHK_APPLICATION: INVOKE adminApproval
				     cmp al, 1
					 je NO_APPLICATION
					 cmp al, 0
					 INVOKE clearScreen
					 je START_ADMIN

	; if no application for approval, print a message on screen
	NO_APPLICATION: call crlf
					mov edx, OFFSET noApplication
					call WriteString
					call crlf
					call crlf
					INVOKE clearScreen
					jmp START_ADMIN

	; log out from admin account, jump to the login process
	LOGOUT_1: mov eax, 1
			  INVOKE getLogout, eax

	ret

AdminModule ENDP

; ------------------------------------------------------------------------
; User Module start here

UserModule PROC C

	; print successfully login to user account in the screen
	INVOKE clearScreen
	call crlf
	mov edx, OFFSET correctUserPass
	call WriteString
	call crlf

	START_USER: 
	call crlf
	mov edx, OFFSET mainMenu
	call WriteString
	call crlf
	mov edx, OFFSET userAction1
	call WriteString
	call crlf
	mov edx, OFFSET userAction2
	call WriteString
	call crlf
	mov edx, OFFSET userAction3
	call WriteString
	call crlf
	mov edx, OFFSET userAction4
	call WriteString
	call crlf
	call crlf
	PROMPTU_ACTION: mov edx, OFFSET promptAction
				    call WriteString

					; get user action as string
				    mov edx, OFFSET strUserAction
				    mov ecx, 20
				    call ReadString
				    INVOKE chkAction, edx
				    cmp al, 0
				    je ERRORU_ACTION
				    cmp al, 1
				    je INFO
					cmp al, 2
					je Start
					cmp al, 3
					je PRINT_STATEMENT
					call crlf
					call crlf
				    cmp al, 4
				    je LOGOUT_2

	; print error message and loop prompt action if input invalid
	ERRORU_ACTION: mov edx, OFFSET errorAction
				   call WriteString
			       call crlf
			       call crlf
			       jmp PROMPTU_ACTION

	; check if the application has been made or already approved/rejected
	INFO: INVOKE checkApplication
		  cmp al, 0
		  je INFO_FILL
		  cmp al, 1
		  je WAIT_MESSAGE
		  cmp al, 2
		  je PROMPT_YEAR
		  cmp al, 3
		  je APPROVED_MESSAGE
	
	; if return 4, ask user whether want to fill application info again
	call crlf
	INVOKE changeFontRed
	mov edx, OFFSET rejectedMsg
	call WriteString
	INVOKE setDefaultFont 
	call crlf
	call crlf
	REJECTED_APP: INVOKE applicationRejectAction
				  cmp al, 1
				  je REJECTED_APP
				  cmp al, 2
				  je INFO_FILL

				  ; return 0 if user enter 'N'
				  cmp al, 0
				  INVOKE clearScreen
				  je START_USER

	; if haven't apply for the loan
	INFO_FILL: INVOKE fill_info
			   cmp al, 1
			   je START_USER
			   jmp START_USER

	; if apply already but haven't check by admin, print waiting message on screen
	WAIT_MESSAGE: call crlf
				  mov edx, OFFSET waitApproval
				  call WriteString
				  call crlf
				  call crlf
				  call crlf
				  jmp START_USER
	
	; if already approved and all the calculation has done, only print message without prompt year for further calculation
	APPROVED_MESSAGE: call crlf
					  mov edx, OFFSET approvedMessage
					  call WriteString
					  call crlf
					  call crlf
					  call crlf
					  jmp START_USER

	; after the application been approved, get years from user
	PROMPT_YEAR: call crlf
				 mov edx, OFFSET approvedMessage
				 call WriteString
				 call crlf
				 call crlf
				 mov edx, OFFSET promptYear
				 call WriteString

				 ; get year as string and pass to cpp for validation
				 mov edx, OFFSET strYear
				 mov ecx, 20
				 call ReadString
				 INVOKE chkYear, edx
				 cmp al, 0
				 je ERROR_YEAR

				 ; eax = interest rate based on the year input
				 mov INTEREST_RATE, eax
				 jmp CALCULATION

	; if the input year invalid, print error message
	ERROR_YEAR: mov edx, OFFSET errorYear
			    call WriteString
			    call crlf
			    call crlf
			    jmp PROMPT_YEAR
	
	; get level (Diploma/Degree) and sum of both guardian to decide amount to borrow
	CALCULATION:
	INVOKE getLevelSalary
	INVOKE getLevel
	cmp al, 0
	je MOVE_DEGREE
	cmp al, 1
	je MOVE_DIPLOMA

	MOVE_DEGREE: mov chkDegree, 1
				 jmp PROCEED

	MOVE_DIPLOMA: mov chkDiploma, 1

	PROCEED: INVOKE getSalary1
			 mov SALARY_GUAR1, eax
			 INVOKE getSalary2
			 mov ebx, SALARY_GUAR1
			 add eax, ebx

			 ; check both guardian salary
			 cmp eax, 2000
			 ja CHECK_1

			 ; salary <= 2000
			 cmp chkDiploma, 1
			 jne B1_DEGREE
			 mov borrowAmount, 13000
			 jmp INTEREST

			 B1_DEGREE: mov borrowAmount, 35000
						jmp INTEREST
	
			 CHECK_1: cmp eax, 7000
					  ja CHECK_2
			
					  ; salary <= 7000
					  cmp chkDiploma, 1
					  jne B2_DEGREE
					  mov borrowAmount, 10000
					  jmp INTEREST

					  B2_DEGREE: mov borrowAmount, 32000
						 		 jmp INTEREST

			 ; salary <= 15000
			 CHECK_2: cmp chkDiploma, 1
					  jne B3_DEGREE
					  mov borrowAmount, 7000
					  jmp INTEREST

			 B3_DEGREE: mov borrowAmount, 28000

			 ; calculate total interest based on interest rate
			 INTEREST: mov eax, borrowAmount
					   mov ebx, 100
					   xor edx, edx
					   div ebx
					   mov ebx, INTEREST_RATE
					   mul ebx
					   mov interestAmount, eax

			 ; get total amount to pay back
			 mov edx, borrowAmount
			 add eax, edx
			 mov paybackAmount, eax

			 ; calculate amount to pay per month
			 mov eax, 0
			 INVOKE getYear					; eax = year
			 mov ebx, 12
			 mul ebx
			 mov ecx, eax
			 mov eax, paybackAmount
			 mov ebx, 1000
			 mul ebx  
			 div ecx
			 INVOKE getValue, borrowAmount, INTEREST_RATE, interestAmount, paybackAmount, eax
			 INVOKE storeValue

	; if user enter 3 at the main menu, directly jump here to print out the statement
	; after getting the year input, will also print this statement
	PRINT_STATEMENT: INVOKE printBalanceStatment
					 cmp al, 1
					 je NO_STATEMENT
					 cmp al, 0
					 je START_USER
	
	; only after application process finish, the statement will be print, otherwise no statement to print
	NO_STATEMENT: mov edx, OFFSET noStatement
				  call crlf
				  call WriteString
				  call crlf
				  call crlf
				  jmp START_USER
	
		Start:
		INVOKE checkApplication
		cmp al, 3
		jne displayErrorMessage
		mov edx, offset prompt0	 ;Print out selection for user
		call writestring
		call crlf
		mov edx, offset prompt1
		call writestring
		call crlf
		mov edx, offset prompt2
		call writestring
		call crlf
		mov edx, offset prompt3
		call writestring
		call readint
		cmp al,1	
		je Pay			;go to payment if user input 1
		cmp al,2
		je Back;		;go back if user input 2	
	InvalidInput:
		mov edx, offset errorPrompt ;display error message
		call writestring
		call crlf
		call crlf
		jmp Start  ; ask user to enter again if user has input wrongly

	Pay:
		invoke chkPaymentStatus ; check payment status of user
		mov paymentStatus,eax	; move returned value into identifier payment Status
		cmp ax,1				; 1 means user is paying back the loan for first time
		je GetMonthlyPayBackAmt ; jump if payment Status is 1
		cmp ax,2				; 2 means user has started paying back the loan before
		je ContinuePayment		; jump if payment Status is 2
		cmp ax,0				; 0 means user has payed all the loan
	
	jz FinishedPaying		; jump if payment Status is 0
		
	GetMonthlyPayBackAmt:
		invoke clearScreen
		mov edx,offset totalPayBackAmount
		invoke getAmount,edx ; get monthly amount*100 that user need to pay from text file(assume returned value is floating value) 
		mov monthlyPayBackAmount, eax  ; mov the amount into identifier
		mov edx, offset prompt4		   
		call writestring
		mov edx,0
		mov ecx,divisor					;move divisor 100d into ecx
		div ecx							;div monthly amount in eax with ecx to get exact monthly amount
		call writedec					;display dividen value in eax
		mov eax,2Eh						;2Eh = '.'
		call writechar					;display symbol '.'
		mov eax,edx						;move quotient in edx into eax for display
		call writedec

	GetTotalPayBackAmt:
		call crlf
		mov eax,totalPayBackAmount
		cmp eax,0					 
		je Quit						; jump if the returned value is 0
		mov edx, offset prompt5		
		call writestring
		mov edx,0
		mov ecx,divisor
		div ecx
		call writedec
		mov eax,2Eh
		call writechar
		mov eax,edx
		call writedec
		call crlf

	
	ProceedPrompt: ;display menu for user to select proceed or back
		
		call crlf
		mov edx, offset prompt6	
		call writestring
		call crlf
		mov edx, offset prompt7
		call writestring
		call crlf
		mov edx, offset prompt8
		call writestring
		call crlf
		mov edx, offset prompt3
		call writestring
		call readdec
		cmp al,1
		je EnterPaymentAmount	;jump if user input 1 (proceed to fill card number)
		call crlf
		cmp al,2		
		je ResetPaymentStatus	;jump if user input 2 (back)
		mov edx, offset errorPrompt ;display error message if user enters invalid input
		call writestring
		jmp ProceedPrompt      ;Ask user to input again

	EnterPaymentAmount:			;invoke function to let user enter amount to pay
		invoke enterAmount,paymentStatus	
		cmp eax,0				;invalid amount if returned value is 0
		jz EnterPaymentAmount   ;repeat if it is invalid
		mov payAmount,eax		;else move the amount into identifier for further usage
	FillCardNo:
		call crlf
		;display amount to pay 
		mov edx, offset prompt13
		call writestring
		mov eax,payAmount
		mov edx,0
		mov ecx,divisor
		div ecx
		call writedec
		mov eax,2Eh
		call writechar
		mov eax,edx
		call writedec
		call crlf
		mov edx, offset prompt9
		call writestring
		mov edx, offset bankNo 
		mov ecx,16						;set maximum size 16
		call readString					;read bank number 
		mov eax,78h						; 78h = 'x'
		cmp ax, [edx]					; if user input 'x'
		je ProceedPrompt				;Allow user to go back 
		invoke checkBankCardNumber,edx  ;Else invoke bank num checking
		mov ebx,3						;give user maximum 3 times of entering wrong cvv
		cmp al,0						
		je FillCvv						;if bank num checking return 0, proceed to invoke cvv checking

		;invalid card number entered
		mov edx, offset errorPrompt1
		call writeString
		call crlf
		call crlf
		jmp FillCardNo					;Ask user to enter again
		
	FillCvv:	
		dec ebx							;decrement ebx everytime ask user to enter cvv
		mov edx, offset prompt10
		call writestring				;ask user to enter cvv for the card
		mov edx, offset cvv
		mov eax,0
		call readint					;read card cvv
		cmp eax,0						
		jz FillCardNo					;if user entered '0', go back to filling card number
		invoke checkCardCvv,eax			;else invoke function to check cvv
		cmp al,0
		je PaymentSuccess				;if returned value is 0, then payment is successful

		;else the cvv is invalid, ask user to enter again
		mov edx, offset errorPrompt2	
		call writeString
		call crlf
		call crlf
		cmp ebx,0					
		jne	FillCvv						;if ebx is not 0, allow user to enter again
		mov edx,offset errorPrompt3		
		call writestring 
		call crlf
		jmp ProceedPrompt						;else if ebx is 0,force user to back to menu
	
	PaymentSuccess:						;display success msg
		call crlf
		mov edx, offset prompt11
		call writestring
		call crlf
	
	MonthlyAmountSubtraction:	;subtract monthly amount with amount paid by user using the two value in identifier
		
		mov eax, monthlyPayBackAmount
		mov ebx, payAmount
		sub eax, ebx
		cmp eax, 0 
		jnz DisplayDueAmount   ;if due amount(eax-ebx) is not zero jump

	ClearDueAmount:				;else if due amount is 0, save it into the identifier
		mov dueAmount,eax
		jmp DisplayTotalAmountLeft

	DisplayDueAmount:			;display the due amount
		
		mov dueAmount,eax
		mov edx, offset prompt17
		call writestring
		mov edx,0
		mov ecx,divisor
		div ecx
		call writedec
		mov eax,2Eh
		call writechar
		mov eax,edx
		mov edx,0
		call writedec
		call crlf

	DisplayTotalAmountLeft:		; calculate the total Payback amount left and display
		mov edx, offset prompt12
		call writestring
		mov eax, totalPayBackAmount
		mov ebx, payAmount
		sub eax,ebx				; subtract total payback Amount with amount paid by user
		mov totalPayBackAmount,eax
		mov edx,0
		mov ecx,divisor
		div ecx
		call writedec
		mov eax,2Eh
		call writechar
		mov eax,edx
		mov edx,0
		call writedec
		call crlf

	UpdatePaymentInfo:
		invoke paymentInfoUpdate,totalPayBackAmount,payAmount,dueAmount ; update and save user payment info in txt file
		cmp totalPayBackAmount,0
		jz ChangePaymentStatus
		jmp START_USER
	
	FinishedPaying:    ;display finished paying msg and jump to START menu
		mov edx, offset prompt14
		call writestring
		call crlf
		call crlf
		jmp Start
	
	ContinuePayment:  ; if user has paid back before (not first time)

	GetMonthlyAndDue:
		invoke clearScreen
		mov edx,offset totalPayBackAmount
		invoke getAllThreeAmount,edx  ; get monthly amount that already added with due amount(if any)
		cmp eax,0
		je ThisMonthIsPaid			  ; if eax is 0, user has paid all for current month
		mov monthlyPayBackAmount, eax ; move the amount into identifier
		jmp GetTotalPayBackAmt        ; jump back to above to repeat the payment process
	
	

	ChangePaymentStatus:  ;change the payment status to"PaidAll" when the total amount payback is 0
		call crlf
		invoke changePayBackStatus
		mov edx, offset prompt14
		call writestring
		call crlf
		jmp Quit
	ResetPaymentStatus:		;change the payment status back to"FirstTime" when the user is not going to pay in the first time
		cmp paymentStatus,2
		je Quit
		invoke resetPayBackStatus
		jmp Start
	ThisMonthIsPaid:		;display msg if user has paid for this month
		call crlf
		mov edx,offset errorPrompt4
		call writestring
		call crlf

	Back:

	Quit:
		jmp START_USER  ;go back to main menu

	displayErrorMessage: call crlf
						 mov edx, OFFSET errorPrompt5
						 call WriteString
						 call crlf
						 jmp START_USER

	; log out from user account, jump to the login process
	LOGOUT_2: 
			 mov eax, 1
			 INVOKE getLogout, eax
	
	ret

UserModule ENDP

END