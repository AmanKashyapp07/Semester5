# CS_Lab_1 - Group 7 Submission (Python Tech Stack)

A Python/Flask port of the Classmate Hub web application for Cyber Security Lab 1.

## Getting Started

1. Create and activate a virtual environment:

   ```bash
   python3 -m venv venv
   source venv/bin/activate
   ```

2. Install dependencies:

   ```bash
   pip install -r requirements.txt
   ```

3. Start the server:

   ```bash
   python app.py
   ```

4. Open your browser to [http://localhost:3000](http://localhost:3000)

The database (`classmates.db`) is created automatically the first time you run the app, pre-seeded with sample accounts (`arjun`, `meera`, `kabir`, `zara`).

## Project Structure

```
cs-a1-python/
├── app.py                 # Flask app entry point and configuration
├── server.py              # Alternative entry point
├── db.py                  # SQLite database setup & connection management
├── views.py               # Shared HTML layout wrapper
├── routes/
│   ├── login.py           # Login page and authentication (SQLi lab target)
│   ├── account.py         # Account dashboard and logout
│   ├── message.py         # Encrypted personal message page
│   └── password.py        # Change password page
├── public/
│   ├── crypto.js          # Client-side WebCrypto AES-GCM helper
│   └── style.css          # Visual styling
└── requirements.txt       # Python dependencies (Flask)
```

## Configuration

By default the app runs on port `3000`. To use a different port, set the `PORT` environment variable before starting:

```bash
PORT=8080 python app.py
```
