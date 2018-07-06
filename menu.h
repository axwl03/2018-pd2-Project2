#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include "mainwindow.h"
#include "custom.h"
#include "rank.h"

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private slots:
    void on_pushButton_3_clicked();

private:
    Ui::Menu *ui;
    MainWindow *main;
    custom *C;
    rank *score;
};

#endif // MENU_H
