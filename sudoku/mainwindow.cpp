#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QFile>
#include <QTableWidget>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_map();
    set_map();
    client_map = map;
    autosolve();
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::check_authority);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::selected_effect);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::selected_same_number);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::show_answer);
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
    //set font color
    if(map.at(9*row+column) == 0) {
        ui->tableWidget->item(row,column)->setForeground(Qt::blue);
        ui->tableWidget->item(row,column)->setTextAlignment(Qt::AlignCenter);
    }
    if(get_element(row,column) >= 1 && get_element(row,column) <= 9)
        client_map[9*row+column] = get_element(row,column);
    else {
        str = "\0";
    //    ui->tableWidget->setItem(row, column, new QTableWidgetItem(str));
    }
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
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QString set_map;
    int i = qrand()%4+1;
    while(1){
        if(input.readLine() == QString::number(i)) {
            for(int i = 0; i < 9; ++i)
                set_map.append(input.readLine());
            break;
        }
    }
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
bool MainWindow::check_map(QVector<int> vector){
    QVector<int> v(9);
    //check row
    for(int i = 0; i < size; i+=9)
        for(int j = 0; j < 9; ++j)
            v[j] = vector.at(i + j);
    if(check(v) == false)
        return false;

    //check column
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            v[j] = vector.at(i + 9*j);
    if(check(v) == false)
        return false;

    //check cell
    for(int m = 0; m <= 60; m += 3) {
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                v[3 * i + j] = vector.at(9 * i + j + m);
        if(m == 6 || m == 33)
            m += 18;
    }
    if(check(v) == false)
        return false;
    return true;
}
void MainWindow::selected_effect(){
    clear_selected();
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
}
void MainWindow::clear_selected(){
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            ui->tableWidget->item(i,j)->setBackground(QColor(0,0,0,0));
}
void MainWindow::selected_same_number(){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j) {
            if(ui->tableWidget->item(i,j)->text() == ui->tableWidget->item(row,column)->text() && ui->tableWidget->item(i,j)->text() != "\0")
                ui->tableWidget->item(i,j)->setBackground(Qt::gray);
        }
}
void MainWindow::autosolve(){
    ans_map = map;
    int i = 0, j;
    bool p = true;
    while(i < 81) {
        p = true;
        if(map.at(i) == 0) {
            ++ans_map[i];

            //check whether success
            while(ans_map[i] > 9 || check_row(i) != true || check_column(i) != true || check_cell(i) != true) {
                ++ans_map[i]; //try next number till success
                //if no possible number could try, jump to last number
                if(ans_map[i] > 9) {
                    ans_map[i] = 0;
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
    if(check_map(ans_map) == false)
        QMessageBox::information(this, "Oops!", "Could not solve");
}
void MainWindow::show_answer(void){
    QString str;
    for(int m = 0; m < size; ++m) {
        if(map.at(m) == 0) {
            str = QString::number(ans_map.at(m));
            ui->tableWidget->setItem(m/9, m%9, new QTableWidgetItem(str));
            ui->tableWidget->item(m/9,m%9)->setForeground(Qt::blue);
            ui->tableWidget->item(m/9,m%9)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(m/9, m%9)->setBackground(QColor(0,0,0,0));
        }
    }
    clear_selected();
}
bool MainWindow::check_row(int i){
    int row = i/9;
    for(int j = 0; j < 9; ++j)
        if(i != j+9*row && ans_map.at(i) == ans_map.at(j+9*row))
            return false;
    return true;
}
bool MainWindow::check_column(int i){
    int column = i%9;
    for(int j = 0; j < 9; ++j)
        if(i != 9*j+column && ans_map.at(i) == ans_map.at(9*j+column))
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
            if(i != 9*(row+m)+column+n && ans_map.at(i) == ans_map.at(9*(row+m)+column+n))
                return false;
        }
    return true;
}
void MainWindow::keyPressEvent(QKeyEvent *d){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    if((d->key() == Qt::Key_Delete || d->key() == Qt::Key_0) && map.at(9*row+column) == 0)
        ui->tableWidget->setItem(row, column, new QTableWidgetItem("\0"));
}

void MainWindow::on_clearButton_clicked()
{
    set_map();
}
