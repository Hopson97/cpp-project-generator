# <PNAME>


## Building and Running

### Libraries

SDL2 is required.

These can be installed from your project manager. For example, on Debian/ Ubuntu:

```sh
sudo apt install libsdl2-dev
```

If this is not possible (eg windows), you can install these manually from their respective website.

### Linux

To build, at the root of the project:

```sh
sh scripts/build.sh
```

To run, at the root of the project:

```sh
sh scripts/run.sh
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release
```