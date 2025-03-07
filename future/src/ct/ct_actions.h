/*
 * ct_actions.h
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

#include "ct_dialogs.h"
#include "ct_codebox.h"
#include "ct_image.h"
#include "ct_table.h"
#include "ct_main_win.h"
#include <optional>

class CtMainWin;
class CtActions
{
public:
    void init(CtMainWin* pCtMainWin, CtTreeStore* pCtTreeStore)
    {
        _pCtMainWin = pCtMainWin;
        _pCtTreestore = pCtTreeStore;
        _find_init();
    }

public:
    CtCodebox*      curr_codebox_anchor = nullptr;
    CtTable*        curr_table_anchor = nullptr;
    CtImageAnchor*  curr_anchor_anchor = nullptr;
    CtImagePng*     curr_image_anchor = nullptr;
    CtImageEmbFile* curr_file_anchor = nullptr;

private:
    ct_dialogs::CtLinkEntry _link_entry;

private:
    size_t                          _next_opened_emb_file_id = 1;
    std::map<Glib::ustring, time_t> _embfiles_opened;
    sigc::connection                _embfiles_timeout_connection;

private:
    CtMainWin*   _pCtMainWin;
    CtTreeStore* _pCtTreestore;

public:
    CtMainWin*   getCtMainWin() { return _pCtMainWin; }

private:
    Glib::RefPtr<Gtk::TextBuffer> curr_buffer();
    bool          _node_sel_and_rich_text();

public: // todo: fix naming
    bool          _is_there_selected_node_or_error();
    bool          _is_tree_not_empty_or_error();
    bool          _is_curr_node_not_read_only_or_error();
    bool          _is_curr_node_not_syntax_highlighting_or_error(bool plain_text_ok = false);

public:
    void object_set_selection(CtAnchoredWidget* widget);

private:
    // helpers for file actions

public:
    // file actions
    void file_save_as();

private:
    // helpers for tree actions
    void          _node_add(bool duplicate, bool add_child);
    void          _node_add_with_data(Gtk::TreeIter curr_iter, CtNodeData& nodeData, bool add_child);
    void          _node_child_exist_or_create(Gtk::TreeIter parentIter, const std::string& nodeName);
    void          _node_move_after(Gtk::TreeIter iter_to_move, Gtk::TreeIter father_iter,
                                   Gtk::TreeIter brother_iter = Gtk::TreeIter(), bool set_first = false);
    bool          _need_node_swap(Gtk::TreeIter& leftIter, Gtk::TreeIter& rightIter, bool ascendings);
    bool          _tree_sort_level_and_sublevels(const Gtk::TreeNodeChildren& children, bool ascending);

public:
    // tree actions
    void node_add()       { _node_add(false, false); }
    void node_dublicate() { _node_add(true, false);  }
    void node_child_add() { _node_add(false, true); }
    void node_edit();
    void node_toggle_read_only();
    void node_date();
    void node_up();
    void node_down();
    void node_right();
    void node_left();
    void node_change_father();
    void tree_sort_ascending();
    void tree_sort_descending();
    void node_siblings_sort_ascending();
    void node_siblings_sort_descending();

    void bookmark_curr_node();
    void bookmark_curr_node_remove();
    void bookmarks_handle();

private:
    // helpers for find actions
    void                _find_init();
    void                _find_in_all_nodes(bool for_current_node);
    std::string         _dialog_search(const std::string& title, bool replace_on, bool multiple_nodes, bool pattern_required);
    bool                _parse_node_name(CtTreeIter node_iter, Glib::RefPtr<Glib::Regex> re_pattern, bool forward, bool all_matches);
    bool                _parse_given_node_content(CtTreeIter node_iter, Glib::ustring pattern, bool forward, bool first_fromsel, bool all_matches);
    bool                _parse_node_content_iter(const CtTreeIter& tree_iter, Glib::RefPtr<Gtk::TextBuffer> text_buffer, const std::string& pattern,
                                                bool forward, bool first_fromsel, bool all_matches, bool first_node);
    Gtk::TextIter       _get_inner_start_iter(Glib::RefPtr<Gtk::TextBuffer> text_buffer, bool forward, const gint64& node_id);
    bool                _is_node_within_time_filter(const CtTreeIter& node_iter);
    bool                _find_pattern(CtTreeIter tree_iter, Glib::RefPtr<Gtk::TextBuffer> text_buffer, std::string pattern,
                                      Gtk::TextIter start_iter, bool forward, bool all_matches);
    std::string         _get_line_content(Glib::RefPtr<Gtk::TextBuffer> text_buffer, Gtk::TextIter text_iter);
    std::string         _get_first_line_content(Glib::RefPtr<Gtk::TextBuffer> text_buffer);
    Glib::ustring       _check_pattern_in_object(Glib::RefPtr<Glib::Regex> pattern, CtAnchoredWidget* obj);
    std::pair<int, int> _check_pattern_in_object_between(Glib::RefPtr<Gtk::TextBuffer> text_buffer, Glib::RefPtr<Glib::Regex> pattern,
                                                         int start_offset, int end_offset, bool forward, std::string& obj_content);
    int                 _get_num_objs_before_offset(Glib::RefPtr<Gtk::TextBuffer> text_buffer, int max_offset);
    void                _iterated_find_dialog();
    void                _update_all_matches_progress();

public:
    // find actions
    void find_in_selected_node();
    void find_in_all_nodes()             { _find_in_all_nodes(false); }
    void find_in_sel_node_and_subnodes() { _find_in_all_nodes(true); }
    void find_a_node();
    void find_again();
    void find_back();
    void replace_in_selected_node();
    void replace_in_all_nodes();
    void replace_in_sel_node_and_subnodes();
    void replace_in_nodes_names();
    void replace_again();
    void find_allmatchesdialog_restore();

private:
    // helpers for view actions

public:
    // view actions
    void toggle_show_hide_tree();
    void toggle_show_hide_toolbar();
    void toggle_show_hide_node_name_header();
    void toggle_tree_text();
    void nodes_expand_all();
    void nodes_collapse_all();
    void toolbar_icons_size_increase();
    void toolbar_icons_size_decrease();
    void fullscreen_toggle();

public:
    // helper for format actions
    void _apply_tag(const Glib::ustring& tag_property, Glib::ustring property_value = "",
                    std::optional<Gtk::TextIter> iter_sel_start = std::nullopt,
                    std::optional<Gtk::TextIter> iter_sel_end = std::nullopt,
                    Glib::RefPtr<Gtk::TextBuffer> text_buffer = Glib::RefPtr<Gtk::TextBuffer>());
public:
    Glib::ustring apply_tag_exist_or_create(const Glib::ustring& tag_property, Glib::ustring property_value);

private:
    struct text_view_n_buffer_codebox_proof {
        CtTextView*                     text_view;
        Glib::RefPtr<Gtk::TextBuffer>   text_buffer;
        std::string                     syntax_highl;
        bool                            from_codebox;
    };
    text_view_n_buffer_codebox_proof _get_text_view_n_buffer_codebox_proof();
    CtCodebox* _codebox_in_use();

    bool _links_entries_pre_dialog(const Glib::ustring& curr_link, ct_dialogs::CtLinkEntry& link_entry);
    Glib::ustring _links_entries_post_dialog(ct_dialogs::CtLinkEntry& link_entry);
    Glib::ustring _link_check_around_cursor();

public:
    // format actions
    void apply_tag_latest();
    void remove_text_formatting();
    void apply_tag_foreground();
    void apply_tag_background();
    void apply_tag_bold();
    void apply_tag_italic();
    void apply_tag_underline();
    void apply_tag_strikethrough();
    void apply_tag_h1();
    void apply_tag_h2();
    void apply_tag_h3();
    void apply_tag_small();
    void apply_tag_superscript();
    void apply_tag_subscript();
    void apply_tag_monospace();
    void list_bulleted_handler();
    void list_numbered_handler();
    void list_todo_handler();
    void apply_tag_justify_left();
    void apply_tag_justify_center();
    void apply_tag_justify_right();
    void apply_tag_justify_fill();

private:
    // helper for edit actions
    void          _image_edit_dialog(Glib::RefPtr<Gdk::Pixbuf> pixbuf, Gtk::TextIter insert_iter, Gtk::TextIter* iter_bound);
    Glib::ustring _get_iter_alignment(Gtk::TextIter text_iter);
    void          _text_selection_change_case(gchar change_type);

public:
    void          image_insert_png(Gtk::TextIter iter_insert, Glib::RefPtr<Gdk::Pixbuf> pixbuf,
                                   const Glib::ustring& link, const Glib::ustring& image_justification);
    void          image_insert_anchor(Gtk::TextIter iter_insert, const Glib::ustring& name, const Glib::ustring& image_justification);

public:
    // edit actions
    void insert_spec_char_action(gunichar ch);
    void requested_step_back();
    void requested_step_ahead();
    void image_handle();
    void table_handle();
    void codebox_handle();
    void embfile_insert();
    void apply_tag_link();
    void anchor_handle();
    void toc_insert();
    void timestamp_insert();
    void horizontal_rule_insert();
    void text_selection_lower_case();
    void text_selection_upper_case();
    void text_selection_toggle_case();
    void toggle_ena_dis_spellcheck();
    void cut_as_plain_text();
    void copy_as_plain_text();
    void paste_as_plain_text();
    void text_row_cut();
    void text_row_copy();
    void text_row_delete();
    void text_row_selection_duplicate();
    void text_row_up();
    void text_row_down();
    void strip_trailing_spaces();

private:
    // helper for others actions
    void _anchor_edit_dialog(CtImageAnchor* anchor, Gtk::TextIter insert_iter, Gtk::TextIter* iter_bound);
    bool _on_embfiles_sentinel_timeout();

public:
    // others actions
    void anchor_cut();
    void anchor_copy();
    void anchor_delete();
    void anchor_edit();
    void embfile_cut();
    void embfile_copy();
    void embfile_delete();
    void embfile_save();
    void embfile_open();
    void image_save();
    void image_edit();
    void image_cut();
    void image_copy();
    void image_delete();
    void image_link_edit();
    void image_link_dismiss();
    void toggle_show_hide_main_window();

    void link_clicked(const Glib::ustring& tag_property_value, bool from_wheel);

    void codebox_cut();
    void codebox_copy();
    void codebox_delete();
    void codebox_delete_keeping_text();
    void codebox_change_properties();
    void exec_code();
    void codebox_load_from_file();
    void codebox_save_to_file();
    void codebox_increase_width();
    void codebox_decrease_width();
    void codebox_increase_height();
    void codebox_decrease_height();

};
