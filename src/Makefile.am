## Process this file with automake to produce Makefile.in

INCLUDES = \
@evas_cflags@ \
@ecore_cflags@ \
@edje_cflags@ \
@emotion_cflags@

AM_CFLAGS = \
-Wall \
-O1

bin_PROGRAMS = calaos_bootsplash

calaos_bootsplash_SOURCES = \
main.cpp \
EApplication.h EApplication.cpp \
EdjeObject.h EdjeObject.cpp \
MainWindow.h MainWindow.cpp \
Utils.h Utils.cpp \
callbacks.h callbacks.cpp

calaos_bootsplash_LDADD = \
@evas_libs@ \
@ecore_libs@ \
@edje_libs@ \
@emotion_libs@
