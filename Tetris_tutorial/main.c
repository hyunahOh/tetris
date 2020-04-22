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
char key;

void check_key(void);
void reset_map(void);
void draw_map(void);
void new_block(void);
void move_block(int);

void gotoxy(int x, int y) {
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main(void) {
	char key=0;
	
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
			move_block(UP);
			break;
		case DOWN:
			move_block(DOWN);
			break;
		case RIGHT:
			move_block(RIGHT);
			break;
		case LEFT:
			move_block(LEFT);
			break;
		case SPACE:
			move_block(SPACE);
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
	system("cls");
	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
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
void new_block() {
	int i, j;

	bx = (MIN_X + MAX_X) / 2;
	by = (MIN_Y + MAX_Y) / 2;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			game_map[by + j][bx + i] = BLOCK;
		}
	}
}

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		game_map[by][bx - 1] = BLOCK;
		game_map[by + 1][bx - 1] = BLOCK;
		game_map[by][bx + 1] = EMPTY;
		game_map[by + 1][bx + 1] = EMPTY;
		bx--;
		break;
	case RIGHT:
		game_map[by][bx + 2] = BLOCK;
		game_map[by + 1][bx + 2] = BLOCK;
		game_map[by][bx] = EMPTY;
		game_map[by + 1][bx] = EMPTY;
		bx++;
		break;
	case UP:
		game_map[by - 1][bx] = BLOCK;
		game_map[by - 1][bx + 1] = BLOCK;
		game_map[by + 1][bx] = EMPTY;
		game_map[by + 1][bx + 1] = EMPTY;
		by--;
		break;
	case DOWN:
		game_map[by + 2][bx] = BLOCK;
		game_map[by + 2][bx + 1] = BLOCK;
		game_map[by][bx] = EMPTY;
		game_map[by][bx + 1] = EMPTY;
		by++;
		break;
	}
}