# 🛡️ SSH Timing Enumeration Tool 

This is a lightweight C++ tool that measures SSH login response times for various usernames in order to detect potentially valid users on a target SSH server.

⏱️ Based on the idea that SSH servers sometimes take slightly longer to respond to valid usernames (even if the password is wrong), this tool can be used as an educational exercise in understanding side-channel analysis and SSH behavior.

> ⚠️ Educational Purpose Only: This tool is designed for learning and ethical testing. Do not use it without explicit permission.

---

## 📦 Features

- 🧠 Username enumeration via timing side-channels
- 🧾 Simple CLI interface: ssh_enum -i <ip> -p <port> -u usernames.txt
- 🗂️ Output response times into a CSV file
- 📤 Easily extendable and minimal dependencies (uses `libssh2`)

---

## 🚀 Usage

### 📥 Build (Linux)

You need to have libssh2 installed. On Debian/Ubuntu:

```bash
sudo apt install libssh2-1-dev
