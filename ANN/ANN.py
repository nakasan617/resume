import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 

def sigmoid(x):
    return 1.0 / (1 + np.exp(-x))

def grad_sigmoid(x):
    return sigmoid(x) * (1 - sigmoid(x))

def squared(y, yhat):
    return np.mean((y - yhat)**2) / 2.0

def grad_squared(y, yhat):
    return yhat - y

class ANN:
    #My neural net class has the following __init__() method:

    # Insantiates a neural network.
    # nodes: a list of the sizes of the layers. Let N be the length of nodes.
    # activs: a list of N-1 activation functions for layers 2 through N.
    # grad_activs: a list of gradients for the activation functions given in
    # activs.
    # loss: the loss function for the neural network.
    # grad_loss: the gradient of the loss function.
    # alpha: The learning rate.
    def __init__(self, nodes, activs, grad_activs, loss, grad_loss, alpha):
        self.nodes = nodes
        self.activs = activs
        self.grad_activs = grad_activs
        self.loss = loss 
        self.grad_loss = grad_loss
        self.alpha = alpha 
        return 

    def set_weights(self, weights):
    # weights is a list of lists that contains the weights for the inputs to each node
        self.weights = weights
        for i in range(len(self.weights)):
            self.weights[i] = np.random.random(weights[i].shape) * (2*np.sqrt(6/(self.nodes[i-1] + self.nodes[i]))) - (np.sqrt(6/(self.nodes[i-1] + self.nodes[i])))
        return 
    
    def set_weights_for_test(self, weights):
            self.weights = weights
            return

    def convert_y(self, y):
        iteration = len(y)
        to_return = []
        for i in range(iteration):
            tmp = np.zeros(10)
            if y[i] == 0:
                tmp[0] = 1
            elif y[i] == 1:
                tmp[1] = 1
            elif y[i] == 2:
                tmp[2] = 1
            elif y[i] == 3:
                tmp[3] = 1
            elif y[i] == 4:
                tmp[4] = 1
            elif y[i] == 5:
                tmp[5] = 1
            elif y[i] == 6:
                tmp[6] = 1 
            elif y[i] == 7:
                tmp[7] = 1
            elif y[i] == 8:
                tmp[8] = 1
            elif y[i] == 9:
                tmp[9] = 1
            else:
                raise ValueError('this should not happen') 
            to_return.append(tmp) 
        return to_return 
            
    def fit(self, X, y, alpha, t, biases):
    # X is an (m, n)-shaped numpy input matrix 
    # y is an (m, 1)-shaped numpy output vector 
        if t > len(X):
            t = len(X)

        #layers = len(X[0])
        layers = 3
        error = []
        for n in range(t):
            delj = []
            A_prev = X[n]
            ins = []
            for j in range(layers - 1):
                to_append = 0
                to_append = np.dot(np.transpose(self.weights[j]), A_prev) + biases[j]
                ins.append(to_append) 
                A_prev = sigmoid(to_append)
            
            error.append(np.sum((y[n] - A_prev)**2)) 
            delj.append(grad_sigmoid(ins[len(ins) - 1]) * (y[n] - A_prev)) 

            for j in range(layers-2):
                to_mult = np.sum(self.weights[layers-j-2] * (delj[0]), axis = 1)
                to_insert = grad_sigmoid(ins[layers-j-3])*to_mult
                delj.insert(0, to_insert)

            # update the weights 
            for j in range(layers - 1):
                to_add = (alpha * sigmoid(ins[j]) * delj[j])
                self.weights[j] = self.weights[j] + to_add

        plt.scatter(np.arange(t), error, s = 0.1) 
        plt.ylabel('error') 
        plt.xlabel('number of iterations') 
        plt.title('error (least square)') 
        plt.savefig('error.png', format = 'png') 
        return 

    def get_weights(self):
        return self.weights

def example():
    ann = ANN([3,5,4], [sigmoid, sigmoid], [grad_sigmoid, grad_sigmoid], squared, grad_squared, 0.001)
    x = np.arange(18).reshape(6,3)
    y = np.zeros((6,4))
    y[0,0] = 1
    y[1,1] = 1
    y[2,2] = 1
    y[3,3] = 1
    y[4,0] = 1
    y[5,1] = 1

    # weights = [np.array([[-0.45253739, -0.16098125, -0.08067616], [-0.20108552, -0.23830988, -0.10435417],[ 0.23856118, -0.41676105, -0.84241045], [-0.48465322,  0.68205681, -0.08747013], [-0.60252568,  0.61336393,  0.15432761]]), np.array([[ 0.51830356, -0.38052089, -0.11475257, -0.638776  , -0.46862776], [-0.5736774 , -0.07588861,  0.12875949,  0.64619578, -0.63165241], [ 0.2970735 , -0.33831311, -0.52670153,  0.02932394,  0.64146576], [ 0.55335698,  0.40126154, -0.04065573, -0.09815836,  0.52545504]])]
    # weights here are written in the reverse order from mine 
    # I am going to add the extra node from the beginning so that I wont have to go throught the hassle 
    biases = [np.array([-1.14282988,  0.22379589,  1.07941935, -1.55562539,  0.05029518]), np.array([ 0.28503219, -0.28988183, -2.15749797,  0.21010699])]
    weights = [np.ones((3,5)), np.ones((5, 4))]
    ann.set_weights(weights)
    weights = [np.transpose(np.array([[-0.45253739, -0.16098125, -0.08067616],
       [-0.20108552, -0.23830988, -0.10435417],
       [ 0.23856118, -0.41676105, -0.84241045],
       [-0.48465322,  0.68205681, -0.08747013],
       [-0.60252568,  0.61336393,  0.15432761]])), np.transpose(np.array([[ 0.51830356, -0.38052089, -0.11475257, -0.638776  , -0.46862776],
       [-0.5736774 , -0.07588861,  0.12875949,  0.64619578, -0.63165241],
       [ 0.2970735 , -0.33831311, -0.52670153,  0.02932394,  0.64146576],
       [ 0.55335698,  0.40126154, -0.04065573, -0.09815836,  0.52545504]]))]
    ann.set_weights_for_test(weights) 
    
    ann.fit(x, y, 0.001, 100, biases)
    print(ann.get_weights())
    return

def mnist_fit():
    df = pd.read_csv('train.csv', header=0)
    y = df[df.columns[0]] 
    X = df.drop(columns = 'label')
    ann = ANN([784, 400, 10], [sigmoid, sigmoid], [grad_sigmoid, grad_sigmoid], squared, grad_squared, 0.001) 
    weights = [np.zeros((784,400)), np.zeros((400, 10))]
    biases = [np.zeros(400), np.zeros(10)]
    
    ann.set_weights(weights) 

    X = X.values 
    y = y.values
    y = ann.convert_y(y) 
    ann.fit(X, y, 0.001, 10000, biases) 
    print(ann.get_weights()) 
    
#example()
mnist_fit() 
