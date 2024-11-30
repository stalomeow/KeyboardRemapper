param (
    [string]$InstallDir
)

$AppName = "KeyboardRemapper"
$AppPath = "$InstallDir\$AppName.exe"

# 杀掉旧进程
Get-Process -Name $AppName -ErrorAction SilentlyContinue | Stop-Process

# 删掉旧任务
if (Get-ScheduledTask -TaskName $AppName -ErrorAction SilentlyContinue) {
    Unregister-ScheduledTask -TaskName $AppName -Confirm:$false
}

# 配置任务触发器：在用户登录时启动
$Trigger = New-ScheduledTaskTrigger -AtLogOn

# 配置任务操作：运行 exe
$Action = New-ScheduledTaskAction -Execute $AppPath

# 创建任务计划
Register-ScheduledTask -TaskName $AppName -Trigger $Trigger -User $env:USERNAME -Action $Action -RunLevel Highest -Force

# 以管理员身份启动
Start-Process $AppPath -Verb RunAs
