// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
@R1
D=M //get value in R1

@D
D=M //get first value in array

(STORE)
@R0
M=D //store in R0

@R2
D=M;

@END
D;JEQ

(LOOP)
@R1
M=M+1
D=M     //get next array value location

@D
D=M  //get that array's value

@R0
D=D-M //compare with stored

@R2
M=M-1

@STORE
D;JLT // Jump to STORE if stored value is greater

(END)
@END
0;JMP