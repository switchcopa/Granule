# Granule

Granule is a 2D sandbox physics engine/game built in C using SDL2.
It was inspired by some other sandbox games, and it serves just as a personal project to learn graphics, physics, and overall project management.

--- 

## Features 

- This game has blocks like sand, wet sand, water, steam, and stone.
- There are different engine modes for your sandbox like erase, which erases blocks, or heat
- Full temperature, and gravity physics

## How to play

- To switch between palette, press the E button on your keyboard
- To change the mode you're currently in, press the S key
- You can see the Cell Type, temperature, and ticks of a cell on the bottom right
- To reset your builds, press R

## Requirements 

- gcc or clang with C11 support
- SDL2 dev libraries. 
- Make

## Build

```bash
make
```

The compiled binary will be at build/granule, you can run it with

```bash
./build/granule
```

Clean build files with:

```bash
make clean
```

## License

This project is licensed under the MIT License. See /LICENSE for more details.
