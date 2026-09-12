#ifndef LIGHTTHEME_H
#define LIGHTTHEME_H

#include <QString>

namespace LightTheme {

inline QString getStyleSheet() {
    return R"(
/* ============================================
   Global Application Style - Light Theme
   ============================================ */

QWidget {
    background-color: #ffffff;
    color: #24292f;
    font-family: "Segoe UI", system-ui, -apple-system, sans-serif;
    font-size: 9pt;
}

QMainWindow {
    background-color: #f6f8fa;
}

/* ============================================
   Menu Bar
   ============================================ */

QMenuBar {
    background-color: #ffffff;
    color: #24292f;
    border-bottom: 1px solid #d0d7de;
    padding: 4px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 6px 12px;
    border-radius: 4px;
}

QMenuBar::item:selected {
    background-color: #f6f8fa;
}

QMenuBar::item:pressed {
    background-color: #eaeef2;
}

QMenu {
    background-color: #ffffff;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    padding: 4px;
}

QMenu::item {
    padding: 8px 24px 8px 12px;
    border-radius: 4px;
}

QMenu::item:selected {
    background-color: #f6f8fa;
}

/* ============================================
   Panels and Frames
   ============================================ */

QFrame#leftPanel {
    background-color: #f6f8fa;
    border-right: 1px solid #d0d7de;
    border-radius: 0px;
}

QFrame#rightPanel {
    background-color: #ffffff;
}

QFrame#detailsBox, QFrame#settingsBox, QFrame#notesBox {
    background-color: #f6f8fa;
    border: 1px solid #d0d7de;
    border-radius: 8px;
    padding: 14px;
}

QFrame#otpSection {
    background: #f6f8fa;
    border: 1px solid #d0d7de;
    border-radius: 8px;
    padding: 16px;
}

QFrame#qrSection {
    background: #f6f8fa;
    border: 1px solid #d0d7de;
    border-radius: 8px;
    padding: 10px;
}

/* ============================================
   Line Edit (Search & Input)
   ============================================ */

QLineEdit {
    background-color: #ffffff;
    color: #24292f;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    padding: 8px 12px;
    selection-background-color: #0969da;
    font-size: 13px;
}

QLineEdit:focus {
    border: 1px solid #0969da;
    outline: none;
}

QLineEdit:disabled {
    background-color: #f6f8fa;
    color: #8c959f;
}

/* ============================================
   Buttons
   ============================================ */

QPushButton {
    background-color: #f6f8fa;
    color: #24292f;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    padding: 8px 16px;
    font-weight: 500;
}

QPushButton:hover {
    background-color: #eaeef2;
    border: 1px solid #0969da;
}

QPushButton:pressed {
    background-color: #dde4ea;
}

QPushButton:disabled {
    background-color: #f6f8fa;
    color: #8c959f;
    border: 1px solid #d0d7de;
}

QPushButton#addButton {
    background-color: #1a7f37;
    border: 1px solid #1a7f37;
    color: #fff;
    font-weight: 600;
}

QPushButton#addButton:hover {
    background-color: #2da44e;
}

QPushButton#editButton {
    background-color: #0969da;
    border: 1px solid #0969da;
    color: #fff;
}

QPushButton#editButton:hover {
    background-color: #0860ca;
}

QPushButton#deleteButton {
    background-color: #cf222e;
    border: 1px solid #cf222e;
    color: #fff;
}

QPushButton#deleteButton:hover {
    background-color: #a40e26;
}

QPushButton#copyButton {
    background-color: #0969da;
    border: none;
    color: #fff;
    padding: 10px 20px;
    font-size: 11pt;
}

QPushButton#copyButton:hover {
    background-color: #0860ca;
}

/* ============================================
   Scroll Bar
   ============================================ */

QScrollBar:vertical {
    background-color: #f6f8fa;
    width: 12px;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:vertical {
    background-color: #d0d7de;
    border-radius: 5px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background-color: #afb8c1;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    background-color: #f6f8fa;
    height: 12px;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:horizontal {
    background-color: #d0d7de;
    border-radius: 5px;
    min-width: 30px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #afb8c1;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* ============================================
   Progress Bar
   ============================================ */

QProgressBar {
    background-color: #eaeef2;
    border: none;
    border-radius: 4px;
    text-align: center;
    height: 8px;
}

QProgressBar::chunk {
    background-color: #1a7f37;
    border-radius: 4px;
}

QProgressBar#lowTime::chunk {
    background-color: #cf222e;
}

/* ============================================
   Labels
   ============================================ */

QLabel {
    background-color: transparent;
    color: #24292f;
}

QLabel#sectionTitle {
    font-size: 11pt;
    font-weight: 600;
    color: #0969da;
}

QLabel#issuerBig {
    font-size: 18pt;
    font-weight: 700;
    color: #24292f;
}

QLabel#otpCode {
    font-size: 38pt;
    font-weight: 700;
    color: #0969da;
    letter-spacing: 6px;
}

QLabel#timerLabel {
    font-size: 13pt;
    color: #57606a;
}

/* ============================================
   Combo Box
   ============================================ */

QComboBox {
    background-color: #f6f8fa;
    color: #24292f;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    padding: 6px 12px;
}

QComboBox:hover {
    border: 1px solid #0969da;
}

QComboBox::drop-down {
    border: none;
    width: 20px;
}

QComboBox::down-arrow {
    image: none;
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-top: 6px solid #57606a;
    margin-right: 6px;
}

QComboBox QAbstractItemView {
    background-color: #ffffff;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    selection-background-color: #f6f8fa;
    outline: none;
}

/* ============================================
   Text Edit (Notes)
   ============================================ */

QTextEdit, QPlainTextEdit {
    background-color: #ffffff;
    color: #24292f;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    padding: 8px;
    selection-background-color: #b6e3ff;
}

QTextEdit:focus, QPlainTextEdit:focus {
    border: 1px solid #0969da;
}

/* ============================================
   Dialog
   ============================================ */

QDialog {
    background-color: #ffffff;
}

/* ============================================
   Tab Widget
   ============================================ */

QTabWidget::pane {
    background-color: #f6f8fa;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    top: -1px;
}

QTabBar::tab {
    background-color: #ffffff;
    color: #57606a;
    border: 1px solid #d0d7de;
    border-bottom: none;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    padding: 8px 16px;
    margin-right: 2px;
}

QTabBar::tab:selected {
    background-color: #f6f8fa;
    color: #0969da;
    border-bottom: 2px solid #0969da;
}

QTabBar::tab:hover:!selected {
    background-color: #f6f8fa;
}

/* ============================================
   Check Box & Radio Button
   ============================================ */

QCheckBox, QRadioButton {
    color: #24292f;
    spacing: 8px;
}

QCheckBox::indicator, QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border: 2px solid #d0d7de;
    border-radius: 4px;
    background-color: #ffffff;
}

QCheckBox::indicator:checked {
    background-color: #0969da;
    border: 2px solid #0969da;
}

QRadioButton::indicator {
    border-radius: 9px;
}

QRadioButton::indicator:checked {
    background-color: #0969da;
    border: 2px solid #0969da;
}

/* ============================================
   Tooltip
   ============================================ */

QToolTip {
    background-color: #24292f;
    color: #ffffff;
    border: 1px solid #57606a;
    border-radius: 4px;
    padding: 4px 8px;
}

/* ============================================
   Status Bar
   ============================================ */

QStatusBar {
    background-color: #f6f8fa;
    color: #57606a;
    border-top: 1px solid #d0d7de;
}

QStatusBar::item {
    border: none;
}

)";
}

} // namespace LightTheme

#endif // LIGHTTHEME_H
