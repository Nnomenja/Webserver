---

# 📄 Config System README

## Overview

This configuration system parses a custom text-based config file describing **server blocks** and their associated **location blocks**.

Each configuration file may contain multiple `server:` sections, each defining:

* Server-level settings (host, port, root, etc.)
* Nested `location:` blocks with route-specific behavior

---

## 📌 File Structure

### Basic Example

```
server:
host 127.0.0.1
port 8080
root /var/www/html
server_name myserver
enable_virtual_hosting ON

location:
path /images
methods GET
root /var/www/images
auto_index ON

location:
path /upload
methods POST
root /var/www/uploads
uploads /uploads
```

---

## 🧱 Server Block

Each `server:` block defines one server configuration.

### Required Fields

| Key  | Description       |
| ---- | ----------------- |
| host | Server IP address |
| port | Port number       |
| root | Root directory    |

If any of these are missing → ❌ Exception thrown

---

### Optional Fields

| Key                    | Description                                      |
| ---------------------- | ------------------------------------------------ |
| enable_virtual_hosting | `ON` / `OFF`                                     |
| max_body_size          | Max request body size (bytes, default: 52428800) |
| server_name            | Domain name                                      |
| error_pages            | Custom error pages                               |

---

### Example

```
server:
host 127.0.0.1
port 8080
root /var/www
server_name example.com
enable_virtual_hosting ON
max_body_size 1000000
error_pages 404 500 /errors/error.html
```

---

### Rules

* Duplicate keys are not allowed
* Each key must appear at most once
* Invalid keys → ❌ `ConfigException`
* Invalid values → ❌ `ConfigException`

---

## 📍 Location Block

Each `location:` defines behavior for a specific route.

---

### Required Fields (depending on type)

| Key     | Description          |
| ------- | -------------------- |
| path    | URL path             |
| methods | Allowed HTTP methods |
| root    | Directory            |

---

### Optional Fields

| Key         | Description              |
| ----------- | ------------------------ |
| index       | Index files              |
| uploads     | Upload directory         |
| auto_index  | ON / OFF                 |
| error_pages | Custom error pages       |
| return      | Redirect (code + target) |
| CGI         | CGI handler              |

---

## 🔀 Location Types

The parser determines the type automatically:

---

### 1. 📁 STATIC

Default type

Requirements:

* methods ✔
* path ✔
* root ✔

---

### 2. 📤 UPLOAD

Triggered when `uploads` is set

Rules:

* Only `POST` allowed
* No `auto_index`
* No `index`
* No CGI

---

### 3. ⚙️ DYNAMIC (CGI)

Triggered when `CGI` is enabled

Example:

```
CGI .py /usr/bin/python3
```

---

### 4. 🔁 REDIRECTION

Triggered when `return` is set

Example:

```
return 301 http://example.com
```

Rules:

* No `methods`
* No `root`

---

## 🧪 Validation Rules

### General

* Unknown keys → ❌ error
* Duplicate keys → ❌ error
* Invalid syntax → ❌ error

---

### Server-Level Checks

* Ports must be unique
* Server names must be unique
* If virtual hosting is ON → `server_name` is required

---

### Location-Level Checks

* Paths must be unique per server
* Upload mode restrictions enforced
* Redirection rules enforced

---

## ⚠️ Error Handling

All errors throw:

```
ConfigException: <message>
```

Examples:

* `wrong key -> 'invalidKey'`
* `duplicate port`
* `host field should be filled`
* `invalid URI -> '/bad/path'`

---

## 💡 Supported Keys Summary

### Server Keys

```
host
port
enable_virtual_hosting
root
max_body_size
error_pages
server_name
```

---

### Location Keys

```
methods
error_pages
path
index
root
uploads
auto_index
return
CGI
```

---

## 🧰 Internal Behavior

* File is parsed line-by-line
* Comments (`#`) are ignored
* Each `server:` starts a new configuration block
* `location:` splits sub-blocks inside a server
* Data is stored in:

  * `configs` (vector of server configs)
  * `locations` inside each server

---

## 🚀 Tips

* Always define `host`, `port`, and `root`
* Avoid duplicate ports or server names
* Keep location paths unique
* Use `POST` only for uploads
* Use `CGI` only when needed

---


