#include "a2pattern.h"
#include "ui_a2pattern.h"
#include <math.h>

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

QImage image(1090, 690, QImage::Format_RGB888);

A2Pattern::A2Pattern(QWidget *parent): QMainWindow(parent), ui(new Ui::A2Pattern){
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->drawPushButton->setDisabled(true);
}

A2Pattern::~A2Pattern(){
    delete ui;
}


void A2Pattern::on_drawPushButton_pressed(){
    if(ui->drawPushButton->text() == "Draw"){
        int x = ui->textEditX->toPlainText().toInt();
        int y = ui->textEditY->toPlainText().toInt();
        int r = ui->textEditR->toPlainText().toInt();

        // Outer circle
        BresenhamCircle(x,y,r);

        // Inscribed triangle
        int xcomp = r*cos(M_PI/6);
        int ycomp = r*sin(M_PI/6);
        dda(x+xcomp, y+ycomp, x, y-r);
        dda(x-xcomp, y+ycomp, x, y-r);
        dda(x-xcomp, y+ycomp, x+xcomp, y+ycomp);

        // Inner Circle
        DDACircle(x,y,ycomp);

        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->textEditX->clear();
        ui->textEditY->clear();
        ui->textEditR->clear();
        ui->textEditX->setDisabled(true);
        ui->textEditY->setDisabled(true);
        ui->textEditR->setDisabled(true);
        ui->drawPushButton->setText("Clear");
    }
    else{
        image.fill(Qt::black);
        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->drawPushButton->setText("Draw");
        ui->drawPushButton->setDisabled(true);
        ui->textEditX->setEnabled(true);
        ui->textEditY->setEnabled(true);
        ui->textEditR->setEnabled(true);
    }
}

void A2Pattern::on_textEditX_textChanged(){
    checkFields();
}

void A2Pattern::on_textEditY_textChanged(){
    checkFields();
}

void A2Pattern::on_textEditR_textChanged(){
    checkFields();
}

void A2Pattern::checkFields(){
    QString x,y,r;

    x = ui->textEditX->toPlainText();
    y = ui->textEditY->toPlainText();
    r = ui->textEditR->toPlainText();

    if(x != "" && y != "" && r != ""){
        if(r.toInt() > x.toInt() || r.toInt() > y.toInt()){
            ui->drawPushButton->setDisabled(true);
            ui->labelStatus->setText("Error: R > x and/or R > y");
        }
        else{
            ui->drawPushButton->setEnabled(true);
            ui->labelStatus->setText("");
        }
    }
}

void A2Pattern::BresenhamCircle(int x,int y,int r) {
    int x1=0;
    int y1=r;
    int S = 3 - (2*r);
    drawCircle(x,y,x1,y1);
    while(y1>=x1) {
        x1++;
        if(S>=0) {
            y1--;
            S = S + 4*(x1-y1) + 10;
        }
        else {
            S = S + 4*x1 + 6;
        }
        drawCircle(x,y,x1,y1);
    }
}

void A2Pattern::drawCircle(int xc,int yc,int x,int y) {
    QRgb value;
    value=qRgb(0,255,0);
    image.setPixel(xc+x, yc+y,value);
    image.setPixel(xc-x, yc+y,value);
    image.setPixel(xc+x, yc-y,value);
    image.setPixel(xc-x, yc-y,value);
    image.setPixel(xc+y, yc+x,value);
    image.setPixel(xc-y, yc+x,value);
    image.setPixel(xc+y, yc-x,value);
    image.setPixel(xc-y, yc-x,value);
}

void A2Pattern::DDACircle(float x1,float y1,float r1) {
    float xc1,xc2,yc1,yc2,eps,sx,sy;
    int i, val;
    xc1=r1;
    yc1=0;
    sx=xc1;
    sy=yc1;
    i=0;

    do{
        val=pow(2, i);
        i++;

    }while(val<r1);
    eps = 1/pow(2,i-1);

    do{
        xc2 = xc1 + yc1*eps;
        yc2 = yc1 - eps*xc2;
        image.setPixel(x1+xc2,y1-yc2, qRgb(0, 255, 0));
        xc1=xc2;
        yc1=yc2;

    }while((yc1-sy)<eps || (sx-xc1)>eps);

}

void A2Pattern::dda(double x1, double y1, double x2, double y2){
    double dx = x2-x1;
    double dy = y2-y1;
    double length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double delx = dx/length;
    double dely = dy/length;

    double x = x1 + 0.5*sign(delx);
    double y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixel(x, y, qRgb(0,255,0));
        x += delx;
        y += dely;
    }

}
