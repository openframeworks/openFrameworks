# download_libs.ps1 - the PowerShell / cmd entry point for download_libs.sh.
#
#   powershell -File scripts\vs\download_libs.ps1 [-a 64|arm64|arm64ec|all] [other download_libs.sh options]
#
# It does not download anything itself: it runs scripts/vs/download_libs.sh with
# the same arguments (so the host-architecture default, -a pass-through, the BITS
# variable, SHA-256 verification and every option behave exactly the same) and
# returns its exit code. It needs bash from Git for Windows (Git Bash) or MSYS2.
$ErrorActionPreference = 'Stop'
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition

$onWindows = ($PSVersionTable.PSEdition -eq 'Desktop') -or ($null -ne (Get-Variable IsWindows -ValueOnly -ErrorAction SilentlyContinue) -and $IsWindows)

function Find-Bash {
	$candidates = @()
	if ($onWindows) {
		# Git for Windows first: its bash has the tools the .sh scripts expect
		$git = Get-Command git -ErrorAction SilentlyContinue
		if ($git) {
			$gitRoot = Split-Path -Parent (Split-Path -Parent $git.Source)
			$candidates += (Join-Path $gitRoot 'bin\bash.exe')
			$candidates += (Join-Path $gitRoot 'usr\bin\bash.exe')
		}
		foreach ($root in @($env:ProgramFiles, ${env:ProgramFiles(x86)}, (Join-Path $env:LOCALAPPDATA 'Programs'))) {
			if ($root) { $candidates += (Join-Path $root 'Git\bin\bash.exe') }
		}
		$candidates += 'C:\msys64\usr\bin\bash.exe'
		foreach ($c in $candidates) {
			if ($c -and (Test-Path -LiteralPath $c)) { return $c }
		}
	}
	# a bash on PATH - but not the WSL launcher in System32: it runs Linux, not the Windows tools
	$found = Get-Command bash -All -ErrorAction SilentlyContinue |
		Where-Object { $_.Source -notmatch '\\Windows\\System32\\' } |
		Select-Object -First 1
	if ($found) { return $found.Source }
	return $null
}

$bash = Find-Bash
if (-not $bash) {
	Write-Error @"
download_libs.ps1 needs bash, which it runs download_libs.sh with.
Install Git for Windows (https://git-scm.com/download/win, includes Git Bash) or MSYS2, then run this again.
"@
	exit 1
}

Set-Location -LiteralPath $scriptDir
& $bash './download_libs.sh' @args
exit $LASTEXITCODE
