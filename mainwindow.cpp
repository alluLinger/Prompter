#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QFont>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_isStarted = false;
    m_currentLineIndex = 0;
    //init status bar
    QLabel *statusLable = new QLabel(this);
    statusLable->setText(tr("https://linger.ink"));
    ui->statusbar->addWidget(statusLable);
    m_statusSpeedLabel = new QLabel(this);
    ui->statusbar->addPermanentWidget(m_statusSpeedLabel);
    //init center
    ui->speedHorizontalSlider->setRange(50,250);
    ui->speedHorizontalSlider->setValue(150);
    ui->showLinesSpinBox->setRange(1,10);

    for(int i = 0; i < ui->showLinesSpinBox->value(); ++i){
        QLabel *label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPixelSize(14);
        label->setFont(font);
        ui->verticalLayout->addWidget(label);
        m_labelVec.push_back(label);
    }

    //init function
    m_textEditDialog = new TextEditDialog(this);
    connect(m_textEditDialog,SIGNAL(sendText(QString)),this,SLOT(receiveText(QString)));
    connect(ui->currLineVerticalSlider, SIGNAL(valueChanged(int)), this, SLOT(setCurrLine(int)));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

    updateComponents();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_inputPushButton_clicked()
{
    m_textEditDialog->show();
}

void MainWindow::updateComponents()
{
    // add lines
    std::lock_guard<std::mutex> guard(m_updateLock);
    int tempIndex = m_currentLineIndex;
    for(size_t i = 0; i < m_labelVec.size(); ++i){
        QString content = "";
        if(static_cast<size_t>(tempIndex) < m_strVec.size() && !m_strVec.at(tempIndex).isEmpty()){
            content = m_strVec.at(tempIndex);
        }
        QString strLine = QString::number(tempIndex + 1) + ": " + content;
        m_labelVec.at(i)->setText(strLine);
        tempIndex++;
    }
}

void MainWindow::receiveText(QString text)
{
    m_strVec.clear();
    auto lines = text.split("\n");
    for(auto line = lines.begin(); line != lines.end(); line++){
        m_strVec.push_back(*line);
    }
    m_currentLineIndex = 0;
    int lineCount = m_strVec.size();
    ui->currLineVerticalSlider->setRange(1, lineCount);
    ui->currLineVerticalSlider->setValue(lineCount); // include updateComponents();

}

void MainWindow::setCurrLine(int value)
{
    m_currentLineIndex = m_strVec.size() - value;
    updateComponents();
}

void MainWindow::timerUpdate()
{
    if(m_currentLineIndex < 0){
        return;
    }
    m_currentLineIndex++;
    updateComponents();
}


void MainWindow::on_startPushButton_clicked()
{
    m_isStarted = !m_isStarted;
    ui->startPushButton->setText(m_isStarted ? tr("Pause") : tr("Start"));
    if(m_isStarted){
        int speed = 1000 * 50  / (ui->speedHorizontalSlider->value() / 10.0);
        QString speedInfo = tr("Speed: ") + QString::number(speed / 1000.0) + tr("s/line");
        m_statusSpeedLabel->setText(speedInfo);
        m_timer->start(speed);
    }else {
        m_timer->stop();
    }
}

void MainWindow::on_showLinesSpinBox_valueChanged(int arg1)
{
    std::lock_guard<std::mutex> guard(m_updateLock);
    int num = arg1 - ui->verticalLayout->count();
    if(num >= 1){
        int num = arg1 - ui->verticalLayout->count();
        for(int i = 0; i < num; ++i){
            QLabel *label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            QFont font;
            font.setPixelSize(14);
            label->setFont(font);
            ui->verticalLayout->addWidget(label);
            m_labelVec.push_back(label);
        }
    }else {
        for(int i = 0; i <= (-num); ++i){
            QLabel *label = m_labelVec.back();
            m_labelVec.pop_back();
            ui->verticalLayout->removeWidget(label);
            delete label;
        }
    }
}

void MainWindow::on_fontPushButton_clicked()
{
    QFontDialog * fontDialog = new QFontDialog(this);
    bool ok = true;
    QFont font = fontDialog->getFont(&ok);
    std::lock_guard<std::mutex> guard(m_updateLock);
    for(auto e: m_labelVec){
        e->setFont(font);
    }
}

void MainWindow::on_currLineVerticalSlider_sliderReleased()
{

}
