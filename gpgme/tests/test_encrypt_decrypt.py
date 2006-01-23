import unittest
import StringIO
from textwrap import dedent

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class EncryptDecryptTestCase(GpgHomeTestCase):

    import_keys = ['key1.pub', 'key1.sec', 'key2.pub', 'key2.sec',
                   'signonly.pub', 'signonly.sec']

    def test_decrypt(self):
        ciphertext = StringIO.StringIO(dedent('''
            -----BEGIN PGP MESSAGE-----
            Version: GnuPG v1.4.1 (GNU/Linux)

            hQIMA6lSIdANy91kARAAtXjViihD6DVBAYGdVs0a2sMPGRXjIR5Tw1ONKx4MtKn+
            pydR+/+0rBRGaQXe/mMODA8gqADQjz7PcTMWBa7ja969+K2nw7j5DUIMatonQVMf
            vpc7ze5hovZ1jXYAgmmXdUzDmk8ZkpHaEc5mMMAHYKFn+mm37AFY5JUjg2Ae9k3H
            29t+pW+n9ncn/QBImW3oVslZ8Fza1xOIWZTUrmvtU0vELdlIxy+d945bvD9EhmTK
            zRrD5m8V1etWINO2tE1Xhd4lV1KxncHzWafXLB5BKprztTqFUXNPAfnucYIczDon
            5VvkOz3WAtl/93o85hUKhbgGK0dvU3m+bj620ZUE5oDpPB4l1CiO5RqUFYtyN4LF
            OSAceVOh7X764VLtpAzuOpNfTYgvzIFJqrFOZHlf3XJRdGdpJuxMe8BwhdLyDscx
            pap4QxajOUSUAeS45x6ERA7xHO0QOwXZNzoxiOt9KRaoIhEacu70A9xRcGNJfZE2
            3z/AEMKr2CK6ny9/S8UQEhNvn1/gYfSXakFjWjM6PUXJSnz8WGjpFKKITpex3WBz
            m/X8bKgG3fT92zqJdYocrl4wgz4Dt3+KirnGG4gITxaEYpTT0y6l6NAO60De0oRh
            yqk+ulj2pvAlA82Ph0vTGZ9uOQGbrfN+NhwsG0HMNq+vmjShS1jJbSfoEt1AAIPS
            RwGMq7SDk/V7nhKqZcxeIWdtRIgFvEf0KljthkOZxT/GozNjQclak7Kt69K2qyvt
            XUfidNAhuOmeZ4pF31qSFiAnKRbzjzHp
            =1s5N
            -----END PGP MESSAGE-----
            '''))
        plaintext = StringIO.StringIO()
        ctx = gpgme.Context()
        sigs = ctx.decrypt(ciphertext, plaintext)

        self.assertEqual(plaintext.getvalue(), 'hello world\n')

    def test_decrypt_verify(self):
        ciphertext = StringIO.StringIO(dedent('''
            -----BEGIN PGP MESSAGE-----
            Version: GnuPG v1.4.1 (GNU/Linux)

            hQIMA6lSIdANy91kAQ/9GGQxL/OWvxrTchSMmIhsvwONJNlFE5cMIC0xejY4eN+t
            HtTg8V1fWXLRw7WY6FNFLeoR2hzqaZWw15lU55TmSJfJmK2mdUZu/IhSpCUFMEFW
            ZQpxslKq7N+S8NZHgq1WG32Ar1auOEflBQUMhj7sRSAtkvU7fWrTwf4Q4mcIV68P
            LiAAQoKxXNNVam9+EV/b3kx3bnJPKTn+ArpJf5Im+5XOGOeu9Ll0QUTbicgFhfpR
            esR6dKI/Ji5FGIu01kYNrDjDeMcJuzI52kNNoT+GJ72R+Gp4bZk2ycd+eVo3eeUW
            klO8K+7E5bd5ni+1H+ZWbVp9bn7Q++mFP6Mruv+v9Di5mvFXxMoFuB/8NzcilFVt
            h5VOexW1OaZk2bMp9bXVja/N7Y1oAADhINk0feaKkwYVOBJU9kJtL2O1WQui85Q3
            2dsL0YRJiR6mXesTezglZO44gsVAvCH8RUCtBnfEazfBg4jhcCHy6ooDgd0M4vcw
            xG4U7IyDU5xyLi9QrTaSg5LzzwNFqb5k/lTemZw3ob3uwZinWewASLwn5N5OPVRs
            gFT0eL0TfvDzHURsM/7QDvq9HX6JS7buyOlr5cZAsdSvm0FyE6YOkSvZR2jwp3vV
            jfs7RHjq9V7jzPVVKHnWEDoJfchkT/3KyMRCIM/ukBk9MwTZTIJRhjTA2Xd4kWTS
            kQEaU/OjumXPtw/T1pUH23nAkVssHsj8qgtxkFSmG/wrwNmfYx4tDhvgsHMJhar9
            hqQKBMsGmLD6RNWKhF/LryNBKI2IRgJabKKYbbOsydom/hw8ZF4aWaZTcCBMoBB2
            nhOi8WEIeWp93FGfHBa60nSBNGwgt24NmoFaXMjnCrJY/yK0L0MAajUC150OhtvG
            OSk=
            =fl3U
            -----END PGP MESSAGE-----
            '''))
        plaintext = StringIO.StringIO()
        ctx = gpgme.Context()
        sigs = ctx.decrypt_verify(ciphertext, plaintext)

        self.assertEqual(plaintext.getvalue(), 'hello world\n')


def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
