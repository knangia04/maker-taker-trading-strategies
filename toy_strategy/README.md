Grid Strategy - Build Instructions

This repository contains the implementation of the Grid Strategy, a trading strategy designed to execute orders across a predefined price grid. Below are the steps and details of the updated build process for this project, including the Makefile configuration.

Updates to the Build Process

The Makefile has been updated to compile the Grid Strategy library (GridStrategy.so) with the following enhancements:

Key Features of the Makefile
Compiler Selection:
Uses the Intel C++ compiler (icc) if available, or defaults to g++.
ifdef INTEL
    CC=icc
else
    CC=g++
endif
Debug and Release Modes:
The Makefile supports two modes:
Debug: Adds debugging symbols with -g.
Release: Optimized for performance with -O3.
These modes are controlled by the DEBUG flag.
ifdef DEBUG
    CFLAGS=-c -g -fPIC -fpermissive -std=c++11
else
    CFLAGS=-c -fPIC -fpermissive -O3 -std=c++11
endif
Library Paths and Includes:
The Makefile points to the Strategy Studio library paths and includes necessary headers.
LIBPATH=/home/vagrant/ss/sdk/RCM/StrategyStudio/libs/x64/
INCLUDES=-I/usr/include -I../../../includes -I/home/vagrant/ss/sdk/RCM/StrategyStudio/includes
Build Artifacts:
The final shared library is named GridStrategy.so.
The source file (GridStrategy.cpp) and header file (GridStrategy.h) define the logic for the strategy.
LIBRARY=GridStrategy.so
SOURCES=GridStrategy.cpp
HEADERS=GridStrategy.h
Clean Target:
A clean target is provided to remove compiled objects and the shared library.
clean:
    rm -rf *.o $(LIBRARY)
Build Instructions

Clone the repository:
git clone https://github.com/yourusername/GridStrategy.git
cd GridStrategy
Edit the Makefile as needed:
Ensure the LIBPATH and INCLUDES are correct for your system.
Build the library:
make
To clean up build artifacts:
make clean
Files Overview

GridStrategy.cpp: Contains the implementation of the Grid Strategy logic.
GridStrategy.h: Header file defining the interface for the strategy.
Makefile: Automates the build process for the shared library.
Example Usage

The compiled GridStrategy.so can be integrated with Strategy Studio or other compatible systems for testing and execution.