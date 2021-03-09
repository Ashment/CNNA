#include "cnna.h"
#include <string.h>

void cnna(int8 in_data[3], int8 in_weights[3], int8 out[3])
{
#pragma HLS INTERFACE m_axi 		depth=3 	offset=slave	port=in_data		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=3		offset=slave	port=in_weights		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=3		offset=slave	port=out			bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

	int i,j;

	// Move data from AXI to buffer
	int8 dbuf[3];
	int8 wbuf[3];
	int8 obuf[3];
	for(i=0; i<3; i++){
		dbuf[i] = in_data[i];
	}
	for(i=0; i<3; i++){
		wbuf[i] = in_weights[i];
	}

	// Do some operation
	for(i=0; i<3; i++){
		obuf[i] = 0;
		for(j=0; j<3; j++){
#pragma HLS pipeline
			obuf[i] += dbuf[i] * wbuf[j];
		}
	}

	// Copy from buffer to output
	for(i=0; i<3; i++){
		out[i] = obuf[i];
	}

	return;
}

/*
void cnna(int8* in_data[3][3], int8* in_weights[3][3][3], int8* out)
{
#pragma HLS INTERFACE m_axi 		depth=3*3 		offset=slave	port=in_data
#pragma HLS INTERFACE m_axi 		depth=3*3*3		offset=slave	port=in_weights
#pragma HLS INTERFACE m_axi							offset=slave	port=out
#pragma HLS INTERFACE s_axilite port=return

	int8 i = 0;
	int8 j, k;
	int8 acc = 0;

	// Move data from AXI to buffer
	int8 dbuf[3][3];
	int8 wbuf[3][3][3];
	memcpy(dbuf, (const int8*) in_data, (3*3)*sizeof(int8));
	memcpy(wbuf, (const int8*) in_weights, (3*3*3)*sizeof(int8));

	// Does this infer into DSP slice as a MAC? Probably.
	for(j=0; j<9; j++){
		for(k=0; k<9; k++){
			#pragma HLS unroll
			acc += dbuf[j][k] * wbuf[i][j][k];
		}
	}

	*out = acc;
	return;
}
*/
