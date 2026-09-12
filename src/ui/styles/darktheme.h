#ifndef DARKTHEME_H
#define DARKTHEME_H

#include <QString>

namespace DarkTheme {

inline QString getStyleSheet() {
    return R"(
/* ============================================
   Global Application Style
   ============================================ */

QWidget {
    background-color: #1a1f26;
    color: #e0e0e0;
    font-family: "Segoe UI", system-ui, -apple-system, sans-serif;
    font-size: 9pt;
}

QMainWindow {
    background-color: #1a1f26;
}

/* ============================================
   Menu Bar
   ============================================ */

QMenuBar {
    background-color: #1a1f26;
    color: #e0e0e0;
    border-bottom: 1px solid #2d3139;
    padding: 4px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 6px 12px;
    border-radius: 4px;
}

QMenuBar::item:selected {
    background-color: #2d3139;
}

QMenuBar::item:pressed {
    background-color: #3d4350;
}

QMenu {
    background-color: #1a1f26;
    border: 1px solid #2d3139;
    border-radius: 6px;
    padding: 4px;
}

QMenu::item {
    padding: 8px 24px 8px 12px;
    border-radius: 4px;
}

QMenu::item:selected {
    background-color: #2d3139;
}

/* ============================================
   Panels and Frames
   ============================================ */

QFrame#leftPanel {
    background-color: #0d1117;
    border-right: 1px solid #21262d;
    border-radius: 0px;
}

QFrame#rightPanel {
    background-color: #1a1f26;
}

QFrame#detailsBox, QFrame#settingsBox, QFrame#notesBox {
    background-color: #0d1117;
    border: 1px solid #21262d;
    border-radius: 8px;
    padding: 14px;
}

QFrame#otpSection {
    background: #0d1117;
    border: 1px solid #21262d;
    border-radius: 8px;
    padding: 16px;
}

QFrame#qrSection {
    background: #0d1117;
    border: 1px solid #21262d;
    border-radius: 8px;
    padding: 10px;
}

/* ============================================
   Line Edit (Search & Input)
   ============================================ */

QLineEdit {
    background-color: #0d1117;
    color: #c9d1d9;
    border: 1px solid #30363d;
    border-radius: 6px;
    padding: 8px 12px;
    selection-background-color: #1f6feb;
    font-size: 13px;
}

QLineEdit:focus {
    border: 1px solid #1f6feb;
    outline: none;
}

QLineEdit:disabled {
    background-color: #161b22;
    color: #484f58;
}

/* ============================================
   Buttons
   ============================================ */

QPushButton {
    background-color: #1a1f26;
    color: #e0e0e0;
    border: 1px solid #2d3139;
    border-radius: 6px;
    padding: 8px 16px;
    font-weight: 500;
}

QPushButton:hover {
    background-color: #2d3139;
    border: 1px solid #4da3ff;
}

QPushButton:pressed {
    background-color: #3d4350;
}

QPushButton:disabled {
    background-color: #151b23;
    color: #666;
    border: 1px solid #2d3139;
}

QPushButton#addButton {
    background-color: #2ea043;
    border: 1px solid #2ea043;
    color: #fff;
    font-weight: 600;
}

QPushButton#addButton:hover {
    background-color: #3fb950;
}

QPushButton#editButton {
    background-color: #1f6feb;
    border: 1px solid #1f6feb;
    color: #fff;
}

QPushButton#editButton:hover {
    background-color: #388bfd;
}

QPushButton#deleteButton {
    background-color: #da3633;
    border: 1px solid #da3633;
    color: #fff;
}

QPushButton#deleteButton:hover {
    background-color: #e5534b;
}

QPushButton#copyButton {
    background-color: #1f6feb;
    border: none;
    color: #fff;
    padding: 10px 20px;
    font-size: 11pt;
}

QPushButton#copyButton:hover {
    background-color: #388bfd;
}

/* ============================================
   List Widget (Account List)
   ============================================ */

QListWidget {
    background-color: #151b23;
    border: 1px solid #2d3139;
    border-radius: 6px;
    padding: 4px;
    outline: none;
}

QListWidget::item {
    background-color: transparent;
    border: none;
    padding: 2px;
}

QListWidget::item:selected {
    background-color: transparent;
    border: none;
}

QListWidget::item:hover {
    background-color: transparent;
}

/* ============================================
   Scroll Bar
   ============================================ */

QScrollBar:vertical {
    background-color: #151b23;
    width: 12px;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:vertical {
    background-color: #2d3139;
    border-radius: 5px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background-color: #3d4350;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    background-color: #151b23;
    height: 12px;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:horizontal {
    background-color: #2d3139;
    border-radius: 5px;
    min-width: 30px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #3d4350;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* ============================================
   Progress Bar
   ============================================ */

QProgressBar {
    background-color: #1a1f26;
    border: none;
    border-radius: 4px;
    text-align: center;
    height: 8px;
}

QProgressBar::chunk {
    background-color: #2ea043;
    border-radius: 4px;
}

QProgressBar#lowTime::chunk {
    background-color: #f85149;
}

/* ============================================
   Labels
   ============================================ */

QLabel {
    background-color: transparent;
    color: #e0e0e0;
}

QLabel#sectionTitle {
    font-size: 11pt;
    font-weight: 600;
    color: #58a6ff;
}

QLabel#issuerBig {
    font-size: 18pt;
    font-weight: 700;
    color: #ffffff;
}

QLabel#otpCode {
    font-size: 38pt;
    font-weight: 700;
    color: #58a6ff;
    letter-spacing: 6px;
}

QLabel#timerLabel {
    font-size: 13pt;
    color: #8b949e;
}

/* ============================================
   Combo Box
   ============================================ */

QComboBox {
    background-color: #1a1f26;
    color: #e0e0e0;
    border: 1px solid #2d3139;
    border-radius: 6px;
    padding: 6px 12px;
}

QComboBox:hover {
    border: 1px solid #4da3ff;
}

QComboBox::drop-down {
    border: none;
    width: 20px;
}

QComboBox::down-arrow {
    image: none;
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-top: 6px solid #8b949e;
    margin-right: 6px;
}

QComboBox QAbstractItemView {
    background-color: #1a1f26;
    border: 1px solid #2d3139;
    border-radius: 6px;
    selection-background-color: #2d3139;
    outline: none;
}

/* ============================================
   Text Edit (Notes)
   ============================================ */

QTextEdit, QPlainTextEdit {
    background-color: #1a1f26;
    color: #e0e0e0;
    border: 1px solid #2d3139;
    border-radius: 6px;
    padding: 8px;
    selection-background-color: #3d7acc;
}

QTextEdit:focus, QPlainTextEdit:focus {
    border: 1px solid #4da3ff;
}

/* ============================================
   Dialog
   ============================================ */

QDialog {
    background-color: #0f1419;
}

/* ============================================
   Tab Widget
   ============================================ */

QTabWidget::pane {
    background-color: #151b23;
    border: 1px solid #2d3139;
    border-radius: 6px;
    top: -1px;
}

QTabBar::tab {
    background-color: #1a1f26;
    color: #8b949e;
    border: 1px solid #2d3139;
    border-bottom: none;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    padding: 8px 16px;
    margin-right: 2px;
}

QTabBar::tab:selected {
    background-color: #151b23;
    color: #58a6ff;
    border-bottom: 2px solid #58a6ff;
}

QTabBar::tab:hover:!selected {
    background-color: #2d3139;
}

/* ============================================
   Check Box & Radio Button
   ============================================ */

QCheckBox, QRadioButton {
    color: #e0e0e0;
    spacing: 8px;
}

QCheckBox::indicator, QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border: 2px solid #2d3139;
    border-radius: 4px;
    background-color: #1a1f26;
}

QCheckBox::indicator:checked {
    background-color: #1f6feb;
    border: 2px solid #1f6feb;
}

QRadioButton::indicator {
    border-radius: 9px;
}

QRadioButton::indicator:checked {
    background-color: #1f6feb;
    border: 2px solid #1f6feb;
}

/* ============================================
   Tooltip
   ============================================ */

QToolTip {
    background-color: #1a1f26;
    color: #e0e0e0;
    border: 1px solid #2d3139;
    border-radius: 4px;
    padding: 4px 8px;
}

/* ============================================
   Status Bar
   ============================================ */

QStatusBar {
    background-color: #151b23;
    color: #8b949e;
    border-top: 1px solid #2d3139;
}

QStatusBar::item {
    border: none;
}

)";
}

} // namespace DarkTheme

#endif // DARKTHEME_H
