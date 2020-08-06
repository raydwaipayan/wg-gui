#include "config.h"
#include <dirent.h>
#include <QDebug>
#include <QProcess>

QMap<QString, QString>Config::parser(const QString& data, const QChar& delimiter)
{
    QStringList lines = data.split('\n');
    QMap<QString, QString> cmap;

    for (int i = 0; i < lines.size(); i++)
    {
        QString& line = lines[i];
        int idx       = -1;

        for (int j = 0; j < line.size(); j++)
        {
            if (line[j] == delimiter)
            {
                idx = j;
                break;
            }
        }

        if (idx == -1) continue;
        QString fp = line.mid(0, idx).trimmed();
        QString lp = line.mid(idx + 1, (line.size() - idx - 1)).trimmed();
        cmap[fp] = lp;
    }
    return cmap;
}

QString Config::read(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) return QString();

    QTextStream in(&file);
    QString     data = in.readAll();

    return data;
}

void Config::write(const QString& data, const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) return;

    QTextStream out(&file);

    out << data;
}

QVector<QPair<QString, QMap<QString, QString> > >Config::getConfigs(
    const QString& path)
{
    struct dirent *entry;
    DIR *dp;
    QVector<QPair<QString, QMap<QString, QString> > > data;

    dp = opendir(path.toUtf8());

    if (dp == NULL) {
        return data;
    }

    while ((entry = readdir(dp)))
    {
        QString temp = QString::fromUtf8(entry->d_name);

        if ((temp == ".") || (temp == "..")) continue;

        QString content             = Config::read(path + temp);
        QMap<QString, QString> dict = Config::parser(content, '=');
        data.push_back(qMakePair(temp.split('.')[0], dict));
    }
    closedir(dp);
    return data;
}
