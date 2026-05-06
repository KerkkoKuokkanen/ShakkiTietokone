
#include "checksAndPins.h"
#include <bit>
#include "pieceMoves.h"
#include "castle.h"
#include "enPassant.h"

//Packs the move into one 32 bit variable
static uint32_t PackMove(uint8_t start, uint8_t end, uint8_t pieceType, uint8_t fullBoardIndex, uint8_t endType)
{
	// bits 0-7:   Start Square
	// bits 8-15:  End Square
	// bits 16-19: Piece Type (4 bits)
	// bits 20-23: Ending board (4 bits: 0-10, 15 for none)
	// bits 24-31: Full Board Index
		
	return (uint32_t)start |
			((uint32_t)end << 8) |
			((uint32_t)pieceType << 16) |
			((uint32_t)endType << 20) |
			((uint32_t)fullBoardIndex << 24);
}

static void PackPawnMoveBlack(uint32_t *moves, uint8_t *index, uint8_t start, uint8_t end)
{
	if ((1ull << end) & 0xFF00000000000000)
	{
		moves[*index] = PackMove(start, end, 6, 13, 7);
		moves[*index + 1] = PackMove(start, end, 6, 13, 8);
		moves[*index + 2] = PackMove(start, end, 6, 13, 9);
		moves[*index + 3] = PackMove(start, end, 6, 13, 10);
		*index += 4;
	}
	moves[*index] = PackMove(start, end, 6, 13, 6);
	*index += 1;
}

static void PackPawnMoveWhite(uint32_t *moves, uint8_t *index, uint8_t start, uint8_t end)
{
	if ((1ull << end) & 0xFF)
	{
		moves[*index] = PackMove(start, end, 0, 12, 1);
		moves[*index + 1] = PackMove(start, end, 0, 12, 2);
		moves[*index + 2] = PackMove(start, end, 0, 12, 3);
		moves[*index + 3] = PackMove(start, end, 0, 12, 4);
		*index += 4;
		return ;
	}
	moves[*index] = PackMove(start, end, 0, 12, 0);
	*index += 1;
}

//This function is for generating all the possible moves into an array to be looped in the pruning.
//It is messy but branching and extra function calls will make it slower.
//Having these two functions separate gives me the possibility to not use extra if statements that slow the recursion down
//This function returns a value 0 if game continues.
//1 if the game has ended in a stalemate
//2 if the game has ended in a checkmate
uint8_t GenerateMovesWhite(uint32_t *moves, uint64_t pawns, uint64_t knights,
						uint64_t bishops, uint64_t rooks, uint64_t queens,
						uint64_t kings, uint64_t fBoard, uint64_t eBoard, uint64_t *allBoards)
{
	uint8_t ret = 0;
	uint8_t index = 0;
	uint8_t kingPos = std::countr_zero(kings);
	uint64_t pinningMask = PinnedMask(kingPos, allBoards[8], allBoards[9], allBoards[10], fBoard, eBoard);
	uint64_t whiteKingChecks = WhiteKingCheckers(kingPos, allBoards[6], allBoards[7], allBoards[8], allBoards[9],
														allBoards[10], allBoards[11], allBoards[12], allBoards[13]);
	if (whiteKingChecks != 0)
		ret = 1;
	uint64_t checkMask = GetCheckMask(kingPos, whiteKingChecks, allBoards[8], allBoards[9], allBoards[10]);
	while (pawns != 0)
	{
		uint8_t pInd = std::countr_zero(pawns);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetAllPawnMovesWhite(fBoard, eBoard, pInd);
		GenerateEnPassantWhite(moves, &index, pInd, allBoards, kingPos);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			PackPawnMoveWhite(moves, &index, pInd, mInd);
			move &= ~(1ULL << mInd);
		}
		pawns &= ~(1ULL << pInd);
	}

	while (knights != 0)
	{
		uint8_t pInd = std::countr_zero(knights);
		uint64_t pieceBit = (1ULL << pInd);
		if (pieceBit & pinningMask)
		{
			knights &= ~(1ULL << pInd);
			continue ;
		}
		uint64_t move = GetKnightMoves(fBoard, pInd);
		move &= checkMask;
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 1, 12, 1);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		knights &= ~(1ULL << pInd);
	}

	while (bishops != 0)
	{
		uint8_t pInd = std::countr_zero(bishops);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetBishopMoves(fBoard, eBoard, pInd);
		move &= checkMask;
		//Checks if the pieceIsPinned
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 2, 12, 2);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		bishops &= ~(1ULL << pInd);
	}

	while (rooks != 0)
	{
		uint8_t pInd = std::countr_zero(rooks);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 3, 12, 3);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		rooks &= ~(1ULL << pInd);
	}

	while (queens != 0)
	{
		uint8_t pInd = std::countr_zero(queens);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd) | GetBishopMoves(fBoard, eBoard, pInd);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 4, 12, 4);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		queens &= ~(1ULL << pInd);
	}
	CastlingWhite(allBoards, moves, &index);
	uint64_t move = GetKingMoves(fBoard, kingPos);
	while (move != 0)
	{
		uint8_t mInd = std::countr_zero(move);
		move &= ~(1ULL << mInd);
		if (WhiteKingInCheck(mInd, allBoards[6], allBoards[7], allBoards[8], allBoards[9],
								allBoards[10], allBoards[11], allBoards[12] & ~kings, allBoards[13]))
			continue ;
		moves[index] = PackMove(kingPos, mInd, 5, 12, 5);
		index += 1;
	}
	moves[index] = 0;
	if (moves[0] == 0)
		return (ret + 1);
	return (0);
}

//Same function but for black pieces
uint8_t GenerateMovesBlack(uint32_t *moves, uint64_t pawns, uint64_t knights,
						uint64_t bishops, uint64_t rooks, uint64_t queens,
						uint64_t kings, uint64_t fBoard, uint64_t eBoard, uint64_t *allBoards)
{
	uint8_t ret = 0;
	uint8_t index = 0;
	uint8_t kingPos = std::countr_zero(kings);
	uint64_t pinningMask = PinnedMask(kingPos, allBoards[2], allBoards[3], allBoards[4], allBoards[13], allBoards[12]);
	uint64_t blackKingChecks = BlackKingCheckers(kingPos, allBoards[0], allBoards[1], allBoards[2], allBoards[3],
														allBoards[4], allBoards[5], allBoards[13], allBoards[12]);
	if (blackKingChecks != 0)
		ret = 1;
	uint64_t checkMask = GetCheckMask(kingPos, blackKingChecks, allBoards[2], allBoards[3], allBoards[4]);
	while (pawns != 0)
	{
		uint8_t pInd = std::countr_zero(pawns);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetAllPawnMovesBlack(fBoard, eBoard, pInd);
		GenerateEnPassantBlack(moves, &index, pInd, allBoards, kingPos);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			PackPawnMoveBlack(moves, &index, pInd, mInd);
			move &= ~(1ULL << mInd);
		}
		pawns &= ~(1ULL << pInd);
	}
	while (knights != 0)
	{
		uint8_t pInd = std::countr_zero(knights);
		uint64_t pieceBit = (1ULL << pInd);
		if (pieceBit & pinningMask)
		{
			knights &= ~(1ULL << pInd);
			continue ;
		}
		uint64_t move = GetKnightMoves(fBoard, pInd);
		move &= checkMask;
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 7, 13, 7);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		knights &= ~(1ULL << pInd);
	}
	while (bishops != 0)
	{
		uint8_t pInd = std::countr_zero(bishops);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetBishopMoves(fBoard, eBoard, pInd);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 8, 13, 8);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		bishops &= ~(1ULL << pInd);
	}
	while (rooks != 0)
	{
		uint8_t pInd = std::countr_zero(rooks);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 9, 13, 9);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		rooks &= ~(1ULL << pInd);
	}
	while (queens != 0)
	{
		uint8_t pInd = std::countr_zero(queens);
		uint64_t pieceBit = (1ULL << pInd);
		uint64_t move = GetRookMoves(fBoard, eBoard, pInd) | GetBishopMoves(fBoard, eBoard, pInd);
		move &= checkMask;
		if (pieceBit & pinningMask)
			move &= GetLineMask(kingPos, pInd);
		while (move != 0)
		{
			uint8_t mInd = std::countr_zero(move);
			moves[index] = PackMove(pInd, mInd, 10, 13, 10);
			move &= ~(1ULL << mInd);
			index += 1;
		}
		queens &= ~(1ULL << pInd);
	}
	CastlingBlack(allBoards, moves, &index);
	uint64_t move = GetKingMoves(fBoard, kingPos);
	while (move != 0)
	{
		uint8_t mInd = std::countr_zero(move);
		move &= ~(1ULL << mInd);
		if (BlackKingInCheck(mInd, allBoards[0], allBoards[1], allBoards[2], allBoards[3],
								allBoards[4], allBoards[5], allBoards[13] & ~kings, allBoards[12]))
			continue ;
		moves[index] = PackMove(kingPos, mInd, 11, 13, 11);
		index += 1;
	}
	moves[index] = 0;
	if (moves[0] == 0)
		return (ret + 1);
	return (0);
}
