import unittest

def test_suite():
    import gpgme.tests.test_context
    import gpgme.tests.test_keys
    suite = unittest.TestSuite()
    suite.addTest(gpgme.tests.test_context.test_suite())
    suite.addTest(gpgme.tests.test_keys.test_suite())
    return suite
