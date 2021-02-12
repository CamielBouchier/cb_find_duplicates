#...................................................................................................
# 
#  $BeginLicense$
#  $EndLicense$
# 
#...................................................................................................

CONFIG         -= debug_and_release
CONFIG         += debug
CONFIG         -= release
CONFIG         += console
TEMPLATE        = app
TARGET          = cb_find_duplicates
DEPENDPATH     += .
DEPENDPATH     += UI
DEPENDPATH     += sources
INCLUDEPATH    += .
INCLUDEPATH    += sources
DESTDIR        += .
OBJECTS_DIR     = objects
MOC_DIR         = moc
UI_HEADERS_DIR  = ui/include
RCC_DIR         = resources
RESOURCES       = cb_find_duplicates.qrc

#
QT += widgets

# Start values : taken from configure
QMAKE_CXXFLAGS_RELEASE     += -g -O2
QMAKE_CXXFLAGS_DEBUG       += -g -O2
QMAKE_CFLAGS_RELEASE       += -g -O2
QMAKE_CFLAGS_DEBUG         += -g -O2
LIBS                       += -lgomp -lpthread -lintl -lregex -lexpat -ltiff -lpng -ljpeg -lm 

# OpenMP
QMAKE_CXXFLAGS_RELEASE     += -fopenmp
QMAKE_CXXFLAGS_DEBUG       += -fopenmp
QMAKE_CFLAGS_RELEASE       += 
QMAKE_CFLAGS_DEBUG         += 
LIBS                       += 

# OpenMP Correction. Magick (fi) might have put them on
# while our configure test decided it will not work.
QMAKE_CXXFLAGS_RELEASE     -= 
QMAKE_CXXFLAGS_DEBUG       -= 
QMAKE_CFLAGS_RELEASE       -= 
QMAKE_CFLAGS_DEBUG         -= 

# In windows the ntohs/htons family is in winsock.
win32 {
  LIBS += -lwsock32
}

# Input
HEADERS += $$system(ls sources/*.h)

FORMS +=   $$system(ls ui/*.ui)

SOURCES += $$system(ls sources/*.cpp | grep -v dlCreateSomeCurves | grep -v dlCreateAdobeProfiles)

#..................................................................................................

# vim: ts=4 sw=4 sts=4 sr et columns=100
