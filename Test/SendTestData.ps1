Param(
	$fileName,
	$comPort = "COM1",
	$baudRate = "4800",
	$parity = "none",
	$dataBits = "8",
	$stopBits = "one",
	$linePause = 1000,
	[switch]$forever
)

$port = new-Object System.IO.Ports.SerialPort $comPort,$baudRate,$parity,$dataBits,$stopBits
$port.Open()
$lines = Get-Content $fileName
do {
	foreach ($line in $lines) {
	Write-Host -NoNewline "$line`r`n"
	$port.Write("$line`r`n")
	Start-Sleep -m $linePause
	}
}
while($forever)
$port.Close()
