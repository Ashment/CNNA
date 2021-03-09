#include "ap_axi_sdata.h"

// General Data Types
typedef ap_axiu<32,1,1,1> stream_type;
typedef ap_uint<8> uint8;
typedef ap_int<8> int8;


// NN Data Types
typedef ap_fixed<16, 6, AP_TRN_ZERO, AP_SAT> FIX_FM;	//Fixed Point for Feature Map
typedef ap_fixed<8, 1, AP_TRN_ZERO, AP_SAT> FIX_WT; 	//Fixed Point for Weights


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
