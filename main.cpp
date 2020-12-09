#include "mainwindow.h"

//After many strange attempts, I am just too afraid to delete any headers even if they are unnecessary

#include <QSqlTableModel>
#include <QDebug>
#include <QStringList>
#include <QApplication>
#include <QTableView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
