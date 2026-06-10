
#include <bit>

static const uint64_t rims = 0x8181818181818181ull;
static const uint64_t firstAndLastRow = 0xFF000000000000FF;
static const uint64_t columnMask[8] = {
	0x0101010101010101ULL, 0x0202020202020202ULL, 0x0404040404040404ULL, 0x0808080808080808ULL,
	0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL
};

static int GetPositionScoreS(uint64_t pawns, uint64_t knights, uint64_t bishops, uint64_t rooks, uint64_t queens)
{
	int32_t score = 0;
	score += std::popcount(pawns) * 100;
	score += std::popcount(knights) * 300;
	score += std::popcount(bishops) * 320;
	score += std::popcount(rooks) * 500;
	score += std::popcount(queens) * 900;
	return (score);
}
//1120

static int GetPawnCenterBonus(uint64_t pawns)
{
	static const uint64_t mask = (1ull << 27) | (1ull << 28) | (1ull << 35) | (1ull << 36);
	return (std::popcount(mask & pawns) * 15);
}

static int GetDoubledPawnMinus(uint64_t pawns)
{
	int penaltyCount = 0;
	for (int i = 0; i < 8; i++)
	{
		int count = std::popcount(pawns & columnMask[i]);
		if (count > 1) 
			penaltyCount += (count - 1);
	}
	return (penaltyCount * 50);
}

static int GetKnightRimMinus(uint64_t knights)
{
	uint64_t kr = knights & rims;
	return (std::popcount(kr) * 100);
}

static int PiecesActive(uint64_t knigth, uint64_t bishops)
{
	uint64_t combined = knigth | bishops;
	int count = std::popcount(combined & firstAndLastRow);
	return (count * 40);
}

#include "stdio.h"

//simple board evaluation based on piece values
int GetPositionScore(uint64_t *boards)
{
	int s1 = GetPositionScoreS(boards[0], boards[1], boards[2], boards[3], boards[4]);
	int s2 = GetPositionScoreS(boards[6], boards[7], boards[8], boards[9], boards[10]);
	s1 += GetPawnCenterBonus(boards[0]);
	s2 += GetPawnCenterBonus(boards[6]);
	s1 -= GetDoubledPawnMinus(boards[0]);
	s2 -= GetDoubledPawnMinus(boards[6]);
	s1 -= GetKnightRimMinus(boards[1]);
	s2 -= GetKnightRimMinus(boards[7]);
	s1 -= PiecesActive(boards[1], boards[2]);
	s2 -= PiecesActive(boards[7], boards[8]);
	return (s1 - s2);
}
