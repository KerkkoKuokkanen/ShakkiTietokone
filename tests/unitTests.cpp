
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "aiMove.h"
#include "test.h"
#include "gameBoard.h"
#include "moveGenerator.h"
#include "posEvaluation.h"

#define INF 20000000

//Helper funtion for counting the possible moves
static int CountHelper(uint32_t *moves)
{
	for (int i = 0; i < 255; i++)
	{
		if (moves[i] == 0)
			return (i);
	}
	return (255);
}

static int CastleCounter(uint32_t *moves, uint8_t target1, uint8_t target2)
{
	int count = 0;
	for (int i = 0; i < 255; i++)
	{
		if (moves[i] == 0)
			break ;
		uint8_t start = moves[i] & 0xFF;
		if (start == target1 || start == target2)
			count += 1;
	}
	return (count);
}

//Helper function for counting the moves for individual piece type
static int PieceTypeCountHelper(uint32_t *moves, uint8_t pType)
{
	//uint8_t pIdx = (move >> 16) & 0xF;
	int count = 0;
	for (int i = 0; i < 255; i++)
	{
		uint8_t pIdx = (moves[i] >> 16) & 0xF;
		if (moves[i] == 0)
			break ;
		if (pType == pIdx)
			count++;
	}
	return (count);
}

//Init so that the systems work
static void Init()
{
	GenerateKnighMoves();
	GenerateKingMoves();
	GenerateSlides();
	GeneratePawnAttacks();
}

//Testing the move generation
//We have many different board states and we check if the chess AI can
//generate all the required moves for the position
//I have calculated the amount of possible moves in theese positions
//We check if the result matches the expected amount of moves
TEST_CASE("Possible move generation", "[moves]")
{
	Init();
	uint32_t moves[256] = {0};
	uint64_t boards[14] = {0};

	SECTION("Black moves board one")
	{
		GenerateMateInTwoBoards2(boards);
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], 
							boards[10], boards[11], boards[13], boards[12], boards);
		
		REQUIRE(CountHelper(moves) == 8);
		REQUIRE(PieceTypeCountHelper(moves, 6) == 0);
		REQUIRE(PieceTypeCountHelper(moves, 9) == 6);
		REQUIRE(PieceTypeCountHelper(moves, 10) == 2);
		REQUIRE(PieceTypeCountHelper(moves, 11) == 0);
	}

	SECTION("White moves board one")
	{
		GenerateMateInTwoBoards2(boards); // Fresh state isolated from Black's section
		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], 
							boards[4], boards[5], boards[12], boards[13], boards);
		
		REQUIRE(CountHelper(moves) == 47);
		REQUIRE(PieceTypeCountHelper(moves, 2) == 12);
		REQUIRE(PieceTypeCountHelper(moves, 3) == 13);
		REQUIRE(PieceTypeCountHelper(moves, 4) == 19);
		REQUIRE(PieceTypeCountHelper(moves, 5) == 3);
	}

	MoveGenerationTest(boards);
	SECTION("Black moves with all piece types")
	{
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], 
							boards[10], boards[11], boards[13], boards[12], boards);
		REQUIRE(CountHelper(moves) == 33);
		REQUIRE(PieceTypeCountHelper(moves, 6) == 2);
		REQUIRE(PieceTypeCountHelper(moves, 7) == 2);
		REQUIRE(PieceTypeCountHelper(moves, 8) == 7);
		REQUIRE(PieceTypeCountHelper(moves, 9) == 7);
		REQUIRE(PieceTypeCountHelper(moves, 10) == 13);
		REQUIRE(PieceTypeCountHelper(moves, 11) == 2);
	}

	SECTION("White moves with all piece types")
	{
		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], 
							boards[4], boards[5], boards[12], boards[13], boards);
		REQUIRE(CountHelper(moves) == 45);
		REQUIRE(PieceTypeCountHelper(moves, 0) == 2);
		REQUIRE(PieceTypeCountHelper(moves, 1) == 4);
		REQUIRE(PieceTypeCountHelper(moves, 2) == 6);
		REQUIRE(PieceTypeCountHelper(moves, 3) == 13);
		REQUIRE(PieceTypeCountHelper(moves, 4) == 19);
		REQUIRE(PieceTypeCountHelper(moves, 5) == 1);
	}

	SECTION("Black en passant")
	{
		EnPassantTestBlack(boards);
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], 
							boards[10], boards[11], boards[13], boards[12], boards);
		REQUIRE(PieceTypeCountHelper(moves, 6) == 2);
	}

	SECTION("White en passant")
	{
		EnPassantTest(boards);
		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], 
							boards[4], boards[5], boards[12], boards[13], boards);
		REQUIRE(PieceTypeCountHelper(moves, 0) == 2);
	}
}

//Testing the board evaluation function
//I have calculated the expected results for single board
//We check if the result from the position score function matches the expected result
TEST_CASE("Board evaluation test", "[evaluation]")
{
	Init();
	uint64_t boards[14] = {0};

	SECTION("Board one")
	{
		GenerateMateInTwoBoards(boards);
		REQUIRE(GetPositionScore(boards) == 235);
	}

	SECTION("Board two")
	{
		GenerateMateInTwoBoards2(boards);
		REQUIRE(GetPositionScore(boards) == 220);
	}

	SECTION("Board three")
	{
		GenerateMateInThree(boards);
		REQUIRE(GetPositionScore(boards) == 440);
	}
}

//Testing the iteration of chess computer itself
//Testing happens such that we have a mate in n moves postion
//We will track if the revieved score of the iterative algorithm
//mathces the expected score of a check mate when closing in on a checkmate
TEST_CASE("Iterative algorithm testing", "[iteration]")
{
	Init();
	uint64_t boards[14] = {0};

	SECTION("First mate scenario")
	{
		GenerateMateInTwoBoards(boards);
		int ret = AiTurn(boards, 6, -INF, INF, true);
		REQUIRE(ret == 10000030);
		GenerateMateInTwoBoardsSecondState(boards);
		ret = AiTurn(boards, 6, -INF, INF, true);
		REQUIRE(ret == 10000050);
	}

	SECTION("Second mate scenario (Black)")
	{
		GenerateMateInTwoBoards2Black(boards);
		int ret = AiTurn(boards, 6, -INF, INF, false);
		REQUIRE(ret == -10000030);
		GenerateMateInTwoBoards2BlackSecondState(boards);
		ret = AiTurn(boards, 6, -INF, INF, false);
		REQUIRE(ret == -10000050);
	}

	SECTION("Mate in three scenario")
	{
		GenerateMateInThree(boards);
		int ret = AiTurn(boards, 6, -INF, INF, true);
		REQUIRE(ret == 10000010);
		GenerateMateInThreeStateTwo(boards);
		ret = AiTurn(boards, 6, -INF, INF, true);
		REQUIRE(ret == 10000030);
		GenerateMateInThreeStateThree(boards);
		ret = AiTurn(boards, 6, -INF, INF, true);
		REQUIRE(ret == 10000050);
	}
}

//Here we test special moves
//En passant was already tested in the move generation but here
//We will see if the computer can generate all the castling moves
//and all the promotion moves also
TEST_CASE("Castling and promotion", "[special]")
{
	Init();
	uint64_t boards[14] = {0};
	uint32_t moves[256] = {0};
	CastleUnitTest(boards);

	SECTION("castle test")
	{
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], 
							boards[10], boards[11], boards[13], boards[12], boards);
		REQUIRE(CastleCounter(moves, 102, 103) == 2);

		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], 
							boards[4], boards[5], boards[12], boards[13], boards);
		REQUIRE(CastleCounter(moves, 100, 101) == 2);
	}

	SECTION("Promotion test")
	{
		PromotionTestBlack(boards);
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], 
							boards[10], boards[11], boards[13], boards[12], boards);
		int amount = PieceTypeCountHelper(moves, 6);
		REQUIRE(amount == 4);

		PromotionTestWhite(boards);
		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], 
							boards[4], boards[5], boards[12], boards[13], boards);
		amount = PieceTypeCountHelper(moves, 0);
		REQUIRE(amount == 4);
	}
}
