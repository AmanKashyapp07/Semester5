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

---

## How to Build & Run

### Quick Start (All Programs)

Run all three demos in sequence with one command:
```bash
make run
```

---

### Compiling Programs Individually

You can build all binaries or specific ones using `make`:

```bash
# Compile all programs (list, kem, sig)
make

# Or compile individual targets
make list
make kem
make sig
```

#### Manual Compilation with `g++` / `clang++`:
If you prefer running manual compiler commands:
```bash
# macOS (Apple Silicon / Homebrew)
g++ -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/opt/openssl@3/include list.cpp -o list -L/opt/homebrew/lib -L/opt/homebrew/opt/openssl@3/lib -loqs -lcrypto
g++ -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/opt/openssl@3/include kem.cpp -o kem -L/opt/homebrew/lib -L/opt/homebrew/opt/openssl@3/lib -loqs -lcrypto
g++ -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/opt/openssl@3/include sig.cpp -o sig -L/opt/homebrew/lib -L/opt/homebrew/opt/openssl@3/lib -loqs -lcrypto

# Linux (Ubuntu / Fedora with liboqs in /usr/local)
g++ -std=c++17 list.cpp -o list -loqs -lcrypto
g++ -std=c++17 kem.cpp -o kem -loqs -lcrypto
g++ -std=c++17 sig.cpp -o sig -loqs -lcrypto
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

# Run with a custom algorithm (e.g. ML-KEM-1024, FrodoKEM-640-AES, BIKE-L1)
./kem ML-KEM-1024
./kem FrodoKEM-640-AES
./kem Kyber512
```

#### 3. Run Digital Signature (SIG)
```bash
# Run with default algorithm (ML-DSA-65)
./sig

# Run with a custom algorithm and custom message
./sig Falcon-512 "Secret instructions for exam"
./sig ML-DSA-87 "Official transaction confirmation"
./sig SLH_DSA_SHAKE_128F
```

---

### Cleaning Up

To remove all compiled binaries and build artifacts:
```bash
make clean
```

---

## Environment & Installation Notes

If running on a new machine:

### macOS:
```bash
# 1. Install dependencies
brew install liboqs openssl@3 cmake git

# 2. Install liboqs-cpp header wrapper
git clone --depth=1 https://github.com/open-quantum-safe/liboqs-cpp /tmp/liboqs-cpp
cmake -S /tmp/liboqs-cpp -B /tmp/liboqs-cpp/build -DCMAKE_INSTALL_PREFIX=/opt/homebrew
cmake --build /tmp/liboqs-cpp/build --target install
```

### Ubuntu / Debian:
```bash
# 1. Dependencies
sudo apt update && sudo apt install -y git cmake gcc g++ libssl-dev

# 2. Build and install liboqs
git clone --depth=1 https://github.com/open-quantum-safe/liboqs
cmake -S liboqs -B liboqs/build -DBUILD_SHARED_LIBS=ON
cmake --build liboqs/build --parallel $(nproc)
sudo cmake --build liboqs/build --target install
sudo ldconfig

# 3. Install liboqs-cpp
git clone --depth=1 https://github.com/open-quantum-safe/liboqs-cpp
cmake -S liboqs-cpp -B liboqs-cpp/build
sudo cmake --build liboqs-cpp/build --target install
```
