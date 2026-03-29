Since you are not handling virtual hosting, you can simplify the process by assuming a single server block (or extracting locations from the first server block). Below is a practical guide to parse the configuration, organize location rules, and use them to process incoming requests.

---

## 1. Parsing the Configuration

The configuration file contains a **server** block with nested **location** blocks. You need to parse it and extract:

- Global server directives (like `client_max_body_size`, `error_page`)  
- A list of **location** objects, each with:
  - A **path** (e.g., `/`, `/upload`)  
  - A **modifier** (optional: `=` for exact match, `~` for regex, `^~` for prefix‑no‑regex – if your server supports them)  
  - Its own directives (`root`, `methods`, `autoindex`, `index`, `return`, `proxy_pass`, `upload_enable`, `upload_store`, `cgi_extension`, `cgi_working_dir`)

Because your config does **not** use regex or exact‑match modifiers, we will focus on **prefix‑based** matching with the longest‑prefix rule.

### 1.1. Lexical Structure

The file uses a simple syntax:
- Lines can end with `;` (directives) or `{` (block start).
- Comments start with `#`.
- Blocks are enclosed in `{ ... }`.
- Directives have a name followed by one or more arguments.

A recursive descent parser can read the file token by token (words, braces, semicolons). When you see `server {`, you parse until the matching `}`. Inside, when you see `location`, you parse its path and then the block.

### 1.2. Data Structures

In Python, you could represent a location as:

```python
class Location:
    def __init__(self, path):
        self.path = path          # string like "/upload"
        self.modifier = None      # '=', '~', '^~', None
        self.root = None
        self.methods = []
        self.autoindex = False
        self.index = []
        self.return_ = None       # (status, target)
        self.proxy_pass = None
        self.upload_enable = False
        self.upload_store = None
        self.cgi_extensions = {}  # e.g., {'.php': '/usr/bin/php-cgi'}
        self.cgi_working_dir = None

# Global server settings (from outside any location)
server_settings = {
    'client_max_body_size': None,
    'error_page': {},           # {404: '/404.html', ...}
    'root': None,               # fallback root for locations without one
    'index': [],                # fallback index files
    # ... other defaults
}

locations = []  # list of Location objects
```

During parsing, you will fill `locations` and also store server‑level directives.

---

## 2. Matching a Request to a Location

When an HTTP request arrives, you have:
- **Method** (GET, POST, PUT, etc.)
- **URI** (e.g., `/upload/photo.jpg`)

You need to select the **most specific** location that matches the URI.

### 2.1. Matching Algorithm (Prefix‑Based)

Since your config does not use regex or exact‑match modifiers, you can implement **longest prefix matching**:

1. Iterate over all locations.
2. If the URI starts with the location’s path, it is a candidate.
3. Among all candidates, choose the one with the longest path string.

Example:
- Locations: `/` (root) and `/upload`
- URI: `/upload/photo.jpg` → both match; `/upload` is longer → selected.
- URI: `/documents/report.pdf` → only `/` matches → selected.

**Important**: Trailing slashes matter. Usually, a location path with a trailing slash (e.g., `/upload/`) matches only if the URI has that slash or a subpath. Without a trailing slash, it may also match the exact directory name. Decide a consistent rule; a common approach is to normalize the URI by stripping a trailing slash except for the root.

### 2.2. Handling `return` (Redirect)

If the matched location has a `return` directive, you immediately send the redirect response and stop further processing.

### 2.3. Handling `proxy_pass`

If the matched location has a `proxy_pass`, you forward the request to the backend URL and return its response.

---

## 3. Processing the Request Based on the Matched Location

Once the location is selected, you combine its directives with the global server settings (where the location does not override).

### 3.1. Method Check

If the location has a `methods` list, ensure the request method is in it. Otherwise return `405 Method Not Allowed`.

### 3.2. File Upload (if enabled)

If `upload_enable` is True and the method is `POST` or `PUT`, handle the upload:

- The `upload_store` directory determines where to save the file. It may be absolute or relative to the location’s `root`.
- Read the request body and store it. You might use a filename from the `Content-Disposition` header or generate one.

### 3.3. CGI Script Execution

If the request path matches an extension defined in `cgi_extensions` (e.g., `.php`), and the file exists:

- Determine the interpreter from the mapping.
- Set the working directory to `cgi_working_dir` (if defined, otherwise the directory of the script).
- Execute the script with the appropriate environment variables (REQUEST_METHOD, QUERY_STRING, etc.) and return its output.

### 3.4. Static File Serving

If none of the above apply, serve static files:

1. **Build the filesystem path**:  
   - Use the location’s `root` (if set) or fall back to the global server root.  
   - Append the URI path (after removing the location prefix, if needed – usually the full URI is appended to root).  
   Example: location root `/var/www/html`, URI `/doc/report.pdf` → `/var/www/html/doc/report.pdf`

2. **Check if the path is a directory**:
   - If it ends with a slash or the OS says it’s a directory, try to serve an index file.
   - Use the location’s `index` list (or global) to look for existing files (e.g., `index.html`, `index.htm`).
   - If an index file is found, serve it as a static file.
   - If `autoindex` is `on` and no index file exists, generate a directory listing.
   - If `autoindex` is `off` and no index file, return `403 Forbidden` or `404 Not Found`.

3. **Serve the file**:
   - If the file exists and is readable, send it with the correct MIME type (determined by extension).
   - If not found, return `404 Not Found`.

---

## 4. Example Implementation Sketch (Python)

```python
def parse_config(filepath):
    # Returns (server_settings, locations)
    pass

def match_location(uri, locations):
    best = None
    best_len = -1
    for loc in locations:
        if uri.startswith(loc.path):
            # optionally handle exact match or trailing slash logic
            if len(loc.path) > best_len:
                best = loc
                best_len = len(loc.path)
    return best

def handle_request(method, uri, headers, body):
    # Load config once (global)
    server_settings, locations = parse_config('template.txt')
    loc = match_location(uri, locations)
    if not loc:
        loc = default_location  # e.g., root '/'

    # Merge settings
    root = loc.root or server_settings.get('root', '/var/www/html')
    methods = loc.methods or server_settings.get('methods', ['GET', 'HEAD'])
    if method not in methods:
        return 405, "Method Not Allowed"

    if loc.return_:
        status, target = loc.return_
        return status, {'Location': target}, ""

    if loc.proxy_pass:
        # forward request to backend
        return proxy_request(loc.proxy_pass, method, uri, headers, body)

    if loc.upload_enable and method in ('POST', 'PUT'):
        store_dir = loc.upload_store or root
        # save file...
        return 200, "Uploaded"

    # Check CGI
    for ext, interpreter in loc.cgi_extensions.items():
        if uri.endswith(ext):
            # execute CGI
            return run_cgi(interpreter, uri, root, loc.cgi_working_dir, method, headers, body)

    # Static file
    file_path = os.path.join(root, uri.lstrip('/'))
    if os.path.isdir(file_path):
        # try index files
        for idx in loc.index or server_settings.get('index', ['index.html']):
            index_path = os.path.join(file_path, idx)
            if os.path.isfile(index_path):
                return serve_file(index_path)
        if loc.autoindex:
            return generate_directory_listing(file_path, uri)
        else:
            return 403, "Directory listing forbidden"
    elif os.path.isfile(file_path):
        return serve_file(file_path)
    else:
        return 404, "Not Found"
```

---

## 5. Important Considerations

- **Order of locations** in the config matters only if you implement regex or exact‑match modifiers. With pure prefix matching, you just need the longest match, regardless of order.
- **Trailing slashes** can cause subtle mismatches. Normalize the URI by removing duplicate slashes and maybe stripping trailing slash unless it’s the root.
- **Root inheritance**: If a location does not define its own `root`, it should inherit the server’s `root` (which might be set in a `server` block outside any location). In the example config, the root is set inside each location; only the error pages use a separate root.
- **Methods directive**: The example config sometimes lists `methods` twice (in `/upload`). Your parser should take the last occurrence or merge them – typically the last one wins or they are combined. It’s safer to let later directives override earlier ones.

By implementing these steps, you can parse your configuration and correctly route requests to the appropriate processing logic.