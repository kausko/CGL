#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void dda(double,double,double,double,QRgb);
    void tableInit(int);
    void Hilbert(int,int,int,int,int);
    ~MainWindow();

private slots:
    void on_createButton_pressed();

    void on_resetButton_pressed();

private:
    int prex, prey, degree, order, offset;
    bool flag;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
