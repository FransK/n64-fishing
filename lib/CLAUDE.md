# lib - CLAUDE.md

## Testing Requirements

When working in the `lib/` directory:

1. **Always run unit tests** after making any code changes:
   ```bash
   cd lib/test/build && cmake .. && make -j4 && ./tests
   ```

2. **Add unit tests** for any new functionality or bug fixes. Tests are located in `lib/test/` and use Catch2.

3. Test file naming convention: `<ClassName>Test.cpp` (e.g., `JsonParserTest.cpp`, `StringHashTest.cpp`)
