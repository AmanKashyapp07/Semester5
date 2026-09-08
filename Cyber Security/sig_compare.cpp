/**
 * ============================================================================
 * POST-QUANTUM CRYPTOGRAPHY (PQC) - DIGITAL SIGNATURE BENCHMARK & COMPARISON
 * ============================================================================
 * 
 * THIS PROGRAM COMPARES MULTIPLE DIGITAL SIGNATURE ALGORITHM FAMILIES:
 *   1. Module Lattice (NIST Standard Dilithium): ML-DSA-44, ML-DSA-65, ML-DSA-87
 *   2. NTRU Fast Fourier Lattice (Falcon): Falcon-512, Falcon-1024
 * 
 * FOR EACH ALGORITHM, IT MEASURES:
 *   - Public Key Size (Bytes)
 *   - Signature Size (Bytes)
 *   - Secret Key Size (Bytes)
 *   - Key Generation Time (Microseconds)
 *   - Signing Time (Microseconds)
 *   - Verification Time (Microseconds)
 *   - Verification & Tamper Detection Status
 * ============================================================================
 */

#define LIBOQS_CPP_VERSION "0.16.0"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <liboqs-cpp/oqs_cpp.hpp>

using namespace std;
using namespace std::chrono;

namespace Color {
    const string RESET  = "\033[0m";
    const string BOLD   = "\033[1m";
    const string CYAN   = "\033[36m";
    const string GREEN  = "\033[32m";
    const string YELLOW = "\033[33m";
    const string RED    = "\033[31m";
}

struct SIGBenchmarkResult {
    string name;
    string family;
    size_t nist_level;
    size_t pk_size;
    size_t sig_size;
    size_t sk_size;
    double keygen_us;
    double sign_us;
    double verify_us;
    bool valid_success;
    bool tamper_rejected;
};

void run_sig_benchmark(const string& alg_name, const string& family, vector<SIGBenchmarkResult>& results) {
    if (!oqs::Sigs::is_sig_enabled(alg_name)) {
        cout << Color::YELLOW << "Skipping " << alg_name << " (not enabled in current build)\n" << Color::RESET;
        return;
    }

    try {
        oqs::Signature signer{alg_name};
        auto details = signer.get_details();

        // 1. Measure Keypair Generation
        auto t0 = high_resolution_clock::now();
        oqs::bytes public_key = signer.generate_keypair();
        auto t1 = high_resolution_clock::now();
        double keygen_time = duration<double, micro>(t1 - t0).count();

        // 2. Prepare Message
        string message_str = "Post-Quantum Cryptography Signature Benchmark - IIIT Allahabad";
        oqs::bytes message(message_str.begin(), message_str.end());

        // 3. Measure Signing Time
        t0 = high_resolution_clock::now();
        oqs::bytes signature = signer.sign(message);
        t1 = high_resolution_clock::now();
        double sign_time = duration<double, micro>(t1 - t0).count();

        // 4. Measure Verification Time
        oqs::Signature verifier{alg_name};
        t0 = high_resolution_clock::now();
        bool is_valid = verifier.verify(message, signature, public_key);
        t1 = high_resolution_clock::now();
        double verify_time = duration<double, micro>(t1 - t0).count();

        // 5. Test Tamper Resistance
        string tampered_str = message_str + " [TAMPERED]";
        oqs::bytes tampered_message(tampered_str.begin(), tampered_str.end());
        bool tampered_is_valid = verifier.verify(tampered_message, signature, public_key);

        results.push_back({
            details.name,
            family,
            details.claimed_nist_level,
            details.length_public_key,
            signature.size(),
            details.length_secret_key,
            keygen_time,
            sign_time,
            verify_time,
            is_valid,
            !tampered_is_valid // Should reject tampered message
        });

    } catch (const exception& e) {
        cerr << "Error evaluating " << alg_name << ": " << e.what() << "\n";
    }
}

int main() {
    cout << "\n" << Color::BOLD << Color::CYAN;
    cout << "====================================================================================================\n";
    cout << "                    POST-QUANTUM DIGITAL SIGNATURE MULTI-ALGORITHM BENCHMARK                        \n";
    cout << "====================================================================================================\n";
    cout << Color::RESET;

    vector<SIGBenchmarkResult> results;

    cout << "Benchmarking selected Signature algorithm families...\n\n";

    // 1. Module Lattice (NIST Primary Standard - Dilithium)
    run_sig_benchmark("ML-DSA-44", "Module Lattice (Dilithium)", results);
    run_sig_benchmark("ML-DSA-65", "Module Lattice (Dilithium)", results);
    run_sig_benchmark("ML-DSA-87", "Module Lattice (Dilithium)", results);

    // 2. NTRU Fast Fourier Lattice (Falcon - Compact Signatures)
    run_sig_benchmark("Falcon-512", "NTRU Lattice (Falcon)", results);
    run_sig_benchmark("Falcon-1024", "NTRU Lattice (Falcon)", results);

    // Display Comparison Table
    cout << "\n" << Color::BOLD;
    cout << left 
         << setw(16) << "Algorithm"
         << setw(28) << "Family"
         << setw(8)  << "Level"
         << setw(14) << "Pub Key (B)"
         << setw(14) << "Sig Size (B)"
         << setw(14) << "KeyGen (µs)"
         << setw(14) << "Sign (µs)"
         << setw(14) << "Verify (µs)"
         << setw(10) << "Status"
         << Color::RESET << "\n";
    cout << string(132, '=') << "\n";

    for (const auto& r : results) {
        cout << left 
             << setw(16) << r.name
             << setw(28) << r.family
             << setw(8)  << r.nist_level
             << setw(14) << r.pk_size
             << setw(14) << r.sig_size
             << setw(14) << fixed << setprecision(1) << r.keygen_us
             << setw(14) << fixed << setprecision(1) << r.sign_us
             << setw(14) << fixed << setprecision(1) << r.verify_us;

        if (r.valid_success && r.tamper_rejected) {
            cout << Color::GREEN << setw(10) << "SUCCESS" << Color::RESET;
        } else {
            cout << Color::RED << setw(10) << "FAIL" << Color::RESET;
        }
        cout << "\n";
    }

    cout << string(132, '=') << "\n\n";

    cout << Color::BOLD << Color::YELLOW << "Key Takeaways from the Comparison:" << Color::RESET << "\n";
    cout << "  1. " << Color::BOLD << "Falcon-512" << Color::RESET 
         << ": Produces the smallest signatures (656 bytes), making it ideal for bandwidth-constrained networks.\n";
    cout << "  2. " << Color::BOLD << "ML-DSA (Dilithium)" << Color::RESET 
         << ": Highly balanced performance, very fast verification, and easier hardware implementation.\n\n";

    return 0;
}
