# buffer

Fixed memory, sized at construction, written in place and handed out as views. Single thread. No allocation after construction.

## Diagram

```
write in place ──▶ buffer ──▶ view lent out ──▶ view destroyed, memory handed back
```

## Variations

- `LentSlotPool`. Equal slots in one contiguous mapping. A slot is acquired empty, filled in place, then lent out and freed in any order.
- `LentMirrorRing`. A byte ring mapped twice in a row, so every view over it is contiguous across the end. Bytes are lent in order and handed back in any order.
- `ViewablePool`. An append buffer of trivially copyable elements, read as one span and rewound by `reset()`.

## Milestones

- [x] ~~`LentSlotPool`~~
- [x] ~~`LentMirrorRing`~~
- [x] ~~`ViewablePool`~~
- [ ] `ViewablePool` stack storage on `std::inplace_vector`, unused slots never constructed (C++26, GCC 16)
- [ ] Compile-time tests of lend and release over every wrap point
