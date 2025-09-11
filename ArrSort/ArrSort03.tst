load ArrSort.asm,
output-file ArrSort03.out,
compare-to ArrSort03.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[40]%D2.6.2 RAM[41]%D2.6.2 RAM[42]%D2.6.2 RAM[43]%D2.6.2;

set PC 0,
set RAM[0] 0,
set RAM[1] 40,
set RAM[2] 4,
set RAM[40] -1000,
set RAM[41] -32768,
set RAM[42] -5000,
set RAM[43] -123;
repeat 600 {
  ticktock;
}
set RAM[1] 40,
set RAM[2] 4,
output;
