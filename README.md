# Mangine

A search engine to find relevant linux/package commands for a user's specified purpose. Leverages a custom BM25 scoring algorithm to provide highly accurate results. This is mean't to be a better alternative to [apropos](https://en.wikipedia.org/wiki/Apropos_(Unix)) where rather than just looking based on keyword existence, this does scoring to ensure that results aren't pruned automatically for not having a specific keyword.

## Installation
### Install Dependencies
- [CMake](https://cmake.org)

### Testing
1. `git clone git@github.com:matthambrecht/Mangine.git`
2. `cd Mangine`
3. `sh run_tests.sh`

### Deployment
**N/A**

## External Libraries / Credits
- [nlohmann/json](https://github.com/nlohmann/json): JSON Parsing
- [GoogleTest](https://github.com/google/googletest) Unit Testing Framework

## Upcoming Features
### Release TODO List
- [X] Switch from Embedding Model to Traditional BM25 Search
- [ ] Integrate BM25 from Scratch
- [X] Remove Python Webserver
- [X] Move over to SQLite from Postgres
- [X] Clean up Install Script
- [ ] Bulk DB Document Updating (Slow as heck currently )
- [ ] Clean up Unit Testing
- [ ] Fix SQLi Vulnerability
- [ ] Setup Preprocessing Pipeline (Lemmatization, Synonym Replacement, etc.)
- [ ] Handle Manpage Updates (Hash Checking)
- [ ] Finish Core Functionality

### Later Version TODO
- [ ] Create user program to interface with the API through pipes
- [ ] Find a better place to get less technical descriptions of commands (No non-technical person looking to view a file is gonna search anything close to `cat - concatenate files and print on the standard output`")
- [ ] Add Ncurses GUI for more user friendliness
- [ ] Maybe move back towards AI/ML? Don't really want Python though, might need to do some Cuda tomfoolery to get even more accurate results
- [ ] DB Caching System for IDF
- [ ] Config Validation (Don't want crappy configs breaking this)