//Header file
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

//Global Variables Declaration
int map[20][14];

int blockset[7][4][4][4];
int block[4][4];
int block_x[4];
int block_y[4];
int blocktype = 0;
int blockphase = 0;

int originalpoint_x = 0;
int originalpoint_y = 0;
int tempblock_x[4];
int tempblock_y[4];

int templines[19][12];
int clearlineposition[5];

int score = 0;

//Functions Declararion
void GetKeyInput();

void Gotoxy(int x, int y);
void PrintMap();
void PrintBlock();
void EraseBlock();//name change erase block
void PrintStatus();

void CreateBlock();
void ChooseRandomBlock();
void AddBlocktoMap();
void BlockMoveDown();
void BlockMoveLeft();
void BlockMoveRight();
void RotateBlock();

int CheckBlockCollisionDown();
int CheckBlockCollisionLeft();
int CheckBlockCollisionRight();
int CheckBlockCollisionRotate();

void CheckLineClear();
void LineClear();
void RemoveLine(int lineidx);
void MoveLinesDownward(int lineidx);

/***************************************************************************/

void GamePlay() {
	//Init
	int framecount = 0;

	clock_t CurTime, OldTime, OldTimeBlockMove;
	OldTime = clock();
	OldTimeBlockMove = clock();

	CreateBlock();
	
	while (1) {
		//Data Update
		//Keyboard input
		GetKeyInput();

		//Implementation before print(BlockMoveDown, LineClear)
		CurTime = clock();
		if (CurTime - OldTimeBlockMove > 500) {
			OldTimeBlockMove = CurTime;
			//Implementation
			if (CheckBlockCollisionDown() == 0) {
				BlockMoveDown();
			}
			else {
				EraseBlock();
				AddBlocktoMap();
				CheckLineClear();
				LineClear();
				system("cls");
				CreateBlock();
				//Add Score

			}
		}

		//Print
		PrintMap();
		PrintBlock();
		PrintStatus();

		//Wait
		while (1) {
			CurTime = clock();
			if (CurTime - OldTime > 33) {
				OldTime = CurTime;
				break;
			}
		}
		framecount++;
	}
}

/***************************************************************************/

void GetKeyInput() {
	int inp;
	if (_kbhit()) {
		inp = _getch();
		if (inp == 'c') {
			if (CheckBlockCollisionRotate()==0) {
				RotateBlock();
			}
		}
		switch (inp) {
		case 'j': {
			if (CheckBlockCollisionLeft()) { break; }
			BlockMoveLeft();
			break;
		}
		case 'l': {
			if (CheckBlockCollisionRight()) { break; }
			BlockMoveRight();
			break;
		}
		}
	}
}

/***************************************************************************/
void Gotoxy(int x, int y) {
	COORD CursorPosition = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

void PrintMap() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 14; j++) {
			if (map[i][j] == 1) {
				Gotoxy(j, i);
				printf("#");
			}
		}
	}
}

void PrintBlock() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("@");
	}
}

void EraseBlock() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("%c", 0);
	}
}

void PrintStatus() {
	Gotoxy(15, 0);
	printf("score : %4d", score*100);
	for (int i = 0; i < 4; i++) {
		Gotoxy(15, 2 * i + 1);
		printf("block_x[%d] : %3d", i, block_x[i]);
		Gotoxy(15, 2 * i + 2);
		printf("block_y[%d] : %3d", i, block_y[i]);
	}
}

/***************************************************************************/

void CreateBlock() {
	//initialization
	ChooseRandomBlock();
	int startplace_x = 5, startplace_y = 0;
	int startpoint_x = 0, startpoint_y = 0;
	int endpoint_x = 3, endpoint_y = 3;
	int isfound = 0;
	int count = 0;
	//find startpoint
	for (int i = 0; i < 4; i++) { //x
		for (int j = 0; j < 4; j++) {
			if (block[j][i] == 1) { startpoint_x = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}
	isfound = 0;

	for (int i = 0; i < 4; i++) { //y
		for (int j = 0; j < 4; j++) {
			if (block[i][j] == 1) { startpoint_y = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}
	isfound = 0;

	//find endpoint
	for (int i = 3; i >= 0; i--) { //x
		for (int j = 0; j < 4; j++) {
			if (block[j][i] == 1) { endpoint_x = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}
	isfound = 0;

	for (int i = 3; i >= 0; i--) { //y
		for (int j = 0; j < 4; j++) {
			if (block[i][j] == 1) { endpoint_y = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}

	//allocate position to each block
	for (int i = startpoint_y; i <= endpoint_y; i++) {
		for (int j = startpoint_x; j <= endpoint_x; j++) {
			if (block[i][j] == 1) {
				block_x[count] = j - startpoint_x + startplace_x;
				block_y[count] = i - startpoint_y + startplace_y;
				count++;
			}
		}
	}

	//calculate originalpoint
	originalpoint_x = block_x[0] - startpoint_x;
	originalpoint_y = block_y[0] - startpoint_y;
}

void ChooseRandomBlock() {
	srand(time(NULL));
	blocktype = rand() % 7;
	blockphase = rand() % 4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = blockset[blocktype][blockphase][i][j];
		}
	}
}

void AddBlocktoMap() {
	for (int i = 0; i < 4; i++) {
		map[block_y[i]][block_x[i]] = 1;
		block_x[i] = 0;
		block_y[i] = 0;
	}
}

void BlockMoveDown() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("%c", 0);
		block_y[i]++;
	}
	originalpoint_y++;
}

void BlockMoveLeft() {
	EraseBlock();
	for (int i = 0; i < 4; i++) {
		block_x[i]--;
	}
	originalpoint_x--;
}

void BlockMoveRight() {
	EraseBlock();
	for (int i = 0; i < 4; i++) {
		block_x[i]++;
	}
	originalpoint_x++;
}

void RotateBlock() {
	EraseBlock();
	blockphase++;
	blockphase = blockphase % 4;

	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blockset[blocktype][blockphase][i][j] == 1) {
				block_x[count] = j + originalpoint_x;
				block_y[count] = i + originalpoint_y;
				count++;
			}
		}
	}
}



/***************************************************************************/
int CheckBlockCollisionDown() {
	for (int i = 0; i < 4; i++) {
		if (map[block_y[i] + 1][block_x[i]] == 1) { //한칸 밑에 블럭 있는지 확인
			return 1;
		}
	}
	return 0;
}

int CheckBlockCollisionLeft() {
	for (int i = 0; i < 4; i++) {
		if (map[block_y[i]][block_x[i] - 1] == 1) { //한칸 왼쪽에 블럭 있는지 확인
			return 1;
		}
	}
	return 0;
}

int CheckBlockCollisionRight() {
	for (int i = 0; i < 4; i++) {
		if (map[block_y[i]][block_x[i] + 1] == 1) { //한칸 오른쪽에 블럭 있는지 확인
			return 1;
		}
	}
	return 0;
}

int CheckBlockCollisionRotate() {
	//init
	int count = 0;
	//한번 돌린 후를 확인하기 위해 blockphase++
	blockphase++;
	blockphase = blockphase % 4;

	for (int i = 0; i < 4; i++) { //한칸 돌린 상태를 tempblock_x, tempblock_y에 저장
		for (int j = 0; j < 4; j++) {
			if (blockset[blocktype][blockphase][i][j] == 1) {
				tempblock_x[count] = j + originalpoint_x;
				tempblock_y[count] = i + originalpoint_y;
				count++;
			}
		}
	}

	for (int i = 0; i < 4; i++) { //temp_x, temp_y중에서 map에 충돌하는 것이 있는지 확인
		if (map[tempblock_y[i]][tempblock_x[i]] == 1) {
			//blockphase-- 를 하기 위해 +3, %4를 해줌(0에서 --하면 -1이 될 수도 있기 때문에 덧셈만 함)
			blockphase += 3;
			blockphase = blockphase % 4;

			return 1;
		}
	}
	//blockphase-- 를 하기 위해 +3, %4를 해줌(0에서 --하면 -1이 될 수도 있기 때문에 덧셈만 함)
	blockphase += 3;
	blockphase = blockphase % 4;
	return 0;
}

/***************************************************************************/
void CheckLineClear() {
	//init
	int blockcount;
	int k;
	memset(clearlineposition, -1, sizeof(clearlineposition));

	//check from the bottom to top
	for (int i = 18; i >= 0; i--) {
		blockcount = 0;
		for (int j = 1; j < 13; j++) {
			if (map[i][j] == 1) { blockcount++; }
		}
		if (blockcount == 12) { //if all line is full of block, add line position(index) to clearlineposition[]
			k = 0;
			while (clearlineposition[k] != -1) {
				k++;
			}
			clearlineposition[k] = i;
		}
	}
}

void LineClear() {
	for (int i = 0; i < 5; i++) {
		if (clearlineposition[i] == -1) { return; }
		RemoveLine(clearlineposition[i]);
		MoveLinesDownward(clearlineposition[i]);
	}
}

void RemoveLine(int lineidx) {
	for (int i = 1; i < 13; i++) {
		map[lineidx][i] = 0;
	}
}

void MoveLinesDownward(int lineidx) {
	for (int i = lineidx - 1; i >= 0; i--) {
		for (int j = 1; j < 13; j++) {
			if (map[i][j] == 1) {
				map[i][j] = 0;
				map[i + 1][j] = 1;
			}
		}
	}
}

/***************************************************************************/

//Arrays

int map[20][14] = {
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1
};


int testblock[4][4] = {
	0,0,1,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int blockset[7][4][4][4] = {
		0,0,0,0,
		1,1,1,1,
		0,0,0,0,
		0,0,0,0,

		0,0,1,0,
		0,0,1,0,
		0,0,1,0,
		0,0,1,0,

		0,0,0,0,
		0,0,0,0,
		1,1,1,1,
		0,0,0,0,

		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
		0,1,0,0,

		1,0,0,0,
		1,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,1,0,
		0,1,0,0,
		0,1,0,0,
		0,0,0,0,

		0,0,0,0,
		1,1,1,0,
		0,0,1,0,
		0,0,0,0,

		0,1,0,0,
		0,1,0,0,
		1,1,0,0,
		0,0,0,0,

		0,0,1,0,
		1,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,0,0,
		0,1,0,0,
		0,1,1,0,
		0,0,0,0,

		0,0,0,0,
		1,1,1,0,
		1,0,0,0,
		0,0,0,0,

		1,1,0,0,
		0,1,0,0,
		0,1,0,0,
		0,0,0,0,


		0,1,1,0,
		0,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,1,0,
		0,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,1,0,
		0,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,1,0,
		0,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,1,0,
		1,1,0,0,
		0,0,0,0,
		0,0,0,0,

		0,1,0,0,
		0,1,1,0,
		0,0,1,0,
		0,0,0,0,

		0,0,0,0,
		0,1,1,0,
		1,1,0,0,
		0,0,0,0,

		1,0,0,0,
		1,1,0,0,
		0,1,0,0,
		0,0,0,0,

		0,1,0,0,
		1,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,0,0,
		0,1,1,0,
		0,1,0,0,
		0,0,0,0,

		0,0,0,0,
		1,1,1,0,
		0,1,0,0,
		0,0,0,0,

		0,1,0,0,
		1,1,0,0,
		0,1,0,0,
		0,0,0,0,

		1,1,0,0,
		0,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,0,1,0,
		0,1,1,0,
		0,1,0,0,
		0,0,0,0,

		0,0,0,0,
		1,1,0,0,
		0,1,1,0,
		0,0,0,0,

		0,1,0,0,
		1,1,0,0,
		1,0,0,0,
		0,0,0,0
};
