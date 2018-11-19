#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTreeWidget>
#include "analysis.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void toAnalysis(QString);

private slots:
    void on_analysisButton_clicked();
    void on_Open_fileAction_triggered();
    void on_PLtext_textChanged();

private:
    void resizeEvent(QResizeEvent *event);
    Ui::MainWindow *ui;
    Analysis *analy;
};

#endif // MAINWINDOW_H
