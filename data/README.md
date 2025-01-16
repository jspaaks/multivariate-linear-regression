# Data

## Artificial

Data generated using known parameterizations and known linear model, which was then corrupted using random normal residuals of known magnitude.

## Housing

Data set used in Stanford's CS221 course. Housing prices around Portland, Oregon, as a more or less linear but noisy function of square footage and number of bedrooms.

## Yann LeCun handwritten numbers

Wayback Machine copies of Yann LeCun et al. handwritten numbers data.

- https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/
- https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz
- https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz
- https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz
- https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz

```console
$ mkdir lecunn
$ cd data

# download
$ wget https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz
$ wget https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz
$ wget https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz
$ wget https://web.archive.org/web/20140214080728/http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz

#decompress
$ gzip --decompress --keep --name --verbose --best ./t10k-images-idx3-ubyte.gz
$ gzip --decompress --keep --name --verbose --best ./t10k-labels-idx1-ubyte.gz
$ gzip --decompress --keep --name --verbose --best ./train-images-idx3-ubyte.gz
$ gzip --decompress --keep --name --verbose --best ./train-labels-idx1-ubyte.gz

# avoid corrupting the data
$ chmod ugo-w t10k* train*

# check the data (ASCII equivalent omitted)

$ xxd -d -s 16 -o -16 -l 784 -c 28 t10k-images.idx3-ubyte
00000000: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000028: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000056: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000084: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000112: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000140: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000168: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
00000196: 0000 0000 0000 54b9 9f97 3c24 0000 0000 0000 0000 0000 0000 0000 0000
00000224: 0000 0000 0000 defe fefe fef1 c6c6 c6c6 c6c6 c6c6 aa34 0000 0000 0000
00000252: 0000 0000 0000 4372 4872 a3e3 fee1 fefe fefa e5fe fe8c 0000 0000 0000
00000280: 0000 0000 0000 0000 0000 0011 420e 4343 433b 15ec fe6a 0000 0000 0000
00000308: 0000 0000 0000 0000 0000 0000 0000 0000 0000 53fd d112 0000 0000 0000
00000336: 0000 0000 0000 0000 0000 0000 0000 0000 0016 e9ff 5300 0000 0000 0000
00000364: 0000 0000 0000 0000 0000 0000 0000 0000 0081 feee 2c00 0000 0000 0000
00000392: 0000 0000 0000 0000 0000 0000 0000 0000 3bf9 fe3e 0000 0000 0000 0000
00000420: 0000 0000 0000 0000 0000 0000 0000 0000 85fe bb05 0000 0000 0000 0000
00000448: 0000 0000 0000 0000 0000 0000 0000 0009 cdf8 3a00 0000 0000 0000 0000
00000476: 0000 0000 0000 0000 0000 0000 0000 007e feb6 0000 0000 0000 0000 0000
00000504: 0000 0000 0000 0000 0000 0000 0000 4bfb f039 0000 0000 0000 0000 0000
00000532: 0000 0000 0000 0000 0000 0000 0013 ddfe a600 0000 0000 0000 0000 0000
00000560: 0000 0000 0000 0000 0000 0000 03cb fedb 2300 0000 0000 0000 0000 0000
00000588: 0000 0000 0000 0000 0000 0000 26fe fe4d 0000 0000 0000 0000 0000 0000
00000616: 0000 0000 0000 0000 0000 001f e0fe 7301 0000 0000 0000 0000 0000 0000
00000644: 0000 0000 0000 0000 0000 0085 fefe 3400 0000 0000 0000 0000 0000 0000
00000672: 0000 0000 0000 0000 0000 3df2 fefe 3400 0000 0000 0000 0000 0000 0000
00000700: 0000 0000 0000 0000 0000 79fe fedb 2800 0000 0000 0000 0000 0000 0000
00000728: 0000 0000 0000 0000 0000 79fe cf12 0000 0000 0000 0000 0000 0000 0000
00000756: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
```

### Experiment

https://yann.lecun.com/exdb/publis/pdf/lecun-98.pdf

- input layer: 784 (28x28)
- hidden layer 300
- hidden layer 100
- output layer 10
