
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
	boards[6] = 0x0000000804000000;		//Black pawns!
	boards[7] = 0x0000000000200000;		//Black knights!
	boards[8] = 0;		//Black bishops
	boards[9] = 0x0000000000002000;		//Black rooks!
	boards[10] = 0;	//Black queen
	boards[11] = 0x0000000000000040;	//Black king!
	boards[12] = 0x00000C0800904000;	//White all pieces!
	boards[13] = 0x0000000824242040;	//Black all pieces!
	return (true);
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
