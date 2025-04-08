# 🔄 Thread-Safe Queue in C

This project provides a **thread-safe queue implementation in C** using `threads.h`. It supports concurrent enqueue and dequeue operations with condition variables and mutex locks to ensure safe access from multiple threads.

---

## 💡 Features

- 🧵 Thread-safe enqueue and dequeue
- 💤 Blocking `dequeue()` with `cond_wait`
- ⚡ Non-blocking `tryDequeue()` method
- 📊 Utility functions for size, waiting threads, and visited items
- ✅ Extensive test suite including multithreaded scenarios and FIFO validation

---

## 📂 Project Structure

```
thread-safe-queue/
├── queue.c       # Queue implementation
├── queue.h       # Queue interface
├── test.c        # Comprehensive test suite
└── .vscode/      # Dev environment configs
```

---

## 🚀 Getting Started

### 🧰 Build the Project

```bash
gcc -std=c11 -pthread test.c -o test
```

> Make sure your compiler supports C11 threads (`threads.h`). On Linux, use GCC ≥ v9.

---

## 🧪 Run Tests

```bash
./test
```

The test suite includes:
- Basic enqueue/dequeue
- Try-dequeue behavior
- Multithreaded producer/consumer tests
- Edge cases and mixed operation flows

---

## 🧠 Design Highlights

- The queue uses a linked-list structure internally.
- Synchronization is handled using:
  - `mtx_t` for locking
  - `cnd_t` for blocking/waking threads
- Counters track:
  - Total and active operations
  - Queue size
  - Visited and waiting elements

---

## 🪪 License

This project is released under the MIT License.

---

## 👤 Author

Made by [mati1mati1](https://github.com/mati1mati1) 