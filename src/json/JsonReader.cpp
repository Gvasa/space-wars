#include "JsonReader.h"

JsonParseStatus parseJsonFile(std::string fileName, JsonValue& value)
{
  std::ifstream in(fileName);
  std::string contents((std::istreambuf_iterator<char>(in)), 
  std::istreambuf_iterator<char>());

  contents.append("0");
  char* source = new char[contents.size()];
  source = const_cast<char*>(contents.c_str());
  char* end;

  JsonAllocator allocator;
  JsonParseStatus status = json_parse(source, &end, &value, allocator);

  return status;
}

void print(JsonValue o)
{
    switch (o.getTag())
    {
        case JSON_TAG_NUMBER:
            printf("%g\n", o.toNumber());
            break;
        case JSON_TAG_BOOL:
            printf("%s\n", o.toBool() ? "true" : "false");
            break;
        case JSON_TAG_STRING:
            printf("\"%s\"\n", o.toString());
            break;
        case JSON_TAG_ARRAY:
            for (auto i : o)
            {
                print(i->value);
            }
            break;
        case JSON_TAG_OBJECT:
            for (auto i : o)
            {
                printf("%s = ", i->key);
                print(i->value);
            }
            break;
        case JSON_TAG_NULL:
            printf("null\n");
            break;
    }
}


  