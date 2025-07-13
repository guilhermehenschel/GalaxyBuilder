# Galaxy Builder - Windows Build Script
# Supports both Debug and Release builds using CMake Presets

param(
    [string]$QtPath = "",
    [ValidateSet("Debug", "Release")]
    [string]$BuildType = "Release",
    [switch]$Clean,
    [switch]$Install,
    [switch]$Run,
    [switch]$Help
)

if ($Help) {
    Write-Host "Galaxy Builder - Windows Build Script" -ForegroundColor Cyan
    Write-Host "Usage: .\build.ps1 [OPTIONS]" -ForegroundColor White
    Write-Host ""
    Write-Host "Options:" -ForegroundColor Yellow
    Write-Host "  -QtPath <path>     Path to Qt6 installation"
    Write-Host "  -BuildType <type>  Debug or Release (default: Release)"
    Write-Host "  -Clean             Clean before building"
    Write-Host "  -Install           Install after building"
    Write-Host "  -Run               Run the application after building"
    Write-Host "  -Help              Show this help message"
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Green
    Write-Host "  .\build.ps1 -BuildType Debug"
    Write-Host "  .\build.ps1 -QtPath 'C:\Qt\6.6.0\msvc2022_64' -Clean"
    Write-Host "  .\build.ps1 -BuildType Release -Install -Run"
    exit 0
}

Write-Host "Galaxy Builder - Windows Build Script" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan

# Check if Qt6 path is provided or try to find it
if (-not $QtPath) {
    Write-Host "Searching for Qt6 installation..." -ForegroundColor Blue
    $QtPaths = @(
        "C:\Qt\6.6.0\msvc2022_64",
        "C:\Qt\6.5.0\msvc2022_64",
        "C:\Qt\6.4.0\msvc2022_64",
        "C:\Qt\6.3.0\msvc2022_64"
    )
    
    foreach ($path in $QtPaths) {
        if (Test-Path $path) {
            $QtPath = $path
            Write-Host "Found Qt6 at: $QtPath" -ForegroundColor Green
            break
        }
    }
    
    if (-not $QtPath) {
        Write-Host "Qt6 not found automatically. Please specify the Qt6 path using -QtPath parameter" -ForegroundColor Red
        Write-Host "Example: .\build.ps1 -QtPath 'C:\Qt\6.6.0\msvc2022_64'" -ForegroundColor Yellow
        exit 1
    }
} else {
    Write-Host "Using specified Qt6 path: $QtPath" -ForegroundColor Green
}

# Verify Qt6 path exists
if (-not (Test-Path $QtPath)) {
    Write-Host "Qt6 path does not exist: $QtPath" -ForegroundColor Red
    exit 1
}

Write-Host "Build Type: $BuildType" -ForegroundColor Green

# Set Qt6 environment
$env:Qt6_DIR = "$QtPath\lib\cmake\Qt6"
$env:CMAKE_PREFIX_PATH = $QtPath
$env:PATH = "$QtPath\bin;$env:PATH"

# Clean build directory if requested
if ($Clean) {
    $buildDir = "build\windows-$($BuildType.ToLower())"
    if (Test-Path $buildDir) {
        Write-Host "Cleaning build directory: $buildDir" -ForegroundColor Yellow
        Remove-Item -Recurse -Force $buildDir
    }
}

# Configure using CMake preset
$presetName = "windows-$($BuildType.ToLower())"
Write-Host "Configuring with preset: $presetName" -ForegroundColor Blue
cmake --preset $presetName

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    exit 1
}

# Build the project
Write-Host "Building project..." -ForegroundColor Blue
cmake --build "build\windows-$($BuildType.ToLower())" --config $BuildType

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "Build completed successfully!" -ForegroundColor Green

# Install if requested
if ($Install) {
    Write-Host "Installing..." -ForegroundColor Blue
    cmake --install "build\windows-$($BuildType.ToLower())" --config $BuildType
}

# Determine executable path
$executablePath = "build\windows-$($BuildType.ToLower())\bin\GalaxyBuilder.exe"
if ($BuildType -eq "Debug") {
    $executablePath = "build\windows-$($BuildType.ToLower())\bin\GalaxyBuilderd.exe"
}

Write-Host "Executable location: $executablePath" -ForegroundColor Green

# Copy Qt6 DLLs for development builds (not needed for installed versions)
if (-not $Install) {
    Write-Host "Copying Qt6 dependencies..." -ForegroundColor Blue
    $targetDir = "build\windows-$($BuildType.ToLower())\bin"
    
    # Ensure target directory exists
    if (-not (Test-Path $targetDir)) {
        New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
    }
    
    $QtDlls = @(
        "Qt6Core.dll",
        "Qt6Gui.dll",
        "Qt6Widgets.dll"
    )
    
    foreach ($dll in $QtDlls) {
        $sourcePath = Join-Path "$QtPath\bin" $dll
        if (Test-Path $sourcePath) {
            Copy-Item $sourcePath $targetDir -Force
            Write-Host "  Copied $dll" -ForegroundColor Gray
        } else {
            Write-Host "  Warning: $dll not found at $sourcePath" -ForegroundColor Yellow
        }
    }
    
    # Copy Qt6 platform plugins
    $platformDir = "$targetDir\platforms"
    if (-not (Test-Path $platformDir)) {
        New-Item -ItemType Directory -Path $platformDir -Force | Out-Null
    }
    
    $platformDll = "$QtPath\plugins\platforms\qwindows.dll"
    if (Test-Path $platformDll) {
        Copy-Item $platformDll $platformDir -Force
        Write-Host "  Copied qwindows.dll" -ForegroundColor Gray
    } else {
        Write-Host "  Warning: qwindows.dll not found" -ForegroundColor Yellow
    }
}

# Run if requested
if ($Run) {
    if (Test-Path $executablePath) {
        Write-Host "Running Galaxy Builder..." -ForegroundColor Blue
        & $executablePath
    } else {
        Write-Host "Executable not found: $executablePath" -ForegroundColor Red
        exit 1
    }
}

Write-Host ""
Write-Host "Ready to run! Execute: $executablePath" -ForegroundColor Green
