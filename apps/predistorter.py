# this module will be imported in the into your flowgraph
import numpy as np

taps = np.genfromtxt('/tmp/predistorter_taps.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps = np.complex_(taps)
