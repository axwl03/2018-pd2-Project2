#include "menu.h"
#include "ui_menu.h"
#include "mainwindow.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_pushButton_clicked()
{
    main = new MainWindow;
    main->show();
}