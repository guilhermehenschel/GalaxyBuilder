# Galaxy Builder - Windows Deployment Script
# This script deploys Qt6 dependencies and creates packages

param(
    [string]$BuildType = "Release",
    [string]$QtDir = "",
    [switch]$CreateInstaller = $false,
    [switch]$CreatePortable = $false,
    [switch]$SkipDeploy = $false
)

$ErrorActionPreference = "Stop"

# Get script directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectDir = $ScriptDir

Write-Host "Galaxy Builder Windows Deployment" -ForegroundColor Green
Write-Host "=================================" -ForegroundColor Green

# Determine build directory
$BuildDir = Join-Path $ProjectDir "build\windows-$(${BuildType}.ToLower())"
$BinaryDir = Join-Path $BuildDir "bin"

if (-not (Test-Path $BinaryDir)) {
    Write-Error "Build directory not found: $BinaryDir"
    Write-Host "Please build the project first using:" -ForegroundColor Yellow
    Write-Host "  cmake --preset=windows-$(${BuildType}.ToLower())" -ForegroundColor Yellow
    Write-Host "  cmake --build $BuildDir --config $BuildType" -ForegroundColor Yellow
    exit 1
}

# Find Qt6 deployment tool
if ([string]::IsNullOrEmpty($QtDir)) {
    # Try to detect Qt6 installation
    $QtPaths = @(
        "C:\Qt\6.9.1\msvc2022_64",
        "C:\Qt\6.7.2\msvc2022_64",
        "C:\Qt\6.6.0\msvc2022_64",
        "C:\Qt\6.5.0\msvc2022_64"
    )
    
    foreach ($Path in $QtPaths) {
        if (Test-Path $Path) {
            $QtDir = $Path
            break
        }
    }
}

if ([string]::IsNullOrEmpty($QtDir) -or -not (Test-Path $QtDir)) {
    Write-Error "Qt6 installation not found. Please specify -QtDir parameter."
    exit 1
}

$WindeployQt = Join-Path $QtDir "bin\windeployqt.exe"
if (-not (Test-Path $WindeployQt)) {
    Write-Error "windeployqt.exe not found at: $WindeployQt"
    exit 1
}

Write-Host "Using Qt6 from: $QtDir" -ForegroundColor Cyan
Write-Host "Build directory: $BuildDir" -ForegroundColor Cyan

# Deploy Qt6 dependencies
if (-not $SkipDeploy) {
    Write-Host "`nDeploying Qt6 dependencies..." -ForegroundColor Yellow
    
    $ExePath = if ($BuildType -eq "Debug") {
        Join-Path $BinaryDir "GalaxyBuilderd.exe"
    } else {
        Join-Path $BinaryDir "GalaxyBuilder.exe"
    }
    
    if (-not (Test-Path $ExePath)) {
        Write-Error "Executable not found: $ExePath"
        exit 1
    }
    
    # Run windeployqt
    $DeployArgs = @(
        "--dir", $BinaryDir,
        "--libdir", $BinaryDir,
        "--plugindir", $BinaryDir,
        "--no-translations",
        "--no-system-d3d-compiler",
        "--no-opengl-sw"
    )
    
    if ($BuildType -eq "Debug") {
        $DeployArgs += "--debug"
    } else {
        $DeployArgs += "--release"
    }
    
    $DeployArgs += $ExePath
    
    Write-Host "Running: $WindeployQt $($DeployArgs -join ' ')" -ForegroundColor Gray
    & $WindeployQt @DeployArgs
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "windeployqt failed with exit code $LASTEXITCODE"
        exit 1
    }
    
    Write-Host "Qt6 dependencies deployed successfully!" -ForegroundColor Green
}

# Create portable package
if ($CreatePortable) {
    Write-Host "`nCreating portable package..." -ForegroundColor Yellow
    
    $PortableDir = Join-Path $ProjectDir "dist\portable-$BuildType"
    if (Test-Path $PortableDir) {
        Remove-Item $PortableDir -Recurse -Force
    }
    New-Item -ItemType Directory -Path $PortableDir -Force | Out-Null
    
    # Copy all files from binary directory
    Copy-Item "$BinaryDir\*" $PortableDir -Recurse -Force
    
    # Copy resources
    $ResourcesDir = Join-Path $ProjectDir "resources"
    if (Test-Path $ResourcesDir) {
        Copy-Item $ResourcesDir $PortableDir -Recurse -Force
    }
    
    # Create README for portable version
    $PortableReadme = @"
Galaxy Builder - Portable Version
=================================

This is a portable version of Galaxy Builder that doesn't require installation.

To run:
1. Extract all files to a folder
2. Run GalaxyBuilder.exe

System Requirements:
- Windows 10 or later
- Visual C++ Redistributable (usually already installed)

For more information, visit: https://github.com/galaxybuilder/galaxybuilder
"@
    
    $PortableReadme | Out-File -FilePath (Join-Path $PortableDir "README.txt") -Encoding UTF8
    
    # Create ZIP archive
    $ZipPath = Join-Path $ProjectDir "dist\GalaxyBuilder-$BuildType-Portable.zip"
    if (Test-Path $ZipPath) {
        Remove-Item $ZipPath -Force
    }
    
    Write-Host "Creating ZIP archive: $ZipPath" -ForegroundColor Cyan
    Compress-Archive -Path "$PortableDir\*" -DestinationPath $ZipPath
    
    Write-Host "Portable package created: $ZipPath" -ForegroundColor Green
}

# Create installer
if ($CreateInstaller) {
    Write-Host "`nCreating Windows installer..." -ForegroundColor Yellow
    
    # Check if NSIS is installed
    $NsisPath = ""
    $NsisPaths = @(
        "${env:ProgramFiles}\NSIS\makensis.exe",
        "${env:ProgramFiles(x86)}\NSIS\makensis.exe"
    )
    
    foreach ($Path in $NsisPaths) {
        if (Test-Path $Path) {
            $NsisPath = $Path
            break
        }
    }
    
    if ([string]::IsNullOrEmpty($NsisPath)) {
        Write-Warning "NSIS not found. Please install NSIS from https://nsis.sourceforge.io/"
        Write-Host "Skipping installer creation." -ForegroundColor Yellow
    } else {
        Write-Host "Using NSIS from: $NsisPath" -ForegroundColor Cyan
        
        # Use CMake to create installer
        Push-Location $BuildDir
        try {
            Write-Host "Running CMake installer target..." -ForegroundColor Gray
            cmake --build . --target installer --config $BuildType
            
            if ($LASTEXITCODE -eq 0) {
                Write-Host "Windows installer created successfully!" -ForegroundColor Green
                
                # Find the created installer
                $InstallerFiles = Get-ChildItem $BuildDir -Name "GalaxyBuilder-*-Setup.exe"
                if ($InstallerFiles) {
                    $InstallerPath = Join-Path $BuildDir $InstallerFiles[0]
                    $DestPath = Join-Path $ProjectDir "dist\$($InstallerFiles[0])"
                    
                    # Ensure dist directory exists
                    $DistDir = Join-Path $ProjectDir "dist"
                    if (-not (Test-Path $DistDir)) {
                        New-Item -ItemType Directory -Path $DistDir -Force | Out-Null
                    }
                    
                    Copy-Item $InstallerPath $DestPath -Force
                    Write-Host "Installer copied to: $DestPath" -ForegroundColor Green
                }
            } else {
                Write-Warning "Installer creation failed. Check the CMake output above."
            }
        } finally {
            Pop-Location
        }
    }
}

Write-Host "`nDeployment completed!" -ForegroundColor Green
Write-Host "Output files are in the dist/ directory" -ForegroundColor Cyan
