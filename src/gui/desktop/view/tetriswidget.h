#ifndef TETRISWIDGET_H
#define TETRISWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <QStandardPaths>
#include <QDir>

extern "C" {
#include "../../../brick_game/tetris/tetris.h"
}

#define SIZE_FIGURE 4
#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
/**
 * @brief Виджет для отображения игры "Тетрис" в Qt.
 * Управляет логикой, отрисовкой фигур и интерфейсом.
 */
class TetrisWidget : public QWidget {
    Q_OBJECT

public:
    explicit TetrisWidget(QWidget *parent = nullptr);
    ~TetrisWidget();

    signals:
            void gameOver();

protected:
    /// @brief Отрисовывает текущую и следующую фигуру, поле.
    void paintEvent(QPaintEvent *event) override;
    /// @brief Обрабатывает клавиши управления (поворот, перемещение).
    void keyPressEvent(QKeyEvent *event) override;

private slots:
            /// @brief Обновляет состояние игры (падение фигуры, проверка линий).
            void updateGameState();

private:
    void initHighScore();
    int loadHighScore();
    void saveHighScore(int score);
    QString getHighScorePath();
    void initGame();
    void drawField(QPainter &painter);
    void drawNextFigure(QPainter &painter);
    void drawFigure(QPainter &painter);
    void drawSidePanel(QPainter &painter);
    void moveFigureHorizontal(Figure *figure, int direction, GameInfo_t *gameInfo);
    void drawCurrentFigure(QPainter &painter);
    QTimer *timer;
    tetris_t *tetris;
    GameInfo_t *gameInfo;
    GameState_t *gameState;
    Figure *figure;
    int interval;
};

#endif // TETRISWIDGET_H