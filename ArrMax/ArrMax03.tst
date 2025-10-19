// ArrMax03.tst — ArrMax tough case.
// Assumptions: argument[0] = base address of array; argument[1] = length; result (max) stored in RAM[16].
// The rest is noise/sentinels, kept stable across tests.

load,
output-file ArrMax03.out,
compare-to ArrMax03.cmp,
output-list sp%D1.6.1 local%D1.6.1 argument%D1.8.1 this%D1.6.1 that%D1.6.1
            RAM[16]%D1.6.1 RAM[17]%D1.6.1 RAM[18]%D1.6.1
            local[0]%D1.8.1 local[1]%D1.8.1 local[2]%D1.8.1
            argument[0]%D1.11.1 argument[1]%D1.11.1 argument[2]%D1.11.1;

set sp 256,
set local 300,
set argument 400,
set this 3000,
set that 3010;

// locals noise
set local[0] 10,
set local[1] 20,
set local[2] 30;

// args: base, length, noise
set argument[0] 2048,
set argument[1] 1,
set argument[2] 300;

// statics/sentinels
set RAM[16] -7777,
set RAM[17] 2048,
set RAM[18] 5555;

// array contents (single element)
set RAM[2048] 1234;

repeat 400 {
  vmstep;
}
output;
