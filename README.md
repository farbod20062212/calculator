# calculator
this is my first real project with c++ and I'll be happy to have new ideas or have some more hands on this project.


Overview

The program runs a loop where you always have a “current result” (or no result yet). You pick an operator, provide numbers (or reuse the current result), and the calculator shows the new result. It keeps asking “Continue?” after every operation – that’s intentional so you never get stuck. You can exit anytime.

Features (what you actually get)

    Basic arithmetic: +, -, *, /, ^ (power), % (modulo – floating‑point, using fmod).

    Scientific functions (unary): sqrt (v), sin (s), cos (c), tan (t), factorial (!), log10 (l), natural log (n), exp (e).

    Angle modes: Degrees (default) or Radians – toggled with d. All trig functions respect the current mode.

    Precision control: p lets you set decimal places (0‑15). Output formatting uses setprecision with fixed.

    Memory: one memory slot. m then s (store), r (recall), c (clear). If you recall, it immediately becomes the new current result and you can undo it.

    History: h shows the last 20 operations with their inputs and results. The history is stored as strings inside a vector.

    Undo: u restores the previous result before the last operation. It only works once per operation – I save the old result just before computing the new one, then invalidate the saved value after an undo so you don’t keep going back forever.

    Reset: r clears everything: result, memory, history, last‑result‑for‑undo.

    

    How the code is structured
    

The whole thing lives inside a Calculator class. The constructor initialises:

    memory = 0

    lastResult and lastResultValid for undo

    radiansMode = false (degrees)

    precision = 6

    an empty vector<string> history

    
