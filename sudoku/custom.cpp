#include "custom.h"
#include "ui_custom.h"
#include "mainwindow.h"
#include <QTableWidget>
#include <QKeyEvent>
#include <QDateTime>
#include <QMessageBox>
custom::custom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::custom)
{
    for(int i = 0; i < 81; ++i)
        client_map.append(0);
    old_map = client_map;
    ui->setupUi(this);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &custom::client_input);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &custom::selected_effect);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &custom::selected_same_number);
}

custom::~custom()
{
    delete ui;
}
void custom::client_input(){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    ui->tableWidget->item(row,column)->setTextAlignment(Qt::AlignCenter);
    if(get_element(row,column) >= 1 && get_element(row,column) <= 9){ //store client input into client_map
        client_map[9*row+column] = get_element(row,column);
    }
    if(client_map != old_map){ //check whether client_map has been changed
        old_map = client_map;
        selected_effect();
    }
}
void custom::set_random_slot(QVector<int> &m){
    int count = 0;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    for(int i = 0; i < 81; ++i){
        if(qrand()%81 < 12){
            ++count;
            m[i] = 0;
        }
        if(i == 80 && count < 45)
            i = 0;
    }
}
void custom::on_inputButton_clicked()
{
    int i;
    QVector<int> ans;
    for(i = 0; i < 81; ++i)
        if(client_map.at(i) == 0)
            break;
    if(i < 81){ //if client enter answer
        map = client_map;
        autosolve(client_map);
    }
    ans = client_map;
    while(1){  //need autosolve to compare with
        set_random_slot(client_map);
        map = client_map;
        autosolve(client_map);
        if(client_map == ans)
            break;
        else client_map = ans;
    }
    main = new MainWindow(map);
    main->show();
    close();
}
int custom::get_element(int row, int column){
    QTableWidgetItem *ptr = ui->tableWidget->item(row, column);
    QString str = ptr->text();
    if(str.toInt() >= 1 && str.toInt() <= 9)
        return str.toInt();
    else return 0;
}
void custom::keyPressEvent(QKeyEvent *d){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    if(d->key() == Qt::Key_Delete || d->key() == Qt::Key_0){
        client_map[9*row+column] = 0;
        ui->tableWidget->setItem(row, column, new QTableWidgetItem("\0"));
        selected_effect();
    }
}
void custom::selected_effect(){
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
}
void custom::selected_same_number(){
    int row = ui->tableWidget->currentRow();
    int column = ui->tableWidget->currentColumn();
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j) {
            if(get_element(i, j) == get_element(row, column) && get_element(i, j) != 0 && (i!=row && j!=column && cell_number(i,j)!=cell_number(row,column)))
                ui->tableWidget->item(i,j)->setBackground(Qt::gray);
            if(get_element(i, j) == get_element(row, column) && get_element(i, j) != 0 && (i==row || j==column || cell_number(i,j)==cell_number(row,column)))
                ui->tableWidget->item(i,j)->setBackground(QColor(255,125,125));
        }
}
void custom::clear_selected(){
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            ui->tableWidget->item(i,j)->setBackground(QColor(0,0,0,0));
}
int custom::cell_number(int row, int column){
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

//
void custom::autosolve(QVector<int> &m){
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
bool custom::check_row(int i, QVector<int> &m){
    int row = i/9;
    for(int j = 0; j < 9; ++j)
        if(i != j+9*row && m.at(i) == m.at(j+9*row))
            return false;
    return true;
}
bool custom::check_column(int i, QVector<int> &m){
    int column = i%9;
    for(int j = 0; j < 9; ++j)
        if(i != 9*j+column && m.at(i) == m.at(9*j+column))
            return false;
    return true;
}
bool custom::check_cell(int i, QVector<int> &m) {
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
bool custom::check(QVector<int> v){
    QVector<int> check(9);
    for(int i = 0; i < 9; ++i)
        check[v.at(i)-1] = 1;
    for(int i = 0; i < 9; ++i)
        if(check[i] != 1)
            return false;
    return true;
}
bool custom::check_map(QVector<int> vector){
    QVector<int> v(9);
    //check row
    for(int i = 0; i < 81; i+=9)
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
