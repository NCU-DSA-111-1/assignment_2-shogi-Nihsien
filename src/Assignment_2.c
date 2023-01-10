#include <stdio.h>
#include <stdlib.h>
#include "Assignment_2.h"
#include <string.h>

struct goma {
	int Num, Kind, x, y, owner, chesser;
};

struct move {
	struct move* next;
	int x1, x2;
	int y1, y2;
};




int main(int argc, char* argv[]) {
	struct goma* Goma;
	struct goma Goma1, Goma2;
	struct move* DataList = NULL;
	struct move* DataQueue = NULL;
	struct move OldgameGoma;
	struct move Move;
	int turn = 0;
	int x, y, x2, y2;
	char xin, yin, xin2, yin2;
	char* filename;

	int** GameBoard = (int**)malloc(DAN * sizeof(int*));
	for (int i = 0; i < DAN; i++) {
		GameBoard[i] = (int*)malloc(SUJI * sizeof(int));
	}
	Goma = (struct goma*)malloc(sizeof(struct goma) * TOTALNUM * 2);
	shogi(Goma);
	BoardUpdate(Goma, GameBoard);

	if (strcmp(argv[1], "-l") == 0) {//開舊局
		PrintBoard(GameBoard, Goma);
		filename = argv[2];
		char control;
		DataList = OpenOldGame(filename);
		while (DataList != NULL) {
			Pop(&DataList, &OldgameGoma);
			Push(&DataQueue, OldgameGoma);
			//printf("%d %d %d %d \n", DataQueue->x1, DataQueue->y1, DataQueue->x2, DataQueue->y2);
		}
		while (1) {
			printf("輸入'f'移動下一手，輸入'b'退回上一手:");
			do { 
				control = getc(stdin); 
			} while (control == ' ' || control == '\n');

			if (control == 'f' && DataQueue != NULL) {
				Pop(&DataQueue, &OldgameGoma);
				x = OldgameGoma.x1;
				y = OldgameGoma.y1;
				x2 = OldgameGoma.x2;
				y2 = OldgameGoma.y2;
				if (GameBoard[x2 - 1][y2 - 1] != EMPTY) {
					Goma[(GameBoard[x2 - 1][y2 - 1] - 1) % 40].x = 0;
					Goma[(GameBoard[x2 - 1][y2 - 1] - 1) % 40].y = 0;
					Goma[(GameBoard[x2 - 1][y2 - 1] - 1) % 40].chesser = Goma[(GameBoard[x - 1][y - 1] - 1) % 40].chesser;
				}
				Goma[(GameBoard[x - 1][y - 1] - 1) % 40].x = x2;
				Goma[(GameBoard[x - 1][y - 1] - 1) % 40].y = y2;
				Push(&DataList, OldgameGoma);
				BoardUpdate(Goma, GameBoard);
				PrintBoard(GameBoard, Goma);
			}
			else if (control == 'f' && DataQueue == NULL) {
				printf("這是最後一步\n");
			}
			else if (control == 'b' && DataList == NULL) {
				printf("這是第一步\n");
			}
			else if (control == 'b' && DataList != NULL) {
				Pop(&DataList,&OldgameGoma);
				x=OldgameGoma.x2;
				y=OldgameGoma.y2;
				x2=OldgameGoma.x1;
				y2=OldgameGoma.y1;
				if (GameBoard[x2 - 1][y2 - 1] != EMPTY) {
					Goma[(GameBoard[x2 - 1][y2 - 1]-1)%40].x = 0;
					Goma[(GameBoard[x2 - 1][y2 - 1]-1)%40].y = 0;
					Goma[(GameBoard[x2 - 1][y2 - 1]-1) % 40].chesser = Goma[(GameBoard[x - 1][y - 1]-1) % 40].chesser;
				}
				Goma[(GameBoard[x - 1][y - 1]-1)%40].x = x2;
				Goma[(GameBoard[x - 1][y - 1]-1)%40].y = y2;
				Push(&DataQueue, OldgameGoma);
				BoardUpdate(Goma, GameBoard);
				PrintBoard(GameBoard, Goma);
			}
			else printf("輸入錯誤!\n");
			if ((Goma[17].chesser != Goma[17].owner) || (Goma[37].chesser != Goma[37].owner)) {
				printf("將死，遊戲結束\n");
				break;
			}
		}
	}
	else if (strcmp(argv[1], "-n") == 0 && strcmp(argv[2], "-s") == 0) {//開新局
		PrintBoard(GameBoard, Goma);
		filename = argv[3];
		while (1) {
			printf("輸入欲移動的棋子座標(S/s儲存棋盤，0悔棋):");
			do {
				xin = getc(stdin);
			} while (xin == ' ' || xin == '\n');
			if (xin == '0' || xin == 'S' || xin == 's') {
				yin = xin;
			}
			else {
				do {
					yin = getc(stdin);
				} while (yin == ' ' || yin == '\n');
			}
			if ((xin == '0' || yin == '0') && turn != 0) {
				Pop(&DataList, &Move);
				Goma[(GameBoard[Move.x2 - 1][Move.y2 - 1] - 1) % 40].x = Move.x1;
				Goma[(GameBoard[Move.x2 - 1][Move.y2 - 1] - 1) % 40].y = Move.y1;
				turn = turn - 1;
				BoardUpdate(Goma, GameBoard);
				PrintBoard(GameBoard,Goma);
				printf("退回上一步，換Player%d輸入。\n", (turn % 2) + 1);
			}
			else if ((xin == '0' || yin == '0') && turn == 0) {
				printf("無法悔棋，重新輸入。\n");
			}
			else if (xin > '0' && xin <= '9' && yin > '0' && yin <= '9') {
				x = xin - '0';
				y = yin - '0';
				if (JudgeBeforeMove(GameBoard, &turn, Goma, DataList, x, y, &Goma1)) {
					while (1) {
						printf("輸入欲移動至的棋子座標:");
						do {
							xin2 = getc(stdin);
						} while (xin2 == ' ' || xin2 == '\n');
						if (xin2 == 's' || xin2 == 'S' || xin2 == '0') {
							yin2 = xin2;
						}
						else {
							do {
								yin2 = getc(stdin);
							} while (yin2 == ' ' || yin2 == '\n');
						}
						if ((xin2 == '0' || yin2 == '0') && turn!=0) {
							Pop(&DataList, &Move);
							Goma[(GameBoard[Move.x2 - 1][Move.y2 - 1] - 1) % 40].x = Move.x1;
							Goma[(GameBoard[Move.x2 - 1][Move.y2 - 1] - 1) % 40].y = Move.y1;
							turn = turn - 1;
							BoardUpdate(Goma, GameBoard);
							PrintBoard(GameBoard, Goma);
							printf("退回上一步，換Player%d輸入。\n", (turn % 2) + 1);
						}
						else if ((xin2 == '0' || yin2 == '0') && turn == 0) {
							printf("無法悔棋，重新輸入。\n");
						}
						else if (xin2 > '0' && xin2 <= '9' && yin2 > '0' && yin2 <= '9') {
							x2 = xin2 - '0';
							y2 = yin2 - '0';
							if (JudgeToMove(GameBoard, &turn, Goma, DataList, x, y, x2, y2, &Goma1, &Goma2)) {
								Move.x1 = x;
								Move.y1 = y;
								Move.x2 = x2;
								Move.y2 = y2;
								Push(&DataList, Move);
								printf("%c %c -> %c %c\n", xin, yin, xin2, yin2);
								break;
							}
						}
						else if (xin2 == 's' || xin2 == 'S' || yin2 == 's' || yin2 == 'S') {//存檔
							while (DataList != NULL) {
								Pop(&DataList, &Move);
								Push(&DataQueue, Move);
							}
							StoreBoard(DataQueue, filename);
							while (DataQueue != NULL) {
								Pop(&DataQueue, &Move);
								Push(&DataList, Move);
							}
							printf("儲存棋盤 遊戲結束\n");
							return 0;
						}
						else {
							printf("輸入錯誤!\n");
						}
					}
				}
			}
			else if (xin == 's' || xin == 'S' || yin == 's' || yin == 'S') {
				while (DataList != NULL) {
					Pop(&DataList, &Move);
					Push(&DataQueue, Move);
				}
				StoreBoard(DataQueue, filename);
				while (DataQueue != NULL) {
					Pop(&DataQueue, &Move);
					Push(&DataList, Move);
				}
				printf("儲存棋盤 遊戲結束\n");
				return 0;
			}
			else {
				printf("輸入錯誤!\n");
			}
			if ((Goma[17].chesser != Goma[17].owner) || (Goma[37].chesser != Goma[37].owner)) {
				printf("將死，遊戲結束\n");
				return 0;
			}
		}
	}
	else {
		printf("輸入錯誤!\n");
	}
	return 0;
}

struct goma* shogi(struct goma* Player) {
	Player[0].Kind = FOOT;
	Player[1].Kind = FOOT;
	Player[2].Kind = FOOT;
	Player[3].Kind = FOOT;
	Player[4].Kind = FOOT;
	Player[5].Kind = FOOT;
	Player[6].Kind = FOOT;
	Player[7].Kind = FOOT;
	Player[8].Kind = FOOT;
	Player[9].Kind = SILVER;
	Player[10].Kind = SILVER;
	Player[11].Kind = CASSIA;
	Player[12].Kind = CASSIA;
	Player[13].Kind = CHARIOT;
	Player[14].Kind = CHARIOT;
	Player[15].Kind = FLYING;
	Player[16].Kind = ANGLE;
	Player[17].Kind = KING;
	Player[18].Kind = GOLD;
	Player[19].Kind = GOLD;

	for (int i = 0; i < 2 * TOTALNUM; i++) {
		Player[i].Num = i + 1;
		Player[i].owner = i / TOTALNUM + 1; /*1是我方 2是敵方*/
		Player[i].chesser = i / TOTALNUM + 1; /*1是我方 2是敵方*/
	}
	/*棋子的初始化位置*/
	Player[0].x = 1; Player[0].y = 7;
	Player[1].x = 2; Player[1].y = 7;
	Player[2].x = 3; Player[2].y = 7;
	Player[3].x = 4; Player[3].y = 7;
	Player[4].x = 5; Player[4].y = 7;
	Player[5].x = 6; Player[5].y = 7;
	Player[6].x = 7; Player[6].y = 7;
	Player[7].x = 8; Player[7].y = 7;
	Player[8].x = 9; Player[8].y = 7;
	Player[9].x = 3; Player[9].y = 9;
	Player[10].x = 7; Player[10].y = 9;
	Player[11].x = 2; Player[11].y = 9;
	Player[12].x = 8; Player[12].y = 9;
	Player[13].x = 1; Player[13].y = 9;
	Player[14].x = 9; Player[14].y = 9;
	Player[15].x = 2; Player[15].y = 8;
	Player[16].x = 8; Player[16].y = 8;
	Player[17].x = 5; Player[17].y = 9;
	Player[18].x = 6; Player[18].y = 9;
	Player[19].x = 4; Player[19].y = 9;

	for (int i = TOTALNUM; i < 2 * TOTALNUM; i++) {
		Player[i].x = Player[i - TOTALNUM].x;
		Player[i].y = 10 - Player[i - TOTALNUM].y;
		Player[i].Kind = Player[i - TOTALNUM].Kind;
	}
	return (Player);
}


int JudgeBeforeMove(int** board, int* round, struct goma* player, struct move* list, int x, int y, struct goma* movingGoma) {
	struct move Move;
	movingGoma->Num = board[x - 1][y - 1];
	if (movingGoma->Num == EMPTY) {
		printf("此處為空棋格，重新輸入。\n");
		return 0;
	}
	else {
		movingGoma->x = x;
		movingGoma->y = y;
		movingGoma->chesser = player[movingGoma->Num % 40].chesser;
		movingGoma->Kind = player[movingGoma->Num % 40].Kind;
		movingGoma->owner = player[movingGoma->Num % 40].owner;
		if (movingGoma->chesser != ((*round) % 2 + 1)) {
			printf("此為對手的棋，重新輸入。\n");
			return 0;
		}
		else return 1;
	}
}

/*使用者可以輸入 段筋 / '0'(悔棋) / s/S(儲存棋譜) */
int JudgeToMove(int** board, int* round, struct goma* player, struct move* list, int x1, int y1, int x2, int y2, struct goma* movingGoma, struct goma* eatenGoma) {
	struct move Move;
	Change(movingGoma, *round);
	eatenGoma->x = x2;
	eatenGoma->y = y2;
	eatenGoma->Kind = player[board[x2 - 1][y2 - 1] - 1].Kind;
	eatenGoma->Num = player[board[x2 - 1][y2 - 1] - 1].Num;
	eatenGoma->chesser = player[board[x2 - 1][y2 - 1] - 1].chesser;
	eatenGoma->owner = player[board[x2 - 1][y2 - 1] - 1].owner;
	if (CheckMove(*round % 2, movingGoma->Kind, movingGoma->x, movingGoma->y, eatenGoma->x, eatenGoma->y, board)) {
		if (board[x2 - 1][y2 - 1] != EMPTY) {
			if (eatenGoma->chesser == ((*round % 2) + 1)) {
				printf("此為自己的棋，重新輸入目的地\n");
				return 0;
			}
			else { //食棋
				Move.x1 = movingGoma->x;
				Move.y1 = movingGoma->y;
				Move.x2 = eatenGoma->x;
				Move.y2 = eatenGoma->y;
				player[(movingGoma->Num - 1) % 40].x = eatenGoma->x;
				player[(movingGoma->Num - 1) % 40].y = eatenGoma->y;
				player[(eatenGoma->Num - 1) % 40].x = 0;
				player[(eatenGoma->Num - 1) % 40].y = 0;
				player[(eatenGoma->Num - 1) % 40].chesser = movingGoma->chesser;
				BoardUpdate(player, board);
				PrintBoard(board, player);
				*round += 1;
				return 1;
			}
		}
		else { //移動棋子
			Move.x1 = movingGoma->x;
			Move.y1 = movingGoma->y;
			Move.x2 = eatenGoma->x;
			Move.y2 = eatenGoma->y;
			player[(movingGoma->Num - 1) % 40].x = eatenGoma->x;
			player[(movingGoma->Num - 1) % 40].y = eatenGoma->y;
			BoardUpdate(player, board);
			PrintBoard(board, player);
			*round += 1;
			return 1;
		}
	}
	else {
		printf("不可移動之位置，重新輸入目的地\n");
		return 0;
	}
}
void Change(struct goma* chess, int turn) {
	if (turn % 2 == chess->chesser) {
		int inverse = chess->chesser ? -1 : 1;
		char c;
		for (int i = 2; i < 5; i++) {
			if (chess->y == 5 + i * inverse && chess->Kind >= 1 && chess->Kind <= 6) {
				printf("可以變棋，變ㄇ?(y/n):");
				scanf("%c", &c);
				if (c == 'y') {
					chess->Kind = chess->Kind + 8;
					chess->Num = chess->Num + 40;
				}
			}
		}
	}

}

void StoreBoard(struct move* stack, char filename[]) {
	struct move* tmp;
	tmp = (struct move*)malloc(sizeof(struct move));
	FILE* fptr;
	if ((fptr = fopen(filename, "w+")) != NULL) {
		tmp = stack;
		while (tmp != NULL) {
			fprintf(fptr, "%d %d %d %d\n", tmp->x1, tmp->y1, tmp->x2, tmp->y2);
			tmp = tmp->next;
		}
	}
	else {
		printf("無法開啟檔案!\n");
	}
	fclose(fptr);
	free(tmp);
}
int distance(int a, int b) {
	if (a >= b) return (a - b);
	else return (b - a);
}

void Pop(struct move** stack, struct move* x) {
	struct move* tmp;
	tmp = (struct move*)malloc(sizeof(struct move));
	tmp = *stack;
	if (tmp != NULL) {
		*stack = tmp->next;
		x->x1 = tmp->x1;
		x->x2 = tmp->x2;
		x->y1 = tmp->y1;
		x->y2 = tmp->y2;
		free(tmp);
	}
}
void Push(struct move** stack, struct move input) {
	struct move* tmp;
	tmp = (struct move*)malloc(sizeof(struct move));
	tmp->x1 = input.x1;
	tmp->x2 = input.x2;
	tmp->y1 = input.y1;
	tmp->y2 = input.y2;
	tmp->next = *stack;
	*stack = tmp;
	
}



struct move* OpenOldGame(char filename[]) {
	FILE* fptr;
	struct move* list=NULL;
	char X1, X2, Y1, Y2;
	struct move Move;
	if ((fptr = fopen(filename, "r+")) != NULL) {
		while (!feof(fptr)) {
			fscanf(fptr, "%c %c %c %c ", &X1, &Y1, &X2, &Y2);
			Move.x1 = X1 - '0';
			Move.x2 = X2 - '0';
			Move.y1 = Y1 - '0';
			Move.y2 = Y2 - '0';
			Push(&list, Move);
		}
		fclose(fptr);
	}
	else printf("開啟檔案失敗!");
	return list;
}

int** BoardUpdate(struct goma* player, int** board) {
	/*有棋子就放編號*/
	for (int i = 0; i < SUJI; i++) {
		for (int j = 0; j < DAN; j++) {
			board[i][j] = EMPTY;
		}
	}

	for (int i = 0; i < TOTALNUM * 2; i++) {
		if (player[i].x != 0 && player[i].y != 0) {

			board[player[i].x - 1][player[i].y - 1] = player[i].Num;
		}
	}
	return(board);
}
void PrintBoard(int** board, struct goma* chess) {
	/*
	(步兵)(銀將)(桂馬)(香車)(飛車)(角行)(王將)(金將)
	(成步)(成銀)(成桂)(成香)(龍王)(龍馬)
	*/
	printf("\n");
	for (int i = 0; i < SUJI; i++) {
		for (int j = 0; j < DAN; j++) {
			if (board[j][i] == 0) printf("    ");
			else if (board[j][i] >= 1 && board[j][i] <= 9) printf("\033[34m步兵\033[m");
			else if (board[j][i] >= 21 && board[j][i] <= 29) printf("\033[31m步兵\033[m");
			else if (board[j][i] >= 10 && board[j][i] <= 11) printf("\033[34m銀將\033[m");
			else if (board[j][i] >= 30 && board[j][i] <= 31) printf("\033[31m銀將\033[m");
			else if (board[j][i] >= 12 && board[j][i] <= 13) printf("\033[34m桂馬\033[m");
			else if (board[j][i] >= 32 && board[j][i] <= 33) printf("\033[31m桂馬\033[m");
			else if (board[j][i] >= 14 && board[j][i] <= 15) printf("\033[34m香車\033[m");
			else if (board[j][i] >= 34 && board[j][i] <= 35) printf("\033[31m香車\033[m");
			else if (board[j][i] == 16) printf("\033[34m飛車\033[m");
			else if (board[j][i] == 36)printf("\033[31m飛車\033[m");
			else if (board[j][i] == 17) printf("\033[34m角行\033[m");
			else if (board[j][i] == 37) printf("\033[31m角行\033[m");
			else if (board[j][i] == 18) printf("\033[34m王將\033[m");
			else if (board[j][i] == 38) printf("\033[31m王將\033[m");
			else if (board[j][i] >= 19 && board[j][i] <= 20) printf("\033[34m金將\033[m");
			else if (board[j][i] >= 39 && board[j][i] <= 40) printf("\033[31m金將\033[m");

			else if (board[j][i] >= 41 && board[j][i] <= 49) printf("\033[34m成步\033[m");
			else if (board[j][i] >= 61 && board[j][i] <= 69) printf("\033[31m成步\033[m");
			else if (board[j][i] >= 50 && board[j][i] <= 51) printf("\033[34m成銀\033[m");
			else if (board[j][i] >= 70 && board[j][i] <= 71) printf("\033[31m成銀\033[m");
			else if (board[j][i] >= 52 && board[j][i] <= 53) printf("\033[34m成桂\033[m");
			else if (board[j][i] >= 72 && board[j][i] <= 73) printf("\033[31m成桂\033[m");
			else if (board[j][i] >= 54 && board[j][i] <= 55) printf("\033[34m成香\033[m");
			else if (board[j][i] >= 74 && board[j][i] <= 75) printf("\033[31m成香\033[m");
			else if (board[j][i] == 56) printf("\033[34m龍王\033[m");
			else if (board[j][i] == 76)printf("\033[31m龍王\033[m");
			else if (board[j][i] == 57) printf("\033[34m龍馬\033[m");
			else if (board[j][i] == 77) printf("\033[31m龍馬\033[m");
			if (j != DAN - 1)printf("|");
		}
		if (i != SUJI - 1)printf("\n————————————————————————————————————————————\n");
		else printf("\n");
	}
}
int CheckMove(int turns, int kind, int pre_x, int pre_y, int next_x, int next_y, int** board) {
	int inverse = turns ? -1 : 1;
	if (pre_x == next_x && pre_y == next_y) return 0;
	else if (kind == FOOT) {
		if (next_x == pre_x && pre_y - next_y == 1 * inverse) return 1;
		else return 0;
	}
	else if (kind == SILVER) {
		if (next_x == pre_x && pre_y - next_y == 1 * inverse) return 1;
		else if (distance(next_x, pre_x) == 1 && distance(next_y, pre_y) == 1) return 1;
		else return 0;
	}
	else if (kind == CASSIA) {
		if (distance(next_x, pre_x) == 1 && pre_y - next_y == 2 * inverse) return 1;
		else return 0;
	}
	else if (kind == CHARIOT) {
		if (next_x == pre_x && ((next_y * inverse) < (pre_y * inverse))) {
			for (int i = 0; pre_y - i > next_y; i++) {
				if (board[pre_x - 1][(pre_y - 1) - i * inverse] != EMPTY) return 0;
			}
			return 1;
		}
		else return 0;
	}
	else if (kind == FLYING) {
		if (next_x == pre_x) {
			for (int i = 0; pre_y + i < next_y; i++) {
				if (board[pre_x - 1][(pre_y - 1) + i] != EMPTY) return 0;
			}
			for (int i = 0; pre_y - i > next_y; i++) {
				if (board[pre_x - 1][(pre_y - 1) - i] != EMPTY) return 0;
			}
			return 1;
		}
		else if (next_y == pre_y) {
			for (int i = 0; (pre_x + i < next_x); i++) {
				if (board[(pre_x - 1) + i][pre_y - 1] != EMPTY)return 0;
			}
			for (int i = 0; (pre_x - i > next_x); i++) {
				if (board[(pre_x - 1) - i][pre_y - 1] != EMPTY)return 0;
			}
			return 1;
		}
		else return 0;
	}
	else if (kind == ANGLE) {
		if (distance(next_x, pre_x) == distance(next_y, pre_y)) {
			if (next_x - pre_x == next_y - pre_y) {
				for (int i = 1; pre_x + i < next_x; i++) {
					if (board[(pre_x - 1) + i][(pre_y - 1) + i] != EMPTY)return 0;
				}
				for (int i = 1; pre_x - i > next_x; i++) {
					if (board[(pre_x - 1) - i][(pre_y - 1) - i] != EMPTY)return 0;
				}
			}
			else {
				for (int i = 1; pre_x + i < next_x; i++) {
					if (board[(pre_x - 1) + i][(pre_y - 1) - i] != EMPTY)return 0;
				}
				for (int i = 1; pre_x - i > next_x; i++) {
					if (board[(pre_x - 1) - i][(pre_y - 1) + i] != EMPTY)return 0;
				}
			}
			return 1;
		}
		else return 0;
	}
	else if (kind == KING) {
		if (distance(next_x, pre_x) <= 1 && distance(next_y, pre_y) <= 1) return 1;
		else return 0;
	}
	else if (kind == GOLD) {
		if (next_x == pre_x && distance(next_y, pre_y) == 1) return 1;
		else if (next_y == pre_y && distance(next_x, pre_x) == 1)return 1;
		else if (distance(next_x, pre_x) == 1 && distance(next_y, pre_y) == 1 && (pre_y * inverse) > (next_y * inverse)) return 1;
		else return 0;
	}
	else if (kind == uFOOT || kind == uSILVER || kind == uCASSIA || kind == uCHARIOT) {
		if (next_x == pre_x && distance(next_y, pre_y) == 1) return 1;
		else if (next_y == pre_y && distance(next_x, pre_x) == 1)return 1;
		else if (distance(next_x, pre_x) == 1 && pre_y - next_y == 1 * inverse) return 1;
		else return 0;
	}
	else if (kind == uFLYING) {
		if (next_x == pre_x) {
			for (int i = 1; pre_y + i < next_y; i++) {
				if (board[pre_x - 1][(pre_y - 1) + i] != EMPTY) return 0;
			}
			for (int i = 1; pre_y - i > next_y; i++) {
				if (board[pre_x - 1][(pre_y - 1) - i] != EMPTY) return 0;
			}
			return 1;
		}
		else if (next_y == pre_y) {
			for (int i = 1; (pre_x + i < next_y); i++) {
				if (board[(pre_x - 1) + i][pre_y - 1] != EMPTY) return 0;
			}
			for (int i = 1; (pre_x - i > next_y); i++) {
				if (board[(pre_x - 1) - i][pre_y - 1] != EMPTY) return 0;
			}
			return 1;
		}
		else if (distance(next_x, pre_x) == 1 && distance(next_y, pre_y) == 1) return 1;
		else return 0;
	}
	else if (kind == uANGLE) {
		if (distance(next_x, pre_x) == distance(next_y, pre_y)) {
			if (next_x - pre_x == next_y - pre_y) {
				for (int i = 1; pre_x + i < next_x; i++) {
					if (board[(pre_x - 1) + i][(pre_y - 1) + i] != EMPTY)return 0;
				}
				for (int i = 1; pre_x - i > next_x; i++) {
					if (board[(pre_x - 1) - i][(pre_y - 1) - i] != EMPTY)return 0;
				}
			}
			else {
				for (int i = 1; pre_x + i < next_x; i++) {
					if (board[(pre_x - 1) + i][(pre_y - 1) - i] != EMPTY)return 0;
				}
				for (int i = 1; pre_x - i > next_x; i++) {
					if (board[(pre_x - 1) - i][(pre_y - 1) + i] != EMPTY)return 0;
				}
			}
			return 1;
		}
		else if (next_x == pre_x && distance(next_y, pre_y) == 1 * inverse) return 1;
		else if (next_y == pre_y && distance(next_x, pre_x) == 1) return 1;
		else return 0;
	}
	else return 0;
}

