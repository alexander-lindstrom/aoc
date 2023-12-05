global  adjacent
section .text

; Argumetns: i - index, j - index, dim - columns per row
; Returns 1 if i is 8-neighbor adjacent to j, otherwise 0
; Initial state: rdi: i, rsi: j, rdx: dim

adjacent:

  mov rax, rdi                ; rax: i
  mov rbx, rdx                ; rbx: dim 
  xor rdx, rdx                ; zero top part of RDX:RAX
  
  idiv rbx                    ; rax: i/dim, rdx: i%dim
  mov r8, rdx                 ; r8: i%dim
  mov r9, rax                 ; r9: i/dim
  
  mov rax, rsi                ; repeat for j
  xor rdx, rdx
  idiv rbx                    
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