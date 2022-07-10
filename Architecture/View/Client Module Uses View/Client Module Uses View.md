# ALPR Client Application Module Uses View
![ALPR Client Application Module Uses View](module_view.png)
<hr>

## Element Catalog 

### ALPR Client App Window
- Receives input from the user
- Updates the view when the model is updated

### Login Window
- Receives login input from a user
- Requests login to NetworkManager

### FrameGenerator
- Extracts frames from selected video file

### NetworkManager
- Requests vehicle query and login to the server

### DataHandler
- Requests UI update when the data is updated
<hr>

## Behavior 
N/A
<hr>

## ADRs
- [ADR1](ADR1.md)
- [ADR2](ADR2.md)
- [ADR3](ADR3.md)
<hr>

## Related Views 
- [ALPR Client Runtime View](../Client%20Runtime%20View/Client%20Runtime%20View.md)