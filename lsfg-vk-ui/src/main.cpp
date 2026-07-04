/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <QIcon>
#include <QColor>
#include <QGuiApplication>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QUrl>

#include "backend.hpp"

using namespace lsfgvk::ui;

// Qt's xdgdesktopportal platform theme on Qt 6.4 populates Window/WindowText
// (from KDE/GNOME color scheme) but leaves Button/ButtonText/Text roles at
// Qt's built-in Fusion defaults (light grey / dark), which look wrong on a
// dark window. Labels in QML using palette.windowText render fine — mirror
// that role into ButtonText/Text so Fusion controls read the same source.
// Button face is derived from Window so it contrasts against the window bg.
static void normalizePalette() {
    QPalette p = QGuiApplication::palette();
    const QColor window = p.color(QPalette::Window);
    const QColor windowText = p.color(QPalette::WindowText);
    const bool isDark = window.lightness() < 128;

    p.setColor(QPalette::Button, isDark ? window.lighter(125) : window.darker(108));
    p.setColor(QPalette::ButtonText, windowText);
    p.setColor(QPalette::Base, isDark ? window.darker(115) : window.lighter(105));
    p.setColor(QPalette::Text, windowText);
    p.setColor(QPalette::AlternateBase, isDark ? window.lighter(108) : window.darker(103));

    // Disabled variants — half-alpha windowText so disabled state still reads.
    QColor disabledText = windowText;
    disabledText.setAlpha(128);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, disabledText);
    p.setColor(QPalette::Disabled, QPalette::Text, disabledText);
    p.setColor(QPalette::Disabled, QPalette::WindowText, disabledText);
    QGuiApplication::setPalette(p);
}

int main(int argc, char* argv[]) {
    const QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":/rsc/gay.pancake.lsfg-vk-ui.png"));
    QGuiApplication::setApplicationName("lsfg-vk-ui");
    QGuiApplication::setApplicationDisplayName("lsfg-vk-ui");

    normalizePalette();

    Backend backend;
    qmlRegisterSingletonInstance("lsfgvk", 1, 0, "Backend", &backend);

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/rsc/UI.qml"));

    return QGuiApplication::exec();
}
