#include "cnna.h"
#include <string.h>

void cnna(int8 in_data[3][3], int8 in_weights[3][3][3], int8& out_data)
{
#pragma HLS INTERFACE s_axilite port=in_data
#pragma HLS INTERFACE s_axilite port=in_weights
#pragma HLS INTERFACE s_axilite port=out_data
#pragma HLS INTERFACE ap_ctrl_none port=return

int8 i = 0;
int8 j, k;
int8 acc = 0;

// Does this infer into DSP slice as a MAC? Probably.
for(j=0; j<9; j++){
	for(k=0; k<9; k++){
		#pragma HLS unroll
		acc += in_data[j][k] * in_weights[i][j][k];
	}
}

out_data = acc;
return;
}
