#include "custom.h"
#include "ui_custom.h"
#include "mainwindow.h"
#include <QTableWidget>
#include <QKeyEvent>
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
void custom::on_inputButton_clicked()
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            client_map.append(get_element(i, j));
    main = new MainWindow(client_map);
    main->show();
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

