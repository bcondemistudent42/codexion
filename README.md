*This project has been created as part of the 42 curriculum by bcondemi.*

## Description
Codexion is a concurrency simulation inspired by the classic “Dining Philosophers” problem.

- You start **N coders** (threads) competing for **N dongles** (shared resources).
- To **compile**, a coder must be allowed to take **both adjacent dongles**.
- After compiling, a coder **debugs** and then **refactors**, repeating until reaching a required number of compiles.
- A dedicated **monitor thread** continuously checks for **burnout** (timeout since last successful progress) or global completion.

The program prints a timestamped log of actions (taking a dongle, compiling, debugging, refactoring, burnout).
Timestamps are expressed in **milliseconds since the simulation start**.

## Instructions

### Build
```sh
make
```
This produces the `codexion` binary.

Useful targets:
```sh
make clean
make fclean
make re
```

### Run
```sh
./codexion \
  number_of_coders \
  time_to_burnout \
  time_to_compile \
  time_to_debug \
  time_to_refactor \
  number_of_compiles_required \
  dongle_cooldown \
  fifo|edf
```

All timing values are intended to be in **milliseconds**.
`dongle_cooldown` must be a **strictly positive** integer.

- `fifo`: per-dongle alternation priority (queue-based)
- `edf`: per-dongle “earliest burnout” priority (closest deadline first)

Example:
```sh
./codexion 5 800 200 200 200 3 50 fifo
```

### Output
Typical output lines:
- `<t_ms> <coder_id> has taken a dongle`
- `<t_ms> <coder_id> is compiling`
- `<t_ms> <coder_id> is debugging`
- `<t_ms> <coder_id> is refactoring`
- `<t_ms> <coder_id> has burnout`

## Blocking cases handled
This project addresses common concurrency failure modes in resource-sharing simulations:

- **Deadlock prevention **
  - The implementation avoids *circular wait while holding locks*: eligibility is checked first (`can_compile()`), and dongle mutexes are only held briefly for state updates.
  - This design aims to break the **circular-wait** condition of Coffman’s deadlock conditions (even though mutual exclusion on dongles is still present by design).

- **Starvation prevention / fairness**
  - In `fifo` mode, each dongle keeps a 2-entry queue of its two neighboring coders and flips priority on release (`swap_priority()`), ensuring the same coder doesn’t monopolize the dongle.
  - In `edf` mode, the dongle grants access to the coder with the most urgent “burnout deadline” (closest to timing out), which prioritizes the coder at greatest risk.

- **Cooldown handling (resource reuse delay)**
  - Each dongle records `last_time_used`, and `check_dongle()` enforces `last_time_used + dongle_cooldown` before allowing another take.

- **Burnout detection (timeout supervision)**
  - The monitor thread periodically scans all coders and declares burnout if the elapsed time since the coder’s last recorded compile progress exceeds `time_to_burnout`.
  - On burnout, the monitor sets a shared end state, and worker threads stop cleanly.

- **Global completion detection**
  - The monitor checks `compile_cnt` for every coder and ends the simulation once all coders reached `number_of_compiles_required`.

- **Log serialization (prevent interleaved prints)**
  - All status logs are guarded by a single `mutex_print`, so each line is printed atomically.

- **Synchronized start (consistent timestamps)**
  - Threads wait on a start barrier so `TM_START` is set once and all coders begin from the same reference time.

## Thread synchronization mechanisms
This project uses POSIX threads (`pthread`) primitives for safe coordination.

### Mutexes (`pthread_mutex_t`)
- **Dongles (`dongle_mtx`)**
  - Protects per-dongle state: `available`, `last_time_used`, and the per-dongle queue.
  - `check_dongle()` runs under this mutex to prevent races when deciding if a dongle can be taken.

- **Per-coder state (`coder_mutex`)**
  - Protects `compile_cnt` and the last progress timestamp used by burnout checks.
  - The monitor locks each coder’s mutex before reading burnout-related fields, ensuring it never reads a partially updated state.

- **Manager state (`mutex_manager`)**
  - Protects the global end state (`end_type`) so worker threads and the monitor agree on whether the simulation is still running.

- **Printing (`mutex_print`)**
  - Serializes `printf()` calls across all threads to avoid log interleaving.

- **Start barrier state (`protect_nb_ready`)**
  - Protects the barrier counters/flags (`nb_ready`, `check_ready`) used to coordinate the common start.

### Condition variables (`pthread_cond_t`)
- **`cond_ready`**: the last worker reaching the barrier signals the manager that all coders are ready.
- **`routine_wait_start`**: broadcast by the manager when `TM_START` is set, waking all waiting coders.

Together these create a classic barrier:
1) Each coder increments `nb_ready` and waits.
2) Once all are ready, the manager sets `TM_START` and broadcasts.
3) All coders start their loop with a shared, consistent time base.

### Custom events / flags
In addition to pthread primitives, the code uses a few shared flags as lightweight “events”, always read/written under the appropriate mutex:

- `check_ready` + the start-barrier condition variables: acts as a one-shot “start now” event.
- `thread_error`: set if thread creation fails, then broadcast wakes all waiting threads so they can exit safely.
- `end_type`: global “stop reason” set by the monitor (burnout) or by completion detection.

### Thread-safe communication patterns (examples)
- **Monitor → coders (stop signal)**
  - Monitor sets `end_type` under `mutex_manager`.
  - Coders periodically check `end_type` under the same mutex inside their action loops, stopping quickly and consistently.

- **Coders ↔ dongles (shared resource access)**
  - A coder only proceeds to compile when *both* adjacent dongles report available, cooldown-respected, and priority-respected under their respective `dongle_mtx`.
  - On release, the dongle updates `last_time_used` and flips/updates the priority order so the next contender can proceed.

## Resources
Classic references:
- POSIX threads overview: `man pthreads`
- Mutexes: `man pthread_mutex_lock`, `man pthread_mutex_init`
- Condition variables: `man pthread_cond_wait`, `man pthread_cond_broadcast`
- Youtube: `https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2`
