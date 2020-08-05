#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ConfigDialog; }
QT_END_NAMESPACE
class ConfigDialog: public QDialog{
    Q_OBJECT
public:
    ConfigDialog(QDialog *parent=nullptr);
    ~ConfigDialog();
    QString getName();
    QString getConfig();

private:
    Ui::ConfigDialog *ui;
    QString name, config;

};

#endif // CONFIGDIALOG_H
