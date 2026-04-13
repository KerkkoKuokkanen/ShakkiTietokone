
#include "aiMove.h"
#include "posEvaluation.h"
#include "pieceMoves.h"
#include <math.h>
#include <stdio.h>

#define NO_CAPTURE 20

static bool IsCheckMate(uint64_t kb1, uint64_t kb2)
{
	if (kb1 == 0 || kb2 == 0)
		return (true);
	return (false);
}

static void UnMakeMove(uint32_t move, uint64_t *boards, uint8_t capture)
{
	uint8_t start = move & 0xFF;
	uint8_t end = (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xFF;
	uint8_t fAll = (move >> 24) & 0xFF;
		
	//Getting enemies position
	uint8_t eAll = fAll ^ 1;

	uint64_t fromBit = (1ULL << start);
	uint64_t toBit = (1ULL << end);

	//Moving piece back
	boards[pIdx] &= ~toBit;
	boards[pIdx] |= fromBit;

	//Also for all friendly pieces
	boards[fAll] &= ~toBit;
	boards[fAll] |= fromBit;

	if (capture == NO_CAPTURE)
		return ;
	//Restore captured if there was any
	boards[capture] |= toBit;
	boards[eAll] |= toBit;
}

static uint8_t MakeMove(uint32_t move, uint64_t *boards)
{
	uint8_t start = move & 0xFF;
	uint8_t end	= (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xFF;
	uint8_t fAll = (move >> 24) & 0xFF;

	//Getting enemy all index
	uint8_t eAll = fAll ^ 1; 
	uint64_t enemyAll = boards[eAll];
		
	//enemy starting index
	uint8_t eStart = (eAll - 12) * 6; 

	uint64_t toMask = (1ULL << end);
	uint64_t fromMaskNot = ~(1ULL << start);

	//Handle own board
	boards[pIdx] &= fromMaskNot;
	boards[pIdx] |= toMask;
		
	boards[fAll] &= fromMaskNot;
	boards[fAll] |= toMask;

	//Handle enemy boards
	boards[eAll] &= ~toMask;
	if (enemyAll == boards[eAll])
		return (NO_CAPTURE);

	//Saving the captured piece for unmakemove later
	for (uint8_t i = eStart; i < eStart + 6; i++)
	{
		if (boards[i] & (1ULL << end))
		{
			boards[i] &= ~toMask;
			return (i);
		}
	}
	return (NO_CAPTURE);
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
} */

static float AiTurn(uint64_t boards[14], uint8_t depth, float alpha, float beta, bool white)
{
	if (depth == 0 || IsCheckMate(boards[5], boards[11]))
		return (GetPositionScore(boards));
	uint32_t moves[256];
	if (white)
	{
		float maxEval = -9999999.0f;
		GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[12], boards[13]);
		uint8_t i = 0;
		while (moves[i] != 0)
		{
			uint8_t capture = MakeMove(moves[i], boards);
			float score = AiTurn(boards, depth - 1, alpha, beta, false);
			UnMakeMove(moves[i], boards, capture);

			maxEval = fmax(maxEval, score);
			alpha = fmax(alpha, score);
			if (beta <= alpha)
				break ;
			i++;
		}
		return (maxEval);
	}
	else
	{
		float minEval = 9999999.0f;
		GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[12], boards[13]);
		uint8_t i = 0;
		while (moves[i] != 0)
		{
			uint8_t capture = MakeMove(moves[i], boards);
			float score = AiTurn(boards, depth - 1, alpha, beta, true);
			UnMakeMove(moves[i], boards, capture);

			minEval = fmin(minEval, score);
			beta = fmin(beta, score);
			if (beta <= alpha)
				break ;
			i++;
		}
		return (minEval);
	}
}

uint16_t GetMove(uint64_t boards[14], bool white)
{
	AiTurn(boards, 8, -9999999.0f, 9999999.0f, white);
	return (1);
}
