// RSA Assignment for ECE4122/6122 Fall 2015

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

using namespace std;

// Implement the RSA_Algorithm methods here

// Constructor
RSA_Algorithm::RSA_Algorithm()
  : rng(gmp_randinit_default)
{
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
  rng.seed(drValue);
// No need to init n, d, or e.
}

// Helper function to generate prime
static mpz_class generatePrime(gmp_randclass& rng, mpz_class bits) {
  // Generate p as prime
  mpz_class p = rng.get_z_bits(bits);
  int is_prime = mpz_probab_prime_p(p.get_mpz_t(), 100);
  while (is_prime == 0) { // Loop until is prime or likely to be prime (1 or 2, not 0)
    p = rng.get_z_bits(bits);
    is_prime = mpz_probab_prime_p(p.get_mpz_t(), 100);
  }
  return p;
}


// Fill in the remainder of the RSA_Algorithm methods
void RSA_Algorithm::GenerateRandomKeyPair(size_t sz)
{
  // Get number of bits as mpz_class
  mpz_class bits(sz);

  // Generate p and q as primes
  mpz_class p = generatePrime(this->rng, sz);
  mpz_class q = generatePrime(this->rng, sz);
  while (p == q) { // Ensure p and q are different
    q = generatePrime(this->rng, sz);
  }

  // Set n and phi_n
  this->n = p * q;
  mpz_class phi_n = (p - 1) * (q - 1);

  // Choose d such that the GCD of d and phi_n is 1
  mpz_class gcd;
  this->d = rng.get_z_bits(bits);
  mpz_gcd(gcd.get_mpz_t(), phi_n.get_mpz_t(), this->d.get_mpz_t());
  while (gcd != 1) { // Keep looping until valid d generated
    this->d = rng.get_z_bits(bits);
    mpz_gcd(gcd.get_mpz_t(), phi_n.get_mpz_t(), this->d.get_mpz_t());
  }

  // Compute e
  mpz_invert(this->e.get_mpz_t(), this->d.get_mpz_t(), phi_n.get_mpz_t());
}

mpz_class RSA_Algorithm::Encrypt(mpz_class M)
{
  mpz_class cipher;
  mpz_powm(cipher.get_mpz_t(), M.get_mpz_t(), this->e.get_mpz_t(), this->n.get_mpz_t());;
  return cipher;
}

mpz_class RSA_Algorithm::Decrypt(mpz_class C)
{
  mpz_class plain;
  mpz_powm(plain.get_mpz_t(), C.get_mpz_t(), this->d.get_mpz_t(), this->n.get_mpz_t());;
  return plain;
}

void RSA_Algorithm::PrintND()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(mpz_class M)
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(mpz_class C)
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}




