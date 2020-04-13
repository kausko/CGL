#include "patlines.h"
#include "ui_patlines.h"
#include <QMouseEvent>

QImage image(1181, 701, QImage::Format_RGB888);

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}


PatLines::PatLines(QWidget *parent) : QMainWindow(parent), ui(new Ui::PatLines){
    ui->setupUi(this);
    flag = 0;
    lineColor = qRgb(255,255,255);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

PatLines::~PatLines(){
    delete ui;
}


void PatLines::on_drawPB_pressed(){
    dda(
        ui->textEditX1->toPlainText().toInt(),
        ui->textEditY1->toPlainText().toInt(),
        ui->textEditX2->toPlainText().toInt(),
        ui->textEditY2->toPlainText().toInt()
    );
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void PatLines::on_clearPB_pressed(){
    image.fill(Qt::black);
    flag = 0;
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void PatLines::mousePressEvent(QMouseEvent * ev){
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
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void PatLines::dda(float x1, float y1, float x2, float y2)
{
    float dx = x2-x1;
    float dy = y2-y1;
    float length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float delx = dx/length;
    float dely = dy/length;

    float x = x1 + 0.5*sign(delx);
    float y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        switch (ui->comboBox->currentIndex()) {
        case 0: {
            image.setPixel(x-10,y-10,lineColor);
            break;
        }
        case 1: {
            if(!(i%4))
                image.setPixel(x-10,y-10,lineColor);
            break;
        }
        case 2: {
            int k = i%10;
            if(k == 0 || k == 1 || k == 2 || k == 3 || k == 4)
                image.setPixel(x-10,y-10,lineColor);
            break;
        }
        case 3: {
            int k = i%10;
            if(k == 0 || k == 1 || k == 2 || k == 3 || k == 4 || k == 7)
                image.setPixel(x-10, y-10, lineColor);
            break;
        }
        case 4: {
            image.setPixel(x-10, y-10, lineColor);
            image.setPixel(x-11, y-9, lineColor);
            image.setPixel(x-9, y-11, lineColor);
            image.setPixel(x-11, y-11, lineColor);
            image.setPixel(x-9, y-9, lineColor);
        }

        }
        x += delx;
        y += dely;
    }

}
