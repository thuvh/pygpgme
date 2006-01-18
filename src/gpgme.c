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

#define ADD_TYPE(type)                \
    Py_INCREF(&PyGpgme ## type ## _Type); \
    PyModule_AddObject(mod, #type, (PyObject *)&PyGpgme ## type ## _Type)

    INIT_TYPE(PyGpgmeContext_Type);
    INIT_TYPE(PyGpgmeKey_Type);
    INIT_TYPE(PyGpgmeSubkey_Type);
    INIT_TYPE(PyGpgmeUserId_Type);
    INIT_TYPE(PyGpgmeKeySig_Type);
    INIT_TYPE(PyGpgmeNewSignature_Type);
    INIT_TYPE(PyGpgmeSignature_Type);
    INIT_TYPE(PyGpgmeKeyIter_Type);

    mod = Py_InitModule("gpgme._gpgme", NULL);

    ADD_TYPE(Context);
    ADD_TYPE(Key);
    ADD_TYPE(Subkey);
    ADD_TYPE(UserId);
    ADD_TYPE(KeySig);
    ADD_TYPE(NewSignature);
    ADD_TYPE(Signature);
    ADD_TYPE(KeyIter);

    Py_INCREF(pygpgme_error);
    PyModule_AddObject(mod, "error", pygpgme_error);
}
