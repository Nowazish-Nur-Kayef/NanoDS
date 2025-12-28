# Security Policy

## Supported Versions

We actively support and provide security updates for the following versions:

| Version | Supported |
| --- | --- |
| 0.0.x | ✅ Yes |
| < 0.0.1 | ❌ No |

## Our Security Guarantees

**NanoDS** is built with a "Security-First" mindset. We implement:

* **Arithmetic Overflow Protection:** Every allocation is checked to prevent buffer overflows.
* **Memory Sanitization:** Tools to zero-out sensitive data before freeing RAM.
* **Leak Prevention:** Safe wrappers for `realloc` to ensure memory integrity.

## Reporting a Vulnerability

If you discover a security vulnerability within this project, please **do not open a public issue.** Instead, help us protect the community by reporting it privately.

1. **Email:** Reach out to nawazishkayef@gmail.com with the subject `SECURITY VULNERABILITY - NanoDS`.
2. **Details:** Include a brief description, a Proof of Concept (PoC) code snippet, and the potential impact.
3. **Response:** You will receive an acknowledgment within 48 hours. We aim to provide a fix or mitigation within 7 days.

## Security Audits

Encouraged to run `make valgrind` before integrating **NanoDS** into production environments to ensure zero memory leaks in their specific implementation.
