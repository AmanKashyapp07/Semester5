# Classmate Hub — Cyber Security Assignment Context

This document explains the full project architecture, database structure, security vulnerabilities, and upcoming requirements for **Lab Assignment 1: Client-Side Message Encryption**.

---

## 1. Executive Summary & Objective

**Classmate Hub** is a lightweight Node.js/Express web application where users (students) can:
1. Log in with a username and password.
2. View a personal dashboard page displaying a personal welcome message.
3. Update/set their personal message.
4. Change their account password.

## # The Problem (Current State):
Currently, user messages are sent over HTTP in **plaintext** and stored in the database in **plaintext**. Anyone with access to the database or network traffic can inspect private messages.

## # The Goal (Assignment Requirement):
Implement **Zero-Knowledge / End-to-End Client-Side Encryption** using the browser's built-in **Web Crypto API** (`crypto.subtle`):
- Plaintext messages must **never** leave the browser.
- Encrypt messages with **AES-GCM** using a key derived from the user's password.
- Generate a fresh random Initialization Vector (**IV**) for every encryption.
- Store **only Ciphertext + IV** in the database.
- Decrypt messages **100% client-side** without any server API requests.

---

## 2. Directory Structure & Key Files

```text
CS_Lab1/
├── server.js # Express app entry point & middleware setup
├── db.js # SQLite database initialization & seeding
├── views.js # Shared HTML page layout wrapper template
├── classmates.db # SQLite database file storing account records
├── routes/
│ ├── login.js # Handles GET / (login UI) and POST /login
│ ├── account.js # Handles GET /account (user dashboard) and GET /logout
│ ├── message.js # Handles GET /set-message and POST /set-message
│ └── password.js # Handles GET /change-password and POST /change-password
└── public/
 └── style.css # CSS styling for all pages
```

---

## 3. Database Schema & Data Storage

The application uses **SQLite** via `better-sqlite3`. The database file is located at `classmates.db`.

## # Current Table Schema (`accounts`)

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

## # Seed Accounts (Pre-configured Data)

| ID | Username | Password | Display Name | Message |
|:---|:---|:---|:---|:---|
| 1 | `arjun` | `Football123` *(or updated)* | Arjun | *(User message)* |
| 2 | `meera` | `SummerFun2024` | Meera | `null` |
| 3 | `kabir` | `ChessMaster9` | Kabir | `null` |
| 4 | `zara` | `RainbowUnicorn` | Zara | `null` |

---

## 4. How the Application Works Currently (Part A)

## # A. Authentication & Session Management (`login.js`, `server.js`)
- The server uses `cookie-parser` middleware (`server.js`).
- When a user logs in via `POST /login`:
 1. The server queries `SELECT * FROM accounts WHERE username = ... AND password = ...`.
 2. If credentials match, the server responds with a cookie header: `res.cookie("username", match.username)`.
 3. All subsequent page requests read `req.cookies.username` to identify the logged-in user.

## # B. Setting a Message (`message.js`)
1. User navigates to `/set-message` (renders HTML form).
2. Submitting the form triggers `POST /set-message` with body `message=<text>`.
3. Server executes:
 ```javascript
 db.prepare("UPDATE accounts SET message = ? WHERE username = ?").run(
 req.body.message,
 req.cookies.username
 );
 ```
4. Plaintext message is stored directly in SQLite and user is redirected to `/account`.

## # C. Displaying the Account Page (`account.js`)
1. User navigates to `/account`.
2. Server queries user record: `SELECT * FROM accounts WHERE username = ?`.
3. Server embeds `me.message` directly into HTML response.
4. Browser renders the message in plain text.

---

## 5. What Needs to Be Done (Part B Preview)

To secure the application according to assignment guidelines:

1. **Create `public/crypto.js` Wrapper:**
 - Single centralized file containing Web Crypto API methods (`crypto.subtle`).
 - Function to hash password using SHA-256/MD5 -> derive AES Key.
 - Function to encrypt plaintext using **AES-GCM** + random IV.
 - Function to decrypt ciphertext using **AES-GCM** + IV + password key.

2. **Update Database Schema (`db.js`):**
 - Add an `iv` column (or alter table) to store the Initialization Vector alongside `message` (ciphertext).

3. **Client-Side Form Interception (`message.js`):**
 - Before form submit, encrypt the user's message client-side in JS.
 - Submit `ciphertext` and `iv` to `POST /set-message`.

4. **Locked Account UI & Zero-Request Decryption (`account.js`):**
 - Render page with message in **locked state** (showing password prompt + "Unlock" button).
 - Clicking "Unlock" decrypts the ciphertext **client-side** using the password entered.
 - No HTTP request made to server during unlock.

---

## Quick Inspection Commands

```bash
# Start the server
cd CS_Lab1 && npm start

# View database contents in terminal
sqlite3 classmates.db "SELECT * FROM accounts;"
```

---

## Execution Flow & Architecture Diagrams

## # Phase 1: Client-Side Encryption & Message Save

![Phase 1 Encryption Flow](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/phase1_encryption_flow.png)

```text
 +-------------------------------------------------------------------------+
 | 1. USER INPUT |
 | User enters Message: "aman kashyap" & Password: "Football123" |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 2. KEY DERIVATION (public/crypto.js) |
 | SHA-256("Football123") 256-bit AES-GCM Key |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 3. RANDOM IV GENERATION |
 | crypto.getRandomValues() 12-byte IV (e.g. 1972ecfe3b67...) |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 4. AES-GCM ENCRYPTION |
 | crypto.subtle.encrypt(Key, IV, "aman kashyap") |
 | Hex Ciphertext (e.g. 31427d750e4b...) |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 5. POST /set-message |
 | Sent Body: message=31427d750e4b...&iv=1972ecfe3b67... |
 | Plaintext & Password NEVER leave the browser! |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 6. SQLITE DATABASE SAVE |
 | UPDATE accounts SET message = '31427d...', iv = '1972ec...' |
 | Stored safely in classmates.db |
 +-------------------------------------------------------------------------+
```

---

## # Phase 2: Page Load & Zero-Network Client-Side Decryption

![Phase 2 Decryption Flow](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/phase2_decryption_flow.png)

```text
 +-------------------------------------------------------------------------+
 | 1. PAGE LOAD (GET /account) |
 | Express fetches row from DB (message = '31427d...', iv = '1972ec...') |
 | Serves HTML page with embedded Ciphertext & IV in JS |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 2. LOCKED UI DISPLAY |
 | Browser displays: " Arjun's message is encrypted " |
 | [ Password Input Box ] [ Unlock Button ] |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 3. USER UNLOCK ACTION |
 | User types Password: "Football123" & clicks "Unlock " |
 | NO HTTP / NETWORK REQUEST MADE! |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 4. AES-GCM DECRYPTION |
 | SHA-256("Football123") Reconstruct Key |
 | crypto.subtle.decrypt(Key, IV, Ciphertext) "aman kashyap" |
 +-------------------------------------------------------------------------+
 |
 v
 +-------------------------------------------------------------------------+
 | 5. DOM DISPLAY UPDATE |
 | Displays: " Arjun's message: aman kashyap" |
 +-------------------------------------------------------------------------+
```

