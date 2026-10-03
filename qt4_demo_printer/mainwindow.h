#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myprinter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startPrinter_clicked();

    void on_startPrinterPage_clicked();

    void on_startPrintPreview_clicked();

    void on_setPrintPixmap_clicked();

    void on_getPrintPixmap_clicked();

    void on_PrintPreview_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap m_Pixmap;		//界面控件内像素
    CPrinter* myprint;
};
#endif // MAINWINDOW_H
