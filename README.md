Cryptography examples:
list of small apps connected with cryptography.

[#OpenSSL](https://github.com/openssl/openssl) is used as crypto library for DES, AES, RSA, MD5.
[#MPIR](https://github.com/wbhart/mpir) is used for big integer support
1) OneTimePad app. XORs INPUT file and KEY file (common length) -> gives OUTPUT file. XOR it with the KEY again to decode it.
2) ProcessMonitor shows you started and finished processes - useful for checking if program works correctly.
3) DES example
4) AES example
5) RC4 example.
6) RSA example
7) MD5 example. openssl function is used.
8) RHash - simple self-made hash function.
9) Entropy lets you count entropy of files through 1 char/2 chars/4 chars/8 chars.
10) Hashtable - reworked hashtable class. It works worse than the standard C++ one but uses custom hash function. Just an example.
11) MeetInTheMiddle - MITM attack demonstration with 4 threads. I've missed the fact DES keys have to be "checked" in openssl so the attack doesn't work correctly: it finds keys but most of them aren't allowed be used in real projects as they aren't cryptographically strong.
12) Client-Server - small ciphered socket-based app with diffie-hellman protocol. It often works badly as [#MPIR](https://github.com/wbhart/mpir) - a library with big integer support makes mistakes while working with integers of different length. That's why the app works well OR it's unable to decode messages.
13) Static library example.
