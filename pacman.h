#pragma once
#include <stdio.h>
#include <easyx.h>
#include <mmsystem.h>
#include <conio.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")  // 多媒体设备接口

// 1、定义数据
#define WIDTH 515
#define HEIGHT 577
#define SIZE_P 16
#define SIZE_E 10
#define SIZE_B 8
#define ENEMY_NUM 3
#define BEAN_NUM 3

// 玩家类的声明
struct Player {
	int x, y;	// 玩家的坐标
	IMAGE img;  // 玩家的图片
	int dirx, diry;  // 方向（通过两个变量表示方向）
	unsigned long t1, t2, dt; // dt = t2 - t1; 速度
};

// 豆子类的声明
struct Bean {
	int x, y;
	bool isexist;
};

// 函数声明
void GameInit();  // 游戏初始化
void GameDraw();  // 游戏显示
void GameUpdate();  // 游戏更新
