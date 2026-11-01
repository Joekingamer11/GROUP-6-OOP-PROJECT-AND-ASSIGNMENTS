cmake --build build
Get-ChildItem build -Recurse -Filter "example_*.exe" | ForEach-Object {
    Write-Host "=== $($_.Name) ===" -ForegroundColor Cyan
    & $_.FullName
}