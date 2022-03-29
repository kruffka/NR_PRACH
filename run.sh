#!/bin/bash

gcc nr_prach.c fftw.c -lm -lfftw3 &&./a.out
