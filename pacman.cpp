#include "pacman.h"

// 元素的定义
Player player;  // 玩家
Player enemy[ENEMY_NUM];  // 敌人
Bean bean[BEAN_NUM];  // 豆子

// 其他定义
IMAGE mapImg, overImg, vectorImg;  //定义地图的图片
unsigned long tt1, tt2;  //间隔时间，变换敌人方向

// 2、初始化数据
void GameInit() {
	//初始化游戏窗口宽高
	initgraph(WIDTH, HEIGHT);

	//初始化背景图片，gameover, vector
	loadimage(&mapImg, "./image/map.png");
	loadimage(&overImg, "./image/gameover.png", 335, 233);
	loadimage(&vectorImg, "./image/vector.png", 349, 213);


	// 加载和播放音乐
	//mciSendString("open pacman.mp3", 0, 0, 0);
	//mciSendString("play pacman.mp3", 0, 0, 0);

	// 初始化玩家
	loadimage(&player.img, "./image/player.png", SIZE_P, SIZE_P);  
	putimage(0, 0, &mapImg);
	player.x = 20;
	player.y = 20;
	player.dirx = 0;
	player.diry = 0;
	player.t1 = GetTickCount();  // 获取系统时间
	player.dt = 5;  // ms


	// 初始化敌人
	for (int i = 0; i < ENEMY_NUM; i++) {
		//loadimage(&enemy[i].img, "enemy.png", SIZE_E, SIZE_E);

		// 随机方向
		int n = rand() % 4; // 0-3随机数
		if (n == 0) enemy[i].dirx = -1, enemy[i].diry = 0;
		else if (n == 1) enemy[i].dirx = 0, enemy[i].diry = 1;
		else if (n == 2) enemy[i].dirx = 1, enemy[i].diry = 0;
		else if (n == 3)enemy[i].dirx = 0, enemy[i].diry = -1;

		// 坐标
		while (1) {
			enemy[i].x = rand() % (WIDTH - SIZE_E);
			enemy[i].y = rand() % (HEIGHT - SIZE_E);
			// 防止在障碍处生成
			if (getpixel(enemy[i].x + SIZE_E / 2, enemy[i].y + SIZE_E / 2) != BLACK) {
				continue;
			}
			else {
				break;
			}
		}

		enemy[i].t1 = GetTickCount();  // 获取系统时间
		enemy[i].dt = 5;  // ms
	}
	tt1 = GetTickCount();

	// 初始化豆子
	for (int j = 0; j < BEAN_NUM; j++) {
		// 随机坐标
		while (1) {
			bean[j].x = rand() % (WIDTH - SIZE_B);
			bean[j].y = rand() % (HEIGHT - SIZE_B);
			// 防止在障碍处生成
			if (getpixel(bean[j].x + (SIZE_B / 2), bean[j].y + (SIZE_B / 2)) != BLACK) {
				continue;
			}
			else {
				bean[j].isexist = true;
				break;
			}
		}
	}
}



// 3、显示程序
void GameDraw() {
	// 开始绘制
	BeginBatchDraw();

	// 绘制背景图片
	putimage(0, 0, &mapImg);

	// 绘制玩家
	putimage(player.x, player.y, &player.img);

	// 绘制敌人
	for (int i = 0; i < ENEMY_NUM; i++) {
		// 自定义点
		setfillcolor(RED);
		fillroundrect(enemy[i].x, enemy[i].y, enemy[i].x + SIZE_E, enemy[i].y + SIZE_E, 0, 0);

		//putimage(enemy[i].x, enemy[i].y, &enemy[i].img);
	}

	// 绘制豆子
	for (int j = 0; j < BEAN_NUM; j++) {
		// 判断豆子是否被吃掉了
		if (bean[j].isexist) {
			setfillcolor(YELLOW);
			fillroundrect(bean[j].x, bean[j].y, bean[j].x + SIZE_B, bean[j].y + SIZE_B, 5, 5);
		}
	}

	// 结束绘制
	EndBatchDraw();
}



// 4、数据变化
void GameUpdate() {
	int cen_x, cen_y;
	// 玩家的数据变化
	// 1、手动变化
	// 键盘改变方向
	char key;
	if (_kbhit()) {
		key = _getch(); // 接收按键
		switch (key) {
		case 72:  // 上
			player.dirx = 0;
			player.diry = -1;
			break;
		case 80:  // 下
			player.dirx = 0;
			player.diry = 1;
			break;
		case 75:  // 左
			player.dirx = -1;
			player.diry = 0;
			break;
		case 77:  // 右
			player.dirx = 1;
			player.diry = 0;
			break;
		}
	}

	// 2、自动变化
	// 移动
	player.t2 = GetTickCount();
	if (player.t2 - player.t1 >= player.dt) {
		// 每 5ms 使他移动一次
		player.t1 = player.t2;  // 更新t1
		player.x += 2 * player.dirx;
		player.y += 2 * player.diry;
	}

	// 玩家碰到墙壁 方向相反
	cen_x = player.x + SIZE_P / 2;  // 定义中心点坐标
	cen_y = player.y + SIZE_P / 2;

	// 左右
	if (player.dirx == 1 || player.dirx == -1) {
		for (int yy = cen_y - SIZE_P / 2; yy <= cen_y + SIZE_P / 2; yy++) {
			if (getpixel(cen_x + (SIZE_P / 2) * player.dirx, yy) != BLACK) {
				player.x -= 2 * player.dirx;  //撞墙以后往后退两格
				player.dirx = 0;
				break;
			}
		}
	}
	// 上下
	else if (player.diry == 1 || player.diry == -1) {
		for (int xx = cen_x - SIZE_P / 2; xx <= cen_x + SIZE_P / 2; xx++) {
			if (getpixel(xx, cen_y + (SIZE_P / 2) * player.diry) != BLACK) {
				player.y -= 2 * player.diry;
				player.diry = 0;
				break;
			}
		}
	}

	// 敌人的数据变化
	// 移动
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].t2 = GetTickCount();
		if (enemy[i].t2 - enemy[i].t1 >= enemy[i].dt) {
			// 每 5ms 使他移动一次
			enemy[i].t1 = enemy[i].t2;  // 更新t1
			enemy[i].x += 2 * enemy[i].dirx;
			enemy[i].y += 2 * enemy[i].diry;
		}

		// 敌人碰到墙壁 方向相反
		cen_x = enemy[i].x + SIZE_E / 2;  // 定义中心点坐标
		cen_y = enemy[i].y + SIZE_E / 2;

		// 左右
		if (enemy[i].dirx == 1 || enemy[i].dirx == -1) {
			for (int yy = cen_y - SIZE_E / 2; yy <= cen_y + SIZE_E / 2; yy++) {
				if (getpixel(cen_x + ((SIZE_E / 2) + 2) * enemy[i].dirx, yy) != BLACK) {
					enemy[i].dirx = -enemy[i].dirx;
					break;
				}
			}
		}
		// 上下
		else if (enemy[i].diry == 1 || enemy[i].diry == -1) {
			for (int xx = cen_x - SIZE_E / 2; xx <= cen_x + SIZE_E / 2; xx++) {
				if (getpixel(xx, cen_y + ((SIZE_E / 2) + 2) * enemy[i].diry) != BLACK) {
					enemy[i].diry = -enemy[i].diry;
					break;
				}
			}
		}
	}

	// 每隔一段时间，重新设置敌人的方向
	tt2 = GetTickCount();
	if (tt2 - tt1 > 2000) {
		for (int i = 0; i < ENEMY_NUM; i++) {
			// 随机方向
			int n = rand() % 4; // 0-3随机数
			if (n == 0) enemy[i].dirx = -1, enemy[i].diry = 0;
			else if (n == 1) enemy[i].dirx = 0, enemy[i].diry = 1;
			else if (n == 2) enemy[i].dirx = 1, enemy[i].diry = 0;
			else if (n == 3)enemy[i].dirx = 0, enemy[i].diry = -1;
		}
		tt1 = tt2;
	}

	// 玩家遇到敌人,交互
	cen_x = player.x + SIZE_P / 2;  // 定义中心点坐标
	cen_y = player.y + SIZE_P / 2;

	for (int i = 0; i < ENEMY_NUM; i++) {
		int cen_ex = enemy[i].x + SIZE_E / 2;
		int cen_ey = enemy[i].y + SIZE_E / 2;

		// 上下
		if (cen_x < cen_ex + (SIZE_E / 2) + 2 && cen_x >= cen_ex - (SIZE_E / 2) - 2) {
			for (int yy = cen_y - SIZE_P / 2; yy <= cen_y + SIZE_P / 2; yy++) {
				if (yy == cen_ey - (SIZE_E / 2) - 2 || yy == cen_ey - (SIZE_E / 2) + 2) {
					while (1) {
						putimage((HEIGHT - 400) / 2, (WIDTH - 200) / 2, &overImg);
						key = _getch(); // 接收按键
						if (key == 32) {
							GameInit();  // 按下空格键重新开始
							break;
						}
					}
					break;
				}
			}
		}
		// 左右
		if (cen_y < cen_ey + (SIZE_E / 2) + 2 && cen_y >= cen_ey - (SIZE_E / 2) - 2) {
			for (int xx = cen_x - SIZE_P / 2; xx <= cen_x + SIZE_P / 2; xx++) {
				if (xx == cen_ex - (SIZE_E / 2) - 2 || xx == cen_ex + (SIZE_E / 2) + 2) {
					while (1) {
						putimage((HEIGHT - 400) / 2, (WIDTH - 200) / 2, &overImg);
						key = _getch(); // 接收按键
						if (key == 32) {
							GameInit();  // 按下空格键重新开始
							break;
						}
					}
					break;
				}
			}
		}
	}

	// 玩家遇到豆子，交互
	cen_x = player.x + SIZE_P / 2;  // 定义中心点坐标
	cen_y = player.y + SIZE_P / 2;

	for (int i = 0; i < BEAN_NUM; i++) {
		int cen_bx = bean[i].x + SIZE_B / 2;
		int cen_by = bean[i].y + SIZE_B / 2;

		// 上下
		if (cen_x < cen_bx + (SIZE_B / 2) + 2 && cen_x >= cen_bx - (SIZE_B / 2) - 2) {
			for (int yy = cen_y - SIZE_P / 2; yy <= cen_y + SIZE_P / 2; yy++) {
				if (bean[i].isexist && yy == cen_by - (SIZE_B / 2) - 2 || yy == cen_by - (SIZE_B / 2) + 2) {
					bean[i].isexist = false;  // 更改豆子存在状态
					break;
				}
			}
		}
		// 左右
		if (cen_y < cen_by + (SIZE_B / 2) + 2 && cen_y >= cen_by - (SIZE_B / 2) - 2) {
			for (int xx = cen_x - SIZE_P / 2; xx <= cen_x + SIZE_P / 2; xx++) {
				if (bean[i].isexist && xx == cen_bx - (SIZE_B / 2) - 2 || xx == cen_bx + (SIZE_B / 2) + 2) {
					bean[i].isexist = false;  // 存在状态设置为false
					break;
				}
			}
		}
	}

	// 判断是否达到终点
	int reward_num = 0;
	for (int i = 0; i < BEAN_NUM; i++) {
		if (bean[i].isexist == true) {
			reward_num++;
		}
	}

	if (getpixel(player.x + SIZE_P / 2, player.y + SIZE_P + 3) == RGB(0,255,0)) {
		// 首先判断是否吃掉了三颗豆子 && 到达了终点
		while (1) {
			putimage((HEIGHT - 400) / 2, (WIDTH - 200) / 2, &vectorImg); // 坐标判断有问题
			key = _getch(); // 接收按键
			if (key == 32) {
				GameInit();  // 按下空格键重新开始
				break;
			}
		}
	}
}