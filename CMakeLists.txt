cmake_minimum_required(VERSION 3.21)

project(MindGraph VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.5)


set_source_files_properties(qml/components/Theme.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_executable(MindGraphApp
    main.cpp

    include/Note.h
    include/DatabaseManager.h
    include/NotesModel.h
    include/GraphController.h
    include/Edge.h
    include/EdgesModel.h

    src/DatabaseManager.cpp
    src/NotesModel.cpp
    src/GraphController.cpp
    src/EdgesModel.cpp
)

target_include_directories(MindGraphApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

qt_add_qml_module(MindGraphApp
    URI MindGraph
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/pages/GraphPage.qml
        qml/pages/NoteEditorPage.qml
        qml/components/GraphNode.qml
        qml/components/AppButton.qml
        qml/components/AppText.qml
        qml/components/Theme.qml
        qml/components/AppTextField.qml
        qml/components/AppTextArea.qml
        qml/components/GraphEdge.qml
)

target_link_libraries(MindGraphApp
    PRIVATE Qt6::Quick Qt6::Sql
)
