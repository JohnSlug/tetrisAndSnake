#include "interface_snake.h"

namespace s21 {
SnakeGameView::SnakeGameView(SnakeController& controller)
    : controller(controller) {}
/**
 * @brief Основной игровой цикл для змейки.
 *
 * Инициализирует предстартовое состояние, обрабатывает ввод пользователя,
 * обновляет состояние игры и перерисовывает поле. Управляет задержкой
 * между кадрами в зависимости от уровня. Цикл продолжается до выхода из игры.
 */
void SnakeGameView::gameLoopS() {
  prestartGameState();
  controller.initGame();

  controller.setGameState(GAME_PRESTART);

  while (controller.getGameState() != GAME_EXIT) {
    UserActionButton action = controller.handleInput();
    controller.userInputS(action, true);
    controller.updateGameState();
    redrawField();

    int delay = 800 - (controller.getGameInfo().level - 1) * 50;
    timeout(delay);
  }
}
/**
 * @brief Отрисовывает игровое поле.
 *
 * Проходит по всем ячейкам поля и отображает их в зависимости от содержимого:
 * пустое пространство, яблоко или тело змейки. Использует цветовые пары
 * ncurses.
 */
void SnakeGameView::drawField() {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (controller.getGameInfo().field[i][j] == 0) {
        attron(COLOR_PAIR(1));  // Включаем цвет
        mvaddch(i, j, ' ');     // Пустое пространство
      } else if (controller.getGameInfo().field[i][j] == 1) {
        attron(COLOR_PAIR(2));   // Включаем цвет для яблока
        mvaddch(i, j, ' ');      // Яблоко
        attroff(COLOR_PAIR(2));  // Выключаем цвет
      } else if (controller.getGameInfo().field[i][j] == 8) {
        attron(COLOR_PAIR(3));   // Включаем цвет для тела змейки
        mvaddch(i, j, ' ');      // Тело змейки
        attroff(COLOR_PAIR(3));  // Выключаем цвет
      }
    }
  }
}
/**
 * @brief Отображает предстартовое меню игры.
 *
 * Выводит приветственное сообщение, инструкции по управлению и кнопкам.
 * Ожидает нажатия 's' для начала игры.
 */
void SnakeGameView::prestartGameState() {
  clear();
  printw("Welcome to Snake!\n");
  printw("Press 's' to start the game.\n");
  printw("Use the arrows to control the shapes.\n");
  printw("Press 'p' to pause/resume the game.\n");
  printw("Press 'q' to exit the game.\n");
  refresh();
}
/**
 * @brief Отображает экран паузы.
 *
 * Показывает сообщение о паузе и инструкцию для возобновления игры.
 */
void SnakeGameView::pauseGame() {
  clear();
  printw("Game Paused\n");
  printw("Press 'p' to resume the game.\n");
  refresh();
}
/**
 * @brief Отображает экран завершения игры (поражение).
 *
 * Показывает итоговый счёт и предлагает выйти из игры.
 */
void SnakeGameView::gameOver() {
  clear();
  printw("Game Over\n");
  printw("Your Score: %d\n", controller.getGameInfo().score);
  printw("Press 'q' to quit the game.\n");
  refresh();
}
/**
 * @brief Отображает экран победы.
 *
 * Показывает поздравление и итоговый счёт. Предлагает выйти из игры.
 */
void SnakeGameView::gameVictory() {
  clear();
  printw("You won!Congratulations!\n");
  printw("Your Score: %d\n", controller.getGameInfo().score);
  printw("Press 'q' to quit the game.\n");
  refresh();
}
/**
 * @brief Отрисовывает боковую панель с информацией.
 *
 * Отображает текущий счёт, рекорд, уровень и скорость игры.
 */
void SnakeGameView::drawSidePanel() {
  mvprintw(0, FIELD_WIDTH + 2, "Score: %d", controller.getGameInfo().score);
  mvprintw(1, FIELD_WIDTH + 2, "High Score: %d",
           controller.getGameInfo().high_score);
  mvprintw(2, FIELD_WIDTH + 2, "Level: %d", controller.getGameInfo().level);
  mvprintw(3, FIELD_WIDTH + 2, "Speed: %d", controller.getGameInfo().speed);
}
/**
 * @brief Обновляет экран игры.
 *
 * Перерисовывает поле, боковую панель и проверяет текущее состояние игры
 * (пауза, поражение, победа) для отображения соответствующих экранов.
 */
void SnakeGameView::redrawField() {
  clear();
  drawField();
  drawSidePanel();

  if (controller.getGameState() == GAME_PAUSED) {
    pauseGame();
  }
  if (controller.getGameState() == GAME_OVER) {
    gameOver();
  }
  if (controller.getGameState() == GAME_WON) {
    gameVictory();
  }
  refresh();
}

}  // namespace s21
/**
 * @brief Отображает главное меню выбора игры.
 *
 * Показывает варианты "Tetris" и "Snake", а также кнопку для выхода.
 */
void printMenu() {
  clear();
  mvprintw(5, 5, "Choose a game:");
  mvprintw(7, 5, "1. Tetris");
  mvprintw(8, 5, "2. Snake");
  mvprintw(10, 5, "Press 'q' to quit.");
  refresh();
}
