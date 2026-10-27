# buffer decisions

1. A lent view hands its memory back when destroyed. It is move-only.
2. Every buffer is single-thread. Crossing threads is a queue's job.
3. `LentSlotPool` slots are a multiple of the cache line, all in one mapping populated at construction.
4. `LentMirrorRing` maps its memory twice in a row, so a view crossing the end is still one contiguous span.
5. `LentMirrorRing` lends in write order and takes views back in any order. The size and the count of views out at once are powers of two.
6. `ViewablePool` holds trivially copyable elements only. Its span is valid until the next reset.
