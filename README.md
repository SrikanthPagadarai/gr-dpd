# About gr-dpd

### Basic Dependencies
 - UHD >= 
 - gnuradio >= 
 - armadillo >=
 - [VXI-11 instruments access library](https://github.com/f4exb/libvxi11)

### Dependencies Needed for QA Testing


### What is implemented?
 - 

### OSs Tested 
 - Ubuntu 16.04 
 
### Installation
#### dpd_externals
`$ git clone https://github.com/SrikanthPagadarai/gr-dpd` <br />
`$ cd gr-dpd/dpd_externals` <br />
`$ mkdir build` <br />
`$ cd build` <br />
`$ cmake ..` <br />
`$ make` <br />
`$ cd ../qa/` <br />
`$ ./test_dpd_externals.sh 0` <br />
`$ cd ../build/` <br />
`$ sudo make install` <br />
`$ sudo ldconfig` <br />

#### gr-dpd
