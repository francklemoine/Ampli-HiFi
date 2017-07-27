Import('env')
env.Replace( UPLOADHEXCMD='"$UPLOADER" -C ~/.platformio/packages/tool-avrdude/avrdude.conf -c usbasp -p m328p -Ulfuse:w:0xE2:m -Uhfuse:w:0xDA:m -Uefuse:w:0xFF:m -U flash:w:$SOURCES:i')
