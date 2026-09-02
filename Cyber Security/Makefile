CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 \
            -I/opt/homebrew/include \
            -I/opt/homebrew/opt/openssl@3/include \
            -I/usr/local/include
LDFLAGS ?= -L/opt/homebrew/lib \
           -L/opt/homebrew/opt/openssl@3/lib \
           -L/usr/local/lib \
           -loqs -lcrypto

TARGETS = list kem sig

all: $(TARGETS)

list: list.cpp
	$(CXX) $(CXXFLAGS) list.cpp -o list $(LDFLAGS)

kem: kem.cpp
	$(CXX) $(CXXFLAGS) kem.cpp -o kem $(LDFLAGS)

sig: sig.cpp
	$(CXX) $(CXXFLAGS) sig.cpp -o sig $(LDFLAGS)

run: all
	@echo "================ RUNNING LIST ================"
	./list
	@echo "\n================ RUNNING KEM ================"
	./kem
	@echo "\n================ RUNNING SIG ================"
	./sig

clean:
	rm -f $(TARGETS)

.PHONY: all clean run
