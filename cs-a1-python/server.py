import os
from app import app, init_db

if __name__ == "__main__":
    init_db()
    port = int(os.environ.get("PORT", 3000))
    print(f"Classmate Hub is running on http://localhost:{port}")
    app.run(host="0.0.0.0", port=port, debug=True)
