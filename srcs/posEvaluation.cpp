
#include <bit>

static uint16_t GetPositionScoreS(uint64_t pawns, uint64_t knights, uint64_t bishops, uint64_t rooks, uint64_t queens, uint64_t king)
{
	uint16_t score = 0;
	if (king != 0)
		score += 5000;
	score += std::popcount(pawns);
	score += std::popcount(knights | bishops) * 3;
	score += std::popcount(queens) * 9;
	score += std::popcount(rooks) * 5;
	return (score);
}

//simple board evaluation based on piece values
float GetPositionScore(uint64_t *boards)
{
	uint16_t s1 = GetPositionScoreS(boards[0], boards[1], boards[2], boards[3], boards[4], boards[5]);
	uint16_t s2 = GetPositionScoreS(boards[6], boards[7], boards[8], boards[9], boards[10], boards[11]);
	return (float)(s1 - s2);
}
