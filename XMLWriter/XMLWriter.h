#ifndef XMLWriter_H
#define XMLWriter_H

#include "XMLParser.h"

class XMLWriter : public XMLParser {
private:
    void openFile() override;
    void openIfExist();


public:
    XMLWriter(const QString fileName);
    ~XMLWriter() = default;
    void writeInFile(const QString listName, const QString name,
                     const QString extension, const QString date,
                     const QString size);
};

#endif  // XMLWriter_H