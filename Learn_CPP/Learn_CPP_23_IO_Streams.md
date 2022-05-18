- [Streams](#streams)
- [`Std::IStream`](#stdistream)
  - [Modifiers](#modifiers)
  - [Whitespace](#whitespace)
- [`Std::Ostream`](#stdostream)
  - [Flags and Manipulators](#flags-and-manipulators)
- [String Streams](#string-streams)
- [States & Validation](#states--validation)
  - [Flags](#flags)
  - [Member Functions](#member-functions)
  - [Validation](#validation)
  - [Regex](#regex)
- [File I/O](#file-io)
  - [Buffering](#buffering)
  - [File Modes](#file-modes)
  - [Random I/O](#random-io)
  - [Misc](#misc)
# Streams
- sequence of bytes that can be accessed sequentially over time
  - data producers like keyboards have input streams
  - data consumers like display have output streams
  - devices like file and network have both streams
- for an input `istream`, `operator>>` is used to remove bytes from the stream for use
- for an output `ostream`, `operator<<` is used to insert bytes into the stream to be consumed
- the combined `iostream` (multiply inherits both functionalities) is bidirectional

- classes in `<iostream>` with the `_withassign` postfix can be assigned to other streams of the same directionality
  - usually it is not necessary to deal with these classes directly
- **standard streams** are pre-configured in the environment and automatically connected to the program
  - `std::cin`, `std::cout`, `std::cerr`, `std::clog`
  - all above are `_withassign`
  - `std::cout` is unbuffered while `std::clog` is buffered

# `Std::IStream`
## Modifiers
- modify the stream itself when used with `operator>>`
- *ex.* `std::setw(size)` in `<iomanip>` limits the maximum number of characters read
  - `std::cin >> std::setw(10) >> ...`
  - note that this sets the maximum to 9, not 10, since the extra space is left for inserting `\0` at the end of whatever subsitutes the `...`
## Whitespace
- `operator>>` automatically skips whitespace per extraction
  - for example, if a stream were to be extracted per `char`, then the result will have no whitespace
- the alternative is to use variations of `std::istream::get` which doesn't skip whitespace
  - `std::istream::get()` gets the next `char`
  - `std::istream::get(char*, size)` gets the next `size`-1 characters and forms a C-style string of length `size`
  - note that `std::istream::get` still ignores `\n`
- use `std::istream::getline` when `\n` needs to be read
  - note that `getline` allows specification of delimiters other than `'n`
  - `getline` stops at either the delimiter *or* the max size is reached
  - a special version of `getline` is defined in the `<string>` library
    - `std::istream::getline(std::istream&, std::string)` reads the whole line into the string buffer
- `std::istream::gcount` returns number of characters read in the last `get`/`getline`
- more `istream` functions support features like
  - ignoring
  - reading without removing from stream
  - putting back to the stream

# `Std::Ostream`
## Flags and Manipulators
- `std::ios::setf(flag)` and `std::ios::unsetf(flag)`
- flags can be combined with `|`
- note that `setf` does not unset semantically related flags
  - for example, the `std::ios::dec` and `std::ios::hex` flags are both for setting number format, but are mutually exclusive
  - however, semantically related flags do belong to different **flag groups**
    - to set a flag as the only flag on within a flag group, do `std::ios::setf(flag, flagGroup)`
    - *ex.* `std::ios::setf(std::ios::hex, std::ios::basefield)`
- the alternative is to use manipulators, which set flags automatically
  - *ex.* `std::cout << std::hex << 27`
> the article lists several useful groups and manipulators, skipped

# String Streams
- located in `<sstream>`
- `std::stringstream` for io, `std::istringstream`, `std::ostringstream` for i, o
    - with `w` prefix (*ex.* `std::wstringstream`) for wide characters
- typically used to buffer data for later use
- as described previously, can be used to convert to and from `std::string`
- clearing string streams
  - first call `::str("")` or `::str(std::string{})` to reset buffer
  - then call `::clear()` to reset error flags

# States & Validation
- after extraction/insertion, streams nter different *states*, some of which are erroneous
  - corresponding *flags* are set and could be accessed either directly or through member functions
## Flags
- `std::ios::goodbit`
- `std::ios::badbit` (fatal error)
- `std::ios::eofbit` (end of file)
- `std::ios::failbit` (non-fatal error)

## Member Functions
- `std::ios::good()`
- `std::ios::bad()`
- `std::ios::eof()`
- `std::ios::fail()`
- `std::ios::clear()` clear and set to `good` by default
  - `std::ios::clear(state)`
- `std::ios::rdstate` read current state
- `std::ios::setstate(state)`

## Validation
- character-level validation checks whether a given number (anything `int`-like) fits some criteria
  - *ex* `std::isalnum(int)` checks if the parameter is a letter or digit
    - note that although the parameter is type `int` it is also intended to accept `char`s
- for string validation, it is common to loop through the input string
  - validator functions (`int` $\to$ `int`) can be stored in maps of the form `std::map<char, std::function<int, int>>` which allows special characters to act as standins for validators
    - *ex* use `#` to stand in for `std:isalnum`, use `*` for unconditional `true`, etc.
- for numeric validation, use the fail bit of a `std::istream` after extraction, and then validate the extracted value
- another way is to just read everything as `std::string` and then try to extract numbers from that, this way no garbage is left in the input stream

## Regex
> the tutorial mentions that there is a regex library, but did not go into detail

# File I/O
- located in `<fstream>`
- 3 main classes
  - `std::ifstream`, `std::ofstream`, `std::fstream`
- to open a file stream, instantiate the appropriate class with the file name as parameter
## Buffering
- file output may not be be immediate, as disk writing create latencies
  - when the outputs do get written, the process is called **flushing**
  - a file is guaranteed to flush when closed
- sometimes the program quits unexpectedly before file close, and the outputs may not be written correctly
  - it is good practice to close all files explicitly prior to `exit()`
- other ways of forcing flushes include
  - calling `std::ostream::flush()` on the stream
  - passing `std::flush` to the stream
  - passing `std::endl` to the stream
    - for this reason, it is sometimes better to use `\n` instead of `std::endl` when using file output, this avoid overuse of flushes
## File Modes
- second parameter when instantiating file streams
  - `std::ios::app` append
  - `std::ios::ate` starts at the end but can go backwards if wanted (see "seek" related explanations below)
  - `std::ios::binary`
  - `std::ios::in` read only (default for `std::ifstream`)
  - `std::ios::out` writable (default for `std::ofstream`)
  - `std::ios::trunc` erases the file if already exists
  - file modes are flags, meaning that they can be combined with `|`
- for `std::fstream`, if a file does not exist, opening in read-only mode may throw exception depending on the compilation
  - if the stream is used to create a new file, make sure to use write mode
- it is possible to open a file again after closing
  - `::close()` closes the file
  - `::open()` opens the file

## Random I/O
- as in, to/from arbitrary locations not in sequential order
- each stream has a file pointer
- `::seekg(offset, flag)` manipulates the *get* pointer (for reads)
- `::seekp(offset, flag)` manipulates the *put* pointer (for writes)
  - note that `offset` is on the byte level and can be negative
  - flags include `std::ios::beg`, `std::ios::cur`, `std::ios::end`
- to read the get/put pointers, use `::tellg()` and `::tellp()` member functions
- note that a "NOP" can be used in `std::fstream` to switch between reading and writing
  - `iofile.seekg(iofile.tellg(), std::ios::beg)` or `iofile.seekg(0, std::ios::cur)` (seek the get pointer to the current get pointer, which does nothing)
  - otherwise, `fstream` doesn't switch smoothly between read and write

## Misc
- delete a file with `::remove()`
- read current open/close state with `::is_open()`
- don't write pointers to files since they are not guaranteed to persist between runs