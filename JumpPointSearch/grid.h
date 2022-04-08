#pragma once


#define WIN_WIDTH 1280
#define WIN_HEIGHT 960

#define GRID_WIDTH 60
#define GRID_HEIGHT 40

#define GRID_SIZE 15

enum Type
{
	Empty,
	Wall
};



struct Grid
{
	Type type;
	BYTE color;

};

void InitGrid(HWND hwnd);
void Render();

extern Grid tile[GRID_HEIGHT][GRID_WIDTH];