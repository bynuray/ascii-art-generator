# ASCII Art Generator (C++)

A C++ program that turns a grayscale image into ASCII art: a picture drawn with text characters in the terminal.

## What it does

An image is a grid of pixels, and in a grayscale image every pixel is just a brightness number from 0 (black) to 255 (white).

The program reads these numbers and replaces each pixel with a character:

- dark pixels become dense characters like `@` and `%`
- light pixels become light characters like `.` or a space

The result is the same picture, drawn with text.

## Before you start: convert your image to PGM

The program works with **grayscale images in PGM format (type `P2`)**. Normal formats like JPG and PNG are compressed and can't be read directly, so the image needs to be converted first.

PGM is a very simple image format where the pixel values are stored as plain text numbers. `P2` is the label at the start of the file meaning "grayscale image, stored as text".

You can convert images with free tools like **GIMP** or **ImageMagick**. When exporting, choose grayscale and the **ASCII** (not binary/Raw) format.

To check the result, open the `.pgm` file in a text editor. The first line must be `P2`. If you see `P5` and unreadable symbols, the image was saved in binary format, which the program doesn't support yet.

**Tip:** keep images around 80–120 px wide. Each pixel becomes one character, so a wider image won't fit in the terminal.

## How to run

You need a C++17 compiler.

```bash
g++ -std=c++17 -o ascii main.cpp
./ascii
```

Put your `.pgm` file in the same folder and set its name in `main.cpp`. A sample image is included: `starry_night.pgm`, a grayscale version of *The Starry Night* by Vincent van Gogh.

## How it works

1. **Opens the file** with `std::ifstream` and stops with an error if the file can't be opened.
2. **Reads the header**: the image type, width, height and maximum brightness. If the type isn't `P2`, it stops with an error.
3. **Stores the pixels** in a 2D grid (`std::vector<std::vector<int>>`). A vector is used because the image size is only known when the program reads the file.
4. **Turns each brightness into a character** from a string of characters ordered from dark to light:
   ```
   index = brightness * ramp.size() / (maxValue + 1)
   ```
5. **Fixes the proportions**: terminal characters are about twice as tall as they are wide, so the program prints every second row to keep the image's real shape.

## Future improvements

This project will keep growing. Planned features:

- **Color images**: support PPM files (`P3`) by converting each pixel's red, green and blue values to one brightness
- **Binary formats**: support `P5` and `P6`, which most tools save by default
- **PNG and JPG support**, so images don't need to be converted first
- **Header comments**: skip lines starting with `#`, which tools like GIMP add automatically
- **Automatic resizing**: let the user choose the output width, so any image size works
- **Invert option** for light and dark terminal backgrounds
- **Save the result** to a `.txt` file
- **Command-line options**, for example `./ascii image.pgm 80`
