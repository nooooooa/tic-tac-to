#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


char board[9];

// Win combinations
int win_combos[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8},  // rows
    {0,3,6}, {1,4,7}, {2,5,8},  // columns
    {0,4,8}, {2,4,6}             // diagonals
};

void init_board() {
    for (int i = 0; i < 9; i++)
        board[i] = '1' + i;
}

void print_board() {
    printf("\n");
    printf(" %c | %c | %c   (7|8|9)\n", board[6], board[7], board[8]);
    printf("---+---+---\n");
    printf(" %c | %c | %c   (4|5|6)\n", board[3], board[4], board[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c   (1|2|3)\n", board[0], board[1], board[2]);
    printf("\n");
}

// Returns 1 if player wins, -1 if AI wins, 0 if no winner yet, 2 if draw
int check_winner() {
    for (int i = 0; i < 8; i++) {
        int a = win_combos[i][0];
        int b = win_combos[i][1];
        int c = win_combos[i][2];
        if (board[a] == board[b] && board[b] == board[c]) {
            if (board[a] == 'X') return 1;
            if (board[a] == 'O') return -1;
        }
    }
    // Check draw
    int filled = 0;
    for (int i = 0; i < 9; i++)
        if (board[i] == 'X' || board[i] == 'O') filled++;
    if (filled == 9) return 2;
    return 0;
}

int is_empty(int pos) {
    return board[pos] != 'X' && board[pos] != 'O';
}

int easy_ai_move() {
    int empty[9], count = 0;
    for (int i = 0; i < 9; i++)
        if (is_empty(i)) empty[count++] = i;
    if (count == 0) return -1;
    return empty[rand() % count];
}

int minimax(int is_maximizing, int depth) {
    int result = check_winner();
    if (result == -1) return 10 - depth;  
    if (result == 1)  return depth - 10;  
    if (result == 2)  return 0;           

    if (is_maximizing) {
        int best = -1000;
        for (int i = 0; i < 9; i++) {
            if (is_empty(i)) {
                board[i] = 'O';
                int score = minimax(0, depth + 1);
                board[i] = '1' + i;
                if (score > best) best = score;
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 9; i++) {
            if (is_empty(i)) {
                board[i] = 'X';
                int score = minimax(1, depth + 1);
                board[i] = '1' + i;
                if (score < best) best = score;
            }
        }
        return best;
    }
}

int hard_ai_move() {
    int best_score = -1000;
    int best_move = -1;

    for (int i = 0; i < 9; i++) {
        if (is_empty(i)) {
            board[i] = 'O';
            int score = minimax(0, 0);
            board[i] = '1' + i;
            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}

void print_menu() {
    printf("+----------------------------------+\n");
    printf("¦         MAIN MENU                ¦\n");
    printf("¦----------------------------------¦\n");
    printf("¦  [1] Play vs Easy AI  (random)   ¦\n");
    printf("¦  [2] Play vs Hard AI  (smart)    ¦\n");
    printf("¦  [3] Quit                        ¦\n");
    printf("+----------------------------------+\n");
    printf("\n  Choose: ");
}

void play_game(int difficulty) {
    init_board();
    int result = 0;
    int move;

    if (difficulty == 1)
        printf("\n  [EASY MODE] You are X, AI is O. AI plays randomly.\n");
    else
        printf("\n  [HARD MODE] You are X, AI is O. Good luck!\n");

    printf("Board positions:\n");
    printf(" 7 | 8 | 9\n");
    printf("---+---+---\n");
    printf(" 4 | 5 | 6\n");
    printf("---+---+---\n");
    printf(" 1 | 2 | 3\n");

    while (1) {
        print_board();

        // Player turn
        printf("  Your move (1-9): ");
        if (scanf("%d", &move) != 1) {
            while (getchar() != '\n');
            printf("  Invalid input! Enter a number 1-9.\n");
            continue;
        }
        move--; // Convert to 0-indexed

        if (move < 0 || move > 8 || !is_empty(move)) {
            printf("  Invalid move! Try again.\n");
            continue;
        }

        board[move] = 'X';
        result = check_winner();
        if (result != 0) break;

        // AI turn
        printf("\n  AI is thinking...\n");
        int ai_move;
        if (difficulty == 1)
            ai_move = easy_ai_move();
        else
            ai_move = hard_ai_move();

        if (ai_move == -1) break;
        board[ai_move] = 'O';
        printf("  AI placed at position %d\n", ai_move + 1);

        result = check_winner();
        if (result != 0) break;
    }

    print_board();

    printf("\n================================\n");
    if (result == 1)
        printf("YOU WIN! Congratulations!\n");
    else if (result == -1)
        printf("AI WINS! Better luck next time.\n");
    else
        printf("IT'S A DRAW!\n");
    printf("================================\n\n");
}

int main() {
    srand((unsigned int)time(NULL));

    int choice;
    while (1) {
        print_menu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\n  Invalid input! Enter 1, 2, or 3.\n\n");
            continue;
        }

        if (choice == 1) {
            play_game(1);
        } else if (choice == 2) {
            play_game(2);
        } else if (choice == 3) {
            printf("\n  Thanks for playing! See you next time.\n\n");
            break;
        } else {
            printf("\n  Invalid choice! Enter 1, 2, or 3.\n\n");
        }
    }

    return 0;
}
