#ifndef CONFIG_H
#define CONFIG_H

#include <QMap>

class Config{
public:
    QMap<QString, QString> static parser(const QString &data, const QChar &delimiter);
};

#endif // CONFIG_H
