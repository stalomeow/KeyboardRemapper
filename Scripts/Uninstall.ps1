param (
    [string]$InstallDir
)

$AppName = "KeyboardRemapper"
$AppPath = "$InstallDir\$AppName.exe"
$DLLPath = $AppPath = "$InstallDir\$AppName.Hook.dll"

# 杀掉进程
Get-Process -Name $AppName -ErrorAction SilentlyContinue | Stop-Process

# 删掉任务
if (Get-ScheduledTask -TaskName $AppName -ErrorAction SilentlyContinue) {
    Unregister-ScheduledTask -TaskName $AppName -Confirm:$false
}

# 尝试删除 Dll，直到成功
while($true) {
    try {
        Remove-Item -Path $DLLPath -ErrorAction Stop
        break
    } catch {
        Write-Warning "Uninstalling ..."
        Start-Sleep -Seconds 2
    }
}
