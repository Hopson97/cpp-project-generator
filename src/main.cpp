#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<fs::path> templatePaths;
std::vector<std::string> templateNames;
std::vector<std::string> currentNames;
std::string projectType;
std::string projectName;

/**
 * @brief Checks if a value exists in a vector
 * 
 * @tparam T Vector template type
 * @param vect The vector to search
 * @param find The value to check for existance
 * @return true Exists
 * @return false Does not exist
 */
template<typename T>
bool exists(const std::vector<T>& vect, const T& find) {
    return std::find(vect.cbegin(), vect.cend(), find) != vect.cend();
}

/**
 * @brief Prints out the vector of project template names
 * 
 */
void printTemplates() {
    std::cout << "Current templates:\n";
    for (auto& name : templateNames) {
        std::cout << name << '\n';
    }
}

/**
 * @brief Verifies the name and the template type
 * Checks if the template type actually exists, and whether the project name
 * already exists
 * @return true Template exists and the project name is new
 * @return false Either the project template is non-existant or the project name already exists
 */
bool verifyTemplateAndName() {
    //Ensure template exists
    if (!exists(templateNames, projectType)) {
        std::cout << "Unable to find project template \"" << projectType << "\"\n";
        printTemplates();
        return false;
    }

    //Check if the template project workspace exists
    if (!fs::exists("./" + projectType)) {
        fs::create_directory(projectType);
    }

    //Make sure the project name does not exist
    for (auto& entry : fs::directory_iterator("./" + projectType + "/")) {
        if (entry.path().filename() == projectName) {
            std::cout << "Project with name " << projectName << " already exists, unable to create project.";
            return false;
        }
    }
    return true;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    std::size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string replaceProjectName(const std::string& fileName) {
    std::ifstream inFile(fileName);
    std::ostringstream stream;
    stream << inFile.rdbuf();
    auto ft = stream.str();
    while(replace(ft, "<PNAME>", projectName));
    return ft;
}

/**
 * @brief Entry point
 * Parses args, ensures no errors will happen, and then creates the project
 * @param argc Argument count
 * @param argv The commands line args
 * @return int Success
 */
int main(int argc, char** argv) {
    //Get list of template and template names
    for (auto& entry : fs::directory_iterator("./templates/")) {
        if (entry.is_directory()) {
            templatePaths.push_back(entry.path());
            templateNames.push_back(entry.path().filename());
        }
    }

    //Check arg count
    if (argc < 3) {
        std::cout << "Usage: \n";
        std::cout << "./new <project type> <projectname>\n";
        printTemplates();
        return 0;
    }

    //Exact args
    projectType = argv[1];
    projectName = argv[2];

    if (!verifyTemplateAndName()) {
        return 0;
    }



    fs::path projectPath = "./" + projectType + "/" + projectName + "/";

    fs::copy("templates/" + projectType, projectPath, fs::copy_options::recursive);
    fs::create_directory(projectPath.string() + "scripts");
    fs::remove(projectPath.string() + "CMakeLists.txt");

    auto writeString = [](const fs::path& fName, const std::string& str) {
        std::ofstream out(fName);
        out << str;
    };
    
        //Create project
    auto runScript = replaceProjectName("templates/run.sh");
    auto debugScript = replaceProjectName("templates/debug.sh");
    auto build = replaceProjectName("templates/build.sh");
    auto cmake     = replaceProjectName("templates/" + projectType + "/CMakeLists.txt");
    auto readme = replaceProjectName("templates/" + projectType + "/README.md");

    writeString(projectPath / fs::path("scripts/run.sh"), runScript);
    writeString(projectPath / fs::path("scripts/debug.sh"), debugScript);
    writeString(projectPath / fs::path("scripts/build.sh"), build);
    writeString(projectPath / fs::path("CMakeLists.txt"), cmake);
    writeString(projectPath / fs::path("README.md"), readme);
    

    return EXIT_SUCCESS;
}