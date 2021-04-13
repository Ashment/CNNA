#include "cnna.h"

int main() {
	printf("TESTING BEGIN\n");


	FIX_FM t_d[3][3] = {{1,1,1},{2,2,2},{3,3,3}};
	FIX_WT t_w[3][3] = {{0.5, 0.5, 0.5},{0.5,0.5,0.5},{0.5,0.5,0.5}};
	FIX_FM t_o = 0;

	CONV3X3(t_d, t_w, &t_o);

	for(int i=0; i<3; i++){
		printf("%.3f \t", t_o[i]);
	}

	printf("\nTESTING END\n");

	return 0;
}
