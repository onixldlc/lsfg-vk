/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <QIcon>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QUrl>

#include "backend.hpp"

using namespace lsfgvk::ui;

int main(int argc, char* argv[]) {
    const QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":/rsc/gay.pancake.lsfg-vk-ui.png"));
    QGuiApplication::setApplicationName("lsfg-vk-ui");
    QGuiApplication::setApplicationDisplayName("lsfg-vk-ui");

    Backend backend;
    qmlRegisterSingletonInstance("lsfgvk", 1, 0, "Backend", &backend);

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/rsc/UI.qml"));

    return QGuiApplication::exec();
}
