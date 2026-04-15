
#include <stdlib.h>
#include "moveGenerator.h"
#include "pieceMoves.h"
#include "aiMove.h"
#include <stdio.h>
#include <unistd.h>
#include "gameBoard.h"

//Standard position layout with the corresponding magic numbers
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

//Initializing move maps
static void Init()
{
	GenerateKnighMoves();
	GenerateKingMoves();
	GenerateSlides();
	GeneratePawnAttacks();
}

static uint8_t MakeMove(uint32_t move, uint64_t *boards)
{
	uint8_t start = move & 0xFF;
	uint8_t end	= (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xFF;
	uint8_t fAll = (move >> 24) & 0xFF;

	//Getting enemy all index
	uint8_t eAll = fAll ^ 1; 
	uint64_t enemyAll = boards[eAll];
		
	//enemy starting index
	uint8_t eStart = (eAll - 12) * 6; 

	uint64_t toMask = (1ULL << end);
	uint64_t fromMaskNot = ~(1ULL << start);

	//Handle own board
	boards[pIdx] &= fromMaskNot;
	boards[pIdx] |= toMask;
		
	boards[fAll] &= fromMaskNot;
	boards[fAll] |= toMask;

	//Handle enemy boards
	boards[eAll] &= ~toMask;
	if (enemyAll == boards[eAll])
		return (20);

	//Saving the captured piece for unmakemove later
	for (uint8_t i = eStart; i < eStart + 6; i++)
	{
		if (boards[i] & (1ULL << end))
		{
			boards[i] &= ~toMask;
			return (i);
		}
	}
	return (20);
}

//Function for the actual game loop that keeps on running
static void GameLoop()
{
	bool white = true;
	uint64_t boards[14];
	GenerateTheStart(boards);
	while (true)
	{
		uint32_t move = GetMove(boards, white);
		MakeMove(move, boards);
		PrintGameBoard(boards);
		white = !white;
		sleep(1);
	}
}

int main()
{
	Init();
	GameLoop();
	return (0);
}
