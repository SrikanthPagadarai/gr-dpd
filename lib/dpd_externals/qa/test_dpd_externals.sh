#!/bin/bash

# compile and run givens_rotate() tests
printf "Compiling and running givens_rotate() tests...\n"
g++ -std=c++11 -o test_givens_rotate.o test_givens_rotate.cc ../src/givens_rotate.cc -ldpd_externals -larmadillo
./test_givens_rotate.o
gvfs-trash *.txt

# compile and run extract_postdistorted_y() tests
printf "Compiling and running extract_postdistorted_y() tests...\n"
g++ -std=c++11 -o test_extract_postdistorted_y.o test_extract_postdistorted_y.cc ../src/extract_postdistorted_y.cc -ldpd_externals -larmadillo
./test_extract_postdistorted_y.o
gvfs-trash *.txt

# compile and run extract_g_vecs() tests
printf "Compiling and running extract_g_vecs() tests...\n"
g++ -std=c++11 -o test_extract_g_vecs.o test_extract_g_vecs.cc ../src/extract_g_vecs.cc -ldpd_externals -larmadillo
./test_extract_g_vecs.o
gvfs-trash *.txt

gvfs-trash *.o
