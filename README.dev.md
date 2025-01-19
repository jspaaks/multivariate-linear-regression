# Developer notes

## CMake

The project has been initialized with a [CMakeLists.txt](CMakeLists.txt)-based
configuration for building with CMake:

```console
# change into the build directory
$ cd build

# generate the build files
$ cmake ..

# build the project
$ cmake --build .

# install the project to <repo>/build/cmake/dist
$ cmake --install .

# checkout man pages, e.g.
$ man -l ./dist/share/man/man1/mkdata.1
```

## `clang-format`

The file `.clang-format` contains an initial configuration for (automatic) formatting with [clang-format](https://clang.llvm.org/docs/ClangFormat.html). Run the formatter with e.g.:

```console
# dry run on main.c
$ clang-format -Werror --dry-run main.c

# format in place all *.c and *.h files under ./src
$ clang-format -i `find ./src -type f -name '*.[c|h]'`
```

## TODO

1. add gradient check https://www.youtube.com/watch?v=h7iBpEHGVNc&t=3503s as a unit test
1. add minibatching https://www.youtube.com/watch?v=h7iBpEHGVNc&t=3776s
1. add multistart
1. unstandardize the weights
1. consider migrating to MSE as a loss function
