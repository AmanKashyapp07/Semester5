"""
routes/message.py
-----------------
Handles creating and updating the user's encrypted personal message.

Client-Side Cryptography Architecture:
1. When the user visits GET `/set-message`, an HTML form is rendered with inputs
   for password, plaintext message, and hidden inputs for ciphertext and IV.
2. Form submission is intercepted in JavaScript (`msgForm.addEventListener('submit')`).
3. JavaScript uses WebCrypto (via `/public/crypto.js`):
   - Computes SHA-256 hash of the password to create an AES key.
   - Generates a random 12-byte initialization vector (IV).
   - Encrypts the message using AES-GCM.
   - Stores the resulting hex-encoded ciphertext and IV into hidden input fields.
   - Removes the 'name' attributes from the plaintext and password inputs so they
     are NOT submitted over HTTP!
4. The server receives ONLY the ciphertext and the IV, which it saves into SQLite.
"""

from flask import Blueprint, request, redirect
from db import get_db
from views import page

message_bp = Blueprint("message", __name__)


# ==============================================================================
# UI Component Helpers
# ==============================================================================

def render_set_message_form() -> str:
    """
    Renders the message form and embeds the client-side encryption script.
    """
    content = """
    <h1>Set My Message</h1>
    <p class="subtitle">This will be encrypted in your browser before it's saved.</p>

    <form id="msgForm" method="POST" action="/set-message">
      <label for="password">Your password</label>
      <input type="password" id="password" placeholder="Your password" required autofocus>

      <label for="message">Your message</label>
      <input type="text" id="message" placeholder="Say something fun!" required>

      <!-- Hidden fields populated by client-side JavaScript -->
      <input type="hidden" name="message" id="hiddenMessage">
      <input type="hidden" name="iv" id="hiddenIv">

      <button type="submit" class="btn btn-yellow">Encrypt & Save</button>
    </form>

    <a href="/account" class="btn btn-pink" style="margin-top: 14px; display:inline-block;">Back</a>

    <!-- Client-side WebCrypto helper script -->
    <script src="/public/crypto.js"></script>
    <script>
      document.getElementById('msgForm').addEventListener('submit', async (e) => {
        // 1. Prevent normal submission until client-side encryption completes
        e.preventDefault();

        const pwd = document.getElementById('password').value;
        const msg = document.getElementById('message').value;

        try {
          // 2. Hash password with SHA-256 to derive AES-GCM cryptographic key
          const key = await hashPassword(pwd);

          // 3. Encrypt message with AES-GCM and a random 12-byte IV
          const { ciphertext, iv } = await encryptData(key, msg);

          // 4. Populate hidden form fields with the encrypted payload
          document.getElementById('hiddenMessage').value = ciphertext;
          document.getElementById('hiddenIv').value = iv;

          // 5. IMPORTANT: Remove names so password and plaintext are never sent to the server
          document.getElementById('password').removeAttribute('name');
          document.getElementById('message').removeAttribute('name');

          // 6. Submit the form carrying only ciphertext and IV
          e.target.submit();
        } catch (err) {
          alert('Encryption failed.');
        }
      });
    </script>
    """
    return page("Set My Message", content)


# ==============================================================================
# Route Handlers
# ==============================================================================

@message_bp.route("/set-message", methods=["GET"])
def set_message_view():
    """
    Renders the encrypted message composition form.
    Requires user to be logged in via 'username' cookie.
    """
    if not request.cookies.get("username"):
        return redirect("/")

    return render_set_message_form()


@message_bp.route("/set-message", methods=["POST"])
def set_message():
    """
    Persists the encrypted message to SQLite.

    Flow:
    1. Verify user session via 'username' cookie.
    2. Extract 'message' (ciphertext) and 'iv' from POST body.
    3. Run parameterized UPDATE query in SQLite.
    4. Commit transaction and redirect back to /account.
    """
    username = request.cookies.get("username")
    if not username:
        return redirect("/")

    ciphertext = request.form.get("message")
    iv = request.form.get("iv")

    db = get_db()
    cursor = db.cursor()

    # Parameterized query protects against SQL injection
    cursor.execute(
        "UPDATE accounts SET message = ?, iv = ? WHERE username = ?",
        (ciphertext, iv, username),
    )
    db.commit()

    return redirect("/account")
