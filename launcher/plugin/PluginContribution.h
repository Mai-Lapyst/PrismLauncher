// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2023 Mai Lapyst <67418776+Mai-Lapyst@users.noreply.github.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <QFileInfo>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QString>

#include <functional>

enum class ExtensionPointKind {
    UNKNOWN,
    CAT_PACK,
};

class Plugin;
class PluginContribution;

class ExtentionPointRegistry {
   public:
    using Factory = std::function<PluginContribution*()>;

    static ExtentionPointRegistry& instance() noexcept;

    bool isKnown(const QString& kind) const;
    void withFactory(const QString& kind, std::function<void(Factory&)> action);
    void registerExtensionpoint(QString name, Factory factory);

   private:
    QMap<QString, Factory> m_factories;
};

#define REGISTER_EXTENSIONPOINT(name, clazz) \
    __attribute__((constructor)) static void __registerExtensionPoint_##name () { \
        ExtentionPointRegistry::instance().registerExtensionpoint(#name, [] () { return new clazz(); }); \
    }

class PluginContribution {
   public:
    virtual ~PluginContribution() {}

    ExtensionPointKind kind() const;

    virtual bool loadConfig(const Plugin& plugin, const QJsonValue& json) = 0;

    virtual void onPluginEnable() = 0;
    virtual void onPluginDisable() = 0;

   protected:
    explicit PluginContribution(ExtensionPointKind kind) : m_kind(kind) {}

   private:
    ExtensionPointKind m_kind;
};

#include "extension_points.h"
