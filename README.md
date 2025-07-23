## Overview

Git450 is a simplified and secure version of GitHub developed as part of the EE450 course to master UNIX socket programming. The system includes a client interface, a main server, and three backend servers to handle authentication, repository management, and deployment via both TCP and UDP sockets.

## System Components

- **Client (`client.c`)**  
  Interfaces with the user and handles authentication and all commands.

- **Main Server (`serverM.c`)**  
  Acts as a central dispatcher, routing requests to appropriate backend servers.

- **Authentication Server (`serverA.c`)**  
  Verifies user credentials stored in `member.txt`.

- **Repository Server (`serverR.c`)**  
  Stores filenames and usernames in `filenames.txt`.

- **Deployment Server (`serverD.c`)**  
  Simulates deployment and logs filenames in `deployed.txt`.

## Supported User Commands

### Member
- `lookup <username>` — View own or another member's files.
- `push <filename>` — Add a new file or overwrite an existing one.
- `remove <filename>` — Remove a file from the repository.
- `deploy` — Deploy all files belonging to the member.
- `log` — (Extra credit) View operation history.

### Guest
- `lookup <username>` — View public files of any member.

## Encryption Scheme

Passwords are encrypted with a Caesar cipher:
- Offset each letter/digit by 3 (circular wrap for letters and digits)
- Case-sensitive
- Special characters are unchanged

**Example:**
| Plaintext | Encrypted |
|-----------|-----------|
| Welcome1  | Zhofrph4  |
| 199xyz@$  | 422abc@$  |

## File Summary

| File Name     | Description                             |
|---------------|-----------------------------------------|
| `client.c`    | Handles client operations and user input|
| `serverM.c`   | Main server that routes requests        |
| `serverA.c`   | Authenticates members via UDP           |
| `serverR.c`   | Repository manager for filenames        |
| `serverD.c`   | Deploys files and logs deployment       |
| `member.txt`  | Encrypted user credentials              |
| `filenames.txt` | Repository metadata                   |
| `Makefile`    | Builds all executables                  |
| `README.md`   | Project documentation                   |

## Execution Instructions

### 1. Compile All Files
```bash
make all
```

### 2. Launch Servers (in separate terminals)
```bash
./serverM
./serverA
./serverR
./serverD
```

### 3. Start Clients
```bash
./client <username> <password>
```

## Port Allocation

Use static ports based on the last 3 digits of your USC ID (e.g., `xxx = 319`):

| Server    | Protocol | Port Used       |
|-----------|----------|-----------------|
| serverA   | UDP      | 21000 + xxx     |
| serverR   | UDP      | 22000 + xxx     |
| serverD   | UDP      | 23000 + xxx     |
| serverM   | UDP      | 24000 + xxx     |
| serverM   | TCP      | 25000 + xxx     |

## Assumptions & Notes

- All hostnames must be `localhost` (`127.0.0.1`)
- Use `getsockname()` for dynamically assigned ports
- Clients can run multiple times; servers stay active
- Only filenames are handled (no file content transfer)
- Start the processes in this order: `serverM`, `serverA`, `serverR`, `serverD`, then `client`
- Comply strictly with on-screen message formats from the spec

## Reused Code

Socket boilerplate taken from [Beej’s Guide to Network Programming](http://www.beej.us/guide/bgnet/) and marked in source files.

---

Author:
Shravya Shashidhar|USC
