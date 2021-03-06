#include "ap_axi_sdata.h"

//#define CSIM_DEBUG

//////////////////
//  Data Types  //
//////////////////

#ifdef CSIM_DEBUG
	typedef int uint8;
	typedef int int8;

	typedef float FIX_FM;	//Fixed Point for Feature Map
	typedef float FIX_WT; 	//Fixed Point for Weights
#else
	typedef ap_uint<8> uint8;
	typedef ap_int<8> int8;

	typedef ap_fixed<16, 6, AP_TRN_ZERO, AP_SAT> FIX_FM;	//Fixed Point for Feature Map
	typedef ap_fixed<8, 1, AP_TRN_ZERO, AP_SAT> FIX_WT; 	//Fixed Point for Weights
#endif

///////////////////////////
//  Classes and Methods  //
///////////////////////////

class CONV3X3PU;

void cnna(FIX_FM in_data[32][32][32], FIX_WT wtL2[32][32][3][3], FIX_FM out[32][32][32]);

//void CONVL1(FIX_FM in_fm[3][32][32], FIX_WT in_wt[32][3][3][3], FIX_FM out_fm[32][32][32]);
//void L1DPU(FIX_FM in_fm[3][34][34], FIX_WT in_wt[32][3][3][3], int anchor[3], FIX_FM *out);

void CONVL2(FIX_FM in_fm[32][32][32], FIX_WT in_wt[32][32][3][3], FIX_FM out_fm[32][32][32]);
void L2DPU(FIX_FM in_fm[32][34][34], FIX_WT in_wt[32][32][3][3], int anchor[3], FIX_FM *out);

void CONV3X3(FIX_FM in_fm[3][3], FIX_WT in_wt[3][3], FIX_FM *out);
