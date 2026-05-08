# calculator
this is my first real project with c++ and I'll be happy to have new ideas or have some more hands on this project.


Overview

The program runs a loop where you always have a “current result” (or no result yet). You pick an operator, provide numbers (or reuse the current result), and the calculator shows the new result. It keeps asking “Continue?” after every operation – that’s intentional so you never get stuck. You can exit anytime.

Features (what you actually get):

    Basic arithmetic: +, -, *, /, ^ (power), % (modulo – floating‑point, using fmod).

    Scientific functions (unary): sqrt (v), sin (s), cos (c), tan (t), factorial (!), log10 (l), natural log (n), exp (e).

    Angle modes: Degrees (default) or Radians – toggled with d. All trig functions respect the current mode.

    Precision control: p lets you set decimal places (0‑15). Output formatting uses setprecision with fixed.

    Memory: one memory slot. m then s (store), r (recall), c (clear). If you recall, it immediately becomes the new current result and you can undo it.

    History: h shows the last 20 operations with their inputs and results. The history is stored as strings inside a vector.

    Undo: u restores the previous result before the last operation. It only works once per operation – I save the old result just before computing the new one, then invalidate the saved value after an undo so you don’t keep going back forever.

    Reset: r clears everything: result, memory, history, last‑result‑for‑undo.

How the code is structured:
The whole thing lives inside a Calculator class. The constructor initialises:

    memory = 0

    lastResult and lastResultValid for undo

    radiansMode = false (degrees)

    precision = 6

    an empty vector<string> history
    
Private helpers:
    clearInput() – flushes cin and ignores the rest of the line after bad input. Used everywhere to prevent infinite loops.
    getNumber(prompt) – loops until the user enters a valid number. Handles non‑numeric garbage 
    getOperator() – prints a menu, reads a single character, normalises it to lowercase, and checks against a list of valid commands. Repeats until you give something sensible.
Then there are the actual math functions: add, subtract, multiply, divide, power, modulo, squareRoot, sine, cosine, tangent, factorial, logarithm10, naturalLog, exponential. Each one throws a runtime_error with a meaningful message when something goes wrong (e.g., division by zero, sqrt of negative, log of non‑positive, factorial of a non‑integer or negative). The tangent function also checks if cos(rad) is too close to zero.
Important detail: The trig functions do the degree‑to‑radian conversion internally only if radiansMode is false. That keeps the callers simple – they just pass the raw number the user typed.

Undo mechanism:
saveCurrentResult(double current, bool hasResult) stores the current result in lastResult and sets lastResultValid = true only if there actually is a valid result. Right before performing any operation (binary or unary), I call saveCurrentResult(result, hasResult). That way, when you press u, the calculator restores that saved value and marks lastResultValid as false so you can’t undo the same step twice. Feels natural in practice.

History:
Every time an operation succeeds, I build a descriptive string (e.g. "10 + 5 = 15" or "sin(45) = 0.707106") and push it into the history vector. If the history exceeds 20 entries, the oldest one is erased. The h command prints all of them with line numbers.

The main loop (run()):
First it prints a big banner with all available commands. Then it enters the while(true) loop.

Each iteration:

    Shows the current result (if any), memory, angle mode, and current precision.

    Calls getOperator() to get a single command.

    Handles commands in a specific order:

        Memory (m) – asks for store/recall/clear. Store can take either the current result or a user‑typed value. Recall overwrites the current result but saves the old one for undo.

        Reset (r) – resets everything.

        Toggle mode (d) – flips radiansMode.

        Set precision (p) – uses getNumber with a range check.

        Show history (h).

        Undo (u) – restores lastResult if valid.

        Unary operations (v, s, c, t, !, l, n, e) – asks whether to use the current result or a new number, calls the corresponding math function inside a try‑catch, saves the result, updates history.

        Binary operations (+, -, *, /, ^, %) – asks whether to use current result as first operand, gets the second number, performs the operation, same try‑catch + history.

After completing any operation (or even after an error), the program always asks "Continue calculating? (y/n)". No hidden exit – you have to explicitly say n to quit. This was a deliberate fix because earlier versions would silently exit after some commands.

Error handling – why it doesn’t crash:
Every math operation is wrapped in a try block catching exception (specifically runtime_error). If something fails, the error message prints, but the current result and history stay unchanged. Input validation for numbers and operators is also bulletproof – clearInput() makes sure leftover newlines or letters don’t break the next prompt.

A quick guide for those who wanna know what does every operator do:

+ – Adds two numbers.

- – Subtracts the second number from the first.

* – Multiplies two numbers.

/ – Divides the first number by the second (throws error if divisor is zero).

^ – Raises the first number to the power of the second.

% – Computes the floating‑point remainder of the first number divided by the second (using fmod; error if second is zero).

v – Takes the square root of a number (error if negative).

s – Takes the sine of an angle (degrees or radians according to current mode).

c – Takes the cosine of an angle.

t – Takes the tangent of an angle (error at asymptotes).

! – Calculates the factorial of a non‑negative integer (error for negative or non‑integer).

l – Computes the base‑10 logarithm (error for non‑positive input).

n – Computes the natural logarithm (error for non‑positive input).

e – Computes the exponential function (eˣ).

d – Toggles angle mode between degrees and radians.

p – Sets the number of decimal places for output (0–15).

h – Shows the history of performed operations.

u – Undoes the last operation, restoring the previous result.

m – Memory operations:

   s – Stores the current result (or a user‑typed value) into memory.

   r – Recalls the stored memory as the current result.

   c – Clears the memory (sets to zero).

r (reset) – Clears current result, memory, history, and undo state.
