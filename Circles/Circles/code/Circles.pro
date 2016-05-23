#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T19:36:08
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Circles

TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

RC_ICONS = icon.ico

RESOURCES += \
    images.qrc \
    fonts.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
