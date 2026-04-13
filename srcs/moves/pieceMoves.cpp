
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <bit>

static uint64_t knightMoves[64];
static uint64_t kingMoves[64];

//slides go in a clockwise order starting from straight up
static uint64_t slides[8][64];

//0 for white and 1 for black
static uint64_t pawnAttacks[2][64];

uint64_t GetKnightMoves(uint64_t fBoard, int pos)
{
	//uint64_t targ = knightMoves[pos] & ~fBoard;
	//int i = std::countr_zero(targ);
	//printf("%d\n", i);
	return (knightMoves[pos] & ~fBoard);
}

/* static void PrinBoard(uint64_t board)
{
	for (int i = 0; i < 64; i++)
	{
		int bit = (board >> i) & 1ULL;
		if (i % 8 == 0)
			printf("\n");
		printf("%d ", bit);
	}
	printf("\n");
}
 */
uint64_t GetKingMoves(uint64_t fBoard, int pos)
{
	return (kingMoves[pos] & ~fBoard);
}

//indexes 1, 3, 5, 7 for bishops
uint64_t GetBishopMoves(uint64_t fBoard, uint64_t eBoard, int pos)
{
	uint64_t allPieces = fBoard | eBoard;
	uint64_t moves = 0;
	int posDirs[] = {1, 3};
	int negDirs[] = {5, 7};

	for (int dir : posDirs)
	{
		uint64_t ray = slides[dir][pos];
		uint64_t blockers = ray & allPieces;
		if (blockers)
		{
			int blockerIdx = std::countr_zero(blockers); 
			ray &= ~slides[dir][blockerIdx];
		}
		moves |= ray;
	}

	for (int dir : negDirs)
	{
		uint64_t ray = slides[dir][pos];
		uint64_t blockers = ray & allPieces;
		if (blockers)
		{
			int blockerIdx = 63 - std::countl_zero(blockers);
			ray &= ~slides[dir][blockerIdx];
		}
		moves |= ray;
	}
	return (moves & ~fBoard);
}

uint64_t GetRookMoves(uint64_t fBoard, uint64_t eBoard, int pos)
{
	uint64_t allPieces = fBoard | eBoard;
	uint64_t moves = 0;
	int posDirs[] = {2, 4};
	int negDirs[] = {0, 6};

	for (int dir : posDirs)
	{
		uint64_t ray = slides[dir][pos];
		uint64_t blockers = ray & allPieces;
		if (blockers)
		{
			int blockerIdx = std::countr_zero(blockers);
			ray &= ~slides[dir][blockerIdx];
		}
		moves |= ray;
	}

	for (int dir : negDirs)
	{
		uint64_t ray = slides[dir][pos];
		uint64_t blockers = ray & allPieces;
		if (blockers)
		{
			int blockerIdx = 63 - std::countl_zero(blockers);
			ray &= ~slides[dir][blockerIdx];
		}
		moves |= ray;
	}
	return (moves & ~fBoard);
}

static uint64_t GetPawnAttacks(uint64_t eBoard, int pos, int color)
{
	return (pawnAttacks[color][pos] & eBoard);
}

static uint64_t GetPawnPushesBlack(uint64_t allPieces, int pos)
{
	uint64_t moves = 0;
	uint64_t bit = (1ULL << pos);
	uint64_t singlePush = bit >> 8;

	if (!(singlePush & allPieces))
	{
		moves |= singlePush;
		if ((pos >= 48 && pos <= 55) && !((bit >> 16) & allPieces))
			moves |= (bit >> 16);
	}
	return (moves);
}

static uint64_t GetPawnPushesWhite(uint64_t allPieces, int pos)
{
	uint64_t moves = 0;
	uint64_t bit = (1ULL << pos);
	uint64_t singlePush = bit << 8;

	if (!(singlePush & allPieces))
	{
		moves |= singlePush;
		if ((pos >= 8 && pos <= 15) && !((bit << 16) & allPieces))
			moves |= (bit << 16);
	}
	return (moves);
}

uint64_t GetAllPawnMovesBlack(uint64_t fBoard, uint64_t eBoard, int pos)
{
	uint64_t allPieces = fBoard | eBoard;
	return (GetPawnAttacks(eBoard, pos, 1) | GetPawnPushesWhite(allPieces, pos));
}

uint64_t GetAllPawnMovesWhite(uint64_t fBoard, uint64_t eBoard, int pos)
{
	uint64_t allPieces = fBoard | eBoard;
	return (GetPawnAttacks(eBoard, pos, 0) | GetPawnPushesBlack(allPieces, pos));
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
