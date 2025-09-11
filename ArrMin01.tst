load ArrMin.asm,
output-file ArrMin01.out,
compare-to ArrMin01.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[10]%D2.6.2 RAM[11]%D2.6.2 RAM[12]%D2.6.2 RAM[13]%D2.6.2;

set PC 0,
set RAM[0] 0,  // R0
set RAM[1] 10, // base address
set RAM[2] 4,  // length
set RAM[10] 15,
set RAM[11] 7,
set RAM[12] 9,
set RAM[13] 12;
repeat 300 {
  ticktock;
}
set RAM[1] 10,
set RAM[2] 4,
output;
