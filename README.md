# C++ Project Generator

This is primarily for personal use, though feel free to use it youself as well.

Often when creating new C++ projects, I would end up copy and pasting CMakeLists.txt files and build scripts, manually having to change project names and other mundane activies. To mitigate this issue, I created this project, which would create these project files for me.

## Building

On linux

```bash
sh build.sh release
```

## Configuartion

### Setting up

After building, the executable can be found in `$PROJECT_ROOT/bin/release/` and is called `new`

For now, the executable file and the template folder must be copy-pasted to a seperate directory eg

```
└── cpp-projects
    ├── new
    └── templates
        ├── build.sh
        ├── debug.sh
        ├── run.sh
        └── terminal
```

This project comes with the template for C++ terminal/console applications, meaning it only uses the C++ standard library.

### Using

To use, `cd` to the `cpp-projects` folder and then:

`./new <Project type> <Project name>

* Project type: The project template to use, eg terminal
* Project name: The name of the project to create, eg my-app

### Adding new templates

To add a template, simply create a folder in the templates/ directory, inside including all the files you need for that template.

#### Example usage

```bash
./new terminal my-application
```

This will create a project folder `cpp-projects/terminal/my-app/` with the structure

```
└── my-app
    ├── CMakeLists.txt
    ├── scripts
    │   ├── build.sh
    │   ├── debug.sh
    │   └── run.sh
    └── src
        └── main.cpp
```

After doing `cd` to the directory, it can then be built and ran:

```bash
sh scripts/build.sh
sh scripts/run.sh
```

After the build stage, it creates the following structure:

```
└── my-app
    ├── bin
    │   ├── debug
    │   │   ├── CMakeCache.txt
    │   │   ├── CMakeFiles
    │   │   ├── cmake_install.cmake
    │   │   ├── Makefile
    │   │   └── new
    │   └── release
    ├── CMakeLists.txt
    ├── scripts
    │   ├── build.sh
    │   ├── debug.sh
    │   └── run.sh
    └── src
        └── main.cpp
```

Where the target was built in the `bin/debug/` directory.

To build and run in release mode, use the `release` argument:

```bash
sh scripts/build.sh release
sh scripts/run.sh release
```

## Future

