/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#ifndef PYGPGME_H
#define PYGPGME_H

#include <Python.h>
#include <gpgme.h>

#define HIDDEN __attribute__((visibility("hidden")))

typedef struct {
    PyObject_HEAD
    gpgme_ctx_t ctx;
} PyGpgmeContext;

extern HIDDEN PyObject *pygpgme_error;
extern HIDDEN PyTypeObject PyGpgmeContext_Type;

HIDDEN int           pygpgme_check_error (gpgme_error_t err);
HIDDEN gpgme_error_t pygpgme_data_new    (gpgme_data_t *dh, PyObject *fp);

#endif
