# MazeSolver - Genetic algorithm

[![Release](https://img.shields.io/github/release/markub3327/MazeSolver_GA)](https://github.com/markub3327/MazeSolver_GA/releases)
![Tag](https://img.shields.io/github/v/tag/markub3327/MazeSolver_GA)

[![Issues](https://img.shields.io/github/issues/markub3327/MazeSolver_GA)](https://github.com/markub3327/MazeSolver_GA/issues)
![Commits](https://img.shields.io/github/commit-activity/w/markub3327/MazeSolver_GA)

![Languages](https://img.shields.io/github/languages/count/markub3327/MazeSolver_GA)
![Size](https://img.shields.io/github/repo-size/markub3327/MazeSolver_GA)

A genetic algorithm is a search heuristic. This algorithm reflects the process of natural selection where the fittest individuals are selected for reproduction in order to produce offspring of the next generation. The best individual is solution of maze. Design of maze is defined in Environment.h

## States

* position X
* position Y

## Actions

* Up
* Down
* Left
* Right

## Compile
You must install the OpenMP library before compiling!

Install OpenMP on MacOS:
```
brew install libomp
```
Install OpenMP on Linux (Ubuntu):
```
sudo apt-get install libomp-dev
```

Compiling for MacOS users:
```
g++ -Xpreprocessor -fopenmp *.cpp -o MazeSolver_GA -lomp --std=c++17
```

Compiling for Linux users:
```
g++ -fopenmp *.cpp -o MazeSolver_GA -lgomp --std=c++17
```

## Run
```
./MazeSolver_GA
```

## License

[![MIT](https://img.shields.io/github/license/markub3327/MazeSolver_GA.svg)](LICENSE)
