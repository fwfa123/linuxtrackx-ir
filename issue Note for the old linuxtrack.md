Goosoe left a comment (uglyDwarf/linuxtrack#206)

This thread helped me a lot to manage to make linuxtrack work with TrackIR 5 on arch 6.15.4 with Steam proton games. As a thank you I decided to write the biggest stumbles I came across and what I did to fix them in order to make this work.

-   First off, I grabbed the latest on master. Building it required me to include the pthread library (`#include <phterad.h>`) in a specific .cpp file, plus remove some deprecated values in a couple of switch case regarding the Wine library (DLL_WINE_PREATTACH) on other files.
    
-   The current wine package in arch merged the 32 and 64 bit builds and moved to the WoW64 build (https://archlinux.org/news/transition-to-the-new-wow64-wine-and-wine-staging/). This means that we do not have the wine 32 bit binaries. This is a big problem since with the lack of them, we won't be able to build and compile the so desired linuxtrack-wine.exe. To fix this, at the moment, I switched to the wine-stable (https://aur.archlinux.org/packages/wine-stable) package which still has the 32-bit version of winegcc and wineg++ to build the linuxtrack-wine.exe.
    
-   Afterwards, using the steps provided by @fwfa123 (#206 (comment)) I had everything compiled properly.
    
-   Now we can run $HOME/linuxtrack/bin/ltr_gui and see if things are working okay. I had to disable 3D Tracking Preview in the Misc. tab to be able to get the camera view without lag. Everything was set up nicely to work natively, but now the other problem: I wanted to play some games through Steam Proton, such as IL-2. For that, we had to install the bridge in the desired game prefix.
    
-   To properly install everything in the game prefix, I used Protontricks (https://aur.archlinux.org/packages/protontricks).
    I had to install some things in there. Initially I ran the command in the console `protontricks <GameId on steam> -q mfc42` to install various .dll's but we were aiming at this one specifically: mfc42u.dll.
    Then I opened Protontricks directly on the game prefix and installed the latest TrackIR5 software, which I downloaded from TrackIR official website.
    Finally I manually installed the previously built linuxtrack-wine.exe.
    The problems did not end here. Firstly, the IR camera would not turn on when the game was launched. The solution was found mentioned in a comment above: Run the prefix and open the Tester64.exe, start it up and then close it. After this, whenever launching the game, the camera will now turn on! But it wasn't working in-game yet, it would just turn on.
    

To fix that, there were a couple of other issues to solve:

-   One of them, was that the TrackIR dummy program was telling, at game startup, that it could not find the mfc42u.dll and TIRViews.dll. That was when I noticed that the symbolic links for those DLL in $HOME/.steam/steam/steamapps/compatdata/<game_id>/pfx/drive_c/Program Files (x86)/Linuxtrack were not properly linked. The links searched for the .dll in .config/linuxtrack/tir_firmware. Since I did some of the manual installing (because the installers in the tool were not working), it probably did not copy the desired .dll files in there. So I manually copied them to .config/linuxtrack/tir_firmware.
    They were found in:
    `$HOME/.steam/steam/steamapps/compatdata/<game_id>/pfx/drive_c/Program Files (x86)/TrackIR5/TIRViews.dll`
    and
    `$HOME/.steam/steam/steamapps/compatdata/<game_id>/pfx/drive_c/windows/syswow64/mfc42u.dll`
    
-   Phew! This might be it, right? No! There was another issue now.
    When I did the ./ltr_extractor --update, it set the IL-2 value in $HOME/.config/linuxtrack/tir_firmware/gamedata.txt to ((null)). This should not be null at all. As much as tried calling the ./ltr_extractor --update again, it would never set the desired value to be different than ((null)). The solution was mentioned in another comment in this issue, this time by @immakyle.
    Since IL-2 is an old game, you can get the old universal package with the bins already built and call the ./ltr_extractor --update present in there. Afther this, checking the gamedata.txt showed a different value!
    With all of this done, I booted up IL-2 again and voila! I had a working headtracker with TrackIR on a Steam/Proton game.
    

Once again, a big thanks to everyone who contributed to this issue especially to @fwfa123 and @immakyle for providing such great solutions.

https://gitlab.winehq.org/wine/wine/-/wikis/Debian-Ubuntu
