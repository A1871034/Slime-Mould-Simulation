#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "core.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shaders.h"
#include "Program.h"
#include "Renderer.h"
#include "Mould.h"

#define RENDER_X 540
#define RENDER_Y 540

#define WINDOW_X 1440
#define WINDOW_Y 1440

int main(void)
{
    GLFWwindow* window;

    // Enable GLFW Debugging Output
    glfwSetErrorCallback(GLFWErrorCallback);

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    // Use GLFW Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, "Slime Mould", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Synchronises with vsync ie monitor refresh rate, enable vsync
    glfwSwapInterval(0);
    
    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    /* Initialize GLEW and ensure it is working*/
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }
    fprintf(stdout, "Status: Using GLEW %s and OpenGL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION)); 

    // Enable OpenGL Debugging Output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC) GLErrorCallback, nullptr);

    float triangle_positions[] = {
         0.0f,                         0.0f,
         0.0f,             (float) RENDER_Y,
         (float) RENDER_X, (float) RENDER_Y,
         (float) RENDER_X,             0.0f
    };

    unsigned int indicies[] = {
        0, 1, 2,
        0, 2, 3,     
    };

    // Set blend mode for alpha channel
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Create Vertex Array object
    VertexArray va;
    va.bind();

    // Vertex Buffer
    VertexBuffer vb(&triangle_positions, 4 * 2 * sizeof(float), GL_STATIC_DRAW);
    // Links this vertex buffer with the vertex array object and describes attributes
    VertexBufferLayout layout;
    layout.push<float>(2);
    va.AddBuffer(vb, layout);

    // Index Buffer
    // is also stored state in the vertex array object
    IndexBuffer ib(indicies, 6);

    // Projection matrix
    glm::mat4 proj = glm::ortho(0.0f, (float) RENDER_X, 0.0f, (float) RENDER_Y);

    // Set up VS and FS program
    Shaders shaders("res/shaders/basic.glsl");

    Program program;
    program.attach_shader(shaders.get_vs());
    program.attach_shader(shaders.get_fs());
    program.link();
    program.bind();

    program.set_uniform_mat4f("u_MVP", proj);

    // Set up Compute Shader program
    Program compute;
    compute.attach_shader(shaders.get_cs());
    compute.link();
    compute.bind();

    // Particle / Mould Initialisation
    unsigned int num_particles = 40824/2;
    Mould mould(RENDER_X, RENDER_Y);

    std::cout << "Generating Mould";
    mould.gen_random_mould(num_particles);
    std::cout << ": Done" << std::endl;
    
    SSBO particles(5, mould.get_particles(), (num_particles) * sizeof(particle), GL_STATIC_COPY);
    SSBO trails[2] = {
        SSBO(3, mould.get_trails(), (RENDER_X) * (RENDER_Y) * sizeof(GLfloat), GL_STATIC_COPY),
        SSBO(4, nullptr, (RENDER_X) * (RENDER_Y) * sizeof(GLfloat), GL_STATIC_COPY)
    };
    bool swap_buffers = false;

    // Initialise Renderer
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        // Call compute shader to handle particles
        renderer.Compute(compute, particles, trails, swap_buffers);

        // Call vertex fragment shaders
        // will also apply the blur / decay
        renderer.Draw(va, ib, program, trails, swap_buffers);
        
        // ImGui Stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui Gui
        {
            //ImGui::SliderFloat("Colour max distance", &(distance_ratios_texOverride[0]), 0.0f, 2.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        // ImGui Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        swap_buffers = !swap_buffers;

        /* Poll for and process events */
        glfwPollEvents();
    }

    

    // ImGui Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}