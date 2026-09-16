"""
db.py
-----
Database connection management and initialization using Python's standard sqlite3.

Architecture:
1. Thread-safe connections via Flask application context (`flask.g`).
   - SQLite connections cannot be shared across multiple threads.
   - Using `flask.g` guarantees that each HTTP request gets its own connection.
2. Auto-migration & Seeding (`init_db`):
   - Creates the SQLite database file (`classmates.db`) if not found.
   - Populates schema: id, username, password, display_name, message, iv.
   - Pre-seeds 4 default accounts (arjun, meera, kabir, zara).
   - If an existing database lacks the `iv` column, it performs a migration to add it.
"""

import sqlite3
from pathlib import Path
from flask import g

# Absolute path to the SQLite database file in the project folder
DB_PATH = Path(__file__).resolve().parent / "classmates.db"


def get_db() -> sqlite3.Connection:
    """
    Returns the SQLite connection for the current request context.

    Configures `sqlite3.Row` as the row factory so query results can be
    accessed like dictionaries (e.g. row['username']) as well as tuples.
    """
    if "db" not in g:
        g.db = sqlite3.connect(DB_PATH)
        g.db.row_factory = sqlite3.Row
    return g.db


def close_db(error=None):
    """
    Closes the SQLite connection when the request teardown occurs.
    Registered in app.py via `app.teardown_appcontext(close_db)`.
    """
    db = g.pop("db", None)
    if db is not None:
        db.close()


def init_db():
    """
    Initializes the database schema and sample user data.

    Executed once on server startup.
    - If `classmates.db` does not exist: creates table and inserts 4 sample users.
    - If `classmates.db` exists: ensures the `iv` column exists (migration helper).
    """
    is_new = not DB_PATH.exists()
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    if is_new:
        # Create accounts table
        cursor.execute("""
            CREATE TABLE accounts (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT UNIQUE NOT NULL,
                password TEXT NOT NULL,
                display_name TEXT NOT NULL,
                message TEXT,
                iv TEXT
            );
        """)

        # Sample student accounts
        sample_accounts = [
            ("arjun", "Football123", "Arjun", None, None),
            ("meera", "SummerFun2024", "Meera", None, None),
            ("kabir", "ChessMaster9", "Kabir", None, None),
            ("zara", "RainbowUnicorn", "Zara", None, None),
        ]

        cursor.executemany(
            "INSERT INTO accounts (username, password, display_name, message, iv) VALUES (?, ?, ?, ?, ?)",
            sample_accounts,
        )
        conn.commit()
        print("Set up a fresh classmates.db with four accounts.")
    else:
        # Schema migration check: Ensure 'iv' column exists
        try:
            cursor.execute("SELECT iv FROM accounts LIMIT 1")
        except sqlite3.OperationalError:
            cursor.execute("ALTER TABLE accounts ADD COLUMN iv TEXT;")
            conn.commit()
            print("Added 'iv' column to existing accounts table.")

    conn.close()
