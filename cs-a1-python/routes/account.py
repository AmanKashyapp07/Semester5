"""
routes/account.py
-----------------
Handles the authenticated user's dashboard (/account) and logout (/logout).

Key concepts:
1. Authentication: Based on a simple 'username' cookie set during login.
2. Zero-Knowledge Message Decryption:
   The encrypted message (ciphertext) and IV are stored in SQLite and sent
   to the browser. The server never sees the plaintext or the decryption key!
   Instead, client-side JavaScript in the browser uses `crypto.js` to derive
   a key from the password (using SHA-256) and decrypts the ciphertext locally.
"""

from flask import Blueprint, request, redirect, make_response
from db import get_db
from views import page

account_bp = Blueprint("account", __name__)


# ==============================================================================
# UI Component Helpers (Generates HTML for the dashboard)
# ==============================================================================

def render_locked_message_box(ciphertext: str, iv: str) -> str:
    """
    Renders the locked message UI and injects client-side decryption script.

    The script:
    - Waits for the user to type their password into #unlockPwd and click #unlockBtn.
    - Calls hashPassword(pwd) from /public/crypto.js (SHA-256 -> AES-GCM Key).
    - Calls decryptData(key, ciphertext, iv) via WebCrypto API.
    - Displays the decrypted plaintext in the message box.
    """
    return f"""
    <div class="message-box" id="msgBox">
      <p>Message is locked.</p>
      <input type="password" id="unlockPwd" placeholder="Enter your password">
      <button id="unlockBtn" class="btn btn-yellow">Unlock</button>
    </div>

    <!-- Client-side WebCrypto helper -->
    <script src="/public/crypto.js"></script>
    <script>
      const ciphertext = "{ciphertext}";
      const iv = "{iv}";

      document.getElementById('unlockBtn').addEventListener('click', async () => {{
        const pwd = document.getElementById('unlockPwd').value;
        try {{
          // 1. Derive AES key from entered password
          const key = await hashPassword(pwd);

          // 2. Decrypt ciphertext using key and IV in browser memory
          const plain = await decryptData(key, ciphertext, iv);

          // 3. Render decrypted plaintext into the DOM
          document.getElementById('msgBox').innerHTML = '<strong>Your message:</strong><br>' + plain;
          document.getElementById('msgBox').classList.remove('empty');
        }} catch(e) {{
          alert('Failed to decrypt. Wrong password?');
        }}
      }});
    </script>
    """


def render_empty_message_box() -> str:
    """Renders a placeholder box when the user hasn't set an encrypted message yet."""
    return '<div class="message-box empty">No message set yet.</div>'


def render_account_page(display_name: str, message_block: str) -> str:
    """Wraps the dashboard layout with greeting, message block, and action buttons."""
    content = f"""
    <h1>Hi, {display_name}!</h1>
    {message_block}
    <div class="button-row">
      <a href="/set-message" class="btn btn-yellow">Set My Message</a>
      <a href="/change-password" class="btn btn-green">Change Password</a>
    </div>
    <a href="/logout" class="btn btn-pink" style="margin-top: 14px; display:inline-block;">Log Out</a>
    """
    return page("My Page", content)


# ==============================================================================
# Route Handlers
# ==============================================================================

@account_bp.route("/account", methods=["GET"])
def account():
    """
    User Dashboard Endpoint.

    Flow:
    1. Read 'username' cookie to identify the current session.
    2. If cookie is missing, redirect user to the login page ('/').
    3. Query SQLite for the user's account details.
    4. If the account doesn't exist in DB, clear the invalid cookie and redirect.
    5. If a message exists, render the locked box; otherwise render empty state.
    """
    # 1. Check authentication cookie
    username = request.cookies.get("username")
    if not username:
        return redirect("/")

    # 2. Fetch user record from database
    db = get_db()
    cursor = db.cursor()
    cursor.execute("SELECT * FROM accounts WHERE username = ?", (username,))
    user = cursor.fetchone()

    # 3. Handle case where cookie username doesn't exist in DB
    if not user:
        resp = make_response(redirect("/"))
        resp.delete_cookie("username")
        return resp

    # 4. Prepare message box UI (locked vs empty)
    if user["message"]:
        message_block = render_locked_message_box(
            ciphertext=user["message"],
            iv=user["iv"] or "",
        )
    else:
        message_block = render_empty_message_box()

    # 5. Render and return full dashboard HTML
    return render_account_page(
        display_name=user["display_name"],
        message_block=message_block,
    )


@account_bp.route("/logout", methods=["GET"])
def logout():
    """
    Logout Endpoint.

    Flow:
    1. Clears the 'username' session cookie from the browser.
    2. Redirects back to the login page ('/').
    """
    resp = make_response(redirect("/"))
    resp.delete_cookie("username")
    return resp
