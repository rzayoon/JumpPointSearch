#include <Windows.h>
#include <iostream>
#include "grid.h"
#include "jps.h"
using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);



bool click = false;
bool draw = false;


int main()
{

	HINSTANCE hInst = GetModuleHandle(NULL);

	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"MyWndClass";
	if (!RegisterClass(&wndclass)) return 1;

	HWND hWnd = CreateWindow(L"MyWndClass", L"AStar",
		WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU, CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT, NULL, (HMENU)NULL, hInst, NULL);
	if (hWnd == NULL) return 1;

	InitGrid(hWnd);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);


	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			Render();

			Sleep(20);
		}
		if (msg.message == WM_QUIT)
			break;

	}


	return 0;

}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:

		pt.x = LOWORD(lParam) / GRID_SIZE;
		pt.y = HIWORD(lParam) / GRID_SIZE;
		if (pt.x < 0 || pt.x >= GRID_WIDTH) break;
		if (pt.y < 0 || pt.y >= GRID_HEIGHT) break;
		click = true;

		if (tile[pt.y][pt.x].type == Empty)
		{
			draw = true;
		}
		else
		{
			draw = false;
		}
		break;
	case WM_LBUTTONUP:

		click = false;
		draw = false;
		break;
	case WM_MOUSEMOVE:
		if (click)
		{
			pt.x = LOWORD(lParam) / GRID_SIZE;
			pt.y = HIWORD(lParam) / GRID_SIZE;

			if (pt.x < 0 || pt.x >= GRID_WIDTH) break;
			if (pt.y < 0 || pt.y >=  GRID_HEIGHT) break;

			if (pt.y == jps.GetDestY() && pt.x == jps.GetDestX()) break;
			if (pt.y == jps.GetSrcY() && pt.x == jps.GetSrcX()) break;


			if (draw)
				tile[pt.y][pt.x].type = Wall;
			else
				tile[pt.y][pt.x].type = Empty;



		}

		break;
	case WM_LBUTTONDBLCLK:


		pt.x = LOWORD(lParam) / GRID_SIZE;
		pt.y = HIWORD(lParam) / GRID_SIZE;

		if (pt.x < 0 || pt.x >= GRID_WIDTH) break;
		if (pt.y < 0 || pt.y >=  GRID_HEIGHT) break;

		jps.SetSrcX(pt.x);
		jps.SetSrcY(pt.y);

		tile[pt.y][pt.x].type = Empty;

		break;
	case WM_RBUTTONDOWN:
		pt.x = LOWORD(lParam) / GRID_SIZE;
		pt.y = HIWORD(lParam) / GRID_SIZE;

		if (pt.x < 0 || pt.x >= GRID_WIDTH) break;
		if (pt.y< 0 || pt.y >=  GRID_HEIGHT) break;

		jps.SetDestX(pt.x);
		jps.SetDestY(pt.y);

		tile[pt.y][pt.x].type = Empty;
		break;
	case WM_MBUTTONDOWN:
		jps.Run();

		break;

	case WM_RBUTTONDBLCLK:
		InitGrid(hWnd);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return 0;

}