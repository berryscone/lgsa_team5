# ADR 6: Using Django
Vehicle Query Server should provide vehicle detail information via network.

## Decision 
We decided to use Python Django framework to develop the server

## Rationale 
- We have a man who has experience with Django framework.
- It is easy to make simple REST server using python than develop C++ TCP server.
- Django gives ORM mechanism to query database.

## Status
Accepted

## Consequences
Team members can focus relatively more on client application as it is quite easy to implement the server.
