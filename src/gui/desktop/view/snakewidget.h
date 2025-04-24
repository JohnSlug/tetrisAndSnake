#ifndef SNAKEWIDGET_H
#define SNAKEWIDGET_H
#include "mainwindow.h"
#include <QDir>
#include <QStandardPaths>

#include "../../../brick_game/snake/controller.h"  // Включаем контроллер змейки
#include "../../../brick_game/snake/snake_logic.h"  // Включаем логику змейки


/**
 * @brief Виджет для отображения игры "Змейка" в Qt.
 * Отрисовывает поле, обрабатывает ввод и обновляет состояние.
 */
class SnakeWidget : public QWidget {
  Q_OBJECT

 public:
  explicit SnakeWidget(QWidget *parent = nullptr);
  ~SnakeWidget();

 protected:
  /// @brief Отрисовывает игровое поле и элементы.
  void paintEvent(QPaintEvent *event) override;
  /// @brief Обрабатывает клавиши управления змейкой.
  void keyPressEvent(QKeyEvent *event) override;
  /// @brief Обновляет игровое состояние (движение, коллизии).

  void updateGameState();

 signals:
  void gameOver();

 private:
  int loadHighScore();
  void saveHighScore(int score);
  QString getHighScorePath();
  void initGame();
  void drawField(QPainter &painter);
  void drawSidePanel(QPainter &painter);
  s21::SnakeLogic snakeLogic;
  QTimer *timer;
  GameState_t gameState_;
  int interval;
};

#endif  // SNAKEWIDGET_H
