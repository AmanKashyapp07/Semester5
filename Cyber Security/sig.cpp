/**
 * ============================================================================
 * POST-QUANTUM CRYPTOGRAPHY (PQC) - DIGITAL SIGNATURE (SIG) DEMO
 * ============================================================================
 * 
 * WHAT IS A DIGITAL SIGNATURE?
 * A digital signature provides:
 *   1. Authenticity : Proves the message was created by the real sender.
 *   2. Integrity    : Guarantees the message was not modified in transit.
 *   3. Non-repudiation: Sender cannot deny having sent the message.
 * 
 * Traditional signature algorithms (like RSA, DSA, ECDSA) are vulnerable to
 * quantum attacks. NIST standardized post-quantum signature schemes like
 * ML-DSA (Dilithium) and Falcon.
 * 
 * HOW DIGITAL SIGNATURES WORK (3 Simple Steps):
 * ----------------------------------------------------------------------------
 * 1. Key Generation (Signer):
 *    Signer generates a Public Verification Key (pk) and Private Signing Key (sk).
 *    Public key is shared openly. Private key is kept secret.
 * 
 * 2. Signing (Signer):
 *    Signer takes a message and signs it with the Private Key -> produces a Signature.
 * 
 * 3. Verification (Verifier / Anyone):
 *    Anyone with the Public Key, the Message, and the Signature can verify:
 *    - Valid Signature   -> Message is authentic!
 *    - Invalid Signature -> Message or signature was modified/forged!
 * ============================================================================
 */

#define LIBOQS_CPP_VERSION "0.16.0"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <liboqs-cpp/oqs_cpp.hpp>

using namespace std;

// ANSI color codes for colorful terminal output
namespace Color {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string CYAN    = "\033[36m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string RED     = "\033[31m";
    const string MAGENTA = "\033[35m";
}

/**
 * Helper function to display binary data in hexadecimal format.
 */
void print_hex(const string& label, const oqs::bytes& data, size_t max_bytes = 24) {
    cout << Color::YELLOW << "  " << label << Color::RESET 
         << " (" << data.size() << " bytes):\n    ";

    size_t count = min(data.size(), max_bytes);
    for (size_t i = 0; i < count; ++i) {
        cout << hex << setw(2) << setfill('0')
             << static_cast<int>(data[i]) << " ";
    }

    if (data.size() > max_bytes) {
        cout << "... [" << dec << data.size() - max_bytes << " more bytes]";
    }
    cout << dec << "\n\n";
}

int main(int argc, char* argv[]) {
    // Default algorithm: ML-DSA-65 (NIST FIPS 204 Standard, Dilithium3)
    string sig_algorithm = (argc > 1) ? argv[1] : "ML-DSA-65";

    cout << "\n" << Color::BOLD << Color::CYAN;
    cout << "======================================================================\n";
    cout << "         POST-QUANTUM DIGITAL SIGNATURE DEMONSTRATION                 \n";
    cout << "======================================================================\n";
    cout << Color::RESET;

    // Verify the algorithm is enabled in liboqs
    if (!oqs::Sigs::is_sig_enabled(sig_algorithm)) {
        cout << "Algorithm '" << sig_algorithm << "' is not enabled. Falling back...\n";
        sig_algorithm = oqs::Sigs::get_enabled_sigs().front();
    }

    try {
        // --------------------------------------------------------------------
        // Setup: Initialize Signer instance and display algorithm info
        // --------------------------------------------------------------------
        oqs::Signature signer{sig_algorithm};
        auto details = signer.get_details();

        cout << Color::BOLD << "[Algorithm Information]" << Color::RESET << "\n";
        cout << "  • Name                   : " << details.name << "\n";
        cout << "  • NIST Security Level    : " << details.claimed_nist_level << " (equivalent to AES-" << (details.claimed_nist_level == 1 ? 128 : details.claimed_nist_level == 3 ? 192 : 256) << ")\n";
        cout << "  • Security Standard      : EUF-CMA (" << (details.is_euf_cma ? "Existentially Unforgeable under Chosen Message Attack" : "No") << ")\n";
        cout << "  • Public Key Size        : " << details.length_public_key << " bytes\n";
        cout << "  • Secret Key Size        : " << details.length_secret_key << " bytes\n";
        cout << "  • Max Signature Size     : " << details.max_length_signature << " bytes\n";
        cout << "----------------------------------------------------------------------\n\n";

        // --------------------------------------------------------------------
        // STEP 1: Signer generates Keypair (Public Key + Secret Key)
        // --------------------------------------------------------------------
        cout << Color::BOLD << Color::MAGENTA 
             << "[STEP 1] Keypair Generation" << Color::RESET << "\n";
        cout << "  Signer creates a public verification key and a private signing key.\n";

        oqs::bytes public_key = signer.generate_keypair();
        oqs::bytes secret_key = signer.export_secret_key();

        print_hex("Public Key (Shared with Verifier)", public_key);
        print_hex("Secret Key (Kept Private by Signer)", secret_key);

        // --------------------------------------------------------------------
        // STEP 2: Prepare Message to Sign
        // --------------------------------------------------------------------
        string original_message_str = "Transfer $1,000 to Bob - Authorized by Alice";
        if (argc > 2) {
            original_message_str = argv[2];
        }
        oqs::bytes message(original_message_str.begin(), original_message_str.end());

        cout << Color::BOLD << "[STEP 2] Message to Sign:" << Color::RESET << "\n";
        cout << "  Original Message: \"" << Color::CYAN << original_message_str << Color::RESET << "\"\n";
        cout << "  Message Size    : " << message.size() << " bytes\n\n";

        // --------------------------------------------------------------------
        // STEP 3: Sign the Message using Secret Key
        // --------------------------------------------------------------------
        cout << Color::BOLD << Color::MAGENTA 
             << "[STEP 3] Signer Signs the Message" << Color::RESET << "\n";
        cout << "  Signer uses the private signing key to compute the digital signature.\n";

        oqs::bytes signature = signer.sign(message);
        print_hex("Generated Signature", signature);

        // --------------------------------------------------------------------
        // STEP 4: Verifier checks the Authentic Message with Public Key
        // --------------------------------------------------------------------
        cout << Color::BOLD << "[STEP 4] Verifying the Authentic Message" << Color::RESET << "\n";
        cout << "  Verifier checks: Does this signature match the message and public key?\n";

        oqs::Signature verifier{sig_algorithm};
        bool is_valid = verifier.verify(message, signature, public_key);

        if (is_valid) {
            cout << Color::BOLD << Color::GREEN 
                 << "  ✓ SUCCESS: Signature is VALID! The message is authentic and untampered.\n\n" 
                 << Color::RESET;
        } else {
            cout << Color::BOLD << Color::RED 
                 << "  ✗ FAILURE: Signature is INVALID!\n\n" 
                 << Color::RESET;
            return 1;
        }

        // --------------------------------------------------------------------
        // STEP 5: Tamper Test - Modify the message and observe verification failure
        // --------------------------------------------------------------------
        cout << Color::BOLD << "[STEP 5] Security Test: Tampering with Message" << Color::RESET << "\n";
        cout << "  Attacker intercepts message and changes '$1,000' to '$100,000'...\n";

        string tampered_message_str = "Transfer $100,000 to Bob - Authorized by Alice";
        oqs::bytes tampered_message(tampered_message_str.begin(), tampered_message_str.end());

        cout << "  Tampered Message: \"" << Color::RED << tampered_message_str << Color::RESET << "\"\n";

        bool is_tampered_valid = verifier.verify(tampered_message, signature, public_key);

        if (!is_tampered_valid) {
            cout << Color::BOLD << Color::GREEN 
                 << "  ✓ SUCCESS: Tampering was DETECTED! The signature was rejected.\n\n" 
                 << Color::RESET;
        } else {
            cout << Color::BOLD << Color::RED 
                 << "  ✗ CRITICAL FAILURE: Tampered message was incorrectly accepted!\n\n" 
                 << Color::RESET;
            return 1;
        }

    } catch (const exception& e) {
        cerr << "Error during execution: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
