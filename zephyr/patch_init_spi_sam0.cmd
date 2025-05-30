@echo off

@REM filename for spi init sam0, to remove unwanted static keyword before init spi function
set "filePath=C:\Users\JohanneVIGNY\zephyrproject1\zephyr\drivers\spi\spi_sam0.c"

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$path = '%filePath%';" ^
  "$lines = Get-Content $path;" ^
  "$lines | ForEach-Object { " ^
    "if ($_ -match '^\s*static\s+int\s+spi_sam0_init\(const struct device \*dev\)') { " ^
      "$_ -replace '^\s*static\s+', '' " ^
    "} else { $_ } } | Set-Content $path"

echo Done.