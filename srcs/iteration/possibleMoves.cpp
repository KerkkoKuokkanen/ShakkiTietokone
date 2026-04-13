
#include <stdlib.h>
#include <stdio.h>
#include <bit>
#include "pieceMoves.h"

static uint32_t PackMove(uint8_t start, uint8_t end, uint8_t pieceType, uint8_t fullBoardIndex)
{
	return (uint32_t)start | ((uint32_t)end << 8) | ((uint32_t)pieceType << 16) | ((uint32_t)fullBoardIndex << 24);
}

//This function is for generating all the possible moves into an array to be looped in the pruning.
//It is messy but branching and extra function calls will make it slower.
void GenerateMovesWhite(uint32_t *moves, uint64_t pawns, uint64_t knights,
						uint64_t bishops, uint64_t rooks, uint64_t queens,
						uint64_t kings, uint64_t fBoard, uint64_t eBoard)
{
	uint8_t index = 0;
	while (pawns != 0)
	{
		uint8_t pInd = std::countr_zero(pawns);
		uint64_t move = GetAllPawnMovesWhite(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 0, 12);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		pawns &= ~(1ULL << pInd);
	}
	while (knights != 0)
	{
		uint8_t pInd = std::countr_zero(knights);
		uint64_t move = GetKnightMoves(fBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 1, 12);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		knights &= ~(1ULL << pInd);
	}
	while (bishops != 0)
	{
		uint8_t pInd = std::countr_zero(bishops);
		uint64_t move = GetBishopMoves(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 2, 12);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		bishops &= ~(1ULL << pInd);
	}
	while (rooks != 0)
	{
		uint8_t pInd = std::countr_zero(rooks);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 3, 12);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		rooks &= ~(1ULL << pInd);
	}
	while (queens != 0)
	{
		uint8_t pInd = std::countr_zero(queens);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd) | GetBishopMoves(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 4, 12);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		queens &= ~(1ULL << pInd);
	}
	while (kings != 0)
	{
		uint8_t pInd = std::countr_zero(kings);
		uint64_t move = GetKingMoves(fBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 5, 12);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		kings &= ~(1ULL << pInd);
	}
	moves[index] = 0;
}

void GenerateMovesBlack(uint32_t *moves, uint64_t pawns, uint64_t knights,
						uint64_t bishops, uint64_t rooks, uint64_t queens,
						uint64_t kings, uint64_t fBoard, uint64_t eBoard)
{
	uint8_t index = 0;
	while (pawns != 0)
	{
		uint8_t pInd = std::countr_zero(pawns);
		uint64_t move = GetAllPawnMovesBlack(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 0, 13);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		pawns &= ~(1ULL << pInd);
	}
	while (knights != 0)
	{
		uint8_t pInd = std::countr_zero(knights);
		uint64_t move = GetKnightMoves(fBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 1, 13);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		knights &= ~(1ULL << pInd);
	}
	while (bishops != 0)
	{
		uint8_t pInd = std::countr_zero(bishops);
		uint64_t move = GetBishopMoves(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 2, 13);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		bishops &= ~(1ULL << pInd);
	}
	while (rooks != 0)
	{
		uint8_t pInd = std::countr_zero(rooks);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 3, 13);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		rooks &= ~(1ULL << pInd);
	}
	while (queens != 0)
	{
		uint8_t pInd = std::countr_zero(queens);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd) | GetBishopMoves(fBoard, eBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 4, 13);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		queens &= ~(1ULL << pInd);
	}
	while (kings != 0)
	{
		uint8_t pInd = std::countr_zero(kings);
		uint64_t move = GetKingMoves(fBoard, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 5, 13);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		kings &= ~(1ULL << pInd);
	}
	moves[index] = 0;
}
