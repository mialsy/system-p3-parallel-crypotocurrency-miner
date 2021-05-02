# Project 3: Parallel Cryptocurrency Miner

**Author**: [Chuxi Wang](https://www.instagram.com/_mialsy_/ "click here to see awesome kitties, helps me survive this project :)") 

**Summmary**: This is the project 3 for CS 521 System programing. In this project, I modified the single-thread version of the crypttocurrency miner to parallel version, using the producer-consumer model. This ```miner``` program allow user to input customized thread number, difficulty level and data block to create their own personalized crytocurrency with desired efficiency (thread count).

## About the project

*When I was young I thought that money was the most important thing in life; now that I am old I know that it is.* as said by my favorite writer Oscar Wilde. 

To show our respect to money, here we built our own cryptocurrency which defines the value of the coin by certain pattern of the zeros in the sha1sum result of the input string. To be specific, our coins are basically strings with a trailing nonce, which has sha1sum of the defined leading zero counts. The more zeros there are, the more value we assigned to the coin. 

For instance, ```gumball4``` with sha1sum of ```3BF9AB79960ECA1B4AAEE7BDBF9884A20FE3E539``` is considered having lower value than ```gumabll587``` which has sha1sum of ```00DF1955AF4D92D0B9DF0FF7E27A23A3D7429FF9```.

Now we have defined the value of our crypotocurrency, we need to actually **mine** the coin so that we can talk about making money!

<div display = "flex">
<img src="http://49.media.tumblr.com/tumblr_lbh1lw2HId1qa1xnko1_500.gif" alt="dwarfs mining in Snow White and The Seven Dwarfs" width="300" margin="auto">
</div>

### What is ```miner```? 

```crash``` is our own version of the command line interface shell. It basically allows user to interact with the computers in the following ways:

- Run built-in functions:
    - Including change the CWD, browsing past commands, comment, listing background jobs, and exiting the ```crash``` program.
    - For detail about running the ```crash```, please check [**Program Options**](https://github.com/usf-cs521-sp21/P2-mialsy#program-options).
- Run external executable functions:
    - This allows user to run external executable functions that are avaiable.
    - Background job are also supported in ```crash```, please review [**Program Options**](https://github.com/usf-cs521-sp21/P2-mialsy#program-options) for how to run background job.

The workflow of how ```crash``` works is as follow: 
<img src="https://github.com/usf-cs521-sp21/P2-mialsy/blob/main/res/p2_main_function.jpg" alt="crash main function workflow" width="900">

### Multi-thread parallel program and the producer consumer model

A process is basically an instance of the running program. 

In the ```crash``` program, we fork a child process to execute an external command. 

The reason we need different process working on this is that we can do then have differnt handling of signal of the child process, and also allow our ```crash``` to run background jobs. Another reason is that by doing so, we would be able to set up enviroment of the execution of program in the child process, and would be able to redirect IO.


### The task queue and why do we need a queue? 

To support the built-in history browsing and retriving, an history list based on circular list (clist) structure is included. 

The clist is essencially a list that gets override when a limit has been reached. It has a size limit (also is the capacity for the list), when the clist reaches the limit, the newly added element will override the old ones.

Here is a domenstration of how clist works:
<img src="https://github.com/usf-cs521-sp21/P2-mialsy/blob/main/res/clist_demo.gif" alt="demo of the clist adding operation" width="700">

## Program options

- Running ```miner```: pass in the thread count, difficulty level, and the input block to mine.
```
```

## Included files

Here is a list of files that is included:

- **Makefile**: Included to compile and run the programs.
- **miner.c** The driver for ```crash``` program. Includes main function and supportive functions and structs:
    - ```main()``` function that handles the program logic;
    - ```struct job_item``` that describe a background job;
    - supportive funtions to:
        - tokenize the input string;
        - handle built-in functions;
        - SIGINT and SIGCHLD handlers;
        - print usage and say goodbye.
- **queue.c**: This includes struct and functions that are used to create a history list, which is basically a wrapper for clist. 
    - ```struct hist_clist``` that stores the history commands;
    - supprotive functions including:
        - creating and destory the hist_clist;
        - printing the history;
        - adding in the hist_list;
        - searching hist_list by index, or by prefix;
        - getting hist_list last added index;
        - validating if an index is valid in hist_list；
        - private method that supporting iteration in hist_clist.
- **sha1.c**: This includes functions that support an text based command line ui. It includes:
    - initializing and destroying ui;
    - set status in prompt to display previous execution status;
    - key down and key up to roll back or move forward in history commands;
    - supprotive function to construct the prompt:
        - get current working directory, and truncated in shortcut version as needed;
        - get username, hostname, and home diretory. 
- **logger.h**: Included for log output. 
- **runner.sh**: Inclued for running a varity of the thread count and the difficulty level, an example output of runner is also included in **test_threads.txt**.
- Header filess are also included for the c files.

For compiling the program, runs:
```console
[miasly@dirtmouth P3-mialsy]$ make
```

For running the ```miner``` runs:
```console
[miasly@dirtmouth P3-mialsy]$ ./miner <thread count> <difficulty level> <mine block>
```

## Program output

The example out put of running miner is as follow. 

```console
[miasly@dirtmouth P3-mialsy]$ ./miner 4 16 skittle
Number of threads: 4
  Difficulty Mask: 00000000000000001111111111111111
       Block data: [skittle]

----------- Starting up miner threads!  -----------

Solution found by thread 3:
Nonce: 218384
 Hash: 0000F7B3EF2F063231E323A2190E3A04F69BD7AA
216894 hashes in 0.10s (2208694.68 hashes/sec)
```

The example output of running miner with incorrect input is shown as below. 

```console
# with not enough argument:
[miasly@dirtmouth P3-mialsy]$ ./miner 4 16
Usage: ./miner threads difficulty 'block data (string)'

# with wrong thread count or difficulty level input
[miasly@dirtmouth P3-mialsy]$ ./miner hi what why
Invalid thread number, exiting.
```

## Testing

For running the test cases, use ```make test```. For updating test cases, runs ```make testupdate```. 

For more detailed testing, please refer to below:

- Run all tests:
    ```console
    [miasly@dirtmouth P3-mialsy]$ make test
    ```
- Run one test (let's say we want to run test x):
    ```console
    [miasly@dirtmouth P3-mialsy]$ make test run=[x]
    ```

- Run a few tests (let's say we want to run test x, y, z):
    ```console
    [miasly@dirtmouth P3-mialsy]$ make test run='[x] [y] [z]'
    ```

- **Note**: before run the test case, make sure you are in the correct directory with the makefile.

# Test record
| Thread Count | Difficulty | Time (sec) | Speed (hashes/sec) |
|-----|------|------|------|
| 1 | 4 | 0.00 | 41053.55 
| 2 | 4 | 0.00 | 119837.26 
| 4 | 4 | 0.00 | 115809.11 
| 8 | 4 | 0.00 | 345299.64 
| 1 | 8 | 0.00 | 110376.42 
| 2 | 8 | 0.00 | 3514416.82 
| 4 | 8 | 0.00 | 2320706.27 
| 8 | 8 | 0.00 | 4548216.68 
| 1 | 12 | 0.03 | 575553.00 
| 2 | 12 | 0.01 | 1585913.45 
| 4 | 12 | 0.01 | 2778279.48 
| 8 | 12 | 0.00 | 10140816.06 
| 1 | 16 | 0.37 | 594371.17 
| 2 | 16 | 0.19 | 1228465.30 
| 4 | 16 | 0.10 | 2429027.32 
| 8 | 16 | 0.06 | 4566735.87 
| 1 | 20 | 3.86 | 591516.62 
| 2 | 20 | 1.92 | 1193618.94 
| 4 | 20 | 0.97 | 2355934.10 
| 8 | 20 | 0.49 | 4747394.88 
| 1 | 24 | 52.07 | 590901.47 
| 2 | 24 | 26.19 | 1174888.13 
| 4 | 24 | 12.98 | 2372107.39 
| 8 | 24 | 6.56 | 4698551.27 