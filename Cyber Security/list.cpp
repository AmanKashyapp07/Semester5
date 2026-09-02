/**
 * ============================================================================
 * POST-QUANTUM CRYPTOGRAPHY (PQC) - ALGORITHM EXPLORER (liboqs)
 * ============================================================================
 * 
 * WHAT THIS PROGRAM DOES:
 * This program queries the `liboqs` library to display all supported and enabled:
 *   1. Key Encapsulation Mechanisms (KEMs) - used for quantum-safe key exchange
 *   2. Digital Signature Algorithms (SIGs) - used for quantum-safe authentication
 * 
 * FOR EACH ALGORITHM, IT SHOWS:
 *   - Name & Status (Enabled / Disabled)
 *   - NIST Claimed Security Level (Level 1 ≈ AES-128, Level 3 ≈ AES-192, Level 5 ≈ AES-256)
 *   - Public Key size (in bytes)
 *   - Ciphertext / Max Signature size (in bytes)
 *   - Secret Key size (in bytes)
 * ============================================================================
 */

#define LIBOQS_CPP_VERSION "0.16.0"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <liboqs-cpp/oqs_cpp.hpp>

using namespace std;

// ANSI color codes for colorful output
namespace Color {
    const string RESET  = "\033[0m";
    const string BOLD   = "\033[1m";
    const string CYAN   = "\033[36m";
    const string GREEN  = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE   = "\033[34m";
}

void print_separator(char c = '=', int width = 112) {
    cout << string(width, c) << '\n';
}

/**
 * Lists all Key Encapsulation Mechanisms (KEMs)
 */
void list_kem_algorithms() {
    print_separator('=');
    cout << Color::BOLD << Color::CYAN 
         << "                        POST-QUANTUM KEY ENCAPSULATION MECHANISMS (KEMs)\n" 
         << Color::RESET;
    print_separator('=');

    const auto& supported_kems = oqs::KEMs::get_supported_KEMs();
    const auto& enabled_kems   = oqs::KEMs::get_enabled_KEMs();

    cout << "Summary: " << Color::BOLD << supported_kems.size() << Color::RESET << " Supported Algorithms | "
         << Color::BOLD << Color::GREEN << enabled_kems.size() << " Enabled in Build" << Color::RESET << "\n\n";

    // Table Header
    cout << Color::BOLD
         << left 
         << setw(38) << "Algorithm Name"
         << setw(10) << "Status"
         << setw(12) << "NIST Level"
         << setw(16) << "Public Key"
         << setw(18) << "Ciphertext"
         << setw(16) << "Secret Key"
         << Color::RESET << "\n";
    print_separator('-');

    for (const auto& alg_name : supported_kems) {
        bool is_enabled = oqs::KEMs::is_KEM_enabled(alg_name);

        cout << left << setw(38) << alg_name;

        if (is_enabled) {
            cout << Color::GREEN << setw(10) << "Enabled" << Color::RESET;
            try {
                oqs::KeyEncapsulation kem{alg_name};
                auto d = kem.get_details();

                cout << setw(12) << ("Level " + to_string(d.claimed_nist_level))
                     << setw(16) << (to_string(d.length_public_key) + " B")
                     << setw(18) << (to_string(d.length_ciphertext) + " B")
                     << setw(16) << (to_string(d.length_secret_key) + " B");
            } catch (...) {
                cout << "Details unavailable";
            }
        } else {
            cout << "\033[31m" << setw(10) << "Disabled" << Color::RESET
                 << setw(12) << "-"
                 << setw(16) << "-"
                 << setw(18) << "-"
                 << setw(16) << "-";
        }
        cout << '\n';
    }
    cout << '\n';
}

/**
 * Lists all Digital Signature Algorithms (SIGs)
 */
void list_sig_algorithms() {
    print_separator('=');
    cout << Color::BOLD << Color::CYAN 
         << "                        POST-QUANTUM DIGITAL SIGNATURE ALGORITHMS (SIGs)\n" 
         << Color::RESET;
    print_separator('=');

    const auto& supported_sigs = oqs::Sigs::get_supported_sigs();
    const auto& enabled_sigs   = oqs::Sigs::get_enabled_sigs();

    cout << "Summary: " << Color::BOLD << supported_sigs.size() << Color::RESET << " Supported Algorithms | "
         << Color::BOLD << Color::GREEN << enabled_sigs.size() << " Enabled in Build" << Color::RESET << "\n\n";

    // Table Header
    cout << Color::BOLD
         << left 
         << setw(46) << "Algorithm Name"
         << setw(10) << "Status"
         << setw(12) << "NIST Level"
         << setw(16) << "Public Key"
         << setw(16) << "Max Signature"
         << setw(16) << "Secret Key"
         << Color::RESET << "\n";
    print_separator('-');

    for (const auto& alg_name : supported_sigs) {
        bool is_enabled = oqs::Sigs::is_sig_enabled(alg_name);

        cout << left << setw(46) << alg_name;

        if (is_enabled) {
            cout << Color::GREEN << setw(10) << "Enabled" << Color::RESET;
            try {
                oqs::Signature sig{alg_name};
                auto d = sig.get_details();

                cout << setw(12) << ("Level " + to_string(d.claimed_nist_level))
                     << setw(16) << (to_string(d.length_public_key) + " B")
                     << setw(16) << (to_string(d.max_length_signature) + " B")
                     << setw(16) << (to_string(d.length_secret_key) + " B");
            } catch (...) {
                cout << "Details unavailable";
            }
        } else {
            cout << "\033[31m" << setw(10) << "Disabled" << Color::RESET
                 << setw(12) << "-"
                 << setw(16) << "-"
                 << setw(16) << "-"
                 << setw(16) << "-";
        }
        cout << '\n';
    }
    cout << '\n';
}

int main() {
    cout << "\n" << Color::BOLD << Color::YELLOW;
    cout << "==============================================================================================================\n";
    cout << "                             OPEN QUANTUM SAFE (liboqs) ALGORITHM EXPLORER\n";
    cout << "==============================================================================================================\n";
    cout << Color::RESET;

    cout << "• liboqs library version : " << Color::BOLD << oqs::oqs_version() << Color::RESET << "\n";
    cout << "• C++ wrapper version    : " << Color::BOLD << oqs::oqs_cpp_version() << Color::RESET << "\n\n";

    list_kem_algorithms();
    list_sig_algorithms();

    return 0;
}
