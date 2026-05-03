
#include <bit>

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

//simple board evaluation based on piece values
float GetPositionScore(uint64_t *boards)
{
	int s1 = GetPositionScoreS(boards[0], boards[1], boards[2], boards[3], boards[4]);
	int s2 = GetPositionScoreS(boards[6], boards[7], boards[8], boards[9], boards[10]);
	return (float)(s1 - s2);
}
