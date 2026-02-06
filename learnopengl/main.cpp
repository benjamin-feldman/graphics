#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    std::cout << "Resizing to " << width << "*" << height << std::endl;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// shader sources
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *orangeFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *yellowFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";



int main() {
    // boilerplate initialization for GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for macOS:
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // SHADER COMPILATION
    // we store the vertex shader ID and create it using glad
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Bind the shader source to the OpenGL shader
    // Second arg is how many strings we are passing
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check that the shader source compiles properly
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Similar for the fragment shaders
    unsigned int orangeFragmentShader;
    orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
    glCompileShader(orangeFragmentShader);

    glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::ORANGE_FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int yellowFragmentShader;
    yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
    glCompileShader(yellowFragmentShader);

    glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::YELLOW_FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create the shader program, that will be used when rendering objects
    unsigned int orangeShaderProgram;
    orangeShaderProgram = glCreateProgram();
    // Attach then link the above compiled shaders to the shaderProgram`
    // glAttachShader adds compiled shader stages to a program, while glLinkProgram validates
    // and combines all attached stages into one runnable GPU program.
    glAttachShader(orangeShaderProgram, vertexShader);
    glAttachShader(orangeShaderProgram, orangeFragmentShader);
    glLinkProgram(orangeShaderProgram);

    glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(orangeShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::ORANGE_PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create the shader program, that will be used when rendering objects
    unsigned int yellowShaderProgram;
    yellowShaderProgram = glCreateProgram();
    // Attach then link the above compiled shaders to the shaderProgram`
    // glAttachShader adds compiled shader stages to a program, while glLinkProgram validates
    // and combines all attached stages into one runnable GPU program.
    glAttachShader(yellowShaderProgram, vertexShader);
    glAttachShader(yellowShaderProgram, yellowFragmentShader);
    glLinkProgram(yellowShaderProgram);

    glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::YELLOW_PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(orangeFragmentShader);
    glDeleteShader(yellowFragmentShader);

    // VERTEX DATA AND BUFFERS
    // All vertices must live in [-1, 1]^3, aka Normalized Device Coordinates (NDC). The viewport will later transform those
    // into the screen-space coordinates.
    // NB: interesting that this is a contiguous array of floating point numbers (with a "stride" of 3, i.e. a group of 3 floats
    //  corresponds to a single point), and not an array of tuple/vec3
    float triangleVertices[] = {
        // original triangle
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // Create the VAO to store vertex input state
    // A VAO records which vertex attributes are enabled, how they are laid out (size, type, stride, offsets), and which VBO/EBO they reference.
    // Without VAOs, switching between multiple objects or vertex layouts would require rebinding buffers and redefining all vertex attribute
    // pointers each time. VAOs allow us to switch between different vertex data and attribute configurations with a single bind call.
    unsigned int VAO_OrangeTriangle;
    glGenVertexArrays(1, &VAO_OrangeTriangle);
    // Vertex Buffer Objects: memory that lives in the GPU.
    unsigned int VBO_OrangeTriangle;
    // Generate 1 buffer and store its id in the VBO integer
    glGenBuffers(1, &VBO_OrangeTriangle);
    // Bind the VAO first
    glBindVertexArray(VAO_OrangeTriangle);
    // Then bind the vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO_OrangeTriangle);
    // Then configure the buffer data
    // GL_STATIC_DRAW means that the data is set once, and used many times. Other possible values are:
    // GL_STEAM_DRAW: data set once, used by the GPU at most a few times
    // GL_DYNAMIC_DRAW: data is changed a lot and used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // At this point: we sent the input vertex data to the GPU, and instructed the GPU on how it should
    // process this data using vertex and fragment shaders.
    // We still have to inform the GPU how it should connect the vertex data (currently: an array of floats) to
    // vertex shader's attributes (the vec3 aPos).
    // Basically, we need to tell which part of our input vertex data goes to which vertex attribute in the
    // vertex shader.
    // Each vertex is (32-bit float, 32-bit float, 32-bit float), which means our vertex input data is
    // a tightly packed array. Also, the first value in the data is the beginning of the buffer.


    // 1st param: Which vertex attribute we are configuring here. Earlier, we set location=0 which means
    // that the position attribute is 0. It's like an ID for a vertex shader variable/param,
    // (e.g. we could have location=1 for the color attribute)
    // 2nd param: size of the vertex attribute (here, 3 values)
    // 3rd param: Type of data
    // 4th param: do we want the data to be normalized? Not in this case.
    // 5th param: stride, i.e. the space between consecutive vertex attributes. NB: we have tightly packed
    // values, so we could have set this to 0 and let OpenGL determine the stride based on the data size (since there's
    // nothing else in between, the stride == data size).
    // 6h param: Offset for the beginning of the position data in the buffer. Weird (void*) cast, not relevant for now
    // The VBO managing the position data being still bound, it's now associated with the vertex attribute 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // unbind the GL_ARRAY_BUFFER from the VBO (since it's now inside the VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Defensive: unbind the active VAO, so that other VAO won't modify this VAO (rare)
    glBindVertexArray(0);

    // addition (combination of exercise 2 and 3): use a second VAO/VBO and EBO to draw a
    // yellow rectangle in the top right corner.

    float rectangleVertices[] = {
        0.9f, 0.9f, 0.0f, // bottom left
        0.9f, 1.0f, 0.0f, // top left
        1.0f, 1.0f, 0.0f, // top right
        1.0f, 0.9f, 0.0f // bottom right
    };

    unsigned int rectangleIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO_YellowRectangle;
    glGenVertexArrays(1, &VAO_YellowRectangle);

    unsigned int VBO_YellowRectangle;
    glGenBuffers(1, &VBO_YellowRectangle);

    unsigned int EBO_YellowRectangle;
    glGenBuffers(1, &EBO_YellowRectangle);

    glBindVertexArray(VAO_YellowRectangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_YellowRectangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_YellowRectangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(orangeShaderProgram);
        glBindVertexArray(VAO_OrangeTriangle);
        // We use the GL_TRIANGLES primitive. It tells OpenGL that each group of 3 vertices form an independent
        // triangle, and should be drawn as such.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // unbind the VAO: defensive
        glBindVertexArray(0);

        glUseProgram(yellowShaderProgram);
        glBindVertexArray(VAO_YellowRectangle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO_OrangeTriangle);
    glDeleteBuffers(1, &VBO_OrangeTriangle);
    glDeleteProgram(orangeShaderProgram);

    glDeleteVertexArrays(1, &VAO_YellowRectangle);
    glDeleteBuffers(1, &VBO_YellowRectangle);
    glDeleteBuffers(1, &EBO_YellowRectangle);
    glDeleteProgram(yellowShaderProgram);

    glfwTerminate();

    return 0;
}
