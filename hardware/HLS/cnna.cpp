#include "cnna.h"
#include <string.h>

void CONV3X3(FIX_FM in_fm[3][3], FIX_WT in_wt[3][3], FIX_FM *out){
	FIX_FM vout = 0;

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			#pragma HLS pipeline
			vout += in_fm[i][j] * in_wt[i][j];
		}
	}

	*out = vout;

	return;
}

void cnna(FIX_FM in_data[3][3][3], FIX_WT in_weights[3][3][3], FIX_FM out[3]){
#pragma HLS INTERFACE m_axi 		depth=3*3*3 		offset=slave	port=in_data		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=3*3*3			offset=slave	port=in_weights		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=3			offset=slave	port=out			bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

#pragma HLS ALLOCATION function instances=CONV3X3 limit=3

	int8 i,j,k;

	// Define input and output buffers
	FIX_FM dbuf[3][3][3];
	FIX_WT wbuf[3][3][3];
	FIX_FM obuf[3];

	// Copy from AXI (DRAM) to buffers
	for(i=0; i<3; i++){
		obuf[i] = 0;
		for(j=0; j<3; j++){
			for(k=0; k<3; k++){
				#pragma HLS pipeline
				dbuf[i][j][k] = in_data[i][j][k];
				wbuf[i][j][k] = in_weights[i][j][k];
			}
		}
	}

	// Do operations
	for(i=0; i<3; i++){
		#pragma HLS unroll
		CONV3X3(dbuf[i], wbuf[i], &obuf[i]);
	}

	// Copy from buffer to output
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			#pragma HLS pipeline
			out[i] = obuf[i];
		}
	}

	return;
}
