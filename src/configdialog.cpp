#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QDialog *parent): QDialog(parent), ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

QString ConfigDialog::getName()
{
    return ui->name->text();
}
QString ConfigDialog::getConfig()
{
    return ui->config->toPlainText();
}
void ConfigDialog::on_buttonBox_accepted()
{

}
