//
// Created by 24260 on 2024/1/14.
//

#include "json.h"

bool Json::read_json(std::string filename) {
    std::ifstream file;
    file.open(filename, std::ios::in);
    //输出文件是否打开成功
    if (file.is_open()) {
        std::cout << "open file success" << std::endl;
        std::string str;
        while (getline(file, str)) {
//            std::cout << str << std::endl;
            original_json += str;
        }
        file.close();
//        std::cout << original_json << std::endl;
        explain_json(original_json);
        return true;
    } else {
        std::cout << "open file failed" << std::endl;
        return false;
    }
}

void Json::explain_json(std::string data) {
    std::cout << "explain_json " << data << std::endl;

    std::string data_json = data;

    //去除空格
//    std::cout << "remove space" << std::endl;
    for (int i = 0; i < data_json.size(); i++) {
        if (data_json[i] == ' ') {
            data_json.erase(i, 1);
            i--;
        }
    }
//    std::cout << "remove space finished" << data_json << std::endl;

    //去除最外层的大括号
//    std::cout << "remove {}" << std::endl;
    data_json.erase(0, 1);
    data_json.erase(data_json.size() - 1, 1);
//    std::cout << "remove {} finished" << data_json << std::endl;

    //按照逗号分割，但注意逗号可能在对象中，或数组中
//    std::cout << "split by ," << std::endl;
    std::vector<std::string> json_vector;
    int start = 0;
    for (int i = 0; i < data_json.size(); i++) {
        if (data_json[i] == ',') {
            //判断是否在对象中
            int count = 0;
            for (int j = start; j < i; j++) {
                if (data_json[j] == '{') {
                    count++;
                } else if (data_json[j] == '}') {
                    count--;
                }
            }
            //判断是否在数组中
            int count2 = 0;
            for (int j = start; j < i; j++) {
                if (data_json[j] == '[') {
                    count2++;
                } else if (data_json[j] == ']') {
                    count2--;
                }
            }
            if (count == 0 && count2 == 0) {
                json_vector.push_back(data_json.substr(start, i - start));
//                std::cout << "add:" << data_json.substr(start, i - start) << std::endl;
                start = i + 1;
            }
        }
    }
    json_vector.push_back(data_json.substr(start, data_json.size() - start));
//    std::cout << "add:" << data_json.substr(start, data_json.size() - start) << std::endl;
//    std::cout << "split by , finished" << std::endl;


    //分类
//    std::cout << "classify" << std::endl;
    for (int i = 0; i < json_vector.size(); i++) {
//        std::cout << json_vector[i] << std::endl;
        //以第一个冒号分割
        std::vector<std::string> json_vector2;
        int start = 0;
        for (int j = 0; j < json_vector[i].size(); j++) {
            if (json_vector[i][j] == ':') {
                json_vector2.push_back(json_vector[i].substr(start, j - start));
                start = j + 1;
                break;
            }
        }
        json_vector2.push_back(json_vector[i].substr(start, json_vector[i].size() - start));

        //分类
        if (json_vector2[1][0] == '{') {
            //对象
//            std::cout << "Object" << std::endl;
//            std::cout << json_vector2[0] << std::endl;
//            std::cout << json_vector2[1] << std::endl;
            //添加到Json对象中
            JsonValue jsonValue;
            jsonValue.type = JsonType::OBJECT;
            //解析对象
            jsonValue.value_object.explain_json(json_vector2[1]);
            value_array.emplace_back(json_vector2[0], jsonValue);


        } else if (json_vector2[1][0] == '[') {
            //数组
//            std::cout << "Array" << std::endl;
//            std::cout << json_vector2[0] << std::endl;
//            std::cout << json_vector2[1] << std::endl;
            //添加到Json对象中
            JsonValue jsonValue;
            jsonValue.type = JsonType::ARRAY;
            //解析数组
            //去除最外层的中括号
            json_vector2[1].erase(0, 1);
            json_vector2[1].erase(json_vector2[1].size() - 1, 1);
            std::vector<std::string> json_vector3;
            int start = 0;
            for (int j = 0; j < json_vector2[1].size(); j++) {
                if (json_vector2[1][j] == ',') {
                    json_vector3.push_back(json_vector2[1].substr(start, j - start));
//                    std::cout << "Array:" << json_vector2[1].substr(start, j - start) << std::endl;
                    start = j + 1;
                }
            }
            json_vector3.push_back(json_vector2[1].substr(start, json_vector2[1].size() - start));
//            std::cout << "Array:" << json_vector2[1].substr(start, json_vector2[1].size() - start) << std::endl;
            //转为数组
            for (int j = 0; j < json_vector3.size(); j++) {
                //字符串转数字
                double num = std::stod(json_vector3[j]);
                jsonValue.value_array.push_back(num);
            }
            value_array.emplace_back(json_vector2[0], jsonValue);

        } else if (json_vector2[1][0] == '\"') {
            //字符串
//            std::cout << "String" << std::endl;
//            std::cout << json_vector2[0] << std::endl;
//            std::cout << json_vector2[1] << std::endl;

            //添加到Json对象中
            JsonValue jsonValue;
            jsonValue.type = JsonType::STRING;
            jsonValue.value_string = json_vector2[1];
            value_array.emplace_back(json_vector2[0], jsonValue);


        } else if (json_vector2[1][0] == 't' || json_vector2[1][0] == 'f') {
            //布尔值
//            std::cout << "Boolean" << std::endl;
//            std::cout << json_vector2[0] << std::endl;
//            std::cout << json_vector2[1] << std::endl;

            //添加到Json对象中
            JsonValue jsonValue;
            jsonValue.type = JsonType::BOOLEAN;
            if (json_vector2[1][0] == 't') {
                jsonValue.value_boolean = true;
            } else {
                jsonValue.value_boolean = false;
            }
            value_array.emplace_back(json_vector2[0], jsonValue);

        } else if (json_vector2[1][0] == 'n') {
            //空值
//            std::cout << "Null" << std::endl;
//            std::cout << json_vector2[0] << std::endl;
//            std::cout << json_vector2[1] << std::endl;

            //添加到Json对象中
            JsonValue jsonValue;
            jsonValue.type = JsonType::NULL_VALUE;
            value_array.emplace_back(json_vector2[0], jsonValue);

        } else {
            //数字
//            std::cout << "Number" << std::endl;
//            std::cout << json_vector2[0] << std::endl;
//            std::cout << json_vector2[1] << std::endl;

            //添加到Json对象中
            JsonValue jsonValue;
            jsonValue.type = JsonType::NUMBER;
            jsonValue.value_number = std::stod(json_vector2[1]);
            value_array.emplace_back(json_vector2[0], jsonValue);

        }
    }

}

void Json::show_json() {
    for(auto & i : value_array){
        std::cout << i.first << std::endl;
        if(i.second.type == JsonType::OBJECT){
            i.second.value_object.show_json();
        }else if(i.second.type == JsonType::ARRAY){
            for(auto & j : i.second.value_array){
                std::cout << j << " ";
            }
        }else if(i.second.type == JsonType::STRING){
            std::cout << i.second.value_string << std::endl;
        }else if(i.second.type == JsonType::NUMBER){
            std::cout << i.second.value_number << std::endl;
        }else if(i.second.type == JsonType::BOOLEAN){
            std::cout << i.second.value_boolean << std::endl;
        }else if(i.second.type == JsonType::NULL_VALUE){
            std::cout << "null" << std::endl;
        }
    }
}


bool Json::write_json(std::string filename) {
    std::string data;
    data += "{\n";
    for(auto & i : value_array){
        data += make_json(i);
    }
    //去除最后一个逗号
    data.erase(data.size() - 2, 1);
    data += "}";
//    std::cout << "write file\n" << data << std::endl;

    std::ofstream file;
    file.open(filename, std::ios::out);
    //输出文件是否打开成功
    if (file.is_open()) {
        std::cout << "open file success" << std::endl;
        file << data;
        file.close();
        return true;
    } else {
        std::cout << "open file failed" << std::endl;
        return false;
    }
}

std::string Json::make_json(std::pair<std::string, JsonValue> value) {
    std::string data;
    data += "\t";
    if(value.second.type == JsonType::OBJECT) {
        data += value.first;
        data += ":";
        data += "{\n";
        for(auto & i : value.second.value_object.value_array){
            data += "\t";
            data += make_json(i);
        }
        //去除最后一个逗号
        data.erase(data.size() - 2, 1);
        data += "\t}";
        data += ",";
        data += "\n";
        return data;
    }else if(value.second.type == JsonType::ARRAY) {
        data += value.first;
        data += ":";
        data += "[";
        for (auto &i: value.second.value_array) {
            data += std::to_string(i);
            data += ",";
        }
        //去除最后一个逗号
        data.erase(data.size() - 1, 1);
        data += "]";
        data += ",";
        data += "\n";
        return data;
    }else if(value.second.type == JsonType::STRING) {
        data += value.first;
        data += ":";
        data += value.second.value_string;
        data += ",";
        data += "\n";
        return data;
    }else if(value.second.type == JsonType::NUMBER) {
        data += value.first;
        data += ":";
        data += std::to_string(value.second.value_number);
        data += ",";
        data += "\n";
        return data;
    }else if(value.second.type == JsonType::BOOLEAN) {
        data += value.first;
        data += ":";
        if (value.second.value_boolean) {
            data += "true";
        } else {
            data += "false";
        }
        data += ",";
        data += "\n";
        return data;
    }else if(value.second.type == JsonType::NULL_VALUE) {
        data += value.first;
        data += ":";
        data += "null";
        data += ",";
        data += "\n";
        return data;
    }
}


Json::Json(const Json &json) {
    filename = json.filename;
    original_json = json.original_json;
    value_array = json.value_array;
}

JsonValue Json::operator[](int i) const {
    if(i < value_array.size()){
        return value_array[i].second;
    }else {
        std::cout << "Json operator[] error" << std::endl;
        return JsonValue();
    }
}


bool Json::add_json(std::pair<std::string, JsonValue> value) {
    //为Name的string加上双引号
    value.first = "\"" + value.first + "\"";
    value_array.emplace_back(value);
    return false;
}

JsonValue Json::operator[](const std::string &key) const {\
    //为Name的string加上双引号
    std::string key2 = "\"" + key + "\"";
    for(auto & i : value_array){
        if(i.first == key2){
            return i.second;
        }
    }
    std::cout << "Json operator[] error" << std::endl;
    return JsonValue();
}


JsonValue::JsonValue(const std::string &value) {
    type = JsonType::STRING;
    value_string = value;
}

JsonValue::JsonValue(double value) {
    type = JsonType::NUMBER;
    value_number = value;
}

JsonValue::JsonValue(bool value) {
    type = JsonType::BOOLEAN;
    value_boolean = value;
}

JsonValue::JsonValue(const Json &value) {
    type = JsonType::OBJECT;
    value_object = value;
}

JsonValue::JsonValue(const std::vector<double> &value) {
    type = JsonType::ARRAY;
    value_array = value;
}



