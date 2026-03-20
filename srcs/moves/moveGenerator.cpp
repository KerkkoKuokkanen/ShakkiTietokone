
#include <stdlib.h>
#include <stdio.h>
#include "pieceMoves.h"

static uint64_t knightMoves[64];
static uint64_t kingMoves[64];
static uint64_t slides[8][64];
static uint64_t pawnAttacks[2][64];

void PrinBoard(uint64_t board)
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

static uint64_t GenerateKnighMoveFromPosition(int i)
{
	uint64_t ret = 0;
	int x = i % 8;
	int y = i / 8;
	int pos1 = i - 8 - 8 - 1;
	int pos2 = i - 8 - 8 + 1;
	int pos3 = i - 8 - 2;
	int pos4 = i - 8 + 2;
	int pos5 = i + 8 - 2;
	int pos6 = i + 8 + 2;
	int pos7 = i + 8 + 8 - 1;
	int pos8 = i + 8 + 8 + 1;
	if (x >= 1 && y >= 2)
		ret |= 1LLU << pos1;
	if (x < 7 && y >= 2)
		ret |= 1LLU << pos2;
	if (x >= 2 && y >= 1)
		ret |= 1LLU << pos3;
	if (x < 6 && y >= 1)
		ret |= 1LLU << pos4;
	if (x >= 2 && y < 7)
		ret |= 1LLU << pos5;
	if (x < 6 && y < 7)
		ret |= 1LLU << pos6;
	if (x >= 1 && y < 6)
		ret |= 1LLU << pos7;
	if (x < 7 && y < 6)
		ret |= 1LLU << pos8;
	return (ret);
}

void GenerateKnighMoves()
{
	for (int i = 0; i < 64; i++)
		knightMoves[i] = GenerateKnighMoveFromPosition(i);
}

static uint64_t GenerateKingMoveFromPosition(int i)
{
	uint64_t ret = 0;
	int x = i % 8;
	int y = i / 8;
	int pos1 = i - 8 - 1;
	int pos2 = i - 8;
	int pos3 = i - 8 + 1;
	int pos4 = i - 1;
	int pos5 = i + 1;
	int pos6 = i + 8 - 1;
	int pos7 = i + 8;
	int pos8 = i + 8 + 1;
	if (x >= 1 && y >= 1)
		ret |= 1LLU << pos1;
	if (y >= 1)
		ret |= 1LLU << pos2;
	if (x < 7 && y >= 1)
		ret |= 1LLU << pos3;
	if (x >= 1)
		ret |= 1LLU << pos4;
	if (x < 7)
		ret |= 1LLU << pos5;
	if (x >= 1 && y < 7)
		ret |= 1LLU << pos6;
	if (y < 7)
		ret |= 1LLU << pos7;
	if (x < 7 && y < 7)
		ret |= 1LLU << pos8;
	return (ret);
}

void GenerateKingMoves()
{
	for (int i = 0; i < 64; i++)
		kingMoves[i] = GenerateKingMoveFromPosition(i);
}

static void GenerateSlidesFromPosition(int i)
{
	int x = i % 8;
	int y = i / 8;
	for (int j = y; j > 0; j--)
		slides[0][i] |= 1LLU << (i - 8 * j);
	int ty = 1;
	int tx = x;
	while (ty <= y && tx < 7)
	{
		slides[1][i] |= 1LLU << (i - 8 * ty + 1 + (tx - x));
		ty += 1;
		tx += 1;
	}
	for (int j = x; j < 7; j++)
		slides[2][i] |= 1LLU << (i + 1 + (j - x));
	ty = y;
	tx = x;
	while (ty < 7 && tx < 7)
	{
		slides[3][i] |= 1LLU << (i + (8 * (1 + ty - y)) + 1 + (tx - x));
		ty += 1;
		tx += 1;
	}
	for (int j = y; j < 7; j++)
		slides[4][i] |= 1LLU << (i + (8 * (1 + j - y)));
	ty = y;
	tx = 1;
	while (ty < 7 && tx <= x)
	{
		slides[5][i] |= 1LLU << (i - tx + (8 * (1 + ty - y)));
		ty++;
		tx++;
	}
	for (int j = 1; j <= x; j++)
		slides[6][i] |= 1LLU << (i - j);
	ty = 1;
	tx = 1;
	while (ty <= y && tx <= x)
	{
		slides[7][i] |= 1LLU << (i - tx - 8 * ty);
		tx++;
		ty++;
	}
}

void GenerateSlides()
{
	for (int i = 0; i < 64; i++)
		GenerateSlidesFromPosition(i);
}

static uint64_t PawnAttackWhite(int i)
{
	uint64_t ret = 0;
	int x = i % 8;
	int y = i / 8;
	int pos1 = i - 8 - 1;
	int pos2 = i - 8 + 1;
	if (x > 0 && y > 0)
		ret |= 1LLU << (pos1);
	if (x < 7 && y > 0)
		ret |= 1LLU << (pos2);
	return (ret);
}

static uint64_t PawnAttackBlack(int i)
{
	uint64_t ret = 0;
	int x = i % 8;
	int y = i / 8;
	int pos1 = i + 8 - 1;
	int pos2 = i + 8 + 1;
	if (x > 0 && y < 7)
		ret |= 1LLU << (pos1);
	if (x < 7 && y < 7)
		ret |= 1LLU << (pos2);
	return (ret);
}

void GeneratePawnAttacks()
{
	for (int i = 0; i < 64; i++)
	{
		pawnAttacks[0][i] = PawnAttackWhite(i);
		pawnAttacks[1][i] = PawnAttackBlack(i);
	}
	SetMoves(knightMoves, kingMoves, slides, pawnAttacks);
}

//bitboard |= 1ULL << i;
//bitboard &= 0ULL << i;
