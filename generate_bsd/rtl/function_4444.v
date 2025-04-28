//circuit accuracy	= 0.993054
//test amounts 	= 100000
//total BDD depth	=12
//total BDD nodes	= 73
//total split modes	= 28
//total BDD nodes recursive	= 73
//total split modes recursive	= 28
//train time	= 0.19708
module function_4444_BDD_id_0 (i,o);

input 	[11:0] i;
output	[8:0]  o;

wire [     8:0] l_0;
wire [     8:0] l_1;
wire [     9:0] l_2;
wire [     8:0] l_3;
wire [     7:0] l_4;
wire [     6:0] l_5;
wire [     5:0] l_6;
wire [     4:0] l_7;
wire [     3:0] l_8;
wire [     2:0] l_9;
wire [     1:0] l_10;
wire [     0:0] l_11;
wire [    -1:0] l_12;

assign o [     8:0] = l_0[     8:0];

assign l_0[     0]    = ( l_1 [     0] &  i[     4]);
assign l_0[     1]    = ( l_1 [     1] &  i[     4]);
assign l_0[     2]    = ( l_1 [     2] &  i[     4]);
assign l_0[     3]    = ( l_1 [     3] &  i[     4]);
assign l_0[     4]    = ( l_1 [     4] &  i[     4]);
assign l_0[     5]    = ( l_1 [     5] &  i[     4]);
assign l_0[     6]    = ( l_1 [     6] &  i[     4]);
assign l_0[     7]    = ( l_1 [     7] &  i[     4]);
assign l_0[     8]    = ( l_1 [     8] &  i[     4]);
assign l_1[     0]    = ( l_2 [     0]);
assign l_1[     1]    = ( l_2 [     1]);
assign l_1[     2]    = !i[     5];
assign l_1[     3]    = ( l_2 [     2] ^  i[     5]);
assign l_1[     4]    = ( l_2 [     2] &  i[     5]);
assign l_1[     5]    = ( l_2 [     3] &  i[     5]);
assign l_1[     6]    = ( l_2 [     4] & !i[     5]) | ( l_2 [     5] &  i[     5]);
assign l_1[     7]    = ( l_2 [     6] & !i[     5]) | ( l_2 [     7] &  i[     5]);
assign l_1[     8]    = ( l_2 [     8] & !i[     5]) | ( l_2 [     9] &  i[     5]);
assign l_2[     0]    = ( l_3 [     0]);
assign l_2[     1]    = ( l_3 [     1]);
assign l_2[     2]    =  i[     6];
assign l_2[     3]    = ( l_3 [     2] &  i[     6]);
assign l_2[     4]    = ( l_3 [     3]);
assign l_2[     5]    = ( l_3 [     3] & !i[     6]) | ( l_3 [     4] &  i[     6]);
assign l_2[     6]    = ( l_3 [     5]);
assign l_2[     7]    = ( l_3 [     5] & !i[     6]) | ( l_3 [     6] &  i[     6]);
assign l_2[     8]    = ( l_3 [     7]);
assign l_2[     9]    = ( l_3 [     7] & !i[     6]) | ( l_3 [     8] &  i[     6]);
assign l_3[     0]    = ( l_4 [     0]);
assign l_3[     1]    =  i[    11];
assign l_3[     2]    = ( l_4 [     1]);
assign l_3[     3]    = ( l_4 [     2]);
assign l_3[     4]    = ( l_4 [     3]);
assign l_3[     5]    = ( l_4 [     4]);
assign l_3[     6]    = ( l_4 [     5]);
assign l_3[     7]    = ( l_4 [     6]);
assign l_3[     8]    = ( l_4 [     7]);
assign l_4[     0]    = ( l_5 [     0]);
assign l_4[     1]    = ( l_5 [     1]);
assign l_4[     2]    = ( l_5 [     2]);
assign l_4[     3]    = ( l_5 [     3]);
assign l_4[     4]    =  i[     9];
assign l_4[     5]    = ( l_5 [     4]);
assign l_4[     6]    = ( l_5 [     5]);
assign l_4[     7]    = ( l_5 [     6]);
assign l_5[     0]    = ( l_6 [     0]);
assign l_5[     1]    =  i[     0];
assign l_5[     2]    = ( l_6 [     1]);
assign l_5[     3]    = ( l_6 [     2]);
assign l_5[     4]    = ( l_6 [     3]);
assign l_5[     5]    = ( l_6 [     4]);
assign l_5[     6]    = ( l_6 [     5]);
assign l_6[     0]    = ( l_7 [     0]);
assign l_6[     1]    = ( l_7 [     1]);
assign l_6[     2]    = ( l_7 [     2]);
assign l_6[     3]    = ( l_7 [     3]);
assign l_6[     4]    = ( l_7 [     4]);
assign l_6[     5]    =  i[     3];
assign l_7[     0]    = ( l_8 [     0]);
assign l_7[     1]    =  i[     8];
assign l_7[     2]    = ( l_8 [     1]);
assign l_7[     3]    = ( l_8 [     2]);
assign l_7[     4]    = ( l_8 [     3]);
assign l_8[     0]    = ( l_9 [     0]);
assign l_8[     1]    = ( l_9 [     1]);
assign l_8[     2]    = ( l_9 [     2]);
assign l_8[     3]    =  i[    10];
assign l_9[     0]    =  i[     7];
assign l_9[     1]    = ( l_10 [     0]);
assign l_9[     2]    = ( l_10 [     1]);
assign l_10[     0]    = ( l_11 [     0]);
assign l_10[     1]    =  i[     2];
assign l_11[     0]    = 0;

endmodule
