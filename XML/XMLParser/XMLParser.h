#ifndef XMLParser_H
#define XMLParser_H

#include <QFile>

class XMLParser {
protected:
    QFile file;
    virtual void openFile() = 0;
    void close();

public:
    XMLParser(const QString fileName);
    virtual ~XMLParser();
};

#endif  // XMLParser_H