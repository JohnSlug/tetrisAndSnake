#include "../../brick_game/snake/controller.h"
#include "../../brick_game/snake/snake_logic.h"
#include "interface_snake.h"
/**
 * @brief Основная функция для запуска игры "Змейка".
 *
 * Настраивает окружение ncurses (цвета, ввод), инициализирует MVC-компоненты
 * и запускает игровой цикл. Завершает работу при выходе из игры.
 */
void snake() {
  srand(time(NULL));
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();

  init_pair(1, COLOR_BLACK, COLOR_CYAN);   // Цвет фона поля
  init_pair(2, COLOR_BLACK, COLOR_RED);    // Цвет для яблока
  init_pair(3, COLOR_BLACK, COLOR_GREEN);  // Цвет для тела змейки

  s21::SnakeLogic model;
  s21::SnakeController controller(model);
  s21::SnakeGameView view(controller);

  view.gameLoopS();
  endwin();
}
