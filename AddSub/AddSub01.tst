
load AddSub.asm,
output-file AddSub01.out,
compare-to AddSub01.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2;

set PC 0,
set RAM[0] 0,        // Clear result
set RAM[1] 32767,    // Max positive value
set RAM[2] -1,       // Negative one
set RAM[3] 12345;    // Mid-size positive
repeat 200 {
  ticktock;          // Run long enough
}
set RAM[1] 32767,
set RAM[2] -1,
set RAM[3] 12345,
output;
