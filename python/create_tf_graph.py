import tensorflow as tf
import numpy as np

with tf.Session() as sess:
    a = tf.Variable(5.0, name='a')
    b = tf.Variable(6.0, name='b')
    c = tf.multiply(a, b, name="c")

    sess.run(tf.global_variables_initializer())

    print a.eval() # 5.0
    print b.eval() # 6.0
    print c.eval() # 30.0
    
    tf.train.write_graph(sess.graph_def, 'models/', 'graph.pb', as_text=False)