#pragma once
void gotoxy(int x, int y) {
	COORD CursorPosition = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

void addblock(Block block) { //add map(x,y) a block : 1
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block.arr[block.phase][i][j] == 1) {
				map[block.y + i][block.x + j] = 1;
			}
		}
	}
}

void removeblock(Block block) { //if not reached floor, remove and render again
	if (checkcollisiondownward(block)==1) { return; }
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block.arr[block.phase][i][j] == 1) {
				map[block.y + i][block.x + j] = 0;
			}
		}
	}
}

void rendermap() { //have to be called with addblock, removeblock
	int x = 0, y = 0;
	for (int i = 0; i < 20; i++) {
		gotoxy(x, y);
		for (int j = 0; j < 14; j++) {
			if (map[i][j] == 1) { printf("%c", 254); }
			else { printf("  "); }
		}
		y++;
	}
}



