#include <iostream>
#include "json.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Json file;
    std::string filename;
    filename = R"(..\global_planner.json)";
    file.read_json(filename);
    file.show_json();
    file.add_json(std::make_pair("test", JsonValue(1.0)));
    file.write_json(R"(..\planner.json)");
    auto value = file[2];
    if(value.type == JsonType::OBJECT)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
    return 0;
}

