#include "tetriswidget.h"

#include <QDebug>
#include <QPainter>

#include "../../../brick_game/tetris/tetris.h"  // Подключаем ваш заголовочный файл с логикой игры
/**
 * @brief Конструктор виджета Tetris.
 *
 * Инициализирует таймер, структуру tetris_t и запускает игру.
 * Связывает сигналы таймера с обновлением состояния.
 */
TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget(parent),
      timer(new QTimer(this)),
      interval(500)

{
  tetris = new tetris_t();
  if (!tetris) {
    qCritical() << "Failed to allocate tetris structure!";
    return;
  }
  connect(timer, &QTimer::timeout, this, &TetrisWidget::updateGameState);
  timer->start(interval);
  initGame();
}
/**
 * @brief Деструктор виджета Tetris.
 *
 * Освобождает ресурсы, связанные с структурой tetris_t, и останавливает таймер.
 */
TetrisWidget::~TetrisWidget() {
  removeTetris(tetris);
  delete tetris;
}
/**
 * @brief Инициализирует состояние игры.
 *
 * Сбрасывает счёт, уровень, скорость и генерирует первую фигуру.
 * Переводит игру в состояние SPAWN_STATE.
 */
void TetrisWidget::initGame() {
  if (initTetris(tetris) != SUCCESS) {
    qCritical() << "Ошибка при инициализации игры!";
    timer->stop();
    return;
  }

  tetris->score = 0;
  tetris->high_score = loadHighScore();
  tetris->level = 1;
  tetris->speed = 1;
  tetris->pause = 0;

  getRandomTetramino(tetris->next_block);
  tetris->state = SPAWN_STATE;
}
/**
 * @brief Обработчик события отрисовки виджета.
 *
 * Рисует игровое поле, боковую панель, следующую фигуру и текущую фигуру.
 * Отображает экраны паузы и завершения игры.
 */
void TetrisWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  drawField(painter);
  drawSidePanel(painter);
  drawNextFigure(painter);
  drawCurrentFigure(painter);

  if (tetris->pause) {
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 50));
    painter.drawText(70, 150, "PAUSE");
  } else if (tetris->state == END_GAME_STATE) {
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 50));
    painter.drawText(0, 150, "GAME OVER");
  }
}
/**
 * @brief Отрисовывает игровое поле.
 *
 * Заполняет ячейки поля цветом в зависимости от их состояния (пустые/занятые).
 */
void TetrisWidget::drawField(QPainter &painter) {
  for (int i = 0; i < ROWS_WIN; ++i) {
    for (int j = 0; j < COLUMNS_WIN; ++j) {
      QRect rect(j * 20, i * 20, 20, 20);
      painter.fillRect(rect, tetris->field[i][j] ? Qt::blue : Qt::white);
    }
  }
}
/**
 * @brief Отрисовывает следующую фигуру в боковой панели.
 *
 * Отображает предварительный просмотр фигуры, которая появится следующей.
 */
void TetrisWidget::drawNextFigure(QPainter &painter) {
  int startX = (COLUMNS_WIN * 20) + 40;
  int startY = 100;

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      if (tetris->next_block[i][j] != ZERO) {
        QRect rect(startX + j * 20, startY + i * 20, 20, 20);
        painter.fillRect(rect, Qt::blue);
      }
    }
  }
}
/**
 * @brief Отрисовывает текущую падающую фигуру.
 *
 * Отображает фигуру в её текущей позиции на игровом поле.
 */
void TetrisWidget::drawCurrentFigure(QPainter &painter) {
  int startX = tetris->current_block_pos.x * 20;
  int startY = tetris->current_block_pos.y * 20;

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      if (tetris->current_block[i][j] != ZERO) {
        QRect rect(startX + j * 20, startY + i * 20, 20, 20);
        painter.fillRect(rect, Qt::blue);
      }
    }
  }
}

/**
 * @brief Отрисовывает боковую панель с информацией.
 *
 * Показывает счёт, рекорд, уровень, скорость и следующую фигуру.
 */
void TetrisWidget::drawSidePanel(QPainter &painter) {
  tetris->high_score = loadHighScore();
  painter.drawText(220, 20, "Score: " + QString::number(tetris->score));
  painter.drawText(220, 40,
                   "High Score: " + QString::number(tetris->high_score));
  painter.drawText(220, 60, "Level: " + QString::number(tetris->level));
  painter.drawText(220, 80, "Speed: " + QString::number(tetris->speed));
  painter.drawText(220, 100, "Next: ");
}
/**
 * @brief Обработчик нажатий клавиш.
 *
 * Управляет перемещением фигур (влево/вправо/вниз), поворотом, паузой
 * и выходом из игры. Проверяет коллизии и обновляет состояние.
 */
void TetrisWidget::keyPressEvent(QKeyEvent *event) {
  if (tetris->state == TERMINATE_STATE) {
    emit gameOver();
    return;
  }

  qDebug() << "Key pressed:" << event->key();
  if (tetris->state == TERMINATE_STATE) {
    emit gameOver();
  } else {
    switch (event->key()) {
      case Qt::Key_Left:
        mvLeft(tetris);
        break;
      case Qt::Key_Right:
        mvRight(tetris);
        break;
      case Qt::Key_Down:
        mvDown(tetris);
        break;
      case Qt::Key_Up:
        if (tetris->pause == 0) {
          mvAction(tetris);
        }
        break;
      case Qt::Key_Space:
        userInput(Start, true);
        break;
      case Qt::Key_P:
        tetris->pause = !tetris->pause;
        break;
      case Qt::Key_S:
        tetris->state = MOVING_STATE;
        break;
      case Qt::Key_Escape:
        tetris->state = TERMINATE_STATE;
        break;
      default:

        break;
    }
    if (tetris->is_fall_time && tetris->pause == 0) {
      ++tetris->current_block_pos.y;
      tetris->is_fall_time = false;
    }
    if (checkingBlockCollisionWithField(
            tetris->current_block, tetris->current_block_pos, tetris->field)) {
      --tetris->current_block_pos.y;
      tetris->state = ATTACHING_STATE;
    }
    updateGameState();
  }
}
/**
 * @brief Обновляет состояние игры.
 *
 * Обрабатывает автоматическое падение фигуры, проверяет завершение игры
 * и обновляет интерфейс. Останавливает таймер при завершении.
 */
void TetrisWidget::updateGameState() {
  setFocus();
  selectNextState(tetris);

  if (tetris->state == MOVING_STATE && !tetris->pause) {
    mvDown(tetris);
  }
  if (tetris->state == TERMINATE_STATE) {
    if (tetris->score > tetris->high_score) {
      tetris->high_score = tetris->score;
      saveHighScore(tetris->high_score);
    }
    timer->stop();
    emit gameOver();
    return;
  }
  if (!tetris->pause) {
    updCurrState();
  }

  update();
}

void TetrisWidget::saveHighScore(int score) {
  QFile file(getHighScorePath());
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << score;
    file.close();
  }
}

int TetrisWidget::loadHighScore() {
  initHighScore();
  QFile file(getHighScorePath());
  int highScore = 0;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    stream >> highScore;
    file.close();
  }
  return highScore;
}

QString TetrisWidget::getHighScorePath() {
  QString dirPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir dir(dirPath);
  if (!dir.exists()) {
    dir.mkpath(dirPath);
  }
  return dirPath + "/highScore.txt";
}

void TetrisWidget::initHighScore() {
  QString path = getHighScorePath();
  if (!QFile::exists(path)) {
    saveHighScore(123);  // Создаем файл с нулевым рекордом
  }
}