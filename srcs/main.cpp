
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

static void MakeMove(uint32_t move, uint64_t *boards)
{
	uint8_t start = move & 0xFF;
	if (start >= 100)
	{
		uint8_t c = 0;
		CastleTheKing(boards, start, &c);
		return ;
	}
	uint8_t end	= (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xF;
	uint8_t endType = (move >> 20) & 0xF;
	uint8_t fAll = (move >> 24) & 0xFF;

	//Getting enemy all index
	uint8_t eAll = fAll ^ 1;

	uint64_t toMask = (1ULL << end);
	uint64_t fromMaskNot = ~(1ULL << start);

	//Handle own board
	boards[pIdx] &= fromMaskNot;
	boards[endType] |= toMask;
		
	boards[fAll] &= fromMaskNot;
	boards[fAll] |= toMask;

	//Handle enemy boards
	boards[eAll] &= ~toMask;
}

static void MakeBoards(int parameter, uint64_t *boards)
{
	if (parameter == 1)
		GenerateMateInTwoBoards(boards);
	else if (parameter == 2)
		GenerateMateInTwoBoards2(boards);
	else if (parameter == 3)
		GenerateMateInThree(boards);
	else if (parameter == 4)
		TestPromotion(boards);
	else if (parameter == 5)
		CastleTest(boards);
	else
		GenerateTheStart(boards);
}

//Function for the actual game loop that keeps on running
static void GameLoop(int parameter)
{
	bool white = true;
	uint64_t boards[14];
	MakeBoards(parameter, boards);
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
		else if (argument == "castle")
			GameLoop(5);
		else
			GameLoop(0);
	}
	return (0);
}
