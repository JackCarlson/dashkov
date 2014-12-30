dashkov
=======
C++ text markov chain generator.  Feel free to leverage the code within however you wish!

-Jack Carlson (jackattack1@gmail.com)

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
    reply = dashkov.get_reply('Some string', 15) # returns a string from the markov model. 
    # the input string is used to seed the model. the integer argument is a maximum of words to return.
    dashkov.learn('Some other string') # add a string to the model
