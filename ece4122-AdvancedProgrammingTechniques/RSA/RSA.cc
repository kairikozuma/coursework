// ECE4122/6122 RSA Encryption/Decryption assignment
// Fall Semester 2015

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cassert>
#include "RSA_Algorithm.h"

using namespace std;

#define KEY_PAIRS 100
#define MESSAGE_COUNT 100
#define SZ_MIN 32
#define SZ_MAX 1024

int main()
{
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // Loop from sz = 32 to 1024 inclusive
  // for each size choose 10 different key pairs
  // For each key pair choose 10 differnt plaintext
  // messages making sure it is smaller than n.
  // If not smaller then n then choose another
  // For eacm message encrypt it using the public key (n,e).
  // After encryption, decrypt the ciphertext using the private
  // key (n,d) and verify it matches the original message.

  // get a random seed for the random number generator
  int dr = open("/dev/random", O_RDONLY);
  if (dr < 0)
    {
      cout << "Can't open /dev/random, exiting" << endl;
      exit(0);
    }
  unsigned long drValue;
  read(dr, (char*)&drValue, sizeof(drValue));
  //cout << "drValue " << drValue << endl;
  gmp_randclass rng(gmp_randinit_default);
  rng.seed(drValue);

  // Iterate through different key sizes
  for (size_t sz = SZ_MIN; sz <= SZ_MAX; sz *= 2) {
    // 10 times per key size
    for (size_t it = 0; it < KEY_PAIRS; it++) {
      // Generate key pair
      RSA.GenerateRandomKeyPair(sz);
      RSA.PrintNDE();

      // Get bit length of n
      size_t bits = mpz_sizeinbase(RSA.n.get_mpz_t(), 2);
	
      // Set bit length of m to 1 less than that of n
      size_t m_bits = bits - 1;
	
      // Generate 10 messages per key pair
      for (size_t it_m = 0; it_m < MESSAGE_COUNT; it_m++) {
        mpz_class M = rng.get_z_bits(m_bits);
        mpz_class C = RSA.Encrypt(M);
        RSA.PrintM(M);
        RSA.PrintC(C);
        assert((M == RSA.Decrypt(C)) && "Decryption not working!");
      }
    }
  }
}

