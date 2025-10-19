load Mult.vm,
output-file Mult02.out,
compare-to Mult02.cmp,
output-list sp%D1.6.1 local%D1.6.1 argument%D1.8.1 this%D1.6.1 that%D1.6.1
            RAM[16]%D1.6.1 RAM[17]%D1.6.1 RAM[18]%D1.6.1
            local[0]%D1.8.1 local[1]%D1.8.1 local[2]%D1.8.1
            argument[0]%D1.11.1 argument[1]%D1.11.1 argument[2]%D1.11.1;

set sp 256,
set local 300,
set argument 400,
set this 3000,
set that 3010;

set RAM[16] -37,
set RAM[17] -24,
set RAM[18] -4321;

set local[0] -9999,
set local[1] -2222,
set local[2] 3333;

set argument[0] -101,
set argument[1] 0,
set argument[2] 42;

repeat 400 {
  vmstep;
}
output;
