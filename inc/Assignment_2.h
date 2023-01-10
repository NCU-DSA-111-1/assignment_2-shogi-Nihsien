

#define CHESSKIND 14
#define TOTALNUM 20
#define DAN 9	/*段*/
#define SUJI 9	/*筋*/


/*依照順序:
(空格)
(步兵)(銀將)(桂馬)(香車)(飛車)(角行)(王將)(金將)
(成步)(成銀)(成桂)(成香)(龍王)(龍馬)*/
const int EMPTY = 0;
const int FOOT = 1;
const int SILVER = 2;
const int CASSIA = 3;
const int CHARIOT = 4;
const int FLYING = 5;
const int ANGLE = 6;
const int KING = 7;
const int GOLD = 8;
const int uFOOT = 9;
const int uSILVER = 10;
const int uCASSIA = 11;
const int uCHARIOT = 12;
const int uFLYING = 13;
const int uANGLE = 14;

struct goma* shogi(struct goma* Player);
int** BoardUpdate(struct goma* player, int** board);
int distance(int a, int b);
void PrintBoard(int** board, struct goma* chess);
void Change(struct goma* chess, int turn);
struct move* OpenOldGame(char filename[]);
void StoreBoard(struct move* stack, char filename[]);
int JudgeToMove(int** board, int* round, struct goma* player, struct move* list, int x1, int y1, int x2, int y2, struct goma* movingGoma, struct goma* eatenGoma);
int JudgeBeforeMove(int** board, int* round, struct goma* player, struct move* list, int x, int y, struct goma* movingGoma);
int CheckMove(int turns, int kind, int pre_x, int pre_y, int next_x, int next_y, int** board);
void Pop(struct move** stack, struct move* x);
void Push(struct move** stack, struct move input);
