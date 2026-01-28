# 🔄 Epsilon-NFA to NFA Converter

> **A comprehensive C implementation for converting ε-NFAs to equivalent epsilon-free NFAs**

**Authors:** Benshrif Mouataz & Ameur Zineddine  
**Language:** C  
**Purpose:** Educational tool for automata theory and formal language courses

---

## 📋 Table of Contents

- [Overview](#overview)
- [Theoretical Background](#theoretical-background)
- [Features](#features)
- [Algorithm Explanation](#algorithm-explanation)
- [Installation & Usage](#installation--usage)
- [Input Specification](#input-specification)
- [Examples](#examples)
- [Code Structure](#code-structure)
- [Complexity Analysis](#complexity-analysis)
- [Limitations & Extensions](#limitations--extensions)
- [References](#references)

---

## 🎯 Overview

This program implements an **epsilon-NFA to NFA converter** that eliminates epsilon (ε) transitions from nondeterministic finite automata. Epsilon transitions allow state changes without consuming input symbols, which can complicate automaton analysis and implementation.

### What is an Epsilon-NFA?

An **ε-NFA** (Epsilon-Nondeterministic Finite Automaton) is an extension of a regular NFA that includes special **epsilon transitions** (ε-transitions). These transitions allow the automaton to move between states without reading any input symbol.

**Key Differences:**

| Feature | Regular NFA | Epsilon-NFA |
|---------|-------------|-------------|
| **Transitions** | Require input symbol | Can transition on ε (empty string) |
| **State Changes** | Only on symbol consumption | Can occur spontaneously |
| **Complexity** | Simpler to implement | More expressive but complex |

---

## 🧠 Theoretical Background

### Formal Definition

An **ε-NFA** is defined as a 5-tuple: **M = (Q, Σ, δ, q₀, F)** where:

- **Q** = Finite set of states
- **Σ** = Input alphabet (finite set of symbols)
- **δ** = Transition function: Q × (Σ ∪ {ε}) → P(Q)
- **q₀** = Initial state (q₀ ∈ Q)
- **F** = Set of accepting/final states (F ⊆ Q)

### Epsilon-Closure

The **ε-closure** of a state q, denoted **ε-closure(q)**, is the set of all states reachable from q using only ε-transitions, including q itself.

**Formally:**
```
ε-closure(q) = {p ∈ Q | there exists a path from q to p using only ε-transitions}
```

### Conversion Theorem

**Theorem:** For every ε-NFA **M**, there exists an equivalent NFA **M'** without ε-transitions such that **L(M) = L(M')**.

**Proof Sketch:**
The conversion preserves the accepted language by incorporating ε-reachability into the transition function and final states.

---

## ✨ Features

- ✅ **Complete ε-transition elimination**
- ✅ **Epsilon-closure computation** using fixed-point iteration
- ✅ **Automatic final state adjustment**
- ✅ **Support for multiple transitions** per state-symbol pair
- ✅ **Clear console-based interface**
- ✅ **Well-documented code** with detailed comments
- ✅ **Example test cases** included

---

## 🔧 Algorithm Explanation

The conversion process consists of **three main phases**:

### Phase 1: Epsilon-Closure Calculation

Computes the ε-closure for every state using a **transitive closure algorithm** similar to Floyd-Warshall.

**Algorithm:**
```
1. Initialize: closure[i][i] = 1 for all states i (reflexive property)
2. Repeat until no changes:
   For each state i:
     For each state j in closure[i]:
       For each state k reachable from j via ε:
         Add k to closure[i]
```

**Complexity:** O(n³) where n is the number of states

### Phase 2: Transition Elimination

Constructs new transitions that account for all possible ε-transitions before and after consuming a symbol.

**Algorithm:**
```
For each state s:
  For each state c in ε-closure(s):
    For each symbol σ:
      For each state m such that δ(c, σ) contains m:
        For each state d in ε-closure(m):
          Add transition δ'(s, σ, d)
```

**Intuition:** If we can reach state `c` from `s` via ε, read symbol `σ` to reach `m`, then reach `d` from `m` via ε, we create a direct transition from `s` to `d` on `σ`.

### Phase 3: Final State Adjustment

Any state that can reach a final state through ε-transitions becomes a final state.

**Algorithm:**
```
For each state i:
  For each state j in ε-closure(i):
    If j is final:
      Mark i as final
```

---

## 💻 Installation & Usage

### Compilation

```bash
gcc epsilon_nfa_converter.c -o epsilon_nfa_converter
```

**With optimization:**
```bash
gcc -O2 epsilon_nfa_converter.c -o epsilon_nfa_converter
```

### Execution

```bash
./epsilon_nfa_converter
```

---

## 📝 Input Specification

The program prompts for the following inputs in order:

| Input | Description | Format |
|-------|-------------|--------|
| **Number of states** | Total states (0 to n-1) | Integer |
| **Number of symbols** | Alphabet size | Integer |
| **Alphabet symbols** | Input symbols | Space-separated characters |
| **Initial state** | Starting state | Integer (0 to n-1) |
| **Number of final states** | Count of accepting states | Integer |
| **Final states** | Accepting state numbers | Space-separated integers |
| **Symbol transitions** | Regular transitions | `from_state symbol to_state` (end with -1) |
| **Epsilon transitions** | ε-transitions | `from_state to_state` (end with -1) |

### Input Example Format

```
Number of states: 3
Number of symbols: 2
Alphabet symbols: a b
Initial state: 0
Number of final states: 1
Final states: 2
Symbol transitions (-1 to stop):
0 a 1
1 b 2
-1
Epsilon transitions (-1 to stop):
0 1
-1
```

---

## 🎓 Examples

### Example 1: Simple Epsilon Chain

**Input Automaton:**

```
States: {0, 1, 2}
Alphabet: {a, b}
Start: 0
Final: {2}

Transitions:
  0 --a--> 1
  1 --b--> 2

Epsilon Transitions:
  0 --ε--> 1
```

**Input:**
```
Number of states: 3
Number of symbols: 2
Alphabet symbols: a b
Initial state: 0
Number of final states: 1
Final states: 2
Symbol transitions (-1 to stop):
0 a 1
1 b 2
-1
Epsilon transitions (-1 to stop):
0 1
-1
```

**Output:**
```
===== ε-free NFA =====
Start state: 0
Final states: 0 2
Transitions:
0 --a--> 1
0 --b--> 2
1 --b--> 2
```

**Explanation:**
- State 0 can reach state 1 via ε, so it inherits the transition `1 --b--> 2` as `0 --b--> 2`
- State 0 becomes final because it can reach final state 2 through path: 0 --ε--> 1 --b--> 2

---

### Example 2: Complex Epsilon Chain

**Input Automaton:**

```
States: {0, 1, 2, 3}
Alphabet: {a}
Start: 0
Final: {3}

Transitions:
  1 --a--> 2
  2 --a--> 3

Epsilon Transitions:
  0 --ε--> 1
  1 --ε--> 2
```

**Input:**
```
Number of states: 4
Number of symbols: 1
Alphabet symbols: a
Initial state: 0
Number of final states: 1
Final states: 3
Symbol transitions (-1 to stop):
1 a 2
2 a 3
-1
Epsilon transitions (-1 to stop):
0 1
1 2
-1
```

**Output:**
```
===== ε-free NFA =====
Start state: 0
Final states: 0 1 2 3
Transitions:
0 --a--> 2
0 --a--> 3
1 --a--> 2
1 --a--> 3
2 --a--> 3
```

**Explanation:**
- ε-closure(0) = {0, 1, 2} (0 can reach 1 and 2 via epsilon chain)
- ε-closure(1) = {1, 2}
- ε-closure(2) = {2}
- State 0 can access transitions from states 1 and 2
- All states {0, 1, 2, 3} become final because they can reach state 3

---

## 🏗️ Code Structure

### Data Structures

```c
// Configuration
int state_count;              // Number of states
int symbol_count;             // Alphabet size
char symbols[SYMBOL_LIMIT];   // Input symbols

// Automaton definition
int start_state;              // Initial state
int accepting_states[STATE_LIMIT];  // Final states (1 = final, 0 = non-final)

// Transition tables
int delta[STATE_LIMIT][SYMBOL_LIMIT][STATE_LIMIT];     // Symbol transitions
int epsilon_delta[STATE_LIMIT][STATE_LIMIT];           // ε-transitions
int closure[STATE_LIMIT][STATE_LIMIT];                 // ε-closure
int result_delta[STATE_LIMIT][SYMBOL_LIMIT][STATE_LIMIT]; // Converted transitions
```

### Function Overview

| Function | Purpose | Complexity |
|----------|---------|------------|
| `reset_arrays()` | Initialize all arrays to zero | O(n²s) |
| `collect_input()` | Read automaton from user | O(t) |
| `calculate_closure()` | Compute ε-closure | O(n³) |
| `perform_conversion()` | Eliminate ε-transitions | O(n⁴s) |
| `adjust_final_states()` | Update final states | O(n²) |
| `print_automaton()` | Display result | O(n²s) |

**Note:** n = number of states, s = alphabet size, t = number of transitions

---

## 📊 Complexity Analysis

### Time Complexity

| Phase | Complexity | Dominant Operation |
|-------|------------|-------------------|
| **Initialization** | O(n²s) | Array reset |
| **Input Reading** | O(t) | User input (t transitions) |
| **ε-Closure** | O(n³) | Fixed-point iteration |
| **Conversion** | O(n⁴s) | Nested loops over states/symbols |
| **Final State Adj.** | O(n²) | Closure iteration |
| **Output** | O(n²s) | Printing transitions |

**Overall:** O(n⁴s) dominated by the conversion phase

### Space Complexity

**O(n²s)** for storing:
- Transition tables: `delta`, `result_delta` → O(n²s)
- ε-transitions and closure: `epsilon_delta`, `closure` → O(n²)
- Auxiliary arrays → O(n + s)

---

## ⚙️ Limitations & Extensions

### Current Limitations

| Limitation | Value | Reason |
|------------|-------|--------|
| **Maximum states** | 10 | `STATE_LIMIT` constant |
| **Maximum alphabet size** | 5 | `SYMBOL_LIMIT` constant |
| **Input validation** | Minimal | No bounds checking |
| **State numbering** | 0 to n-1 | Array-based indexing |

### Modifying Limits

To support larger automata, modify the constants:

```c
#define STATE_LIMIT 20    // Increase for more states
#define SYMBOL_LIMIT 10   // Increase for larger alphabet
```

**Note:** Memory usage grows as O(STATE_LIMIT² × SYMBOL_LIMIT)

### Possible Extensions

1. **Input Validation**
   - Validate state numbers are within range
   - Check for duplicate transitions
   - Verify symbol existence in alphabet

2. **Enhanced Features**
   - Named states instead of numbers
   - Export to DOT format for visualization
   - JSON/XML input/output
   - DFA conversion capability
   - Automaton minimization

3. **Performance Optimizations**
   - Sparse matrix representation for large automata
   - Bit vectors for state sets
   - Incremental closure computation

4. **Visualization**
   - Generate state diagrams
   - Interactive web interface
   - Step-by-step conversion animation

---

## 🎯 Applications

This converter is valuable for:

1. **Education**
   - Understanding automata transformations
   - Visualizing ε-transition elimination
   - Learning formal language theory

2. **Compiler Design**
   - Lexical analysis preprocessing
   - Regular expression to NFA conversion
   - Pattern matching optimization

3. **Research**
   - Algorithm testing and verification
   - Automata theory experimentation
   - Formal methods validation

4. **Practical Tools**
   - Text processing engines
   - Protocol verification
   - Model checking preparation

---

## 📚 References

1. **Hopcroft, J. E., Motwani, R., & Ullman, J. D.** (2006).  
   *Introduction to Automata Theory, Languages, and Computation* (3rd ed.).  
   Pearson Education.

2. **Sipser, M.** (2012).  
   *Introduction to the Theory of Computation* (3rd ed.).  
   Cengage Learning.

3. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006).  
   *Compilers: Principles, Techniques, and Tools* (2nd ed.).  
   Addison-Wesley.

4. **Kozen, D. C.** (1997).  
   *Automata and Computability*.  
   Springer-Verlag.

---

## 🛠️ Troubleshooting

### Common Issues

**Issue 1: Unexpected Final States**

```
Cause: ε-transitions create paths to final states
Solution: This is correct behavior - states that can reach 
          final states via ε must become final
```

**Issue 2: Missing Transitions**

```
Cause: Incomplete ε-transition input
Solution: Ensure all ε-transitions are entered before typing -1
```

**Issue 3: Program Crashes**

```
Cause: State numbers exceed STATE_LIMIT
Solution: Increase STATE_LIMIT or reduce input size
```

---

## 📄 License

This educational software is provided **as-is** for learning purposes.  
Free to use, modify, and distribute for educational and non-commercial purposes.

---

## 👥 Contributors

**Benshrif Mouataz**  
**Ameur Zineddine**

*Automata Theory Project*  
*Computer Science Department*

---

## 🙏 Acknowledgments

We would like to thank our professor for providing guidance on automata theory and for the opportunity to implement this fundamental algorithm. This project has deepened our understanding of finite automata and their applications in computer science.

---

**Last Updated:** January 2026  
**Version:** 1.0

---

