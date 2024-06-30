This is my final project for both Magshimim and School CS class.

***Constists of 3 main components:*** Proxy (Client), Relay and Directory.

Along with them, ***2 Encryptions:*** RSA and AES, and ***2 Helper Scripts:*** Compile (For compiling & running the project) and BrowserReplacement (was used for early testing).

Relay and Directory are meant to run on Linux - specifically Ubuntu Docker containers. Tested with Ubuntu 24.
Compiling them requires Compile.py - instructions on it are commented inside the file.

The Encryptions can run on both Linux and Windows if compiled correctly.

***External Dependencies:***
Proxy, Relay, Directory: SFML Network module + nlohmann::json, both included inside project directory.
RSA: Boost's cpp_int module, not included because of its large size. Compiled Boost on my machine.

***Versions Tested:***
SFML 2.6.1
Boost 1.85.
