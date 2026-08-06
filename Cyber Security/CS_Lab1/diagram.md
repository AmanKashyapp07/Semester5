# Cyber Security Lab 1 — Execution & Architecture Diagrams

This document breaks down the **End-to-End Encryption Flow** into simple, visual steps.

---

## Viva Quick Summary (Explain in 30 Seconds)

1. **Password Key Derivation:** User enters password `aman1857`. Browser hashes it using **SHA-256** to derive a 256-bit AES-GCM Key.
2. **Encryption on Submit (`/set-message`):** Browser generates a fresh random 12-byte **IV** (`crypto.getRandomValues`), encrypts `"aman kashyap"` with **AES-GCM**, and sends **ONLY** `message` (ciphertext) and `iv` over HTTP.
3. **Storage in SQLite:** The server saves `message` (`31427d...`) and `iv` (`1972ec...`) in [`classmates.db`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/classmates.db).
4. **Zero-Network Decryption (`/account`):** The server sends the locked HTML card. User enters `aman1857` and clicks **Unlock**. The browser reconstructs the AES key and decrypts the ciphertext **100% in RAM with ZERO network requests**.

---

## Phase 1: Client-Side Encryption & Message Save

![Phase 1 Diagram](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/phase1_encryption_diagram_v2.png)

## # Step-by-Step Breakdown:

| Step | Action | Values / Details | Code Pointers |
|:---|:---|:---|:---|
| **1. User Input** | Enters plaintext message & password | Message: `"aman kashyap"`<br/>Password: `"aman1857"` | [`routes/message.js`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/routes/message.js) |
| **2. Key Derivation** | Hashes password with SHA-256 | SHA-256(`"aman1857"`) AES Key | [`public/crypto.js`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/public/crypto.js# L16-L28) |
| **3. Random IV** | Generates fresh 12-byte IV | IV: `1972ecfe3b67...` | `crypto.getRandomValues()` |
| **4. AES-GCM Encrypt** | Encrypts plaintext message | Output: `31427d750e4b...` | `crypto.subtle.encrypt()` |
| **5. Form Submit** | Swaps message with ciphertext | POST Body: `message=31427d...&iv=1972ec...` | Password field has no `name` attribute! |
| **6. Database Save** | Server updates SQLite table | `UPDATE accounts SET message = ..., iv = ...` | [`db.js`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/db.js) |

---

## Phase 2: Page Load & Zero-Network Decryption

![Phase 2 Diagram](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/phase2_decryption_diagram_v2.png)

## # Step-by-Step Breakdown:

| Step | Action | Values / Details | Code Pointers |
|:---|:---|:---|:---|
| **1. GET /account** | Server fetches row from SQLite | Ciphertext: `31427d...`<br/>IV: `1972ec...` | [`routes/account.js`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/routes/account.js# L11) |
| **2. Locked UI** | Displays locked card | ` Arjun's message is encrypted ` | Plaintext is **NOT** present in HTML DOM! |
| **3. User Unlock** | Types password & clicks Unlock | Enters: `"aman1857"` | ** 0 HTTP Requests Made!** |
| **4. In-Memory Key** | Reconstructs AES Key in RAM | SHA-256(`"aman1857"`) AES Key | [`public/crypto.js`](file:///Users/amankashyap/Documents/Semester-5/Cyber%20Security/CS_Lab1/public/crypto.js# L60-L75) |
| **5. AES Decrypt** | Decrypts ciphertext in memory | `crypto.subtle.decrypt(...)` `"aman kashyap"` | Returns string in RAM |
| **6. DOM Update** | Replaces locked card with message | Displays: ` Arjun's message: aman kashyap` | `# unlocked-box` shown |

---

## ASCII Architecture Overview (Viewable Anywhere)

```text
===================================================================================
 PHASE 1: SAVING AN ENCRYPTED MESSAGE
===================================================================================

 [ USER INPUT ] [ CLIENT BROWSER (crypto.js) ] [ SERVER / DB ]
 Message: "aman kashyap" ──► 1. SHA-256("aman1857") AES Key
 Password: "aman1857" 2. getRandomValues() IV: 1972ec...
 3. encrypt("aman kashyap")
 │
 ▼
 [ CIPHERTEXT: 31427d... ]
 │
 └─► HTTP POST /set-message ──► UPDATE accounts
 (No Password / Plaintext) SET message='31427d...',
 iv='1972ec...'

===================================================================================
 PHASE 2: UNLOCKING ON ACCOUNT PAGE
===================================================================================

 [ SERVER / DB ] [ CLIENT BROWSER (RAM) ] [ USER DISPLAY ]
 DB Row: 31427d... + 1972ec... ──► Serves Locked Card HTML
 │
 ▼
 Password Entered: "aman1857" ──► 1. SHA-256("aman1857") AES Key
 2. decrypt(31427d..., 1972ec...)
 │
 ▼
 [ "aman kashyap" ] ──────────────────► Displays message!
 ( ZERO Network Requests!)
```
