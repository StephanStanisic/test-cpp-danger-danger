$t = '[DllImport("user32.dll")] public static extern bool ShowWindow(int handle, int state);'
add-type -name win -member $t -namespace native
[native.win]::ShowWindow(([System.Diagnostics.Process]::GetCurrentProcess() | Get-Process).MainWindowHandle, 0)

Write-Host "Started in directory $pwd"

$watcher = New-Object IO.FileSystemWatcher (get-item $pwd).parent.FullName, "*.md" -Property @{ 
    IncludeSubdirectories = $true
    EnableRaisingEvents = $true
}

$changeAction = [scriptblock]::Create('
    $path = $Event.SourceEventArgs.FullPath
    $newFile = [System.IO.Path]::ChangeExtension($path,".pdf")
    $errorFile = [System.IO.Path]::ChangeExtension($path,".error.txt")
    Write-Host "New change event at $path"

    if(Test-Path $errorFile) {
        Remove-Item $errorFile
    }

    function Show-Notification {
        [cmdletbinding()]
        Param (
            [string]
            $ToastTitle,
            [string]
            [parameter(ValueFromPipeline)]
            $ToastText
        )

        [Windows.UI.Notifications.ToastNotificationManager, Windows.UI.Notifications, ContentType = WindowsRuntime] > $null
        $Template = [Windows.UI.Notifications.ToastNotificationManager]::GetTemplateContent([Windows.UI.Notifications.ToastTemplateType]::ToastText02)

        $RawXml = [xml] $Template.GetXml()
        ($RawXml.toast.visual.binding.text|where {$_.id -eq "1"}).AppendChild($RawXml.CreateTextNode($ToastTitle)) > $null
        ($RawXml.toast.visual.binding.text|where {$_.id -eq "2"}).AppendChild($RawXml.CreateTextNode($ToastText)) > $null

        $SerializedXml = New-Object Windows.Data.Xml.Dom.XmlDocument
        $SerializedXml.LoadXml($RawXml.OuterXml)

        $Toast = [Windows.UI.Notifications.ToastNotification]::new($SerializedXml)
        $Toast.Tag = "PowerShell"
        $Toast.Group = "PowerShell"
        $Toast.ExpirationTime = [DateTimeOffset]::Now.AddMinutes(1)

        $Notifier = [Windows.UI.Notifications.ToastNotificationManager]::CreateToastNotifier("Yo man")
        $Notifier.Show($Toast);
    }


    Get-Content "$pwd\settings.txt" | foreach-object -begin {$h=@{}} -process { $k = [regex]::split($_,"="); if(($k[0].CompareTo("") -ne 0) -and ($k[0].StartsWith("[") -ne $True)) { $h.Add($k[0], $k[1]) } }

    if($h.Get_Item("ShowNotifications") -eq $true) {
        Show-Notification "Converting to pdf" "$path"
    }

    $output = [string](&pandoc.exe "--citeproc" "--bibliography" "../bibliography.bib" "--csl" "university-of-york-apa.csl" "-o" "$newFile" "$path" 2>&1)

    if($LastExitCode -ne 0) {
        Show-Notification "Something went wrong while converting!" "$output"
        $output | Out-File -FilePath "$errorFile"
    } else {
        if($h.Get_Item("ShowNotifications") -eq $true) {
            Show-Notification "Conversion complete" "$path"
        }
    }
')

Register-ObjectEvent $Watcher -EventName "Changed" -Action $changeAction

while($true) {
    Start-Sleep -Seconds 1
}
