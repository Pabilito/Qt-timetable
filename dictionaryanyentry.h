#ifndef DICTIONARYANYENTRY_H
#define DICTIONARYANYENTRY_H

#include <QDialog>
#include <QAbstractButton>
#include <QtCore>
#include <QMessageBox>

namespace Ui {
class dictionaryAnyEntry;
}

class dictionaryAnyEntry : public QDialog
{
    Q_OBJECT

public:
    explicit dictionaryAnyEntry(QWidget *parent = nullptr);
    QStringList* dataList;
    QString dataType;
    void RefreshModel();
    ~dictionaryAnyEntry();

signals:
    void ClearSlots(QString data, QString type);
    void signalAddRoom(QString room);
    void signalRemoveRoom(QString room);

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_2_clicked();

private:
    Ui::dictionaryAnyEntry *ui;
    QStringListModel *model;
};

#endif // DICTIONARYANYENTRY_H
