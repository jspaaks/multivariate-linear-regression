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
1. consider making the test binaries startable from ctest: https://cmake.org/cmake/help/v3.28/command/add_test.html
1. unstandardize the weights
1. turn runann into readidx, remove its libmatrix dependency
1. add errplot2, an executable for plotting a 1-d feature array against its true labels and its predictions
1. add errplot3, an executable for plotting a 2-d feature array against its true labels and its predictions
1. Investigate possible problem:

   ```console
   # compare
   ./dist/bin/train -e 10 -r 0.1 -v \
         ../data/artificial/d2/n9/s0/features.txt \
         ../data/artificial/d2/n9/s0/labels.txt`
   # with
   mkdir test && \
      ./dist/bin/mkdata -d 2 -n 9 -w 0,1,2 -s 0 -b test/ && \
      ./dist/bin/train -e 10 -r 0.1 -v ./test/features.txt ./test/labels.txt
   ```

## Notes

1. softmax implementations use `log10`, not `ln`, following examples from
   https://www.youtube.com/watch?v=h7iBpEHGVNc&t=2698s. Not sure it matters
   since both are monotonic. PyTorch seems to be using `ln` however.
