# Teen Talwar: Code Walkthrough

This file explains what every file in the project is for, how the code fits together, who owns what, and how to push it to GitHub.

---

## 1. What the game is

A short 2D story adventure in C++ with SFML 2.6. You pick one of three heroes (Amaan the Knight, Daniyal the Mage, Muzzamil the Rogue) and go through chapters. Each chapter is:

```
Story scenes (dialogue + a choice)
   -> Devil's Deep minigame (collect coins, avoid Evil Aarijs)
   -> Armoury shop (spend gold on gear)
   -> Real-time arena fight against the chapter's mini boss
   -> Level up (spend stat points) if you gained a level
   -> Auto-save -> next chapter
```

Chapters 1 and 2 are built. After chapter 2 a "To be continued" screen appears.

---

## 2. Folder and file map

```
TeenTalwar/
├── run.bat                 double-click on Windows: finds SFML + g++, compiles if code changed, runs the game
├── CMakeLists.txt          build script for CMake (optional, g++ command also works)
├── README.md               short overview, build steps, controls, remaining work
├── CODE_WALKTHROUGH.md     this file
├── .gitignore              tells git to ignore build output, .exe files and save.txt
├── include/                every class's header (.hpp): the declarations
├── src/                    every class's source (.cpp): the definitions, plus main.cpp
└── assets/
    ├── fonts/DejaVuSans.ttf    the font used for all text (free licence)
    ├── sprites/                our own pixel art
    │   ├── amaan.png           Amaan (Knight)
    │   ├── dani.png            Daniyal (Mage)
    │   ├── muzz.png            Muzzamil (Rogue)
    │   └── armour.png          helmet + chestplate, drawn over a hero once armour is bought
    ├── story/
    │   ├── chapter1.txt        dialogue, NPCs and choices for chapter 1
    │   └── chapter2.txt        same for chapter 2
    └── audio/README.txt        where to drop theme.ogg / click.wav (optional)
```

`save.txt` is created next to the game when a chapter is finished. It is not in git.

---

## 3. How the program runs

### 3.1 Start-up
`main.cpp` seeds the random number generator, creates one `Game` object and calls `game.run()`.

The `Game` constructor:
- opens the 960x600 window
- loads the font
- asks `SoundManager` to play `theme.ogg` (nothing happens if it is missing)
- creates the first screen, `MenuState`

### 3.2 The game loop (`Game::run`)
Every frame (60 per second):
1. **Events:** read keyboard and mouse events and pass each one to `currentState->handleInput(event)`.
2. **Update:** call `currentState->update(dt)`, where `dt` is the seconds since the last frame.
3. **Switch screens:** call `applyPendingState()`. If a screen asked to change, the old screen is deleted and the new one becomes current.
4. **Draw:** clear the window, call `currentState->draw(window)`, then display.

### 3.3 Switching screens (State pattern)
Every screen is a subclass of the abstract class `GameState`, which has three pure virtual functions: `handleInput`, `update` and `draw`. `Game` only holds a `GameState*`, so it never needs to know which screen is active.

A screen never deletes itself. It calls a `Game` function such as `game->startShop()`. That function creates the new screen with `new` and stores it as `pendingState`. The swap happens at the end of the frame, so the old screen is never deleted while its own code is still running.

The `Game` functions that control the flow:

| Function | Opens |
|---|---|
| `showMenu()` | MenuState |
| `showHeroSelect()` | HeroSelectState |
| `startNewGame(type)` | creates the Hero, chapter = 1, then `startChapter()` |
| `continueGame()` | loads `save.txt` through SaveManager, then `startChapter()` |
| `startChapter()` | loads `chapterN.txt` and opens StoryState (or the "to be continued" screen after the last built chapter) |
| `startMinigame()` | MinigameState with a minigame from MinigameFactory |
| `startShop()` | ShopState |
| `startArena()` | heals the hero, opens ArenaState with `MiniBoss::createForChapter(chapter)` |
| `showLevelUp()` | LevelUpState |
| `finishChapter()` | chapter + 1, saves, `startChapter()` |
| `showGameOver(msg, canRetry)` | GameOverState |

---

## 4. Every class explained (grouped by owner)

### 4.1 Amaan: fighters and the arena (~1,270 lines)

**Stats** (`Stats.hpp/.cpp`)
Holds strength, defence, agility and vitality.
- `increase(index)` and `increase(index, amount)` raise one stat (function overloading).
- Operators:
  - `+` adds two Stats together (base stats + item bonuses)
  - `+=` adds a bonus into a running total
  - `==` compares two Stats
  - `<<` writes the four numbers to a file
- The constants `STRENGTH`, `DEFENCE`, `AGILITY` and `VITALITY` (0 to 3) are used as indexes.

**CombatAction.hpp**
An enum of action codes: advance, retreat, quick attack, power attack, special, rest. The enemy AI returns one of these.

**Character** (abstract)
The base class for everything that stands on the ground: heroes, enemies and NPCs.
- Stores the name, base stats, health, max health, x position, height above ground (for jumping) and a `CharacterSprite` (composition).
- Max health = 50 + vitality × 10.
- `takeDamage(amount)` lowers health, flashes the sprite red, and lays the sprite down at 0 HP. The overload `takeDamage(amount, multiplier)` scales the damage first.
- `heal`, `restoreFullHealth`, `upgradeStat` and `moveBy` do what their names say.
- `draw` is virtual: it draws the sprite and the name label.
- `getTitle()` is pure virtual, which makes the class abstract.
- `static int characterCount` counts how many characters currently exist (a static member).

**Hero** (abstract, inherits Character)
Adds level, experience, gold, stat points, crystal shards and an `Inventory` (composition).
- `getTotalStats()` overrides Character's version and returns base stats + inventory bonus.
- `addExperience` levels up every `level × 100` XP and gives 3 stat points per level.
- `spendStatPoint(i)` raises one stat.
- `refreshAppearance()` shows the armour image once the inventory gives +2 defence or more.
- Pure virtual functions each hero class must write: `useSpecial`, `getSpecialName`, `getSpecialRange`, `getHeroType`.
- `static Hero* create(type)` is a Factory: it returns a new Knight, Mage or Rogue.

**Knight** (Amaan's hero)
Loads `amaan.png`. Special: **Shield Bash**, range 150, damage = strength × 4 + 5 − enemy defence, and pushes the enemy back 60 px.

**Enemy** (inherits Character)
Stores the gold and XP it gives when beaten. It faces left.

`chooseAction(distance, count)` is the AI and is virtual:
- too far away → advance
- low HP and every 5th attack → rest
- every 3rd attack → power attack
- otherwise → quick attack

**MiniBoss** (inherits Enemy, so Character → Enemy → MiniBoss is a 3-level hierarchy)
Adds a special move that it uses every 4th attack (overrides `chooseAction`) and draws its label in red (overrides `draw`).

`static MiniBoss* createForChapter(chapter)` builds the boss for each chapter:

| Chapter | Boss | Stats (str, def, agi, vit) | Special | Reward |
|---|---|---|---|---|
| 1 | Forest Guardian | 6, 4, 3, 9 | Vine Lash, 14 damage | 60 gold, 120 XP |
| 2 | Frost Giant | 9, 6, 2, 13 | Ice Boulder, 22 damage | 120 gold, 220 XP |

It tries to load `forest_guardian.png` / `frost_giant.png`; until those exist the boss is a coloured block.

**ArenaState** (the real-time fight)
Owns the `MiniBoss*` and deletes it in its destructor. Copying is deleted.

The player:
- `handleInput` reacts to single key presses:
  - W / Up / Space: jump
  - J / Z: quick attack
  - K / X: power attack
  - L / C: special
- `updateHero(dt)`:
  - reads held keys: A/D or arrows to move, S/Down to block
  - movement speed = 180 + agility × 8 pixels per second
  - applies gravity to jumps
  - counts down the three cooldowns: quick 0.5 s, power 1.2 s, special 5 s

The boss, in `updateEnemy(dt)`, cycles through three modes:
1. **APPROACH:** walks towards the hero. Once in range it asks `chooseAction` what to do.
2. **WINDUP:** waits 0.45 s (quick), 0.8 s (power) or 1.0 s (special) while "!" or "!!" shows above it.
3. **RECOVER:** pauses 0.9 s, then goes back to APPROACH.

At the end of the windup, `enemyStrike()` deals damage:
- If the hero is blocking, they take only a third.
- The special slam misses if the hero is in the air.

Damage formula (`calculateDamage`): (strength + 2 + random 0–3) × multiplier − defender's defence / 2, minimum 1.
- Quick attack: × 1
- Hero power attack: × 2
- Enemy power attack: × 1.8

Other helpers:
- `keepApart()` keeps the fighters at least 110 px apart and inside the arena.
- `checkFightEnd()` gives gold, a shard and XP on a win. Three seconds later it opens LevelUpState (if there are stat points) or `finishChapter()`. On a loss it opens GameOverState.

**LevelUpState**
Shows the four stats with "+" buttons. Each click calls `hero->spendStatPoint(i)`. Continue → `finishChapter()`.

**GameOverState**
Shows "GAME OVER" with **Retry Fight** (→ `startArena()`) and **Main Menu**. With `canRetry = false` it becomes the "TO BE CONTINUED" screen shown after the last built chapter.

### 4.2 Daniyal: story, minigames, menu and saving (~1,240 lines)

**DialogueLine**
Holds one line: the speaker and the text. A speaker of `HERO` is replaced by the hero's name on screen.

**Choice**
Holds a choice's button text, the NPC's reply and the gold reward.

**NPC** (inherits Character)
A story character with a role, e.g. "Village Elder". It faces left and shows as a block until it has a sprite.

**StoryScene**
One scene: title, background colour, the NPC on screen, its dialogue lines and its choices.
- It holds an `NPC*` that it does **not** own. StoryManager owns the NPC, so this is **aggregation**.
- It owns its lines and choices (composition).
- `operator[]` returns line i.
- `addLine(DialogueLine)` and `addLine(speaker, text)` are overloads.

**StoryManager**
Reads `assets/story/chapterN.txt` line by line and builds the scenes. It owns the `NPC*`s, deletes them in `clear()` and in its destructor, and its copy operations are deleted. It also tracks the current scene and line (`nextLine`, `nextScene`, `isLastLine`).

Story file format (each part separated by `;`):
```
TITLE Chapter 1: The Whispering Forest
NPC <name>;<role>;<r>;<g>;<b>                       (NPCs are numbered 0, 1, 2... in order)
SCENE <title>;<npc number>;<bg r>;<bg g>;<bg b>
LINE <speaker>;<text>                               (speaker HERO = the player's hero)
CHOICE <button text>;<NPC reply>;<gold reward>      (only on the last line of a scene)
```
To add a chapter, write `chapter3.txt` in this format; no code change is needed except raising `LAST_BUILT_CHAPTER`.

**StoryState**
The dialogue screen. It draws the background, the hero on the left, the NPC on the right and the dialogue box.
- Space, Enter or a click moves to the next line.
- On a scene's last line, if the scene has choices, it shows them as buttons (or keys 1 and 2). Picking one shows the reply and adds the gold.
- After the last scene → `startMinigame()`.

**Minigame** (abstract)
Shared data for every minigame: name, instructions, timer, score, finished and won.
- `tickTimer` and `finish` are shared helpers (reuse instead of copy-paste).
- `reset`, `update` and `draw` are pure virtual.
- `getGoldReward()`: score × 3 + 20 if won, score × 2 if lost.
- It also holds a pointer to the hero's sprite (not owned) so it can draw the hero small.

**DevilsDeep** (inherits Minigame)
- Move with WASD / arrows inside the red arena.
- Coins respawn when collected; Evil Aarijs bounce around.
- Touching an Aarij or running out of time ends it. You win if you collected enough coins.

**MinigameFactory** (Factory pattern)
`create(chapter)` returns the right minigame as a `Minigame*`. For now it always returns Devil's Deep: 8 coins needed in chapter 1, plus 4 per later chapter, and one more Aarij each chapter. Knightrunners and Alchemist get added here.

**MinigameState**
Owns the `Minigame*` and uses it only through base-class calls (polymorphism). It has three phases:
1. intro screen → Start
2. playing
3. result → Retry (costs 5 gold) or Continue (adds the reward → `startShop()`)

**MenuState**
Title screen with Start New Game, Continue (only enabled when `save.txt` exists), Instructions (a pop-up panel) and Exit.

**SaveManager**
Writes and reads `save.txt`:
```
<hero type> <chapter>
<level> <xp> <gold> <stat points> <shards>
<base str> <def> <agi> <vit>            (written with Stats operator<<)
<number of items>
<type> <price> <str> <def> <agi> <vit> <item name>     (one line per item, Item operator<<)
```
`load()` rebuilds the hero with `Hero::create`, restores the stats, progress and items, then refreshes the armour image.

**Mage** (Daniyal's hero)
Loads `dani.png`. Special: **Fireball**, which works from anywhere on screen. Damage = 25 + agility × 2.

### 4.3 Muzzamil: engine, art, shop and sound (~1,230 lines)

**main.cpp**
Creates the `Game` and runs it.

**Game**
The engine. It owns:
- the window and font
- the current and pending screens
- the `Hero*`
- the StoryManager, SaveManager and MinigameFactory (all composition)

It has the game loop and all the flow functions in section 3.3. Its destructor deletes the screens, the hero and the SoundManager. Copying is deleted.

`WINDOW_WIDTH` and `WINDOW_HEIGHT` are global **constants** (allowed), and `LAST_BUILT_CHAPTER` is a static constant member.

**GameState** (abstract)
The base class for all 8 screens. It keeps a `Game*` and shared drawing helpers that every screen reuses:
- `drawText` (two overloads: with or without a colour)
- `drawCenteredText`
- `drawPanel`

**Button**
A clickable rectangle with centred text.
- `isClicked(event)` is true for a left click inside an enabled button.
- `setEnabled(false)` greys it out.

**CharacterSprite**
Loads and draws one character's image. It only moves our own images; it does not draw any art itself.
- `loadTexture(path, originX, originY)`: the origin is the point between the feet, so the sprite stands on the ground. There is an overload without an origin that uses the bottom centre.
- `loadArmour(path)` plus `setShowArmour(true)` draws `armour.png` over the hero.
- Animation:
  - idle: 1-pixel bob
  - `playAttack()`: short lunge and tilt
  - `playHurt()`: red tint
  - `setDefeated(true)`: lies down
- `setFacingRight(false)` mirrors the image.
- If no image is loaded, it draws a plain coloured block instead.
- Our sprites are 54×65 pixels and drawn 3× bigger (bosses 4×) with smoothing off, so the pixels stay sharp.

**Rogue** (Muzzamil's hero)
Loads `muzz.png`. Special: **Dash Strike**, range 450. It teleports next to the enemy and hits for strength × 2 + agility × 3 − defence.

**HeroSelectState**
Creates one preview hero of each class (owns them, deletes them in its destructor) and shows them as cards with stats and special. Click or use the arrow keys to choose, then Confirm → `startNewGame(type)`.

**Item**
A shop item: name, type (weapon / armour / trinket), price and a Stats bonus.
- `operator==` compares by name, so you can't buy the same item twice.
- `operator<<` writes the item to the save file.
- `static getShopCatalog(chapter)` lists what the shop sells. Two stronger items unlock in chapter 2.

**Inventory**
The hero's items. `+=` buys an item, `[]` reads item i, and `addItem(Item)` / `addItem(name, type, price, bonus)` are overloads. `getTotalBonus()` adds up every item's Stats.

**ShopState**
The Armoury: a list of items with Buy buttons, plus your gold, stats and a preview of your hero. Buying refreshes max HP and the armour image. "Enter the Arena" → `startArena()`.

**SoundManager** (Singleton pattern)
- Only one copy can exist: the constructor is private, and `getInstance()` creates it the first time.
- `static SoundManager* instance` is a static member.
- `playMusic(path)` and `playMusic(path, volume)` are overloads.
- Missing sound files are silently skipped.

**run.bat**
Windows launcher:
1. Looks for SFML at `C:\SFML-2.6.1` (or inside the game folder) and for g++ at `C:\mingw64\bin` (or inside the game folder), then adds that compiler to PATH just for this run.
2. Copies the SFML DLLs if they are missing.
3. Recompiles only if `TeenTalwar.exe` is missing or any file in `src/` or `include/` is newer than it.
4. Starts the game.

If something is missing it prints what to download and where to put it.

**CMakeLists.txt**
Builds every `.cpp` in `src/` and copies `assets/` next to the program.

---

## 5. Class relationships (for the class diagram)

| Relationship | Example | Lifetime |
|---|---|---|
| Inheritance | Character → Hero → Knight / Mage / Rogue; Character → Enemy → MiniBoss; Character → NPC; GameState → 8 screens; Minigame → DevilsDeep | |
| Composition | Character has a CharacterSprite; Hero has an Inventory; StoryScene has DialogueLines and Choices; Game has StoryManager, SaveManager, MinigameFactory | destroyed together with the owner |
| Ownership by pointer | Game owns `Hero*` and the screens; ArenaState owns `MiniBoss*`; MinigameState owns `Minigame*`; StoryManager owns `NPC*`s | owner calls `delete` in its destructor; copying deleted |
| Aggregation | StoryScene → `NPC*` (owned by StoryManager); Minigame → hero's CharacterSprite | not deleted by the holder |
| Association | every screen uses the Hero through `Game*`; `Hero::useSpecial(Character& target)` | short-lived use, no ownership |

Design patterns:
- **State:** GameState and its screens
- **Singleton:** SoundManager
- **Factory:** MinigameFactory, `Hero::create`, `MiniBoss::createForChapter`

---

## 6. How to add things later

| To add | Do this |
|---|---|
| A chapter | Write `assets/story/chapter3.txt`, add a boss in `MiniBoss::createForChapter`, raise `LAST_BUILT_CHAPTER` in Game.hpp |
| A minigame | Make a class that inherits `Minigame` and writes `reset`, `update` and `draw`; return it from `MinigameFactory::create` |
| A shop item | One `catalog.push_back(Item(...))` line in `Item::getShopCatalog` |
| A boss sprite | Draw it like the hero sprites, shrink it to one pixel per block, save it as `assets/sprites/<name>.png`, and call `loadTexture` in `createForChapter` |
| Sound | Drop `theme.ogg` and `click.wav` into `assets/audio/` |

---

## 7. Who owns which files

Each person only edits and commits their own files. Everyone owns their own hero.

**Amaan**
```
include/Stats.hpp          src/Stats.cpp
include/CombatAction.hpp
include/Character.hpp      src/Character.cpp
include/Hero.hpp           src/Hero.cpp
include/Knight.hpp         src/Knight.cpp
include/Enemy.hpp          src/Enemy.cpp
include/MiniBoss.hpp       src/MiniBoss.cpp
include/ArenaState.hpp     src/ArenaState.cpp
include/LevelUpState.hpp   src/LevelUpState.cpp
include/GameOverState.hpp  src/GameOverState.cpp
assets/sprites/amaan.png
```

**Daniyal**
```
include/DialogueLine.hpp     src/DialogueLine.cpp
include/Choice.hpp           src/Choice.cpp
include/NPC.hpp              src/NPC.cpp
include/StoryScene.hpp       src/StoryScene.cpp
include/StoryManager.hpp     src/StoryManager.cpp
include/StoryState.hpp       src/StoryState.cpp
include/Minigame.hpp         src/Minigame.cpp
include/DevilsDeep.hpp       src/DevilsDeep.cpp
include/MinigameFactory.hpp  src/MinigameFactory.cpp
include/MinigameState.hpp    src/MinigameState.cpp
include/MenuState.hpp        src/MenuState.cpp
include/SaveManager.hpp      src/SaveManager.cpp
include/Mage.hpp             src/Mage.cpp
assets/story/chapter1.txt  assets/story/chapter2.txt
assets/sprites/dani.png
```

**Muzzamil**
```
src/main.cpp
include/Game.hpp             src/Game.cpp
include/GameState.hpp        src/GameState.cpp
include/Button.hpp           src/Button.cpp
include/CharacterSprite.hpp  src/CharacterSprite.cpp
include/Rogue.hpp            src/Rogue.cpp
include/HeroSelectState.hpp  src/HeroSelectState.cpp
include/Item.hpp             src/Item.cpp
include/Inventory.hpp        src/Inventory.cpp
include/ShopState.hpp        src/ShopState.cpp
include/SoundManager.hpp     src/SoundManager.cpp
assets/sprites/muzz.png    assets/sprites/armour.png
assets/fonts/DejaVuSans.ttf  assets/audio/README.txt
run.bat  CMakeLists.txt  .gitignore  README.md  CODE_WALKTHROUGH.md
```

---

## 8. Pushing to GitHub

### One-time setup (everyone)
1. Install Git from git-scm.com, keeping the default options.
2. In a terminal, set your name and email:
   ```
   git config --global user.name "Your Name"
   git config --global user.email "your-github-email@example.com"
   ```

### Step 1: Muzzamil creates the repo and pushes first
1. On github.com click **New repository**, name it `TeenTalwar`, and **don't** tick "Add a README". Create it.
2. Go to Settings → Collaborators → add Amaan and Daniyal by GitHub username. They must accept the email invite.
3. Unzip the project, open a terminal **inside the TeenTalwar folder**, and run:
   ```
   git init
   git branch -M main
   git add README.md CODE_WALKTHROUGH.md CMakeLists.txt .gitignore run.bat
   git add assets/fonts assets/audio assets/sprites/muzz.png assets/sprites/armour.png
   git add src/main.cpp
   git add include/Game.hpp src/Game.cpp include/GameState.hpp src/GameState.cpp include/Button.hpp src/Button.cpp
   git add include/CharacterSprite.hpp src/CharacterSprite.cpp include/Rogue.hpp src/Rogue.cpp
   git add include/HeroSelectState.hpp src/HeroSelectState.cpp include/ShopState.hpp src/ShopState.cpp
   git add include/Item.hpp src/Item.cpp include/Inventory.hpp src/Inventory.cpp include/SoundManager.hpp src/SoundManager.cpp
   git commit -m "Engine, screens, shop, sprites and sound"
   git remote add origin https://github.com/<muzz-username>/TeenTalwar.git
   git push -u origin main
   ```
   The first push opens a browser window to sign in to GitHub. Run `git status` before committing to check that only your files are listed under "Changes to be committed".

### Step 2: Amaan pushes his files
1. Clone the repo into a **new** folder (not the unzipped one):
   ```
   git clone https://github.com/<muzz-username>/TeenTalwar.git
   cd TeenTalwar
   ```
2. Copy **only your files** from the zip into the same places in this folder (list in section 7).
3. Commit and push:
   ```
   git add include/Stats.hpp src/Stats.cpp include/CombatAction.hpp
   git add include/Character.hpp src/Character.cpp include/Hero.hpp src/Hero.cpp include/Knight.hpp src/Knight.cpp
   git add include/Enemy.hpp src/Enemy.cpp include/MiniBoss.hpp src/MiniBoss.cpp
   git add include/ArenaState.hpp src/ArenaState.cpp include/LevelUpState.hpp src/LevelUpState.cpp
   git add include/GameOverState.hpp src/GameOverState.cpp assets/sprites/amaan.png
   git commit -m "Fighters, real-time arena, level up and game over"
   git pull
   git push
   ```

### Step 3: Daniyal pushes his files
Same as Amaan, with Daniyal's files:
```
git clone https://github.com/<muzz-username>/TeenTalwar.git
cd TeenTalwar
(copy only your files from the zip in)
git add include/DialogueLine.hpp src/DialogueLine.cpp include/Choice.hpp src/Choice.cpp include/NPC.hpp src/NPC.cpp
git add include/StoryScene.hpp src/StoryScene.cpp include/StoryManager.hpp src/StoryManager.cpp include/StoryState.hpp src/StoryState.cpp
git add include/Minigame.hpp src/Minigame.cpp include/DevilsDeep.hpp src/DevilsDeep.cpp
git add include/MinigameFactory.hpp src/MinigameFactory.cpp include/MinigameState.hpp src/MinigameState.cpp
git add include/MenuState.hpp src/MenuState.cpp include/SaveManager.hpp src/SaveManager.cpp include/Mage.hpp src/Mage.cpp
git add assets/story assets/sprites/dani.png
git commit -m "Story system, minigame, menu, saving and Mage"
git pull
git push
```

The game only compiles once all three have pushed, because the classes use each other. After step 3, everyone runs `git pull` and builds.

### Every day after that
```
git pull                       (always first, to get the others' work)
...edit your own files, build, test...
git add <the files you changed>
git commit -m "What you did, e.g. Added Knightrunners minigame"
git pull                       (get anything pushed while you worked)
git push
```
Rules that avoid trouble:
- Only edit files you own. If you need a change in someone else's file, ask them.
- Never commit `save.txt`, `.exe`, `.dll` files or `build/`; `.gitignore` already blocks these.
- If `git pull` reports a **conflict**, open the file it names, keep the right lines between `<<<<<<<` and `>>>>>>>`, delete the markers, then `git add` that file, `git commit` and `git push`.
- Write a clear commit message every time, since the commit history shows each person's contribution.
