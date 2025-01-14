# gdfit

For instructions on building, formatting, testing, etc, see [`README.dev.md`](README.dev.md).

## Executables

- `mkdata` Makes an n-dimensional linear data set with normally distributed noise of a given standard deviation.
- `train` Command line program for iterative fitting of multivariate linear data sets using steepest descent.

## Dependencies

- `libboxmuller`: Normally distributed random number generation
- `libkwargs`: Handling of command line arguments
- `libmatrix`:  Matrix data type and operations on instances of that type

## `man` pages

- `mkdata.1`

## Example

```console
$ cd build
$ ./dist/bin/mkdata --nfeatures 2 --nsamples 100 -s 10 --true_weights 98.7,65.4,32.1 -b data/
$ ./dist/bin/train --nepochs 2500 data/features.txt data/labels.txt
```

## Data

See [data/README.md](data/README.md).

## Acknowledgements

_This project was initialized using [Copier](https://pypi.org/project/copier)
and the [Copier template for C projects](https://github.com/jspaaks/copier-template-for-c-projects)._
