# Updated Database Schema Documentation

This document provides a complete explanation of the updated SQLite database schema for **Classmate Hub** ([`classmates.db`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/classmates.db)).

---

## 1. Database Overview

* **Database Engine:** SQLite 3
* **Node.js Driver:** `better-sqlite3`
* **File Location:** [`/Users/amankashyap/Documents/Semester-5/Cyber Security/CS_Lab1/classmates.db`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/classmates.db)
* **Main Table Name:** `accounts`

---

## 2. Schema Evolution (Before vs After)

## # Original Schema (Part A — Unencrypted)
```sql
CREATE TABLE accounts (
 id INTEGER PRIMARY KEY AUTOINCREMENT,
 username TEXT UNIQUE NOT NULL,
 password TEXT NOT NULL,
 display_name TEXT NOT NULL,
 message TEXT
);
```

## # Updated Schema (Part B — End-to-End Encrypted)
```sql
CREATE TABLE accounts (
 id INTEGER PRIMARY KEY AUTOINCREMENT,
 username TEXT UNIQUE NOT NULL,
 password TEXT NOT NULL,
 display_name TEXT NOT NULL,
 message TEXT,
 iv TEXT
);
```

---

## 3. Detailed Column-by-Column Breakdown

| Column | SQL Data Type | Constraints | Security Status | Description & Purpose |
|:---|:---|:---|:---|:---|
| **`id`** | `INTEGER` | `PRIMARY KEY AUTOINCREMENT` | Non-sensitive | Unique auto-incrementing record identifier. |
| **`username`** | `TEXT` | `UNIQUE NOT NULL` | Public identifier | User's unique login handle (e.g. `arjun`, `meera`). |
| **`password`** | `TEXT` | `NOT NULL` | ** PBKDF2 (Salted)** | Stores `<salt>:<hash>` string (16-byte random salt + 100,000 iterations of PBKDF2-HMAC-SHA256). Plaintext is never saved. |
| **`display_name`** | `TEXT` | `NOT NULL` | Non-sensitive | Human-readable name displayed on the user's dashboard (e.g. `Arjun`). |
| **`message`** | `TEXT` | `NULL` | ** AES-256-GCM Ciphertext (Hex)** | Stores **ONLY** hex-encoded encrypted message ciphertext. Plaintext is never stored here. |
| **`iv`** *(New)* | `TEXT` | `NULL` | ** Public Random IV (Hex)** | Stores the 12-byte random Initialization Vector (Hex) required for AES-GCM decryption. |

---

## 4. Deep Dive: The New `iv` Column

## # Why was `iv` added?
AES-GCM encryption requires two pieces of information to decrypt a ciphertext:
1. **The Secret Key** (Derived client-side from the user's password using SHA-256).
2. **The Initialization Vector (IV)** (Generated randomly per encryption operation using `crypto.getRandomValues`).

Without the corresponding IV, `crypto.subtle.decrypt()` cannot decrypt the ciphertext even if the correct password key is supplied.

## # Key Characteristics of the `iv` Column:
- **Format:** 24-character Hexadecimal string (representing 12 raw bytes). Example: `1972ecfe3b6753442172b921`.
- **Public / Non-Secret:** The IV does not need to be kept secret. Storing it in plain text in the database is standard cryptographic practice.
- **Uniqueness:** Generated fresh for every single message update, ensuring encrypting the same text twice produces completely different ciphertexts.

---

## 5. Live Database Data Comparison

## # Example Row in SQLite (`accounts` table):

```text
id: 1
username: arjun
password: aman1857
display_name: Arjun
message: 31427d750e4bdded48cd032c3e61abc3125f95f280748d79288a7dd8d8a84d7f817aec2a1f53
iv: 1972ecfe3b6753442172b921
```

> **Notice:** Looking at the database directly reveals zero information about the user's message (`"aman kashyap"`). Anyone inspecting `classmates.db` sees only the hex ciphertext and random IV.

---

## 6. Migration Code Implementation ([`db.js`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/db.js))

The schema migration is handled automatically on server startup:

```javascript
if (isNewDatabase) {
 // Fresh database creation with iv column
 db.exec(`
 CREATE TABLE accounts (
 id INTEGER PRIMARY KEY AUTOINCREMENT,
 username TEXT UNIQUE NOT NULL,
 password TEXT NOT NULL,
 display_name TEXT NOT NULL,
 message TEXT,
 iv TEXT
 );
 `);
} else {
 // Migration check for pre-existing databases
 const columns = db.prepare("PRAGMA table_info(accounts)").all();
 const hasIv = columns.some(col => col.name === "iv");
 if (!hasIv) {
 db.exec("ALTER TABLE accounts ADD COLUMN iv TEXT;");
 }
}
```
