# Gravity is Your Worst Enemy

## Introduction
_"Gravity is Your Worst Enemy"_ is an original lunar landing video game which was created in the span of a month. This project was conceived to familiarize myself with C++ and its vast range of libraries. I wanted to challenge myself to not only understand the intricacies of the language but to also manage and organize a large-scale project and use github. Throughout the development process, I found it very challenging to imitate the nuances of space physics, particularly the inertia and momentum of the rocket ship movement and the complexities of rocket landing on a rotating planet.

<img width="466" alt="Screen Shot 2023-09-05 at 4 48 54 PM" src="https://github.com/tcohenkl/Space-Game/assets/132385526/cd130271-a6ea-4f43-8933-33eaa6ba33f4">

## Tech Stack
- **Language**: C++
- **Graphics**: SFML
- **Collisions**: Nick Koirala's Collision file

## Game Mechanics
**Your mission**: Navigate space and travel the furthest distance possible without crashing your ship into any planet. 

- **Ship Movement**: Rotate your ship and thrust forward.

  _Ship with no thrust:_

  <img width="58" alt="Screen Shot 2023-09-05 at 5 01 18 PM" src="https://github.com/tcohenkl/Space-Game/assets/132385526/6ebede44-7f3a-4b77-8a3e-b54ef0276f60">

  _Ship with thrust (activated by pressing w):_

  <img width="58" alt="Screen Shot 2023-09-05 at 5 02 30 PM" src="https://github.com/tcohenkl/Space-Game/assets/132385526/bf08d209-2ecb-4db7-9331-9de4a8786d9c">

- **Gravity Fields**: Visible around each planet, they'll pull your ship in.

  _Planet Gravity Field:_

  <img width="284" alt="Screen Shot 2023-09-05 at 5 03 35 PM" src="https://github.com/tcohenkl/Space-Game/assets/132385526/1fb65d75-453b-4a6f-bde1-8b75b714b89a">

- **Landings**: Land with precision on a planet's thruster. Use the planet's rotation for strategic maneuvers. But remember, prolonged survival means faster planet spawn times...

  <img width="288" alt="Screen Shot 2023-09-05 at 5 05 49 PM" src="https://github.com/tcohenkl/Space-Game/assets/132385526/cc507c9b-ec42-4f9a-876b-b3f01c1be647">

- **Game Over**: Reflect on your journey with metrics on distance and survival time.
  

  _Improper Landing:_
  
  https://github.com/tcohenkl/Space-Game/assets/132385526/16812280-4ff5-4760-a838-4d0ca56255f9

  _Gameover Screen:_

  <img width="511" alt="Screen Shot 2023-09-05 at 4 50 39 PM" src="https://github.com/tcohenkl/Space-Game/assets/132385526/6b986c9e-50fc-4651-9dbc-23e8860e72d0">

## Controls
- **W**: Thrust.
- **A**: Rotate counter-clockwise.
- **D**: Rotate clockwise.

## Sprites
-  pixelartmaker.com 
