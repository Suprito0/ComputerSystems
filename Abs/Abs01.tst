//BITFLIP

load Abs.asm,
output-file Abs01.out,
compare-to Abs01.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2;

set PC 0,
set RAM[0] 0,       // Clear output
set RAM[1] -32768;  // Minimum 16-bit value
repeat 100 {
  ticktock;
}
set RAM[1] -32768,
output;
