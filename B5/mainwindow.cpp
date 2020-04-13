#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <math.h>
#include <QTimer>
#define SIDE 1024

QImage image(SIDE,SIDE, QImage::Format_RGB888);
QRgb gray = qRgb(105,105,105);
QColor white = Qt::white;

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    flag = true;
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->resetButton->setEnabled(false);
    ui->spinBox->setRange(0,8);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tableInit(int order){
    for (int i=1;i<order;i++) {
        dda(SIDE*i/order,0,SIDE*i/order,SIDE,gray);
        dda(0,SIDE*i/order,SIDE,SIDE*i/order,gray);
    }
}

void MainWindow::Hilbert(int x, int y, int lg , int i1, int i2){

    if (lg == 1) {
        //std::cout<<x*order + ((x-1)*order)<<","<<y*order + ((y-1)*order)<<std::endl; /* Output coordinates. */

        if(flag){
            prex = x*offset + ((x-1)*offset);
            prey = y*offset + ((y-1)*offset);
            flag = false;
        }
        else{
            dda(prex,prey, x*offset + ((x-1)*offset), y*offset + ((y-1)*offset),white.rgb());
            prex = x*offset + ((x-1)*offset);
            prey = y*offset + ((y-1)*offset);
            ui->label->setPixmap(QPixmap::fromImage(image));
        }

        return;
    }

    lg >>= 1; /* Divide by 2. */

    Hilbert(x+i1*lg, y+i1*lg, lg, i1, 1-i2);
    Hilbert(x+i2*lg, y+(1-i2)*lg, lg, i1, i2);
    Hilbert(x+(1-i1)*lg, y+(1-i1)*lg, lg, i1, i2);
    Hilbert(x+(1-i2)*lg, y+i2*lg, lg, 1-i1, i2);
}

void MainWindow::on_createButton_pressed(){
    order = ui->spinBox->value();
    degree = pow(2,order);
    offset = 512/degree;
    tableInit(degree);
    Hilbert(1,1,degree,0,0);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->resetButton->setEnabled(true);
}


void MainWindow::on_resetButton_pressed(){
    flag = true;
    image.fill(Qt::black);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->spinBox->setValue(0);
    ui->resetButton->setEnabled(false);
}

void MainWindow::dda(double x1, double y1, double x2, double y2, QRgb value){
    double dx = x2-x1;
    double dy = y2-y1;
    double length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double delx = dx/length;
    double dely = dy/length;

    double x = x1 + 0.5*sign(delx);
    double y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixel(x,y,value);
        x += delx;
        y += dely;
    }

}
