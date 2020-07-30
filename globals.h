#ifndef GLOBALS_H
#define GLOBALS_H

#include <QStringList>

/*************************/
/**** UNICODE SORTING ****/
/*************************/
extern void sortUnicode(QStringList &sList)
{
    std::sort(sList.begin(), sList.end(), [](const QString &s1, const QString &s2){
        return s1.localeAwareCompare(s2) < 0;
    });
}

#endif // GLOBALS_H
