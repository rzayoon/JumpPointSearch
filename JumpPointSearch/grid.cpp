#include <Windows.h>
#include "grid.h"
#include "jps.h"

Grid tile[GRID_HEIGHT][GRID_WIDTH];

HWND hWnd;


extern bool closed[GRID_HEIGHT][GRID_WIDTH];
extern int open[GRID_HEIGHT][GRID_WIDTH];

void InitGrid(HWND hwnd)
{
	hWnd = hwnd;

	for (int y = 0; y < GRID_HEIGHT; y++)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			tile[y][x].type = Empty;
			closed[y][x] = false;
			open[y][x] = MAXINT32;
		}
	}



}

void Render()
{
	static HDC hdc, memDC;

	static HBITMAP backBit, oldBackBit;
	static RECT bufferRect;
	hdc = GetDC(hWnd);

	
	memDC = CreateCompatibleDC(hdc);
	backBit = CreateCompatibleBitmap(hdc, bufferRect.right, bufferRect.bottom);
	oldBackBit = (HBITMAP)SelectObject(memDC, backBit);
	PatBlt(memDC, 0, 0, bufferRect.right, bufferRect.bottom, WHITENESS);

	HDC tmpDC = hdc;
	hdc = memDC;
	memDC = tmpDC;


	HBRUSH wallBrush = (HBRUSH)CreateSolidBrush(RGB(150, 150, 150));
	HBRUSH srcBrush = (HBRUSH)CreateSolidBrush(RGB(0, 220, 220));
	HBRUSH destBrush = (HBRUSH)CreateSolidBrush(RGB(150, 0, 0));
	HBRUSH openBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 200));
	HBRUSH closeBrush = (HBRUSH)CreateSolidBrush(RGB(200, 200, 0));


	HPEN linePen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	HPEN pathPen = CreatePen(PS_SOLID, 2, RGB(150, 0, 0));
	HPEN originPen = (HPEN)SelectObject(hdc, linePen);

	int y, x;
	for (y = 0; y < GRID_HEIGHT; y++)
	{
		MoveToEx(hdc, 0, (y + 1) * GRID_SIZE, NULL);
		LineTo(hdc, GRID_WIDTH * GRID_SIZE, (y + 1) * GRID_SIZE);
	}
	
	for (x = 0; x < GRID_WIDTH; x++)
	{
		MoveToEx(hdc, (x + 1) * GRID_SIZE, 0, NULL);
		LineTo(hdc, (x + 1) * GRID_SIZE, GRID_HEIGHT * GRID_SIZE);
		
	}
	DeleteObject(linePen);

	

	for (y = 0; y < GRID_HEIGHT; y++)
	{
		for (x = 0; x < GRID_WIDTH; x++)
		{
			int rectY = y * GRID_SIZE;
			int rectX = x * GRID_SIZE;

			
			if (open && open[y][x] < MAXINT32)
			{
				SelectObject(hdc, openBrush);
				Rectangle(hdc, rectX, rectY, rectX + GRID_SIZE, rectY + GRID_SIZE);

			}
			
			if (closed && closed[y][x])
			{
				SelectObject(hdc, closeBrush);
				Rectangle(hdc, rectX, rectY, rectX + GRID_SIZE, rectY + GRID_SIZE);
			}

			if (tile[y][x].type == Wall)
			{
				SelectObject(hdc, wallBrush);
				Rectangle(hdc, rectX, rectY, rectX + GRID_SIZE, rectY + GRID_SIZE);

			}
		}

	}

	

	SelectObject(hdc, srcBrush);
	Rectangle(hdc, jps.GetSrcX() * GRID_SIZE, jps.GetSrcY() * GRID_SIZE,
		(jps.GetSrcX() + 1) * GRID_SIZE, (jps.GetSrcY() + 1) * GRID_SIZE);

	SelectObject(hdc, destBrush);
	Rectangle(hdc, jps.GetDestX() * GRID_SIZE, jps.GetDestY() * GRID_SIZE,
		(jps.GetDestX() + 1) * GRID_SIZE, (jps.GetDestY() + 1) * GRID_SIZE);

	if (!path.empty())
	{
		SelectObject(hdc, pathPen);
		MoveToEx(hdc, path[0].x * GRID_SIZE + GRID_SIZE / 2, path[0].y * GRID_SIZE + GRID_SIZE / 2, NULL);
		for (int i = 1; i < path.size(); i++)
		{
			LineTo(hdc, path[i].x * GRID_SIZE + GRID_SIZE / 2, path[i].y * GRID_SIZE + GRID_SIZE /2);
			MoveToEx(hdc, path[i].x * GRID_SIZE + GRID_SIZE / 2, path[i].y * GRID_SIZE + GRID_SIZE / 2, NULL);

		}
	}

	DeleteObject(pathPen);
	DeleteObject(wallBrush);
	DeleteObject(srcBrush);
	DeleteObject(destBrush);
	DeleteObject(openBrush);
	DeleteObject(closeBrush);
	SelectObject(hdc, originPen);


	tmpDC = hdc;
	hdc = memDC;
	memDC = tmpDC;
	GetClientRect(hWnd, &bufferRect);
	BitBlt(hdc, 0, 0, bufferRect.right, bufferRect.bottom, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBackBit);
	DeleteObject(backBit);
	DeleteDC(memDC);


	ReleaseDC(hWnd, hdc);
}
