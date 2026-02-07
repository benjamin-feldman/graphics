#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

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

    // Shaders
    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

    // VERTEX DATA AND BUFFERS
    // All vertices must live in [-1, 1]^3, aka Normalized Device Coordinates (NDC). The viewport will later transform those
    // into the screen-space coordinates.
    // NB: interesting that this is a contiguous array of floating point numbers (with a "stride" of 3, i.e. a group of 3 floats
    //  corresponds to a single point), and not an array of tuple/vec3
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    // Create the VAO to store vertex input state
    // A VAO records which vertex attributes are enabled, how they are laid out (size, type, stride, offsets), and which VBO/EBO they reference.
    // Without VAOs, switching between multiple objects or vertex layouts would require rebinding buffers and redefining all vertex attribute
    // pointers each time. VAOs allow us to switch between different vertex data and attribute configurations with a single bind call.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Vertex Buffer Objects: memory that lives in the GPU.
    unsigned int VBO;
    // Generate 1 buffer and store its id in the VBO integer
    glGenBuffers(1, &VBO);
    // Bind the VAO first
    glBindVertexArray(VAO);
    // Then bind the vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Then configure the buffer data
    // GL_STATIC_DRAW means that the data is set once, and used many times. Other possible values are:
    // GL_STEAM_DRAW: data set once, used by the GPU at most a few times
    // GL_DYNAMIC_DRAW: data is changed a lot and used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind the GL_ARRAY_BUFFER from the VBO (since it's now inside the VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Defensive: unbind the active VAO, so that other VAO won't modify this VAO (rare)
    glBindVertexArray(0);

    float i = 0.0f;

    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float xOffset = i;

        ourShader.use();
        ourShader.setFloat("xOffset", xOffset);

        i += .01f;
        i = i > 1.0f ? 0.0f : i;

        glBindVertexArray(VAO);
        // We use the GL_TRIANGLES primitive. It tells OpenGL that each group of 3 vertices form an independent
        // triangle, and should be drawn as such.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // unbind the VAO: defensive
        glBindVertexArray(0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
