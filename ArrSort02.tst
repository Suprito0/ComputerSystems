load ArrSort.asm,
output-file ArrSort02.out,
compare-to ArrSort02.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[30]%D2.6.2 RAM[31]%D2.6.2 RAM[32]%D2.6.2 RAM[33]%D2.6.2;

set PC 0,
set RAM[0] 0,
set RAM[1] 30,
set RAM[2] 4,
set RAM[30] -5,
set RAM[31] 2,
set RAM[32] -10,
set RAM[33] 7;
repeat 600 {
  ticktock;
}
set RAM[1] 30,
set RAM[2] 4,
output;
