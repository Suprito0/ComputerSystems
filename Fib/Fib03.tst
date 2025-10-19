// FibXX.tst — Change-only-the-important-values (n and expected Fib(n)).
// Assumptions: n in RAM[16], result Fib(n) in RAM[17].
// The rest is noise/sentinels to ensure memory hygiene.

load,
output-file Fib03.out,
compare-to Fib03.cmp,
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

// args noise
set argument[0] 100,
set argument[1] 200,
set argument[2] 300;

// input + sentinel
set RAM[16] 10,
set RAM[17] -9999,   // will be overwritten by result
set RAM[18] 5555;

repeat 400 {
  vmstep;
}
output;
