import pandas as pd
from pandas import Series
#import tensorflow as tf
import tensorflow.compat.v1 as tf
import numpy as np
#import matplotlib.pyplot as plt
import sys

def test_data(series, forecast, num_periods):
    test_x_setup = TS[-(num_periods + forecast):]
    testX = test_x_setup[:num_periods].reshape(-1, 12, 1)
    testY = TS[-(num_periods):].reshape(-1, 12, 1)
    return testX, testY

# import data

fname = '/home/smarteco/DATA/forecast/' + sys.argv[1] + '/in/' + sys.argv[2]

series = Series.from_csv(fname, header=0)

TS = np.array(series)
num_periods = 12
f_horizon = 1

#print(len(TS))
#print(len(TS) % num_periods)

x_data = TS[:(len(TS) - (len(TS) % num_periods))]
x_batches = x_data.reshape(-1, 12, 1)

y_data = TS[1:(len(TS)-(len(TS) % num_periods)) + f_horizon]
y_batches = y_data.reshape(-1, 12, 1)

X_test, Y_test = test_data(TS, f_horizon, num_periods)

#tf.reset_default_graph()
tf.disable_eager_execution()
tf.reset_default_graph()
num_periods = 12
inputs = 1
hidden = 100
output = 1

X = tf.placeholder(tf.float32, [None, num_periods, inputs])
y = tf.placeholder(tf.float32, [None, num_periods, output])

basic_cell = tf.nn.rnn_cell.BasicRNNCell(num_units=hidden, activation=tf.nn.relu)
rnn_output, states = tf.nn.dynamic_rnn(basic_cell, X, dtype=tf.float32)

learning_rate = 0.001

stacked_rnn_output = tf.reshape(rnn_output, [-1, hidden])
stacked_outputs = tf.layers.dense(stacked_rnn_output, output)
outputs = tf.reshape(stacked_outputs, [-1, num_periods, output])

loss = tf.reduce_sum(tf.square(outputs - y)) # define the cost function which evaluates the quality of our model
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
training_op = optimizer.minimize(loss)

init = tf.global_variables_initializer()

epochs = 1000       # number of iterations or training cycles, includes both the FeedForward and Backpropogation

with tf.Session() as sess:
    init.run()

    for ep in range(epochs):
        sess.run(training_op, feed_dict={X: x_batches, y: y_batches})
        if ep % 100 == 0:
            mse = loss.eval(feed_dict={X: x_batches, y: y_batches})
#            print(ep, "\tMSE:", mse)

    y_pred = sess.run(outputs, feed_dict={X: X_test})

    mse = np.sqrt(((X_test - y_pred) ** 2).mean())

    pred = [x_row[0] for x_row in y_pred[0]]

    sr = series.head(0)
    print(sr.name)
    print('{0:.2f}'.format(100 - mse))

    for i in pred:
        print('{0:.2f}'.format(i)) 

#plt.title("Forecast vs Actual", fontsize=14)
#plt.plot(pd.Series(np.ravel(Y_test)), "bo", markersize=10, label="Actual")
#plt.plot(pd.Series(np.ravel(y_pred)), "r.", markersize=10, label="Forecast")
#plt.legend(loc="upper left")
#plt.xlabel("Time Periods")
#plt.ylim(ymax=31.0, ymin=29.0)
#plt.show()
