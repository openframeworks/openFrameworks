# Wrap MSVC cl.exe with sccache for msbuild / Visual Studio CMake generators.
# hendrikmuhs/ccache-action (variant: sccache) must run first so sccache is on PATH.
$ErrorActionPreference = "Stop"

$sccache = (Get-Command sccache).Source
$wrapDir = Join-Path $env:RUNNER_TEMP "sccache-cl"
New-Item -ItemType Directory -Force -Path $wrapDir | Out-Null
$cmdPath = Join-Path $wrapDir "sccache-cl.cmd"
@"
@echo off
"$sccache" cl.exe %*
"@ | Set-Content -Encoding ascii -Path $cmdPath

$props = "/p:CLToolExe=sccache-cl.cmd /p:CLToolPath=$wrapDir /p:TrackFileAccess=false /p:UseMultiToolTask=true /p:DebugInformationFormat=OldStyle"
if ($env:GITHUB_ENV) {
    Add-Content -Path $env:GITHUB_ENV -Value "MSBUILD_SCCACHE_PROPS=$props"
    Add-Content -Path $env:GITHUB_ENV -Value "CMAKE_VS_MSBUILD_ARGS=$props"
}
Write-Host "sccache: $sccache"
Write-Host "wrapper: $cmdPath"
Write-Host "msbuild props: $props"
& sccache --version
