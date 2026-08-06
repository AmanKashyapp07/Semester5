const express = require("express");
const router = express.Router();
const db = require("../db");
const { page } = require("../views");

router.get("/account", (req, res) => {
  if (!req.cookies.username) {
    return res.redirect("/");
  }

  const me = db.prepare("SELECT * FROM accounts WHERE username = ?").get(req.cookies.username);
  if (!me) {
    res.clearCookie("username");
    return res.redirect("/");
  }

  let messageBlock = `<div class="message-box empty">💬 No message set yet.</div>`;

  if (me.message && me.iv) {
    messageBlock = `
      <script src="/public/crypto.js"></script>
      <div id="locked-box" class="message-box">
        💬 <strong>${me.display_name}'s message is encrypted 🔒</strong>
        <div style="margin-top: 10px;">
          <input type="password" id="unlock-password" placeholder="Enter password to unlock" style="margin-bottom: 8px;">
          <button type="button" id="unlock-btn" class="btn btn-blue">Unlock 🔓</button>
        </div>
        <p id="unlock-error" class="subtitle sad" style="display:none; color: #d32f2f; margin-top: 8px;">
          😕 Decryption failed. Incorrect password.
        </p>
      </div>

      <div id="unlocked-box" class="message-box" style="display: none;">
        💬 <strong>${me.display_name}'s message:</strong><br>
        <span id="decrypted-text" style="font-weight: bold; color: #1b5e20; word-break: break-word;"></span>
      </div>

      <script>
        document.getElementById("unlock-btn").addEventListener("click", async () => {
          const password = document.getElementById("unlock-password").value;
          const errorEl = document.getElementById("unlock-error");
          errorEl.style.display = "none";
          
          try {
            const ciphertext = ${JSON.stringify(me.message)};
            const iv = ${JSON.stringify(me.iv)};
            const plaintext = await decryptMessage(ciphertext, iv, password);
            document.getElementById("decrypted-text").innerText = plaintext;
            document.getElementById("locked-box").style.display = "none";
            document.getElementById("unlocked-box").style.display = "block";
          } catch (err) {
            errorEl.style.display = "block";
          }
        });
      </script>
    `;
  }

  res.send(page("My Page", `
    <h1>👋 Hi, ${me.display_name}!</h1>
    ${messageBlock}
    <div class="button-row">
      <a href="/set-message" class="btn btn-yellow">✏️ Set My Message</a>
      <a href="/change-password" class="btn btn-green">🔑 Change Password</a>
    </div>
    <a href="/logout" class="btn btn-pink" style="margin-top: 14px; display:inline-block;">Log Out</a>
  `));
});

router.get("/logout", (req, res) => {
  res.clearCookie("username");
  res.redirect("/");
});

module.exports = router;
