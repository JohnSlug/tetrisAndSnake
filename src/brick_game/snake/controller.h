#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../../gui/cli/interface_snake.h"
#include "snake_logic.h"

namespace s21 {
/**
 * @brief Контроллер для управления игрой "Змейка".
 * Связывает модель (логику) и представление, обрабатывает ввод и обновляет
 * состояние.
 */
class SnakeController {
 private:
  SnakeLogic& model;

 public:
  /// @brief Инициализирует игру через модель.
  inline void initGame() { model.initGame(); }
  /// @brief Устанавливает текущее состояние игры.
  inline void setGameState(GameState_t state) { model.setGameState(state); }
  /// @brief Возвращает текущее состояние игры.
  inline GameState_t getGameState() { return model.getGameState(); }
  /// @brief Возвращает игровую информацию (счет, уровень и т.д.).
  inline GameInfo_t getGameInfo() const { return model.getGameInfo(); }
  /// @brief Конструктор, связывающий контроллер с моделью.
  SnakeController(SnakeLogic& model);
  /// @brief Обрабатывает ввод пользователя (клавиши).
  UserActionButton handleInput();
  /// @brief Применяет действие пользователя к игре.
  void userInputS(UserActionButton action, bool hold);
  /// @brief Обновляет игровое состояние на основе логики.
  void updateGameState();
};

}  // namespace s21

#endif  // CONTROLLER_H
