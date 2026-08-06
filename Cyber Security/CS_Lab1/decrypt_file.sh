#!/usr/bin/env bash
#
# decrypt_file.sh
#
# Decrypts a file previously encrypted with encrypt_file.sh
# (AES-256-CBC, key derived from a password via PBKDF2).
#
# Usage:
#   ./decrypt_file.sh
#   (it will prompt for the encrypted file path and the password)
#
# Or non-interactively for the file path:
#   ./decrypt_file.sh /path/to/document.pdf.enc
#
# Output:
#   The original file, with ".enc" stripped from the filename
#   (e.g. document.pdf.enc -> document.pdf). If a file with that
#   name already exists, you'll be asked whether to overwrite it,
#   or you can supply a custom output path as the 2nd argument.

set -euo pipefail

if ! command -v openssl >/dev/null 2>&1; then
  echo "Error: openssl is required but not found on this system." >&2
  exit 1
fi

# --- get input file ---
if [[ $# -ge 1 ]]; then
  INPUT_FILE="$1"
else
  read -rp "Path to encrypted file (.enc): " INPUT_FILE
fi

if [[ ! -f "$INPUT_FILE" ]]; then
  echo "Error: file '$INPUT_FILE' not found." >&2
  exit 1
fi

# --- work out default output filename ---
if [[ $# -ge 2 ]]; then
  OUTPUT_FILE="$2"
elif [[ "$INPUT_FILE" == *.enc ]]; then
  OUTPUT_FILE="${INPUT_FILE%.enc}"
else
  OUTPUT_FILE="${INPUT_FILE}.decrypted"
fi

if [[ -e "$OUTPUT_FILE" ]]; then
  read -rp "Output file '$OUTPUT_FILE' already exists. Overwrite? [y/N]: " ans
  if [[ ! "$ans" =~ ^[Yy]$ ]]; then
    echo "Aborted."
    exit 1
  fi
fi

# --- get password ---
read -rsp "Enter password: " PASSWORD
echo

# --- decrypt ---
if ! openssl enc -d -aes-256-cbc -pbkdf2 -iter 200000 \
      -in "$INPUT_FILE" -out "$OUTPUT_FILE" -pass pass:"$PASSWORD" 2>/tmp/decrypt_err.$$; then
  echo "Error: decryption failed. Wrong password, or the file is not a valid" >&2
  echo "openssl AES-256-CBC/PBKDF2 encrypted file." >&2
  cat /tmp/decrypt_err.$$ >&2
  rm -f /tmp/decrypt_err.$$ "$OUTPUT_FILE"
  unset PASSWORD
  exit 1
fi

rm -f /tmp/decrypt_err.$$
unset PASSWORD

echo
echo "Decrypted file written to: $OUTPUT_FILE"
