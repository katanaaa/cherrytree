/*
 * ct_main_win.h
 *
 * Copyright 2017-2019 Giuseppe Penone <giuspen@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <gtkmm/statusbar.h>
#include "ct_treestore.h"
#include "ct_misc_utils.h"
#include "ct_menu.h"
#include "ct_widgets.h"


class CtDialogTextEntry : public Gtk::Dialog
{
public:
    CtDialogTextEntry(const char* title, const bool forPassword, Gtk::Window* pParent);
    virtual ~CtDialogTextEntry();
    Glib::ustring getEntryText();

protected:
    bool _onEntryKeyPress(GdkEventKey* eventKey);
    void _onEntryIconPress(Gtk::EntryIconPosition iconPosition, const GdkEventButton* event);
    Gtk::Entry _entry;
};

struct CtStatusBar
{
    Gtk::Statusbar   statusBar;
    guint            statusId;
    Gtk::ProgressBar progressBar;
    Gtk::Button      stopButton;
    Gtk::Frame       frame;
    Gtk::HBox        hbox;

    void set_progress_stop(bool stop) { _progress_stop = stop; }
    bool is_progress_stop()           { return _progress_stop; }
    void update_status(const Glib::ustring& text) { statusBar.pop(statusId); statusBar.push(text, statusId); }

private:
    bool _progress_stop;
};

struct CtWinHeader
{
    Gtk::HBox        headerBox;
    Gtk::HButtonBox  buttonBox;
    Gtk::Label       nameLabel;
    Gtk::Image       lockIcon;
    Gtk::Image       bookmarkIcon;
    Gtk::EventBox    eventBox;
};

class CtMenu;
class CtActions;
class CtMainWin : public Gtk::ApplicationWindow
{
public:
    CtMainWin(CtMenu* pCtMenu);
    virtual ~CtMainWin();

    bool readNodesFromGioFile(const Glib::RefPtr<Gio::File>& r_file, const bool isImport);
    void config_apply_before_show_all();
    void config_apply_after_show_all();
    void configureTheme();
    void update_window_save_needed(std::string update_type = "",
                                   bool new_machine_state = false, const CtTreeIter* give_tree_iter = nullptr) { /* todo: */ }

    CtTreeIter    curr_tree_iter()  { return _ctTreestore.to_ct_tree_iter(_ctTreeview.get_selection()->get_selected()); }
    CtTreeStore&  get_tree_store()  { return _ctTreestore; }
    CtTreeView&   get_tree_view()   { return _ctTreeview; }
    CtTextView&   get_text_view()   { return _ctTextview; }
    CtMenu&       get_ct_menu()     { return *_ctMenu; }
    CtStatusBar&  get_status_bar()  { return _ctStatusBar; }

    bool&         user_active()     { return _userActive; } // use as a function, because it's easer to put breakpoint
    int&          cursor_key_press() { return _cursorKeyPress; }
    int&          hovering_link_iter_offset() { return _hovering_link_iter_offset; }

    Glib::RefPtr<Gtk::TextBuffer> curr_buffer() { return _ctTextview.get_buffer(); }

private:
    Gtk::HBox&    _initStatusBar();
    Gtk::EventBox& _initWindowHeader();

public:
    void window_header_update();
    void window_header_update_lock_icon(bool show);
    void window_header_update_bookmark_icon(bool show);
    void window_header_update_last_visited();
    void window_header_update_num_last_visited();

    void menu_tree_update_for_bookmarked_node(bool is_bookmarked);
    void bookmark_action_select_node(gint64 node_id);
    void set_bookmarks_menu_items();

    void set_menu_items_special_chars();

    void show_hide_toolbar(bool visible)    { _pToolbar->property_visible() = visible; }
    void show_hide_tree_view(bool visible)  { _scrolledwindowTree.property_visible() = visible; }
    void show_hide_win_header(bool visible) { _ctWinHeader.headerBox.property_visible() = visible; }
    void set_toolbar_icon_size(int size)    { _pToolbar->property_icon_size() = CtMiscUtil::getIconSize(size); }

protected:
    bool                _onTheWindowSignalKeyPressEvent(GdkEventKey* event);

    void                _onTheTreeviewSignalCursorChanged();
    bool                _onTheTreeviewSignalButtonPressEvent(GdkEventButton* event);
    bool                _onTheTreeviewSignalKeyPressEvent(GdkEventKey* event);
    bool                _onTheTreeviewSignalPopupMenu();

    void                _onTheTextviewPopulateMenu(Gtk::Menu* menu);
    bool                _onTheTextviewMotionNotifyEvent(GdkEventMotion* event);
    bool                _onTheTextviewVisibilityNotifyEvent(GdkEventVisibility* event);
    void                _onTheTextviewSizeAllocate(Gtk::Allocation& allocation);
    bool                _onTheTextviewEvent(GdkEvent* event);
    void                _onTheTextviewEventAfter(GdkEvent* event);

    void                _titleUpdate(bool saveNeeded);

protected:
    Gtk::VBox           _vboxMain;
    Gtk::VBox           _vboxText;
    Gtk::HPaned         _hPaned;
    Gtk::MenuBar*       _pMenu;
    Gtk::Toolbar*       _pToolbar;
    CtMenu*             _ctMenu;
    CtStatusBar         _ctStatusBar;
    CtWinHeader         _ctWinHeader;
    Gtk::MenuItem*      _pBookmarksSubmenu;
    Gtk::MenuItem*      _pSpecialCharsSubmenu;
    Gtk::ScrolledWindow _scrolledwindowTree;
    Gtk::ScrolledWindow _scrolledwindowText;
    CtTreeStore         _ctTreestore;
    CtTreeView          _ctTreeview;
    CtTextView          _ctTextview; /* todo: rename? because _ctTextview and _ctTreeview look the same */
    std::string         _currFileName;
    std::string         _currFileDir;

    Glib::RefPtr<Gtk::CssProvider> _css_provider_theme;
    Glib::RefPtr<Gtk::CssProvider> _css_provider_theme_font;

private:
    bool                _userActive;
    int                 _cursorKeyPress;
    int                 _hovering_link_iter_offset;
    int                 _prevTextviewWidth;
};
