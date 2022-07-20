 # ADR 11: Using Postgre SQL
We need a database with high retrieve performance, supporting partial match 
functionality. Also the DB should be supported in Django framework.

## Decision 
We will use PostgreSQL database.

## Rationale 
1. The database should be supported by Django framework as the server 
application is going to be implemented on top of that framework.
2. The database should support partial match functionality.
3. The database must provide acceptable level for retrieval performance with 
more than 25 million records. 
4. Rejected Alternative:
   - Redis: Although provides good retrieval performance, Django doesn’t 
support non-RDBMS directly.

## Status
Accepted

## Consequences
1. Good retrieval performance for the project is guaranteed.

      a. Under 1ms for exact match
  
2. Easy to use – Django support PostgreSQL directly and there are a lot of 
references online.
3. Partial match can be easily implemented by built-in functionality of 
PostgreSQL
