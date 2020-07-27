#!/usr/bin/env python3

import sys
import os
import json
import shutil

if __name__ == "__main__":
    argv = sys.argv
    argc = len(argv)

    templateNames = []
    with os.scandir("templates/") as it:
        for entry in it:
            if entry.is_dir() and not entry.name.startswith("_"):
                templateNames.append(entry.name)
    print()
    if argc < 3:
        print("Error: No template or project name provided")
        print("Usage:")
        print("./new <project_type> <project name>")
        exit(1)

    projectType = argv[1]
    projectName = argv[2]
    
    if not projectType in templateNames:
        print("Error: No template called '" + projectType + "' exists.")
        exit(1)
    projectType += "/"

    dependancyPath = "templates/_deps/"
    templatePath = "templates/" + projectType
    projectPath = "./" + projectName + "/"
    if (os.path.isdir(projectPath)):
        print("Error: Project with the name '" + projectPath + "' already exists.")
        exit(1)

    shutil.copytree(templatePath, projectPath)

    with open(projectPath + "/config.json") as f:
        data = f.read()
    os.remove(projectPath + "/config.json")
    os.remove(projectPath + "/CMakeLists.txt")
    os.remove(projectPath + "/README.md")
    os.mkdir(projectPath + "/scripts/")

    config = json.loads(data)
    for dep in config["deps"]:
        if dep == "sfml":
            shutil.copytree(dependancyPath + "cmake_modules", projectPath + "cmake_modules") 
        else:
            shutil.copytree(dependancyPath + dep, projectPath + dep)

    def copyWithProjectName(name, loc, dest = ""):
        shutil.copyfile("templates/" + loc + name, projectPath + dest + name + "temp")
        with open(projectPath + dest + name + "temp", "r") as inf:
            with open(projectPath + dest + name, "w") as outf:
                for line in inf:
                    outf.write(line.replace("<PNAME>", projectName))
        os.remove(projectPath + dest + name + "temp")

    
    copyWithProjectName(".clang-format", "_common/")
    copyWithProjectName("CMakeLists.txt", projectType)
    copyWithProjectName("README.md", projectType)
    copyWithProjectName(".gitignore", "_common/")
    
    copyWithProjectName("run.sh", "_common/", "scripts/")
    copyWithProjectName("build.sh", "_common/", "scripts/")
    copyWithProjectName("debug.sh", "_common/", "scripts/")
    

