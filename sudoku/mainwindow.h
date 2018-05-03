#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <QTableWidget>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void read_map(void);
    int get_element(int row, int column);
    bool check(QVector<int> v);
    bool check_map(QVector<int> vector);

    void autosolve(void);
    bool check_row(int i);
    bool check_column(int i);
    bool check_cell(int i);
    void show_answer(void);
    void keyPressEvent(QKeyEvent *d);
    static const int size = 81;

public slots:
    void set_map(void);
    void check_authority(void);
    void selected_effect(void);
    void clear_selected(void);
    void selected_same_number(void);

private slots:
    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<int> map;
    QVector<int> client_map;
    QVector<int> ans_map;
};

#endif // MAINWINDOW_H
