load Mult.asm,
output-file Mult01.out,
compare-to Mult01.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set PC 0,
set RAM[0] 0,
set RAM[1] 123,
set RAM[2] 200;
repeat 1500 {
  ticktock;
}
set RAM[1] 123,
set RAM[2] 200,
output;
