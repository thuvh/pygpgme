/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
    pygpgme - a Python wrapper for the gpgme library
    Copyright (C) 2006  James Henstridge

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "pygpgme.h"
#include <structmember.h>

static void
pygpgme_genkey_result_dealloc(PyGpgmeGenkeyResult *self)
{
    Py_XDECREF(self->primary);
    Py_XDECREF(self->sub);
    Py_XDECREF(self->fpr);
    PyObject_Del(self);
}

static const char pygpgme_genkey_result_primary_doc[] =
    "True if a primary key was generated.";

static const char pygpgme_genkey_result_sub_doc[] =
    "True if a subkey was generated.";

static const char pygpgme_genkey_result_fpr_doc[] =
    "String containing the fingerprint of the generated key.\n"
    "\n"
    "If both a primary and a subkey were generated then this is the\n"
    "fingerprint of the primary key. For crypto backends that do not\n"
    "provide key fingerprints this is ``None``.";

static PyMemberDef pygpgme_genkey_result_members[] = {
    { "primary", T_OBJECT, offsetof(PyGpgmeGenkeyResult, primary), READONLY,
      pygpgme_genkey_result_primary_doc },
    { "sub", T_OBJECT, offsetof(PyGpgmeGenkeyResult, sub), READONLY,
      pygpgme_genkey_result_sub_doc },
    { "fpr", T_OBJECT, offsetof(PyGpgmeGenkeyResult, fpr), READONLY,
      pygpgme_genkey_result_fpr_doc },
    { NULL, 0, 0, 0 },
};

static const char pygpgme_genkey_result_doc[] =
    "Key generation result.\n"
    "\n"
    "Instances of this class are usually obtained as the return value\n"
    "of :meth:`Context.genkey`.\n";

PyTypeObject PyGpgmeGenkeyResult_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "gpgme.GenkeyResult",
    sizeof(PyGpgmeGenkeyResult),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)pygpgme_genkey_result_dealloc,
    .tp_members = pygpgme_genkey_result_members,
    .tp_doc = pygpgme_genkey_result_doc,
};

PyObject *
pygpgme_genkey_result(gpgme_ctx_t ctx)
{
    gpgme_genkey_result_t result;
    PyGpgmeGenkeyResult *self;

    result = gpgme_op_genkey_result(ctx);

    if (result == NULL)
        Py_RETURN_NONE;

    self = PyObject_New(PyGpgmeGenkeyResult, &PyGpgmeGenkeyResult_Type);
    if (!self)
        return NULL;

    self->primary = PyBool_FromLong(result->primary);
    self->sub = PyBool_FromLong(result->sub);
    if (result->fpr)
        self->fpr = PyUnicode_DecodeASCII(result->fpr, strlen(result->fpr),
                                          "replace");
    else {
        Py_INCREF(Py_None);
        self->fpr = Py_None;
    }

    return (PyObject *) self;
}
