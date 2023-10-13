#ifndef XMLWriter_H
#define XMLWriter_H

#include <QDomElement>

#include "XMLParser.h"

class XMLWriter : public XMLParser {
private:
    void openFile() override;
    void openIfExist();

    QDomElement getRoot(const QString &listName, QDomDocument &document);
    QDomElement createFileElement(const QString &name, const QString &extension,
                                  const QString &date, const QString &size,
                                  QDomDocument &document);

    void writeToFile(QDomDocument &document);
    bool checkAndUpdateFile(const QString &name, const QString &extension,
                            const QString &date, const QString &size,
                            QDomElement &root);

public:
    XMLWriter(const QString &fileName);
    ~XMLWriter() = default;
    void writeInFile(const QString listName, const QString name,
                     const QString extension, const QString date,
                     const QString size);
};

#endif  // XMLWriter_H
