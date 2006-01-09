/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#include "pygpgme.h"

static void
pygpgme_context_dealloc(PyGpgmeContext *self)
{
    if (self->ctx)
        gpgme_release(self->ctx);
    self->ctx = NULL;
    PyObject_Del(self);
}

static int
pygpgme_context_init(PyGpgmeContext *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "", kwlist))
        return -1;

    if (self->ctx != NULL) {
        PyErr_SetString(PyExc_ValueError, "context already initialised");
        return -1;
    }

    if (pygpgme_check_error(gpgme_new(&self->ctx)))
        return -1;

    return 0;
}

static PyObject *
pygpgme_context_get_protocol(PyGpgmeContext *self)
{
    return PyInt_FromLong(gpgme_get_protocol(self->ctx));
}

static int
pygpgme_context_set_protocol(PyGpgmeContext *self, PyObject *value)
{
    gpgme_protocol_t protocol;

    protocol = PyInt_AsLong(value);
    if (PyErr_Occurred())
        return -1;

    if (pygpgme_check_error(gpgme_set_protocol(self->ctx, protocol)))
        return -1;

    return 0;
}

static PyObject *
pygpgme_context_get_armor(PyGpgmeContext *self)
{
    return PyBool_FromLong(gpgme_get_armor(self->ctx));
}

static int
pygpgme_context_set_armor(PyGpgmeContext *self, PyObject *value)
{
    int armor;

    armor = PyInt_AsLong(value) != 0;
    if (PyErr_Occurred())
        return -1;

    gpgme_set_armor(self->ctx, armor);
    return 0;
}

static PyObject *
pygpgme_context_get_textmode(PyGpgmeContext *self)
{
    return PyBool_FromLong(gpgme_get_textmode(self->ctx));
}

static int
pygpgme_context_set_textmode(PyGpgmeContext *self, PyObject *value)
{
    int textmode;

    textmode = PyInt_AsLong(value) != 0;
    if (PyErr_Occurred())
        return -1;

    gpgme_set_textmode(self->ctx, textmode);
    return 0;
}

static PyObject *
pygpgme_context_get_include_certs(PyGpgmeContext *self)
{
    return PyInt_FromLong(gpgme_get_include_certs(self->ctx));
}

static int
pygpgme_context_set_include_certs(PyGpgmeContext *self, PyObject *value)
{
    int nr_of_certs;

    nr_of_certs = PyInt_AsLong(value);
    if (PyErr_Occurred())
        return -1;

    gpgme_set_include_certs(self->ctx, nr_of_certs);
    return 0;
}

static PyObject *
pygpgme_context_get_keylist_mode(PyGpgmeContext *self)
{
    return PyInt_FromLong(gpgme_get_keylist_mode(self->ctx));
}

static int
pygpgme_context_set_keylist_mode(PyGpgmeContext *self, PyObject *value)
{
    gpgme_keylist_mode_t keylist_mode;

    keylist_mode = PyInt_AsLong(value);
    if (PyErr_Occurred())
        return -1;

    if (pygpgme_check_error(gpgme_set_keylist_mode(self->ctx, keylist_mode)))
        return -1;

    return 0;
}

/* XXX: passphrase_cb */
/* XXX: progress_cb */

static PyGetSetDef pygpgme_context_getsets[] = {
    { "protocol", (getter)pygpgme_context_get_protocol,
      (setter)pygpgme_context_set_protocol },
    { "armor", (getter)pygpgme_context_get_armor,
      (setter)pygpgme_context_set_armor },
    { "textmode", (getter)pygpgme_context_get_textmode,
      (setter)pygpgme_context_set_textmode },
    { "include_certs", (getter)pygpgme_context_get_include_certs,
      (setter)pygpgme_context_set_include_certs },
    { "keylist_mode", (getter)pygpgme_context_get_keylist_mode,
      (setter)pygpgme_context_set_keylist_mode },
    { NULL, (getter)0, (setter)0 }
};

/* XXX: set_locale */
static PyObject *
pygpgme_context_set_locale(PyGpgmeContext *self, PyObject *args)
{
    int category;
    const char *value;

    if (!PyArg_ParseTuple(args, "iz", &category, &value))
        return NULL;

    if (pygpgme_check_error(gpgme_set_locale(self->ctx, category, value)))
        return NULL;

    Py_RETURN_NONE;
}

/* the following don't seem to be used */
/* XXX: signers_clear */
/* XXX: signers_add */
/* XXX: signers_enum */

static PyObject *
pygpgme_context_get_key(PyGpgmeContext *self, PyObject *args)
{
    const char *fpr;
    int secret = 0;
    gpgme_error_t err;
    gpgme_key_t key;
    PyObject *ret;

    if (!PyArg_ParseTuple(args, "s|i", &fpr, &secret))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    err = gpgme_get_key(self->ctx, fpr, &key, secret);
    Py_END_ALLOW_THREADS;

    if (pygpgme_check_error(err))
        return NULL;

    ret = pygpgme_key_new(key);
    gpgme_key_unref(key);
    return ret;
}

/* XXX: cancel -- not needed unless we wrap the async calls */

/* annotate exception with encrypt_result data */
static void
decode_encrypt_result(PyGpgmeContext *self)
{
    PyObject *err_type, *err_value, *err_traceback;
    gpgme_encrypt_result_t res;
    gpgme_invalid_key_t key;
    PyObject *list;

    PyErr_Fetch(&err_type, &err_value, &err_traceback);

    if (!PyErr_GivenExceptionMatches(err_type, pygpgme_error))
        goto end;

    list = PyList_New(0);
    res = gpgme_op_encrypt_result(self->ctx);
    if (res == NULL)
        goto end;

    for (key = res->invalid_recipients; key != NULL; key = key->next) {
        PyObject *item, *err;

        err = pygpgme_error_object(key->reason);
        item = Py_BuildValue("(zN)", key->fpr, err);
        PyList_Append(list, item);
        Py_DECREF(item);
    }

    PyObject_SetAttrString(err_value, "invalid_recipients", list);
    Py_DECREF(list);

 end:
    PyErr_Restore(err_type, err_value, err_traceback);
}

static PyObject *
pygpgme_context_encrypt(PyGpgmeContext *self, PyObject *args)
{
    PyObject *py_recp, *py_plain, *py_cipher;
    int flags, i, length;
    gpgme_key_t *recp;
    gpgme_data_t plain, cipher;
    gpgme_error_t err;

    if (!PyArg_ParseTuple(args, "OiOO", &py_recp, &flags,
                          &py_plain, &py_cipher))
        return NULL;

    py_recp = PySequence_Fast(py_recp, "first argument must be a sequence");
    if (py_recp == NULL)
        return NULL;

    length = PySequence_Fast_GET_SIZE(py_recp);
    recp = malloc((length + 1) * sizeof (gpgme_key_t));
    for (i = 0; i < length; i++) {
        PyObject *item = PySequence_Fast_GET_ITEM(py_recp, i);

        if (!PyObject_TypeCheck(item, &PyGpgmeKey_Type)) {
            free(recp);
            Py_DECREF(py_recp);
            PyErr_SetString(PyExc_TypeError, "items in first argument must "
                            "be gpgme.Key objects");
            return NULL;
        }
        recp[i] = ((PyGpgmeKey *)item)->key;
    }
    recp[i] = NULL;

    if (pygpgme_check_error(pygpgme_data_new(&plain, py_plain))) {
        free(recp);
        Py_DECREF(py_recp);
        return NULL;    
    }
    if (pygpgme_check_error(pygpgme_data_new(&cipher, py_cipher))) {
        free(recp);
        Py_DECREF(py_recp);
        gpgme_data_release(plain);
        return NULL;    
    }

    Py_BEGIN_ALLOW_THREADS;
    err = gpgme_op_encrypt(self->ctx, recp, flags, plain, cipher);
    Py_END_ALLOW_THREADS;

    free(recp);
    Py_DECREF(py_recp);
    gpgme_data_release(plain);
    gpgme_data_release(cipher);

    if (pygpgme_check_error(err)) {
        decode_encrypt_result(self);
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
pygpgme_context_encrypt_sign(PyGpgmeContext *self, PyObject *args)
{
    PyObject *py_recp, *py_plain, *py_cipher;
    int flags, i, length;
    gpgme_key_t *recp;
    gpgme_data_t plain, cipher;
    gpgme_error_t err;

    if (!PyArg_ParseTuple(args, "OiOO", &py_recp, &flags,
                          &py_plain, &py_cipher))
        return NULL;

    py_recp = PySequence_Fast(py_recp, "first argument must be a sequence");
    if (py_recp == NULL)
        return NULL;

    length = PySequence_Fast_GET_SIZE(py_recp);
    recp = malloc((length + 1) * sizeof (gpgme_key_t));
    for (i = 0; i < length; i++) {
        PyObject *item = PySequence_Fast_GET_ITEM(py_recp, i);

        if (!PyObject_TypeCheck(item, &PyGpgmeKey_Type)) {
            free(recp);
            Py_DECREF(py_recp);
            PyErr_SetString(PyExc_TypeError, "items in first argument must "
                            "be gpgme.Key objects");
            return NULL;
        }
        recp[i] = ((PyGpgmeKey *)item)->key;
    }
    recp[i] = NULL;

    if (pygpgme_check_error(pygpgme_data_new(&plain, py_plain))) {
        free(recp);
        Py_DECREF(py_recp);
        return NULL;    
    }
    if (pygpgme_check_error(pygpgme_data_new(&cipher, py_cipher))) {
        free(recp);
        Py_DECREF(py_recp);
        gpgme_data_release(plain);
        return NULL;    
    }

    Py_BEGIN_ALLOW_THREADS;
    err = gpgme_op_encrypt_sign(self->ctx, recp, flags, plain, cipher);
    Py_END_ALLOW_THREADS;

    free(recp);
    Py_DECREF(py_recp);
    gpgme_data_release(plain);
    gpgme_data_release(cipher);

    if (pygpgme_check_error(err)) {
        decode_encrypt_result(self);
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyMethodDef pygpgme_context_methods[] = {
    { "set_locale", (PyCFunction)pygpgme_context_set_locale, METH_VARARGS },
    { "get_key", (PyCFunction)pygpgme_context_get_key, METH_VARARGS },
    { "encrypt", (PyCFunction)pygpgme_context_encrypt, METH_VARARGS },
    { "encrypt_sign", (PyCFunction)pygpgme_context_encrypt_sign, METH_VARARGS },
    { NULL, 0, 0 }
};

PyTypeObject PyGpgmeContext_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "gpgme.Context",
    sizeof(PyGpgmeContext),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)pygpgme_context_dealloc,
    .tp_init = (initproc)pygpgme_context_init,
    .tp_getset = pygpgme_context_getsets,
    .tp_methods = pygpgme_context_methods,
};
