/**
 * rocky c++
 * Copyright 2023 Pelican Mapping
 * MIT License
 */
#include <rocky_vsg/Application.h>

#if !defined(ROCKY_SUPPORTS_TMS)
#error This example requires TMS support. Check CMake.
#endif

#include <rocky/TMSImageLayer.h>
#include <rocky/TMSElevationLayer.h>

#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>

template<class T>
int layerError(T layer)
{
    rocky::Log::warn() << "Problem with layer \"" <<
        layer->name() << "\" : " << layer->status().message << std::endl;
    return -1;
}


auto mainGUI = [&]()
{
    if (ImGui::Begin("Welcome to Rocky."))
    {
        ImGui::Text("Testing.");
        ImGui::End();
    }
    return true;
};


int main(int argc, char** argv)
{
    // instantiate the application engine.
    rocky::Application app(argc, argv);

    // add an imagery layer to the map
    auto layer = rocky::TMSImageLayer::create();
    layer->setURI("https://readymap.org/readymap/tiles/1.0.0/7/");
    app.map()->layers().add(layer);

    // check for error
    if (layer->status().failed())
        return layerError(layer);

    // start up the gui
    app.createMainWindow(1920, 1080);
    app.viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());
    app.mainScene->addChild(vsgImGui::RenderImGui::create(app.mainWindow, mainGUI));

    // run until the user quits.
    return app.run();
}