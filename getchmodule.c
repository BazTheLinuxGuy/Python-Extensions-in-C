#include "/usr/include/python3.6m/Python.h"
#include <termios.h>

// This module provides two functions for Unix-based terminals,
// equivalent to the old Microsoft functions "getch()" and "getche()".
// These allow the console user to input a single keystroke,without
// having to press enter, and returns the ordinal character value
// of the key press. 
//
// For example, at a menu, with the options preceeded by numbers,
// if the user presses a number "1", the value 49 is returned, the
// ascii ordinal value of the character "1". 
//
// The two functions, called getch() and getche(), are identical, 
// except the getche() echos the character pressed, while getch()
// does not.
//
// This module cobbled together by Bryan A. Zimmer 
// <bazthelinuxguy@gmail.com>, on March 29, 2018, based on widespread 
// knowledge of Unix-based (including Linux) terminal i/o control.
//
// This code is hereby placed in the public domain for whomever
// wishes to use it. Caution is advised, as the module has not been
// thouroughly checked for memory leaks, and does not implement
// reference-counting of symnbols for python garbage-collection.
//
// The following command compiles this module into a
// .so library:
// gcc -Wall -fpic -shared -O2 -o getch.so getchmodule.c
//
// The methodology is taken directly from https://docs.python.org/3/.
// from the section on building extension modules in C.
//

static struct termios old, new;
/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo) {
		new.c_lflag |= ECHO; /* set echo mode */
	} else {
		new.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &old);
}


/* Read 1 character without echo */
static PyObject *getch_getch(PyObject *self) 
{
	char ch;

	initTermios(0); // "0" means turn echo off
	ch = getchar();
	resetTermios();

	return PyLong_FromLong(ch);
}

/* Read 1 character with echo */
static PyObject *getch_getche(PyObject *self) 
{
	char ch;

	initTermios(1); // "1" means turn echo on.
	ch = getchar();
	resetTermios();

	return PyLong_FromLong(ch);
}   

static PyMethodDef GetchMethods[] = {

  {"getch", (PyCFunction)getch_getch, METH_NOARGS,
   "Retrieve a single keystroke without echo."},

  {"getche", (PyCFunction)getch_getche, METH_NOARGS,
   "Retrieve a single keystroke with echo."},

  {NULL, NULL, 0, NULL} // sentinel 
};

static struct PyModuleDef getchmodule = {
    PyModuleDef_HEAD_INIT,
    "getch",   /* name of module */
    "Retrieves a single key from the console, with or without echo.",
				/* module documentation, may be NULL */
    -1,			/* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
     GetchMethods
};

PyMODINIT_FUNC PyInit_getch(void)
{
    return PyModule_Create(&getchmodule);
}
