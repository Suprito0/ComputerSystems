load Mult.asm,
output-file Mult02.out,
compare-to Mult02.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set PC 0,
set RAM[0] 0,
set RAM[1] 32767,   // max positive
set RAM[2] -1;      // multiply by negative one
repeat 375000 {
  ticktock;
}
set RAM[1] 32767,
set RAM[2] -1,
output;
