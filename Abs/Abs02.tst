load Abs.vm,
output-file Abs02.out,
compare-to Abs02.cmp,
output-list sp%D1.6.1 local%D1.6.1 argument%D1.8.1 this%D1.6.1 that%D1.6.1
            RAM[16]%D1.7.1 RAM[17]%D1.7.1 RAM[18]%D1.7.1
            local[0]%D1.8.1 local[1]%D1.8.1
            argument[0]%D1.11.1;

set sp 256,
set local 300,
set argument 400,
set this 3000,
set that 3010;

set RAM[16] 32767,   // x
set RAM[17] -2222,   // y pre-noise
set RAM[18] -5555,   // sentinel should remain

set local[0] 7777,
set local[1] -8888,
set argument[0] 9999;

repeat 200 {
  vmstep;
}
output;
