
#include "aiMove.h"
#include "posEvaluation.h"
#include "pieceMoves.h"
#include <math.h>
#include "castle.h"
#include "gameBoard.h"
#include <stdio.h>
#include "enPassant.h"
#include <algorithm>

#define NO_CAPTURE 20
#define DEPTH 5

static bool gameOver = false;

bool GetGameOver()
{
	return (gameOver);
}

//Unmakes the already made move so that the board stays clean and usable
static void UnMakeMove(uint32_t move, uint64_t *boards, uint8_t capture, uint8_t castleRights)
{
	uint8_t start = move & 0xFF;
	if (start >= 100)
	{
		UnCastleTheKing(boards, start);
		ResetCastleRights(castleRights);
		return ;
	}

	uint8_t end = (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xF;
	uint8_t endType = (move >> 20) & 0xF;
	uint8_t fAll = (move >> 24) & 0xFF;

	if (endType == 14)
	{
		uint8_t victimSquare = (fAll == 12) ? end + 8 : end - 8;

		boards[pIdx] &= ~(1ull << end);
		boards[pIdx] |= (1ull << start);
		boards[fAll] &= ~(1ull << end);
		boards[fAll] |= (1ull << start);

		boards[capture] |= (1ull << victimSquare);
		boards[fAll ^ 1] |= (1ull << victimSquare);
		return ;
	}

	//Getting enemies position
	uint8_t eAll = fAll ^ 1;

	uint64_t fromBit = (1ULL << start);
	uint64_t toBit = (1ULL << end);

	//Moving piece back
	boards[endType] &= ~toBit;
	boards[pIdx] |= fromBit;

	//Also for all friendly pieces
	boards[fAll] &= ~toBit;
	boards[fAll] |= fromBit;
	ResetCastleRights(castleRights);

	if (capture == NO_CAPTURE)
		return ;

	//Restore captured if there was any
	boards[capture] |= toBit;
	boards[eAll] |= toBit;
}

//Unpacks the move from the 32 bit integer and makes it
static uint8_t MakeMove(uint32_t move, uint64_t *boards, uint8_t *castleRights)
{
	uint8_t start = move & 0xFF;
	if (start >= 100)
	{
		CastleTheKing(boards, start, castleRights);
		SetEnPassantSquare(100);
		return (NO_CAPTURE);
	}

	uint8_t end	= (move >> 8) & 0xFF;
	uint8_t pIdx = (move >> 16) & 0xF;
	uint8_t endType = (move >> 20) & 0xF;
	uint8_t fAll = (move >> 24) & 0xFF;

	if (endType == 14)
	{
		uint8_t victimSquare;
		uint8_t victimBoardIdx;

		if (fAll == 12)
		{
			victimSquare = end + 8;
			victimBoardIdx = 6;
		}
		else
		{
			victimSquare = end - 8;
			victimBoardIdx = 0;
		}

		boards[victimBoardIdx] &= ~(1ull << victimSquare);
		boards[fAll ^ 1] &= ~(1ull << victimSquare);

		boards[pIdx] &= ~(1ull << start);
		boards[pIdx] |= (1ull << end);
		boards[fAll] &= ~(1ull << start);
		boards[fAll] |= (1ull << end);

		SetEnPassantSquare(100);
		return victimBoardIdx;
	}

	if (pIdx == 0)
	{
		if (abs((int)start - (int)end) == 16)
			SetEnPassantSquare(start + 8);
	}
	else if (pIdx == 6)
	{
		if (abs((int)start - (int)end) == 16)
			SetEnPassantSquare(start - 8);
	}
	else
		SetEnPassantSquare(100);


	//Getting enemy all index
	uint8_t eAll = fAll ^ 1; 
	uint64_t enemyAll = boards[eAll];
		
	//enemy starting index
	uint8_t eStart = (eAll - 12) * 6; 

	uint64_t toMask = (1ULL << end);
	uint64_t fromMaskNot = ~(1ULL << start);

	//Handle own board
	boards[pIdx] &= fromMaskNot;
	boards[endType] |= toMask;
		
	boards[fAll] &= fromMaskNot;
	boards[fAll] |= toMask;

	//Handle enemy boards
	boards[eAll] &= ~toMask;
	SetCastleRights(castleRights, start, end);
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

//The recursive function itself
static int AiTurn(uint64_t boards[14], uint8_t depth, int alpha, int beta, bool white)
{
	if (depth == 0 || boards[5] == 0 || boards[11] == 0)
		return (GetPositionScore(boards));
	//Stores all the possible moves
	uint32_t moves[256];
	uint8_t castleRights = 0;
	if (white)
	{
		int maxEval = -9999999;
		//Generates all the possible moves
		uint8_t r = GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[12], boards[13], boards);
		if (r == 1)
			return (0);
		if (r == 2)
			return (maxEval - depth * 10);
		uint8_t i = 0;
		while (moves[i] != 0)
		{
			int8_t epSquareTemp = GetEnPassantSquare();
			uint8_t capture = MakeMove(moves[i], boards, &castleRights);			//We make a move
			int score = AiTurn(boards, depth - 1, alpha, beta, false);		//Call the recursion for it
			UnMakeMove(moves[i], boards, capture, castleRights);					//And unmake it
			SetEnPassantSquare(epSquareTemp);

			//Checking the score and pruning if we need to
			maxEval = std::max(maxEval, score);
			alpha = std::max(alpha, score);
			if (beta <= alpha)
				break ;
			i++;
		}
		return (maxEval);
	}
	else
	{
		int minEval = 9999999;
		uint8_t r = GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[13], boards[12], boards);
		if (r == 1)
			return (0);
		if (r == 2)
			return (minEval + depth * 10);
		uint8_t i = 0;
		while (moves[i] != 0)
		{
			int8_t epSquareTemp = GetEnPassantSquare();
			uint8_t capture = MakeMove(moves[i], boards, &castleRights);
			int score = AiTurn(boards, depth - 1, alpha, beta, true);
			UnMakeMove(moves[i], boards, capture, castleRights);
			SetEnPassantSquare(epSquareTemp);

			minEval = std::min(minEval, score);
			beta = std::min(beta, score);
			if (beta <= alpha)
				break ;
			i++;
		}
		return (minEval);
	}
}

//Function that calls for the recursion and gets the score
uint32_t GetMove(uint64_t boards[14], bool white)
{
	uint32_t move = 0;
	uint32_t moves[256];
	uint8_t castleRights = 0;
	if (white)
	{
		int best = -99999999;
		uint8_t r = GenerateMovesWhite(moves, boards[0], boards[1], boards[2], boards[3], boards[4], boards[5], boards[12], boards[13], boards);
		if (r != 0)
		{
			gameOver = true;
			return (0);
		}
		uint8_t i = 0;
		while (moves[i] != 0)
		{
			int8_t epSquareTemp = GetEnPassantSquare();
			uint8_t capture = MakeMove(moves[i], boards, &castleRights);
			int score = AiTurn(boards, DEPTH, -9999999, 9999999, false);
			UnMakeMove(moves[i], boards, capture, castleRights);
			SetEnPassantSquare(epSquareTemp);

			if (score > best)
			{
				best = score;
				move = moves[i];
			}
			i++;
		}
	}
	else
	{
		int best = 99999999;
		uint8_t r = GenerateMovesBlack(moves, boards[6], boards[7], boards[8], boards[9], boards[10], boards[11], boards[13], boards[12], boards);
		if (r != 0)
		{
			gameOver = true;
			return (0);
		}
		uint8_t i = 0;
		while (moves[i] != 0)
		{
			int8_t epSquareTemp = GetEnPassantSquare();
			uint8_t capture = MakeMove(moves[i], boards, &castleRights);
			int score = AiTurn(boards, DEPTH, -9999999, 9999999, true);
			UnMakeMove(moves[i], boards, capture, castleRights);
			SetEnPassantSquare(epSquareTemp);

			if (score < best)
			{
				best = score;
				move = moves[i];
			}
			i++;
		}
	}
	return (move);
}
