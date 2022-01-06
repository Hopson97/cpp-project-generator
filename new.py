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
    if os.path.isdir(projectPath):
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
    deps = []
    for dep in config["deps"]:
        if not os.path.isdir(projectPath + "deps/"): 
            os.mkdir(projectPath + "deps/")
        shutil.copytree(dependancyPath + dep, projectPath + "deps/" + dep)
        deps.append(dep)
    if len(deps) > 0:
        with open(projectPath + "deps/CMakeLists.txt", "w") as outfile:
            for dep in deps:
                outfile.write("add_subdirectory(" + dep + ")\n")

    def copy_generator_file(name, loc, dest = ""):
        shutil.copyfile("templates/" + loc + name, projectPath + dest + name + "temp")

    def replace_project_name(startpath):
        for root, dirs, files in os.walk(startpath):
            for f in files:
                file_path = os.path.join(root, f)
                print("replacing <PNAME> in : " + file_path)
                with open(file_path, "rt") as inf:
                    data = inf.read()
                    data = data.replace("<PNAME>", projectName)
                    data = data.replace("<#PNAME#>", projectName.upper())

                with open(file_path, "wt") as outf:
                    outf.write(data)
                
    replace_project_name(projectPath)            
    
    copy_generator_file(".clang-format", "_common/")
    copy_generator_file(".gitignore", "_common/")
    
    copy_generator_file("run.sh", "_common/", "scripts/")
    copy_generator_file("build.sh", "_common/", "scripts/")
    copy_generator_file("debug.sh", "_common/", "scripts/")
    

