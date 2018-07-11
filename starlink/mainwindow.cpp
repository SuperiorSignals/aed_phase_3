#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    std::cout << "MainWindow(QWidget *parent)" << std::endl;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
