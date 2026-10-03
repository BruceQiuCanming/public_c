#ifndef MYPRINTER_H
#define MYPRINTER_H

#include <QPrinter>
#include <QPixmap>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QPageSetupDialog>

class CPrinter : public QObject
{
    Q_OBJECT
public:
    CPrinter();
    ~CPrinter();

public://打印窗口
    /*
     *  @brief:打印窗口
     */
    void startPrinter(QPixmap &pixmap);

public://打印页面设置

    /*
     *  @brief:页面设置
     */
    void startPrinterPage();

public://打印预览
    /*
     *  @brief:开启打印预览
     */
    void startPrintPreview(QPixmap &pixmap);
private:
    /*
     *  @brief:设置预览内容
     */
    void setPrintPixmap(QPixmap &pixmap);
    /*
     *  @brief:获取打印内容
     */
    QPixmap getPrintPixmap();
    /*
     *  @brief:开启打印预览窗口
     */
    void PrintPreview();

private slots:

    void Slot_Preview(QPrinter* printer);

private:
    QPixmap m_Pixmap;		//界面控件内像素
    QPrinter *m_pPrinter;	//打印机
};

#endif // MYPRINTER_H
