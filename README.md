# Sequence Generator 

Sequence Generator, _seqgen_ , is an alternative to implement numeric auto increment & UUID surrogates keys for MySQL & postgreSQL.

## Why seqgen?

- _seqgen_ allows you to handle and monitor your numeric sequences without fetch data from the database server.
- There is no need to create an extra index if you need a field with automatic data and therefore you need less space in disk.
- The first insert operations in a table with _seqgen_ after a database server starts are more faster.

Note that _seqgen_ aims to improve your management of your surrogate keys, and it's designed to small tables for easy models, _seqgen_ should not be used in big tables.


## Getting Started

### Requirements

- Linux SO with kernel 2.6 or greater.
- GCC Compiler
- CMake (>= 2.6)
- MySQL (>= 5.6) or PostgreSQL (>= 9.1)
- git (optional)

### Installation

1. Clone the project if you have git or download the zip project in any directory: 

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
$ sudo apt-get install linux-headers-$(uname -r)
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
#### Create sequence
Every sequence starts with 0 by default, when you create a new sequence you can set a initial value and the sequence will increment by one the next number starting with the initial value

#### Update sequence
You can update the current value of a sequence specifying the index of the sequence, and setting a new value.

#### Restart sequence
Selecting this option, a specific sequence will back to 0 value.

**Note:** It's important to remember that these three operations of above can incurring into violation of referential integrity, for this reason you will asked to stop the server and then run the operation. Please consider the situation before executing one of this options

#### Get current number
When you select this option, you can verify the current value of a determined sequence, specifying it's index.

In any case of above you will always be asked for two parameters: number of sequence (index) and new value to set, except for the option _Get current number_ which only needs the index value for return the current value. This is the example for the option _Create sequence_
```
Please select the number of sequence
1
Please insert a initial value
3
Press any key
```
### Shorthand commands

Also you can perform the action previously explained above in a single line of command, as detailed below:

```
-h --help                                            Print help            			     

-c --cre_seq database number_sequence initial_value  Create a new sequence in the determined dbms with an initial value

-g --get_seq database number_sequence                Get the current number of a sequence

-s --set_seq database number_sequence new_value      Set value for a sequence WARNING: It's recommended to stop the database service

-r --res_seq database number_sequence                Restart sequence WARNING: It's recommended to stop the database service

-u --uuid                                            Generate UUID

```

### Using in DBMS

When you have done installing _seqgen_, either MySQL or PostgreSQL will have the functions: ``` get_sequence(int) ``` and ``` get_uuid() ```.

#### ``` get_sequence(int) ```
This function receives a ```unsigned int``` parameter that represents the index of a sequence and returns a ```unsigned int``` value that will increment every time that the function is called for the specific sequcene.
```get_sequence(int)``` can call from any SQL statement.

Example:

```
INSERT INTO TABLE (id, name, country) VAUES (get_sequence(1), "Linus Torvalds" , "Finland");
```
Consider that calling the function from a ```SELECT``` statement will also increment the current value of the sequence.

```
SELECT get_sequence(2);
```
#### ``` get_uuid() ```
This function will generate a universal unique identifier (UUID) in the case that you need a non-numeric field auto-generated, there is no need of parameters because a UUID is not a part of a collection of fields like numeric sequence.

Example:

```
INSERT INTO TABLE (id, name, country) VAUES (get_uuid(), "Linus Torvalds" , "Finland");
```

## Troubleshooting, other uses and contact author

Finally, if you get bugs or if you'd like to get further details about it's implementation in the unlikely case that somebody cares, email me (pedrocruz657@gmail.com).

Also this was not only designed for purposes of databases concerns, if you find out other use, feel free to update, modify or overwrite the code for your owns purposes.

Pedro


## Author

* **Pedro Cruz** 
  - https://www.upwork.com/o/profiles/users/_~0172a739a43382b64b/  
  - https://github.com/pedrocruzlopez

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Edgar René Ornelis Hoil (Software Engineer) for his advice

