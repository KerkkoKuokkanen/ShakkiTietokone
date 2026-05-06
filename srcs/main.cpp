
#include <stdlib.h>
#include "moveGenerator.h"
#include "pieceMoves.h"
#include "aiMove.h"
#include <stdio.h>
#include <unistd.h>
#include "gameBoard.h"
#include "test.h"
#include <string>
#include <iostream>
#include "castle.h"
#include "enPassant.h"
#include "posEvaluation.h"

//Standard position layout with the corresponding magic numbers
static bool GenerateTheStart(uint64_t *boards)
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
	return (true);
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
	uint8_t r = 0;
	if (start >= 100)
	{
		CastleTheKing(boards, start, &r);
		SetEnPassantSquare(100);
		return (20);
	}

	uint8_t end	= (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xF;
	uint8_t endType = (move >> 20) & 0xF;
	uint8_t fAll = (move >> 24) & 0xFF;

	if (endType == 14)
	{
		uint8_t victimSquare;
		uint8_t victimBoardIdx;

		if (fAll == 12)
		{
			victimSquare = end + 8;
			victimBoardIdx = 6;
		}
		else
		{
			victimSquare = end - 8;
			victimBoardIdx = 0;
		}

		boards[victimBoardIdx] &= ~(1ull << victimSquare);
		boards[fAll ^ 1] &= ~(1ull << victimSquare);

		boards[pIdx] &= ~(1ull << start);
		boards[pIdx] |= (1ull << end);
		boards[fAll] &= ~(1ull << start);
		boards[fAll] |= (1ull << end);

		SetEnPassantSquare(100);
		return victimBoardIdx;
	}

	if (pIdx == 0)
	{
		if (abs((int)start - (int)end) == 16)
			SetEnPassantSquare(start + 8);
	}
	else if (pIdx == 6)
	{
		if (abs((int)start - (int)end) == 16)
			SetEnPassantSquare(start - 8);
	}
	else
		SetEnPassantSquare(100);


	//Getting enemy all index
	uint8_t eAll = fAll ^ 1; 
	uint64_t enemyAll = boards[eAll];
		
	//enemy starting index
	uint8_t eStart = (eAll - 12) * 6; 

	uint64_t toMask = (1ULL << end);
	uint64_t fromMaskNot = ~(1ULL << start);

	//Handle own board
	boards[pIdx] &= fromMaskNot;
	boards[endType] |= toMask;
		
	boards[fAll] &= fromMaskNot;
	boards[fAll] |= toMask;

	//Handle enemy boards
	boards[eAll] &= ~toMask;
	SetCastleRights(&r, start, end);
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

static bool MakeBoards(int parameter, uint64_t *boards)
{
	if (parameter == 1)
		return GenerateMateInTwoBoards(boards);
	else if (parameter == 2)
		return GenerateMateInTwoBoards2(boards);
	else if (parameter == 3)
		return GenerateMateInThree(boards);
	else if (parameter == 4)
		return TestPromotion(boards);
	else if (parameter == 5)
		return CastleTest(boards);
	else if (parameter == 6)
		return CastleTestBlack(boards);
	else if (parameter == 7)
		return (EnPassantTest(boards));
	else if (parameter == 8)
		return (EnPassantTestBlack(boards));
	else
		return GenerateTheStart(boards);
}

//Function for the actual game loop that keeps on running
static void GameLoop(int parameter)
{
	uint64_t boards[14];
	bool white = MakeBoards(parameter, boards);
	PrintGameBoard(boards);
	sleep(1);
	while (true)
	{
		uint32_t move = GetMove(boards, white);
		if (GetGameOver())
		{
			printf("Game over!\n");
			break ;
		}
		MakeMove(move, boards);
		PrintGameBoard(boards);
		white = !white;
		printf("Press enter to continue\n");
		std::cin.ignore();
		printf("Thinking...\n");
	}
}

int main(int argc, char **argv)
{
	Init();
	if (argc == 1)
		GameLoop(0);
	else
	{
		std::string argument = argv[1];
		if (argument == "test1")
			GameLoop(1);
		else if (argument == "test2")
			GameLoop(2);
		else if (argument == "test3")
			GameLoop(3);
		else if (argument == "test4")
			GameLoop(4);
		else if (argument == "castle1")
			GameLoop(5);
		else if (argument == "castle2")
			GameLoop(6);
		else if (argument == "pass")
			GameLoop(7);
		else if (argument == "passb")
			GameLoop(8);
		else
			GameLoop(0);
	}
	return (0);
}
