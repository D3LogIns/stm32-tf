#include "serialterm.h"
#include "ui_serialterm.h"

serialterm::serialterm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serialterm)
{
    ui->setupUi(this);

    ui->txtTerminal->installEventFilter(this);

    my_port= new QSerialPort(this);
    connect(my_port,SIGNAL(readyRead()),SLOT(read_data()));
}

serialterm::~serialterm()
{
    delete ui;
}

bool serialterm::eventFilter(QObject *obj, QEvent *event){
    if(event->type()== QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent* >(event);
        if(my_port->isOpen()){
            QString s = keyEvent->text();
            if(s.length()){
                char ch = s.at(0).toLatin1();
                QByteArray chb= QByteArray::fromRawData(&ch,1);
                my_port->write(chb);
            }
        }
        return true;
    }
    else{
        return QObject::eventFilter(obj, event);
    }
}

void serialterm::on_btnOpen_clicked()
{
    QString dev_name="/dev/";
    dev_name += ui->cmbPort->currentText();

    if(ui->btnOpen->text()=="Open"){

        my_port->setPortName(dev_name);

        if(my_port->open(QIODevice::ReadWrite)){
            my_port->setBaudRate(QSerialPort::Baud38400,QSerialPort::AllDirections);
            my_port->setDataBits(QSerialPort::Data8);
            my_port->setStopBits(QSerialPort::OneStop);
            my_port->setFlowControl(QSerialPort::NoFlowControl);
            my_port->setParity(QSerialPort::NoParity);
            QMessageBox::information(this,"success","port success on "+dev_name );
            ui->btnOpen->setText("Close");
        }
        else{
            QMessageBox::critical(this,"Failed","port failed on "+dev_name);
            ui->btnOpen->setText("Open");
        }
    }

    else{
        if(my_port->isOpen()){
            my_port->close();
        }
        ui->btnOpen->setText("Open");
    }
}

void serialterm::read_data(){
    QByteArray rawdata=my_port->readAll();
    ui->txtTerminal->insertPlainText(rawdata);
}

void serialterm::on_actionClear_triggered()
{
    ui->txtTerminal->clear();
}

void serialterm::on_actionExit_triggered()
{
    if(my_port->isOpen()){my_port->close();}
    QApplication::quit();
}
