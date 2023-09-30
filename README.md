# Pong-Game

## Overview
This project involves coding the classic game Pong using the C Language, OpenGL, and x86 inline assembly language. The program comprises an introductory screen and the game itself.

## Introduction Screen

During the development process, I encountered several challenges that required significant effort to overcome. Here are some key points regarding the implementation:

### Integration of GLUT and GLAD
Integrating GLUT and GLAD posed a notable challenge. Coordinating these libraries for seamless functionality required meticulous attention to detail and careful configuration.

### Rendering 3D Objects and Shaders
Rendering 3D objects and implementing shaders presented complex tasks. Achieving the desired visual effects demanded in-depth understanding and experimentation with OpenGL and GLSL.

### Skyline Implementation
To create a skyline effect in the introductory screen, I leveraged fragment shaders to manipulate colors. By predominantly applying a gray shade with a contrasting blue, I aimed to evoke a cityscape ambiance.

### 3D Cuboid Rendering
Despite numerous attempts, rendering a 3D cuboid proved elusive. Various approaches were explored, yet none yielded the desired outcome. As a result, 2D rectangles were employed to represent the paddles.

### Ball Representation
Efforts were made to utilize triangles for drawing the spherical ball. However, achieving a smooth appearance proved to be a challenge, leading to alternative approaches to ensure visual coherence.

These challenges prompted creative problem-solving and underscored the intricate nature of graphics programming. While certain objectives were met with success, others required adaptability and resourcefulness to achieve the desired result. Overall, this project has been a valuable learning experience in the realm of graphics programming and game development.

## Game Description
- The game is displayed in a traditional Pong format.
- A playing field with boundary lines.
- Ball bounces off the walls at a constant speed.
- Goals on the right and left for scoring.
- Scoring happens when the ball goes through a goal.
- Ball restarts at the center after a goal.
- Initial ball direction determined randomly.
- Score digits can be within or outside the playing field.
- Paddles are simple rectangles moving up or down.
- Ball bounces off paddles or walls at a constant speed.
- Game ends when one player reaches 9 points.
- End of game message: "End of Game! Press any key to end or 'r' to restart."

## Players
- One player is AI-controlled.
- The other player is human.

## Additional Information
- The game is in 2D.
- Game has a constant ball speed.

## Side Notes

During the development process, a specific gameplay issue was encountered that proved to be challenging to resolve. The problem arises when the ball makes direct contact with the top or bottom edge of the AI or player paddle. In this scenario, the ball's movement along the x-axis experiences consistent negation, resulting in jittery motion along the y-axis.

The root cause of this behavior is attributed to the prescribed function description, which mandates the negation of the ball's x-axis movement upon collision with a paddle.

Efforts were made to rectify this issue through multiple iterations, but due to the inherent dynamics of the game and the specified behavior, finding a satisfactory solution proved to be elusive.

While this issue persists, it is important to note that it occurs under specific circumstances and may not significantly impact overall gameplay. Nevertheless, it remains an area of potential improvement for future iterations.
