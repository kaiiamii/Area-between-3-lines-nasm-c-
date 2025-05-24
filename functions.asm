bits 32
section .data
    const35 dq 0.350000
    const95 dq 0.950000
    const27 dq 2.700000
    const2 dq 2.000000
    const3 dq 3.000000

section .text
global f1, f2, f3, f1_p, f2_p, f3_p  
f1:
    push ebp
    mov ebp, esp
    finit
    fld qword[const27]
    fld qword[const95]
    fld qword[ebp + 8]
    fmulp st1, st0
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp st1, st0
    fld qword[const35]
    fmulp st1, st0
    fsub st0, st1
    fadd st0, st2
    mov esp, ebp
    pop ebp
    ret

f2:
    push ebp
    mov ebp, esp
    finit
    fld qword[const3]
    fld qword[ebp + 8]
    fmulp st1, st0
    fld1
    fadd st0, st1
    mov esp, ebp
    pop ebp
    ret

f3:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]  
    fadd qword[const2]
    fld1                  
    fdivrp st1, st0   
    mov esp, ebp    
    pop ebp
    ret

f1_p:
    push ebp
    mov ebp, esp
    finit
    fld qword[const95]
    fld qword[ebp + 8]
    fld qword[const35]
    fmulp st1, st0
    fld qword[const2]
    fmulp st1, st0
    fsub st0, st1
    mov esp, ebp
    pop ebp
    ret

f2_p:
    push ebp
    mov ebp, esp
    finit
    fld qword[const3]
    mov esp, ebp
    pop ebp
    ret

f3_p:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]   
    fadd qword[const2]  
    fld st0              
    fmulp st1, st0      
    fld1                 
    fdivrp st1, st0       
    fchs          
    mov esp, ebp
    pop ebp
    ret
    
section .note.GNU-stack noalloc noexec nowrite progbits
