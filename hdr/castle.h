
#ifndef CASTLE_H
# define CASTLE_H

# include <stdlib.h>

void SetCastleRights(uint8_t *oldRights, uint8_t start, uint8_t end);
void ResetCastleRights(uint8_t oldRights);

void CastlingWhite(uint64_t *boards, uint32_t *moves, uint8_t *index);
void CastlingBlack(uint64_t *boards, uint32_t *moves, uint8_t *index);

void CastleTheKing(uint64_t *boards, uint8_t parameter, uint8_t *oldRights);
void UnCastleTheKing(uint64_t *boards, uint8_t parameter);

#endif
