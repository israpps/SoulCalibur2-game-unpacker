# SoulCalibur2-game-unpacker
program to decrypt and unpack the `PS2AC05` binary blob from System246 SoulCalibur2

PS2AC05 is the main game binary blob. the game loader (`SCSLOAD` file inside dongle) will look for `PS2AC05` inside the security dongle (`mc0`), developer flash (`rom1:`) or inside the disc (`cdrom0:\`) (in this order)

the file gets directly loaded into ram `0x1000000`, from there `SCSLOAD` will decrypt and unpack the file in-place, then the `0x1000000` is casted as a function pointer and called, starting the game

thanks to [@DiscoStarslayer](https://github.com/DiscoStarslayer) for helping me to get the unpacking and decryption working.

## Usage
```cmd
SC2_unpacker.exe <INPUT> <OUTPUT1> <OUTPUT2>
```

- **INPUT**: your `PS2AC05` file
- **OUTPUT1**: the path to write the decrypted binary (normally you don't care of this file)
- **OUTPUT2**: the path to write the decrypted and decompressed binary
