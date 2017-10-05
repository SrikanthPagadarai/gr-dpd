#!/bin/bash

if [ $# -lt 1 ]
then
  echo "Usage: $./test_dpd_externals.sh <0-8>"
  echo
  echo "           Tests"
  echo "-----------------------------"
  echo "[0] All"
  echo "[1] gen_GMPvector()"
  echo "[2] extract_postdistorted_y()"
  echo "[3] extract_g_vecs()"
  echo "[4] givens_rotate()"
  echo "[5] hgivens_rotate()"
  echo "[6] apply_rotations()"
  echo "[7] almost_equals_zero()"
  echo "[8] gauss_smooth()"

  exit
fi

if [ "$1" -eq 1 ] || [ "$1" -eq 0 ]; then 
  # compile and run gen_GMPvector() tests
  printf "Compiling and running tests for gen_GMPvector()...\n"
  g++ -std=c++11 -o test_gen_GMPvector.o test_gen_GMPvector.cc ../src/gen_GMPvector.cc -ldpd_externals -larmadillo
  ./test_gen_GMPvector.o
  gvfs-trash *.txt
fi

if [ "$1" -eq 2 ] || [ "$1" -eq 0 ]; then 
  # compile and run extract_postdistorted_y() tests
  printf "Compiling and running tests for extract_postdistorted_y()...\n"
  g++ -std=c++11 -o test_extract_postdistorted_y.o test_extract_postdistorted_y.cc ../src/extract_postdistorted_y.cc -ldpd_externals -larmadillo
  ./test_extract_postdistorted_y.o
  gvfs-trash *.txt
fi

if [ "$1" -eq 3 ] || [ "$1" -eq 0 ]; then 
  # compile and run extract_g_vecs() tests
  printf "Compiling and running tests for extract_g_vecs()...\n"
  g++ -std=c++11 -o test_extract_g_vecs.o test_extract_g_vecs.cc ../src/extract_g_vecs.cc -ldpd_externals -larmadillo
  ./test_extract_g_vecs.o
  gvfs-trash *.txt
fi

if [ "$1" -eq 4 ] || [ "$1" -eq 0 ]; then 
  # compile and run givens_rotate() tests
  printf "Compiling and running tests for givens_rotate()...\n"
  g++ -std=c++11 -o test_givens_rotate.o test_givens_rotate.cc ../src/givens_rotate.cc -ldpd_externals -larmadillo
  ./test_givens_rotate.o
  gvfs-trash *.txt
fi

if [ "$1" -eq 5 ] || [ "$1" -eq 0 ]; then 
  # compile and run hgivens_rotate() tests
  printf "Compiling and running tests for hgivens_rotate()...\n"
  g++ -std=c++11 -o test_hgivens_rotate.o test_hgivens_rotate.cc ../src/hgivens_rotate.cc -ldpd_externals -larmadillo
  ./test_hgivens_rotate.o
  gvfs-trash *.txt
fi

if [ "$1" -eq 6 ] || [ "$1" -eq 0 ]; then 
  # compile and run apply_rotations() tests
  printf "Compiling and running tests for apply_rotations()...\n"
  g++ -std=c++11 -o test_apply_rotations.o test_apply_rotations.cc ../src/givens_rotate.cc ../src/hgivens_rotate.cc ../src/apply_rotations.cc -ldpd_externals -larmadillo
  ./test_apply_rotations.o
  gvfs-trash *.txt
fi

if [ "$1" -eq 7 ] || [ "$1" -eq 0 ]; then 
  # compile and run almost_equals_zero() tests
  printf "Compiling and running tests for almost_equals_zero()...\n"
  g++ -std=c++11 -o test_almost_equals_zero.o test_almost_equals_zero.cc ../src/almost_equals_zero.cc -ldpd_externals
  ./test_almost_equals_zero.o  
fi

if [ "$1" -eq 8 ] || [ "$1" -eq 0 ]; then 
  # compile and run gauss_smooth() tests
  printf "Compiling and running tests for gauss_smooth()...\n"
  g++ -std=c++11 -o test_gauss_smooth.o test_gauss_smooth.cc ../src/gauss_smooth.cc -ldpd_externals
  ./test_gauss_smooth.o  
  gvfs-trash *.txt
fi

gvfs-trash *.o
