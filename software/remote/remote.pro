QT += qml quick websockets quickcontrols2
CONFIG += c++11 disable-desktop

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS += \
    sources/jsonfile.h \
    sources/launcher.h \
    sources/display_control.h \
    sources/translation.h

SOURCES += \
        sources/main.cpp \
    sources/jsonfile.cpp \
    sources/launcher.cpp \
    sources/display_control.cpp

RESOURCES += qml.qrc \
    images.qrc \
    translations.qrc

# TRANSLATION
lupdate_only{
SOURCES = main.qml \
          MainContainer.qml \
          StandbyControl.qml \
          basic_ui/*.qml \
          basic_ui/main_navigation/*.qml \
          basic_ui/settings/*.qml \
          components/light/*.qml \
          integrations/*.qml \
}
TRANSLATIONS = translations/bg_BG.ts \
               translations/cs_CZ.ts \
               translations/da_DK.ts \
               translations/de_DE.ts \
               translations/el_GR.ts \
               translations/en_US.ts \
               translations/es_ES.ts \
               translations/et_EE.ts \
               translations/fi_FI.ts \
               translations/fr_CA.ts \
               translations/fr_FR.ts \
               translations/ga_IE.ts \
               translations/hr_HR.ts \
               translations/hu_HU.ts \
               translations/it_IT.ts \
               translations/lt_LT.ts \
               translations/lv_LV.ts \
               translations/mt_MT.ts \
               translations/nl_NL.ts \
               translations/no_NO.ts \
               translations/pl_PL.ts \
               translations/pt_BR.ts \
               translations/pt_PT.ts \
               translations/ro_RO.ts \
               translations/sk_SK.ts \
               translations/sl_SI.ts \
               translations/sv_SE.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

# Copy config file to output directory
copyConfig.commands = $(COPY_DIR) $$shell_path($$PWD/config.json) $$OUT_PWD

# Copy translation file to output directory
copyTranslations.commands = $(COPY_DIR) $$shell_path($$PWD/translations.json) $$OUT_PWD

# Add copy commands to build output
first.depends = $(first) copyConfig copyTranslations
export(first.depends)
export(copyConfig.commands)
export(copyTranslations.commands)
QMAKE_EXTRA_TARGETS += first copyConfig copyTranslations