#include <QApplication>

#include "mainwindow.h"
/**
 * @brief Точка входа в приложение.
 * Создает и отображает главное окно.
 */
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
