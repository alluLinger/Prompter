#include "texteditdialog.hpp"
#include "ui_texteditdialog.h"

TextEditDialog::TextEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextEditDialog)
{
    ui->setupUi(this);
}

TextEditDialog::~TextEditDialog()
{
    delete ui;
}

void TextEditDialog::on_buttonBox_accepted()
{
    emit sendText(this->ui->plainTextEdit->toPlainText());
}
