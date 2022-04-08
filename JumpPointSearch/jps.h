#pragma once
#include <vector>

using std::vector;

enum class DIR
{
	UU = 0,
	LL,
	DD,
	RR,
	UL,
	DL,
	DR,
	UR,
	None
};

struct Pos
{
	int y;
	int x;
};

struct Node
{


	int f;
	int g;
	int y;
	int x;

	DIR dir;
	bool left_corner;
	bool right_corner;


	Node* parent;
};

struct cmp
{
	bool operator()(const Node* n1, const Node* n2)
	{
		return n1->f > n2->f;
	}
};

class JumpPointSearch
{

public:
	JumpPointSearch();
	virtual ~JumpPointSearch();

	void SetSrcY(int y) { srcY = y; }
	void SetSrcX(int x) { srcX = x; }
	void SetDestY(int y) { destY = y; }
	void SetDestX(int x) { destX = x; }

	int GetSrcY() { return srcY; }
	int GetSrcX() { return srcX; }
	int GetDestY() { return destY; }
	int GetDestX() { return destX; }

	void Run();

protected:

	bool CheckCornerR(Node* in, Node** out);
	bool CheckCornerU(Node* in, Node** out);
	bool CheckCornerL(Node* in, Node** out);
	bool CheckCornerD(Node* in, Node** out);
	bool CheckCornerUR(Node* in, Node** out);
	bool CheckCornerUL(Node* in, Node** out);
	bool CheckCornerDR(Node* in, Node** out);
	bool CheckCornerDL(Node* in, Node** out);

	bool CheckCornerR(int cur_y, int cur_x);
	bool CheckCornerU(int cur_y, int cur_x);
	bool CheckCornerL(int cur_y, int cur_x);
	bool CheckCornerD(int cur_y, int cur_x);


	int destY;
	int destX;
	int srcY;
	int srcX;

	const int WEIGHT = 8;

};

extern JumpPointSearch jps;
extern vector<Pos> path;