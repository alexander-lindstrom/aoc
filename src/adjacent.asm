global  adjacent_asm
section .text

; initial state: rdi: i, rsi: j, rdx: dim
adjacent_asm:

  mov rax, rdi                ; rax: i
  mov rbx, rdx                ; rbx: dim 
  xor rdx, rdx                ; zero top part of RDX:RAX
  
  idiv rbx                    ; rax: i/dim, rdx: i%dim
  mov r8, rdx                 ; r8: i%dim
  mov r9, rax                 ; r9: i/dim
  
  mov rax, rsi                ; rax: j
  xor rdx, rdx
  idiv rbx                    ; rax: j/dim, rdx: j%dim
  mov r10, rdx                ; r10: j%dim
  mov r11, rax                ; r11: j/dim
  
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