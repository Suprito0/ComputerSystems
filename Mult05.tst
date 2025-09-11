load Mult.asm,
output-file Mult05.out,
compare-to Mult05.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set PC 0,
set RAM[0] 0,
set RAM[1] -32767,
set RAM[2] 0;
repeat 200 {
  ticktock;
}
set RAM[1] -32767,
set RAM[2] 0,
output;
