# yaed

![](https://user-images.githubusercontent.com/109112234/235519159-d3cf018b-bb82-4186-850c-1325c2092415.png)

A yet another simple level editor for some tile-based games

## Supported games

- Snowy: Treasure Hunter

## TODO

- [ ] Remove copyrighted assets
  - [ ] Write tool that automatically generates necessary texture atlases from original game distribution 
- [ ] Refactor with C++20 features
- [ ] Properly implement file dialogs, using XDG Desktop Portal on Linux
  - [x] Implement XDG Desktop Portal-based backed
  - [ ] Make dialogs non-blocking 
- [ ] Improve stability
- [ ] Implement keyboard shortcuts for menu bar entries
  - [ ] Implement action system (also, can be used for something like command palette)
- [ ] Implement proper tile linking (portal ins/outs, monster respawn points, etc)
- [ ] Implement necessary editing tools
  - [ ] Flood fill
  - [ ] Select (with boolean operations)
  - [ ] Move
- [ ] Implement copy-paste
- [ ] Implement undo/redo buffer
- [ ] Rewrite GUI using in-house framework