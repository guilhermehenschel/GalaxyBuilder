#!/usr/bin/env powershell

# Test runner script for Galaxy Builder
# This script builds and runs all unit tests

param(
    [switch]$BuildOnly,
    [switch]$TestOnly,
    [string]$TestFilter = "",
    [switch]$Verbose
)

$ErrorActionPreference = "Stop"

Write-Host "Galaxy Builder Test Runner" -ForegroundColor Green
Write-Host "============================" -ForegroundColor Green

# Get script directory and project root
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Split-Path -Parent $ScriptDir
$BuildDir = Join-Path $ProjectRoot "build"
$TestDir = Join-Path $BuildDir "bin" "tests"

if (-not $TestOnly) {
    Write-Host "`nBuilding tests..." -ForegroundColor Yellow
    
    # Create build directory if it doesn't exist
    if (-not (Test-Path $BuildDir)) {
        New-Item -Path $BuildDir -ItemType Directory -Force | Out-Null
    }
    
    # Configure with CMake
    Set-Location $BuildDir
    $configureArgs = @(
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DBUILD_TESTING=ON",
        ".."
    )
    
    Write-Host "Configuring with CMake..." -ForegroundColor Cyan
    & cmake @configureArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        exit 1
    }
    
    # Build the project
    Write-Host "Building project..." -ForegroundColor Cyan
    & cmake --build . --target all --parallel
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed"
        exit 1
    }
    
    Write-Host "Build completed successfully!" -ForegroundColor Green
}

if ($BuildOnly) {
    Write-Host "Build-only mode. Exiting." -ForegroundColor Yellow
    exit 0
}

if (-not (Test-Path $TestDir)) {
    Write-Error "Test directory not found: $TestDir"
    exit 1
}

Write-Host "`nRunning tests..." -ForegroundColor Yellow
Set-Location $TestDir

# Define test executables
$TestExecutables = @(
    @{Name="Galaxy Core Tests"; Exe="test_galaxy.exe"},
    @{Name="System Data Manager Tests"; Exe="test_systemdatamanager.exe"},
    @{Name="XML Validator Tests"; Exe="test_xmlvalidator.exe"},
    @{Name="Integration Tests"; Exe="test_integration.exe"}
)

$TotalTests = 0
$PassedTests = 0
$FailedTests = 0
$FailedTestDetails = @()

foreach ($Test in $TestExecutables) {
    if ($TestFilter -and $Test.Name -notlike "*$TestFilter*") {
        continue
    }
    
    $TestExe = Join-Path $TestDir $Test.Exe
    
    if (-not (Test-Path $TestExe)) {
        Write-Warning "Test executable not found: $TestExe"
        continue
    }
    
    Write-Host "`nRunning: $($Test.Name)" -ForegroundColor Cyan
    Write-Host ("=" * 50) -ForegroundColor Cyan
    
    $TestArgs = @()
    if ($Verbose) {
        $TestArgs += "-v2"
    }
    
    try {
        $output = & $TestExe @TestArgs 2>&1
        $exitCode = $LASTEXITCODE
        
        # Parse QTest output
        $testResults = $output | Where-Object { $_ -match "^PASS|^FAIL|^SKIP" }
        $summary = $output | Where-Object { $_ -match "Totals:" }
        
        if ($exitCode -eq 0) {
            Write-Host "PASSED" -ForegroundColor Green
            $PassedTests++
        } else {
            Write-Host "FAILED" -ForegroundColor Red
            $FailedTests++
            $FailedTestDetails += @{
                Name = $Test.Name
                Output = $output
                ExitCode = $exitCode
            }
        }
        
        if ($Verbose -or $exitCode -ne 0) {
            Write-Host "Output:" -ForegroundColor Gray
            $output | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }
        } elseif ($summary) {
            Write-Host "  $summary" -ForegroundColor Gray
        }
        
        $TotalTests++
    }
    catch {
        Write-Error "Failed to run test: $($_.Exception.Message)"
        $FailedTests++
        $TotalTests++
    }
}

# Summary
Write-Host "`n" + ("=" * 60) -ForegroundColor Yellow
Write-Host "TEST SUMMARY" -ForegroundColor Yellow
Write-Host ("=" * 60) -ForegroundColor Yellow
Write-Host "Total tests run: $TotalTests" -ForegroundColor White
Write-Host "Passed: $PassedTests" -ForegroundColor Green
Write-Host "Failed: $FailedTests" -ForegroundColor Red

if ($FailedTests -gt 0) {
    Write-Host "`nFailed test details:" -ForegroundColor Red
    foreach ($Failed in $FailedTestDetails) {
        Write-Host "`n$($Failed.Name) (Exit code: $($Failed.ExitCode))" -ForegroundColor Red
        if ($Verbose) {
            $Failed.Output | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }
        }
    }
    
    Write-Host "`nSome tests failed. Please check the output above." -ForegroundColor Red
    exit 1
} else {
    Write-Host "`nAll tests passed!" -ForegroundColor Green
    exit 0
}
