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
    explicit MainWindow(QVector<int> m, QWidget *parent = 0);
    ~MainWindow();
    QVector<int> & read_map(void);
    int get_element(int row, int column);
    void set_element(int row, int column, int input);
    bool check(QVector<int> v);
    bool check_map(QVector<int> vector);
    int cell_number(int row, int column);
    void autosolve(QVector<int> &m);
    bool check_row(int i, QVector<int> &m);
    bool check_column(int i, QVector<int> &m);
    bool check_cell(int i, QVector<int> &m);
    void show_answer(void);
    void keyPressEvent(QKeyEvent *d);
    void save_score(int t);
    static const int size = 81;


public slots:
    void set_map(bool exist, const QVector<int> &temp);
    void client_input(void);
    void selected_effect(void);
    void clear_selected(void);
    void on_clearButton_clicked();
    QString update();

private slots:
    void on_refreshButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<int> map;
    QVector<int> client_map;
    QVector<int> ans_map;
    QVector<int> old_map;
    bool state, checkDisable;
    int time;
    QTimer *timer;
};

#endif // MAINWINDOW_H
