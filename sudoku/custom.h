#ifndef CUSTOM_H
#define CUSTOM_H

#include <QMainWindow>
#include <QVector>
#include "mainwindow.h"

namespace Ui {
class custom;
}

class custom : public QMainWindow
{
    Q_OBJECT

public:
    explicit custom(QWidget *parent = 0);
    ~custom();
    int get_element(int row, int column);
    void keyPressEvent(QKeyEvent *d);
    void client_input();
    void selected_effect();
    void selected_same_number();
    void clear_selected();
    int cell_number(int row, int column);

private slots:
    void on_inputButton_clicked();

private:
    Ui::custom *ui;
    QVector<int> client_map;
    QVector<int> old_map;
    MainWindow *main;
};

#endif // CUSTOM_H
