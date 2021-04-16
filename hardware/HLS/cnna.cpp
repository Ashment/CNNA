#include "cnna.h"
#include <string.h>
void cnna(FIX_FM in_data[32][32][32], FIX_WT in_weights[32][32][3][3], FIX_FM out[32][32][32]){
#pragma HLS INTERFACE m_axi 		depth=32*32 	offset=slave	port=in_data		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=32*32		offset=slave	port=in_weights		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=32*32		offset=slave	port=out			bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

//#pragma HLS ALLOCATION function instances=CONVL2 limit=1

	// Define input and output buffers
	FIX_FM dbuf[32][32][32];
	FIX_WT wbuf[32][32][3][3];
	FIX_FM obuf[32][32][32];

	// Copy from AXI (DRAM) to buffers
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				dbuf[i][j][k] = in_data[i][j][k];

			}
		}
	}
	for(int ch=0; ch<32; ch++){
		for(int i=0; i<32; i++){
			for(int j=0; j<3; j++){
				for(int k=0; k<3; k++){
					#pragma HLS pipeline
					wbuf[ch][i][j][k] = in_weights[ch][i][j][k];
				}
			}
		}
	}

	// Do CONVL2
	CONVL2(dbuf, wbuf, obuf);

	// Copy from buffer to output
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				out[i][j][k] = obuf[i][j][k];
			}
		}
	}

	return;
}

void CONV3X3(FIX_FM in_fm[3][3], FIX_WT in_wt[3][3], FIX_FM *out){
	// 3x3 2D convolution
	FIX_FM vbuf;
	FIX_FM vout;

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			#pragma HLS pipeline
			vbuf = in_fm[i][j] * in_wt[i][j];
			vout += vbuf;
		}
	}
	*out = vout;
	return;
}

void L2DPU(FIX_FM in_fm[32][32][32], FIX_WT in_wt[32][32][3][3], int anchor[3], FIX_FM *out){
	// Does volume convolution on one channel.
	// anchor is the corresponding output location. (ch, j, k) from parent function
	#pragma HLS ALLOCATION function instances=CONV3X3 limit=32
	FIX_FM d_buf[32][3][3];
	FIX_WT w_buf[32][3][3];
	FIX_FM vbuf[32];


	// Fill local buffers
	for(int i=0; i<32; i++){
		for(int j=0; j<3; j++){
			for(int k=0; k<3; k++){
				#pragma HLS pipeline
				w_buf[i][j][k] = in_wt[anchor[0]][i][j][k];
				d_buf[i][j][k] = in_fm[i][anchor[1]+j][anchor[2]+k];
			}
		}
	}

	// Do convolutions and write output
	for(int i=0; i<32; i++){
		#pragma HLS unroll factor=32
		#pragma HLS pipeline
		CONV3X3(d_buf[i], w_buf[i], &vbuf[i]);
	}
	*out = 0;
	for(int i=0; i<32; i++){
		*out += vbuf[i];
	}
}

void CONVL2(FIX_FM in_fm[32][32][32], FIX_WT in_wt[32][32][3][3], FIX_FM out_fm[32][32][32]){
	// First CONV layer. No buffer for out_fm. Padding=1 for same padding
	// (32x32x32) INPUT | 32 Channels 3x3 | (32x32x32) OUTPUT
	#pragma HLS ALLOCATION function instances=L2DPU limit=32
	// Explicitly zero output buffer... (placeholder until padding is working)
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				out_fm[i][j][k] = 0;
			}
		}
	}

	for(int ch=0; ch<32; ch++){
		#pragma HLS unroll factor=32
		for(int j=1; j<31; j++){
			for(int k=1; k<31; k++){
				#pragma HLS pipeline
				int cAnchor[3] = {ch, j, k};
				L2DPU(in_fm, in_wt, cAnchor, &out_fm[ch][j][k]);
			}
		}
	}
}
