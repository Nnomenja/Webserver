#!/usr/bin/env python3

"""
Usage:
    python3 t.py tests.txt

Test file format:
    METHOD PORT HOST PATH EXPECT STATUS

Example:
    GET 1234 127.0.0.1 / EXPECT 200
    GET 1234 127.0.0.1 /missing EXPECT 404
"""

import sys
import http.client
import socket

TIMEOUT = 5

# ANSI colors
GREEN = "\033[92m"
RED = "\033[91m"
RESET = "\033[0m"


def http_request(method, host, port, path):
    conn = http.client.HTTPConnection(host, port, timeout=TIMEOUT)
    conn.request(method, path)
    response = conn.getresponse()
    conn.close()
    return response.status


def load_tests_file(filename):
    tests = []

    with open(filename, "r") as f:
        for lineno, line in enumerate(f, start=1):
            line = line.strip()

            if not line or line.startswith("#"):
                continue

            parts = line.split()

            if len(parts) != 6 or parts[4] != "EXPECT":
                raise ValueError(f"[line {lineno}] invalid format: {line}")

            method = parts[0].upper()
            port = int(parts[1])
            host = parts[2]
            path = parts[3]
            expected = int(parts[5])

            tests.append((lineno, method, host, port, path, expected))

    return tests


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 t.py tests.txt")
        sys.exit(1)

    tests = load_tests_file(sys.argv[1])

    ok_count = 0
    ko_count = 0

    for lineno, method, host, port, path, expected in tests:
        try:
            status = http_request(method, host, port, path)

            if status == expected:
                print(f"{GREEN}OK{RESET}  {method} {path} -> {status}")
                ok_count += 1
            else:
                print(
                    f"{RED}KO{RESET}  {method} {path} "
                    f"(expected {expected}, got {status})"
                )
                ko_count += 1

        except socket.timeout:
            print(f"{RED}KO{RESET}  {method} {path} -> TIMEOUT")
            ko_count += 1

        except ConnectionRefusedError:
            print(f"{RED}KO{RESET}  {method} {path} -> CONNECTION REFUSED")
            ko_count += 1

    print("\n====== SUMMARY ======")
    print(f"OK: {ok_count}")
    print(f"KO: {ko_count}")

    sys.exit(0 if ko_count == 0 else 1)


if __name__ == "__main__":
    main()