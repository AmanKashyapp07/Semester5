/**
 * public/crypto.js
 * Centralized Web Crypto API wrapper for hashing, AES-GCM encryption, and decryption.
 * Uses browser built-in Web Crypto API (crypto.subtle).
 */

function getCrypto() {
  if (typeof window !== "undefined" && window.crypto) {
    return window.crypto;
  }
  return globalThis.crypto;
}

/**
 * Derives an AES-GCM CryptoKey from a password string using SHA-256 digest.
 * @param {string} password 
 * @returns {Promise<CryptoKey>}
 */
async function deriveKeyFromPassword(password) {
  const cryptoObj = getCrypto();
  const encoder = new TextEncoder();
  const passwordBytes = encoder.encode(password);
  
  // 1. Hash the user's password with SHA-256 (Web Crypto API)
  const hashBuffer = await cryptoObj.subtle.digest("SHA-256", passwordBytes);
  
  // 2. Import the 256-bit hash directly as an AES-GCM key
  return await cryptoObj.subtle.importKey(
    "raw",
    hashBuffer,
    { name: "AES-GCM" },
    false,
    ["encrypt", "decrypt"]
  );
}

/**
 * Encrypts a plaintext message using AES-GCM and the derived password key.
 * Generates a fresh random 12-byte IV for every encryption.
 * @param {string} plaintext 
 * @param {string} password 
 * @returns {Promise<{ ciphertext: string, iv: string }>}
 */
async function encryptMessage(plaintext, password) {
  const cryptoObj = getCrypto();
  const key = await deriveKeyFromPassword(password);
  
  // Generate fresh random 12-byte IV
  const ivBytes = cryptoObj.getRandomValues(new Uint8Array(12));
  const encoder = new TextEncoder();
  const plaintextBytes = encoder.encode(plaintext);

  // Encrypt with AES-GCM
  const encryptedBuffer = await cryptoObj.subtle.encrypt(
    { name: "AES-GCM", iv: ivBytes },
    key,
    plaintextBytes
  );

  return {
    ciphertext: bufferToHex(encryptedBuffer),
    iv: bufferToHex(ivBytes)
  };
}

/**
 * Decrypts a hex-encoded ciphertext using AES-GCM, hex-encoded IV, and password.
 * @param {string} ciphertextHex 
 * @param {string} ivHex 
 * @param {string} password 
 * @returns {Promise<string>}
 */
async function decryptMessage(ciphertextHex, ivHex, password) {
  const cryptoObj = getCrypto();
  const key = await deriveKeyFromPassword(password);
  const ivBytes = hexToBuffer(ivHex);
  const ciphertextBytes = hexToBuffer(ciphertextHex);

  // Decrypt with AES-GCM
  const decryptedBuffer = await cryptoObj.subtle.decrypt(
    { name: "AES-GCM", iv: ivBytes },
    key,
    ciphertextBytes
  );

  const decoder = new TextDecoder();
  return decoder.decode(decryptedBuffer);
}

/**
 * Helper: Convert ArrayBuffer / Uint8Array to Hex string.
 */
function bufferToHex(buffer) {
  const bytes = new Uint8Array(buffer);
  return Array.from(bytes)
    .map(b => b.toString(16).padStart(2, "0"))
    .join("");
}

/**
 * Helper: Convert Hex string to Uint8Array.
 */
function hexToBuffer(hex) {
  const bytes = new Uint8Array(hex.length / 2);
  for (let i = 0; i < hex.length; i += 2) {
    bytes[i / 2] = parseInt(hex.substring(i, i + 2), 16);
  }
  return bytes;
}
