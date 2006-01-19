import unittest

def test_suite():
    import gpgme.tests.test_context
    import gpgme.tests.test_keys
    import gpgme.tests.test_keylist
    import gpgme.tests.test_import
    suite = unittest.TestSuite()
    suite.addTest(gpgme.tests.test_context.test_suite())
    suite.addTest(gpgme.tests.test_keys.test_suite())
    suite.addTest(gpgme.tests.test_keylist.test_suite())
    suite.addTest(gpgme.tests.test_import.test_suite())
    return suite
