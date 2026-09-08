# Exploring Post-Quantum Cryptography (PQC) with liboqs

This repository contains C++ implementations demonstrating **Post-Quantum Cryptography (PQC)** using the **Open Quantum Safe (OQS)** library ([liboqs](https://github.com/open-quantum-safe/liboqs)) and its C++ wrapper ([liboqs-cpp](https://github.com/open-quantum-safe/liboqs-cpp)).

---

## Background & Concepts

### Why Post-Quantum Cryptography?
Classical public-key cryptographic algorithms (such as RSA, Diffie-Hellman, and Elliptic Curve Cryptography / ECC) rely on the mathematical hardness of integer factorization and discrete logarithms. Sufficiently large quantum computers running **Shor's Algorithm** can break these systems in polynomial time.

To secure communications against quantum threats, the National Institute of Standards and Technology (**NIST**) standardized quantum-safe algorithms:
1. **FIPS 203**: **ML-KEM** (Module-Lattice-Based Key-Encapsulation Mechanism, formerly Kyber).
2. **FIPS 204**: **ML-DSA** (Module-Lattice-Based Digital Signature Algorithm, formerly Dilithium).
3. **FIPS 205**: **SLH-DSA** (Stateless Hash-Based Digital Signature Algorithm, formerly SPHINCS+).
4. **Falcon**: Fast Fourier Lattice-based digital signature scheme.

---

## Repository Structure

```text
Cyber Security/
├── list.cpp           # Program 1: Discovers and prints all supported KEM & SIG algorithms
├── kem.cpp            # Program 2: Post-Quantum Key Encapsulation Mechanism (Key Exchange) demo
├── sig.cpp            # Program 3: Post-Quantum Digital Signature generation & verification demo
├── kem_compare.cpp    # Program 4: Multi-algorithm KEM benchmark & comparison tool
├── sig_compare.cpp    # Program 5: Multi-algorithm Digital Signature benchmark & comparison tool
├── Makefile           # Automated build and run configuration
├── compile_flags.txt  # Language server flags for clangd / IDE IntelliSense
├── .gitignore         # Ignores compiled binaries and temporary build files
└── README.md          # Comprehensive documentation and instructions
```

---

## Programs Overview

### 1. Algorithm Explorer (`list.cpp`)
- Queries the `liboqs` library for all supported and enabled cryptographic mechanisms.
- Displays a formatted table with:
  - **Algorithm Name & Status** (Enabled / Disabled)
  - **NIST Claimed Security Level** (Level 1 ≈ AES-128, Level 3 ≈ AES-192, Level 5 ≈ AES-256)
  - **Public Key Length** (in bytes)
  - **Ciphertext / Maximum Signature Length** (in bytes)
  - **Secret Key Length** (in bytes)

### 2. Key Encapsulation Mechanism (`kem.cpp`)
Demonstrates quantum-safe key exchange between **Alice** and **Bob** in 4 steps:
1. **Keypair Generation (Alice)**: Alice generates a Public Key ($pk$) and a Secret Key ($sk$).
2. **Encapsulation (Bob)**: Bob uses Alice's $pk$ to generate a 256-bit symmetric shared secret and encapsulates it into a ciphertext ($ct$).
3. **Decapsulation (Alice)**: Alice decrypts $ct$ using her private $sk$ to recover the exact same shared secret.
4. **Verification**: Checks that Alice's and Bob's keys match bit-for-bit.

*Default algorithm:* `ML-KEM-768` (NIST Security Level 3).

### 3. Digital Signatures (`sig.cpp`)
Demonstrates quantum-safe authenticity, integrity, and tamper detection in 5 steps:
1. **Keypair Generation (Signer)**: Signer creates a Public Verification Key and Private Signing Key.
2. **Message Definition**: Prepares a transaction message (`"Transfer $1,000 to Bob - Authorized by Alice"`).
3. **Message Signing**: Computes a digital signature using the Private Key.
4. **Verification**: Verifier checks the authentic message and signature using the Public Key ($\rightarrow$ `VALID`).
5. **Tamper Detection Test**: Simulates an attacker tampering with the message (`"$100,000"`) and verifies that the signature is rejected ($\rightarrow$ `REJECTED`).

*Default algorithm:* `ML-DSA-65` (NIST Security Level 3 / Dilithium3).

### 4. Multi-Algorithm KEM Benchmark (`kem_compare.cpp`)
Benchmarks and compares multiple KEM algorithm families side-by-side:
- **ML-KEM-512 / 768 / 1024** (Module Lattice)
- **Classic-McEliece-348864** (Code-Based Goppa Codes)
- **FrodoKEM-640-AES** (Unstructured Lattice)
- **BIKE-L1** (Code-Based Bit-Flipping)

Measures and displays:
- Public Key, Ciphertext, and Secret Key sizes
- KeyGen, Encapsulation, and Decapsulation latencies (in microseconds)

### 5. Multi-Algorithm Signature Benchmark (`sig_compare.cpp`)
Benchmarks and compares multiple Digital Signature algorithm families side-by-side:
- **ML-DSA-44 / 65 / 87** (Module Lattice / Dilithium)
- **Falcon-512 / 1024** (NTRU Fast Fourier Lattice)

Measures and displays:
- Public Key, Signature, and Secret Key sizes
- KeyGen, Signing, and Verification latencies (in microseconds)

---

## How to Build & Run

### Quick Start (All Programs)

Run all demos and benchmarks with one command:
```bash
make run
```

---

### Compiling Programs Individually

You can build all binaries or specific ones using `make`:

```bash
# Compile all programs
make

# Or compile individual targets
make list
make kem
make sig
make kem_compare
make sig_compare
```

---

### Running the Programs

#### 1. List Algorithms
```bash
./list
```

#### 2. Run Key Exchange (KEM)
```bash
# Run with default algorithm (ML-KEM-768)
./kem

# Run with a custom algorithm
./kem ML-KEM-1024
./kem FrodoKEM-640-AES
./kem Classic-McEliece-348864
```

#### 3. Run Digital Signature (SIG)
```bash
# Run with default algorithm (ML-DSA-65)
./sig

# Run with a custom algorithm and custom message
./sig Falcon-512 "Secret instructions for exam"
./sig ML-DSA-87 "Official transaction confirmation"
```

#### 4. Run KEM Comparison Benchmark
```bash
./kem_compare
```

#### 5. Run Signature Comparison Benchmark
```bash
./sig_compare
```

---

### Cleaning Up

To remove all compiled binaries and build artifacts:
```bash
make clean
```
