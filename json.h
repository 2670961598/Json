//
// Created by 24260 on 2024/1/14.
//

#ifndef NAVIGATION_JSON_H
#define NAVIGATION_JSON_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class JsonValue;

// 定义JSON值类型
enum class JsonType {
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOLEAN,
    NULL_VALUE
};

//定义JSON
class Json {
    std::string filename;
    std::string original_json;
    std::vector<std::pair<std::string ,JsonValue>> value_array;

public:

    //构造函数
    Json() = default;
    //拷贝构造
    Json(const Json &json);

    bool read_json(std::string filename);
    void explain_json(std::string data);
    bool write_json(std::string filename);
    std::string make_json(std::pair<std::string ,JsonValue> value);
    void show_json();
    bool add_json(std::pair<std::string ,JsonValue> value);

    //获取JSON值
    JsonValue operator[](int i) const;
    JsonValue operator[](const std::string &key) const;

};

//定义JSON值
class JsonValue {
public:
    //构造函数
    JsonValue() = default;
    explicit JsonValue(const std::string &value);
    explicit JsonValue(double value);
    explicit JsonValue(bool value);
    explicit JsonValue(const Json &value);
    explicit JsonValue(const std::vector<double> &value);

    //数据类型
    JsonType type;
    //字符串
    std::string value_string;
    //数字
    double value_number;
    //布尔值
    bool value_boolean;
    //对象
    Json value_object;
    //数组
    std::vector<double> value_array;
};



#endif //NAVIGATION_JSON_H
