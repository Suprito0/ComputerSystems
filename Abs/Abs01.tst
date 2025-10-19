load Abs.vm,
output-file Abs01.out,
compare-to Abs01.cmp,
output-list sp%D1.6.1 local%D1.6.1 argument%D1.8.1 this%D1.6.1 that%D1.6.1
            RAM[16]%D1.7.1 RAM[17]%D1.7.1 RAM[18]%D1.7.1
            local[0]%D1.8.1 local[1]%D1.8.1
            argument[0]%D1.11.1;

set sp 256,        // stack pointer
set local 300,     // base address of the local segment
set argument 400,  // base address of the argument segment
set this 3000,     // base address of the this segment
set that 3010;     // base address of the that segment

set RAM[16] -32768,  // static 0 -> x
set RAM[17] -999,    // static 1 -> y (pre-noise)
set RAM[18] 1234,    // static 2 -> sentinel (must remain unchanged)

set local[0] -123,   // local noise (must remain unchanged)
set local[1] 456,
set argument[0] -789;

repeat 200 {
  vmstep;
}
output;
