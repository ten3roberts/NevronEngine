rem makes a backup of NevronEngine in the D: drive
color a
SET source="c:\Users\Tim Roberts\Documents\1_NevronEngine"
rem Names the backup date(YYYY-MM-DD) and time(HH-MM)
SET target="d:\NevronEngineVersions\%date%"


robocopy %source% %target% /e /copy:dat /xo /purge /r:1 /w:5

echo Backup succeded