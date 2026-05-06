
#include <stdlib.h>
#include "checksAndPins.h"
#include "gameBoard.h"

//Mask for making the casting move
//When we make a move we apply this mask to the rights so we know if we
//are still allowed to castle
const uint8_t CastlingMask[64] = {
	7, 15, 15, 15, 3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14
};

static uint8_t castleRights = 15;

//Magic numbers to check if castilng is allowed
const uint8_t wKingSide = 1;
const uint8_t wQueenSide = 2;
const uint8_t bKingSide = 4;
const uint8_t bQueenSide = 8;

static uint32_t PackMove(uint8_t start, uint8_t end, uint8_t pieceType, uint8_t fullBoardIndex, uint8_t endType)
{
	return (uint32_t)start |
			((uint32_t)end << 8) |
			((uint32_t)pieceType << 16) |
			((uint32_t)endType << 20) |
			((uint32_t)fullBoardIndex << 24);
}

void SetCastleRights(uint8_t *oldRights, uint8_t start, uint8_t end)
{
	*oldRights = castleRights;
	castleRights &= CastlingMask[start];
	castleRights &= CastlingMask[end];
}

void ResetCastleRights(uint8_t oldRights)
{
	castleRights = oldRights;
}

void UnCastleTheKing(uint64_t *boards, uint8_t parameter)
{
	if (parameter == 100)
	{
		boards[5] |= (1ull << 60);
		boards[5] &= ~(1ull << 62);
		boards[3] |= (1ull << 63);
		boards[3] &= ~(1ull << 61);
		boards[12] |= ((1ull << 60) | (1ull << 63));
		boards[12] &= ~(1ull << 62) | (1ull << 61);
	}
	else if (parameter == 101)
	{
		boards[5] |= (1ull << 60);
		boards[5] &= ~(1ull << 58);
		boards[3] |= (1ull << 56);
		boards[3] &= ~(1ull << 59);
		boards[12] |= ((1ull << 60) | (1ull << 56));
		boards[12] &= ~(1ull << 58) | (1ull << 59);
	}
	else if (parameter == 102)
	{
		boards[11] |= (1ull << 4);
		boards[11] &= ~(1ull << 6);
		boards[9] |= (1ull << 7);
		boards[9] &= ~(1ull << 5);
		boards[13] |= ((1ull << 4) | (1ull << 7));
		boards[13] &= ~(1ull << 6) | (1ull << 5);
	}
	else
	{
		boards[11] |= (1ull << 4);
		boards[11] &= ~(1ull << 2);
		boards[9] |= (1ull);
		boards[9] &= ~(1ull << 3);
		boards[13] |= ((1ull << 4) | (1ull));
		boards[13] &= ~(1ull << 2) | (1ull << 3);
	}
}

void CastleTheKing(uint64_t *boards, uint8_t parameter, uint8_t *oldRights)
{
	if (parameter == 100)
	{
		boards[5] &= ~(1ull << 60);
		boards[5] |= (1ull << 62);
		boards[3] &= ~(1ull << 63);
		boards[3] |= (1ull << 61);
		boards[12] &= ~((1ull << 60) | (1ull << 63));
		boards[12] |= (1ull << 62) | (1ull << 61);
		*oldRights = castleRights;
		castleRights &= 12;
	}
	else if (parameter == 101)
	{
		boards[5] &= ~(1ull << 60);
		boards[5] |= (1ull << 58);
		boards[3] &= ~(1ull << 56);
		boards[3] |= (1ull << 59);
		boards[12] &= ~((1ull << 60) | (1ull << 56));
		boards[12] |= (1ull << 58) | (1ull << 59);
		*oldRights = castleRights;
		castleRights &= 12;
	}
	else if (parameter == 102)
	{
		boards[11] &= ~(1ull << 4);
		boards[11] |= (1ull << 6);
		boards[9] &= ~(1ull << 7);
		boards[9] |= (1ull << 5);
		boards[13] &= ~((1ull << 4) | (1ull << 7));
		boards[13] |= (1ull << 6) | (1ull << 5);
		*oldRights = castleRights;
		castleRights &= 3;
	}
	else
	{
		boards[11] &= ~(1ull << 4);
		boards[11] |= (1ull << 2);
		boards[9] &= ~(1ull);
		boards[9] |= (1ull << 3);
		boards[13] &= ~((1ull << 4) | (1ull));
		boards[13] |= (1ull << 2) | (1ull << 3);
		*oldRights = castleRights;
		castleRights &= 3;
	}
}

void CastlingWhite(uint64_t *boards, uint32_t *moves, uint8_t *index)
{
	static const uint64_t occupancyMaskRight = (1ull << 60) | (1ull << 61) | (1ull << 62) | (1ull << 63);
	static const uint64_t occupancyMaskLeft = (1ull << 60) | (1ull << 59) | (1ull << 58) | (1ull << 57) | (1ull << 56);

	if (wKingSide & castleRights)
	{
		if ((occupancyMaskRight & boards[12]) == 10376293541461622784ull)
		{
			if (!(WhiteKingInCheck(60, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12] & ~boards[5], boards[13]))
				&& !(WhiteKingInCheck(61, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12] & ~boards[5], boards[13]))
				&& !(WhiteKingInCheck(62, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12] & ~boards[5], boards[13])))
			{
				//I use 100 as a magic number to tell the program the we are castling
				//100 white king side
				//101 white queen side
				//102 black king side
				//103 black queen side
				moves[*index] = PackMove(100, 0, 0, 0, 0);
				*index += 1;
			}
		}
	}
	if (wQueenSide & castleRights)
	{
		if ((occupancyMaskLeft & boards[12]) == 1224979098644774912ull)
		{
			if (!(WhiteKingInCheck(60, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12] & ~boards[5], boards[13]))
				&& !(WhiteKingInCheck(59, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12] & ~boards[5], boards[13]))
				&& !(WhiteKingInCheck(58, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12] & ~boards[5], boards[13])))
			{
				moves[*index] = PackMove(101, 0, 0, 0, 0);
				*index += 1;
			}
		}
	}
}

void CastlingBlack(uint64_t *boards, uint32_t *moves, uint8_t *index)
{
	static const uint64_t occupancyMaskRight = (1ull << 4) | (1ull << 5) | (1ull << 6) | (1ull << 7);
	static const uint64_t occupancyMaskLeft = (1ull) | (1ull << 1) | (1ull << 2) | (1ull << 3) | (1ull << 4);

	if (bKingSide & castleRights)
	{
		if ((occupancyMaskRight & boards[13]) == 144ull)
		{
			if (!(BlackKingCheckers(4, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[13] & ~boards[11], boards[12]))
				&& !(BlackKingCheckers(5, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[13] & ~boards[11], boards[12]))
				&& !(BlackKingCheckers(6, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[13] & ~boards[11], boards[12])))
			{
				moves[*index] = PackMove(102, 0, 0, 0, 0);
				*index += 1;
			}
		}
	}
	if (bQueenSide & castleRights)
	{
		if ((occupancyMaskLeft & boards[13]) == 17ull)
		{
			if (!(BlackKingCheckers(4, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[13] & ~boards[11], boards[12]))
				&& !(BlackKingCheckers(3, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[13] & ~boards[11], boards[12]))
				&& !(BlackKingCheckers(2, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[13] & ~boards[11], boards[12])))
			{
				moves[*index] = PackMove(103, 0, 0, 0, 0);
				*index += 1;
			}
		}
	}
}
