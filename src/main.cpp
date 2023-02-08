#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using json_t = nlohmann::json;

int main()
{
    std::cout << sizeof(uint64_t) << std::endl;
}