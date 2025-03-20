#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    collatzSolver = new CollatzSolver(this);
    ui->setupUi(this);

    ui->stop_btn->setEnabled(false);

    ui->limitSpinBox->setMaximum(1000000000);

    ui->threadCountSlider->setMinimum(1);
    ui->threadCountSlider->setMaximum(std::thread::hardware_concurrency());


    connect(ui->exit_btn, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->start_btn, &QPushButton::clicked, this, &MainWindow::start);
    connect(ui->stop_btn, &QPushButton::clicked, this, &MainWindow::stop);

    connect(ui->threadCountSlider, &QAbstractSlider::valueChanged, this, &MainWindow::setCountOfThreads);
    emit ui->threadCountSlider->valueChanged(1);

    connect(collatzSolver, &CollatzSolver::stopSignal, this, &MainWindow::printOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete collatzSolver;
}

void MainWindow::start()
{
    stoppedButtonClick=false;
    ui->outputText->clear();
    collatzSolver->SetLongestSequence(0);
    collatzSolver->SetNumberWithLongestSequence(0);
    ui->stop_btn->setEnabled(true);
    ui->start_btn->setEnabled(false);
    collatzSolver->SetLimit(ui->limitSpinBox->value());
    collatzSolver->Start();
}

void MainWindow::stop()
{
    stoppedButtonClick=true;
    collatzSolver->Stop();
    ui->stop_btn->setEnabled(false);
    ui->start_btn->setEnabled(true);
}

void MainWindow::setCountOfThreads(int value)
{
    collatzSolver->SetThreadsCount(value);
    ui->numberOfThreadsLabel->setText(QString::number(value));
}

void MainWindow::printOutput()
{
    size_t longestSeq = collatzSolver->GetLongestSequence();
    size_t numberLongestSeq = collatzSolver->GetNumberWithLongestSequence();
    size_t time = collatzSolver->GetTime().count();
    QString ouputStr = "Longest sequence = " + QString::number(longestSeq) +
                       " With number = " + QString::number(numberLongestSeq) +
                       " Time = " + QString::number(time);

    ui->outputText->setText(ouputStr);
    ui->stop_btn->setEnabled(false);
    ui->start_btn->setEnabled(true);
}
