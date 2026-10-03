#include "myprinter.h"
#include<QDebug>
CPrinter::CPrinter() :m_pPrinter(NULL)
{
    if(m_pPrinter == NULL)
        m_pPrinter = new QPrinter;
}

CPrinter::~CPrinter()
{
    if (m_pPrinter != nullptr)
        delete m_pPrinter;
    m_pPrinter = NULL;
}

void CPrinter::startPrinter(QPixmap &pixmap)
{
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        qDebug()<<"startPrinter : printerName="<<printer.printerName();
        QPainter painterPixmap;
        painterPixmap.begin(&printer);
        painterPixmap.drawPixmap(0, 0, pixmap);
        painterPixmap.end();
    }
}

void CPrinter::startPrinterPage()
{
    QPrinter printer;
    QPageSetupDialog pageSetUpdlg(&printer);
    if (pageSetUpdlg.exec() == QDialog::Accepted)
    {
        printer.setOrientation(QPrinter::Landscape);
    }
    else
    {
        printer.setOrientation(QPrinter::Portrait);
    }
}

void CPrinter::startPrintPreview(QPixmap &pixmap)
{
    if (pixmap.isNull())
        return;
    setPrintPixmap(pixmap);
    PrintPreview();
}

void CPrinter::setPrintPixmap(QPixmap &pixmap)
{
    m_Pixmap = pixmap;
}

QPixmap CPrinter::getPrintPixmap()
{
    return m_Pixmap;
}

void CPrinter::PrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog previewDialog(&printer);
    connect(&previewDialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(Slot_Preview(QPrinter*)));
    previewDialog.exec(); //打印预览对话框显示，paintRequest触发
}

void CPrinter::Slot_Preview(QPrinter* printer)
{
    qDebug()<<"Slot_Preview : printerName="<<printer->printerName();
    QPainter painterPixmap;
    painterPixmap.begin(printer);
    painterPixmap.drawPixmap(0, 0, getPrintPixmap());
    painterPixmap.end();
}



