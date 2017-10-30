# this module will be imported in the into your flowgraph
import numpy as np

taps1 = np.genfromtxt('/tmp/predistorter_taps1.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps1 = np.complex_(taps1)

taps2 = np.genfromtxt('/tmp/predistorter_taps.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps2 = np.complex_(taps2)

taps3 = np.genfromtxt('/tmp/predistorter_taps3.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps3 = np.complex_(taps3)

taps4 = np.genfromtxt('/tmp/predistorter_taps4.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps4 = np.complex_(taps4)

taps5 = np.genfromtxt('/tmp/predistorter_taps5.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps5 = np.complex_(taps5)

taps6 = np.genfromtxt('/tmp/predistorter_taps6.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps6 = np.complex_(taps6)

taps7 = np.genfromtxt('/tmp/predistorter_taps7.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps7 = np.complex_(taps7)

taps8 = np.genfromtxt('/tmp/predistorter_taps8.csv', converters={0: lambda x: x.replace('i','j')},dtype=str)
taps8 = np.complex_(taps8)

taps_sum = taps1+taps2+taps3+taps4+taps5+taps6+taps7+taps8
taps = [x * 0.125 for x in taps_sum]
taps = taps2
