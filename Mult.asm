// This file is based on part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: Mult.asm

// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@0
D=A
@R0
M=D

@R1
D=M

@R3
M=D

(LOOP)
@R2
D=M

@R0
M=M+D

@R3
M=M-1
D=M

@END
D;JEQ

@LOOP
0;JMP

(END)
@END
0;JMP