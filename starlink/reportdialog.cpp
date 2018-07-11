#include "reportdialog.h"
#include "ui_reportdialog.h"

#include <iostream>
#include <string>
#include <vector>

ReportDialog::ReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    std::vector<std::string> reportData;
    QString temporary;
    std::string s;
    std::string temp;

    std::cout << "ReportDialog(QWidget *parent) line 1" << std::endl;
    ui->setupUi(this);
    std::cout << "ReportDialog(QWidget *parent) line 2" << std::endl;
    reportData = database.readReportData();
    std::cout << "ReportDialog(QWidget *parent) line 3" << std::endl;
    if (reportData.size() > 3) {
    	std::cout << "ReportDialog(QWidget *parent) line 3a" << std::endl;
        if (user != nullptr) {
    	    std::cout << "ReportDialog(QWidget *parent) line 3b" << std::endl;
            s = user->getFirstName();
    	    std::cout << "ReportDialog(QWidget *parent) line 3c" << std::endl;
            s.append(" ");
    	    std::cout << "ReportDialog(QWidget *parent) line 3d" << std::endl;
            try {
                temp = user->getLastName();
                s.append(temp);
            } catch (const std::bad_alloc& e) {
                std::cout << "Failed to allocate last name: ";
                std::cout << e.what() << std::endl;
            }
            std::cout << "ReportDialog(QWidget *parent) line 3e" << std::endl;
            temporary = QString::fromStdString(s);
            std::cout << "ReportDialog(QWidget *parent) line 3f" << std::endl;
            ui->userValueReportLabel->setText(temporary);
        }
        std::cout << "ReportDialog(QWidget *parent) line 4" << std::endl;
        temporary = QString::fromStdString(reportData[0]);
        std::cout << "ReportDialog(QWidget *parent) line 5" << std::endl;
        ui->dateValueReportLabel->setText(temporary);
        std::cout << "ReportDialog(QWidget *parent) line 6" << std::endl;
        temporary = QString::fromStdString(reportData[1]);
        std::cout << "ReportDialog(QWidget *parent) line 7" << std::endl;
        ui->timeValueReportLabel->setText(temporary);
        std::cout << "ReportDialog(QWidget *parent) line 8" << std::endl;
        temporary = QString::fromStdString(reportData[2]);
        std::cout << "ReportDialog(QWidget *parent) line 9" << std::endl;
        ui->longitudeValueReportLabel->setText(temporary);
        std::cout << "ReportDialog(QWidget *parent) line 10" << std::endl;
        temporary = QString::fromStdString(reportData[3]);
        std::cout << "ReportDialog(QWidget *parent) line 11" << std::endl;
        ui->latitudeValueReportLabel->setText(temporary);
        std::cout << "ReportDialog(QWidget *parent) line 12" << std::endl;
    }
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::on_reportExitPushButton_clicked()
{
    this->close(); // Dialog has been closed, but not deleted
}

User *ReportDialog::getUser() const
{
    return user;
}

void ReportDialog::setUser(User *myUser)
{
    user = myUser;
}

void ReportDialog::refreshLabels()
{
    std::vector<std::string> reportData;
    QString temporary;
    std::string s;

    reportData = database.readReportData();
    if (reportData.size() > 3) {
        if (user != nullptr) {
            s = user->getFirstName();
            s.append(" ");
            s.append(user->getLastName());
            temporary = QString::fromStdString(s);
            ui->userValueReportLabel->setText(temporary);
        }
        temporary = QString::fromStdString(reportData[0]);
        ui->dateValueReportLabel->setText(temporary);
        temporary = QString::fromStdString(reportData[1]);
        ui->timeValueReportLabel->setText(temporary);
        temporary = QString::fromStdString(reportData[2]);
        ui->longitudeValueReportLabel->setText(temporary);
        temporary = QString::fromStdString(reportData[3]);
        ui->latitudeValueReportLabel->setText(temporary);
    }
}
