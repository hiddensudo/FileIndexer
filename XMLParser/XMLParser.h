#ifndef XMLParser_H
#define XMLParser_H

#include <string>

class XMLParser {
private:
    QFile file;
    bool isReadOnly;
protected:
    void openReadOnly();
    void openWriteOnly();
    void close();

    void openIfFileExist();

public:
    XMLParser(const QString fileName, bool isReadOnly);
    virtual ~XMLParser();
};

#endif  // XMLParser_H