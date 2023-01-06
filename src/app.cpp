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
float bgAdjust = 0.0f, mixVal = 0.5f, scaleGlobal = 1.0f, rotateGlobal = -55.0f;
vec3 Loc(0.0f, 0.0f, 0.0f);
bool rotateBool = false, scaleBool = false, backgroundBool = true;

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
    glfwSetFramebufferSizeCallback(newWindow, frameResizeCallback);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // setting draw to wireframe mode


    Shader shader("shaders/vertex", "shaders/fragment");

    // ********************************* VERTEX AND INDEX ARRAYS ********************************* //

    float vertices[] = { // defining vertices to be used for drawing
    -0.2f, -0.2f, 0.0f,
    0.2f, -0.2f, 0.0f,
    0.0f, 0.0f, 0.0f,
    -0.2f, 0.2f, 0.0f,
    0.2f, 0.2f, 0.0f
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

    // ********************************* RENDER LOOP ********************************* //

    while (!glfwWindowShouldClose(newWindow)) {
        processInput(newWindow);
        // rendering here

        // ********************************* CALCULATIONS ********************************* //

        float timeValue = glfwGetTime(); // built in function to retrieve current time
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // greenValue will change value according to sin function (in waves)
        float alternate = (sin(timeValue) / 4.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "theColor"); // finding the location of our uniform theColor from shaderProgram

        // ********************************* TRANSFORMATIONS ********************************* //
        /*
        mat4 trans = mat4(1.0f); // creating 4x4 matrix with 1 on diagonal (identity matrix)
        trans = translate(trans, Loc);
        trans = scale(trans, vec3(sin(timeValue), sin(timeValue), sin(timeValue)));
        trans = rotate(trans, timeValue, vec3(0.0f, 0.0f, 1.0f));
        unsigned int transformLocation = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(trans));
        */

        mat4 modelMatrix = mat4(1.0f), viewMatrix = mat4(1.0f), projectionMatrix = mat4(1.0f), translateMatrix = mat4(1.0f);

        viewMatrix = translate(viewMatrix, vec3(0.0f, 0.0f, -3.0f)); // translating 3.0f away in the z-axis
        projectionMatrix = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);
        modelMatrix = rotate(modelMatrix, radians(rotateGlobal), vec3(0.5f, 0.5f, 0.0f)); // rotate identity matrix along the x-axis by -55.0 degrees
        translateMatrix = translate(translateMatrix, Loc);

        int modelLoc = glGetUniformLocation(shader.ID, "model"), 
            viewLoc = glGetUniformLocation(shader.ID, "view"),
            projectionLoc = glGetUniformLocation(shader.ID, "perspective"),
            translateLoc = glGetUniformLocation(shader.ID, "transform");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projectionMatrix));
        glUniformMatrix4fv(translateLoc, 1, GL_FALSE, value_ptr(translateMatrix));

        // ********************************* COLOR CHANGES ********************************* //

        if (backgroundBool) {
            glClearColor(greenValue, alternate, greenValue, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // ********************************* RUNNING PROGRAM ********************************* //

        shader.use();
        shader.setFloat("mixValue", mixVal);
        glBindVertexArray(VAO);
        //glUniform4f(vertexColorLocation, 1 - alternate, alternate, 0.0f, 1.0f); // setting the value of the uniform vertexColorLocation every frame
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*
        trans = mat4(1.0f);
        trans = translate(trans, vec3(-0.5, 0.5, 0));
        trans = scale(trans, vec3(sin(timeValue), sin(timeValue), sin(timeValue)));
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &trans[0][0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (Loc.y < 1.0f) {
            Loc.y += 0.005f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (Loc.y > -1.0f) {
            Loc.y -= 0.005f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (Loc.x > -1.0f) {
            Loc.x -= 0.005f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (Loc.x < 1.0f) {
            Loc.x += 0.005f;
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
        if (mixVal > 0.0f) {
            mixVal -= 0.005f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (mixVal < 1.0f) {
            mixVal += 0.005f;
        }
    }
}