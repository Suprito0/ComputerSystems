// Calculates a = x * y
// a is a local variable
// x and y are static variables

// initialize a = 0
push constant 0
pop local 0     // local 0 represents 'a'

// create a counter = y
push static 1    // y
pop local 1      // local 1 will act as 'counter'

// label for loop
label MULT_LOOP

// if counter == 0, end loop
push local 1
push constant 0
eq
if-goto END

// a = a + x
push local 0
push static 0    // x
add
pop local 0

// counter = counter - 1
push local 1
push constant 1
sub
pop local 1

// repeat
goto MULT_LOOP

label END
// program halts here (no goto END)
