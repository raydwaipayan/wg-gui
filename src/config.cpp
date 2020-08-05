#include "config.h"

QMap<QString,QString> Config::parser(const QString &data, const QChar &delimiter)
{
    QStringList lines=data.split('\n');
    QMap<QString,QString> cmap;
    for(int i=0;i<lines.size();i++)
    {
        QString &line=lines[i];
        int idx=-1;
        for(int j=0;j<line.size();i++)
        {
            if(line[j]==delimiter)
            {
                idx=j;
                break;
            }
        }
        if(idx==-1)continue;
        QString fp=line.mid(0,idx).trimmed();
        QString lp=line.mid(idx+1,(line.size()-idx-1)).trimmed();
        cmap[fp]=lp;
    }
    return cmap;
}

