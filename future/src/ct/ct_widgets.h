/*
 * ct_widgets.h
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
#include <libxml++/libxml++.h>
#include <sqlite3.h>
#include "ct_list.h"

enum class CtAnchWidgType { CodeBox, Table, ImagePng, ImageAnchor, ImageEmbFile };

class CtAnchoredWidget : public Gtk::EventBox
{
public:
    CtAnchoredWidget(const int charOffset, const std::string& justification);
    virtual ~CtAnchoredWidget();

    void insertInTextBuffer(Glib::RefPtr<Gsv::Buffer> rTextBuffer);
    Glib::RefPtr<Gtk::TextChildAnchor> getTextChildAnchor() { return _rTextChildAnchor; }

    virtual void applyWidthHeight(const int parentTextWidth) = 0;
    virtual void to_xml(xmlpp::Element* p_node_parent, const int offset_adjustment) = 0;
    virtual bool to_sqlite(sqlite3* pDb, const gint64 node_id, const int offset_adjustment) = 0;
    virtual CtAnchWidgType get_type() = 0;

    void updateOffset(int charOffset) { _charOffset = charOffset; }
    void updateJustification(std::string justification) { _justification = justification; }

    int getOffset() { return _charOffset; }
    const std::string& getJustification() { return _justification; }

protected:
    Gtk::Frame _frame;
    Gtk::Label _labelWidget;
    int _charOffset;
    std::string _justification;
    Glib::RefPtr<Gtk::TextChildAnchor> _rTextChildAnchor;
};


class CtTreeView : public Gtk::TreeView
{
public:
    CtTreeView();
    virtual ~CtTreeView();
    void set_cursor_safe(const Gtk::TreeIter& iter);

protected:
};


class CtTextView : public Gsv::View
{
public:
    CtTextView();
    virtual ~CtTextView();

    void setupForSyntax(const std::string& syntaxHighlighting);
    void set_pixels_inside_wrap(int space_around_lines, int relative_wrapped_space);
    void set_selection_at_offset_n_delta(int offset, int delta, Glib::RefPtr<Gtk::TextBuffer> text_buffer = Glib::RefPtr<Gtk::TextBuffer>());
    void list_change_level(Gtk::TextIter iter_insert, const CtListInfo& list_info, bool level_increase);
    void replace_text(const Glib::ustring& text, int start_offset, int end_offset);

    void for_event_after_double_click_button1(GdkEvent* event);
    void for_event_after_button_press(GdkEvent* event);
    void for_event_after_key_press(GdkEvent* event, const Glib::ustring& syntaxHighlighting);

    void cursor_and_tooltips_handler(int x, int y);
    void zoom_text(bool is_increase);

private:
    bool          _apply_tag_try_link(Gtk::TextIter iter_end, int offset_cursor);
    Glib::ustring _get_former_line_indentation(Gtk::TextIter iter_start);
    void          _special_char_replace(gunichar special_char, Gtk::TextIter iter_start, Gtk::TextIter iter_insert);

public:
    static const double TEXT_SCROLL_MARGIN;

protected:
    void _setFontForSyntax(const std::string& syntaxHighlighting);
};
