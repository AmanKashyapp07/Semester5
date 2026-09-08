/**
 * ============================================================================
 * POST-QUANTUM CRYPTOGRAPHY (PQC) - KEM ALGORITHM BENCHMARK & COMPARISON
 * ============================================================================
 * 
 * THIS PROGRAM COMPARES MULTIPLE KEM ALGORITHM FAMILIES SIDE-BY-SIDE:
 *   1. Module Lattice (NIST Standard): ML-KEM-768
 *   2. Code-Based (Goppa Codes): Classic-McEliece-348864
 *   3. Unstructured Lattice (Generic LWE): FrodoKEM-640-AES
 *   4. Code-Based (Hamming Quasi-Cyclic): HQC-128
 *   5. Code-Based (Bit-Flipping): BIKE-L1
 * 
 * FOR EACH ALGORITHM, IT MEASURES:
 *   - Public Key Size (Bytes)
 *   - Ciphertext Size (Bytes)
 *   - Secret Key Size (Bytes)
 *   - Key Generation Time (Microseconds)
 *   - Encapsulation Time (Microseconds)
 *   - Decapsulation Time (Microseconds)
 *   - Verification (Shared secret match)
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

struct KEMBenchmarkResult {
    string name;
    string family;
    size_t nist_level;
    size_t pk_size;
    size_t ct_size;
    size_t sk_size;
    double keygen_us;
    double encap_us;
    double decap_us;
    bool success;
};

void run_kem_benchmark(const string& alg_name, const string& family, vector<KEMBenchmarkResult>& results) {
    if (!oqs::KEMs::is_KEM_enabled(alg_name)) {
        cout << Color::YELLOW << "Skipping " << alg_name << " (not enabled in current build)\n" << Color::RESET;
        return;
    }

    try {
        oqs::KeyEncapsulation alice_kem{alg_name};
        auto details = alice_kem.get_details();

        // 1. Measure Keypair Generation
        auto t0 = high_resolution_clock::now();
        oqs::bytes alice_pk = alice_kem.generate_keypair();
        auto t1 = high_resolution_clock::now();
        double keygen_time = duration<double, micro>(t1 - t0).count();

        // 2. Measure Encapsulation (Bob)
        oqs::KeyEncapsulation bob_kem{alg_name};
        t0 = high_resolution_clock::now();
        auto enc_res = bob_kem.encap_secret(alice_pk);
        t1 = high_resolution_clock::now();
        double encap_time = duration<double, micro>(t1 - t0).count();

        oqs::bytes ciphertext = enc_res.first;
        oqs::bytes bob_secret = enc_res.second;

        // 3. Measure Decapsulation (Alice)
        t0 = high_resolution_clock::now();
        oqs::bytes alice_secret = alice_kem.decap_secret(ciphertext);
        t1 = high_resolution_clock::now();
        double decap_time = duration<double, micro>(t1 - t0).count();

        bool match = (alice_secret == bob_secret);

        results.push_back({
            details.name,
            family,
            details.claimed_nist_level,
            details.length_public_key,
            details.length_ciphertext,
            details.length_secret_key,
            keygen_time,
            encap_time,
            decap_time,
            match
        });

    } catch (const exception& e) {
        cerr << "Error evaluating " << alg_name << ": " << e.what() << "\n";
    }
}

int main() {
    cout << "\n" << Color::BOLD << Color::CYAN;
    cout << "====================================================================================================\n";
    cout << "                       POST-QUANTUM KEM MULTI-ALGORITHM COMPARISON BENCHMARK                        \n";
    cout << "====================================================================================================\n";
    cout << Color::RESET;

    vector<KEMBenchmarkResult> results;

    cout << "Benchmarking selected KEM algorithm families...\n\n";

    // 1. Module Lattice (NIST Primary Standard)
    run_kem_benchmark("ML-KEM-512", "Module Lattice", results);
    run_kem_benchmark("ML-KEM-768", "Module Lattice", results);
    run_kem_benchmark("ML-KEM-1024", "Module Lattice", results);

    // 2. Code-Based (Classic McEliece - Oldest PQC scheme since 1978)
    run_kem_benchmark("Classic-McEliece-348864", "Code-Based (Goppa)", results);

    // 3. Unstructured Lattice (Generic LWE - Most conservative security)
    run_kem_benchmark("FrodoKEM-640-AES", "Unstructured Lattice", results);

    // 4. Code-Based (HQC & BIKE)
    run_kem_benchmark("HQC-128", "Code-Based (HQC)", results);
    run_kem_benchmark("BIKE-L1", "Code-Based (BIKE)", results);

    // Display Comparison Table
    cout << "\n" << Color::BOLD;
    cout << left 
         << setw(26) << "Algorithm"
         << setw(22) << "Family"
         << setw(8)  << "Level"
         << setw(14) << "Pub Key (B)"
         << setw(14) << "Ciphertext"
         << setw(14) << "KeyGen (µs)"
         << setw(14) << "Encap (µs)"
         << setw(14) << "Decap (µs)"
         << setw(10) << "Status"
         << Color::RESET << "\n";
    cout << string(136, '=') << "\n";

    for (const auto& r : results) {
        cout << left 
             << setw(26) << r.name
             << setw(22) << r.family
             << setw(8)  << r.nist_level
             << setw(14) << r.pk_size
             << setw(14) << r.ct_size
             << setw(14) << fixed << setprecision(1) << r.keygen_us
             << setw(14) << fixed << setprecision(1) << r.encap_us
             << setw(14) << fixed << setprecision(1) << r.decap_us;

        if (r.success) {
            cout << Color::GREEN << setw(10) << "SUCCESS" << Color::RESET;
        } else {
            cout << Color::RED << setw(10) << "FAIL" << Color::RESET;
        }
        cout << "\n";
    }

    cout << string(136, '=') << "\n\n";

    cout << Color::BOLD << Color::YELLOW << "Key Takeaways from the Comparison:" << Color::RESET << "\n";
    cout << "  1. " << Color::BOLD << "ML-KEM (Kyber)" << Color::RESET 
         << ": Extremely fast (under 100 µs) with compact keys (~1 KB) and small ciphertexts (~1 KB).\n";
    cout << "  2. " << Color::BOLD << "Classic McEliece" << Color::RESET 
         << ": Tiniest ciphertext (96 bytes) and super fast decapsulation, but requires a massive 260 KB public key.\n";
    cout << "  3. " << Color::BOLD << "FrodoKEM" << Color::RESET 
         << ": Maximum conservative security (unstructured lattices), with larger keys (~9.6 KB) and slower execution.\n\n";

    return 0;
}
