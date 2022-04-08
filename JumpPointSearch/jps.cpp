#include <Windows.h>
#include <queue>
#include <iostream>
#include "jps.h"
#include "grid.h"

JumpPointSearch jps;
//UU LL DD RR UL DL DR UR
const int deltaY[] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int deltaX[] = { 0, -1, 0, 1, -1, -1, 1, 1 };

const int cost[] = { 10, 10, 10, 10, 14, 14, 14, 14 };

// 출력 없으면 함수 내부로
bool closed[GRID_HEIGHT][GRID_WIDTH];
int open[GRID_HEIGHT][GRID_WIDTH];

vector<Pos> path;

JumpPointSearch::JumpPointSearch()
{
	destY = GRID_HEIGHT / 2;
	destX = GRID_WIDTH / 3 * 2;
	srcY = GRID_HEIGHT / 2;
	srcX = GRID_WIDTH / 3;
}

JumpPointSearch::~JumpPointSearch()
{

}

void JumpPointSearch::Run()
{
	using std::priority_queue;

	path.clear();

	int i, j;
	
	

	
	for (i = 0; i < GRID_HEIGHT; i++)
	{
		for (j = 0; j < GRID_WIDTH; j++)
		{
			closed[i][j] = false;
			open[i][j] = MAXINT32;
		}
	}

	priority_queue<Node*, std::vector<Node*>, cmp> pq;
	vector<Node*> delete_list;

	Node* new_node = new Node;
	new_node->g = 0;
	new_node->f = WEIGHT * (abs(destY - srcY) + abs(destX - srcX));
	new_node->y = srcY;
	new_node->x = srcX;
	new_node->parent = nullptr;
	new_node->left_corner = false;
	new_node->right_corner = false;
	new_node->dir = DIR::None;

	pq.push(new_node);
	open[srcY][srcX] = new_node->f;
	delete_list.push_back(new_node);

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
				delete_list.push_back(node);
			}

			if (cur_node->right_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			break;
		case DIR::LL:
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->right_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			break;
		case DIR::DD:
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->right_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			break;
		case DIR::RR:
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->right_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			break;
		case DIR::UL:
			if (CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (cur_node->right_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			break;
		case DIR::DL:
			if (CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (cur_node->right_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			break;

		case DIR::DR:
			if (CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (cur_node->right_corner && CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			break;
		case DIR::UR:
			if (CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (cur_node->right_corner && CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (cur_node->left_corner && CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			break;
		case DIR::None:
			if (CheckCornerU(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}

			if (CheckCornerUL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerDL(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerD(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerDR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
			}
			if (CheckCornerUR(cur_node, &node))
			{
				pq.push(node);
				delete_list.push_back(node);
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

	while (!delete_list.empty())
	{
		cur_node = delete_list.back();
		delete_list.pop_back();

		delete cur_node;

	}

}

bool JumpPointSearch::CheckCornerR(Node* in, Node** out)
{
	int cur_x = in->x + 1;
	int cur_y = in->y;

	bool left_corner = false;
	bool right_corner = false;

	while (cur_x <= GRID_WIDTH - 1)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::RR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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


		if (cur_x >= GRID_WIDTH - 1) break;

		if (cur_y > 0) {
			if (tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (cur_y < GRID_HEIGHT - 1)
		{
			if (tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (left_corner || right_corner)
		{
			int g = in->g + cost[(int)DIR::RR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

	while (cur_x >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::LL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

		if (cur_x <= 0) break;

		if (cur_y > 0) {
			if (tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (cur_y < GRID_HEIGHT - 1)
		{
			if (tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (left_corner || right_corner)
		{
			int g = in->g + cost[(int)DIR::LL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

	while (cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::UU] * abs(cur_y - in->y);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
		if (cur_y <= 0) break;

		if (cur_x > 0) {
			if (tile[cur_y][cur_x - 1].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (cur_x < GRID_WIDTH - 1)
		{
			if (tile[cur_y][cur_x + 1].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (left_corner || right_corner)
		{
			int g = in->g + cost[(int)DIR::UU] * abs(cur_y - in->y);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

	while (cur_y < GRID_HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::DD] * abs(cur_y - in->y);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
		if (cur_y >= GRID_HEIGHT - 1) break;
		// 코너 확인
		if (cur_x > 0) {
			if (tile[cur_y][cur_x - 1].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (cur_x < GRID_WIDTH - 1)
		{
			if (tile[cur_y][cur_x + 1].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			{
				left_corner = true;
			}
		}
		// 코너 있으면 노드 생성
		if (left_corner || right_corner)
		{
			int g = in->g + cost[(int)DIR::DD] * abs(cur_y - in->y);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
	while (cur_x < GRID_WIDTH && cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::UR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

		// 코너 확인
		if (cur_y > 0 && 
			tile[cur_y][cur_x - 1].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_x < GRID_WIDTH - 1 &&
			tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
		{
			right_corner = true;
		}
		// 코너 있거나 없으면 수직수평 확인 후 노드 생성 
		if (left_corner || right_corner	|| 
			CheckCornerU(cur_y, cur_x) || CheckCornerR(cur_y, cur_x))
		{
			int g = in->g + cost[(int)DIR::UR] * abs(cur_y - in->y); // x, y 이동 거리 같음
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
	while (cur_x >= 0 && cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::UL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

		// 코너 확인
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
		// 코너 있거나 없으면 수직수평 확인 후 노드 생성 
		if (left_corner || right_corner || 
			CheckCornerU(cur_y, cur_x) || CheckCornerL(cur_y, cur_x))
		{
			int g = in->g + cost[(int)DIR::UL] * abs(cur_y - in->y); // x, y 이동 거리 같음
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
	while (cur_x >= 0 && cur_y < GRID_HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::DL] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
		// 코너 확인
		if (cur_y < GRID_HEIGHT - 1 &&
			tile[cur_y][cur_x + 1].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_x > 0 &&
			tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
		{
			right_corner = true;
		}
		// 코너 있거나 없으면 수직수평 확인 후 노드 생성 
		if (left_corner || right_corner || 
			CheckCornerD(cur_y, cur_x) || CheckCornerL(cur_y, cur_x))
		{
			int g = in->g + cost[(int)DIR::DL] * abs(cur_y - in->y); // x, y 이동 거리 같음
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
	while (cur_x < GRID_WIDTH && cur_y < GRID_HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			int g = in->g + cost[(int)DIR::DR] * abs(cur_x - in->x);
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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
		// 코너 확인
		if (cur_x < GRID_WIDTH - 1 &&
			tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
		{
			left_corner = true;

		}
		if (cur_y < GRID_HEIGHT - 1 &&
			tile[cur_y][cur_x - 1].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
		{
			right_corner = true;
		}
		// 코너 있거나 없으면 수직수평 확인 후 노드 생성 
		bool cornerD = false;
		bool cornerR = false;
		if (left_corner || right_corner || 
			(cornerD = CheckCornerD(cur_y, cur_x)) || 
			(cornerR = CheckCornerR(cur_y, cur_x)))
		{
			int g = in->g + cost[(int)DIR::DR] * abs(cur_y - in->y); // x, y 이동 거리 같음
			int h = WEIGHT * (abs(cur_x - destX) + abs(cur_y - destY));

			if (open[cur_y][cur_x] < g + h) break;
			open[cur_y][cur_x] = g + h;

			*out = new Node;
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

	bool left_corner = false;
	bool right_corner = false;

	while (cur_x <= GRID_WIDTH - 1)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
			return true;

		if (cur_x >= GRID_WIDTH - 1) break;
		if (cur_y > 0) {
			if (tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (cur_y < GRID_HEIGHT - 1)
		{
			if (tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (left_corner || right_corner)
			return true;
		
		else
			cur_x++;
	}

	return false;
}

bool JumpPointSearch::CheckCornerU(int cur_y, int cur_x)
{
	cur_y--;

	bool left_corner = false;
	bool right_corner = false;

	while (cur_y >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;
		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			return true;
		}
		if (cur_y <= 0) break;
		if (cur_x > 0) {
			if (tile[cur_y][cur_x - 1].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (cur_x < GRID_WIDTH - 1)
		{
			if (tile[cur_y][cur_x + 1].type == Wall && tile[cur_y - 1][cur_x + 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (left_corner || right_corner)
		{
			return true;
		}
		else
			cur_y--;
	}

	return false;
}


bool JumpPointSearch::CheckCornerL(int cur_y, int cur_x)
{
	cur_x--;

	bool left_corner = false;
	bool right_corner = false;

	while (cur_x >= 0)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			return true;
		}
		if (cur_x <= 0) break;
		if (cur_y > 0) {
			if (tile[cur_y - 1][cur_x].type == Wall && tile[cur_y - 1][cur_x - 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (cur_y < GRID_HEIGHT - 1)
		{
			if (tile[cur_y + 1][cur_x].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			{
				left_corner = true;
			}
		}
		if (left_corner || right_corner)
		{
			return true;
		}
		else
			cur_x--;
	}

	return false;
}


bool JumpPointSearch::CheckCornerD(int cur_y, int cur_x)
{
	cur_y++;

	bool left_corner = false;
	bool right_corner = false;

	while (cur_y < GRID_HEIGHT)
	{
		if (tile[cur_y][cur_x].type == Wall)
			break;

		// 도착
		if (cur_y == destY && cur_x == destX)
		{
			return true;
		}
		if (cur_y >= GRID_HEIGHT - 1) break;
		// 코너 확인
		if (cur_x > 0) {
			if (tile[cur_y][cur_x - 1].type == Wall && tile[cur_y + 1][cur_x - 1].type == Empty)
			{
				right_corner = true;
			}
		}
		if (cur_x < GRID_WIDTH - 1)
		{
			if (tile[cur_y][cur_x + 1].type == Wall && tile[cur_y + 1][cur_x + 1].type == Empty)
			{
				left_corner = true;
			}
		}
		// 코너 있으면 노드 생성
		if (left_corner || right_corner)
		{
			return true;
		}
		else
			cur_y++;
	}

	return false;
}