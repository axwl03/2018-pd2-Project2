#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QFile>
#include <QTableWidget>
#include <QTextStream>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_map();
    client_map = map;

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::set_map);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::check_authority);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::selected_effect);
  //  connect(ui->tableWidget, &QTableWidget::cellChanged, this, &MainWindow::selected_effect);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &MainWindow::input_font_color);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::autosolve);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::check_authority(){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    QString str;
    if(map.at(9*row+column) != 0)
        if(get_element(row,column) != map.at(9*row+column) && get_element(row,column) != 0) {
            str = QString::number(map.at(9*row+column));
            ui->tableWidget->setItem(row, column, new QTableWidgetItem(str));
            ui->tableWidget->item(row,column)->setTextAlignment(Qt::AlignCenter);
            QMessageBox::information(this, "Oops!!", "You could not change the question's element");
            return;
        }
    client_map[9*row+column] = get_element(row,column);
}

void MainWindow::set_map() {
    //display on ui->tablewidget
    QString str;
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j){
            if(map.at(9*i+j) != 0){
                str = QString::number(map.at(9*i+j));
            }else str = "\0";
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(str));
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
}
void MainWindow::read_map() {
    //read text file question
    QFile myfile("in");
    if (!myfile.open(QIODevice::ReadOnly))
        QMessageBox::warning(this, "Oops!", "file not open");
    QTextStream input(&myfile);
    QString set_map = input.readAll();
    myfile.close();

    //store into map
    int n;
    for(int i = 0; set_map[i] != '\0'; ++i)
        if(set_map[i] >= '0' && set_map[i] <= '9'){
            n = set_map[i].unicode() - '1' + 1;
            map.push_back(n);
        }
}
int MainWindow::get_element(int row, int column){
    QTableWidgetItem *ptr = ui->tableWidget->item(row, column);
    QString str = ptr->text();
    return str.toInt();
}
bool MainWindow::check(QVector<int> v){
    QVector<int> check(9);
    for(int i = 0; i < 9; ++i)
        check[v.at(i)-1] = 1;
    for(int i = 0; i < 9; ++i)
        if(check[i] != 1)
            return false;
    return true;
}
bool MainWindow::check_map(){
    QVector<int> v(9);
    //check row
    for(int i = 0; i < size; i+=9)
        for(int j = 0; j < 9; ++j)
            v[j] = client_map.at(i + j);
    if(check(v) == false)
        return false;

    //check column
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            v[j] = client_map.at(i + 9*j);
    if(check(v) == false)
        return false;

    //check cell
    for(int m = 0; m <= 60; m += 3) {
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                v[3 * i + j] = client_map.at(9 * i + j + m);
        if(m == 6 || m == 33)
            m += 18;
    }
    if(check(v) == false)
        return false;
    return true;
}
void MainWindow::selected_effect(){
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            ui->tableWidget->item(i,j)->setBackground(QColor(0,0,0,0));
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    for(int i = 0; i < 9; ++i) {
        ui->tableWidget->item(row,i)->setBackground(Qt::lightGray);
        ui->tableWidget->item(i,column)->setBackground(Qt::lightGray);
        for(int j = 0; j < 9; ++j){
            if(row%9<=2 && i%9<=2) {
                if(column%9<=2 && j%9<=2)
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
                if(column%9>=6 && j%9>=6)
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
                if((column%9>=3 && column%9<=5) && (j%9>=3 && j%9<=5))
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
            }
            if(row%9>=6 && i%9>=6) {
                if(column%9<=2 && j%9<=2)
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
                if(column%9>=6 && j%9>=6)
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
                if((column%9>=3 && column%9<=5) && (j%9>=3 && j%9<=5))
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
            }
            if((row%9>=3 && row%9<=5) && (i%9>=3 && i%9<=5)) {
                if(column%9<=2 && j%9<=2)
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
                if(column%9>=6 && j%9>=6)
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
                if((column%9>=3 && column%9<=5) && (j%9>=3 && j%9<=5))
                    ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
            }
        }
    }
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j) {
            if(ui->tableWidget->item(i,j)->text() == ui->tableWidget->item(row,column)->text() && ui->tableWidget->item(i,j)->text() != "\0")
                ui->tableWidget->item(i,j)->setBackground(Qt::gray);
        }
}

void MainWindow::input_font_color(){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    if(map.at(9*row+column) == 0) {
        ui->tableWidget->item(row,column)->setForeground(Qt::blue);
        ui->tableWidget->item(row,column)->setTextAlignment(Qt::AlignCenter);
    }
}
void MainWindow::autosolve(){
    client_map = map;
    int i = 0, j;
    bool p = true;
    while(i < 81) {
        p = true;
        if(map.at(i) == 0) {
            ++client_map[i];

            //check whether success
            while(client_map[i] > 9 || check_row(i) != true || check_column(i) != true || check_cell(i) != true) {
                ++client_map[i]; //try next number till success
                //if no possible number could try, jump to last number
                if(client_map[i] > 9) {
                    client_map[i] = 0;
                    for(j = i - 1; map.at(j) != 0; --j);
                    p = false;
                    i = j;
                    break;
                }
            }
        }
        if(p == true)
            ++i;
    }
    QString str;
    for(int m = 0; m < size; ++m) {
        if(map.at(m) == 0) {
            str = QString::number(client_map.at(m));
            ui->tableWidget->setItem(m/9, m%9, new QTableWidgetItem(str));
            ui->tableWidget->item(m/9,m%9)->setForeground(Qt::blue);
            ui->tableWidget->item(m/9,m%9)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
bool MainWindow::check_row(int i){
    int row = i/9;
    for(int j = 0; j < 9; ++j)
        if(i != j+9*row && client_map.at(i) == client_map.at(j+9*row))
            return false;
    return true;
}
bool MainWindow::check_column(int i){
    int column = i%9;
    for(int j = 0; j < 9; ++j)
        if(i != 9*j+column && client_map.at(i) == client_map.at(9*j+column))
            return false;
    return true;
}
bool MainWindow::check_cell(int i) {
    int row, column;
    if(i / 9 >= 0 && i / 9 <= 2)
        row = 0;
    if(i / 9 >= 3 && i / 9 <= 5)
        row = 3;
    if(i / 9 >= 6 && i / 9 <= 8)
        row = 6;
    if(i % 9 >= 0 && i % 9 <= 2)
        column = 0;
    if(i % 9 >= 3 && i % 9 <= 5)
        column = 3;
    if(i % 9 >= 6 && i % 9 <= 8)
        column = 6;
    for(int m = 0; m < 3; ++m)
        for(int n = 0; n < 3; ++n) {
            if(i != 9*(row+m)+column+n && client_map.at(i) == client_map.at(9*(row+m)+column+n))
                return false;
        }
    return true;
}

























