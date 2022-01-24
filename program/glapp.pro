QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#LIBS += -lGLU
#LIBS +=  -lglew32 -lopengl32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    frame.cpp \
    glslprogram.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    picker.cpp \
    texture.cpp \
    widget.cpp \
    proctree.cpp

HEADERS += \
    camera.h \
    frame.h \
    gldebug.h \
    glmath.h \
    glslprogram.h \
    json.hpp \
    mainwindow.h \
    mesh.h \
    picker.h \
    texture.h \
    widget.h \
    proctree.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    file.json

RESOURCES += \
    Res.qrc

FORMS += \
    mainwindow.ui
