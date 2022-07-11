# ADR 10: Enhance ALPR system to support camera live feed

ALPR system currently supports loading videos/images. However as an enhancement ALPR system could be integrated with camera to have live video feeds.

## Decision 

We will have multiple instances of FrameGenerator component to handle multiple kinds of inputs like (FrameImageGenerator, FrameVideoGeneration, FrameLiveGenerator etc)

## Rationale 

- Creating different components to handle different kinds of input will help in separation of concerns and allow extensibility.

## Status

Proposed

## Consequences

Pro:

- Provide an interface for future extensions of inputs.

Cons:

- Would require code changes to support this behavior(not configurable).