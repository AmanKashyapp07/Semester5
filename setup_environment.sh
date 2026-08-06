#!/usr/bin/env zsh
# ==============================================================================
# Semester 5 Environment Setup Script (AI, ML, and Cyber Security) for macOS
# ==============================================================================

set -e

echo "=== Starting Semester 5 Software & Tools Installation ==="

# 1. Add Homebrew paths to current shell
if [[ -f "/opt/homebrew/bin/brew" ]]; then
    eval "$(/opt/homebrew/bin/brew shellenv)"
elif [[ -f "/usr/local/bin/brew" ]]; then
    eval "$(/usr/local/bin/brew shellenv)"
fi

# Install Homebrew if not already installed
if ! command -v brew &> /dev/null; then
    echo "--> Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    if [[ -f "/opt/homebrew/bin/brew" ]]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi
else
    echo "--> Homebrew is already installed."
fi

# 2. Install Core Programming Tools & Python
echo "--> Installing Python 3 & Miniconda via Homebrew..."
brew install python || true
brew install --cask miniconda || true

# Find conda binary
CONDA_BIN=""
if command -v conda &> /dev/null; then
    CONDA_BIN="$(command -v conda)"
elif [[ -f "/opt/homebrew/caskroom/miniconda/base/bin/conda" ]]; then
    CONDA_BIN="/opt/homebrew/caskroom/miniconda/base/bin/conda"
elif [[ -f "$HOME/miniconda3/bin/conda" ]]; then
    CONDA_BIN="$HOME/miniconda3/bin/conda"
elif [[ -f "/usr/local/Caskroom/miniconda/base/bin/conda" ]]; then
    CONDA_BIN="/usr/local/Caskroom/miniconda/base/bin/conda"
fi

if [[ -n "$CONDA_BIN" ]]; then
    echo "--> Found conda at: $CONDA_BIN"
    "$CONDA_BIN" init zsh || true
    eval "$("$CONDA_BIN" shell.zsh hook)"
else
    echo "--> Warning: Conda binary not found directly. Will proceed with brew environment."
fi

# 3. Install VS Code and Extensions
echo "--> Installing VS Code..."
brew install --cask visual-studio-code || true

# Locate VS Code CLI
CODE_BIN=""
if command -v code &> /dev/null; then
    CODE_BIN="$(command -v code)"
elif [[ -f "/Applications/Visual Studio Code.app/Contents/Resources/app/bin/code" ]]; then
    CODE_BIN="/Applications/Visual Studio Code.app/Contents/Resources/app/bin/code"
fi

if [[ -n "$CODE_BIN" ]]; then
    echo "--> Installing VS Code extensions..."
    "$CODE_BIN" --install-extension ms-python.python || true
    "$CODE_BIN" --install-extension ms-toolsai.jupyter || true
    "$CODE_BIN" --install-extension ms-python.vscode-pylance || true
else
    echo "--> Note: VS Code CLI 'code' not found in PATH yet."
fi

# 4. Create and Configure Conda Environment for AI/ML
if [[ -n "$CONDA_BIN" ]]; then
    echo "--> Setting up 'sem5' Conda Environment..."
    "$CONDA_BIN" create -n sem5 python=3.11 -y || true

    echo "--> Installing AI & ML Data Science Libraries..."
    "$CONDA_BIN" install -n sem5 -y numpy pandas scikit-learn matplotlib seaborn networkx jupyter jupyterlab || true

    echo "--> Installing PyTorch with Apple Silicon GPU support (MPS)..."
    "$CONDA_BIN" run -n sem5 pip install torch torchvision torchaudio || true
fi

# 5. Install Cyber Security Tools
echo "--> Installing Cyber Security Applications..."
brew install --cask wireshark || true
brew install --cask utm || true
brew install --cask docker || true
brew install --cask burp-suite || true

# 6. Install Math / Writing Tools
echo "--> Installing GeoGebra..."
brew install --cask geogebra || true

echo "=============================================================================="
echo "=== Semester 5 Setup Completed! ==="
echo "To activate your AI/ML environment anytime, run:  conda activate sem5"
echo "=============================================================================="
