# projects for my resume
Projects shown in the resume

## 1. POS implementation
### EXPLANATION
This is the data structure I work on in the current lab. In the case of other version control systems such as GitHub, Mercury, subversions, Merkle trees are used for the network communications. Merkle tree is n-ary tree with the hash value of each file in the repository at leaf nodes. Internal nodes have the hash value of the concatenated hash value of children nodes. This data structure enables check of the bundle of chunks at once, because, assuming there is no hash collision, if a parent node's hash turns out to be the same, it is the same. This fits the case of version control system, where most of the files remain the same for each versions. Whenever the system finds different files by computing the hash values, it updates the leaf nodes as well as internal nodes that has the leaf nodes at the end of the tree. <br />This data structure called Merkle tree does not cope well with the chunks that are cut with Rabin fingerprint, because of chunk shift problem. Chunk shift problem occurs whenever the boundary window of the chunk has been modified. The modification either creates new chunk or concatenate two chunks, which would shift the chunks on the right either towards right (when new chunk is created) or left (when two chunks are concatenated). If chunk shift happens, the whole structure of Merkle tree is changed, therefore we would not be able to take advantage of the data structure. To prevent this, Wang et al. (2018) has thought of new data structure called POS tree. The basic idea is to 

### NOTE
This data structure implements the hash value in SHA-1, which we know it is broken, yet we used it for the simplicity. 

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

