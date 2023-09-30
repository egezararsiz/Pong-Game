// Nuri Ege Zararsiz

// Template by: Allan Wei
// allan.wei@mail.mcgill.ca


#include <stdio.h>
#include <stdlib.h>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <math.h>




//Represents a point in 2D space
//x and y are in pixels
typedef struct Point{
    int x; //Pixels
    int y; //Pixels
} Point;

//Color struct
//OpenGl can use either float or unsigned char for color values.
//This struct uses GLubyte which is the same as unsigned char.
//Make sure that you are calling the correct color function in OpenGL, when using this struct.
typedef struct Color{
    GLubyte r; //0-255
    GLubyte g; //0-255
    GLubyte b; //0-255
} Color;

//Global variables struct
typedef struct Global{
    Point playerPaddlePosition;
    Point aiPaddlePosition;
    int playerScore;
    int aiScore;
    Point ballPosition;
    int ballSpeed; //Pixels/Frame
    Point ballDirection;
    int lastScore; //0 = player, 1 = ai
    int gameOver; //0 = false, 1 = true
} Global;
Global global;

//Consts you should not change these values.
#define screenWidth 1920
#define screenHeight 1080
#define paddleOffset 120
#define scoreSize 22
#define scorePosition (screenHeight * 0.9)
const int aiPaddleSpeed = 5;
const int scoreGap = 50;
const int ballSpeedupFactor = 1;
const int paddleWidth = 40;
const int paddleLength = 200;
const int ballSideLength = 30;
const int initialBallSpeed = 5;
const Color paddleColor = (Color){255, 255, 255};
const Point playerScorePosition = (Point){screenWidth - paddleOffset, scorePosition};
const Point aiScorePosition = (Point){paddleOffset - scoreSize, scorePosition};
const Point initialBallPosition = (Point) {screenWidth / 2, screenHeight / 2};
const Point initialBallDirection = (Point) {1, 1};

void initGlobals(){
    //Initializes the global variables
    //They are all under the global struct, and can be access using global.variableName
    //You should not change this function.
    global.playerPaddlePosition = (Point){screenWidth - paddleOffset - paddleWidth, (screenHeight / 2) - paddleLength / 2};
    global.aiPaddlePosition = (Point){paddleOffset, (screenHeight / 2) - (paddleLength / 2)};
    global.playerScore = 0;
    global.aiScore = 0;
    global.ballPosition = initialBallPosition;
    global.ballSpeed = initialBallSpeed;
    global.ballDirection = initialBallDirection;
    global.lastScore = 0;
    global.gameOver = 0;
}


//Helper functions to convert from pixel coordinates into screen space, which OpenGl expects.
//You should use these functions to convert your pixel coordinates into screen space.
//We use pixel coordinates because it is easier to work with in assembly, than floating point numbers.
float pixelToScreenX(int x){
    return (2.0f * (float) x / (float) (screenWidth - 1) - 1.0f);
}
float pixelToScreenY(int y){
    return -(2.0f * (float) y / (float) (screenHeight - 1) - 1.0f);
}

// Helper function to draw walls around the field
void drawWalls(){
    float beginningX = pixelToScreenX(0);
    float beginningY = pixelToScreenY(0);
    float endX = pixelToScreenX(screenWidth);
    float endY = pixelToScreenY(screenHeight);

    // goal is 3 times the paddle length?
    float goalUpY = pixelToScreenY(screenHeight / 2 - 1.5 * paddleLength);
    float goalDownY = pixelToScreenY(screenHeight / 2 + 1.5 * paddleLength);

    glLineWidth(10.0f); //10 pixel width lines

    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex2f(beginningX, endY);
    glVertex2f(endX, endY);
    glVertex2f(beginningX, beginningY);
    glVertex2f(beginningX, goalUpY);
    glVertex2f(beginningX, goalDownY);
    glVertex2f(beginningX, endY);
    glVertex2f(endX, goalDownY);
    glVertex2f(endX, endY);
    glVertex2f(endX, beginningY);
    glVertex2f(endX, goalUpY);
    glVertex2f(beginningX, beginningY);
    glVertex2f(endX, beginningY);
    glEnd();
};

void drawBall(){
    //EXAMPLE:
    //This is an example of how your OpenGL code should look like.
    //You can use this as a template for your own code.
    //You must use glBegin and glEnd to draw your shapes.
    //You must convert the pixel coordinates to screen coordinates using pixelToScreenX and pixelToScreenY.
    //The pixelToScreen functions are nonlinear meaning that f(x + y) != f(x) + f(y).
    //So you have to add the pixel values before you convert to screen space.
    float x = pixelToScreenX(global.ballPosition.x);
    float y = pixelToScreenY(global.ballPosition.y);
    float widthX = pixelToScreenX(global.ballPosition.x + ballSideLength);
    float lengthY = pixelToScreenY(global.ballPosition.y + ballSideLength);

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(paddleColor.r, paddleColor.g, paddleColor.b);
    glVertex2f(x, y);
    glVertex2f(widthX, y);
    glVertex2f(widthX, lengthY);
    glVertex2f(x, y);
    glVertex2f(widthX, lengthY);
    glVertex2f(x, lengthY);
    glEnd();
}

void drawPaddle(){
    //Draws the player paddle and the AI paddle
    //The paddle is a rectangle with a width of paddleWidth and a length of paddleLength
    //Both paddles are white
    //The player paddle is on the right, the AI paddle is on the left
    //The paddles are placed at global.playerPaddlePosition and global.aiPaddlePosition

    //Similar logic to ball
    float playerX = pixelToScreenX(global.playerPaddlePosition.x);
    float playerY = pixelToScreenY(global.playerPaddlePosition.y);
    float aiX = pixelToScreenX(global.aiPaddlePosition.x);
    float aiY = pixelToScreenY(global.aiPaddlePosition.y);

    float playerWidthX = pixelToScreenX(global.playerPaddlePosition.x + paddleWidth);
    float playerLengthY = pixelToScreenY(global.playerPaddlePosition.y + paddleLength);
    float aiWidthX = pixelToScreenX(global.aiPaddlePosition.x + paddleWidth);
    float aiLengthY = pixelToScreenY(global.aiPaddlePosition.y + paddleLength);

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(paddleColor.r, paddleColor.g, paddleColor.b);
    glVertex2f(playerX, playerY);
    glVertex2f(playerWidthX, playerY);
    glVertex2f(playerWidthX, playerLengthY);
    glVertex2f(playerX, playerLengthY);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(paddleColor.r, paddleColor.g, paddleColor.b);
    glVertex2f(aiX, aiY);
    glVertex2f(aiWidthX, aiY);
    glVertex2f(aiWidthX, aiLengthY);
    glVertex2f(aiX, aiLengthY);
    glEnd();
}

void drawScore(){
    int coordX;
    int coordY;
    float x;
    float y;
    float widthX;
    float lengthY;
    //Draws the score for both the player and the AI
    //Player score is green, AI score is red
    //Player score is on the right, AI score is on the left
    //Score is drawn as a series of squares
    //The number of squares is equal to the score
    //The squares are drawn in a row, with the scoreGap between each square
    //The squares have a side length of scoreSize

    //nonlinear pixel func
    //player score goes LEFT (-X AXIS)
    for(int i=0; i<global.playerScore; i++){
        coordX = playerScorePosition.x - (i * (scoreSize + scoreGap));
        coordY = playerScorePosition.y;
        widthX = pixelToScreenX(coordX - scoreSize);
        lengthY = pixelToScreenY(coordY + scoreSize);
        x = pixelToScreenX(coordX);
        y = pixelToScreenY(coordY);

        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(0, 255, 0);
        glVertex2f(x,y);
        glVertex2f(widthX,y);
        glVertex2f(widthX,lengthY);
        glVertex2f(x,lengthY);
        glEnd();
    }

    //ai score goes RIGHT (+X AXIS)
    for (int i = 0; i < global.aiScore; i++) {
        coordX = aiScorePosition.x + (i * (scoreSize + scoreGap));
        coordY = aiScorePosition.y;
        widthX = pixelToScreenX(coordX + scoreSize);
        lengthY = pixelToScreenY(coordY + scoreSize);
        x = pixelToScreenX(coordX);
        y = pixelToScreenY(coordY);

        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(255, 0, 0);
        glVertex2f(x,y);
        glVertex2f(widthX,y);
        glVertex2f(widthX,lengthY);
        glVertex2f(x,lengthY);
        glEnd();
    }
}

void resetBall(){
    //EXAMPLE:
    //This is an example of how your assembly functions should look like.
    //You can use this as a template for your own code.
    //I've provided the original C code that I wrote, and the corresponding assembly code.
    //I recommend that you first write the C code and test if it works. Then you can convert it to assembly.

    //Resets the ball to the initial position and speed.
    //The ball will go in the opposite direction of the last player to score

    //C code:
    //global.ballPosition = initialBallPosition;
    //if (global.lastScore == 0){
    //    global.ballDirection = (Point) {-initialBallDirection.x, initialBallDirection.y};
    //}
    //else {
    //    global.ballDirection = initialBallDirection;
    //}
    //global.ballSpeed = initialBallSpeed;

    //Assembly code:
    //You should always use __volatile__ to prevent the compiler from incorrectly optimizing away your code.
    //__asm__ is the keyword to start writing assembly code.
    //GCC uses AT&T syntax, which is different from Intel syntax that we have been using so far.
    //The biggest changes are that the parameters are in reverse order.
    //Registers are prefixed with a % sign. (Two % in our case to avoid the compiler from interpreting it as a positional argument)
    //Integer literals are prefixed with a $ sign.
    //To refer to the memory pointed to by a register you must use the () syntax.
    //When a label is created, it can be referenced from anywhere in the code, so ensure your labels are unique.
    __asm__ __volatile__(
        //%0 is the first parameter, %1 is the second parameter, and so on.
        //The parameters start counting from the output parameters then to the input parameters.
        //So %5 in this case is the first input parameter(initialBallPosition.x).
        //And %0 is the first output parameter(global.ballPosition.x).
        //You must include the newline character at the end of each line.
            "mov %5, %0\n" //This is equivalent to global.ballPosition.x = initialBallPosition.x; in C.
            "mov %6, %1\n" // global.ballPosition.y = initialBallPosition.y;
            "cmp $0, %10\n" // if (global.lastScore == 0)
            "jne resetBallPlayer\n" // {
            "mov %7, %%eax\n" // eax = initialBallDirection.x
            "imul $-1, %%eax\n" // eax = -initialBallDirection.x
            "mov %%eax, %2\n" // global.ballDirection.x = -initialBallDirection.x;
            "mov %8, %3\n" // global.ballDirection.y = initialBallDirection.y;
            "jmp resetBallEnd\n" // }
            "resetBallPlayer:\n" // else {
            "mov %7, %2\n" // global.ballDirection.x = initialBallDirection.x;
            "mov %8, %3\n" // global.ballDirection.y = initialBallDirection.y;
            "resetBallEnd:\n" // }
            "mov %9, %4\n" // global.ballSpeed = initialBallSpeed;
            //An example of how to use the eax register, and integer literals.
            "mov $0, %%eax\n" //Now eax is 0
            : "=m" (global.ballPosition.x), "=m" (global.ballPosition.y), "=m" (global.ballDirection.x), "=m" (global.ballDirection.y), "=m" (global.ballSpeed)
        //Output parameters go here. Use "=r" for values stored in registers, use "=m" for values stored in memory
            : "r" (initialBallPosition.x), "r" (initialBallPosition.y), "r" (initialBallDirection.x), "r" (initialBallDirection.y), "r" (initialBallSpeed), "r" (global.lastScore)
        //Input parameters go here use "r" for values stored in registers, use "m" for values stored in memory
            : "eax"
        //You should list all the registers you use here, because they will be clobbered and the compiler has to know which ones to save
            );
}

void updateBall(){
    //Check if the ball collides with the edges of the screen, and check if it collides with the paddles. DONE
    //If the ball collides with the edges of the screen, it will add a point to the other player and reset the ball. DONE
    //If the ball collides with the paddles, it will change the x direction of the ball. DONE
    //If the ball collides with the top or bottom of the screen, it will change the y direction of the ball. DONE
    //The ball will also increase in speed every time it collides with the AI paddle. DONE
    //Update the ball position using the global.ballSpeed and global.ballDirection variables DONE
    //Make sure to update the global.lastScore variable to indicate who scored the last point DONE
    __asm__ __volatile__(
            "mov %4, %%eax\n"   //ballspeed
            "imul %2, %%eax\n"  // ballspeed * x_dir
            "add %%eax, %0\n"   // move horizontally

            "mov $10, %%eax\n"  //left wall
            "cmp %%eax, %0\n"
            "jl check_goal\n"    //collision

            "cmp %8, %0\n"  //right wall
            "jg check_goal\n"    //collision

            "mov %4, %%eax\n"
            "imul %3, %%eax\n"
            "add %%eax, %1\n"   //move vertically

            "mov $10, %%eax\n"
            "cmp %%eax, %1\n"   //upper wall
            "jl neg_y\n"    //collision

            "cmp %9, %1\n"  //lower wall
            "jg neg_y\n"    //collision

            "mov %0, %%eax\n"   //paddle left
            "add %10, %%eax\n"
            "cmp %11, %%eax\n"
            "jng left_paddle_check\n"

            "mov %11, %%eax\n"   //paddle right
            "add %13, %%eax\n"
            "cmp %%eax, %0\n"
            "jnl left_paddle_check\n"

            "mov %1, %%eax\n"   //paddle top
            "add %10, %%eax\n"
            "cmp %12, %%eax\n"
            "jng left_paddle_check\n"

            "mov %12, %%eax\n"   //paddle bottom
            "add %14, %%eax\n"
            "cmp %%eax, %1\n"
            "jnl left_paddle_check\n"

            //for sure in paddle range (hits from bottom or top will trickle :/)
            "jmp neg_x\n"

            "left_paddle_check:\n"  //ai paddle
            // same logic here as right paddle
            "mov %0, %%eax\n"
            "add %10, %%eax\n"
            "cmp %15, %%eax\n"
            "jng update_ball\n"

            "mov %15, %%eax\n"
            "add %13, %%eax\n"
            "cmp %%eax, %0\n"
            "jnl update_ball\n"

            "mov %1, %%eax\n"
            "add %10, %%eax\n"
            "cmp %16, %%eax\n"
            "jng update_ball\n"

            "mov %16, %%eax\n"
            "add %14, %%eax\n"
            "cmp %%eax, %1\n"
            "jnl update_ball\n"

            "add $1, %4\n" //ball speed up when ai hits
            "jmp neg_x\n"

            "neg_x:\n"  //negate ball direction x axis
            "neg %2\n"
            "jmp update_ball\n"

            "neg_y:\n"  //negate ball direction y axis
            "neg %3\n"
            "jmp update_ball\n"

            "check_goal:\n"
            "mov %1, %%eax\n"   //goal bottom
            "add %10, %%eax\n"
            "cmp %18, %%eax\n"
            "jge neg_x\n"   //goal post should negate direction no goal

            "cmp %17, %1\n" //goal top
            "jle neg_x\n"   //goal post should negate direction no goal

            "jmp award_goal\n"  //if inside the goal, award

            "award_goal:\n"
            "cmp $1, %2\n"      //if ball goes right, ai scored
            "je ai_score\n"

            "jmp player_score\n"    //else player scored

            "ai_score:\n"
            "add $1, %6\n"
            "mov $1, %7\n"
            "call resetBall\n"  //award points and reset the ball
            "jmp update_ball\n"

            "player_score:\n"
            "add $1, %5\n"
            "mov $0, %7\n"      //award points and reset the ball
            "call resetBall\n"
            "update_ball:\n"
            : "=m" (global.ballPosition.x), "=m" (global.ballPosition.y), "=m" (global.ballDirection.x), "=m" (global.ballDirection.y), "=m" (global.ballSpeed), "=m" (global.playerScore), "=m" (global.aiScore), "=m" (global.lastScore)
            : "r" (screenWidth-40), "r" (screenHeight-40), "r" (ballSideLength), "r" (global.playerPaddlePosition.x), "r" (global.playerPaddlePosition.y), "r" (paddleWidth), "r" (paddleLength), "r" (global.aiPaddlePosition.x), "r" (global.aiPaddlePosition.y), "r" ((int) (screenHeight / 2 - 1.5 * paddleLength)), "r" ((int) (screenHeight / 2 + 1.5 * paddleLength))
            : "eax"
            );
}

void updateAI(){
    //The AI is very simple, it just follows the ball on the Y axis only if the ball is on the left side of the screen
    //It moves at the speed set by the global.aiSpeed variable

    __asm__ __volatile__(
            "cmp %3, %1\n"      //stay still if ball in the right side of the pitch
            "jge end\n"

            "cmp %0, %2\n"      //move according to the ball's y
            "jl decrease_y\n"
            "jg increase_y\n"
            "jmp end\n"

            "decrease_y:\n"
            "sub %4, %0\n"  //decrease y by ai paddle speed
            "jmp end\n"

            "increase_y:\n"
            "add %4, %0\n"  //increase y by ai paddle speed
            "jmp end\n"

            "end:\n"
            : "=m" (global.aiPaddlePosition.y)
            : "r" (global.ballPosition.x), "r" (global.ballPosition.y), "r" (screenWidth/2), "r" (aiPaddleSpeed)
            : "eax"
            );
}

void gameLogic(){
    //The game is over when one of the players reaches 9 points otherwise call updateBall and updateAI
    //Make sure to update the global.gameOver variable
    __asm__ __volatile__(
            "mov $9, %%eax\n"   //check if game ended
            "cmp %1, %%eax\n"
            "je player_wins\n"

            "mov $9, %%eax\n"   //check if game ended
            "cmp %2, %%eax\n"
            "je ai_wins\n"

            "call updateBall\n"     //if didn't end continue calling updateball and ai
            "call updateAI\n"
            "jmp not_over\n"

            "player_wins:\n"
            //do smt                no instruction given in terms of celebration for player
            "jmp game_end\n"

            "ai_wins:\n"
            //do another thing      no instruction given in terms of celebration for ai
            "jmp game_end\n"

            "game_end:\n"
            "mov $1, %0\n"          //game_over = 1
            "not_over:\n"
            : "=m" (global.gameOver)
            : "r" (global.playerScore), "r" (global.aiScore)
            : "eax"
            );
}


void mouse(int x, int y){
    //The paddle is always centered on the mouse
    __asm__ __volatile__(
            "mov %1, %%eax\n"
            "sub %2, %%eax\n"   // player paddle middle
            "mov %%eax, %0\n"   //move with the mouse
            : "=m" (global.playerPaddlePosition.y)
            : "r" (y), "r" (paddleLength/2)
            : "%eax"
            );
}

void keyboard(unsigned char key, int x, int y){
    //Pressing 'r' resets the game if the game is over
    __asm__ __volatile__(
            "cmp $0, %0\n"
            "je no_reset\n" //when game continues do nothing

            "game_over:\n"
            "cmp $'r', %1\n"    //check if person hits r
            "je restart_game\n"

            "call exit\n"   //if not exit

            "restart_game:\n"
            "call initGlobals\n"    //initglobals = reset
            "no_reset:\n"
            : "=m" (global.gameOver)
            : "r" (key)
            : "eax"
            );
}

//helper function to draw/print characters in terms of strings to given coordinates
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawWalls();
    drawPaddle();
    drawBall();
    drawScore();
    if(global.gameOver){        //game over screen
        glColor3ub(255, 255, 255);
        renderBitmapString(-0.25f, 0.5f, GLUT_BITMAP_TIMES_ROMAN_24, "End of Game! Press any key to end or r to restart.");
    }
    glutSwapBuffers();
    glutPostRedisplay();
}

//callback function for intro screen to disappear
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) { //any key press closes window
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

//intro screen
int runintro() {
    // Vertex shader source for sphere
    const char* vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "    gl_Position = vec4(aPos, 1.0);\n"
                                     "}\0";

    // Fragment shader source for sphere
    const char* fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"    //orange
                                       "}\0";

    //skyline
    const char* fragmentShaderSource2 =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec3 skyColor = vec3(0.2, 0.4, 0.6);    \n" // Sky blue
            "    vec3 buildingColor = vec3(0.5, 0.5, 0.5);\n" // Building gray
            "\n"
            "    vec2 uv = gl_FragCoord.xy / vec2(1920.0, 1080.0);\n" //coords [0,1]
            "\n"
            "    if (uv.y >= 0.7) {\n"  //bottom majority is gray (building)
            "        FragColor = vec4(skyColor, 1.0);\n"
            "    } else {\n"
            "        FragColor = vec4(buildingColor, 1.0);\n"
            "    }\n"
            "}\n";
    //paddle (rectangle) colors
    const char* fragmentShaderSource3 = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    FragColor = vec4(0.5, 0.7, 1.0, 0.0);\n"
                                       "}\0";
    //check
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    //fullscreen
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "308 Pong Game", NULL, NULL);

    //check again
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // check glad's opengl funcs
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    // Create and compile the vertex shader for sphere
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader for sphere
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create and compile the fragment shader for skyline
    GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);

    // Create and compile the fragment shader for rectangles(paddles)
    GLuint fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
    glCompileShader(fragmentShader3);

    // Create the shader programs and link the shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLuint shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    GLuint shaderProgram3 = glCreateProgram();
    glAttachShader(shaderProgram3, fragmentShader3);
    glLinkProgram(shaderProgram3);

    // Clean up the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    glDeleteShader(fragmentShader3);

    //set a callback for leaving intro screen
    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        //skyline
        glUseProgram(shaderProgram2);

        // Render a full-screen blue/gray
        glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(3.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, 3.0f, 0.0f);
        glEnd();

        // sphere
        glUseProgram(shaderProgram);

        // Draw a sphere
        //https://stackoverflow.com/questions/10294345/texture-coordinates-for-rendering-a-3d-sphere
        //couple more sources, tried to combined the logic offered in solutions and 10 degree increments
        //used degrees
        //turned out okay-ish
        glBegin(GL_TRIANGLES);
        for (float lat = 0; lat <= 180; lat += 10) {    //latitude
            for (float lon = 0; lon < 360; lon += 10) { //longtitude
                float x1 = 0.5f * sin(lat * M_PI / 180) * cos(lon * M_PI / 180);
                float y1 = 0.5f * sin(lat * M_PI / 180) * sin(lon * M_PI / 180);
                float z1 = 0.5f * cos(lat * M_PI / 180);

                float x2 = 0.5f * sin((lat + 10) * M_PI / 180) * cos(lon * M_PI / 180);
                float y2 = 0.5f * sin((lat + 10) * M_PI / 180) * sin(lon * M_PI / 180);
                float z2 = 0.5f * cos((lat + 10) * M_PI / 180);

                float x3 = 0.5f * sin(lat * M_PI / 180) * cos((lon + 10) * M_PI / 180);
                float y3 = 0.5f * sin(lat * M_PI / 180) * sin((lon + 10) * M_PI / 180);
                float z3 = 0.5f * cos(lat * M_PI / 180);

                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
                glVertex3f(x3, y3, z3);

                //reverse order
                glVertex3f(x3, y3, z3);
                glVertex3f(x2, y2, z2);
                glVertex3f(x1, y1, z1);
            }
        }
        glEnd();

        //shader for 2 paddles (rectangles)
        //couldnt 3d completely
        //tried many cuboids none worked so used something similar to lecture slides
        glUseProgram(shaderProgram3);
        glBegin(GL_TRIANGLES);
        glVertex3f(-0.45f, 0.8f, -1.0f);
        glVertex3f(-0.15f, 0.8f, -1.0f);
        glVertex3f(-0.45f, 0.9f, -1.0f);

        glVertex3f(-0.15f, 0.8f, -1.0f);
        glVertex3f(-0.45f, 0.9f, -1.0f);
        glVertex3f(-0.15f, 0.9f, -1.0f);

        //second
        glVertex3f(0.2f, 0.8f, -1.0f);
        glVertex3f(0.5f, 0.8f, -1.0f);
        glVertex3f(0.2f, 0.9f, -1.0f);

        glVertex3f(0.5f, 0.8f, -1.0f);
        glVertex3f(0.2f, 0.9f, -1.0f);
        glVertex3f(0.5f, 0.9f, -1.0f);
        glEnd();

        //reset shader program use
        glUseProgram(0);

        //text render
        renderBitmapString(0.0f, 0.7f, GLUT_BITMAP_TIMES_ROMAN_24, "PONG");
        renderBitmapString(-0.07f, 0.6f, GLUT_BITMAP_TIMES_ROMAN_24, "Press any key to start!");

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);
    glDeleteProgram(shaderProgram3);
    glfwTerminate();
    return 0;
}

int main(int argc, char **argv)
{
    // Initialize GLUT and process user parameters
    glutInit(&argc, argv);
    //run intro
    runintro();
    initGlobals();

    // Request double buffered true color window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth,screenHeight);
    glutInitWindowPosition(0, 0);

    // Create window
    glutCreateWindow("COMP308 Pong");

    // Callback functions
    glutDisplayFunc(draw);
    glutIdleFunc(gameLogic);
    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyboard);

    // Pass control to GLUT for events
    glutMainLoop();
    return 0;
}