#include <render/gl3/render_gl3.hpp>

//
// gl3_core.cpp
// Core GL3 renderer functions - initialisation, main loop, and shutdown.
//
namespace Volt
{
    /* Globals */
    GL3RendererState gl3_render_state;

    /* Functions only used in this translation unit */
    void R_GL3_SetViewportSize(GLFWwindow* window, int32_t width, int32_t height);
    void R_GL3_OnFullscreenChanged();
    void R_GL3_Close(GLFWwindow* window);

    // TEMP CODE
    float clear_r = 0.0f; 
    float clear_g = 0.0f; 
    float clear_b = 0.0f; 
    float clear_a = 1.0f; 
    float clear_frame_number = 0;

    // Initialises the GL3 renderer
    void R_GL3_Init()
    {
        Logging_LogChannel("******** R_GL3_Init ********", LogChannel::Debug);

        // initialise glfw
        if (!glfwInit())
            Logging_LogChannel("GL3 Renderer failed to initialise: Failed to initialise GLFW. Can only run with +set renderer none.", LogChannel::Fatal);
        else
            Logging_LogChannel("GLFW %s initialised", LogChannel::Debug, glfwGetVersionString());

        // OpenGL 3.3, Core Profile - very compatible
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // set up width, height and window title
        // if fullscreen, ignore the size and just get the current video mode

        Cvar_SetOnSetCallback(render_fullscreen, R_GL3_OnFullscreenChanged);

        // create the window and store the primary monitor
        gl3_render_state.window = glfwCreateWindow(int(render_width->value), int(render_height->value),
        render_window_title->string, NULL, NULL);
        gl3_render_state.monitor_primary = glfwGetPrimaryMonitor();

        glfwMakeContextCurrent(gl3_render_state.window);
        glfwSetFramebufferSizeCallback(gl3_render_state.window, R_GL3_SetViewportSize);
        glfwSetWindowCloseCallback(gl3_render_state.window, R_GL3_Close);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            Logging_LogChannel("GL3 Renderer failed to initialise. gladLoadGLLoader failed. Can only run with +set renderer none.", LogChannel::Fatal);

        // At this point it's safe to do rendering stuff, so set fullscreen if it's set
        R_GL3_OnFullscreenChanged();

        // set the initial viewport
        glViewport(0, 0, int(render_width->value), int(render_height->value));

        // turn off vsync
        glfwSwapInterval(0);

        Logging_LogChannel("******** R_GL3_Init was successful! ********", LogChannel::Message);
    }

    #define BUF_SIZE    10 // max int32 size is 10

    /* Called when the size of the framebuffer changes. */
    void R_GL3_SetViewportSize(GLFWwindow* window, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height);

        // bit of a hack - we need to do this natively in the ConVar system
        char buf[BUF_SIZE] = {0};

        // set the cvars appropriately
        snprintf(buf, BUF_SIZE, "%d", width);
        Cvar_Set("render_width", buf, true);
        //clear out the buffer so parts of the width don't remain in a cas elike 1024x768
        memset(buf, 0x00, strlen(buf));
        snprintf(buf, BUF_SIZE, "%d", height); 
        Cvar_Set("render_height", buf, true);

        Logging_LogChannel("Render size is now %dx%d", LogChannel::Debug, width, height);
    }

    // Called when render_fullscreen cvar changed
    void R_GL3_OnFullscreenChanged()
    {
        if (render_fullscreen->value == 0)
        {
            Logging_LogChannel("Exiting borderless fullscreen", LogChannel::Debug);

            // restore decorations
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

            // leave fullscreen
            glfwSetWindowSize(gl3_render_state.window, int(render_width->value), int(render_height->value));

            // restore the window position
            glfwSetWindowPos(gl3_render_state.window, gl3_render_state.last_window_pos_x, gl3_render_state.last_window_pos_y);
        }
        else 
        {
            Logging_LogChannel("Entering borderless fullscreen", LogChannel::Debug);

            // remember the old window position
            glfwGetWindowPos(gl3_render_state.window, &gl3_render_state.last_window_pos_x, &gl3_render_state.last_window_pos_y);

            // In glfw fullscreen works by setting the window size to teh current window omde
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

            /* We have to set every part of the mode, otherwise the taskbar is still visible */
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
            
            // this kills the window position
            glfwSetWindowMonitor(gl3_render_state.window, monitor, 0, 0, video_mode->width, video_mode->height, video_mode->refreshRate); 
           
        }
    }

    // Called on each frame to update the state of the wporld.
    void R_GL3_Frame()
    {

        /* THE START - GET INPUT FROM THE INPUT SUBSYSTEM */

        // test code
        float frame_number = float(clear_frame_number) / 1048576.0f;

        clear_r = cos(frame_number * (180/M_PI));
        clear_g = sin(frame_number * (180/M_PI));
        clear_b = clear_r + clear_g;

        glClearColor(clear_r, clear_g, clear_b, clear_a);

        glClear(GL_COLOR_BUFFER_BIT);

        /* THE END OF THE RENDERER LOOP */
        glfwPollEvents(); 
        glfwSwapBuffers(gl3_render_state.window);

        clear_frame_number++; 
    }

    void R_GL3_Close(GLFWwindow* window)
    {
        // renderer_running is set to false in render_core.cpp
        // because the renderer exists, we know this_client exists
        //emulation.initialised = false
    }

    // Shuts down the renderer
    void R_GL3_Shutdown()
    {
        // destroy the window
        glfwDestroyWindow(gl3_render_state.window);
        gl3_render_state.window = nullptr;
        glfwTerminate();
    }
}
