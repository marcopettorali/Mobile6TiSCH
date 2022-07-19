# Stage init protocol

## Stage 0
- NC initializes its structures
- Radio initializes their parameters
- Mobility initializes **positions** and **BR arrangement**
---
## Stage 1
- TSCH initializes params and retrieves **macAddress**
- BR retrieves **macAddress** + init **graphics**
- BR registers to NC
---
## Stage 2
- MN retrieves **macAddress** + init **traffic** + init **graphics**
- MN registers to NC with their **address** and **position**
- For each MN, a first **delegateBR** is assigned (knowing their initial position)
---
## Stage 3
- NC builds **global schedule**
---
## Stage 4
- BR retrieves list of initial position of nodes
- BR download **local schedule** according to the initial positions
- MN downloads **local schedule** 
---
