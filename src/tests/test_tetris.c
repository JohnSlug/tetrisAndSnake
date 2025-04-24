#include "test_tetris.h"

// ==================== Additional Tests ====================

START_TEST(additional_isBlockBeyondLimit_00) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = 0, .y = 0};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);
  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 0);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_isBlockBeyondLimit_01) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = 0, .y = 0};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      tetromino[i][j] = FULL;
    }
  }

  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 0);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_isBlockBeyondLimit_02) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = COLUMNS_WIN / 2, .y = ROWS_WIN / 2};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      tetromino[i][j] = FULL;
    }
  }

  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 0);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_isBlockBeyondLimit_03) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = COLUMNS_WIN + BLOCK_SIZE,
                              .y = ROWS_WIN + BLOCK_SIZE};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);
  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 0);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_isBlockBeyondLimit_04) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = -BLOCK_SIZE, .y = -BLOCK_SIZE};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);
  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 0);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_isBlockBeyondLimit_05) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = 0, .y = -1};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      tetromino[i][j] = FULL;
    }
  }

  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 1);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_isBlockBeyondLimit_06) {
  int **tetromino = NULL;
  coordinate tetromino_pos = {.x = -1, .y = -1};

  ck_assert_int_eq(crtTetramino(&tetromino), SUCCESS);
  tetromino[0][0] = FULL;
  ck_assert_int_eq(isBlockBeyondLimit(tetromino, tetromino_pos), 1);
  rmvTetramino(&tetromino);
}
END_TEST

START_TEST(additional_getRewardByCompleteLines_00) {
  ck_assert_int_eq(getRewardByCompleteLines(1), 100);
  ck_assert_int_eq(getRewardByCompleteLines(2), 300);
  ck_assert_int_eq(getRewardByCompleteLines(3), 700);
  ck_assert_int_eq(getRewardByCompleteLines(4), 1500);

  ck_assert_int_eq(getRewardByCompleteLines(-1), 0);
  ck_assert_int_eq(getRewardByCompleteLines(100), 1500);
}
END_TEST

Suite *additional_suite() {
  TCase *tcase = tcase_create("additional_tcase");
  Suite *suite = suite_create("additional");

  tcase_add_test(tcase, additional_isBlockBeyondLimit_00);
  tcase_add_test(tcase, additional_isBlockBeyondLimit_01);
  tcase_add_test(tcase, additional_isBlockBeyondLimit_02);
  tcase_add_test(tcase, additional_isBlockBeyondLimit_03);
  tcase_add_test(tcase, additional_isBlockBeyondLimit_04);
  tcase_add_test(tcase, additional_isBlockBeyondLimit_05);
  tcase_add_test(tcase, additional_isBlockBeyondLimit_06);

  tcase_add_test(tcase, additional_getRewardByCompleteLines_00);

  suite_add_tcase(suite, tcase);
  return suite;
}

// ==================== Block Tests ====================

START_TEST(block_getAllStaticBlocks_00) {
  int blocks[BLOCK_COUNT][BLOCK_SIZE][BLOCK_SIZE] = {};

  getAllTetramino(blocks);

  ck_assert_int_eq(blocks[0][0][0], 0);
  ck_assert_int_ne(blocks[0][1][0], 0);
  ck_assert_int_ne(blocks[0][1][1], 0);
  ck_assert_int_ne(blocks[0][1][2], 0);
  ck_assert_int_ne(blocks[0][1][3], 0);
}
END_TEST

START_TEST(block_getRandomStaticBlock_00) {
  int **block = NULL;
  ck_assert_int_eq(crtTetramino(&block), SUCCESS);
  ck_assert_int_eq(getRandomTetramino(block), SUCCESS);
  rmvTetramino(&block);
}
END_TEST

START_TEST(block_getRandomStaticBlock_01) {
  int **block = NULL;
  ck_assert_int_eq(crtTetramino(&block), SUCCESS);
  ck_assert_int_eq(getRandomTetramino(block), SUCCESS);

  int is_full = 0;
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      if (block[i][j] != ZERO) {
        is_full = 1;
        break;
      }
    }
  }
  ck_assert_int_eq(is_full, 1);
  rmvTetramino(&block);
}
END_TEST

START_TEST(block_create_block_00) {
  int **block1;
  ck_assert_int_eq(crtTetramino(&block1), SUCCESS);
  ck_assert_int_eq(block1[0][0], 0);
  rmvTetramino(&block1);
}
END_TEST

START_TEST(block_remove_block_00) {
  int **block1 = NULL;
  rmvTetramino(&block1);
}
END_TEST

START_TEST(block_copyblock_00) {
  int **block1;
  ck_assert_int_eq(crtTetramino(&block1), SUCCESS);

  int **block2;
  ck_assert_int_eq(crtTetramino(&block2), SUCCESS);

  cpTetramino(&block1, &block2);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block2[i][j], block1[i][j]);
    }
  }

  rmvTetramino(&block1);
  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_copyblock_01) {
  int **block1;
  ck_assert_int_eq(crtTetramino(&block1), SUCCESS);

  int **block2;
  ck_assert_int_eq(crtTetramino(&block2), SUCCESS);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      block1[i][j] = FULL;
    }
  }

  cpTetramino(&block1, &block2);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block2[i][j], block1[i][j]);
    }
  }

  rmvTetramino(&block1);
  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_copyblock_02) {
  int **block1;
  ck_assert_int_eq(crtTetramino(&block1), SUCCESS);

  int **block2;
  ck_assert_int_eq(crtTetramino(&block2), SUCCESS);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      block1[i][j] = ZERO;
      block2[i][j] = FULL;
    }
  }

  cpTetramino(&block1, &block2);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block2[i][j], block1[i][j]);
    }
  }

  rmvTetramino(&block1);
  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_rotateblock_00) {
  int block1[BLOCK_SIZE][BLOCK_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  int **block2;
  crtTetramino(&block2);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block2[i][j], block1[(BLOCK_SIZE - 1) - j][i]);
    }
  }

  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_rotateblock_01) {
  int block1[BLOCK_SIZE][BLOCK_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  int **block2;
  crtTetramino(&block2);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block2[i][j], block1[(BLOCK_SIZE - 1) - j][i]);
    }
  }

  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_rotateblock_02) {
  int block1[BLOCK_SIZE][BLOCK_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  int **block2;
  crtTetramino(&block2);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block2[i][j], block1[(BLOCK_SIZE - 1) - j][i]);
    }
  }

  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_rotateblock_03) {
  int block1[BLOCK_SIZE][BLOCK_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  int **block2;
  crtTetramino(&block2);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, false);
  rotateTetramino(block2, false);
  rotateTetramino(block2, false);
  rotateTetramino(block2, false);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block1[i][j], block2[i][j]);
    }
  }

  rmvTetramino(&block2);
}
END_TEST

START_TEST(block_rotateblock_04) {
  int block1[BLOCK_SIZE][BLOCK_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  int **block2;
  crtTetramino(&block2);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block1[i], block2[i], BLOCK_SIZE * sizeof(int));
  }

  rotateTetramino(block2, true);
  rotateTetramino(block2, true);
  rotateTetramino(block2, true);
  rotateTetramino(block2, true);

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      ck_assert_int_eq(block1[i][j], block2[i][j]);
    }
  }

  rmvTetramino(&block2);
}
END_TEST

Suite *block_suite() {
  TCase *tcase = tcase_create("block_tcase");
  Suite *suite = suite_create("block");

  tcase_add_test(tcase, block_getAllStaticBlocks_00);

  tcase_add_test(tcase, block_getRandomStaticBlock_00);
  tcase_add_loop_test(tcase, block_getRandomStaticBlock_01, 0, 10);

  tcase_add_test(tcase, block_create_block_00);
  tcase_add_test(tcase, block_remove_block_00);

  tcase_add_test(tcase, block_copyblock_00);
  tcase_add_test(tcase, block_copyblock_01);
  tcase_add_test(tcase, block_copyblock_02);

  tcase_add_test(tcase, block_rotateblock_00);
  tcase_add_test(tcase, block_rotateblock_01);
  tcase_add_test(tcase, block_rotateblock_02);
  tcase_add_test(tcase, block_rotateblock_03);
  tcase_add_test(tcase, block_rotateblock_04);

  suite_add_tcase(suite, tcase);

  return suite;
}

// ==================== Field Tests ====================

START_TEST(field_createfield_00) {
  int **field = NULL;
  ck_assert_int_eq(crtField(&field), SUCCESS);
  ck_assert_int_eq(field[0][0], 0);
  rmvField(&field);
}
END_TEST

START_TEST(field_removefield_00) {
  int **field = NULL;
  rmvField(&field);
}
END_TEST

START_TEST(field_copyfield_00) {
  int **field1;
  ck_assert_int_eq(crtField(&field1), SUCCESS);

  int **field2;
  ck_assert_int_eq(crtField(&field2), SUCCESS);

  copyField(&field1, &field2);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      ck_assert_int_eq(field2[i][j], field1[i][j]);
    }
  }

  rmvField(&field1);
  rmvField(&field2);
}
END_TEST

START_TEST(field_copyfield_01) {
  int **field1;
  ck_assert_int_eq(crtField(&field1), SUCCESS);

  int **field2;
  ck_assert_int_eq(crtField(&field2), SUCCESS);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      field1[i][j] = FULL;
    }
  }

  copyField(&field1, &field2);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      ck_assert_int_eq(field2[i][j], field1[i][j]);
    }
  }

  rmvField(&field1);
  rmvField(&field2);
}
END_TEST

START_TEST(field_copyfield_02) {
  int **field1;
  ck_assert_int_eq(crtField(&field1), SUCCESS);

  int **field2;
  ck_assert_int_eq(crtField(&field2), SUCCESS);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      field1[i][j] = ZERO;
      field2[i][j] = FULL;
    }
  }

  copyField(&field1, &field2);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      ck_assert_int_eq(field2[i][j], field1[i][j]);
    }
  }

  rmvField(&field1);
  rmvField(&field2);
}
END_TEST

START_TEST(field_checkingBlockCollisionWithField_00) {
  tetris_t tetris;
  initTetris(&tetris);

  ck_assert_int_eq(
      checkingBlockCollisionWithField(tetris.current_block,
                                      tetris.current_block_pos, tetris.field),
      0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_checkingBlockCollisionWithField_01) {
  tetris_t tetris;
  initTetris(&tetris);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      tetris.current_block[i][j] = FULL;
    }
  }

  ck_assert_int_eq(
      checkingBlockCollisionWithField(tetris.current_block,
                                      tetris.current_block_pos, tetris.field),
      0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_checkingBlockCollisionWithField_02) {
  tetris_t tetris;
  initTetris(&tetris);

  for (int i = 0; i < ROWS_WIN; i++) {
    for (int j = 0; j < COLUMNS_WIN; j++) {
      tetris.field[i][j] = FULL;
    }
  }

  ck_assert_int_eq(
      checkingBlockCollisionWithField(tetris.current_block,
                                      tetris.current_block_pos, tetris.field),
      0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_checkingBlockCollisionWithField_03) {
  tetris_t tetris;
  initTetris(&tetris);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      tetris.current_block[i][j] = FULL;
    }
  }

  for (int i = 0; i < ROWS_WIN; i++) {
    for (int j = 0; j < COLUMNS_WIN; j++) {
      tetris.field[i][j] = FULL;
    }
  }

  ck_assert_int_ne(
      checkingBlockCollisionWithField(tetris.current_block,
                                      tetris.current_block_pos, tetris.field),
      0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_checkingBlockCollisionWithField_04) {
  tetris_t tetris;
  initTetris(&tetris);

  tetris.current_block[0][0] = FULL;

  for (int i = 0; i < ROWS_WIN; i++) {
    for (int j = 0; j < COLUMNS_WIN; j++) {
      tetris.field[i][j] = FULL;
    }
  }

  ck_assert_int_ne(
      checkingBlockCollisionWithField(tetris.current_block,
                                      tetris.current_block_pos, tetris.field),
      0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_addBlockInfield_00) {
  tetris_t tetris;
  initTetris(&tetris);

  addBlockInField(tetris.current_block, tetris.current_block_pos, tetris.field);

  for (int i = 0; i < ROWS_WIN; i++) {
    for (int j = 0; j < COLUMNS_WIN; j++) {
      ck_assert_int_eq(tetris.field[i][j], 0);
    }
  }

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_addBlockInfield_01) {
  tetris_t tetris;
  initTetris(&tetris);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      tetris.current_block[i][j] = FULL;
    }
  }

  addBlockInField(tetris.current_block, tetris.current_block_pos, tetris.field);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      ck_assert_int_ne(tetris.field[tetris.current_block_pos.x + i]
                                   [tetris.current_block_pos.y + j],
                       0);
    }
  }

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_addBlockInfield_02) {
  tetris_t tetris;
  initTetris(&tetris);

  tetris.current_block[0][0] = FULL;

  addBlockInField(tetris.current_block, tetris.current_block_pos, tetris.field);

  ck_assert_int_ne(
      tetris.field[tetris.current_block_pos.y][tetris.current_block_pos.x], 0);

  for (int i = 1; i < COLUMNS_WIN; i++) {
    ck_assert_int_eq(tetris.field[tetris.current_block_pos.y]
                                 [tetris.current_block_pos.x + i],
                     0);
  }

  for (int i = 1; i < ROWS_WIN; i++) {
    for (int j = 0; j < COLUMNS_WIN; j++) {
      ck_assert_int_eq(tetris.field[i][j], 0);
    }
  }

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_addBlockInfield_03) {
  tetris_t tetris;
  initTetris(&tetris);

  tetris.current_block[BLOCK_SIZE - 1][BLOCK_SIZE - 1] = FULL;

  addBlockInField(tetris.current_block, tetris.current_block_pos, tetris.field);

  ck_assert_int_ne(tetris.field[tetris.current_block_pos.y + 3]
                               [tetris.current_block_pos.x + 3],
                   0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_addBlockInfield_04) {
  tetris_t tetris;
  initTetris(&tetris);

  tetris.current_block_pos.x = 2;
  tetris.current_block_pos.y = 2;

  tetris.current_block[0][0] = FULL;

  addBlockInField(tetris.current_block, tetris.current_block_pos, tetris.field);

  ck_assert_int_ne(
      tetris.field[tetris.current_block_pos.y][tetris.current_block_pos.x], 0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(field_completeLineInField_00) {
  int **field = NULL;
  ck_assert_int_eq(crtField(&field), SUCCESS);
  ck_assert_int_eq(completeLineInField(field), 0);
  rmvField(&field);
}
END_TEST

START_TEST(field_completeLineInField_01) {
  int **field = NULL;
  ck_assert_int_eq(crtField(&field), SUCCESS);

  for (int i = 0; i < COLUMNS_WIN; ++i) {
    field[0][i] = FULL;
  }

  ck_assert_int_eq(completeLineInField(field), 1);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      ck_assert_int_eq(field[i][j], ZERO);
    }
  }

  rmvField(&field);
}
END_TEST

START_TEST(field_completeLineInField_02) {
  int **field = NULL;
  ck_assert_int_eq(crtField(&field), SUCCESS);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      field[i][j] = FULL;
    }
  }

  ck_assert_int_eq(completeLineInField(field), ROWS_WIN);

  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      ck_assert_int_eq(field[i][j], ZERO);
    }
  }

  rmvField(&field);
}
END_TEST

START_TEST(field_completeLineInField_03) {
  int **field = NULL;
  ck_assert_int_eq(crtField(&field), SUCCESS);

  field[0][0] = FULL;

  ck_assert_int_eq(completeLineInField(field), 0);

  ck_assert_int_eq(field[0][0], FULL);

  rmvField(&field);
}
END_TEST

START_TEST(field_completeLineInField_04) {
  int **field = NULL;
  ck_assert_int_eq(crtField(&field), SUCCESS);

  for (int i = 0; i < COLUMNS_WIN - 1; ++i) {
    field[0][i] = FULL;
  }

  ck_assert_int_eq(completeLineInField(field), 0);

  ck_assert_int_eq(field[0][COLUMNS_WIN - 1], ZERO);

  for (int i = 0; i < COLUMNS_WIN - 1; ++i) {
    ck_assert_int_eq(field[0][i], FULL);
  }

  rmvField(&field);
}
END_TEST

Suite *field_suite() {
  TCase *tcase = tcase_create("field_tcase");
  Suite *suite = suite_create("field");

  tcase_add_test(tcase, field_createfield_00);
  tcase_add_test(tcase, field_removefield_00);

  tcase_add_test(tcase, field_copyfield_00);
  tcase_add_test(tcase, field_copyfield_01);
  tcase_add_test(tcase, field_copyfield_02);

  tcase_add_test(tcase, field_checkingBlockCollisionWithField_00);
  tcase_add_test(tcase, field_checkingBlockCollisionWithField_01);
  tcase_add_test(tcase, field_checkingBlockCollisionWithField_02);
  tcase_add_test(tcase, field_checkingBlockCollisionWithField_03);
  tcase_add_test(tcase, field_checkingBlockCollisionWithField_04);

  tcase_add_test(tcase, field_addBlockInfield_00);
  tcase_add_test(tcase, field_addBlockInfield_01);
  tcase_add_test(tcase, field_addBlockInfield_02);
  tcase_add_test(tcase, field_addBlockInfield_03);
  tcase_add_test(tcase, field_addBlockInfield_04);

  tcase_add_test(tcase, field_completeLineInField_00);
  tcase_add_test(tcase, field_completeLineInField_01);
  tcase_add_test(tcase, field_completeLineInField_02);
  tcase_add_test(tcase, field_completeLineInField_03);
  tcase_add_test(tcase, field_completeLineInField_04);

  suite_add_tcase(suite, tcase);
  return suite;
}

// ==================== FSM Tests ====================

START_TEST(fsm_selectNextState_00) {
  ck_assert_int_eq(selectNextState(NULL), FAIL);
}
END_TEST

START_TEST(fsm_selectNextState_01) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

  removeTetris(&tetris);
}
END_TEST

START_TEST(fsm_selectNextState_02) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, MOVING_STATE);

  tetris.is_fall_time = 1;
  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

  tetris.state = ATTACHING_STATE;

  tetris.current_block_pos.x = COLUMNS_WIN / 2;
  tetris.current_block_pos.y = ROWS_WIN / 2;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  removeTetris(&tetris);
}
END_TEST

START_TEST(fsm_selectNextState_03) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  for (int i = 0; i < ROWS_WIN; i++) {
    for (int j = 0; j < COLUMNS_WIN - 1; j++) {
      tetris.field[i][j] = FULL;
    }
  }

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, MOVING_STATE);

  tetris.last_user_input.action = Down;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, END_GAME_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

  removeTetris(&tetris);
}
END_TEST

START_TEST(fsm_selectNextState_04) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, MOVING_STATE);

  {
    tetris.last_user_input.action = Pause;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Pause;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Action;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Terminate;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.state = END_GAME_STATE;

    tetris.last_user_input.action = Start;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.state = END_GAME_STATE;

    tetris.last_user_input.action = Terminate;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  }

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, TERMINATE_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

  removeTetris(&tetris);
}
END_TEST

START_TEST(fsm_selectNextState_05) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, MOVING_STATE);

  {
    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Left;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Action;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Action;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  }

  removeTetris(&tetris);
}
END_TEST

START_TEST(fsm_selectNextState_06) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, MOVING_STATE);

  {
    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Right;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);

    tetris.last_user_input.action = Action;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Action;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  }

  removeTetris(&tetris);
}
END_TEST

START_TEST(fsm_selectNextState_07) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, MOVING_STATE);

  {
    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
    tetris.last_user_input.action = Down;
    tetris.last_user_input.hold = true;
    ck_assert_int_eq(selectNextState(&tetris), SUCCESS);
  }

  removeTetris(&tetris);
}
END_TEST

Suite *fsm_suite() {
  TCase *tcase = tcase_create("fsm_tcase");
  Suite *suite = suite_create("fsm");

  tcase_add_test(tcase, fsm_selectNextState_00);
  tcase_add_test(tcase, fsm_selectNextState_01);
  tcase_add_test(tcase, fsm_selectNextState_02);
  tcase_add_test(tcase, fsm_selectNextState_03);
  tcase_add_test(tcase, fsm_selectNextState_04);
  tcase_add_test(tcase, fsm_selectNextState_05);
  tcase_add_test(tcase, fsm_selectNextState_06);
  tcase_add_test(tcase, fsm_selectNextState_07);

  suite_add_tcase(suite, tcase);
  return suite;
}

// ==================== Interfaces Tests ====================

START_TEST(interfaces_initTetris_00) {
  ck_assert_int_eq(initTetris(NULL), FAIL);
}
END_TEST

START_TEST(interfaces_initTetris_01) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);
  ck_assert_int_eq(tetris.state, START_STATE);
  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_userInput_00) { userInput(Action, true); }
END_TEST

START_TEST(interfaces_userInput_01) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = false;

  userInput(Action, true);

  ck_assert_int_eq(tetris.last_user_input.action, Action);
  ck_assert_int_eq(tetris.last_user_input.hold, true);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_userInput_02) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  userInput(Action, false);

  ck_assert_int_eq(tetris.last_user_input.action, Action);
  ck_assert_int_eq(tetris.last_user_input.hold, false);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_userInput_03) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  userInput(Start, false);

  ck_assert_int_eq(tetris.last_user_input.action, Start);
  ck_assert_int_eq(tetris.last_user_input.hold, false);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_updateCurrentState_00) {
  GameInfo_tetris game_info = updCurrState();
  ck_assert_ptr_null(game_info.field);
}
END_TEST

START_TEST(interfaces_updateCurrentState_01) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  GameInfo_tetris game_info = updCurrState();

  ck_assert_int_eq(tetris.state, SPAWN_STATE);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      ck_assert_int_eq(tetris.next_block[i][j], game_info.next[i][j]);
    }
  }

  ck_assert_int_eq(tetris.high_score, game_info.high_score);
  ck_assert_int_eq(tetris.score, game_info.score);

  ck_assert_int_eq(tetris.level, game_info.level);
  ck_assert_int_eq(tetris.speed, game_info.speed);

  ck_assert_int_eq(tetris.pause, game_info.pause);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_updateCurrentState_02) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  tetris.last_user_input.action = Start;
  tetris.last_user_input.hold = true;

  updCurrState();
  GameInfo_tetris game_info = updCurrState();

  ck_assert_int_eq(tetris.state, MOVING_STATE);

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      ck_assert_int_eq(tetris.next_block[i][j], game_info.next[i][j]);
    }
  }

  ck_assert_int_eq(tetris.high_score, game_info.high_score);
  ck_assert_int_eq(tetris.score, game_info.score);

  ck_assert_int_eq(tetris.level, game_info.level);
  ck_assert_int_eq(tetris.speed, game_info.speed);

  ck_assert_int_eq(tetris.pause, game_info.pause);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_isGameEnd_00) { ck_assert_int_eq(isGameEnd(), 1); }
END_TEST

START_TEST(interfaces_isGameEnd_01) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);
  ck_assert_int_eq(isGameEnd(), 0);
  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_isGameEnd_02) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  tetris.state = TERMINATE_STATE;
  ck_assert_int_eq(isGameEnd(), 1);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_game_start_00) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  gameStart();
  ck_assert_int_eq(tetris.is_game_start, 1);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_setHighScore_00) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  tetris.high_score = 0;
  setHighScore(100);
  ck_assert_int_eq(tetris.high_score, 100);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_getHighScore_00) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  tetris.high_score = 100;
  ck_assert_int_eq(getHighScore(), 100);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_isPause_00) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  tetris.pause = 0;
  ck_assert_int_eq(isPause(), 0);

  removeTetris(&tetris);
}
END_TEST

START_TEST(interfaces_isPause_01) {
  tetris_t tetris;
  ck_assert_int_eq(initTetris(&tetris), SUCCESS);

  tetris.pause = 1;
  ck_assert_int_eq(isPause(), 1);

  removeTetris(&tetris);
}
END_TEST

Suite *interfaces_suite() {
  TCase *tcase = tcase_create("interfaces_tcase");
  Suite *suite = suite_create("interfaces");

  tcase_add_test(tcase, interfaces_initTetris_00);
  tcase_add_test(tcase, interfaces_initTetris_01);

  tcase_add_test(tcase, interfaces_userInput_00);
  tcase_add_test(tcase, interfaces_userInput_01);
  tcase_add_test(tcase, interfaces_userInput_02);
  tcase_add_test(tcase, interfaces_userInput_03);

  tcase_add_test(tcase, interfaces_updateCurrentState_00);
  tcase_add_test(tcase, interfaces_updateCurrentState_01);
  tcase_add_test(tcase, interfaces_updateCurrentState_02);

  tcase_add_test(tcase, interfaces_isGameEnd_00);
  tcase_add_test(tcase, interfaces_isGameEnd_01);
  tcase_add_test(tcase, interfaces_isGameEnd_02);

  tcase_add_test(tcase, interfaces_game_start_00);

  tcase_add_test(tcase, interfaces_setHighScore_00);
  tcase_add_test(tcase, interfaces_getHighScore_00);

  tcase_add_test(tcase, interfaces_isPause_00);
  tcase_add_test(tcase, interfaces_isPause_01);

  suite_add_tcase(suite, tcase);
  return suite;
}

// ==================== Main Runner ====================

int main() {
  run_all_tests();
  return 0;
}

int run_all_tests() {
  SRunner *runner = srunner_create(NULL);

  add_all_suites_in_runner(runner);

  srunner_run_all(runner, CK_NORMAL);

  int failed_count = srunner_ntests_failed(runner);

  srunner_free(runner);

  return failed_count;
}

void add_all_suites_in_runner(SRunner *runner) {
  srunner_add_suite(runner, block_suite());
  srunner_add_suite(runner, field_suite());
  srunner_add_suite(runner, interfaces_suite());
  srunner_add_suite(runner, additional_suite());
  srunner_add_suite(runner, fsm_suite());
}