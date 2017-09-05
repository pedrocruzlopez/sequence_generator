# Numeric Sequence & UUID Generator 

Sequence Generator is an alternative to implement numeric auto increment & UUID fields for MySQL (postgreSQL & Oracle in progress), based in the optimal performance of kernel modules of Linux.

## Getting Started

### Requistes

- Linux SO 
- GCC Compiler
- CMake
- MySQL or PostgreSQL (Any version)

### Installation

0. Install MySQL Server or PostreSQL (even both if you will) 

1. Open a terminal and install build essentials if you don't have it: 

```
sudo apt-get update
sudo apt-get install build-essential
```
2. If you don't have cmake, please install it:

```
sudo apt-get install cmake
```

3. Create a folder or simply clone the project in whatever directory: 

```
git clone https://github.com/pedrocruzlopez/sequence_generator.git
```
3. Give permission to folder 

```
chmod -R 777 /path/sequence_generator
```

5. Execute install script

```
./install.sh
```

## Authors

* **Pedro Cruz** 
  - https://www.upwork.com/o/profiles/users/_~0172a739a43382b64b/  
  - https://github.com/pedrocruzlopez

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Edgar René Ornelis Hoil

