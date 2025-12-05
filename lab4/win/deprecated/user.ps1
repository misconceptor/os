[System.Environment]::SetEnvironmentVariable("PROC_TO_KILL", "explorer,telegram", "User")
$Env:PROC_TO_KILL = [System.Environment]::GetEnvironmentVariable("PROC_TO_KILL", "User")
$procNames = $Env:PROC_TO_KILL -split ","

Write-Host "Before killing:"
foreach ($n in $procNames) {
    $cur = $n.Trim()
    if (![string]::IsNullOrWhiteSpace($cur)) {
        $found = Get-Process -Name $cur -ErrorAction SilentlyContinue
        if ($found) {
            Write-Host "$cur is running"
        } else {
            Write-Host "$cur is not running"
        }
    }
}
Start-Process -FilePath "powershell.exe" `
    -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File C:\Users\Dell\Downloads\proc\killer.ps1" `
    -Verb RunAs -Wait
Write-Host "Processes after killing:"
foreach ($n in $procNames) {
    $cur = $n.Trim()
    if (![string]::IsNullOrWhiteSpace($cur)) {
        $found = Get-Process -Name $cur -ErrorAction SilentlyContinue
        if ($found) {
            Write-Host "$cur is still running"
        } else {
            Write-Host "$cur has been terminated"
        }
    }
}
[System.Environment]::SetEnvironmentVariable("PROC_TO_KILL", $null, "User")
