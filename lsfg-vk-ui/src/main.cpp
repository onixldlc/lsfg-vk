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

// Qt platform themes (especially xdgdesktopportal on Qt 6.4) can deliver
// palettes where Button/ButtonText/Base/Text roles are either missing or
// low-contrast against Window/WindowText. Fusion draws every control from
// those roles, so a bad Button/ButtonText combo renders as unreadable.
// Derive the control-surface roles from Window and force text to match
// WindowText — the user's chosen window+text pair stays the source of
// truth; we only fill in gaps and enforce contrast.
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
