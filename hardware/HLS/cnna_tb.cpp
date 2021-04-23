#include "cnna.h"

void CONV3X3_t(){
	printf("\n\n= = = = = Testing CONV3X3 = = = = =\n");

	FIX_FM t_d[3][3] = {{1,1,1},{2,2,2},{3,3,3}};
	FIX_WT t_w[3][3] = {{0.5, 0.5, 0.5},{0.5,0.5,0.5},{0.5,0.5,0.5}};
	FIX_FM t_o = 0;

	printf("INPUT:\n");
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			printf("%.3f\t", t_d[i][j]);
		}
		printf("\t | \t");
		for(int j=0; j<3; j++){
			printf("%.3f \t", t_w[i][j]);
		}
		printf("\n");
	}
	CONV3X3(t_d, t_w, &t_o);

	printf("OUTPUT: %.3f \n", t_o);
	printf("\n= = = = = END CONV3X3_t = = = = =\n");
}

void L2DPU_t(){
	printf("\n\n= = = = = Testing L2DPU = = = = =\n");

	FIX_FM t_d[32][32][32];
	FIX_WT t_w[32][32][3][3];
	int anchor[3] = {0,0,0};
	FIX_FM t_o = 0;

	//Populate Test Data:
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			for(int k=0; k<32; k++){
				t_d[i][j][k] = 0.1;
			}
			for(int jj=0; jj<3; jj++){
				for(int kk=0; kk<3; kk++){
					t_w[i][j][jj][kk] = 0.5;
				}
			}
		}
	}

	printf("INPUT:\n");
	printf("\tIFM at depth 0:\n");
	for(int jj=0; jj<3; jj++){
		for(int kk=0; kk<3; kk++){
			printf("%.3f \t", t_d[0][anchor[1]+jj][anchor[2]+kk]);
		}
		printf("\n");
	}

	printf("\tWeights at depth 0:\n");
	for(int jj=0; jj<3; jj++){
		for(int kk=0; kk<3; kk++){
			printf("%.3f \t", t_w[anchor[0]][0][jj][kk]);
		}
		printf("\n");
	}

	L2DPU(t_d, t_w, anchor, &t_o);

	printf("OUTPUT: %.3f \n", t_o);
	printf("\n= = = = = END L2DPU_t = = = = =\n");
}

int main() {
	printf("\n> > > > > TESTING BEGIN < < < < <\n\n");

	CONV3X3_t();
	L2DPU_t();

	printf("\n> > > > > TESTING END < < < < <\n\n");

	return 0;
}
