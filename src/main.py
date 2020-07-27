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

    templatePath = "templates/" + projectType
    projectPath = "./" + projectName
    if (os.path.isdir(projectPath)):
        print("Error: Project with the name '" + projectPath + "' already exists.")
        exit(1)

    shutil.copytree(templatePath, projectPath)

    with open(projectPath + "/config.json") as f:
        data = f.read()
    os.remove(projectPath + "/config.json")
    config = json.loads(data)
    

    print(config)
    

