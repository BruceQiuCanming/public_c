#include "widget.h"
#include "ui_widget.h"
#include "device_src.h"
#include "serial.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->spinBox->setMaximum(100);
      ui->horizontalSlider->setMaximum(100);

    int val;
    DevCmd_GetBright(&val);
    ui->spinBox->setValue(val);
    ui->horizontalSlider->setValue(val);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_1_clicked()
{
     Beep(800,3);
}

void Widget::on_spinBox_valueChanged(int arg1)
{
     DevCmd_SetBright(arg1);
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
      ui->spinBox->setValue(ui->horizontalSlider->value());
}

void Widget::on_pushButton_clicked()
{
    trun_backlight(0);
}



void Widget::on_pushButton_2_clicked()
{
    int fd;
    int ret =setcom_ctl();

    fd = ev_plc_init(115200,8,1,0);
    char buff[]={'f','l','e','x','e','m','-','q','t'};
    if(fd <= 0 )
    {
        return;
    }
    else{
        write(fd,buff,9);
    }
    ::close(fd);
}
