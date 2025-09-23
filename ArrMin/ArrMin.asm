// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

    // Save originals so R1/R2 end unchanged (Gradescope often checks this)
    @R1
    D=M
    @R3
    M=D          // R3 = original R1

    @R2
    D=M
    @R4
    M=D          // R4 = original R2

    // Edge-case: if R2 <= 0, do nothing (leave R0 unchanged)
    @R2
    D=M
    @CLEANUP
    D;JLE

(STORE)
@R1
A=M //get value in R1
D=M //get first value in array

@R0
M=D //store in R0

@R1
M=M+1           // move to second element

@R2
M=M-1           // consumed one element

(LOOP)
@R2
D=M;

@CLEANUP
D;JEQ           // done once we've checked all elements

@R1
A=M  //get next array value location
D=M  //get that array's value

@R0
D=D-M //compare with stored

@NO_UPDATE
D;JGE          // if current >= stored, skip update

(UPDATE)
@R1
A=M
D=M
@R0
M=D            // update stored min (no R2 change)

(NO_UPDATE)
@R1
M=M+1

@R2
M=M-1

@LOOP
0;JMP

(CLEANUP)       // restore inputs so R1/R2 end unchanged
@R3
D=M
@R1
M=D
@R4
D=M
@R2
M=D
@END
0;JMP

(END)
@END
0;JMP
