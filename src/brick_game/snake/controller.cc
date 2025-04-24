#include "controller.h"

namespace s21 {
/**
 * @brief Конструктор контроллера, связывающий его с игровой логикой.
 * @param model Ссылка на экземпляр игровой логики (SnakeLogic).
 */

SnakeController::SnakeController(SnakeLogic& model) : model(model) {}

/**
 * @brief Обрабатывает ввод пользователя и преобразует его в действия.
 * @return UserActionButton — действие (направление движения, старт, пауза и
 * т.д.).
 * @note Использует библиотеку ncurses (getch()) для получения кода клавиши.
 * - Стрелки: управление направлением.
 * - 's': старт игры.
 * - 'p': пауза/продолжение.
 * - 'q': выход из игры.
 */
UserActionButton SnakeController::handleInput() {
  int ch = getch();
  UserActionButton action = static_cast<UserActionButton>(-1);
  switch (ch) {
    case KEY_UP:
      action = Up;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case 's':
      action = Start;
      break;
    case 'p':
      action = Pause;
      break;
    case 'q':
      action = Terminate;
      break;
  }
  return action;
}
/**
 * @brief Обрабатывает действие пользователя в зависимости от состояния игры.
 * @param action Действие (из UserActionButton).
 * @param hold Флаг удержания клавиши (не используется в текущей реализации).
 *
 * Логика:
 * - Start: Переводит игру из состояния GAME_PRESTART в GAME_START.
 * - Pause: Переключает между GAME_MOVING и GAME_PAUSED.
 * - Направления: Изменяет направление змейки (с проверкой на разворот на 180°).
 * - Terminate: Завершает игру в разрешенных состояниях (пауза, завершение и
 * т.д.).
 */
void SnakeController::userInputS(UserActionButton action, bool hold) {
  GameState_t& state = model.getGameState();

  if (action == Start && state == GAME_PRESTART && hold) {
    state = GAME_START;
  } else if (action == Pause) {
    if (state == GAME_MOVING) {
      state = GAME_PAUSED;
    } else if (state == GAME_PAUSED) {
      state = GAME_MOVING;
    }
  } else if (action == Left || action == Right || action == Up ||
             action == Down) {
    if (state == GAME_MOVING || state == GAME_SPAWN) {
      int newDirection;
      switch (action) {
        case Up:
          newDirection = 0;
          break;
        case Right:
          newDirection = 1;
          break;
        case Down:
          newDirection = 2;
          break;
        case Left:
          newDirection = 3;
          break;
        default:
          return;
      }
      if (newDirection != (model.direction_ + 2) % 4) {
        model.rotateDirection(newDirection);
      }
    }
  } else if (action == Terminate) {
    if (state == GAME_PRESTART || state == GAME_PAUSED || state == GAME_OVER ||
        state == GAME_WON) {
      state = GAME_EXIT;
    }
  }
}
/**
 * @brief Обновляет игровое состояние на основе текущей логики.
 *
 * Этапы:
 * 1. GAME_START → Инициализация игры (initGame()).
 * 2. GAME_SPAWN → Генерация яблока.
 * 3. GAME_MOVING → Движение змейки, проверка коллизий и победы.
 * 4. GAME_OVER → Сохранение рекорда (если счет побил предыдущий).
 */
void SnakeController::updateGameState() {
  GameState_t& state = model.getGameState();

  if (state == GAME_START) {
    model.initGame();
    state = GAME_SPAWN;
  }

  if (state == GAME_SPAWN) {
    model.generateApple();
    state = GAME_MOVING;
  }

  if (state == GAME_MOVING) {
    model.moveSnake();
    if (model.checkCollision()) {
      state = GAME_OVER;
    } else {
      model.checkVictory();
    }
  }

  if (state == GAME_OVER) {
    if (model.getGameInfo().score > model.getGameInfo().high_score) {
      model.saveGameScore();
    }
  }
}

}  // namespace s21
