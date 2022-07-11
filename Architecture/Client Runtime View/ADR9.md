# ADR 9: Realtime display of Vehicle number

ALPR system needs to display vehicle number to use in an intuitive way so that user can identify the exact time during which the vehicle was identified and easily identify the vehicle of interest.

## Decision 

We will use a slider along with vehicle numbers of interest as thumbnails so that could could be easily identified during video playback.

## Rationale 

- Having vehicle of interest as thumbnail will help user to seek to particular instance of video and see the surroundings in the frame to easily locate the vehicle.
- It will also help to clearly identify when the exact timestamp when the vehicle was located.
- As an overall view, it will help to visualize the frequency of vehicle of interest during the video duration. For example; if the video was taken for whole day, it can easy identify when majority of vehicle of interest we identified.
- In case slider is enabled to show all vehicle nos, then it could identify the cluster of vehicles identified(i.e. traffic conditions).

## Status

Proposed

## Consequences

Pro:

- User experience improvement.

Con:

- Complex slider component implementation.
- Could clutter the display if many vehicles are identified.