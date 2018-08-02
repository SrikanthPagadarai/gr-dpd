# About gr-dpd
gr-dpd is a GNU Radio implementation of our work presented in the publication: S. Pagadarai, R. Grover, S. J. Macmullan and A. M. Wyglinski, "Digital Predistortion of Power Amplifiers for Spectrally Agile Wireless Transmitters," 2016 IEEE 83rd Vehicular Technology Conference (VTC Spring), Nanjing, 2016, pp. 1-5.

### Basic Dependencies
 - UHD >= 3.9.2
 - gnuradio >= 3.7.10.1
 - armadillo >= 6.700
 - [VXI-11 instruments access library](https://github.com/f4exb/libvxi11)

### Dependencies Needed for QA Testing
 - octave >= 4.0.2
 - octave-signal >= 1.3.2
 - scipy >= 0.15.1
 - oct2py >= 3.5.9
		 
### What is implemented?
 - A GNU Radio block to programmatically control an Agilent N1996A Spectrum Analyzer.
 - An implementation of Hyperbolic-Givens rotation as described in 2.6.4 and 2.A of Fast Reliable Algorithms for Matrices with Structure - Edited by Sayed and Kailath.
 - Fast-RLS algorithm for PA digital predistortion.

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
`$ cd ..` <br />
`$ mkdir build` <br />
`$ cd build` <br />
`$ cmake ..` <br />
`$ make` <br />
`$ sudo make install` <br />
`$ sudo ldconfig` <br />