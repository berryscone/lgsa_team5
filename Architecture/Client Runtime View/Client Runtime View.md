# ALPR Client Application Runtime View
![ALPR Client Application Runtime View](runtime_view.png)
<hr>

## Element Catalog 

### ALPR Process Thread
- Threads in thread pool. Frame Generator sends each frames to single thread in the pool.

### License Plate Local Cache
- It stores vehicle detail information from Vehicle Query service

### Network Proxy
- It hides existence of local cache to NetworkManager

### Handlers
- Data to update UI are given to each handlers.
- Each handlers send signal to main window to update UI.

<hr>

## Behavior 
### Login Sequence Diagram
![](login_seq_diagram.png)

### User Interface Update Sequence Diagram
![](ui_update_seq_diagram.png)

### Vehicle Query Sequence Diagram
![](query_seq_diagram.png)

### User Interface Scenario
![](user_interface.jpg)
<hr>

## ADRs
- [ADR4 : Introduce Concurrency](ADR4.md)
- [ADR5 : Proxy pattern for local cache](ADR5.md)
<hr>

## Related Views 
- [ALPR Client Module Uses View](../Client%20Module%20Uses%20View/Client%20Module%20Uses%20View.md)
- [ALPR System Deployment View](../System%20Deployment%20View/System%20Deployment%20View.md)
