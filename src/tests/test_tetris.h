#ifndef TESTS_H
#define TESTS_H

#include <check.h>
#include <stdint.h>
#include <stdio.h>

#include "../brick_game/tetris/tetris.h"

// Объявления тестовых сьютов
Suite *block_suite();
Suite *field_suite();
Suite *interfaces_suite();
Suite *additional_suite();
Suite *fsm_suite();

// Функции запуска тестов
int run_all_tests();
void add_all_suites_in_runner(SRunner *runner);

#endif  // TESTS_H