# RIA
RIA (Reference Image Application) is a cross platform image organizer for both Windows and Linux designed to help artist organize their reference material through the use of tags and folders. 

# Installation
Download the latest verion or compile it from source and run the RIA. Note that RIA expects the font and icon images to be in an image folder in the directory you run it from.

# Image Board
**ESC** - Unselect all images 

**DELETE** - Remove all selected images from the board

**H** - Flip selected images horizontally

**R** - Rotate the selected images (Cancelled by RM or ESC and confirmed by LM)

**S** - Save the current state of the program. Any edits made without saving the program will be lost on closing.

**SPACE** - Toggle origin

**TAB** - Open the folder view

**V** - Flip selected images vertically

**~** - Open the tag view

**LM click** - Select an image

**L-Shift + LM click** - Select multiple images

**LM drag** - Select images in a window

**RM drag** - Scale images

**L-Shift + RM drag** - Scale images while maintaining their ratio

**MM Drag** - Move the view

**Scroll wheel** - Zoom in/out


# Folder View (**TAB**)

**DELETE** (While hovered over a location) - Remove the folder from the view.

**LM Click** (Folder) - Expand a folder and display its contents. If a file is tagged, a white box will be displayed on the left side of the file.

**LM Click** (File) - Load the image into the preview on the bottom right side of the screen.

**The "+" Button**  - Add a new location to the view. 


# Tag View (**~**)

**LM Click** - Expand a tags and display its contents.

**LM Click** (File) - Load the image into the preview on the bottom right side of the screen. Images 

**RM Click** - Add/remove an image from a tag/sub tag.

**RM Click** (No preview image) - Edit a tag/sub tag.

**The "+" Button** - Add a new tag to the view.

**The "+" Button** (Tag header) - Add a new sub tag to the tag.


Right clicking a tag without an image in the preview will allow you to edit the tag. Right clicking a tag while previewing an image will add or remove the image from the tag/sub tag. If the preview image is already tagged, a white box will appear on the left side of the tag. Clicking a file in the tag view will load a preview.

# Import (**Import Button**)
Image packs are saved to the shared directory and must follow a similar format to the example below.

Importing
---------
1) Click on an image pack to import and press the import button.
2) Select the folders from the image pack you want to import.
3) Verify you want to import the tags. Pressing the sub tag button will enable/disable importing sub tags.
> **_NOTE:_**  Imported tags do not overwrite color values of existing tags with the same name

Creating
--------
1) Create a new folder in the shared directory with the name of your image pack.
2) Create subfolders to categorizes the images you will be using.
3) Tag the image in RIA.
4) Open the import menu, select your image pack and press the create button. 
> **_NOTE:_**  Importing only uses the first level of folders in an image pack. further subdirectories are ignored

Image Pack Layout Example
------------------
```
[Folder] Photographs

	-> [Folder] Buildings
 
		-> [Image] New York.png
  
			ect
   
	-> [Folder] Trees
 
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

> **_NOTE:_**  Command may vary between distros/OS. 
