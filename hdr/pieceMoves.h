
#ifndef PIECE_MOVES_H
# define PIECE_MOVES_H

# include <stdlib.h>

void SetMoves(uint64_t knm[64], uint64_t km[64], uint64_t sl[8][64], uint64_t pa[2][64]);
uint64_t GetKnightMoves(uint64_t fBoard, int pos);
uint64_t GetKingMoves(uint64_t fBoard, int pos);
uint64_t GetBishopMoves(uint64_t fBoard, uint64_t eBoard, int pos);
uint64_t GetRookMoves(uint64_t fBoard, uint64_t eBoard, int pos);
uint64_t GetAllPawnMovesWhite(uint64_t fBoard, uint64_t eBoard, int pos);
uint64_t GetAllPawnMovesBlack(uint64_t fBoard, uint64_t eBoard, int pos);
uint64_t GetPawnAttacks(uint64_t eBoard, int pos, int color);

uint64_t GetRayBetween(uint8_t pos1, uint8_t pos2);
uint64_t GetLineMask(uint8_t pos1, uint8_t pos2);
uint64_t GetRookSingleRayPinBoard(uint64_t eBoard, uint8_t pos, uint8_t dir);
uint64_t GetBighopSingleRayPinBoard(uint64_t eBoard, uint8_t pos, uint8_t dir);

#endif
