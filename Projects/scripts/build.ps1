Function Invoke-CMakeBuild {
  Param(
    [Parameter()]
    [ValidateSet('Release','Debug')]
    [string] $buildType = 'Release',
    [ValidateSet('ninja','vs2019')]
    [string] $generator = 'vs2019',
    [Parameter()]
    [AllowNull()]
    [string] $platform,
    [Parameter()]
    [string[]] $options = @()
  )

  # Add all arguments
  if ($platform) {
    $genArgs += ('-DCMAKE_SYSTEM_NAME={0}' -f $platform);
  }

  $genArgs += ('-DCMAKE_BUILD_TYPE={0} ..' -f $buildType);

  $genArgs += $options;

  # Create the generate call
  $genCall = ('cmake {0}' -f ($genArgs -Join ' '));

  Write-Host $genCall;
  Invoke-Expression $genCall
 }

Set-Location -Path Projects\AdminPanel
if (Test-Path 'build')
{
	# Do nothing
}
else
{
	New-Item -Path . -Name "build" -ItemType "directory"
}
Set-Location -Path build
Invoke-CMakeBuild

Set-Location -Path ..\..\..

Set-Location -Path Projects\BowlingScore
if (Test-Path 'build')
{
	# Do nothing
}
else
{
	New-Item -Path . -Name "build" -ItemType "directory"
}
Set-Location -Path build
Invoke-CMakeBuild

Set-Location -Path ..\..\..

Set-Location -Path Projects\PointsGenerator
if (Test-Path 'build')
{
	# Do nothing
}
else
{
	New-Item -Path . -Name "build" -ItemType "directory"
}
Set-Location -Path build
Invoke-CMakeBuild
