section .text

; Arguments: i - index, j - index, dim - columns per row
; Returns 1 if i is 8-neighbor adjacent to j, otherwise 0
; Initial state: rdi - i, rsi - j, rdx - dim
global  adjacent
adjacent:

  mov rax, rdi 		; rax: i
  mov rcx, rdx    ; rcx: dim 
  xor rdx, rdx    ; zero top part of RDX:RAX
  
  idiv rcx        ; rax: i/dim, rdx: i%dim
  mov r8, rdx    	; r8: i%dim
  mov r9, rax     ; r9: i/dim
  
  mov rax, rsi
  xor rdx, rdx
  idiv rcx                    
  mov r10, rdx                
  mov r11, rax                
  
  ; return false if abs(rowdiff) > 1
  cmp r9, r11
  jg .noswap1
  xchg r9, r11
  
.noswap1:
  sub r9, r11 
  cmp r9, 1
  jg .retfalse
  
  ; return false if abs(coldiff) > 1
  cmp r8, r10    
  jg .noswap2
  xchg r8, r10
 
.noswap2:
  sub r8, r10 
  cmp r8, 1
  jg .retfalse
  
  mov rax, 1
  ret

.retfalse:
  xor rax, rax
  ret

; Check if i is on [j, k]
; Initial state: rdi - i, rsi - j, rdx - k
global  between
between:
  cmp rdi, rsi
  jl .retfalse
  
  cmp rdi, rdx
  jg .retfalse
  
  mov rax, 1
  ret
  
.retfalse:
  xor rax, rax
  ret
  
; Concatenate two numbers a, b
; Initial state: rdi - x, rsi - y
; Example: x = 33, y = 41 -> 3341
global concat
concat:
	mov rax, 10
	mov rcx, 10
	
.loop:
	cmp rax, rsi
	jg .done
	mul rcx 			; rcx*rax -> rax
	jmp .loop
	
.done:
	mul rdi 			; rdi*rax -> rax
	add rax, rsi
	ret
	

; Return LCM (a,b). Assumed to fit in 64 bits
; Uses LCM = (a*b)/gcd
; Arguments: a, b
global LCM
LCM:

	mov rax, rdi	
	mov rcx, rsi

; Initially rax - a, rsi - b
; When done rax - gcd
.gcd:
	xor rdx, rdx
	div rcx
	mov rax, rcx
	mov rcx, rdx
	cmp rcx, 0
	jnz .gcd

; Initially rdi - a, rsi, b, rax - gcd
.lcm:
	mov rbx, rax	; rbx - gcd
	mov rax, rdi
	mul rsi				; rax - a*b		

	xor rdx, rdx
	div rbx				; rax - a*b/gcd
	ret
	




  
