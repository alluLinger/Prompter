#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QLabel>
#include <QTimer>
#include <QMainWindow>
#include "texteditdialog.hpp"
#include <list>
#include <vector>
#include <mutex>

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
    void on_inputPushButton_clicked();
    void updateComponents();
    void receiveText(QString text);
    void setCurrLine(int value);
    void timerUpdate();
    void on_startPushButton_clicked();

    void on_showLinesSpinBox_valueChanged(int arg1);

    void on_currLineVerticalSlider_sliderReleased();

    void on_fontPushButton_clicked();

private:
    Ui::MainWindow *ui;
    TextEditDialog *m_textEditDialog;
    QTimer *m_timer;
    QLabel *m_statusSpeedLabel;
    std::vector<QString> m_strVec;
    std::vector<QLabel*> m_labelVec;
    bool m_isStarted;
    size_t m_currentLineIndex;
    std::mutex m_updateLock;
};
#endif // MAINWINDOW_HPP
