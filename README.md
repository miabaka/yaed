# yaed

![](https://user-images.githubusercontent.com/109112234/197661857-598ed5c7-8755-4b0c-9133-80acdee613d0.png)

A **y**et **a**nother level **ed**itor for some old tile-based games including:

- Snowy: Treasure Hunter

Currently, its user interface based on Dear ImGui library, but will be rewritten using custom toolkit in near future, and the level renderer itself based on OpenGL 3.3

## Supported Games

- [ ] Gold Sprinter
- [x] Snowy: Treasure Hunter
  - [X] Basic tile editing
  - [X] Level rendering
  - [ ] Level reordering
  - [X] Teleport and monster allocation
  - [ ] Teleport and monster linking
  - [X] World / level creation
  - [X] World export
  - [X] World import
- [ ] Snowy: Treasure Hunter III

## Tested Platforms

- Windows 11 21H2 (x86, msvc, Visual Studio 2022)
- Windows 11 21H2 (x86_64, clang, MSYS2 Clang)
- Windows 11 21H2 (x86_64, clang, MSYS2 MinGW)
- Windows 11 21H2 (x86_64, gcc, MSYS2 MinGW)
- Arch Linux (x86_64, gcc)
- Elbrus Linux 7.1 (e2k, lcc 1.26.15)