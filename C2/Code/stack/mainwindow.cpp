#include "mainwindow.h"
#include "ui_mainwindow.h"
QImage image(511,671,QImage::Format_RGB888);

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    left = 150;
    lbase = 621;
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->popButton->setEnabled(false);
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_pushButton_pressed(){
    QLabel * tempLabel = new QLabel(this);
    tempLabel->setGeometry(left+1, lbase, 199, 29);
    tempLabel->setText(ui->dataField->toPlainText());
    tempLabel->setStyleSheet("QLabel {background-color : black; color : white;  }");
    tempLabel->setAlignment(Qt::AlignCenter);
    tempLabel->setVisible(true);
    lbase -= 30;
    data.push_back(tempLabel);
    if(!ui->popButton->isEnabled()){
        ui->popButton->setEnabled(true);
        ui->popButton->setText("Pop");
    }
    drawStack();
    ui->dataField->setText("");
}

void MainWindow::on_popButton_pressed(){
    data.back()->setVisible(false);
    data.pop_back();
    lbase += 30;
    if(data.empty()){
        ui->popButton->setEnabled(false);
        ui->popButton->setText("Stack Empty");
        image.fill(Qt::black);
        ui->label->setPixmap(QPixmap::fromImage(image));
    }
    else
        drawStack();
}

void MainWindow::drawStack(){
    image.fill(Qt::black);
    double base = 650;
    for (unsigned long i = 0; i < data.size(); i++, base -= 30) {
        dda(left,base,left+200,base);
        dda(left,base,left,base-30);
        dda(left+200,base,left+200,base-30);
    }
    dda(left,base,left+200,base);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::dda(double x1, double y1, double x2, double y2){
    QRgb value = qRgb(0,255,0);
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
