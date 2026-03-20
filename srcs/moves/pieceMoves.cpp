
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <bit>

static uint64_t knightMoves[64];
static uint64_t kingMoves[64];
static uint64_t slides[8][64];
static uint64_t pawnAttacks[2][64];

uint64_t GetKnightMoves(uint64_t fBoard, int pos)
{
	//uint64_t targ = knightMoves[pos] & ~fBoard;
	//int i = std::countr_zero(targ);
	//printf("%d\n", i);
	return (knightMoves[pos] & ~fBoard);
}

uint64_t GetKingMoves(uint64_t fBoard, int pos)
{
	return (kingMoves[pos] & ~fBoard);
}

void SetMoves(uint64_t knm[64], uint64_t km[64], uint64_t sl[8][64], uint64_t pa[2][64])
{
	memmove(knightMoves, knm, sizeof(uint64_t) * 64);
	memmove(kingMoves, km, sizeof(uint64_t) * 64);
	memmove(slides[0], sl[0], sizeof(uint64_t) * 64);
	memmove(slides[1], sl[1], sizeof(uint64_t) * 64);
	memmove(slides[2], sl[2], sizeof(uint64_t) * 64);
	memmove(slides[3], sl[3], sizeof(uint64_t) * 64);
	memmove(slides[4], sl[4], sizeof(uint64_t) * 64);
	memmove(slides[5], sl[5], sizeof(uint64_t) * 64);
	memmove(slides[6], sl[6], sizeof(uint64_t) * 64);
	memmove(slides[7], sl[7], sizeof(uint64_t) * 64);
	memmove(pawnAttacks[0], pa[0], sizeof(uint64_t) * 64);
	memmove(pawnAttacks[1], pa[1], sizeof(uint64_t) * 64);
}
