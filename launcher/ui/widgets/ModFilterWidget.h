#pragma once

#include <QButtonGroup>
#include <QTabWidget>

#include "Version.h"

#include "VersionProxyModel.h"
#include "meta/VersionList.h"

#include "minecraft/MinecraftInstance.h"
#include "modplatform/ModIndex.h"

class MinecraftInstance;

namespace Ui {
class ModFilterWidget;
}

class ModFilterWidget : public QTabWidget {
    Q_OBJECT
   public:
    struct Filter {
        std::list<Version> versions;
        std::list<ModPlatform::IndexedVersionType> releases;
        ModPlatform::ModLoaderTypes loaders;
        QString side;
        bool hideInstalled;

        bool operator==(const Filter& other) const
        {
            return hideInstalled == other.hideInstalled && side == other.side && loaders == other.loaders && versions == other.versions &&
                   releases == other.releases;
        }
        bool operator!=(const Filter& other) const { return !(*this == other); }
    };

    static unique_qobject_ptr<ModFilterWidget> create(MinecraftInstance* instance, bool extendedSupport, QWidget* parent = nullptr);
    virtual ~ModFilterWidget();

    auto getFilter() -> std::shared_ptr<Filter>;
    auto changed() const -> bool { return m_filter_changed; }

   signals:
    void filterChanged();
    void filterUnchanged();

   private:
    ModFilterWidget(MinecraftInstance* instance, bool extendedSupport, QWidget* parent = nullptr);

    void loadVersionList();
    void prepareBasicFilter();

   private slots:
    void onVersionFilterChanged();
    void onVersionFilterTextChanged(QString version);
    void onReleaseFilterChanged();
    void onLoadersFilterChanged();
    void onSideFilterChanged();
    void onHideInstalledFilterChanged();
    void onIncludeSnapshotsChanged();

   private:
    Ui::ModFilterWidget* ui;

    MinecraftInstance* m_instance = nullptr;
    std::shared_ptr<Filter> m_filter;
    bool m_filter_changed = false;

    Meta::VersionList::Ptr m_version_list;
    VersionProxyModel* m_versions_proxy = nullptr;
};
