#include <render/gl4/render_gl4.hpp>

//
// VOLT
// Copyright © 2025 starfrost
//
// gl4_core.cpp: Core GL4 renderer functions - initialisation, main loop, shader stuff, and shutdown.
//

namespace Volt
{
    /* Globals */
    RendererStateGL4 render_state_gl4;

    std::unordered_map<uint32_t, uint32_t> volt_formats_to_gl_formats =
    {
        { TextureFormat::RGBA32, GL_RGBA32F }, // GL_RGBA == GL_RGBA32F
    };

    /* Functions only used in this translation unit */
    void R_GL4_SetViewportSize(GLFWwindow* window, int32_t width, int32_t height);
    void R_GL4_OnFullscreenChanged();
    void R_GL4_Close(GLFWwindow* window);

    // TEMP CODE
    float clear_r = 0.0f; 
    float clear_g = 0.0f; 
    float clear_b = 0.0f; 
    float clear_a = 1.0f; 
    float clear_frame_number = 0;

    void R_GL4_Init_SetupFunctions()
    {
        renderer_state_global.Shader_CompileFunction = R_GL4_CompileShader;
        renderer_state_global.Shader_UseFunction = R_GL4_UseShader;
        renderer_state_global.Shader_FreeFunction = R_GL4_FreeShader;
        renderer_state_global.Texture_CreateFunction = R_GL4_CreateTexture;
    }

    // Initialises the GL4 renderer
    // Failure to initialise is a FATAL ERROR.
    void R_GL4_Init()
    {
        Logging_LogChannel("******** R_GL4_Init ********", LogChannel::Debug);

        // initialise glfw
        if (!glfwInit())
            Logging_LogChannel("GL4 Renderer failed to initialise: Failed to initialise GLFW. Can only run with +set renderer none.", LogChannel::Fatal);
        else
            Logging_LogChannel("GLFW %s initialised", LogChannel::Debug, glfwGetVersionString());

        // OpenGL 4.3, Core Profile. We need compute shaders and works on all consumer GPUs from the last 15 years.
        // Mac will need metal (modern OpenGL on mac sucks anyway)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // set up width, height and window title
        // if fullscreen, ignore the size and just get the current video mode

        Cvar_SetOnSetCallback(render_fullscreen, R_GL4_OnFullscreenChanged);

        // create the window and store the primary monitor
        render_state_gl4.window = glfwCreateWindow(int(render_width->value), int(render_height->value),
        render_window_title->string, NULL, NULL);
        render_state_gl4.monitor_primary = glfwGetPrimaryMonitor();

        glfwMakeContextCurrent(render_state_gl4.window);
        glfwSetFramebufferSizeCallback(render_state_gl4.window, R_GL4_SetViewportSize);
        glfwSetWindowCloseCallback(render_state_gl4.window, R_GL4_Close);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            Logging_LogChannel("GL4 Renderer failed to initialise. gladLoadGLLoader failed. Can only run with +set renderer none.", LogChannel::Fatal);

        // Make sure that we actually got OpenGL 4.3
        GLint gl_major, gl_minor;

        glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
        glGetIntegerv(GL_MINOR_VERSION, &gl_minor);

        if (gl_major < 4
        || (gl_major == 4) && (gl_minor < 3))
        {
            Logging_LogChannel("Your graphics hardware or drivers are not OpenGL 4.3 compliant.\n"
                "OpenGL 4.3 is required due to the use of compute shaders in Volt. In general, most dedicated graphics hardware released in 2010 or later, "
                ", and integrated graphics after 2013 have OpenGL 4.3 compliance.\n\n"
                "- If you are using a Mac, Metal support is not available yet, and macOS does not support beyond OpenGL 4.1. Sorry. Blame Apple.\n"
                "- If you are using official drivers on an NVIDIA GPU, only Fermi architecture (GTX 400 series) and later GPUs are supported.\n"
                "- If you are using official drivers on an AMD GPU, only TeraScale 2 (HD 5000) architecture and later GPUs are supported.\n"
                "- If you are using official drivers on an Intel GPU, only units made after 2013 (starting with HD 4x00) are supported.\n"
                "If you have a supported GPU, try updating your graphics drivers.\n"
                "If you do not, try using unofficial drivers on Linux like Mesa3D. Otherwise, get a newer graphics card.\n\n"
                "OpenGL version provided: %d.%d", 
                LogChannel::Fatal, gl_major, gl_minor);
            return;
        }

        Logging_LogChannel("OpenGL Version %s", LogChannel::Debug, glGetString(GL_VERSION));

        // Set up function pointers for the rest of the engine
        R_GL4_Init_SetupFunctions();

        // At this point it's safe to do rendering stuff, so set fullscreen if it's set
        R_GL4_OnFullscreenChanged();

        // set the initial viewport
        glViewport(0, 0, int(render_width->value), int(render_height->value));

        // turn off vsync
        glfwSwapInterval(0);

        Logging_LogChannel("******** R_GL4_Init was successful! ********", LogChannel::Message);

    }

    #define BUF_SIZE    10 // max int32 size is 10

    /* Called when the size of the framebuffer changes. */
    void R_GL4_SetViewportSize(GLFWwindow* window, int32_t width, int32_t height)
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
    void R_GL4_OnFullscreenChanged()
    {
        if (render_fullscreen->value == 0)
        {
            Logging_LogChannel("Exiting borderless fullscreen", LogChannel::Debug);

            // restore decorations
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

            // leave fullscreen
            glfwSetWindowSize(render_state_gl4.window, int(render_width->value), int(render_height->value));

            // restore the window position
            glfwSetWindowPos(render_state_gl4.window, render_state_gl4.last_window_pos_x, render_state_gl4.last_window_pos_y);
        }
        else 
        {
            Logging_LogChannel("Entering borderless fullscreen", LogChannel::Debug);

            // remember the old window position
            glfwGetWindowPos(render_state_gl4.window, &render_state_gl4.last_window_pos_x, &render_state_gl4.last_window_pos_y);

            // In glfw fullscreen works by setting the window size to teh current window omde
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

            /* We have to set every part of the mode, otherwise the taskbar is still visible */
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
            
            // this kills the window position
            glfwSetWindowMonitor(render_state_gl4.window, monitor, 0, 0, video_mode->width, video_mode->height, video_mode->refreshRate); 
           
        }
    }

    // Called on each frame to update the state of the wporld.
    void R_GL4_Frame()
    {
        switch (render_state_gl4.task)
        {
            case RendererStateGL4Task::Spin:
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
                glfwSwapBuffers(render_state_gl4.window);

                clear_frame_number++; 
        }

    }

    bool R_GL4_CompileSingleShader(VoltShaderSet* set, VoltShaderType type)
    {
        auto shader_type = GL_VERTEX_SHADER;
        VoltShader& target = set->vertex;
        int32_t successful;

        if (type == VoltShaderType::Fragment)
        {
            shader_type = GL_FRAGMENT_SHADER;
            target = set->fragment;
        }
        if (type == VoltShaderType::Geometry)
        {
            shader_type = GL_GEOMETRY_SHADER;
            target = set->geometry;
        }
        if (type == VoltShaderType::Compute)
        {
            shader_type = GL_COMPUTE_SHADER;
            target = set->compute;
        }

        auto shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &target.code, nullptr);
        glCompileShader(shader);

        target.id = shader; //check

        // See if the shader was successfully compiled!
        glGetShaderiv(shader, GL_COMPILE_STATUS, &successful);

        if (!successful)
        {
            char info_log[MAX_STRING_LENGTH] = {0};
            glGetShaderInfoLog(shader, MAX_STRING_LENGTH, nullptr, info_log);

            Logging_LogChannel("******** Shader compilation failure. ********\nInformation: %s", LogChannel::Fatal, info_log);
            return false;
        }

        target.loaded = true; 
        return true;
    }

    bool R_GL4_CompileShader(VoltShaderSet* set)
    {
        // copy in the name of the file

        bool successful = true; 

        if (set->vertex.code)
            successful = R_GL4_CompileSingleShader(set, VoltShaderType::Vertex);
    
        if (!successful)
            return false;

        if (set->fragment.code)
            successful = R_GL4_CompileSingleShader(set, VoltShaderType::Fragment);

        if (!successful)
            return false;

        if (set->geometry.code)
            successful = R_GL4_CompileSingleShader(set, VoltShaderType::Geometry);

        if (!successful)
            return false;
    
        if (set->compute.code)
            successful = R_GL4_CompileSingleShader(set, VoltShaderType::Compute);

        if (!successful)
            return false;

        // Now, link the shaders into a shader program
        set->program_id = glCreateProgram();

        if (set->vertex.loaded)
            glAttachShader(set->program_id, set->vertex.id);

        if (set->fragment.loaded)
            glAttachShader(set->program_id, set->fragment.id);
        
        if (set->geometry.loaded)
            glAttachShader(set->program_id, set->geometry.id);
    
        if (set->compute.loaded)
            glAttachShader(set->program_id, set->compute.id);

        glLinkProgram(set->program_id);
        int32_t program_compile_successful, program_link_successful;
 
        // See if the program was successful.
        glGetProgramiv(set->program_id, GL_COMPILE_STATUS, &program_compile_successful);
        glGetProgramiv(set->program_id, GL_LINK_STATUS, &program_link_successful);

        char info_log[MAX_STRING_LENGTH] = {0};

        if (!program_compile_successful)
        {
            glGetProgramInfoLog(set->program_id, MAX_STRING_LENGTH, nullptr, info_log);
            Logging_LogChannel("******** Shader program compilation failure. ********\nInformation: %s", LogChannel::Error, info_log);
            return false;
        }

        if (!program_link_successful)
        {
            glGetProgramInfoLog(set->program_id, MAX_STRING_LENGTH, nullptr, info_log);
            Logging_LogChannel("******** Shader program linking failure. ********\nInformation: %s", LogChannel::Error, info_log);
            return false;
        }

        // Once you have linked the program, you can delete the original loose shaders since they will be contained as part of the shader program
        if (set->vertex.loaded)
        {
            glDeleteShader(set->vertex.id);
            set->vertex.id = 0;   
        }

        if (set->fragment.loaded)
        {
            glDeleteShader(set->fragment.id);
            set->fragment.id = 0;   
        }

        if (set->geometry.loaded)
        {
            glDeleteShader(set->geometry.id);
            set->geometry.id = 0;   
        }
    
        if (set->compute.loaded)
        {
            glDeleteShader(set->compute.id);
            set->compute.id = 0;   
        }

        return true; 
    }

    void R_GL4_UseShader(VoltShaderSet* set)
    {
        glUseProgram(set->program_id);
    }

    bool R_GL4_FreeShader(VoltShaderSet* set)
    {
        glDeleteProgram(set->program_id);
        return true; 
    }

    void R_GL4_CreateTexture(Texture* texture)
    {
        glGenTextures(1, &texture->id);

        glBindTexture(GL_TEXTURE_2D, texture->id);
        glTexImage2D(GL_TEXTURE_2D, 0, volt_formats_to_gl_formats[texture->format], 
        texture->size.x, texture->size.y, 0, volt_formats_to_gl_formats[texture->format], GL_UNSIGNED_BYTE, (void*)texture->pixels);
    }

    void R_GL4_Close(GLFWwindow* window)
    {
        // renderer_state_global.running is set to false in render_core.cpp
        // because the renderer exists, we know this_client exists
        Common_Shutdown();
    }

    // Shuts down the renderer
    void R_GL4_Shutdown()
    {
        // destroy the window
        glfwDestroyWindow(render_state_gl4.window);
        render_state_gl4.window = nullptr;
        glfwTerminate();
    }
}
