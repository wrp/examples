
### 3. **ASCII Art Diagram**

```markdown
    In github, this renders pretty much verbatim:

    +---+       +--------+
    | A |<----> |  Host  |
    +---+       +--------+
      |
      v             ^
    +---+           |
    | B |    +------|------------------+
    +---+    |      v                  |
             |    +-------------+      |         +----------+
             | /->|  proc 1     |<==============>|  Host 2  |
             | |  +-------------+      |         +----------+
             | |                       |
             | |    host-3-us-west     |
             +-|-----------------------+
               |
               |
           ====|===============================
               |
   +---+       |
   | C |-------*   +--------+
   +---+       \-> |  Host  |
                   +--------+



```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;



$$
\begin{tikzpicture}
\node (A) {A};
\node (B) [below=of A] {B};
\draw[->] (A) -- (B);
\end{tikzpicture}
$$
