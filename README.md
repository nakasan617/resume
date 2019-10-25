# projects for my resume
Projects shown in the resume

## 1. POS implementation on destor
### BRIEF EXPLANATION
This is the Pattern-Oriented-Split tree implemented on destor. It is a mixture of Merkle Tree and B+ tree and used for the faster retrieval of the data. This tree structure has been implemented due to the difficulty of implementing Merkle tree on top of content defined chunks made by rabin fingerprint. 
It currently has the implementation of basic POS tree, which is written in the 
POS phase of the backup operation.
What needs to be implemented in the next 3 weeks is the implementation of restoration module and client side implementation.

### THE PURPOSE OF THE POST
This is what I do as a research, it should contribute to the field by showing a new concept that is publicly available.

### NOTE
To configure the program, there is a dependency problem of glib.h, libglib.so, and libssl-dev for the SHA-1 calculation, which is specified in the README in the directory.

## 2. undefeatable tic-tac-toe
### BRIEF EXPLANATION
This program allows you to play tic-tac-toe against the computer, who is supposedly undefeatable. 
It uses min-max algorithm and plays the optimal strategy. Explanation to how to play is written in the directory tic-tac-toe. 

### THE PURPOSE OF THE POST
I posted this simply because I thought this was a cool program, and it shows my interest in game theory and economics, which I studied for the undergrad. 

### SHORTCOMINGS
If in the situation where if you play optimally, the computer giveup, i.e. it plays the left most slot available. 
The game is, obviously, computationally intensive, because strategy set grows exponentially and as of now serial implementation is done. 

## 3. Neural Network implementation from scratch in python
### BRIEF EXPLANATION 
This is the python implementation over the data of MNIST.  
I made this to gain more understanding over the algorithm. 

### THE PURPOSE OF THE POST
To show my mathematical understanding of the machine learning algorithms, and my ability to use python libraries, mainly numpy and pandas 
 
## 4. Ray Tracing using GPU
### BRIEF EXPLANATION
This is to compute Ray Tracing, method for rendering objects with light interactions.  
This uses cuda, which uses GPU to do parallel computing.

