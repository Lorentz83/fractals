FRACTALS ;)
===========

Fractals ;) is a c++/qt4 project which generate fractals, it uses
range image techniques to make them colorful and create a fake 3D
projection.


This project was developed by me, [Lorenzo Bossi](https://github.com/Lorentz83), 
and my friend Stefano Massera as project for the final examination of
the Image Computing class during our Master degree in Computer Science.


Building the project
--------------------

The code requires only QT4 but it uses some gcc not standard
extensions.

It is tested on linux with QT 4.8.6 and g++ 4.8.2.
It used to work on windows too using an old mingw, to compile it with
vc++ the code needs some minor modification that I'll plan to do in
the next future.

To make the project move in the `src` directory and execute the
following commands: 

```
$ qmake
$ make
$ make install

```

note that the last command does not really install the program
(therefore no root privileges are required) but it just copies in the
`bin` directory the executable and all the files it needs to work. 

Documentation
-------------

The directory `doc` contains a pdf file with the project documentation
written in Italian.

The code itself is not really documented and contains a lot of Italian
words. I'm sorry for this issue.
