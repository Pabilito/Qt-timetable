#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <QString>
#include <QDebug>

class DataHolder
{
public:
    DataHolder();
    void setData(QString k, QString t, QString c);
    void print();
    QString getKlasa();
    QString getTeacher();
    QString getCourse();
private:
    QString klasa;
    QString teacher;
    QString course;
};

#endif // DATAHOLDER_H
