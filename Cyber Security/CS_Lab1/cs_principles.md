# Cybersecurity Principles Applied in CS Lab Assignment 1

This document explains the core **Cybersecurity Principles**, cryptographic concepts, and architectural design patterns applied in this assignment.

---

## Executive Summary

In this lab, we transformed a basic web application storing plain-text data into a **Zero-Knowledge, End-to-End Encrypted (E2EE)** system using the browser's native **Web Crypto API** (`window.crypto.subtle`).

---

## 1. Confidentiality (CIA Triad)

* **Principle:** Ensuring that sensitive information is accessible **only** to authorized users who possess the decryption key.
* **Implementation:** 
 * The user's personal message (e.g. `"aman kashyap"`) is encrypted using **AES-256-GCM** before leaving the client browser.
 * Stored in SQLite as hexadecimal ciphertext (e.g. `31427d750e4b...`).
* **Security Benefit:** Even if an attacker gains full access to the database ([`classmates.db`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/classmates.db)) or sniffs network HTTP traffic, they see only unreadable ciphertext gibberish.

---

## 2. Integrity & Authenticated Encryption (AES-GCM)

* **Principle:** Guaranteeing that data has not been altered, tampered with, or corrupted in transit or storage.
* **Implementation:**
 * We chose **AES-GCM** (*Galois/Counter Mode*), an **AEAD** (*Authenticated Encryption with Associated Data*) cipher.
* **Security Benefit:** AES-GCM automatically appends a 128-bit authentication tag to the ciphertext. If an attacker modifies even a single byte of the ciphertext or IV in the database, `crypto.subtle.decrypt()` immediately throws an error and rejects the payload.

---

## 3. Zero-Knowledge Architecture & Client-Side Cryptography

* **Principle:** The server acts strictly as a storage relay and has **zero knowledge** of the plaintext data or the encryption keys.
* **Implementation:**
 * **Key Derivation (`SHA-256`):** Computed in browser RAM from the user's typed password.
 * **Encryption (`AES-GCM`):** Performed in browser RAM prior to form submission.
 * **Decryption (`AES-GCM`):** Performed in browser RAM upon clicking "Unlock".
* **Security Benefit:** The server never receives, logs, or stores the plaintext message or the secret key. If the server is compromised, user privacy remains protected.

---

## 4. Cryptographic Randomness & Nonce Uniqueness (IV)

* **Principle:** Encryption algorithms must produce completely different ciphertexts every time, even if the exact same message is encrypted repeatedly with the same key.
* **Implementation:**
 * Every encryption generates a fresh, 12-byte (96-bit) random **Initialization Vector (IV)** using `crypto.getRandomValues(new Uint8Array(12))`.
* **Security Benefit:**
 * **Prevents Pattern Leakage:** Encrypting `"aman kashyap"` 100 times yields 100 different ciphertexts.
 * **Prevents GCM Nonce Reuse:** Reusing an IV in AES-GCM allows attackers to XOR ciphertexts and recover plaintexts. Fresh IVs neutralize this threat.

---

## 5. Key Derivation from Human Passwords

* **Principle:** Passwords vary in length and entropy and cannot be used directly as raw binary keys for block ciphers.
* **Implementation:**
 * We use `crypto.subtle.digest("SHA-256", passwordBytes)` to hash the user's variable-length password into a fixed 256-bit (32-byte) digest.
 * `crypto.subtle.importKey()` imports that 256-bit hash directly as an AES-GCM key.
* **Security Benefit:** Ensures the key supplied to AES-GCM satisfies exact key-length and cryptographic randomness requirements.

---

## 6. Minimizing Attack Surface & Least Privilege

* **Principle:** Restrict data exposure to only what is strictly necessary for the operation.
* **Implementation:**
 * The password field on the "Set Message" form has **no `name` attribute** (`<input type="password" id="password-input">`).
* **Security Benefit:** Browsers automatically exclude inputs without a `name` attribute from HTTP POST payloads. This guarantees that raw passwords are **never transmitted over the network** during message updates.

---

## Summary Matrix for Viva Presentation

| Cybersec Principle | Cryptographic Primitive / Technology | Applied in Project |
|:---|:---|:---|
| **Confidentiality** | AES-GCM (256-bit) | Plaintext encrypted into hex ciphertext before saving |
| **Integrity** | AEAD Authentication Tag | `crypto.subtle.decrypt()` rejects tampered ciphertexts |
| **Zero-Knowledge** | Web Crypto API (`window.crypto.subtle`) | Key derivation & encryption executed 100% in browser RAM |
| **Randomness / Anti-Pattern**| `crypto.getRandomValues()` | Fresh 12-byte random IV per message save |
| **Key Derivation** | SHA-256 Digest | Password converted into 256-bit AES CryptoKey |
| **Attack Surface Reduction**| HTML Attribute Control | Password input omitted from HTTP POST body |
