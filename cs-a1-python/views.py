"""
views.py
--------
Shared HTML page layout generator.

Provides a unified shell for all pages in Classmate Hub, wrapping content
inside a centered card (<div class="card">) and linking the global stylesheet.
"""

PAGE_TEMPLATE = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>__TITLE__ - Classmate Hub</title>
  <link rel="stylesheet" href="/public/style.css">
</head>
<body>
  <div class="wrapper">
    <div class="card">
      __CONTENT__
    </div>
  </div>
</body>
</html>"""


def page(title: str, content: str) -> str:
    """
    Wraps the given page title and HTML content inside the global template.

    Args:
        title (str): The page title displayed in the browser tab.
        content (str): The HTML string to render inside the central card.

    Returns:
        str: Fully rendered HTML document.
    """
    return PAGE_TEMPLATE.replace("__TITLE__", title).replace("__CONTENT__", content)
