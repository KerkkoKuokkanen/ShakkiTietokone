
#include <stdlib.h>
#include <bit>
#include "pieceMoves.h"

uint64_t BlackKingCheckers(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard)
{
	uint64_t pawn = GetPawnAttacks(eBoard, pos, 1);
	uint64_t knight = GetKnightMoves(fBoard, pos);
	uint64_t bishop = GetBishopMoves(fBoard, eBoard, pos);
	uint64_t rook = GetRookMoves(fBoard, eBoard, pos);
	uint64_t queen = bishop | rook;
	uint64_t king = GetKingMoves(fBoard, pos);
	return ((pawn & ePawns) | (knight & eKnights) | (bishop & eBishops) | (rook & eRooks) | (queen & eQueens) | (king & eKings));
}

uint64_t WhiteKingCheckers(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard)
{
	uint64_t pawn = GetPawnAttacks(eBoard, pos, 0);
	uint64_t knight = GetKnightMoves(fBoard, pos);
	uint64_t bishop = GetBishopMoves(fBoard, eBoard, pos);
	uint64_t rook = GetRookMoves(fBoard, eBoard, pos);
	uint64_t queen = bishop | rook;
	uint64_t king = GetKingMoves(fBoard, pos);
	return ((pawn & ePawns) | (knight & eKnights) | (bishop & eBishops) | (rook & eRooks) | (queen & eQueens) | (king & eKings));
}

uint64_t GetCheckMask(uint8_t kingPos, uint64_t checkers, uint64_t eBishops, uint64_t eRooks, uint64_t eQueens)
{
	if (checkers == 0)
		return (0xFFFFFFFFFFFFFFFF);

	//If double check then no piece but king can try to move
	if (std::popcount(checkers) > 1)
		return (0);

	uint8_t checkerSq = std::countr_zero(checkers);
	uint64_t checkerBit = (1ULL << checkerSq);

	if (checkerBit & eBishops || checkerBit & eRooks || checkerBit & eQueens)
		return (checkerBit | GetRayBetween(checkerSq, kingPos));

	return (checkerBit);
}

bool BlackKingInCheck(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard)
{
	uint64_t pawn = GetPawnAttacks(eBoard, pos, 1);
	uint64_t knight = GetKnightMoves(fBoard, pos);
	uint64_t bishop = GetBishopMoves(fBoard, eBoard, pos);
	uint64_t rook = GetRookMoves(fBoard, eBoard, pos);
	uint64_t queen = bishop | rook;
	uint64_t king = GetKingMoves(fBoard, pos);
	if (pawn & ePawns || knight & eKnights || bishop & eBishops || rook & eRooks || queen & eQueens || king & eKings)
		return (true);
	return (false);
}

bool WhiteKingInCheck(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard)
{
	uint64_t pawn = GetPawnAttacks(eBoard, pos, 0);
	uint64_t knight = GetKnightMoves(fBoard, pos);
	uint64_t bishop = GetBishopMoves(fBoard, eBoard, pos);
	uint64_t rook = GetRookMoves(fBoard, eBoard, pos);
	uint64_t queen = bishop | rook;
	uint64_t king = GetKingMoves(fBoard, pos);
	if (pawn & ePawns || knight & eKnights || bishop & eBishops || rook & eRooks || queen & eQueens || king & eKings)
		return (true);
	return (false);
}

//Creates a mask of pieces that are currently pinned to the king.
uint64_t PinnedMask(uint8_t kingPos, uint64_t eBishops, uint64_t eRooks, uint64_t eQueens, uint64_t fBoard, uint64_t eBoard)
{
	uint64_t mask = 0;
	uint64_t combinedOccupancy = fBoard | eBoard;
	uint64_t orthogonalAttackers = eRooks | eQueens;
	uint64_t diagonalAttackers = eBishops | eQueens;

	for (uint8_t i = 0; i < 8; i++)
	{
		uint64_t ray;
		if (i % 2 == 0)
		{
			ray = GetRookSingleRayPinBoard(orthogonalAttackers, kingPos, i);
			if (ray & orthogonalAttackers)	//Checks if the ray intersects with rooks or queens
			{
				uint64_t blockers = ray & combinedOccupancy;
				if (std::popcount(blockers) == 2 && (blockers & fBoard))	//Check if there is only one piece and its firendly
					mask |= (blockers & fBoard);
			}
		}
		else
		{
			ray = GetBighopSingleRayPinBoard(diagonalAttackers, kingPos, i);
			if (ray & diagonalAttackers)	//bishops or queens
			{
				uint64_t blockers = ray & combinedOccupancy;
				if (std::popcount(blockers) == 2 && (blockers & fBoard))
					mask |= (blockers & fBoard);
			}
		}
	}
	return (mask);
}
