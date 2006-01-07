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

typedef struct {
    PyObject_HEAD
    gpgme_key_t key;
} PyGpgmeKey;

typedef struct {
    PyObject_HEAD
    gpgme_subkey_t subkey;
    PyObject *parent;
} PyGpgmeSubkey;

typedef struct {
    PyObject_HEAD
    gpgme_user_id_t user_id;
    PyObject *parent;
} PyGpgmeUserId;

typedef struct {
    PyObject_HEAD
    gpgme_key_sig_t key_sig;
    PyObject *parent;
} PyGpgmeKeySig;

extern HIDDEN PyObject *pygpgme_error;
extern HIDDEN PyTypeObject PyGpgmeContext_Type;
extern HIDDEN PyTypeObject PyGpgmeKey_Type;
extern HIDDEN PyTypeObject PyGpgmeSubkey_Type;
extern HIDDEN PyTypeObject PyGpgmeUserId_Type;
extern HIDDEN PyTypeObject PyGpgmeKeySig_Type;

HIDDEN int           pygpgme_check_error  (gpgme_error_t err);
HIDDEN PyObject *    pygpgme_error_object (gpgme_error_t err);
HIDDEN gpgme_error_t pygpgme_data_new     (gpgme_data_t *dh, PyObject *fp);
HIDDEN PyObject *    pygpgme_key_new      (gpgme_key_t key);

#endif
