cd ../../esp-idf;
cmd.exe /c `
'"C:\Users\thorn\.espressif\idf_cmd_init.bat" "C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python37_64\" "C:\Program Files\Git\cmd\" '`
'& cd ..\esp\door_reminder & idf.py build '`
'&& C:\Program Files\PuTTY\putty.exe" -load "ESP32"';
cd ..\esp\door_reminder