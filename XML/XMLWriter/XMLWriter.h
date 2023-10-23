#ifndef XMLWriter_H
#define XMLWriter_H

#include <QDomElement>
#include <QFile>

class XMLWriter {
private:
    QDomDocument document;
    QFile file;

    void openFile();
    void openIfExist();

    QDomElement getRoot(const QString &listName, QDomDocument &document);
    QDomElement createFileElement(const QString &name, const QString &extension,
                                  const QString &date, const QString &size,
                                  QDomDocument &document);

    void writeToFile(QDomDocument &document);
    bool checkAndUpdateFile(const QString &name, const QString &extension,
                            const QString &date, const QString &size,
                            QDomElement &root);
    QDomElement createTextElement(const QString &tagName, const QString &text,
                                  QDomDocument &document);
    bool isFileExists(const QString &listName, const QString &fileName);

public:
    explicit XMLWriter(const QString &fileName);
    ~XMLWriter();
    void close();
    void writeInFile(const QString &listName, const QString &name,
                     const QString &extension, const QString &date,
                     const QString &size);
};

#endif  // XMLWriter_H
