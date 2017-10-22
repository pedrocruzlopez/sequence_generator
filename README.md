# Sequence Generator 

Sequence Generator, _seqgen_ , is an alternative to implement numeric auto increment & UUID surrogates keys for MySQL & postgreSQL.

## Why seqgen?

- _seqgen_ allows you to handle and monitor your numeric sequences without fetch data from the database server.
- There is no need to create an extra index if you need a field with automatic data and therefore you need less space in disk.
- The first insert operations in a table with _seqgen_ after a database server starts are more faster.

Note that _seqgen_ aims to improve your management of your surrogate keys, and it's designed to small tables for easy models. _seqgen_ should not be used in big tables.


## Getting Started

### Requirements

- Linux SO with kernel 2.6 or greater.
- GCC Compiler
- CMake (>= 2.6)
- MySQL (>= 5.6) or PostgreSQL (>= 9.1)
- git (optional)

### Installation

1. Clone the projet if you have git or download the zip project in any directory: 

```
$ git clone https://github.com/pedrocruzlopez/sequence_generator.git
```
2. Give permission to folder 

```
$ chmod -R 777 /path/sequence_generator
```

3. Execute install script

```
$ ./install.sh
```

4. Install kernel headers.

```
sudo apt-get install linux-headers-$(uname -r)
```
## Quick details of it's implementation


Every numeric sequence belongs to a collections of sequences

## How to use

### Managing your surrogates keys

1. After installation you can type the command ```seqgen``` in a terminal. You will get a menu with the database server installed in your local.

```
$ seqgen

Please choose your database : 
1) MySQL Server
2) PostgreSQL
3) Exit
```
2. Once you have choosen the database, you will be asked for your credentials (you can choose to save credentials to not require them anymore) and then it will print a new menu with the following options: 

```
Select option : 
1) Create sequence
2) Update sequence
3) Restart sequence
4) Get current number
5) Generate UUID
6) Exit

```
### Using in DBMS


## Author

* **Pedro Cruz** 
  - https://www.upwork.com/o/profiles/users/_~0172a739a43382b64b/  
  - https://github.com/pedrocruzlopez

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Edgar René Ornelis Hoil (Software Engineer) for his advice

