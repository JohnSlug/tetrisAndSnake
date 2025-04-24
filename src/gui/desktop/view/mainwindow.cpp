#include "mainwindow.h"

#include "snakewidget.h"
#include "tetriswidget.h"
#include "ui_mainwindow.h"
/**
 * @brief Конструктор главного окна.
 * Инициализирует интерфейс и создает меню.
 * @param parent Родительский виджет.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  createMenu();
}

void MainWindow::on_widget_customContextMenuRequested(const QPoint& pos) {
  Q_UNUSED(pos);
}

/**
 * @brief Деструктор главного окна.
 */
MainWindow::~MainWindow() { delete ui; }
/**
 * @brief Создает главное меню с кнопками выбора игр.
 * - Кнопки "Tetris" и "Snake".
 * - Надпись с инструкцией для выхода.
 */

void MainWindow::createMenu() {
  QPushButton* button1 = new QPushButton("Press key '1' to start Tetris");
  QPushButton* button2 = new QPushButton("Press key '2' to start Snake");
  QLabel* label1 = new QLabel("Press key 'esc' to exit");

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(button1);
  layout->addWidget(button2);
  layout->addWidget(label1);

  ui->widget->setLayout(layout);

  connect(button1, &QPushButton::clicked, this, &MainWindow::Tetris);
  connect(button2, &QPushButton::clicked, this, &MainWindow::Snake);

  button1->setFixedSize(450, 100);
  button2->setFixedSize(450, 100);
  button1->setStyleSheet(
      "background-color: white; color: green; font-size: 20px;");
  button2->setStyleSheet(
      "background-color: white; color: blue; font-size: 20px;");
  label1->setStyleSheet(
      "font-size: 30px; color: red; qproperty-alignment: AlignCenter;");
}
/**
 * @brief Обрабатывает нажатия клавиш для навигации:
 * - '1': запуск Tetris.
 * - '2': запуск Snake.
 * - 'Esc': выход из приложения.
 * @param event Событие нажатия клавиши.
 */
void MainWindow::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_1:
      Tetris();
      break;
    case Qt::Key_2:
      Snake();
      break;
    case Qt::Key_Escape:
      close();
      break;
    default:
      QMainWindow::keyPressEvent(event);
  }
}
/**
 * @brief Запускает игру "Tetris" и заменяет центральный виджет.
 */
void MainWindow::Tetris() {
  TetrisWidget* gameWidget = new TetrisWidget(this);
  connect(gameWidget, &TetrisWidget::gameOver, this, &MainWindow::returnToMenu);
  setCentralWidget(gameWidget);
}
/**
 * @brief Запускает игру "Snake" и заменяет центральный виджет.
 */
void MainWindow::Snake() {
  SnakeWidget* gameWidget = new SnakeWidget(this);
  connect(gameWidget, &SnakeWidget::gameOver, this, &MainWindow::returnToMenu);
  setCentralWidget(gameWidget);
}
/**
 * @brief Возвращает пользователя в главное меню после завершения игры.
 * Создает новый виджет меню и настраивает кнопки.
 */
void MainWindow::returnToMenu() {
  QWidget* menuWidget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(menuWidget);

  QPushButton* button1 = new QPushButton("Prees key '1' to start Tetris");
  QPushButton* button2 = new QPushButton("Prees key '2' to start Snake");
  QLabel* label1 = new QLabel("Want more?");
  QLabel* label2 = new QLabel("Prees key 'esc' to exit");
  layout->addWidget(label1);
  layout->addWidget(button1);
  layout->addWidget(button2);

  layout->addWidget(label2);

  connect(button1, &QPushButton::clicked, this, &MainWindow::Tetris);
  connect(button2, &QPushButton::clicked, this, &MainWindow::Snake);
  button1->setFixedSize(450, 100);
  button2->setFixedSize(450, 100);
  button1->setStyleSheet(
      "background-color: white; color: green; font-size: 20px;");
  button2->setStyleSheet(
      "background-color: white; color: blue; font-size: 20px;");
  label1->setStyleSheet(
      "font-size: 30px; color: white; qproperty-alignment: AlignCenter;");
  label2->setStyleSheet(
      "font-size: 30px; color: red; qproperty-alignment: AlignCenter;");
  setCentralWidget(menuWidget);
}
