#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"

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

    // Instantiating and binding the textures
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // wrapping/filtering options, for the currently bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // Loading our image for the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);

    // Generating the texture using the image data
    // 1st param: which texture are we targeting (eg if we also had a 3D texture
    // currently bound, then it would not be affected)
    // 2nd param: mipmap level
    // 3rd param: storage format
    // 4-5th param: setting the resulting texture's dimensions
    // 6th param: always 0 (legacy...)
    // 7-8th param: format and datatype of source image
    // 9th param: the image data
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // wrapping/filtering options, for the currently bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);

    // Using RGBA since the image is a PNG with alpha bg
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Shaders
    Shader ourShader("shaders/shader_texture.vert", "shaders/shader_texture.frag");

    // VERTEX DATA AND BUFFERS
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
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

    unsigned int EBO;
    glGenBuffers(1, &EBO);


    // Bind the VAO first
    glBindVertexArray(VAO);
    // Then bind the vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Then configure the buffer data
    // GL_STATIC_DRAW means that the data is set once, and used many times. Other possible values are:
    // GL_STREAM_DRAW: data set once, used by the GPU at most a few times
    // GL_DYNAMIC_DRAW: data is changed a lot and used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind the GL_ARRAY_BUFFER from the VBO (since it's now inside the VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Defensive: unbind the active VAO, so that other VAO won't modify this VAO (rare)
    glBindVertexArray(0);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    float t = 0.0f;
    float mixAmount;

    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();

        // mixAmount = cos(t/100.0);
        mixAmount = 0.3;
        ourShader.setFloat("mixAmount", mixAmount);
        t += 1;

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
