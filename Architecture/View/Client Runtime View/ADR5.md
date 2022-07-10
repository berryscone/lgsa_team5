# ADR 5: Proxy pattern for local cache
The same license plate may be detected multiple times

## Decision 
We decided to introduce NetworkProxy to hide existence of local cache to NetworkManager

## Rationale 
- NetworkManager does not need to know about the existence of the cache.

## Status
Accepted

## Consequences
Modifiability increases due to the introduction of NetworkProxy
Performance increses due to local cache.