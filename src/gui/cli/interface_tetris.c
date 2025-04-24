#include "../../brick_game/tetris/tetris.h"

// int main() {
//   srand(time(NULL));
//   initscr();
//   keypad(stdscr, TRUE);
//   noecho();
//   curs_set(0);

//   game_loop();

//   endwin();
//   return 0;
// }

void crtWin(WINDOW **window, coordinate window_size, coordinate window_pos) {
  *window = newwin(window_size.y, window_size.x, (int)window_pos.y,
                   (int)window_pos.x);
  wclear(*window);
  wrefresh(*window);
}

/**
 * @brief Отрисовка старотового окна
 */
void printStartPreview() {
  WINDOW *temp_win = newwin(4, 17, 11, 15);
  wmove(temp_win, 13, 13);
  wprintw(temp_win,
          "\n  Press 'Enter'"
          "\n     to START");
  wborder(temp_win, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL,
          FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL);
  wrefresh(temp_win);
  delwin(temp_win);
}

/**
 * @brief Прорисовка Конца игры
 */
void printEndPreview() {
  WINDOW *temp_win = newwin(6, 26, 10, 10);
  wmove(temp_win, 13, 13);
  wprintw(temp_win,
          "\n        GAMEOVER"
          "\n  Press 'Enter' to START"
          "\n     'Esc' to Escape");
  wborder(temp_win, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL,
          FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL);
  wrefresh(temp_win);
  delwin(temp_win);
}

/**
 * @brief Экран Паузы
 */
void printPausePreview() {
  WINDOW *temp_win = newwin(3, 9, 10, 10);
  wmove(temp_win, 13, 13);
  wprintw(temp_win, "\n  Pause");
  wborder(temp_win, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL,
          FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL);
  wrefresh(temp_win);
  delwin(temp_win);
}

/**
 * @brief Прорисовка одной ячейки
 * @param win Окно в котором нужно вывести.
 * @param symbol Символ, которым будет заполнена ячейка.
 */
void printFieldCeil(WINDOW *win, chtype symbol) {
  for (int i = 0; i < VIEW_CHAR_SIZE_Y; i++) {
    for (int j = 0; j < VIEW_CHAR_SIZE_X; j++) {
      waddch(win, symbol);
    }
  }
}

/**
 * @brief Является хранилищем для структуры cliTetris.
 */
void sliInfoStorage(cliTetris **cli, bool remove) {
  static cliTetris *static_cli = NULL;
  if (!remove) {
    if (cli) {
      if (*cli) {
        static_cli = *cli;
      } else {
        *cli = static_cli;
      }
    }
  } else {
    static_cli = NULL;
  }
}

/**
 * @brief Создание экземпляра структуры cliTetris.
 * Существует только один элемент и обращение происходит по ссылке на него.
 */
void InitCli(cliTetris *cli) {
  initCurses();

  {
    cli->game_win_size.x = COLUMNS_WIN * VIEW_CHAR_SIZE_X + (WIN_X * 2);
    cli->game_win_size.y = ROWS_WIN * VIEW_CHAR_SIZE_Y + (WIN_Y * 2);

    cli->stat_win_size.x = COLUMNS_WIN * VIEW_CHAR_SIZE_X + (WIN_X * 2);
    cli->stat_win_size.y = ROWS_WIN * VIEW_CHAR_SIZE_Y + (WIN_Y * 2);

    cli->game_win_pos.x = 1;
    cli->game_win_pos.y = 1;

    cli->stat_win_pos.x = cli->game_win_pos.x + cli->game_win_size.x;
    cli->stat_win_pos.y = cli->game_win_pos.y;
  }

  crtWin(&cli->game_win, cli->game_win_size, cli->game_win_pos);
  crtWin(&cli->stat_win, cli->stat_win_size, cli->stat_win_pos);

  sliInfoStorage(&cli, false);
}

/**
 * @brief ncurses установки.
 */
void initCurses(void) {
  initscr();
  noecho();
  nodelay(stdscr, true);
  keypad(stdscr, true);
  curs_set(0);
  if (has_colors()) {
    start_color();
    {
      init_color(ColorGray, 450, 450, 450);
      init_color(ColorRed, 976, 0, 0);
      init_color(ColorOrange, 976, 644, 0);
      init_color(ColorYellow, 976, 976, 0);
      init_color(ColorPink, 976, 0, 976);
      init_color(ColorGreen, 0, 976, 0);
      init_color(ColorBlue, 0, 0, 976);
      init_color(ColorPurple, 578, 0, 824);
    }
    init_pair(ZERO, ColorGray, 0);
    init_pair(FULL, ColorGray, ColorGray);
    init_pair(IBLOCK, ColorBlue, ColorBlue);
    init_pair(OBLOCK, ColorYellow, ColorYellow);
    init_pair(TBLOCK, ColorPurple, ColorPurple);
    init_pair(JBLOCK, ColorPink, ColorPink);
    init_pair(LBLOCK, ColorOrange, ColorOrange);
    init_pair(ZBLOCK, ColorGreen, ColorGreen);
    init_pair(SBLOCK, ColorRed, ColorRed);
  }
  clear();
  refresh();
}

/**
 * @brief Удаление элемент структуры cliTetris.
 */
void removeCli(cliTetris *cli) {
  delwin(cli->game_win);
  delwin(cli->stat_win);

  memset(cli, 0, sizeof(*cli));

  sliInfoStorage(&cli, true);

  endwin();
}

/**
 * @brief  Нажатие игрока.
 */
UserInput_t getUserInput(void) {
  UserInput_t return_user_input = {};
  cliTetris *cli = NULL;
  sliInfoStorage(&cli, false);
  if (cli) {
    switch (getch()) {
      case 10:
        return_user_input.action = Start;
        return_user_input.hold = true;
        break;
      case 27:
        return_user_input.action = Terminate;
        return_user_input.hold = true;
        break;
      case ' ':
        return_user_input.action = Action;
        return_user_input.hold = true;
        break;
      case KEY_UP:
        return_user_input.action = Up;
        return_user_input.hold = true;
        break;
      case KEY_DOWN:
        return_user_input.action = Down;
        return_user_input.hold = true;
        break;
      case KEY_LEFT:
        return_user_input.action = Left;
        return_user_input.hold = true;
        break;
      case KEY_RIGHT:
        return_user_input.action = Right;
        return_user_input.hold = true;
        break;
      case 'p':
      case 'P':
        return_user_input.action = Pause;
        return_user_input.hold = true;
        break;
      default:
        return_user_input.hold = false;
        break;
    }
  }

  return return_user_input;
}

/**
 * @brief Отрисовка текущего состояния игры.
 */
void printCurrentState(GameInfo_tetris game_info) {
  cliTetris *cli = NULL;
  sliInfoStorage(&cli, false);
  if (cli) {
    if (game_info.level == LEVEL_START) {
      printStartPreview();
    } else if (game_info.level == LEVEL_END) {
      printEndPreview();
    } else if (game_info.pause) {
      printPausePreview();
    } else {
      werase(cli->game_win);
      werase(cli->stat_win);
      for (int i = 0; i < ROWS_WIN; i++) {
        wmove(cli->game_win, cli->game_win_pos.y + i, WIN_X);
        for (int j = 0; j < COLUMNS_WIN; j++) {
          if (game_info.field[i][j]) {
            printFieldCeil(cli->game_win,
                           FULL_SYMBOL | COLOR_PAIR(game_info.field[i][j]));
          } else {
            printFieldCeil(cli->game_win, EMPTY_SYMBOL | COLOR_PAIR(ZERO));
          }
        }
      }
      statWinPrint(cli, &game_info);
      wborder(cli->game_win, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL,
              FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL);

      wrefresh(cli->game_win);
    }
  }
}

/**
 * @brief Окно Статитстики
 */
void statWinPrint(cliTetris *cli, GameInfo_tetris *game_info) {
  if (cli && game_info) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      wmove(cli->stat_win, cli->stat_win_pos.y + i,
            (cli->stat_win_size.x / 2 - (BLOCK_SIZE / 2)) + WIN_X - 3);
      for (int j = 0; j < BLOCK_SIZE; j++) {
        if (game_info->next[i][j]) {
          printFieldCeil(cli->stat_win,
                         FULL_SYMBOL | COLOR_PAIR(game_info->next[i][j]));
        } else {
          printFieldCeil(cli->stat_win, EMPTY_SYMBOL);
        }
      }
    }
    wmove(cli->stat_win, cli->stat_win_pos.y + 4, 0);
    wprintw(cli->stat_win, "%*s\n%*d\n\n", (int)cli->stat_win_size.x / 2 + 4,
            "High score", (int)cli->stat_win_size.x / 2 + 1,
            game_info->high_score);
    wprintw(cli->stat_win, "%*s\n%*d\n\n", (int)cli->stat_win_size.x / 2 + 2,
            "Score", (int)cli->stat_win_size.x / 2 + 1, game_info->score);
    wprintw(cli->stat_win, "%*s\n%*d\n\n", (int)cli->stat_win_size.x / 2 + 2,
            "Level", (int)cli->stat_win_size.x / 2 + 1, game_info->level);
    wprintw(cli->stat_win, "%*s\n%*d\n\n", (int)cli->stat_win_size.x / 2 + 2,
            "Speed", (int)cli->stat_win_size.x / 2 + 1, game_info->speed);
    wborder(cli->stat_win, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL,
            FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL, FULL_SYMBOL);
    wrefresh(cli->stat_win);
  }
}
