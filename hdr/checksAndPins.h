
#ifndef CHECKS_AND_PINS_H
# define CHECKS_AND_PINS_H

# include <stdlib.h>

uint64_t BlackKingCheckers(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard);

uint64_t WhiteKingCheckers(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard);

bool BlackKingInCheck(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard);


bool WhiteKingInCheck(uint8_t pos, uint64_t ePawns, uint64_t eKnights,
							uint64_t eBishops, uint64_t eRooks, uint64_t eQueens,
							uint64_t eKings, uint64_t fBoard, uint64_t eBoard);

uint64_t PinnedMask(uint8_t kingPos, uint64_t eBishops, uint64_t eRooks, uint64_t eQueens, uint64_t fBoard, uint64_t eBoard);
uint64_t GetCheckMask(uint8_t kingPos, uint64_t checkers, uint64_t eBishops, uint64_t eRooks, uint64_t eQueens);

#endif
