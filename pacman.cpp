#include "pacman.h"

// Ԫ�صĶ���
Player player;  // ���
Player enemy[ENEMY_NUM];  // ����
Bean bean[BEAN_NUM];  // ����

// ��������
IMAGE mapImg, overImg, vectorImg;  //�����ͼ��ͼƬ
unsigned long tt1, tt2;  //���ʱ�䣬�任���˷���

// 2����ʼ������
void GameInit() {
	//��ʼ����Ϸ���ڿ��
	initgraph(WIDTH, HEIGHT);

	//��ʼ������ͼƬ��gameover, vector
	loadimage(&mapImg, "./image/map.png");
	loadimage(&overImg, "./image/gameover.png", 335, 233);
	loadimage(&vectorImg, "./image/vector.png", 349, 213);


	// ���غͲ�������
	//mciSendString("open pacman.mp3", 0, 0, 0);
	//mciSendString("play pacman.mp3", 0, 0, 0);

	// ��ʼ�����
	loadimage(&player.img, "./image/player.png", SIZE_P, SIZE_P);  
	putimage(0, 0, &mapImg);
	player.x = 20;
	player.y = 20;
	player.dirx = 0;
	player.diry = 0;
	player.t1 = GetTickCount();  // ��ȡϵͳʱ��
	player.dt = 5;  // ms


	// ��ʼ������
	for (int i = 0; i < ENEMY_NUM; i++) {
		//loadimage(&enemy[i].img, "enemy.png", SIZE_E, SIZE_E);

		// �������
		int n = rand() % 4; // 0-3�����
		if (n == 0) enemy[i].dirx = -1, enemy[i].diry = 0;
		else if (n == 1) enemy[i].dirx = 0, enemy[i].diry = 1;
		else if (n == 2) enemy[i].dirx = 1, enemy[i].diry = 0;
		else if (n == 3)enemy[i].dirx = 0, enemy[i].diry = -1;

		// ����
		while (1) {
			enemy[i].x = rand() % (WIDTH - SIZE_E);
			enemy[i].y = rand() % (HEIGHT - SIZE_E);
			// ��ֹ���ϰ�������
			if (getpixel(enemy[i].x + SIZE_E / 2, enemy[i].y + SIZE_E / 2) != BLACK) {
				continue;
			}
			else {
				break;
			}
		}

		enemy[i].t1 = GetTickCount();  // ��ȡϵͳʱ��
		enemy[i].dt = 5;  // ms
	}
	tt1 = GetTickCount();

	// ��ʼ������
	for (int j = 0; j < BEAN_NUM; j++) {
		// �������
		while (1) {
			bean[j].x = rand() % (WIDTH - SIZE_B);
			bean[j].y = rand() % (HEIGHT - SIZE_B);
			// ��ֹ���ϰ�������
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



// 3����ʾ����
void GameDraw() {
	// ��ʼ����
	BeginBatchDraw();

	// ���Ʊ���ͼƬ
	putimage(0, 0, &mapImg);

	// �������
	putimage(player.x, player.y, &player.img);

	// ���Ƶ���
	for (int i = 0; i < ENEMY_NUM; i++) {
		// �Զ����
		setfillcolor(RED);
		fillroundrect(enemy[i].x, enemy[i].y, enemy[i].x + SIZE_E, enemy[i].y + SIZE_E, 0, 0);

		//putimage(enemy[i].x, enemy[i].y, &enemy[i].img);
	}

	// ���ƶ���
	for (int j = 0; j < BEAN_NUM; j++) {
		// �ж϶����Ƿ񱻳Ե���
		if (bean[j].isexist) {
			setfillcolor(YELLOW);
			fillroundrect(bean[j].x, bean[j].y, bean[j].x + SIZE_B, bean[j].y + SIZE_B, 5, 5);
		}
	}

	// ��������
	EndBatchDraw();
}



// 4�����ݱ仯
void GameUpdate() {
	int cen_x, cen_y;
	// ��ҵ����ݱ仯
	// 1���ֶ��仯
	// ���̸ı䷽��
	char key;
	if (_kbhit()) {
		key = _getch(); // ���հ���
		switch (key) {
		case 72:  // ��
			player.dirx = 0;
			player.diry = -1;
			break;
		case 80:  // ��
			player.dirx = 0;
			player.diry = 1;
			break;
		case 75:  // ��
			player.dirx = -1;
			player.diry = 0;
			break;
		case 77:  // ��
			player.dirx = 1;
			player.diry = 0;
			break;
		}
	}

	// 2���Զ��仯
	// �ƶ�
	player.t2 = GetTickCount();
	if (player.t2 - player.t1 >= player.dt) {
		// ÿ 5ms ʹ���ƶ�һ��
		player.t1 = player.t2;  // ����t1
		player.x += 2 * player.dirx;
		player.y += 2 * player.diry;
	}

	// �������ǽ�� �����෴
	cen_x = player.x + SIZE_P / 2;  // �������ĵ�����
	cen_y = player.y + SIZE_P / 2;

	// ����
	if (player.dirx == 1 || player.dirx == -1) {
		for (int yy = cen_y - SIZE_P / 2; yy <= cen_y + SIZE_P / 2; yy++) {
			if (getpixel(cen_x + (SIZE_P / 2) * player.dirx, yy) != BLACK) {
				player.x -= 2 * player.dirx;  //ײǽ�Ժ�����������
				player.dirx = 0;
				break;
			}
		}
	}
	// ����
	else if (player.diry == 1 || player.diry == -1) {
		for (int xx = cen_x - SIZE_P / 2; xx <= cen_x + SIZE_P / 2; xx++) {
			if (getpixel(xx, cen_y + (SIZE_P / 2) * player.diry) != BLACK) {
				player.y -= 2 * player.diry;
				player.diry = 0;
				break;
			}
		}
	}

	// ���˵����ݱ仯
	// �ƶ�
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].t2 = GetTickCount();
		if (enemy[i].t2 - enemy[i].t1 >= enemy[i].dt) {
			// ÿ 5ms ʹ���ƶ�һ��
			enemy[i].t1 = enemy[i].t2;  // ����t1
			enemy[i].x += 2 * enemy[i].dirx;
			enemy[i].y += 2 * enemy[i].diry;
		}

		// ��������ǽ�� �����෴
		cen_x = enemy[i].x + SIZE_E / 2;  // �������ĵ�����
		cen_y = enemy[i].y + SIZE_E / 2;

		// ����
		if (enemy[i].dirx == 1 || enemy[i].dirx == -1) {
			for (int yy = cen_y - SIZE_E / 2; yy <= cen_y + SIZE_E / 2; yy++) {
				if (getpixel(cen_x + ((SIZE_E / 2) + 2) * enemy[i].dirx, yy) != BLACK) {
					enemy[i].dirx = -enemy[i].dirx;
					break;
				}
			}
		}
		// ����
		else if (enemy[i].diry == 1 || enemy[i].diry == -1) {
			for (int xx = cen_x - SIZE_E / 2; xx <= cen_x + SIZE_E / 2; xx++) {
				if (getpixel(xx, cen_y + ((SIZE_E / 2) + 2) * enemy[i].diry) != BLACK) {
					enemy[i].diry = -enemy[i].diry;
					break;
				}
			}
		}
	}

	// ÿ��һ��ʱ�䣬�������õ��˵ķ���
	tt2 = GetTickCount();
	if (tt2 - tt1 > 2000) {
		for (int i = 0; i < ENEMY_NUM; i++) {
			// �������
			int n = rand() % 4; // 0-3�����
			if (n == 0) enemy[i].dirx = -1, enemy[i].diry = 0;
			else if (n == 1) enemy[i].dirx = 0, enemy[i].diry = 1;
			else if (n == 2) enemy[i].dirx = 1, enemy[i].diry = 0;
			else if (n == 3)enemy[i].dirx = 0, enemy[i].diry = -1;
		}
		tt1 = tt2;
	}

	// �����������,����
	cen_x = player.x + SIZE_P / 2;  // �������ĵ�����
	cen_y = player.y + SIZE_P / 2;

	for (int i = 0; i < ENEMY_NUM; i++) {
		int cen_ex = enemy[i].x + SIZE_E / 2;
		int cen_ey = enemy[i].y + SIZE_E / 2;

		// ����
		if (cen_x < cen_ex + (SIZE_E / 2) + 2 && cen_x >= cen_ex - (SIZE_E / 2) - 2) {
			for (int yy = cen_y - SIZE_P / 2; yy <= cen_y + SIZE_P / 2; yy++) {
				if (yy == cen_ey - (SIZE_E / 2) - 2 || yy == cen_ey - (SIZE_E / 2) + 2) {
					while (1) {
						putimage((HEIGHT - 400) / 2, (WIDTH - 200) / 2, &overImg);
						key = _getch(); // ���հ���
						if (key == 32) {
							GameInit();  // ���¿ո�����¿�ʼ
							break;
						}
					}
					break;
				}
			}
		}
		// ����
		if (cen_y < cen_ey + (SIZE_E / 2) + 2 && cen_y >= cen_ey - (SIZE_E / 2) - 2) {
			for (int xx = cen_x - SIZE_P / 2; xx <= cen_x + SIZE_P / 2; xx++) {
				if (xx == cen_ex - (SIZE_E / 2) - 2 || xx == cen_ex + (SIZE_E / 2) + 2) {
					while (1) {
						putimage((HEIGHT - 400) / 2, (WIDTH - 200) / 2, &overImg);
						key = _getch(); // ���հ���
						if (key == 32) {
							GameInit();  // ���¿ո�����¿�ʼ
							break;
						}
					}
					break;
				}
			}
		}
	}

	// ����������ӣ�����
	cen_x = player.x + SIZE_P / 2;  // �������ĵ�����
	cen_y = player.y + SIZE_P / 2;

	for (int i = 0; i < BEAN_NUM; i++) {
		int cen_bx = bean[i].x + SIZE_B / 2;
		int cen_by = bean[i].y + SIZE_B / 2;

		// ����
		if (cen_x < cen_bx + (SIZE_B / 2) + 2 && cen_x >= cen_bx - (SIZE_B / 2) - 2) {
			for (int yy = cen_y - SIZE_P / 2; yy <= cen_y + SIZE_P / 2; yy++) {
				if (bean[i].isexist && yy == cen_by - (SIZE_B / 2) - 2 || yy == cen_by - (SIZE_B / 2) + 2) {
					bean[i].isexist = false;  // ���Ķ��Ӵ���״̬
					break;
				}
			}
		}
		// ����
		if (cen_y < cen_by + (SIZE_B / 2) + 2 && cen_y >= cen_by - (SIZE_B / 2) - 2) {
			for (int xx = cen_x - SIZE_P / 2; xx <= cen_x + SIZE_P / 2; xx++) {
				if (bean[i].isexist && xx == cen_bx - (SIZE_B / 2) - 2 || xx == cen_bx + (SIZE_B / 2) + 2) {
					bean[i].isexist = false;  // ����״̬����Ϊfalse
					break;
				}
			}
		}
	}

	// �ж��Ƿ�ﵽ�յ�
	int reward_num = 0;
	for (int i = 0; i < BEAN_NUM; i++) {
		if (bean[i].isexist == true) {
			reward_num++;
		}
	}

	if (getpixel(player.x + SIZE_P / 2, player.y + SIZE_P + 3) == RGB(0,255,0)) {
		// �����ж��Ƿ�Ե������Ŷ��� && �������յ�
		while (1) {
			putimage((HEIGHT - 400) / 2, (WIDTH - 200) / 2, &vectorImg); // �����ж�������
			key = _getch(); // ���հ���
			if (key == 32) {
				GameInit();  // ���¿ո�����¿�ʼ
				break;
			}
		}
	}
}