# raycaster
This was originally a project intended as a framework to fork to get up and running in SDL2 on windows. I didn't stick to my own original ideas and this has accidentally turned into my raycaster game project. At some point I should change the repository name and factor out the framework bits to their own repo.

### Build and Run
1. Install Visual Studio 2022 Community Edition https://visualstudio.microsoft.com/vs/community/
1. clone this repository ```git clone https://github.com/rad-corps/sdl2-starter.git```
1. Open the solution file in the root of this repository ./sdl2-starter.sln
1. CTRL+SHIFT+B to build
1. F5 to run

### Raycaster keys
Key           | Function
------------- | -------------
Tab           | Display top down view
`             | Show/hide player rays
Arrows        | Move/rotate player
CTRL+Arrows   | Strafe

### TODO
- [ ] Fix project directory structure ./raycaster/src/game/src/game is not intuitive
- [ ] Better application structure
  - [ ] Currently two main components. It's not really clear where the responsibilities lie between these. 
    - [ ] GameSceneRaycaster - Outer game loop. Proof of concept code also ends up in here.
    - [ ] RaycastEngine - Calculate wall and floor positions. Renders floors, but GameSceneRaycaster currently renders walls (even though they are calculated here)
    - [ ] Create a unified place for rendering
  - [x] Add a Sprite component (POC currently lives in GameSceneRaycaster)
    - [x] don't draw sprites behind walls
      - [x]consider sprites partially covered by walls
    - [ ] animate sprites based on viewing angle (sides and back of enemies)
    - [x] Sprite scaling
    - [ ] Fisheye correction
    - [ ] Sort sprites furthest to nearest (so closest sprites are drawn last)
    - [ ] bug: sprites clipped too early off LHS of screen
  - [ ] Add a cordoned off section for proof of concepts
- [ ] Maps
  - [ ] Load a map from disk
    - [ ] import from some pre-existing filetype (Tiled editor maybe?)
  - [ ] Define:
    - [ ] wall tiles
    - [ ] floor tiles
    - [ ] enemy types and positions
    - [ ] pickup types and positions
    - [ ] rules?
  - [ ] Randomly generate a map
  - [ ] Move any map specifics out of GameSceneRaycaster
- [ ] Raycasting
  - [ ] ~~Go back to software/pixel raycasting? The performance of rendering gpu texture column strips may not be worth the issues it presents~~
- [ ] Bug: FixedUpdate attempts to catch up if FPS target is not being reached
