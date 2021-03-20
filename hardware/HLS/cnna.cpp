#include "cnna.h"
#include <string.h>

void cnna(int8 in_data[3][3], int8 in_weights[3][3], int8 out[3])
{
#pragma HLS INTERFACE m_axi 		depth=3*3 		offset=slave	port=in_data		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=3*3		offset=slave	port=in_weights		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=3			offset=slave	port=out			bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

	int8 i,j;

	// Move data from AXI to buffer, initialize output buffer
	int8 dbuf[3][3];
	int8 wbuf[3][3];
	int8 obuf[3];
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			#pragma HLS unroll
			dbuf[i][j] = in_data[i][j];
		}
	}
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			#pragma HLS unroll
			wbuf[i][j] = in_weights[i][j];
		}
	}

	// Do operations
	for(i=0; i<3; i++){
		obuf[i] = 0;
		for(j=0; j<3; j++){
			obuf[i] += dbuf[i][j] * wbuf[i][j];
		}
	}

	// Copy from buffer to output
	for(i=0; i<3; i++){
		#pragma HLS unroll
		out[i] = obuf[i];
	}

	return;
}
