# Assemble @GSC_Vertical_Slot_Fix for publishing: mod_build writes addons/ and signs it, this puts
# the rest in place. Run it after mod_build. Running it twice is harmless.
#
#   .\package.ps1            assemble the folder
#   .\package.ps1 -Check     say what is missing or stale, change nothing

[CmdletBinding()]
param([switch]$Check)

$ErrorActionPreference = 'Stop'
$mod  = 'GSC_Vertical_Slot_Fix'
$root = $PSScriptRoot
$src  = Join-Path $root "packaging\$mod"
$dst  = Join-Path $root "@$mod"

if (-not (Get-ChildItem (Join-Path $dst 'addons') -Filter *.pbo -ErrorAction SilentlyContinue)) {
    Write-Host "not built yet (no pbo in @$mod\addons): run mod_build first" -ForegroundColor Yellow
    exit 1
}

$problems = 0
foreach ($file in Get-ChildItem $src -File) {
    $target = Join-Path $dst $file.Name
    $same = (Test-Path $target) -and ((Get-FileHash $target).Hash -eq (Get-FileHash $file.FullName).Hash)
    if ($same) { continue }

    if ($Check) {
        Write-Host "  $($file.Name) differs from packaging\" -ForegroundColor Yellow
        $problems++
    } else {
        Copy-Item $file.FullName $target -Force
        Write-Host "  $($file.Name)" -ForegroundColor Green
    }
}

# A server with verifySignatures = 2 installs the public key from here. mod_build copies it while
# signing, so this only reports a folder that was assembled without a signing build.
if (-not (Test-Path (Join-Path $dst "keys\$mod.bikey"))) {
    Write-Host "  keys\$mod.bikey is missing: the pbo was not signed" -ForegroundColor Yellow
    $problems++
}

if ($problems -gt 0) { Write-Host "$problems thing(s) to fix" -ForegroundColor Yellow; exit 1 }
Write-Host "@$mod matches packaging\" -ForegroundColor Green
exit 0
