# Cyber Security Assignment 1 - Steps & Commands Executed

This document logs the step-by-step commands executed to inspect the directory, decrypt the encrypted assignment file using the provided password `"Honesty"`, and extract the assignment contents.

---

## 1. Inspect Directory Contents

Check what files exist in the `Cyber Security` directory:

```bash
ls -la "/Users/amankashyap/Documents/Semester-5/Cyber Security"
```

**Files found:**
- `Encrypted_Assignment_1.enc`
- `decrypt_file.sh`

---

## 2. Inspect the Decryption Script

Viewed `decrypt_file.sh` to check the parameters used for decryption (AES-256-CBC with PBKDF2 and 200,000 iterations):

```bash
cat "/Users/amankashyap/Documents/Semester-5/Cyber Security/decrypt_file.sh"
```

---

## 3. Decrypt `Encrypted_Assignment_1.enc`

Decrypted the `.enc` file using `openssl` with the provided password `"Honesty"`:

```bash
openssl enc -d -aes-256-cbc -pbkdf2 -iter 200000 \
 -in "/Users/amankashyap/Documents/Semester-5/Cyber Security/Encrypted_Assignment_1.enc" \
 -out "/Users/amankashyap/Documents/Semester-5/Cyber Security/Assignment_1_decrypted" \
 -pass pass:"Honesty"
```

---

## 4. Verify Decrypted File Type

Checked the file format of the decrypted output:

```bash
file "/Users/amankashyap/Documents/Semester-5/Cyber Security/Assignment_1_decrypted"
```

**Output:** `PDF document, version 1.4`

---

## 5. Rename Output to `.pdf` & Extract Text

Renamed `Assignment_1_decrypted` to `Assignment_1.pdf` and extracted its text content using `pdftotext`:

```bash
mv "/Users/amankashyap/Documents/Semester-5/Cyber Security/Assignment_1_decrypted" \
 "/Users/amankashyap/Documents/Semester-5/Cyber Security/Assignment_1.pdf"

pdftotext "/Users/amankashyap/Documents/Semester-5/Cyber Security/Assignment_1.pdf" -
```

---

## Summary of Results

- Decrypted file saved to: [Assignment_1.pdf](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/Assignment_1.pdf)
- Repository for the assignment: [https://github.com/SMaityCodes/CS_Lab1](https://github.com/SMaityCodes/CS_Lab1)
