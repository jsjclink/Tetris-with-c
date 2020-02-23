#pragma once
/*if header file use global variables or struct... it is required to
declare things before declare of headerfile or in first header file.
*/

int blockset[7][4][4][4]; 
int map[20][14];

//
typedef struct G_block {
	int phase;
	int x, y;
	int arr[4][4][4];
}Block;

Block createblock() {
	Block block;
	block.x = 3;
	block.y = 0;
	clock_t CurTime = clock();

	int rphase, rtype;
	srand(CurTime);
	rphase = rand() % 3;
	rtype = rand() % 6;
	block.phase = rphase;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				block.arr[i][j][k] = blockset[rtype][i][j][k];
			}
		}
	}
	return block;
}