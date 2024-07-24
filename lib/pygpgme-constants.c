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

PyObject *PyGpgmeDataEncoding_Type;
PyObject *PyGpgmePubkeyAlgo_Type;
PyObject *PyGpgmeHashAlgo_Type;
PyObject *PyGpgmeSigMode_Type;
PyObject *PyGpgmeValidity_Type;
PyObject *PyGpgmeProtocol_Type;
PyObject *PyGpgmeKeylistMode_Type;
PyObject *PyGpgmePinentryMode_Type;
PyObject *PyGpgmeExportMode_Type;
PyObject *PyGpgmeStatus_Type;
PyObject *PyGpgmeEncryptFlags_Type;
PyObject *PyGpgmeSigsum_Type;
PyObject *PyGpgmeImport_Type;
PyObject *PyGpgmeErrSource_Type;
PyObject *PyGpgmeErrCode_Type;

static void
add_enum_value(PyObject *dict, const char *key, long value)
{
    PyObject *py_value = PyLong_FromLong(value);

    PyDict_SetItemString(dict, key, py_value);
    Py_DECREF(py_value);
}

static PyObject *
make_enum(PyObject *mod, const char *base_name, const char *name, PyObject *values)
{
    PyObject *enum_module, *base_class, *enum_name, *module_name, *kwnames;
    PyObject *args[4] = { NULL, };
    PyObject *enum_class;

    base_class = PyUnicode_FromString(base_name);
    enum_name = PyUnicode_FromString(name);
    module_name = PyUnicode_FromString("gpgme");
    kwnames = Py_BuildValue("(s)", "module");

    enum_module = PyImport_ImportModule("enum");
    args[0] = enum_module;
    args[1] = enum_name;
    args[2] = values;
    args[3] = module_name;

    enum_class = PyObject_VectorcallMethod(base_class, args, 3 + PY_VECTORCALL_ARGUMENTS_OFFSET, kwnames);

    Py_DECREF(enum_module);
    Py_DECREF(kwnames);
    Py_DECREF(module_name);
    Py_DECREF(enum_name);
    Py_DECREF(base_class);

    PyModule_AddObject(mod, name, enum_class);
    return enum_class;
}

void
pygpgme_add_constants (PyObject *mod)
{
    PyObject *values;

    /* gpgme_data_encoding_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_DATA_ENCODING_##name)
    CONST(NONE);
    CONST(BINARY);
    CONST(BASE64);
    CONST(ARMOR);
    PyGpgmeDataEncoding_Type = make_enum(mod, "IntEnum", "DataEncoding", values);
    Py_DECREF(values);

    /* gpgme_pubkey_algo_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_PK_##name)
    CONST(RSA);
    CONST(RSA_E);
    CONST(RSA_S);
    CONST(ELG_E);
    CONST(DSA);
    CONST(ELG);
    CONST(ECDSA);
    CONST(ECDH);
    CONST(EDDSA);
    PyGpgmePubkeyAlgo_Type = make_enum(mod, "IntEnum", "PubkeyAlgo", values);
    Py_DECREF(values);

    /* gpgme_hash_algo_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_MD_##name)
    CONST(NONE);
    CONST(MD5);
    CONST(SHA1);
    CONST(RMD160);
    CONST(MD2);
    CONST(TIGER);
    CONST(HAVAL);
    CONST(SHA256);
    CONST(SHA384);
    CONST(SHA512);
    CONST(MD4);
    CONST(CRC32);
    CONST(CRC32_RFC1510);
    CONST(CRC24_RFC2440);
    PyGpgmeHashAlgo_Type = make_enum(mod, "IntEnum", "HashAlgo", values);
    Py_DECREF(values);

    /* gpgme_sig_mode_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_SIG_MODE_##name)
    CONST(NORMAL);
    CONST(DETACH);
    CONST(CLEAR);
    PyGpgmeSigMode_Type = make_enum(mod, "IntEnum", "SigMode", values);
    Py_DECREF(values);

    /* gpgme_validity_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_VALIDITY_##name)
    CONST(UNKNOWN);
    CONST(UNDEFINED);
    CONST(NEVER);
    CONST(MARGINAL);
    CONST(FULL);
    CONST(ULTIMATE);
    PyGpgmeValidity_Type = make_enum(mod, "IntEnum", "Validity", values);
    Py_DECREF(values);

    /* gpgme_protocol_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_PROTOCOL_##name)
    CONST(OpenPGP);
    CONST(CMS);
    CONST(GPGCONF);
    CONST(ASSUAN);
    CONST(G13);
    CONST(UISERVER);
    CONST(SPAWN);
    CONST(DEFAULT);
    CONST(UNKNOWN);
    PyGpgmeProtocol_Type = make_enum(mod, "IntEnum", "Protocol", values);
    Py_DECREF(values);

    /* gpgme_keylist_mode_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_KEYLIST_MODE_##name)
    CONST(LOCAL);
    CONST(EXTERN);
    CONST(SIGS);
    CONST(SIG_NOTATIONS);
    CONST(WITH_SECRET);
    CONST(WITH_TOFU);
    CONST(WITH_KEYGRIP);
    CONST(EPHEMERAL);
    CONST(VALIDATE);
    CONST(FORCE_EXTERN);
    CONST(LOCATE);
    CONST(LOCATE_EXTERNAL);
    PyGpgmeKeylistMode_Type = make_enum(mod, "IntFlag", "KeylistMode", values);
    Py_DECREF(values);

    /* gpgme_pinentry_mode_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_PINENTRY_MODE_##name)
    CONST(DEFAULT);
    CONST(ASK);
    CONST(CANCEL);
    CONST(ERROR);
    CONST(LOOPBACK);
    PyGpgmePinentryMode_Type = make_enum(mod, "IntEnum", "PinentryMode", values);
    Py_DECREF(values);

    /* gpgme_export_mode_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_EXPORT_MODE_##name)
    CONST(EXTERN);
    CONST(MINIMAL);
    CONST(SECRET);
    CONST(RAW);
    CONST(PKCS12);
    CONST(SSH);
    CONST(SECRET_SUBKEY);
    PyGpgmeExportMode_Type = make_enum(mod, "IntFlag", "ExportMode", values);
    Py_DECREF(values);

    /* gpgme_status_code_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_STATUS_##name)
    CONST(EOF);
    CONST(ENTER);
    CONST(LEAVE);
    CONST(ABORT);
    CONST(GOODSIG);
    CONST(BADSIG);
    CONST(ERRSIG);
    CONST(BADARMOR);
    CONST(RSA_OR_IDEA);
    CONST(KEYEXPIRED);
    CONST(KEYREVOKED);
    CONST(TRUST_UNDEFINED);
    CONST(TRUST_NEVER);
    CONST(TRUST_MARGINAL);
    CONST(TRUST_FULLY);
    CONST(TRUST_ULTIMATE);
    CONST(SHM_INFO);
    CONST(SHM_GET);
    CONST(SHM_GET_BOOL);
    CONST(SHM_GET_HIDDEN);
    CONST(NEED_PASSPHRASE);
    CONST(VALIDSIG);
    CONST(SIG_ID);
    CONST(ENC_TO);
    CONST(NODATA);
    CONST(BAD_PASSPHRASE);
    CONST(NO_PUBKEY);
    CONST(NO_SECKEY);
    CONST(NEED_PASSPHRASE_SYM);
    CONST(DECRYPTION_FAILED);
    CONST(DECRYPTION_OKAY);
    CONST(MISSING_PASSPHRASE);
    CONST(GOOD_PASSPHRASE);
    CONST(GOODMDC);
    CONST(BADMDC);
    CONST(ERRMDC);
    CONST(IMPORTED);
    CONST(IMPORT_OK);
    CONST(IMPORT_PROBLEM);
    CONST(IMPORT_RES);
    CONST(FILE_START);
    CONST(FILE_DONE);
    CONST(FILE_ERROR);
    CONST(BEGIN_DECRYPTION);
    CONST(END_DECRYPTION);
    CONST(BEGIN_ENCRYPTION);
    CONST(END_ENCRYPTION);
    CONST(DELETE_PROBLEM);
    CONST(GET_BOOL);
    CONST(GET_LINE);
    CONST(GET_HIDDEN);
    CONST(GOT_IT);
    CONST(PROGRESS);
    CONST(SIG_CREATED);
    CONST(SESSION_KEY);
    CONST(NOTATION_NAME);
    CONST(NOTATION_DATA);
    CONST(POLICY_URL);
    CONST(BEGIN_STREAM);
    CONST(END_STREAM);
    CONST(KEY_CREATED);
    CONST(USERID_HINT);
    CONST(UNEXPECTED);
    CONST(INV_RECP);
    CONST(NO_RECP);
    CONST(ALREADY_SIGNED);
    CONST(SIGEXPIRED);
    CONST(EXPSIG);
    CONST(EXPKEYSIG);
    CONST(TRUNCATED);
    CONST(ERROR);
    CONST(NEWSIG);
    CONST(REVKEYSIG);
    CONST(SIG_SUBPACKET);
    CONST(NEED_PASSPHRASE_PIN);
    CONST(SC_OP_FAILURE);
    CONST(SC_OP_SUCCESS);
    CONST(CARDCTRL);
    CONST(BACKUP_KEY_CREATED);
    CONST(PKA_TRUST_BAD);
    CONST(PKA_TRUST_GOOD);
    CONST(PLAINTEXT);
    CONST(INV_SGNR);
    CONST(NO_SGNR);
    CONST(SUCCESS);
    CONST(DECRYPTION_INFO);
    CONST(PLAINTEXT_LENGTH);
    CONST(MOUNTPOINT);
    CONST(PINENTRY_LAUNCHED);
    CONST(ATTRIBUTE);
    CONST(BEGIN_SIGNING);
    CONST(KEY_NOT_CREATED);
    CONST(INQUIRE_MAXLEN);
    CONST(FAILURE);
    CONST(KEY_CONSIDERED);
    CONST(TOFU_USER);
    CONST(TOFU_STATS);
    CONST(TOFU_STATS_LONG);
    CONST(NOTATION_FLAGS);
    CONST(DECRYPTION_COMPLIANCE_MODE);
    CONST(VERIFICATION_COMPLIANCE_MODE);
    CONST(CANCELED_BY_USER);
    PyGpgmeStatus_Type = make_enum(mod, "IntEnum", "Status", values);
    Py_DECREF(values);

    /* gpgme_encrypt_flags_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_ENCRYPT_##name)
    CONST(ALWAYS_TRUST);
    CONST(NO_ENCRYPT_TO);
    CONST(PREPARE);
    CONST(EXPECT_SIGN);
    CONST(NO_COMPRESS);
    CONST(SYMMETRIC);
    CONST(THROW_KEYIDS);
    CONST(WRAP);
    CONST(WANT_ADDRESS);
    PyGpgmeEncryptFlags_Type = make_enum(mod, "IntFlag", "EncryptFlags", values);
    Py_DECREF(values);

    /* gpgme_sigsum_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_SIGSUM_##name)
    CONST(VALID);
    CONST(GREEN);
    CONST(RED);
    CONST(KEY_REVOKED);
    CONST(KEY_EXPIRED);
    CONST(SIG_EXPIRED);
    CONST(KEY_MISSING);
    CONST(CRL_MISSING);
    CONST(CRL_TOO_OLD);
    CONST(BAD_POLICY);
    CONST(SYS_ERROR);
    CONST(TOFU_CONFLICT);
    PyGpgmeSigsum_Type = make_enum(mod, "IntFlag", "Sigsum", values);
    Py_DECREF(values);

    /* import status */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPGME_IMPORT_##name)
    CONST(NEW);
    CONST(UID);
    CONST(SIG);
    CONST(SUBKEY);
    CONST(SECRET);
    PyGpgmeImport_Type = make_enum(mod, "IntFlag", "Import", values);
    Py_DECREF(values);

    /* gpg_err_source_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPG_ERR_SOURCE_##name)
    CONST(UNKNOWN);
    CONST(GCRYPT);
    CONST(GPG);
    CONST(GPGSM);
    CONST(GPGAGENT);
    CONST(PINENTRY);
    CONST(SCD);
    CONST(GPGME);
    CONST(KEYBOX);
    CONST(KSBA);
    CONST(DIRMNGR);
    CONST(GSTI);
    CONST(GPA);
    CONST(KLEO);
    CONST(ASSUAN);
    CONST(TPM2D);
    CONST(TLS);
    CONST(TKD);
    CONST(ANY);
    CONST(USER_1);
    CONST(USER_2);
    CONST(USER_3);
    CONST(USER_4);
    PyGpgmeErrSource_Type = make_enum(mod, "IntEnum", "ErrSource", values);
    Py_DECREF(values);

    /* gpg_err_code_t */
    values = PyDict_New();
#undef CONST
#define CONST(name) add_enum_value(values, #name, GPG_ERR_##name)
    CONST(NO_ERROR);
    CONST(GENERAL);
    CONST(UNKNOWN_PACKET);
    CONST(UNKNOWN_VERSION);
    CONST(PUBKEY_ALGO);
    CONST(DIGEST_ALGO);
    CONST(BAD_PUBKEY);
    CONST(BAD_SECKEY);
    CONST(BAD_SIGNATURE);
    CONST(NO_PUBKEY);
    CONST(CHECKSUM);
    CONST(BAD_PASSPHRASE);
    CONST(CIPHER_ALGO);
    CONST(KEYRING_OPEN);
    CONST(INV_PACKET);
    CONST(INV_ARMOR);
    CONST(NO_USER_ID);
    CONST(NO_SECKEY);
    CONST(WRONG_SECKEY);
    CONST(BAD_KEY);
    CONST(COMPR_ALGO);
    CONST(NO_PRIME);
    CONST(NO_ENCODING_METHOD);
    CONST(NO_ENCRYPTION_SCHEME);
    CONST(NO_SIGNATURE_SCHEME);
    CONST(INV_ATTR);
    CONST(NO_VALUE);
    CONST(NOT_FOUND);
    CONST(VALUE_NOT_FOUND);
    CONST(SYNTAX);
    CONST(BAD_MPI);
    CONST(INV_PASSPHRASE);
    CONST(SIG_CLASS);
    CONST(RESOURCE_LIMIT);
    CONST(INV_KEYRING);
    CONST(TRUSTDB);
    CONST(BAD_CERT);
    CONST(INV_USER_ID);
    CONST(UNEXPECTED);
    CONST(TIME_CONFLICT);
    CONST(KEYSERVER);
    CONST(WRONG_PUBKEY_ALGO);
    CONST(TRIBUTE_TO_D_A);
    CONST(WEAK_KEY);
    CONST(INV_KEYLEN);
    CONST(INV_ARG);
    CONST(BAD_URI);
    CONST(INV_URI);
    CONST(NETWORK);
    CONST(UNKNOWN_HOST);
    CONST(SELFTEST_FAILED);
    CONST(NOT_ENCRYPTED);
    CONST(NOT_PROCESSED);
    CONST(UNUSABLE_PUBKEY);
    CONST(UNUSABLE_SECKEY);
    CONST(INV_VALUE);
    CONST(BAD_CERT_CHAIN);
    CONST(MISSING_CERT);
    CONST(NO_DATA);
    CONST(BUG);
    CONST(NOT_SUPPORTED);
    CONST(INV_OP);
    CONST(TIMEOUT);
    CONST(INTERNAL);
    CONST(EOF_GCRYPT);
    CONST(INV_OBJ);
    CONST(TOO_SHORT);
    CONST(TOO_LARGE);
    CONST(NO_OBJ);
    CONST(NOT_IMPLEMENTED);
    CONST(CONFLICT);
    CONST(INV_CIPHER_MODE);
    CONST(INV_FLAG);
    CONST(INV_HANDLE);
    CONST(TRUNCATED);
    CONST(INCOMPLETE_LINE);
    CONST(INV_RESPONSE);
    CONST(NO_AGENT);
    CONST(AGENT);
    CONST(INV_DATA);
    CONST(ASSUAN_SERVER_FAULT);
    CONST(ASSUAN);
    CONST(INV_SESSION_KEY);
    CONST(INV_SEXP);
    CONST(UNSUPPORTED_ALGORITHM);
    CONST(NO_PIN_ENTRY);
    CONST(PIN_ENTRY);
    CONST(BAD_PIN);
    CONST(INV_NAME);
    CONST(BAD_DATA);
    CONST(INV_PARAMETER);
    CONST(WRONG_CARD);
    CONST(NO_DIRMNGR);
    CONST(DIRMNGR);
    CONST(CERT_REVOKED);
    CONST(NO_CRL_KNOWN);
    CONST(CRL_TOO_OLD);
    CONST(LINE_TOO_LONG);
    CONST(NOT_TRUSTED);
    CONST(CANCELED);
    CONST(BAD_CA_CERT);
    CONST(CERT_EXPIRED);
    CONST(CERT_TOO_YOUNG);
    CONST(UNSUPPORTED_CERT);
    CONST(UNKNOWN_SEXP);
    CONST(UNSUPPORTED_PROTECTION);
    CONST(CORRUPTED_PROTECTION);
    CONST(AMBIGUOUS_NAME);
    CONST(CARD);
    CONST(CARD_RESET);
    CONST(CARD_REMOVED);
    CONST(INV_CARD);
    CONST(CARD_NOT_PRESENT);
    CONST(NO_PKCS15_APP);
    CONST(NOT_CONFIRMED);
    CONST(CONFIGURATION);
    CONST(NO_POLICY_MATCH);
    CONST(INV_INDEX);
    CONST(INV_ID);
    CONST(NO_SCDAEMON);
    CONST(SCDAEMON);
    CONST(UNSUPPORTED_PROTOCOL);
    CONST(BAD_PIN_METHOD);
    CONST(CARD_NOT_INITIALIZED);
    CONST(UNSUPPORTED_OPERATION);
    CONST(WRONG_KEY_USAGE);
    CONST(NOTHING_FOUND);
    CONST(WRONG_BLOB_TYPE);
    CONST(MISSING_VALUE);
    CONST(HARDWARE);
    CONST(PIN_BLOCKED);
    CONST(USE_CONDITIONS);
    CONST(PIN_NOT_SYNCED);
    CONST(INV_CRL);
    CONST(BAD_BER);
    CONST(INV_BER);
    CONST(ELEMENT_NOT_FOUND);
    CONST(IDENTIFIER_NOT_FOUND);
    CONST(INV_TAG);
    CONST(INV_LENGTH);
    CONST(INV_KEYINFO);
    CONST(UNEXPECTED_TAG);
    CONST(NOT_DER_ENCODED);
    CONST(NO_CMS_OBJ);
    CONST(INV_CMS_OBJ);
    CONST(UNKNOWN_CMS_OBJ);
    CONST(UNSUPPORTED_CMS_OBJ);
    CONST(UNSUPPORTED_ENCODING);
    CONST(UNSUPPORTED_CMS_VERSION);
    CONST(UNKNOWN_ALGORITHM);
    CONST(INV_ENGINE);
    CONST(PUBKEY_NOT_TRUSTED);
    CONST(DECRYPT_FAILED);
    CONST(KEY_EXPIRED);
    CONST(SIG_EXPIRED);
    CONST(ENCODING_PROBLEM);
    CONST(INV_STATE);
    CONST(DUP_VALUE);
    CONST(MISSING_ACTION);
    CONST(MODULE_NOT_FOUND);
    CONST(INV_OID_STRING);
    CONST(INV_TIME);
    CONST(INV_CRL_OBJ);
    CONST(UNSUPPORTED_CRL_VERSION);
    CONST(INV_CERT_OBJ);
    CONST(UNKNOWN_NAME);
    CONST(LOCALE_PROBLEM);
    CONST(NOT_LOCKED);
    CONST(PROTOCOL_VIOLATION);
    CONST(INV_MAC);
    CONST(INV_REQUEST);
    CONST(BUFFER_TOO_SHORT);
    CONST(SEXP_INV_LEN_SPEC);
    CONST(SEXP_STRING_TOO_LONG);
    CONST(SEXP_UNMATCHED_PAREN);
    CONST(SEXP_NOT_CANONICAL);
    CONST(SEXP_BAD_CHARACTER);
    CONST(SEXP_BAD_QUOTATION);
    CONST(SEXP_ZERO_PREFIX);
    CONST(SEXP_NESTED_DH);
    CONST(SEXP_UNMATCHED_DH);
    CONST(SEXP_UNEXPECTED_PUNC);
    CONST(SEXP_BAD_HEX_CHAR);
    CONST(SEXP_ODD_HEX_NUMBERS);
    CONST(SEXP_BAD_OCT_CHAR);
    CONST(USER_1);
    CONST(USER_2);
    CONST(USER_3);
    CONST(USER_4);
    CONST(USER_5);
    CONST(USER_6);
    CONST(USER_7);
    CONST(USER_8);
    CONST(USER_9);
    CONST(USER_10);
    CONST(USER_11);
    CONST(USER_12);
    CONST(USER_13);
    CONST(USER_14);
    CONST(USER_15);
    CONST(USER_16);
    CONST(UNKNOWN_ERRNO);
    CONST(EOF);
    CONST(E2BIG);
    CONST(EACCES);
    CONST(EADDRINUSE);
    CONST(EADDRNOTAVAIL);
    CONST(EADV);
    CONST(EAFNOSUPPORT);
    CONST(EAGAIN);
    CONST(EALREADY);
    CONST(EAUTH);
    CONST(EBACKGROUND);
    CONST(EBADE);
    CONST(EBADF);
    CONST(EBADFD);
    CONST(EBADMSG);
    CONST(EBADR);
    CONST(EBADRPC);
    CONST(EBADRQC);
    CONST(EBADSLT);
    CONST(EBFONT);
    CONST(EBUSY);
    CONST(ECANCELED);
    CONST(ECHILD);
    CONST(ECHRNG);
    CONST(ECOMM);
    CONST(ECONNABORTED);
    CONST(ECONNREFUSED);
    CONST(ECONNRESET);
    CONST(ED);
    CONST(EDEADLK);
    CONST(EDEADLOCK);
    CONST(EDESTADDRREQ);
    CONST(EDIED);
    CONST(EDOM);
    CONST(EDOTDOT);
    CONST(EDQUOT);
    CONST(EEXIST);
    CONST(EFAULT);
    CONST(EFBIG);
    CONST(EFTYPE);
    CONST(EGRATUITOUS);
    CONST(EGREGIOUS);
    CONST(EHOSTDOWN);
    CONST(EHOSTUNREACH);
    CONST(EIDRM);
    CONST(EIEIO);
    CONST(EILSEQ);
    CONST(EINPROGRESS);
    CONST(EINTR);
    CONST(EINVAL);
    CONST(EIO);
    CONST(EISCONN);
    CONST(EISDIR);
    CONST(EISNAM);
    CONST(EL2HLT);
    CONST(EL2NSYNC);
    CONST(EL3HLT);
    CONST(EL3RST);
    CONST(ELIBACC);
    CONST(ELIBBAD);
    CONST(ELIBEXEC);
    CONST(ELIBMAX);
    CONST(ELIBSCN);
    CONST(ELNRNG);
    CONST(ELOOP);
    CONST(EMEDIUMTYPE);
    CONST(EMFILE);
    CONST(EMLINK);
    CONST(EMSGSIZE);
    CONST(EMULTIHOP);
    CONST(ENAMETOOLONG);
    CONST(ENAVAIL);
    CONST(ENEEDAUTH);
    CONST(ENETDOWN);
    CONST(ENETRESET);
    CONST(ENETUNREACH);
    CONST(ENFILE);
    CONST(ENOANO);
    CONST(ENOBUFS);
    CONST(ENOCSI);
    CONST(ENODATA);
    CONST(ENODEV);
    CONST(ENOENT);
    CONST(ENOEXEC);
    CONST(ENOLCK);
    CONST(ENOLINK);
    CONST(ENOMEDIUM);
    CONST(ENOMEM);
    CONST(ENOMSG);
    CONST(ENONET);
    CONST(ENOPKG);
    CONST(ENOPROTOOPT);
    CONST(ENOSPC);
    CONST(ENOSR);
    CONST(ENOSTR);
    CONST(ENOSYS);
    CONST(ENOTBLK);
    CONST(ENOTCONN);
    CONST(ENOTDIR);
    CONST(ENOTEMPTY);
    CONST(ENOTNAM);
    CONST(ENOTSOCK);
    CONST(ENOTSUP);
    CONST(ENOTTY);
    CONST(ENOTUNIQ);
    CONST(ENXIO);
    CONST(EOPNOTSUPP);
    CONST(EOVERFLOW);
    CONST(EPERM);
    CONST(EPFNOSUPPORT);
    CONST(EPIPE);
    CONST(EPROCLIM);
    CONST(EPROCUNAVAIL);
    CONST(EPROGMISMATCH);
    CONST(EPROGUNAVAIL);
    CONST(EPROTO);
    CONST(EPROTONOSUPPORT);
    CONST(EPROTOTYPE);
    CONST(ERANGE);
    CONST(EREMCHG);
    CONST(EREMOTE);
    CONST(EREMOTEIO);
    CONST(ERESTART);
    CONST(EROFS);
    CONST(ERPCMISMATCH);
    CONST(ESHUTDOWN);
    CONST(ESOCKTNOSUPPORT);
    CONST(ESPIPE);
    CONST(ESRCH);
    CONST(ESRMNT);
    CONST(ESTALE);
    CONST(ESTRPIPE);
    CONST(ETIME);
    CONST(ETIMEDOUT);
    CONST(ETOOMANYREFS);
    CONST(ETXTBSY);
    CONST(EUCLEAN);
    CONST(EUNATCH);
    CONST(EUSERS);
    CONST(EWOULDBLOCK);
    CONST(EXDEV);
    CONST(EXFULL);
    PyGpgmeErrCode_Type = make_enum(mod, "IntEnum", "ErrCode", values);
    Py_DECREF(values);
}
