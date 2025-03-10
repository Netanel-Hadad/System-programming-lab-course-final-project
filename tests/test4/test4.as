mov #3, #4
add XYZ, #4
stop
jmp #6
add r9, #12
macr TEST
jmp MCR
endmacr
MCR: add *r3, #12
LABEL1: .data 0, 1, 5
.string asd
LABEL2: .extern A
jmp MCR james
print #5
