coherent-noise
==============

C++ coherent noise (i.e. perlin, simplex) library and experiments.
Made as Visual Studio 2012 solution. Uses some C++11 features. Requires at least Microsoft Visual C++ November 2012 CTP.

Here I am trying to achieve good speed with N-dimensional noise written with C++ templates. For now without specializations.
At the moment only old Perlin Noise is implemented. Unfortunately, my template implementation is still 40% slower than the original Perlin's one. Seems like a flaw of optimization in compiler.