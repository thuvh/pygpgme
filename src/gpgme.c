/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#include <Python.h>
#include "pygpgme.h"

PyMODINIT_FUNC
init_gpgme(void)
{
    PyObject *mod;

    pygpgme_error = PyErr_NewException("gpgme.error",
                                       PyExc_RuntimeError, NULL);

#define INIT_TYPE(type)                      \
    if (!type.ob_type)                       \
        type.ob_type = &PyType_Type;         \
    if (!type.tp_alloc)                      \
        type.tp_alloc = PyType_GenericAlloc; \
    if (!type.tp_new)                        \
        type.tp_new = PyType_GenericNew;     \
    if (PyType_Ready(&type) < 0)             \
        return

    INIT_TYPE(PyGpgmeContext_Type);

    mod = Py_InitModule("_gpgme", NULL);

    Py_INCREF(&PyGpgmeContext_Type);
    PyModule_AddObject(mod, "Context", (PyObject *)&PyGpgmeContext_Type);
    Py_INCREF(pygpgme_error);
    PyModule_AddObject(mod, "error", pygpgme_error);
}
