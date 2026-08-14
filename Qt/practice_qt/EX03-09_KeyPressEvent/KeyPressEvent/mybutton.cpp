#include "mybutton.h"

MyButton::MyButton(QWidget *parent):QPushButton(parent)
{
    setStyleSheet("background-color:yellow;");
}

void MyButton::mousePressEvent(QMouseEvent *e)
{
    setStyleSheet("background-color:red;");
    click_count++;
}

void MyButton::mouseReleaseEvent(QMouseEvent *e)
{
    setStyleSheet("background-color:green;");
}

int MyButton::clickcount()
{
    return click_count;
}