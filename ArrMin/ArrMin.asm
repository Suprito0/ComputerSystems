// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
@R2
D=M

@END
D;JEQ

(STORE)
@R1
A=M //get value in R1
D=M //get first value in array

@R0
M=D //store in R0

@R2
M=M-1

(LOOP)
@R2
D=M;

@END
D;JEQ

@R1
M=M+1

A=M     //get next array value location
D=M  //get that array's value

@R0
D=D-M //compare with stored

@STORE
D;JLT // Jump to STORE if stored value is greater

@R2
M=M-1

@LOOP
0;JMP

(END)
@END
0;JMP
