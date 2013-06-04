.\Debug\bradpitch.out
-a
-image
-zero
-memwidth 8
-map .\Debug\bradpitch_hex.map
-boot
-bootorg 0x64000400
-bootsection .boot_load 0x64000000
-o .\Debug\bradpitch.hex

ROMS
{
    FLASH: org = 0x64000000, len = 0x0080000, romwidth = 8, files = {.\Debug\bradpitch.hex}
}
