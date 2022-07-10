# ADR 1: Adapt MVC pattern using Qt C++ framework
ALPR client application requires to provide GUI to the users and needs high level functionality.

## Decision 
We will use Qt framework for ALPR client application development.

## Rationale 
- As some of the team members have experience in application development using Qt framework, it would lead to faster development.
- Qt provides many high level functionalities such as network, authentication and multimedia. These fetures are required to develop ALPR client application.

## Status
Accepted

## Consequences
- Pros:
  - Fast GUI development using widgets and QML UI elements.
  - Qt signal/slot system make communication between multiple objects and threads easy.
- Cons:
  - Additional platform specific modules are included which can increase application size and memory consumption.