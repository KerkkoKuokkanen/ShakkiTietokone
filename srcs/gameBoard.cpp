
#include <stdlib.h>
#include <stdio.h>
#include <bit>

void PrintGameBoard(uint64_t boards[12])
{
	uint64_t bCopy[12];
	for (int i = 0; i < 12; i++)
		bCopy[i] = boards[i];
	char white[64];
	char black[64];
	for (int i = 0; i < 64; i++)
	{
		white[i] = '.';
		black[i] = '.';
	}
	while (bCopy[0] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[0]);
		white[pInd] = 'P';
		bCopy[0] &= ~(1ULL << pInd);
	}
	while (bCopy[1] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[1]);
		white[pInd] = 'R';
		bCopy[1] &= ~(1ULL << pInd);
	}
	while (bCopy[2] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[2]);
		white[pInd] = 'B';
		bCopy[2] &= ~(1ULL << pInd);
	}
	while (bCopy[3] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[3]);
		white[pInd] = 'T';
		bCopy[3] &= ~(1ULL << pInd);
	}
	while (bCopy[4] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[4]);
		white[pInd] = 'Q';
		bCopy[4] &= ~(1ULL << pInd);
	}
	while (bCopy[5] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[5]);
		white[pInd] = 'K';
		bCopy[5] &= ~(1ULL << pInd);
	}
	while (bCopy[6] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[6]);
		black[pInd] = 'P';
		bCopy[6] &= ~(1ULL << pInd);
	}
	while (bCopy[7] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[7]);
		black[pInd] = 'R';
		bCopy[7] &= ~(1ULL << pInd);
	}
	while (bCopy[8] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[8]);
		black[pInd] = 'B';
		bCopy[8] &= ~(1ULL << pInd);
	}
	while (bCopy[9] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[9]);
		black[pInd] = 'T';
		bCopy[9] &= ~(1ULL << pInd);
	}
	while (bCopy[10] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[10]);
		black[pInd] = 'Q';
		bCopy[10] &= ~(1ULL << pInd);
	}
	while (bCopy[11] != 0)
	{
		uint8_t pInd = std::countr_zero(bCopy[11]);
		black[pInd] = 'K';
		bCopy[11] &= ~(1ULL << pInd);
	}
	for (int i = 0; i < 64; i++)
	{
		if (i % 8 == 0)
			printf("\n");
		if (white[i] == '.' && black[i] == '.')
			printf(".");
		else if (white[i] != '.')
			printf("\033[32m%c\033[0m", white[i]);
		else if (black[i] != '.')
			printf("\033[31m%c\033[0m", black[i]);
	}
	printf("\n");
}
