#include "checkform.h"
#include "ui_checkform.h"

#include <iostream>

CheckForm::CheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckForm)
{
    std::cout << "CheckForm(QWidget *parent)" << std::endl;
    ui->setupUi(this);
}

CheckForm::~CheckForm()
{
    delete ui;
}

void CheckForm::completeText(std::string input, int checkBoxNumber)
{
    QString qString;

    qString = QString::fromStdString(input);

    switch(checkBoxNumber) {
    case 1: ui->checkBox_1->setText(qString);
        break;
    case 2: ui->checkBox_2->setText(qString);
        break;
    case 3: ui->checkBox_3->setText(qString);
        break;
    case 4: ui->checkBox_4->setText(qString);
        break;
    default:
        break;
    }
}

void CheckForm::removeCheckBox(int checkBoxCount)
{
    switch(checkBoxCount) {
    case 3: ui->checkBox_2->setText("");
        //ui->checkBox_2->setChecked(true);
        ui->checkBox_2->setCheckable(false);
    case 2: ui->checkBox_3->setText("");
        //ui->checkBox_3->setChecked(true);
        ui->checkBox_3->setCheckable(false);
    case 1: ui->checkBox_4->setText("");
        //ui->checkBox_4->setChecked(true);
        ui->checkBox_4->setCheckable(false);
        break;
    default:
        break;
    }
}

QCheckBox * CheckForm::getCheckBox(int number)
{
    switch(number) {
    case 1: return ui->checkBox_1;
        break;
    case 2: return ui->checkBox_2;
        break;
    case 3: return ui->checkBox_3;
        break;
    case 4: return ui->checkBox_4;
        break;
    default: return nullptr;
        break;
    }

    return nullptr;
}

bool CheckForm::getIsChecked(int number) {
    if (number > 0 && number < 5) {
        return getCheckBox(number)->isChecked();
    }

    return false;
}
