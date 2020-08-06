#ifndef CONFIG_H
#define CONFIG_H

#include <QMap>
#include <QFile>
#include <QDataStream>

class Config {
public:

    QMap<QString, QString>static parser(
        const QString& data,
        const QChar  & delimiter);

    QString static                                          read(const QString&);

    void static                                             write(const QString&,
                                                                  const QString&);
    QVector<QPair<QString, QMap<QString, QString> > >static getConfigs(
        const QString&);
};

#endif // CONFIG_H
