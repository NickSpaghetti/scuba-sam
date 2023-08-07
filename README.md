# scuba-sam
A scuba diving game for the playdate

# Running the game on your device
1. clone the code and run either ```make``` or ```cmake```
2. If you get no errors you should see a scuba_sam.pdx file.
3. Open the simulator and click Register Device if you have not already done so.
4. Drag and drop that on the simulator.
5. Plug in your device into your computer
6. Playdate => Run On Device.

# Local Development
## On MAC
### VSCode
1. open project at the root in VSCode
2. In the terminal Run ```make```
3. Take scuba_sam.pdx and drag and drop it on playdate simulator app.
### Clion
Read the guide on [sdk.play.date](https://sdk.play.date/inside-playdate-with-c/#_clioncmake) 
If you set this up correctly when you click Debug the Playdate simulator should automatically pop up.

## On Windows
### Visual Studio
1. In the terminal run the following command ```cmake ./CMakeList.txt```
2. Open up Visual Studios and open up the solution ```suba_sam.sln```
3. Set the statup project as scuba_sam.vcxproj and when you hit debug the plate date simulator will pop up.