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
        return -1;

    PyErr_SetObject(pygpgme_error, exc);

    return -1;
}

gpgme_error_t
pygpgme_check_pyerror(void)
{
    PyObject *err_type, *err_value, *err_traceback;
    gpgme_error_t err;
    PyObject *args = NULL, *code = NULL;

    if (!PyErr_Occurred())
        return GPG_ERR_NO_ERROR;

    PyErr_Fetch(&err_type, &err_value, &err_traceback);
    PyErr_NormalizeException(&err_type, &err_value, &err_traceback);
    err = gpgme_error(GPG_ERR_GENERAL);

    /* get the first argument of the exception */
    args = PyObject_GetAttrString(err_value, "args");
    if (args == NULL)
        goto end;
    code = PyTuple_GetItem(args, 0);
    if (code == NULL)
        goto end;

    if (PyErr_GivenExceptionMatches(err_type, pygpgme_error)) {
        if (PyInt_Check(code))
            err = PyInt_AsLong(code);
    } else if (PyErr_GivenExceptionMatches(err_type, PyExc_IOError) ||
               PyErr_GivenExceptionMatches(err_type, PyExc_OSError)) {
        if (PyInt_Check(code))
            err = gpgme_err_code_from_errno(PyInt_AsLong(code));
    }

 end:
    Py_XDECREF(err_type);
    Py_XDECREF(err_value);
    Py_XDECREF(err_traceback);
    Py_XDECREF(args);
    PyErr_Clear();

    return err;
}

int
pygpgme_no_constructor(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyErr_Format(PyExc_NotImplementedError,
                 "can not directly create instances of %s",
                 self->ob_type->tp_name);
    return -1;
}
