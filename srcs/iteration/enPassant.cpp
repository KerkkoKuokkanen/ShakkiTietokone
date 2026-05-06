
#include <stdlib.h>
#include "checksAndPins.h"

#define NOT_POSSIBLE 100

static int8_t enPassantSquare = NOT_POSSIBLE;

static uint32_t PackMove(uint8_t start, uint8_t end, uint8_t pieceType, uint8_t fullBoardIndex, uint8_t endType)
{
	return (uint32_t)start |
			((uint32_t)end << 8) |
			((uint32_t)pieceType << 16) |
			((uint32_t)endType << 20) |
			((uint32_t)fullBoardIndex << 24);
}

void SetEnPassantSquare(int8_t square)
{
	enPassantSquare = square;
}

int8_t GetEnPassantSquare()
{
	return (enPassantSquare);
}

static bool IsLegalWhite(uint64_t *boards, uint8_t pInd, uint8_t kingPos)
{
	uint64_t pawns = boards[0];
	uint64_t noPawn = pawns & ~(1ull << pInd);

	//We create the new pawn board with the enPassant move for white
	pawns = noPawn | (1ull << enPassantSquare);

	//Same for black
	uint64_t pawnRemovedBlack = boards[6] & ~(1ull << (enPassantSquare - 8));
	uint64_t fullBoard = (boards[12] & ~(boards[0])) | pawns;
	uint64_t fullBoardBlack = (boards[13] & ~(boards[6])) | pawnRemovedBlack;

	return (!WhiteKingInCheck(kingPos, pawnRemovedBlack, boards[7], boards[8], boards[9], boards[10], boards[11], fullBoard, fullBoardBlack));
}

static bool IsLegalBlack(uint64_t *boards, uint8_t pInd, uint8_t kingPos)
{
	uint64_t pawns = boards[6];
	uint64_t noPawn = pawns & ~(1ull << pInd);

	//We create the new pawn board with the enPassant move for white
	pawns = noPawn | (1ull << enPassantSquare);

	//Same for black
	uint64_t pawnRemovedWhite = boards[0] & ~(1ull << (enPassantSquare + 8));
	uint64_t fullBoard = (boards[13] & ~(boards[6])) | pawns;
	uint64_t fullBoardWhite = (boards[12] & ~(boards[0])) | pawnRemovedWhite;

	return (!BlackKingInCheck(kingPos, pawnRemovedWhite, boards[1], boards[2], boards[3], boards[4], boards[5], fullBoard, fullBoardWhite));
}

#include "stdio.h"

void GenerateEnPassantWhite(uint32_t *moves, uint8_t *index, uint8_t pInd, uint64_t *boards, uint8_t kingPos)
{
	if (enPassantSquare == NOT_POSSIBLE)
		return;

	if (pInd < 24 || pInd > 31) 
		return;

	int8_t pawnCol = pInd % 8;
	int8_t targetCol = enPassantSquare % 8;

	if (abs(pawnCol - targetCol) != 1)
		return;

	int8_t diff = pInd - enPassantSquare;
	if (diff == 7 || diff == 9)
	{
		if (IsLegalWhite(boards, pInd, kingPos))
		{
			moves[*index] = PackMove(pInd, enPassantSquare, 0, 12, 14);
			*index += 1;
		}
	}
}
void GenerateEnPassantBlack(uint32_t *moves, uint8_t *index, uint8_t pInd, uint64_t *boards, uint8_t kingPos)
{
	if (enPassantSquare == NOT_POSSIBLE)
		return;

	if (pInd < 32 || pInd > 39)
		return;

	int8_t pawnCol = pInd % 8;
	int8_t targetCol = enPassantSquare % 8;

	if (abs(pawnCol - targetCol) != 1)
		return;

	int8_t diff = enPassantSquare - pInd;
	if (diff == 7 || diff == 9)
	{
		if (IsLegalBlack(boards, pInd, kingPos))
		{
			moves[*index] = PackMove(pInd, enPassantSquare, 6, 13, 14);
			*index += 1;
		}
	}
}
