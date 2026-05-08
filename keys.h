#pragma once

namespace ScanCodes {
    // https://users.utcluj.ro/~baruch/sie/labor/PS2/Scan_Codes_Set_1.htm
    enum : unsigned short {
        Escape = 0x01,
        Key_1 = 0x02,
        Key_2 = 0x03,
        Key_3 = 0x04,
        Key_4 = 0x05,
        Key_5 = 0x06,
        Key_6 = 0x07,
        Key_7 = 0x08,
        Key_8 = 0x09,
        Key_9 = 0x0A,
        Key_0 = 0x0B,
        Minus = 0x0C,
        Equals = 0x0D,
        Backspace = 0x0E,
        Tab = 0x0F,
        Q = 0x10, W = 0x11, E = 0x12, R = 0x13, T = 0x14, Y = 0x15, U = 0x16, I = 0x17, O = 0x18, P = 0x19,
        LeftBracket = 0x1A,
        RightBracket = 0x1B,
        Enter = 0x1C,
        LeftControl = 0x1D,
        A = 0x1E, S = 0x1F, D = 0x20, F = 0x21, G = 0x22, H = 0x23, J = 0x24, K = 0x25, L = 0x26,
        Semicolon = 0x27,
        Apostrophe = 0x28,
        Grave = 0x29,
        LeftShift = 0x2A,
        Backslash = 0x2B,
        Z = 0x2C, X = 0x2D, C = 0x2E, V = 0x2F, B = 0x30, N = 0x31, M = 0x32,
        Comma = 0x33,
        Period = 0x34,
        Slash = 0x35,
        RightShift = 0x36,
        Numpad_Multiply = 0x37,
        LeftAlt = 0x38,
        Space = 0x39,
        CapsLock = 0x3A,
        F1 = 0x3B, F2 = 0x3C, F3 = 0x3D, F4 = 0x3E, F5 = 0x3F,
        F6 = 0x40, F7 = 0x41, F8 = 0x42, F9 = 0x43, F10 = 0x44,
        NumLock = 0x45,
        ScrollLock = 0x46,
        F11 = 0x57, F12 = 0x58
    };

    static const char* GetKey(unsigned short code) {
        switch (code) {
        case Q: return "Q"; case W: return "W"; case E: return "E"; case R: return "R";
        case T: return "T"; case Y: return "Y"; case U: return "U"; case I: return "I";
        case O: return "O"; case P: return "P"; case A: return "A"; case S: return "S";
        case D: return "D"; case F: return "F"; case G: return "G"; case H: return "H";
        case J: return "J"; case K: return "K"; case L: return "L"; case Z: return "Z";
        case X: return "X"; case C: return "C"; case V: return "V"; case B: return "B";
        case N: return "N"; case M: return "M";
        case Space: return "SPACE"; case Enter: return "ENTER"; case Escape: return "ESC";
        case LeftShift: return "L_SHIFT"; case LeftControl: return "L_CTRL";
        case Key_1: return "1"; case Key_2: return "2"; case Key_3: return "3";
        default: return "UNKNOWN";
        }
    }
}