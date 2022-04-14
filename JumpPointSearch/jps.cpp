#include <Windows.h>
#include <queue>
#include <iostream>
#include "jps.h"

JumpPointSearch jps(GRID_HEIGHT, GRID_WIDTH);
//UU LL DD RR UL DL DR UR
const int deltaY[] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int deltaX[] = { 0, -1, 0, 1, -1, -1, 1, 1 };

const int cost[] = { 10, 10, 10, 10, 14, 14, 14, 14 };



vector<Pos> path;

JumpPointSearch::JumpPointSearch(int height, int width) : HEIGHT(height), WIDTH(width)
{
	destY = HEIGHT / 2;
	destX = WIDTH / 3 * 2;
	srcY = HEIGHT / 2;
	srcX = WIDTH / 3;

	closed = new bool* [HEIGHT];
	openNode = new Node * [HEIGHT];
	for (int i = 0; i < HEIGHT; i++)
	{
		closed[i] = new bool[WIDTH];
		openNode[i] = new Node[WIDTH];
	}

	InitList();
}

JumpPointSearch::~JumpPointSearch()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		delete[] closed[i];
		delete[] openNode[i];

	}
	delete[] closed;
	delete[] openNode;
}


void JumpPointSearch::InitList()
{
	int i, j;



	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			closed[i][j] = false;
			openNode[i][j].f = MAXINT32;
		}
	}

}

void JumpPointSearch::Run()
{
	using std::priority_queue;

	path.clear();
	InitList();

	priority_queue<Node*, std::vector<Node*>, cmp> pq;

	Node* new_node = &openNode[srcY][srcX];
	new_node->g = 0;
	new_node->f = WEIGHT * (abs(destY - srcY) + abs(destX - srcX));
	new_node->y = srcY;
	new_node->x = srcX;
	new_node->parent = nullptr;
	new_node->left_corner = false;
	new_node->right_corner = false;
	new_node->dir = DIR::None;

	pq.push(new_node);

	Node* cur_node = nullptr;

	while (!pq.empty())
	{
		cur_node = pq.top();
		pq.pop();

		if (closed[cur_node->y][cur_node->x])
		{
			continue;
		}
		closed[cur_node->y][cur_node->x] = true;

		Render();

		Sleep(200);

		// 도착
		if (cur_node->y == destY && cur_node->x == destX)
			break;

		Node* node;

		switch (cur_node->dir)
		{
		case DIR::UU:
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->right_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		case DIR::LL:
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->right_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		case DIR::DD:
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->right_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		case DIR::RR:
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->right_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
			}

			break;
		case DIR::UL:
			if (CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
			}
			if (cur_node->right_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		case DIR::DL:
			if (CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
			}
			if (cur_node->right_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
			}

			break;

		case DIR::DR:
			if (CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
			}
			if (cur_node->right_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		case DIR::UR:
			if (CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
			}
			if (cur_node->right_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
			}

			if (cur_node->left_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		case DIR::None:
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
			}

			if (CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
			}
			if (CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
			}
			break;
		}


	}

	
	// 경로 저장
	if(cur_node->x == destX && cur_node->y == destY)
		while (cur_node)
		{
			Pos pos;
			pos.y = cur_node->y;
			pos.x = cur_node->x;

			path.push_back(pos);
			Node* tmp = cur_node;
			cur_node = cur_node->parent;

		}



}

bool JumpPointSearch::CheckCornerR(Node* in, Node** out)
{
	int cur_x = in->x + 1;
	int cur_y = in->y;

	bool left_corner = false;
	bool right_corner = false;
	bool find = false;

	while (cur_x <= WIDTH - 1)
	{
		if (tile[cur_y][cur_x].type == Wall) break;


		if (cur_y == destY && cur_x == destX) find = true;


		if (cur_x < WIDTH - 1) {
			if (cur_y > 0
				&& tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			{
				left_corner = true;
			}

			if (cur_y < HEIGHT - 1
				&& tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			{
				right_corner = true;
			}
		}

		if (left_corner || right_corner || find)
		{
			int g = in->g + cost[(int)DIR::RR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::RR;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}
		else
			cur_x++;

	}

	return false;
}

bool JumpPointSearch::CheckCornerL(Node* in, Node** out)
{
	int cur_x = in->x - 1;
	int cur_y = in->y;

	bool left_corner = false;
	bool right_corner = false;

	bool find = false;

	while (cur_x >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;


		if (cur_y == destY && cur_x == destX) find = true;

		if (cur_x > 0) {
			if (cur_y > 0
				&& tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			{
				right_corner = true;
			}
			if (cur_y < HEIGHT - 1
				&& tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (left_corner || right_corner || find)
		{
			int g = in->g + cost[(int)DIR::LL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::LL;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}
		else
			cur_x--;
	}

	return false;
}

bool JumpPointSearch::CheckCornerU(Node* in, Node** out)
{
	int cur_x = in->x;
	int cur_y = in->y - 1;

	bool left_corner = false;
	bool right_corner = false;
	bool find = false;

	while (cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) find = true;

		if (cur_y > 0) {
			if (cur_x > 0
				&& tile[cur_y][cur_x - 1].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			{
				left_corner = true;
			}
			if (cur_x < WIDTH - 1
				&& tile[cur_y][cur_x + 1].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			{
				right_corner = true;
			}
		}

		if (left_corner || right_corner || find)
		{
			int g = in->g + cost[(int)DIR::UU] * abs(cur_y - in->y);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::UU;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}
		else
			cur_y--;
	}

	return false;
}

bool JumpPointSearch::CheckCornerD(Node* in, Node** out)
{
	int cur_x = in->x;
	int cur_y = in->y + 1;

	bool left_corner = false;
	bool right_corner = false;
	bool find = false;

	while (cur_y < HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;


		if (cur_y == destY && cur_x == destX) find = true;


		if (cur_y < HEIGHT - 1) {

			if (cur_x > 0
				&& tile[cur_y][cur_x - 1].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			{
				right_corner = true;
			}

			if (cur_x < WIDTH - 1
				&& tile[cur_y][cur_x + 1].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			{
				left_corner = true;
			}
		}

		if (left_corner || right_corner || find)
		{
			int g = in->g + cost[(int)DIR::DD] * abs(cur_y - in->y);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::DD;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}
		else
			cur_y++;
	}

	return false;
}

bool JumpPointSearch::CheckCornerUR(Node* in, Node** out)
{
	int cur_x = in->x + 1;
	int cur_y = in->y - 1;


	bool left_corner = false;
	bool right_corner = false;

	bool find = false;

	while (cur_x < WIDTH && cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) find = true;


		if (cur_y > 0 && 
			tile[cur_y][cur_x - 1].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_x < WIDTH - 1 &&
			tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
		{
			right_corner = true;
		}

		if (left_corner || right_corner	|| find ||
			CheckCornerU(cur_y, cur_x) || CheckCornerR(cur_y, cur_x))
		{
			int g = in->g + cost[(int)DIR::UR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::UR;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}
		

		cur_x++;
		cur_y--;
	}

	return false;
}

bool JumpPointSearch::CheckCornerUL(Node* in, Node** out)
{
	int cur_x = in->x - 1;
	int cur_y = in->y - 1;


	bool left_corner = false;
	bool right_corner = false;

	bool find = false;

	while (cur_x >= 0 && cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) find = true;


		if (cur_x > 0 &&
			tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_y > 0 &&
			tile[cur_y][cur_x + 1].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
		{
			right_corner = true;
		}

		if (left_corner || right_corner || find ||
			CheckCornerU(cur_y, cur_x) || CheckCornerL(cur_y, cur_x))
		{
			int g = in->g + cost[(int)DIR::UL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::UL;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}


		cur_x--;
		cur_y--;
	}

	return false;
}

bool JumpPointSearch::CheckCornerDL(Node* in, Node** out)
{
	int cur_x = in->x - 1;
	int cur_y = in->y + 1;


	bool left_corner = false;
	bool right_corner = false;

	bool find = false;

	while (cur_x >= 0 && cur_y < HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;


		if (cur_y == destY && cur_x == destX) find = true;

		if (cur_y < HEIGHT - 1 &&
			tile[cur_y][cur_x + 1].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_x > 0 &&
			tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
		{
			right_corner = true;
		}

		if (left_corner || right_corner || find ||
			CheckCornerD(cur_y, cur_x) || CheckCornerL(cur_y, cur_x))
		{
			int g = in->g + cost[(int)DIR::DL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::DL;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}


		cur_x--;
		cur_y++;
	}

	return false;
}

bool JumpPointSearch::CheckCornerDR(Node* in, Node** out)
{
	int cur_x = in->x + 1;
	int cur_y = in->y + 1;


	bool left_corner = false;
	bool right_corner = false;

	bool find = false;

	while (cur_x < WIDTH && cur_y < HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) find = true;

		if (cur_x < WIDTH - 1 &&
			tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_y < HEIGHT - 1 &&
			tile[cur_y][cur_x - 1].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
		{
			right_corner = true;
		}

		if (left_corner || right_corner || find ||
			(CheckCornerD(cur_y, cur_x)) || 
			(CheckCornerR(cur_y, cur_x)))
		{
			int g = in->g + cost[(int)DIR::DR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (openNode[cur_y][cur_x].f < g + h) break;
			openNode[cur_y][cur_x].f = g + h;

			*out = &openNode[cur_y][cur_x];
			(*out)->x = cur_x;
			(*out)->y = cur_y;
			(*out)->left_corner = left_corner;
			(*out)->right_corner = right_corner;
			(*out)->dir = DIR::DR;
			(*out)->g = g;
			(*out)->f = g + h;
			(*out)->parent = in;

			return true;
		}


		cur_x++;
		cur_y++;
	}

	return false;
}


bool JumpPointSearch::CheckCornerR(int cur_y, int cur_x)
{
	cur_x++;


	while (cur_x <= WIDTH - 1)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX)
			return true;

		if (cur_x >= WIDTH - 1) break;

		if (cur_y > 0
			&& tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			return true;
		if (cur_y < HEIGHT - 1
			&& tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			return true;

		cur_x++;
	}

	return false;
}

bool JumpPointSearch::CheckCornerU(int cur_y, int cur_x)
{
	cur_y--;


	while (cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) return true;

		if (cur_y <= 0) break;

		if (cur_x > 0
			&& tile[cur_y][cur_x - 1].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			return true;

		if (cur_x < WIDTH - 1
			&& tile[cur_y][cur_x + 1].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			return true;

		cur_y--;
	}

	return false;
}


bool JumpPointSearch::CheckCornerL(int cur_y, int cur_x)
{
	cur_x--;

	while (cur_x >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) return true;

		if (cur_x <= 0) break;

		if (cur_y > 0
			&& tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			return true;
		if (cur_y < HEIGHT - 1
			&& tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			return true;
		else
			cur_x--;
	}

	return false;
}


bool JumpPointSearch::CheckCornerD(int cur_y, int cur_x)
{
	cur_y++;

	while (cur_y < HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		if (cur_y == destY && cur_x == destX) return true;
		if (cur_y >= HEIGHT - 1) break;

		if (cur_x > 0
			&& tile[cur_y][cur_x - 1].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			return true;
		if (cur_x < WIDTH - 1
			&& tile[cur_y][cur_x + 1].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			return true;

		cur_y++;
	}

	return false;
}

