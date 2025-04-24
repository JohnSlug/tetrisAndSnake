#include "../../brick_game/tetris/tetris.h"

void tetris() {
  tetris_t tetris = {};
  cliTetris cli = {};
  {
    initTetris(&tetris);
    InitCli(&cli);
  }
  {
    FILE *high_score_file = fopen("highScore.txt", "r+");
    int temp_high_score;
    if (high_score_file) {
      if (fscanf(high_score_file, "%d", &temp_high_score)) {
        if (temp_high_score > getHighScore()) {
          setHighScore(temp_high_score);
        }
      }
      fclose(high_score_file);
    }
  }
  main_loop(&tetris);
  {
    FILE *high_score_file = fopen("highScore.txt", "w+");
    int temp_high_score;
    if (high_score_file) {
      if (fscanf(high_score_file, "%d", &temp_high_score)) {
        if (temp_high_score < getHighScore()) {
          fseek(high_score_file, 0, SEEK_SET);
          fprintf(high_score_file, "%d", getHighScore());
        }
      }
      fclose(high_score_file);
    }
  }
  {
    removeTetris(&tetris);
    removeCli(&cli);
  }
}
void main_loop(tetris_t *tetris) {
  clock_t last_fall_time = clock();
  UserInput_t last_user_input = {};
  char exit_flag = 0;
  while (!exit_flag) {
    last_user_input = getUserInput();
    userInput(last_user_input.action, last_user_input.hold);
    printCurrentState(updCurrState());
    if (!isPause() &&
        clock() >
            last_fall_time + (1000 * (DEFAULT_FALL_TIME_MS -
                                      tetris->speed * SPEED_STEP_TIME_MS))) {
      tetris->is_fall_time = 1;
      last_fall_time = clock();
    } else if (isPause()) {
      last_fall_time = clock();
    }
    if (isGameEnd()) {
      exit_flag = 1;
      break;
    }
  }
}