// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// Edge-case guard: empty or non-positive length
@R2
D=M
@END_EMPTY
D;JLE

(STORE)                 // initial seed of min from first element
@R1
A=M //get value in R1
D=M //get first value in array

@R0
M=D //store in R0

@R1
M=M+1           // advance to the second element

@R2
M=M-1           

(LOOP)
@R2
D=M;

@END
D;JEQ

@R1
A=M  //get next array value location
D=M  //get that array's value

@R0
D=D-M //compare with stored

@NO_UPDATE
D;JGE           // if current >= stored, skip update

// --- Update min without touching R2 here ---
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

(END_EMPTY)             // --- Define behavior for empty array ---
@R0
M=0                     // choose 0 for "no minimum"
@END
0;JMP
