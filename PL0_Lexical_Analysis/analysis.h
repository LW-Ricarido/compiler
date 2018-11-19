#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QDialog>
#include <QTextStream>
#include <QMessageBox>
namespace Ui {
class Analysis;
}

class Analysis : public QDialog
{
    Q_OBJECT

public:
    explicit Analysis(QWidget *parent = 0);
    ~Analysis();

signals:
    void toMainWindow();

private slots:
    void on_oKButton_clicked();
    void fromMainWindow(QString);
private:
    Ui::Analysis *ui;
    QString code;
    void resizeEvent(QResizeEvent *event);
    int iter;
    QString getIEEEFloat(double);
    void setItem(QString s1,QString s2,QString s3);
    void analysisCode();
    bool isconst();
    bool isvar();
    bool isprocedure();
    bool isif();
    bool isthen();
    bool iselse();
    bool iswhile();
    bool isdo();
    bool iscall();
    bool isbegin();
    bool isend();
    bool isrepeat();
    bool isuntil();
    bool isread();
    bool iswrite();
    bool isodd();
    double isNum();

};

#endif // ANALYSIS_H
