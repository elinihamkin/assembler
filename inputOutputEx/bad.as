; file ps.as
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
lea W, r6, h
inc r6
mov: r3, K
54dh: dec W
sub r1, r4
bne END
cmp K, #-6
bne %END
dec W
.entry MAIN
jmp fg %LOOP
add L3, L3
END: stop
STR: .string "abcd" g
LIST: .data 6, -9
.data -100 654
K: .data 31 gh
.extern L3
