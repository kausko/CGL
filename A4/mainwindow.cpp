#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>
#include <QMouseEvent>
#include <QLabel>
#include<QColorDialog>

QImage image(1181,701, QImage::Format_RGB888);
QImage imageFC(31,31, QImage::Format_RGB888);
QImage imageLC(31,31,QImage::Format_RGB888);

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow){
    fillcolor = linecolor = Qt::black;
    flag = 0;
    ui->setupUi(this);
    image.fill(Qt::white);
    dda(0,0,1180,0);
    dda(1180,0,1180,700);
    dda(0,700,1180,700);
    dda(0,0,0,700);
    imageFC.fill(fillcolor);
    imageLC.fill(linecolor);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->labelFC->setPixmap(QPixmap::fromImage(imageFC));
    ui->labelLC->setPixmap(QPixmap::fromImage(imageLC));
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent * ev){
    if(ev->buttons() == Qt::LeftButton){
        if(!flag){
            stx = currx = ev->x();
            sty = curry = ev->y();
            flag = 1;
        }
        else{
            dda(currx,curry,ev->x(),ev->y());
            currx = ev->x();
            curry = ev->y();
        }
    }
    else if(ev->buttons() == Qt::RightButton){
        dda(currx, curry, stx, sty);
        flag = 0;
    }
    else if(ev->buttons() == Qt::MiddleButton){
        floodFill(ev->x(),ev->y(),fillcolor.rgb(),image.pixel(ev->x(), ev->y()));
    }
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::dda(float x1, float y1, float x2, float y2)
{
    float dx = x2-x1;
    float dy = y2-y1;
    float length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float delx = dx/length;
    float dely = dy/length;

    float x = x1 + 0.5*sign(delx);
    float y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixel(x,y,linecolor.rgb());
        x += delx;
        y += dely;
    }

}

void MainWindow::floodFill(float x, float y, QRgb fillcolor, QRgb bgcolor){
    if(image.pixel(x,y) == bgcolor){
        image.setPixel(x,y,fillcolor);
        floodFill(x+1,y,fillcolor,bgcolor);
        floodFill(x,y+1,fillcolor,bgcolor);
        floodFill(x-1,y,fillcolor,bgcolor);
        floodFill(x,y-1,fillcolor,bgcolor);
    }
}

void MainWindow::on_pushButton_2_clicked(){
    fillcolor = QColorDialog::getColor(Qt::yellow, this );
    imageFC.fill(fillcolor);
    ui->labelFC->setPixmap(QPixmap::fromImage(imageFC));
}


void MainWindow::on_pushButton_clicked(){
    linecolor = QColorDialog::getColor(Qt::yellow, this );
    imageLC.fill(linecolor);
    ui->labelLC->setPixmap(QPixmap::fromImage(imageLC));
}

void MainWindow::on_pushButtonCl_pressed(){
    image.fill(Qt::white);
    dda(0,0,1180,0);
    dda(1180,0,1180,700);
    dda(0,700,1180,700);
    dda(0,0,0,700);
    ui->label->setPixmap(QPixmap::fromImage(image));
}
