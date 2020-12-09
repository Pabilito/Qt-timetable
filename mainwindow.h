#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "entryedit.h"
#include "dictionaryanyentry.h"
#include "dataholder.h"

#include <QSqlTableModel>
#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    EntryEdit* ee;
    dictionaryAnyEntry* any;

private:
    Ui::MainWindow *ui;
    void InitializeModel(QStandardItemModel* model);
    QStringList* teachers;
    QStringList* rooms;
    QStringList* classes;
    QStringList* courses;
    QStandardItemModel* model;
    QVector<DataHolder> data;
    QVector<QVector<DataHolder>> allData;


private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_actionEdit_Dictionary_triggered();
    void on_actionSave_triggered();
    void on_actionLOad_triggered();  
    void refreshTableOnRoomChange(int roomIndex);

    void on_actionEdit_classes_triggered();
    void on_actionEdit_courses_triggered();
    void on_actionEdit_rooms_triggered();

    void addRoom(QString room);
    void RemoveRoom(QString room);
    void ClearAllSlots(QString data, QString type); //removes data when teacher/class/course is deleted
    void DetectDoubleSlot();
};
#endif // MAINWINDOW_H
