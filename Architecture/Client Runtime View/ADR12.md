# ADR 12: Using Https

ALPR system needs to secure communication between laptop app and server. 

## Decision 

We will use the Https between laptop(client) and server

## Rationale 
- Research for the ways to use HTTPS on django and available web server
- Apply HTTPS on django and web server
- Check whether it is secure

## Status

Accepted

## Consequences

- HTTPS is easily applicable on Nginx + Django
- Self signed certificate is needed and it can be created using openssl
- We proved secure via Wireshark packet sniffing
