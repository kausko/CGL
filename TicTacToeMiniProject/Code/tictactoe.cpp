#include "tictactoe.h"
#include "ui_tictactoe.h"
#include <QColorDialog>

QImage image;

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

TicTacToe::TicTacToe(QWidget *parent): QMainWindow(parent), ui(new Ui::TicTacToe){
    moveCntr = 0;
    liveCntr = 0;
    ui->setupUi(this);
    QMenu * menuGame = new QMenu(tr("&Game"));
    QMenu * menuEdit = new QMenu(tr("&Edit"));
    QMenu * menuHelp = new QMenu(tr("&Help"));
    ui->menubar->addMenu(menuGame);
    ui->menubar->addMenu(menuEdit);
    ui->menubar->addMenu(menuHelp);
    setupUndoButton(menuEdit);  //sets up Undo Button QAction object
    setupNewGame(menuGame); //sets up New Game QAction object
    ui->pushButtonNG->setVisible(false);
    ui->pushButtonNG->setDisabled(true);
}

TicTacToe::~TicTacToe(){
    delete ui;
}

void TicTacToe::setupNewGame(QMenu * menuGame){
    newGame = new QAction("New Game");
    newGame->setShortcut(QKeySequence::New);
    menuGame->addAction(newGame);
    connect(newGame, &QAction::triggered, [this](){
        moveCntr = 0;
        ui->lcdNumberP1->display(0);
        ui->lcdNumberP2->display(0);
        ui->textEditP1->setText("");
        ui->textEditP2->setText("");
        renewGame();
    });
}

void TicTacToe::setupUndoButton(QMenu * menuEdit){
    undoGame = new QAction("Undo Move");
    undoGame->setShortcut(QKeySequence::Undo);
    menuEdit->addAction(undoGame);
    undoGame->setDisabled(true);
    connect(undoGame, &QAction::triggered, [this](){
        if(!moves.empty()){
            undoGame->setEnabled(true);
            moves.back()->setText("");
            moves.back()->setEnabled(true);
            moves.pop_back();
            moveCntr--;
            liveCntr--;
            if(moves.empty())
                undoGame->setDisabled(true);
        }
        else
            undoGame->setDisabled(true);
        //turnIndicator();
    });
}

void TicTacToe::buttonPressed(QPushButton * button){
    button->setText(getChar());
    //turnIndicator();
    liveCntr++;
    button->setDisabled(true);
    moves.push_back(button);
    undoGame->setEnabled(true);
    if(liveCntr == 9){
        ui->pushButtonNG->setEnabled(true);
        ui->pushButtonNG->setVisible(true);
    }
    checkwin();
}

void TicTacToe::on_pushButton00_pressed(){
    buttonPressed(ui->pushButton00);
}

void TicTacToe::on_pushButton01_pressed(){
    buttonPressed(ui->pushButton01);
}

void TicTacToe::on_pushButton02_pressed(){
    buttonPressed(ui->pushButton02);
}

void TicTacToe::on_pushButton10_pressed(){
    buttonPressed(ui->pushButton10);
}

void TicTacToe::on_pushButton11_pressed(){
    buttonPressed(ui->pushButton11);
}

void TicTacToe::on_pushButton12_pressed(){
    buttonPressed(ui->pushButton12);
}

void TicTacToe::on_pushButton20_pressed(){
    buttonPressed(ui->pushButton20);
}

void TicTacToe::on_pushButton21_pressed(){
    buttonPressed(ui->pushButton21);
}

void TicTacToe::on_pushButton22_pressed(){
    buttonPressed(ui->pushButton22);
}

void TicTacToe::on_textEditP1_textChanged(){
    if(ui->textEditP1->toPlainText()==""){
        ui->textEditP1->setFrameStyle(6);
        ui->textEditP1->setStyleSheet("");
        return;
    }
    ui->textEditP1->setFrameStyle(0);
    ui->textEditP1->setStyleSheet("background-color: rgba(64, 50, 3, 0);");
}

void TicTacToe::on_textEditP2_textChanged(){
    if(ui->textEditP2->toPlainText()==""){
        ui->textEditP2->setFrameStyle(6);
        ui->textEditP2->setStyleSheet("");
        return;
    }
    ui->textEditP2->setFrameStyle(0);
    ui->textEditP2->setStyleSheet("background-color: rgba(64, 50, 3, 0);");
}

QString TicTacToe::getChar(){
    if((moveCntr++)%2)
        return "O";
    return "X";
}

void TicTacToe::checkwin(){
    QPushButton *pb00, *pb01, *pb02, *pb10, *pb11, *pb12, *pb20, *pb21, *pb22;//, winChar;
    pb00 = ui->pushButton00;
    pb01 = ui->pushButton01;
    pb02 = ui->pushButton02;
    pb10 = ui->pushButton10;
    pb11 = ui->pushButton11;
    pb12 = ui->pushButton12;
    pb20 = ui->pushButton20;
    pb21 = ui->pushButton21;
    pb22 = ui->pushButton22;
    if(pb00->text() == pb01->text() && pb01->text() == pb02->text() && pb00->text() != NULL)
        setWin(pb00, pb01, pb02);
    else if(pb10->text() == pb11->text() && pb11->text() == pb12->text() && pb10->text() != NULL)
        setWin(pb10, pb11, pb12);
    else if(pb20->text() == pb21->text() && pb21->text() == pb22->text() && pb20->text() != NULL)
        setWin(pb20, pb21, pb22);
    else if(pb00->text() == pb10->text() && pb10->text() == pb20->text() && pb00->text() != NULL)
        setWin(pb00, pb10, pb20);
    else if(pb01->text() == pb11->text() && pb11->text() == pb21->text() && pb01->text() != NULL)
        setWin(pb01, pb11, pb21);
    else if(pb02->text() == pb12->text() && pb12->text() == pb22->text() && pb02->text() != NULL)
        setWin(pb02, pb12, pb22);
    else if(pb00->text() == pb11->text() && pb11->text() == pb22->text() && pb00->text() != NULL)
        setWin(pb00, pb11, pb22);
    else if(pb02->text() == pb11->text() && pb11->text() == pb20->text() && pb02->text() != NULL)
        setWin(pb02, pb11, pb20);
}

void TicTacToe::updateScore(){
    if(--moveCntr%2)
        ui->lcdNumberP2->display(ui->lcdNumberP2->intValue()+1);
    else
        ui->lcdNumberP1->display(ui->lcdNumberP1->intValue()+1);
}

void TicTacToe::disableAll(bool val){
    ui->pushButton00->setDisabled(val);
    ui->pushButton01->setDisabled(val);
    ui->pushButton02->setDisabled(val);
    ui->pushButton10->setDisabled(val);
    ui->pushButton11->setDisabled(val);
    ui->pushButton12->setDisabled(val);
    ui->pushButton20->setDisabled(val);
    ui->pushButton21->setDisabled(val);
    ui->pushButton22->setDisabled(val);
}

void TicTacToe::setWin(QPushButton * pb1, QPushButton * pb2, QPushButton * pb3){
    updateScore();
    disableAll(true);
    pb1->setStyleSheet("font: 42pt \"Ubuntu\";background-color: rgb(115, 210, 22);color: rgb(255, 255, 255);");
    pb2->setStyleSheet("font: 42pt \"Ubuntu\";background-color: rgb(115, 210, 22);color: rgb(255, 255, 255);");
    pb3->setStyleSheet("font: 42pt \"Ubuntu\";background-color: rgb(115, 210, 22);color: rgb(255, 255, 255);");
    undoGame->setDisabled(true);
    ui->pushButtonNG->setEnabled(true);
    ui->pushButtonNG->setVisible(true);
    //ui->turnIndicator->setText(getChar()+" wins!");
}


void TicTacToe::on_pushButtonNG_pressed(){
    disableAll(false);
    ui->pushButtonNG->setDisabled(true);
    ui->pushButtonNG->setVisible(false);
    renewGame();
}

void TicTacToe::renewGame(){
    ui->pushButton00->setEnabled(true);
    ui->pushButton00->setText("");
    ui->pushButton00->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton01->setEnabled(true);
    ui->pushButton01->setText("");
    ui->pushButton01->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton02->setEnabled(true);
    ui->pushButton02->setText("");
    ui->pushButton02->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton10->setEnabled(true);
    ui->pushButton10->setText("");
    ui->pushButton10->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton11->setEnabled(true);
    ui->pushButton11->setText("");
    ui->pushButton11->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton12->setEnabled(true);
    ui->pushButton12->setText("");
    ui->pushButton12->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton20->setEnabled(true);
    ui->pushButton20->setText("");
    ui->pushButton20->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton21->setEnabled(true);
    ui->pushButton21->setText("");
    ui->pushButton21->setStyleSheet("font: 42pt \"Ubuntu\";");
    ui->pushButton22->setEnabled(true);
    ui->pushButton22->setText("");
    ui->pushButton22->setStyleSheet("font: 42pt \"Ubuntu\";");
    moves.clear();
    undoGame->setDisabled(true);
    liveCntr = 0;
    //turnIndicator();
}

void TicTacToe::turnIndicator(){
    QString p1, p2;
    p1 = ui->textEditP1->toPlainText();
    p2 = ui->textEditP2->toPlainText();
    if(p1 == "" || p1 == NULL)
        p1 = "Player X";
    if(p2 == "" || p2 == NULL)
        p2 = "Player O";
    if(moveCntr%2)
        ui->turnIndicator->setText(p2+"'s turn");
    else
        ui->turnIndicator->setText(p1+"'s turn");
}

void TicTacToe::dda(float x1, float y1, float x2, float y2)
{
    QRgb value = qRgb(0,255,0);
    float dx = x2-x1;
    float dy = y2-y1;
    float length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float delx = dx/length;
    float dely = dy/length;

    float x = x1 + 0.5*sign(delx);
    float y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixel(x,y,value);
        x += delx;
        y += dely;
    }

}

void TicTacToe::floodFill(float x, float y, QRgb fillcolor, QRgb bgcolor){
    if(image.pixel(x,y) == bgcolor){
        image.setPixel(x,y,fillcolor);
        floodFill(x+1,y,fillcolor,bgcolor);
        floodFill(x,y+1,fillcolor,bgcolor);
        floodFill(x-1,y,fillcolor,bgcolor);
        floodFill(x,y-1,fillcolor,bgcolor);
    }
}
