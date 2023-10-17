#include "XMLParser.h"

#include <QDebug>
#include <iostream>

void XMLParser::close() { this->file.close(); }

XMLParser::XMLParser(const QString fileName) : file(fileName) {}

XMLParser::~XMLParser() { close(); }