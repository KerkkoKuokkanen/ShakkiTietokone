
#include <stdlib.h>
#include "moveGenerator.h"
#include "pieceMoves.h"

static void GenerateTheStart(uint64_t *boards)
{
	boards[0] = 0x00FF000000000000;		//White pawns
	boards[1] = 0x4200000000000000;		//White knights
	boards[2] = 0x2400000000000000;		//White bishops
	boards[3] = 0x8100000000000000;		//White Rooks
	boards[4] = 0x0800000000000000;		//White queen
	boards[5] = 0x1000000000000000;		//White king
	boards[6] = 0x000000000000FF00;		//Black pawns
	boards[7] = 0x0000000000000042;		//Black knights
	boards[8] = 0x0000000000000024;		//Black bishops
	boards[9] = 0x0000000000000081;		//Black rooks
	boards[10] = 0x0000000000000008;	//Black queen
	boards[11] = 0x0000000000000010;	//Black king
	boards[12] = 0xFFFF000000000000;	//White all pieces
	boards[13] = 0x000000000000FFFF;	//Black all pieces
}

static void Init()
{
	GenerateKnighMoves();
	GenerateKingMoves();
	GenerateSlides();
	GeneratePawnAttacks();
}

static void GameLoop()
{
	uint64_t boards[14];
	GenerateTheStart(boards);
}

static void Testing()
{
	uint64_t b = 68736253951llu;
	uint64_t b1 = 72746074111llu;
	PrinBoard(b);
	PrinBoard(b1);
	uint64_t bm = GetRookMoves(b, b1, 36);
	PrinBoard(bm);
}

int main()
{
	Init();
	Testing();
	GameLoop();
	return (0);
}
