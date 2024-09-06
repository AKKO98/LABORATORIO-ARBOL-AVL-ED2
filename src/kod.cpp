#include <iostream>
#include <windows.h>
#include <filesystem>
#include "AVLTree.hpp"
#include "Pelicula.h"
#include "Helper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#define IMGUI_DEFINE_MATH_OPERATOR
//#define ENTEROS

GLuint LoadTextureFromFile(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;  // Devuelve 0 si no se pudo cargar
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        (channels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);  // Liberar los datos de imagen después de cargar la textura

    return textureID;  // Devuelve el GLuint
}

void paletaDeColores() {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.61f, 0.49f, 0.67f, 0.26f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.89f, 0.64f, 0.76f, 0.59f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.89f, 0.64f, 0.76f, 0.78f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.62f, 0.60f, 0.61f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.89f, 0.64f, 0.76f, 0.68f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_Tab] = ImVec4(0.63f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabSelected] = ImVec4(0.86f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TabDimmed] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

static GLuint arbol = 0;

void UpdateTexture(const char* filename)
{
    // Liberar la textura anterior si existe
    if (arbol != 0) {
        glDeleteTextures(1, &arbol);
        arbol = 0;
    }

    // Cargar la nueva textura
    arbol = LoadTextureFromFile(filename);
}

void Render(bool& imagen, bool& imagenCambiada, std::string& path)
{
    ImGui::Begin("Imagen", &imagen);

    if (imagenCambiada) {
        UpdateTexture(path.c_str());
        imagenCambiada = false;  // Resetear el flag después de actualizar
    }

    if (arbol != 0) {
        ImGui::Image((void*)(intptr_t)arbol, ImVec2(ImGui::GetWindowWidth() * .98f, ImGui::GetWindowHeight() * .97f));
    }
    else {
        ImGui::Text("Error loading texture");
    }

    ImGui::End();
}

void prueba(AVLTree<Pelicula>& p) {
    std::vector<std::string> ns = { "Mission to Mars",  "Final Destination", "The Family Man", "Space Cowboys", "Remember the Titans", "Big Momma's House",
                                    "The Emperor's New Groove", "Autumn in New York", "Bring It On", "The Beach", "Coyote Ugly", "Billy Elliot", "Romeo Must Die"
                                    };
    for (auto n : ns)
    {
        p.insert(*Helper::buscarPeliculaPorTitulo("dataset_movies.csv", n));
        
    }
    p.display("Arbol.png");
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int principal() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Laboratorio ED2 AVL TREE", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    paletaDeColores();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool menu = true;
    bool imagen = true;
    bool change = true;
    bool background = true;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    AVLTree<Pelicula> tree;
    std::filesystem::path directorioEjecutable = std::filesystem::current_path();
    std::string path = directorioEjecutable.string().append("\\dataset_movies.csv");
    std::string imagenPaht = directorioEjecutable.string().append("\\Arbol.png");
    std::vector<Pelicula> table;
    static std::string console;

    //prueba(tree);

    // Main loop
    while (!glfwWindowShouldClose(window))

    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            //ImGui_ImplGlfw_Sleep(10);
            //continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //prueba(tree);

        ImGui::Begin("Menu", &menu);
        ImGui::Text("Add, Search or Delete a Node");
        static char addTitle[34] = ""; ImGui::InputText(" ", addTitle, IM_ARRAYSIZE(addTitle));

        if (ImGui::Button(" Add "))
        {
            std::unique_ptr<Pelicula> nodo = Helper::buscarPeliculaPorTitulo(path, addTitle);
            if (nodo != nullptr)
            {
                tree.insert(*nodo.get());
                tree.display(imagenPaht);
                change = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(" Delete "))
        {
            std::unique_ptr<Pelicula> nodo = Helper::buscarPeliculaPorTitulo(path, addTitle);
            if (nodo != nullptr)
            {
                tree.erase(*nodo.get());
                tree.display(imagenPaht);
                change = true;
                console = "Nodo eliminado con exito.";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button(" Buscar Nodo "))
        {
            table.clear();
            auto nodo = Helper::getNodeByName(tree, addTitle);
            if (nodo != nullptr)
            {
                table.push_back(nodo->value);
            }
            else
            {
                console = "El nodo no se encuentra en el arbol";
            }
        }


        ImGui::SameLine();
        if (ImGui::Button(" Buscar padre del nodo "))
        {
            table.clear();
            auto nodo = Helper::findParentByTitle(tree, addTitle);
            if (nodo != nullptr)
            {
                table.push_back(nodo->value);
            }
            else
            {
                console = "El nodo no se encuentra en el arbol o no tiene padre (es la raiz).";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button(" Buscar abuelo del nodo "))
        {
            table.clear();
            auto nodo = Helper::findGrandParentByTitle(tree, addTitle);
            if (nodo != nullptr)
            {
                table.push_back(nodo->value);
            }
            else
            {
                console = "El nodo no se encuentra en el arbol o no tiene abuelo (Es hijo de la raiz).";
            }
        }


        if (ImGui::Button(" Buscar tio del nodo "))
        {
            table.clear();
            auto nodo = Helper::findUncleByTitle(tree, addTitle);
            if (nodo != nullptr)
            {
                table.push_back(nodo->value);
            }
            else
            {
                console = "El nodo no se encuentra en el arbol o no tiene tio.";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("BFS"))
        {
            console = "";
            auto l = tree.bfsRecursive();
            for (size_t i = 0; i < l.size(); i++)
            {
                console = console + l[i]->value.getTitle();
                if (i != l.size() - 1)
                {
                    console = console + " - ";
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button(" Nivel de un nodo "))
        {
            console = "";
            auto nivel = Helper::getLevelByName(tree, addTitle);
            if (nivel != -1)
            {
                console = "El nivel del nodo es : " + std::to_string(nivel);
            }
            else
            {
                console = "El nodo no se encuentra en el arbol.";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button(" Balanceo "))
        {
            console = "";
            auto balance = Helper::getBalanceByName(tree, addTitle);
            if (balance != -1)
            {
                console = "El factor de balanceo del nodo es : " + std::to_string(balance);
            }
            else
            {
                console = "El nodo no se encuentra en el arbol.";
            }
        }

        for (size_t i = 0; i < 15; i++)
            ImGui::Spacing();

        static char year[256], FE[256];
        // Crear campos de entrada de texto para el usuario
        ImGui::InputText(" <-- Year", year, IM_ARRAYSIZE(year));
        ImGui::InputText(" <-- Foreing Earning", FE, IM_ARRAYSIZE(FE));

        for (size_t i = 0; i < 5; i++)
            ImGui::Spacing();


        if (ImGui::Button(" Busqueda por atributos "))
        {
            table.clear();
            table = Helper::searchWithCriteria(tree, std::stoi(year), std::stod(FE));
            if (table.empty())
            {
                console = "No hay nodos que cumplan con esos criterios de busqueda.";
            }
        }

        for (size_t i = 0; i < 25; i++)
            ImGui::Spacing();


        ImGui::Text(" Informacion de los Nodos");
        // Tabla
        ImGui::BeginTable("Datos: ", 7, ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInner);
        ImGui::TableNextColumn(); ImGui::Text("Year");
        ImGui::TableNextColumn(); ImGui::Text("Title");
        ImGui::TableNextColumn(); ImGui::Text("Foreign Earnings");
        ImGui::TableNextColumn(); ImGui::Text("Domestic Earnings");
        ImGui::TableNextColumn(); ImGui::Text("Worldwide Earnings");
        ImGui::TableNextColumn(); ImGui::Text("Foreign Percent Earnings");
        ImGui::TableNextColumn(); ImGui::Text("Domestic Percent Earnings");

        if (!table.empty())
        {
            for (auto t : table)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text(std::to_string(t.getYear()).c_str());
                ImGui::TableNextColumn(); ImGui::Text(t.getTitle().c_str());
                ImGui::TableNextColumn(); ImGui::Text(std::to_string(t.getFE()).c_str());
                ImGui::TableNextColumn(); ImGui::Text(std::to_string(t.getDE()).c_str());
                ImGui::TableNextColumn(); ImGui::Text(std::to_string(t.getWWE()).c_str());
                ImGui::TableNextColumn(); ImGui::Text(std::to_string(t.getFPE()).c_str());
                ImGui::TableNextColumn(); ImGui::Text(std::to_string(t.getDPE()).c_str());
            }
        }
        ImGui::EndTable(); //Final de la tabla

        ImGui::End();

        Render(imagen, change, imagenPaht);

        ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);



        // Cambia el texto programáticamente cuando sea necesario
        const size_t size = console.size() + 1;
        char* copia = new char[strlen(console.c_str()) + 1];
        strcpy_s(copia, console.size() + 1, console.c_str());

        // Mostrar la caja de texto de solo lectura
        ImGui::PushTextWrapPos(ImGui::GetWindowWidth() * 0.98f);
        ImGui::InputTextMultiline(" ", copia, IM_ARRAYSIZE(copia),
            ImVec2(ImGui::GetWindowWidth() * 0.98f, ImGui::GetWindowHeight() * 0.85f),
            ImGuiInputTextFlags_AllowTabInput);
        ImGui::PopTextWrapPos();
        ImGui::End();


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

int enteros() {
    std::filesystem::path directorioEjecutable = std::filesystem::current_path();
    std::string imagenPaht = directorioEjecutable.string().append("\\Arbol.png");
    AVLTree<int> arbol;

    std::vector<int> numeros = {122, 130, 115, 7, 2, 9, 16, 29, 40, 28, 101, 24, 55, 65, 78, 83, 104, 105, 70, 90, 85, 95, 88, 93, 117, 147, 8, 5};

    for (size_t i = 0; i < numeros.size(); i++)
    {
        arbol.insert(numeros[i]);
        arbol.display(imagenPaht);
        std::cout << "Numero insertado es: " << numeros[i] << std::endl;
        std::cin.get();
    }

    return 0;
}

// Main code
int main(int, char**)
{
#ifdef ENTEROS
    return enteros();
#else
    return principal();
#endif // !ENTEROS
    
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return principal();
}