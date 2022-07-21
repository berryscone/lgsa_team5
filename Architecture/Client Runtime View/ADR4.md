# ADR 4: Concurrency using threads
ALPR client application requires to provide video frames to UI with at least 25 fps.

## Decision 
We will use thread pool to run ALPR algorithm on each frame in a single thread.

## Rationale 
- To play video at least 25 fps, each single frame should be handled in 40 ms.
- In case of running ALPR algorithm for a frame needs more than 40 ms, multiple thread should be used for concurrency.

## Status
Accepted

## Consequences
Even if it takes a long time to apply the ALPR algorithm to the previous frame, the ALPR algorithm can be applied directly to the next frame.
