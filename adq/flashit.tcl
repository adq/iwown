halt
flash write_image adq/_build/watchapp.bin 0x16000
flash write_image adq/_build/watchapp.bin.bootdat 0x3fc00
reset
