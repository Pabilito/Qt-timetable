#include "dataholder.h"

DataHolder::DataHolder(){}

void DataHolder::setData(QString k, QString t, QString c){
    klasa = k;
    teacher = t;
    course = c;
}

void DataHolder::print(){
    qInfo() << klasa << teacher << course;
}

QString DataHolder::getKlasa(){
    return klasa;
}
QString DataHolder::getTeacher(){
    return teacher;
}
QString DataHolder::getCourse(){
    return course;
}
