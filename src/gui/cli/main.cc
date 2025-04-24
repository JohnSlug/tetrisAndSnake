#include "../../brick_game/snake/snake_logic.h"
#include "../../brick_game/tetris/tetris.h"
#include "interface_snake.h"

extern "C" {
void tetris();
}
/**
 * @brief Точка входа в программу.
 *
 * Инициализирует ncurses, отображает главное меню и обрабатывает выбор
 * пользователя. Запускает выбранную игру (Tetris/Snake) или завершает
 * программу.
 */
int main() {
  initscr();
  int ch;
  while (true) {
    printMenu();
    ch = getch();

    switch (ch) {
      case '1':
        clear();
        tetris();
        break;
      case '2':
        clear();
        snake();
        break;
      case 'q':
        endwin();
        return 0;
    }
  }
  endwin();
  return 0;
}
