#include "cnna.h"
#include <string.h>

void cnna(FIX_FM in_data[3][3][3], FIX_WT in_weights[3][3][3], FIX_FM out[3][3])
{
#pragma HLS INTERFACE m_axi 		depth=3*3*3 		offset=slave	port=in_data		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=3*3*3			offset=slave	port=in_weights		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=3*3			offset=slave	port=out			bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

	int8 i,j,k;

	// Move data from AXI to buffer, initialize output buffer
	FIX_FM dbuf[3][3][3];
	FIX_WT wbuf[3][3][3];
	FIX_FM obuf[3][3];

	for(i=0; i<3; i++){
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
		for(j=0; j<3; j++){
			#pragma HLS pipeline
			obuf[i][j] = 0;
			for(k=1; k<3; k++){
				#pragma HLS unroll
				obuf[i][j] += dbuf[i][j][k] * wbuf[i][j][k];
			}
		}
	}

	// Copy from buffer to output
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			#pragma HLS pipeline
			out[i][j] = obuf[i][j];
		}
	}

	return;
}
