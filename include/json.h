#ifndef JSON_H
#define JSON_H
#include <string>

class Json{
    private:
        int capacity = 10;
        int size = 0;
        std::string base;
        std::string** tags = new std::string*[capacity];
        std::string** values = new std::string*[capacity];
        void expand();
        int getIndex(std::string tag);

    
    public:
        void addValue(std::string tag, std::string value);
        std::string getValue(std::string tag);
        void changeValue(std::string tag, std::string value);
        void remove(std::string tag);
        std::string returnJson();
        Json(std::string base);
};


#endif