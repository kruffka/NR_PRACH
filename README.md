#NR_PRACH

Building CMake
==============

  $ mkdir build && cd build  
  $ cmake ..  
  $ make
  
Running simulation
==============
  $ ./nr_prach is_float  

is_float = 0 -> int16 IQ format  
is_float = 1 -> float IQ format  

![alt text](https://github.com/kruffka/NR_PRACH/blob/main/some_results/SNR.png?raw=true)  

Based on OAI source code with changes for my small research..   
  
PRACH Generation and detection    
based on OAI source code   
  
float dft/idft from fftw3 lib   
int dft/idft from OAI    


![alt text](https://github.com/kruffka/NR_PRACH/blob/main/constellation.jpg?raw=true)  
![alt text](https://github.com/kruffka/NR_PRACH/blob/main/Spectrogram.jpg?raw=true)  

  
