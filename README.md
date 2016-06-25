### LRUCache
- A C++11 implementation of LRU cache based on bimap of boost library.
- This implementation uses locking to prevent cache stampede or dogpiling (https://en.wikipedia.org/wiki/Cache_stampede).
- It is thread-safe.

### Dependencies
- boost library
- googletest library (optional)

### Example
- Refer to test cases in src/test
