#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void drawStack();
    void dda(double,double,double,double);

    ~MainWindow();

private slots:
    void on_pushButton_pressed();
    void on_popButton_pressed();

private:
    Ui::MainWindow *ui;
    std::vector<QLabel *> data;
    double left, lbase;
};
#endif // MAINWINDOW_H
