
#ifndef AI_MOVE_H
# define AI_MOVE_H

# include <stdlib.h>

uint8_t GenerateMovesWhite(uint32_t *moves, uint64_t pawns, uint64_t knights,
						uint64_t bishops, uint64_t rooks, uint64_t queens,
						uint64_t kings, uint64_t fBoard, uint64_t eBoard, uint64_t *allBoards);
uint8_t GenerateMovesBlack(uint32_t *moves, uint64_t pawns, uint64_t knights,
						uint64_t bishops, uint64_t rooks, uint64_t queens,
						uint64_t kings, uint64_t fBoard, uint64_t eBoard, uint64_t *allBoards);
uint32_t GetMove(uint64_t boards[14], bool white);
bool GetGameOver();

#endif
