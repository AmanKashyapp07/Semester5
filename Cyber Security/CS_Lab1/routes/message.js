const express = require("express");
const router = express.Router();
const db = require("../db");
const { page } = require("../views");

router.get("/set-message", (req, res) => {
  if (!req.cookies.username) {
    return res.redirect("/");
  }

  res.send(page("Set My Message", `
    <h1>✏️ Set My Message</h1>
    <p class="subtitle">This will be encrypted client-side before saving.</p>
    <script src="/public/crypto.js"></script>
    <form id="message-form" method="POST" action="/set-message">
      <label>Your message</label>
      <input type="text" id="message-input" name="message" placeholder="Say something fun!" required autofocus>
      
      <label>Your password (for encryption)</label>
      <input type="password" id="password-input" placeholder="Enter your account password" required>
      
      <input type="hidden" id="iv-input" name="iv">
      
      <button type="submit" class="btn btn-yellow">Save Message 💾</button>
    </form>
    <script>
      document.getElementById("message-form").addEventListener("submit", async function(e) {
        e.preventDefault();
        const plaintext = document.getElementById("message-input").value;
        const password = document.getElementById("password-input").value;
        
        try {
          const encrypted = await encryptMessage(plaintext, password);
          document.getElementById("message-input").value = encrypted.ciphertext;
          document.getElementById("iv-input").value = encrypted.iv;
          document.getElementById("password-input").value = "";
          this.submit();
        } catch (err) {
          alert("Encryption failed: " + err.message);
        }
      });
    </script>
    <a href="/account" class="btn btn-pink" style="margin-top: 14px; display:inline-block;">Back</a>
  `));
});

router.post("/set-message", (req, res) => {
  if (!req.cookies.username) {
    return res.redirect("/");
  }

  const ciphertext = req.body.message;
  const iv = req.body.iv;

  db.prepare("UPDATE accounts SET message = ?, iv = ? WHERE username = ?").run(
    ciphertext,
    iv,
    req.cookies.username
  );

  res.redirect("/account");
});

module.exports = router;
