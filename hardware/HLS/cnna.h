#include "ap_axi_sdata.h"

//#define CSIM_DEBUG
// NN Data Types
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

void cnna(FIX_FM in_data[3][3][3], FIX_WT in_weights[3][3][3], FIX_FM out[3]);
void CONV3X3(FIX_FM in_fm[3][3], FIX_WT in_wt[3][3], FIX_FM *out);
void CONVDEPTH32(FIX_FM in_fm[32][3][3], FIX_WT in_wt[32][3][3], FIX_FM *out);
void CONVL2(FIX_FM in_fm[32][32][32], FIX_WT in_wt[32][32][3][3], FIX_FM out_fm[32][32][32]);

/*
#ifdef CSIM_DEBUG
	typedef float FIX_32_4;	//fix point
	typedef float FIX_32_25;	//fix point
	typedef float FIX_FM;	//fix point for feature map
	typedef float FIX_WT;	//fix point for weights
	typedef float FIX_32_16;	//fix point
	typedef float FIX_32_12;	//fix point
	typedef float FIX_16_1;
#else
	typedef ap_fixed<16, 6, AP_TRN_ZERO, AP_SAT> FIX_FM;	//fix point for feature map
	typedef ap_fixed<8,  1, AP_TRN_ZERO, AP_SAT> FIX_WT;	//fix point for weights
	typedef ap_fixed<16, 1, AP_TRN_ZERO, AP_SAT> FIX_16_1;	//fix point for weights
	typedef ap_fixed<32,16, AP_TRN_ZERO, AP_SAT> FIX_32_16;	//fix point
	typedef ap_fixed<32,12, AP_TRN_ZERO, AP_SAT> FIX_32_12;
	typedef ap_fixed<32, 4, AP_TRN_ZERO, AP_SAT> FIX_32_4;	//fix point
	typedef ap_fixed<32,25, AP_TRN_ZERO, AP_SAT> FIX_32_25;	//fix point
#endif
*/

