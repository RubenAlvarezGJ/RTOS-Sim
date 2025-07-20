$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force -Path "build" | Out-Null
Set-Location -Path "build"
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
Write-Host "`n Build complete."
Write-Host "Run simulator with: .\Release\rtos_kernel_sim.exe"
Write-Host "Run tests with:     .\Release\tests.exe"