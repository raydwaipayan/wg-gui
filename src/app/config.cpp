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


#include <QDebug>
#include <QProcess>
#include <QDirIterator>

#include "config.h"
#include <dirent.h>


auto wg::Config::parser(const QString& data,
                        const QChar  & delimiter) -> QMap<QString, QString>

// Takes in the config data read from the file, and attempts to parse
// the input by breaking the lines into two parts on basis of a delimeter.
{
    QStringList lines = data.split('\n');
    QMap<QString, QString> cmap;

    for (const auto& line:lines) {
        int idx = line.indexOf(delimiter);

        if (idx == -1) continue;

        // fp will take in the string on left part of the delimeter
        QString fp = line.mid(0, idx).trimmed();

        // lp contains the string on the right part of the delimeter
        QString lp = line.mid(idx + 1, (line.size() - idx - 1)).trimmed();
        cmap[fp] = lp;
    }
    return cmap;
}

QString wg::Config::read(const QString& filename)

// Opens and read the config file supplied. Returns a QString containing the
// read data. In case of failure an emptry string will be returned.

{
    QFile   file(filename);
    QString data = QString();

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        data = in.readAll();
    }
    return data;
}

bool wg::Config::write(const QString& data, const QString& filename)

// Opens and writes the data into the file specified by filename. Return values
// depends on the success of the operation
{
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);
        out << data;
        return true;
    }
    return false;
}

auto wg::Config::getConfigs(
    const QString& rootdir) -> QVector<QPair<QString, QString> >

// Iterates over the directory and reads and stores all configuration
{
    QVector<QPair<QString, QString> > data;

    // QDirIterator filters over files only having the .conf extension which is
    // meant for configuration files

    QDirIterator it(rootdir, QStringList() << "*.conf", QDir::Files,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        auto temp = it.next();

        // The config file is first read, then parsed and then pushed into the
        // given data container
        QString content = Config::read(temp);


        // Split the path string and get the tunnel name, which is equal to the
        // file name
        QStringList pieces = temp.split('/');
        data.push_back(qMakePair(pieces[pieces.length() - 1].split('.')[0],
                                 content));
    }
    return data;
}

auto wg::Config::getMappedConfigs(const QVector<QPair<QString,
                                                      QString> >& data) -> QVector<QPair<QString,
                                                                                         QMap<QString,
                                                                                              QString> > >
{
    QVector<QPair<QString, QMap<QString, QString> > > mapped_data;

    for (auto x:data)
    {
        auto dict = parser(x.second, '=');
        mapped_data.push_back(qMakePair(x.first, dict));
    }
    return mapped_data;
}
