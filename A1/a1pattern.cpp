#include "a1pattern.h"
#include "ui_a1pattern.h"

QImage image(1151, 701, QImage::Format_RGB888);

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

A1Pattern::A1Pattern(QWidget *parent) : QMainWindow(parent), ui(new Ui::A1Pattern){
    ui->setupUi(this);
    initQDraw();
    ui->ddaPB->setDisabled(true);
    ui->QDrawBresPB->setDisabled(true);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

A1Pattern::~A1Pattern(){
    delete ui;
}

void A1Pattern::initQDraw(){
    ui->labelTLp1->setText("Top-Left Corner");
    ui->labelBRp2->setText("Bottom-Left Corner");
    ui->ddaPB->setVisible(false);
    ui->QDrawBresPB->setText("QuickDraw");
}

void A1Pattern::initPDraw(){
    ui->labelTLp1->setText("Point 1 Coords");
    ui->labelBRp2->setText("Point 2 Coords");
    ui->ddaPB->setVisible(true);
    ui->QDrawBresPB->setText("Bresenham");
}

void A1Pattern::on_comboBox_currentIndexChanged(int index){
    switch (index) {
    case 0:
        initQDraw();
        break;
    case 1:
        initPDraw();
        break;
    case 2:
        image.fill(Qt::black);
        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->comboBox->setCurrentIndex(0);
        break;
    }
}


void A1Pattern::on_QDrawBresPB_pressed(){
    image.fill(Qt::black);
    QString x1 = ui->textEditX1->toPlainText();
    QString x2 = ui->textEditX2->toPlainText();
    QString y1 = ui->textEditY1->toPlainText();
    QString y2 = ui->textEditY2->toPlainText();

    if(ui->comboBox->currentIndex() == 0)
        drawPattern(x1.toInt(),y1.toInt(),x2.toInt(),y2.toInt());
    else
        bresenham(x1.toInt(),y1.toInt(),x2.toInt(),y2.toInt());
    ui->textEditX1->clear();
    ui->textEditY1->clear();
    ui->textEditX2->clear();
    ui->textEditY2->clear();
    ui->QDrawBresPB->setDisabled(true);
    ui->ddaPB->setDisabled(true);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void A1Pattern::on_ddaPB_pressed(){
    image.fill(Qt::black);
    int x1 = ui->textEditX1->toPlainText().toInt();
    int x2 = ui->textEditX2->toPlainText().toInt();
    int y1 = ui->textEditY1->toPlainText().toInt();
    int y2 = ui->textEditY2->toPlainText().toInt();
    dda(x1, y1, x2, y2);
    ui->textEditX1->clear();
    ui->textEditY1->clear();
    ui->textEditX2->clear();
    ui->textEditY2->clear();
    ui->QDrawBresPB->setDisabled(true);
    ui->ddaPB->setDisabled(true);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void A1Pattern::on_textEditX1_textChanged(){
    enablePBs();
}

void A1Pattern::on_textEditY1_textChanged(){
    enablePBs();
}

void A1Pattern::on_textEditX2_textChanged(){
    enablePBs();
}

void A1Pattern::on_textEditY2_textChanged(){
    enablePBs();
}

void A1Pattern::enablePBs(){
    if(ui->textEditX1->toPlainText()!= "" &&
       ui->textEditX2->toPlainText()!= "" &&
       ui->textEditY1->toPlainText()!= "" &&
       ui->textEditY2->toPlainText()!= ""){
        ui->QDrawBresPB->setEnabled(true);
        ui->ddaPB->setEnabled(true);
    }
}

void A1Pattern::dda(double x1, double y1, double x2, double y2){
    double dx = x2-x1;
    double dy = y2-y1;
    double length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double delx = dx/length;
    double dely = dy/length;

    double x = x1 + 0.5*sign(delx);
    double y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixelColor(x, y, value);
        x += delx;
        y += dely;
    }

}

void A1Pattern::bresenham(int x1, int y1, int x2, int y2){
    const bool steep = (abs(y2 - y1) > abs(x2 - x1));
    if(steep){
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = abs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = x2;

    for(int x = x1; x <= maxX; x++){
        if(steep)
            image.setPixelColor(y,x,value);
        else
            image.setPixelColor(x,y, value);

        error -= dy;
        if(error < 0){
            y += ystep;
            error += dx;
        }
    }
}

void A1Pattern::drawPattern(int x1, int y1, int x2, int y2){

    // Outer rectangle

    bresenham(x1, y1, x2, y1);
    bresenham(x2, y1, x2, y2);
    bresenham(x2, y2, x1, y2);
    bresenham(x1, y2, x1, y1);

    int xmid = (x1+x2)/2;
    int ymid = (y1+y2)/2;

    // Diamond

    dda(xmid, y1, x2, ymid);
    dda(x2, ymid, xmid, y2);
    dda(xmid, y2, x1, ymid);
    dda(x1, ymid, xmid, y1);

    int inx1 = (x1+xmid)/2;
    int iny1 = (y1+ymid)/2;
    int inx2 = (x2+xmid)/2;
    int iny2 = (y2+ymid)/2;

    // Inner rectangle

    bresenham(inx1, iny1, inx2, iny1);
    bresenham(inx2, iny1, inx2, iny2);
    bresenham(inx2, iny2, inx1, iny2);
    bresenham(inx1, iny2, inx1, iny1);


}
