#include "chessboard.h"
#include "ui_chessboard.h"
#include <math.h>

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

QImage image(1081, 731, QImage::Format_RGB888);


ChessBoard::ChessBoard(QWidget *parent): QMainWindow(parent), ui(new Ui::ChessBoard){
    ui->setupUi(this);
    linecolor = fillcolor = qRgb(255,255,255);
    bgcolor = qRgb(0,0,0);
    baseX = 540;
    baseY = 10;
    redraw();
}

ChessBoard::~ChessBoard(){
    delete ui;
}


void ChessBoard::on_orderSpinBox_valueChanged(int arg1){
    redraw();
}

void ChessBoard::on_dimSpinBox_valueChanged(int arg1){
    redraw();
}

void ChessBoard::on_verticalSlider_valueChanged(int value){
    ui->angleSpinBox->setValue(value);
    redraw();
}

void ChessBoard::on_angleSpinBox_valueChanged(int arg1){
    ui->verticalSlider->setValue(arg1);
    redraw();
}


void ChessBoard::dda(float x1, float y1, float x2, float y2)
{
    float dx = x2-x1;
    float dy = y2-y1;
    float length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float delx = dx/length;
    float dely = dy/length;

    float x = x1 + 0.5*sign(delx);
    float y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixel(x,y,linecolor);
        x += delx;
        y += dely;
    }

}

void ChessBoard::floodFill(float x, float y, QRgb fillcolor, QRgb bgcolor){
    if(image.pixel(x,y) == bgcolor){
        image.setPixel(x,y,fillcolor);
        floodFill(x+1,y,fillcolor,bgcolor);
        floodFill(x,y+1,fillcolor,bgcolor);
        floodFill(x-1,y,fillcolor,bgcolor);
        floodFill(x,y-1,fillcolor,bgcolor);
    }
}

void ChessBoard::redraw(){
    image.fill(Qt::black);
    order = ui->orderSpinBox->value();
    dim = ui->dimSpinBox->value();
    angle = ui->verticalSlider->value();    // angle is measured from TL-BR, i.e. 45° || (x=y)

    angle = angle * M_PI/180;   // conversion of angle to radians

    float sineComp = dim*sin(angle);    // sine component of the dimension
    float cosineComp = dim*cos(angle);  // cosine component of the dimension

    float sineInterval = sineComp/order;        // sine component of one square
    float cosineInterval = cosineComp/order;    // cosine component of one square


    for(int i=0; i<= order; i++){

        // drawing all the \\\  lines from 1-89°
        dda(
            baseX - (i*sineInterval),
            baseY + (i*cosineInterval),
            baseX + cosineComp - (i*sineInterval),
            baseY + sineComp + (i*cosineInterval)
        );

        // drawing all the /// lines at 1-89°
        dda(
            baseX + (i*cosineInterval),
            baseY + (i*sineInterval),
            baseX - sineComp + (i*cosineInterval),
            baseY + cosineComp + (i*sineInterval)
        );
    }

    // moving across the main \ diagonal of the chessboard

    for(int i=1; i<=order; i++){

        // calculation of the center of the i-th square on the diagonal

        float x = (baseX - ((2*i - 1)*sineInterval) + baseX + ((2*i - 1)*cosineInterval))/2;
        float y = (baseY + ((2*i - 1)*cosineInterval) + baseY + ((2*i - 1)*sineInterval))/2;

        // calculation of x and y components of the diagonals of the i-th square on the diagonal

        float dx = ( (baseX - ((2*i - 1)*sineInterval) )  - (baseX + ((2*i - 1)*cosineInterval)) )/(2*i-1);
        float dy = ( (baseY + ((2*i - 1)*cosineInterval)) - (baseY + ((2*i - 1)*sineInterval))   )/(2*i-1);

        floodFill(x,y,fillcolor,bgcolor);

        // moving across the line perpendicular to main diagonal from the center of i-th square

        for (int j=1 ; j<std::min(i, int(order-i+1)) ; j++) {
            floodFill(x+(j*dx), y+(j*dy), fillcolor, bgcolor);
            floodFill(x-(j*dx), y-(j*dy), fillcolor, bgcolor);
        }

    }

    ui->label->setPixmap(QPixmap::fromImage(image));

}


