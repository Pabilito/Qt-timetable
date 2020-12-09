#ifndef ENTRYEDIT_H
#define ENTRYEDIT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QStringListModel>
#include <QStandardItem>
#include <QMessageBox>
#include "dataholder.h"

namespace Ui {
class EntryEdit;
}

class EntryEdit : public QDialog
{
    Q_OBJECT

public:
    explicit EntryEdit(QWidget *parent = nullptr);
    ~EntryEdit();
    QStringList* teachers;
    QStringList* classes;
    QStringList* courses;
    void RefreshModels();
    void SetRoom(QString RoomNb);
    QStandardItem* item;
    DataHolder* dh;
    void LoadData();
    void ResetClickable();
    void CheckIfUnassingAccessible();

signals:
    void notifyAboutChange();

private slots:
    void on_pushButton_2_clicked();
    void DetectClicksClass(int whatClicked);
    void DetectClicksCourse(int whatClicked);
    void DetectClicksTeacher(int whatClicked);
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::EntryEdit *ui;
    QStringListModel *modelClass;
    QStringListModel *modelCourse;
    QStringListModel *modelTeacher;
    bool selectedClass = false;
    bool selectedCourse = false;
    bool selectedTeacher = false;
    void chcekIfClickable();
};

#endif // ENTRYEDIT_H
