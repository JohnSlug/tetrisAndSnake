#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFont>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {

/**
 * @brief Главное окно приложения.
 * Содержит меню выбора игр ("Тетрис" и "Змейка") и управляет навигацией.
 */
class MainWindow;
}  // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 protected:
  /// @brief Обрабатывает нажатия клавиш (для переключения между играми).
  void keyPressEvent(QKeyEvent *event) override;

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  /// @brief Запускает игру "Тетрис".
  void Tetris();
  /// @brief Запускает игру "Змейка".
  void Snake();
  void on_widget_customContextMenuRequested(const QPoint &pos);

 private:
  Ui::MainWindow *ui;
  void createMenu();
  /// @brief Возвращает пользователя в главное меню.

  void returnToMenu();
};
#endif  // MAINWINDOW_H
