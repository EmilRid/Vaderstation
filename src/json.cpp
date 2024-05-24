#include <string>
#include "json.h"

void Json::expand(){
    std::string** tempTags = tags;
    std::string** tempValues = values;
    tags = new std::string*[capacity+10];
    values = new std::string*[capacity+10];
    for(int i=0; i<size; i++){
        tags[i] = tempTags[i];
        values[i] = tempValues[i];
    }
    delete tempTags;
    delete tempValues;
    capacity += 10;
}
int Json::getIndex(std::string tag){
    int index = -1;
    for(int i=0; i<Json::size; i++){
        if(*Json::tags[i] == tag){
            index = i;
            break;
        }
    }
    return index;
}
void Json::addValue(std::string tag, std::string value){
    if(size>=capacity){
        expand();
    }
    tags[size] = new std::string(tag);
    values[size] = new std::string(value);
    size++;
}
std::string Json::getValue(std::string tag){
    int index = getIndex(tag);
    if(index != -1){
        return(*values[index]);
    }
    else{
        return("Missing value");
    }
}
void Json::changeValue(std::string tag, std::string value){
    int index = getIndex(tag);
    if(index != -1){
        *values[index] = value;
    }
}
void Json::remove(std::string tag){
    int index = getIndex(tag);
    if(index != -1){
        delete tags[index];
        delete values[index];
        for(int i = index; i<size-1; i++){
            tags[i] = tags[i+1];
            values[i] = values[i+1];
        }
        tags[size] = nullptr;
        values[size] = nullptr;
        size--;
    }
}
std::string Json::returnJson(){
    std::string output = "{\n\"" + base + "\": {";
    for(int i=0; i<size; i++){
        //this is bad
        if(i!=0){
            output+=",";
        }
        output += "\n\"" + *tags[i] + "\": " + *values[i];
    }
    output += "\n}\n}";
    return output;
}

Json::Json(std::string base){
    Json::base = base;
}

Json::~Json(){
    for(int i=0; i<size; i++){
        delete tags[i];
        delete values[i];
    }
    delete tags;
    delete values;
}
