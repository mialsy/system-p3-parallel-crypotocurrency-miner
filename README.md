# Project 3: Parallel Cryptocurrency Miner

**Author**: [Chuxi Wang](https://www.instagram.com/_mialsy_/ "click here to see awesome kitties, helps me survive this project :)") 

**Summmary**: This is the project 3 for CS 521 System programing. In this project, I modified the single-thread version of the crypttocurrency miner to parallel version, using the producer-consumer model. This ```miner``` program allow user to input customized thread number, difficulty level and data block to create their own personalized crytocurrency with desired efficiency (thread count).

## About the project

*When I was young I thought that money was the most important thing in life; now that I am old I know that it is.* as said by my favorite writer Oscar Wilde. 

To show our respect to money, here we built our own cryptocurrency which defines the value of the coin by certain pattern of the zeros in the sha1sum result of the input string. To be specific, our coins are basically strings with a trailing nonce, which has sha1sum of the defined leading zero counts. The more zeros there are, the more value we assigned to the coin. 

For instance, ```gumball4``` with sha1sum of ```3BF9AB79960ECA1B4AAEE7BDBF9884A20FE3E539``` is considered having lower value than ```gumabll587``` which has sha1sum of ```00DF1955AF4D92D0B9DF0FF7E27A23A3D7429FF9```.

Now we have defined the value of our crypotocurrency, we need to actually **mine** the coin so that we can talk about making money!

<p align="center">
<img src="http://49.media.tumblr.com/tumblr_lbh1lw2HId1qa1xnko1_500.gif" alt="dwarfs mining in Snow White and The Seven Dwarfs" width="450">
</p>

### What is ```miner```? 

```miner``` is a program that finds the nonce with user provided data block that can have at least leading zero number given by difficulty. The```mine``` function of the  ```miner``` program provides the main functionality of the mining, it traverse from the start to the end of the nonce mining range, and calcuate sha1sum of each nonce, and stores the sha1sum result in the digest table. If a data block and nonce combination produces a sha1sum with the desired leading zeros, it would be considered as we found the nonce. Once a nonce is found, it would be returned; otherwise, zero value would be returned to indicate that the no nonce has been found, as our nonce is defined to start at one.

### Multi-thread parallel program and the producer consumer model

Though a sigel thread program is sufficient to find the nonce, a multi-thread program makes finding the nonce more efficient ~~(this is the reason why there are seven dwarfs!)~~. 

A multi-thread version allows different thread to work on mining different nonce range, and since there are multiple thread works together, altogether they would have a higher chance to find the block comparing to a single thread version in the same time peroid. If we simplified this problem and assuming that all thread can process one nonce in one second, and they are perfectly parallel, then ten thread can check ten nonce in one second, wheras one thread can only check one in the same one second. 

Here we agreed on a multi-thread program persumably runs faster than a single thread version, but why bother using the producer-consumer model? The producer-consumer model is a model came up by Dijstra. This model basically requires two part, one is the producer, which produces the task and adds the task to the buffer; the other part is the consumer, which takes task from the buffer. Comparing this to the alternative we have, which is just assign all the range evenly (or not evenly, does not matter) to the worker intially, the producer-consumer model provide a more flexible way to deal with the problem. For instance, the tasks may comes in a stream, i.e. the whole range may have not been given to us initially. Then we would worry about how do we handle the tasks coming afterwards if we choose the alternative way. 

Another cases is that if the worker thread can get offline at some point, when that happens, it would be more resonable that the other worker thread takes the leftover tasks from the queue, rather than witing the offline thread to be online again to work on the tasks it has been assigned initially.

### The task queue and why do we need a queue? 

To support the producer-consumer model, a task queue is used to buffer the task produced. The use of the task queue provide better decoupling of the producer and consumer. Consider the following situation, the producer and consumer can have different rate in doing their own job, it is possible that producer produce more than one task and the consumer can consumer only one task at a time. Supposing we are not using a task queue, the producer would only produce one task and than wait for the consumer to consume the task, and *vice versa*. Thus the efficiency of both sides would depend on each other. With the task queue, the producer does not need to worry about how many task has been consumed, all its job is to add to the task queue; and it is the same for the consumer side. 

In this project, the queue is implemented with a linked list structure. A head and a tail are stored in this linked list structure, when offering one element to the queue, it is appended to the tail of the queue. When polling from queue, data is retrived from the head of the queue. Storing head and tail provide O(1) time for offering and polling from the queue. Additonally, the queue size is recorded in this implementation as well, in order to provide O(1) time getting the size of the queue. 

Note that the implementation is specific for this project, the task queue only stores uint64_t data, which is the start of the nonce mining range. It can be further improved to use varity of the data type.

## Program options

- Running ```miner```: pass in the thread count, difficulty level, and the input block to mine.
    - thread count: the count for the worker thread, at least one. 
    - difficulty level: the number of leading zeros, range from 0-32 (inclusive).
    - data block: the prefix of the nonce, data block + nonce together provide the sha1sum that matches the difficulty.  

## Included files

Here is a list of files that is included:

- **Makefile**: Included to compile and run the programs.
- **miner.c** The driver for ```miner``` program. Includes main function and supportive functions:
    - ```main()``` function that has the functionality of: 
        - parse the program arguments;
        - function as producer:
            - create tasks;
            - adding tasks to the task queue;
        - reporting the the result, including:
            - the correct nonce;
            - the sha1sum value of the nonce;
            - the time used to process the nonce, and the corresponding speed.
    - ```get_time()``` get the current wall clock time, used to compute the running time and speed.
    - ```print_binary32()``` to print digits in binary. 
    - ```mine()``` mine the data block with given nonce range.
    - ```worker_thread()``` the thread routine for the worker threads, has the following functionality
        - consumer a task from the task queue;
        - work on the mining range assigned with the task;
        - boardcast to other worker threads when a nonce is found.

- **queue.c**: This includes struct and functions that are used to create task queue, which is implemeted with a linked list structure.
    - ```struct qnode``` the node for linked list, inlcuding:
        - ```uint64_t data```, the data of the task, which is just the start nonce;
        - ```next```, the next node in the linked list.
    - ```struct queue``` the queue structure, including:
        - ```head``` and ```tail```;
        - ```size```.
    - supprotive functions including:
        - creating and destory the queue;
        - printing the queue;
        - offering to the tail;
        - polling from the head;
        - getting the size of the queue.
- **sha1.c**: This includes functions to calculate sha1sum.
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

In this project I also included a runner.sh file to run test for a varity of input to get a better understanding of the multi-thread program efficiency. 

The following table is an exmaple output of the runner. 

| Thread Count | Difficulty | Time (sec) | Speed (hashes/sec) |
|-----|------|------|------|
| 1 | 4 | 0.00 | 44779.05 
| 2 | 4 | 0.00 | 101112.69 
| 4 | 4 | 0.00 |225211.40
| 8 | 4 | 0.00 | 166647.29 
| 1 | 8 | 0.00 | 89576.84 
| 2 | 8 | 0.00 | 218697.42 
| 4 | 8 | 0.00 | 517161.76 
| 8 | 8 | 0.00 | 615216.18 
| 1 | 12 | 0.03 | 566005.62 
| 2 | 12 | 0.01 | 1066125.95 
| 4 | 12 | 0.01 | 2081329.56 
| 8 | 12 | 0.01 | 2644963.77 
| 1 | 16 | 0.40 |537389.61 
| 2 | 16 | 0.20 |1089555.49 
| 4 | 16 | 0.10 |2097374.41 
| 8 | 16 | 0.06 |3702793.82 
| 1 | 20 | 4.04 |559574.83 
| 2 | 20 | 2.06 |1100647.19 
| 4 | 20 | 1.03 |2196448.22 
| 8 | 20 | 0.56 |4066734.82 
| 1 | 24 | 55.80 |545946.42 
| 2 | 24 | 27.74 |1098216.84 
| 4 | 24 | 13.90 |2192240.95 
| 8 | 24 | 7.34 |4152149.08
| 1 | 28 | 593.49 |551761.19
| 2 | 28 | 295.40 |1108538.19
| 4 | 28 | 149.91 |2184460.46
| 8 | 28 | 78.71 |4160143.33

And here is a plot showing the efficiency:
<img src="https://github.com/usf-cs521-sp21/P3-mialsy/blob/main/images/Picture1.svg" alt="plot of time vs thread" width="600">
