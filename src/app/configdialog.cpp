// Copyright 2020 Banbreach
// Written by Dwaipayan Ray
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "configdialog.h"
#include "ui_configdialog.h"

wg::ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(
        new Ui::ConfigDialog)
{
    ui->setupUi(this);
}

wg::ConfigDialog::~ConfigDialog()
{
    delete ui;
}

QString wg::ConfigDialog::getName()

// return the name from the ui's name component
{
    return ui->name->text();
}

QString wg::ConfigDialog::getConfig()

// return the configuration from the ui's config component
{
    return ui->config->toPlainText();
}

void wg::ConfigDialog::setName(const QString& name)

// Sets the text in ui's name component
{
    ui->name->setText(name);
}

void wg::ConfigDialog::setConfig(const QString& config)

// Sets the text in ui's config component
{
    ui->config->setText(config);
}

void wg::ConfigDialog::disableName(bool b)

// Disable the name component
{
    ui->name->setDisabled(b);
}

void wg::ConfigDialog::disableConfig(bool b)

// Disable the config component
{
    ui->config->setDisabled(b);
}
