#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <iostream>
#include <shader.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;

void frameResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processMouseInput(GLFWwindow* window, double xPos, double yPos);


float bgAdjust = 0.0f, mixVal = 0.0f, scaleGlobal = 1.0f, rotateGlobal = -35.0f; 
float timeDiff = 0.0f, lastTime = 0.0f;
vec3 Loc(0.0f, 0.0f, 0.0f);
vec3 camPos = vec3(0.0f, 0.0f, 6.0f), camUp = vec3(0.0f, 1.0f, 0.0f), camFront = vec3(0.0f, 0.0f, -1.0f);
bool rotateBool = false, scaleBool = false, backgroundBool = false, firstMouse = true;
float theYaw = -90.0f, thePitch = 0.0f, mouseX = 400.0f, mouseY = 400.0f;

int main()
{
    glfwInit(); // initializing window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // specifying specific version + using core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* newWindow = glfwCreateWindow(800, 800, "Texture Rendering", NULL, NULL); // creating new window of width 800 and height 800

    if (newWindow == NULL) { // if window failed to create, display message to screen and terminate
        cout << "Failed to create window." << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(newWindow); // sets context to window we have just created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // initializing GLAD, and displaying message if failed
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 800); // setting viewport (can be smaller than our window)
    glfwSetFramebufferSizeCallback(newWindow, frameResizeCallback); // setting frame resize to our created function
    glfwSetCursorPosCallback(newWindow, processMouseInput); // setting cursor processing to our created function
    glfwSetInputMode(newWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // making mouse invisible, unable to move out of frame
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // setting draw to wireframe mode


    Shader shader("shaders/vertex", "shaders/fragment");

    // ********************************* VERTEX AND INDEX ARRAYS ********************************* //

    float crosshair[] = { // defining vertices to be used for drawing
    -0.02f, -0.05f, 0.0f,
    -0.02f, 0.05f, 0.0f,
    0.02f, -0.05f, 0.0f,

    -0.02f, 0.05f, 0.0f,
    0.02f, 0.05f, 0.0f,
    0.02f, -0.05f, 0.0f,

    -0.05f, -0.02f, 0.0f,
    -0.05f, 0.02f, 0.0f,
    0.05f, 0.02f, 0.0f,

    0.05f, 0.02f, 0.0f,
    0.05f, -0.02f, 0.0f,
    -0.05f, -0.02f, 0.0f,
    };

    float vertices2[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    /*
    float vertices2[] = {
    -0.2f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f
    - 0.2f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.2f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };
    */

    float vertices3[] = {
        -0.4f, -0.8f, 0.0f,
        0.0f, -0.8f, 0.0f,
        0.0f, -0.4f, 0.0f
    };

    unsigned int indices[] = { // defining indices for drawing with EBO
        0, 1, 3,
        1, 2, 3
    };

    unsigned int indices2[] = {
        0, 1, 3,
        1, 2, 3
    };

    vec3 cubePos[] = {
        vec3(0.0f, 0.0f, 0.0f),
        vec3(2.4f, 0.6f, -2.0f),
        vec3(-1.8f, 2.6f, 1.0f),
        vec3(0.6f, -2.6f, -10.0f)
    };

    // ********************************* VERTEX INFORMATION AND CREATION ********************************* //

    unsigned int VAO; // initializing VAO object (holds attributes)
    unsigned int VBO; // initializing vertex buffer
    unsigned int EBO; // initializing element buffer object

    glGenBuffers(1, &EBO); // generating element buffer object
    glGenBuffers(1, &VBO); // generating vertex buffer
    glGenVertexArrays(1, &VAO); // generating VAO vertex attributes

    glBindVertexArray(VAO); // binding VAO to current object
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding buffers to the array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // binding element buffer object to element array

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0); 
    glEnableVertexAttribArray(0);
    /*
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    */
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    // ********************************* TEXTURE LOADING ********************************* //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // setting mirrored texture repeating in the S (X) axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1); // generating texture object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1); // binding so texture commands will make changes to desired texture object

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // flip loaded textures on y-axis

    unsigned char* data = stbi_load("src/container.jpg", &width, &height, &nrChannels, 0);
    // loading image and storing in const char array data
    if (!data) {
        cout << "TEXTURE::LOAD::FAILED\n";
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps for currently bound texture 
    }
    stbi_image_free(data); // free memory allocated to store image 

    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    data = stbi_load("src/awesomeface.png", &width, &height, &nrChannels, 0);
    if (!data) {
        cout << "TEXTURE2::LOAD::FAILED\n";
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    shader.setInt("texture2", 1);

    // ********************************* TRANSFORMATIONS ********************************* //

    glEnable(GL_DEPTH_TEST);
    /*
    vec3 camPos = vec3(0.0f, 0.0f, 3.0f); // setting position of camera
    vec3 camTarget = vec3(0.0f, 0.0f, 0.0f); // setting position of target (location to be looked at)
    vec3 camDirection = normalize(camPos - camTarget); // creating direction vector (points in positive z-direction)
    vec3 up = vec3(0.0f, 1.0f, 0.0f); // global direction of up
    vec3 camRight = normalize(cross(up, camDirection)); // points in positive x-direction
    camUp = cross(camDirection, camRight); // points in positive y-direction
    */
    // ********************************* RENDER LOOP ********************************* //

    while (!glfwWindowShouldClose(newWindow)) {
        processInput(newWindow);
        // rendering here

        // ********************************* CALCULATIONS ********************************* //

        float timeValue = glfwGetTime(); // built in function to retrieve current time
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // greenValue will change value according to sin function (in waves)
        float alternate = (sin(timeValue) / 4.0f) + 0.5f;
        timeDiff = timeValue - lastTime;
        lastTime = timeValue;

        // ********************************* TRANSFORMATIONS ********************************* //
        /*
        mat4 trans = mat4(1.0f); // creating 4x4 matrix with 1 on diagonal (identity matrix)
        trans = translate(trans, Loc);
        trans = scale(trans, vec3(sin(timeValue), sin(timeValue), sin(timeValue)));
        trans = rotate(trans, timeValue, vec3(0.0f, 0.0f, 1.0f));
        unsigned int transformLocation = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(trans));
        */

        mat4 viewMatrix = mat4(1.0f), projectionMatrix = mat4(1.0f), translateMatrix = mat4(1.0f);

        float radius = 10.0f, camX = sin(timeValue) * radius, camZ = cos(timeValue) * radius;
        viewMatrix = lookAt(camPos, camPos + camFront, camUp);
        projectionMatrix = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);
        translateMatrix = translate(translateMatrix, Loc);
        translateMatrix = scale(translateMatrix, vec3(scaleGlobal, scaleGlobal, scaleGlobal));

        int modelLoc = glGetUniformLocation(shader.ID, "model"), 
            viewLoc = glGetUniformLocation(shader.ID, "view"),
            projectionLoc = glGetUniformLocation(shader.ID, "perspective"),
            translateLoc = glGetUniformLocation(shader.ID, "transform");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projectionMatrix));
        glUniformMatrix4fv(translateLoc, 1, GL_FALSE, value_ptr(translateMatrix));

        // ********************************* COLOR CHANGES ********************************* //

        if (backgroundBool) {
            glClearColor(greenValue, alternate, greenValue, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else {
            glClearColor(1.0f, 0.8f, 0.84f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // ********************************* RUNNING PROGRAM ********************************* //

        shader.use();
        shader.setFloat("mixValue", mixVal);
        //glBindVertexArray(VAO);
        for (int i = 0; i < 4; ++i) {
            mat4 modelMatrix = mat4(1.0f);
            modelMatrix = translate(modelMatrix, cubePos[i]);
            float angle = 15.0 * i + 5.0;
            modelMatrix = rotate(modelMatrix, angle, vec3(1.0f, 0.5f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /*
        glBindVertexArray(VAO); // binding VAO to current object
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding buffers to the array
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // binding element buffer object to element array

        glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair), crosshair, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        */

        // ********************************* POST-RENDER ********************************* //

        // rendering end
        glfwSwapBuffers(newWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void frameResizeCallback(GLFWwindow* window, int width, int height) { // for resizing display screen
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) { // continually called to check if user has pressed ESC, in which case we exit
    const float camSpeed = 5.0f * timeDiff;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camPos += camFront * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camPos -= camFront * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camPos -= normalize(cross(camFront, camUp)) * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camPos += normalize(cross(camFront, camUp)) * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camPos.y += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camPos.y -= 0.01f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        rotateGlobal -= 0.4f;
        /*
        if (rotateGlobal > -90.f) {
            rotateGlobal -= 0.5f;
        }
        */
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        rotateGlobal += 0.4f;
        /*
        if (rotateGlobal < 0.0f) {
            rotateGlobal += 0.5f;
        }
        */
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (scaleGlobal > 0.0f) {
            scaleGlobal -= 0.001f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (scaleGlobal < 1.0f) {
            scaleGlobal += 0.001f;
        }
    }
}

void processMouseInput(GLFWwindow* window, double xPos, double yPos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        const float sens = 0.1f;

        if (firstMouse == true) {
            mouseX = xPos;
            mouseY = yPos;
            firstMouse = false;
        }

        float xChange = xPos - mouseX, yChange = yPos - mouseY;
        mouseX = xPos, mouseY = yPos;
        xChange *= sens, yChange *= sens;
        theYaw += xChange, thePitch -= yChange;

        if (thePitch > 89.0f) thePitch = 89.0f; // restricting vertical mouse movement
        if (thePitch < -89.0f) thePitch = -89.0f;

        vec3 direction;
        direction.x = cos(radians(theYaw)) * cos(radians(thePitch));
        direction.y = sin(radians(thePitch));
        direction.z = sin(radians(theYaw)) * cos(radians(thePitch));
        camFront = normalize(direction);
    }
}