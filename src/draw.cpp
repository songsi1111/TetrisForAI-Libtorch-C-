#include <graphics.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

using std::cout;		using std::endl;
using std::vector;      using std::pair;
using std::rand;		using std::to_string;

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 500
#define BLOCK_WIDTH_NUM 10
#define BLOCK_HEIGHT_NUM 30
#define BLOCK_SIZE 20
#define PIC_NUMS 8
#define COLOR_NUMS 8
#define WORD_X 15
#define WORD_Y 30
#define SHOW_CMD false

void load_pics(vector<IMAGE>& images) {
	loadimage(&images[0], "./pics/blue.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
	loadimage(&images[1], "./pics/lightblue.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
	loadimage(&images[2], "./pics/green.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
	loadimage(&images[3], "./pics/purple.png", BLOCK_SIZE - 1 , BLOCK_SIZE - 1);
	loadimage(&images[4], "./pics/orange.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
	loadimage(&images[5], "./pics/red.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
	loadimage(&images[6], "./pics/yellow.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
	loadimage(&images[7], "./pics/black.png", BLOCK_SIZE - 1, BLOCK_SIZE - 1);
}

void draw(const vector<vector<int>>& all_blocks, const vector<pair<int, int>>& moving_blocks, int moving_color, 
			const vector<IMAGE>& pics, int scores, int pieces, int lines) {
	line(BLOCK_SIZE * BLOCK_WIDTH_NUM, 0, BLOCK_SIZE * BLOCK_WIDTH_NUM, WINDOW_HEIGHT);
	solidrectangle(BLOCK_SIZE * BLOCK_WIDTH_NUM, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	for (int i = 0; i < BLOCK_HEIGHT_NUM; i++) {
		for (int j = 0; j < BLOCK_WIDTH_NUM; j++) {
			putimage(j * BLOCK_SIZE, i * BLOCK_SIZE, &pics[all_blocks[i][j]]);
		}
	}
	for (int i = 0; i < 4; i++) {
		putimage(moving_blocks[i].first * BLOCK_SIZE, moving_blocks[i].second * BLOCK_SIZE, &pics[moving_color]);
	}
	outtextxy(BLOCK_SIZE * BLOCK_WIDTH_NUM + 15, 10, "Score:");
	outtextxy(BLOCK_SIZE * BLOCK_WIDTH_NUM + 15, 100, "Pieces:");
	outtextxy(BLOCK_SIZE * BLOCK_WIDTH_NUM + 15, 200, "Lines:");
	outtextxy(BLOCK_SIZE * BLOCK_WIDTH_NUM + 15, 40, to_string(scores).c_str());
	outtextxy(BLOCK_SIZE * BLOCK_WIDTH_NUM + 15, 130, to_string(pieces).c_str());
	outtextxy(BLOCK_SIZE * BLOCK_WIDTH_NUM + 15, 230, to_string(lines).c_str());
	FlushBatchDraw();
}

int main() {
	// 加载图片
	vector<IMAGE> pics(PIC_NUMS);
	load_pics(pics);

    // 初始化图形界面
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOW_CMD);
	setfillcolor(RGB(254, 204, 203));
	setbkmode(TRANSPARENT);
	settextcolor(RGB(160, 62, 137));
	settextstyle(WORD_Y, WORD_X, "Times New Roman");
	BeginBatchDraw();

	// 随机生成方块
	srand(time(NULL));
	vector<vector<int>> all_blocks(BLOCK_HEIGHT_NUM, vector<int>(BLOCK_WIDTH_NUM));
	vector<pair<int, int>> moving_blocks(4);
	int moving_color = 0;
	int score, pieces, lines;
    while (1) {
		for (int i = 0; i < BLOCK_HEIGHT_NUM; i++) {
			for (int j = 0; j < BLOCK_WIDTH_NUM; j++) {
				all_blocks[i][j] = rand() % COLOR_NUMS;
				if (all_blocks[i][j] != 7) {
					if (rand() % 2) {
						all_blocks[i][j] = 7;
					}
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			moving_blocks[i].first = rand() % BLOCK_WIDTH_NUM;
			moving_blocks[i].second = rand() % BLOCK_HEIGHT_NUM;
		}
		moving_color = rand() % (COLOR_NUMS - 1);
		score = rand() % 10000;
		pieces = rand() % 500;
		lines = rand() % 100;

		draw(all_blocks, moving_blocks, moving_color, pics, score, pieces, lines);
		Sleep(1000);
    }

    // 关闭图形界面
	EndBatchDraw();
    closegraph();
    return 0;
}
