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
    INIT_TYPE(PyGpgmeKey_Type);
    INIT_TYPE(PyGpgmeSubkey_Type);
    INIT_TYPE(PyGpgmeUserId_Type);
    INIT_TYPE(PyGpgmeKeySig_Type);
    INIT_TYPE(PyGpgmeNewSignature_Type);
    INIT_TYPE(PyGpgmeSignature_Type);

    mod = Py_InitModule("gpgme._gpgme", NULL);

    Py_INCREF(&PyGpgmeContext_Type);
    PyModule_AddObject(mod, "Context", (PyObject *)&PyGpgmeContext_Type);
    Py_INCREF(&PyGpgmeKey_Type);
    PyModule_AddObject(mod, "Key", (PyObject *)&PyGpgmeKey_Type);
    Py_INCREF(&PyGpgmeSubkey_Type);
    PyModule_AddObject(mod, "Subkey", (PyObject *)&PyGpgmeSubkey_Type);
    Py_INCREF(&PyGpgmeUserId_Type);
    PyModule_AddObject(mod, "UserId", (PyObject *)&PyGpgmeUserId_Type);
    Py_INCREF(&PyGpgmeKeySig_Type);
    PyModule_AddObject(mod, "KeySig", (PyObject *)&PyGpgmeKeySig_Type);
    Py_INCREF(&PyGpgmeNewSignature_Type);
    PyModule_AddObject(mod, "NewSignature",
                       (PyObject *)&PyGpgmeNewSignature_Type);
    Py_INCREF(&PyGpgmeSignature_Type);
    PyModule_AddObject(mod, "Signature", (PyObject *)&PyGpgmeSignature_Type);
    Py_INCREF(pygpgme_error);
    PyModule_AddObject(mod, "error", pygpgme_error);
}
