"""
routes/password.py
------------------
Handles the password update functionality.

Security & Architecture Note:
- The app stores passwords in plaintext in the SQLite database (an intentional simplicity
  in this security lab demonstration).
- Notice that when a user changes their password, any previously saved message is NOT
  automatically re-encrypted! The existing ciphertext in the DB was encrypted using a key
  derived from the old password, so unlocking that message will still require the old password
  unless a new message is saved.
"""

from flask import Blueprint, request, redirect
from db import get_db
from views import page

password_bp = Blueprint("password", __name__)


# ==============================================================================
# UI Component Helpers
# ==============================================================================

def render_change_password_form() -> str:
    """Renders the password change form HTML."""
    content = """
    <h1>Change Password</h1>
    <p class="subtitle">Pick something only you know!</p>

    <form method="POST" action="/change-password">
      <label for="password">New password</label>
      <input type="password" id="password" name="password" placeholder="New password" required autofocus>
      <button type="submit" class="btn btn-green">Save Password</button>
    </form>

    <a href="/account" class="btn btn-pink" style="margin-top: 14px; display:inline-block;">Back</a>
    """
    return page("Change Password", content)


# ==============================================================================
# Route Handlers
# ==============================================================================

@password_bp.route("/change-password", methods=["GET"])
def change_password_view():
    """
    Renders the password change form.
    Requires an active 'username' session cookie.
    """
    if not request.cookies.get("username"):
        return redirect("/")

    return render_change_password_form()


@password_bp.route("/change-password", methods=["POST"])
def change_password():
    """
    Updates the password for the currently logged-in user.

    Flow:
    1. Read 'username' cookie to identify the account.
    2. Extract 'password' from POST body.
    3. Update the 'accounts' table using a parameterized query.
    4. Redirect back to the dashboard (/account).
    """
    username = request.cookies.get("username")
    if not username:
        return redirect("/")

    new_password = request.form.get("password")

    db = get_db()
    cursor = db.cursor()

    # Parameterized query protects against SQL injection on the update statement
    cursor.execute(
        "UPDATE accounts SET password = ? WHERE username = ?",
        (new_password, username),
    )
    db.commit()

    return redirect("/account")
