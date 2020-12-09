#include "entryedit.h"
#include "ui_entryedit.h"
#include "mainwindow.h"

EntryEdit::EntryEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryEdit)
{
    ui->setupUi(this);   
    setWindowTitle("Entry Edit");

    connect(ui->comboBox,  SIGNAL(activated(int)),this,SLOT(DetectClicksClass(int)));
    connect(ui->comboBox_2,SIGNAL(activated(int)),this,SLOT(DetectClicksCourse(int)));
    connect(ui->comboBox_3,SIGNAL(activated(int)),this,SLOT(DetectClicksTeacher(int)));

    teachers = new QStringList;
    classes= new QStringList;
    courses = new QStringList;

    modelClass = new QStringListModel(this);
    modelCourse = new QStringListModel(this);
    modelTeacher = new QStringListModel(this);

    modelClass->setStringList(*classes);
    modelCourse->setStringList(*courses);
    modelTeacher->setStringList(*teachers);

    ui->comboBox->setModel(modelClass);
    ui->comboBox_2->setModel(modelCourse);
    ui->comboBox_3->setModel(modelTeacher);

    item = new QStandardItem();
}

EntryEdit::~EntryEdit()
{
    delete ui;
}


void EntryEdit::on_pushButton_2_clicked() //unassignButton
{
    if(2048 == QMessageBox::warning(this, "Are you sure?", "Confirm deletion", QMessageBox::Cancel| QMessageBox::Save)){ //2048 == accepted
        dh->setData("", "", "");
        item -> setText("");
        close();
    }
}

void EntryEdit::on_buttonBox_clicked(QAbstractButton *button)
{
    if((QPushButton *)button== ui->buttonBox->button(QDialogButtonBox::Save) ){ //save
        item->setText(ui->comboBox->currentText());       
        dh->setData(ui->comboBox->currentText(), ui->comboBox_3->currentText(), ui->comboBox_2->currentText());
        notifyAboutChange();
        close();
    }
    else{ //cancel
        close();
    }
}

void EntryEdit::RefreshModels(){
    modelClass->setStringList(*classes);
    modelCourse->setStringList(*courses);
    modelTeacher->setStringList(*teachers);
}

void EntryEdit::SetRoom(QString RoomNb){
    ui->label_5->setText(RoomNb);
}

void EntryEdit::LoadData(){
    ui->comboBox->setCurrentText(dh->getKlasa());
    ui->comboBox_2->setCurrentText(dh->getCourse());
    ui->comboBox_3->setCurrentText(dh->getTeacher());
}

void EntryEdit::DetectClicksClass(int whatClicked){
    selectedClass = true;
    chcekIfClickable();
}

void EntryEdit::DetectClicksCourse(int whatClicked){
    selectedCourse = true;
    chcekIfClickable();
}

void EntryEdit::DetectClicksTeacher(int whatClicked){
    selectedTeacher = true;
    chcekIfClickable();
}

void EntryEdit::chcekIfClickable(){ //Entry can be added only when all 3 fields are selected
    if(selectedClass && selectedCourse && selectedTeacher){
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
    }
}

void EntryEdit::ResetClickable(){ //Blocks button responsible for adding entry
    selectedClass = false;
    selectedCourse = false;
    selectedTeacher = false;
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

void EntryEdit::CheckIfUnassingAccessible(){ //Unasigning empty fileds not allowed
    if(item->text() == ""){
        ui->pushButton_2->setEnabled(false);
    }else{
        ui->pushButton_2->setEnabled(true);
    }
}

