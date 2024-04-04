# RIA (Early release)
RIA (Reference Image Application) is a cross platform image organizer for both Windows and Linux designed to help artists organize their reference material through the use of tags and folders. 

# Installation
Download the latest verion from release page for your operating system or compile it from source and run RIA. Note that RIA expects the font and icon images to be in the image folder in the directory you run it from.
> **_NOTE:_** Currently, the Windows command prompt will open as a display debug information

### Tested Distros:
- Arch
- Ubuntu

> **_NOTE:_** The Linux version relies on Zenity to add new folders to the folder view.

# Image Board

**ESC** - Save the boards current state

**Left mouse click** - Select an image

**Left shift + left mouse click** - Select multiple images

**Left mouse drag** - Select images in a window

**Right mouse click** - Open image options menu

**Right mouse drag** - Scale images

**Left-Shift + Right mouse drag** - Scale images while maintaining their ratio

**Scroll wheel/+/-** - Zoom in/out

**DELETE** - Remove all selected images from the board

**H** - Flip selected images horizontally

**R** - Rotate the selected images (Cancelled by RMB or ESC and confirmed by LM)

**WASD/Arrow keys/Middle mouse drag** - Move the image board view

**SPACE** - Toggle the origin display

**TAB** - Open the folder view

**V** - Flip selected images vertically

**~** - Open the tag view

> **_FOR MORE INFO:_** Open help.html or press the help button in RIA


# Folder View (**TAB**)
Contains saved paths to folders that contain the images that you want to tag/use.

**DELETE** (While hovered overthe main folder) - Remove the folder from the view.

**LMB Click** (Folder) - Expand a folder and display its contents. If a file is tagged, a white box will be displayed on the left side of the file.

**LMB Click** (File) - Load the image into the preview on the bottom right side of the screen. Files that already have been tagged are indicated with a white bar to the left of the file name.

**The "+" Button**  - Add a new location to the view. 


# Tag View (**~**)
Only contains images that have been tagged under said tags.


**LMB Click** - Expand a tags and display its contents. While previewing an image, a white box will appear on the left side of any of the tags it belongs to.

**LMB Click** (File) - Load the image into the preview on the bottom right side of the screen.

**RMB Click** - Add/remove an image from a tag/sub tag.

**RMB Click** (No preview image) - Edit a tag/sub tag.

**The "+" Button** - Add a new tag to the view.

**The "+" Button** (Tag header) - Add a new sub tag to the tag.

# Image Packs (**Image Pack Button**)
Image packs are an experimental idea only found in RIA. An image pack contains pre-tagged files so they can easily be shared between users. Follow the below instructions for how to create an image pack. Image packs use folders a categories instead of tags and relies on the images to be manually tagged by one users before being able to be shared. While importing an image pack, it gives you the option to exclude folders int the image pack from being imported. EX) An image pack has 3 categories: Buildings, Landscapes, and Clothes. A user can choose to only import images from the Buildings and Landscapes folder and all the images in the Clothes folder will remain without being added to RIA. 
> **_NOTE:_**  When creating an image pack, it only checks the tags that are in your program. If you hit the create button on a previously imported image pack and removed some of the tags prior, it will overwrite the import file and remove the untagged images from the import file.

Importing Image Packs
---------
1) Click on an image pack to import and press the import button. The image pack with a square indicates that the image pack is selected.
2) Select/deselect the folders from the image pack you want to import. Folders with a square next to them are selected. 
3) Verify you want to import the tags. Pressing the sub tag button will enable/disable importing sub tags.
> **_NOTE:_**  Imported tags do not overwrite color values of existing tags with the same name

Creating Image Packs
--------
1) Create a new folder in the shared directory with the name of your image pack.
2) Create subfolders to categorizes the images you will be using.
3) Tag the images in RIA.
4) Open the import menu, select your image pack and press the create button. 
> **_NOTE:_**  Importing only uses the first level of folders in an image pack. further subdirectories are ignored

Image Pack Layout Example (See the example image pack in the latest release)
------------------
```
[Folder] Photographs (Image pack name)

	-> [Folder] Buildings (Category)
 
		-> [Image] New York.png
  
			ect
   
	-> [Folder] Trees (Category)
 
		-> [Image] Oak.png
  
			ect
```


# Compiling

### RIA depends on:

**Glad**: https://github.com/Dav1dde/glad

**STB_Image**: https://github.com/nothings/stb

**glfw3-x11 or glfw-dev**

**xorg-dev**

**g++**



Linux:
Arch Linux/Manjaro:
```
g++ main.cpp linux/glad/glad.c -o RIA -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
```
Ubuntu:
```
g++ main.cpp linux/glad/glad.c -o RIA -lstdc++fs -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -static-libgcc -static-libstdc++
```
> **_NOTE:_**  Be sure to set UBUNTU to 1 in "config.hpp" before compiling.

Windows:
```
g++ main.cpp windows/glad/glad.c -o RIA windows/glfw/libglfw3.a -lopengl32 -lgdi32 -lole32 -loleaut32 -luuid -static -static-libgcc -static-libstdc++
```

> **_NOTE:_**  The command may vary between distros/OS. 

