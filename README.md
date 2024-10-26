# Brainrot Programming Language

Brainrot is a meme-inspired programming language that translates common programming keywords into internet slang and meme references. It's built using Flex (lexical analyzer) and Bison (parser generator), making it a fun way to learn about language processing and compiler design.

## 🤔 What is Brainrot?

Brainrot is a C-like programming language where traditional keywords are replaced with popular internet slang. For example:

- `void` → `skibidi`
- `int` → `rizz`
- `for` → `flex`
- `return` → `bussin`

## 📋 Requirements

To build and run the Brainrot compiler, you'll need:

- GCC (GNU Compiler Collection)
- Flex (Fast Lexical Analyzer)
- Bison (Parser Generator)

### Installation on Different Platforms

#### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install gcc flex bison
```

#### Arch Linux

```bash
sudo pacman -S gcc flex bison
```

#### macOS (using Homebrew)

```bash
brew install gcc flex bison
```

## 🚀 Building the Compiler

1. Clone this repository:

```bash
git clone https://github.com/yourusername/brainrot.git
cd brainrot
```

2. Generate the parser and lexer:

```bash
bison -d -Wcounterexamples lang.y -o lang.tab.c
flex -o lang.lex.c lang.l
```

3. Compile the compiler:

```bash
make
```

## 💻 Usage

1. Create a Brainrot source file (e.g., `hello.brainrot`):

```c
skibidi main {
    yapping("Hello, World!");
    bussin 0;
}
```

2. Run your Brainrot program:

```bash
./brainrot < hello.brainrot
```

## 📚 Language Reference

### Keywords

| Brainrot | C Equivalent |
| -------- | ------------ |
| skibidi  | void         |
| rizz     | int          |
| flex     | for          |
| bussin   | return       |
| edging   | if           |
| amogus   | else         |
| goon     | while        |
| bruh     | break        |
| grind    | continue     |
| chad     | float        |
| gigachad | double       |
| yap      | char         |
| grimace  | const        |
| sigma    | case         |
| based    | default      |
| mewing   | do           |
| gyatt    | enum         |
| whopper  | extern       |
| cringe   | goto         |
| giga     | long         |
| edgy     | register     |
| soy      | short        |
| nut      | signed       |
| maxxing  | sizeof       |
| salty    | static       |
| gang     | struct       |
| ohio     | switch       |
| chungus  | union        |
| nonut    | unsigned     |
| schizo   | volatile     |

### Builtin functions

- `yapping(string)`: equivalent to `printf`

### Operators

The language supports basic arithmetic operators:

- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `=` Assignment
- `<` Less than
- `>` Greater than
- `&&` Logical AND
- `||` Logical OR

## ⚠️ Limitations

Current limitations include:

- No support for increment/decrement operators (++/--)
- Limited support for complex expressions
- No support for functions other than main
- Basic error reporting

## 🤝 Contributing

Feel free to contribute to this project by:

1. Forking the repository
2. Creating a new branch for your feature
3. Submitting a pull request

## 📝 License

This project is licensed under the GPL License - see the LICENSE file for details.

## 🙏 Acknowledgments

- This project is created for educational purposes
- Inspired by meme culture and internet slang
- Built using Flex and Bison tools

## 🐛 Known Issues

1. Error reporting is minimal
2. No support for arrays
3. Limited string manipulation capabilities
4. No support for complex control structures

Please report any additional issues in the GitHub Issues section.
