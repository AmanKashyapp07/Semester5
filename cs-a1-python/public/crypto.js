// public/crypto.js

function strToArrayBuffer(str) {
    return new TextEncoder().encode(str);
}

function arrayBufferToStr(buffer) {
    return new TextDecoder().decode(buffer);
}

function bufferToHex(buffer) {
    return Array.from(new Uint8Array(buffer))
        .map(b => b.toString(16).padStart(2, '0'))
        .join('');
}

function hexToBuffer(hex) {
    const bytes = new Uint8Array(hex.length / 2);
    for (let i = 0; i < hex.length; i += 2) {
        bytes[i/2] = parseInt(hex.substring(i, i + 2), 16);
    }
    return bytes;
}

async function hashPassword(passwordStr) {
    const data = strToArrayBuffer(passwordStr);
    const hashBuffer = await window.crypto.subtle.digest('SHA-256', data);
    return await window.crypto.subtle.importKey(
        'raw',
        hashBuffer,
        { name: 'AES-GCM' },
        false,
        ['encrypt', 'decrypt']
    );
}

async function encryptData(key, plainTextStr) {
    const iv = window.crypto.getRandomValues(new Uint8Array(12));
    const data = strToArrayBuffer(plainTextStr);
    const encryptedBuffer = await window.crypto.subtle.encrypt(
        { name: 'AES-GCM', iv: iv },
        key,
        data
    );
    return {
        ciphertext: bufferToHex(encryptedBuffer),
        iv: bufferToHex(iv)
    };
}

async function decryptData(key, ciphertextHex, ivHex) {
    const ciphertextBuffer = hexToBuffer(ciphertextHex);
    const ivBuffer = hexToBuffer(ivHex);
    const decryptedBuffer = await window.crypto.subtle.decrypt(
        { name: 'AES-GCM', iv: ivBuffer },
        key,
        ciphertextBuffer
    );
    return arrayBufferToStr(decryptedBuffer);
}
