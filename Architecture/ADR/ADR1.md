# ADR 1: Using UI framework for Laptop application
ALPR client application requires application developed in Windows platform and require fast development.

## Decision 
We will use Qt framework for ALPR client application development

## Rationale 
As most of the team members have experience in application development using Qt framework, it would lead to fast development.

## Status
Accepted

## Consequences
- Pro:
  - Fast UI development using widgets and QML UI elements.
  - Qt signal/slot for communication between multiple threads.
  - Cross platform support for future enhancement of application to be used in other OS (like Linux, MacOS etc.)
- Cons:
  - Licensed software for development and deployment.
  - Additional platform specific modules are included which can increase application size and memory consumption.