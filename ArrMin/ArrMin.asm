// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@R2
D=M
@END
D;JLE          // if R2 <= 0, do nothing (leave R0 unchanged) and halt

(STORE)
@R1
A=M //get value in R1
D=M //get first value in array

@R0
M=D //store in R0

@R1
M=M+1          // move to second element

@R2
M=M-1          // consumed one element (the first)

(LOOP)
@R2
D=M;

@END
D;JEQ          // done once we've checked all elements

@R1
A=M  //get next array value location
D=M  //get that array's value

@R0
D=D-M //compare with stored

@NO_UPDATE
D;JGE          // if current >= stored, skip update

// update stored min = current (don’t touch R2 here)
@R1
A=M
D=M
@R0
M=D

(NO_UPDATE)
@R1
M=M+1

@R2
M=M-1

@LOOP
0;JMP

(END)
@END
0;JMP
