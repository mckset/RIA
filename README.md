# RIA
RIA (Reference Image Application) is a cross platform image organizer for both Windows and Linux designed to help artists organize their reference material through the use of tags and folders. RIA supports PNG, JPEG, and WebP images.

# Installation
Download the latest verion from release page for your OS or compile it from source and run RIA.
> **_Windows:_** You do not need to run the install script if RIA runs without issues.
> 
> **_Linux:_** The install script move RIA into /usr/local/bin and adds a desktop entry and icon. RIA can run without the script being used. 

### Tested Distros:
- Arch
- Ubuntu
- Fedora
- Pop OS
- KDE

> **_NOTE:_** The Linux version relies on zenity to add new folders to the folder view and xclip/wl-copy to handle copying and pasting.

# Shortcuts
## Image Board

**TAB** - Open the folder view

**~** - Open the tag view

**Escape** - Clear selection or open the image board menu if nothing is selected

**Ctrl + S** - Save the boards current state

**Left mouse click** - Select an image

**Shift + left mouse click** - Select multiple images

**Left mouse drag** - Select images in a window

**Right mouse click** - Open image options menu

**Right mouse drag** - Scale images

**Shift + Right mouse drag** - Scale images while maintaining their ratio

**Scroll wheel/+/-** - Zoom in/out

**DELETE** - Remove all selected images from the board

**H** - Flip selected images horizontally

**R** - Rotate the selected images (Cancelled by RMB or ESC and confirmed by LM)

**WASD/Arrow keys/Middle mouse drag** - Move the image board view

**SPACE** - Toggle the origin display

**V** - Flip selected images vertically

> **_FOR MORE INFO:_** Open help.html or press the help button in RIA


## Folder View (**TAB**)
Contains saved paths to folders that contain the images that you want to tag/use.

**DELETE** (While hovered over a main folder) - Remove the folder from the view.

**LMB Click** (Folder) - Expand a folder and display its contents. If a file is tagged, a white box will be displayed on the left side of the file.

**LMB Click** (File) - Load the image into the preview on the bottom right side of the screen. Files that already have been tagged are indicated with a white bar to the left of the file name.

**Ctrl + R** - Refresh locations

**The "+" Button**  - Add a new location to the view. 


## Tag View (**~**)
Only contains images that have been tagged under said tags.


**LMB Click** - Expand a tags and display its contents. While previewing an image, a white box will appear on the left side of any of the tags it belongs to.

**LMB Click** (File) - Load the image into the preview on the bottom right side of the screen.

**RMB Click** - Add/remove an image from a tag/sub tag.

**RMB Click** (No preview image) - Edit a tag/sub tag.

**The "+" Button** - Add a new tag to the view.

**The "+" Button** (Tag header) - Add a new sub tag to the tag.

**The "edit" Button** (Tag header) - Edit an existing tag.

# Image Packs (**Image Pack Button**)
Image packs are an experimental idea that saves pre-tagged files so they can easily be shared between users. Follow the below instructions for how to create an image pack. Image packs use folders a categories instead of tags and relies on the images to be manually tagged by one users before being shared. While importing an image pack, it gives you the option to exclude folders in the image pack from being imported. EX) An image pack has 3 categories: Buildings, Landscapes, and Clothes. A user can choose to only import images from the Buildings and Landscapes folder and all the images in the Clothes folder will remain without being added to RIA. 
> **_NOTE:_**  When creating an image pack, it only checks the tags that are in your program. If you hit the create button on a previously imported image pack and removed some of the tags prior, it will overwrite the import file and remove the untagged images from the import file.

Importing Image Packs
---------
1) Select and image pack and press the import button. A dot will appear next to the selected image pack.
2) Select/deselect the folders from the image pack you want to import. A dot is again used to indicate if the folder is going to be imported. 
3) Verify you want to import the tags. Pressing the sub tag button will enable/disable importing sub tags.
> **_NOTE:_**  Imported tags do not overwrite existing tags with the same name

Creating Image Packs
--------
1) Create a new folder in the shared directory with the name of your image pack.
2) Create subfolders to categorizes the images you will be using. Multiple levels of subfolders can be used.
3) Tag the images in RIA.
4) Open the import menu, select your image pack and press the create button. 
> **_NOTE:_**  Importing can read multiple levels of sub directories but the example pack keeps things simple.

Image Pack Layout Example (See the example image pack in the latest release)
------------------
```
[Folder] Photographs (Image pack name)

	-> [Folder] Buildings (Category)
 
		-> [Folder] Cities (Sub-Category)

            -> [Image] New York.png

            -> [Image] Chicago.png

        -> [Image] Farm house.png
  
			ect
   
	-> [Folder] Trees (Category)
 
		-> [Image] Oak.png
  
			ect
```


# Compiling

### RIA depends on:

**Glad**: <https://github.com/Dav1dde/glad>

**STB_Image**: <https://github.com/nothings/stb>

**glfw or glfw-dev** (Sometimes called **libglfw-dev** or **libglfw3-dev**)

**g++**

**libwebp** or **libwebp-dev**

**freetype font** or **libfreetype-dev**


### Package Dependencies:
APT:
```
sudo apt-get install g++ libglfw3-dev libfreetype-dev libwebp-dev
```

DNF:
```
sudo dnf install g++ glfw-devel freetype-devel libwebp-devel
```

Pacman:
```
sudo pacman -S gcc glfw freetype libwebp
```

### Compile Command:
Linux:
```
g++ main.cpp src/dependencies/linux/glad/glad.c -o RIA -I/usr/include/freetype2 -lstdc++fs -lglfw -lGL -lXrandr -lX11 -lpthread -ldl -static-libgcc -lwebp -lfreetype
```
> **_NOTE:_**  If you are having problems with the filesystem libraries, try setting UBUNTU to 1 in the include.hpp file before compiling

Windows:
```
g++ main.cpp src\dependencies\windows\glad\glad.c -o RIA  "src\dependencies\windows\glfw\libglfw3.a" -Isrc/dependencies/libwebp/include -Isrc/dependencies/freetype -lopengl32 -lgdi32 -lole32 -loleaut32 -luuid -Lwebp freetype.dll libwebp.dll -static -static-libgcc -static-libstdc++
```

> **_NOTE:_**  The command may vary between distros/OS. 

# Troubleshooting
### Error while loading shared libraries: libglfw3.so
Install libglfw3-dev, glfw, or glfw-devel depending on your operating system

### DLLs missing on 32-bit Windows systems
Copy the DLLs from the SysWOW64 folder into System32 and replace any files that already exist

### Not working on Mac
RIA was not tested on or compiled for Mac


