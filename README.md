# Splitter

Splitter is a lightweight tool to split a PNG image (strip) into multiple smaller images, identifying distinct objects separated by transparent pixels. It also allows combining multiple objects when they belong together.

## Features
- Automatically splits objects in a PNG file.
- Combines multiple objects into one using customizable indices.
- Saves outputs to a specified or default folder.

---

## Usage

### Basic Usage
```bash
splitter -i image.png
```
This splits the objects in `image.png` into separate images and saves them in the default output folder (`out/`).

Another way to achieve the same is to drag and drop the image into the executable.

### Arguments

#### Input File
`-i <input file>`  
Specifies the input PNG file to process.  

#### Output Folder
`-o <output folder>`  
Specifies the folder where the split images will be saved. Default is `out/`.

Example:
```bash
splitter -i image.png -o my_output_folder/
```

#### Combine Objects
`-c <index1> <index2> ...`  
Combines objects that are separated by transparent pixels. Use this option to merge symbols or elements that consist of multiple parts.

For example:
- `-c 1` combines the object at index `1` with the next object.
- `-c 1 1 1` combines the object at index `1` with the next three objects sequentially.
- You can specify multiple indices in a single `-c` command.

Example:

Let's say you have the following image:\
![example](https://github.com/user-attachments/assets/355e54bb-7c5b-4a4d-a60b-c7ccfec25081)
```bash
splitter -i letters.png
```
Will produce:\
![image](https://github.com/user-attachments/assets/9fb2415d-6255-4330-8819-9436f210273a)

As you can see, the double quotation marks were detected as 2 separate symbols. To combine them into one, use the argument `-c 1` to merge the symbol at index 1 with the one after it.
```bash
splitter -i letters.png -c 1
```
![image](https://github.com/user-attachments/assets/62ae6e8a-e330-4f83-8712-7d0699b690c2)

---

## Notes
- Indices for `-c` are 0-based and refer to the order in which objects are detected.
- The `-c` option supports multiple index combinations in a single command.

---

## Limitations

- Currently, splitting is strictly based on transparency; other criteria (e.g., color-based separation) are not yet implemented.  
- Error handling for unsupported or corrupt PNG files is minimal.

These limitations may be addressed in future updates.  

--- 

## Requirements

- A C++ compiler supporting C++17 or later (e.g., GCC, Clang, or MSVC).
- The [stb_image](https://github.com/nothings/stb) library:
  - This project uses `stb_image` and `stb_image_write` for loading and saving PNG files.  
  - No external installation is needed
- A shell or terminal environment to run the program.
