rm Makefile
rm *.o
rm grab_test
qmake -o Makefile grab_test.pro
make
