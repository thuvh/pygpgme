PyGPGME API
###########

.. py:module:: gpgme


Context
=======

.. autoclass:: Context
   :members:
   :undoc-members:


GenkeyResult
============

.. autoclass:: GenkeyResult
   :members:
   :undoc-members:


Key
===

.. autoclass:: Key
   :members:
   :undoc-members:


NewSignature
============

.. autoclass:: NewSignature
   :members:
   :undoc-members:


Signature
=========

.. autoclass:: Signature
   :members:
   :undoc-members:


Helper Objects
==============

Stuff that's mostly used internally, but it's good to know it's there.

.. py:class:: KeyIter

    Iterable yielding :py:class:`Key` instances for keylist results.

.. py:data:: gpgme_version

    Version string of libgpgme used to build this module.

.. py:class:: GpgmeError
.. py:class:: ImportResult
.. py:class:: KeySig
.. py:class:: Subkey
.. py:class:: UserId



Constants
=========

Protocol Selection
------------------

The following constants can be used as value for :py:attr:`Context.protocol`.
They are also returned via :py:attr:`Key.protocol`.

.. autoclass:: Protocol(enum.IntEnum)

  .. autoattribute:: OpenPGP

    This specifies the OpenPGP protocol.

  .. autoattribute:: CMS

    This specifies the Cryptographic Message Syntax.

  .. autoattribute:: ASSUAN

     Under development. Please ask on  gnupg-devel@gnupg.org for help.

  .. autoattribute:: G13

     Under development. Please ask on gnupg-devel@gnupg.org for help.

  .. autoattribute:: UISERVER

     Under development. Please ask on gnupg-devel@gnupg.org for help.

  .. autoattribute:: SPAWN

     Special protocol for use with ``gpgme_op_spawn``.

  .. autoattribute:: UNKNOWN

     Reserved for future extension. You may use this to indicate that
     the used protocol is not known to the application.  Currently,
     GPGME does not accept this value in any operation, though, except
     for ``gpgme_get_protocol_name``.


Key Listing Mode
----------------

Bitwise OR combinations of the following constants can be used as values for
:py:attr:`Context.keylist_mode`.

.. autoclass:: KeylistMode(enum.IntFlag)

  .. autoattribute:: LOCAL

    Specifies that the local keyring should be searched. This is the default.

  .. autoattribute:: EXTERN

    Specifies that an external source should be searched. The type of external
    source is dependant on the crypto engine used and whether it is combined
    with :py:data:`LOCAL`. For example, it can be a remote keyserver or LDAP
    certificate server.

  .. autoattribute:: SIGS

    Specifies that the key signatures should be included in the listed keys.

  .. autoattribute:: SIG_NOTATIONS

    Specifies that the signature notations on key signatures should be
    included in the listed keys. This only works if :py:data:`SIGS` is
    also enabled.

  .. autoattribute:: WITH_SECRET

    Returns information about the presence of a corresponding secret
    key in a public key listing. A public key listing with this mode
    is slower than a standard listing but can be used instead of a
    second run to list the secret keys. This is only supported for
    GnuPG versions >= 2.1.

  .. autoattribute:: EPHEMERAL

    Specifies that keys flagged as ephemeral are included in the
    listing.

  .. autoattribute:: VALIDATE

    Specifies that the backend should do key or certificate validation
    and not just get the validity information from an internal
    cache. This might be an expensive operation and is in general not
    useful.  Currently only implemented for the S/MIME backend and
    ignored for other backends.


Encryption Flags
----------------

Bitwise OR combinations of the following constants can be used for the
``flags`` parameter of :py:meth:`Context.encrypt` and
:py:meth:`Context.encrypt_sign`.

.. autoclass:: EncryptFlags(enum.IntFlags)

  .. autoattribute:: ALWAYS_TRUST

  Specifies that all the recipients in recp should be trusted, even if
  the keys do not have a high enough validity in the keyring. This
  flag should be used with care; in general it is not a good idea to
  use any untrusted keys.

  .. autoattribute:: NO_ENCRYPT_TO

  Specifies that no default or hidden default recipients as configured
  in the crypto backend should be included. This can be useful for
  managing different user profiles.

  .. autoattribute:: PREPARE

  Used with the UI Server protocol to prepare an encryption.

  .. autoattribute:: EXPECT_SIGN

  Used with the UI Server protocol to advise the UI server to expect a
  sign command.

  .. autoattribute:: NO_COMPRESS

  Specifies that the plaintext shall not be compressed before it is
  encrypted. This is in some cases useful if the length of the
  encrypted message may reveal information about the plaintext.


Signing Modes
-------------

The following constants can be used for the ``mode`` parameter of
:py:meth:`Context.sign`.

.. autoclass:: SigMode(enum.IntEnum)

  .. autoattribute:: NORMAL

    A normal signature is made, the output includes the plaintext and the
    signature. :py:attr:`Context.armor` is respected.

  .. autoattribute:: DETACH

    A detached signature is created. :py:attr:`Context.armor` is respected.

  .. autoattribute:: CLEAR

    A cleartext signature is created. :py:attr:`Context.armor` is ignored.


Signature Verification
----------------------

The following bit masks can be used to extract individual bits from
:py:attr:`Signature.summary` using bitwise AND.

.. autoclass:: Sigsum(enum.IntFlag)

  .. autoattribute:: VALID

    The signature is fully valid.

  .. autoattribute:: GREEN

    The signature is good but one might want to display some extra information.
    Check the other bits.

  .. autoattribute:: RED

    The signature is bad. It might be useful to check other bits and display
    more information, i.e. a revoked certificate might not render a signature
    invalid when the message was received prior to the cause for the
    revocation.

  .. autoattribute:: KEY_REVOKED

    The key or at least one certificate has been revoked.

  .. autoattribute:: KEY_EXPIRED

    The key or one of the certificates has expired.

  .. autoattribute:: SIG_EXPIRED

    The signature has expired.

  .. autoattribute:: KEY_MISSING

    Can’t verify due to a missing key or certificate.

  .. autoattribute:: CRL_MISSING

    The certificate revocation list (or an equivalent mechanism) is not
    available.

  .. autoattribute:: CRL_TOO_OLD

    The available certificate revocation list is too old.

  .. autoattribute:: BAD_POLICY

    A policy requirement was not met.

  .. autoattribute:: SYS_ERROR

    A system error occured.
