entry LOOP
.entry 1LENGTH
.extern 
.extern BEN
MAIN: mov K [2-4],W
add r56,STR
LOOP: jmp W
prn #-5-
bob r1, r4
inc K
mov LOOP[13-1],r3
bne L3
END: stop
STR: .string abcdef"
LENGTH: .data 6&,-9,15
K: .data dd