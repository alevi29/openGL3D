/*
const char* vertexShaderSource = "#version 330 core\n" // information for the vertex shader (edges)
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 theColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   theColor = aColor;\n"
"}\0";



const char* fragmentShaderSource1 = "#version 330 core\n" // information for the fragment shader (color)
"out vec4 FragColor;\n"
"in vec3 theColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(theColor, 1.0);\n"
"}\0";



const char* fragmentShaderSource2 = "#version 330 core\n" // information for the fragment shader (color)
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\0";
*/

// **SHADER CREATION**

/*
unsigned int vertexShader; // initializing a vertex shader variable
vertexShader = glCreateShader(GL_VERTEX_SHADER); // creating the shader
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // giving source of shader (defined above)
glCompileShader(vertexShader); // compiling shader


unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
glCompileShader(fragmentShader);

unsigned int shaderProgram; // initializing program to compile all shaders
shaderProgram = glCreateProgram(); // creating program
glAttachShader(shaderProgram, vertexShader); // attaching vertex shader to program
glAttachShader(shaderProgram, fragmentShader); // attaching fragment shader to program
glLinkProgram(shaderProgram); // linking program
*/