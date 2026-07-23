
```mermaid
---
config:
        look: handDrawn
        theme: neutral
        layout: elk
        elk:
                mergeEdges: true
                nodePlacementStrategy: LINEAR_SEGMENTS
---
flowchart TD
        subgraph Frontend
                A[User Interface] --> B[API Client]
        end
        subgraph Backend
                direction LR
                B --> C[Authentication]
                C[source] --- D & E --- S[sink]
        end

Frontend --> Backend
```
