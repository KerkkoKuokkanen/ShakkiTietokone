
#ifndef EN_PASSANT_H
# define EN_PASSANT_H

# include <stdlib.h>

void SetEnPassantSquare(int8_t square);
int8_t GetEnPassantSquare();

void GenerateEnPassantWhite(uint32_t *moves, uint8_t *index, uint8_t pInd, uint64_t *boards, uint8_t kingPos);
void GenerateEnPassantBlack(uint32_t *moves, uint8_t *index, uint8_t pInd, uint64_t *boards, uint8_t kingPos);

#endif
