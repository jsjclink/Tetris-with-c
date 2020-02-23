#pragma once
int checkcollisiondownward(Block block) { //collide -> 1, or 0
	int lowest = checklowest(block);
	for (int i = 0; i < 4; i++) {
		if (block.arr[block.phase][lowest][i] == 1 && map[block.y + lowest + 1][i] == 1) {
			return 1; 
		}
	}
	return 0;
}


int checklowest(Block block) {
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 4; j++) {
			if (block.arr[block.phase][i][j] == 1) {
				return i;
			}
		}
	}
	return 0;
}

