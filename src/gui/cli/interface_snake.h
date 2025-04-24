#ifndef INTERFACE_SNAKE_H
#define INTERFACE_SNAKE_H

#include "../../brick_game/snake/controller.h"
#include "../../brick_game/snake/snake_logic.h"

namespace s21 {
/**
 * @brief Представление (интерфейс) игры "Змейка".
 * Отрисовывает поле, меню, панель статистики и обрабатывает UI.
 */
class SnakeController;

class SnakeGameView {
 public:
  /// @brief Конструктор, связывающий представление с контроллером.

  SnakeGameView(SnakeController& controller);
  /// @brief Отрисовывает игровое поле.
  void drawField();
  /// @brief Отображает экран паузы.
  void pauseGame();
  /// @brief Показывает стартовое меню.
  void prestartGameState();
  /// @brief Отображает экран завершения игры (поражение).
  void gameOver();
  /// @brief Отображает экран победы.
  void gameVictory();
  void drawSidePanel();
  /// @brief Обновляет интерфейс (поле + панель).
  void redrawField();
  /// @brief Запускает игровой цикл.
  void gameLoopS();

 private:
  SnakeController& controller;
};

}  // namespace s21

#endif  // INTERFACE_SNAKE_H
