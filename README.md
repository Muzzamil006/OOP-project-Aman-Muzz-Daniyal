# Teen Talwar (CS/CE 224 Project, Group 9)

Story-driven adventure from our proposal, with Swords and Sandals style arena fights:
real-time arena fights (move, jump, block, quick / power / special attacks),
gold, an armoury shop, XP and levelling up with stat points.

## Current state (about 50%)
Playable loop for Chapters 1 and 2:

Menu -> Hero Select -> Story (dialogue + choices) -> Devil's Deep minigame -> Armoury shop -> Arena boss fight -> Level Up -> auto-save -> next chapter

After Chapter 2 the game shows "Demo complete".

## Run (Windows, easiest)
1. Put SFML 2.6.1 (GCC 13.1.0 MinGW SEH 64-bit) at `C:\SFML-2.6.1`.
2. Put the matching WinLibs 13.1.0 compiler at `C:\mingw64`. Both folders can also go inside the game folder.
3. Double-click **run.bat**. It finds SFML and the compiler, copies the DLLs, compiles when the code has changed, and starts the game.

## Build manually
SFML 2.5 / 2.6.

CMake:
```
mkdir build && cd build
cmake ..
cmake --build .
./TeenTalwar
```

g++ (run from the project root so `assets/` is found):
```
g++ -std=c++17 -Iinclude src/*.cpp -o TeenTalwar -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./TeenTalwar
```

## Sprites (drawn by the team)
`assets/sprites/` holds our own pixel art, shrunk to native size (54x65, one pixel per block):
- `amaan.png`: Amaan, Knight
- `dani.png`: Daniyal, Mage
- `muzz.png`: Muzzamil, Rogue
- `armour.png`: helmet and chestplate, drawn over the hero once any armour is bought

`CharacterSprite` only loads and positions these images: it flips them to face left or right, moves them slightly for idle, attack and jump, tints them red when hit, and lays them down when defeated.
Characters with no image yet (bosses, NPCs) show as plain coloured blocks.
To add boss art, draw it on the same grid and save it as `assets/sprites/forest_guardian.png` or `frost_giant.png`, with the feet at the bottom centre of the image. The game loads it automatically.

## Arena controls (real-time)
| Key | Action |
|---|---|
| A / D or Left / Right | Move |
| W / Up / Space | Jump (dodges the boss's "!!" slam) |
| S / Down (hold) | Block (takes about a third of the damage, can't move or attack) |
| J or Z | Quick attack (0.5 s cooldown) |
| K or X | Power attack (double damage plus knockback, 1.2 s cooldown) |
| L or C | Class special (5 s cooldown) |

The boss walks up to you, shows "!" before a normal hit and "!!" before its special slam.

Sound is optional. Drop `assets/audio/theme.ogg` and `assets/audio/click.wav` in and they will play.

## Work split
Each person owns about 1,250 lines and their own hero class. See CODE_WALKTHROUGH.md for the exact file lists, what every file does and how to push to GitHub.

| Person | Area |
|---|---|
| Amaan | Fighters and the real-time arena: Stats, Character, Hero, Knight, Enemy, MiniBoss, ArenaState, LevelUpState, GameOverState |
| Daniyal | Story, minigames, menu and saving: story classes, NPC, Mage, Minigame, DevilsDeep, MinigameFactory, MinigameState, MenuState, SaveManager |
| Muzzamil | Engine, art, shop and sound: Game, GameState, Button, CharacterSprite, Rogue, HeroSelectState, ShopState, Item, Inventory, SoundManager |

## Remaining 50%
| Task | Owner |
|---|---|
| Boss sprites (forest_guardian.png, frost_giant.png) and bosses 3 and 4 in MiniBoss::createForChapter | Amaan |
| FinalBoss (Shadow Lord) : MiniBoss, using all earlier specials | Amaan |
| One new enemy attack pattern (e.g. a charge) in ArenaState | Amaan |
| Knightrunners and Alchemist minigames + add them to MinigameFactory | Daniyal |
| chapter3.txt, chapter4.txt and the ending scene | Daniyal |
| Pause menu (Esc) in MenuState style | Daniyal |
| EndingState (final score screen); raise LAST_BUILT_CHAPTER in Game.hpp to 5 | Muzzamil |
| NPC sprites, arena background image, sound files | Muzzamil |
| More shop items for chapters 3-4 | Muzzamil |
| Final implementation class diagram (draw.io) + design-pattern write-up | All |

## OOP requirement map
| Requirement | Where |
|---|---|
| Inheritance (2+ levels) | Character -> Hero -> Knight/Mage/Rogue; Character -> Enemy -> MiniBoss; Character -> NPC; GameState -> 8 states; Minigame -> DevilsDeep |
| Abstract classes + pure virtual | Character::getTitle, Hero::useSpecial, GameState::handleInput/update/draw, Minigame::update/draw/reset |
| Base pointers / override | Game holds GameState*; MinigameState holds Minigame*; HeroSelectState holds vector<Hero*>; MiniBoss::draw, chooseAction |
| Operator overloading | Stats + += == <<; Item == <<; Inventory += []; StoryScene [] |
| Function overloading | Stats::increase, Character::takeDamage, CharacterSprite::draw, Inventory::addItem, StoryScene::addLine, SoundManager::playMusic, GameState::drawText |
| Composition | Character owns its CharacterSprite; Hero owns Inventory; StoryScene owns DialogueLines/Choices; Game owns StoryManager, SaveManager; ArenaState owns its MiniBoss |
| Aggregation | StoryScene points to an NPC owned by StoryManager; Minigame points to the hero's CharacterSprite |
| Association | States use Game's Hero through Game*; Hero::useSpecial(Character& target) |
| Static members | Character::characterCount, SoundManager::instance, MiniBoss::createForChapter, Hero::create, Item::getShopCatalog |
| Design patterns | State (GameState), Singleton (SoundManager), Factory (MinigameFactory, Hero::create, MiniBoss::createForChapter) |
| Dynamic memory | States, heroes, bosses, minigames, NPCs: owner deletes in destructor; copy ops deleted (= delete) |
| No globals | Only constants (WINDOW_WIDTH, WINDOW_HEIGHT) |
