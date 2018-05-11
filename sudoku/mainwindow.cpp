#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QFile>
#include <QTableWidget>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QKeyEvent>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkDisable = 0;
    state = 0;
    set_map(0, read_map());
    client_map = map;
    old_map = client_map;
    autosolve(ans_map);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::client_input);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::selected_effect);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::show_answer);
}
MainWindow::MainWindow(QVector<int> m, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkDisable = 0;
    set_map(0, m);
    state = 0;
    client_map = map;
    old_map = client_map;
    autosolve(ans_map);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::client_input);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::selected_effect);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::show_answer);
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::client_input(){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    if(map.at(9*row+column) != 0 && get_element(row,column) != map.at(9*row+column) && checkDisable == 0){
        set_element(row, column, map.at(9*row+column));
        QMessageBox::information(this, "Oops!!", "You could not change the question's element");
        return;
    }
    //set font color
    int c = 0;
    if(map.at(9*row+column) == 0){
        for(int i = 0; i < size; ++i)
            if((i/9==row || i%9==column || cell_number(i/9,i%9)==cell_number(row,column)) && (get_element(i/9, i%9) == get_element(row, column)) && get_element(row, column) != 0){
                if(i/9 == row && i%9 == column)
                    continue;
                ui->tableWidget->item(row, column)->setForeground(Qt::red);
                ++c;
                break;
            }
        if(c == 0)
            ui->tableWidget->item(row,column)->setForeground(Qt::blue);
        ui->tableWidget->item(row,column)->setTextAlignment(Qt::AlignCenter);
    }
    if(get_element(row,column) >= 1 && get_element(row,column) <= 9){ //store client input into client_map
        client_map[9*row+column] = get_element(row,column);
    }
    if(client_map != old_map){ //check whether client_map has been changed
        old_map = client_map;
        selected_effect();
    }
    for(int i = 0; i < size; ++i){ //check whether client finish all slot
        if(client_map.at(i) == 0){
            state = 0;
            return;
        }
    }
    if(state == 0 && check_map(client_map) == true){
        ++state;
        QMessageBox msgBox;
        msgBox.setText("Congratulations!!!");
        msgBox.setInformativeText("Try again?");
        msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Retry){
            msgBox.close();
            on_refreshButton_clicked();
        }
    }
}

void MainWindow::set_map(bool exist, const QVector<int> &temp) {
    if(exist == 0){
        int r = qrand();
        if(r%6 == 0)
            for(int i = 0; i < 9; ++i)
                for(int j = 0; j < 9; ++j)
                    map.push_back(temp.at(9*(8-i)+j));
        if(r%6 == 1)
            for(int i = 0; i < 9; ++i)
                for(int j = 0; j < 9; ++j)
                    map.push_back(temp.at(9*(8-i)+(8-j)));
        if(r%6 == 2)
            for(int i = 0; i < 9; ++i)
                for(int j = 0; j < 9; ++j)
                    map.push_back(temp.at(9*i+(8-j)));
        if(r%6 == 3)
            for(int j = 0; j < 9; ++j)
                for(int i = 0; i < 9; ++i)
                    map.push_back(temp.at(9*(8-i)+j));
        if(r%6 == 4)
            for(int j = 0; j < 9; ++j)
                for(int i = 0; i < 9; ++i)
                    map.push_back(temp.at(9*(8-i)+(8-j)));
        if(r%6 == 5)
            for(int j = 0; j < 9; ++j)
                for(int i = 0; i < 9; ++i)
                    map.push_back(temp.at(9*i+(8-j)));
    }
    //display on ui->tablewidget
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j){
            set_element(i, j, map.at(9*i+j));
        }
}
QVector<int> & MainWindow::read_map() {
    //read text file question
    QFile myfile("in");
    if (!myfile.open(QIODevice::ReadOnly))
        QMessageBox::warning(this, "Oops!", "file not open");
    QTextStream input(&myfile);
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QString set_map;
    int r = qrand()%4+1;
    while(1){
        if(input.readLine() == QString::number(r)) {
            for(int i = 0; i < 9; ++i)
                set_map.append(input.readLine());
            break;
        }
    }
    myfile.close();

    static QVector<int> temp;
    //store into map
    int n;
    for(int i = 0; set_map[i] != '\0'; ++i)
        if(set_map[i] >= '0' && set_map[i] <= '9'){
            n = set_map[i].unicode() - '1' + 1;
            temp.push_back(n);
        }
    return temp;
}
int MainWindow::get_element(int row, int column){
    QTableWidgetItem *ptr = ui->tableWidget->item(row, column);
    QString str = ptr->text();
    if(str.toInt() >= 1 && str.toInt() <= 9)
        return str.toInt();
    else return 0;
}
void MainWindow::set_element(int row, int column, int input){
    QString str;
    if(input != 0)
        str = QString::number(input);
    else
        str = "\0";
    ui->tableWidget->setItem(row, column, new QTableWidgetItem(str));
    ui->tableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
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
            if(cell_number(i, j) == cell_number(row, column))
                ui->tableWidget->item(i,j)->setBackground(Qt::lightGray);
        }
    }
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j) {
            if(get_element(i, j) == get_element(row, column) && get_element(i, j) != 0 && (i!=row && j!=column && cell_number(i,j)!=cell_number(row,column)))
                ui->tableWidget->item(i,j)->setBackground(Qt::gray);
            if(get_element(i, j) == get_element(row, column) && get_element(i, j) != 0 && (i==row || j==column || cell_number(i,j)==cell_number(row,column)))
                if(!(i==row && j==column))
                    ui->tableWidget->item(i,j)->setBackground(QColor(255,125,125));
        }
}
void MainWindow::clear_selected(){
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            ui->tableWidget->item(i,j)->setBackground(QColor(0,0,0,0));
}
void MainWindow::autosolve(QVector<int> &m){
    m = map;
    int i = 0, j;
    bool p = true;
    while(i < 81) {
        p = true;
        if(map.at(i) == 0) {
            ++m[i];

            //check whether success
            while(m[i] > 9 || check_row(i, m) != true || check_column(i, m) != true || check_cell(i, m) != true) {
                ++m[i]; //try next number till success
                //if no possible number could try, jump to last number
                if(m[i] > 9) {
                    m[i] = 0;
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
    if(check_map(m) == false)
        QMessageBox::information(this, "Oops!", "Could not solve");
}
void MainWindow::show_answer(void){
    for(int m = 0; m < size; ++m) {
        if(map.at(m) == 0) {
            set_element(m/9, m%9, ans_map.at(m));
            ui->tableWidget->item(m/9,m%9)->setForeground(Qt::blue);
            ui->tableWidget->item(m/9, m%9)->setBackground(QColor(0,0,0,0));
        }
    }
    clear_selected();
}
bool MainWindow::check_row(int i, QVector<int> &m){
    int row = i/9;
    for(int j = 0; j < 9; ++j)
        if(i != j+9*row && m.at(i) == m.at(j+9*row))
            return false;
    return true;
}
bool MainWindow::check_column(int i, QVector<int> &m){
    int column = i%9;
    for(int j = 0; j < 9; ++j)
        if(i != 9*j+column && m.at(i) == m.at(9*j+column))
            return false;
    return true;
}
bool MainWindow::check_cell(int i, QVector<int> &m) {
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
    for(int r = 0; r < 3; ++r)
        for(int n = 0; n < 3; ++n) {
            if(i != 9*(row+r)+column+n && m.at(i) == m.at(9*(row+r)+column+n))
                return false;
        }
    return true;
}
void MainWindow::keyPressEvent(QKeyEvent *d){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    if((d->key() == Qt::Key_Delete || d->key() == Qt::Key_0) && map.at(9*row+column) == 0){
        client_map[9*row+column] = 0;
        ui->tableWidget->setItem(row, column, new QTableWidgetItem("\0"));
        selected_effect();
    }
}

void MainWindow::on_clearButton_clicked()
{
    set_map(1, map);
    clear_selected();
}
int MainWindow::cell_number(int row, int column){
    if(row >= 0 && row <= 2){
        if(column >= 0 && column <= 2)
            return 1;
        if(column >= 3 && column <= 5)
            return 2;
        if(column >= 6 && column <= 8)
            return 3;
    }
    if(row >= 3 && row <= 5){
        if(column >= 0 && column <= 2)
            return 4;
        if(column >= 3 && column <= 5)
            return 5;
        if(column >= 6 && column <= 8)
            return 6;
    }
    if(row >= 6 && row <= 8){
        if(column >= 0 && column <= 2)
            return 7;
        if(column >= 3 && column <= 5)
            return 8;
        if(column >= 6 && column <= 8)
            return 9;
    }return 0;
}
void MainWindow::on_refreshButton_clicked()
{
    ++checkDisable;
    map.clear();
    state = 0;
    set_map(0, read_map());
    client_map = map;
    old_map = client_map;
    autosolve(ans_map);
    checkDisable = 0;
}
