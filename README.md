# XORList: Efficient C++ Linked List (No-throw by default)

This repository implements an XOR Linked List (XORList) in C++, offering:
- **Single Header File:** Easy to include in your C++ projects.
- **Space Efficiency:** Utilizes the XOR operation to store pointers, reducing memory usage compared to traditional linked lists.
- **Constant-time Operations:** Enables insertions/deletions at any position in constant time.
- **Optional Throwing Behavior:** Choose between a no-throw (`CanThrow::NoThrow`) and throwing (`CanThrow::Throw`) version using the template parameter.

## Table of Contents
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building](#building)
  - [Running Tests](#running-tests)
- [Usage](#usage)
  - [Example Usage](#example-usage)
- [Contributing](#contributing)
- [License](#license)
- [Additional Notes](#additional-notes)

## Getting Started
**🚀🚀Faster than std::list🚀🚀**
![](/benchmark/assets/benchmark_comparison.png)
![](/benchmark/assets/benchmark_iterators.png)
![](/benchmark/assets/benchmark_algorithm.png)
![](/benchmark/assets/benchmark_uniq_sort.png)
![](/benchmark/assets/benchmark_reserve.png)
![](/benchmark/assets/benchmark_resize.png)

### Prerequisites

- A C++ compiler supporting C++17, such as:
  - GCC ([https://gcc.gnu.org/](https://gcc.gnu.org/))
  - Clang ([https://clang.llvm.org/](https://clang.llvm.org/))
  - Microsoft Visual C++ ([https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/))
- CMake build system ([https://cmake.org/](https://cmake.org/))

### Building

1. Clone this repository.
2. Use CMake to build the project:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

- To run all tests:

```bash
ctest
```

- To run a specific test:

```bash
./build/t_xorlist
```

## Usage

1. Include the `XORList.hpp` header file in your C++ code:

```c++
#include <XORList.hpp>
```

2. Create an instance of `XORList` with the desired data type:

```c++
scc::XORList<int> myList;
```

3. Use member functions to manipulate the list:

- `push_front(value)`: Inserts a new element at the front of the list.
- `push_back(value)`: Inserts a new element at the back of the list.
- `pop_front()`: Removes the element at the front of the list. (Throws an exception if the list is empty with `scc::CanThrow::Throw`)
- `pop_back()`: Removes the element at the back of the list. (Throws an exception if the list is empty with `scc::CanThrow::Throw`)
- `insert(position, value)`: Inserts a new element at the specified position (0-based indexing). (Throws an exception for out-of-bounds positions with `scc::CanThrow::Throw`)
- `erase(position)`: Removes the element at the specified position. (Throws an exception for out-of-bounds positions with `scc::CanThrow::Throw`)
- `size()`: Returns the number of elements in the list.
- `empty()`: Checks if the list is empty.
- `front()`: Returns a reference to the element at the front of the list. (Throws an exception if the list is empty with `scc::CanThrow::Throw`)
- `back()`: Returns a reference to the element at the back of the list. (Throws an exception if the list is empty with `scc::CanThrow::Throw`)
- `reverse()`: Reverses the order of elements in the list.
- `clear()`: Removes all elements from the list.
- `begin()`: Returns an iterator to the beginning of the list.
- `cbegin() const`: Returns a const iterator to the beginning of the list.
- `end()`: Returns an iterator to the end of the list.
- `cend() const`: Returns a const iterator to the end of the list.
- `rbegin()`: Returns a reverse iterator to the beginning of the reversed list.
- `crbegin() const`: Returns a const reverse iterator to the beginning of the reversed list.
- `rend()`: Returns a reverse iterator to the end of the reversed list.
- `crend() const`: Returns a const reverse iterator to the end of the reversed list.
- `max_size() const`: Returns the maximum possible number of elements in the list.
- `emplace(position, args...)`: Constructs elements in place at the specified position. (Throws an exception for out-of-bounds positions with `scc::CanThrow::Throw`)
- `emplace_back(args...)`: Constructs elements in place at the end of the list.
- `resize(count, value)`: Resizes the list to contain `count` elements, initializing new elements with `value`.
- `unique()`: Removes consecutive duplicate elements from the list.
- `sort()`: Sorts the elements in the list.
- `splice(position, other_list)`: Transfers elements from `other_list` into this list at the specified position.
- `merge(other_list)`: Merges elements from `other_list` into this list, maintaining sorted order.
- `get_allocator() const`: Returns a copy of the allocator object used by the list.
- `assign(count, value)`: Assigns new contents to the list, replacing its current contents.

### Example Usage

```c++
#include <XORList.hpp>

int main() {
  scc::XORList<std::string> names;
  names.push_back("Alice");
  names.push_front("Bob");
  names.insert(1, "Charlie");

  std::cout << "List size: " << names.size() << std::endl;  // Output: List size: 3

  std::cout << "Front: " << names.front() << std::endl;        // Output: Front: Bob
  std::cout << "Back: " << names.back() << std::endl;          // Output: Back: Alice

  names.pop_back();
  std::cout << "After pop_back: " << names.front() << std::endl;  // Output: After pop_back: Bob

  return 0;
}
```

## Contributing

We welcome contributions to this project. Please see the CONTRIBUTING.md file (if available) for guidelines on how to contribute.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Additional Notes

- The `CMakeLists.txt` file configures the build process, including Google Test integration for unit testing. (Refer to the file for detailed explanations)
