$cFile = "main.c"
$outFile = "main.exe"


gcc -Wall -g $cFile -o $outFile

if ($LASTEXITCODE -eq 0) {
    Write-Host "Success! Executable created: $outFile" -ForegroundColor Green

    Write-Host "Running application..." -ForegroundColor Yellow
    & ".\$outFile"
}