# Run all INTRIVIZ demonstration examples in PowerShell
# Usage from the repository root: .\run_examples.ps1

$ErrorActionPreference = "Stop"

if (-not (Test-Path "build")) {
    Write-Host "Build folder not found. Please run: cmake -S . -B build -G `"MinGW Makefiles`" -DINTRIVIZ_BUILD_EXAMPLES=ON" -ForegroundColor Yellow
    exit 1
}

# Compile any changed files first
cmake --build build -j4

# Ensure data/output directory exists
if (-not (Test-Path "data/output")) {
    New-Item -ItemType Directory -Path "data/output" -Force | Out-Null
}

$examples = @(
    "example_annotations.exe",
    "example_dashboard.exe",
    "example_heatmap.exe",
    "example_load_and_describe.exe",
    "example_pointplot.exe",
    "example_swarmplot.exe",
    "example_themes.exe",
    "example_week3_showcase.exe",
    "example_week_2.exe",
    "week1_showcase.exe"
)

$passed = 0
$failed = 0

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "  Running All 10 INTRIVIZ Examples        " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

foreach ($name in $examples) {
    # Check possible build locations (flat build or Release subfolder)
    $exePath = ""
    if (Test-Path "build\$name") {
        $exePath = "build\$name"
    } elseif (Test-Path "build\Release\$name") {
        $exePath = "build\Release\$name"
    }

    if (-not $exePath) {
        Write-Host "SKIP: $name (binary not found)" -ForegroundColor Yellow
        $failed++
        continue
    }

    Write-Host "`n>>> Running: $name" -ForegroundColor Green
    try {
        & ".\$exePath"
        if ($LASTEXITCODE -eq 0 -or $null -eq $LASTEXITCODE) {
            Write-Host "OK: $name" -ForegroundColor Green
            $passed++
        } else {
            Write-Host "FAILED: $name (Exit Code $LASTEXITCODE)" -ForegroundColor Red
            $failed++
        }
    } catch {
        Write-Host "ERROR: $name - $_" -ForegroundColor Red
        $failed++
    }
}

Write-Host "`n==========================================" -ForegroundColor Cyan
Write-Host "Summary: $passed Passed, $failed Failed/Skipped" -ForegroundColor Cyan
Write-Host "Generated BMP images are located in: data/output/" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

if ($failed -gt 0) { exit 1 }
