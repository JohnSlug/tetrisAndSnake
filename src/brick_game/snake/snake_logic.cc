#include "snake_logic.h"
namespace s21 {
/**
 * @brief Конструктор класса SnakeLogic. Инициализирует состояние игры.
 *
 * Инициализирует структуру gameState_.
 */
SnakeLogic::SnakeLogic() : gameState_() {
  //  initGame();
}
/**
 * @brief Создает динамическую матрицу для игрового поля.
 *
 * Выделяет память под двумерный массив размером FIELD_HEIGHT x FIELD_WIDTH.
 * @return Указатель на созданную матрицу. Если размеры некорректны — возвращает
 * nullptr.
 */
int** SnakeLogic::createMatrixField() {
  if (FIELD_WIDTH <= 0 || FIELD_HEIGHT <= 0) {
    return nullptr;
  }
  int** matrix_ = new int*[FIELD_HEIGHT];
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    matrix_[i] = new int[FIELD_WIDTH]();
  }
  return matrix_;
}
/**
 * @brief Деструктор класса SnakeLogic. Освобождает ресурсы.
 *
 * Удаляет динамически выделенную память для игрового поля.
 */
SnakeLogic::~SnakeLogic() {
  if (gameInfo_.field) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      delete[] gameInfo_.field[i];
    }
    delete[] gameInfo_.field;
    gameInfo_.field = nullptr;
  }
}
/**
 * @brief Инициализирует игровые параметры.
 *
 * - Создает поле.
 * - Сбрасывает счет, уровень и скорость.
 * - Загружает рекорд из файла.
 * - Генерирует змейку и яблоко.
 */
void SnakeLogic::initGame() {
  this->gameInfo_.field = createMatrixField();
  this->gameInfo_.score = 0;
  this->gameInfo_.high_score = readGameScore();
  this->gameInfo_.level = 1;
  this->gameInfo_.speed = 1;
  this->gameInfo_.pause = 0;

  resetMatrixField();
  generateApple();
  generateSnake();
  direction_ = 0;
}
/**
 * @brief Сбрасывает игровое поле в начальное состояние (все ячейки = 0).
 */
void SnakeLogic::resetMatrixField() {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      gameInfo_.field[i][j] = 0;
    }
  }
}
/**
 * @brief Генерирует яблоко на случайной свободной позиции.
 *
 * Позиция выбирается до тех пор, пока не будет найдена пустая ячейка.
 */
void SnakeLogic::generateApple() {
  do {
    apple.x = rand() % FIELD_WIDTH;
    apple.y = rand() % FIELD_HEIGHT;
  } while (gameInfo_.field[apple.y][apple.x] != 0);
  gameInfo_.field[apple.y][apple.x] = 1;
}
/**
 * @brief Создает змейку в центре поля.
 *
 * Начальная длина — 4 сегмента. Сегменты размещаются вертикально.
 */
void SnakeLogic::generateSnake() {
  int initialLength = 4;
  int startX = FIELD_WIDTH / 2;
  int startY = FIELD_HEIGHT / 2;

  body.clear();
  for (int i = 0; i < initialLength; i++) {
    body.push_back({startX, startY + i});
    gameInfo_.field[startY + i][startX] = 8;
  }
}
/**
 * @brief Перемещает змейку в текущем направлении.
 *
 * - Проверяет выход за границы поля (GAME_OVER).
 * - При съедании яблока увеличивает длину, обновляет счет и уровень.
 * - Обновляет состояние поля.
 */
void SnakeLogic::moveSnake() {
  Coordinates head = body.front();
  switch (direction_) {
    case 0:
      head.y--;
      break;
    case 1:
      head.x++;
      break;
    case 2:
      head.y++;
      break;
    case 3:
      head.x--;
      break;
    default:
      head.y--;
      break;
  }
  if (head.x < 0 || head.x >= FIELD_WIDTH || head.y < 0 ||
      head.y >= FIELD_HEIGHT) {
    gameState_ = GAME_OVER;
    return;
  }
  if (head.x == apple.x && head.y == apple.y) {
    gameInfo_.score++;
    body.push_back(head);
    generateApple();
    updateLevel();
  } else {
    body.insert(body.begin(), head);
    body.pop_back();
  }
  resetMatrixField();
  for (const auto& segment : body) {
    gameInfo_.field[segment.y][segment.x] =
        8;  // Обновляем поле с сегментами змейки
  }
  gameInfo_.field[apple.y][apple.x] = 1;  // Обновляем поле с яблоком
}
/**
 * @brief Меняет направление движения змейки.
 * @param newDirection Новое направление (0-3: вверх, вправо, вниз, влево).
 * @note Запрещает разворот на 180 градусов (например, вверх → вниз).
 */
void SnakeLogic::rotateDirection(int newDirection) {
  if (newDirection == (direction_ + 2) % 4) return;
  direction_ = newDirection;
}
/**
 * @brief Проверяет столкновение головы змейки с телом.
 * @return true — столкновение, false — нет.
 */
bool SnakeLogic::checkCollision() {
  Coordinates head = body.front();

  for (size_t i = 1; i < body.size(); ++i) {
    if (head.x == body[i].x && head.y == body[i].y) {
      return true;
    }
  }
  return false;
}
/**
 * @brief Возвращает текущее состояние игры (пауза, игра, завершение и т.д.).
 */
GameState_t& SnakeLogic::getGameState() { return gameState_; }
/**
 * @brief Возвращает игровую информацию (счет, поле, уровень и т.д.).
 */
GameInfo_t SnakeLogic::getGameInfo() const { return gameInfo_; }
/**
 * @brief Устанавливает новое состояние игры.
 * @param state Состояние (например, GAME_PAUSED, GAME_OVER).
 */
void SnakeLogic::setGameState(GameState_t state) { gameState_ = state; }
/**
 * @brief Проверяет условие победы (длина змейки >= 200).
 * @note При выполнении условия переводит игру в состояние GAME_WON.
 */
void SnakeLogic::checkVictory() {
  if (body.size() >= 200) {
    gameState_ = GAME_WON;
  }
}
/**
 * @brief Обновляет уровень на основе счета.
 *
 * Уровень = (счет / 5) + 1. Максимальный уровень — 10.
 */
void SnakeLogic::updateLevel() {
  if (gameInfo_.score >= 5) {
    gameInfo_.level = (gameInfo_.score / 5) + 1;
    if (gameInfo_.level > 10) {
      gameInfo_.level = 10;
    }
    gameInfo_.speed = gameInfo_.level;
  }
}
/**
 * @brief Сохраняет рекорд в файл "snake_score.txt".
 * @note Сохранение происходит только если текущий счет превышает предыдущий
 * рекорд.
 */
void SnakeLogic::saveGameScore() {
  if (gameInfo_.score > gameInfo_.high_score) {
    gameInfo_.high_score = gameInfo_.score;
    std::ofstream file("snake_score.txt");
    if (file.is_open()) {
      file << gameInfo_.score;
    }
    file.close();
  }
}

/**
 * @brief Загружает рекорд из файла "snake_score.txt".
 * @return Загруженный рекорд. Если файла нет — возвращает 0.
 */
int SnakeLogic::readGameScore() {
  std::ifstream file("snake_score.txt");
  int score = 0;
  if (file.is_open()) {
    file >> score;
    gameInfo_.high_score = score;
    file.close();
  }
  return score;
}

}  // namespace s21