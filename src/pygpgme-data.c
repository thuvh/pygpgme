/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#include <Python.h>
#include <errno.h>
#include "pygpgme.h"

/* called when a Python exception is set.  Clears the exception and tries
 * to set errno appropriately. */
static void
set_errno(void)
{
    PyObject *exc, *value, *tb, *py_errno;

    PyErr_Fetch(&exc, &value, &tb);

    /* if we have an IOError, try and get the actual errno */
    if (PyErr_GivenExceptionMatches(exc, PyExc_IOError) && value != NULL) {
        py_errno = PyObject_GetAttrString(value, "errno");
        if (py_errno != NULL && PyInt_Check(py_errno)) {
            errno = PyInt_AsLong(py_errno);
        } else {
            PyErr_Clear();
            errno = EINVAL;
        }
        Py_XDECREF(py_errno);
    } else {
        errno = EINVAL;
    }
    Py_XDECREF(tb);
    Py_XDECREF(value);
    Py_DECREF(exc);
}

static ssize_t
read_cb(void *handle, void *buffer, size_t size)
{
    PyObject *fp = handle;
    PyObject *result;
    int result_size;

    result = PyObject_CallMethod(fp, "read", "l", (long)size);
    /* check for exceptions or non-string return values */
    if (result == NULL) {
        set_errno();
        return -1;
    }
    /* if we don't have a string return value, consider that an error too */
    if (!PyString_Check(result)) {
        Py_DECREF(result);
        errno = EINVAL;
        return -1;
    }
    /* copy the result into the given buffer */
    result_size = PyString_Size(result);
    if (result_size > size)
        result_size = size;
    memcpy(buffer, PyString_AsString(result), result_size);
    Py_DECREF(result);
    return result_size;
}

static ssize_t
write_cb(void *handle, const void *buffer, size_t size)
{
    PyObject *fp = handle;
    PyObject *result;

    result = PyObject_CallMethod(fp, "write", "s#", buffer, (int)size);
    if (result == NULL) {
        set_errno();
        return -1;
    }
    Py_DECREF(result);
    return size;
}

static off_t
seek_cb(void *handle, off_t offset, int whence)
{
    PyObject *fp = handle;
    PyObject *result;

    result = PyObject_CallMethod(fp, "seek", "li", (long)offset, whence);
    if (result == NULL) {
        set_errno();
        return -1;
    }
    Py_DECREF(result);

    /* now get the file location */
    result = PyObject_CallMethod(fp, "tell", NULL);
    if (result == NULL) {
        set_errno();
        return -1;
    }
    if (!PyInt_Check(result)) {
        Py_DECREF(result);
        errno = EINVAL;
        return -1;
    }
    offset = PyInt_AsLong(result);
    Py_DECREF(result);
    return offset;
}

static void
release_cb(void *handle)
{
    PyObject *fp = handle;

    Py_DECREF(fp);
}

static struct gpgme_data_cbs python_data_cbs = {
    .read    = read_cb,
    .write   = write_cb,
    .seek    = seek_cb,
    .release = release_cb,
};

/* create a gpgme data object wrapping a Python file like object */
gpgme_error_t
pygpgme_data_new(gpgme_data_t *dh, PyObject *fp)
{
    gpgme_error_t error;

    error = gpgme_data_new_from_cbs(dh, &python_data_cbs, fp);

    /* if no error, then the new gpgme_data_t object owns a reference to
     * the python object */
    if (error == GPG_ERR_NO_ERROR)
        Py_INCREF(fp);

    return error;
}
