# Markov Text Generator with Custom Hash Tables

A trigram-based Markov text generator built in C++ that models word probability distributions and generates new text based on observed patterns. Implements and benchmarks three custom hash table architectures against the C++ standard library.

## Overview

The program reads a text file word by word, tracks how often each three-word sequence (trigram) appears, then uses those probabilities to generate new 400-word text samples. The next word is always chosen based on the two previous words — a classic Markov process used in AI and NLP.

For example, given the sequence "if you can" appearing 3 times and "if you" appearing 3 times:

```
p(can | if you) = 3/3 = 1.0
```

## Hash Table Implementations

Three custom hash table strategies were implemented and benchmarked:

| Implementation | Strategy | Collision Handling |
|---|---|---|
| Separate Chaining | ArrayList of linked lists | Chains collisions into linked lists per bucket |
| Linear Probing | Open addressing with ArrayList | Probes sequentially for empty slots |
| Cuckoo Hashing | Open addressing with two hash functions | Kicks out existing entries to alternate positions |

All three are benchmarked against `std::unordered_map` for performance and correctness comparison.

## Project Structure

```
TextGenerator/
├── include/
│   ├── ArrayList.hpp / ArrayList.tpp
│   ├── SinglyLinkedList.hpp / SinglyLinkedList.tpp
│   ├── SeparateChaining.hpp
│   ├── LinearProbing.hpp
│   ├── CuckooHash.hpp
│   ├── FreqList.hpp
│   ├── TextGenerator.hpp
│   ├── StringPair.hpp
│   ├── UnorderedWrapper.hpp
│   └── WordStream.hpp
├── src/
│   ├── FreqList.cpp
│   ├── WordStream.cpp
│   └── main.cpp
├── gtest/
│   └── TextGenerator_tests.cpp
├── text/
│   ├── big.txt
│   ├── BlowinWind.txt
│   ├── Colours.txt
│   ├── if.txt
│   ├── Ihaveadream.txt
│   └── ripple.txt
└── CMakeLists.txt
```

## Build

```bash
cd TextGenerator
mkdir build && cd build
cmake ..
make
```

## Run

```bash
# Run the text generator
./bin/text_generator

# Run unit tests
./bin/text_generator_tests
```

When prompted, enter the path to a text file. For performance testing use:
```
../../text/big.txt
```

## Sample Output

Based on Bob Dylan's "Blowin' in the Wind":

```
how many years can a man turn his head pretending he just doesn't see ?
the answer my friend is blowin' in the wind the answer is blowin' in the wind
```

## Key Components

**FreqList** — Wraps `std::unordered_map` to store word frequencies. The `get(double p)` method selects a word based on probability p in [0.0, 1.0].

**StringPair** — Represents a pair of two strings used as hash table keys, with custom equals and hash implementations.

**TextGenerator** — Template class that builds the trigram table using `enter(w1, w2, w3)` and generates new text using `getNextWord(w1, w2)`.

**WordStream** — Tokenizes input text files word by word.

## Language

C++
