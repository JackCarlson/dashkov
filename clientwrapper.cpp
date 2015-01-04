#include <Python.h>
#include "client.cpp"

static PyObject *
dashkov_populate(PyObject *self, PyObject *args)
{
	const char *filename;

	if (!PyArg_ParseTuple(args, "s", &filename))
		return NULL;

    if ( fileExists( "dashkov.db" ) )
        loadFromDb();
    else
        populateChain( "nightmareabbey.txt" );

	Py_RETURN_NONE;
}

static PyObject *
dashkov_get_reply(PyObject *self, PyObject *args)
{
	const char *input;
	int maxwords;

	if(!PyArg_ParseTuple(args, "si", &input, &maxwords))
		return NULL;

	return Py_BuildValue("s", getResponse(input, maxwords));
}

static PyObject *
dashkov_learn(PyObject *self, PyObject *args)
{
	const char *input;

	if(!PyArg_ParseTuple(args, "s", &input))
		return NULL;

	learn(input);

	Py_RETURN_NONE;
}

static PyMethodDef ClientMethods[] = {
	{"populate", dashkov_populate, METH_VARARGS, "Read text file into markov chain"},
	{"get_reply", dashkov_get_reply, METH_VARARGS, "Generate a reply"},
	{"learn", dashkov_learn, METH_VARARGS, "Learn some text"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdashkov(void)
{
	(void) Py_InitModule("dashkov", ClientMethods);
}