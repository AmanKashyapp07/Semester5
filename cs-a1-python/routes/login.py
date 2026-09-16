"""
routes/login.py
---------------
Handles user authentication and the main landing page.

Security & Educational Notes (Cyber Security Lab 1):
- The `POST /login` route contains an INTENTIONAL SQL Injection vulnerability.
  The query is constructed via raw string concatenation:
      "SELECT * FROM accounts WHERE username = '" + username + "' AND password = '" + password + "'"
  This allows testing SQL injection payloads such as:
      Username: arjun' --
      Password: <anything>
  which comments out the password verification logic and logs in as arjun.
- Successful logins set an unencrypted, plaintext 'username' cookie in the browser
  for session tracking (demonstrating simple cookie-based authentication).
"""

from flask import Blueprint, request, redirect, make_response
from db import get_db
from views import page

login_bp = Blueprint("login", __name__)


# ==============================================================================
# UI Component Helpers
# ==============================================================================

def render_login_form(error_message: str = None) -> str:
    """
    Renders the HTML login page.
    Displays an optional error notification when credentials are invalid.
    """
    notice = (
        f'<p class="subtitle sad">{error_message}</p>'
        if error_message
        else '<p class="subtitle">Log in to see your page!</p>'
    )

    form_html = f"""
    <h1>Classmate Hub</h1>
    {notice}
    <form method="POST" action="/login">
      <label for="username">Username</label>
      <input type="text" id="username" name="username" placeholder="e.g. arjun" required autofocus>

      <label for="password">Password</label>
      <input type="password" id="password" name="password" placeholder="Your password" required>

      <button type="submit" class="btn btn-blue">Log In</button>
    </form>
    """
    return page("Log In", form_html)


# ==============================================================================
# Route Handlers
# ==============================================================================

@login_bp.route("/", methods=["GET"])
def index():
    """
    Landing Page Endpoint.

    Flow:
    - If user already has a 'username' cookie, redirect directly to /account.
    - Otherwise, display the login form.
    """
    if request.cookies.get("username"):
        return redirect("/account")

    return render_login_form()


@login_bp.route("/login", methods=["POST"])
def login():
    """
    Authentication Processing Endpoint.

    Flow:
    1. Extract 'username' and 'password' from POST form submission.
    2. Construct the SQL query using direct string concatenation (INTENTIONAL SQLi).
    3. Execute query on SQLite.
    4. If no matching row is returned, show error message on login page.
    5. If match found, set 'username' cookie on response and redirect to /account.
    """
    username = request.form.get("username", "")
    password = request.form.get("password", "")

    # -------------------------------------------------------------------------
    # INTENTIONAL VULNERABILITY (SQL Injection):
    # Unsanitized string concatenation enables SQL injection exploits.
    # Safe equivalent would be:
    #   cursor.execute("SELECT * FROM accounts WHERE username = ? AND password = ?", (username, password))
    # -------------------------------------------------------------------------
    check_query = (
        "SELECT * FROM accounts WHERE username = '"
        + username
        + "' AND password = '"
        + password
        + "'"
    )

    db = get_db()
    cursor = db.cursor()

    try:
        cursor.execute(check_query)
        match = cursor.fetchone()
    except Exception:
        # Catch SQL syntax errors triggered by malformed injection attempts
        match = None

    # Invalid credentials or failed query
    if not match:
        return render_login_form("That username/password didn't match. Try again!")

    # Login successful: issue session cookie and redirect
    response = make_response(redirect("/account"))
    response.set_cookie("username", match["username"])
    return response
