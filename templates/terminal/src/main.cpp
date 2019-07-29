#include <iostream>
#include <filesystem>

int main() {
    std::cout << std::filesystem::create_directory(std::filesystem::path("hello"));
    std::cout << "Hello world\n";
}
