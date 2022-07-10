# ADR 3: Proxy pattern for network
ALPR client application needs to communicate with Vehicle Query server.

## Decision 
We decided to introduce NetworkManager to communicate with Vehicle Query server.

## Rationale 
- Two types of network requests required: vehicle query and login.
- Response of vehicle query request can be stored locally not to request several times for the same vehicle.
- The module which requires network communication don't have to know what exactly happen behind.

## Status
Accepted

## Consequences
As the module which requires network functionality only needs to know NetworkManager, it makes the application more modifiable.