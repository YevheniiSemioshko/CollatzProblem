#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "collatzsolver.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void start();
    void stop();
    void setCountOfThreads(int value);
    void printOutput();
private:
    Ui::MainWindow *ui;
    CollatzSolver *collatzSolver;
    bool stoppedButtonClick;
};
#endif // MAINWINDOW_H
