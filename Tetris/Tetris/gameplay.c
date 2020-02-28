//Header file
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

//Global Variables Declaration
int map[20][14];
int blockset[7][4][4][4];
int testblock[4][4];
int block_x[4];
int block_y[4];
int tempblock_x[4];
int tempblock_y[4];
int turncount = 0;
int stage = 0;
int iswaiting = 1;
int blocktype = 0;
int blockphase = 0;
int originalpoint_x = 0;
int originalpoint_y = 0;

//Functions Declararion
void Gotoxy(int x, int y);
void PrintMap();
void PrintStatus();
void PressAnyKey();
void CreateBlock();
void PrintBlock();
void BlockMoveDown();
int CheckBlockCollisionDown();
void RemoveBlock();
void AddBlocktoMap();
void GetKbInput();
void BlockMoveLeft();
void BlockMoveRight();
int CheckBlockCollisionLeft();
int CheckBlockCollisionRight();
void ChooseRandomBlock();
void RotateBlock();
int CheckBlockCollisionRotate();



void GamePlay() {
	//Initialize
	CreateBlock();

	PrintMap();
	PrintStatus();
	PrintBlock();
	while (1) {
		//init
		PrintStatus();

		//wait
		_getch();
		iswaiting = 0;
		turncount++;
		PrintStatus();

		//keyboardinput
		GetKbInput();
		
		//implement before print
		if (CheckBlockCollisionDown() == 0) {
			BlockMoveDown();
		}
		else {
			RemoveBlock();
			AddBlocktoMap();
			stage++;
			CreateBlock();
		}
		
		//print
		PrintMap();
		PrintStatus();
		PrintBlock();


		//terminate
		Sleep(1000);
		iswaiting = 1;
	}//while
}

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

void PrintStatus() {
	Gotoxy(15, 0);
	printf("stage : %2d", stage);
	for (int i = 0; i < 4; i++) {
		Gotoxy(15, 2*i+1);
		printf("block_x[%d] : %3d", i, block_x[i]);
		Gotoxy(15, 2 * i + 2);
		printf("block_y[%d] : %3d", i, block_y[i]);
	}
	Gotoxy(15, 9);
	printf("turncount : %3d", turncount);
	Gotoxy(15, 10);
	printf("iswaiting : %d", iswaiting);
}

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
			if (testblock[j][i] == 1) { startpoint_x = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}
	isfound = 0;

	for (int i = 0; i < 4; i++) { //y
		for (int j = 0; j < 4; j++) {
			if (testblock[i][j] == 1) { startpoint_y = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}
	isfound = 0;

	//find endpoint
	for (int i = 3; i >= 0; i--) { //x
		for (int j = 0; j < 4; j++) {
			if (testblock[j][i] == 1) { endpoint_x = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}
	isfound = 0;

	for (int i = 3; i >= 0; i--) { //y
		for (int j = 0; j < 4; j++) {
			if (testblock[i][j] == 1) { endpoint_y = i; isfound = 1; break; }
		}
		if (isfound == 1) { break; }
	}

	//allocate position to each block
	for (int i = startpoint_y; i <= endpoint_y; i++) {
		for (int j = startpoint_x; j <= endpoint_x; j++) {
			if (testblock[i][j] == 1) {
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

void PrintBlock() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("@");
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

int CheckBlockCollisionDown() {
	for (int i = 0; i < 4; i++) {
		if (map[block_y[i]+1][block_x[i]] == 1) { //한칸 밑에 블럭 있는지 확인
			return 1;
		}
	}
	return 0;
}

void RemoveBlock() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("%c", 0);
	}
}

void AddBlocktoMap() {
	for (int i = 0; i < 4; i++) {
		map[block_y[i]][block_x[i]] = 1;
		block_x[i] = 0;
		block_y[i] = 0;
	}
}

void GetKbInput() {
	char inp;
	inp = _getch();
	switch (inp) {
	case 'j':{
		if (CheckBlockCollisionLeft() == 1) { break; }
		BlockMoveLeft();
		break;
	}
	case 'l': {
		if (CheckBlockCollisionRight() == 1) { break; }
		BlockMoveRight();
		break;
	}
	case 'c': {
		if (CheckBlockCollisionRotate()) { break; }
		RemoveBlock();
		blockphase++;
		blockphase = blockphase % 4;
		RotateBlock();
		break;
	}
	}
}

void BlockMoveLeft() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("%c", 0);
		block_x[i]--;
	}
	originalpoint_x--;
}

void BlockMoveRight() {
	for (int i = 0; i < 4; i++) {
		Gotoxy(block_x[i], block_y[i]);
		printf("%c", 0);
		block_x[i]++;
	}
	originalpoint_x++;
}

int CheckBlockCollisionLeft() {
	for (int i = 0; i < 4; i++) {
		if (map[block_y[i]][block_x[i]-1] == 1) { //한칸 왼쪽에 블럭 있는지 확인
			return 1;
		}
	}
	return 0;
}
int CheckBlockCollisionRight() {
	for (int i = 0; i < 4; i++) {
		if (map[block_y[i]][block_x[i]+1] == 1) { //한칸 오른쪽에 블럭 있는지 확인
			return 1;
		}
	}
	return 0;
}

void ChooseRandomBlock() {
	srand(time(NULL));
	blocktype = rand()%7;
	blockphase = rand()%4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			testblock[i][j] = blockset[blocktype][blockphase][i][j];
		}
	}
}
void RotateBlock() {
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
			blockphase += 3 ;
			blockphase = blockphase % 4;

			return 1; 
		}
	}
	//blockphase-- 를 하기 위해 +3, %4를 해줌(0에서 --하면 -1이 될 수도 있기 때문에 덧셈만 함)
	blockphase += 3;
	blockphase = blockphase % 4;
	return 0;
}





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