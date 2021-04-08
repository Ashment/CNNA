#include "cnna.h"
#include <string.h>

void CONV3X3(FIX_FM in_fm[3][3], FIX_WT in_wt[3][3], FIX_FM *out){
	// Simple 3x3 2D convolution
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

void CONVDEPTH32(FIX_FM in_fm[32][3][3], FIX_WT in_wt[32][3][3], FIX_FM *out){
	// For convolution of volume of 32x3x3
	FIX_FM vout = 0;
	FIX_FM vbuf;

	for(int i=0; i<32; i++){
		#pragma HLS unroll factor=4
		// (i) for each input channel.
		CONV3X3(in_fm[i], in_wt[i], &vbuf);
	}

	*out = vout;
	return;
}


void CONVL2(FIX_FM in_fm[32][32][32], FIX_WT in_wt[32][32][3][3], FIX_FM out_fm[32][32][32]){
	// First CONV layer. No buffer for out_fm. Padding=1 for same padding
	// (32x32x32) INPUT | 32 Channels 3x3 | (32x32x32) OUTPUT

	FIX_FM fmBuf[32][3][3];
	FIX_WT wtBuf[32][3][3];
	FIX_FM tout;

	// Explicitly zero output buffer... (placeholder until padding is working)
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS loop_flatten
				#pragma HLS pipeline
				out_fm[i][j][k] = 0;

			}
		}
	}

	// Aiming for output stationary kind of array.
	// (ch,j,k) is output indices. Load the IFM only once for each CONV frame, all channels
	for(int i=0; i<32; i++){
		for(int j=1; j<31; j++){
			for(int k=1; k<31; k++){
				// Populate IFM buffer.
				for(int jj=0; jj<3; jj++){
					for(int kk=0; kk<3; kk++){
						#pragma HLS pipeline
						fmBuf[i][jj][kk] = in_fm[i][j+jj-1][k+kk-1];
					}
				}

				// populate weights for given channel and compute output
				for(int ch=0; ch<32; ch++){
					for(int jj=0; jj<3; jj++){
						for(int kk=0; kk<3; kk++){
							#pragma HLS pipeline
							wtBuf[i][jj][kk] = in_wt[ch][i][j+jj-1][k+kk-1];
						}
					}
					CONVDEPTH32(fmBuf, wtBuf, &out_fm[ch][j][k]);
				}
			}
		}
	}
	return;
}

void cnna(FIX_FM in_data[32][32][32], FIX_WT in_weights[32][32][3][3], FIX_FM out[32][32][32]){
#pragma HLS INTERFACE m_axi 		depth=32*32*32 		offset=slave	port=in_data		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=32*32*3*3		offset=slave	port=in_weights		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=32*32*32		offset=slave	port=out			bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

#pragma HLS ALLOCATION function instances=CONV3X3 limit=3

	// Define input and output buffers
	FIX_FM dbuf[32][32][32];
	FIX_WT wbuf[32][32][3][3];
	FIX_FM obuf[32][32][32];

	// Copy from AXI (DRAM) to buffers
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS loop_flatten
				#pragma HLS pipeline
				dbuf[i][j][k] = in_data[i][j][k];

			}
		}
	}
	for(int ch=0; ch<32; ch++){
		for(int i=0; i<32; i++){
			for(int j=0; j<3; j++){
				for(int k=0; k<3; k++){
					#pragma HLS loop_flatten
					#pragma HLS pipeline
					wbuf[ch][i][j][k] = in_weights[ch][i][j][k]
				}
			}
		}
	}

	// Do CONVL2
	CONVL2(dbuf, wbuf, obuf);

	// Copy from buffer to output
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			#pragma HLS pipeline
			out[i] = obuf[i];
		}
	}

	return;
}
