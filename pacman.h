#pragma once
#include <stdio.h>
#include <easyx.h>
#include <mmsystem.h>
#include <conio.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")  // ��ý���豸�ӿ�

// 1����������
#define WIDTH 515
#define HEIGHT 577
#define SIZE_P 16
#define SIZE_E 10
#define SIZE_B 8
#define ENEMY_NUM 3
#define BEAN_NUM 3

// ����������
struct Player {
	int x, y;	// ��ҵ�����
	IMAGE img;  // ��ҵ�ͼƬ
	int dirx, diry;  // ����ͨ������������ʾ����
	unsigned long t1, t2, dt; // dt = t2 - t1; �ٶ�
};

// �����������
struct Bean {
	int x, y;
	bool isexist;
};

// ��������
void GameInit();  // ��Ϸ��ʼ��
void GameDraw();  // ��Ϸ��ʾ
void GameUpdate();  // ��Ϸ����
