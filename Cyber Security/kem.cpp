/**
 * ============================================================================
 * POST-QUANTUM CRYPTOGRAPHY (PQC) - KEY ENCAPSULATION MECHANISM (KEM) DEMO
 * ============================================================================
 * 
 * WHAT IS A KEM?
 * In traditional cryptography (like RSA/Diffie-Hellman), two parties (Alice & Bob)
 * exchange keys to establish a shared secret. Because quantum computers can break
 * RSA and ECC using Shor's Algorithm, NIST standardized quantum-resistant KEMs
 * like ML-KEM (Kyber).
 * 
 * HOW KEM WORKS (3 Simple Steps):
 * ----------------------------------------------------------------------------
 * 1. Key Generation (Alice):
 *    Alice generates a Public Key (pk) and Secret Key (sk).
 *    Alice sends her Public Key to Bob (in public).
 * 
 * 2. Encapsulation (Bob):
 *    Bob takes Alice's Public Key and runs `encapsulate()`.
 *    This produces:
 *      - A Ciphertext (ct)
 *      - Bob's Shared Secret (key)
 *    Bob sends the Ciphertext to Alice (in public).
 * 
 * 3. Decapsulation (Alice):
 *    Alice receives the Ciphertext and uses her Secret Key to run `decapsulate()`.
 *    This recovers the exact same Shared Secret.
 * 
 * Result: Both Alice and Bob now have the same quantum-safe 256-bit symmetric key!
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

// ANSI color codes for colorful, easy-to-read terminal output
namespace Color {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string CYAN    = "\033[36m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
}

/**
 * Helper function to print byte arrays in a clean hexadecimal format.
 * Shows the first few bytes and the total length.
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
    // Default algorithm: ML-KEM-768 (NIST FIPS 203 Post-Quantum Standard, Security Level 3)
    string kem_algorithm = (argc > 1) ? argv[1] : "ML-KEM-768";

    cout << "\n" << Color::BOLD << Color::CYAN;
    cout << "======================================================================\n";
    cout << "          POST-QUANTUM KEY ENCAPSULATION MECHANISM (KEM)              \n";
    cout << "======================================================================\n";
    cout << Color::RESET;

    // Verify the chosen algorithm is supported and enabled in liboqs
    if (!oqs::KEMs::is_KEM_enabled(kem_algorithm)) {
        cout << "Algorithm '" << kem_algorithm << "' is not enabled. Falling back to default...\n";
        kem_algorithm = oqs::KEMs::get_enabled_KEMs().front();
    }

    try {
        // --------------------------------------------------------------------
        // Setup: Initialize Alice's KEM instance and display algorithm info
        // --------------------------------------------------------------------
        oqs::KeyEncapsulation alice_kem{kem_algorithm};
        auto details = alice_kem.get_details();

        cout << Color::BOLD << "[Algorithm Information]" << Color::RESET << "\n";
        cout << "  • Name                : " << details.name << "\n";
        cout << "  • NIST Security Level : " << details.claimed_nist_level << " (equivalent to AES-" << (details.claimed_nist_level == 1 ? 128 : details.claimed_nist_level == 3 ? 192 : 256) << ")\n";
        cout << "  • Public Key Size     : " << details.length_public_key << " bytes\n";
        cout << "  • Secret Key Size     : " << details.length_secret_key << " bytes\n";
        cout << "  • Ciphertext Size     : " << details.length_ciphertext << " bytes\n";
        cout << "  • Shared Secret Size  : " << details.length_shared_secret << " bytes (" << (details.length_shared_secret * 8) << " bits)\n";
        cout << "----------------------------------------------------------------------\n\n";

        // --------------------------------------------------------------------
        // STEP 1: Alice generates her Keypair (Public Key + Secret Key)
        // --------------------------------------------------------------------
        cout << Color::BOLD << Color::MAGENTA 
             << "[STEP 1] Alice: Generates Keypair" << Color::RESET << "\n";
        cout << "  Alice creates a public key (to share) and a secret key (to keep safe).\n";

        oqs::bytes alice_public_key = alice_kem.generate_keypair();
        oqs::bytes alice_secret_key = alice_kem.export_secret_key();

        print_hex("Alice's Public Key (Sent to Bob)", alice_public_key);
        print_hex("Alice's Secret Key (Kept Private)", alice_secret_key);

        // --------------------------------------------------------------------
        // STEP 2: Bob encapsulates a shared secret using Alice's Public Key
        // --------------------------------------------------------------------
        cout << Color::BOLD << Color::BLUE 
             << "[STEP 2] Bob: Encapsulates Shared Secret" << Color::RESET << "\n";
        cout << "  Bob receives Alice's Public Key, creates a secret, and encapsulates it.\n";

        oqs::KeyEncapsulation bob_kem{kem_algorithm};
        pair<oqs::bytes, oqs::bytes> bob_result = bob_kem.encap_secret(alice_public_key);

        oqs::bytes ciphertext        = bob_result.first;  // Transmitted over network
        oqs::bytes bob_shared_secret = bob_result.second; // Bob's derived key

        print_hex("Ciphertext (Sent Bob -> Alice)", ciphertext);
        print_hex("Bob's Derived Shared Secret", bob_shared_secret);

        // --------------------------------------------------------------------
        // STEP 3: Alice decapsulates the Ciphertext to recover the Shared Secret
        // --------------------------------------------------------------------
        cout << Color::BOLD << Color::MAGENTA 
             << "[STEP 3] Alice: Decapsulates Ciphertext" << Color::RESET << "\n";
        cout << "  Alice receives the Ciphertext from Bob and decrypts it with her Secret Key.\n";

        oqs::bytes alice_shared_secret = alice_kem.decap_secret(ciphertext);

        print_hex("Alice's Recovered Shared Secret", alice_shared_secret);

        // --------------------------------------------------------------------
        // STEP 4: Verification - Ensure Alice and Bob have the EXACT same key
        // --------------------------------------------------------------------
        cout << "----------------------------------------------------------------------\n";
        cout << Color::BOLD << "[STEP 4] Verification: Checking if shared secrets match...\n" << Color::RESET;

        if (alice_shared_secret == bob_shared_secret) {
            cout << Color::BOLD << Color::GREEN;
            cout << "\n  ✓ SUCCESS: Key exchange completed successfully!\n";
            cout << "  Both Alice and Bob have derived the identical quantum-safe key:\n  " << Color::RESET;

            for (auto byte : alice_shared_secret) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(byte);
            }
            cout << dec << "\n\n";
        } else {
            cout << Color::BOLD << "\033[31m  ✗ FAILURE: Shared secrets do not match!\033[0m\n\n";
            return 1;
        }

    } catch (const exception& e) {
        cerr << "Error during execution: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
