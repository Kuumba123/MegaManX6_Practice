# MegaMan X6 Practice
This is a hack to assist with practicing speedruns of the PSX version of MegaMan X6. It primarly adds save states which are not normally possible on PSX games due to the nature of the Disc Games.

## Compile
NOTE these steps might change in the future so keep that in mind when fetching the newest version of this repro.
1st your gonna need a total of 3 repros (including this one)
 * My Fork of Red-Hot's PSX Modding Repro https://github.com/Kuumba123/psx-modding-toolchain
 * My MMX6 PSX Repro https://github.com/Kuumba123/MegaManX6_PS1_Modding

 Also your gonna need the psyq library files (`psyq-4.7-converted-full.7z`) from https://psx.arthus.net/sdk/Psy-Q/

 Once you have all 3 go back to the github with Red-Hot's PSX modding repro and following the instructions there. If you can compile any of the examples included in that repro you should be able to compile this one.

 Then put the MegaMan X5 PSX repro inside the `games` folder and then this repro inside your `mods` folder and put your MegaMan X6 ISO files inside the `build` folder (must be named `MMX6.cue` and `MMX6.bin` or `RMX6.cue` and `RMX6.cue` for the japanese version).

 Once your done setting up all the files go back to the  MMX6 Practice repro and run the MOD.BAT file. The only options you should really be interested in are the option `3` since thats the option extracts the game files. Extract the game files for your respective version then quit via option `20`. Go Back to the build folder are you will see a new folder containing the extract game files. 
 Copy the `ROCK_X6.DAT` for your respective version into the `arc` folder of your respective version in the MMX6 Practice repro. Do the same thing but with the `ROCK_X6.BIN` but put in the `ovl` folder in the MMX6 Practice repro. Once you copied both files you need to 1st take care of the data inside `ROCK_X6.DAT`. Run the `EXTRACT.BAT` file to extract the data then run the `TEXTURE.BAT` file.
 Make sure you only run the `TEXTURE.BAT` file once otherwise some of the games textures will be glitched and you will need to run `EXTRACT.BAT` again. Lastly run `MERGE.BAT` and that should create a file called `DATA.DAT` , you should now only need to do this each time you edit these ARC files. Now its time for the `ROCK_X6.BIN` , go back to the `ovl` folder and run the batch file for your respective version.
 The Visual Studio Code `Build` task auto extracts the specfic overlays that gonna be edited very often so you should need to run that batch file once. As for the last step it is best to just use the Visaul Studio Tasks since they make the Build process much simpler
 Open the MMX6 Practice repro in Visaul Studio Code then press CRTL + Shift + B to run a task. 1st , run the `Compile` task for your respective version then run the `Build` task for your respective version. The Build task might take awhile depending on the speed of your drives but once it is done you should see the new disc files in the build folder we went to earlier.
 If you want to compile both the North American and Japanese version of the game make sure you run the `Clean` task so that GCC re-compiles the C code.