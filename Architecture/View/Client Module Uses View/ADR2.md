# ADR 2: Separate Frame Generator and ALPR modules
ALPR client application should show selected video with at least 25 fps.

## Decision 
We decided to separate Frame Generator and ALPR modules.

## Rationale 
- Frame Generator have to get frames from selected video in real time.
- ALPR algorithm requires a lot of CPU resource.

## Status
Accepted

## Consequences
Frame Generator only get frames from video file and provide them to ALPR module.
This separation makes the client app more stable when heavy resources are needed to run ALPR algorithm.