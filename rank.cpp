#include "rank.h"
#include "ui_rank.h"

rank::rank(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::rank)
{
    ui->setupUi(this);
    display();
}

rank::~rank()
{
    delete ui;
}
void rank::display(){
    QFile input("rank");
    if (!input.open(QIODevice::ReadWrite))
        QMessageBox::warning(this, "Oops!", "file not open");
    QTextStream s(&input);
    int i;
    s >> i;
    while(i != 0){
        score.push_back(i);
        s >> i;
    }
    input.close();
    QString str;
    str.append("Rank: \n\n");
    QFile output("rank");
    if(!output.open(QIODevice::WriteOnly))
        QMessageBox::warning(this, "Oops!", "file not open");
    QTextStream out(&output);
    std::sort(score.begin(),score.end());
    for(int j = 0; j < score.size(); ++j){
        if(j >= 10)
            break;
        str.append(QString::number(j+1));
        str.append(".\t");
        if(score.at(j)/60 != 0){
            str.append(QString::number(score.at(j)/60));
            str.append(" min ");
        }
        str.append(QString::number(score.at(j)%60));
        str.append(" sec\n\n");
        out << score.at(j) << '\n';
    }ui->textBrowser->setText(str);
}
