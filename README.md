# hash-bench-3ds

Native **Nintendo 3DS / 2DS / New 3DS** hashing-algorithm benchmark —
**32 algorithms** running on the ARM11 MPCore application processor
through libctru, with cycle-accurate timing via `svcGetSystemTick()`.

Same algorithm set and source files as
[`hash-bench-nds`](https://github.com/dmang-dev/hash-bench-nds) and
[`hash-bench-gba`](https://github.com/dmang-dev/hash-bench-gba) — but this is a **native 3DS
ARM11 build**, not a DS-compat ROM. That means:

- Real FPU, real L1+L2 cache, real branch predictor
- 268 MHz on Old 3DS / Old 2DS, **804 MHz on New 3DS / New 2DS XL**
  (via `osSetSpeedupEnable(true)`)
- ARMv6K instruction set (vs DS's ARMv5TE) — slightly newer rotates,
  ldrex/strex, etc.
- Output `.3dsx` runs via the Homebrew Launcher (FBI / Anemone / etc.)

[![ROM](https://img.shields.io/badge/ROM-prebuilt%20%26%20committed-success)](artifacts/)
[![Built with devkitARM + libctru](https://img.shields.io/badge/built%20with-devkitARM%20%2B%20libctru-orange)](https://devkitpro.org)

---

## Try it

| File | Target | Notes |
|---|---|---|
| `hash-bench-3ds.3dsx` | 3DS / 2DS family (homebrew loader) | ARM11 native, ~147 KB. Drop into `/3ds/hash-bench-3ds/hash-bench-3ds.3dsx` on the SD card and launch from the Homebrew Launcher. |
| `hash-bench-3ds.smdh` | Companion metadata | Auto-generated icon + title, lives next to the `.3dsx`. |

For testing on a PC: load `hash-bench-3ds.3dsx` in **[Citra](https://citra-emu.org/)** or **[Lime3DS](https://lime3ds.github.io/)**.

---

## Build from source

Requires devkitPro with the `3ds-dev` meta-package (devkitARM + libctru
+ 3dsxtool). https://github.com/devkitPro/installer/releases.

```
.\build.bat            # build hash-bench-3ds.3dsx + smdh
.\build.bat clean
```

Linux/macOS/CI: `make`.

---

## What's interesting on the 3DS

The ARM11 is the first CPU in this benchmark suite with:

1. **Real cache** — L1 D-cache (16 KB) easily fits the entire 1 KB
   workload buffer + state. Throughput should be cache-bound, not
   memory-bound, for the entire sweep.
2. **Hardware FPU** — not used by any hash algorithm (they're all
   integer), but a baseline data point for "modern" mobile CPU.
3. **Out-of-order issue** (in the New 3DS Cortex-A9) — instruction
   parallelism that helps the ARX-style hashes more than the
   bit-by-bit CRCs.
4. **804 MHz boost mode** — `osSetSpeedupEnable(true)` triples clock
   on New 3DS. The exact ratio of speedup vs the NDS at 33.5 MHz
   (24×) shows how much non-clock benefits there are.

The on-screen status footer reports the active clock (268 / 804 MHz)
so you know which mode you're in.

---

## Algorithm set

Identical to [`hash-bench-nds`](https://github.com/dmang-dev/hash-bench-nds#algorithms) — see
the NDS README for the full table. All 32 sources are byte-identical
across the GBA / NDS / DSi / 3DS projects.

Iteration counts are dialed 10× higher than the NDS build because
the ARM11 is so fast — a sweep takes ~3 seconds end-to-end vs ~10s
on the NDS, despite running 10× more iterations per algo.

---

## Timing methodology

```c
u64 t0 = svcGetSystemTick();
for (k = 0; k < iters; k++) hash(buf, 1024, digest);
u64 ticks = svcGetSystemTick() - t0;
u64 us    = ticks * 1000000ULL / arm11_clock_hz;   // SYSCLOCK_ARM11 or _NEW
```

`svcGetSystemTick()` returns a free-running 64-bit counter at the
ARM11 clock (`SYSCLOCK_ARM11` = 268111856 Hz on Old, `SYSCLOCK_ARM11_NEW`
= 804000000 Hz on New 3DS). The host runtime auto-detects via
`APT_CheckNew3DS()` and adjusts the divisor accordingly.

---

## Layout

```
source/
  main.c                    Boot, dual-console, dispatch, run_sweep
  (32 hash algorithm files, byte-identical to hash-bench-nds/source/)
include/
  hashes.h
artifacts/
  hash-bench-3ds.3dsx       Native 3DS homebrew binary
  hash-bench-3ds.smdh       Icon + metadata
tests/
  refs.txt                  Reference digests (shared)
  verify.c                  Host gcc cross-check
Makefile                    devkitPro 3DS template
build.bat                   Windows wrapper
```

---

## Acknowledgments

- [devkitPro / devkitARM](https://devkitpro.org/)
- [libctru](https://github.com/devkitPro/libctru) — 3DS hardware
  abstraction, syscalls, console
- [Citra](https://citra-emu.org/) / [Lime3DS](https://lime3ds.github.io/)
  — 3DS emulators
- [`hash-bench-nds`](https://github.com/dmang-dev/hash-bench-nds) / [`hash-bench-gba`](https://github.com/dmang-dev/hash-bench-gba)
  — sibling projects supplying the algorithm sources
