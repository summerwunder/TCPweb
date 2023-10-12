#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_server=new QTcpServer(this);
    connect(ui->openBtn,&QPushButton::clicked,this,&Widget::openBtnFunc);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openBtnFunc()
{
    QString str=ui->portLineEdit->text();
    m_server->listen(QHostAddress::Any,str.toUShort());
    ui->openBtn->setDisabled(true);
}

