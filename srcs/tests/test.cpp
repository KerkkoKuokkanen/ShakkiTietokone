
#include <stdlib.h>
#include "gameBoard.h"
#include "aiMove.h"
#include "enPassant.h"
#include <stdio.h>

bool GenerateMateInTwoBoards(uint64_t *boards)
{
	boards[0] = 0x0000040800004000;		//White pawns!
	boards[1] = 0;		//White knights
	boards[2] = 0x0000080000000000;		//White bishops!
	boards[3] = 0x0000000000100000;		//White Rooks!
	boards[4] = 0;		//White queen
	boards[5] = 0x0000000000800000;		//White king!
	boards[6] = 1ull << 26;			//Black pawns!
	boards[7] = 0x0000000000200000;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0x0000000000002000;		//Black rooks!
	boards[10] = 0;	//Black queen
	boards[11] = 0x0000000000000040;	//Black king!
	boards[12] = 0x00000C0800904000;	//White all pieces!
	boards[13] = boards[6] | boards[7] | boards[9] | boards[11];	//Black all pieces!
	return (true);
}

void GenerateMateInTwoBoardsSecondState(uint64_t *boards)
{
	boards[0] = 4432406265856llu;
	boards[1] = 0llu;
	boards[2] = 8796093022208llu;
	boards[3] = 0llu;
	boards[4] = 0llu;
	boards[5] = 8388608llu;
	boards[6] = 67108864llu;
	boards[7] = 16llu;
	boards[8] = 0llu;
	boards[9] = 8192llu;
	boards[10] = 0llu;
	boards[11] = 64llu;
	boards[12] = 13228507676672llu;
	boards[13] = 67117136llu;
}

bool GenerateMateInTwoBoards2Black(uint64_t *boards)
{
	boards[6] = 0;		//White pawns!
	boards[7] = 0;		//White knights
	boards[8] = 0x1000000000;		//White bishops!
	boards[9] = 0x100000000000000;		//White Rooks!
	boards[10] = 0x80;		//White queen
	boards[11] = 0x100000000000;		//White king
	boards[0] = 0x80000000000000;		//Black pawns!
	boards[1] = 0;		//Black knights!
	boards[2] = 0;		//Black bishops
	boards[3] = 0x4000000000000000;		//Black rooks!
	boards[4] = 0x40000000000000;	//Black queen
	boards[5] = 0x8000000000000000;	//Black king!
	boards[12] = boards[2] | boards[3] | boards[4] | boards[5];	//White all pieces!
	boards[13] = boards[6] | boards[9] | boards[10] | boards[11];	//Black all pieces!
	return (false);
}

bool GenerateMateInTwoBoards2BlackSecondState(uint64_t *boards)
{
	boards[6] = 0;		//White pawns!
	boards[7] = 0;		//White knights
	boards[8] = 1llu;		//White bishops!
	boards[9] = 0llu;		//White Rooks!
	boards[10] = 128llu;		//White queen
	boards[11] = 17592186044416llu;		//White king
	boards[0] = 36028797018963968llu;		//Black pawns!
	boards[1] = 0;		//Black knights!
	boards[2] = 0;		//Black bishops
	boards[3] = 72057594037927936llu;		//Black rooks!
	boards[4] = 18014398509481984llu;	//Black queen
	boards[5] = 9223372036854775808llu;	//Black king!
	boards[12] = boards[2] | boards[3] | boards[4] | boards[5];	//White all pieces!
	boards[13] = boards[6] | boards[9] | boards[10] | boards[11];	//Black all pieces!
	return (false);
}

bool GenerateMateInTwoBoards2(uint64_t *boards)
{
	boards[0] = 0;		//White pawns!
	boards[1] = 0;		//White knights
	boards[2] = 0x1000000000;		//White bishops!
	boards[3] = 0x100000000000000;		//White Rooks!
	boards[4] = 0x80;		//White queen
	boards[5] = 0x100000000000;		//White king
	boards[6] = 0x80000000000000;		//Black pawns!
	boards[7] = 0;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0x4000000000000000;		//Black rooks!
	boards[10] = 0x40000000000000;	//Black queen
	boards[11] = 0x8000000000000000;	//Black king!
	boards[12] = boards[2] | boards[3] | boards[4] | boards[5];	//White all pieces!
	boards[13] = boards[6] | boards[9] | boards[10] | boards[11];	//Black all pieces!
	return (true);
}

bool GenerateMateInThree(uint64_t *boards)
{
	boards[0] = 0;		//White pawns!
	boards[1] = 1ull << 35;		//White knights
	boards[2] = 0;		//White bishops!
	boards[3] = 1ull << 60;		//White Rooks!
	boards[4] = 0;		//White queen
	boards[5] = 1ull << 53;		//White king
	boards[6] = 1ull << 55;		//Black pawns!
	boards[7] = 1ull << 62;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0;		//Black rooks!
	boards[10] = 0;	//Black queen
	boards[11] = 1ull << 63;	//Black king!
	boards[12] = boards[1] | boards[3] | boards[5];	//White all pieces!
	boards[13] = boards[6] | boards[7] | boards[11];	//Black all pieces!
	return (true);
}

bool GenerateMateInThreeStateTwo(uint64_t *boards)
{
	boards[0] = 0;		//White pawns!
	boards[1] = 34359738368llu;		//White knights
	boards[2] = 0;		//White bishops!
	boards[3] = 4503599627370496llu;		//White Rooks!
	boards[4] = 0;		//White queen
	boards[5] = 9007199254740992llu;		//White king
	boards[6] = 36028797018963968llu;		//Black pawns!
	boards[7] = 35184372088832llu;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0;		//Black rooks!
	boards[10] = 0;	//Black queen
	boards[11] = 9223372036854775808llu;	//Black king!
	boards[12] = boards[1] | boards[3] | boards[5];	//White all pieces!
	boards[13] = boards[6] | boards[7] | boards[11];	//Black all pieces!
	return (true);
}

bool GenerateMateInThreeStateThree(uint64_t *boards)
{
	boards[0] = 0;		//White pawns!
	boards[1] = 536870912llu;		//White knights
	boards[2] = 0;		//White bishops!
	boards[3] = 4503599627370496llu;		//White Rooks!
	boards[4] = 0;		//White queen
	boards[5] = 9007199254740992llu;		//White king
	boards[6] = 36028797018963968llu;		//Black pawns!
	boards[7] = 268435456llu;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0;		//Black rooks!
	boards[10] = 0;	//Black queen
	boards[11] = 9223372036854775808llu;	//Black king!
	boards[12] = boards[1] | boards[3] | boards[5];	//White all pieces!
	boards[13] = boards[6] | boards[7] | boards[11];	//Black all pieces!
	return (true);
}

bool TestPromotion(uint64_t *boards)
{
	boards[0] = 1ull << 10;		//White pawns!
	boards[1] = 0;		//White knights
	boards[2] = 0;		//White bishops!
	boards[3] = 0;		//White Rooks!
	boards[4] = 0;		//White queen
	boards[5] = 1ull << 53;		//White king
	boards[6] = 0;		//Black pawns!
	boards[7] = 0;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0;		//Black rooks!
	boards[10] = 1ull << 1;	//Black queen
	boards[11] = 1ull << 63;	//Black king!
	boards[12] = boards[0] | boards[5];	//White all pieces!
	boards[13] = boards[11] | boards[10];	//Black all pieces!
	return (true);
}

bool CastleTest(uint64_t *boards)
{
	boards[0] = (1ull << 51) | (1ull << 50) | 0x00F0000000000000;
	boards[1] = 0;
	boards[2] = (1ull << 7);
	boards[3] = (1ull << 63);
	boards[4] = 0;
	boards[5] = (1ull << 60);
	boards[6] = (1ull << 49) | (1ull << 48);
	boards[7] = 0;
	boards[8] = 0;
	boards[9] = 0;
	boards[10] = 0;
	boards[11] = (1ull << 56);
	boards[12] = boards[0] | boards[3] | boards[5] | boards[2];
	boards[13] = boards[6] | boards[11];
	return (true);
}

bool CastleTestBlack(uint64_t *boards)
{
	boards[0] = (1ull << 8) | (1ull << 9);
	boards[1] = 0;
	boards[2] = 0;
	boards[3] = 0;
	boards[4] = 0;
	boards[5] = (1ull);
	boards[6] = (1ull << 10) | (1ull << 11) | (1ull << 12) | (1ull << 13) | (1ull << 14) | (1ull << 15);
	boards[7] = 0;
	boards[8] = (1ull << 63);
	boards[9] = (1ull << 7);
	boards[10] = 0;
	boards[11] = (1ull << 4);
	boards[12] = boards[0] | boards[5];
	boards[13] = boards[6] | boards[8] | boards[9] | boards[11];
	return (false);
}

bool EnPassantTest(uint64_t *boards)
{
	boards[0] = (1ull << 24);
	boards[1] = 0;
	boards[2] = 0;
	boards[3] = 0;
	boards[4] = 0;
	boards[5] = (1ull << 63);
	boards[6] = (1ull << 25);
	boards[7] = 0;
	boards[8] = 0;
	boards[9] = 0;
	boards[10] = 0;
	boards[11] = (1ull << 7);
	boards[12] = boards[0] | boards[5];
	boards[13] = boards[6] | boards[11];
	SetEnPassantSquare(17);
	return (true);
}

bool EnPassantTestBlack(uint64_t *boards)
{
	boards[0] = (1ull << 32);
	boards[1] = 0;
	boards[2] = 0;
	boards[3] = 0;
	boards[4] = 0;
	boards[5] = (1ull << 63);
	boards[6] = (1ull << 33);
	boards[7] = 0;
	boards[8] = 0;
	boards[9] = 0;
	boards[10] = 0;
	boards[11] = (1ull << 7);
	boards[12] = boards[0] | boards[5];
	boards[13] = boards[6] | boards[11];
	SetEnPassantSquare(40);
	return (false);
}

bool MoveGenerationTest(uint64_t *boards)
{
	boards[0] = (1ull << 54);
	boards[1] = (1ull << 32);
	boards[2] = (1ull << 40);
	boards[3] = (1ull << 45);
	boards[4] = (1ull << 60);
	boards[5] = (1ull << 63);
	boards[6] = (1ull << 12);
	boards[7] = (1ull << 2);
	boards[8] = (1ull << 8);
	boards[9] = (1ull << 5);
	boards[10] = (1ull << 6);
	boards[11] = (1ull << 7);
	boards[12] = boards[0] | boards[1] | boards[2] | boards[3] | boards[4] | boards[5];
	boards[13] = boards[6] | boards[7] | boards[8] | boards[9] | boards[10] | boards[11];
	return (false);
}

void CastleUnitTest(uint64_t *boards)
{
	boards[0] = 0x00FF000000000000;
	boards[1] = 0;
	boards[2] = 0;
	boards[3] = (1llu << 56) | (1llu << 63);
	boards[4] = 0;
	boards[5] = (1ull << 60);
	boards[6] = 0x000000000000FF00;
	boards[7] = 0;
	boards[8] = 0;
	boards[9] = 1llu | ((1llu) << 7);;
	boards[10] = 0;
	boards[11] = (1ull << 4);
	boards[12] = boards[0] | boards[3] | boards[5];
	boards[13] = boards[6] | boards[9] | boards[11];
}

void PromotionTestBlack(uint64_t *boards)
{
	boards[0] = 0;
	boards[1] = 0;
	boards[2] = 0;
	boards[3] = 0;
	boards[4] = 0;
	boards[5] = 35184372088832llu;
	boards[6] = 281474976710656llu;
	boards[7] = 0;
	boards[8] = 0;
	boards[9] = 0;
	boards[10] = 0;
	boards[11] = 128llu;
	boards[12] = 35184372088832llu;
	boards[13] = 281474976710784llu;
}

void PromotionTestWhite(uint64_t *boards)
{
	boards[0] = 512llu;
	boards[1] = 0;
	boards[2] = 0;
	boards[3] = 0;
	boards[4] = 0;
	boards[5] = 9223372036854775808llu;
	boards[6] = 0;
	boards[7] = 0;
	boards[8] = 0;
	boards[9] = 0;
	boards[10] = 0;
	boards[11] = 64llu;
	boards[12] = 9223372036854776320llu;
	boards[13] = 64llu;
}
