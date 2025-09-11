// Sorts the array of length R2 whose first element is at RAM[R1] in ascending order in place. Sets R0 to True (-1) when complete.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
@R2
D=M
@TRUE
D;JEQ
@R2
M=M+1 

(LOOPO)
    @R2
    M=M-1
    D=M //assiging the length of array to D

    @count
    M=D  // assigning the length of array to the variable count

    @R1
    D=M // assigning the index of array to D

    @index
    M=D // assigning the index of array to the variable index

    (LOOPI)
        @index
        D=M
        A=D // @index
        D=M // assigning value of array to D

        A=A+1   //@ next index
        D=D-M   // compare the two index

        @SWITCH
        D;JGT   //if >0 jump

        @index
        M=M+1

        @count
        M=M-1
        D=M

        @LOOPO
        D-1;JEQ

        @R2
        D=M

        @TRUE
        D-1;JEQ

        @LOOPI
        0;JMP

(SWITCH)
    @index
    D=M     //get index

    A=D  // go to index
    D=M //store specific value

    @temp  
    M=D  //store the value in the variable temp

    @index 
    A=M+1   //go to address of the next index
    D=M     //store value of that index

    @index
    A=M     // go to the original index
    M=D     //assign the next index here

    @temp   
    D=M     // go to temp to get the value of the original index

    @index
    M=M+1   // increment index

    A=M     // go to that address
    M=D     // set the value of the original index here 
            //switching complete

    @count
    M=M-1
    D=M

    @LOOPO
    D-1;JEQ

    @R2
    D=M

    @TRUE
    D-1;JEQ

    @LOOPI
    0;JMP

(TRUE)
@R0
M=!A

(END)
@END
0;JMP
