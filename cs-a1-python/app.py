"""
app.py
------
Main application entry point for Classmate Hub (Python / Flask).

Architecture:
1. Static Asset Hosting:
   Hosts CSS (`style.css`) and client-side crypto scripts (`crypto.js`) from `/public`.
2. Blueprints:
   Modular route handlers:
   - `login_bp`: Handles `/` and `/login` (contains intentional SQL injection vulnerability)
   - `account_bp`: Handles `/account` dashboard and `/logout`
   - `message_bp`: Handles `/set-message` (client-side encrypted messages)
   - `password_bp`: Handles `/change-password`
3. Lifecycle Hooks:
   - `teardown_appcontext`: Automatically closes SQLite connections after each request.
   - `init_db`: Ensures SQLite database is created and seeded upon startup.
"""

import os
from pathlib import Path
from flask import Flask

from db import init_db, close_db
from routes.login import login_bp
from routes.account import account_bp
from routes.message import message_bp
from routes.password import password_bp

# ==============================================================================
# Flask Application Setup
# ==============================================================================

app = Flask(
    __name__,
    static_folder=str(Path(__file__).resolve().parent / "public"),
    static_url_path="/public",
)

# Automatically close SQLite database connections at the end of each HTTP request
app.teardown_appcontext(close_db)

# ==============================================================================
# Register Route Blueprints (Modular Controller Structure)
# ==============================================================================

app.register_blueprint(login_bp)       # GET /, POST /login
app.register_blueprint(account_bp)     # GET /account, GET /logout
app.register_blueprint(message_bp)     # GET /set-message, POST /set-message
app.register_blueprint(password_bp)    # GET /change-password, POST /change-password

# Ensure the database and initial seed accounts exist
init_db()

# ==============================================================================
# Local Development Server Execution
# ==============================================================================

if __name__ == "__main__":
    # Allow overriding port via PORT environment variable (default: 3000)
    port = int(os.environ.get("PORT", 3000))
    print(f"Classmate Hub is running on http://localhost:{port}")
    app.run(host="0.0.0.0", port=port, debug=True)
