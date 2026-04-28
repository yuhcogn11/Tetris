// LogicDemo.cpp
// File demo riêng để test logic Tetris bằng menu console.
// File này chỉ dùng để kiểm tra Board.cpp và Tetromino.cpp, không dùng SDL, render hay âm thanh.

// Lưu ý: dòng dưới chỉ dùng trong file demo để truy cập grid của Board,
// giúp tạo dòng đầy kiểm tra clearLines() và gameOver(). Không dùng cách này trong code game chính.
#define private public
#include "Board.h"
#undef private

#include "Tetromino.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;

string typeToString(TetrominoType type) {
    switch (type) {
        case TetrominoType::I: return "I";
        case TetrominoType::O: return "O";
        case TetrominoType::T: return "T";
        case TetrominoType::S: return "S";
        case TetrominoType::Z: return "Z";
        case TetrominoType::J: return "J";
        case TetrominoType::L: return "L";
        case TetrominoType::NONE:
        default: return ".";
    }
}

char typeToChar(TetrominoType type) {
    switch (type) {
        case TetrominoType::I: return 'I';
        case TetrominoType::O: return 'O';
        case TetrominoType::T: return 'T';
        case TetrominoType::S: return 'S';
        case TetrominoType::Z: return 'Z';
        case TetrominoType::J: return 'J';
        case TetrominoType::L: return 'L';
        case TetrominoType::NONE:
        default: return '.';
    }
}

bool canPlacePiece(const Board& board, const Tetromino& piece, int testX, int testY) {
    for (int row = 0; row < TETROMINO_SIZE; ++row) {
        for (int col = 0; col < TETROMINO_SIZE; ++col) {
            if (!piece.isCellFilled(col, row)) {
                continue;
            }

            int boardCol = testX + col;
            int boardRow = testY + row;

            if (boardCol < 0 || boardCol >= BOARD_WIDTH || boardRow >= BOARD_HEIGHT) {
                return false;
            }

            if (boardRow >= 0 && !board.isCellEmpty(boardCol, boardRow)) {
                return false;
            }
        }
    }

    return true;
}

void printPieceMatrix(const Tetromino& piece) {
    cout << "\nMa tran 4x4 cua khoi " << typeToString(piece.getType())
         << " - rotation = " << piece.getRotation() << "\n";

    for (int row = 0; row < TETROMINO_SIZE; ++row) {
        for (int col = 0; col < TETROMINO_SIZE; ++col) {
            cout << (piece.isCellFilled(col, row) ? '#' : '.') << ' ';
        }
        cout << '\n';
    }
}

void printBoardWithPiece(const Board& board, const Tetromino& piece) {
    cout << "\nBOARD hien tai | Khoi dang roi: " << typeToString(piece.getType())
         << " | x = " << piece.x << ", y = " << piece.y
         << " | rotation = " << piece.getRotation() << "\n";

    cout << "+";
    for (int col = 0; col < BOARD_WIDTH; ++col) cout << "--";
    cout << "+\n";

    for (int row = 0; row < BOARD_HEIGHT; ++row) {
        cout << "|";
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            char cell = typeToChar(board.getCellType(col, row));

            // Ve de len vi tri cua khoi dang roi.
            for (int pr = 0; pr < TETROMINO_SIZE; ++pr) {
                for (int pc = 0; pc < TETROMINO_SIZE; ++pc) {
                    if (piece.isCellFilled(pc, pr) && piece.x + pc == col && piece.y + pr == row) {
                        cell = '#';
                    }
                }
            }

            cout << cell << ' ';
        }
        cout << "|\n";
    }

    cout << "+";
    for (int col = 0; col < BOARD_WIDTH; ++col) cout << "--";
    cout << "+\n";
}

void fillBottomLineForTest(Board& board) {
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        board.grid[BOARD_HEIGHT - 1][col] = TetrominoType::T;
    }
}

void fillTopLineForGameOverTest(Board& board) {
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        board.grid[0][col] = TetrominoType::Z;
    }
}

void printMenu() {
    cout << "\n========== MENU TEST LOGIC TETRIS ==========" << '\n';
    cout << "1. Hien thi board va thong tin khoi hien tai" << '\n';
    cout << "2. Tao khoi ngau nhien moi - test createRandom()" << '\n';
    cout << "3. In ma tran 4x4 cua khoi - test isCellFilled()" << '\n';
    cout << "4. Di chuyen khoi trai/phai/xuong - test moveLeft/moveRight/moveDown" << '\n';
    cout << "5. Xoay khoi - test rotateCW/rotateCCW" << '\n';
    cout << "6. Tinh ghost piece - test getGhostY()" << '\n';
    cout << "7. Khoa khoi, xoa dong, game over - test Board logic" << '\n';
    cout << "0. Thoat" << '\n';
    cout << "Chon chuc nang: ";
}

int main() {
    Board board;
    Tetromino current = Tetromino::createRandom();

    int choice = -1;

    while (choice != 0) {
        printMenu();
        cin >> choice;

        if (!cin) {
            cout << "Nhap sai. Ket thuc chuong trinh.\n";
            return 0;
        }

        switch (choice) {
            case 1: {
                printBoardWithPiece(board, current);
                break;
            }

            case 2: {
                current = Tetromino::createRandom();
                cout << "Da tao khoi moi: " << typeToString(current.getType()) << '\n';
                printBoardWithPiece(board, current);
                break;
            }

            case 3: {
                printPieceMatrix(current);
                break;
            }

            case 4: {
                cout << "Nhap huong di chuyen: a = trai, d = phai, s = xuong: ";
                char dir;
                cin >> dir;

                int newX = current.x;
                int newY = current.y;

                if (dir == 'a' || dir == 'A') --newX;
                else if (dir == 'd' || dir == 'D') ++newX;
                else if (dir == 's' || dir == 'S') ++newY;
                else {
                    cout << "Huong khong hop le.\n";
                    break;
                }

                if (canPlacePiece(board, current, newX, newY)) {
                    if (dir == 'a' || dir == 'A') current.moveLeft();
                    if (dir == 'd' || dir == 'D') current.moveRight();
                    if (dir == 's' || dir == 'S') current.moveDown();
                    cout << "Di chuyen thanh cong.\n";
                } else {
                    cout << "Khong the di chuyen vi cham bien hoac cham khoi khac.\n";
                }

                printBoardWithPiece(board, current);
                break;
            }

            case 5: {
                cout << "Nhap kieu xoay: r = xoay phai, l = xoay trai: ";
                char dir;
                cin >> dir;

                if (dir == 'r' || dir == 'R') {
                    current.rotateCW();
                    if (!canPlacePiece(board, current, current.x, current.y)) {
                        current.rotateCCW();
                        cout << "Khong xoay duoc, da tra ve trang thai cu.\n";
                    } else {
                        cout << "Xoay phai thanh cong.\n";
                    }
                } else if (dir == 'l' || dir == 'L') {
                    current.rotateCCW();
                    if (!canPlacePiece(board, current, current.x, current.y)) {
                        current.rotateCW();
                        cout << "Khong xoay duoc, da tra ve trang thai cu.\n";
                    } else {
                        cout << "Xoay trai thanh cong.\n";
                    }
                } else {
                    cout << "Lua chon xoay khong hop le.\n";
                }

                printPieceMatrix(current);
                printBoardWithPiece(board, current);
                break;
            }

            case 6: {
                int ghostY = current.getGhostY(board);
                cout << "GhostY cua khoi hien tai = " << ghostY << '\n';
                cout << "Nghia la neu roi thang xuong, khoi se dung o hang y = " << ghostY << ".\n";
                break;
            }

            case 7: {
                cout << "Chon test Board logic:\n";
                cout << "1. lockPiece(): khoa khoi hien tai vao board\n";
                cout << "2. clearLines(): tao dong day roi xoa\n";
                cout << "3. isGameOver(): lap hang tren cung de test game over\n";
                cout << "Lua chon: ";

                int subChoice;
                cin >> subChoice;

                if (subChoice == 1) {
                    board.lockPiece(current);
                    cout << "Da khoa khoi " << typeToString(current.getType()) << " vao board.\n";
                    int cleared = board.clearLines();
                    cout << "So dong vua xoa sau khi khoa khoi = " << cleared << '\n';
                    current = Tetromino::createRandom();
                    cout << "Da tao khoi moi sau khi lock.\n";
                } else if (subChoice == 2) {
                    fillBottomLineForTest(board);
                    cout << "Da tao 1 dong day o day board de test.\n";
                    printBoardWithPiece(board, current);
                    int cleared = board.clearLines();
                    cout << "clearLines() tra ve: " << cleared << '\n';
                } else if (subChoice == 3) {
                    fillTopLineForGameOverTest(board);
                    cout << "Da lap hang tren cung. isGameOver() = "
                         << (board.isGameOver() ? "true" : "false") << '\n';
                } else {
                    cout << "Lua chon khong hop le.\n";
                }

                printBoardWithPiece(board, current);
                break;
            }

            case 0: {
                cout << "Thoat demo logic.\n";
                break;
            }

            default: {
                cout << "Lua chon khong hop le. Hay nhap tu 0 den 7.\n";
                break;
            }
        }
    }

    return 0;
}
