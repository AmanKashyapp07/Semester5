import unittest
import sqlite3
from app import app, init_db
from db import DB_PATH


class TestClassmateHub(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        init_db()

    def setUp(self):
        self.client = app.test_client()

    def test_01_index_page(self):
        resp = self.client.get("/")
        self.assertEqual(resp.status_code, 200)
        self.assertIn(b"Classmate Hub", resp.data)
        self.assertIn(b"Log In", resp.data)

    def test_02_static_files(self):
        css = self.client.get("/public/style.css")
        self.assertEqual(css.status_code, 200)
        self.assertIn(b".card", css.data)

        crypto = self.client.get("/public/crypto.js")
        self.assertEqual(crypto.status_code, 200)
        self.assertIn(b"hashPassword", crypto.data)

    def test_03_login_failure(self):
        resp = self.client.post(
            "/login",
            data={"username": "arjun", "password": "WrongPassword"},
        )
        self.assertEqual(resp.status_code, 200)
        self.assertIn("didn't match".encode("utf-8"), resp.data)

    def test_04_login_success(self):
        resp = self.client.post(
            "/login",
            data={"username": "arjun", "password": "Football123"},
        )
        self.assertEqual(resp.status_code, 302)
        self.assertEqual(resp.headers.get("Location"), "/account")
        cookie = resp.headers.get("Set-Cookie", "")
        self.assertIn("username=arjun", cookie)

    def test_05_sqli_login(self):
        # Intentional SQL injection demonstration
        resp = self.client.post(
            "/login",
            data={"username": "arjun' --", "password": "any"},
        )
        self.assertEqual(resp.status_code, 302)
        self.assertEqual(resp.headers.get("Location"), "/account")
        cookie = resp.headers.get("Set-Cookie", "")
        self.assertIn("username=arjun", cookie)

    def test_06_account_without_cookie(self):
        resp = self.client.get("/account")
        self.assertEqual(resp.status_code, 302)
        self.assertEqual(resp.headers.get("Location"), "/")

    def test_07_account_with_cookie(self):
        self.client.set_cookie("username", "meera")
        resp = self.client.get("/account")
        self.assertEqual(resp.status_code, 200)
        self.assertIn(b"Hi, Meera!", resp.data)

    def test_08_set_and_view_message(self):
        self.client.set_cookie("username", "kabir")
        post_resp = self.client.post(
            "/set-message",
            data={"message": "deadbeef1234", "iv": "aabbccddeeff"},
        )
        self.assertEqual(post_resp.status_code, 302)
        self.assertEqual(post_resp.headers.get("Location"), "/account")

        acct_resp = self.client.get("/account")
        self.assertEqual(acct_resp.status_code, 200)
        self.assertIn(b"deadbeef1234", acct_resp.data)
        self.assertIn(b"aabbccddeeff", acct_resp.data)
        self.assertIn(b"Message is locked", acct_resp.data)

    def test_09_change_password(self):
        self.client.set_cookie("username", "zara")
        post_resp = self.client.post(
            "/change-password",
            data={"password": "NewSecretPassword123"},
        )
        self.assertEqual(post_resp.status_code, 302)
        self.assertEqual(post_resp.headers.get("Location"), "/account")

        # Verify password changed in DB
        conn = sqlite3.connect(DB_PATH)
        row = conn.cursor().execute("SELECT password FROM accounts WHERE username = 'zara'").fetchone()
        conn.close()
        self.assertEqual(row[0], "NewSecretPassword123")

    def test_10_logout(self):
        self.client.set_cookie("username", "arjun")
        resp = self.client.get("/logout")
        self.assertEqual(resp.status_code, 302)
        self.assertEqual(resp.headers.get("Location"), "/")


if __name__ == "__main__":
    unittest.main()
