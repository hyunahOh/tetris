//1. 벽/ 블록 만들어주기
//2. game_map이라는 배열 -> 
//	타입 : Int
//	차원 : 2
//	크기 : 따로 MAX_X, MAX_Y, MIN_X, MIN_Y
//3. 블록 떨어지는거(SpaceBar, dropdown, move_block, rotate_block)
//4. 블록이 바닥에 떨어지면 다 찬 줄이 있는지 체크후, 삭제
//5. 블록의 종류가 랜덤하게 나오게
//6. FAIL조건(바닥에 떨어지면 블록이 천장과 겹치는 지 체크후, 겹치면 FAIL)
//7. score/speed/level

#include <stdio.h>
#include <windows.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32

#define MIN_Y 2
#define MAX_Y 25
#define MIN_X 0
#define MAX_X 15

#define EMPTY 0
#define WALL 1
#define BLOCK 2

int bx;
int by;

int game_map[MAX_Y][MAX_X];
int game_map_copy[MAX_Y][MAX_X];
char key;
int b_type;
int b_rotation;

int blocks[7][4][4][4] = {
{{0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0},{0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0},
{0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0},{0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0},{0,2,0,0,0,2,0,0,0,2,0,0,0,2,0,0},
 {0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0},{0,2,0,0,0,2,0,0,0,2,0,0,0,2,0,0}},
{{0,0,0,0,2,2,0,0,0,2,2,0,0,0,0,0},{0,0,0,0,0,0,2,0,0,2,2,0,0,2,0,0},
 {0,0,0,0,2,2,0,0,0,2,2,0,0,0,0,0},{0,0,0,0,0,0,2,0,0,2,2,0,0,2,0,0}},
{{0,0,0,0,0,2,2,0,2,2,0,0,0,0,0,0},{0,0,0,0,2,0,0,0,2,2,0,0,0,2,0,0},
 {0,0,0,0,0,2,2,0,2,2,0,0,0,0,0,0},{0,0,0,0,2,0,0,0,2,2,0,0,0,2,0,0}},
{{0,0,0,0,0,0,2,0,2,2,2,0,0,0,0,0},{0,0,0,0,2,2,0,0,0,2,0,0,0,2,0,0},
 {0,0,0,0,0,0,0,0,2,2,2,0,2,0,0,0},{0,0,0,0,0,2,0,0,0,2,0,0,0,2,2,0}},
{{0,0,0,0,2,0,0,0,2,2,2,0,0,0,0,0},{0,0,0,0,0,2,0,0,0,2,0,0,2,2,0,0},
 {0,0,0,0,0,0,0,0,2,2,2,0,0,0,2,0},{0,0,0,0,0,2,2,0,0,2,0,0,0,2,0,0}},
{{0,0,0,0,0,2,0,0,2,2,2,0,0,0,0,0},{0,0,0,0,0,2,0,0,0,2,2,0,0,2,0,0},
 {0,0,0,0,0,0,0,0,2,2,2,0,0,2,0,0},{0,0,0,0,0,2,0,0,2,2,0,0,0,2,0,0}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용

void check_key(void);
void reset_map(void);
void draw_map(void);
void new_block(void);
void move_block(int);
int check_crush(int, int, int);

void gotoxy(int x, int y) {
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hidecursor() {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

int main(void) {
	char key=0;
	system("cls");
	hidecursor();
	reset_map();
	new_block();
	draw_map();
	
	while (1) {
		check_key();
	}
	return 0;
}

void check_key() {
	if (_kbhit()) {
		key = _getch();
		switch (key)
		{
		case UP:
			if (check_crush(bx, by - 1, b_rotation) == 1) {
				move_block(UP);
			}
			break;
		case DOWN:
			if (check_crush(bx, by + 1, b_rotation) == 1) {
				move_block(DOWN);
			}
			break;
		case RIGHT:
			if (check_crush(bx + 1, by, b_rotation) == 1) {
				move_block(RIGHT);
			}
			break;
		case LEFT:
			if (check_crush(bx - 1, by, b_rotation) == 1) {
				move_block(LEFT);
			}
			break;
		case SPACE:
			
			break;
		default:
			break;
		}
		draw_map();
		//_getch(); // 쓰레기값이 찍혀서 넣어둠
	}

}

void reset_map(void) {
	int i, j;

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			game_map[i][j] = EMPTY;
		}
	}

	for (i = MIN_X; i < MAX_X; i++) {
		game_map[MIN_Y][i] = WALL;
		game_map[MAX_Y - 1][i] = WALL;
	}

	for (i = MIN_Y; i < MAX_Y; i++) {
		game_map[i][MIN_X] = WALL;
		game_map[i][MAX_X - 1] = WALL;
	}
}

void draw_map() {
	int i, j;
	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			if (game_map_copy[i][j] != game_map[i][j]) {
				gotoxy(j, i);
				switch (game_map[i][j])
				{
				case EMPTY:
					printf("  ");
					break;
				case WALL:
					printf("▩");
					break;
				case BLOCK:
					printf("■");
					break;
				default:
					break;
				}
			}
		}
	}
	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			game_map_copy[i][j] = game_map[i][j];
		}
	}
}
void new_block() {
	int i, j;

	bx = (MIN_X + MAX_X) / 2 - 2;
	by = (MIN_Y + MAX_Y) / 2 - 2;

	b_type = 4;
	b_rotation = 2;
	

	for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] == EMPTY 
					&& blocks[b_type][b_rotation][j][i] == BLOCK) {
					game_map[by + j][bx + i] = BLOCK;
				}
			}
		}
	
	
}

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] != WALL) {
					game_map[by + j][bx + i] = EMPTY;
				}
			}
		}
		bx--;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] == EMPTY
					&& blocks[b_type][b_rotation][j][i] == BLOCK) {
					game_map[by + j][bx + i] = BLOCK;
				}
			}
		}
		break;
	case RIGHT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] != WALL) {
					game_map[by + j][bx + i] = EMPTY;
				}
			}
		}
		bx++;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] == EMPTY
					&& blocks[b_type][b_rotation][j][i] == BLOCK) {
					game_map[by + j][bx + i] = BLOCK;
				}
			}
		}
		break;
	case UP:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] != WALL) {
					game_map[by + j][bx + i] = EMPTY;
				}
			}
		}
		by--;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] == EMPTY
					&& blocks[b_type][b_rotation][j][i] == BLOCK) {
					game_map[by + j][bx + i] = BLOCK;
				}
			}
		}
		break;
	case DOWN:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] != WALL) {
					game_map[by + j][bx + i] = EMPTY;
				}
			}
		}
		by++;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + j][bx + i] == EMPTY
					&& blocks[b_type][b_rotation][j][i] == BLOCK) {
					game_map[by + j][bx + i] = BLOCK;
				}
			}
		}
		break;
	}
}

int check_crush(int bx, int by, int b_rotation) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][j][i] != EMPTY && game_map[by + j][bx + i] == WALL) return 0;
		}
	}
	return 1;
}