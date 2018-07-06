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
    void set_random_slot(QVector<int> &m);

    bool check(QVector<int> v);
    bool check_map(QVector<int> vector);
    void autosolve(QVector<int> &m);
    bool check_row(int i, QVector<int> &m);
    bool check_column(int i, QVector<int> &m);
    bool check_cell(int i, QVector<int> &m);

private slots:
    void on_inputButton_clicked();

private:
    Ui::custom *ui;
    QVector<int> client_map;
    QVector<int> old_map;
    QVector<int> map;
    MainWindow *main;
};

#endif // CUSTOM_H
