#!/usr/bin/env pwsh
# build.ps1 - Clean, configure, and build with CMake + Ninja

Write-Host "Removing old build directory..." -ForegroundColor Cyan
if (Test-Path build) {
    Remove-Item -Recurse -Force build
    Write-Host "Removed build directory." -ForegroundColor Green
}

Write-Host "Configuring project with CMake..." -ForegroundColor Cyan
cmake -S . -B build
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit $LASTEXITCODE
}

Write-Host "Entering build directory..." -ForegroundColor Cyan
Set-Location -Path build
if ($?) {
    Write-Host "Running Ninja build..." -ForegroundColor Cyan
    ninja
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Ninja build failed."
        exit $LASTEXITCODE
    }
} else {
    Write-Error "Failed to enter build directory."
    exit 1
}

Write-Host "Build completed successfully." -ForegroundColor Green