#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPrinter>
#include <QPainter>
#include <QPrinterInfo>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myprint = new CPrinter();
    m_Pixmap.load("/root/800_480.bmp");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete myprint;
}

void MainWindow::on_startPrinter_clicked()
{

    myprint->startPrinter(m_Pixmap);
}

void MainWindow::on_startPrinterPage_clicked()
{
    myprint->startPrinterPage();
}

void MainWindow::on_startPrintPreview_clicked()
{
    myprint->startPrintPreview(m_Pixmap);
}

void MainWindow::on_setPrintPixmap_clicked()
{
    //刷新系统的打印机设置
      QList<QPrinterInfo> printer_list = QPrinterInfo::availablePrinters();
      ui->comboBox->clear();
      //删除现有已配置打印机
      for(auto& printer_info : printer_list)
      {
          ui->textBrowser->append(QString("lpadmin -x ")+printer_info.printerName());
          system(QString("lpadmin -x " + printer_info.printerName()).toStdString().data());
          ui->comboBox->addItem(printer_info.printerName());
      }
       ui->textBrowser->append(QString("hp-setup -b usb -x -a"));
      //重新打印机，需要打印机已正确连接的情况下再执行此命令
      system("hp-setup -b usb -x -a");//网络打印机命令：hp-setup -i -a -x 192.168.3.173 ,(192.168.3.173为打印机ip)
}

void MainWindow::on_getPrintPixmap_clicked()
{
    QString printer_name =  ui->comboBox->currentText();
    //检测当前已连接打印机状态，如果设置的名称与已连接的打印机名称不一致则不执行打印
    FILE* fp = popen(QString("lpinfo -v 2>&1").toStdString().c_str(), "r");
    if(NULL == fp)
    {
        ui->textBrowser->append(QString("popen lpinfo -v error!"));
        return;
    }

    bool connected = false;
    char result_buf[1024] = {0};
    std::string result_str;
    while(fgets(result_buf, 1024, fp) != NULL)
    {
        if('\n' == result_buf[strlen(result_buf) - 1])
        {
            result_buf[strlen(result_buf) - 1] = '\0';
        }

        result_str = result_buf;
        if(!printer_name.isEmpty())
        {
            if(result_str.find(printer_name.toStdString().c_str()) != std::string::npos)
            {
                ui->textBrowser->append(QString("find ") + printer_name);
                connected = true;
                break;
            }
        }
        else
        {
            ui->textBrowser->append(QString("printer_name isEmpty! "));
            break;
        }
         ui->textBrowser->append(QString(result_str.c_str()));
        result_str.clear();
        memset(result_buf, 0, 1024);
    }

    int rc = pclose(fp);
    if(-1 == rc)
    {
        ui->textBrowser->append(QString("lpinfo pclose!"));
         perror("lpinfo pclose");
    }

    if(!connected)
    {
        ui->textBrowser->append(QString("not connected return! "));
        return;
    }
    system("cancel -a");
    system(QString("cupsenable %1").arg(printer_name).toStdString().c_str());
}

void MainWindow::on_PrintPreview_clicked()
{
//
}
