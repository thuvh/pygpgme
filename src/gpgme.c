#include <Python.h>
#include "pygpgme.h"

PyMODINIT_FUNC
init_gpgme(void)
{
    PyObject *mod;

    mod = Py_InitModule("gpgme._gpgme", NULL);
}
