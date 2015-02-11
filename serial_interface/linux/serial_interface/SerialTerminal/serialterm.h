#ifndef SERIALTERM_H
#define SERIALTERM_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>

namespace Ui {
class serialterm;
}

class serialterm : public QMainWindow
{
    Q_OBJECT

public:
    explicit serialterm(QWidget *parent = 0);
    ~serialterm();

private slots:
    void on_btnOpen_clicked();

    void on_actionClear_triggered();

    void read_data();

    bool eventFilter(QObject *obj, QEvent *event);

    void on_actionExit_triggered();

private:
    Ui::serialterm *ui;
    QSerialPort* my_port;
};

#endif // SERIALTERM_H
