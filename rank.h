#ifndef RANK_H
#define RANK_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class rank;
}

class rank : public QMainWindow
{
    Q_OBJECT

public:
    explicit rank(QWidget *parent = 0);
    ~rank();
    void display();

private:
    Ui::rank *ui;
    QVector<int> score;
};

#endif // RANK_H
