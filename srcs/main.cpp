
#include <stdlib.h>
#include "moveGenerator.h"
#include "pieceMoves.h"

struct HashEntry {
    uint64_t zobristKey;	// 64 bits: The unique ID
    int16_t  score;			// 16 bits: The Centipawn score
    int16_t  bestMove;		// 16 bits: Compressed move (Source -> Dest)
    uint8_t  depth;			// 8 bits: How deep we searched
    uint8_t  type;			// 8 bits: Exact, Upper Bound, or Lower Bound
};



int main()
{
	GenerateKnighMoves();
	GenerateKingMoves();
	GenerateSlides();
	GeneratePawnAttacks();
	uint64_t b = 18446462598732906495llu;
	uint64_t t = GetKnightMoves(b, 0);
	PrinBoard(t);
	return (0);
}
