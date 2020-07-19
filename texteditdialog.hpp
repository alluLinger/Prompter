#ifndef TEXTEDITDIALOG_HPP
#define TEXTEDITDIALOG_HPP

#include <QDialog>

namespace Ui {
class TextEditDialog;
}

class TextEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextEditDialog(QWidget *parent = nullptr);
    ~TextEditDialog();
signals:
    void sendText(QString);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::TextEditDialog *ui;
};

#endif // TEXTEDITDIALOG_HPP
