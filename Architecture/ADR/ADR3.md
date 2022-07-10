# ADR 3: ALPR system to have frame rates at 25 fps
ALPR system shows a playback of videos selected by user

## Decision 
We will have 5secs pre-buffering of frames during initial loading of video and will continue to fill the buffer while the video is getting played.

## Rationale 
Frame rates could be managed at 25fps as there would be already pre cached frames available with parsed license number available. As its been observed that some frames take long to process and identify the required license number.

## Status
Proposed

## Consequences
- Pros
  - Better frame rates due to pre cached frames(bound queue sizes).
  - Reduced number of partial/invalid/duplicate vehicle number hits to server to gather vehicle information(prioritize events).
  - Even the network lost/server unavailable for 5 secs, this pre-cached buffer will help is keeping the user interactions active and also cache the required server queries. (Store & forward)
- Cons
  - Seek operations during initial buffering of video will cause concerns in managing the pre-cached buffer (seek operation could be stopped until the whole video is played completely or should ask user to wait until the pre-cache buffer is filled with required frames).