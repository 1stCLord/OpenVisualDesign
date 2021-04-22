#include "UserInterface.h"
#include "CallablesWindow.h"
#include "DefinitionsWindow.h"
#include "ExecutionLane.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl2.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

namespace OVD
{
    UserInterface::UserInterface()
    {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        window = SDL_CreateWindow("OpenVisualDesign", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.window_size.x, config.window_size.y, window_flags);
        gl_context = SDL_GL_CreateContext(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL2_Init();

        //windows.push_back(std::make_unique<CallablesWindow>(this));
        windows.push_back(std::make_unique<ExecutionLane>(this));
        windows.push_back(std::make_unique<DefinitionsWindow>(this));
    }

    UserInterface::~UserInterface()
    {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void UserInterface::update()
    {
        ImGuiIO& io = ImGui::GetIO();
        config.window_size = io.DisplaySize;
        payload_accepted = false;
    }

    void UserInterface::run()
    {
        while (true)
        {
            update();
            if (poll())return;
            render();
        }
    }

    bool UserInterface::poll()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                return true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                return true;
        }
        return false;
    }

    void UserInterface::render()
    {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::SetNextWindowPos({ 0, 0 });
            ImGui::SetNextWindowSize({ config.window_size.x, config.window_size.y });
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, config.window_padding);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, config.panel_colour.Value);

            bool open;
            ImGui::Begin("OVD", &open, flags);

            for (std::unique_ptr<Window> &window : windows)
                window->render();

            bool demo = true;
            ImGui::ShowDemoWindow(&demo);

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();

            ImGui::End();
        }

        ImGui::Render();
        glViewport(0, 0, (int)config.window_size.x, (int)config.window_size.y);
        glClearColor(config.window_colour.Value.x, config.window_colour.Value.y, config.window_colour.Value.z, config.window_colour.Value.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    int UserInterface::Config::calc_borders(int window_count)
    {
        return (((window_count * 2) + 1) * border);
    }
    ImVec2 UserInterface::Config::calc_fill(ImVec2 position)
    {
        return { (window_size.x - position.x) - border, (window_size.y - position.y) - border };
    }
}