section .text

; Arguments: i - index, j - index, dim - columns per row
; Returns 1 if i is 8-neighbor adjacent to j, otherwise 0
; Initial state: rdi - i, rsi - j, rdx - dim
global  adjacent
adjacent:

  mov rax, rdi
  mov rcx, rdx
  xor rdx, rdx
  
  idiv rcx
  mov r8, rdx
  mov r9, rax 
  
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
	mul rcx
	jmp .loop
	
.done:
	mul rdi
	add rax, rsi
	ret
	

; Return LCM(a,b). Assumed to fit in 64 bits
; Uses LCM(a,b) = (a*b)/gcd(a,b)
; Initially rdi - a, rsi - b
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

; Initially rdi - a, rsi - b, rax - gcd
.lcm:
	mov rcx, rax	
	mov rax, rdi
	mul rsi					

	xor rdx, rdx
	div rcx
	ret

; Initially rdi - arr*, rsi - n
global LCMM
LCMM:
	mov r8, rdi
	mov r9, rsi
	cmp r9, 2
	jl .fail
	
	; Get first LCM
	mov rsi, qword[r8]
	mov rdi, qword[r8 + 8]
	add r8, 16

	; LCM(i, i+1) now in rax
	call LCM
	
	mov r10, 2
	
.loop:
	cmp r10, r9
	jge .success
	mov rdi, qword [r8]
	mov rsi, rax
	call LCM
	inc r10
	add r8, 8
	jmp .loop
	
.fail:
	xor rax, rax
	ret
	
.success:
	ret