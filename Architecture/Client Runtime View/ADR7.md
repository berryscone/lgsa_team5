# ADR 7: Pipe & filter pattern for frame generation 

ALPR system should consider videos inputs recorded during various environment conditions( like rain, snow, sunny. motion etc).

## Decision 
FrameGenerator component will use pipe and filter technique to apply optimization techniques on frame one by one to remove blur effect.

## Rationale 
- As the frame rendering would require various OpnenCV APIs(like Laplacian, Sobel etc.) to get smooth and better quality image. It would be better to have them applied in phases such that they could be added removed as required for optimization. Like:

  Original Frame > Detect & Remove Blur > Remove Noise > Smooth Image > Identify Vehicle No > Highlight the region of Identified Vehicle No > Generated Frame

## Status
Proposed

## Consequences
Pro:

- Smooth frames are created which will increase the accuracy of OpenCV based recognition of license number.
- This approach can help in adding additional filter techniques if required in future.( increase modifiability)

Cons:

- Latency is getting the license number identified frame(impact frame rates).