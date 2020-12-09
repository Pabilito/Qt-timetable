#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Timetable");

    allData = QVector<QVector<DataHolder>>();
    data = QVector<DataHolder>();

    for(int i=0; i<4; i++){
        for(int i=0; i<50; i++){
            DataHolder* dh = new DataHolder();
            data.append(*dh);
        }
        allData.append(data);
        data.clear();
    }

    model = new QStandardItemModel(10, 5);
    InitializeModel(model);
    ui->tableView->setModel(model);

    teachers = new QStringList;
    rooms = new QStringList;
    classes = new QStringList;
    courses = new QStringList;

    /*===SOME=INITIAL=DATA=====*/
    rooms->append("110");
    rooms->append("111");
    rooms->append("120");
    rooms->append("121");
    classes->append("1a");
    classes->append("1b");
    classes->append("1c");
    classes->append("2a");
    classes->append("2b");
    classes->append("3a");
    classes->append("3b");
    classes->append("4a");
    classes->append("4b");
    courses->append("mat");
    courses->append("geo");
    courses->append("eng");
    courses->append("phys");
    courses->append("biol");
    teachers->append("john");
    teachers->append("smith");
    /*====================*/

    ui->comboBox->addItems(*rooms);
    ee = new EntryEdit();
    any = new dictionaryAnyEntry();

    connect(any, SIGNAL(signalAddRoom(QString)),this,SLOT(addRoom(QString)));
    connect(any, SIGNAL(signalRemoveRoom(QString)),this,SLOT(RemoveRoom(QString)));
    connect(any, SIGNAL(ClearSlots(QString, QString)),this,SLOT(ClearAllSlots(QString, QString)));
    connect(any, SIGNAL(ClearSlots(QString, QString)),this,SLOT(ClearAllSlots(QString, QString)));
    connect(any, SIGNAL(ClearSlots(QString, QString)),this,SLOT(ClearAllSlots(QString, QString)));
    connect(ee, SIGNAL(notifyAboutChange()),this,SLOT(DetectDoubleSlot())); //Informs when new entry makes a collision (class/teacher twice in the same slot)
    connect(ui->comboBox,  SIGNAL(activated(int)),this,SLOT(refreshTableOnRoomChange(int))); //Refreshes table contents when combobox values are changed
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitializeModel(QStandardItemModel* model){
    //("room, group, class, slot, teacher"));
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QStandardItem *room = new QStandardItem();
            room->setTextAlignment(Qt::AlignCenter);
            room->setText("");
            model->setItem(row, column, room);
        }
    }

    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    model->setHeaderData(0, Qt::Horizontal, "Monday");
    model->setHeaderData(1, Qt::Horizontal, "Tuesday");
    model->setHeaderData(2, Qt::Horizontal, "Wednesday");
    model->setHeaderData(3, Qt::Horizontal, "Thursday");
    model->setHeaderData(4, Qt::Horizontal, "Friday");
    model->setHeaderData(0, Qt::Vertical, "08:00 - 08:45");
    model->setHeaderData(1, Qt::Vertical, "08:55 - 09:40");
    model->setHeaderData(2, Qt::Vertical, "09:50 - 10:35");
    model->setHeaderData(3, Qt::Vertical, "10:50 - 11:35");
    model->setHeaderData(4, Qt::Vertical, "11:55 - 12:40");
    model->setHeaderData(5, Qt::Vertical, "12:50 - 13:35");
    model->setHeaderData(6, Qt::Vertical, "13:45 - 14:30");
    model->setHeaderData(7, Qt::Vertical, "14:40 - 15:25");
    model->setHeaderData(8, Qt::Vertical, "15:35 - 16:20");
    model->setHeaderData(9, Qt::Vertical, "16:30 - 17:15");
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    ee->teachers = teachers;
    ee->classes = classes;
    ee->courses = courses;
    ee->dh = &allData[ui->comboBox->currentIndex()][10*index.column()+index.row()];
    ee->item = model->itemFromIndex(index);
    ee->SetRoom(ui->comboBox->currentText());
    ee->RefreshModels();
    if(model->itemFromIndex(index)->text() == ""){ //if entry is empty
        ee->ResetClickable();
    }else{
        ee->LoadData();
    }
    ee->CheckIfUnassingAccessible();
    ee->exec();
}

void MainWindow::on_actionSave_triggered() //Save to file
{

    /* Format to implement
     {
        "rooms":["110","111","120","121"],
        "groups":["1a","1b","1c","2a","2b","3a","3b","4a","4b"],
        "classes":["mat","geo","eng","phys","biol"],
        "teachers":["kowalski","nowak","smith","clarkson","may","hammond","atkinson"],
        "activities":[
         {"room":"110", "group":"1a", "class":"mat", "slot":1, "teacher":"kowalski"},
         {"room":"121", "group":"1c", "class":"eng", "slot":3, "teacher":"nowak"]
    }
    */

    QString json_filter = "JSON (*.json)";
    QString savefile = QFileDialog::getSaveFileName(this, tr("Save"), "/", tr("Other format (*);;%1").arg(json_filter),
                                                    &json_filter, QFileDialog::DontUseNativeDialog);
    if(!savefile.isEmpty()){
        QJsonDocument doc;
        QJsonObject obj;
        QJsonArray arrRooms;
        QJsonArray arrGroups;
        QJsonArray arrClasses;
        QJsonArray arrTeachers;
        QJsonArray arrActivities;
        QJsonObject activity;

        for(int i=0; i<rooms->size(); i++){
            arrRooms.append(rooms->value(i));
        }
        for(int i=0; i<classes->size(); i++){
            arrGroups.append(classes->value(i));
        }
        for(int i=0; i<courses->size(); i++){
            arrClasses.append(courses->value(i));
        }
        for(int i=0; i<teachers->size(); i++){
            arrTeachers.append(teachers->value(i));
        }

        for(int i=0; i<allData.size(); i++){
            for(int j=0; j<50; j++){
                if(allData[i][j].getKlasa() != ""){
                    activity.insert("room", rooms->value(i));
                    activity.insert("group", allData[i][j].getKlasa());
                    activity.insert("class", allData[i][j].getCourse());
                    activity.insert("slot", j);
                    activity.insert("teacher", allData[i][j].getTeacher());
                    arrActivities.append(activity);
                    activity = QJsonObject();
                }
            }
        }

        obj.insert("rooms", arrRooms);
        obj.insert("groups", arrGroups);
        obj.insert("classes", arrClasses);
        obj.insert("teachers", arrTeachers);
        obj.insert("activities", arrActivities);

        doc.setObject(obj);
        QByteArray data_json = doc.toJson(); //QJsonDocument::Compact if I want to make it compact
        QFile output(savefile);

        if(output.open(QIODevice::WriteOnly | QIODevice::Text)){
            output.write(data_json);
            output.close();
            QMessageBox::information(this, tr("Message"), tr("File saved successfully"));
        }else{
            QMessageBox::critical(this, tr("Error"), output.errorString());
        }
    }
}

void MainWindow::on_actionLOad_triggered() //LOAD from file
{
    QString json_filter = "JSON (*.json)";
    QString savefile = QFileDialog::getOpenFileName(this, tr("Open"), "/", tr("Other format (*);;%1").arg(json_filter),
                                                    &json_filter, QFileDialog::DontUseNativeDialog);
    if(!savefile.isEmpty()){ //if file exists
        QFile input(savefile);
        QJsonObject obj;
        QJsonDocument doc;
        QByteArray data_json;

        QJsonArray arrRooms;
        QJsonArray arrGroups;
        QJsonArray arrClasses;
        QJsonArray arrTeachers;
        QJsonArray arrActivities;
        QJsonObject activity;

        if(input.open(QIODevice::ReadOnly | QIODevice::Text)){
            data_json = input.readAll();
            doc = doc.fromJson(data_json);
            obj = doc.object();

            arrRooms = obj["rooms"].toArray();
            arrGroups = obj["groups"].toArray();
            arrClasses = obj["classes"].toArray();
            arrTeachers = obj["teachers"].toArray();
            arrActivities = obj["activities"].toArray();

            teachers->clear();
            rooms->clear();
            classes->clear();
            courses->clear();

            for(int i=0; i<arrRooms.size(); i++){
                rooms->append(arrRooms[i].toString());
            }
            for(int i=0; i<arrGroups.size(); i++){
                classes->append(arrGroups[i].toString());
            }
            for(int i=0; i<arrClasses.size(); i++){
                courses->append(arrClasses[i].toString());
            }
            for(int i=0; i<arrTeachers.size(); i++){
                teachers->append(arrTeachers[i].toString());
            }

            allData.clear();
            for(int i=0; i<rooms->size(); i++){
                for(int i=0; i<50; i++){
                    DataHolder* dh = new DataHolder();
                    data.append(*dh);
                }
                allData.append(data);
                data.clear();
            }

            ui->comboBox->clear();
            ui->comboBox->addItems(*rooms);

            for(int i=0; i<arrActivities.size(); i++){
                activity = arrActivities[i].toObject();
                int slot = activity["slot"].toInt();
                QString room = activity["room"].toString();
                allData[ui->comboBox->findText(room)][slot].setData(activity["group"].toString(), activity["teacher"].toString(), activity["class"].toString());
            }

            ui->comboBox->setCurrentIndex(0);
            refreshTableOnRoomChange(0); //load data for initial class
            QMessageBox::information(this, tr("Message"), tr("File loaded successfully"));
        }else{
            QMessageBox::critical(this, tr("Error"), input.errorString());
        }
    }
}

void MainWindow::refreshTableOnRoomChange(int roomIndex){
    for (int i=0; i<5; i++){
        for(int j=0; j<10; j++){
            model->item(j, i)->setText(allData[roomIndex][10*i+j].getKlasa());
        }
    }
}

void MainWindow::on_actionEdit_Dictionary_triggered() //Edit teachers
{
    any->dataList = teachers;
    any->RefreshModel();
    any->dataType = "teacher";
    any->exec();
}

void MainWindow::on_actionEdit_classes_triggered() //Edit classes
{
    any->dataList = classes;
    any->RefreshModel();
    any->dataType = "class";
    any->exec();
}

void MainWindow::on_actionEdit_courses_triggered() //Edit courses
{
    any->dataList = courses;
    any->RefreshModel();
    any->dataType = "course";
    any->exec();
}

void MainWindow::on_actionEdit_rooms_triggered() //Edit rooms
{
    any->dataList = rooms;
    any->RefreshModel();
    any->dataType = "room";
    any->exec();
}

void MainWindow::addRoom(QString room){
    rooms->sort();
    int index = rooms->indexOf(room);

    for(int i=0; i<50; i++){
        DataHolder* dh = new DataHolder();
        data.append(*dh);
    }
    allData.insert(index, data);
    data.clear();

    ui->comboBox->clear();
    ui->comboBox->addItems(*rooms);
    refreshTableOnRoomChange(index);
    ui->comboBox->setCurrentIndex(index); //go to newly created room automatically
}

void MainWindow::RemoveRoom(QString room){
    int index = ui->comboBox->findText(room);
    ui->comboBox->removeItem(index);
    allData.remove(index);
    refreshTableOnRoomChange(ui->comboBox->currentIndex());
}

void MainWindow::DetectDoubleSlot(){ //Detects teacher/class being twice in one slot
    int slotChanged = ui->tableView->currentIndex().row() + 10 * ui->tableView->currentIndex().column();
    int CRI = ui->comboBox->currentIndex(); //Current Room Index acronym is CRI
    for(int i=0; i<ui->comboBox->count(); i++){
        if(i!=CRI && (allData[i][slotChanged].getTeacher() == allData[CRI][slotChanged].getTeacher() || allData[i][slotChanged].getKlasa() == allData[CRI][slotChanged].getKlasa())){
            QString warning = "\n\nRoom:\t" + rooms->at(i) + "\nClass:\t" + allData[i][slotChanged].getKlasa()+"\nTeacher:\t" + allData[i][slotChanged].getTeacher();
            if(2048 == QMessageBox::warning(this, "Keep this entry?", "Collision detection - following data will be deleted if saved: " + warning,
                                            QMessageBox::Discard| QMessageBox::Save)){ //2048 == accepted
                allData[i][slotChanged].setData("","","");
            }else{
                allData[CRI][slotChanged].setData("","","");
                refreshTableOnRoomChange(CRI);
                return;
            }
        }
    }
}

void MainWindow::ClearAllSlots(QString data, QString type){ //reset data after deleting teacher/class/course
    if(type=="teacher"){
        for(int i=0; i<allData.size(); i++){
            for(int j=0; j<50; j++){
                if(allData[i][j].getTeacher() == data){
                    allData[i][j].setData("", "", "");
                    refreshTableOnRoomChange(ui->comboBox->currentIndex());
                }
            }
        }
    }else if(type=="class"){
        for(int i=0; i<allData.size(); i++){
            for(int j=0; j<50; j++){
                if(allData[i][j].getKlasa() == data){
                    allData[i][j].setData("", "", "");
                    refreshTableOnRoomChange(ui->comboBox->currentIndex());
                }
            }
        }
    }else if(type == "course"){
        for(int i=0; i<allData.size(); i++){
            for(int j=0; j<50; j++){
                if(allData[i][j].getCourse() == data){
                    allData[i][j].setData("", "", "");
                    refreshTableOnRoomChange(ui->comboBox->currentIndex());
                }
            }
        }
    }
}
