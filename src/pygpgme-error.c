/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#include <Python.h>
#include "pygpgme.h"

PyObject *pygpgme_error = NULL;

PyObject *
pygpgme_error_object(gpgme_error_t err)
{
    char buf[256] = { '\0' };
    PyObject *exc, *attr;

    if (err == GPG_ERR_NO_ERROR)
        Py_RETURN_NONE;

    /* get the error string */
    if (gpgme_strerror_r(err, buf, 255) != 0)
        strcpy(buf, "Unknown");

    exc = PyObject_CallFunction(pygpgme_error, "ls", (long)err, buf);
    if (!exc)
        return NULL;
    /* set the source and code as attributes of the exception object: */
    attr = PyInt_FromLong(gpgme_err_source(err));
    PyObject_SetAttrString(exc, "source", attr);
    Py_DECREF(attr);

    attr = PyInt_FromLong(gpgme_err_code(err));
    PyObject_SetAttrString(exc, "code", attr);
    Py_DECREF(attr);

    return exc;
}

/* check whether the given gpgme_error_t value indicates an error.  If so,
 * raise an equivalent Python exception and return TRUE */
int
pygpgme_check_error(gpgme_error_t err)
{
    PyObject *exc;

    if (err == GPG_ERR_NO_ERROR)
        return 0;

    exc = pygpgme_error_object(err);
    if (!exc)
        return 1;

    PyErr_SetObject(pygpgme_error, exc);

    return 1;
}
