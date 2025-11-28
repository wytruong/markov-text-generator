
## README for Markov Text Generator + Hash Tables (Lab 7)


```markdown
# Trigram Markov Text Generator with Custom Hash Tables

This project implements a **trigram-based Markov text generator** in C++.  
It also includes three custom hash table implementations and compares them against `std::unordered_map`:

- Separate chaining
- Linear probing
- Cuckoo hashing

Built as part of **ICS 46: Data Structure Implementation** at UC Irvine.

---

## High-Level Idea

1. Read a source text file word by word (punctuation counts as words).
2. For each three-word sequence `(w1, w2, w3)`, record that `w3` follows the pair `(w1, w2)`.
3. For each pair `(w1, w2)`, maintain a frequency list of all possible next words.
4. To generate text:
   - Start with an initial pair `(w1, w2)` (e.g., the first two words in the text).
   - Use the learned frequencies to randomly sample the next word.
   - Slide the window and repeat until ~400 words are generated.

This is a classic Markov process: the probability of the next word depends only on the two previous words.

---

## Key Components

- **FreqList**
  - Wraps a hash table mapping `std::string -> count`
  - Maintains `totalFreq_` for the sum of all counts
  - Supports:
    - `add(word)` – increment or insert word frequency
    - `get(p)` – return a word corresponding to a probability mass in `[0,1]`

- **StringPair**
  - Represents a pair of strings `(w1, w2)`
  - Provides equality and hashing so it can be used as a key in hash tables

- **Hash Tables**
  - `SeparateChaining` – array of buckets, each bucket is a linked list
  - `LinearProbing` – open addressing with probing on collisions
  - `CuckooHash` – uses two hash functions and “kicks out” entries on collision, possibly triggering rehashing
  - `UnorderedWrapper` – adapter around `std::unordered_map` for comparison

- **TextGenerator**
  - Core logic for:
    - Reading the corpus via `WordStream`
    - Building a trigram table: `(w1, w2) -> FreqList of w3`
    - Generating new text samples using a random number generator
    - Comparing performance of the different hash table implementations

- **WordStream**
  - Utility class that tokenizes an input file into words and punctuation

---

## Project Structure

```text
TextGenerator/
├── build/
├── gtest/
│   └── TextGenerator_tests.cpp
├── include/
│   ├── ArrayList.hpp, ArrayList.tpp
│   ├── SinglyLinkedList.hpp, SinglyLinkedList.tpp
│   ├── FreqList.hpp
│   ├── StringPair.hpp
│   ├── SeparateChaining.hpp
│   ├── LinearProbing.hpp
│   ├── CuckooHash.hpp
│   ├── UnorderedWrapper.hpp
│   ├── TextGenerator.hpp
│   └── WordStream.hpp
├── src/
│   ├── FreqList.cpp
│   ├── WordStream.cpp
│   └── main.cpp
├── text/
│   ├── big.txt
│   ├── BlowinWind.txt
│   ├── Colours.txt
│   ├── if.txt
│   ├── Ihaveadream.txt
│   └── ripple.txt
└── CMakeLists.txt

Build Instructions
# From the repo root
mkdir -p build
cd build

cmake ..
make

Binaries are placed in build/bin.

Running

From inside build/bin:

# Run the main text generator
./text_generator


The program will:

Ask for a text file (e.g., ../../text/big.txt)
Build the trigram model
Generate a sample of at least 400 words, inserting a newline every 20 words for readability
Print a table of frequencies for verification

