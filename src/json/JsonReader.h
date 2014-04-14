#ifndef JSONREADER_H
#define JSONREADER_H

#include <fstream>
#include <stdlib.h> 
#include <iostream>
#include <string>

#include "json/gason.h"

JsonParseStatus parseJsonFile(std::string fileName, JsonValue& value);

void print(JsonValue o);


#endif