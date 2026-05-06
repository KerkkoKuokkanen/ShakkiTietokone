
#ifndef TEST_H
# define TEST_H

# include <stdlib.h>

bool GenerateMateInTwoBoards(uint64_t *boards);
bool GenerateMateInTwoBoards2(uint64_t *boards);
bool GenerateMateInThree(uint64_t *boards);
bool TestPromotion(uint64_t *boards);
bool CastleTest(uint64_t *boards);
bool CastleTestBlack(uint64_t *boards);
bool EnPassantTest(uint64_t *boards);
bool EnPassantTestBlack(uint64_t *boards);

#endif
