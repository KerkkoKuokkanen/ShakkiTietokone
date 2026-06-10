
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
TEST_CASE("Possible move generation", "[generation]")
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

struct BWboard {
	uint64_t white;
	uint64_t black;
};

const static BWboard expectedResultsBlack[] = {{10394344228149788672llu, 1049060llu},
										 {10394344228149788672llu, 268435940llu},
										 {10394344228149788672llu, 135648llu},
										 {10394344228149788672llu, 528864llu},
										 {10394344228149788672llu, 4326llu},
										 {10394344228149788672llu, 135396llu},
										 {10394344228149788672llu, 67113188llu},
										 {10394344228149788672llu, 34359742692llu},
										 {10394344228149788672llu, 17592186048740llu},
										 {10394344228149788672llu, 9007199254745316llu},
										 {10394344228149788672llu, 4611686018427392228llu},
										 {10394344228149788672llu, 4556llu},
										 {10394344228149788672llu, 4564llu},
										 {10394344228149788672llu, 12740llu},
										 {10394344228149788672llu, 2101700llu},
										 {10394344228149788672llu, 536875460llu},
										 {10394344228149788672llu, 137438958020llu},
										 {10394309043777699840llu, 35184372093380llu},
										 {10394344228149788672llu, 12708llu},
										 {10394344228149788672llu, 20900llu},
										 {10394344228149788672llu, 37284llu},
										 {10394344228149788672llu, 1053092llu},
										 {10394344228149788672llu, 4198820llu},
										 {10394344228149788672llu, 134222244llu},
										 {10394344228149788672llu, 1073746340llu},
										 {10394344228149788672llu, 17179873700llu},
										 {10394344228149788672llu, 274877911460llu},
										 {10394344228149788672llu, 2199023260068llu},
										 {10394344228149788672llu, 70368744182180llu},
										 {10394344228149788672llu, 281474976715172llu},
										 {10376329829640306688llu, 18014398509486500llu},
										 {10394344228149788672llu, 20836llu},
										 {10394344228149788672llu, 37220llu}};

const static BWboard expectedResultsWhite[] = {{10376330104518213632llu, 4580llu},
											  {10376400198384484352llu, 4580llu},
											  {10394344223854952448llu, 4580llu},
											  {10394344223921930240llu, 4580llu},
											  {10394348621901332480llu, 4580llu},
											  {10394907173808242688llu, 4580llu},
											  {10394343128638164992llu, 484llu},
											  {10394343128638685184llu, 4580llu},
											  {10394343128705269760llu, 4580llu},
											  {10394343137228095488llu, 4580llu},
											  {10394906078591582208llu, 4580llu},
											  {10682573504789872640llu, 4580llu},
											  {10394309043777699872llu, 4548llu},
											  {10394309043777708032llu, 4580llu},
											  {10394309043779796992llu, 4580llu},
											  {10394309044314570752llu, 4580llu},
											  {10394309181216653312llu, 4580llu},
											  {10394311242800955392llu, 4580llu},
											  {10394313441824210944llu, 4580llu},
											  {10394317839870722048llu, 4580llu},
											  {10394326635963744256llu, 4580llu},
											  {10394379412521877504llu, 4580llu},
											  {10394449781266055168llu, 4580llu},
											  {10403316243032440832llu, 4580llu},
											  {12700152052991393792llu, 4580llu},
											  {9241422723542945792llu, 484llu},
											  {9241422723543990272llu, 4580llu},
											  {9241422723559718912llu, 4580llu},
											  {9241422723811377152llu, 4580llu},
											  {9241422732132876288llu, 4580llu},
											  {9241422792262418432llu, 4580llu},
											  {9241423273298755584llu, 4580llu},
											  {9241427121589452800llu, 4580llu},
											  {9241440315728986112llu, 4580llu},
											  {9241493092287119360llu, 4580llu},
											  {9243674523356626944llu, 4580llu},
											  {9245926323170312192llu, 4580llu},
											  {9250429922797682688llu, 4580llu},
											  {9313480317580869632llu, 4580llu},
											  {9385537911618797568llu, 4580llu},
											  {9529653099694653440llu, 4580llu},
											  {9817883475846365184llu, 4580llu},
											  {11547265732756635648llu, 4580llu},
											  {13853108741970329600llu, 4580llu},
											  {1207000988313976832llu, 4580llu}};

//This test block tests the MakeMove and UnmakeMove functions
//We have a board with all types of pieces and we test that the
//Bitboard maps correspond to the expected ones when we make a move
TEST_CASE("making moves", "[moves]")
{
	Init();
	uint64_t boards[14] = {0};
	uint32_t moves[256] = {0};

	MoveGenerationTest(boards);
	SECTION("Black all moves")
	{
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], 
								boards[10], boards[11], boards[13], boards[12], boards);
		uint8_t castleRighs = 0;
		for (int i = 0; i < 255; i++)
		{
			if (moves[i] == 0)
				break ;

			uint64_t board1 = boards[12];
			uint64_t board2 = boards[13];
			uint8_t ret = MakeMove(moves[i], boards, &castleRighs);
			REQUIRE(expectedResultsBlack[i].white == boards[12]);
			REQUIRE(expectedResultsBlack[i].black == boards[13]);
			UnMakeMove(moves[i], boards, ret, castleRighs);
			REQUIRE(board1 == boards[12]);
			REQUIRE(board2 == boards[13]);
		}
	}
	MoveGenerationTest(boards);
	SECTION("White all moves")
	{
		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], 
							boards[4], boards[5], boards[12], boards[13], boards);
		uint8_t castleRighs = 0;
		for (int i = 0; i < 255; i++)
		{
			if (moves[i] == 0)
				break ;

			uint64_t board1 = boards[12];
			uint64_t board2 = boards[13];
			uint8_t ret = MakeMove(moves[i], boards, &castleRighs);
			REQUIRE(expectedResultsWhite[i].white == boards[12]);
			REQUIRE(expectedResultsWhite[i].black == boards[13]);
			UnMakeMove(moves[i], boards, ret, castleRighs);
			REQUIRE(board1 == boards[12]);
			REQUIRE(board2 == boards[13]);
		}
	}
}
