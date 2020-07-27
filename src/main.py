#!/usr/bin/env python3

import sys
import os



if __name__ == "__main__":
    argv = sys.argv
    argc = len(argv)
    
    templates = []
    with os.scandir("templates/") as it:
        for entry in it:
            if entry.is_dir() and not entry.name.startswith("_"):
                templates.append(entry.name)
    print()
    if argc < 3:
        print("Error: No template or project name provided")
        print("Usage:")
        print("./new <project_type> <project name>")
        exit(1)

    projectType = argv[1]
    projectName = argv[2]

