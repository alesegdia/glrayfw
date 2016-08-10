HEADERS += \
    $$PWD/glrayfw/app/sdlglapp.h \
    $$PWD/glrayfw/core/core.h \
    $$PWD/glrayfw/core/dynamicarray.h \
    $$PWD/glrayfw/core/fpscounter.h \
    $$PWD/glrayfw/core/matrix2d.h \
    $$PWD/glrayfw/core/random.h \
    $$PWD/glrayfw/core/transform.h \
    $$PWD/glrayfw/core/unordereddynamicarray.h \
    $$PWD/glrayfw/entity/controller/entitycontroller.h \
    $$PWD/glrayfw/entity/entity.h \
    $$PWD/glrayfw/entity/entitymanager.h \
    $$PWD/glrayfw/entity/pawn.h \
    $$PWD/glrayfw/map/map.h \
    $$PWD/glrayfw/physics/layers.h \
    $$PWD/glrayfw/physics/physics.h \
    $$PWD/glrayfw/render/animation.h \
    $$PWD/glrayfw/render/animationsystem.h \
    $$PWD/glrayfw/render/Bitmap.h \
    $$PWD/glrayfw/render/block.h \
    $$PWD/glrayfw/render/camera.h \
    $$PWD/glrayfw/render/context.h \
    $$PWD/glrayfw/render/font.h \
    $$PWD/glrayfw/render/glcorearb.h \
    $$PWD/glrayfw/render/mesh.h \
    $$PWD/glrayfw/render/plane.h \
    $$PWD/glrayfw/render/program.h \
    $$PWD/glrayfw/render/quad.h \
    $$PWD/glrayfw/render/renderer.h \
    $$PWD/glrayfw/render/sdlcontext.h \
    $$PWD/glrayfw/render/shader.h \
    $$PWD/glrayfw/render/sprite3d.h \
    $$PWD/glrayfw/render/stb_image.h \
    $$PWD/glrayfw/render/Texture.h

SOURCES += \
    $$PWD/glrayfw/app/sdlglapp.cpp \
    $$PWD/glrayfw/core/transform.cpp \
    $$PWD/glrayfw/entity/controller/entitycontroller.cpp \
    $$PWD/glrayfw/entity/entity.cpp \
    $$PWD/glrayfw/entity/entitymanager.cpp \
    $$PWD/glrayfw/physics/physics.cpp \
    $$PWD/glrayfw/render/animation.cpp \
    $$PWD/glrayfw/render/Bitmap.cpp \
    $$PWD/glrayfw/render/block.cpp \
    $$PWD/glrayfw/render/camera.cpp \
    $$PWD/glrayfw/render/context.cpp \
    $$PWD/glrayfw/render/sdlcontext.cpp \
    $$PWD/glrayfw/render/shader.cpp \
    $$PWD/glrayfw/render/Texture.cpp \
    $$PWD/glrayfw/render/font.cpp

INCLUDEPATH += $$PWD \
				/usr/include/freetype2
DEPENDPATH += $$PWD

LIBS += -lBox2D -lfreetype -lSDL2 -lGL
