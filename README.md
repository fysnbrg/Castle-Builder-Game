# Castle-Builder-Game
A 2d game that lets you build colonies on an isometric grid - built on cocos-2dx

To run the game you’ll need to install cocos-2dx.
•	Download cocos2d-x 4.0 from https://www.cocos.com/en/cocos2dx-download
•	Extract and install cocos2dx-4.0 by running setup.py
•	Create the project folder at a given directory using command : 
o	cocos new MyGameName -l cpp -p 
•	Go into the proj.win32 folder inside the project folder and run the command:
o	cmake .. -G “Visual Studio 17 2022” -AWin32 
Requirements : CMake, Python 2.x version
Detailed step-by-step guide : https://www.youtube.com/watch?v=EPncmb5ujJo

After project setup is done:
•	Go into your project folder and delete the Classes folder
•	Copy all the files in client source folder into proj.win32 folder
•	Replace the Resources folder with my Resources folder in img
•	Run the MyGameName.sln file and load the files into your project
•	Build and run the solution

