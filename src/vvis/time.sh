#!/bin/bash
cd research/quantitative
make clean
make
sleep 10
make run_timing
cvs commit -m "Commited from script" tout

cd ../transformative
make clean
make
sleep 10
make run_timing
cvs commit -m "Commited from script" tout

cd ../convolute
make clean
make
sleep 10
make run_timing
cvs commit -m "Commited from script" tout

cd ../timing/
make clean
make
sleep 10
make run_timing
cvs commit -m "Commited from script" tout
