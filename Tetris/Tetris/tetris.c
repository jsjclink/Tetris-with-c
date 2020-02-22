#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

void gotoxy(int x, int y);
void hidecursor();
void printblock(int(*arr)[4][4][4], int a, int block, int x, int y);
void getkey(int* x, int* y);

int arr[7][4][4][4];
int phase = 0;

int main(void) {
	system("chcp 437");
	clock_t CurTime, OldTime;
	int block = 1;
	int x = 10, y = 0;
	hidecursor();


	while (1) {
		OldTime = clock();
		system("cls");
		//Render
		getkey(&x, &y);
		printblock(arr, phase, block, x, y);
		y++;

		//Frame setting
		while (1) {
			CurTime = clock();
			if (CurTime - OldTime > 660) { break; }
		}

	}
}


void gotoxy(int x, int y) {
	COORD CursorPosition = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}
void hidecursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void printblock(int(*arr)[4][4][4], int phase, int block, int x, int y) {

	for (int i = 0; i < 4; i++) {
		gotoxy(x, y);
		for (int j = 0; j < 4; j++) {
			if (arr[block][phase%4][i][j] == 1) { printf("%c", 254); }
			else { printf("  "); }
		}
		y++;
	}
}
void getkey(int *x, int *y) {
	if (_kbhit()) {
		if (_getch() == 0xE0) {
			switch (_getch()) {
			case 72: phase++; break;
			case 75: (*x)--; break;
			case 77: (*x)++; break;
			}
		}
	}
}

int arr[7][4][4][4] = {

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
		1,1,0,0,
		0,1,1,0,
		0,0,0,0,

		0,1,0,0,
		1,1,0,0,
		1,0,0,0,
		0,0,0,0
};
