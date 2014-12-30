dashkov
=======

C++ text markov chain generator


Building
--------
To run a test on the sample text (Nightmare Abbey by Thomas Love Peacock):

    make
    ./dashkovgen

Python Bindings
---------------
Build the shared object by running `python setup.py build`. The python interface is in a declarative style:

    import dashkov
    dashmod.populate('somefile.txt')
    reply = dashkov.get_reply('Some string') # returns a string from the markov model
    dashkov.learn('Some other string') # add a string to the model
