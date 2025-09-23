// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@R2
D=M
@END
D;JLE          // if R2 <= 0, do nothing (leave R0 unchanged) and halt

// Make working copies so R1 and R2 remain unchanged
@R1
D=M
@R13
M=D            // R13 = current pointer (copy of R1)

@R2
D=M
@R14
M=D            // R14 = remaining count (copy of R2)

(STORE)
@R13
A=M //get value in R1
D=M //get first value in array

@R0
M=D //store in R0

@R13
M=M+1          // advance working pointer

@R14
M=M-1          // consumed one element from the copy

(LOOP)
@R14
D=M;

@END
D;JEQ

@R13
A=M  //get next array value location
D=M  //get that array's value

@R0
D=D-M //compare with stored

@NO_UPDATE
D;JGE          // if current >= stored, skip update

// update stored min = current (R1/R2 untouched)
@R13
A=M
D=M
@R0
M=D

(NO_UPDATE)
@R13
M=M+1          // advance working pointer copy

@R14
M=M-1          // decrement remaining count copy

@LOOP
0;JMP

(END)
@END
0;JMP
