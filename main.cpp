#include "mainwindow.h"

#include <QApplication>

/**
 * @brief The main function of the application.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return The exit code of the Qt application (0 for success, non-zero for errors).
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
