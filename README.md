*This project has been created as part of the 42 curriculum by aravelom, nnomenja, toloandr.*

# Webserv

## Description

**Webserv** is a custom HTTP server written entirely in **C++98** as part of the 42 curriculum.

The goal of this project is to understand how web servers work internally by implementing a fully functional HTTP server capable of communicating with a real web browser. The server is responsible for receiving HTTP requests, processing them, and serving appropriate responses such as HTML pages, images, or dynamically generated content.

This project focuses on several core concepts:

* HTTP protocol fundamentals
* Non-blocking I/O
* Socket programming
* Event-driven architecture using `epoll`
* Configuration parsing
* CGI execution
* Static file serving
* Request parsing and response generation

The project combines **system programming**, **network programming**, and **web technologies** while respecting the constraints of the **C++98 standard**.

---

# Features

* HTTP/1.1 server implementation
* Non-blocking server using `epoll`
* HTTP request parsing
* HTTP response generation
* Static file serving
* CGI support
* MIME type management
* Custom configuration files
* Error handling and custom error pages
* File uploads
* Directory listing support
* HTTP redirections
* Multiple client connection handling
* Request logging system

---

# Instructions

## Requirements

The project requires:

* A POSIX-compatible operating system (Linux recommended)
* `make`
* A C++ compiler supporting the C++98 standard (`c++`, `g++`, or `clang++`)

---

## Compilation

Run the following command at the root of the repository:

```bash
make
```

Useful Makefile targets:

```bash
make
make re
make clean
make fclean
```

---

## Execution

The generated executable is:

```bash
./webserv [CONFIG_FILE]
```

Example:

```bash
./webserv Config/default.conf
```

Replace `[CONFIG_FILE]` with the configuration file you want to use.

---

# Project Structure

```text
Project root:
├── Makefile
├── main.cpp
├── README.md

├── Core/
│   ├── HTTP/
│   │   ├── CgiParser.*
│   │   ├── Webserv.*
│   │   ├── MimeTypes.*
│   │   ├── Template.*
│   │   ├── ErrorProcess.*
│   │   ├── Request/
│   │   │   ├── HttpRequestParser.*
│   │   │   ├── RequestProcessor.*
│   │   │   ├── RequestParserState/
│   │   │   └── RequestProcessStrategy/
│   │
│   ├── Network/
│   │   ├── Epoll.*
│   │   ├── ServerSocket.*
│   │   ├── Socket.*
│   │   └── Signal.*
│   │
│   └── Settings/
│       ├── Config.*
│       ├── File.*
│       ├── Validator.*
│       └── helpers.*

├── Data/
│   ├── Client.*
│   ├── Request.*
│   ├── Response.*
│   ├── Process.*
│   └── RequestLogger.*

├── Config/
├── Enum/
├── Exception/
├── objs/

└── utils/
    ├── getFileContent.cpp
    ├── getListDirectory.cpp
    ├── number_utils.cpp
    ├── path_utils.cpp
    └── time.cpp
```

---

# Technical Choices

## Event-driven architecture

The server uses `epoll` to handle multiple simultaneous client connections efficiently without blocking the execution flow.

## C++98 Constraint

The entire project is implemented using the **C++98** standard, without modern C++ features such as:

* `auto`
* smart pointers
* lambdas
* STL additions from C++11+

This constraint encourages deeper understanding of low-level programming and memory management.

## Request Processing

HTTP requests are parsed through a state-based parser architecture, allowing incremental and structured parsing of incoming client data.

## CGI Support

The server supports CGI execution for generating dynamic content.

---

# Usage Example

Start the server:

```bash
./webserv Config/default.conf
```

Open a browser and visit:

```text
http://localhost:8080
```

You can also test the server using:

```bash
curl http://localhost:8080
```

---

# Resources

## Documentation

* RFC 7230 — Hypertext Transfer Protocol (HTTP/1.1)
* RFC 7231 — HTTP Semantics and Content
* Linux `epoll` manual pages
* Beej's Guide to Network Programming
* MDN Web Docs — HTTP
* cppreference.com (C++98 references)

## Useful Links

* https://developer.mozilla.org/en-US/docs/Web/HTTP
* https://man7.org/linux/man-pages/man7/epoll.7.html
* https://beej.us/guide/bgnet/
* https://en.cppreference.com/

---

# AI Usage

* Quick implementation of helper functions
* Analysing valgrind leaks and fixing them
* Debugging assistance
* Concept explanations
* Code review suggestions
* Architecture discussions
* Documentation writing help


---

# Authors

* aravelom
* nnomenja
* toloandr
