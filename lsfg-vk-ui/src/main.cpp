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

// Qt's platform-theme relay can hand back a palette where Window is dark
// (from portal) but Button is light (from a different code path e.g. GTK
// Adwaita), which makes controls render as if disabled or light-on-dark.
// Only intervene if Window and Button sit on opposite ends of the lightness
// spectrum — that indicates the palette was assembled from mismatched
// sources. If the theme is internally consistent, respect it fully.
static void normalizePalette() {
    QPalette p = QGuiApplication::palette();
    const QColor window = p.color(QPalette::Window);
    const QColor button = p.color(QPalette::Button);
    const bool windowDark = window.lightness() < 128;
    const bool buttonDark = button.lightness() < 128;
    if (windowDark == buttonDark) return;

    const QColor windowText = p.color(QPalette::WindowText);
    p.setColor(QPalette::Button, windowDark ? window.lighter(115) : window.darker(105));
    p.setColor(QPalette::ButtonText, windowText);
    p.setColor(QPalette::Base, windowDark ? window.darker(115) : window.lighter(105));
    p.setColor(QPalette::Text, windowText);
    p.setColor(QPalette::AlternateBase, windowDark ? window.lighter(105) : window.darker(103));
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
