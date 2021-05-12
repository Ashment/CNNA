#include "cnna.h"
#include <string.h>

void cnna(FIX_FM in_data[32][32][32], FIX_WT wtL2[32][32][3][3], FIX_FM out[32][32][32]){
#pragma HLS INTERFACE m_axi 		depth=3*32*32 		offset=slave		port=in_data	bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=32*3*9		offset=slave		port=wtL1		bundle=INPUT
#pragma HLS INTERFACE m_axi 		depth=32*32*9		offset=slave		port=wtL2		bundle=INPUT
#pragma HLS INTERFACE m_axi			depth=32*32*32		offset=slave		port=out		bundle=OUTPUT
#pragma HLS INTERFACE s_axilite register port=return

//#pragma HLS ALLOCATION function instances=CONVL2 limit=1

	// Define input and output buffers
	FIX_FM dbuf[32][32][32];
	//FIX_WT wbufL1[32][3][3][3];
	FIX_WT wbufL2[32][32][3][3];
	//FIX_FM obufL1[32][32][32];
	FIX_FM obufL2[32][32][32];

	// Copy from AXI (DRAM) to buffers
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				dbuf[i][j][k] = in_data[i][j][k];

			}
		}
	}
	// L1 weights
	/*
	for(int ch=0; ch<32; ch++){
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				for(int k=0; k<3; k++){
					#pragma HLS pipeline
					wbufL1[ch][i][j][k] = wtL1[ch][i][j][k];
				}
			}
		}
	}
	*/
	// L2 weights
	for(int ch=0; ch<32; ch++){
		for(int i=0; i<32; i++){
			for(int j=0; j<3; j++){
				for(int k=0; k<3; k++){
					#pragma HLS pipeline
					wbufL2[ch][i][j][k] = wtL2[ch][i][j][k];
				}
			}
		}
	}

	// Do CONVL1, fills obufL2
	//CONVL1(dbuf, wbufL1, obufL1);

	// Do CONVL2
	CONVL2(dbuf, wbufL2, obufL2);

	// Copy from buffer to output
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				out[i][j][k] = obufL2[i][j][k];
			}
		}
	}

	return;
}

//////////////////
// CONV LAYER 1 //
//////////////////
/*
void CONVL1(FIX_FM in_fm[3][32][32], FIX_WT in_wt[32][3][3][3], FIX_FM out_fm[32][32][32]){
	// CONV layer. Padding=1 for same padding
	// (3x32x32) INPUT | 32 Channels 3x3 | (32x32x32) OUTPUT
	//#pragma HLS ALLOCATION function instances=L1DPU limit=32

	FIX_WT wt_bufL1[32][3][3][3];
	#pragma HLS ARRAY_PARTITION variable=wt_buf complete dim=1
	for(int ch=0; ch<32; ch++){
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				for(int k=0; j<3; k++){
					#pragma HLS pipeline
					wt_bufL1[ch][i][j][k] = in_wt[ch][i][j][k];
				}
			}
		}
	}

	// Buffer input FM with Padding
	FIX_FM in_padded[3][34][34];
	for(int ch=0; ch<3; ch++){
		// Padding
		for(int i=0; i<34; i++){
			//#pragma HLS pipeline
			in_padded[ch][0][i] = 0;
			in_padded[ch][i][0] = 0;
			in_padded[ch][33][i] = 0;
			in_padded[ch][i][33] = 0;
		}
		// Copy rest from input
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				in_padded[ch][j+1][k+1] = in_fm[ch][j][k];
			}
		}
	}

	for(int ch=0; ch<3; ch++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS unroll factor=4
				#pragma HLS pipeline
				int cAnchor[3] = {ch, j, k};
				L1DPU(in_padded, in_wt, cAnchor, &out_fm[ch][j][k]);
			}
		}
	}
}

void L1DPU(FIX_FM in_fm[3][34][34], FIX_WT in_wt[32][3][3][3], int anchor[3], FIX_FM *out){
	// Does volume convolution on one channel. (3x3x3)
	// anchor is the corresponding output location. (ch, j, k) from caller
	FIX_FM d_buf[3][3][3];
	FIX_WT w_buf[3][3][3];
	FIX_FM vbuf[3];
	FIX_FM obuf = 0;

	// Fill local buffers
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			for(int k=0; j<3; k++){
				#pragma HLS pipeline
				w_buf[i][j][k] = in_wt[anchor[0]][i][j][k];
				d_buf[i][j][k] = in_fm[i][anchor[1]+j][anchor[2]+k];
			}
		}
	}

	// Do convolution and write output
	for(int i=0; i<3; i++){
		//#pragma HLS unroll factor=3
		#pragma HLS pipeline
		CONV3X3(d_buf[i], w_buf[i], &vbuf[i]);
	}
	for(int i=0; i<3; i++){
		#pragma HLS pipeline
		obuf += vbuf[i];
	}
	*out = (obuf >= FIX_FM(0)) ? obuf : FIX_FM(0);
}
*/

//////////////////
// CONV LAYER 2 //
//////////////////

void CONVL2(FIX_FM in_fm[32][32][32], FIX_WT in_wt[32][32][3][3], FIX_FM out_fm[32][32][32]){
	// CONV layer. No buffer for out_fm. Padding=1 for same padding
	// (32x32x32) INPUT | 32 Channels 3x3 | (32x32x32) OUTPUT
	#pragma HLS ALLOCATION function instances=L2DPU limit=32

	FIX_WT wt_bufL2[32][32][3][3];
	#pragma HLS ARRAY_PARTITION variable=wt_bufL2 complete dim=1
	for(int ch=0; ch<32; ch++){
		for(int i=0; i<32; i++){
			for(int j=0; j<3; j++){
				for(int k=0; k<3; k++){
					#pragma HLS pipeline
					wt_bufL2[ch][i][j][k] = in_wt[ch][i][j][k];
				}
			}
		}
	}

	// Buffer input FM with padding
	FIX_FM in_padded[32][34][34];
	#pragma HLS ARRAY_PARTITION variable=in_padded complete dim=1
	for(int ch=0; ch<32; ch++){
		// Padding
		for(int i=0; i<34; i++){
			//#pragma HLS pipeline
			in_padded[ch][0][i] = 0;
			in_padded[ch][i][0] = 0;
			in_padded[ch][33][i] = 0;
			in_padded[ch][i][33] = 0;
		}
		// Copy rest from input
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS pipeline
				in_padded[ch][j+1][k+1] = in_fm[ch][j][k];
			}
		}
	}

	for(int ch=0; ch<32; ch++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				#pragma HLS unroll factor=32
				#pragma HLS pipeline
				int cAnchor[3] = {ch, j, k};
				L2DPU(in_padded, wt_bufL2, cAnchor, &out_fm[ch][j][k]);
			}
		}
	}
}


void L2DPU(FIX_FM in_fm[32][34][34], FIX_WT in_wt[32][32][3][3], int anchor[3], FIX_FM *out){
	// Does volume convolution on one channel. (32x3x3)
	// anchor is the corresponding output location. (ch, j, k) from caller
	#pragma HLS ALLOCATION function instances=CONV3X3 limit=32
	FIX_FM d_buf[32][3][3];
	#pragma HLS ARRAY_PARTITION variable=d_buf complete dim=1
	FIX_WT w_buf[32][3][3];
	#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1
	FIX_FM vbuf[32];
	#pragma HLS ARRAY_PARTITION variable=vbuf complete
	FIX_FM obuf = 0;

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
		CONV3X3(d_buf[i], w_buf[i], &vbuf[i]);
	}
	for(int i=0; i<32; i++){
		#pragma HLS pipeline
		obuf += vbuf[i];
	}
	*out = (obuf >= FIX_FM(0)) ? obuf : FIX_FM(0);
}

void CONV3X3(FIX_FM in_fm[3][3], FIX_WT in_wt[3][3], FIX_FM *out){
	// 3x3 2D convolution
	FIX_FM vbuf = 0;
	FIX_FM vout = 0;
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			#pragma HLS pipeline
			vbuf = in_fm[i][j] * in_wt[i][j];
			vout += vbuf;
		}
	}
	*out = vout;
}


