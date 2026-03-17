# Switchboard — Distributed File Repository System

A simplified, GitHub-inspired file repository built entirely in C using UNIX sockets. Five processes communicate over TCP and UDP to handle authentication, file management, and deployment across a distributed architecture.

---

## Architecture

```
  ╔══════════════════════════════════════════════════════╗
  ║                       CLIENT                         ║
  ╚══════════════════════════════╤═══════════════════════╝
                                 │
                           TCP · 25542
                                 │
  ╔══════════════════════════════▼═══════════════════════╗
  ║                     SERVER M                         ║
  ║                  request router                      ║
  ╚══════════╤════════════════╤══════════════╤═══════════╝
             │                │              │
        UDP · 21542      UDP · 22542    UDP · 23542
             │                │              │
  ╔══════════▼═══╗   ╔════════▼═════╗  ╔════▼══════════╗
  ║   SERVER A   ║   ║   SERVER R   ║  ║   SERVER D    ║
  ║     auth     ║   ║  repository  ║  ║  deployment   ║
  ╚══════════════╝   ╚══════════════╝  ╚═══════════════╝
```

- **Server M** receives all client requests over TCP and dispatches them to the appropriate backend server over UDP
- **Server A** validates credentials against an encrypted member store
- **Server R** manages the file repository (lookup, push, remove)
- **Server D** handles deployment and logs results to `deployed.txt`
- **Client** connects once, authenticates, and enters either member or guest mode

---

## Quick Start

### 1. Build

```bash
make all
```

### 2. Start the servers (each in its own terminal)

```bash
./serverM   # start first
./serverA
./serverR
./serverD
```

### 3. Connect a client

```bash
# As a guest (read-only access)
./client guest guest

# As a registered member (credentials in members.txt)
./client <username> <password>
```

---

## Demo Walkthrough

### Guest session

```
$ ./client guest guest

guest guest
The client is up and running.
...
Enter a command: lookup HannahWilliams598
...
```

Guests can only run `lookup` and only see public files.

### Member session

```
$ ./client <username> <password>

The client is up and running.
...
Enter a command: lookup <username>
Enter a command: push myfile.txt
Enter a command: remove oldfile.txt
Enter a command: deploy
Enter a command: log
```

Full command reference:

| Command | Access | Description |
|---------|--------|-------------|
| `lookup <username>` | Member + Guest | List files owned by a user |
| `push <filename>` | Member | Add a file; prompts before overwrite |
| `remove <filename>` | Member | Delete a file from the repository |
| `deploy` | Member | Deploy all your files to Server D |
| `log` | Member | View your operation history |

---

## How It Works

### Authentication flow

1. Client sends `username password` over TCP to Server M
2. Server M forwards to Server A over UDP
3. Server A applies a **Caesar cipher** (shift +3 on letters and digits) and compares against `members.txt`
4. Server M returns `"Valid user"` or `"Invalid user"` to the client

**Caesar cipher examples:**

| Plaintext | Encrypted |
|-----------|-----------|
| `Welcome1` | `Zhofrph4` |
| `199xyz@$` | `422abc@$` |

Special characters are unchanged. Shift wraps around (`z → c`, `9 → 2`).

### Push with overwrite protection

If a file already exists in the repository, Server R flags it and Server M relays a confirmation prompt back to the client before overwriting.

### Deployment

`deploy` triggers Server M to:
1. Request the user's full file list from Server R
2. Forward the list to Server D
3. Server D appends all filenames to `deployed.txt` and confirms

---

## Project Structure

```
switchboard/
├── serverM.c / serverM.h   # Main server — TCP listener + UDP dispatcher
├── serverA.c / serverA.h   # Auth server — Caesar cipher + credential check
├── serverR.c / serverR.h   # Repo server — file metadata management
├── serverD.c / serverD.h   # Deploy server — deployment logger
├── client.c  / client.h    # Client — CLI interface
├── members.txt             # Encrypted user credentials
├── filenames.txt           # Repository file metadata
├── deployed.txt            # Deployment log (written at runtime)
├── loghistory              # Operation history log (written at runtime)
└── Makefile
```

---

## Port Allocation

| Process | Protocol | Port |
|---------|----------|------|
| Server A | UDP | 21542 |
| Server R | UDP | 22542 |
| Server D | UDP | 23542 |
| Server M | UDP | 24542 |
| Server M | TCP | 25542 |

---

## Tech Stack

- **Language**: C (POSIX)
- **Transport**: TCP (client ↔ Server M), UDP (inter-server)
- **Concurrency**: `select()` multiplexing for multiple simultaneous clients
- **Persistence**: Plain-text flat files
- **Build**: GNU Make + GCC

---

## Notes

- All processes run on `localhost` (`127.0.0.1`)
- Servers stay running between client connections; restart is not required between sessions
- Only filenames are tracked — no file content is transferred
- The `log` command stores up to 1024 operations in memory per session
