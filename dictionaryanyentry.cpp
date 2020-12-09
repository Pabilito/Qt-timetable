#include "dictionaryanyentry.h"
#include "ui_dictionaryanyentry.h"
#include "QInputDialog"

dictionaryAnyEntry::dictionaryAnyEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dictionaryAnyEntry)
{
    ui->setupUi(this);
    setWindowTitle("Dictionary");

    dataList = new QStringList;

    model = new QStringListModel(this);
    model->setStringList(*dataList);
    ui->listView->setModel(model);

}

dictionaryAnyEntry::~dictionaryAnyEntry()
{
    delete ui;
}

void dictionaryAnyEntry::on_pushButton_clicked() //add
{
    bool ok;
    QString newDataToAdd = QInputDialog::getText(this, "Insert new data", "Data", QLineEdit::Normal, QString(), &ok);
    if(newDataToAdd!="" && ok){ //don't accept empty input
        if(dataList->indexOf(newDataToAdd) == -1){ //prevent duplicates
            dataList->append(newDataToAdd);
            dataList->sort();
            RefreshModel();
            if(dataType == "room"){
                signalAddRoom(newDataToAdd);
            }
        }else{
            QMessageBox::warning(this, "Warning", "Duplicate data forbidden", QMessageBox::Cancel);
        }
    }
}

void dictionaryAnyEntry::on_buttonBox_clicked(QAbstractButton *button) //cancel
{
    close();
}

void dictionaryAnyEntry::on_pushButton_2_clicked() //remove
{
    //Maybe I gave to many warnings, but I think they are useful

    if(dataType == "room"){
        QString deletedData = ui->listView->currentIndex().data().toString();

        if(ui->listView->currentIndex().row()<0){
            QMessageBox::warning(this, "Warning", "You have to select item to delete", QMessageBox::Cancel);
        }
        else if(dataList->count() != 1){
            if(2048 == QMessageBox::warning(this, "Are you sure?", "Deleting room will affect timetable", QMessageBox::Cancel| QMessageBox::Save)){ //2048 == accepted
            dataList->removeAt(ui->listView->currentIndex().row());
            RefreshModel();
            signalRemoveRoom(deletedData);
            }
        }
        else{
            QMessageBox::warning(this, "Warning", "There must be at least one room", QMessageBox::Cancel);
        }
    }else{
        if(ui->listView->currentIndex().row()>=0){ //remove protection below
            if(2048 == QMessageBox::warning(this, "Are you sure?", "Deleting data may affect timetable", QMessageBox::Cancel| QMessageBox::Save)){ //2048 == accepted
                QString deleted = ui->listView->currentIndex().data().toString();
                dataList->removeAt(ui->listView->currentIndex().row());
                RefreshModel();
                ClearSlots(deleted, dataType);
            }
        }
        else{
            QMessageBox::warning(this, "Warning", "You have to select item to delete", QMessageBox::Cancel);
        }
    }
}

void dictionaryAnyEntry::RefreshModel(){
    model->setStringList(*dataList);
}
