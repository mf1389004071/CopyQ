/*
    Copyright (c) 2014, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ITEMNOTES_H
#define ITEMNOTES_H

#include "gui/icons.h"
#include "item/itemwidget.h"

#include <QWidget>

#include <memory>

namespace Ui {
class ItemNotesSettings;
}

class QTextEdit;
class QTimer;

enum NotesPosition {
    NotesAbove,
    NotesBelow,
    NotesBeside,
};

class ItemNotes : public QWidget, public ItemWidget
{
    Q_OBJECT

public:
    ItemNotes(ItemWidget *childItem, const QString &text, const QByteArray &icon,
              NotesPosition notesPosition, bool showToolTip);

    void setCurrent(bool current) override;

protected:
    void highlight(const QRegExp &re, const QFont &highlightFont,
                           const QPalette &highlightPalette) override;

    QWidget *createEditor(QWidget *parent) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const override;

    bool hasChanges(QWidget *editor) const override;

    QObject *createExternalEditor(const QModelIndex &index, QWidget *parent) const override;

    void updateSize(QSize maximumSize, int idealWidth) override;

    void paintEvent(QPaintEvent *event) override;

    bool eventFilter(QObject *, QEvent *event) override;

private:
    void showToolTip();

    QTextEdit *m_notes;
    QWidget *m_icon;
    std::unique_ptr<ItemWidget> m_childItem;
    QTimer *m_timerShowToolTip;
    QString m_toolTipText;
    bool m_isCurrent = false;
};

class ItemNotesLoader : public QObject, public ItemLoaderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID COPYQ_PLUGIN_ITEM_LOADER_ID)
    Q_INTERFACES(ItemLoaderInterface)

public:
    ItemNotesLoader();
    ~ItemNotesLoader();

    QString id() const override { return "itemnotes"; }
    QString name() const override { return tr("Notes"); }
    QString author() const override { return QString(); }
    QString description() const override { return tr("Display notes for items."); }
    QVariant icon() const override { return QVariant(IconPenSquare); }

    QStringList formatsToSave() const override;

    QVariantMap applySettings() override;

    void loadSettings(const QVariantMap &settings) override { m_settings = settings; }

    QWidget *createSettingsWidget(QWidget *parent) override;

    ItemWidget *transform(ItemWidget *itemWidget, const QVariantMap &data) override;

    bool matches(const QModelIndex &index, const QRegExp &re) const override;

private:
    QVariantMap m_settings;
    std::unique_ptr<Ui::ItemNotesSettings> ui;
};

#endif // ITEMNOTES_H
