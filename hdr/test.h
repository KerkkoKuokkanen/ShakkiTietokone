
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
bool MoveGenerationTest(uint64_t *boards);
void GenerateMateInTwoBoardsSecondState(uint64_t *boards);
bool GenerateMateInTwoBoards2Black(uint64_t *boards);
bool GenerateMateInTwoBoards2BlackSecondState(uint64_t *boards);
bool GenerateMateInThreeStateTwo(uint64_t *boards);
bool GenerateMateInThreeStateThree(uint64_t *boards);
void CastleUnitTest(uint64_t *boards);
void PromotionTestBlack(uint64_t *boards);
void PromotionTestWhite(uint64_t *boards);

#endif
