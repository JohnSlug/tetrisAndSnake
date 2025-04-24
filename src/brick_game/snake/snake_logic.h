#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../tetris/tetris.h"

namespace s21 {
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define SIZE_FIGURE 4
#define NUM_FIGURE 7
#define VALUE_X FIELD_WIDTH / 2 - SIZE_FIGURE / 2
#define VALUE_Y 0
#define KEY_SPACE 32

/**
 * @brief Хранит координаты объекта на игровом поле.
 */

struct Coordinates {
  int x;
  int y;
};
/**
 * @brief Логика игры "Змейка".
 * Управляет состоянием игры, змейкой, яблоками, счетом и уровнями.
 */
class SnakeLogic {
 private:
 public:
  GameInfo_t gameInfo_;    ///< Информация об игре (счет, поле, уровень).
  GameState_t gameState_;  ///< Текущее состояние игры (пауза, игра и т.д.).
  SnakeLogic();
  ~SnakeLogic();
  /// @brief Создает динамическое игровое поле.
  int** createMatrixField();
  /// @brief Инициализирует игру (сброс параметров, генерация змейки и яблока).
  void initGame();
  /// @brief Обнуляет игровое поле.
  void resetMatrixField();
  /// @brief Перемещает змейку и обрабатывает коллизии.
  void moveSnake();
  /// @brief Меняет направление движения змейки.
  void rotateDirection(int);
  /// @brief Проверяет столкновение головы змейки с телом.
  bool checkCollision();
  /// @brief Возвращает текущее состояние игры.
  GameState_t& getGameState();
  /// @brief Возвращает игровую информацию.
  GameInfo_t getGameInfo() const;
  void setGameState(GameState_t state);
  std::vector<Coordinates> body;
  std::vector<Coordinates>& getSnake() { return body; }
  Coordinates apple;
  Coordinates getApple() const { return apple; }
  /// @brief Генерирует змейку в центре поля.

  void generateSnake();
  /// @brief Создает яблоко на свободной позиции.
  void generateApple();
  void updateLevel();
  void checkVictory();
  void saveGameScore();
  int readGameScore();
  int direction_;
};
}  // namespace s21

void snake();
void printMenu();

#endif  // SNAKE_LOGIC_H