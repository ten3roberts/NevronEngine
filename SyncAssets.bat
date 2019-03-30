rem syncs the debug and release with the same folder .\Assets
rem Visual studio uses the assets folder in debug

xcopy .\Assets .\bin\x64Debug\Assets /e/i/y
rem xcopy .\Assets .\bin\Win32Debug\Assets /e/i/y

xcopy .\Assets .\bin\x64Release\Assets /e/i/y
rem xcopy .\Assets .\bin\Win32Release\Assets /e/i/y
