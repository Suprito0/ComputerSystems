load AddSub.asm,
output-file AddSub02.out,
compare-to AddSub02.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2;

set PC 0,
set RAM[0] 0,        // Clear result
set RAM[1] -12345,   // Negative
set RAM[2] -22222,   // More negative
set RAM[3] -32768;   // Min possible value
repeat 200 {
  ticktock;
}
set RAM[1] -12345,
set RAM[2] -22222,
set RAM[3] -32768,
output;
