param(
    [string]$name,
    [int]$id
)

function Kill-ProcessById($pid) {
    try {
        Stop-Process -Id $pid -Force -PassThru -ErrorAction Stop | ForEach-Object {
            echo "Killed process ID $($_.Id)"
        }
    } catch {
        echo "Failed to kill process ID $pid: $_"
    }
}

function Kill-ProcessByName($procName) {
    try {
        if ($procName -ieq "explorer") {
            echo "Killing Explorer using taskkill"
            taskkill /F /IM explorer.exe /T
            Start-Sleep -Seconds 2
        } else {
            Stop-Process -Name $procName -Force -PassThru -ErrorAction Stop | ForEach-Object {
                echo "Killed process $($_.ProcessName) (ID $($_.Id))"
            }
        }
    } catch {
        echo "Failed to kill process $procName: $_"
    }
}

if ($id) {
    echo "Kill process with id $id"
    Kill-ProcessById $id
}

if ($name) {
    echo "Kill process named $name"
    Kill-ProcessByName $name
}

if (-not $name -and -not $id -and $Env:PROC_TO_KILL) {
    $procNames = $Env:PROC_TO_KILL -split ","
    foreach ($proc in $procNames) {
        $trimmed = $proc.Trim()
        if (![string]::IsNullOrWhiteSpace($trimmed)) {
            echo "Killing processes named: $trimmed"
            Kill-ProcessByName $trimmed
        }
    }
}
