# Galaxy Builder Test Suite

This directory contains comprehensive unit tests and integration tests for the Galaxy Builder project.

## Test Structure

### Test Files

1. **test_galaxy.cpp** - Core galaxy functionality tests
2. **test_systemdatamanager.cpp** - SystemDataManager XML import/export and persistence tests  
3. **test_xmlvalidator.cpp** - XmlValidator validation and error checking tests
4. **test_integration.cpp** - Integration tests between SystemDataManager and XmlValidator

### Test Categories

#### Unit Tests
- **SystemDataManager**: Tests XML export/import, persistence, auto-save functionality
- **XmlValidator**: Tests XML structure validation, data validation, schema validation
- **Galaxy Core**: Tests core galaxy and star system functionality

#### Integration Tests
- **SystemDataManager + XmlValidator**: Tests that both components work together correctly
- **Round-trip validation**: Tests export → validate → import → validate cycles
- **Performance tests**: Tests with large datasets and complex systems
- **Error handling**: Tests recovery from validation errors and corrupted files

## Running Tests

### Using the Test Runner (Recommended)

#### PowerShell (Windows)
```powershell
# Run all tests
.\run_tests.ps1

# Build only (no test execution)
.\run_tests.ps1 -BuildOnly

# Run only specific tests
.\run_tests.ps1 -TestFilter "SystemData"

# Verbose output
.\run_tests.ps1 -Verbose
```

#### Command Prompt (Windows)
```cmd
# Run all tests
run_tests.bat
```

### Manual Test Execution

1. **Build the project with tests enabled:**
   ```bash
   cd build
   cmake -DBUILD_TESTING=ON ..
   cmake --build . --target all
   ```

2. **Run individual test executables:**
   ```bash
   cd build/bin/tests
   ./test_galaxy.exe
   ./test_systemdatamanager.exe
   ./test_xmlvalidator.exe
   ./test_integration.exe
   ```

### Using CTest

```bash
cd build
ctest --verbose
```

## Test Coverage

### SystemDataManager Tests
- ✅ XML export for single systems
- ✅ XML export for galaxy collections
- ✅ XML import for single systems  
- ✅ XML import for galaxy collections
- ✅ Persistence and auto-save functionality
- ✅ Error handling for invalid files/paths
- ✅ Round-trip data integrity
- ✅ Special character handling
- ✅ Edge cases (extreme values, large datasets)

### XmlValidator Tests
- ✅ Basic XML well-formedness validation
- ✅ Structure validation (required elements/attributes)
- ✅ Data type validation
- ✅ Numeric range validation
- ✅ Schema validation
- ✅ Performance testing with large files
- ✅ Unicode and special character support
- ✅ Error reporting and diagnostics

### Integration Tests
- ✅ SystemDataManager export + XmlValidator validation
- ✅ XmlValidator validation + SystemDataManager import
- ✅ Round-trip testing with validation at each step
- ✅ Performance testing with validation enabled
- ✅ Error recovery and handling integration
- ✅ Batch operations with validation

## Test Data

The tests create temporary files and directories for testing. All test data is cleaned up automatically after test completion.

### Sample Test Data
- Simple star systems with 0-3 planets
- Complex galaxies with multiple star systems
- Edge case systems (extreme values, special characters)
- Large datasets (50+ systems, 100+ planets)
- Invalid/corrupted XML files for error testing

## Performance Benchmarks

The test suite includes performance benchmarks to ensure that:
- XML export completes in < 1 second for complex systems
- XML validation completes in < 1 second for large galaxies  
- XML import completes in < 1 second for complex systems
- Round-trip operations maintain data integrity

## Adding New Tests

### Creating Unit Tests

1. Add test methods to the appropriate test class
2. Follow the naming convention: `test[FeatureName][Scenario]()`
3. Use helper methods to create test data
4. Verify results with QVERIFY/QCOMPARE macros
5. Clean up test objects in cleanup() or with deleteLater()

### Creating Integration Tests

1. Add methods to `TestIntegration` class in `test_integration.cpp`
2. Test interaction between multiple components
3. Verify end-to-end workflows
4. Include performance and error recovery testing

### Example Test Method

```cpp
void TestClass::testNewFeature()
{
    // Arrange
    StarSystemQml* system = createTestSystem(1, "Test System");
    QString filePath = m_tempDir->filePath("test.xml");
    
    // Act
    bool result = m_dataManager->exportSystemToXml(system, filePath);
    
    // Assert
    QVERIFY(result);
    QVERIFY(QFile::exists(filePath));
    
    // Cleanup
    system->deleteLater();
}
```

## Test Dependencies

- Qt6::Test - QTest framework
- Qt6::Core - Core Qt functionality  
- Qt6::Xml - XML parsing and DOM
- GalaxyCore - Project modules under test

## Troubleshooting

### Common Issues

1. **Tests fail to build**: Ensure GalaxyCore module builds successfully
2. **Tests crash on startup**: Check that all required Qt modules are linked
3. **File permission errors**: Ensure test has write access to temp directory
4. **XML validation failures**: Check that XML output matches expected schema

### Debug Mode

Run tests with verbose output to see detailed information:
```bash
./test_name.exe -v2
```

### Memory Debugging

For memory leak detection, run tests with Qt's debug tools:
```bash
export QT_LOGGING_RULES="qt.test.memory=true"
./test_name.exe
```

## Continuous Integration

The test suite is designed to run in CI environments. Use the following commands:

```bash
# Configure for CI
cmake -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Debug ..

# Build and run tests
cmake --build . --target all
ctest --output-on-failure --verbose
```

Exit codes:
- 0: All tests passed
- 1: One or more tests failed
- 2: Build or configuration error
