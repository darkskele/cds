# queue decisions

1. Every queue is bounded and a power of two in capacity. A full push returns false and the caller decides.
2. Cursors and ready flags sit on their own cache line.
3. Storage moves to the heap only by a template flag, for capacities too large for the stack.
4. `SlotRing` is addressed by absolute position, so producers finish out of order and the consumer still reads in order.
