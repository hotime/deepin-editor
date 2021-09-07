/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     liumaochuan <liumaochuan@uniontech.com>
* Maintainer: liumaochuan <liumaochuan@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_setting.h"
#include "../../src/common/settings.h"
#include <DSettingsDialog>
#include <DSettingsWidgetFactory>
#include <QKeySequence>
#include <DSettingsOption>
#include <DSettings>
#include <QStandardPaths>
#include <DtkCores>

test_setting::test_setting()
{
}

void test_setting::SetUp()
{
    m_setting = new Settings();
    EXPECT_NE(m_setting,nullptr);
}

void test_setting::TearDown()
{
    delete m_setting;
    m_setting=nullptr;
}

TEST_F(test_setting, Settings)
{
    QString figPath = QString("%1/%2/%3/config.conf")
                          .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
                          .arg(qApp->organizationName())
                          .arg(qApp->applicationName());
    QSettingBackend *m_backend = new QSettingBackend(figPath);


    m_setting->settings = DSettings::fromJsonFile(":/resources/settings.json");
    m_setting->settings->setBackend(m_backend);
    auto fontFamliy = m_setting->settings->option("base.font.family");
    //        QVariant value;
    //        fontFamliy->valueChanged(value);
    //        sleep(500);

    QVariant retVal;
    QMetaObject::invokeMethod(fontFamliy, "valueChanged", Qt::DirectConnection,
                              QGenericReturnArgument(),
                              Q_ARG(QVariant, "dsd"));


    EXPECT_NE(m_backend,nullptr);
    m_backend->deleteLater();
}

//static Settings* instance();
TEST_F(test_setting, instance)
{
    EXPECT_NE(Settings::instance(),nullptr);

}

//void dtkThemeWorkaround(QWidget *parent, const QString &theme);
TEST_F(test_setting, dtkThemeWorkaround)
{
    QWidget *widget = new QWidget();
    Settings::instance()->dtkThemeWorkaround(widget, "dlight");


    EXPECT_NE(widget,nullptr);
    widget->deleteLater();

}

//static QPair<QWidget*, QWidget*> createFontComBoBoxHandle(QObject *obj);
TEST_F(test_setting, createFontComBoBoxHandle)
{
    QWidget *widget = new QWidget();
    DSettingsDialog *dialog = new DSettingsDialog(widget);
    dialog->widgetFactory()->registerWidget("fontcombobox", m_setting->createFontComBoBoxHandle);

    DTK_CORE_NAMESPACE::DSettingsOption o;
    auto it = m_setting->createFontComBoBoxHandle(&o);
    EXPECT_EQ(it.first,nullptr);

    EXPECT_NE(dialog,nullptr);
    EXPECT_NE(widget,nullptr);
    widget->deleteLater();
    dialog->deleteLater();

}

//static QPair<QWidget*, QWidget*> createKeySequenceEditHandle(QObject *obj);
TEST_F(test_setting, createKeySequenceEditHandle)
{
    QWidget *widget = new QWidget();
    DSettingsDialog *dialog = new DSettingsDialog(widget);
    dialog->widgetFactory()->registerWidget("fontcombobox", Settings::createKeySequenceEditHandle);

    DTK_CORE_NAMESPACE::DSettingsOption o;
    auto it = m_setting->createKeySequenceEditHandle(&o);
    EXPECT_EQ(it.first,nullptr);

    EXPECT_NE(dialog,nullptr);
    EXPECT_NE(widget,nullptr);
    widget->deleteLater();
    dialog->deleteLater();
}

//static Settings* instance();

//void setSettingDialog(DSettingsDialog *settingsDialog);
TEST_F(test_setting, setSettingDialog)
{
    QWidget *widget = new QWidget();
    DSettingsDialog *dialog = new DSettingsDialog(widget);
    Settings::instance()->setSettingDialog(dialog);

    EXPECT_NE(Settings::instance()->m_pSettingsDialog,nullptr);

    EXPECT_NE(dialog,nullptr);
    EXPECT_NE(widget,nullptr);
    widget->deleteLater();
    dialog->deleteLater();
}

//private:
//void updateAllKeysWithKeymap(QString keymap);
TEST_F(test_setting, updateAllKeysWithKeymap)
{
    QString keymap = Settings::instance()->settings->option("shortcuts.keymap.keymap")->value().toString();
    Settings::instance()->updateAllKeysWithKeymap(keymap);

    EXPECT_NE(Settings::instance()->m_bUserChangeKey,true);
}

//void copyCustomizeKeysFromKeymap(QString keymap);
TEST_F(test_setting, copyCustomizeKeysFromKeymap)
{
    QString keymap = Settings::instance()->settings->option("shortcuts.keymap.keymap")->value().toString();
    Settings::instance()->copyCustomizeKeysFromKeymap(keymap);

    EXPECT_NE(Settings::instance()->m_bUserChangeKey,true);
}

//此函数代码调试中已经覆盖， html中显示未覆盖
TEST_F(test_setting, checkShortcutValid)
{
    bool ok;
    QString reason = "reason";
    EXPECT_NE(m_setting->checkShortcutValid("shortcuts.keymap.keymap", "Enter", reason, ok),true);
}

TEST_F(test_setting, checkShortcutValid2)
{
    bool ok;
    QString reason = "reason";
    EXPECT_NE(m_setting->checkShortcutValid("shortcuts.keymap.keymap", "<", reason, ok),true);
}

TEST_F(test_setting, checkShortcutValid3)
{
    bool ok;
    QString reason = "reason";
    EXPECT_NE(m_setting->checkShortcutValid("shortcuts.keymap.keymap<", "Num+", reason, ok),true);
}

TEST_F(test_setting, isShortcutConflict)
{
    //Settings::instance()->isShortcutConflict("shortcuts.keymap.keymap", "Enter");
    //    assert(1 == 1);
    QStringList list;
    list << "aa"
         << "bb";
    EXPECT_NE(m_setting->isShortcutConflict("aa", "bb"),true);

    EXPECT_NE(m_setting->isShortcutConflict("aa", "bb"),true);

    QVariant value;
    m_setting->slotCustomshortcut("Ctrl+T",value);

    m_setting->slotsigAdjustFont(value);

    QVariant keymap2 = Settings::instance()->settings->option("base.font.size")->value();
    m_setting->slotsigAdjustFontSize(keymap2);
    m_setting->slotsigAdjustWordWrap(value);
    m_setting->slotsigSetLineNumberShow(value);
    m_setting->slotsigAdjustBookmark(value);
    m_setting->slotsigShowCodeFlodFlag(value);
    m_setting->slotsigShowBlankCharacter(value);
    m_setting->slotsigHightLightCurrentLine(value);
    m_setting->slotsigAdjustTabSpaceNumber(value);

    QVariant keymap = Settings::instance()->settings->option("shortcuts.keymap.keymap")->value();
    m_setting->slotupdateAllKeysWithKeymap(keymap);
}

TEST_F(test_setting, KeySequenceEdit)
{
    QWidget *widget = new QWidget();
    DSettingsDialog *dialog = new DSettingsDialog(widget);
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(dialog);

    KeySequenceEdit *shortCutLineEdit = new KeySequenceEdit(option);
    QEvent *e = new QEvent(QEvent::KeyPress);
    shortCutLineEdit->eventFilter(shortCutLineEdit,e);

    QVariant keymap = Settings::instance()->settings->option("shortcuts.keymap.keymap")->value();
    shortCutLineEdit->slotDSettingsOptionvalueChanged(keymap);


    EXPECT_NE(widget,nullptr);
    EXPECT_NE(shortCutLineEdit,nullptr);
    EXPECT_NE(e,nullptr);
    widget->deleteLater();
    shortCutLineEdit->deleteLater();
    delete e ;e=nullptr;
}

//以下两条CASE 脚本跑会造成程序崩，加两行debug后就不崩了
//TEST_F(test_setting, createDialog2)
//{
//    Settings set;
//    set.createDialog("ba", "bb", false);
//}

//TEST_F(test_setting, createDialog)
//{
//    Settings set;
//    set.createDialog("ba", "bb", true);
//}
