HTTP clients can send incomplete requests in various ways. The incompleteness may occur at different stages of the HTTP message, and a server like Nginx must detect each scenario and respond appropriately. Below is a breakdown of the common types of incomplete requests and how Nginx handles them.

## 1. Incomplete Request Line
The request line is the first line of an HTTP request (e.g., `GET /index.html HTTP/1.1`). It can be incomplete if:
- Only part of the method is sent (e.g., `GE`).
- The URI is missing or truncated (e.g., `GET /ind`).
- The HTTP version is missing (e.g., `GET /index.html` without version).
- The line ends prematurely (e.g., only `GET` and a space, then connection stalls).

**Nginx handling**:  
Nginx reads the request line byte by byte. If the line is not terminated by `\r\n` within the `client_header_timeout` period, the connection times out and a **408 (Request Timeout)** is returned. If the received data is syntactically invalid (e.g., no space after method), Nginx may return a **400 (Bad Request)** immediately.

## 2. Partial Header Block
After the request line, the client sends headers, each ending with `\r\n`, terminated by an empty line (`\r\n`). Incompleteness can occur as:
- Only some headers are received (e.g., `Host: example.com\r\nUser-Agent: ` and then no more data).
- A header line is truncated (e.g., `Content-Length: 5` without the trailing `\r\n`).
- The final blank line is missing (the client never sends the empty line that signals the end of headers).

**Nginx handling**:  
Nginx continues reading until either:
- All headers are received (double `\r\n` detected),
- The buffer for headers is exhausted (controlled by `large_client_header_buffers`), or
- The `client_header_timeout` expires.

If headers are incomplete when the timeout hits, Nginx returns **408**. If the headers exceed buffer limits, a **400** or **494** (Request Header Too Large) is returned. If the client sends a partial header line and then stops, the timeout eventually triggers a 408.

## 3. Missing Mandatory Headers
Even if all header lines are fully received, the request may be considered incomplete from a protocol perspective if required headers are absent. For example:
- In HTTP/1.1, the `Host` header is mandatory.
- A request with `Content-Length` but no body might be missing the body.

**Nginx handling**:  
Nginx validates headers after the complete header block is received. If a mandatory header is missing (like `Host`), it responds immediately with a **400 (Bad Request)** without waiting for a body. This is not a timeout but a protocol error.

## 4. Incomplete Body (with `Content-Length`)
If the request includes a `Content-Length` header, Nginx expects exactly that many bytes in the body. Incompleteness happens when:
- The client sends fewer bytes than advertised and then stops sending (without closing the connection).
- The client sends data slower than the body timeout.

**Nginx handling**:  
Nginx reads the body using `client_body_timeout` between reads. If the full number of bytes is not received within that time, it returns **408 (Request Timeout)**. If the client closes the connection prematurely before sending the full body, Nginx logs a **499** (client closed connection) and discards the request.

## 5. Incomplete Chunked Body
When `Transfer-Encoding: chunked` is used, the body is sent in chunks, each with a size line, data, and trailing `\r\n`, ending with a zero-size chunk. Incompleteness can be:
- A chunk size line is incomplete (e.g., only `4\r`).
- A chunk’s data is truncated (e.g., only 2 bytes of a promised 4-byte chunk).
- The final zero chunk or its trailing `\r\n` is missing.

**Nginx handling**:  
Nginx parses the chunked encoding incrementally. If a chunk is incomplete, it waits for more data. If no data arrives within `client_body_timeout`, it returns **408**. If the chunked format is malformed (e.g., invalid hex size), Nginx may return a **400** immediately.

## 6. Premature Connection Closure
The client may close the TCP connection at any time while sending the request (e.g., user cancels, network loss). This results in an incomplete request because the connection is gone.

**Nginx handling**:  
When Nginx attempts to read and gets an EOF or connection reset, it aborts the request. In the access log, this is typically logged with status **499** (custom Nginx code for “client closed connection”). No response is sent because the connection is already closed.

## Summary of Nginx Responses to Incomplete Requests
| Scenario                                      | Nginx Response / Action                         |
|-----------------------------------------------|-------------------------------------------------|
| Partial request line or headers (timeout)     | 408 Request Timeout                             |
| Malformed request line or headers             | 400 Bad Request                                 |
| Missing mandatory header (e.g., Host)         | 400 Bad Request                                 |
| Headers too large                             | 400 / 494 Request Header Too Large              |
| Partial body (timeout)                        | 408 Request Timeout                             |
| Partial body (client disconnect)               | 499 Client Closed Connection (log only)         |
| Incomplete chunked body (timeout)             | 408 Request Timeout                             |
| Malformed chunked data                        | 400 Bad Request                                 |

By distinguishing between these types, Nginx can apply the correct error response and avoid hanging resources. Configuration directives like `client_header_timeout`, `client_body_timeout`, `client_max_body_size`, and `large_client_header_buffers` give administrators control over how long to wait and how much to accept before treating a request as incomplete.