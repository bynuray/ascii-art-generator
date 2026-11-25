# ASCII Art Generator (C++)

A C++ program that turns a grayscale image into ASCII art: a picture drawn with text characters in the terminal.

## What it does

An image is a grid of pixels, and in a grayscale image every pixel is just a brightness number from 0 (black) to 255 (white).

The program reads these numbers, shrinks the image to fit the terminal, and replaces each part of the image with a character:

- dark areas become dense characters like `@` and `%`
- light areas become light characters like `.` or a space

The result is the same picture, drawn with text, at a size that fits your screen.

## Before you start: convert your image to PGM

The program works with **grayscale images in PGM format (type `P2`)**. Normal formats like JPG and PNG are compressed and can't be read directly, so the image needs to be converted first.

PGM is a very simple image format where the pixel values are stored as plain text numbers. `P2` is the label at the start of the file meaning "grayscale image, stored as text".

You can convert images with free tools like **GIMP** or **ImageMagick**. When exporting, choose grayscale and the **ASCII** (not binary/Raw) format. With ImageMagick:

```bash
magick input.jpg -colorspace Gray -compress none output.pgm
```

To check the result, open the `.pgm` file in a text editor. The first line must be `P2`. If you see `P5` and unreadable symbols, the image was saved in binary format, which the program doesn't support yet.

**Tip:** any image size works, because the program resizes it automatically. P2 files are large, though (a 400 × 317 image is about 420 KB), so a few hundred pixels wide is a practical size.

## How to run

You need a C++17 compiler.

```bash
g++ -std=c++17 -o ascii main.cpp
./ascii <file.pgm> [width]
```

- `<file.pgm>` (required): the image to convert
- `[width]` (optional): the output width in characters; the default is 80

Examples:

```bash
./ascii starry_night_400.pgm        # 80 characters wide
./ascii starry_night_400.pgm 120    # wider, more detail
./ascii starry_night_400.pgm 40     # smaller
```

A sample image is included: `starry_night_400.pgm`, a 400 × 317 grayscale version of *The Starry Night* by Vincent van Gogh.

**OnlineGDB:** upload the `.pgm` file, then type the arguments (without `./ascii`) into the **Command line arguments** box, for example `starry_night_400.pgm 120`.

## How it works

1. **Reads the command-line arguments**: the file name and the optional width, and checks they are valid.
2. **Opens the file** with `std::ifstream` and stops with an error if the file can't be opened.
3. **Reads the header**: the image type, width, height and maximum brightness. If the type isn't `P2`, it stops with an error.
4. **Stores the pixels** in a 2D grid (`std::vector<std::vector<int>>`). A vector is used because the image size is only known when the program reads the file.
5. **Calculates the output size**: the output width is 80 characters by default, or the width the user chose, and the height is calculated from the image so the picture keeps its shape (see *Update: automatic resizing* below).
6. **Averages each block**: every output character covers a small block of pixels. The program adds up all the pixels in the block and divides by how many there are.
6. **Turns each average into a character** from a string of characters ordered from dark to light:
   ```
   index = average * ramp.size() / (maxValue + 1)
   ```

## Update: input validation

The program now checks the file carefully instead of assuming it is correct:

- **Header comments:** lines starting with `#` (which tools like GIMP add automatically) are skipped anywhere in the header, using a small `Skip()` function.
- **Fail early:** the image type is checked right after it is read, before anything else.
- **Valid header:** if width, height or maximum brightness is not a valid positive number, the program stops with `Invalid header in PGM file`.
- **Missing pixels:** if the file runs out of pixel data, the program stops and reports the exact row and column.
- **Out-of-range values:** pixels below `0` or above the maximum brightness are clamped to the valid range, and one warning shows how many pixels were changed.

## Update: automatic resizing

Before this update, every pixel became one character, so wide images overflowed the terminal, and the program printed every second row to fix the proportions (losing half the image). Now the image is properly resized in both directions.

**Width.** The output is 80 characters wide. The scale is how many image pixels fit into one character:

```
scale = width / outputWidth      // e.g. 400 / 80 = 5
```

`scale` is a `double`, not an `int`. With integer division, a 159-pixel image at 80 columns would get `scale = 1` and lose half of the image. With `scale = 1.9875`, every pixel is used.

**Which pixels belong to which character.** A small `Start()` function gives the first pixel of each block:

```
Start(c) = floor(c * scale)
block of column c = Start(c) ... Start(c + 1) - 1
```

The blocks fit the image exactly: no pixel is skipped, and nothing is read past the end.

**Height.** Terminal characters are about twice as tall as they are wide, so the height is shrunk by `scale` and then by 2 more:

```
outputHeight = height / scale / 2
row_scale    = height / outputHeight
```

`row_scale` uses the real `outputHeight` (after rounding), so the last row block ends exactly on the last image row.

**Averaging.** Each character covers a 2D block (for example 5 × 10 pixels). The program adds up every pixel in the block (`sum`) and counts them (`count`), then uses `sum / count`. Taking the average works like the human eye from a distance: a dark pixel next to a light one looks grey. Simpler methods, like taking only the brightest pixel, would make thin dark lines disappear.

**Edge cases.**
- If the image is narrower than 80 pixels, the output width is reduced to the image width. Otherwise `scale` would be below 1, some blocks would be empty, and the program would divide by zero.
- If the image is very short, the output always has at least 1 line, so the result is never blank.

**Result:** *The Starry Night* (400 × 317) now prints as 80 × 31 characters with the right proportions, instead of 80 × 159.

## Update: command-line options

The file name and output width are no longer hard-coded. The program reads them from the command line through `main(int argc, char* argv[])`:

- `argc` is the number of words typed, including the program name
- `argv[0]` is the program name, `argv[1]` the file, `argv[2]` the width

**Checks:**

- **Wrong number of arguments:** if `argc` is not 2 or 3, the program prints `Usage: ./ascii <file.pgm> [width]` and stops. This check runs first, before `argv[1]` is used.
- **Not a number:** every character of the width is checked with `isdigit`, so input like `hello`, `-20` or `120abc` is rejected. (`stoi("120abc")` would silently return 120, so this check is needed.)
- **Too large for an `int`:** `stoi` is wrapped in `try` / `catch (...)`, so a value like `99999999999` gives an error message instead of a crash.
- **Width below 1:** rejected with an error, since a width of 0 would cause division by zero.
- **Width larger than the image:** capped to the image width (for example, `999` on a 400-pixel image gives 400).

## Future improvements

This project will keep growing. Planned features:

- **Invert option** (`--invert`) so the picture looks right on dark terminal backgrounds *(next)*
- **Save the result** to a `.txt` file
- **Color images**: support PPM files (`P3`) by converting each pixel's red, green and blue values to one brightness
- **Binary formats**: support `P5` and `P6`, which most tools save by default
- **PNG and JPG support**, so images don't need to be converted first

### Done

- ~~**Automatic resizing**: any image size works~~ (see *Update: automatic resizing*)
- ~~**Command-line options**: choose the file and width when running~~ (see *Update: command-line options*)
