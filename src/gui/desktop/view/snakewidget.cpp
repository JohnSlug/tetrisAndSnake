#include "snakewidget.h"
/**
 * @brief Конструктор виджета "Змейка".
 * Инициализирует таймер, устанавливает интервал обновления и запускает игру.
 * @param parent Родительский виджет.
 */
SnakeWidget::SnakeWidget(QWidget *parent) : QWidget(parent) {
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &SnakeWidget::updateGameState);
  interval = 350;
  timer->start(interval);
  initGame();
}
/**
 * @brief Деструктор виджета "Змейка".
 */
SnakeWidget::~SnakeWidget() {}
/**
 * @brief Инициализирует игровую логику и устанавливает начальное состояние
 * игры.
 */
void SnakeWidget::initGame() {
  snakeLogic.initGame();
  snakeLogic.setGameState(GAME_START);
  snakeLogic.gameInfo_.high_score = 7;
}
/**
 * @brief Отрисовывает игровое поле, змейку, яблоко и UI-элементы.
 * @param event Событие отрисовки.
 */
void SnakeWidget::paintEvent(QPaintEvent *event) {
  snakeLogic.gameInfo_.high_score = snakeLogic.readGameScore();
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  drawField(painter);
  drawSidePanel(painter);
  if (snakeLogic.getGameState() == GAME_PAUSED) {
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 50));
    painter.drawText(70, 150, "PAUSE");
  }

  if (snakeLogic.getGameState() == GAME_OVER) {
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 50));
    painter.drawText(0, 150, "GAME OVER");
    snakeLogic.saveGameScore();
  }
}
/**
 * @brief Обрабатывает нажатия клавиш для управления змейкой.
 * @param event Событие нажатия клавиши.
 *
 * - Стрелки: изменение направления змейки.
 * - 'S': старт игры.
 * - 'P': пауза/продолжение.
 * - 'Q': выход (в состоянии паузы или завершения).
 */
void SnakeWidget::keyPressEvent(QKeyEvent *event) {
  setFocus();
  if (snakeLogic.getGameState() == GAME_OVER) {
    emit gameOver();
  } else {
    switch (event->key()) {
      case Qt::Key_Up:
        snakeLogic.rotateDirection(0);
        break;
      case Qt::Key_Right:
        snakeLogic.rotateDirection(1);
        break;
      case Qt::Key_Down:
        snakeLogic.rotateDirection(2);
        break;
      case Qt::Key_Left:
        snakeLogic.rotateDirection(3);
        break;
      case Qt::Key_S:
        snakeLogic.setGameState(GAME_MOVING);
        break;
      case Qt::Key_P:
        if (snakeLogic.getGameState() == GAME_MOVING) {
          snakeLogic.setGameState(GAME_PAUSED);
        } else if (snakeLogic.getGameState() == GAME_PAUSED) {
          snakeLogic.setGameState(GAME_MOVING);
        }
        break;
      case Qt::Key_Q:
        if (snakeLogic.getGameState() == GAME_PAUSED ||
            snakeLogic.getGameState() == GAME_OVER)
          snakeLogic.setGameState(GAME_EXIT);
        break;
    }
    update();
  }
}
/**
 * @brief Обновляет игровое состояние:
 * - Перемещает змейку.
 * - Проверяет съедение яблока и коллизии.
 * - Обновляет уровень и скорость.
 * - Сохраняет рекорд при завершении игры.
 */
void SnakeWidget::updateGameState() {
  setFocus();
  if (snakeLogic.getGameState() == GAME_MOVING) {
    snakeLogic.moveSnake();
    if (snakeLogic.getSnake().front().x == snakeLogic.getApple().x &&
        snakeLogic.getSnake().front().y == snakeLogic.getApple().y) {
      snakeLogic.generateApple();
      snakeLogic.gameInfo_.score++;
      snakeLogic.updateLevel();
    }
    if (snakeLogic.checkCollision()) {
      snakeLogic.setGameState(GAME_OVER);
      emit gameOver();
    } else {
      snakeLogic.checkVictory();
    }
    int newInterval = interval - (snakeLogic.gameInfo_.level * 20);
    timer->start(qMax(newInterval, 200));
  }
  if (snakeLogic.getGameState() == GAME_OVER) {
    if (snakeLogic.gameInfo_.score > snakeLogic.gameInfo_.high_score)
      saveHighScore(snakeLogic.gameInfo_.score);
  }
  update();
}
/**
 * @brief Отрисовывает игровое поле и элементы.
 * @param painter Объект для отрисовки.
 */
void SnakeWidget::drawField(QPainter &painter) {
  painter.fillRect(0, 0, 200, 400, Qt::blue);

  for (const auto &segment : snakeLogic.getSnake()) {
    painter.fillRect(segment.x * 20, segment.y * 20, 20, 20,
                     Qt::green);  // Отрисовка тела змеи
  }
  painter.fillRect(snakeLogic.getApple().x * 20, snakeLogic.getApple().y * 20,
                   20, 20, Qt::red);  // Отрисовка яблока
}
/**
 * @brief Рисует боковую панель с игровой статистикой.
 * @param painter Объект для отрисовки.
 */
void SnakeWidget::drawSidePanel(QPainter &painter) {
  snakeLogic.gameInfo_.high_score = loadHighScore();
  painter.drawText(220, 20,
                   QString("Score: %1").arg(snakeLogic.gameInfo_.score));
  painter.drawText(
      220, 40, QString("High Score: %1").arg(snakeLogic.gameInfo_.high_score));
  painter.drawText(220, 60,
                   QString("Level: %1").arg(snakeLogic.gameInfo_.level));
  painter.drawText(220, 80,
                   QString("Speed: %1").arg(snakeLogic.gameInfo_.speed));
  painter.drawText(220, 100, QString("For start press S"));
  painter.drawText(220, 120, QString("For exit press Q"));
}

void SnakeWidget::saveHighScore(int score) {
  QFile file(getHighScorePath());
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << score;
    file.close();
  }
}

int SnakeWidget::loadHighScore() {
  QFile file(getHighScorePath());
  int highScore = 0;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    stream >> highScore;
    file.close();
  }
  return highScore;
}

QString SnakeWidget::getHighScorePath() {
  QString dirPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir dir(dirPath);
  if (!dir.exists()) {
    dir.mkpath(dirPath);
  }
  return dirPath + "/snake_score.txt";
}
