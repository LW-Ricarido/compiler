#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PLtext->setAcceptRichText(true);
    setMinimumSize(450,320);
//    ui->PLtext->setTextBackgroundColor(Qt::blue);
    setWindowTitle("PLO_Lexical_Analysis");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    int h = this->geometry().height();
    int w = this->geometry().width();
    ui->PLtext->setGeometry(0,0,w,h-90);
    ui->analysisButton->move(w-100,h-60);
}

void MainWindow::on_analysisButton_clicked()
{
    if(ui->PLtext->toPlainText() == nullptr)
    {
        QMessageBox::warning(this,"Warning","no code string",QMessageBox::Button::Yes);
    }
    else{
        analy = new Analysis;
        connect(analy,SIGNAL(toMainWindow()),this,SLOT(showNormal()));
        analy->show();
        connect(this,SIGNAL(toAnalysis(QString)),analy,SLOT(fromMainWindow(QString)));
        emit toAnalysis(ui->PLtext->toPlainText());
        this->hide();
    }
}

void MainWindow::on_PLtext_textChanged()
{
//    ui->PLtext->setTextBackgroundColor(Qt::blue);
}

void MainWindow::on_Open_fileAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "/",
                                            tr("PL0 code Files (*.pl0 *.txt)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QMessageBox::warning(this,"Warning","Can't Open File",QMessageBox::Button::Yes);
    }
    QTextStream out(&file);
    QString code = out.readAll();
    ui->PLtext->setText(code);
}
